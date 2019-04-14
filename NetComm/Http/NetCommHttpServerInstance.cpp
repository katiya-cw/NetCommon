#include "NetCommHttpServerInstance.h"
#include "EvppHttpServer.h"
#include "EvppHttpClientManager.h"
#include "DeviceEvent.h"
#include "Utils/Utils.h"
#include "Utils/NetClient.h"
#include "db/DbOperate.h"
#include "MD5/common.h"
#include <vector>
#include <functional>

using namespace std::placeholders;

unsigned DeviceDataWork(std::string uri);

void SendBACKtoPHPserver(CString text)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	std::string ret = CEvppHttpClientManager::getInstance()->request(text.GetBuffer());
	LOG_INFO << "Send to PHP server response : " << ret;
}

bool CNetCommHttpServerInstance::start(int threadNum, int port) {
	CEvppHttpServer::getInstance()->registerHandler("/",
		std::bind(&CNetCommHttpServerInstance::handler, this, _1, _2, _3));
	std::vector<int> ports;
	ports.push_back(port);
	CEvppHttpServer::getInstance()->start(threadNum, ports);
	return true;
}

void CNetCommHttpServerInstance::handler(evpp::EventLoop* loop,
	const evpp::http::ContextPtr& ctx,
	const evpp::http::HTTPSendResponseCallback& cb) {
	std::string machineIdKey = "machineid";
	std::string machineIdValue = ctx->GetQuery(machineIdKey.c_str(), machineIdKey.length());
	int machineId = atoi(machineIdValue.c_str());
	std::string uri = ctx->original_uri();
	std::string response = "<html><head><title></title></head><body>" + 
		uri + "</body></html>\r\n";
	cb(response);
	m_threadPool.enqueue([](std::string uri) {
		DeviceDataWork(uri);
	}, machineId, uri);
}

//�߳�
unsigned DeviceDataWork(std::string uri)
{
	const char * MYmd5val; Ccommon md5Class;
	char strError[1024] = { 0 }; 
	int i;
	float  paymoney2 = 0;
	const char *b = "127.0.0.1";  char *a = "abcdefgh127.0.0.1";
	unsigned char tem = 0x0; 
	CString str; CString getTIME; 
	unsigned int Down_Data[30];
	unsigned int CheckBox[2];  unsigned int CH; CString myAction;

	//��ȡ�豸ID
	std::vector<CString> vec = GetURLParamValue(CString(uri.c_str()));
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
		HANDLE hEvent = Devent.GetEvent(dtu_id); //�Ȱ�ͬһ�豸����һ���¼�������еĻ��������ˣ���ֹ�ظ�Ӱ����������
		if (hEvent != NULL)
		{
			// ������֣�dtu_id ֻ������һ���̴߳����������ﴦ��һ�£�ͬʱ��¼��־
			LOG_ERROR << "dtu id work is duplicate : " << dtu_id;
			SetEvent(hEvent);
			CloseHandle(hEvent);
			Devent.Delete(dtu_id);
		}
		Devent.Create(dtu_id);
		hEvent = Devent.GetEvent(dtu_id);

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
