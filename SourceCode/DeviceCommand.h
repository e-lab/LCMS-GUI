#ifndef DEVICE_COMMAND_H
#define DEVICE_COMMAND_H

#include <wx/wx.h>
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

	/**
	 * Set the path and filename of the bitfile used in the DeviceInterface Initialize function.
	 *
	 * @param filename
	 *   Path and filename to the bitfile.
	 */
	void SetFileName (wxString);
	/**
	 * Get the path and filename of the bitfile used in the DeviceInterface Initialize function.
	 *
	 * @retval filename
	 *   Path and filename to the bitfile.
	 */
	wxString GetFileName ();

private:

	CommandVariable::CommandID 	commandID;
	int 				commandValue;
	wxString 			filename;

};

#endif /* DEVICE_COMMAND_H */
