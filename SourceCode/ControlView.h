#ifndef PANEL_VIEW_H
#define PANEL_VIEW_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "ControlPanel.h"

class	DeviceController;

/** Declare ControlView event IDs.  These are arbitrary but must be unique. */
const int SAVE_FRAME =			300;
const int SAVE_MOVIE =	 		301;
const int SLIDER_EVENTS =		302;
const int SLIDER_TIMER =		303;
const int TIME_OUT =			308;

const int RESET_SAVE_TYPE =		304;
const int RESET_TRIGGER_OUT =		305;

/**
 * Constructs Video controls.
 *
 * This class is initialized within MainFrame and it is responsible for
 * constructing the graphical components used to control reference biases.
 */
class ControlView : public ControlPanel
{
public:
	/**
	 * Constructor.
	 *
	 * The constructor initializes all controls (sliders, buttons, etc)
	 * contained within the panel and defines its layout.
	 *
	 * @param owner
	 *   Pointer to the parent wxWindow derived class.
	 * @param device
	 *   Pointer to the DeviceController object.
	 */
	ControlView (wxWindow*, DeviceController*);
	/**
	 * Destructor.
	 */
	~ControlView();

	/**
	 * Change Save Type.
	 *
	 * Handles the event which is generated when one of the save type
	 * radio button is selected.  The event id is used to select the
	 * proper function which will send the change of state to the
	 * Divice object.
	 *
	 * @param event
	 *   Reference to a wxCommandEvent.
	 */
	void OnSaveType (wxCommandEvent&);
	/**
	 * Change Save Type to frame.
	 *
	 * When the save type "Frame" radio button is selected, this
	 * function will send this change of state to the Divice object.
	 */
	void ChangeSaveFrame ();
	/**
	 * Change Save Type to movie.
	 *
	 * When the save type "Movie" radio button is selected, this
	 * function will send this change of state to the Divice object.
	 */
	void ChangeSaveMovie ();

	/**
	 * Change the Event Number.
	 *
	 * Handles the event which is generated when a 'Event Number' slider
	 * is moved.  The change in position will be sent to the xem device
	 * and the sliders label updated.  This value controls in hardware
	 * the number of events from the camera collected before the data
	 * is sent to the computer.
	 *
	 * @param event
	 *   Reference to a wxScrollEvent.
	 */
	void ChangeEventNum (wxScrollEvent&);
	/**
	 * Set the 'Event Number' text label to the current slider value.
	 */
	void SetTextEventNum ();

	/**
	 * Change the Time Out.
	 *
	 * Handles the event which is generated when the 'Time Out' spiner
	 * is changed.  The change in value will be sent to the xem device.
	 * This value controls the hardware time out (in msec) when the
	 * device will send the collected events to the computer, even if
	 * the event number has not been reached.
	 *
	 * @param event
	 *   Reference to a wxSpinEvent.
	 */
	void ChangeTimeOut (wxSpinEvent&);

	/**
	 * Reset a control box.
	 *
	 * Handles all the events which are generated if a 'Reset' button in this
	 * class is pressed.  If more then one 'Reset' button, the event ID is used
	 * to identify from which button it come from and then the necessary function
	 * is called to reset the control values or default state.
	 *
	 * @param event
	 *   Reference to a wxCommandEvent.
	 */
	void ResetControls (wxCommandEvent&);
	/**
	 * Reset all control elements to the default values.
	 */
	void ResetSaveType();
	/**
	 * Reset all control elements to the default values.
	 */
	void ResetTriggerOut();

	/**
	 * Send all control values to the device.
	 */
	void Start();

private:
	DeviceController*	xemDeviceCtrl;

	wxRadioButton*		saveFrame;
	wxRadioButton*		saveMovie;

	int			maxValueEvents;
	int			defaultValueEvents;
	float			maxVoltageEvents;
	wxSlider*		sliderEvents;
	wxStaticText*		textEvents;

	int			maxValueTimeOut;
	int			defaultValueTimeOut;
	wxSpinCtrl* 		timeOut;
};


#endif /* PANEL_VIEW_H */
