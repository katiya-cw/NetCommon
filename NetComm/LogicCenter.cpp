#include "stdafx.h"
#include "LogicCenter.h"
#include "DeviceEvent.h"
#include "ThreadPool.h"
//����ͷ�ļ�   
#include "MD5/common.h"  
#include "db/DbOperate.h""
#include "../MyDefine.h"

extern ThreadPool threadPool;
extern unsigned char MENGsendToDtu(unsigned long DevID, unsigned char Dev_id, unsigned long money, unsigned long deyTime);
extern void SendBACKtoPHPserver(CString text);
extern int SQLwriteFlag;
extern int myDTU_ID;
extern int myID;
#ifdef ADD_More_CH
extern int pascal NetSendMyDataTrue2(int DtuID, unsigned char nMeter_ID, int money, int delayTime, int CH);
extern int pascal NetSendMyDataTrue3(int DtuID, unsigned char nMeter_ID, int money, int delayTime, int CH, int paymoney);
#endif


#ifdef SENDforOK
extern unsigned char DevSendTimes[SENDTIMES_LENS];
#endif
//��ȡURL�Ĳ���ֵ
vector<CString> GetURLParamValue(CString param)
{
	vector<CString> vec;
	int i = 0;
	int first = 0;
	int last = 0;
	while (i < 8)
	{
		first = param.Find("=", last);
		if (first != -1) {
			last = param.Find("&", first + 1);
			if (last == -1)
			{
				last = param.GetLength();
			}
		}
		else {
			break;
		}

		if (last != first && last > first)
		{
			CString strTemp = param.Mid(first + 1, last - first - 1);
			vec.push_back(strTemp);
			i++;
		}
		else
		{
			break;
		}
	}

	return vec;
}

CString gettime()
{
	CString temp;
	CString str;
	SYSTEMTIME st;
	GetLocalTime(&st); // ��ȡ��ǰʱ��
	temp = "";
	str.Format("%d", st.wYear); temp = temp + str;
	str.Format("%d", st.wMonth); temp = temp + str;
	str.Format("%d", st.wDay); temp = temp + str;
	str.Format("%d", st.wHour); temp = temp + str;
	str.Format("%d", st.wMinute); temp = temp + str;

	return temp;
}

CString gettime2()
{
	CString temp;
	CString str;
	SYSTEMTIME st;
	GetLocalTime(&st); // ��ȡ��ǰʱ��
	temp = "";

	str.Format("%d", st.wHour); temp = temp + str;
	str.Format("%d", st.wMinute); temp = temp + str;

	return temp;
}

