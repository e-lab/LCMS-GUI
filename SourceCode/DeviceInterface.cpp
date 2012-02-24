/**
 * @file
 * Contains the only class that interacts with the hardware.
 */
#include "DeviceInterface.h"
#include "DeviceEvent.h"

DeviceInterface::DeviceInterface (wxEvtHandler* displayTmp) : wxThread (wxTHREAD_JOINABLE)
{
	pollDevice = false;

	display = displayTmp;

	rawEvent = new DeviceEvent();

	xem = (okCFrontPanel*) NULL;
	pll = (okCPLL22150*) NULL;

#ifdef __WXMAC__
	Initialize (wxT ("LCMS.app/Contents/Resources/LCMS.bit"));
#else
	Initialize (wxT ("LCMS.bit"));
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

wxThread::ExitCode DeviceInterface::Entry()
{

	//while (!TestDestroy()) {
	while (1) {

		Command::packet packet;

		// Process commands
		if (wxMSGQUEUE_NO_ERROR == commandQueue.ReceiveNoWait (packet)) {

			switch (packet.commandID) {
				case Command::DEV_INIT :
					Initialize (packet.filename);
					break;
				case Command::DEV_START :
					Start();
					break;
				case Command::DEV_STOP:
					Stop();
					break;
				default :
					SetCommand (packet.commandID, packet.commandValue);
					break;
			}
		}

		// Get data from device
		if (pollDevice && (NULL != xem)) {

			// Poll the xem, asking if there is data to collect.
			xem->UpdateWireOuts();
			int length = (int) (2 * xem->GetWireOutValue (0x21));

			if (length > 0) {
				unsigned char *rawData = new unsigned char[length];

				// PipeOut transfer
				xem->ReadFromPipeOut (0xA0, (int) length, rawData);

				// Raw data array from the pipe transfer is given to the rawEvent
				rawEvent->SetRawData (rawData, length);

				// Send a reference for the object rawEvent to display.
				wxPostEvent (display, (*rawEvent));
			}
		}
	}
	return (wxThread::ExitCode) 0;
}


SimpleQueue<Command::packet>& DeviceInterface::GetQueue ()
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
	wxString libfilename = wxT ("./LCMS.app/Contents/Resources/libokFrontPanel.dylib");
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
	return (true);

}

void DeviceInterface::Start() {
	if (NULL == xem)
		return;

	xem->SetWireInValue (0x00, 1, 0x0001);
	xem->UpdateWireIns();

	pollDevice = true;
}

void DeviceInterface::Stop() {
	pollDevice = false;

	if (NULL == xem)
		return;

	xem->SetWireInValue (0x00, 0, 0x0001);
	xem->UpdateWireIns();
}

void DeviceInterface::SetCommand (Command::CommandID commandID, int value) {
	if (NULL == xem)
		return;

	switch (commandID) {
		case Command::OCT_PD_BIAS :
			xem->SetWireInValue (0x01, value, 0x00FF);
			xem->UpdateWireIns();
			break;
		case Command::OCT_VDDA :
			xem->SetWireInValue (0x02, value, 0x00FF);
			xem->UpdateWireIns();
			break;
		case Command::OCT_VDDR :
			xem->SetWireInValue (0x03, value, 0x00FF);
			xem->UpdateWireIns();
			break;
		case Command::OCT_LPU :
			xem->SetWireInValue (0x04, value, 0x00FF);
			xem->UpdateWireIns();
			break;
		case Command::OCT_EVENT_NUM :
			xem->SetWireInValue (0x05, value, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::OCT_TIME_OUT:
			xem->SetWireInValue (0x06, value, 0xFFFF);
			xem->SetWireInValue (0x07, (pll->GetOutputFrequency (1)), 0xFFFF); // Microseconds
			xem->UpdateWireIns();
			break;
		case Command::SAVE_TYPE :
			rawEvent->SetVariable (commandID, value);
			break;
		default :
			break;
	}
}
