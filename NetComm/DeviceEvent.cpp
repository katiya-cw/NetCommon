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

bool    CDeviceEvent::Create(int id)
{
	if (m_devEvent.find(id) == m_devEvent.end())
	{
		Lock();
		m_devEvent[id] = CreateEvent(NULL, TRUE, FALSE, NULL);		
		UnLock();
	}
	else
	{
		ResetEvent(m_devEvent[id]);
	}
	return true;
}

HANDLE  CDeviceEvent::GetEvent(int id)
{
	if (m_devEvent.find(id) != m_devEvent.end())
	{
		return m_devEvent[id];
	}
	else
	{
		return NULL;
	}
}

bool    CDeviceEvent::Delete(int id)
{
	Lock();
	m_devEvent.erase(id);
	UnLock();
	return true;
}