//�߳�
unsigned __stdcall DeviceDataWork(void *strURL)
{
	char strError[1024] = { 0 }; int i; float  paymoney2 = 0;
	const char *b = "127.0.0.1";  char *a = "abcdefgh127.0.0.1";
	unsigned char tem; CString str; CString getTIME; const char * MYmd5val; Ccommon md5Class;
	unsigned int Down_Data[30];
	unsigned int CheckBox[2];  unsigned int CH; CString myAction;

	//��ȡ�豸ID
	vector<CString> vec = GetURLParamValue(CString((char*)strURL));
	if (!vec.empty())
	{
		int dtu_id = 0; int paymoney = 0; int id = 0; CString md5val; CString time; float blance = 0; int id2 = 0;
#ifdef NewFaction
		int dtu_id_A = 0; int dtu_id_B = 0;
#endif
		sscanf_s(vec[0].GetBuffer(0), "%d", &dtu_id);  //��ȡmachineid

		OutputDebug("myTEST908  not found 127.0.0.1.dtu_id:%d.", dtu_id);

		//�����¼�
		CDeviceEvent Devent;
		//ͨ��ID�����¼�,��Ҫ�ж�����--��������֪ͨ��Ӧ���¼����ö�Ӧ��ÿ���豸�ĵȴ��߳�ֹͣ������	
		HANDLE hEvent1 = Devent.GetEvent(dtu_id); //�Ȱ�ͬһ�豸����һ���¼�������еĻ��������ˣ���ֹ�ظ�Ӱ����������
		if (hEvent1 != NULL)
		{
			SetEvent(hEvent1);
		}
		if (hEvent1)
		{
			CloseHandle(hEvent1);
		}
		Devent.Create(dtu_id);
		HANDLE hEvent = Devent.GetEvent(dtu_id);


		///
		//ִ�з������ݵ��豸...............

		sscanf_s(vec[5].GetBuffer(0), "%f", &blance);
		if (blance == -1)		//��ʱ�ǻ�Ա��http://127.0.0.1:8090/?machineid=56&paymoney=2.00&id=115347&md5val=187c8207b32ee9eb37ccff8a9bf15b02&time=201611130605&blance=-1
		{
			sscanf_s(vec[1].GetBuffer(0), "%d", &paymoney);

			sscanf_s(vec[2].GetBuffer(0), "%d", &id);
#ifdef ADD_More_CH
			sscanf_s(vec[2].GetBuffer(0), "%d", &CH); //ͨ����
			if (CH > 10) CH = 0;;//���CH>10��˵������ǰ�Ĳ���ͨ����Э��ʹ�õ�����ID������ٶ�����ID������10���е���ա�

#endif
			md5val = vec[3];
			time = vec[4];

			//��֤MD5�������Ƿ���ȷ
			MYmd5val = md5Class.md5(time + "zzxt6565911");
			if (strcmp(MYmd5val, md5val) == 0)
			{
				OutputDebug2(strError);

				//�������ݸ���Ӧ�豸
				paymoney *= 100;
				Devent.Lock();
#ifdef NewFaction
				if ((dtu_id >= 3000)&(dtu_id < 7000))
				{
					dtu_id_A = dtu_id / 10; dtu_id_B = dtu_id % 10;
					tem = MENGsendToDtu(dtu_id_A, dtu_id_B, paymoney, 2); //"2"��֪ͨ�豸���ǻ�Ա����
				}
				else
				{
#ifdef ADD_More_CH
					//int pascal NetSendMyDataTrue2(int DtuID, unsigned char nMeter_ID, int money, int delayTime,int CH)

					tem = NetSendMyDataTrue2(dtu_id, 1, paymoney, 2, CH); //"2"��֪ͨ�豸���ǻ�Ա����
#else
					tem = MENGsendToDtu(dtu_id, 1, paymoney, 2); //"2"��֪ͨ�豸���ǻ�Ա����
#endif
				}
#else
				tem = MENGsendToDtu(dtu_id, 1, paymoney, 2); //"2"��֪ͨ�豸���ǻ�Ա����
#endif
#ifdef SENDforOK
				if (dtu_id < SENDTIMES_LENS)
					DevSendTimes[dtu_id] = SENDTIMES; //��֤������Ͳ��ɹ����������Ͷ�Σ�
#endif
				Devent.UnLock();
				if (tem)
				{

					::CoInitialize(NULL);//��ʼ��COM�� �߳��ڶ�дSQL�����ʼ���ͷ���ʼ�������򾭳�����SQL��д����
					CDbOperate SqlOPP;
					SqlOPP.OpenSql();
					if (SqlOPP.MyQuerySQLBB(dtu_id) == 1)
					{
						SqlOPP.UpdateSQL(dtu_id, 0);//�ǻ�Ա�����ѱ��ֱ�Ӹĳ�0д���
					}
					else
					{
						SqlOPP.InsertSQLPayRecord(dtu_id, 0);//�ǻ�Ա�����ѱ��ֱ�Ӹĳ�0д���
					}
					SqlOPP.CloseSql();
					::CoUninitialize();//����ʼ��COM��
				}
			}
			else
			{
				sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5��ƥ��2", __FILE__, __LINE__);
				OutputDebug2(strError);
			}
		}
#ifdef SENDforOK
		else  if (blance == -2)		//��ʱ�ǻ�Ա��http://127.0.0.1:8090/?machineid=56&paymoney=2.00&id=115347&md5val=187c8207b32ee9eb37ccff8a9bf15b02&time=201611130605&blance=-2
		{
			sscanf_s(vec[1].GetBuffer(0), "%d", &paymoney);
			sscanf_s(vec[2].GetBuffer(0), "%d", &id);
#ifdef ADD_More_CH
			sscanf_s(vec[2].GetBuffer(0), "%d", &CH); //ͨ����
			if (CH > 10) CH = 0;;//���CH>10��˵������ǰ�Ĳ���ͨ����Э��ʹ�õ�����ID������ٶ�����ID������10���е���ա�
#endif
			md5val = vec[3];
			time = vec[4];

			//��֤MD5�������Ƿ���ȷ
			MYmd5val = md5Class.md5(time + "zzxt6565911");
			if (strcmp(MYmd5val, md5val) == 0)
			{
				//sprintf(strError, "meng error  %d  File:%s Line = %d ","md5��ƥ��1" , __FILE__, __LINE__);
				OutputDebug2(strError);


				//�������ݸ���Ӧ�豸
				paymoney *= 100;
				// paymoney += 100;
				Devent.Lock();
#ifdef NewFaction
				if ((dtu_id >= 3000)&(dtu_id < 7000))
				{
					dtu_id_A = dtu_id / 10; dtu_id_B = dtu_id % 10;
					tem = MENGsendToDtu(dtu_id_A, dtu_id_B, paymoney, 2); //"2"��֪ͨ�豸���ǻ�Ա����
				}
				else
				{
#ifdef ADD_More_CH
					//int pascal NetSendMyDataTrue2(int DtuID, unsigned char nMeter_ID, int money, int delayTime,int CH)				
					tem = NetSendMyDataTrue2(dtu_id, 1, paymoney, 2, CH); //"2"��֪ͨ�豸���ǻ�Ա����
#else
					tem = MENGsendToDtu(dtu_id, 1, paymoney, 2); //"2"��֪ͨ�豸���ǻ�Ա����
#endif
				}
#else
				tem = MENGsendToDtu(dtu_id, 1, paymoney, 2); //"2"��֪ͨ�豸���ǻ�Ա����
#endif
				Devent.UnLock();
			}
			else
			{
				sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5��ƥ��2", __FILE__, __LINE__);
				OutputDebug2(strError);
			}
		}
#endif
#ifdef SetPAM

		else if (blance == -3)		//���ò�����http://127.0.0.1:8090/? machineid =1001& action=3& temp1=0& md5val=kfwjeklx15151sdw23&time=201611130605&blance��-3
		{
			OutputDebug("myTEST5  MENGsendToDtu blance= %d start..", blance);
			md5val = vec[3];
			time = vec[4];
			//��֤MD5�����Ƿ���ȷ
			MYmd5val = md5Class.md5(time + "zzxt6565911");
			if (strcmp(MYmd5val, md5val) == 0)
			{
				OutputDebug("myTEST5 MD5�����Ƿ���ȷ.");
				for (i = 0; i < 30; i++)
				{
					Down_Data[i] = i;
				}

				::CoInitialize(NULL);//��ʼ��COM�� �߳��ڶ�дSQL�����ʼ���ͷ���ʼ�������򾭳�����SQL��д����
				CDbOperate SqlOPP;
				SqlOPP.OpenSql();

				if (SqlOPP.MyQueryDown_DataSQL(dtu_id, Down_Data, CheckBox) == 1)
				{
					OutputDebug("myTEST5  MENGsendToDtu OpenSql= %d start..", dtu_id);

					if ((Down_Data[8] == 0) && (Down_Data[9] == 0) && (Down_Data[10] == 0) && (Down_Data[11] == 0) && (Down_Data[12] == 0) && (Down_Data[13] == 0) && (Down_Data[14] == 0) && (Down_Data[15] == 0))
					{
						OutputDebug("myTEST5  q");

						if ((Down_Data[0] != 0) || (Down_Data[1] != 0) || (Down_Data[2] != 0) || (Down_Data[3] != 0) || (Down_Data[4] != 0) || (Down_Data[5] != 0) || (Down_Data[6] != 0) || (Down_Data[7] != 0))
						{
							Devent.Lock();
#ifdef NewFaction
							if ((dtu_id >= 3000)&(dtu_id < 7000))
							{
								dtu_id_A = dtu_id / 10; dtu_id_B = dtu_id % 10;
								tem = MENGsendDown_Data(dtu_id_A, dtu_id_B, Down_Data, 0); ////����ǰ8������
							}
							else
							{
								tem = MENGsendDown_Data(dtu_id, 1, Down_Data, 0); ////����ǰ8������
							}
#else
							tem = MENGsendDown_Data(dtu_id, 1, Down_Data, 0); ////����ǰ8������
#endif

							OutputDebug("myTEST5  MENGsendToDtu tem= %d start..", tem);
							Devent.UnLock();
						}
					}
					if ((Down_Data[0] == 0) && (Down_Data[1] == 0) && (Down_Data[2] == 0) && (Down_Data[3] == 0) && (Down_Data[4] == 0) && (Down_Data[5] == 0) && (Down_Data[6] == 0) && (Down_Data[7] == 0))
					{
						OutputDebug("myTEST5  wwww");
						if ((Down_Data[8] != 0) || (Down_Data[9] != 0) || (Down_Data[10] != 0) || (Down_Data[11] != 0) || (Down_Data[12] != 0) || (Down_Data[13] != 0) || (Down_Data[14] != 0) || (Down_Data[15] != 0))
						{
							Devent.Lock();
#ifdef NewFaction
							if ((dtu_id >= 3000)&(dtu_id < 7000))
							{
								dtu_id_A = dtu_id / 10; dtu_id_B = dtu_id % 10;
								tem = MENGsendDown_Data(dtu_id_A, dtu_id_B, Down_Data, 8); //���͵ڶ���8������
							}
							else
							{
								tem = MENGsendDown_Data(dtu_id, 1, Down_Data, 8); //���͵ڶ���8������
							}
#else
							tem = MENGsendDown_Data(dtu_id, 1, Down_Data, 8); //���͵ڶ���8������
#endif

							OutputDebug("myTEST5  MENGsendToDtu tem2= %d start..", tem);
							Devent.UnLock();
						}
					}
				}

				for (i = 0; i < 30; i++) Down_Data[i] = 0; for (i = 0; i < 2; i++) CheckBox[i] = 0;   //����������ݣ��ȴ������ϴ�
				if (SqlOPP.myUpdateDown_DataSQL(dtu_id, Down_Data, CheckBox) == true)
				{
					//		AfxMessageBox("1�ɹ�");
				}
				SqlOPP.CloseSql();
				::CoUninitialize();//����ʼ��COM��

			}
			else
			{
				sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5��ƥ��2", __FILE__, __LINE__);
				OutputDebug2(strError);
			}

		}
#endif
#ifdef ShuiQuanZuanYong
		else if (blance == -4)		//http://xxx.Ӳ���豸.xxx:8090/?machineid=1001&AlreadyUsed_Money=��ǰ���ý��&temp=0&md5val=kfwjeklx15151sdw23&time=201611130605&blance�� - 4
		{
			OutputDebug("myTEST5  MENGsendToDtu blance= %d start..", blance);
			md5val = vec[3];
			time = vec[4];
			//��֤MD5�����Ƿ���ȷ
			MYmd5val = md5Class.md5(time + "zzxt6565911");
			int AlreadyUsed_Money;
			sscanf_s(vec[1].GetBuffer(0), "%d", &AlreadyUsed_Money);
			if (strcmp(MYmd5val, md5val) == 0)
			{
				OutputDebug("myTEST5 MD5�����Ƿ���ȷ.");
				Devent.Lock();
				tem = MENGsendToDtu(dtu_id, 1, AlreadyUsed_Money, 4); //"4"��֪ͨ�豸����ʾ���յ��������ý��
				OutputDebug("myTEST5  MENGsendToDtu tem= %d start..", tem);
				Devent.UnLock();
			}
			else
			{
				sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5��ƥ��2", __FILE__, __LINE__);
				OutputDebug2(strError);

			}

		}
#endif
		else  		//��Ա����Ա��http://127.0.0.1:8090/?machineid=56&paymoney=6.0&id=115347&md5val=kfwjeklx15151sdw23&time=201611130605&blance=94
					//http://xxx.xxx.xxx.xxx:8001/�ӿ��ļ���machineid=1001&paymoney=6.00&id=115347&md5val=kfwjeklx15151sdw23&time=201611130605&blance��94
		{
#ifdef UseFloatCanSHU
			sscanf_s(vec[1].GetBuffer(0), "%f", &paymoney2);
#else
			sscanf_s(vec[1].GetBuffer(0), "%d", &paymoney);
#endif
			sscanf_s(vec[2].GetBuffer(0), "%d", &id);
			//	sscanf_s(vec[2].GetBuffer(1), "%d", &id2);
#ifdef ADD_More_CH
			if ((vec.size()) == 8)
			{

				//http://127.0.0.1:8090/?machineid=2&paymoney=2.00&id=7788999&md5val=187c8207b32ee9eb37ccff8a9bf15b&time=201611130605&blance=30&channel=10&myaction=MoreChannel
				sscanf_s(vec[6].GetBuffer(0), "%d", &CH); //ͨ����
				myAction = vec[7];//����

			}
			else
			{
				CH = 0;
				myAction = "";
			}
#endif
			md5val = vec[3];
			time = vec[4];

			//const char * name = md5Class.md5("201611130605zzxt6565911");
			//��֤MD5�������Ƿ���ȷ
			MYmd5val = md5Class.md5(time + "zzxt6565911");
			if (strcmp(MYmd5val, md5val) == 0)
			{
				//	sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5��ƥ��1", __FILE__, __LINE__);
				OutputDebug2(strError);
				//�������ݿ⼸���ֶ�

				//�������ݸ���Ӧ�豸
#ifdef UseFloatCanSHU
				paymoney = paymoney2 * 100;
#else
				paymoney *= 100;
#endif
				blance *= 100;
#ifdef LinShi1
				if (blance >= 100)
#endif
				{
					//����ǰ���ж��Ƿ�ǰ������ʹ�õ��û���
					::CoInitialize(NULL);//��ʼ��COM�� �߳��ڶ�дSQL�����ʼ���ͷ���ʼ�������򾭳�����SQL��д����
					CDbOperate SqlOPP;
					SqlOPP.OpenSql();
					OutputDebug("myTEST908  555555555555555555not found 127.0.0.1.dtu_id:%d.", dtu_id);
#ifdef ConfirmJS
					if (SqlOPP.MyQuerySQL3(dtu_id) == 0) //==0֤��û������ʹ�õ��û�������ִ�������
					{
						OutputDebug("myTEST908  6666666666666666666not found 127.0.0.1.dtu_id:%d.", dtu_id);
						Devent.Lock();
						//	OutputDebug("myTEST4  MENGsendToDtu  start..");
#ifdef NewFaction
						if ((dtu_id >= 3000)&(dtu_id < 7000))
						{
							dtu_id_A = dtu_id / 10; dtu_id_B = dtu_id % 10;
							tem = MENGsendToDtu(dtu_id_A, dtu_id_B, blance, 1); //�����ҪӲ����ʾ�û����Ͱ�paymoney��Ϊblance
						}
						else
						{
#ifdef ADD_More_CH
							//int pascal NetSendMyDataTrue2(int DtuID, unsigned char nMeter_ID, int money, int delayTime,int CH)

							if (strcmp(myAction, "MoreChannelStartCH") == 0)
							{
								tem = NetSendMyDataTrue3(dtu_id, 1, blance, 1, CH, paymoney); //"2"��֪ͨ�豸���ǻ�Ա����
							}
							else
							{
								tem = MENGsendToDtu(dtu_id, 1, blance, 1); //�����ҪӲ����ʾ�û����Ͱ�paymoney��Ϊblance
							}
#else
							tem = MENGsendToDtu(dtu_id, 1, blance, 1); //�����ҪӲ����ʾ�û����Ͱ�paymoney��Ϊblance
#endif

						}
#else
						tem = MENGsendToDtu(dtu_id, 1, blance, 1); //�����ҪӲ����ʾ�û����Ͱ�paymoney��Ϊblance
#endif
						Devent.UnLock();
					}
#endif
					//	tem = MENGsendToDtu(dtu_id, 1, paymoney, 1);
					OutputDebug("myTEST908  7777777777not found 127.0.0.1.dtu_id:%d.", dtu_id);
					if (tem)//���ͳɹ�����dtu_id�� ���ѿ�id��д�����ݿ�
					{
						//	CDbOperate SqlOPP;
						OutputDebug("myTEST908  88888888888not found 127.0.0.1.dtu_id:%d.", dtu_id);

						if (SqlOPP.MyQuerySQLBB(dtu_id) == 1)
						{
							//		AfxMessageBox("1�ɹ�");
							OutputDebug("myTEST908  888880000000not found 127.0.0.1.dtu_id:%d.", dtu_id);
#ifdef JavaResolvesConflicts
							SqlOPP.nnUpdateSQL(dtu_id, id, 0);//
#else
							SqlOPP.UpdateSQL(dtu_id, id);//
#endif
						}
						else
						{
							OutputDebug("myTEST908  333333333333not found 127.0.0.1.dtu_id:%d.", dtu_id);
#ifdef JavaResolvesConflicts
							SqlOPP.nnInsertSQLPayRecord(dtu_id, id, 0);
#else
							SqlOPP.InsertSQLPayRecord(dtu_id, id);
#endif
						}
					}
					else
					{

					}
					//	AfxMessageBox("3�ɹ�");
					SqlOPP.CloseSql();
					::CoUninitialize();//����ʼ��COM��
				}
			}
			else
			{
				sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5��ƥ��2", __FILE__, __LINE__);
				OutputDebug2(strError);
			}
		}

		/////////////////////////
		if (hEvent != NULL)
		{
			//�ȴ��¼�
			DWORD ret = WaitForSingleObject(hEvent, 4000); //�ȴ���ʱ  6000��6000���룬����6��
			if (ret == WAIT_OBJECT_0)
			{
				//���ͳɹ���Ϣ��PHP��������http://zizhu.sdxjyjd.com/notify.php?machineid=56&kk=���ѽ��&id=115347&md5val=kfwjeklx15151sdw23&time=201611130605&error=0

				getTIME = gettime();
				MYmd5val = md5Class.md5(getTIME + "zzxt6565911");

				// CString tt = "http://zizhu.sdxjyjd.com/notify.php";
				CString tt = "http://127.0.0.1/notify.php";
				//CString tt = "http://127.0.0.1:8090"; //���������������������
				tt += "?machineid=";
				str.Format("%d", dtu_id); tt = tt + str;
				tt += "&kk=";
				str.Format("%d", 0); tt = tt + str;//��ʱ��Ա����������=0��
				tt += "&id=";
				str.Format("%d", id); tt = tt + str;

				tt += "&md5val=";
				tt = tt + MYmd5val;
				tt += "&time=";
				tt = tt + getTIME;
				tt += "&error=";
				str.Format("%d", 0); tt = tt + str;
				OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
#ifdef SENDforOK
				DevSendTimes[dtu_id] = 0;  //��������޵�ʱ���ڷ��ͳɹ�����ô��ֹͣ��εķ���
#endif
				Devent.Lock();
				SendBACKtoPHPserver(tt);
				Devent.UnLock();
			}
			else
			{
				SYSTEMTIME st;
				GetLocalTime(&st); // ��ȡ��ǰʱ��

				getTIME = gettime();
				MYmd5val = md5Class.md5(getTIME + "zzxt6565911");

				// CString tt = "http://zizhu.sdxjyjd.com/notify.php";
				CString tt = "http://127.0.0.1/notify.php";
				//CString tt = "http://127.0.0.1:8090"; //���������������������
				tt += "?machineid=";
				str.Format("%d", dtu_id); tt = tt + str;
				tt += "&kk=";
				str.Format("%d", 0); tt = tt + str;//��ʱ��Ա����������=0��
				tt += "&id=";
				str.Format("%d", id); tt = tt + str;

				tt += "&md5val=";
				tt = tt + MYmd5val;
				tt += "&time=";
				tt = tt + getTIME;
				tt += "&error=";
				str.Format("%d", 1); tt = tt + str;
				OutputClientString(tt);//�������ʹ�����Ϊ��ʾ
				Devent.Lock();
				SendBACKtoPHPserver(tt);
				Devent.UnLock();
#ifdef SENDforOK
				if (DevSendTimes[dtu_id] > 0)
				{//��ʱ�ǻ�Ա��http://127.0.0.1:8090/?machineid=56&paymoney=2.00&id=115347&md5val=187c8207b32ee9eb37ccff8a9bf15b02&time=201611130605&blance=-2

					paymoney = paymoney / 100;
					//	dtu_id++;
					DevSendTimes[dtu_id] = DevSendTimes[dtu_id] - 1;
					//	DevSendTimes[1] = DevSendTimes[1] - 1;
					//	CString tt2 = "http://zizhu.sdxjyjd.com/RepeatSend.php";
					//	CString tt2 = "http://127.0.0.1:8090/";
					CString tt2 = "http://127.0.0.1/RepeatSend.php";
					tt2 += "?machineid=";
					//dtu_id++;
					str.Format("%d", dtu_id); tt2 = tt2 + str;
					tt2 += "&paymoney=";
					str.Format("%d", paymoney); tt2 = tt2 + str;

					tt2 += "&id=";
					str.Format("%d", id); tt2 = tt2 + str;

					tt2 += "&md5val=";
					tt2 = tt2 + MYmd5val;
					tt2 += "&time=";
					tt2 = tt2 + getTIME;
					tt2 += "&blance=";
					str.Format("%d", -2); tt2 = tt2 + str;
					OutputClientString(tt2);//�������ʹ�����Ϊ��ʾ
				// tt2 = "http://127.0.0.1:8090/?machineid=56&paymoney=2.00&id=115347&md5val=187c8207b32ee9eb37ccff8a9bf15b02&time=201611130605&blance=-2";
					Devent.Lock();
					SendBACKtoPHPserver(tt2);
					Devent.UnLock();

				}
#endif
				//��ʱ����
			}

		}
		Devent.Delete(dtu_id);//ɾ���¼�
		if (hEvent)
		{
			CloseHandle(hEvent);
		}
	}
	//_endthreadex(0);//_beginthreadex���˳�RETUNE���Դ�������Ҫ���
	/*ע�⣺
_endthread ���Զ��ر��߳̾���� ������Ϊ�� Win32 ExitThread API ��ͬ������ˣ�����ʹ�� _beginthread �� _endthread ʱ���벻Ҫͨ������ Win32 CloseHandle API ����ʽ�ر��߳̾����

�� Win32 ExitThread API ��ͬ��_endthreadex ����ر��߳̾���� ��ˣ�����ʹ�� _beginthreadex �� _endthreadex ʱ������ͨ������ Win32 CloseHandle API ���ر��߳̾����

_endthread �� _endthreadex �ᵼ�� C++ ���������ڲ�����õ��߳��д��ڹ���״̬

���ߵ�����
beginthreadex��һ��C����ʱ��ĺ�����CreateThread��һ��ϵͳAPI�� ����_beginthreadex�ڲ�������CreateThread��ΪʲôҪ�������أ���ΪC ���п�������һЩ����ʹ����ȫ���������ʹ��CreateThread �������ʹ����ЩC ���п�ĺ������ͻ���ֲ���ȫ�����⡣�� _beginthreadex Ϊ��Щȫ�ֱ������˴���ʹ��ÿ���̶߳���һ�ݶ����ġ�ȫ�֡�����


	*/
	return 0;
}

