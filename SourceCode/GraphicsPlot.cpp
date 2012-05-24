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
	spectrumBuffer = new SimpleCircularBuffer<float>(30000);
	timeBuffer = new SimpleCircularBuffer<float>(30000);
	lengthBuffer = 0;
	lastTime = 0;

	wxBoxSizer* sizerPlot = new wxBoxSizer (wxVERTICAL);

	plot = new mpWindow (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
	sizerPlot->Add (plot, 1, wxEXPAND | wxALIGN_CENTER | wxTOP, 10);

	information = new GraphicsInformation (this);
	sizerPlot->Add (information, 0, wxALIGN_CENTER);

	plot->EnableDoubleBuffer (true);

	plot->SetScaleX (1);
	plot->SetScaleY (1);
//	plot->SetPos (0.03, 1);


	xScale = new mpScaleX;
	yScale = new mpScaleY;

	plot->AddLayer (xScale);
	plot->AddLayer (yScale);

	plot->Fit(-200, 10000, -0.3, 3.3, NULL, NULL);  //sets the view from -200 to 3200 ms and -.3 to 3.3 V 
	xScale->SetName (wxT ("Time (ms)"));
	yScale->SetName (wxT ("Voltage (V)"));

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

	while (measurements.size() > 0) {
		delete (measurements.back());
		measurements.pop_back();
	}
}

void GraphicsPlot::OnDeviceEvent (DeviceEvent& rawEvent)
{
	// Clears previous data events from the continuous save 'measurements' vector.
	int saveType = rawEvent.GetVariable (Command::PA_SAVE_TYPE);

//	if ( (1 == saveType) // True when the "Save Type" radio button is set to "Continues".
	if ( ( 0 == saveType) //for now always do this code, since we do not have a "Save Type" radio button
	                || ( (measurements.size() > 0) && (saveType != (measurements.at (0))->GetVariable (Command::PA_SAVE_TYPE)))
	                // True if there is a raw data event saved in the measurements vector AND if the first raw data event in the measurements vector
	                // 	has a different PA_SAVE_TYPE to the current raw data event.  This will only be so for the first frame after the
	                //	"Save Type" radio button is changed, thus preventing data events with different save types being in the same measurements.
	                || (1 == rawEvent.GetVariable (Command::PA_FIRST_MEAS))) {
		// True if this is the first measurement after the 'start' button has been pressed.
		while (measurements.size() > 0) {
			delete (measurements.back());
			measurements.pop_back();
		}

		//information->ClearMeasurements();
	}

	int a, b;
	rawEvent.GetInAvailable(a);
	rawEvent.GetOutAvailable(b);	
	information->SetBufferInfo(a, b);

 	measurements.push_back (rawEvent.Clone());


	// Remove 'layer' from display if there.
	plot->DelLayer (layer);

	UnpackEvent (rawEvent);
	delete[] rawCount;
	rawCount = NULL;

	// Ownership of the 'time' and 'spectrum' arrays have been passed to
	// the GraphicsPlotData object, therefore, the arrays memory will be
	// freed by GraphicsPlotData.

	float* tmp1_spectrum = spectrumBuffer->GetBuffer();
	float* tmp1_time = timeBuffer->GetBuffer();

	float* tmp2_spectrum = new float[lengthBuffer];
	float* tmp2_time = new float[lengthBuffer];

	//memcpy(tmp2_spectrum, tmp1_spectrum, lengthBuffer);
	//memcpy(tmp2_time, tmp1_time, lengthBuffer);

	for (int xx = 0; xx < lengthBuffer; xx++) {
		tmp2_spectrum[xx] = tmp1_spectrum[xx];
		tmp2_time[xx] = tmp1_time[xx];
	}

	//plot->Fit(tmp2_time[0], tmp2_time[lengthBuffer-1], -0.3, 3.3, NULL, NULL);

	layer->SetData (&tmp2_time[0], &tmp2_spectrum[0], lengthBuffer);

	layer->SetPen (*mypen);
	layer->SetContinuity (true);

	plot->AddLayer (layer);
	//information->AddNumOfMeasurements (length);
}

