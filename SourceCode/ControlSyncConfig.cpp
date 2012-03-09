/**
 * @file
 * Contains the class that constructs the bias controls.
 */
#include "ControlSyncConfig.h"
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

ControlSyncConfig::ControlSyncConfig (wxWindow* owner, DeviceController* controller) : ControlPanel (owner)
{
	xemDeviceCtrl =			controller;

	minValueSlider =		0;
	maxValueSlider =		65535; //16 bits
	maxVoltageSlider =		3.3f;

	wxSizer* sizerConfig = new wxBoxSizer (wxVERTICAL);

	//Create Box for Low Pass Filter (LPF)
	wxStaticBoxSizer* sizerBox1= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Low Pass Filter:"));
	wxFlexGridSizer* sizerBox1Inner= new wxFlexGridSizer (2,2,10);
	//create BYPASS_LPF Radio Buttons
	bypass_lpf0 = new wxRadioButton (this, BYPASS_LPF0, wxT ("Enable LPF"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	bypass_lpf1 = new wxRadioButton (this, BYPASS_LPF1, wxT ("Bypass LPF"));
	sizerBox1Inner->Add (bypass_lpf0);
	sizerBox1Inner->Add (bypass_lpf1);
	sizerBox1->Add (sizerBox1Inner);
	sizerConfig->Add (sizerBox1, 0, wxEXPAND | wxALL | wxALIGN_CENTER, 10);


	wxStaticBoxSizer* sizerBox2= new wxStaticBoxSizer (wxVERTICAL, this, wxT ("Sampling/Reset P&D in usec:"));
	//create choice for Voltage Mode Sampling Rate (independant of integrator/post-amp reset frequency)
	wxBoxSizer* row = new wxBoxSizer (wxHORIZONTAL);
	wxArrayString stringsVoltageSamplingRate;
	stringsVoltageSamplingRate.Add (wxT ("100Hz (10ms)"));
	stringsVoltageSamplingRate.Add (wxT ("500Hz (2ms)"));
	stringsVoltageSamplingRate.Add (wxT ("1KHz  (1ms)"));
	stringsVoltageSamplingRate.Add (wxT ("5KHz  (200us)"));
	stringsVoltageSamplingRate.Add (wxT ("10KHz (100us)"));
	stringsVoltageSamplingRate.Add (wxT ("20KHz (50us)"));
	stringsVoltageSamplingRate.Add (wxT ("25KHz (40us)"));
	stringsVoltageSamplingRate.Add (wxT ("33KHz (30.303us)"));
	stringsVoltageSamplingRate.Add (wxT ("40KHz (25us)"));
	stringsVoltageSamplingRate.Add (wxT ("50KHz (20us)"));
	stringsVoltageSamplingRate.Add (wxT ("100KHz (10us)"));
	choiceVoltageSamplingRate = new wxChoice (this, VOLTAGESAMPLINGRATE, wxDefaultPosition, wxDefaultSize, stringsVoltageSamplingRate);
	row->Add (new wxStaticText (this, -1, wxT ("Voltage Sampling Rate"), wxDefaultPosition, wxSize (120, 20), wxALIGN_LEFT));
	row->Add (choiceVoltageSamplingRate, 0, wxTOP, 5);
	sizerBox2->Add (row);
	//create choice for ResetPeriod
	row = new wxBoxSizer (wxHORIZONTAL);
	wxArrayString stringsResetPeriod;
	stringsResetPeriod.Add (wxT ("100Hz (10ms)"));
	stringsResetPeriod.Add (wxT ("500Hz (2ms)"));
	stringsResetPeriod.Add (wxT ("1KHz  (1ms)"));
	stringsResetPeriod.Add (wxT ("5KHz  (200us)"));
	stringsResetPeriod.Add (wxT ("10KHz (100us)"));
	stringsResetPeriod.Add (wxT ("20KHz (50us)"));
	stringsResetPeriod.Add (wxT ("25KHz (40us)"));
	stringsResetPeriod.Add (wxT ("33KHz (30.303us)"));
	stringsResetPeriod.Add (wxT ("40KHz (25us)"));
	stringsResetPeriod.Add (wxT ("50KHz (20us)"));
	stringsResetPeriod.Add (wxT ("100KHz (10us)"));
	choiceResetPeriod = new wxChoice (this, RESETPERIOD, wxDefaultPosition, wxDefaultSize, stringsResetPeriod);
	row->Add (new wxStaticText (this, -1, wxT ("Reset Period"), wxDefaultPosition, wxSize (120, 20), wxALIGN_LEFT));
	row->Add (choiceResetPeriod, 0, wxTOP, 5);
	sizerBox2->Add (row);
	//create choice for ResetIntDuration
	row = new wxBoxSizer (wxHORIZONTAL);
	wxArrayString stringsResetDuration;
	stringsResetDuration.Add (wxT ("1us"));
	stringsResetDuration.Add (wxT ("2us"));
	stringsResetDuration.Add (wxT ("3us"));
	stringsResetDuration.Add (wxT ("7us"));
	stringsResetDuration.Add (wxT ("10us"));
	stringsResetDuration.Add (wxT ("20us"));
	stringsResetDuration.Add (wxT ("30us"));
	stringsResetDuration.Add (wxT ("40us"));
	stringsResetDuration.Add (wxT ("50us"));
	stringsResetDuration.Add (wxT ("Always Reset"));
	choiceIntResetDuration = new wxChoice (this, INTRESETDURATION, wxDefaultPosition, wxDefaultSize, stringsResetDuration);
	row->Add (new wxStaticText (this, -1, wxT ("Int Reset Duration"), wxDefaultPosition, wxSize (120, 20), wxALIGN_LEFT));
	row->Add (choiceIntResetDuration, 0, wxTOP, 5);
	sizerBox2->Add (row);
	//create choice for ResetPostDuration
	row = new wxBoxSizer (wxHORIZONTAL);
	wxArrayString stringsResetPostDuration;
	stringsResetPostDuration.Add (wxT ("1us"));
	stringsResetPostDuration.Add (wxT ("2us"));
	stringsResetPostDuration.Add (wxT ("3us"));
	stringsResetPostDuration.Add (wxT ("4us"));
	stringsResetPostDuration.Add (wxT ("5us"));
	stringsResetPostDuration.Add (wxT ("Always Reset"));
	choicePostResetDuration = new wxChoice (this, POSTRESETDURATION, wxDefaultPosition, wxDefaultSize, stringsResetPostDuration);
	row->Add (new wxStaticText (this, -1, wxT ("Post Reset Duration"), wxDefaultPosition, wxSize (120, 20), wxALIGN_LEFT));
	row->Add (choicePostResetDuration, 0, wxTOP, 5);
	sizerBox2->Add (row);


	//create slider for CDS Time1
	row = new wxBoxSizer (wxHORIZONTAL);
	wxArrayString stringsCDSTime;

	//these values are needed for the filter, add them as appropriate to the list above
	stringsCDSTime.Add (wxT ("2us"));
	stringsCDSTime.Add (wxT ("30us"));
	stringsCDSTime.Add (wxT ("40us"));
	stringsCDSTime.Add (wxT ("45us"));
	stringsCDSTime.Add (wxT ("50us"));
	stringsCDSTime.Add (wxT ("60us"));
	stringsCDSTime.Add (wxT ("90us"));
	stringsCDSTime.Add (wxT ("190us"));
	stringsCDSTime.Add (wxT ("220us"));
	stringsCDSTime.Add (wxT ("940us"));
	stringsCDSTime.Add (wxT ("990us"));

	choiceCDSTime1 = new wxChoice (this, CDSTIME1, wxDefaultPosition, wxDefaultSize, stringsCDSTime);
	row->Add (new wxStaticText (this, -1, wxT ("CDSTime1 Delay"), wxDefaultPosition, wxSize (120, 20), wxALIGN_LEFT));
	row->Add (choiceCDSTime1, 0, wxTOP, 5);
	sizerBox2->Add (row);

	//create slider for CDS Time2
	row = new wxBoxSizer (wxHORIZONTAL);
	wxArrayString stringsCDSTime2;

	//these values are needed for the filter, add them as appropriate to the list above
	stringsCDSTime2.Add (wxT ("9990us"));
	stringsCDSTime2.Add (wxT ("1990us"));
	stringsCDSTime2.Add (wxT ("990us"));
	stringsCDSTime2.Add (wxT ("190us"));
	stringsCDSTime2.Add (wxT ("90us"));
	stringsCDSTime2.Add (wxT ("40us"));
	stringsCDSTime2.Add (wxT ("30us"));
	stringsCDSTime2.Add (wxT ("20us"));
	stringsCDSTime2.Add (wxT ("15us"));
	stringsCDSTime2.Add (wxT ("10us"));
	stringsCDSTime2.Add (wxT ("5xus"));

	choiceCDSTime2 = new wxChoice (this, CDSTIME2, wxDefaultPosition, wxDefaultSize, stringsCDSTime2);
	row->Add (new wxStaticText (this, -1, wxT ("CDSTime2 Delay"), wxDefaultPosition, wxSize (120, 20), wxALIGN_LEFT));
	row->Add (choiceCDSTime2, 0, wxTOP, 5);
	sizerBox2->Add (row);
	sizerConfig->Add (sizerBox2, 0, wxEXPAND | wxALL | wxALIGN_CENTER, 10);

	//Create reset button
	wxButton* buttonReset =  new wxButton (this, RESET_SYNC_CONFIG, wxT ("Reset"), wxDefaultPosition, wxDefaultSize, 0);
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (buttonReset, 0, wxALIGN_CENTER);
	sizerConfig->Add (row, 0, wxTOP | wxBOTTOM | wxALIGN_CENTER, 10);

	this->SetSizerAndFit (sizerConfig);

	//set controls to defaults when it starts
	Reset();

	// Connect events to event handlers
	//connect reset button
	Connect (RESET_SYNC_CONFIG, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (ControlSyncConfig::ResetControls));

	//connect radio buttons
	Connect (BYPASS_LPF0, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlSyncConfig::OnBypass_lpf0));
	Connect (BYPASS_LPF1, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (ControlSyncConfig::OnBypass_lpf1));

	//connect choices
	Connect (VOLTAGESAMPLINGRATE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler (ControlSyncConfig::OnChoiceVoltageSamplingRate));
	Connect (RESETPERIOD, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler (ControlSyncConfig::OnChoiceResetPeriod));
	Connect (INTRESETDURATION, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler (ControlSyncConfig::OnChoiceIntResetDuration));
	Connect (POSTRESETDURATION, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler (ControlSyncConfig::OnChoicePostResetDuration));
	Connect (CDSTIME1, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler (ControlSyncConfig::OnChoiceCDSTime1));
	Connect (CDSTIME2, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler (ControlSyncConfig::OnChoiceCDSTime2));

}

ControlSyncConfig::~ControlSyncConfig()
{
}

void ControlSyncConfig::OnBypass_lpf0 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("bypass_lpf0 value:  0"));
	xemDeviceCtrl->SetCommand (Command::LCMS_BYPASS_LPF, 0);
}
void ControlSyncConfig::OnBypass_lpf1 (wxCommandEvent& evt)
{
	::wxLogMessage (wxT ("bypass_lpf1 value:  1"));
	xemDeviceCtrl->SetCommand (Command::LCMS_BYPASS_LPF, 1);
}

