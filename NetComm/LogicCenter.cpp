#include "stdafx.h"
#include "LogicCenter.h"
#include "DeviceEvent.h"
#include "ThreadPool.h"
//引入头文件   
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
//获取URL的参数值
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
	GetLocalTime(&st); // 获取当前时间
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
	GetLocalTime(&st); // 获取当前时间
	temp = "";

	str.Format("%d", st.wHour); temp = temp + str;
	str.Format("%d", st.wMinute); temp = temp + str;

	return temp;
}

//线程
unsigned __stdcall DeviceDataWork(void *strURL)
{
	char strError[1024] = { 0 }; int i; float  paymoney2 = 0;
	const char *b = "127.0.0.1";  char *a = "abcdefgh127.0.0.1";
	unsigned char tem; CString str; CString getTIME; const char * MYmd5val; Ccommon md5Class;
	unsigned int Down_Data[30];
	unsigned int CheckBox[2];  unsigned int CH; CString myAction;

	//获取设备ID
	vector<CString> vec = GetURLParamValue(CString((char*)strURL));
	if (!vec.empty())
	{
		int dtu_id = 0; int paymoney = 0; int id = 0; CString md5val; CString time; float blance = 0; int id2 = 0;
#ifdef NewFaction
		int dtu_id_A = 0; int dtu_id_B = 0;
#endif
		sscanf_s(vec[0].GetBuffer(0), "%d", &dtu_id);  //获取machineid

		OutputDebug("myTEST908  not found 127.0.0.1.dtu_id:%d.", dtu_id);

		//创建事件
		CDeviceEvent Devent;
		//通过ID来置事件,需要判断数据--（下面是通知相应的事件，让对应于每个设备的等待线程停止工作）	
		HANDLE hEvent1 = Devent.GetEvent(dtu_id); //先把同一设备的上一个事件（如果有的话）结束了，防止重复影响其他订单
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
		//执行发送数据到设备...............

		sscanf_s(vec[5].GetBuffer(0), "%f", &blance);
		if (blance == -1)		//临时非会员：http://127.0.0.1:8090/?machineid=56&paymoney=2.00&id=115347&md5val=187c8207b32ee9eb37ccff8a9bf15b02&time=201611130605&blance=-1
		{
			sscanf_s(vec[1].GetBuffer(0), "%d", &paymoney);

			sscanf_s(vec[2].GetBuffer(0), "%d", &id);
#ifdef ADD_More_CH
			sscanf_s(vec[2].GetBuffer(0), "%d", &CH); //通道号
			if (CH > 10) CH = 0;;//如果CH>10，说明是以前的不带通道的协议使用的消费ID，这里假定消费ID都大于10，有点风险。

#endif
			md5val = vec[3];
			time = vec[4];

			//验证MD5加密码是否正确
			MYmd5val = md5Class.md5(time + "zzxt6565911");
			if (strcmp(MYmd5val, md5val) == 0)
			{
				OutputDebug2(strError);

				//发送数据给对应设备
				paymoney *= 100;
				Devent.Lock();
#ifdef NewFaction
				if ((dtu_id >= 3000)&(dtu_id < 7000))
				{
					dtu_id_A = dtu_id / 10; dtu_id_B = dtu_id % 10;
					tem = MENGsendToDtu(dtu_id_A, dtu_id_B, paymoney, 2); //"2"是通知设备，非会员消费
				}
				else
				{
#ifdef ADD_More_CH
					//int pascal NetSendMyDataTrue2(int DtuID, unsigned char nMeter_ID, int money, int delayTime,int CH)

					tem = NetSendMyDataTrue2(dtu_id, 1, paymoney, 2, CH); //"2"是通知设备，非会员消费
#else
					tem = MENGsendToDtu(dtu_id, 1, paymoney, 2); //"2"是通知设备，非会员消费
#endif
				}
#else
				tem = MENGsendToDtu(dtu_id, 1, paymoney, 2); //"2"是通知设备，非会员消费
#endif
#ifdef SENDforOK
				if (dtu_id < SENDTIMES_LENS)
					DevSendTimes[dtu_id] = SENDTIMES; //保证如果发送不成功就连续发送多次，
#endif
				Devent.UnLock();
				if (tem)
				{

					::CoInitialize(NULL);//初始化COM库 线程内读写SQL必须初始化和反初始化，否则经常出现SQL读写错误
					CDbOperate SqlOPP;
					SqlOPP.OpenSql();
					if (SqlOPP.MyQuerySQLBB(dtu_id) == 1)
					{
						SqlOPP.UpdateSQL(dtu_id, 0);//非会员的消费编号直接改成0写如库
					}
					else
					{
						SqlOPP.InsertSQLPayRecord(dtu_id, 0);//非会员的消费编号直接改成0写如库
					}
					SqlOPP.CloseSql();
					::CoUninitialize();//反初始化COM库
				}
			}
			else
			{
				sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5不匹配2", __FILE__, __LINE__);
				OutputDebug2(strError);
			}
		}
#ifdef SENDforOK
		else  if (blance == -2)		//临时非会员：http://127.0.0.1:8090/?machineid=56&paymoney=2.00&id=115347&md5val=187c8207b32ee9eb37ccff8a9bf15b02&time=201611130605&blance=-2
		{
			sscanf_s(vec[1].GetBuffer(0), "%d", &paymoney);
			sscanf_s(vec[2].GetBuffer(0), "%d", &id);
#ifdef ADD_More_CH
			sscanf_s(vec[2].GetBuffer(0), "%d", &CH); //通道号
			if (CH > 10) CH = 0;;//如果CH>10，说明是以前的不带通道的协议使用的消费ID，这里假定消费ID都大于10，有点风险。
#endif
			md5val = vec[3];
			time = vec[4];

			//验证MD5加密码是否正确
			MYmd5val = md5Class.md5(time + "zzxt6565911");
			if (strcmp(MYmd5val, md5val) == 0)
			{
				//sprintf(strError, "meng error  %d  File:%s Line = %d ","md5不匹配1" , __FILE__, __LINE__);
				OutputDebug2(strError);


				//发送数据给对应设备
				paymoney *= 100;
				// paymoney += 100;
				Devent.Lock();
#ifdef NewFaction
				if ((dtu_id >= 3000)&(dtu_id < 7000))
				{
					dtu_id_A = dtu_id / 10; dtu_id_B = dtu_id % 10;
					tem = MENGsendToDtu(dtu_id_A, dtu_id_B, paymoney, 2); //"2"是通知设备，非会员消费
				}
				else
				{
#ifdef ADD_More_CH
					//int pascal NetSendMyDataTrue2(int DtuID, unsigned char nMeter_ID, int money, int delayTime,int CH)				
					tem = NetSendMyDataTrue2(dtu_id, 1, paymoney, 2, CH); //"2"是通知设备，非会员消费
#else
					tem = MENGsendToDtu(dtu_id, 1, paymoney, 2); //"2"是通知设备，非会员消费
#endif
				}
#else
				tem = MENGsendToDtu(dtu_id, 1, paymoney, 2); //"2"是通知设备，非会员消费
#endif
				Devent.UnLock();
			}
			else
			{
				sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5不匹配2", __FILE__, __LINE__);
				OutputDebug2(strError);
			}
		}
#endif
#ifdef SetPAM

		else if (blance == -3)		//设置参数：http://127.0.0.1:8090/? machineid =1001& action=3& temp1=0& md5val=kfwjeklx15151sdw23&time=201611130605&blance＝-3
		{
			OutputDebug("myTEST5  MENGsendToDtu blance= %d start..", blance);
			md5val = vec[3];
			time = vec[4];
			//验证MD5加密是否正确
			MYmd5val = md5Class.md5(time + "zzxt6565911");
			if (strcmp(MYmd5val, md5val) == 0)
			{
				OutputDebug("myTEST5 MD5加密是否正确.");
				for (i = 0; i < 30; i++)
				{
					Down_Data[i] = i;
				}

				::CoInitialize(NULL);//初始化COM库 线程内读写SQL必须初始化和反初始化，否则经常出现SQL读写错误
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
								tem = MENGsendDown_Data(dtu_id_A, dtu_id_B, Down_Data, 0); ////发送前8个数据
							}
							else
							{
								tem = MENGsendDown_Data(dtu_id, 1, Down_Data, 0); ////发送前8个数据
							}
#else
							tem = MENGsendDown_Data(dtu_id, 1, Down_Data, 0); ////发送前8个数据
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
								tem = MENGsendDown_Data(dtu_id_A, dtu_id_B, Down_Data, 8); //发送第二个8个数据
							}
							else
							{
								tem = MENGsendDown_Data(dtu_id, 1, Down_Data, 8); //发送第二个8个数据
							}
#else
							tem = MENGsendDown_Data(dtu_id, 1, Down_Data, 8); //发送第二个8个数据
#endif

							OutputDebug("myTEST5  MENGsendToDtu tem2= %d start..", tem);
							Devent.UnLock();
						}
					}
				}

				for (i = 0; i < 30; i++) Down_Data[i] = 0; for (i = 0; i < 2; i++) CheckBox[i] = 0;   //清空所有数据，等待数据上传
				if (SqlOPP.myUpdateDown_DataSQL(dtu_id, Down_Data, CheckBox) == true)
				{
					//		AfxMessageBox("1成功");
				}
				SqlOPP.CloseSql();
				::CoUninitialize();//反初始化COM库

			}
			else
			{
				sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5不匹配2", __FILE__, __LINE__);
				OutputDebug2(strError);
			}

		}
