/**
 * @file
 * Contains class that saves data to disk.
 */
#include "GraphicsSaveData.h"

// When debugging changes all calls to "new" to be calls to "DEBUG_NEW" allowing for memory leaks to
// give you the file name and line number where it occurred.
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#define new DEBUG_NEW
#else
#define DEBUG_NEW new
#endif


//GraphicsSaveData::GraphicsSaveData (wxEvtHandler* displayTmp) : wxThread (wxTHREAD_JOINABLE)
GraphicsSaveData::GraphicsSaveData () : wxThread (wxTHREAD_JOINABLE)
{
	//display = displayTmp;
	nb_points = 0;
	nb_files = 0;
	start_new = true;
	default_filename = wxT ("tmp_save_file_");

	tmp_filename = new wxFileName(wxT ("tmp_save"), wxString::Format(wxT ("%s%i"), default_filename, 0), wxT ("txt"));
	if (!tmp_filename->DirExists()) {
		tmp_filename->Mkdir();
	}

	tmp_file = new wxFFile (tmp_filename->GetFullPath (), wxT("w"));
}

GraphicsSaveData::~GraphicsSaveData()
{
	delete tmp_filename;
	tmp_filename = (wxFileName*) NULL;

	delete tmp_file;
	tmp_file = (wxFFile*) NULL;
}

wxThread::ExitCode GraphicsSaveData::Entry()
{

	//while (!TestDestroy()) {
	while (1) {

		// Process messages
		wxMessageQueueError result = messageQueue.ReceiveTimeout (10, current_message);

		if (wxMSGQUEUE_NO_ERROR == result) {

			switch (current_message.type) {
				case GraphicsSaveData::GSD_SAVE :
					SaveData ();
					break;
				case GraphicsSaveData::GSD_DELETE :
					DeleteData ();
					break;
				case GraphicsSaveData::GSD_DATA :
					WriteData ();
					break;
				default :
					break;
			}

		} else if (wxMSGQUEUE_TIMEOUT == result) {
			wxThread::Sleep(100);
		}
	}
	return (wxThread::ExitCode) 0;
}


SimpleQueue<struct GraphicsSaveData::save_data>& GraphicsSaveData::GetQueue ()
{
	return messageQueue;
}

void GraphicsSaveData::SaveData ()
{
	wxFileName *new_full_path = new wxFileName(current_message.filename);
	wxString new_name = new_full_path->GetName ();

	// close temp file that is being written to
	tmp_file->Flush ();
	tmp_file->Close ();
	delete tmp_file;
	tmp_file = (wxFFile*) NULL;

	// move temp file to save file dir with new name
	wxArrayString *fileList = new wxArrayString();
	fileList->Clear();
	wxDir::GetAllFiles (tmp_filename->GetPath (), fileList, wxT ("*.txt"), wxDIR_FILES);
	for (unsigned int xx = 0; xx < fileList->GetCount(); xx++) {
		// make new name
		new_full_path->SetName (wxString::Format(wxT ("%s_%i"), new_name, xx));

		// move to new dir with new name
		wxRenameFile((fileList->Item (xx)).GetData (), new_full_path->GetFullPath ());
	}
	delete fileList;
	fileList = (wxArrayString*) NULL;

	// open new temp file
	nb_files = 0;
	tmp_filename->SetName (wxString::Format(wxT ("%s%i"), default_filename, nb_files));
	tmp_file = new wxFFile (tmp_filename->GetFullPath (), wxT("w"));
	start_new = true;
}

void GraphicsSaveData::DeleteData ()
{
	// close temp file that is being written to
	delete tmp_file;
	tmp_file = (wxFFile*) NULL;

	// delete all tmp file in tmp dir.
	wxArrayString *fileList = new wxArrayString();
	fileList->Clear();
	wxDir::GetAllFiles (tmp_filename->GetPath (), fileList, wxT ("*.txt"), wxDIR_FILES);
	for (unsigned int xx = 0; xx < fileList->GetCount(); xx++) {
		wxRemoveFile((fileList->Item (xx)).GetData ());
	}
	delete fileList;
	fileList = (wxArrayString*) NULL;

	// open new temp file
	nb_files = 0;
	tmp_filename->SetName (wxString::Format(wxT ("%s%i"), default_filename, nb_files));
	tmp_file = new wxFFile (tmp_filename->GetFullPath (), wxT("w"));
	start_new = true;
}

void GraphicsSaveData::WriteData ()
{
	// timestap new file
	if (start_new) {
		wxString timeStamp = wxT ("# ");
		timeStamp << wxNow() << wxT ("\n");
		tmp_file->Write (timeStamp);

		wxString tileLine = wxT ("# ");
		tileLine << current_message.tile_time << wxT ("\t") << current_message.tile_spectrum << wxT ("\n");
		tmp_file->Write (tileLine);

		start_new = false;
	}

	// write to temp data file
	for (int xx = 0; xx < current_message.length; xx++) {
		wxString dataLine = wxT ("");
		dataLine << current_message.time[xx] << wxT ("\t") << current_message.spectrum[xx] << wxT ("\n");
		tmp_file->Write (dataLine);
		nb_points++;
	}

	delete[] current_message.time;
	delete[] current_message.spectrum;

	// if file is 1,000,000 lines, start new file.
	if (nb_points_max <= nb_points) {
		tmp_file->Flush ();
		tmp_file->Close ();
		delete tmp_file;
		tmp_file = (wxFFile*) NULL;

		nb_points = 0;

		nb_files++;
		tmp_filename->SetName (wxString::Format(wxT ("%s%i"), default_filename, nb_files));
		tmp_file = new wxFFile (tmp_filename->GetFullPath (), wxT("w"));
		start_new = true;
	}
}