//�������������ݻص�
bool callback_OnServerRecv(CString strURL)
{
	OutputDebug("myTEST  callback_OnServerRecv()  start..");
	static CString buff[1000];
	static int Count = 0;
#ifdef PreventMemoryOverflow
	buff[Count] = strURL;
	if (strstr(((char *)strURL.GetBuffer()), "127.0.0.1") == NULL) //���˵�һЩ��ǽ���򣬼�ȥһЩ�鷳��
	{
		OutputDebug("myTEST908  not found 127.0.0.1..");
		return -21;
	}

	//��ȡ�豸ID
	vector<CString> vec = GetURLParamValue(strURL);
	if (!vec.empty())
	{
		if ((vec.size()) < 6)
		{
			OutputDebug("myTEST vector����Ԫ��С��6......");
			return -1;
		}
#ifdef ADD_More_CH
		if ((vec.size()) > 8)
		{
			OutputDebug("myTEST vector����Ԫ�ش���8......");
			return -2;
		}
#else
		if ((vec.size()) > 6)
		{
			OutputDebug("myTEST vector����Ԫ�ش���6......");
			return -2;
		}
#endif

		int dtu_id = 0;
		sscanf_s(vec[0].GetBuffer(0), "%d", &dtu_id);  //��ȡmachineid
		threadPool.enqueue([](void* buffer) {
			DeviceDataWork(buffer);
		}, dtu_id, (void*)buff[Count].GetBuffer());
	}
#else
	buff[Count] = strURL;
	_beginthreadex(NULL, 0, DeviceDataWork, (void*)buff[Count].GetBuffer(0), 0, 0);
#endif
	Count++;
	if (Count >= 1000)
	{
		Count = 0;
	}
	OutputDebug("myTEST  callback_OnServerRecv()   end..");
	return true;
}
