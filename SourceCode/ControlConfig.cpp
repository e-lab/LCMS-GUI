/**
 * @file
 * Contains the class that constructs the bias controls.
 */
#include "ControlConfig.h"
#include "Command.h"
#include "DeviceController.h"
#include "GraphicsPlot.h"

// When debugging changes all calls to "new" to be calls to "DEBUG_NEW" allowing for memory leaks to
// give you the file name and line number where it occurred.
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#define new DEBUG_NEW
#else
#define DEBUG_NEW new
#endif

ControlConfig::ControlConfig (wxWindow* owner, DeviceController* device) : ControlPanel (owner)
{
	xemDeviceCtrl =			device;

	minValueSlider =		0;
	maxValueSlider =		65535; //16 bits
	maxVoltageSlider =		3.3f;

	//Create Box for CAPSELECT
	wxSizer* sizerConfig = new wxBoxSizer (wxVERTICAL);
	wxStaticBoxSizer* sizerBox= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Feedback Capacitor For Integrator:"));
	wxFlexGridSizer* sizerBoxInner= new wxFlexGridSizer (2,2,0);
	//create Feedback Capacitor Radio Buttons
	capSelect0 = new wxRadioButton (this, CAPSELECT0, wxT ("1pF"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	capSelect1 = new wxRadioButton (this, CAPSELECT1, wxT ("100fF"));
	sizerBoxInner->Add (capSelect0);
	sizerBoxInner->Add (capSelect1);
	sizerBox->Add (sizerBoxInner);
	sizerConfig->Add (sizerBox, 0, wxALL | wxEXPAND | wxALIGN_CENTER, 10);

	//Create Box for BYPASSPOST and POSTGAINSELECT
	wxStaticBoxSizer* sizerBox2= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Post Amp Configuration:"));
	wxFlexGridSizer* sizerBox2Inner= new wxFlexGridSizer (2,2,10);
	//create bypassPost Radio Buttons
	bypassPost0 = new wxRadioButton (this, BYPASSPOST0, wxT ("Enabled"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	bypassPost1 = new wxRadioButton (this, BYPASSPOST1, wxT ("Bypassed"));
	sizerBox2Inner->Add (bypassPost0);
	sizerBox2Inner->Add (bypassPost1);
	//create postGainSelect Radio Buttons
	postGainSelect0 = new wxRadioButton (this, POSTGAINSELECT0, wxT ("Gain 5"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	postGainSelect1 = new wxRadioButton (this, POSTGAINSELECT1, wxT ("Gain 10"));
	sizerBox2Inner->Add (postGainSelect0);
	sizerBox2Inner->Add (postGainSelect1);

	sizerBox2->Add (sizerBox2Inner);
	sizerConfig->Add (sizerBox2, 0, wxEXPAND | wxALL | wxALIGN_CENTER, 10);

	//Create Box for CHANNEL SELECTION
	wxStaticBoxSizer* sizerBox4= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Input Channel Selection:"));
	wxFlexGridSizer* sizerBox4Inner= new wxFlexGridSizer (2,2,10);
	////create ChannelSelect Radio Buttons
	channel1Sel  = new wxRadioButton (this, CHANNEL1_SEL,  wxT ("Channel 1"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	channel9Sel  = new wxRadioButton (this, CHANNEL9_SEL,  wxT ("Channel 2"));
	channel5Sel  = new wxRadioButton (this, CHANNEL5_SEL,  wxT ("Channel 3"));
	channel13Sel = new wxRadioButton (this, CHANNEL13_SEL, wxT ("Channel 4"));
	channel3Sel  = new wxRadioButton (this, CHANNEL3_SEL,  wxT ("Channel 5"));
	channel11Sel = new wxRadioButton (this, CHANNEL11_SEL, wxT ("Channel 6"));
	channel7Sel  = new wxRadioButton (this, CHANNEL7_SEL,  wxT ("Channel 7"));
	channel15Sel = new wxRadioButton (this, CHANNEL15_SEL, wxT ("Channel 8"));
	channel2Sel  = new wxRadioButton (this, CHANNEL2_SEL,  wxT ("Channel 9"));
	channel10Sel = new wxRadioButton (this, CHANNEL10_SEL, wxT ("Channel 10"));
	channel6Sel  = new wxRadioButton (this, CHANNEL6_SEL,  wxT ("Channel 11"));
	channel14Sel = new wxRadioButton (this, CHANNEL14_SEL, wxT ("Channel 12"));
	channel4Sel  = new wxRadioButton (this, CHANNEL4_SEL,  wxT ("Channel 13"));
	channel12Sel = new wxRadioButton (this, CHANNEL12_SEL, wxT ("Channel 14"));
	channel8Sel  = new wxRadioButton (this, CHANNEL8_SEL,  wxT ("Channel 15"));
	channel16Sel = new wxRadioButton (this, CHANNEL16_SEL, wxT ("Channel 16"));

	sizerBox4Inner->Add (channel1Sel);
	sizerBox4Inner->Add (channel9Sel);
	sizerBox4Inner->Add (channel5Sel);
	sizerBox4Inner->Add (channel13Sel);
	sizerBox4Inner->Add (channel3Sel);
	sizerBox4Inner->Add (channel11Sel);
	sizerBox4Inner->Add (channel7Sel);
	sizerBox4Inner->Add (channel15Sel);
	sizerBox4Inner->Add (channel2Sel);
	sizerBox4Inner->Add (channel10Sel);
	sizerBox4Inner->Add (channel6Sel);
	sizerBox4Inner->Add (channel14Sel);
	sizerBox4Inner->Add (channel4Sel);
	sizerBox4Inner->Add (channel12Sel);
	sizerBox4Inner->Add (channel8Sel);
	sizerBox4Inner->Add (channel16Sel);

	sizerBox4->Add (sizerBox4Inner);
	sizerConfig->Add (sizerBox4, 0, wxEXPAND | wxALL | wxALIGN_CENTER, 10);

	//Create Box for GAIN
	wxStaticBoxSizer* sizerBox5= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Gain (x10^-2):"));
	//wxFlexGridSizer* sizerBox5Inner= new wxFlexGridSizer(2,2,10);
	spinGain = new wxSpinCtrl (this, GAIN, wxEmptyString, wxDefaultPosition, wxSize (100, wxDefaultCoord));
	spinGain->SetRange (-400, 400);
	spinGain->SetValue (100);
	sizerBox5->Add (spinGain);
	sizerConfig->Add (sizerBox5, 0, wxEXPAND | wxALL | wxALIGN_CENTER, 10);

	//Create reset button
	wxButton* buttonReset =  new wxButton (this, RESET_CONFIG, wxT ("Reset"), wxDefaultPosition, wxDefaultSize, 0);
	wxBoxSizer* row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (buttonReset, 0, wxALIGN_CENTER);
	sizerConfig->Add (row, 0, wxTOP | wxBOTTOM | wxALIGN_CENTER, 10);

	this->SetSizerAndFit (sizerConfig);

	//set controls to defaults when it starts
	Reset();

	// Connect events to event handlers
	//connect reset button
	Connect (RESET_CONFIG, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (ControlConfig::ResetControls));

	//connect radio buttons
	Connect (CAPSELECT0, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnCapSelect0));
	Connect (CAPSELECT1, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnCapSelect1));
	Connect (BYPASSPOST0, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnBypassPost0));
	Connect (BYPASSPOST1, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnBypassPost1));

	Connect (POSTGAINSELECT0, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnPostGainSelect0));
	Connect (POSTGAINSELECT1, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnPostGainSelect1));

	Connect (CHANNEL1_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel1Sel));
	Connect (CHANNEL2_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel2Sel));
	Connect (CHANNEL3_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel3Sel));
	Connect (CHANNEL4_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel4Sel));
	Connect (CHANNEL5_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel5Sel));
	Connect (CHANNEL6_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel6Sel));
	Connect (CHANNEL7_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel7Sel));
	Connect (CHANNEL8_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel8Sel));
	Connect (CHANNEL9_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel9Sel));
	Connect (CHANNEL10_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel10Sel));
	Connect (CHANNEL11_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel11Sel));
	Connect (CHANNEL12_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel12Sel));
	Connect (CHANNEL13_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel13Sel));
	Connect (CHANNEL14_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel14Sel));
	Connect (CHANNEL15_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel15Sel));
	Connect (CHANNEL16_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel16Sel));

	//connect spin ctrls
	Connect (GAIN, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler (ControlConfig::OnSpinGain));

}

