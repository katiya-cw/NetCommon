#pragma once

#include "stdafx.h"

//下载过程回调函数
typedef bool(*CALL_BACK_DOWNLOAD)(int nType, char* pText, int nTextLen);

//数据库执行回调函数
typedef bool(*CALL_BACK_EXCUTE)(int nType, char* pText, int nTextLen);
/*
nType 0 操作成功，其他为错误
pText 错误描述
nTextLen 错误描述的长度
*/

//以下是实时表数据的返回与说明
/*
nDut_ID		DTU编号
nMeter_ID	表编号
nType		表协议类型 1 2 3
tCommn		通信的时间
pData		实时数据，二进制
nDataLen	实时数据长度
*/
//=====================协议V1
/*
0x68,表号，数据长度（总包长度），控制码03，通道数量（1-255），各通道的数据，各通道的数据（从第1个开始，每通道三字节，前两个是数据，高位在前，最后一个是报警状态，1报警，0不报警），  CSL，      CSH，16H

读回的数据是：（2位小数点）
68 07 20 03   08     00 0D   00       08 8B     00    00 06     00    04 46   00    00 32     00 00 00 00 00 00 00 00 00 00   46 14    16
已用电量         剩余电量         已用钱数       剩余钱数       电价                                     CRC效验
0X000D =0.13    0X088B=21.87    0X0006=0.06     0X0446=1094    0X0032=0.50
度             度               元               元            元
*/
//======================协议V2
/*
如下收到数据 07是表号
0x68,表号，数据长度（总包长度），控制码03，通道数量（1-255），各通道的数据，各通道的数据（从第1个开始，每通道三字节，都是数据，高位在前，低位在后），  CSL，      CSH，16H

68 07 20 03   08     00 0D  01       00 01 01       00 06  00      00 03 01    00 00 32       00 01 20     00 02 02   00 00 11    46 14    16

已用电量         剩余电量         已用钱数       剩余钱数       电价        累积流量    瞬时流量     户号    CRC效验
0X000D01 =33.29 0X000101=2.57   0X000600=15.36  0X000301=7.69 0X000032=0.50  0X000120=  0X000202=    17
度              度               元               元            元         288米3     5.14米3/小时  户
*/
//=======================协议V3(RTU)
/*
0x68,表号，数据长度（总包长度），控制码03，通道数量（1-255），各通道的数据，各通道的数据（从第1个开始，每通道三字节，前两个是数据，高位在前，最后一个是报警状态，1报警，0不报警），  CSL，      CSH，16H

读回的数据是：
68 07 20 03   08     00 0D   00       08 8B     00    00 06     00    04 46   00    00 32     00 00 00 00 00 00 00 00 00 00   46 14    16
通道1流量         水位             温度            湿度         其他                                     CRC效验
0X000D =0.13    0X088B=21.87    0X0006=0.06     0X0446=1094    0X0032=0.50
立方米
*/
typedef bool(*CALL_BACK_GET_DATA)(int nDut_ID, int nMeter_ID, int nType, long tCommn, BYTE* pData, int nDataLen);

typedef bool(*CALL_BACK_GET_DATA_GPRS)(int nDtu_ID, int nStatus, long lastTime);

//通信连接断开回调
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
