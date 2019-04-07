#if !defined(________H_________)
#define ________H_________
#include "../stdafx.h"
#include "../../MyDefine.h"
#include "../DeviceEvent.h"
#include "db/DbOperate.h""
#include <vector>
//����ͷ�ļ�   
#include "MD5/common.h"  
extern void myOUTerr(unsigned char err);
extern unsigned char MENGsendToDtu(unsigned long DevID, unsigned char Dev_id, unsigned long money, unsigned long deyTime);
extern void SendBACKtoPHPserver(CString text);
//���͵��ͻ����б���ʾ
//extern  void OutputClientString(CString str);
extern int MyEmpID;
extern int connectFlgNum;
extern int SQLwriteFlag2;
extern int myDTU_ID2;
extern int myID2;

extern int SQLwriteFlag3;
extern int SQLwriteFlag4;
extern int myDTU_ID3;


extern CString gettime();
extern CString gettime2();

unsigned char myREVflag=0; 
int		mynDut_ID;
int     mynMeter_ID;
//////////////////////////
inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55 : x + 48;
}
CString URLEncode(CString sIn)
{
	CString sOut;

	const int nLen = sIn.GetLength() + 1;

	register LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	register LPBYTE pInTmp = NULL;
	LPBYTE pInBuf = (LPBYTE)sIn.GetBuffer(nLen);
	BYTE b = 0;

	//alloc out buffer
	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen * 3 - 2);//new BYTE [nLen  * 3];

	if (pOutBuf)
	{
		pInTmp = pInBuf;
		pOutTmp = pOutBuf;

		// do encoding
		while (*pInTmp)
		{
			if (isalnum(*pInTmp))
				*pOutTmp++ = *pInTmp;
			else
				if (isspace(*pInTmp))
					*pOutTmp++ = '+';
				else
				{
					*pOutTmp++ = '%';
					*pOutTmp++ = toHex(*pInTmp >> 4);
					*pOutTmp++ = toHex(*pInTmp % 16);
				}
			pInTmp++;
		}
		*pOutTmp = '\0';
		//sOut=pOutBuf;
		//delete [] pOutBuf;
		sOut.ReleaseBuffer();
	}
	sIn.ReleaseBuffer();
	return sOut;
}


////////////////////////////

//���ع��̻ص�����
typedef bool (*CALL_BACK_DOWNLOAD)(int nType,char* pText,int nTextLen);
/*
nType ���� 0 �������  1  ���ʹ���  2 ���մ��� 3 �汾����  4 ���ش��� 5 �������� 9 ��ʱ
pText ����
nTextLen �����ĳ���
*/
/*
//���ͨ�Žṹ
struct _DATA_METER
{	
	int nDTU_ID;//DTU���
	int nID;//ID �豸���
	
	int nType1;//���� == 1

	int nUedE;//���õ��� �� 2λС��
	BYTE nUedE_E;//���õ��� ������ 1���� 0������
	
	int nLeftE;//ʣ����� �� 2λС��
	BYTE nLeftE_E;//ʣ����� ������ 1���� 0������
	
	int nUsedM;//����Ǯ Ԫ 2λС��
	BYTE nUsedM_E;//����Ǯ ������ 1���� 0������
	
	int nLeftM;//ʣ��Ǯ Ԫ
	BYTE nLeftM_E;//ʣ��Ǯ ������ 1���� 0������
	
	int nRate;//���� Ԫ/�� 2λС��
	BYTE nRate_E;//���� ������ 1���� 0������
	
	int nFlow;//���� ������ 2λС��
	BYTE nFlow_E;//���� ������ 1���� 0������
	
	int nFlow2;//˲������ ������/Сʱ 2λС��
	BYTE nFlow2_E;//˲������  ������ 1���� 0������
	
	int nUID;//��ǰ�û�ID
	BYTE nUID_E;//��ǰ�û�ID ������ 1���� 0������
	
	long lTime;//����ϴ���ʱ�� ��19
};

struct _DATA_METER_2//2.0�汾 3���ֽ����� û�б���
{	
	int nDTU_ID;//DTU���
	int nID;//ID �豸���
	
	int nType2;//���� == 2

	int nUedE;//���õ��� �� 2λС��
	BYTE nUedE_E;// == 0
	
	int nLeftE;//ʣ����� �� 2λС��
	BYTE nLeftE_E;// == 0
	
	int nUsedM;//����Ǯ Ԫ 2λС��
	BYTE nUsedM_E;// == 0
	
	int nLeftM;//ʣ��Ǯ Ԫ 2λС��
	BYTE nLeftM_E;// == 0
	
	int nRate;//���� Ԫ/�� 2λС��
	BYTE nRate_E;// == 0
	
	int nFlowTotal;//�ۻ����� ������
	BYTE nFlowTotal_E;// == 0
	
	int nFlow2;//˲������ ������/Сʱ 2λС��
	BYTE nFlow2_E;// == 0
	
	int nUID;//��ǰ�û�ID
	BYTE nUID_E;// == 0
	
	long lTime;//����ϴ���ʱ�� ��19
};

struct _DATA_RTU
{	
	int nDTU_ID;//DTU���

	int nID;//ID �豸���

	int nType3;//���� == 3
	
	int nFlow;//���� ������ 2λС��
	BYTE nFlow_E;// == 0
	
	int nWater;//ˮλ
	BYTE nWater_E;// == 0
	
	int nTempr;//�¶�  2λС��
	BYTE nTempr_E;//==0
	
	int nHumidity;//ʪ��
	BYTE nHumidity_E;//==0
	
	int nOther1;//����1
	BYTE nOther1_E;//==0
	
	int nOther2;//����2
	BYTE nOther2_E;//==0
	
	int nOther3;//����3
	BYTE nOther3_E;//==0
	
	int nOther4;//����4
	BYTE nOther4_E;//==0
	
	long lTime;//����ϴ���ʱ�� ��19
};
*/
//ʵʱͨ�Żص�����,ʵʱ�ص�ͨ�ŷ������ı��״̬����
//typedef bool (*CALL_BACK_GET_DATA)(_DATA_METER* pData);

//ʵʱͨ�Żص�����,ʵʱ�ص�ͨ�ŷ�������DTU��״̬����
//typedef bool (*CALL_BACK_GET_DATA_GPRS)(int id,int nStatus,long lastTime);

//ʵʱͨ�Żص�����,ʵʱ�ص�ͨ�ŷ������ı��״̬����
//typedef bool (*CALL_BACK_GET_DATA2)(_DATA_METER_CHAR* pData);
/*
pData ʵʱ����
*/

//���ݿ�ִ�лص�����
typedef bool (*CALL_BACK_EXCUTE)(int nType,char* pText,int nTextLen);
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
typedef bool (*CALL_BACK_GET_DATA)(int nDut_ID,int nMeter_ID,int nType,long tCommn,BYTE* pData,int nDataLen);

