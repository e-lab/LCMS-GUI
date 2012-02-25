#ifndef CONTROL_CONFIGURATION_H
#define CONTROL_CONFIGURATION_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "ControlPanel.h"

class	DeviceController;
class	GraphicsPlot;

/** Declare ControlConfig event IDs.  These are arbitrary but must be unique. */
enum {
	RESET_CONFIG = 600,
	CAPSELECT0,
	CAPSELECT1,
	BYPASSPOST0,
	BYPASSPOST1,
	POSTGAINSELECT0,
	POSTGAINSELECT1,

	CHANNEL1_SEL,
	CHANNEL2_SEL,
	CHANNEL3_SEL,
	CHANNEL4_SEL,
	CHANNEL5_SEL,
	CHANNEL6_SEL,
	CHANNEL7_SEL,
	CHANNEL8_SEL,
	CHANNEL9_SEL,
	CHANNEL10_SEL,
	CHANNEL11_SEL,
	CHANNEL12_SEL,
	CHANNEL13_SEL,
	CHANNEL14_SEL,
	CHANNEL15_SEL,
	CHANNEL16_SEL,
	GAIN,
};

/**
 * Constructs PC3 configuration controls.
 *
 * This class is initialized within MainFrame and it is responsible for
 * constructing the graphical components used to control the configuration
 * of the device.
 */
class ControlConfig : public ControlPanel
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
	ControlConfig (wxWindow*, DeviceController*);
	/**
	 * Destructor.
	 */
	~ControlConfig();

	void OnCapSelect0 (wxCommandEvent& evt);
	void OnCapSelect1 (wxCommandEvent& evt);
	void OnChannel1Sel (wxCommandEvent& evt);
	void OnChannel2Sel (wxCommandEvent& evt);
	void OnChannel3Sel (wxCommandEvent& evt);
	void OnChannel4Sel (wxCommandEvent& evt);
	void OnChannel5Sel (wxCommandEvent& evt);
	void OnChannel6Sel (wxCommandEvent& evt);
	void OnChannel7Sel (wxCommandEvent& evt);
	void OnChannel8Sel (wxCommandEvent& evt);
	void OnChannel9Sel (wxCommandEvent& evt);
	void OnChannel10Sel (wxCommandEvent& evt);
	void OnChannel11Sel (wxCommandEvent& evt);
	void OnChannel12Sel (wxCommandEvent& evt);
	void OnChannel13Sel (wxCommandEvent& evt);
	void OnChannel14Sel (wxCommandEvent& evt);
	void OnChannel15Sel (wxCommandEvent& evt);
	void OnChannel16Sel (wxCommandEvent& evt);
	void OnBypassPost0 (wxCommandEvent& evt);
	void OnBypassPost1 (wxCommandEvent& evt);
	void OnPostGainSelect0 (wxCommandEvent& evt);
	void OnPostGainSelect1 (wxCommandEvent& evt);

	void OnSpinGain (wxSpinEvent& evt);

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

	void SetGraphicsPlot (GraphicsPlot *);

private:

	GraphicsPlot*  		graphicsPlot;
	DeviceController*	xemDeviceCtrl;

	int			maxValueSlider;
	int			minValueSlider;
	float			maxVoltageSlider;

	wxRadioButton*		capSelect0;
	wxRadioButton*		capSelect1;

	wxRadioButton*		bypassPost0;
	wxRadioButton*		bypassPost1;

	wxRadioButton*		postGainSelect0;
	wxRadioButton*		postGainSelect1;

	wxRadioButton*		channel1Sel;
	wxRadioButton*		channel2Sel;
	wxRadioButton*		channel3Sel;
	wxRadioButton*		channel4Sel;
	wxRadioButton*		channel5Sel;
	wxRadioButton*		channel6Sel;
	wxRadioButton*		channel7Sel;
	wxRadioButton*		channel8Sel;
	wxRadioButton*		channel9Sel;
	wxRadioButton*		channel10Sel;
	wxRadioButton*		channel11Sel;
	wxRadioButton*		channel12Sel;
	wxRadioButton*		channel13Sel;
	wxRadioButton*		channel14Sel;
	wxRadioButton*		channel15Sel;
	wxRadioButton*		channel16Sel;
	wxSpinCtrl*		spinGain;

};


#endif /* CONTROL_CONFIGURATION_H */
