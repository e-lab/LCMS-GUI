/**
 * @file
 * Contains the only class that interacts with the hardware.
 */
#include "DeviceInterface.h"
#include "DeviceEvent.h"

// When debugging changes all calls to "new" to be calls to "DEBUG_NEW" allowing for memory leaks to
// give you the file name and line number where it occurred.
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#define new DEBUG_NEW
#else
#define DEBUG_NEW new
#endif


DeviceInterface::DeviceInterface (wxEvtHandler* displayTmp) : wxThread (wxTHREAD_JOINABLE)
{
	pollDevice = false;

	display = displayTmp;

	rawDataInLength = 2;
	rawDataIn = new unsigned char[rawDataInLength];
	rawDataIn[0] = 0;
	rawDataIn[1] = 0;

	rawEvent = new DeviceEvent();

	xem = (okCFrontPanel*) NULL;
	pll = (okCPLL22393*) NULL;

#ifdef __WXMAC__
	Initialize (wxT ("LCMS.app/Contents/Resources/LCMS.bit"));
#else
	//Initialize (wxT ("LCMS.bit"));
	Initialize (wxT ("LCMS2012_xem6010_top.bit"));
#endif
}

DeviceInterface::~DeviceInterface()
{

	delete[] rawDataIn;
	rawDataIn = NULL;

	delete rawEvent;
	rawEvent = (DeviceEvent*) NULL;

	if (NULL != xem) {
		delete xem;
		xem = (okCFrontPanel*) NULL;
	}
	if (NULL != pll) {
		delete pll;
		pll = (okCPLL22393*) NULL;
	}
}

