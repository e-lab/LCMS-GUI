#ifndef DEVICE_INTERFACE_H
#define DEVICE_INTERFACE_H

#include <wx/wx.h>
#include "SimpleQueue.h"
#include "Command.h"
#include "okFrontPanelDLL.h"

class DeviceEvent;

/**
 * Class providing the high level interface to the device.
 *
 * This class is the only class in the application which interacts with the
 * Opal Kelly and e-Lab hardware.  It loads libraries and initializes the Opal
 * Kelly board, it can change variables in the device and ask for the data
 * collected.  The public functions provide the rest of the application with
 * the only means of interaction with the device.
 */
class DeviceInterface : public wxThread
{
public:
	/**
	 * Constructor.
	 *
	 * The constructor tries to initialize the the xem device by supplying a default
	 * filename to the Initialize() member-function.  If the file does not exist or
	 * an error occurred during initialization an error message will be displayed and
	 * the user will need to explicitly configure the xem at some later point.
	 *
	 * Also initialized is a wxTimer object for which this class is the listener.  The
	 * timer is used to control the frequency that the xem devices is polled for data.
	 *
	 * @param displayTmp
	 *   Reference to the object being used to display data.
	 */
	DeviceInterface (wxEvtHandler*);
	/**
	 * Destructor.
	 */
	~DeviceInterface ();

	/**
	 * Thread execution starts here.
	 */
	virtual void *Entry();

	/**
	 * Alows access to the message (command) queue so other threads can send commands
	 * to the device interface.
	 *
	 * @retval Command Queue
	 */
	SimpleQueue<Command::command>& GetQueue ();

	/**
	 * Initialize the hardware device.
	 *
	 * This member-function performs all necessary steps to initialize a device.
	 * First it load the Opal Kelly library and tries to open a communication
	 * channel to an Opal Kelly board.  If a board is found its pll (Phase Locked
	 * Loop) is configured before finally the Xilinx configuration '.bit' file is
	 * upload to it.  If an error occurs during any of these steps an error
	 * message will be displayed and the current object returned to its original
	 * state before the function returns false.
	 *
	 * @param filename
	 *   String containing a '.bit' filename.
	 * @retval TRUE
	 *   If successful.
	 * @retval FALSE
	 *   If an error occurred.
	 */
	bool Initialize (wxString);

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

	/**
	 * Send a predefined command to the device.
	 *
	 * The new value for the command is sent to the device and then the device
	 * is reset for the change in value to take effect.
	 *
	 * @param commandID
	 *   Command name of type CommandID.
	 * @param value
	 *   Value to be sent to the device.
	 */
	void SetCommand (Command::CommandID, int);


private:
	SimpleQueue<Command::command>	commandQueue;

	okCFrontPanel* 		xem;		//!< Pointer to the Opal Kelly library object.
	okCPLL22150*		pll;		//!< Pointer to the Phased Locked Loop (pll) container object.
	wxEvtHandler*		display;	//!< Pointer to the GraphicsPanel object being used to display data.

	DeviceEvent*		rawEvent;

	bool 			pollDevice;

};

#endif /* DEVICE_INTERFACE_H */
