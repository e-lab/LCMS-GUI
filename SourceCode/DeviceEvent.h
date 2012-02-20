#ifndef DEVICE_EVENT_H
#define DEVICE_EVENT_H

#include <wx/wx.h>
#include <map>

#include "Command.h"

extern const wxEventType DEVICE_EVENT;

/**
 * Event class which contains device raw data.
 */
class DeviceEvent : public wxEvent
{
public:
	/**
	 * Constructor.
	 */
	DeviceEvent();
	/**
	 * Copy Constructor.
	 */
	DeviceEvent (const DeviceEvent*);
	/**
	 * Destructor.
	 */
	~DeviceEvent();

	/**
	 * Clone function.
	 */
	DeviceEvent* Clone() const;

	/**
	 * Retrieve the raw data.
	 *
	 * @param dataLength
	 *   Length that the raw data array.
	 * @return data
	 *   Pointer to the first element of the raw data array.
	 */
	unsigned char* GetRawData (int&) const;
	/**
	 * Set the data array for this event.
	 *
	 * Ownership of the data array has been transferred to this class
	 * and thus, this class now has the responsibility of freeing the
	 * memory associated with the array.
	 *
	 * @param data
	 *   Pointer to a unsigned char data array.
	 * @param dataLength
	 *   Length that the data array.
	 */
	void SetRawData (unsigned char*, int&);

	/**
	 * Retrieve the value of the variable.
	 *
	 * @param variable
	 *   ID of the variable.
	 * @param value
	 *   Integer into which the 'variable' value is saved.
	 */
	void GetVariable (Command::CommandID, int&);
	/**
	 * Retrieve the value of the variable.
	 *
	 * @param variable
	 *   ID of the variable.
	 * @return
	 *   Integer value of the 'variable'.
	 */
	int GetVariable (Command::CommandID);
	/**
	 * Set the value of the variable.
	 *
	 * @param variable
	 *   ID of the variable.
	 * @param value
	 *   Integer value of the variable.
	 */
	void SetVariable (Command::CommandID, int);

	DECLARE_DYNAMIC_CLASS (DeviceEvent)

protected:
	std::map<int, int>* CopyVariables() const;	//!<  Used by Clone() to copy all variable states.

private:
	unsigned char* rawData;					//!<  Pointer to an array containing raw data.
	int length;								//!<  Length of the rawData array.

	std::map<int, int>* variables;			//!<  Map to store device variables.

};

typedef void (wxEvtHandler::*deviceEventFunction) (DeviceEvent&);
\

#define EVT_DEVICE_EVENT(func)										\
	DECLARE_EVENT_TABLE_ENTRY(										\
		DEVICE_EVENT,												\
		-1,															\
		wxID_ANY,													\
		(wxObjectEventFunction) (deviceEventFunction) & func,		\
		(wxObject *) NULL											\
	),

#endif /* DEVICE_EVENT_H */
