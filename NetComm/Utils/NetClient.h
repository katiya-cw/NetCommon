#pragma once

#include "stdafx.h"

//���ع��̻ص�����
typedef bool(*CALL_BACK_DOWNLOAD)(int nType, char* pText, int nTextLen);

//���ݿ�ִ�лص�����
typedef bool(*CALL_BACK_EXCUTE)(int nType, char* pText, int nTextLen);
/*
nType 0 �����ɹ�������Ϊ����
pText ��������
nTextLen ���������ĳ���
*/

//������ʵʱ�����ݵķ�����˵��
/*
nDut_ID		DTU���
nMeter_ID	����
nType		��Э������ 1 2 3
tCommn		ͨ�ŵ�ʱ��
pData		ʵʱ���ݣ�������
nDataLen	ʵʱ���ݳ���
*/
//=====================Э��V1
/*
0x68,��ţ����ݳ��ȣ��ܰ����ȣ���������03��ͨ��������1-255������ͨ�������ݣ���ͨ�������ݣ��ӵ�1����ʼ��ÿͨ�����ֽڣ�ǰ���������ݣ���λ��ǰ�����һ���Ǳ���״̬��1������0����������  CSL��      CSH��16H

���ص������ǣ���2λС���㣩
68 07 20 03   08     00 0D   00       08 8B     00    00 06     00    04 46   00    00 32     00 00 00 00 00 00 00 00 00 00   46 14    16
���õ���         ʣ�����         ����Ǯ��       ʣ��Ǯ��       ���                                     CRCЧ��
0X000D =0.13    0X088B=21.87    0X0006=0.06     0X0446=1094    0X0032=0.50
��             ��               Ԫ               Ԫ            Ԫ
*/
//======================Э��V2
/*
�����յ����� 07�Ǳ��
0x68,��ţ����ݳ��ȣ��ܰ����ȣ���������03��ͨ��������1-255������ͨ�������ݣ���ͨ�������ݣ��ӵ�1����ʼ��ÿͨ�����ֽڣ��������ݣ���λ��ǰ����λ�ں󣩣�  CSL��      CSH��16H

68 07 20 03   08     00 0D  01       00 01 01       00 06  00      00 03 01    00 00 32       00 01 20     00 02 02   00 00 11    46 14    16

���õ���         ʣ�����         ����Ǯ��       ʣ��Ǯ��       ���        �ۻ�����    ˲ʱ����     ����    CRCЧ��
0X000D01 =33.29 0X000101=2.57   0X000600=15.36  0X000301=7.69 0X000032=0.50  0X000120=  0X000202=    17
��              ��               Ԫ               Ԫ            Ԫ         288��3     5.14��3/Сʱ  ��
*/
//=======================Э��V3(RTU)
/*
0x68,��ţ����ݳ��ȣ��ܰ����ȣ���������03��ͨ��������1-255������ͨ�������ݣ���ͨ�������ݣ��ӵ�1����ʼ��ÿͨ�����ֽڣ�ǰ���������ݣ���λ��ǰ�����һ���Ǳ���״̬��1������0����������  CSL��      CSH��16H

���ص������ǣ�
68 07 20 03   08     00 0D   00       08 8B     00    00 06     00    04 46   00    00 32     00 00 00 00 00 00 00 00 00 00   46 14    16
ͨ��1����         ˮλ             �¶�            ʪ��         ����                                     CRCЧ��
0X000D =0.13    0X088B=21.87    0X0006=0.06     0X0446=1094    0X0032=0.50
������
*/
typedef bool(*CALL_BACK_GET_DATA)(int nDut_ID, int nMeter_ID, int nType, long tCommn, BYTE* pData, int nDataLen);

typedef bool(*CALL_BACK_GET_DATA_GPRS)(int nDtu_ID, int nStatus, long lastTime);

//ͨ�����ӶϿ��ص�
typedef bool(*CALL_BACK_SHUTDOWN)();

typedef struct _GET_DATA_
{
	CALL_BACK_GET_DATA callGetBack;
	CALL_BACK_EXCUTE callExcuteBack;
	CALL_BACK_SHUTDOWN callShutdownBack;

	CALL_BACK_GET_DATA_GPRS callGetBack2;

	SOCKET sSocket;
	BOOL bConnect;

}_GET_DATA_;


typedef struct _DOWNLOAD_DATA_
{
	char name[MAX_PATH];
	CALL_BACK_DOWNLOAD callBack;
	SOCKET sSocket;

}_DOWNLOAD_DATA_;

int NetComm(char* pServerIP, int nPort, CALL_BACK_GET_DATA callGetBack,
	CALL_BACK_GET_DATA_GPRS callGetBack2, CALL_BACK_EXCUTE callExcuteBack,
	CALL_BACK_SHUTDOWN callShutdownBack);
int DownLoadFile(char* pServerIP, int nPort, char* strSaveFile, CALL_BACK_DOWNLOAD callBack);
int myfction(int data1);
int NetSendMyData(int DtuID, unsigned char MepID, unsigned char *pData, int nLen);
int NetSendMyDataTrue(int DtuID, unsigned char nMeter_ID, int money, int delayTime);
int NetSendMyData(int DtuID, unsigned char MepID, unsigned char *pData, int nLen);
int ___CheckAuth();
void MaskRem(CString strKey, CString strData, CString & strNew);
UINT ______AFX_DOWN_LOAD(LPVOID pParam);
UINT ______AFX_CHECK_METER(LPVOID pParam);
int tcpreceive(SOCKET s, char *pchbuf, int nsize, int ntimeoverSec);
unsigned char MENGsendToDtu(unsigned long DevID, unsigned char Dev_id, unsigned long money, unsigned long deyTime);

int NetSendMyAccount(int DtuID, unsigned char nMeter_ID, int OKorNO);
int MENGsendDown_Data(int DtuID, unsigned char nMeter_ID, unsigned int* Down_Data, unsigned char ttm);
