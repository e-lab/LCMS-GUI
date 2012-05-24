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
	continuousLength = 0;
	for (int i=0; i<30000;i++) {
		continuousSpectrum[i]=0;
		continuousTime[i]=0;
	}

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

	plot->Fit(-200, 3200, -0.3, 3.3, NULL, NULL);  //sets the view from -200 to 3200 ms and -.3 to 3.3 V 
	xScale->SetName (wxT ("Time (ms)"));
	yScale->SetName (wxT ("Voltage (V)"));

	mypen = new wxPen (wxT ("RED"), 1, wxSOLID);

	layer = new GraphicsPlotData();

	SetSizerAndFit (sizerPlot);
}

GraphicsPlot::~GraphicsPlot()
{
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


	//create a subset of the data to plot, based on the plot size on the screen in pixels
//	MinMaxDec(&ct[0], &cs[0], &cl);
	// Ownership of the 'time' and 'spectrum' arrays have been passed to
	// the GraphicsPlotData object, therefore, the arrays memory will be
	// freed by GraphicsPlotData.
//	layer->SetData (&time[0], &spectrum[0], length);
	layer->SetData (&ct[0], &cs[0], continuousLength);
//	layer->SetData (continuousTime, continuousSpectrum, continuousLength);
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

	int prevContinuousLength = continuousLength;
	//need array bound check here
	if ( (length + prevContinuousLength) > 30000) {
		//measurements_continuous.push_back(rawEvent.Clone()); //store this complete buffer in measurements_continuous
		std::vector<float> continuousTime_v_inner;
		std::vector<float> continuousSpectrum_v_inner;
		for (int i = 0; i < continuousLength; i++) {
			//if any processing is needed, do it before the push so that it is included when saving to a file
			int a = continuousTime_v.size();
			if (a > 0) {
				int b = continuousTime_v.at(a-1).size();
				float last_time = continuousTime_v.at(a-1).at(b-1);
				continuousTime_v_inner.push_back(continuousTime[i]+last_time);
			}
			else  {
				continuousTime_v_inner.push_back(continuousTime[i]);
			}
			continuousSpectrum_v_inner.push_back(continuousSpectrum[i]);
		}
		
		continuousTime_v.push_back(continuousTime_v_inner);
		continuousSpectrum_v.push_back(continuousSpectrum_v_inner);

		prevContinuousLength = 0;  //maybe something more clever later
		continuousLength = 0;
	}
	continuousLength = continuousLength + length;

	cs = new float[continuousLength];
	ct = new float[continuousLength];

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
		}


		//how about trying if raw_counter < 0?
		
		else 
			spectrum[i] = ( ( (float) raw_count) / (float) 65535) * 3.3f;
	//	if (mode == 2) //sw cds mode
	//	{
	//		if (spectrum[i] > 1.65)
	//			spectrum[i] = 3.3 - spectrum[i];
	//	}

			

		time[i] = (float) i * dt;  // Constant time

		continuousSpectrum[i+prevContinuousLength]=spectrum[i];
		continuousTime[i+prevContinuousLength] = time[i]+continuousTime[prevContinuousLength-1]+dt;
	}
	for (int i =0; i < continuousLength; i++) {
		cs[i] = continuousSpectrum[i];
		ct[i] = continuousTime[i];
	}

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

