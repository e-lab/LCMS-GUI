/**
 * @file
 * Displaying Information.
 *
 * This file contains the class which is used to display information
 * in real time.
 */
#include "GraphicsInformation.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#define new DEBUG_NEW
#else
#define DEBUG_NEW new
#endif

// This will display the state and other information.
GraphicsInformation::GraphicsInformation (wxWindow* owner) : wxPanel (owner)
{
	wxSizer* sizerBiases = new wxBoxSizer (wxHORIZONTAL);
	wxStaticBoxSizer* sizerBox= new wxStaticBoxSizer (wxHORIZONTAL, this, wxT ("Device Information"));


	wxStaticText* frameNum = new wxStaticText (this, -1, wxT ("Avail Profile Buffer %:  "));
	textMovie = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	wxStaticText* frameNum2 = new wxStaticText (this, -1, wxT ("Used Measurement Buffer %:  "));
	textMovie2 = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	wxSizer* col = new wxBoxSizer (wxHORIZONTAL);
	col->Add (frameNum);
	col->Add (textMovie, 0, wxLEFT, 10);
	col->Add (frameNum2);
	col->Add (textMovie2, 0, wxLEFT, 10);
	sizerBox->Add (col, 0, wxLEFT | wxRIGHT, 10);
	sizerBiases->Add (sizerBox, 0, wxALL | wxALIGN_CENTER, 10);

	this->SetSizerAndFit (sizerBiases);
}

GraphicsInformation::~GraphicsInformation()
{
}

void GraphicsInformation::SetMovieFrames (int value)  //now this shows the profile buffer
{
	// Convert int to string
	char str[20] = "";
	sprintf (str, "%i", value);
	textMovie->SetLabel (wxString (str, wxConvUTF8));
}

void GraphicsInformation::SetBufferInfo(int profile, int measurement)
{
	char str[20] = "";
	sprintf (str, "%i", profile);
	textMovie->SetLabel (wxString (str, wxConvUTF8));

	char str2[20] ="";
	sprintf (str2, "%i", measurement);
	textMovie2->SetLabel (wxString (str2, wxConvUTF8));
}
