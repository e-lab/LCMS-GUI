#ifndef DEVICE_CONTROLLER_H
#define DEVICE_CONTROLLER_H

#include <wx/wx.h>
#include <map>

#include "Command.h"

class	DeviceInterface;


/**
 * Constructs the controller for the device interface.
 *
 * This class is initialized within MainFrame and it is responsible for all
 * control/commuication with the DeviceInterface. It also acts as a store for
 * all control values that have been sent to the interface.
 */
class DeviceController
{
public:
	/**
	 * Constructor.
	 *
	 * The constructor initializes the device interface through which all
	 * commuication with the Opal Kelly device is done. It also initializes a map
	 * that stores the control key/value pairs.
	 *
	 * @param display
	 *   Reference to the object being used to display data.
	 */
	DeviceController (wxEvtHandler*);
	/**
	 * Destructor.
	 */
	~DeviceController();

	/**
	 * Set the value of the variable.
	 *
	 * @param variable
	 *   ID of the variable.
	 * @param value
	 *   Integer value of the variable.
	 */
	void SetCommand (Command::CommandID, int);
	/**
	 * Retrieve the value of the variable.
	 *
	 * @param variable
	 *   ID of the variable.
	 * @param value
	 *   Integer into which the 'variable' value is saved.
	 */
	void GetCommand (Command::CommandID, int&);
	/**
	 * Retrieve the value of the variable.
	 *
	 * @param variable
	 *   ID of the variable.
	 * @return
	 *   Integer value of the 'variable'.
	 */
	int GetVariable (Command::CommandID);


	/**
	 * Initialize the hardware device.
	 *
	 * This member-function performs will initialize a device.
	 *
	 * @param filename
	 *   String containing a '.bit' filename.
	 */
	void Initialize (wxString);
	/**
	 * Start polling the device for data.
	 *
	 * Starts the timer and sets the frequency of the polling to the value in
	 * milliseconds.
	 */
	void Start();
	/**
	 * Stop polling the device for data.
	 */
	void Stop();


private:
	DeviceInterface* xemDevice;

	std::map<int, int>* variables;			//!<  Map to store device variables.

};


#endif /* DEVICE_CONTROLLER_H */
