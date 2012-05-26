/**
 * @file
 * Contains the class that defines the raw data event.
 */

#include "DeviceEvent.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#define new DEBUG_NEW
#else
#define DEBUG_NEW new
#endif

IMPLEMENT_DYNAMIC_CLASS (DeviceEvent, wxEvent)

const wxEventType DEVICE_EVENT = wxNewEventType();


DeviceEvent::DeviceEvent()
{
	SetEventType (DEVICE_EVENT);

	length = 1;
	rawData = new unsigned char[length];
	rawData[0] = 0;

	inAvailable = 1;
	outAvailable = 1;

	variables = new std::map<int, int>();
}

DeviceEvent::DeviceEvent (const DeviceEvent* original)
{
	SetEventType (DEVICE_EVENT);

	rawData = original->GetRawData (length);
	variables = original->CopyVariables();
	original->GetInAvailable(inAvailable);
	original->GetOutAvailable(outAvailable);
}

DeviceEvent::~DeviceEvent()
{
	delete[] rawData;
	rawData = NULL;

	delete variables;
	variables = NULL;
}

DeviceEvent* DeviceEvent::Clone() const
{
	return (new DeviceEvent (this));
}

std::map<int, int>* DeviceEvent::CopyVariables() const
{
	return (new std::map<int, int> (*variables));
}

unsigned char* DeviceEvent::GetRawData (int& dataLength) const
{         
	dataLength = length;

	unsigned char* data = new unsigned char[dataLength];
	memcpy(data, rawData, dataLength*sizeof(rawData[0]));

	return &data[0];
}

void DeviceEvent::GetInAvailable(int& a) const
{
//	return inAvailable;
	a = inAvailable;
}

void DeviceEvent::GetOutAvailable(int& a) const
{
//	return outAvailable;
	a = outAvailable;
}

void DeviceEvent::SetRawData (unsigned char* data, int& dataLength)
{
	length = dataLength;

	delete[] rawData;
	rawData = data;
}


void DeviceEvent::SetInAvailable(int& buffInAvailable)
{
	inAvailable = buffInAvailable*100/2048;
}

void DeviceEvent::SetOutAvailable(int& buffOutAvailable)
{
	outAvailable = buffOutAvailable*100/2048;
 }

void DeviceEvent::GetVariable (Command::CommandID variable, int& value)
{
	value = (*variables) [variable];
}

int DeviceEvent::GetVariable (Command::CommandID variable)
{
	return (*variables) [variable];
}

void DeviceEvent::SetVariable (Command::CommandID variable, int value)
{
	(*variables) [variable] = value;
}