wxThread::ExitCode DeviceInterface::Entry()
{

	//while (!TestDestroy()) {
	while (1) {

		Command::packet packet;

		// Process commands
		while (wxMSGQUEUE_NO_ERROR == commandQueue.ReceiveNoWait (packet)) {
			ProcessCommand (packet);
		}

		// Transfer data to/from device
		if (pollDevice && (NULL != xem)) {
			TransferData();

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
		pll = (okCPLL22393*) NULL;
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
	/*if (xem->GetBoardModel() != okCFrontPanel::brdXEM3001v2) {
		delete xem;
		xem = (okCFrontPanel*) NULL;

		::wxMessageBox (wxT ("The XEM board was not of a type \"XEM3001v2\"\n"
		                     "and thus could not be initialized.\n"),
		                wxT ("Initialization Failed"));

		return (false);*/
	if (xem->GetBoardModel() != okCFrontPanel::brdXEM6010LX150) {
		delete xem;
		xem = (okCFrontPanel*) NULL;

		::wxMessageBox (wxT ("The XEM board was not of a type \"XEM6010\"\n"
		                     "and thus could not be initialized.\n"),
		                wxT ("Initialization Failed"));
		return (false);


	} else {
		pll = new okCPLL22393;

		// Set output 0 to 100 MHz and output1 to 40 MHz.
		pll->SetReference (48.0f);
		pll->SetPLLParameters (0, 400, 48); //48*400/48=400 MHz
		pll->SetOutputSource (0, okCPLL22393::ClkSrc_PLL0_0);
		pll->SetOutputDivider (0, 4); //48*400/48/4=100 MHz
		pll->SetOutputEnable (0, true);
		pll->SetOutputSource(1, okCPLL22393::ClkSrc_PLL0_0);
	    pll->SetOutputDivider(1, 20); //48*400/48/20 = 20 MHz
	    pll->SetOutputEnable(1, true);
		//::wxLogMessage(wxT("Pll frequence:  %f"), pll->GetOutputFrequency(0));
		//::wxLogMessage(wxT("Pll frequence:  %f"), pll->GetOutputFrequency(1));


		if (okCFrontPanel::NoError != xem->SetPLL22393Configuration (*pll)) {
			delete xem;
			xem = (okCFrontPanel*) NULL;
			delete pll;
			pll = (okCPLL22393*) NULL;
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
		pll = (okCPLL22393*) NULL;
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

	//xem->SetWireInValue (0x00, 1, 0x0001);  //no need for this
	//xem->UpdateWireIns();

	::wxLogMessage (wxT ("Start!"));
	//if this is the first time this function runs, clear all the buffers
	xem->SetWireInValue (0x01, 1, 0x0001); //send reset, then there will be some delay waiting for the usb
	xem->UpdateWireIns();
	xem->SetWireInValue (0x01, 0, 0x0001); //then clear the reset
	xem->UpdateWireIns();
	::wxLogMessage (wxT ("cleared all buffers\n"));

	pollDevice = true;

	xem->SetWireInValue (0x11, 0x1, 0x1); //say start
	xem->UpdateWireIns(); //say start
}

void DeviceInterface::Stop() {
	pollDevice = false;

	if (NULL == xem)
		return;

	xem->SetWireInValue (0x11, 0, 0x1); //say stop, it would be nice to wait for the end of the profile first
	xem->UpdateWireIns(); //say stop
	//might also need a reset

	::wxLogMessage (wxT ("Stop!"));
	//xem->SetWireInValue (0x00, 0, 0x0001);  //no need for this
	//xem->UpdateWireIns();
}

void DeviceInterface::SetCommand (Command::packet packet) {
	if (NULL == xem)
		return;

	rawEvent->SetVariable (packet.commandID,packet.commandValue);

	switch (packet.commandID) {

		case Command::PROFILE:
			delete[] rawDataIn;
			rawDataIn = packet.profile;
			rawDataInLength = packet.commandValue;
			break;
		case Command::SAVE_TYPE :
			rawEvent->SetVariable (packet.commandID, packet.commandValue);
			break;

			/////need to add all of the variables here...painfull --
			// ControlProtocol  --maybe the protocol shouldn't be set on the device interface level, this level should just have a pointer to the data so the send buffer routine has it
			/*
			case Command::LCMS_SELECTPROTOCOL :
				xem->UpdateWireIns();
				break;
			*/
			/*case Command::PC5_PEAK_1_HEIGHT :
				xem->UpdateWireIns();
				break;
			case Command::PC5_PEAK_2_HEIGHT :
				xem->UpdateWireIns();
				break;
			case Command::PC5_LEAD_TIME :
				xem->UpdateWireIns();
				break;
			case Command::PC5_INTERVAL :
				xem->UpdateWireIns();
				break;
			case Command::PC5_PEAK_1_DUR :
				xem->UpdateWireIns();
				break;
			case Command::PC5_PEAK_2_DUR :
				xem->UpdateWireIns();
				break;
			*/

			// ControlBiases
		case Command::LCMS_INT_GBT :
			xem->SetWireInValue (0x02, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_INT_VBN :
			xem->SetWireInValue (0x03, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_INT_VBP :
			xem->SetWireInValue (0x04, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_POST_GBT :
			xem->SetWireInValue (0x05, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_POST_VBN :
			xem->SetWireInValue (0x06, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_POST_VBP :
			xem->SetWireInValue (0x07, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_OBUFF_GBT :
			xem->SetWireInValue (0x08, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_OBUFF_VBN :
			xem->SetWireInValue (0x09, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_OBUFF_VBP :
			xem->SetWireInValue (0x0A, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_VREF :
			xem->SetWireInValue (0x0B, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();

			break;
			//case Command::PC5_V_CMD_OFFSET :   //this is controlled by the profile generator in software, not the fpga
			//	xem->SetWireInValue (0x0B, packet.commandValue, 0xFFFF);
			//	xem->UpdateWireIns();
			//	break;

			// ControlConfiguration
		case Command::LCMS_CAPSELECT :
			if (packet.commandValue == 0) {  //1 pf
				xem->SetWireInValue (0x01, 1<<5, 0x0020);
				xem->UpdateWireIns();
				xem->SetWireInValue (0x01, 1<<10, 0x0400);
				xem->UpdateWireIns();
			}
			else if (packet.commandValue == 1) { // 100 fF
				xem->SetWireInValue (0x01, 1<<5, 0x0020);
				xem->UpdateWireIns();
				xem->SetWireInValue (0x01, 0<<10, 0x0400);
				xem->UpdateWireIns();
			}
			else if (packet.commandValue == 2) {  //none
				xem->SetWireInValue (0x01, 0<<5, 0x0020);
				xem->UpdateWireIns();
				xem->SetWireInValue (0x01, 0<<10, 0x0400);
				xem->UpdateWireIns();
			}
			break;

		case Command::LCMS_RES_SELECT :
			xem->SetWireInValue(0x01, packet.commandValue<<11, 0x0800);
			xem->UpdateWireIns();
			break;

		case Command::LCMS_BYPASS_CDS :
			xem->SetWireInValue(0x01, packet.commandValue<<12, 0x1000);
			xem->UpdateWireIns();
			break;

		case Command::LCMS_INFILTER_SELN :
			xem->SetWireInValue(0x01, packet.commandValue<<13, 0x2000);
			xem->UpdateWireIns();
			break;


		case Command::LCMS_POSTGAINSELECT :
			xem->SetWireInValue (0x01, packet.commandValue<<6, 0x0040);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_BYPASSPOST :
			xem->SetWireInValue (0x01, packet.commandValue<<7, 0x0080);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_CHANNEL_SEL :
			switch (packet.commandValue) {
				case 0:
					xem->SetWireInValue	(0x01, 0x0000, 0x000E);
					break;
				case 1:
					xem->SetWireInValue (0x01, 0x0002, 0x000E);
					break;
				case 2:
					xem->SetWireInValue (0x01, 0x0004, 0x000E);
					break;
				case 3:
					xem->SetWireInValue (0x01, 0x0006, 0x000E);
					break;
				case 4:
					xem->SetWireInValue (0x01, 0x0008, 0x000E);
					break;
				case 5:
					xem->SetWireInValue (0x01, 0x000A, 0x000E);
					break;
				case 6:
					xem->SetWireInValue (0x01, 0x000C, 0x000E);
					break;
				case 7:
					xem->SetWireInValue (0x01, 0x000E, 0x000E);
					break;
				default:
					xem->SetWireInValue (0x01, 0x0000, 0x000E);
				break;
			}
			xem->UpdateWireIns();
			break;

			// ControlSyncConfig
		case Command::LCMS_BYPASS_LPF :
			xem->SetWireInValue (0x01, packet.commandValue<<8, 0x100);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_VOLTAGESAMPLINGRATE :
			xem->SetWireInValue (0x10, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_RESETPERIOD :
			xem->SetWireInValue (0x0D, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_INT_RESET_DURATION :
			xem->SetWireInValue (0x0E, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_POST_RESET_DURATION :
			xem->SetWireInValue (0x0F, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_CDS_TIME1 :
			xem->SetWireInValue (0x12, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_CDS_TIME2 :
			xem->SetWireInValue (0x13, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;
		case Command::LCMS_CDS_PULSE_WIDTH :
			xem->SetWireInValue (0x14, packet.commandValue, 0xFFFF);
			xem->UpdateWireIns();
			break;

		case Command::LCMS_MODE :
			if (packet.commandValue == 0) {  //V mode, for debugging = {mode,mode2}={0,0}
				xem->SetWireInValue (0x01, 0<<9, 0x0200);
				xem->UpdateWireIns();
				xem->SetWireInValue (0x01, 0<<14, 0x4000);
				xem->UpdateWireIns();
			}
			else if (packet.commandValue == 1) { //hw cds mode = {mode,mode2}={0,1}
				xem->SetWireInValue (0x01, 0<<9, 0x0200);
				xem->UpdateWireIns();
				xem->SetWireInValue (0x01, 1<<14, 0x4000);
				xem->UpdateWireIns();
			}
			else if (packet.commandValue == 2) {  //digital (sw) cds mode, = {mode,mode2}={1,0}
				xem->SetWireInValue (0x01, 1<<9, 0x0200);
				xem->UpdateWireIns();
				xem->SetWireInValue (0x01, 0<<14, 0x4000);
				xem->UpdateWireIns();
			}
			break;

			// Default case
		default :
			break;
	}
}

void DeviceInterface::ProcessCommand (Command::packet packet) {

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
			SetCommand (packet);
			break;
	}
}

void DeviceInterface::TransferData () {

	::wxLogMessage (wxT ("entered transferData()"));

	int rawDataInPtrLength = rawDataInLength;
	unsigned char* rawDataInPtr = &rawDataIn[0];

	while (1) {
		/* Send in data
		 *
		 * Read inAvailable (wireOut 0x20) which provides the current
		 * rx buffer size.  This size is the max # of data words that
		 * can currently be sent to device.  Send in no more then
		 * inAvailable data words via pipeIn (0x80).
		 */
		xem->UpdateWireOuts();
		int inAvailable = (int) (2 * xem->GetWireOutValue (0x20));

		::wxLogMessage (wxT ("free profile buffer size: %i"), inAvailable);
		if (inAvailable >= 2046) {
			::wxLogMessage (wxT ("profile buffer underrun! (ignore this if immediately after start)\n"));
		}

		if (inAvailable < rawDataInPtrLength) {

			// PipeIn transfer a chunk of the profile to device.
			xem->WriteToPipeIn (0x80, inAvailable, rawDataInPtr);
			rawDataInPtrLength = rawDataInPtrLength - inAvailable;
			rawDataInPtr = &rawDataInPtr[inAvailable];
		} else {

			Command::packet packet;
			if (wxMSGQUEUE_NO_ERROR == commandQueue.ReceiveNoWait (packet)) {
				// Since there's a command to process, send only the current profile.
				xem->WriteToPipeIn (0x80, rawDataInPtrLength, rawDataInPtr);
				rawDataInPtrLength = 0;

				// Collect last of the data from current profile.
				xem->UpdateWireOuts();
				int outAvailable = (int) (2 * xem->GetWireOutValue (0x21));
				while (outAvailable > 0) {
					unsigned char *rawData = new unsigned char[outAvailable];
					xem->ReadFromPipeOut (0xA0, outAvailable, rawData);
					rawEvent->SetRawData (rawData, outAvailable);
					wxPostEvent (display, (*rawEvent));

					xem->UpdateWireOuts();
					outAvailable = (int) (2 * xem->GetWireOutValue (0x21));
				}

				// Process command & break transfer.
				ProcessCommand (packet);
				break;
			} else {
				// With no command to process, rap round profile.
				unsigned char *rapProfile = new unsigned char[inAvailable];

				for (int xx = 0; xx < rawDataInPtrLength; xx++) {
					rapProfile[xx] = rawDataInPtr[xx];
				}

				int newProfileLength = inAvailable-rawDataInPtrLength;

				for (int xx = 0; xx < newProfileLength; xx++) {
					rapProfile[rawDataInPtrLength + xx] = rawDataIn[xx];
				}

				xem->WriteToPipeIn (0x80, inAvailable, &rapProfile[0]);
				rawDataInPtrLength = rawDataInLength - newProfileLength;
				rawDataInPtr = &rawDataIn[newProfileLength];

				delete[] rapProfile;
				rapProfile = NULL;
			}
		}


		// Poll the xem, asking if there is data to collect.
		xem->UpdateWireOuts();
		int outAvailable = (int) (2 * xem->GetWireOutValue (0x21));
		::wxLogMessage (wxT ("measurement data to collect: %i"), outAvailable/2);
		if (outAvailable >= 2038) { // not *2 here
			::wxLogMessage (wxT ("measurement buffer overrun!\n"));
		}
		if (outAvailable > 0) {
			unsigned char *rawData = new unsigned char[outAvailable];

			// PipeOut transfer
			xem->ReadFromPipeOut (0xA0, (int) outAvailable, rawData);

			// Raw data array from the pipe transfer is given to the rawEvent
			rawEvent->SetRawData (rawData, outAvailable);

			// Send a reference for the object rawEvent to display.
			wxPostEvent (display, (*rawEvent));
		}
	}
}