//void ControlSyncConfig::OnSliderPBias_LPF (wxScrollEvent& evt)
//{
//	::wxLogMessage (wxT ("sliderPBias_LPF value:  %i"), evt.GetPosition());
//	xemDeviceCtrl->SetCommand (Command::LCMS_PBIAS_LPF, evt.GetPosition());
//	float voltage = ( (float) sliderPBias_LPF->GetValue()) / ( (float) maxValueSlider) * maxVoltageSlider;
//	// Convert float to string
//	char str[20] = "";
//	sprintf (str, "%f", voltage);
//	textPBias_LPF->SetLabel (wxString (str, wxConvUTF8));
//}

void ControlSyncConfig::OnChoiceVoltageSamplingRate (wxCommandEvent& evt)
{
	int in_value = evt.GetSelection();
	::wxLogMessage (wxT ("OnChoiceVoltageSamplingRate in value:  %i"), in_value);
	int out_value;

	switch (in_value) {
		case 0: //100 Hz
			//cannot do this by methods of other cases due to size
			out_value = 10000;  //10000us
			break;
		case 1: //500Hz
			out_value=2000;
			break;
		case 2: //1KHz
			out_value = 1/1E3*1E6;  //1000us
			break;
		case 3: //5KHz
			out_value = 1/5E3*1E6;  //200us
			break;
		case 4: //10KHz
			out_value = 1/10E3*1E6; //100us
			break;
		case 5: //20KHz
			out_value=1/20E3*1E6;
			break;
		case 6: //25KHz
			out_value=1/25E3*1E6;
			break;
		case 7: //33KHz
			out_value=1/33E3*1E6;
			break;
		case 8: //40KHz
			out_value=1/40E3*1E6;
			break;
		case 9: //50KHz
			out_value=1/50E3*1E6;
			break;
		case 10: //100KHz
			out_value = 1/100E3*1E6; //10us
			break;
		default:  //use 10khz as default value
			out_value = 1/10E3*1E6; //100us
			break;
	}
	::wxLogMessage (wxT ("OnChoiceVoltageSamplingRate out value:  %i"), out_value);
	xemDeviceCtrl->SetCommand (Command::LCMS_VOLTAGESAMPLINGRATE,out_value);
	xemDeviceCtrl->BuildProfile(xemDeviceCtrl->GetVariable(Command::PC5_LEAD_TIME),xemDeviceCtrl->GetVariable(Command::PC5_PEAK_1_DUR),xemDeviceCtrl->GetVariable(Command::PC5_PEAK_1_HEIGHT),xemDeviceCtrl->GetVariable(Command::PC5_PEAK_2_DUR),xemDeviceCtrl->GetVariable(Command::PC5_PEAK_2_HEIGHT),xemDeviceCtrl->GetVariable(Command::PC5_INTERVAL),xemDeviceCtrl->GetVariable(Command::PC5_V_CMD_OFFSET),xemDeviceCtrl->GetVariable(Command::LCMS_VOLTAGESAMPLINGRATE)); //this causes some problems because the profile update and the new sampling rate aren't sent at the same time, maybe put a stop first
}


