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

void DeviceController::BuildProfile ()
{
	int PC5_INTERVAL = GetVariable (Command::PC5_INTERVAL);
	int LCMS_VOLTAGESAMPLINGRATE	= GetVariable (Command::LCMS_VOLTAGESAMPLINGRATE);
	double scaling = ( (double) (1000.0 / LCMS_VOLTAGESAMPLINGRATE));
	int length = PC5_INTERVAL * scaling;
	float *profile_array;
	profile_array = new float[length];

	if (GetVariable(Command::LCMS_PROFILE_TYPE) == 0) {
		BuildProfileSquare(&profile_array[0]);
	} else if (GetVariable(Command::LCMS_PROFILE_TYPE) ==  1) {
		BuildProfileTriangle(&profile_array[0]);
	}
	else {
		BuildProfileFile();
		//read from file not yet implemented
	}

	long int transfer_profile_length =length;
	unsigned char* transfer_profile = new unsigned char[ (transfer_profile_length*2) ];
	//char str[100];
	for (int xx = 0; xx < transfer_profile_length; xx++) {
		int value = (int) profile_array[xx];
		//sprintf_s(str, "profile[%i]=%i\n", xx, value);
//		OutputDebugString(str);
		transfer_profile[ (xx * 2) ] = (unsigned char) (value & 0x00FF);  //lower 8-bits
		transfer_profile[ (xx * 2) + 1] = (unsigned char) ( (value >> 8) & 0x00FF); //upper 8-bits
		//::wxLogMessage (wxT ("profile[%i]=%i"), xx, value);
	}

	delete[] profile_array;

	SetProfile(&transfer_profile[0], transfer_profile_length*2);
}

void DeviceController::BuildProfileSquare(float *profile_array)
{
	int PC5_LEAD_TIME               = GetVariable (Command::PC5_LEAD_TIME);
	int PC5_PEAK_1_DUR              = GetVariable (Command::PC5_PEAK_1_DUR);
	int PC5_PEAK_1_HEIGHT           = GetVariable (Command::PC5_PEAK_1_HEIGHT);
	int PC5_PEAK_2_DUR              = GetVariable (Command::PC5_PEAK_2_DUR);
	int PC5_PEAK_2_HEIGHT           = GetVariable (Command::PC5_PEAK_2_HEIGHT);
	int PC5_INTERVAL                = GetVariable (Command::PC5_INTERVAL);
	int PC5_V_CMD_OFFSET            = GetVariable (Command::PC5_V_CMD_OFFSET);
	int LCMS_VOLTAGESAMPLINGRATE	= GetVariable (Command::LCMS_VOLTAGESAMPLINGRATE);
	double scaling = ( (double) (1000.0 / LCMS_VOLTAGESAMPLINGRATE));
	int length = PC5_INTERVAL * scaling;

	int lead_time = PC5_LEAD_TIME * scaling;
	float p0_height = PC5_V_CMD_OFFSET;
	int p1_dur = PC5_PEAK_1_DUR * scaling;
	int p1_height = PC5_PEAK_1_HEIGHT;
	int p2_dur = PC5_PEAK_2_DUR * scaling;
	int p2_height = PC5_PEAK_2_HEIGHT;
	int p3_height = p0_height;

	for (int xx = 0; xx < lead_time; xx++) {		//lead time
		profile_array[xx] = p0_height;
	}
	for (int xx = lead_time; xx < (lead_time + p1_dur); xx++) {		//peak 1
		profile_array[xx] = (float) p0_height + (float) p1_height;
	}
	for (int xx = (lead_time + p1_dur); xx < (lead_time + p1_dur + p2_dur); xx++) {	//peak 2
		profile_array[xx] = (float) p0_height + (float) p2_height;
	}
	for (int xx = (lead_time + p1_dur + p2_dur); xx < length; xx++) {	//rest of the interval
		profile_array[xx] = (float) p3_height;
	}
}