ControlConfig::~ControlConfig()
{
}

void ControlConfig::OnCapSelect0 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnCapSelect0 value:  0"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CAPSELECT, 1);
}

void ControlConfig::OnCapSelect1 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnCapSelect1 value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CAPSELECT, 0);
}
void ControlConfig::OnBypassPost0 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("BypassPost0 value:  0"));
	xemDeviceCtrl->SetCommand (Command::LCMS_BYPASSPOST, 0);
}

void ControlConfig::OnBypassPost1 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("BypassPost1 value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_BYPASSPOST, 1);
}

void ControlConfig::OnPostGainSelect0 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("postGainSelect0 value:  0"));
	xemDeviceCtrl->SetCommand (Command::LCMS_POSTGAINSELECT, 0);
}

void ControlConfig::OnPostGainSelect1 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("postGainSelect1 value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_POSTGAINSELECT, 1);
}

void ControlConfig::OnChannel1Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel1Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 1);
}
void ControlConfig::OnChannel2Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel2Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 2);
}
void ControlConfig::OnChannel3Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel3Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 3);
}
void ControlConfig::OnChannel4Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel4Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 4);
}
void ControlConfig::OnChannel5Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel5Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 5);
}
void ControlConfig::OnChannel6Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel6Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 6);
}
void ControlConfig::OnChannel7Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel7Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 7);
}
void ControlConfig::OnChannel8Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel8Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 8);
}
void ControlConfig::OnChannel9Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel9Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 9);
}
void ControlConfig::OnChannel10Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel10Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 10);
}
void ControlConfig::OnChannel11Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel11Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 11);
}
void ControlConfig::OnChannel12Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel2Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 12);
}
void ControlConfig::OnChannel13Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel13Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 13);
}
void ControlConfig::OnChannel14Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel14Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 14);
}
void ControlConfig::OnChannel15Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel15Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 15);
}
void ControlConfig::OnChannel16Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel16Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 16);
}


