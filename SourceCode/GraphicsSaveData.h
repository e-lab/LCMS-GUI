#ifndef GRAPHICS_SAVE_DATA_H
#define GRAPHICS_SAVE_DATA_H

#include <wx/wx.h>
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
		FILENAME,   	// root filename to use
		FINALIZE,	// no more data will be sent
		DATA       	// contains data
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
	SimpleQueue<struct GraphicsSaveData::save_data> messageQueue;

	//wxEvtHandler* display; //!< Pointer to the GraphicsPanel object sending data.

	struct save_data current_message;

	/**
	 * Sava data to the disk.
	 */
	void SaveData ();
};

#endif /* GRAPHICS_SAVE_DATA_H */
