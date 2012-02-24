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


GraphicsPlot::GraphicsPlot (wxWindow* owner) : wxPanel (owner)
{
	wxBoxSizer* sizerPlot = new wxBoxSizer (wxVERTICAL);

	plot = new mpWindow (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
	sizerPlot->Add (plot, 1, wxEXPAND | wxALIGN_CENTER | wxTOP, 10);

	information = new GraphicsInformation (this);
	sizerPlot->Add (information, 0, wxALIGN_CENTER);

	plot->EnableDoubleBuffer (true);

	plot->SetScaleX (5000);
	plot->SetScaleY (100);
	plot->SetPos (0.03, 1);

	xScale = new mpScaleX;
	yScale = new mpScaleY;

	plot->AddLayer (xScale);
	plot->AddLayer (yScale);

	xScale->SetName (wxT ("Time"));
	yScale->SetName (wxT ("Current (nA)"));

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
	// Clears previous data events from the continues save 'measurements' vector.
	int saveType = rawEvent.GetVariable (Command::PA_SAVE_TYPE);

	if ( (1 == saveType) // True when the "Save Type" radio button is set to "Continues".
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

	measurements.push_back (rawEvent.Clone());

	// Remove 'layer' from display if there.
	plot->DelLayer (layer);

	UnpackEvent (rawEvent);
	delete[] rawCount;
	rawCount = NULL;

	// Ownership of the 'time' and 'spectrum' arrays have been passed to
	// the GraphicsPlotData object, therefore, the arrays memory will be
	// freed by GraphicsPlotData.
	layer->SetData (&time[0], &spectrum[0], length);
	layer->SetPen (*mypen);
	layer->SetContinuity (true);

	plot->AddLayer (layer);
	//information->AddNumOfMeasurements (length);
}

void GraphicsPlot::UnpackEvent (DeviceEvent& rawEvent)
{
	int voltMinus = rawEvent.GetVariable (Command::PA_VOLT_M);
	int voltPlus = rawEvent.GetVariable (Command::PA_VOLT_P);
	int voltRef = rawEvent.GetVariable (Command::PA_VOLT_REF);
	int capFlag = rawEvent.GetVariable (Command::PA_CAPACITANCE);

	double deltaVplus = ( (double) (voltPlus - voltRef)) * 3.3 / 255.0;
	double deltaVminus = ( (double) (voltMinus - voltRef)) * 3.3 / 255.0;

	int rawLength; // Length of the rawData array.
	unsigned char* rawData = rawEvent.GetRawData (rawLength);

	length = (rawLength / 4);
	rawCount = new int[length];
	spectrum = new float[length];
	time = new float[length];

	for (int i = 0; i < length; i++) {

		double raw_count = (double) ( ( (rawData[i * 4 + 1] & 0x7F) << 24) + ( (rawData[i * 4] & 0xFF)  << 16) + ( (rawData[i * 4 + 3] & 0xFF) << 8) + (rawData[i * 4 + 2] & 0xFF));
		rawCount[i] = (int) raw_count;

		//if ( i < 10) {
		//	::wxLogMessage(wxT("Raw Count:  %i"), (int) raw_count);
		//	::wxLogMessage(wxT("Raw Data 1:  %i"), (int) (rawData[i * 4 + 2]));
		//	::wxLogMessage(wxT("Raw Data 2:  %i"), (int) ( (rawData[i * 4 + 3])));
		//	::wxLogMessage(wxT("Raw Data 3:  %i"), (int) ( (rawData[i * 4])));
		//	::wxLogMessage(wxT("Raw Data 4:  %i"), (int) ( (rawData[i * 4 + 1])));
		//}

		double delta = deltaVplus;

		if (1 == ( (rawData[i * 4 + 1] & 0x80) >> 7)) {
			delta = deltaVminus;
		}

		spectrum[i] = delta * ( ( (40 * pow (10.0, 6)) / raw_count) * pow (10.0, -12)); // Capacitance High

		if (0 == capFlag) {
			spectrum[i] = delta * ( ( (40 * pow (10.0, 6)) / raw_count) * pow (10.0, -13)); // Capacitance Low
		}

		time[i] = i;
	}

	delete[] rawData;

	rawData = NULL;
}

void GraphicsPlot::SetCommandString (Command::CommandID command, wxString string)
{
	switch (command) {
		case Command::SAVE_DATA:
			SaveData (string);
			break;
		default :
			break;
	}
}

void GraphicsPlot::SaveData (wxString outputFile)
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
