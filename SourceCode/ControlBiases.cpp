/**
 * @file
 * Contains the class that constructs the bias controls.
 */
#include "ControlBiases.h"
#include "Command.h"
#include "DeviceController.h"

ControlBiases::ControlBiases (wxWindow* owner, DeviceController* controller) : ControlPanel (owner)
{
	xemDeviceCtrl = controller;

	maxValuePBBias =		255;
	defaultValuePBBias =		139;
	maxVoltagePBBias =		3.3f;

	maxValueVDDA =			255;
	defaultValueVDDA =		0;
	maxVoltageVDDA =		3.3f;

	maxValueVDDR =			255;
	defaultValueVDDR =		0;
	maxVoltageVDDR =		3.3f;

	maxValueLPU =			255;
	defaultValueLPU =		123;
	maxVoltageLPU =			3.3f;

	wxSizer* sizerBiases = new wxBoxSizer (wxVERTICAL);
	wxStaticBoxSizer* sizerBox= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Bias Values"));

	// Create slider "PBBias"
	sliderPBBias = new wxSlider (this, SLIDER_PD_BIAS, defaultValuePBBias, 0, maxValuePBBias, wxDefaultPosition, wxSize (145, 50), wxSL_LABELS | wxSL_INVERSE);
	textPBBias = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	wxSizer* row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, -1, wxT ("PBBias:"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderPBBias);
	row->Add (textPBBias, 0, wxLEFT, 10);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 10);

	// Create slider "VDDA"
	sliderVDDA = new wxSlider (this, SLIDER_VDDA, defaultValueVDDA, 0, maxValueVDDA, wxDefaultPosition, wxSize (145, 50), wxSL_LABELS);
	//sliderVDDA = new wxSlider (this, SLIDER_VDDA, defaultValueVDDA, 0, maxValueVDDA, wxDefaultPosition, wxSize (145, 50), wxSL_LABELS | wxSL_INVERSE);
	textVDDA = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, -1, wxT ("VDDA:"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderVDDA);
	row->Add (textVDDA, 0, wxLEFT, 10);
	sizerBox->Add (row, 0, wxTOP | wxALIGN_CENTER, 5);

	// Create slider "VDDR"
	sliderVDDR = new wxSlider (this, SLIDER_VDDR, defaultValueVDDR, 0, maxValueVDDR, wxDefaultPosition, wxSize (145, 50), wxSL_LABELS);
	//sliderVDDR = new wxSlider (this, SLIDER_VDDR, defaultValueVDDR, 0, maxValueVDDR, wxDefaultPosition, wxSize (145, 50), wxSL_LABELS | wxSL_INVERSE);
	textVDDR = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, -1, wxT ("VDDR:"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderVDDR);
	row->Add (textVDDR, 0, wxLEFT, 10);
	sizerBox->Add (row, 0, wxTOP | wxALIGN_CENTER, 5);

	// Create slider "LPU"
	sliderLPU = new wxSlider (this, SLIDER_LPU, defaultValueLPU, 0, maxValueLPU, wxDefaultPosition, wxSize (145, 50), wxSL_LABELS | wxSL_INVERSE);
	textLPU = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, -1, wxT ("LPU:"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderLPU);
	row->Add (textLPU, 0, wxLEFT, 10);
	sizerBox->Add (row, 0, wxTOP | wxALIGN_CENTER, 5);


	// Create Bias reset button
	wxButton* buttonReset =  new wxButton (this, RESET_OCT_BIASES, wxT ("Reset"), wxDefaultPosition, wxDefaultSize, 0);
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (buttonReset, 0, wxALIGN_CENTER);
	sizerBox->Add (row, 0, wxTOP | wxBOTTOM | wxALIGN_CENTER, 10);

	sizerBiases->Add (sizerBox, 0, wxALL | wxALIGN_CENTER, 10);


	this->SetSizerAndFit (sizerBiases);

	ResetBiasValues();

	// Connect events to event handlers
	Connect (SLIDER_PD_BIAS, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderPBBias));
	Connect (SLIDER_VDDA, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderVDDA));
	Connect (SLIDER_VDDR, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderVDDR));
	Connect (SLIDER_LPU, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderLPU));

	Connect (RESET_OCT_BIASES, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (ControlBiases::ResetControls));
}

ControlBiases::~ControlBiases()
{
}

void ControlBiases::OnSliderPBBias (wxScrollEvent& event)
{
	xemDeviceCtrl->SetCommand (Command::OCT_PD_BIAS, event.GetPosition());
	SetTextPBBias();
}

void ControlBiases::OnSliderVDDA (wxScrollEvent& event)
{
	xemDeviceCtrl->SetCommand (Command::OCT_VDDA, event.GetPosition());
	SetTextVDDA();
}

void ControlBiases::OnSliderVDDR (wxScrollEvent& event)
{
	xemDeviceCtrl->SetCommand (Command::OCT_VDDR, event.GetPosition());
	SetTextVDDR();
}

void ControlBiases::OnSliderLPU (wxScrollEvent& event)
{
	xemDeviceCtrl->SetCommand (Command::OCT_LPU, event.GetPosition());
	SetTextLPU();
}

void ControlBiases::ResetControls (wxCommandEvent& event)
{
	ResetBiasValues();
}

void ControlBiases::ResetBiasValues()
{
	sliderPBBias->SetValue (defaultValuePBBias);
	SetTextPBBias();
	xemDeviceCtrl->SetCommand (Command::OCT_PD_BIAS, sliderPBBias->GetValue());

	sliderVDDA->SetValue (defaultValueVDDA);
	SetTextVDDA();
	xemDeviceCtrl->SetCommand (Command::OCT_VDDA, sliderVDDA->GetValue());

	sliderVDDR->SetValue (defaultValueVDDR);
	SetTextVDDR();
	xemDeviceCtrl->SetCommand (Command::OCT_VDDR, sliderVDDR->GetValue());

	sliderLPU->SetValue (defaultValueLPU);
	SetTextLPU();
	xemDeviceCtrl->SetCommand (Command::OCT_LPU, sliderLPU->GetValue());
}

void ControlBiases::SetTextPBBias()
{
	float voltage = ( (float) maxValuePBBias - (float) sliderPBBias->GetValue()) / ( (float) maxValuePBBias) * maxVoltagePBBias;

	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textPBBias->SetLabel (wxString (str, wxConvUTF8));
}

void ControlBiases::SetTextVDDA()
{
	float voltage = ( (float) sliderVDDA->GetValue()) / ( (float) maxValueVDDA) * maxVoltageVDDA;

	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textVDDA->SetLabel (wxString (str, wxConvUTF8));
}

void ControlBiases::SetTextVDDR()
{
	float voltage = ( (float) sliderVDDR->GetValue()) / ( (float) maxValueVDDR) * maxVoltageVDDR;

	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textVDDR->SetLabel (wxString (str, wxConvUTF8));
}

void ControlBiases::SetTextLPU()
{
	float voltage = ( (float) maxValueLPU - (float) sliderLPU->GetValue()) / ( (float) maxValueLPU) * maxVoltageLPU;

	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textLPU->SetLabel (wxString (str, wxConvUTF8));
}

void ControlBiases::Start()
{
	xemDeviceCtrl->SetCommand (Command::OCT_PD_BIAS, sliderPBBias->GetValue());
	xemDeviceCtrl->SetCommand (Command::OCT_VDDA, sliderVDDA->GetValue());
	xemDeviceCtrl->SetCommand (Command::OCT_VDDR, sliderVDDR->GetValue());
	xemDeviceCtrl->SetCommand (Command::OCT_LPU, sliderLPU->GetValue());

}
