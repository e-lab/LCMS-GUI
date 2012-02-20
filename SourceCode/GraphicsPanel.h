#ifndef GRAPHICS_PANEL_H
#define GRAPHICS_PANEL_H

#include <wx/wx.h>
#include "DeviceEvent.h"
#include "Command.h"

/**
 * Abstract class providing an inherited interface to derived class.
 *
 * Once the raw data from the device has been passed to a class implementing
 * this interface the data is unpacked and transformed into a usable form.
 * The implementing class must also provide functions for the manipulation
 * of the graphing display.
 */
class GraphicsPanel  :  public wxEvtHandler
{
public:
	/**
	 * Constructor.  Connects the device event to the event handler.
	 */
	GraphicsPanel() {
		Connect (wxID_ANY, DEVICE_EVENT, wxObjectEventFunction (&GraphicsPanel::OnDeviceEvent));
	};

	/**
	 * Handle a raw data event from the device.
	 *
	 * This function is the event handler used to process the DeviceEvent
	 * and is used to pass raw data to this object.  See implementing class
	 * for details.
	 *
	 * @param rawEvent
	 *   Reference to the raw data object.
	 */
	virtual void OnDeviceEvent (DeviceEvent&) {};

	/**
	 * Send a predefined command to the display.
	 *
	 * See implementing class for details.
	 *
	 * @param command
	 * @param string
	 */
	virtual void SetCommandString (Command::CommandID, wxString) {};
};

#endif /* GRAPHICS_PANEL_H */
