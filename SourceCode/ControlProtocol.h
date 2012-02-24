#ifndef CONTROL_PROTOCOL_H
#define CONTROL_PROTOCOL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/fileconf.h>
#include <wx/wfstream.h>
#include <wx/dir.h>
#include "ControlPanel.h"

class	DeviceController;

/** Declare ControlProtocol event IDs.  These are arbitrary but must be unique. */
const int RESET_PROTOCOL =	200;
const int PEAK_1_HEIGHT =	201;
const int PEAK_2_HEIGHT =	202;
const int LEAD_TIME =		203;
const int INTERVAL =		204;
const int PEAK_1_DUR =		205;
const int PEAK_2_DUR =		206;
const int PEAK_1_STEP =		207;
const int NUM_OF_STEPS =	208;
const int AVERAGE_OVER =	209;
const int PROTOCOL_CONFIG =	210;

//const int PROTOCOL_LIVE =	252;
//const int PROTOCOL_MEAS =	253;




/**
 * Constructs Protocol controls.
 *
 * This class is initialized within MainFrame and it is responsible for
 * constructing the graphical components used to control protocols in
 * the device.
 */
class ControlProtocol : public ControlPanel
{
public:
	/**
	 * Constructor.
	 *
	 * The constructor initializes all controls (sliders, buttons, etc) contained
	 * within the panel and defines its layout.
	 *
	 * @param owner
	 *   Pointer to the parent wxWindow derived class.
	 * @param device
	 *   Pointer to the DeviceController object.
	 */
	ControlProtocol (wxWindow*, DeviceController*);
	/**
	 * Destructor.
	 */
	~ControlProtocol();

	/**
	 * Change the value of a protocol variable.
	 *
	 * Handles all the events which are generated if a protocol variable is altered.
	 * The event ID is used to identify which variable was altered and then the
	 * appropriate function is called.
	 *
	 * @param evt
	 *   Reference to a wxSpinEvent.
	 */
	void ChangeProtocolValue (wxSpinEvent&);

	/**
	 * Reset the controls.
	 *
	 * Handles all the events which are generated if a 'Reset' button in this
	 * class is pressed.  If more then one 'Reset' button, the event ID is used
	 * to identify from which button it come from and then the necessary function
	 * is called to reset the control values or default state.
	 *
	 * @param evt
	 *   Reference to a wxCommandEvent.
	 */
	void ResetControls (wxCommandEvent&);
	/**
	 * Reset all sliders and text labels to the default values.
	 */
	void ResetProtocolValues();

	/**
	 * Reload the protocol configuration files.
	 *
	 * Reads all *.cfg files contained in a directory, the path for which is passed
	 * as the argument.
	 *
	 * @param path
	 *   wxString containing the full path-name to a directory.
	 */
	void LoadConfigFiles (wxString);

	/**
	 * Change the protocol configuration to a predefined setting.
	 *
	 * Handles all the events which are generated if a 'Configuration List' selection
	 * is changed. The corresponding config file is then read and the protocol setting
	 * are reset.
	 *
	 * @param evt
	 *   Reference to a wxCommandEvent.
	 */
	void OnConfigList (wxCommandEvent&);

	/**
	 * Save the current protocol configuration to file.
	 *
	 * Saves the current protocol settings to a file with the a filename and path of
	 * the the argument.
	 *
	 * @param filename
	 *   Filename and path.
	 */
	void SaveCurrentConfig (wxString);

	/**
	 * Select a voltage measurement protocol to use.
	 *
	 * Handles all the events which are generated when any of the 'Protocol'
	 * radio buttons are selected.  The event ID is used to identify from
	 * which button it come from and then the necessary function is called to
	 * set the protocol type.
	 *
	 * @param evt
	 *   Reference to a wxCommandEvent.
	 */
	void SetProtocolType (wxCommandEvent &);

	/**
	 * Send all current control values to the device.
	 */
	void Start();


	/**
	 * Gets height of peak 1
	 */
	int GetPeak1Height() {
		return GetSpinControlVal (1);
	}

	/**
	 * Gets height of peak 2
	 */
	int GetPeak2Height() {
		return GetSpinControlVal (2);
	}

	/**
	 * Gets lead time
	 */
	int GetLeadTime() {
		return GetSpinControlVal (3);
	}

	/**
	 * Gets duration of peak 1
	 */
	int GetPeak1Duration() {
		return GetSpinControlVal (4);
	}

	/**
	 * Gets duration of peak 2
	 */
	int GetPeak2Duration() {
		return GetSpinControlVal (5);
	}

	/**
	 * Gets interval
	 */
	int GetInterval() {
		return GetSpinControlVal (6);
	}

private:
	DeviceController*	xemDeviceCtrl;

	wxSpinCtrl* 	peak1Height;
	wxSpinCtrl* 	peak2Height;
	wxSpinCtrl* 	leadTime;
	wxSpinCtrl* 	peak1Duration;
	wxSpinCtrl* 	peak2Duration;
	wxSpinCtrl* 	interval;

	wxRadioButton*	protoLive;
	wxRadioButton*	protoMeasure;

	wxChoice*	configList;
	wxArrayString*	fileList;


	/**
	 * Gets the value from a wxSpinCtrl
	 */
	int GetSpinControlVal (int);
	/**
	 * Sets the value passed as the argument as the new hardware setting and display.
	 */
	void SetPeak1Height (int);
	/**
	 * Sets the value passed as the argument as the new hardware setting and display.
	 */
	void SetPeak2Height (int);
	/**
	 * Sets the value passed as the argument as the new hardware setting and display.
	 */
	void SetLeadTime (int);
	/**
	 * Sets the value passed as the argument as the new hardware setting and display.
	 */
	void SetPeak1Duration (int);
	/**
	 * Sets the value passed as the argument as the new hardware setting and display.
	 */
	void SetPeak2Duration (int);
	/**
	 * Sets the value passed as the argument as the new hardware setting and display.
	 */
	void SetInterval (int);

	void SetIntervalRange();

	/**
	 * Sets the value passed as the argument as the new sampling clock frequency.
	 */


};

#endif /* CONTROL_PROTOCOL_H */
