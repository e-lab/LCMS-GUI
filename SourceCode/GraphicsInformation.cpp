/**
 * @file
 * Displaying Information.
 *
 * This file contains the class which is used to display information
 * in real time.
 */
#include "GraphicsInformation.h"

// This will display the state and other information.
GraphicsInformation::GraphicsInformation (wxWindow* owner) : wxPanel (owner)
{
	wxSizer* sizerBiases = new wxBoxSizer (wxHORIZONTAL);
	wxStaticBoxSizer* sizerBox= new wxStaticBoxSizer (wxHORIZONTAL, this, wxT ("Device Information"));


	wxStaticText* frameNum = new wxStaticText (this, -1, wxT ("Number of movie frames in save buffer:  "));
	textMovie = new wxStaticText (this, -1, wxT ("0"), wxDefaultPosition, wxSize (70, 20), wxALIGN_LEFT);
	wxSizer* col = new wxBoxSizer (wxHORIZONTAL);
	col->Add (frameNum);
	col->Add (textMovie, 0, wxLEFT, 10);
	sizerBox->Add (col, 0, wxLEFT | wxRIGHT, 10);
	sizerBiases->Add (sizerBox, 0, wxALL | wxALIGN_CENTER, 10);

	this->SetSizerAndFit (sizerBiases);
}

GraphicsInformation::~GraphicsInformation()
{
}

void GraphicsInformation::SetMovieFrames (int value)
{
	// Convert int to string
	char str[20] = "";
	sprintf (str, "%i", value);
	textMovie->SetLabel (wxString (str, wxConvUTF8));
}
