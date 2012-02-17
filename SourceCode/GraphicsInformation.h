#ifndef GRAPHICS_INFORMATION_H
#define GRAPHICS_INFORMATION_H


#include <wx/wx.h>


/**
 */
class GraphicsInformation : public wxPanel
{
public:
	/**
	 * Constructor.
	 *
	 * The constructor initializes and the information panel.
	 *
	 * @param owner
	 *   Pointer to the parent wxWindow derived class.
	 */
	GraphicsInformation (wxWindow *);
	/**
	 * Destructor.
	 */
	~GraphicsInformation();

	/**
	 * Set number of frames in the movie.
	 */
	void SetMovieFrames (int);

private:
	wxStaticText*	textMovie;

};


#endif /* GRAPHICS_INFORMATION_H */