void ControlSyncConfig::OnChoiceResetPeriod (wxCommandEvent& evt)
{
	int in_value = evt.GetSelection();
	::wxLogMessage (wxT ("OnChoiceResetPeriod in value:  %i"), in_value);
	int out_value;

	switch (in_value) {
		case 0: //100 Hz
			//cannot do this by methods of other cases due to size
			out_value = 10000;  //10000us
			break;
		case 1: //500Hz
			out_value=2000;
			break;
		case 2: //1KHz
			out_value = 1/1E3*1E6;  //1000us
			break;
		case 3: //5KHz
			out_value = 1/5E3*1E6;  //200us
			break;
		case 4: //10KHz
			out_value = 1/10E3*1E6; //100us
			break;
		case 5: //20KHz
			out_value=1/20E3*1E6;
			break;
		case 6: //25KHz
			out_value=1/25E3*1E6;
			break;
		case 7: //33KHz
			out_value=1/33E3*1E6;
			break;
		case 8: //40KHz
			out_value=1/40E3*1E6;
			break;
		case 9: //50KHz
			out_value=1/50E3*1E6;
			break;
		case 10: //100KHz
			out_value = 1/100E3*1E6; //10us
			break;
		default:  //use 10khz as default value
			out_value = 1/10E3*1E6; //100us
			break;
	}
	::wxLogMessage (wxT ("OnChoiceResetPeriod out value:  %i"), out_value);
	xemDeviceCtrl->SetCommand (Command::LCMS_RESETPERIOD,out_value);
}