//this function performs a min-max decimation filter on the continuous data to get a subset of the data to display on the plot
void GraphicsPlot::MinMaxDec (float* ct, float* cs, int* cl)
{
	int screen_x_size = plot->GetScrX();	//the plot is x pixels wide
	*cl=2*screen_x_size;
	cs = new float[*cl];
	ct = new float[*cl];

	if (continuousLength < screen_x_size) {	//the amount of data points is less than pixels on the screen, so we can display every point
		// plotting everything
		for (int i =0; i < continuousLength; i++) {
			cs[i] = continuousSpectrum[i];
			ct[i] = continuousTime[i];
		}
		for (int i = continuousLength; i< *cl; i++) {
			cs[i]=0;
			ct[i]=0;
		}
		*cl=continuousLength;
	} else { //need to subsample to speed up rendering, using max-min decimation
		int chunk_size=continuousLength/screen_x_size;
		if (*cl % screen_x_size != 0)
			chunk_size++;	//round up to the nearest interval
		for (int i=0; i<*cl; i++) {
			//find max and min of each chunk
			int lower_lim=i*chunk_size;
			int upper_lim= (i*chunk_size) +chunk_size-1;
			float* vals = new float[1];
			float* data_to_process = &continuousTime[lower_lim];
			min_max_in_order (data_to_process, (upper_lim-lower_lim),vals);
			cs[2*i]=vals[0];
			cs[2*i+1]=vals[1];
			ct[2*i]=continuousTime[lower_lim];
			ct[2*i+1]=continuousTime[lower_lim];
			delete[] vals;
		}
	}
}

void GraphicsPlot::min_max_in_order (float* data,int data_length,float* result)
{
	float min = data[0];
	float max = data[0];
	int min_num = 0;
	int max_num = 0;
//	int result[1];
	for (int i=1; i<=data_length; i++) {
		if (data[i] > max) {
			max = data[i];
			max_num = i;
		}
		if (data[i] < min) {
			min = data[i];
			min_num = i;
		}
	}
	result[0]=max;
	result[1]=min;
	if (min_num < max_num) {
		result[0]=min;
		result[1]=max;
	}
//	return result;
}

void GraphicsPlot::Clear(void)
{
	int a = continuousTime_v.size();
	for (int i=0; i<a; i++) {
		continuousTime_v.at(i).pop_back();
		continuousSpectrum_v.at(i).pop_back();		
	}
	while (continuousTime_v.size() > 0) {
		continuousTime_v.pop_back();
		continuousSpectrum_v.pop_back();
	}

	while (measurements.size() > 0) {
		delete (measurements.back());
		measurements.pop_back();
	}

	continuousLength=0;
	continuousTime[continuousLength]=0;
}

void GraphicsPlot::SaveData (wxString outputFile)
{
	unsigned int ii = 0;
	unsigned int ms = measurements.size();
	unsigned int mcs = continuousTime_v.size()+1;
	int a = continuousLength;

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

	float last_time = 0;

	if(mcs > 0) 
	{
		///unpack the vector of vectors and print it to files, one file per outer vector
		int a = continuousTime_v.size();
		for (int i=0; i<a; i++) {
			//make a new file
			int b = continuousTime_v.at(i).size();
			for (int j=0; j<b; j++) {
				fileOut.AddLine(wxString::Format( wxT("%f \t %f"), continuousTime_v.at(i).at(j),continuousSpectrum_v.at(i).at(j)));   ///don't forget to add the inner time array to the outter last time
			//	fileOut.Write(&continuousTime_v.at(i), continuousTime_v.at(i).size());
			}
			continuousTime_v.at(i).pop_back();
			continuousSpectrum_v.at(i).pop_back();		
		}
		fileOut.Write();
		//fileOut.Close();

		if (a > 0) {
			int b = continuousTime_v.at(a-1).size();
			last_time = continuousTime_v.at(a-1).at(b-1);
		}
	}
		while (continuousTime_v.size() > 0) {
			continuousTime_v.pop_back();
			continuousSpectrum_v.pop_back();
		}

	if (measurements.size() == 0) {
		wxMessageBox( wxT("No measurements to record") );
		return;
	}
	
	for (int xx = 0; xx < continuousLength; xx++) {
		fileOut.AddLine(wxString::Format( wxT("%f \t %f"), continuousTime[xx]+last_time,continuousSpectrum[xx]));  //don't forget to add the time to the previous last time
	}

	while (measurements.size() > 0) {
		delete (measurements.back());
		measurements.pop_back();
	}

	continuousLength=0;
	continuousTime[continuousLength]=0;
	last_time=0;

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