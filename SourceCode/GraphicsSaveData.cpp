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
}

GraphicsSaveData::~GraphicsSaveData()
{

}

wxThread::ExitCode GraphicsSaveData::Entry()
{

	//while (!TestDestroy()) {
	while (1) {

		// Process messages
		if (wxMSGQUEUE_NO_ERROR == messageQueue.Receive (current_message)) {

			switch (current_message.type) {
				case GraphicsSaveData::FILENAME :
					break;
				case GraphicsSaveData::FINALIZE :
					break;
				case GraphicsSaveData::DATA :
					SaveData ();
					break;
				default :
					break;
			}
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

}
