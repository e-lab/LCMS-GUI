/**
 * @file
 * Contains the class used to displays the e-Lab logo.
 */
#include "Logo.h"
//#include "elab_logo.xpm"
#include "elab_new_logo.xpm"

#ifdef _DEBUG
    #include <crtdbg.h>
    #define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
    #define new DEBUG_NEW
#else
    #define DEBUG_NEW new
#endif

Logo::Logo (wxWindow* owner) : wxPanel (owner)
{
	// Needed for wxAutoBufferedPaintDC
	//this->SetBackgroundStyle (wxBG_STYLE_CUSTOM);

	// Scaled elab logo
	//wxBitmap logo_xpm(elab_new_logo);
	//wxImage image = logo_xpm.ConvertToImage();
	//image.Rescale(240, 75);
	//bitmap = new wxBitmap(image);
	//this->SetSize(320, 100);

	// Full sized elab logo
	bitmap = new wxBitmap (elab_new_logo);

	// Sets the size of the logo from the size of the bitmap
	SetSize (bitmap->GetWidth(), bitmap->GetHeight());

	// Event handler
	Connect (wxEVT_PAINT, wxPaintEventHandler (Logo::OnPaint));
}

Logo::~Logo()
{
	delete bitmap;
	bitmap = (wxBitmap*) NULL;
}

void Logo::OnPaint (wxPaintEvent& event)
{
	//wxAutoBufferedPaintDC dc (this);
	wxPaintDC dc (this);

	//dc.SetBrush( *wxWHITE_BRUSH );
	//dc.DrawRectangle( -1, -1, 322, 102 );
	//dc.DrawText( wxT("Your Name Here"), 200, 80 );

	dc.DrawBitmap (*bitmap, 0, 0, true);
}
