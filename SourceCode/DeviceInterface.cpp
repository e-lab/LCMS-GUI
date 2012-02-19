/**
 * @file
 * Contains the only class that interacts with the hardware.
 */
#include "DeviceCommand.h"
#include "DeviceInterface.h"
#include "DeviceEvent.h"

DeviceInterface::DeviceInterface (wxEvtHandler* displayTmp) : wxTimer()
{
	controlPrevious = 0;

	display = displayTmp;

	rawEvent = new DeviceEvent();

	xem = (okCFrontPanel*) NULL;
	pll = (okCPLL22150*) NULL;

#ifdef __WXMAC__
	Initialize (wxT ("Template.app/Contents/Resources/Template.bit"));
#else
	Initialize (wxT ("Template.bit"));
#endif
}

DeviceInterface::~DeviceInterface()
{
	delete rawEvent;
	rawEvent = (DeviceEvent*) NULL;

	if (NULL != xem) {
		delete xem;
		xem = (okCFrontPanel*) NULL;
	}
	if (NULL != pll) {
		delete pll;
		pll = (okCPLL22150*) NULL;
	}
}

SimpleQueue<DeviceCommand>& DeviceInterface::GetQueue ()
{
	return commandQueue;
}

bool DeviceInterface::Initialize (wxString filename)
{
	Stop();

	if (NULL != xem) {
		delete xem;
		xem = (okCFrontPanel*) NULL;
	}
	if (NULL != pll) {
		delete pll;
		pll = (okCPLL22150*) NULL;
	}

	// Load the FrontPanel Library.
#ifdef __WXMAC__
	wxString libfilename = wxT ("./Template.app/Contents/Resources/libokFrontPanel.dylib");
	if (FALSE == okFrontPanelDLL_LoadLib ( (char*) libfilename.char_str())) {
#else
	if (FALSE == okFrontPanelDLL_LoadLib ( (char*) NULL)) {
#endif
		::wxMessageBox (wxT ("The FrontPanel library could not be loaded.  If in Windows,\n"
		                     "make certain the DLL is in the application start directory.\n"
		                     "If in OSX make certain that the dylib is in the Resources\n"
		                     "folder in the application .app folder"),
		                wxT ("Initialization Failed"));
		return false;
	}

	// Tries to open the first XEM
	xem = new okCFrontPanel;
	if (okCFrontPanel::NoError != xem->OpenBySerial()) {
		delete xem;
		xem = (okCFrontPanel*) NULL;
		::wxMessageBox (wxT ("A XEM board was not detected.\n"),
		                wxT ("Initialization Failed"));

		return (false);
	}

	// Try only for boards of type XEM3001v2
	if (xem->GetBoardModel() != okCFrontPanel::brdXEM3001v2) {
		delete xem;
		xem = (okCFrontPanel*) NULL;

		::wxMessageBox (wxT ("The XEM board was not of a type \"XEM3001v2\"\n"
		                     "and thus could not be initialized.\n"),
		                wxT ("Initialization Failed"));
		return (false);
	} else {
		pll = new okCPLL22150;

		// Set output 1 to 4 MHz.
		pll->SetReference (48.0f, false);
		pll->SetVCOParameters (200, 24);
		pll->SetDiv1 (okCPLL22150::DivSrc_VCO, 100);
		pll->SetOutputSource (1, okCPLL22150::ClkSrc_Div1ByN);
		pll->SetOutputEnable (1, true);

		//::wxLogMessage(wxT("Pll frequence:  %f"), pll->GetOutputFrequency(1));

		//pll->SetVCOParameters (6, 250);
		//pll->SetDiv1 (okCPLL22150::DivSrc_VCO, 128);
		//pll->SetOutputSource (1, okCPLL22150::ClkSrc_Div1ByN);
		//pll->SetOutputEnable (1, true);

		//pll->SetReference (48.0f, false);
		//pll->SetVCOParameters(400, 60);  // Setting the VCO (Voltage Controlled Oscillator) to 320MHz
		////pll->SetVCOParameters (100, 60); // Setting the VCO (Voltage Controlled Oscillator) to ....
		//pll->SetDiv1 (okCPLL22150::DivSrc_VCO, 5); // Set Divider1 to 5
		//pll->SetOutputSource (0, okCPLL22150::ClkSrc_Div1ByN); //  Set output 0 to be sourced from Divider1
		//pll->SetOutputEnable (0, true);

		if (okCFrontPanel::NoError != xem->SetPLL22150Configuration (*pll)) {
			delete xem;
			xem = (okCFrontPanel*) NULL;
			delete pll;
			pll = (okCPLL22150*) NULL;
			::wxMessageBox (wxT ("The PLL could not be set.\n"),
			                wxT ("Initialization Failed"));

			return (false);
		}
	}

	// Upload the configuration file.
	if (okCFrontPanel::NoError != xem->ConfigureFPGA ( (const char *) filename.char_str())) {
		delete xem;
		xem = (okCFrontPanel*) NULL;
		delete pll;
		pll = (okCPLL22150*) NULL;
		::wxMessageBox (wxT ("An error occurred when trying to loading the XEM\n"
		                     "configuration (bitstream) bit file."),
		                wxT ("Initialization Failed"));


		return (false);
	}
	//::wxLogMessage(wxT("FPGA configuration complete."));
	return (true);

}

void DeviceInterface::Notify() {

	// Poll the xem, asking if there is data to collect.
	xem->UpdateWireOuts();
	unsigned long control = xem->GetWireOutValue (0x20);

	if ( (controlPrevious & 0x0003) != (control & 0x0003)) { // True when there is data to collect

		rawEvent->SetVariable (CommandVariable::IMG_WIDTH, 80);
		rawEvent->SetVariable (CommandVariable::IMG_HEIGHT, 60);

		// Read the length value from wire 0x21;
		int length = (int) (xem->GetWireOutValue (0x21) * 2);
		unsigned char* rawData = new unsigned char[length];

		//::wxLogMessage (wxT ("length:  %i"), length);
		//::wxLogMessage(wxT("debug:  %i"), ((control & 0x7FFC) >> 2));
		//::wxLogMessage(wxT("debug test:   %i%i%i%i"), ((control & 0x20) >> 5), ((control & 0x10) >> 4), ((control & 0x8) >> 3), ((control & 0x4) >> 2));
		//::wxLogMessage (wxT ("debug full:   %i"), ( (control & 0x0004) >> 2));
		//::wxLogMessage (wxT ("debug empty:  %i"), ( (control & 0x0008) >> 3));
		//::wxLogMessage (wxT ("debug control:  %i"), ( (control & 0x0003)));

		// Pipe transfer
		xem->ReadFromPipeOut (0xA1, length, rawData);

		// The array of raw data from the pipe transfer is given to the rawEvent
		rawEvent->SetRawData (rawData, length);

		// Send a reference for the object rawEvent to display.
		wxPostEvent (display, (*rawEvent));

		//controlPrevious = control;
		//Stop();
	}
	controlPrevious = control;

	//::wxLogMessage(wxT("End of Notify()  control:  %i"), (control & 0x01));
}

void DeviceInterface::Start() {
	if (NULL == xem)
		return;

	xem->SetWireInValue (0x00, 1, 0x0001);
	xem->UpdateWireIns();

	int milliseconds = 1;
	wxTimer::Start (milliseconds);
}

void DeviceInterface::Stop() {
	if (NULL == xem)
		return;

	xem->SetWireInValue (0x00, 0, 0x0001);
	xem->UpdateWireIns();

	wxTimer::Stop();
}

void DeviceInterface::SetCommand (CommandVariable::CommandID command, int value) {
	if (NULL == xem)
		return;

	switch (command) {
		case CommandVariable::OCT_PD_BIAS :
			xem->SetWireInValue (0x01, value, 0x00FF);
			xem->UpdateWireIns();
			break;
		case CommandVariable::OCT_VDDA :
			xem->SetWireInValue (0x02, value, 0x00FF);
			xem->UpdateWireIns();
			break;
		case CommandVariable::OCT_VDDR :
			xem->SetWireInValue (0x03, value, 0x00FF);
			xem->UpdateWireIns();
			break;
		case CommandVariable::OCT_LPU :
			xem->SetWireInValue (0x04, value, 0x00FF);
			xem->UpdateWireIns();
			break;
		case CommandVariable::OCT_EVENT_NUM :
			xem->SetWireInValue (0x05, value, 0xFFFF);
			xem->UpdateWireIns();
			//::wxLogMessage (wxT ("Number of events %i"), value);
			break;
		case CommandVariable::OCT_TIME_OUT:
			xem->SetWireInValue (0x06, value, 0xFFFF);
			//xem->SetWireInValue (0x07, (pll->GetOutputFrequency (1) * 1000), 0xFFFF);  // Milliseconds
			xem->SetWireInValue (0x07, (pll->GetOutputFrequency (1)), 0xFFFF); // Microseconds
			xem->UpdateWireIns();
			//::wxLogMessage (wxT ("Time Out %i"), value);
			break;
		case CommandVariable::SAVE_TYPE :
			rawEvent->SetVariable (command, value);
			break;
		default :
			break;
	}
}
