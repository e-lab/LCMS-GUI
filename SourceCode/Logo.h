#ifndef LOGO_H
#define LOGO_H

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/file.h>

/**
 * Panel upon which the e-Lab logo is drawn.
 *
 * This class currently uses a 'xpm' version of the e-Lab logo which
 * gets compiled into the binary.  While this may make the final
 * executable slightly larger it has the benefit that there are fewer
 * external files needed at run time.
 */
class Logo : public wxPanel
{
public:
	/**
	 * Constructor.
	 *
	 * Within the constructor the bitmap of the logo, used in the OnPaint function,
	 * is created and the size of the panel is set.
	 *
	 * @param owner
	 *   Pointer to the parent wxPanel.
	 */
	Logo (wxWindow* owner);
	/**
	 * Destructor.
	 */
	~Logo();

	/**
	 * Draw the logo image onto the panel.
	 *
	 * In addition to the image, text or other graphics could be added to the
	 * panel when it is drawn, lessening the need to edit the image
	 * directly.
	 *
	 * @param event
	 *   Reference to a wxPaintEvent, generated from within the wxWidgets library.
	 */
	void OnPaint (wxPaintEvent & event);

private:
	wxBitmap* bitmap; //!< The bitmap into which the logo is loaded.
};


#endif /* LOGO_H */
