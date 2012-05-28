#ifndef GRAPHICS_SAVE_DATA_H
#define GRAPHICS_SAVE_DATA_H

#include <wx/wx.h>
#include <wx/ffile.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include "SimpleQueue.h"


/**
 * Class providing the ability to save data without blocking main thread.
 *
 * Commands and data are both passed via the same struct & through the same
 * queue. Once the type of struct has been identified (command or data), the
 * command is executed or data written to file as needed.
 */
class GraphicsSaveData : public wxThread
{
public:

	enum save_data_type {
		GSD_SAVE,	// save filename and dir
		GSD_DELETE,	// delete temp save files
		GSD_DATA	// contains data
	};

	struct save_data {
		save_data_type 	type;
		float*		spectrum;
		float*		time;
		int 		length;
		wxString	tile_spectrum;
		wxString	tile_time;
		wxString	filename;
	};

	/**
	 * Constructor.
	 *
	 * @param displayTmp
	 *   Reference of object thats the source of save data.
	 */
	//GraphicsSaveData (wxEvtHandler*);
	GraphicsSaveData ();
	/**
	 * Destructor.
	 */
	~GraphicsSaveData ();

	/**
	 * Thread execution starts here.
	 */
	virtual void *Entry();

	/**
	 * Allows access to the message (struct) queue so other threads can send commands
	 * and data to be saved.
	 *
	 * @retval Message Queue
	 */
	SimpleQueue<struct GraphicsSaveData::save_data>& GetQueue ();

private:
	wxString		default_filename;
	wxFFile 		*tmp_file;
	wxFileName 		*tmp_filename;

	static const int	nb_points_max = 1000000;
	int			nb_points;
	int			nb_files;
	bool			start_new;

	SimpleQueue<struct GraphicsSaveData::save_data> messageQueue;

	//wxEvtHandler* display; //!< Pointer to the GraphicsPanel object sending data.

	struct save_data current_message;

	/**
	 * Move tmp data files to save dir and rename files.
	 */
	void SaveData ();

	/**
	 * Delete tmp data files.
	 */
	void DeleteData ();

	/**
	 * Write data to the file.
	 */
	void WriteData ();
};

#endif /* GRAPHICS_SAVE_DATA_H */
