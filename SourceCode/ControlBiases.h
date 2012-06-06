#ifndef CONTROL_BIASES_H
#define CONTROL_BIASES_H

#include <wx/wx.h>
#include "ControlPanel.h"
#include <wx/spinctrl.h>

class	DeviceController;

/** Declare ControlBiases event IDs.  These are arbitrary but must be unique. */
enum {
	RESET_BIASES = 500,
	Int_gbt,
	Int_Vbn,
	Int_Vbp,
	Post_gbt,
	Post_Vbn,
	Post_Vbp,
	OBuff_gbt,
	OBuff_Vbn,
	OBuff_Vbp,
	Vref,
	V_Cmd_Offset,
	OffsetCorrection
};

/**
 * Constructs PC3 bias controls.
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
	 * The constructor initializes all controls (sliders, buttons, etc)
	 * contained within the panel and defines its layout.
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

	void OnSliderInt_gbt (wxScrollEvent& evt);
	void OnSliderInt_Vbn (wxScrollEvent& evt);
	void OnSliderInt_Vbp (wxScrollEvent& evt);
	void OnSliderPost_gbt (wxScrollEvent& evt);
	void OnSliderPost_Vbn (wxScrollEvent& evt);
	void OnSliderPost_Vbp (wxScrollEvent& evt);
	void OnSliderOBuff_gbt (wxScrollEvent& evt);
	void OnSliderOBuff_Vbn (wxScrollEvent& evt);
	void OnSliderOBuff_Vbp (wxScrollEvent& evt);
	void OnSliderVref (wxScrollEvent& evt);
	void OnSliderV_Cmd_Offset (wxScrollEvent& evt);
	void OnSpinOffsetCorrection (wxSpinEvent& evt);

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

	int		defaultValueInt_gbt;
	wxSlider*	sliderInt_gbt;
	wxStaticText*	textInt_gbt;

	int		defaultValueInt_Vbn;
	wxSlider*	sliderInt_Vbn;
	wxStaticText*	textInt_Vbn;

	int		defaultValueInt_Vbp;
	wxSlider*	sliderInt_Vbp;
	wxStaticText*	textInt_Vbp;

	int		defaultValuePost_gbt;
	wxSlider*	sliderPost_gbt;
	wxStaticText*	textPost_gbt;

	int		defaultValuePost_Vbn;
	wxSlider*	sliderPost_Vbn;
	wxStaticText*	textPost_Vbn;

	int		defaultValuePost_Vbp;
	wxSlider*	sliderPost_Vbp;
	wxStaticText*	textPost_Vbp;

	int		defaultValueOBuff_gbt;
	wxSlider*	sliderOBuff_gbt;
	wxStaticText*	textOBuff_gbt;

	int		defaultValueOBuff_Vbn;
	wxSlider*	sliderOBuff_Vbn;
	wxStaticText*	textOBuff_Vbn;

	int		defaultValueOBuff_Vbp;
	wxSlider*	sliderOBuff_Vbp;
	wxStaticText*	textOBuff_Vbp;

	int		defaultValueVref;
	wxSlider*	sliderVref;
	wxStaticText*	textVref;

	int		defaultValueV_Cmd_Offset;
	wxSlider*	sliderV_Cmd_Offset;
	wxStaticText*	textV_Cmd_Offset;

	int		defaultValueOffsetCorrection;
	wxSpinCtrl*		spinOffsetCorrection;
};


#endif /* CONTROL_BIASES_H */
