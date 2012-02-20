#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/bookctrl.h>

#include <vector>
#include <iostream>

class	ControlPanel;
class	GraphicsPanel;
class	DeviceController;

/** Declare MainFrame event IDs.  These are arbitrary but must be unique. */
const int BOOKCTRL =		100;
const int CONFIGURATION =	101;
const int ON_START =		102;
const int ON_STOP =		103;

/**
 * Frame in which the application is constructed.
 *
 * This class is initialized within ELabApp and it is responsible for
 * constructing the graphical components and all other objects needed to
 * interface with the hardware and the display of data.
 */
class MainFrame : public wxFrame
{
public:
	/**
	 * Constructor.
	 *
	 * The constructor calls the member-functions to create the following:
	 *		- Icon (displayed in top-left corner)
	 *		- Menu bar
	 *		- Graphics for the plot or images
	 *		- Control panels
	 *
	 * Also created is a device object to provide a high level interface to the
	 * hardware.  Once all the above have been completed the events of for this
	 * class are associated with event handlers.
	 *
	 * @param title
	 *   Reference to a string containing the title that will be displayed in the frame.
	 * @param size
	 *   Reference to a size object that defines the initial size of the application.
	 */
	MainFrame (const wxString& title, const wxSize& size);
	/**
	 * Destructor.
	 */
	~MainFrame();

private:
	wxPanel*		graphics;
	wxPanel*		controls;

	GraphicsPanel*		display;
	DeviceController*	xemDeviceCtrl;

	/**
	 * Vector saves PanelControl objects.
	 */
	std::vector<ControlPanel*> controlBook;


	/**
	 * Set all values in hardware and start measuring.
	 *
	 * Handles the event which is generated when the 'Start' button is pressed.
	 * Calls the start functions of all the command panels, these will reads
	 * all the control values and sends them to the device.  After calling the
	 * command panels start functions this function will then call the xem
	 * device DeviceController::Start() to begin the measurements.
	 *
	 * @param event
	 *   Reference to a wxCommandEvent.
	 */
	void OnStart (wxCommandEvent&);
	/**
	 * Stop measuring.
	 *
	 * Handles the event which is generated when the 'Stop' button is pressed.
	 * It will call DeviceController:Stop() which will stop the measurements.
	 *
	 * @param event
	 *   Reference to a wxCommandEvent.
	 */
	void OnStop (wxCommandEvent&);

	/**
	 * Save to file the data currently displayed.
	 *
	 * Handles the event which is generated when the 'Save' menu item is selected.
	 * It brings up a file selection dialog which is used to select where and with
	 * what name the data file will be saved. It will then call the SaveData
	 * function in the GraphicsPanel object which will do the actual saving.
	 *
	 * @param event
	 *   Reference to a wxCommandEvent.
	 */
	void OnSave (wxCommandEvent&);
	/**
	 * Configure and initialize the xem device.
	 *
	 * Handles the event which is generated when the 'Config' menu item is
	 * selected.  It brings up a file selection dialog which is used to select
	 * the Xilinx Configuration '.bit' file.  The file name is then passed
	 * via the DeviceController to the DeviceInterface objects Initialize
	 * member-function which will do the actual configuration and
	 * initialization of the xem device.
	 *
	 * @param event
	 *   Reference to a wxCommandEvent.
	 */
	void Configuration (wxCommandEvent&);
	/**
	 * Quit application and close the window.
	 *
	 * Handles the default ID exit event which is generated when the
	 * 'Exit' menu item is selected or 'Alt-X' is pressed.
	 *
	 * @param event
	 *   Reference to a wxCommandEvent.
	 */
	void OnQuit (wxCommandEvent&);
	/**
	 * Show the About window.
	 *
	 * Handles the default ID about event which is generated when the
	 * 'About' menu item is selected or 'F1' is pressed.
	 *
	 * @param event
	 *   Reference to a wxCommandEvent.
	 */
	void OnAbout (wxCommandEvent&);

	/**
	 * Create an icon to be displayed in top-left hand corner.
	 *
	 * The (yale/purdue)_icon.xpm image will be used to make this icon.
	 */
	void SetIconResource();
	/**
	 * Create and set the menu-bar for the application.
	 */
	void CreateMenuBar();
	/**
	 * Create the graphics panel.
	 *
	 * This function will initiate the 'graphics' panel and add too it the objects
	 * needed to display the data.  Currently, this needs to be called before the
	 * DeviceController object is created.
	 *
	 * @param panelMain
	 *   Reference to the top most panel in the application.
	 */
	void CreateGraphics (wxPanel*);
	/**
	 * Create the control panel.
	 *
	 * This function will initiate the 'controls' panel and add to it those
	 * objects needed to create and display the application controls (buttons,
	 * slider, etc).  It will also create and add the Logo panel. Currently, this
	 * needs to be called after the DeviceController object has been created.
	 *
	 * @param panelMain
	 *   Reference to the top most panel in the application.
	 */
	void CreateControls (wxPanel*);

};

#endif /* MAIN_WINDOW_H */
