#ifndef PANEL_LOGGING_H
#define PANEL_LOGGING_H

#include <wx/wx.h>

// Declare event IDs. These are arbitrary but must be unique.
const int TEXTBOX1 = 500;

/**
 * Constructs and shows log panel.
 *
 * This class is initialized within MainFrame and it is responsible for
 * constructing the log used to print action messages from within the
 * application.
 */
class Logging : public wxPanel
{
public:
	/**
	 * Constructor.
	 *
	 * The constructor initializes a wxTextCtrl which is then set as the wxLog
	 * target.  A message can be written to this object from anywhere in the
	 * application by using the following functions, which has the same
	 * arguments as a printf() command:
	 *
	 * - ::wxLogMessage(wxT("Message to be written"));
	 * - ::wxLogMessage(wxT("Print a int number:  %i"), 1);
	 *
	 * Other functions that can be used can be found at the following
	 * address:-
	 *
	 * http://docs.wxwidgets.org/2.8.6/wx_logfunctions.html#logfunctions
	 *
	 * @param owner
	 *   Pointer to the parent wxWindow derived class.
	 */
	Logging (wxWindow *);
	/**
	 * Destructor.
	 */
	~Logging();

private:
	wxTextCtrl* textLog;

};

#endif /* PANEL_LOGGING_H */
