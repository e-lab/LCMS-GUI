#ifndef CONTROL_SYNC_CONFIG_H
#define CONTROL_SYNC_CONFIG_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "ControlPanel.h"

class	DeviceController;

/** Declare ControlSyncConfig event IDs.  These are arbitrary but must be unique. */
enum {
	RESET_SYNC_CONFIG = 700,
	BYPASS_LPF0,
	BYPASS_LPF1,
	RESETPERIOD,
	INTRESETDURATION,
	POSTRESETDURATION,
	CDSTIME1,
	CDSTIME2,
	VOLTAGESAMPLINGRATE,
};

/**
 * Constructs PC3 synchronous configuration controls.
 *
 * This class is initialized within MainFrame and it is responsible for
 * constructing the graphical components used to control the synchronous
 * configuration in the device.
 */
class ControlSyncConfig : public ControlPanel
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
	 * @param controller
	 *   Pointer to the DeviceController object.
	 */
	ControlSyncConfig (wxWindow*, DeviceController*);
	/**
	 * Destructor.
	 */
	~ControlSyncConfig();

	void OnBypass_lpf0 (wxCommandEvent& evt);
	void OnBypass_lpf1 (wxCommandEvent& evt);

	void OnChoiceVoltageSamplingRate (wxCommandEvent& evt);
	void OnChoiceResetPeriod (wxCommandEvent& evt);
	void OnChoiceIntResetDuration (wxCommandEvent& evt);
	void OnChoicePostResetDuration (wxCommandEvent& evt);
	void OnChoiceCDSTime1 (wxCommandEvent& evt);
	void OnChoiceCDSTime2 (wxCommandEvent& evt);
	/**
	 * Reset the controls.
	 *
	 * Handles all the events which are generated if a 'Reset' button in
	 * this class is pressed.  If more then one 'Reset' button, the event
	 * ID is use to identify from which button it come from and then the
	 * necessary function is called to reset the control values or
	 * default state.
	 *
	 * @param event
	 *   Reference to a wxCommandEvent.
	 */
	void ResetControls (wxCommandEvent&);
	/**
	 * Reset all sliders and text labels to the default values.
	 */

	void Reset (void);

private:
	DeviceController*	xemDeviceCtrl;

	int			maxValueSlider;
	int			minValueSlider;
	float			maxVoltageSlider;

	wxRadioButton*		bypass_lpf0;
	wxRadioButton*		bypass_lpf1;

	int			defaultValueVoltageSamplingRate;
	wxChoice*		choiceVoltageSamplingRate;


	int			defaultValueResetPeriod;
	wxChoice*		choiceResetPeriod;

	int			defaultValueIntResetDuration;
	wxChoice*		choiceIntResetDuration;

	int			defaultValuePostResetDuration;
	wxChoice*		choicePostResetDuration;

	int			defaultValueCDSTime1;
	wxChoice*		choiceCDSTime1;

	int			defaultValueCDSTime2;
	wxChoice*		choiceCDSTime2;

};


#endif /* CONTROL_SYNC_CONFIG_H */
