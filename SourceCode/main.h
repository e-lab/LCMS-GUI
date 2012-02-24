#ifndef MAIN_H
#define MAIN_H

#include <wx/wx.h>

/**
 * Class called by the OS to initialize the app.
 *
 * This class is derived from the wxWidgets base class wxApp.  Every wxWidgets
 * application must have one class so derived and it can be thought of as the
 * entry point for the OS into the application.
 */
class ELabApp : public wxApp
{
public:

	/**
	 * Called on application start-up.
	 *
	 * @retval TRUE
	 *   If successful.
	 * @retval FALSE
	 *   Will terminate application start-up.
	 */
	virtual bool OnInit();
};

#endif /* MAIN_H */
