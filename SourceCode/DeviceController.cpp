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

DeviceController::DeviceController (wxEvtHandler* display)
{
	variables = new std::map<int, int>();

	xemDevice = new DeviceInterface (display);
	xemDevice->Create();
	xemDevice->Run();
}

DeviceController::~DeviceController()
{
//	delete xemDevice;
//	xemDevice = (DeviceInterface*) NULL;

	delete variables;
	variables = NULL;
}

void DeviceController::SetCommand (Command::CommandID commandID, int value)
{
	(*variables) [commandID] = value;

	struct Command::packet packet;
	packet.commandID = commandID;
	packet.commandValue = value;

	xemDevice->GetQueue().Post(packet);
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

	xemDevice->GetQueue().Post(packet);
}

void DeviceController::Start()
{
	struct Command::packet packet;
	packet.commandID = Command::DEV_START;

	xemDevice->GetQueue().Post(packet);
}

void DeviceController::Stop()
{
	struct Command::packet packet;
	packet.commandID = Command::DEV_STOP;

	xemDevice->GetQueue().Post(packet);
}
