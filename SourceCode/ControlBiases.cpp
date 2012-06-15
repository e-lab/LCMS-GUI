/**
 * @file
 * Contains the class that constructs the bias controls.
 */
#include "ControlBiases.h"
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

ControlBiases::ControlBiases (wxWindow* owner, DeviceController* controller) : ControlPanel (owner)
{
	xemDeviceCtrl =			controller;

	minValueSlider =		0;
	maxValueSlider =		65535; //16 bits
	maxVoltageSlider =		3.3f;

	wxSizer* sizerBiases = new wxBoxSizer (wxVERTICAL);
	wxStaticBoxSizer* sizerBox= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Bias Values"));

	//create slider for Int_gbt
	wxBoxSizer* row = new wxBoxSizer (wxHORIZONTAL);
	sliderInt_gbt = new wxSlider (this, Int_gbt, defaultValueInt_gbt, minValueSlider, maxValueSlider, wxDefaultPosition, wxSize (145, 20));
	textInt_gbt = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row->Add (new wxStaticText (this, -1, wxT ("Int_gbt"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderInt_gbt);
	row->Add (textInt_gbt);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//create slider for Int_Vbn
	row = new wxBoxSizer (wxHORIZONTAL);
	sliderInt_Vbn = new wxSlider (this, Int_Vbn, defaultValueInt_Vbn, minValueSlider, maxValueSlider, wxDefaultPosition, wxSize (145, 20));
	textInt_Vbn = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row->Add (new wxStaticText (this, -1, wxT ("Int_Vbn"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderInt_Vbn);
	row->Add (textInt_Vbn);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//create slider for Int_Vbp
	row = new wxBoxSizer (wxHORIZONTAL);
	sliderInt_Vbp = new wxSlider (this, Int_Vbp, defaultValueInt_Vbp, minValueSlider, maxValueSlider, wxDefaultPosition, wxSize (145, 20));
	textInt_Vbp = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row->Add (new wxStaticText (this, -1, wxT ("Int_Vbp"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderInt_Vbp);
	row->Add (textInt_Vbp);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//create slider for Post_gbt
	row = new wxBoxSizer (wxHORIZONTAL);
	sliderPost_gbt = new wxSlider (this, Post_gbt, defaultValuePost_gbt, minValueSlider, maxValueSlider, wxDefaultPosition, wxSize (145, 20));
	textPost_gbt = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row->Add (new wxStaticText (this, -1, wxT ("Post_gbt"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderPost_gbt);
	row->Add (textPost_gbt);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//create slider for Post_Vbn
	row = new wxBoxSizer (wxHORIZONTAL);
	sliderPost_Vbn = new wxSlider (this, Post_Vbn, defaultValuePost_Vbn, minValueSlider, maxValueSlider, wxDefaultPosition, wxSize (145, 20));
	textPost_Vbn = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row->Add (new wxStaticText (this, -1, wxT ("Post_Vbn"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderPost_Vbn);
	row->Add (textPost_Vbn);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//create slider for Post_Vbp
	row = new wxBoxSizer (wxHORIZONTAL);
	sliderPost_Vbp = new wxSlider (this, Post_Vbp, defaultValuePost_Vbp, minValueSlider, maxValueSlider, wxDefaultPosition, wxSize (145, 20));
	textPost_Vbp = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row->Add (new wxStaticText (this, -1, wxT ("Post_Vbp"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderPost_Vbp);
	row->Add (textPost_Vbp);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//create slider for OBuff_gbt
	row = new wxBoxSizer (wxHORIZONTAL);
	sliderOBuff_gbt = new wxSlider (this, OBuff_gbt, defaultValueOBuff_gbt, minValueSlider, maxValueSlider, wxDefaultPosition, wxSize (145, 20));
	textOBuff_gbt = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row->Add (new wxStaticText (this, -1, wxT ("OBuff_gbt"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderOBuff_gbt);
	row->Add (textOBuff_gbt);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//create slider for OBuff_Vbn
	row = new wxBoxSizer (wxHORIZONTAL);
	sliderOBuff_Vbn = new wxSlider (this, OBuff_Vbn, defaultValueOBuff_Vbn, minValueSlider, maxValueSlider, wxDefaultPosition, wxSize (145, 20));
	textOBuff_Vbn = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row->Add (new wxStaticText (this, -1, wxT ("OBuff_Vbn"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderOBuff_Vbn);
	row->Add (textOBuff_Vbn);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//create slider for OBuff_Vbp
	row = new wxBoxSizer (wxHORIZONTAL);
	sliderOBuff_Vbp = new wxSlider (this, OBuff_Vbp, defaultValueOBuff_Vbp, minValueSlider, maxValueSlider, wxDefaultPosition, wxSize (145, 20));
	textOBuff_Vbp = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row->Add (new wxStaticText (this, -1, wxT ("OBuff_Vbp"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderOBuff_Vbp);
	row->Add (textOBuff_Vbp);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//create slider for Vref
	row = new wxBoxSizer (wxHORIZONTAL);
	sliderVref = new wxSlider (this, Vref, defaultValueVref, minValueSlider, maxValueSlider, wxDefaultPosition, wxSize (145, 20));
	textVref = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row->Add (new wxStaticText (this, -1, wxT ("Vref"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderVref);
	row->Add (textVref);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//create slider for V_Cmd_Offset
	row = new wxBoxSizer (wxHORIZONTAL);
	sliderV_Cmd_Offset = new wxSlider (this, V_Cmd_Offset, defaultValueV_Cmd_Offset, minValueSlider, maxValueSlider, wxDefaultPosition, wxSize (145, 20));
	textV_Cmd_Offset = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	row->Add (new wxStaticText (this, -1, wxT ("V_Cmd_Offset"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT));
	row->Add (sliderV_Cmd_Offset);
	row->Add (textV_Cmd_Offset);
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//create spin for Digital OffsetCorrection
	row = new wxBoxSizer (wxHORIZONTAL);
	spinOffsetCorrection = new wxSpinCtrl (this, OffsetCorrection, wxEmptyString, wxDefaultPosition, wxSize (70, 20));
	row->Add (new wxStaticText(this, -1, wxT("Digital Offset Correction: "), wxDefaultPosition, wxSize(130, 20), wxALIGN_LEFT));
	row->Add (spinOffsetCorrection);
	row->Add (new wxStaticText (this, -1, wxT ("mV"), wxDefaultPosition, wxSize(85,20), wxALIGN_LEFT));
	sizerBox->Add (row, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 1);

	//Create compensation reset button
	wxButton* buttonReset =  new wxButton (this, RESET_BIASES, wxT ("Reset"), wxDefaultPosition, wxDefaultSize, 0);
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (buttonReset, 0, wxALIGN_CENTER);
	sizerBox->Add (row, 0, wxTOP | wxBOTTOM | wxALIGN_CENTER, 10);

	sizerBiases->Add (sizerBox, 0, wxALL | wxALIGN_CENTER, 10);
	this->SetSizerAndFit (sizerBiases);

	//set to controls to defaults when it starts
	Reset();

	// Connect events to event handlers

	//connect reset button
	Connect (RESET_BIASES, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (ControlBiases::ResetControls));

	//connect sliders
	Connect (Int_gbt, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderInt_gbt));
	Connect (Int_Vbn, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderInt_Vbn));
	Connect (Int_Vbp, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderInt_Vbp));
	Connect (Post_gbt, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderPost_gbt));
	Connect (Post_Vbn, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderPost_Vbn));
	Connect (Post_Vbp, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderPost_Vbp));
	Connect (OBuff_gbt, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderOBuff_gbt));
	Connect (OBuff_Vbn, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderOBuff_Vbn));
	Connect (OBuff_Vbp, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderOBuff_Vbp));
	Connect (Vref, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderVref));
	Connect (V_Cmd_Offset, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ControlBiases::OnSliderV_Cmd_Offset));
	Connect (OffsetCorrection, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler (ControlBiases::OnSpinOffsetCorrection));
}

ControlBiases::~ControlBiases()
{
}

void ControlBiases::OnSliderInt_gbt (wxScrollEvent& evt)
{
	::wxLogMessage (wxT ("sliderInt_gbt value:  %i"), evt.GetPosition());
	xemDeviceCtrl->SetCommand (Command::LCMS_INT_GBT, evt.GetPosition());
	float voltage = ( (float) sliderInt_gbt->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textInt_gbt->SetLabel (wxString (str, wxConvUTF8));
}

void ControlBiases::OnSliderInt_Vbn (wxScrollEvent& evt)
{
	::wxLogMessage (wxT ("sliderInt_Vbn value:  %i"), evt.GetPosition());
	xemDeviceCtrl->SetCommand (Command::LCMS_INT_VBN, evt.GetPosition());
	float voltage = ( (float) sliderInt_Vbn->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textInt_Vbn->SetLabel (wxString (str, wxConvUTF8));
}
void ControlBiases::OnSliderInt_Vbp (wxScrollEvent& evt)
{
	::wxLogMessage (wxT ("sliderInt_Vbp value:  %i"), evt.GetPosition());
	xemDeviceCtrl->SetCommand (Command::LCMS_INT_VBP, evt.GetPosition());
	float voltage = ( (float) sliderInt_Vbp->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textInt_Vbp->SetLabel (wxString (str, wxConvUTF8));
}

void ControlBiases::OnSliderPost_gbt (wxScrollEvent& evt)
{
	::wxLogMessage (wxT ("sliderPost_gbt value:  %i"), evt.GetPosition());
	xemDeviceCtrl->SetCommand (Command::LCMS_POST_GBT, evt.GetPosition());
	float voltage = ( (float) sliderPost_gbt->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textPost_gbt->SetLabel (wxString (str, wxConvUTF8));
}

void ControlBiases::OnSliderPost_Vbn (wxScrollEvent& evt)
{
	::wxLogMessage (wxT ("sliderPost_Vbn value:  %i"), evt.GetPosition());
	xemDeviceCtrl->SetCommand (Command::LCMS_POST_VBN, evt.GetPosition());
	float voltage = ( (float) sliderPost_Vbn->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textPost_Vbn->SetLabel (wxString (str, wxConvUTF8));
}

void ControlBiases::OnSliderPost_Vbp (wxScrollEvent& evt)
{
	::wxLogMessage (wxT ("sliderPost_Vbp value:  %i"), evt.GetPosition());
	xemDeviceCtrl->SetCommand (Command::LCMS_POST_VBP, evt.GetPosition());
	float voltage = ( (float) sliderPost_Vbp->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textPost_Vbp->SetLabel (wxString (str, wxConvUTF8));
}
void ControlBiases::OnSliderOBuff_gbt (wxScrollEvent& evt)
{
	::wxLogMessage (wxT ("sliderOBuff_gbt value:  %i"), evt.GetPosition());
	xemDeviceCtrl->SetCommand (Command::LCMS_OBUFF_GBT, evt.GetPosition());
	float voltage = ( (float) sliderOBuff_gbt->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textOBuff_gbt->SetLabel (wxString (str, wxConvUTF8));
}
void ControlBiases::OnSliderOBuff_Vbn (wxScrollEvent& evt)
{
	::wxLogMessage (wxT ("sliderOBuff_Vbn value:  %i"), evt.GetPosition());
	xemDeviceCtrl->SetCommand (Command::LCMS_OBUFF_VBN, evt.GetPosition());
	float voltage = ( (float) sliderOBuff_Vbn->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textOBuff_Vbn->SetLabel (wxString (str, wxConvUTF8));
}

void ControlBiases::OnSliderOBuff_Vbp (wxScrollEvent& evt)
{
	::wxLogMessage (wxT ("sliderOBuff_Vbp value:  %i"), evt.GetPosition());
	xemDeviceCtrl->SetCommand (Command::LCMS_OBUFF_VBP, evt.GetPosition());
	float voltage = ( (float) sliderOBuff_Vbp->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textOBuff_Vbp->SetLabel (wxString (str, wxConvUTF8));
}


void ControlBiases::OnSliderVref (wxScrollEvent& evt)
{
	::wxLogMessage (wxT ("sliderVref value:  %i"), evt.GetPosition());
	xemDeviceCtrl->SetCommand (Command::LCMS_VREF, evt.GetPosition());
	float voltage = ( (float) sliderVref->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textVref->SetLabel (wxString (str, wxConvUTF8));
}

void ControlBiases::OnSliderV_Cmd_Offset (wxScrollEvent& evt)
{
	::wxLogMessage (wxT ("sliderV_Cmd_Offset value:  %i"), evt.GetPosition());
	xemDeviceCtrl->SetCommand (Command::PC5_V_CMD_OFFSET, evt.GetPosition());
	float voltage = ( (float) sliderV_Cmd_Offset->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
	// Convert float to string
	char str[20] = "";
	sprintf (str, "%f", voltage);
	textV_Cmd_Offset->SetLabel (wxString (str, wxConvUTF8));
}

void ControlBiases::OnSpinOffsetCorrection (wxSpinEvent& evt)
{
	::wxLogMessage (wxT ("sliderOffsetCorrection value:  %i"), evt.GetPosition());
	spinOffsetCorrection->SetRange (-3000, 3000);
	xemDeviceCtrl->SetCommand (Command::LCMS_OFFSET_CORRECTION, evt.GetPosition());
}


void ControlBiases::Reset (void)
{
	float vMax = 3.3f;
	float sliderMax = 65535.0f;

	float defaultInt_gbtV=1.280f;	//1.28V
	float defaultInt_VbnV=1.600f;	//1.6V
	float defaultInt_VbpV=1.300f;	//1.3V

	float defaultPost_gbtV=0.93f;	//.93V
	float defaultPost_VbnV=1.35f;	//1.35V

	//float defaultPost_VbpV=1.6f;	//1.6V  use this value later!!!
	float defaultPost_VbpV=1.3f;	//only because on the chip on board 3 int vbp is tied to post vbp via wirebond (broken pin)

	float defaultOBuff_gbtV=1.15f;		//1.15V
	float defaultOBuff_VbnV=1.4f;		//1.4V
	float defaultOBuff_VbpV=0.95f;		//0.95V

	float defaultVrefV=1.65f;		 //1.65V
	float defaultV_Cmd_OffsetV=1.65f;	 //1.65V
	float defaultOffsetCorrection=150.0;

	int defaultInt_gbt= (defaultInt_gbtV/vMax) *sliderMax;
	int defaultInt_Vbn= (defaultInt_VbnV/vMax) *sliderMax;
	int defaultInt_Vbp= (defaultInt_VbpV/vMax) *sliderMax;

	int defaultPost_gbt= (defaultPost_gbtV/vMax) *sliderMax;
	int defaultPost_Vbn= (defaultPost_VbnV/vMax) *sliderMax;
	int defaultPost_Vbp= (defaultPost_VbpV/vMax) *sliderMax;

	int defaultOBuff_gbt= (defaultOBuff_gbtV/vMax) *sliderMax;
	int defaultOBuff_Vbn= (defaultOBuff_VbnV/vMax) *sliderMax;
	int defaultOBuff_Vbp= (defaultOBuff_VbpV/vMax) *sliderMax;

	int defaultVref= (defaultVrefV/vMax) *sliderMax;
	int defaultV_Cmd_Offset= (defaultV_Cmd_OffsetV/vMax) *sliderMax;

	sliderInt_gbt->SetValue (defaultInt_gbt);
	sliderInt_Vbn->SetValue (defaultInt_Vbn);
	sliderInt_Vbp->SetValue (defaultInt_Vbp);

	sliderPost_gbt->SetValue (defaultPost_gbt);
	sliderPost_Vbn->SetValue (defaultPost_Vbn);
	sliderPost_Vbp->SetValue (defaultPost_Vbp);

	sliderOBuff_gbt->SetValue (defaultOBuff_gbt);
	sliderOBuff_Vbn->SetValue (defaultOBuff_Vbn);
	sliderOBuff_Vbp->SetValue (defaultOBuff_Vbp);

	sliderVref->SetValue (defaultVref);
	sliderV_Cmd_Offset->SetValue (defaultV_Cmd_Offset);
	spinOffsetCorrection->SetRange (-3000, 3000);
	spinOffsetCorrection->SetValue(defaultOffsetCorrection);

	char str[20] = "";

	sprintf (str, "%f", sliderInt_gbt->GetValue() /sliderMax*vMax);
	textInt_gbt->SetLabel (wxString (str, wxConvUTF8));
	sprintf (str, "%f", sliderInt_Vbn->GetValue() /sliderMax*vMax);
	textInt_Vbn->SetLabel (wxString (str, wxConvUTF8));
	sprintf (str, "%f", sliderInt_Vbp->GetValue() /sliderMax*vMax);
	textInt_Vbp->SetLabel (wxString (str, wxConvUTF8));

	sprintf (str, "%f", sliderPost_gbt->GetValue() /sliderMax*vMax);
	textPost_gbt->SetLabel (wxString (str, wxConvUTF8));
	sprintf (str, "%f", sliderPost_Vbn->GetValue() /sliderMax*vMax);
	textPost_Vbn->SetLabel (wxString (str, wxConvUTF8));
	sprintf (str, "%f", sliderPost_Vbp->GetValue() /sliderMax*vMax);
	textPost_Vbp->SetLabel (wxString (str, wxConvUTF8));

	sprintf (str, "%f", sliderOBuff_gbt->GetValue() /sliderMax*vMax);
	textOBuff_gbt->SetLabel (wxString (str, wxConvUTF8));
	sprintf (str, "%f", sliderOBuff_Vbn->GetValue() /sliderMax*vMax);
	textOBuff_Vbn->SetLabel (wxString (str, wxConvUTF8));
	sprintf (str, "%f", sliderOBuff_Vbp->GetValue() /sliderMax*vMax);
	textOBuff_Vbp->SetLabel (wxString (str, wxConvUTF8));

	sprintf (str, "%f", sliderVref->GetValue() /sliderMax*vMax);
	textVref->SetLabel (wxString (str, wxConvUTF8));
	sprintf (str, "%f", sliderV_Cmd_Offset->GetValue() /sliderMax*vMax);
	textV_Cmd_Offset->SetLabel (wxString (str, wxConvUTF8));

	xemDeviceCtrl->SetCommand (Command::LCMS_INT_GBT,defaultInt_gbt);
	xemDeviceCtrl->SetCommand (Command::LCMS_INT_VBN,defaultInt_Vbn);
	xemDeviceCtrl->SetCommand (Command::LCMS_INT_VBP,defaultInt_Vbp);
	xemDeviceCtrl->SetCommand (Command::LCMS_POST_GBT,defaultPost_gbt);
	xemDeviceCtrl->SetCommand (Command::LCMS_POST_VBN,defaultPost_Vbn);
	xemDeviceCtrl->SetCommand (Command::LCMS_POST_VBP,defaultPost_Vbp);
	xemDeviceCtrl->SetCommand (Command::LCMS_OBUFF_GBT,defaultOBuff_gbt);
	xemDeviceCtrl->SetCommand (Command::LCMS_OBUFF_VBN,defaultOBuff_Vbn);
	xemDeviceCtrl->SetCommand (Command::LCMS_OBUFF_VBP,defaultOBuff_Vbp);
	xemDeviceCtrl->SetCommand (Command::LCMS_VREF,defaultVref);
	xemDeviceCtrl->SetCommand (Command::PC5_V_CMD_OFFSET,defaultV_Cmd_Offset);
	xemDeviceCtrl->SetCommand (Command::LCMS_OFFSET_CORRECTION,defaultOffsetCorrection);
}

void ControlBiases::ResetControls (wxCommandEvent& evt)
{
	Reset();
}
