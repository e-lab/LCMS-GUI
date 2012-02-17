/**
 * @file
 * Contains a class that is shared between all ControlPanels and the Graphics
 * system, it is through this class that all control values are passes to the
 * DeviceInterface. It is also a convenient place to share data between
 * ControlPanels.
 */
#include "DeviceController.h"
#include "DeviceInterface.h"
#include "CommandVariable.h"

DeviceController::DeviceController (wxEvtHandler* display)
{
	xemDevice = new DeviceInterface (display);
	variables = new std::map<int, int>();
}

DeviceController::~DeviceController()
{
	delete xemDevice;
	xemDevice = (DeviceInterface*) NULL;

	delete variables;
	variables = NULL;
}

void DeviceController::SetCommand (CommandVariable::CommandID command, int value)
{
	(*variables) [command] = value;
	xemDevice->SetCommand (command, value);
}

void DeviceController::GetCommand (CommandVariable::CommandID command, int &value)
{
	value = (*variables) [command];
}

int DeviceController::GetVariable (CommandVariable::CommandID command)
{
	return (*variables) [command];
}

void DeviceController::Initialize (wxString filename)
{
	xemDevice->Initialize (filename);
}

void DeviceController::Start()
{
	xemDevice->Start();
}

void DeviceController::Stop()
{
	xemDevice->Stop();
}
