/**
 * @file
 * The Ploting System.
 *
 * This file contains class that will display both the plot and the info
 * panels.  This class is also responsible for taking the raw data from the
 * DeviceInterface object and converting it into useful information need for
 * display.
 */
#include "GraphicsPlot.h"
#include "DeviceEvent.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#define new DEBUG_NEW
#else
#define DEBUG_NEW new
#endif

GraphicsPlot::GraphicsPlot (wxWindow* owner) : wxPanel (owner)
{

	saveToFile = new GraphicsSaveData();


	max_size_buffer = 100000;
	max_view_millisec = 5000;  //how many miliseconds to display
	max_view_size = 50000;  //how many points to display
	white_space_pct = 5; //5 percent white space to make the scrolling look nice

	spectrumBuffer = new SimpleCircularBuffer<float>(max_size_buffer);
	timeBuffer = new SimpleCircularBuffer<float>(max_size_buffer);
	lengthBuffer = 0;
	lengthDisplay = 0;
	lastTime = 0;

	wxBoxSizer* sizerPlot = new wxBoxSizer (wxVERTICAL);

	plot = new mpWindow (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
	sizerPlot->Add (plot, 1, wxEXPAND | wxALIGN_CENTER | wxTOP, 10);

	plot->SetMPScrollbars(true);

	information = new GraphicsInformation (this);
	sizerPlot->Add (information, 0, wxALIGN_CENTER);

	plot->EnableDoubleBuffer (true);

	plot->SetScaleX (1);
	plot->SetScaleY (1);

	xScale = new mpScaleX(wxT("Time (ms)"), mpALIGN_BORDER_BOTTOM, true);
	yScale = new mpScaleY(wxT("Voltage (V)"), mpALIGN_BORDER_LEFT, true);

	plot->AddLayer (xScale);
	plot->AddLayer (yScale);

	plot->Fit(0, max_view_millisec+(white_space_pct/100.0*max_view_millisec), -0.3, 3.3, NULL, NULL);  //sets the default view

	mypen = new wxPen (wxT ("RED"), 1, wxSOLID);

	layer = new GraphicsPlotData();

	SetSizerAndFit (sizerPlot);
}

GraphicsPlot::~GraphicsPlot()
{
	delete spectrumBuffer;
	spectrumBuffer = NULL;

	delete timeBuffer;
	timeBuffer = NULL;

	delete xScale;
	xScale = (mpScaleX*) NULL;

	delete yScale;
	yScale = (mpScaleY*) NULL;

	delete mypen;
	mypen = (wxPen*) NULL;

	delete layer;
	layer = (GraphicsPlotData*) NULL;

	delete plot;
	plot = (mpWindow*) NULL;

	delete information;
	information = (GraphicsInformation*) NULL;

	Clear();
}

void GraphicsPlot::OnDeviceEvent (DeviceEvent& rawEvent)
{
	int a, b;
	rawEvent.GetInAvailable(a);
	rawEvent.GetOutAvailable(b);	
	information->SetBufferInfo(a, b);

	// Remove 'layer' from display if there.
	plot->DelLayer (layer);

	int last_data = rawEvent.GetVariable(Command::DEV_STOP);
	if (0 == last_data)
		UnpackEvent (rawEvent);		//only do the unpack if this was not a dummy stop event


	// Ownership of the 'time' and 'spectrum' arrays have been passed to
	// the GraphicsPlotData object, therefore, the arrays memory will be
	// freed by GraphicsPlotData.

	float* tmp1_spectrum = spectrumBuffer->GetBuffer();
	float* tmp1_time = timeBuffer->GetBuffer();

	float* tmp2_spectrum;
	float* tmp2_time;

	if (0 == last_data) {	//more data coming, plot what we have for scrolling
		tmp2_spectrum = new float[lengthDisplay];
		tmp2_time = new float[lengthDisplay];
		memcpy(tmp2_spectrum, &tmp1_spectrum[lengthBuffer-lengthDisplay], lengthDisplay*sizeof(float));
		memcpy(tmp2_time, &tmp1_time[lengthBuffer-lengthDisplay], lengthDisplay*sizeof(float));
		if (lengthDisplay >= max_view_size) {  //time to scroll the window!
			plot->Fit(	tmp2_time[0], \
					tmp2_time[lengthDisplay-1]+(white_space_pct/100.0)*max_view_millisec, \
					plot->GetDesiredYmin(), \
					plot->GetDesiredYmax(), NULL, NULL);
		}
		layer->SetData (&tmp2_time[0], &tmp2_spectrum[0], lengthDisplay);  //plot the data
	} else {
		tmp2_spectrum = new float[lengthBuffer];	//we finished getting data so allow the user to scroll back
		tmp2_time = new float[lengthBuffer];
		memcpy(tmp2_spectrum, &tmp1_spectrum[0], lengthBuffer*sizeof(float));
		memcpy(tmp2_time, &tmp1_time[0], lengthBuffer*sizeof(float));
		if (lengthBuffer > lengthDisplay) {
			plot->Fit(	tmp2_time[lengthBuffer-lengthDisplay-1], \
						tmp2_time[lengthBuffer-1]+(white_space_pct/100.0)*max_view_millisec, \
						plot->GetDesiredYmin(), \
						plot->GetDesiredYmax(), NULL, NULL);
		}
		layer->SetData (&tmp2_time[0], &tmp2_spectrum[0], lengthBuffer);  //plot the data
		if (lengthBuffer > 0 )
			plot->SetMPScrollbars(true); //shows the scrollbar when stopped if there is data available to scroll through
	}

	layer->SetPen (*mypen);
	layer->SetContinuity (true);

	plot->AddLayer (layer);
}

void GraphicsPlot::UnpackEvent (DeviceEvent& rawEvent)
{
	float dt = float (rawEvent.GetVariable (Command::LCMS_VOLTAGESAMPLINGRATE) /1000.0);
	max_view_size = (max_view_millisec/dt);
	int capSel = rawEvent.GetVariable (Command::LCMS_CAPSELECT);
	int cdst1 =  rawEvent.GetVariable(Command::LCMS_CDS_TIME1);
	int cdst2 =  rawEvent.GetVariable(Command::LCMS_CDS_TIME2);
	double gain = double (rawEvent.GetVariable(Command::LCMS_GAIN)/100.0);

	double C;
	if (0 == capSel) // 1pF
		C=1.0E-12f;
	else if (1== capSel)	//100 fF
		C=0.1E-12f;
	else C = 0; //no C!

	int mode = rawEvent.GetVariable(Command::LCMS_MODE);

	int rawLength; // Length of the rawData array.
	unsigned char* rawData = rawEvent.GetRawData (rawLength);
	
	length = (rawLength / 2);
	spectrum = new float[length];
	time = new float[length];
	if (lengthBuffer < max_size_buffer) {
		lengthBuffer = lengthBuffer + length;

		if (lengthBuffer > max_size_buffer) {
			lengthBuffer = max_size_buffer;
		}
	}

	if (lengthBuffer < max_view_size) {
		lengthDisplay = lengthBuffer;
	} else {
		lengthDisplay = max_view_size;
	}


	for (int i = 0; i < length; i++) {
		double raw_count = (double) ( (rawData[i * 2 + 1] & 0xFF) << 8) + (rawData[i * 2] & 0xFF);

		if (mode == 1) // hardware cds mode
		{
			float cds_time = (cdst2-cdst1)*1E-6;
			spectrum[i] = ( ( (float) raw_count) / (float) 65535) * 3.3f;
			spectrum[i] = spectrum[i] - 1.65;
			spectrum[i]= (C/cds_time)*(spectrum[i])/1E-12;
			spectrum[i]= spectrum[i]*gain;
			spectrumBuffer->PushToBuffer(spectrum[i]);  //add the results to the circular spectrum buffer used for plotting
		}

		else {
			spectrum[i] = ( ( (float) raw_count) / (float) 65535) * 3.3f;
			spectrumBuffer->PushToBuffer(spectrum[i]);
		}

	//	if (mode == 2) //sw cds mode not yet implemented
	//	{
	//		if (spectrum[i] > 1.65)
	//			spectrum[i] = 3.3 - spectrum[i];
	//	}
		time[i] = lastTime + ((float) i * dt);  // Constant time, keep track of time from last measurement
		timeBuffer->PushToBuffer(time[i]); //add the results to the circular time buffer used for plotting
	}

	lastTime = time[length-1]; //keep track of time from last measurement

	struct save_data *data = new struct save_data;
	data->spectrum	= &spectrum[0];
	data->time	= &time[0];
	data->length 	= length;
	data->tile_time	= wxT("Time (ms)");

	if(mode==1) {
		yScale->SetName (wxT ("Current (pA)"));
		data->tile_spectrum = wxT ("Current (pA)");
	}
	if(mode==0) {
		yScale->SetName (wxT ("Voltage (V)"));
		data->tile_spectrum = wxT ("Voltage (V)");
	}

	save_data_store.push_back (data);

	delete[] rawData;
	rawData = NULL;
}

void GraphicsPlot::SetCommandString (Command::CommandID command, wxString string)
{
	switch (command) {
		case Command::SAVE_DATA:
			SaveData (string);
			break;
		case Command::CLEAR :
			Clear();
			ResetView();
			break;
		default :
			break;
	}
}

void GraphicsPlot::ResetView(void)
{
	lastTime=0.0;
	lengthDisplay=0;
	lengthBuffer=0;
	spectrumBuffer->ResetBuffer();
	timeBuffer->ResetBuffer();
	plot->DelLayer (layer);
	plot->Fit(0, max_view_millisec+(white_space_pct/100.0*max_view_millisec), -0.3, 3.3, NULL, NULL);  //sets the default view
	plot->SetMPScrollbars(false);  //removes the scrollbar when there is no data to display
}


void GraphicsPlot::Clear(void)
{
	while (save_data_store.size() > 0) {
		struct save_data *data = save_data_store.back();
		delete[] data->spectrum;
		delete[] data->time;

		save_data_store.pop_back();
	}
}

void GraphicsPlot::SaveData (wxString outputFile)
{
	if (0 == save_data_store.size()) {
		wxMessageBox( wxT("No measurements to record") );
		return;
	}

	wxFileName fileName = wxFileName (outputFile);

	if (!fileName.HasExt()) {
		fileName.SetExt (wxT (".txt"));
	}
	
	if (fileName.FileExists()) {
		::wxMessageBox (wxT ("A file with the current save name already exists.\n\n"
							 "File Name:  ") + fileName.GetFullName(),
						wxT ("Save Aborted"));

		return;
	}

	// wxTextFile should not be used for files > 1 Meg
	wxTextFile fileOut;

	if (!fileOut.Create (fileName.GetFullPath())) {
		::wxMessageBox (wxT ("The file could not be created.\n"),
						wxT ("File Creation Failed"));
		return;
	}

	wxString timeStamp = wxT ("# ");
	timeStamp << wxNow();
	fileOut.AddLine (timeStamp);
	
	struct save_data *first_save_data = save_data_store.at(0);
	wxString tileLine = wxT ("# ");
	tileLine << first_save_data->tile_time << wxT ("\t") << first_save_data->tile_spectrum;
	fileOut.AddLine (tileLine);

	for (unsigned int ii = 0; ii < save_data_store.size(); ii++) {
		struct save_data *data = save_data_store.at(ii);

		for (int xx = 0; xx < data->length; xx++) {
			wxString dataLine= wxT ("");
			dataLine << data->time[xx] << wxT ("\t") << data->spectrum[xx];
			fileOut.AddLine (dataLine);
		}
	}

	fileOut.Write();
	fileOut.Close();	
	Clear();
	ResetView();
}
