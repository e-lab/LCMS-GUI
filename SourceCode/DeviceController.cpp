/**
 * @file
 * Contains a class that is shared between all ControlPanels and the Graphics
 * system, it is through this class that all control values are passes to the
 * DeviceInterface. It is also a convenient place to share data between
 * ControlPanels.
 */
#include "DeviceController.h"
#include "DeviceCommand.h"
#include "DeviceInterface.h"
#include "CommandVariable.h"

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

void DeviceController::SetCommand (CommandVariable::CommandID commandID, int value)
{
	(*variables) [commandID] = value;

	DeviceCommand *command = new DeviceCommand (commandID, value);
	xemDevice->GetQueue().Post (*command);
}

void DeviceController::GetCommand (CommandVariable::CommandID commandID, int &value)
{
	value = (*variables) [commandID];
}

int DeviceController::GetVariable (CommandVariable::CommandID commandID)
{
	return (*variables) [commandID];
}

void DeviceController::Initialize (wxString filename)
{
	DeviceCommand *command = new DeviceCommand (CommandVariable::DEV_INIT);
	command->SetFileName (filename);
	xemDevice->GetQueue().Post (*command);
}

void DeviceController::Start()
{
	DeviceCommand *command = new DeviceCommand (CommandVariable::DEV_START);
	xemDevice->GetQueue().Post (*command);
}

void DeviceController::Stop()
{
	DeviceCommand *command = new DeviceCommand (CommandVariable::DEV_STOP);
	xemDevice->GetQueue().Post (*command);
}
