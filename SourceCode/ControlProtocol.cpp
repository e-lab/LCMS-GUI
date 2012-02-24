/**
 * @file
 * Contains the class that constructs the protocol controls.
 */
#include "ControlProtocol.h"
#include "Command.h"
#include "DeviceController.h"

// When debugging changes all calls to "new" to be calls to "DEBUG_NEW" allowing for memory leaks to
// give you the file name and line number where it occurred.
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#define new DEBUG_NEW
#else
#define DEBUG_NEW new
#endif

ControlProtocol::ControlProtocol (wxWindow *owner, DeviceController *controller) : ControlPanel (owner)
{
	xemDeviceCtrl = controller;

	wxSizer* sizerProtocol = new wxBoxSizer (wxVERTICAL);
	wxStaticBoxSizer* sizerBox = new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Protocol (NOTE TIMING DEPENDS ON VOLTAGE SAMPLING RATE!"));
	wxFlexGridSizer* sizerGrid = new wxFlexGridSizer (5, 3, 5, 5);

	wxStaticBoxSizer* sizerBox2 = new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Input"));
	wxFlexGridSizer* sizerGrid2 = new wxFlexGridSizer (5, 3, 5, 5);

	sizerGrid->Add (new wxStaticText (this, -1, wxT ("Peak 1 Height:")), 1, wxALIGN_LEFT);
	peak1Height = new wxSpinCtrl (this, PEAK_1_HEIGHT, wxEmptyString, wxDefaultPosition, wxSize (80, wxDefaultCoord));
	sizerGrid->Add (peak1Height);
	sizerGrid->Add (new wxStaticText (this, -1, wxT ("mV")), 1, wxALIGN_LEFT);

	sizerGrid->Add (new wxStaticText (this, -1, wxT ("Peak 2 Height:")), 1, wxALIGN_LEFT);
	peak2Height = new wxSpinCtrl (this, PEAK_2_HEIGHT, wxEmptyString, wxDefaultPosition, wxSize (80, wxDefaultCoord));
	sizerGrid->Add (peak2Height);
	sizerGrid->Add (new wxStaticText (this, -1, wxT ("mV")), 1, wxALIGN_LEFT);

	sizerGrid->Add (new wxStaticText (this, -1, wxT ("Lead time:")), 1, wxALIGN_LEFT);
	leadTime = new wxSpinCtrl (this, LEAD_TIME, wxEmptyString, wxDefaultPosition, wxSize (80, wxDefaultCoord));
	sizerGrid->Add (leadTime);
	sizerGrid->Add (new wxStaticText (this, -1, wxT ("ms")), 1, wxALIGN_LEFT);

	sizerGrid->Add (new wxStaticText (this, -1, wxT ("Peak 1 Duration:")), 1, wxALIGN_LEFT);
	peak1Duration = new wxSpinCtrl (this, PEAK_1_DUR, wxEmptyString, wxDefaultPosition, wxSize (80, wxDefaultCoord));
	sizerGrid->Add (peak1Duration);
	sizerGrid->Add (new wxStaticText (this, -1, wxT ("ms")), 1, wxALIGN_LEFT);

	sizerGrid->Add (new wxStaticText (this, -1, wxT ("Peak 2 Duration:")), 1, wxALIGN_LEFT);
	peak2Duration = new wxSpinCtrl (this, PEAK_2_DUR, wxEmptyString, wxDefaultPosition, wxSize (80, wxDefaultCoord));
	sizerGrid->Add (peak2Duration);
	sizerGrid->Add (new wxStaticText (this, -1, wxT ("ms")), 1, wxALIGN_LEFT);

	sizerGrid->Add (new wxStaticText (this, -1, wxT ("Interval:")), 1, wxALIGN_LEFT);
	interval = new wxSpinCtrl (this, INTERVAL, wxEmptyString, wxDefaultPosition, wxSize (80, wxDefaultCoord));
	sizerGrid->Add (interval);
	sizerGrid->Add (new wxStaticText (this, -1, wxT ("ms")), 1, wxALIGN_LEFT);

	sizerBox->Add (sizerGrid, 1, wxALIGN_CENTER | wxALL, 10);

	configList = new wxChoice (this, PROTOCOL_CONFIG, wxDefaultPosition, wxSize (120, wxDefaultCoord), 0, wxCB_SORT);
	wxBoxSizer* row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, -1, wxT ("Predefined Configurations:  ")), 1, wxALIGN_LEFT);
	row->Add (configList);

	sizerBox2->Add (row, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, 10);
	sizerBox2->Add (sizerGrid2, 1, wxALIGN_CENTER | wxALL, 10);

	fileList = new wxArrayString();
	LoadConfigFiles (wxT ("C:\\LCMS2010\\LCMS2010_Software\\working_copy\\Application\\config"));


	sizerProtocol->Add (sizerBox, 0, wxALL | wxALIGN_CENTER, 10);
	sizerProtocol->Add (sizerBox2, 0, wxALL | wxALIGN_CENTER, 10);
	this->SetSizerAndFit (sizerProtocol);

	ResetProtocolValues();

	Connect (wxID_ANY, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler (ControlProtocol::ChangeProtocolValue));
	Connect (PROTOCOL_CONFIG, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler (ControlProtocol::OnConfigList));
}

