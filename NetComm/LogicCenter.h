#pragma once
#include "vector"
using std::vector;

//��ȡURL�Ĳ���ֵ
vector<CString> GetURLParamValue(CString url);
//��ȡ�豸���ݻص�����
typedef bool (*callBack_GetDataFunc_t)(int nDut_ID, int nMeter_ID, int nType, long tCommn, BYTE* pData, int nDataLen);
//�������������ݻص�
typedef bool (*callback_OnServerRecv_t)(CString strURL);

//��ȡ�豸���ݻص�����
bool callBack_GetDataFunc(int nDut_ID, int nMeter_ID, int nType, long tCommn, BYTE* pData, int nDataLen);
//�������������ݻص�
bool callback_OnServerRecv(CString strURL);