void GraphicsPlot::UnpackEvent (DeviceEvent& rawEvent)
{
//	int voltMinus = rawEvent.GetVariable (Command::PA_VOLT_M);  //whose code is this?
//	int voltPlus = rawEvent.GetVariable (Command::PA_VOLT_P);
//	int voltRef = rawEvent.GetVariable (Command::PA_VOLT_REF);
//	int capFlag = rawEvent.GetVariable (Command::PA_CAPACITANCE);

//	double deltaVplus = ( (double) (voltPlus - voltRef)) * 3.3 / 255.0;
//	double deltaVminus = ( (double) (voltMinus - voltRef)) * 3.3 / 255.0;
	double dt = double (rawEvent.GetVariable (Command::LCMS_VOLTAGESAMPLINGRATE) /1000.0);
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
	rawCount = new int[length];
	spectrum = new float[length];
	time = new float[length];
	if (lengthBuffer < 30000) {
		lengthBuffer = lengthBuffer + length;

		if (lengthBuffer > 30000) {
			lengthBuffer = 30000;
		}
	}

	for (int i = 0; i < length; i++) {
		double raw_count = (double) ( (rawData[i * 2 + 1] & 0xFF) << 8) + (rawData[i * 2] & 0xFF);
		rawCount[i] = (int) raw_count;

		//if ( i < 10) {
		//	::wxLogMessage(wxT("Raw Count:  %i"), (int) raw_count);
		//	::wxLogMessage(wxT("Raw Data 1:  %i"), (int) (rawData[i * 4 + 2]));
		//	::wxLogMessage(wxT("Raw Data 2:  %i"), (int) ( (rawData[i * 4 + 3])));
		//	::wxLogMessage(wxT("Raw Data 3:  %i"), (int) ( (rawData[i * 4])));
		//	::wxLogMessage(wxT("Raw Data 4:  %i"), (int) ( (rawData[i * 4 + 1])));
		//}

		if (mode == 1) // hardware cds mode
		{
			float cds_time = (cdst2-cdst1)*1E-6;
			spectrum[i] = ( ( (float) raw_count) / (float) 65535) * 3.3f;
			spectrum[i] = spectrum[i] - 1.65;
			spectrum[i]=(C/cds_time)*(spectrum[i])/1E-12;
			//lets figure out the real value for 100E-6 while we adjust the cds sampling times
			//spectrum[i]=spectrum[i]*.711;
			//spectrum[i]=spectrum[i]-95;
			spectrum[i]=spectrum[i]*gain;
			//fix the axes --- do we reallly need this in the loop?
			spectrumBuffer->PushToBuffer(spectrum[i]);
		}


		//how about trying if raw_counter < 0?
		
		else {
			spectrum[i] = ( ( (float) raw_count) / (float) 65535) * 3.3f;
			spectrumBuffer->PushToBuffer(spectrum[i]);
		}

	//	if (mode == 2) //sw cds mode
	//	{
	//		if (spectrum[i] > 1.65)
	//			spectrum[i] = 3.3 - spectrum[i];
	//	}

			

		time[i] = lastTime + ((float) i * dt);  // Constant time
		timeBuffer->PushToBuffer(time[i]);
	}
	lastTime = time[length-1];

	delete[] rawData;
	delete[] spectrum;
	delete[] time;

	rawData = NULL;

	if(mode==1) {
		yScale->SetName (wxT ("Current (pA)"));  
//		plot->Fit(-200, 3200, -600, 600, NULL, NULL);  //sets the view from -200 to 3200 ms and -600 to 600 pA
	}
}

void GraphicsPlot::SetCommandString (Command::CommandID command, wxString string)
{
	switch (command) {
		case Command::SAVE_DATA:
			SaveData (string);
			break;
		case Command::CLEAR :
			Clear();
		default :
			break;
	}
}

void GraphicsPlot::Clear(void)
{
	while (measurements.size() > 0) {
		delete (measurements.back());
		measurements.pop_back();
	}
}