void ControlConfig::OnSpinGain (wxSpinEvent& evt)
{
	//int value = spinGain->GetValue();
	//graphicsPlot->SetGain ( (double) value/100.0);
}

void ControlConfig::Reset (void)
{
//	float vMax = 3.3f;
//	float sliderMax = 65535.0f;

	bool defaultcapSelect=true;//1pF
	bool defaultbypassPost=true;//disabled
	bool defaultPostGainSelect=true;//10

	bool defaultChannelSel=true;//ch1


	//float defaultVthpV=1.8f;
	//float defaultVthmV=1.5f;

	//float defaultVthpV=1.68f;
	//float defaultVthmV=1.62f;

	//float defaultVoffsetB=0.0f;

//	int defaultVthp=(defaultVthpV/vMax)*sliderMax;
//	int defaultVthm=(defaultVthmV/vMax)*sliderMax;

	//int defaultCounterClock=2; //20 mhz

	capSelect0->SetValue (defaultcapSelect);
	capSelect1->SetValue (!defaultcapSelect);

	bypassPost0->SetValue (!defaultbypassPost);
	bypassPost1->SetValue (defaultbypassPost);

	postGainSelect0->SetValue (!defaultPostGainSelect);
	postGainSelect1->SetValue (defaultPostGainSelect);

	channel1Sel->SetValue (defaultChannelSel);
	channel2Sel->SetValue (!defaultChannelSel);
	channel3Sel->SetValue (!defaultChannelSel);
	channel4Sel->SetValue (!defaultChannelSel);
	channel5Sel->SetValue (!defaultChannelSel);
	channel6Sel->SetValue (!defaultChannelSel);
	channel7Sel->SetValue (!defaultChannelSel);
	channel8Sel->SetValue (!defaultChannelSel);
	channel9Sel->SetValue (!defaultChannelSel);
	channel10Sel->SetValue (!defaultChannelSel);
	channel11Sel->SetValue (!defaultChannelSel);
	channel12Sel->SetValue (!defaultChannelSel);
	channel13Sel->SetValue (!defaultChannelSel);
	channel14Sel->SetValue (!defaultChannelSel);
	channel15Sel->SetValue (!defaultChannelSel);
	channel16Sel->SetValue (!defaultChannelSel);

	spinGain->SetValue (100);


//	char str[20] = "";

	xemDeviceCtrl->SetCommand (Command::LCMS_CAPSELECT, defaultcapSelect);
	xemDeviceCtrl->SetCommand (Command::LCMS_POSTGAINSELECT, defaultcapSelect);
	xemDeviceCtrl->SetCommand (Command::LCMS_BYPASSPOST, defaultbypassPost);
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 1);
}

void ControlConfig::ResetControls (wxCommandEvent& evt)
{
	Reset();
}

void ControlConfig::SetGraphicsPlot (GraphicsPlot *a)
{
	graphicsPlot = a;
}
