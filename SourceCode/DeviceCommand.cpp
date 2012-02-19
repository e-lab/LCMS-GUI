/**
 * @file
 * Contains the class that defines device interface command.
 */

#include "DeviceCommand.h"

DeviceCommand::DeviceCommand(CommandVariable::CommandID name, int value)
{
	commandID = name;
	commandValue = value;
}

DeviceCommand::DeviceCommand(CommandVariable::CommandID name)
{
	commandID = name;
	commandValue = 0;
}

DeviceCommand::~DeviceCommand()
{
}

CommandVariable::CommandID DeviceCommand::GetID()
{
	return commandID;
}

int DeviceCommand::GetValue()
{
	return commandValue;
}

void DeviceCommand::SetValue (int value)
{
	commandValue = value;
}