void ControlSyncConfig::OnChoiceIntResetDuration (wxCommandEvent& evt)
{
	int in_value = evt.GetSelection();
	::wxLogMessage (wxT ("OnChoiceIntResetDuration in value:  %i"), in_value);
	int out_value;

	switch (in_value) {
		case 0: //1
			out_value = 1;  //1us
			break;
		case 1: //2
			out_value = 2;  //2us
			break;
		case 2: //3
			out_value = 3; //3us
			break;
		case 3: //7
			out_value = 7; //7us
			break;
		case 4: //10
			out_value = 10; //10us
			break;
		case 5: //20
			out_value = 20; //20us
			break;
		case 6: //30
			out_value = 30; //30us
			break;
		case 7: //40
			out_value = 40; //40us
			break;
		case 8: //50
			out_value = 50; //50us
			break;
		case 9: //always reset
			out_value = 0; //the verilog is setup to interpret 0 as always reset
			break;
		default:  //use 2us as default value
			out_value = 2; //2us
			break;
	}
	::wxLogMessage (wxT ("OnChoiceIntResetDuration out value:  %i"), out_value);
	xemDeviceCtrl->SetCommand (Command::LCMS_INT_RESET_DURATION,out_value);
}

void ControlSyncConfig::OnChoicePostResetDuration (wxCommandEvent& evt)
{
	int in_value = evt.GetSelection();
	::wxLogMessage (wxT ("OnChoicePostResetDuration in value:  %i"), in_value);
	int out_value;

	switch (in_value) {
		case 0: //1
			out_value = 1;  //1us
			break;
		case 1: //2
			out_value = 2;  //2us
			break;
		case 2: //3
			out_value = 3; //3us
			break;
		case 3: //7
			out_value = 4; //4us
			break;
		case 4: //10
			out_value = 5; //5us
			break;
		case 5: //always reset
			out_value = 0; //the verilog is setup to interpret 0 as always reset
			break;
		default:  //use 2us as default value
			out_value = 2; //2us
			break;
	}
	::wxLogMessage (wxT ("OnChoicePostResetDuration out value:  %i"), out_value);
	xemDeviceCtrl->SetCommand (Command::LCMS_POST_RESET_DURATION,out_value);
}