typedef bool (*CALL_BACK_GET_DATA_GPRS)(int nDtu_ID,int nStatus,long lastTime);

//ͨ�����ӶϿ��ص�
typedef bool (*CALL_BACK_SHUTDOWN)();

//�������ݿ�
//int __stdcall DownLoadFile(char* pServerIP,int nPort,char* strSaveFile,CALL_BACK_DOWNLOAD callBack);
/*
���� 
0 ����׼����ʼ����
1 ���ش���
2 �󶨴���
3 ���Ӵ���

����
pServerIP ��������ַ
nPort     �������˿�
strSaveFile ��Ҫ������ļ�������·��
CALL_BACK_DOWNLOAD ���ع��̻ص�
*/

//ʵʱͨ�Žӿ�
//int __stdcall NetComm(char* pServerIP,int nPort,CALL_BACK_GET_DATA callGetBack,CALL_BACK_GET_DATA_GPRS callGetBack2,CALL_BACK_EXCUTE callExcuteBack,CALL_BACK_SHUTDOWN callShutdownBack);
//int __stdcall NetComm(char* pServerIP,int nPort,CALL_BACK_GET_DATA callGetBack,CALL_BACK_GET_DATA_GPRS callGetBack2,CALL_BACK_EXCUTE callExcuteBack,CALL_BACK_SHUTDOWN callShutdownBack);
/*
���� 
0 ���ӳɹ�
1 ���ش���
2 �󶨴���
3 ���Ӵ���

 ����
pServerIP, ��������ַ
nPort, �������˿�

callGetBack, ʵʱ���ݻص�
callGetBack2, ʵʱ���ݻص�
callExcuteBack, ִ�����ݿ�ص�
callShutdownBack ���ӶϿ�
*/

//�����ݿ�Ĳ�������׼SQL��䣬��ɺ��ʧ�ܺ��ͨ���ص���ʾ
//int __stdcall NetSend(char *pData,int nLen);
/*
���� ���ط��͵����ݳ��� -2δ����

  ����
pData ��Ҫ���͵��ַ���
nLen  �ַ�������
*/

//�ڵ���DLLʱ�������ĺ���ʱһ��Ҫ�ں���ǰ�����FAR PASCAL����������ǳ���ͻȻ�Ĵ���
typedef int  (FAR PASCAL   *MyDownLoadFile)(char* pServerIP,int nPort,char* strSaveFile,CALL_BACK_DOWNLOAD callBack);

//typedef  int  (*MyNetComm)(char* pServerIP,int nPort,CALL_BACK_GET_DATA callGetBack,CALL_BACK_GET_DATA_GPRS callGetBack2,CALL_BACK_EXCUTE callExcuteBack,CALL_BACK_SHUTDOWN callShutdownBack);
typedef  int  (FAR PASCAL  *MyNetComm)(char*,int ,CALL_BACK_GET_DATA ,CALL_BACK_GET_DATA_GPRS ,CALL_BACK_EXCUTE ,CALL_BACK_SHUTDOWN );


typedef   int  (FAR PASCAL *MyNetSend)(char *pData,int nLen);

typedef   int  (FAR PASCAL *Nmyfction)(int data1);
typedef   int  (FAR PASCAL *MYNetSendMyData)(int DtuID,unsigned char MepID,unsigned char *pData,int nLen);
typedef   int  (FAR PASCAL *MYNetSendMyDataTrue)(int DtuID,unsigned char  nMeter_ID,int money,int delayTime  );
typedef int (FAR PASCAL *MYDownLoadFile)(char* pServerIP,int nPort,char* strSaveFile,CALL_BACK_DOWNLOAD callBack);
typedef int (FAR PASCAL *MYregMYdevice)(void);


/*
 System.DateTime ConvertIntDateTime(long d)
        {
            System.DateTime time = System.DateTime.MinValue;
            System.DateTime startTime = TimeZone.CurrentTimeZone.ToLocalTime(new System.DateTime(1970, 1, 1));
            time = startTime.AddSeconds(d);
            return time;
		
        }
	*/
 bool callBack_GetDataFunction(int nDut_ID,int nMeter_ID,int nType,long tCommn,BYTE* pData,int nDataLen)
