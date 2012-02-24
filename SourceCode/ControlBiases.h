#ifndef PANEL_OCT_BIASES_H
#define PANEL_OCT_BIASES_H

#include <wx/wx.h>
#include "ControlPanel.h"

class	DeviceController;

/** Declare ControlBiases event IDs.  These are arbitrary but must be unique. */
const int SLIDER_PD_BIAS =		700;
const int SLIDER_VDDA =			701;
const int SLIDER_VDDR =			702;
const int SLIDER_LPU =			703;
const int RESET_OCT_BIASES =		704;

/**
 * Constructs Bias controls.
 *
 * This class is initialized within MainFrame and it is responsible for
 * constructing the graphical components used to control the biases in
 * the device.
 */
class ControlBiases : public ControlPanel
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
	 * @param controller
	 *   Pointer to the DeviceController object.
	 */
	ControlBiases (wxWindow*, DeviceController*);
	/**
	 * Destructor.
	 */
	~ControlBiases();

	/**
	 * Change the PBBias.
	 *
	 * Handles the event which is generated when a 'PBBias' slider is
	 * moved.  The change in position will be sent to the xem device
	 * and the sliders label updated.  This value controls in hardware
	 * the Vout D for the DAC.
	 *
	 * @param event
	 *   Reference to a wxScrollEvent.
	 */
	void OnSliderPBBias (wxScrollEvent&);
	/**
	 * Change the VDDA.
	 *
	 * Handles the event which is generated when the 'VDDA' slider is
	 * moved.  This change in position will be sent to the xem device
	 * and the sliders label updated.
	 *
	 * @param event
	 *   Reference to a wxScrollEvent.
	 */
	void OnSliderVDDA (wxScrollEvent&);
	/**
	 * Change the VDDR.
	 *
	 * Handles the event which is generated when the 'VDDR' slider is
	 * moved.  This change in position will be sent to the xem device
	 * and the sliders label updated.
	 *
	 * @param event
	 *   Reference to a wxScrollEvent.
	 */
	void OnSliderVDDR (wxScrollEvent&);
	/**
	 * Change the LPU.
	 *
	 * Handles the event which is generated when the 'LPU' slider is
	 * moved.  This change in position will be sent to the xem device
	 * and the sliders label updated.  This value controls in hardware
	 * the Vout A for the DAC.

	 *
	 * @param event
	 *   Reference to a wxScrollEvent.
	 */
	void OnSliderLPU (wxScrollEvent&);

	/**
	 * Reset the controls.
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
	 * Reset all sliders and text labels to the default values.
	 */
	void ResetBiasValues();

	/**
	 * Set the PBBias text label to the current slider value.
	 */
	void SetTextPBBias();
	/**
	 * Set the VDDA text label to the current slider value.
	 */
	void SetTextVDDA();
	/**
	 * Set the VDDR text label to the current slider value.
	 */
	void SetTextVDDR();
	/**
	 * Set the LPU text label to the current slider value.
	 */
	void SetTextLPU();

	/**
	 * Send all current control values to the device.
	 */
	void Start();

private:
	DeviceController*	xemDeviceCtrl;

	int			maxValuePBBias;
	int			defaultValuePBBias;
	float			maxVoltagePBBias;
	wxSlider*		sliderPBBias;
	wxStaticText*		textPBBias;

	int			maxValueVDDA;
	int			defaultValueVDDA;
	float			maxVoltageVDDA;
	wxSlider*		sliderVDDA;
	wxStaticText*		textVDDA;

	int			maxValueVDDR;
	int			defaultValueVDDR;
	float			maxVoltageVDDR;
	wxSlider*		sliderVDDR;
	wxStaticText*		textVDDR;

	int			maxValueLPU;
	int			defaultValueLPU;
	float			maxVoltageLPU;
	wxSlider*		sliderLPU;
	wxStaticText*		textLPU;

};


#endif /* PANEL_OCT_BIASES_H */
