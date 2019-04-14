#include "stdafx.h"
#include "DeviceEvent.h"

map<int, HANDLE> CDeviceEvent::m_devEvent;
CRITICAL_SECTION CDeviceEvent::m_section;

bool CDeviceEvent::InitLock()
{
	InitializeCriticalSection(&m_section);
	return true;
}
bool CDeviceEvent::DeletLock()
{
	DeleteCriticalSection(&m_section);
	return true;
}

bool CDeviceEvent::Lock()
{
	EnterCriticalSection(&m_section);
	return true;
}

bool CDeviceEvent::UnLock()
{
	LeaveCriticalSection(&m_section);
	return true;
}

bool CDeviceEvent::Create(int id)
{
	Lock();
	if (m_devEvent.find(id) == m_devEvent.end())
	{
		m_devEvent[id] = CreateEvent(NULL, TRUE, FALSE, NULL);		
	}
	else
	{
		ResetEvent(m_devEvent[id]);
	}
	UnLock();
	return true;
}

HANDLE CDeviceEvent::GetEvent(int id)
{
	HANDLE event = NULL;
	Lock();
	if (m_devEvent.find(id) != m_devEvent.end())
	{
		event = m_devEvent[id];
	}
	UnLock();
	return event;
}

bool CDeviceEvent::Delete(int id)
{
	Lock();
	m_devEvent.erase(id);
	UnLock();
	return true;
}