void ControlSyncConfig::OnChoiceCDSTime1 (wxCommandEvent& evt)
{
	int in_value = evt.GetSelection();
	::wxLogMessage (wxT ("OnChoiceCDS1Time in value:  %i"), in_value);
	int out_value;

	switch (in_value) {
		case 0: //2us
			out_value = 2;
			break;
		case 1: //30us
			out_value = 30;
			break;
		case 2: //40us
			out_value = 40;
			break;
		case 3: //45us
			out_value = 45;
			break;
		case 4: //50us
			out_value = 50;
			break;
		case 5: //60us
			out_value = 60;
			break;
		case 6: //90us
			out_value = 90;
			break;
		case 7: //190us
			out_value = 190;
			break;
		case 8: //220us
			out_value = 220;
			break;
		case 9: //940us
			out_value = 940;
			break;
		case 10: //990us
			out_value = 990;
			break;
		default:  //use 2us as default value
			out_value = 2;
			break;

			//divide number of ns by 25 to get the appropriate value to send to the VHDL
			/*
			case 0: //2us
				out_value = 2000/25;
				break;
			case 1: //30us
				out_value = 30000/25;
				break;
			case 2: //40us
				out_value = 40000/25;
				break;
			case 3: //45us
				out_value = 45000/25;
				break;
			case 4: //50us
				out_value = 50000/25;
				break;
			case 5: //60us
				out_value = 60000/25;
				break;
			case 6: //90us
				out_value = 90000/25;
				break;
			case 7: //190us
				out_value = 190000/25;
				break;
			case 8: //220us
				out_value = 220000/25;
				break;
			case 9: //940us
				out_value = 940000/25;
				break;
			case 10: //990us
				out_value = 990000/25;
				break;
			default:  //use 2us as default value
				out_value = 2000/25;
				break;
			*/
	}
	::wxLogMessage (wxT ("OnChoiceCDS1Time out value:  %i"), out_value);
	xemDeviceCtrl->SetCommand (Command::LCMS_CDS_TIME1,out_value);
}
void ControlSyncConfig::OnChoiceCDSTime2 (wxCommandEvent& evt)
{
	int in_value = evt.GetSelection();
	::wxLogMessage (wxT ("OnChoiceCDS2Time in value:  %i"), in_value);
	int out_value;

	switch (in_value) {
			//divide number of ns by 25 to get the appropriate value to send to the VHDL
		case 0:
			out_value = 99990;
			break;
		case 1:
			out_value = 1990;
			break;
		case 2:
			out_value = 990;
			break;
		case 3:
			out_value = 190;
			break;
		case 4:
			out_value = 90;
			break;
		case 5:
			out_value = 40;
			break;
		case 6:
			out_value = 30;
			break;
		case 7:
			out_value = 20;
			break;
		case 8:
			out_value = 15;
			break;
		case 9:
			out_value = 10;
			break;
		case 10:
			out_value = 5;
			break;
		default:  //use 90us as default value
			out_value = 90;
			break;
	}

	::wxLogMessage (wxT ("OnChoiceCDS2Time out value:  %i"), out_value);
	xemDeviceCtrl->SetCommand (Command::LCMS_CDS_TIME2,out_value);
}

