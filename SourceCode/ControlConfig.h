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
	CAPSELECT2,
	RES_SELECT0,
	RES_SELECT1,
	BYPASSPOST0,
	BYPASSPOST1,
	POSTGAINSELECT0,
	POSTGAINSELECT1,
	BYPASS_CDS0,
	BYPASS_CDS1,
	INFILTER_SELN0,
	INFILTER_SELN1,
	CHANNEL0_SEL,
	CHANNEL1_SEL,
	CHANNEL2_SEL,
	CHANNEL3_SEL,
	CHANNEL4_SEL,
	CHANNEL5_SEL,
	CHANNEL6_SEL,
	CHANNEL7_SEL,
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
	void OnCapSelect2 (wxCommandEvent& evt);
	void OnResSelect0 (wxCommandEvent& evt);
	void OnResSelect1 (wxCommandEvent& evt);
	void OnInFiltNSel0 (wxCommandEvent& evt);
	void OnInFiltNSel1 (wxCommandEvent& evt);
	void OnChannel0Sel (wxCommandEvent& evt);
	void OnChannel1Sel (wxCommandEvent& evt);
	void OnChannel2Sel (wxCommandEvent& evt);
	void OnChannel3Sel (wxCommandEvent& evt);
	void OnChannel4Sel (wxCommandEvent& evt);
	void OnChannel5Sel (wxCommandEvent& evt);
	void OnChannel6Sel (wxCommandEvent& evt);
	void OnChannel7Sel (wxCommandEvent& evt);
	void OnBypassPost0 (wxCommandEvent& evt);
	void OnBypassPost1 (wxCommandEvent& evt);
	void OnPostGainSelect0 (wxCommandEvent& evt);
	void OnPostGainSelect1 (wxCommandEvent& evt);
	void OnBypassCDS0 (wxCommandEvent& evt);
	void OnBypassCDS1 (wxCommandEvent& evt);
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
	wxRadioButton*		capSelect2;

	wxRadioButton*		resSelect0;
	wxRadioButton*		resSelect1;

	wxRadioButton*		bypassPost0;
	wxRadioButton*		bypassPost1;

	wxRadioButton*		postGainSelect0;
	wxRadioButton*		postGainSelect1;

	wxRadioButton*		bypassCDS0;
	wxRadioButton*		bypassCDS1;

	wxRadioButton*		inFiltSelN0;
	wxRadioButton*		inFiltSelN1;

	wxRadioButton*		channel0Sel;
	wxRadioButton*		channel1Sel;
	wxRadioButton*		channel2Sel;
	wxRadioButton*		channel3Sel;
	wxRadioButton*		channel4Sel;
	wxRadioButton*		channel5Sel;
	wxRadioButton*		channel6Sel;
	wxRadioButton*		channel7Sel;
	wxSpinCtrl*		spinGain;

};


#endif /* CONTROL_CONFIGURATION_H */