void GraphicsPlot::SaveData (wxString outputFile)
{
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

	wxTextFile fileOut;

	if (!fileOut.Create (fileName.GetFullPath())) {
		::wxMessageBox (wxT ("The file could not be created.\n"),
						wxT ("File Creation Failed"));
		return;
	}

	wxString timeStamp = wxT ("# ");

	timeStamp << wxNow();
	fileOut.AddLine (timeStamp);
	fileOut.AddLine(wxString::Format( wxT("time (ms)\tADC (V)")));

	if (measurements.size() == 0) {
		wxMessageBox( wxT("No measurements to record") );
		return;
	}
	
	while (measurements.size() > 0) {
		delete (measurements.back());
		measurements.pop_back();
	}

	fileOut.Write();
	fileOut.Close();	
}


/*void GraphicsPlot::SaveData_old (wxString outputFile)	
{
	unsigned int ii = 0;

	while (ii < measurements.size()) {
		wxFileName fileName = wxFileName (outputFile);

		if (!fileName.HasExt()) {
			fileName.SetExt (wxT ("dat"));
		}

		if (1 != (measurements.at (ii))->GetVariable (Command::PA_SAVE_TYPE)) {
			fileName.SetName (fileName.GetName() << wxT ("_") << ii);
		}

		if (fileName.FileExists()) {
			::wxMessageBox (wxT ("A file with the current save name already exists.\n\n"
			                     "File Name:  ") + fileName.GetFullName(),
			                wxT ("Save Aborted"));

			return;
		}

		wxTextFile fileOut;

		if (!fileOut.Create (fileName.GetFullPath())) {
			::wxMessageBox (wxT ("The file could not be created.\n"),
			                wxT ("File Creation Failed"));
			return;
		}


		wxString timeStamp = wxT ("# ");

		timeStamp << wxNow();
		fileOut.AddLine (timeStamp);

		UnpackEvent (*measurements.at (ii));
		delete[] spectrum;
		spectrum = NULL;
		delete[] time;
		time = NULL;

		int voltMinus = (*measurements.at (ii)).GetVariable (Command::PA_VOLT_M);
		int voltPlus = (*measurements.at (ii)).GetVariable (Command::PA_VOLT_P);
		int voltRef = (*measurements.at (ii)).GetVariable (Command::PA_VOLT_REF);
		int voltPUP = (*measurements.at (ii)).GetVariable (Command::PA_VOLT_PUP);
		int capFlag = (*measurements.at (ii)).GetVariable (Command::PA_CAPACITANCE);

		fileOut.AddLine (wxString::Format (wxT ("# Bias Volt Minus:\t%.2f"), ( ( (double) voltMinus) * 3.3 / 255.0)));
		fileOut.AddLine (wxString::Format (wxT ("# Bias Volt Plus :\t%.2f"), ( ( (double) voltPlus) * 3.3 / 255.0)));
		fileOut.AddLine (wxString::Format (wxT ("# Bias Volt Ref  :\t%.2f"), ( ( (double) voltRef) * 3.3 / 255.0)));
		fileOut.AddLine (wxString::Format (wxT ("# Bias Volt PUP  :\t%.2f"), ( ( (double) voltPUP) * 3.3 / 255.0)));

		if (1 == capFlag) {
			fileOut.AddLine (wxString::Format (wxT ("# Capacitance: High")));
		} else {
			fileOut.AddLine (wxString::Format (wxT ("# Capacitance: Low")));
		}

		//int missedCycles = (*measurements.at (ii)).GetVariable (Command::PA_MISSED_CYCLES);
		//fileOut.AddLine (wxString::Format (wxT ("# Missed Cycles  :\t%i\n"), missedCycles));

		for (int xx = 0; xx < length; xx++) {
			fileOut.AddLine (wxString::Format (wxT ("%i\t\t%i"), xx, rawCount[xx]));
		}

		delete[] rawCount;

		rawCount = NULL;

		fileOut.Write();
		fileOut.Close();
		ii++;
	}
}
*/
