#ifndef PANEL_CONTROLS_H
#define PANEL_CONTROLS_H

#include <wx/wx.h>

class	Device;

/**
 * Abstract class providing an inherited interface to control panel class.
 *
 * Control panel classes are responsible for construcing the graphical
 * components used to control aspeces of the hardware of software
 * application.
 */
class ControlPanel : public wxPanel
{
public:
	/**
	 * Constructor.
	 *
	 * @param owner
	 *   Pointer to the parent wxWindow derived class.
	 */
	ControlPanel (wxWindow* owner) : wxPanel (owner) {};

	/**
	 * Send all control values to the device.
	 */
	virtual void Start() {};
};


#endif /* PANEL_CONTROLS_H */
