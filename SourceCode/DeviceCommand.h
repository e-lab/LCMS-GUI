#ifndef DEVICE_COMMAND_H
#define DEVICE_COMMAND_H

#include "CommandVariable.h"


/**
 * Class which contains a command to be sent to the DeviceInterface Queue.
 */
class DeviceCommand
{
public:
	/**
	 * ID & value Constructor.
	 */
	DeviceCommand (CommandVariable::CommandID, int);
	/**
	 * ID Constructor, sets value to zero.
	 */
	DeviceCommand (CommandVariable::CommandID);
	/**
	 * Destructor.
	 */
	~DeviceCommand ();

	/**
	 * Retrieve the Command ID.
	 */
	CommandVariable::CommandID GetID ();
	/**
	 * Retrieve the Command value.
	 */
	int GetValue ();
	/**
	 * Set the value of command.
	 *
	 * @param value
	 *   Integer value of the command.
	 */
	void SetValue (int);

private:

	CommandVariable::CommandID 	commandID;
	int 				commandValue;

};

#endif /* DEVICE_COMMAND_H */
