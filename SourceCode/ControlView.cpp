/**
 * @file
 * Contains the class that constructs the 'View' controls.
 */
#include "ControlView.h"
#include "Command.h"
#include "DeviceController.h"

ControlView::ControlView (wxWindow* owner, DeviceController* controller) : ControlPanel (owner)
{
	xemDeviceCtrl = controller;

	maxValueEvents =	4095; // 13 bit max devided by 2
	defaultValueEvents =	4095;

	maxValueTimeOut =	65535;
	defaultValueTimeOut =	65535;

	wxSizer* sizerView = new wxBoxSizer (wxVERTICAL);

	// Saving frame or movie.
	wxStaticBoxSizer* sizerBox= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Saving Images"));

	// Create save type radio buttons.
	saveFrame = new wxRadioButton (this, SAVE_FRAME, wxT ("Save Frame"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	saveMovie = new wxRadioButton (this, SAVE_MOVIE, wxT ("Save Movie"), wxDefaultPosition, wxDefaultSize);
	wxBoxSizer* row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (saveFrame, 0, wxLEFT | wxRIGHT, 40);
	row->Add (saveMovie, 0, wxLEFT | wxRIGHT, 40);
	sizerBox->Add (row, 0, wxTOP | wxBOTTOM  | wxALIGN_CENTER, 10);

	// Create image type reset button.
	wxButton* resetSaveType =  new wxButton (this, RESET_SAVE_TYPE, wxT ("Reset"), wxDefaultPosition, wxDefaultSize, 0);
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (resetSaveType, 0, wxALIGN_CENTER);
	sizerBox->Add (row, 0, wxTOP | wxBOTTOM | wxALIGN_CENTER, 10);

	sizerView->Add (sizerBox, 0, wxALL | wxALIGN_CENTER, 10);


	// Setting the trigger out mechanism.
	sizerBox= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Trigger Data Out"));

	// Create slider "Events"
	sliderEvents = new wxSlider (this, SLIDER_EVENTS, defaultValueEvents, 1, maxValueEvents, wxDefaultPosition, wxSize (145, 50), wxSL_LABELS);
	textEvents = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, -1, wxT ("Event Number:"), wxDefaultPosition, wxSize (100, 20), wxALIGN_LEFT));
	row->Add (sliderEvents);
	row->Add (textEvents, 0, wxLEFT, 10);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 10);


	// Create spiner "Time Out"
	timeOut = new wxSpinCtrl (this, TIME_OUT, wxEmptyString, wxDefaultPosition, wxSize (80, wxDefaultCoord));
	timeOut->SetRange (0, maxValueTimeOut);
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, -1, wxT ("Time Out:"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (timeOut);
	row->Add (new wxStaticText (this, -1, wxT ("microseconds")), 1, wxALIGN_LEFT);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 10);

	// Create 'Trigger Data Out' reset button.
	wxButton* resetTriggerOut =  new wxButton (this, RESET_TRIGGER_OUT, wxT ("Reset"), wxDefaultPosition, wxDefaultSize, 0);
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (resetTriggerOut, 0, wxALIGN_CENTER);
	sizerBox->Add (row, 0, wxTOP | wxBOTTOM | wxALIGN_CENTER, 10);


	sizerView->Add (sizerBox, 0, wxALL | wxALIGN_CENTER, 10);


	// Set sizer containing controls to this panel.
	this->SetSizerAndFit (sizerView);

	ResetSaveType();
	ResetTriggerOut();

	// Connect events to event handlers.
	Connect (wxID_ANY, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlView::OnSaveType));
	Connect (SLIDER_EVENTS, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlView::ChangeEventNum));
	Connect (TIME_OUT, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler (ControlView::ChangeTimeOut));

	Connect (wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (ControlView::ResetControls));
}

ControlView::~ControlView()
{
}

void ControlView::OnSaveType (wxCommandEvent& event)
{
	switch (event.GetId()) {
		case SAVE_FRAME:
			ChangeSaveFrame ();
			break;
		case SAVE_MOVIE:
			ChangeSaveMovie ();
			break;
	}
}

void ControlView::ChangeSaveFrame ()
{
	xemDeviceCtrl->SetCommand (Command::SAVE_TYPE, 1);
}

void ControlView::ChangeSaveMovie ()
{
	xemDeviceCtrl->SetCommand (Command::SAVE_TYPE, 0);
}

void ControlView::ChangeEventNum (wxScrollEvent& event)
{
	xemDeviceCtrl->SetCommand (Command::OCT_EVENT_NUM, event.GetPosition());
	SetTextEventNum ();
}

void ControlView::SetTextEventNum ()
{
	// Convert int to string
	char str[20] = "";
	sprintf (str, "%i", sliderEvents->GetValue());
	textEvents->SetLabel (wxString (str, wxConvUTF8));
}

void ControlView::ChangeTimeOut (wxSpinEvent& event)
{
	xemDeviceCtrl->SetCommand (Command::OCT_TIME_OUT, event.GetPosition());
}

void ControlView::ResetControls (wxCommandEvent& event)
{
	switch (event.GetId()) {
		case RESET_SAVE_TYPE :
			ResetSaveType ();
			break;
		case RESET_TRIGGER_OUT:
			ResetTriggerOut ();
			break;
	}
}

void ControlView::ResetSaveType ()
{
	saveFrame->SetValue (true);
	ChangeSaveFrame ();
	//xemDeviceCtrl->SetCommand (Command::SAVE_TYPE, 1);
}

void ControlView::ResetTriggerOut ()
{
	sliderEvents->SetValue (defaultValueEvents);
	xemDeviceCtrl->SetCommand (Command::OCT_EVENT_NUM, sliderEvents->GetValue());
	SetTextEventNum ();

	timeOut->SetValue (defaultValueTimeOut);
	xemDeviceCtrl->SetCommand (Command::OCT_TIME_OUT, timeOut->GetValue());
}

void ControlView::Start ()
{
	if (saveFrame->GetValue()) {
		ChangeSaveFrame ();
		//xemDeviceCtrl->SetCommand (Command::SAVE_TYPE, 1);
	} else {
		ChangeSaveMovie();
		//xemDeviceCtrl->SetCommand (Command::SAVE_TYPE, 0);
	}

	xemDeviceCtrl->SetCommand (Command::OCT_EVENT_NUM, sliderEvents->GetValue());
	xemDeviceCtrl->SetCommand (Command::OCT_TIME_OUT, timeOut->GetValue());
}

