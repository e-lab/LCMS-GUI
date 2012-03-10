/**
 * @file
 * Contains a class that is shared between all ControlPanels and the Graphics
 * system, it is through this class that all control values are passes to the
 * DeviceInterface. It is also a convenient place to share data between
 * ControlPanels.
 */
#include "DeviceController.h"
#include "DeviceInterface.h"
#include "Command.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#define new DEBUG_NEW
#else
#define DEBUG_NEW new
#endif

DeviceController::DeviceController (wxEvtHandler* display)
{
	variables = new std::map<int, int>();

	xemDevice = new DeviceInterface (display);
	xemDevice->Create();
	xemDevice->Run();
}

DeviceController::~DeviceController()
{
	//xemDevice->Wait();
	delete xemDevice;
	xemDevice = (DeviceInterface*) NULL;

	delete variables;
	variables = NULL;
}

void DeviceController::BuildProfile (int PC5_LEAD_TIME, int PC5_PEAK_1_DUR, int PC5_PEAK_1_HEIGHT, int PC5_PEAK_2_DUR, int PC5_PEAK_2_HEIGHT, int PC5_INTERVAL, int PC5_V_CMD_OFFSET, int LCMS_VOLTAGESAMPLINGRATE)
{
	double scaling = ( (double) (1000.0 / LCMS_VOLTAGESAMPLINGRATE));
	float *profile_array;
	int length = PC5_INTERVAL * scaling;
	profile_array = new float[length];

	int lead_time = PC5_LEAD_TIME * scaling;
	float p0_height = PC5_V_CMD_OFFSET;
	for (int xx = 0; xx < lead_time; xx++) {
		profile_array[xx] = p0_height;
	}

	int p1_dur = PC5_PEAK_1_DUR * scaling;
	int p1_height = PC5_PEAK_1_HEIGHT;
	for (int xx = lead_time; xx < (lead_time + p1_dur); xx++) {
		profile_array[xx] = (float) p0_height + (float) p1_height;
	}

	int p2_dur = PC5_PEAK_2_DUR * scaling;
	int p2_height = PC5_PEAK_2_HEIGHT;
	for (int xx = (lead_time + p1_dur); xx < (lead_time + p1_dur + p2_dur); xx++) {
		profile_array[xx] = (float) p0_height + (float) p2_height;
	}

	int p3_height = p0_height;
	for (int xx = (lead_time + p1_dur + p2_dur); xx < length; xx++) {
		profile_array[xx] = (float) p3_height;
	}

	long int transfer_profile_length =length;
	unsigned char* transfer_profile = new unsigned char[ (transfer_profile_length*2) ];
	unsigned char* transfer_profile_ptr = &transfer_profile[0];
	//char str[100];
	for (int xx = 0; xx < transfer_profile_length; xx++) {
		int value = (int) profile_array[xx];
		//sprintf_s(str, "profile[%i]=%i\n", xx, value);
//		OutputDebugString(str);
		transfer_profile[ (xx * 2) ] = (unsigned char) (value & 0x00FF);  //lower 8-bits
		transfer_profile[ (xx * 2) + 1] = (unsigned char) ( (value >> 8) & 0x00FF); //upper 8-bits
		//::wxLogMessage (wxT ("profile[%i]=%i"), xx, value);
	}

	struct Command::packet packet;
	packet.commandID = Command::PROFILE;
	packet.commandValue = transfer_profile_length;
	packet.profile = transfer_profile;

	xemDevice->GetQueue().Post (packet);

	delete[] profile_array;
}

void DeviceController::SetProfile (unsigned char *profile, int length)
{
	struct Command::packet packet;
	packet.commandID = Command::PROFILE;
	packet.commandValue = length;
	packet.profile = profile;

	xemDevice->GetQueue().Post (packet);
}

void DeviceController::SetCommand (Command::CommandID commandID, int value)
{
	(*variables) [commandID] = value;

	struct Command::packet packet;
	packet.commandID = commandID;
	packet.commandValue = value;

	xemDevice->GetQueue().Post (packet);
}

void DeviceController::GetCommand (Command::CommandID commandID, int &value)
{
	value = (*variables) [commandID];
}

int DeviceController::GetVariable (Command::CommandID commandID)
{
	return (*variables) [commandID];
}

void DeviceController::Initialize (wxString filename)
{
	struct Command::packet packet;
	packet.commandID = Command::DEV_INIT;
	packet.filename = filename;

	xemDevice->GetQueue().Post (packet);
}

void DeviceController::Start()
{
	struct Command::packet packet;
	packet.commandID = Command::DEV_START;

	xemDevice->GetQueue().Post (packet);
}

void DeviceController::Stop()
{
	struct Command::packet packet;
	packet.commandID = Command::DEV_STOP;

	xemDevice->GetQueue().Post (packet);
}