void DeviceController::BuildProfileTriangle(float *profile_array)
{
	int PC5_LEAD_TIME               = GetVariable (Command::PC5_LEAD_TIME);
	int PC5_PEAK_1_DUR              = GetVariable (Command::PC5_PEAK_1_DUR);
	int PC5_PEAK_1_HEIGHT           = GetVariable (Command::PC5_PEAK_1_HEIGHT);
	int PC5_PEAK_2_DUR              = GetVariable (Command::PC5_PEAK_2_DUR);
	int PC5_PEAK_2_HEIGHT           = GetVariable (Command::PC5_PEAK_2_HEIGHT);
	int PC5_INTERVAL                = GetVariable (Command::PC5_INTERVAL);
	int PC5_V_CMD_OFFSET            = GetVariable (Command::PC5_V_CMD_OFFSET);
	int LCMS_VOLTAGESAMPLINGRATE	= GetVariable (Command::LCMS_VOLTAGESAMPLINGRATE);
	double scaling = ( (double) (1000.0 / LCMS_VOLTAGESAMPLINGRATE));
	int length = PC5_INTERVAL * scaling;

	int lead_time = PC5_LEAD_TIME * scaling;
	float p0_height = PC5_V_CMD_OFFSET;
	int p1_dur = PC5_PEAK_1_DUR * scaling;
	int p1_height = PC5_PEAK_1_HEIGHT;
	int p2_dur = PC5_PEAK_2_DUR * scaling;
	int p2_height = PC5_PEAK_2_HEIGHT;
	int p3_height = p0_height;

	for (int xx = 0; xx < lead_time; xx++) {	//lead time
		profile_array[xx] = p0_height;
	}
	for (int xx = lead_time; xx < (lead_time + (p1_dur/2)); xx++) { //peak 1 ramp up
		profile_array[xx] = profile_array[xx-1] + float((float)p1_height*2.0/(float)p1_dur);
	}
	for (int xx = (lead_time + (p1_dur/2)); xx < (lead_time + p1_dur); xx++) { //peak 1 ramp down
		profile_array[xx] = profile_array[xx-1] - float((float)p1_height*2.0/(float)p1_dur);
	}
	for (int xx = (lead_time + p1_dur); xx < (lead_time + p1_dur + (p2_dur/2)); xx++) { //peak 2 ramp up
		profile_array[xx] = profile_array[xx-1] + float((float)p2_height*2.0/(float)p2_dur);
	}
	for (int xx = (lead_time + p1_dur + (p2_dur/2)); xx < (lead_time + p1_dur + p2_dur); xx++) { //peak 2 ramp down
		profile_array[xx] = profile_array[xx-1] - float((float)p2_height*2.0/(float)p2_dur);
	}
	for (int xx = (lead_time + p1_dur + p2_dur); xx < length; xx++) {	//rest of the interval
		profile_array[xx] = (float) p3_height;
	}
}
void DeviceController::BuildProfileFile(void)
{
}

void DeviceController::SetProfile (unsigned char *profile, int length)
{
	int w_length; // wrap profile length
	int n_length; // new length of (pristine/concat) profile length

	// determine wrap profile array length
	if (length >= DEVICE_BUFFER_SIZE ) {
		n_length = length;
		w_length = length+DEVICE_BUFFER_SIZE;
	} else {
		int lengths_in_buf = ((int) DEVICE_BUFFER_SIZE / length);
		n_length = length+(lengths_in_buf*length);
		w_length = n_length+DEVICE_BUFFER_SIZE;
	}

	// alloc wrap profile array and populate first segment
	unsigned char *w_profile = new unsigned char[w_length];
	memcpy(&w_profile[0], &profile[0], length*sizeof(profile[0]));

	delete[] profile;
	profile = NULL;

	// fill wrap round portion of wrap profile array
	int profile_ptr = length;
	int segment_length;

	while (w_length > profile_ptr) {

		segment_length = (w_length-profile_ptr);
		if (segment_length > profile_ptr) {
			segment_length = profile_ptr;
		}

		memcpy(&w_profile[profile_ptr], &w_profile[0],  segment_length*sizeof(w_profile[0]));

		profile_ptr = profile_ptr + segment_length;
	}

	struct Command::packet packet;
	packet.commandID = Command::PROFILE;
	packet.commandValue = n_length;
	packet.profile = w_profile;

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
