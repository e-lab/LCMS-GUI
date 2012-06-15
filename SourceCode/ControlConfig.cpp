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
	wxStaticBoxSizer* sizerBox= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Feedback Capacitor For Integrator (ch0-3):")); //note: for ch 4-7, choose "none" for 100fF and "100fF" for 1pF
	wxFlexGridSizer* sizerBoxInner= new wxFlexGridSizer (3,3,0);
	//create Feedback Capacitor Radio Buttons
	capSelect0 = new wxRadioButton (this, CAPSELECT0, wxT ("1 pF"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	capSelect1 = new wxRadioButton (this, CAPSELECT1, wxT ("100 fF"));
	capSelect2 = new wxRadioButton (this, CAPSELECT2, wxT ("none"));
	sizerBoxInner->Add (capSelect0);
	sizerBoxInner->Add (capSelect1);
	sizerBoxInner->Add (capSelect2);
	sizerBox->Add (sizerBoxInner);
	sizerConfig->Add (sizerBox, 0, wxALL | wxEXPAND | wxALIGN_CENTER, 10);

	//Create box for RES_SELECT
	wxStaticBoxSizer* sizerBox3= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Feedback Resistor For Amp:"));
	wxFlexGridSizer* sizerBox3Inner= new wxFlexGridSizer (3,3,0);
	//create Feedback Capacitor Radio Buttons
	resSelect0 = new wxRadioButton (this, RES_SELECT0, wxT ("none"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	resSelect1 = new wxRadioButton (this, RES_SELECT1, wxT ("25 Kohms"));
	sizerBox3Inner->Add (resSelect0);
	sizerBox3Inner->Add (resSelect1);
	sizerBox3->Add (sizerBox3Inner);
	sizerConfig->Add (sizerBox3, 0, wxALL | wxEXPAND | wxALIGN_CENTER, 10);


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
	
	//Create box for CDS enabled or bypassed
	wxStaticBoxSizer* sizerBox7= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("On-chip CDS (channels 0-3 only):"));
	wxFlexGridSizer* sizerBox7Inner= new wxFlexGridSizer (3,3,0);
	//create Feedback Capacitor Radio Buttons
	bypassCDS0 = new wxRadioButton (this, BYPASS_CDS0, wxT ("Enabled"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	bypassCDS1 = new wxRadioButton (this, BYPASS_CDS1, wxT ("Bypassed"));
	sizerBox7Inner->Add (bypassCDS0);
	sizerBox7Inner->Add (bypassCDS1);
	sizerBox7->Add (sizerBox7Inner);
	sizerConfig->Add (sizerBox7, 0, wxALL | wxEXPAND | wxALIGN_CENTER, 10);

	//Create box for Input Filter Select
	wxStaticBoxSizer* sizerBox6= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Vcmd Input Filter Configuration:"));
	wxFlexGridSizer* sizerBox6Inner= new wxFlexGridSizer (3,3,0);
	//create Feedback Capacitor Radio Buttons
	inFiltSelN0 = new wxRadioButton (this, INFILTER_SELN0, wxT ("Enabled"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	inFiltSelN1 = new wxRadioButton (this, INFILTER_SELN1, wxT ("Bypassed"));
	sizerBox6Inner->Add (inFiltSelN0);
	sizerBox6Inner->Add (inFiltSelN1);
	sizerBox6->Add (sizerBox6Inner);
	sizerConfig->Add (sizerBox6, 0, wxALL | wxEXPAND | wxALIGN_CENTER, 10);


	//Create Box for CHANNEL SELECTION
	wxStaticBoxSizer* sizerBox4= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Input Channel Selection:"));
	wxFlexGridSizer* sizerBox4Inner= new wxFlexGridSizer (2,2,10);
	////create ChannelSelect Radio Buttons
	channel0Sel = new wxRadioButton (this, CHANNEL0_SEL, wxT ("Channel 0"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	channel1Sel = new wxRadioButton (this, CHANNEL1_SEL, wxT ("Channel 1"));
	channel2Sel = new wxRadioButton (this, CHANNEL2_SEL, wxT ("Channel 2"));
	channel3Sel = new wxRadioButton (this, CHANNEL3_SEL, wxT ("Channel 3"));
	channel4Sel = new wxRadioButton (this, CHANNEL4_SEL, wxT ("Channel 4"));
	channel5Sel = new wxRadioButton (this, CHANNEL5_SEL, wxT ("Channel 5"));
	channel6Sel = new wxRadioButton (this, CHANNEL6_SEL, wxT ("Channel 6"));
	channel7Sel = new wxRadioButton (this, CHANNEL7_SEL, wxT ("Channel 7"));
	
	sizerBox4Inner->Add (channel0Sel);
	sizerBox4Inner->Add (channel1Sel);
	sizerBox4Inner->Add (channel2Sel);
	sizerBox4Inner->Add (channel3Sel);
	sizerBox4Inner->Add (channel4Sel);
	sizerBox4Inner->Add (channel5Sel);
	sizerBox4Inner->Add (channel6Sel);
	sizerBox4Inner->Add (channel7Sel);
	
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
	Connect (CAPSELECT2, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnCapSelect2));

	Connect (RES_SELECT0, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnResSelect0));
	Connect (RES_SELECT1, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnResSelect1));

	Connect (BYPASSPOST0, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnBypassPost0));
	Connect (BYPASSPOST1, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnBypassPost1));

	Connect (POSTGAINSELECT0, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnPostGainSelect0));
	Connect (POSTGAINSELECT1, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnPostGainSelect1));

	Connect (BYPASS_CDS0, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnBypassCDS0));
	Connect (BYPASS_CDS1, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnBypassCDS1));

	Connect (INFILTER_SELN0, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnInFiltNSel0));
	Connect (INFILTER_SELN1, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnInFiltNSel1));

	Connect (CHANNEL0_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel0Sel));
	Connect (CHANNEL1_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel1Sel));
	Connect (CHANNEL2_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel2Sel));
	Connect (CHANNEL3_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel3Sel));
	Connect (CHANNEL4_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel4Sel));
	Connect (CHANNEL5_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel5Sel));
	Connect (CHANNEL6_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel6Sel));
	Connect (CHANNEL7_SEL, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlConfig::OnChannel7Sel));

	//connect spin ctrls
	Connect (GAIN, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler (ControlConfig::OnSpinGain));

}

