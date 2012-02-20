#ifndef GRAPHICS_IMAGE_H
#define GRAPHICS_IMAGE_H


#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/textfile.h>

#include <vector>
#include <fstream>

#include "GraphicsPanel.h"
#include "Command.h"


class DeviceEvent;
class GraphicsImageCanvas;
class GraphicsInformation;


/**
 * Receives and displays data from the device.
 *
 * Once the raw data from the device has been passed to this class it is
 * unpacked into an array of pixel values. This array is passed to the
 * GraphicsImageCanvas and its contents are copyed to a bitmap and displayed.
 * A second panel is also initilised and is used to show in real time values
 * from the hardware for debuggin perpuses.
 *
 */
class GraphicsImage : public wxPanel, public GraphicsPanel
{
public:
	/**
	 * Constructor.
	 *
	 * The constructor initializes and sets up the image area and
	 * information panel, it also readies the class for receiving data.
	 *
	 * @param owner
	 *   Pointer to the parent wxWindow derived class.
	 */
	GraphicsImage (wxWindow *);
	/**
	 * Destructor.
	 */
	~GraphicsImage();

	/**
	 * Pass raw data from the device to this object.
	 *
	 * This function is called by the DeviceInterface to pass raw data to
	 * this object.  Once the data have been received it is translated (via
	 * UnpackEvent()) to a pixel array and sent to the GraphicsImageCanvas
	 * object.  Ownership of the raw data event object remains with the
	 * device.
	 *
	 * @param rawEvent
	 *   Reference to the raw data object.
	 */
	void OnDeviceEvent (DeviceEvent&);

	/**
	 * Send a predefined command to the GraphicsPanel.
	 *
	 * The new string value for the command is sent to this class.
	 *
	 * @param command
	 *   Command name of type CommandID.
	 * @param string
	 *   String containing information.
	 */
	void SetCommandString (Command::CommandID, wxString);

private:
	GraphicsImageCanvas*	canvas;
	GraphicsInformation*	information;

	int*		x_coor;
	int*		y_coor;
	int*		timeStamp;
	//int*		events;
	int		eventsLength;

	/**
	 * Vector for raw events.
	 *
	 * This vector is used to save multiple image data events which will be
	 * used to make a movie.
	 */
	std::vector<DeviceEvent*> movie;

	/**
	 * Translate raw data to a usable form.
	 *
	 * This function is called from within DeviceEvent to translate the raw
	 * data from the DeviceInterface into a form that can be displayed by
	 * the GraphicsImageCanvas object.
	 *
	 * @param rawEvent
	 *   Reference to the raw data object.
	 */
	void UnpackEvent (DeviceEvent&);

	/**
	 * Save currently displayed data.
	 *
	 * The data currently being displayed by this object is saved to a data
	 * file with with path/filename of the argument string.
	 *
	 * @param outputFile
	 *   Path and file name for data file.
	 */
	void SaveData (wxString);

};

#endif /* GRAPHICS_IMAGE_H */