ControlProtocol::~ControlProtocol()
{
	delete fileList;
	fileList = (wxArrayString*) NULL;
}

void ControlProtocol::ChangeProtocolValue (wxSpinEvent& evt)
{
	switch (evt.GetId()) {
		case PEAK_1_HEIGHT :
			SetPeak1Height (peak1Height->GetValue());
			xemDeviceCtrl->SetCommand (Command::LCMS_SELECTPROTOCOL, 0);
			configList->SetSelection (0);
			break;
		case PEAK_2_HEIGHT :
			SetPeak2Height (peak2Height->GetValue());
			xemDeviceCtrl->SetCommand (Command::LCMS_SELECTPROTOCOL, 0);
			configList->SetSelection (0);
			break;
		case LEAD_TIME :
			SetLeadTime (leadTime->GetValue());
			xemDeviceCtrl->SetCommand (Command::LCMS_SELECTPROTOCOL, 0);
			configList->SetSelection (0);
			break;
		case PEAK_1_DUR :
			SetPeak1Duration (peak1Duration->GetValue());
			xemDeviceCtrl->SetCommand (Command::LCMS_SELECTPROTOCOL, 0);
			configList->SetSelection (0);
			break;
		case PEAK_2_DUR :
			SetPeak2Duration (peak2Duration->GetValue());
			xemDeviceCtrl->SetCommand (Command::LCMS_SELECTPROTOCOL, 0);
			configList->SetSelection (0);
			break;
		case INTERVAL :
			SetInterval (interval->GetValue());
			xemDeviceCtrl->SetCommand (Command::LCMS_SELECTPROTOCOL, 0);
			configList->SetSelection (0);
			break;
	}
}



int ControlProtocol::GetSpinControlVal (int sel)
{
	wxSpinCtrl* sc;

	switch (sel) {
		case 1:
			sc = peak1Height;
			break;
		case 2:
			sc = peak2Height;
			break;
		case 3:
			sc = leadTime;
			break;
		case 4:
			sc = peak1Duration;
			break;
		case 5:
			sc = peak2Duration;
			break;
		case 6:
			sc = interval;
			break;
	}

	return sc->GetValue();
}

void ControlProtocol::SetPeak1Height (int value)
{
	peak1Height->SetRange (-500, 500);

	peak1Height->SetValue (value);
	xemDeviceCtrl->SetCommand (Command::PC5_PEAK_1_HEIGHT, (value*65535/3300));
}

void ControlProtocol::SetPeak2Height (int value)
{
	peak2Height->SetRange (-500, 500);

	peak2Height->SetValue (value);
	xemDeviceCtrl->SetCommand (Command::PC5_PEAK_2_HEIGHT, (value*65535/3300));
}

void ControlProtocol::SetLeadTime (int value)
{

	leadTime->SetValue (value);
	SetIntervalRange();

	xemDeviceCtrl->SetCommand (Command::PC5_LEAD_TIME, leadTime->GetValue ());
}

void ControlProtocol::SetPeak1Duration (int value)
{
	peak1Duration->SetValue (value);
	SetIntervalRange();

	xemDeviceCtrl->SetCommand (Command::PC5_PEAK_1_DUR, peak1Duration->GetValue ());
}