ControlConfig::~ControlConfig()
{
}

void ControlConfig::OnCapSelect0 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnCapSelect0 value:  0"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CAPSELECT, 0);
}

void ControlConfig::OnCapSelect1 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnCapSelect1 value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CAPSELECT, 1);
}
void ControlConfig::OnCapSelect2 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnCapSelect1 value:  2"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CAPSELECT, 2);
}

void ControlConfig::OnResSelect0 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnResSelect0 value:  0"));
	xemDeviceCtrl->SetCommand (Command::LCMS_RES_SELECT, 0);
}

void ControlConfig::OnResSelect1 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnResSelect1 value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_RES_SELECT, 01);
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

void ControlConfig::OnBypassCDS0 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("BypassCDS0 value:  0"));
	xemDeviceCtrl->SetCommand (Command::LCMS_BYPASS_CDS, 0);
}

void ControlConfig::OnBypassCDS1 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("BypassCDS1 value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_BYPASS_CDS, 1);
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

void ControlConfig::OnInFiltNSel0 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("InFiltSelN value:  0"));
	xemDeviceCtrl->SetCommand (Command::LCMS_INFILTER_SELN, 0);
}

void ControlConfig::OnInFiltNSel1 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("InFiltSelN value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_INFILTER_SELN, 1);
}

void ControlConfig::OnChannel0Sel (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("OnChannel0Sel value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 0);
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


void ControlConfig::OnSpinGain (wxSpinEvent& evt)
{
	int value = spinGain->GetValue();
	xemDeviceCtrl->SetCommand (Command::LCMS_GAIN, value);
}

void ControlConfig::Reset (void)
{
//	float vMax = 3.3f;
//	float sliderMax = 65535.0f;


	bool defaultbypassPost=true;//disabled
	bool defaultPostGainSelect=true;//10

	bool defaultChannelSel=true;//ch0

	
	bool defaultresSelect=false; //no feedback resistor
	bool defaultbypassCDS=false; //bypass the cds
	bool defaultinFilt=true; //bypass the input filter

	int defaultGain = 100;

	//float defaultVthpV=1.8f;
	//float defaultVthmV=1.5f;

	//float defaultVthpV=1.68f;
	//float defaultVthmV=1.62f;

	//float defaultVoffsetB=0.0f;

//	int defaultVthp=(defaultVthpV/vMax)*sliderMax;
//	int defaultVthm=(defaultVthmV/vMax)*sliderMax;

	//int defaultCounterClock=2; //20 mhz

	capSelect0->SetValue (0);
	capSelect2->SetValue (0);
	capSelect1->SetValue (1);

	bypassPost0->SetValue (!defaultbypassPost);
	bypassPost1->SetValue (defaultbypassPost);

	postGainSelect0->SetValue (!defaultPostGainSelect);
	postGainSelect1->SetValue (defaultPostGainSelect);

	resSelect0->SetValue(!defaultresSelect);
	resSelect1->SetValue(defaultresSelect);
	
	bypassCDS0->SetValue(!defaultbypassCDS);
	bypassCDS1->SetValue(defaultbypassCDS);

	inFiltSelN0->SetValue(!defaultinFilt);
	inFiltSelN1->SetValue(defaultinFilt);

	channel0Sel->SetValue (defaultChannelSel);
	channel1Sel->SetValue (!defaultChannelSel);
	channel2Sel->SetValue (!defaultChannelSel);
	channel3Sel->SetValue (!defaultChannelSel);
	channel4Sel->SetValue (!defaultChannelSel);
	channel5Sel->SetValue (!defaultChannelSel);
	channel6Sel->SetValue (!defaultChannelSel);
	channel7Sel->SetValue (!defaultChannelSel);

	spinGain->SetValue (defaultGain);


//	char str[20] = "";
	xemDeviceCtrl->SetCommand (Command::LCMS_CAPSELECT, 1);
	xemDeviceCtrl->SetCommand (Command::LCMS_POSTGAINSELECT, defaultPostGainSelect);
	xemDeviceCtrl->SetCommand (Command::LCMS_BYPASSPOST, defaultbypassPost);
	xemDeviceCtrl->SetCommand (Command::LCMS_CHANNEL_SEL, 0);
	xemDeviceCtrl->SetCommand (Command::LCMS_INFILTER_SELN, 1);
	xemDeviceCtrl->SetCommand (Command::LCMS_GAIN, 100);
}

void ControlConfig::ResetControls (wxCommandEvent& evt)
{
	Reset();
}

void ControlConfig::SetGraphicsPlot (GraphicsPlot *a)
{
	graphicsPlot = a;
}