//bool callBack_GetDataFunction(int nDut_ID, int nMeter_ID, int nType, int tCommn, BYTE*  pData, int nDataLen)
        {unsigned long ttemp, gg,ttemp1,ttemp2;int i ;
 CString  mm[32];double dtemp; unsigned char MYbb[32];
 unsigned char tem; CString str; CString getTIME; const char * MYmd5val; Ccommon md5Class; CString str2;
 CDeviceEvent Devent;  long tem3 = 0; float tem4;
 unsigned int Down_Data[30];
 unsigned int CheckBox[2];

 extern CDbOperate SqlOPP2;

                int MM,NN;long ee;  
				OutputDebug("myTEST  callBack_GetDataFunction()  start..");
				memset(MYbb, 0, 32);//����ǰ����ս��ջ�����       //X ����������

                myREVflag=1;                                       //X ����������
				mynDut_ID=nDut_ID;mynMeter_ID=nMeter_ID;ee=tCommn;  //X ����������

                 memcpy(MYbb,pData,32);  //ע��memcpy�����ǲ������뺯��   //X ����������

      //�ж��Ƿ�Ϊ���ص�ȷ����
#ifdef NewFaction
				 if ((nDut_ID >= 300)&(nDut_ID < 700))
				 {
					 mynDut_ID = mynDut_ID * 10;
					 mynDut_ID = mynDut_ID + mynMeter_ID;
				 }
				 else
				 {
					 mynDut_ID = nDut_ID; mynMeter_ID = nMeter_ID;
				 }
#else
				 mynDut_ID = nDut_ID; mynMeter_ID = nMeter_ID;

#endif


				// OutputDebug("myTEST908 0 NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);



														  //�ж��Ƿ�Ϊ���ص�ȷ����
				 if (MYbb[3] == 0x04) //�ж��Ƿ���ȷ����
				 {
					 //ͨ��ID�����¼�,��Ҫ�ж�����--��������֪ͨ��Ӧ���¼����ö�Ӧ��ÿ���豸�ĵȴ��߳�ֹͣ������					
					 HANDLE hEvent = Devent.GetEvent(mynDut_ID); //X ����������
					 OutputDebug("myTEST907  dtu_id:%d.", mynDut_ID);
					 if (hEvent != NULL)
					 {
						 SetEvent(hEvent);   //X ����������
					 }
					 if (hEvent)
					 {
						// CloseHandle(hEvent);
					 }
				 }
        #ifdef SetPAM
				 if (MYbb[3] == 14) //����ǰ8����������Ӧ��
				 {
					 //ͨ��ID�����¼�,��Ҫ�ж�����--��������֪ͨ��Ӧ���¼����ö�Ӧ��ÿ���豸�ĵȴ��߳�ֹͣ������					
					 HANDLE hEvent = Devent.GetEvent(mynDut_ID);   //X ����������
					 if (hEvent != NULL)
					 {
						 SetEvent(hEvent); //X ����������
					 }
					 if (hEvent)
					 {
						// CloseHandle(hEvent);
					 }

					 for (i = 0; i < 30; i++) Down_Data[i] = 0; for (i = 0; i < 2; i++) CheckBox[i] = 0;
					 Down_Data[0] = MYbb[5];  Down_Data[0] <<= 8;  Down_Data[0] |= MYbb[6];  Down_Data[0] <<= 8;     Down_Data[0] |= MYbb[7];
					 Down_Data[1] = MYbb[8];  Down_Data[1] <<= 8;  Down_Data[1] |= MYbb[9];  Down_Data[1] <<= 8;     Down_Data[1] |= MYbb[10];
					 Down_Data[2] = MYbb[11]; Down_Data[2] <<= 8;  Down_Data[2] |= MYbb[12]; Down_Data[2] <<= 8;     Down_Data[2] |= MYbb[13];
					 Down_Data[3] = MYbb[14]; Down_Data[3] <<= 8;  Down_Data[3] |= MYbb[15]; Down_Data[3] <<= 8;     Down_Data[3] |= MYbb[16];
					 Down_Data[4] = MYbb[17]; Down_Data[4] <<= 8;  Down_Data[4] |= MYbb[18]; Down_Data[4] <<= 8;     Down_Data[4] |= MYbb[19];
					 Down_Data[5] = MYbb[20]; Down_Data[5] <<= 8;  Down_Data[5] |= MYbb[21]; Down_Data[5] <<= 8;     Down_Data[5] |= MYbb[22];
					 Down_Data[6] = MYbb[23]; Down_Data[6] <<= 8;  Down_Data[6] |= MYbb[24]; Down_Data[6] <<= 8;     Down_Data[6] |= MYbb[25];
					 Down_Data[7] = MYbb[26]; Down_Data[7] <<= 8;  Down_Data[7] |= MYbb[27]; Down_Data[7] <<= 8;     Down_Data[7] |= MYbb[28];
					 if (SqlOPP2.myUpdateDown_DataSQL(mynDut_ID, Down_Data, CheckBox) == true)
					 {
					 //		AfxMessageBox("1�ɹ�");
					 }
				

				 }
				 if (MYbb[3] == 16) //���ú�8����������Ӧ��
				 {
					 //ͨ��ID�����¼�,��Ҫ�ж�����--��������֪ͨ��Ӧ���¼����ö�Ӧ��ÿ���豸�ĵȴ��߳�ֹͣ������					
					 HANDLE hEvent = Devent.GetEvent(mynDut_ID); //X ����������
					 if (hEvent != NULL)
					 {
						 SetEvent(hEvent);
					 }
					 if (hEvent)
					 {
					//	 CloseHandle(hEvent);
					 }

					 for (i = 0; i < 30; i++) Down_Data[i] = 0; for (i = 0; i < 2; i++) CheckBox[i] = 0;
					 Down_Data[8] = MYbb[5];   Down_Data[8] <<= 8;   Down_Data[8] |= MYbb[6];   Down_Data[8] <<= 8;      Down_Data[8] |= MYbb[7];
					 Down_Data[9] = MYbb[8];   Down_Data[9] <<= 8;   Down_Data[9] |= MYbb[9];   Down_Data[9] <<= 8;      Down_Data[9] |= MYbb[10];
					 Down_Data[10] = MYbb[11]; Down_Data[10] <<= 8;  Down_Data[10] |= MYbb[12]; Down_Data[10] <<= 8;     Down_Data[10] |= MYbb[13];
					 Down_Data[11] = MYbb[14]; Down_Data[11] <<= 8;  Down_Data[11] |= MYbb[15]; Down_Data[11] <<= 8;     Down_Data[11] |= MYbb[16];
					 Down_Data[12] = MYbb[17]; Down_Data[12] <<= 8;  Down_Data[12] |= MYbb[18]; Down_Data[12] <<= 8;     Down_Data[12] |= MYbb[19];
					 Down_Data[13] = MYbb[20]; Down_Data[13] <<= 8;  Down_Data[13] |= MYbb[21]; Down_Data[13] <<= 8;     Down_Data[13] |= MYbb[22];
					 Down_Data[14] = MYbb[23]; Down_Data[14] <<= 8;  Down_Data[14] |= MYbb[24]; Down_Data[14] <<= 8;     Down_Data[14] |= MYbb[25];
					 Down_Data[15] = MYbb[26]; Down_Data[15] <<= 8;  Down_Data[15] |= MYbb[27]; Down_Data[15] <<= 8;     Down_Data[15] |= MYbb[28];
					 if (SqlOPP2.myUpdateDown_DataSQL(mynDut_ID, Down_Data, CheckBox) == true)
					 {
						 //		AfxMessageBox("1�ɹ�");
					 }


				 }
         #endif
				 if (MYbb[3] == 0x07) //�൱�������룬����ȷ����Щ�����ߵģ���Щ�ǲ����ߵ� �20�������û�����ݾ����¼���
				 {
 //�ĳɴ洢�������
					// SQLwriteFlag2 = 1;  //������д��־��������������綨ʱ��ȥִ��д���ݿ�Ĳ������������ܷ��ϲ���ϵͳ���ص㣬�������׶�д���ݿⱨ��
					// myDTU_ID2 = mynDut_ID;
				//	 myID2 = 0;

					 if (SqlOPP2.MyQuerySQL(mynDut_ID) == 1)
					 {
						 //		AfxMessageBox("1�ɹ�");
					
					 }
					 else
					 {
						 SqlOPP2.InsertSQLPayRecord(mynDut_ID, 0);//�ǻ�Ա�����ѱ��ֱ�Ӹĳ�0д���
					 }
					 SqlOPP2.my2UpdateSQL(mynDut_ID, 1);///////д������״̬dbo.Dev_NetState �� ConnectOK   //X ���������� ����ΪmynDut_ID��
					 //	AfxMessageBox("3�ɹ�");

       
					  connectFlgNum = 0; //ˢ�±�־λ


					  if (SqlOPP2.MyQuerySQL2(mynDut_ID) == 1)
					  {
					  }
					  else
					  {
						  SqlOPP2.InsertSQLPayRecord2(mynDut_ID);//���û�и��豸��ͨ����ʱ������Ӹ��豸����WK_SetState
					  }

					 unsigned  int  bb[8];  
					 if (MYbb[23] == 1)//ȱˮ���� ��Һλ����
					 {
						// OutputDebug("myTEST 1��Һλ����  000..nDut_ID=%d,nMeter_ID=%d", nDut_ID, 1);
						// bb[0] = 0x0203;
						 if (bb[0] == 0x0203)
						 {
							 bb[0] = 0x0A03;
						 }
						 else
						 {
							 bb[0] = 0x0803;
						 }

						 //��΢��ƽ̨����ȱˮ������Ϣ������Ա�����յ�
				/*
				http://zizhu.sdxjyjd.com/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=56&msg=HighLiquidLevelWARNING��Һλ
						 ������
						 �豸���   Set =  56
						 ��������   msg=��Һλ
				*/
						 CString tt = "http://127.0.0.1/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=";
						 

						 str.Format("%d", mynDut_ID); tt = tt + str;
						 tt += "&msg=";

					//	CString test_str1 = "��Һλ";
					//	 tt += URLEncode(test_str1);
						 tt += "WARNING";
					//	 tt += "��Һλ";
						 tt += URLEncode("WARNING1");  //X �������� �����������
						// SYSTEMTIME st22;
					//	 GetLocalTime(&st22); // ��ȡ��ǰʱ��
					//	if(( st22.wHour==0��&&��st22.wMinute==0��&&��st22.wMinute==0����
						 getTIME = gettime2();  //X ��������������
					//	 if( (strcmp(getTIME, "1229") == 0)|| (strcmp(getTIME, "1230") == 0) || (strcmp(getTIME, "1231") == 0) || (strcmp(getTIME, "1232") == 0) || (strcmp(getTIME, "1233") == 0) || (strcmp(getTIME, "1234") == 0) || (strcmp(getTIME, "1235") == 0) || (strcmp(getTIME, "1236") == 0) || (strcmp(getTIME, "1237") == 0) || (strcmp(getTIME, "1238") == 0) || (strcmp(getTIME, "1239") == 0)) //ÿ������12�����ҷ���һ��
						{
						 OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
						// OutputDebug("myTEST ��Һλ����  000..nDut_ID=%d,nMeter_ID=%d", nDut_ID, 1);
						// Devent.Lock();
						 SendBACKtoPHPserver(tt);
				//
					//	 Devent.UnLock();
						}
					 }
#ifdef WarningFlg
					 if (MYbb[22] == 100)//ȱˮ���� ��Һλ����  0X64�ǲ�����
#else
					 if (MYbb[22] == 0)//ȱˮ���� ��Һλ����  0X64�ǲ�����
#endif
					 {
						 // OutputDebug("myTEST 1��Һλ����  000..nDut_ID=%d,nMeter_ID=%d", nDut_ID, 1);

						 if (bb[0] == 0x0803)
						 {
							 bb[0] = 0x0A03;
						 }
						 else
						 {
							 bb[0] = 0x0203;
						 }


						 //��΢��ƽ̨����ȱˮ������Ϣ������Ա�����յ�
						 /*
						 http://zizhu.sdxjyjd.com/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=56&msg=LowLiquidLevelWARNING��Һλ
						 ������
						 �豸���   Set =  56
						 ��������   msg=��Һλ
						 */
						 CString tt = "http://127.0.0.1/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=";


						 str.Format("%d", mynDut_ID); tt = tt + str;
						 tt += "&msg=";

						 //	CString test_str1 = "��Һλ";
						 //	 tt += URLEncode(test_str1);
						 tt += "WARNING";
						// tt += "��Һλ";
						 tt += URLEncode("WARNING3");
						 // SYSTEMTIME st22;
						 //	 GetLocalTime(&st22); // ��ȡ��ǰʱ��
						 //	if(( st22.wHour==0��&&��st22.wMinute==0��&&��st22.wMinute==0����
						 getTIME = gettime2();  //X �������ԣ�
						 //	 if( (strcmp(getTIME, "1229") == 0)|| (strcmp(getTIME, "1230") == 0) || (strcmp(getTIME, "1231") == 0) || (strcmp(getTIME, "1232") == 0) || (strcmp(getTIME, "1233") == 0) || (strcmp(getTIME, "1234") == 0) || (strcmp(getTIME, "1235") == 0) || (strcmp(getTIME, "1236") == 0) || (strcmp(getTIME, "1237") == 0) || (strcmp(getTIME, "1238") == 0) || (strcmp(getTIME, "1239") == 0)) //ÿ������12�����ҷ���һ��
						 {
							 OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
													// OutputDebug("myTEST ��Һλ����  000..nDut_ID=%d,nMeter_ID=%d", nDut_ID, 1);
													// Devent.Lock();
							 SendBACKtoPHPserver(tt);
							 //
							 //	 Devent.UnLock();
						 }
					 }
						 if ((MYbb[22] != 0)&& (MYbb[23] != 1))
					 {
						 bb[0] = 0x0003;
					 }

#ifdef DaoShenFlowmeter
						 unsigned  long getMY_CSQ, WENDU, TDS1, TDS2, VERSION_temp;
						
						 getMY_CSQ = MYbb[5]; getMY_CSQ <<= 8; getMY_CSQ |= MYbb[6];
						 getMY_CSQ <<= 8; getMY_CSQ |= MYbb[7];
						 WENDU = MYbb[8]; WENDU <<= 8; WENDU |= MYbb[9];
						 WENDU <<= 8; WENDU |= MYbb[10];
						 TDS1 = MYbb[11]; TDS1 <<= 8; TDS1 |= MYbb[12];
						 TDS1 <<= 8; TDS1 |= MYbb[13];
						 TDS2 = MYbb[14]; TDS2 <<= 8; TDS2 |= MYbb[15];
						 TDS2 <<= 8; TDS2 |= MYbb[16];
						 VERSION_temp = MYbb[17]; VERSION_temp <<= 8; VERSION_temp |= MYbb[18];
						 VERSION_temp <<= 8; VERSION_temp |= MYbb[19];

						 bb[5] = VERSION_temp; bb[5] <<= 16; bb[5] |= getMY_CSQ;
						 bb[2] = TDS1; bb[2] <<= 16; bb[2] |= TDS2;
						 bb[3] = 0; bb[3] <<= 16; bb[3] |= 0;
						 bb[1] = WENDU; bb[1] <<= 16;


						 //��������


#endif
					// bb[1] = 0; bb[2] = 0; bb[3] = 0; bb[4] = MYbb[23]; bb[5] = 0;

					 // bb[0] = 0x0203;
					 SqlOPP2.my3UpdateSQL(mynDut_ID, bb);//

#ifdef ConfirmJS

					 if (MYbb[24] == 0)//��������豸��������ʹ��״̬����ô��ˢ�����߱�־λ
					 {

						 if (SqlOPP2.MyQuerySQLBB(mynDut_ID) == 1)
						 {
							 //		AfxMessageBox("1�ɹ�");
							 SqlOPP2.UpdateSQL(mynDut_ID, 0);//���ѱ��ֱ�Ӹĳ�0д���
						 }
						 else
						 {
							 SqlOPP2.InsertSQLPayRecord(mynDut_ID, 0);//���ѱ��ֱ�Ӹĳ�0д���
						 }
      #ifndef ADD_More_CH
						 if (SqlOPP2.MyQuerySQLCC(mynDut_ID) == 1)
						 {
							 //		AfxMessageBox("1�ɹ�");
							 SqlOPP2.UpdateSQLCCC(mynDut_ID, 0);//��ʾ�豸û������
						 }
						 else
						 {
							 SqlOPP2.InsertSQLPayRecordCCC(mynDut_ID, 0);//��ʾ�豸û������
						 }
       #endif
						 //	AfxMessageBox("3�ɹ�");

					 }
					 else
					 {

      #ifndef ADD_More_CH 
						 if (SqlOPP2.MyQuerySQLCC(mynDut_ID) == 1)
						 {
							 //		AfxMessageBox("1�ɹ�");
							 SqlOPP2.UpdateSQLCCC(mynDut_ID, 1);//��ʾ�豸��������
						 }
						 else
						 {
							 SqlOPP2.InsertSQLPayRecordCCC(mynDut_ID, 1);//��ʾ�豸��������
						 }
      #endif				 
						 //	AfxMessageBox("3�ɹ�");


					 }


		#ifdef ADD_More_CH
		
							 int mLtemp = MYbb[28]; mLtemp <<= 8;
							 mLtemp |= MYbb[27];


							 if (SqlOPP2.MyQuerySQLCC(mynDut_ID) == 1)
							 {
								 //		AfxMessageBox("1�ɹ�");
								 SqlOPP2.UpdateSQLCCC(mynDut_ID, mLtemp);//д����·���ã���·Ϊ��
							 }
							 else
							 {
								 SqlOPP2.InsertSQLPayRecordCCC(mynDut_ID, mLtemp);//д����·���ã���·Ϊ��
							 }


		#endif

#endif					

#ifdef ADD_More_CH
					 if (MYbb[25] != 0)//����0����ͨ����
					 {
/*
http://PHP��̨�ӿ��ļ���Ĭ��80�˿ڣ�/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=56&msg=WARNING3
ע����Set=56�������������Ϣ����Ա��Ϊ56���豸
1> ��msg=WARNING3�������Һλ����
2>��msg=CH10_Complete;�������10ͨ��ʹ�ý����������Ӧ���վ����������Ҫͨ������Ӫ�̵Ĺ��ں�֪ͨ����Ӧ���û���
*/        
						 CString strTTemp[10] = { "CH1_Complete","CH2_Complete","CH3_Complete","CH4_Complete","CH5_Complete","CH6_Complete","CH7_Complete","CH8_Complete","CH9_Complete","CH10_Complete" };
						 CString strTTemp2[10] = { "CH1_Interupt","CH2_Interupt","CH3_Interupt","CH4_Interupt","CH5_Interupt","CH6_Interupt","CH7_Interupt","CH8_Interupt","CH9_Interupt","CH10_Interupt" };

						 if (MYbb[26]==1)//
						 { 
							 OutputDebug("ͨ��%ʹ�����....", MYbb[25]);
							 CString tt = "http://127.0.0.1/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=";
							 str.Format("%d", mynDut_ID); tt = tt + str;
							 tt += "&msg=";

							// tt += "WARNING";
							 tt += URLEncode(strTTemp[MYbb[25]]);

							 getTIME = gettime2();

								 OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
								 SendBACKtoPHPserver(tt);

						 }
						 else  if (MYbb[26] == 2)//
						 {
							 OutputDebug("ͨ��%ʹ���ж�....", MYbb[25]);
							 CString tt = "http://127.0.0.1/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=";
							 str.Format("%d", mynDut_ID); tt = tt + str;
							 tt += "&msg=";

							 // tt += "WARNING";
							 tt += URLEncode(strTTemp[MYbb[25]]);

							 getTIME = gettime2();

							 OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
							 SendBACKtoPHPserver(tt);

						 }
						 else  if (MYbb[26] == 3)//
						 {

						 }

					 }
			

#endif

					 
				 }
				 if (MYbb[3] == 0x06) //�ж��ǿ��ţ�10λBCD�룬����������06ʱ����ĺ������ϴ����ţ�
				 {
					 //					 68 01 20 06   08     00 00   0D       08 8B     00    00 06     00    04 46   00    00 32     00   00 00 00 00 00 00 00 00 00   46 14    16
					 //0X00000D088B(10BCD��Ŀ��ţ�
					 //���Ϳ��Ÿ�PHP��������get����ϵͳhttp://zizhu.sdxjyjd.com/cardcode.php?cid=9819085342&machineid=1001&md5val=kfwjeklx15151sdw23&time=201611130605

					 getTIME = gettime(); //X ��������������
					 MYmd5val = md5Class.md5(getTIME + "zzxt6565911");
					// CString tt = "http://zizhu.sdxjyjd.com/cardcode.php";
					 CString tt = "http://127.0.0.1/cardcode.php";
					 //CString tt = "http://127.0.0.1:8090"; //�������������������
					 str2 = "";
					 tem = MYbb[5] >> 4; str.Format("%d", tem); str2 += str;//
					 tem = (MYbb[5] & 0x0f); str.Format("%d", tem); str2 += str;//
					 tem = MYbb[6] >> 4; str.Format("%d", tem); str2 += str;//
					 tem = (MYbb[6] & 0x0f); str.Format("%d", tem); str2 += str;//
					 tem = MYbb[7] >> 4; str.Format("%d", tem); str2 += str;//
					 tem = (MYbb[7] & 0x0f); str.Format("%d", tem); str2 += str;//
					 tem = MYbb[8] >> 4; str.Format("%d", tem); str2 += str;//
					 tem = (MYbb[8] & 0x0f); str.Format("%d", tem); str2 += str;//
					 tem = MYbb[9] >> 4; str.Format("%d", tem); str2 += str;//
					 tem = (MYbb[9] & 0x0f); str.Format("%d", tem); str2 += str;//

					 
					 /*
					 tem = MYbb[5] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[5] & 0x0f);
					 str.Format("%d", tem); str2 += str;//
					 tem = MYbb[6] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[6] & 0x0f);
					 str.Format("%d", tem); str2 += str;//
					 tem = MYbb[7] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[7] & 0x0f);
					 str.Format("%d", tem); str2 += str;
					 tem = MYbb[8] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[8] & 0x0f);
					 str.Format("%d", tem); str2 += str;
					 tem = MYbb[9] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[9] & 0x0f);
					 str.Format("%d", tem); str2 += str;
					 */
					 tt += "?cid=";
					  tt = tt + str2;//
					 tt += "&machineid=";
					 str.Format("%d", mynDut_ID); tt = tt + str;

					 tt += "&md5val=";
					 tt = tt + MYmd5val;
					 tt += "&time=";
					 tt = tt + getTIME;
					 OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
					 Devent.Lock();
					 SendBACKtoPHPserver(tt);
					 Devent.UnLock();
					 myOUTerr(0x31);
					 //δ��ʱ�Ĵ������


				 }
				 if (MYbb[3] == 0x55) //�ж����ֻ��ţ�11λBCD�룬����������55ʱ����ĺ������ϴ��ֻ��ţ�
				 {//http://zizhu.sdxjyjd.com/phonecode.php?machineid=1001&phone=6&pwd=6612151&md5val=kfwjeklx15151sdw23&time=201611130605
					 //		68 01 20 55   08     00 00   0D       08 8B     00    00 06     00    04 46   00    00 32     00   00 00 00 00 00 00 00 00 00   46 14    16
					 //0X00000D088B0(11BCD����ֻ��ţ� ����0X000600
					 getTIME = gettime();
					 MYmd5val = md5Class.md5(getTIME + "zzxt6565911");
				//	 CString tt = "http://zizhu.sdxjyjd.com/phonecode.php";
					 CString tt = "http://127.0.0.1/phonecode.php";
					 //CString tt = "http://127.0.0.1:8090"; //�������������������
					
					 tt += "?machineid=";
					 str.Format("%d", mynDut_ID); tt = tt + str;

					 tem = MYbb[5] >> 4; str.Format("%d", tem); str2 = str;//
					 tem =  (MYbb[5] & 0x0f); str.Format("%d", tem); str2 += str;//
					 tem = MYbb[6] >> 4; str.Format("%d", tem); str2 += str; //
					 tem = (MYbb[6] & 0x0f); str.Format("%d", tem); str2 += str;//
					 tem = MYbb[7] >> 4; str.Format("%d", tem); str2 += str; //
					 tem = (MYbb[7] & 0x0f); str.Format("%d", tem); str2 += str;//
					 tem = MYbb[8] >> 4; str.Format("%d", tem); str2 += str; //
					 tem = (MYbb[8] & 0x0f); str.Format("%d", tem); str2 += str;//
					 tem = MYbb[9] >> 4; str.Format("%d", tem); str2 += str; //
					 tem = (MYbb[9] & 0x0f); str.Format("%d", tem); str2 += str;//
					 tem = MYbb[10] >> 4; str.Format("%d", tem); str2 += str; //

               /*
					 tem = MYbb[5] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[5] & 0x0f);
					 str.Format("%d", tem); str2 += str;//
					 tem = MYbb[6] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[6] & 0x0f);
					 str.Format("%d", tem); str2 += str;//
					 tem = MYbb[7] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[7] & 0x0f);
					 str.Format("%d", tem); str2 += str;
					 tem = MYbb[8] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[8] & 0x0f);
					 str.Format("%d", tem); str2 += str;
					 tem = MYbb[9] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[9] & 0x0f);
					 str.Format("%d", tem); str2 += str;
					 tem = MYbb[10] >> 4; tem = tem & 0x0f; 
					 str.Format("%d", tem); str2 += str;
            */
					 tt += "&phone=";
					 tt = tt + str2;//
					
					 str2 = "";

					 tem = MYbb[11] >> 4; str.Format("%d", tem); str2 = str;//
					 tem = (MYbb[11] & 0x0f); str.Format("%d", tem); str2 += str;//
					 tem = MYbb[12] >> 4; str.Format("%d", tem); str2 += str;//
					 tem = (MYbb[12] & 0x0f); str.Format("%d", tem); str2 += str;//
					 tem = MYbb[13] >> 4; str.Format("%d", tem);  str2 += str;//
					 tem = (MYbb[13] & 0x0f); str.Format("%d", tem); str2 += str;//

                  /*
					 tem = MYbb[11] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[11] & 0x0f);
					 str.Format("%d", tem); str2 += str;
					 tem = MYbb[12] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[12] & 0x0f);
					 str.Format("%d", tem); str2 += str;
					 tem = MYbb[13] >> 4; tem = tem & 0x0f; tem = tem * 10;
					 tem = tem + (MYbb[13] & 0x0f);
					 str.Format("%d", tem); str2 += str;
                  */
					 tt += "&pwd=";
					 tt = tt + str2;//

					 tt += "&md5val=";
					 tt = tt + MYmd5val;
					 tt += "&time=";
					 tt = tt + getTIME;
					 OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
					 Devent.Lock();
					 SendBACKtoPHPserver(tt);
					 Devent.UnLock();
					 myOUTerr(0x32);
				 }
				 if (MYbb[3] == 0x22) //�������ʹ�ú�get���󣺣�
				 {//http://zizhu.sdxjyjd.com/notify.php?machineid=1001&kk=���ѽ��&id=115347&md5val=kfwjeklx15151sdw23&time=201611130605&error=0
				  //68 01 20 22   08     00 0D 00       00 00  01      00 00  60        04 00  46    00 00  32      00 00 00     00 00 00     
//					 00 00 00   46 14    16
				  //�������ѽ��   0X000D00 =33.28Ԫ
					 getTIME = gettime();
					 MYmd5val = md5Class.md5(getTIME + "zzxt6565911");

					// CString tt = "http://zizhu.sdxjyjd.com/notify.php";
               #ifdef	 forSQPT_where
					 CString tt = "http://xxx.xxx.xxx.xx/xxx";
               #else
					 CString tt = "http://127.0.0.1/notify.php";
					 //CString tt = "http://127.0.0.1:8090"; //���������������������
               #endif

					 tt += "?machineid=";
					 str.Format("%d", mynDut_ID); tt = tt + str;

					// tt += "&kk=-"; //ע��:����Ӹ�"-"ֵ�����Ǵ������������ǻ��յ�
					 tt += "&kk=";

					 tem3 = MYbb[5] ; tem3 = tem3<<8;
					 tem3|= MYbb[6]; tem3 = tem3 <<8;
					 tem3 |= MYbb[7]; 
					 tem4 = tem3;
					 tem4 = tem4/ 100; //��2λ��С��
              ////////////////			 //������д��־��������������綨ʱ��ȥִ��д���ݿ�Ĳ������������ܷ��ϲ���ϵͳ���ص㣬�������׶�д���ݿⱨ��

#ifdef JavaResolvesConflicts 
					 int nnMyEmpID = 0;
					 unsigned char nnOpType;
					 // if (SqlOPP2.MyQuerySQL(mynDut_ID) == 1) //�����Ӧ�豸ID��EmpID
					 if (SqlOPP2.nnMyQuerySQL(mynDut_ID, &nnMyEmpID, &nnOpType) == 1) //�����Ӧ�豸ID��EmpID
					 {
						 if (nnOpType !=1)//==1����JAVA��
						 {
							 //	 AfxMessageBox("92�ɹ�");
							 str.Format("%f", tem4); tt = tt + str;//�������
							 tt += "&id=";
							 str.Format("%d", nnMyEmpID); tt = tt + str;//������						

							 tt += "&md5val=";
							 tt = tt + MYmd5val;
							 tt += "&time=";
							 tt = tt + getTIME;
							 tt += "&error=";
							 str.Format("%d", 0); tt = tt + str;//������	
																//	 OutputDebug("myTEST908 1 NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);
							 if (nnMyEmpID != 0)//ֻ�л�Ա�ŷ���������
							 {
								 //	 OutputDebug("myTEST908 2  NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);

								 OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
							 }
							 Devent.Lock();
							 if (nnMyEmpID != 0)//ֻ�л�Ա�ŷ���������
							 {
								 SendBACKtoPHPserver(tt);
							 }
							 Devent.UnLock();
							 myOUTerr(0x33);
							 OutputDebug("myTEST  NetSendMyAccount()  000..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
  #ifdef ConfirmJS
							 if (nnMyEmpID != 0)//
							 {//����豸��SQL���ݿ�����ĵǼǻ��߰���Ӧ�豸�Ķ����š�MyEmpID������Ϊ0��
							  // OutputDebug("myTEST908 3 NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);
							  // if (nDut_ID != 11)//
								 {
									 if ((NetSendMyAccount(nDut_ID, nMeter_ID, 1)) == 45)//����0X23��ȷ��������豸) ����ͷ��β��45���ֽ�
									 {
										 OutputDebug("myTEST  NetSendMyAccount()  1..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
										 SqlOPP2.UpdateSQL(mynDut_ID, 0);//�Ѷ�Ӧ��Ӳ���豸�����ѱ�Ÿ�Ϊ0����ֹ�´��ظ��۷ѣ�������ѻ��ղ���ȷ��ָ�
									 }
								 }
								 //����0X23��ȷ��������豸
								 /*
								 ��λ���õ�������Ϣ��������OKȷ��֡���豸��ȷ��֡һ�����£�
								 ����������23ʱ����������������λ������ȷ��֡��

								 68 01 20 23   08     00 0D 00       00 00  01      00 00  60        04 00  46    00 00  32      00 00 00     00 00 00     00 00 00   46 14    16
								 ���01             ����           ȷ��ʶ��        ����             ����         ����             ����         ����        ����      CRCЧ��
								 0X000001=1
								 */


							 }
							 else // ����Ѿ�ɾ���˸��豸��Ҳ����˵�Ѿ����͹�0X23��������ξ�ֻ����0X23��ȷ��������豸
							 {
								 //	if (nDut_ID !=11)//
								 {
									 OutputDebug("myTEST  NetSendMyAccount()  2..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
									 NetSendMyAccount(nDut_ID, nMeter_ID, 1);//����0X23��ȷ��������豸
								 }
							 }
						 }
  #endif
					 }
#else			
					 if (SqlOPP2.MyQuerySQL(mynDut_ID) == 1) //�����Ӧ�豸ID��EmpID
					 {					
						 //	 AfxMessageBox("92�ɹ�");
						 str.Format("%f", tem4); tt = tt + str;//�������
						 tt += "&id=";
						 str.Format("%d", MyEmpID); tt = tt + str;//������						

						 tt += "&md5val=";
						 tt = tt + MYmd5val;
						 tt += "&time=";
						 tt = tt + getTIME;
						 tt += "&error=";
						 str.Format("%d", 0); tt = tt + str;//������	
					//	 OutputDebug("myTEST908 1 NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);
						 if (MyEmpID != 0)//ֻ�л�Ա�ŷ���������
						 {   
						//	 OutputDebug("myTEST908 2  NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);

							 OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
						 }
						 Devent.Lock();
						if(MyEmpID !=0)//ֻ�л�Ա�ŷ���������
						{ 
						 SendBACKtoPHPserver(tt);
						 }
						 Devent.UnLock();
						 myOUTerr(0x33);
						 OutputDebug("myTEST  NetSendMyAccount()  000..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
   #ifdef ConfirmJS
						 if (MyEmpID != 0)//
						 {//����豸��SQL���ݿ�����ĵǼǻ��߰���Ӧ�豸�Ķ����š�MyEmpID������Ϊ0��
							// OutputDebug("myTEST908 3 NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);
							// if (nDut_ID != 11)//
							 {
								 if ((NetSendMyAccount(nDut_ID, nMeter_ID, 1)) == 45)//����0X23��ȷ��������豸) ����ͷ��β��45���ֽ�  //X ����������
								 {
									 OutputDebug("myTEST  NetSendMyAccount()  1..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
									 SqlOPP2.UpdateSQL(mynDut_ID, 0);//�Ѷ�Ӧ��Ӳ���豸�����ѱ�Ÿ�Ϊ0����ֹ�´��ظ��۷ѣ�������ѻ��ղ���ȷ��ָ�
								 }
							 }
	//����0X23��ȷ��������豸
/*
��λ���õ�������Ϣ��������OKȷ��֡���豸��ȷ��֡һ�����£�
����������23ʱ����������������λ������ȷ��֡��

68 01 20 23   08     00 0D 00       00 00  01      00 00  60        04 00  46    00 00  32      00 00 00     00 00 00     00 00 00   46 14    16
���01             ����           ȷ��ʶ��        ����             ����         ����             ����         ����        ����      CRCЧ��
                                 0X000001=1
*/
						 

						 }
						else // ����Ѿ�ɾ���˸��豸��Ҳ����˵�Ѿ����͹�0X23��������ξ�ֻ����0X23��ȷ��������豸
						{
						//	if (nDut_ID !=11)//
							{
								OutputDebug("myTEST  NetSendMyAccount()  2..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
								NetSendMyAccount(nDut_ID, nMeter_ID, 1);//����0X23��ȷ��������豸  //X ����������
							}
						}
     #endif
					 }
 #endif


			  
			  /*	//�����ǵ��ö�ʱ���������дSQL����
					 SQLwriteFlag3= 1; SQLwriteFlag4 = 1;
					 myDTU_ID2 = nDut_ID;   	//	AfxMessageBox("91�ɹ�");
					 while (SQLwriteFlag4);//�ȴ���ʱ���������ݿ����
					 if (SQLwriteFlag3 == 2)
					 {
						 SQLwriteFlag3 = 0;
					//	 AfxMessageBox("92�ɹ�");
						 str.Format("%f", tem4); tt = tt + str;//�������
						 tt += "&id=";
						 str.Format("%d", MyEmpID); tt = tt + str;//������						

						 tt += "&md5val=";
						 tt = tt + MYmd5val;
						 tt += "&time=";
						 tt = tt + getTIME;
						 tt += "&error=";
						 str.Format("%d", 0); tt = tt + str;//������	
						 OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
						 Devent.Lock();
						 SendBACKtoPHPserver(tt);
						 Devent.UnLock();
						 myOUTerr(0x33);
						   
				     }
              */

               /////////////////////////////
            /*
					 CDbOperate SqlOPP2;
					 SqlOPP2.OpenSql();
					 if (SqlOPP2.MyQuerySQL(nDut_ID) == 1) //�����Ӧ�豸ID��EmpID
					 {

						 str.Format("%d", tem3); tt = tt + str;//�������
						 tt += "&id=";
						 str.Format("%d", 0); tt = tt + str;//������

						 tt += "&md5val=";
						 tt = tt + MYmd5val;
						 tt += "&time=";
						 tt = tt + getTIME;
						 tt += "&error=";
						 str.Format("%d", MyEmpID); tt = tt + str;
						 Devent.Lock();
						 SendBACKtoPHPserver(tt);
						 Devent.UnLock();
						 myOUTerr(0x33);
					 }
					 SqlOPP2.CloseSql();
			   */
				 }
#ifdef ShuiQuanZuanYong
				 if (MYbb[3] == 0x08) /////��ʱ�ϴ� ˲ʱ����,������ˮ��,�����õ� ��Լ1����һ��
				 {
				// http://XXX.JSPˮȨƽ̨.COM/XXX?machineid=1001&flow_speed=˲ʱ����&AlreadyUsed_flow_count=������ˮ��&AlreadyUsed_Electrical_Quantity=�����õ��� &md5val=kfwjeklx15151sdw23&time=201611130605&error=0
				  //		68 01 20 55   08     00 00   0D       08 8B     00    00 06     00    04 46   00    00 32     00   00 00 00 00 00 00 00 00 00   46 14    16
				  //0X00000D088B0(11BCD����ֻ��ţ� ����0X000600
					 getTIME = gettime();
					 MYmd5val = md5Class.md5(getTIME + "zzxt6565911");
					 //	 CString tt = "http://zizhu.sdxjyjd.com/phonecode.php";
			    #ifdef	 forSQPT_where
					 CString tt = "http://xxx.x.xxxx/xxxx.php";
                #endif

					 tt += "?machineid=";
					 str.Format("%d", mynDut_ID); tt = tt + str;

					int tempNSQ = MYbb[5]; tempNSQ <<= 8; tempNSQ |= MYbb[6];
					tempNSQ <<= 8; tempNSQ |= MYbb[7];
					 tt += "&flow_speed=";
					 str.Format("%d", tempNSQ); tt = tt + str;

					 tempNSQ = MYbb[8]; tempNSQ <<= 8; tempNSQ |= MYbb[9];
					 tempNSQ <<= 8; tempNSQ |= MYbb[10];
					 tt += "&AlreadyUsed_flow_count=";
					 str.Format("%d", tempNSQ); tt = tt + str;

					 tempNSQ = MYbb[11]; tempNSQ <<= 8; tempNSQ |= MYbb[12];
					 tempNSQ <<= 8; tempNSQ |= MYbb[13];
					 tt += "&AlreadyUsed_Electrical_Quantity=";
					 str.Format("%d", tempNSQ); tt = tt + str;


					 tt += "&md5val=";
					 tt = tt + MYmd5val;
					 tt += "&time=";
					 tt = tt + getTIME;
					 OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
					 Devent.Lock();
					 SendBACKtoPHPserver(tt);
					 Devent.UnLock();
					 myOUTerr(0x32);
				 }
				 if (MYbb[3] == 0x09)  ////��ʱ�ϴ� 8�����ϲ���  ��Լ10����һ��
				 {
					 unsigned  long getMY_CSQ, WENDU, TDS1, TDS2, VERSION_temp;

					 //getMY_CSQ = MYbb[5]; getMY_CSQ <<= 8; getMY_CSQ |= MYbb[6];
					 //getMY_CSQ <<= 8; getMY_CSQ |= MYbb[7];
					 //WENDU = MYbb[8]; WENDU <<= 8; WENDU |= MYbb[9];
					 //WENDU <<= 8; WENDU |= MYbb[10];
					 //TDS1 = MYbb[11]; TDS1 <<= 8; TDS1 |= MYbb[12];
					 //TDS1 <<= 8; TDS1 |= MYbb[13];
					 //TDS2 = MYbb[14]; TDS2 <<= 8; TDS2 |= MYbb[15];
					 //TDS2 <<= 8; TDS2 |= MYbb[16];
					 //VERSION_temp = MYbb[17]; VERSION_temp <<= 8; VERSION_temp |= MYbb[18];
					 //VERSION_temp <<= 8; VERSION_temp |= MYbb[19];

					 //bb[5] = VERSION_temp; bb[5] <<= 16; bb[5] |= getMY_CSQ;
					 //bb[2] = TDS1; bb[2] <<= 16; bb[2] |= TDS2;
					 //bb[3] = 0; bb[3] <<= 16; bb[3] |= 0;
					 //bb[1] = WENDU; bb[1] <<= 16;



					 // bb[1] = 0; bb[2] = 0; bb[3] = 0; bb[4] = MYbb[23]; bb[5] = 0;

					 // bb[0] = 0x0203;
					 //::CoInitialize(NULL);//��ʼ��COM�� �߳��ڶ�дSQL�����ʼ���ͷ���ʼ�������򾭳�����SQL��д����
					 //SqlOPP2.OpenSql();
					 //SqlOPP2.my3UpdateSQL(mynDut_ID, bb);//
					 //SqlOPP2.CloseSql();
					 //::CoUninitialize();//����ʼ��COM��
				 }
#endif
				 OutputDebug("myTEST  callBack_GetDataFunction()   end..");

	 return true;
        }



 bool callBack_GPRSFunction(int nDtu_ID,int nStatus,long lastTime)

//bool callBack_GPRSFunction(int DtuId, int nStatus, int lastTime)
        {
	 int MM,NN;long ee;
            try
            {
				MM=nDtu_ID;NN=nStatus;ee=lastTime;
            /*
                if (waterList.Exists(p => p.DTU_L == DtuId))
                {
                    meterData.UpdateDataMeter(DtuId, nStatus == 1 ? true : false, ConvertIntDateTime(lastTime));
                }
			*/
            }
            catch(_com_error *e) 
            {
              //  Log.Write(ex);
            }
			

	     return true;
        }
bool callBack_EXCUTEFunction(int nType,char* pText,int nTextLen)
//bool callBack_EXCUTEFunction(int nType, char* pText, int nTextLen)
        { 
            
	return true;
        }
   bool callBack_ShutDownFunction()
        {
           // timer1.Enabled = true;
	   return true;
        }
            
          
           
         
      









 //CALL_BACK_DOWNLOAD callback_Download;


/*
      void NetCommStart()
        {

            int i = 0;
            try
            {

                i = NetComm(Ip, Comm, callBack_GetData,callBack_GPRS,
                callBack_EXCUTE, callBack_ShutDown);
            }
            catch
            {

            }
            if (i == 0)
            {
               // timer1.Enabled = false;
               // toolStripStatusLink.Text = "����";
            }
            else
            {
               // timer1.Enabled = true;
              //  toolStripStatusLink.Text = "����....30�������";
            }
        }


*/
#endif
