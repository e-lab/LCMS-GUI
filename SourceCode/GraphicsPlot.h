#ifndef GRAPHICS_PLOT_P
#define GRAPHICS_PLOT_P

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/utils.h>

#include <vector>
#include <math.h>

#include "GraphicsPanel.h"
#include "Command.h"
#include "mathplot.h"

#include "GraphicsPlotData.h"
#include "GraphicsInformation.h"
#include "SimpleCircularBuffer.h"
#include "GraphicsSaveData.h"

class DeviceEvent;


/**
 * Receives and displays data from the device.
 *
 * Once the raw data from the device has been passed to this class it is
 * unpacked into two arrays that are saved in a GraphicsPlotData object.  Also
 * saved in the same data object are details about how the plot should look
 * when it is displayed.  This class also provides functions for limited
 * manipulation of the plotting display.
 */

class GraphicsPlot : public wxPanel, public GraphicsPanel
{
public:
	/**
	 * Constructor.
	 *
	 * The constructor initializes and sets up the graphing area and
	 * readies the class for receiving data.
	 *
	 * @param owner
	 *   Pointer to the parent wxWindow derived class.
	 */
	GraphicsPlot (wxWindow *);
	/**
	 * Destructor.
	 */
	~GraphicsPlot();

	void GraphicsPlot::OnQuit();

	/**
	 * Pass raw data from the device to this object.
	 *
	 * This function is called by the DeviceInterface to pass raw data to
	 * this object.  Once the data have been received it is translated (via
	 * UnpackEvent()) to a x/y plot format and saved into a GraphicsPlotData
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

	float* spectrum;
	float* time;
	int length;	//!<  Length of both the spectrum and time arrays.

	mpWindow*		plot;
	GraphicsInformation*	information;
	GraphicsPlotData*	layer;

	mpScaleX*		xScale;
	mpScaleY*		yScale;

	wxPen*			mypen;

	SimpleCircularBuffer<float> *spectrumBuffer;
	SimpleCircularBuffer<float> *timeBuffer;
	int			lengthBuffer;
	int			lengthDisplay;
	float		lastTime;
	int			max_size_buffer;
	int			max_view_size;
	float		max_view_millisec;
	int			white_space_pct; // percent of white space while plotting to make the scrolling look nice
	GraphicsSaveData*	saveToFile;
	int			prev_mode;	//keep track of the mode so we can detect if it changed

	/**
	 * Translate raw data to a usable form.
	 *
	 * This function is called from within DeviceEvent to translate the raw
	 * data from the DeviceInterface into a form that can be saved into a
	 * GraphicsPlotData object.
	  *
	 * @param rawEvent
	 *   Reference to the raw data object.
	 */
	void UnpackEvent (DeviceEvent&);

	/**
	 * Save currently displayed data.
	 *
	 * The data currently being displayed by this object is saved to a data file
	 * with the path/filename of the argument string.
	 *
	 * @param output_file
	 *   Path and file name for data file.
	 */
	void SaveData (wxString);
	void Clear(void);
	void ResetView(void);


};

#endif /* GRAPHICS_PLOT_P */
