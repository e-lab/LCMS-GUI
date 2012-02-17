#ifndef DISPLAY_IMAGE_CAVAS_H
#define DISPLAY_IMAGE_CAVAS_H


#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/rawbmp.h>


/**
 * Panel upon which the images are drawn.
 *
 * The traslated data in the form of a pixel array is gets passed to this class
 * and then is displayed when the panel gets a repaint event.
 */
class GraphicsImageCanvas : public wxPanel
{
public:
	/**
	 * Constructor.
	 *
	 * The constructor initializes and sets up the image area and readies
	 * the class for receiving data.
	 *
	 * @param owner
	 *   Pointer to the parent wxWindow derived class.
	 */
	GraphicsImageCanvas (wxWindow *);
	/**
	 * Destructor.
	 */
	~GraphicsImageCanvas();

	/**
	 * Set pixel data array used to create that bitmap image.
	 */
	void SetData (int, int, float*, int);
	/**
	 * Returns a pointer to the data being displayed.
	 */
	float* GetImageData (int&);

	/**
	 * Handle a repaint event, draws the bitmap image for display.
	 */
	void OnPaint (wxPaintEvent&);
	/**
	 * Used to trigger full screen mode.
	 */
	void OnDoubleClick (wxMouseEvent&);

private:
	int		width;				//!< Width of the image.
	int		height;				//!< Height of the image.
	int		imageType;
	int		length;				//!< Length of both the current and time arrays
	float*		imageData;			//!< Array containg image information.

	wxBitmap*	image;				//!< The bitmap into which the image is loaded.
	wxFrame*	fullscreen;			//!< Frame used in fullscreen mode.

};

#endif /* DISPLAY_IMAGE_CAVAS_H */
