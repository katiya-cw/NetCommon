#if !defined(________H_________)
#define ________H_________

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
int __stdcall DownLoadFile(char* pServerIP,int nPort,char* strSaveFile,CALL_BACK_DOWNLOAD callBack);
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
int __stdcall NetComm(char* pServerIP,int nPort,CALL_BACK_GET_DATA callGetBack,CALL_BACK_GET_DATA_GPRS callGetBack2,CALL_BACK_EXCUTE callExcuteBack,CALL_BACK_SHUTDOWN callShutdownBack);
 
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
int __stdcall NetSend(char *pData,int nLen);
/*
���� ���ط��͵����ݳ��� -2δ����

  ����
pData ��Ҫ���͵��ַ���
nLen  �ַ�������
*/
//������λ������ָ�����ȵ����ݸ��豸�ն�
int __stdcall NetSendMyData(int DtuID,unsigned char MepID,unsigned char *pData,int nLen);







//������λ������ָ��Ǯ�����豸�ն�
int __stdcall NetSendMyDataTrue(int DtuID,unsigned char nMeter_ID, int money,int delayTime  );
/*
���� ���ͳɹ��󷵻ط��ͳɹ����ֽ���
  ����
DtuID  �ն��豸���
money  ֧������Ǯ,��λ��,����7609����76Ԫ��9��

*/

//���ע��,����1��ʾ�ɹ�
int pascal regMYdevice(void);

//�����ݿ�Ĳ�������׼SQL��䣬��ɺ��ʧ�ܺ��ͨ���ص���ʾ
int __stdcall GetQueryData(char *pData,int nLen,char* pRetData,int& nRetLen);


int __stdcall myfction(int data1);
#endif