#endif
#ifdef ShuiQuanZuanYong
		else if (blance == -4)		//http://xxx.硬件设备.xxx:8090/?machineid=1001&AlreadyUsed_Money=当前已用金额&temp=0&md5val=kfwjeklx15151sdw23&time=201611130605&blance＝ - 4
		{
			OutputDebug("myTEST5  MENGsendToDtu blance= %d start..", blance);
			md5val = vec[3];
			time = vec[4];
			//验证MD5加密是否正确
			MYmd5val = md5Class.md5(time + "zzxt6565911");
			int AlreadyUsed_Money;
			sscanf_s(vec[1].GetBuffer(0), "%d", &AlreadyUsed_Money);
			if (strcmp(MYmd5val, md5val) == 0)
			{
				OutputDebug("myTEST5 MD5加密是否正确.");
				Devent.Lock();
				tem = MENGsendToDtu(dtu_id, 1, AlreadyUsed_Money, 4); //"4"是通知设备，表示接收到的是已用金额
				OutputDebug("myTEST5  MENGsendToDtu tem= %d start..", tem);
				Devent.UnLock();
			}
			else
			{
				sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5不匹配2", __FILE__, __LINE__);
				OutputDebug2(strError);

			}

		}
#endif
		else  		//会员：会员：http://127.0.0.1:8090/?machineid=56&paymoney=6.0&id=115347&md5val=kfwjeklx15151sdw23&time=201611130605&blance=94
					//http://xxx.xxx.xxx.xxx:8001/接口文件？machineid=1001&paymoney=6.00&id=115347&md5val=kfwjeklx15151sdw23&time=201611130605&blance＝94
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
				sscanf_s(vec[6].GetBuffer(0), "%d", &CH); //通道号
				myAction = vec[7];//功能

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
			//验证MD5加密码是否正确
			MYmd5val = md5Class.md5(time + "zzxt6565911");
			if (strcmp(MYmd5val, md5val) == 0)
			{
				//	sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5不匹配1", __FILE__, __LINE__);
				OutputDebug2(strError);
				//更新数据库几个字段

				//发送数据给对应设备
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
					//发送前先判断是否当前有正在使用的用户，
					::CoInitialize(NULL);//初始化COM库 线程内读写SQL必须初始化和反初始化，否则经常出现SQL读写错误
					CDbOperate SqlOPP;
					SqlOPP.OpenSql();
					OutputDebug("myTEST908  555555555555555555not found 127.0.0.1.dtu_id:%d.", dtu_id);
#ifdef ConfirmJS
					if (SqlOPP.MyQuerySQL3(dtu_id) == 0) //==0证明没有正在使用的用户，才能执行下面的
					{
						OutputDebug("myTEST908  6666666666666666666not found 127.0.0.1.dtu_id:%d.", dtu_id);
						Devent.Lock();
						//	OutputDebug("myTEST4  MENGsendToDtu  start..");
#ifdef NewFaction
						if ((dtu_id >= 3000)&(dtu_id < 7000))
						{
							dtu_id_A = dtu_id / 10; dtu_id_B = dtu_id % 10;
							tem = MENGsendToDtu(dtu_id_A, dtu_id_B, blance, 1); //如果需要硬件显示用户余额，就把paymoney改为blance
						}
						else
						{
#ifdef ADD_More_CH
							//int pascal NetSendMyDataTrue2(int DtuID, unsigned char nMeter_ID, int money, int delayTime,int CH)

							if (strcmp(myAction, "MoreChannelStartCH") == 0)
							{
								tem = NetSendMyDataTrue3(dtu_id, 1, blance, 1, CH, paymoney); //"2"是通知设备，非会员消费
							}
							else
							{
								tem = MENGsendToDtu(dtu_id, 1, blance, 1); //如果需要硬件显示用户余额，就把paymoney改为blance
							}
#else
							tem = MENGsendToDtu(dtu_id, 1, blance, 1); //如果需要硬件显示用户余额，就把paymoney改为blance
#endif

						}
#else
						tem = MENGsendToDtu(dtu_id, 1, blance, 1); //如果需要硬件显示用户余额，就把paymoney改为blance
#endif
						Devent.UnLock();
					}
#endif
					//	tem = MENGsendToDtu(dtu_id, 1, paymoney, 1);
					OutputDebug("myTEST908  7777777777not found 127.0.0.1.dtu_id:%d.", dtu_id);
					if (tem)//发送成功，把dtu_id和 消费卡id号写入数据库
					{
						//	CDbOperate SqlOPP;
						OutputDebug("myTEST908  88888888888not found 127.0.0.1.dtu_id:%d.", dtu_id);

						if (SqlOPP.MyQuerySQLBB(dtu_id) == 1)
						{
							//		AfxMessageBox("1成功");
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
					//	AfxMessageBox("3成功");
					SqlOPP.CloseSql();
					::CoUninitialize();//反初始化COM库
				}
			}
			else
			{
				sprintf(strError, "meng error  %d  File:%s Line = %d ", "md5不匹配2", __FILE__, __LINE__);
				OutputDebug2(strError);
			}
		}

		/////////////////////////
		if (hEvent != NULL)
		{
			//等待事件
			DWORD ret = WaitForSingleObject(hEvent, 4000); //等待超时  6000是6000毫秒，就是6秒
			if (ret == WAIT_OBJECT_0)
			{
				//发送成功消息给PHP服务器：http://zizhu.sdxjyjd.com/notify.php?machineid=56&kk=消费金额&id=115347&md5val=kfwjeklx15151sdw23&time=201611130605&error=0

				getTIME = gettime();
				MYmd5val = md5Class.md5(getTIME + "zzxt6565911");

				// CString tt = "http://zizhu.sdxjyjd.com/notify.php";
				CString tt = "http://127.0.0.1/notify.php";
				//CString tt = "http://127.0.0.1:8090"; //发给本服务器自身测试用
				tt += "?machineid=";
				str.Format("%d", dtu_id); tt = tt + str;
				tt += "&kk=";
				str.Format("%d", 0); tt = tt + str;//临时会员不返回余额（即=0）
				tt += "&id=";
				str.Format("%d", id); tt = tt + str;

				tt += "&md5val=";
				tt = tt + MYmd5val;
				tt += "&time=";
				tt = tt + getTIME;
				tt += "&error=";
				str.Format("%d", 0); tt = tt + str;
				OutputClientString(tt);//发给发送窗口作为提示
#ifdef SENDforOK
				DevSendTimes[dtu_id] = 0;  //如果在有限的时间内发送成功，那么就停止多次的发送
#endif
				Devent.Lock();
				SendBACKtoPHPserver(tt);
				Devent.UnLock();
			}
			else
			{
				SYSTEMTIME st;
				GetLocalTime(&st); // 获取当前时间

				getTIME = gettime();
				MYmd5val = md5Class.md5(getTIME + "zzxt6565911");

				// CString tt = "http://zizhu.sdxjyjd.com/notify.php";
				CString tt = "http://127.0.0.1/notify.php";
				//CString tt = "http://127.0.0.1:8090"; //发给本服务器自身测试用
				tt += "?machineid=";
				str.Format("%d", dtu_id); tt = tt + str;
				tt += "&kk=";
				str.Format("%d", 0); tt = tt + str;//临时会员不返回余额（即=0）
				tt += "&id=";
				str.Format("%d", id); tt = tt + str;

				tt += "&md5val=";
				tt = tt + MYmd5val;
				tt += "&time=";
				tt = tt + getTIME;
				tt += "&error=";
				str.Format("%d", 1); tt = tt + str;
				OutputClientString(tt);//发给发送窗口作为提示
				Devent.Lock();
				SendBACKtoPHPserver(tt);
				Devent.UnLock();
#ifdef SENDforOK
				if (DevSendTimes[dtu_id] > 0)
				{//临时非会员：http://127.0.0.1:8090/?machineid=56&paymoney=2.00&id=115347&md5val=187c8207b32ee9eb37ccff8a9bf15b02&time=201611130605&blance=-2

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
					OutputClientString(tt2);//发给发送窗口作为提示
				// tt2 = "http://127.0.0.1:8090/?machineid=56&paymoney=2.00&id=115347&md5val=187c8207b32ee9eb37ccff8a9bf15b02&time=201611130605&blance=-2";
					Devent.Lock();
					SendBACKtoPHPserver(tt2);
					Devent.UnLock();

				}
#endif
				//超时处理
			}

		}
		Devent.Delete(dtu_id);//删除事件
		if (hEvent)
		{
			CloseHandle(hEvent);
		}
	}
	//_endthreadex(0);//_beginthreadex的退出RETUNE，自带，不需要添加
	/*注意：
_endthread 会自动关闭线程句柄。 （该行为与 Win32 ExitThread API 不同。）因此，当你使用 _beginthread 和 _endthread 时，请不要通过调用 Win32 CloseHandle API 来显式关闭线程句柄。

与 Win32 ExitThread API 相同，_endthreadex 不会关闭线程句柄。 因此，当你使用 _beginthreadex 和 _endthreadex 时，必须通过调用 Win32 CloseHandle API 来关闭线程句柄。

_endthread 和 _endthreadex 会导致 C++ 析构函数在不会调用的线程中处于挂起状态

两者的区别：
beginthreadex是一个C运行时库的函数，CreateThread是一个系统API函 数，_beginthreadex内部调用了CreateThread。为什么要有两个呢？因为C 运行库里面有一些函数使用了全局量，如果使用CreateThread 的情况下使用这些C 运行库的函数，就会出现不安全的问题。而 _beginthreadex 为这些全局变量做了处理，使得每个线程都有一份独立的“全局”量。


	*/
	return 0;
}

//服务器接收数据回调
bool callback_OnServerRecv(CString strURL)
{
	OutputDebug("myTEST  callback_OnServerRecv()  start..");
	static CString buff[1000];
	static int Count = 0;
#ifdef PreventMemoryOverflow
	buff[Count] = strURL;
	if (strstr(((char *)strURL.GetBuffer()), "127.0.0.1") == NULL) //过滤掉一些爬墙程序，减去一些麻烦，
	{
		OutputDebug("myTEST908  not found 127.0.0.1..");
		return -21;
	}

	//获取设备ID
	vector<CString> vec = GetURLParamValue(strURL);
	if (!vec.empty())
	{
		if ((vec.size()) < 6)
		{
			OutputDebug("myTEST vector数组元素小于6......");
			return -1;
		}
#ifdef ADD_More_CH
		if ((vec.size()) > 8)
		{
			OutputDebug("myTEST vector数组元素大于8......");
			return -2;
		}
#else
		if ((vec.size()) > 6)
		{
			OutputDebug("myTEST vector数组元素大于6......");
			return -2;
		}
#endif

		int dtu_id = 0;
		sscanf_s(vec[0].GetBuffer(0), "%d", &dtu_id);  //获取machineid
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
