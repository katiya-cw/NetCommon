#pragma once
#include "stdafx.h"
#include <map>
using std::map;

class CDeviceEvent
{
public:
	static bool InitLock();
	static bool DeletLock();
	static bool Lock();
	static bool UnLock();
	bool    Create(int id);
	HANDLE  GetEvent(int id);
	bool    Delete(int id);
private:
	static map<int, HANDLE> m_devEvent;
	static CRITICAL_SECTION m_section;
};