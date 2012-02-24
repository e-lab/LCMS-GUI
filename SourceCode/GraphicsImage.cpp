/**
 * @file
 * The Image Graphics System.
 *
 * This file contains the class which is used to display the image
 * canvas and information panel.
 */
#include "GraphicsImage.h"

#include "GraphicsImageCanvas.h"
#include "GraphicsInformation.h"
#include "DeviceEvent.h"

GraphicsImage::GraphicsImage (wxWindow* owner) : wxPanel (owner)
{
	wxBoxSizer* sizerImage = new wxBoxSizer (wxVERTICAL);

	canvas = new GraphicsImageCanvas (this);
	//sizerImage->Add (canvas, 1, wxSHAPED | wxALIGN_CENTER);
	sizerImage->Add (canvas, 1, wxEXPAND | wxALIGN_CENTER | wxTOP, 10);

	information = new GraphicsInformation (this);
	sizerImage->Add (information, 0, wxALIGN_CENTER);

	y_coor= new int[1];
	y_coor[0] = 0;

	x_coor = new int[1];
	x_coor[0] = 0;

	timeStamp = new int[1];
	timeStamp[0] = 0;

	SetSizerAndFit (sizerImage);
}

GraphicsImage::~GraphicsImage()
{
	delete canvas;
	canvas = (GraphicsImageCanvas*) NULL;

	delete information;
	information = (GraphicsInformation*) NULL;

	delete[] x_coor;
	x_coor = NULL;

	delete[] y_coor;
	y_coor = NULL;

	delete[] timeStamp;
	timeStamp = NULL;

	if (movie.size() > 0) {
		while (movie.size() > 0) {
			delete (movie.back());
			movie.pop_back();
		}
	}
}

void GraphicsImage::OnDeviceEvent (DeviceEvent& rawEvent)
{
	int saveType = rawEvent.GetVariable (Command::SAVE_TYPE);
	if (1 == saveType) {
		while (movie.size() > 0) {
			delete (movie.back());
			movie.pop_back();
		}
	}
	movie.push_back (rawEvent.Clone());
	information->SetMovieFrames (movie.size());

	UnpackEvent (rawEvent);
}

void GraphicsImage::UnpackEvent (DeviceEvent& rawEvent)
{
	int width = rawEvent.GetVariable (Command::IMG_WIDTH);
	int height = rawEvent.GetVariable (Command::IMG_HEIGHT);

	if (0 == width || 0 == height) {
		return;
	}

	int rawLength;
	unsigned char* rawData = rawEvent.GetRawData (rawLength);

	int length = (width * height);
	float* imageData = new float[length];
	for (int ii = 0; ii < length; ii++) {
		imageData[ii] = 0;
	}

	//eventsLength = rawLength/2;
	eventsLength = rawLength/4;
	delete[] y_coor;
	y_coor = new int[eventsLength];

	delete[] x_coor;
	x_coor = new int[eventsLength];

	delete[] timeStamp;
	timeStamp = new int[eventsLength];

	for (int ii = 0; ii < eventsLength; ii++) {
		//int x = (~rawData[2*ii]) & 0x007F;
		//int y = (~rawData[2*ii + 1]) & 0x003F;

		int x = (~rawData[4*ii]) & 0x007F;
		int y = (~rawData[4*ii + 1]) & 0x003F;

		y_coor[ii] = y;
		x_coor[ii] = x;

		int addr = (y * width) + x;
		if (addr >=0 && addr < length) {
			imageData[addr] = (imageData[addr]) + 1;
		}

		int time_stamp_a = (int) rawData[4*ii + 2] & 0x00FF;
		int time_stamp_b = (int) (rawData[4*ii + 3] & 0x00FF) << 8;

		timeStamp[ii] = (time_stamp_a + time_stamp_b);
	}

	delete[] rawData;

	// Note: ownership of imageData array is now with canvas.
	canvas->SetData (width, height, imageData, length);
	canvas->Refresh (true, NULL); // Causes the image panel to be repainted.
}

void GraphicsImage::SetCommandString (Command::CommandID command, wxString string)
{
	switch (command) {
		case Command::SAVE_DATA:
			SaveData (string);
			break;
		default :
			break;
	}
}

void GraphicsImage::SaveData (wxString outputFile)
{
	unsigned int ii = 0;
	while (ii < movie.size()) {
		wxFileName fileName = wxFileName (outputFile);
		if (!fileName.HasExt()) {
			fileName.SetExt (wxT ("dat"));
		}

		if (movie.size() > 1) {
			fileName.SetName (fileName.GetName() << wxT ("_") << ii);
		}

		if (fileName.FileExists()) {
			::wxMessageBox (wxT ("A file with the current save name already exists.\n\n"
			                     "File Name:  ") + fileName.GetFullName(),
			                wxT ("Save Aborted"));

			return;
		}

		wxTextFile fileOut;
		if (!fileOut.Create (fileName.GetFullPath())) {
			::wxMessageBox (wxT ("The file could not be created.\n"),
			                wxT ("File Creation Failed"));
			return;
		}

		UnpackEvent (*movie.at (ii));

		wxString header = wxT ("# x\ty\ttime stamp (microseconds)\n");
		fileOut.AddLine (header);

		unsigned char x;
		unsigned char y;
		int t;

		for (int i = 0; i < eventsLength; i++) {
			x = x_coor[i];
			y = y_coor[i];
			t = timeStamp[i];

			wxString line;
			line << (int) x << wxT ("\t") << (int) y << wxT ("\t") << (int) t;
			fileOut.AddLine (line);
		}
		fileOut.Write();
		fileOut.Close();
		ii++;
	}
}