void ControlSyncConfig::Reset (void)
{
	//float vMax = 3.3f;
	//float sliderMax = 65535.0f;

	bool defaultbypass_lpf=true;//bypass
	//float defaultVoffsetB=0.0f;

	int defaultVoltageSamplingRate=100; //100 us, 10kHz
	int defaultResetPeriod=100; //100us
	int defaultIntResetDuration = 2;
	int defaultPostResetDuration= 2;
	int defaultCDSTime1=2;
	int defaultCDSTime2=90;

	bypass_lpf0->SetValue (!defaultbypass_lpf);
	bypass_lpf1->SetValue (defaultbypass_lpf);

	choiceVoltageSamplingRate->SetStringSelection (wxT ("10KHz (100us)"));
	choiceResetPeriod->SetStringSelection (wxT ("10KHz (100us)"));
	choiceIntResetDuration->SetStringSelection (wxT ("2us"));
	choicePostResetDuration->SetStringSelection (wxT ("2us"));


	choiceCDSTime1->SetStringSelection (wxT ("2us"));
	choiceCDSTime2->SetStringSelection (wxT ("90us"));

	//char str[20] = "";

	xemDeviceCtrl->SetCommand (Command::LCMS_BYPASS_LPF, defaultbypass_lpf);
	xemDeviceCtrl->SetCommand (Command::LCMS_VOLTAGESAMPLINGRATE, defaultVoltageSamplingRate);
	xemDeviceCtrl->SetCommand (Command::LCMS_RESETPERIOD, defaultResetPeriod);
	xemDeviceCtrl->SetCommand (Command::LCMS_INT_RESET_DURATION, defaultIntResetDuration);
	xemDeviceCtrl->SetCommand (Command::LCMS_POST_RESET_DURATION, defaultPostResetDuration);
	xemDeviceCtrl->SetCommand (Command::LCMS_CDS_TIME1, defaultCDSTime1);
	xemDeviceCtrl->SetCommand (Command::LCMS_CDS_TIME2, defaultCDSTime2);
}


void ControlSyncConfig::ResetControls (wxCommandEvent& evt)
{
	Reset();
}