void ControlProtocol::SetPeak2Duration (int value)
{
	peak2Duration->SetValue (value);
	SetIntervalRange();

	xemDeviceCtrl->SetCommand (Command::PC5_PEAK_2_DUR, peak2Duration->GetValue ());
}

void ControlProtocol::SetInterval (int value)
{
	interval->SetValue (value);
	xemDeviceCtrl->SetCommand (Command::PC5_INTERVAL, value);
}

void ControlProtocol::SetIntervalRange ()
{
	//int buffer_size = 8191; // This value must be known for the device.  It's arbitrary,should work for any value.
	//this allows us to take measurements for a longer time.
	int buffer_size = 5000000; // This value must be known for the device.  It's arbitrary,should work for any value.

	leadTime->SetRange (0, (buffer_size) - peak1Duration->GetValue() - peak2Duration->GetValue() - leadTime->GetValue());
	leadTime->SetValue (leadTime->GetValue());

	peak1Duration->SetRange (0, (buffer_size) - peak2Duration->GetValue() - (2 * leadTime->GetValue()));
	peak1Duration->SetValue (peak1Duration->GetValue());

	peak2Duration->SetRange (0, (buffer_size) - peak1Duration->GetValue() - (2 * leadTime->GetValue()));
	peak2Duration->SetValue (peak2Duration->GetValue());

	// interval range min cannot be lower then leadTime, peak1Duration and peak2Duration combined
	interval->SetRange ( (2 * leadTime->GetValue()) + peak1Duration->GetValue() + peak2Duration->GetValue(), (buffer_size));
	SetInterval (interval->GetValue());

}

void ControlProtocol::ResetControls (wxCommandEvent& evt)
{
	ResetProtocolValues();
}

void ControlProtocol::ResetProtocolValues()
{
	//SetSamplingIncrement (20);

	SetPeak1Height (200);
	SetPeak2Height (-200);
	SetLeadTime (10);
	SetPeak1Duration (10);
	SetPeak2Duration (10);
	SetInterval (50);

	//SetPeak1Height (200);
	//SetPeak2Height (0);
	//SetLeadTime (0);
	//SetPeak1Duration (50);
	//SetPeak2Duration (0);
	//SetInterval (100);

	configList->SetStringSelection (wxT ("<USE PROTOCOL>"));
}

void ControlProtocol::LoadConfigFiles (wxString path)
{
	configList->Clear();
	configList->Append (wxT ("<USE PROTOCOL>"));

	if (wxDir::Exists (path)) {

		fileList->Clear();
		wxDir::GetAllFiles (path, fileList, wxT ("*.cfg"), wxDIR_FILES);

		for (unsigned int xx = 0; xx < fileList->GetCount(); xx++) {
			wxString file = fileList->Item (xx);
			file.Remove (0, (path.size() + 1));
			file.RemoveLast();
			file.RemoveLast();
			file.RemoveLast();
			file.RemoveLast();
			configList->Append (file);
		}
	}
}

void ControlProtocol::OnConfigList (wxCommandEvent& evt)
{
	int value = configList->GetSelection();
	xemDeviceCtrl->SetCommand (Command::LCMS_SELECTPROTOCOL, value);
	return;
}

void ControlProtocol::SaveCurrentConfig (wxString filename)
{
	wxFileConfig* config = new wxFileConfig (filename);

	config->Write (wxT ("PC5_PEAK_1_HEIGHT"),	peak1Height->GetValue());
	config->Write (wxT ("PC5_PEAK_2_HEIGHT"),	peak2Height->GetValue());
	config->Write (wxT ("PC5_LEAD_TIME"),		leadTime->GetValue());
	config->Write (wxT ("PC5_PEAK_1_DUR"),		peak1Duration->GetValue());
	config->Write (wxT ("PC5_PEAK_2_DUR"),		peak2Duration->GetValue());
	config->Write (wxT ("PC5_INTERVAL"),		interval->GetValue());

	delete config;
	config = (wxFileConfig*) NULL;
}

void ControlProtocol::Start()
{
	SetPeak1Height (peak1Height->GetValue());
	SetPeak2Height (peak2Height->GetValue());
	SetLeadTime (leadTime->GetValue());
	SetPeak1Duration (peak1Duration->GetValue());
	SetPeak2Duration (peak2Duration->GetValue());
	SetInterval (interval->GetValue());
}
