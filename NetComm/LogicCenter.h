#pragma once
#include "vector"
using std::vector;

//获取URL的参数值
vector<CString> GetURLParamValue(CString url);
//读取设备数据回调函数
typedef bool (*callBack_GetDataFunc_t)(int nDut_ID, int nMeter_ID, int nType, long tCommn, BYTE* pData, int nDataLen);
//服务器接收数据回调
typedef bool (*callback_OnServerRecv_t)(CString strURL);

//读取设备数据回调函数
bool callBack_GetDataFunc(int nDut_ID, int nMeter_ID, int nType, long tCommn, BYTE* pData, int nDataLen);
//服务器接收数据回调
bool callback_OnServerRecv(CString strURL);