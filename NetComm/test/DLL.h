#if !defined(________H_________)
#define ________H_________
#include "../stdafx.h"
#include "../../MyDefine.h"
#include "../DeviceEvent.h"
#include "db/DbOperate.h""
#include <vector>
//引入头文件   
#include "MD5/common.h"  
#include "Utils/Utils.h"
#include "Utils/NetClient.h"

extern unsigned char MENGsendToDtu(unsigned long DevID, unsigned char Dev_id, unsigned long money, unsigned long deyTime);
extern void SendBACKtoPHPserver(CString text);
//发送到客户端列表显示
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

int		mynDut_ID;
int     mynMeter_ID;

/*
返回 返回发送的数据长度 -2未连接

  参数
pData 需要发送的字符串
nLen  字符串长度
*/

bool callBack_GetDataFunction(int nDut_ID, int nMeter_ID, int nType, long tCommn, BYTE* pData, int nDataLen)
{
	unsigned long ttemp, gg, ttemp1, ttemp2;
	int i;
	CString  mm[32];
	double dtemp; 
	unsigned char MYbb[32];
	unsigned char tem; 
	CString str; CString getTIME; 
	const char * MYmd5val; 
	Ccommon md5Class; 
	CString str2;
	CDeviceEvent Devent;  long tem3 = 0; float tem4;
	unsigned int Down_Data[30];
	unsigned int CheckBox[2];

	extern CDbOperate SqlOPP2;

	int MM, NN;
	long ee;
	OutputDebug("myTEST  callBack_GetDataFunction()  start..");
	memset(MYbb, 0, 32);//发送前先清空接收缓冲区       //X 不可重入性

	mynDut_ID = nDut_ID;
	mynMeter_ID = nMeter_ID;
	ee = tCommn;

	memcpy(MYbb, pData, 32); 

//判断是否为返回的确认码
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
	//判断是否为返回的确认码
	if (MYbb[3] == 0x04) //判断是否是确认码
	{
		//通过ID来置事件,需要判断数据--（下面是通知相应的事件，让对应于每个设备的等待线程停止工作）					
		HANDLE hEvent = Devent.GetEvent(mynDut_ID); 
		OutputDebug("myTEST907  dtu_id:%d.", mynDut_ID);
		if (hEvent != NULL)
		{
			SetEvent(hEvent);   
		}
		if (hEvent)
		{
			// CloseHandle(hEvent);
		}
	}
#ifdef SetPAM
	if (MYbb[3] == 14) //设置前8个参数返回应答
	{
		//通过ID来置事件,需要判断数据--（下面是通知相应的事件，让对应于每个设备的等待线程停止工作）					
		HANDLE hEvent = Devent.GetEvent(mynDut_ID);   //X 不可重入性
		if (hEvent != NULL)
		{
			SetEvent(hEvent); //X 不可重入性
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
			//		AfxMessageBox("1成功");
		}


	}
	if (MYbb[3] == 16) //设置后8个参数返回应答
	{
		//通过ID来置事件,需要判断数据--（下面是通知相应的事件，让对应于每个设备的等待线程停止工作）					
		HANDLE hEvent = Devent.GetEvent(mynDut_ID); //X 不可重入性
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
			//		AfxMessageBox("1成功");
		}
	}
#endif
	if (MYbb[3] == 0x07) //相当于心跳码，用来确认哪些是上线的，哪些是不在线的 最长20分钟如果没来数据就重新计算
	{
		if (SqlOPP2.MyQuerySQL(mynDut_ID) == 1)
		{
		}
		else
		{
			SqlOPP2.InsertSQLPayRecord(mynDut_ID, 0);//非会员的消费编号直接改成0写如库
		}
		SqlOPP2.my2UpdateSQL(mynDut_ID, 1);///////写入联网状态dbo.Dev_NetState 的 ConnectOK   //X 不可重入性 （因为mynDut_ID）
		connectFlgNum = 0; //刷新标志位


		if (SqlOPP2.MyQuerySQL2(mynDut_ID) == 1)
		{
		}
		else
		{
			SqlOPP2.InsertSQLPayRecord2(mynDut_ID);//如果没有该设备就通过定时包来添加该设备到表WK_SetState
		}

		unsigned  int  bb[8];
		if (MYbb[23] == 1)//缺水报警 高液位报警
		{
			// OutputDebug("myTEST 1底液位报警  000..nDut_ID=%d,nMeter_ID=%d", nDut_ID, 1);
			// bb[0] = 0x0203;
			if (bb[0] == 0x0203)
			{
				bb[0] = 0x0A03;
			}
			else
			{
				bb[0] = 0x0803;
			}

			//向微信平台发送缺水报警信息，管理员就能收到
   /*
   http://zizhu.sdxjyjd.com/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=56&msg=HighLiquidLevelWARNING高液位
			参数：
			设备编号   Set =  56
			报警内容   msg=低液位
   */
			CString tt = "http://127.0.0.1/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=";


			str.Format("%d", mynDut_ID); tt = tt + str;
			tt += "&msg=";

			//	CString test_str1 = "低液位";
			//	 tt += URLEncode(test_str1);
			tt += "WARNING";
			//	 tt += "低液位";
			tt += URLEncode("WARNING1");
			OutputClientString(tt);
			SendBACKtoPHPserver(tt);
		}
#ifdef WarningFlg
		if (MYbb[22] == 100)//缺水报警 底液位报警  0X64是不报警
#else
		if (MYbb[22] == 0)//缺水报警 底液位报警  0X64是不报警
#endif
		{
			// OutputDebug("myTEST 1底液位报警  000..nDut_ID=%d,nMeter_ID=%d", nDut_ID, 1);

			if (bb[0] == 0x0803)
			{
				bb[0] = 0x0A03;
			}
			else
			{
				bb[0] = 0x0203;
			}

			//向微信平台发送缺水报警信息，管理员就能收到
			/*
			http://zizhu.sdxjyjd.com/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=56&msg=LowLiquidLevelWARNING低液位
			参数：
			设备编号   Set =  56
			报警内容   msg=低液位
			*/
			CString tt = "http://127.0.0.1/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=";


			str.Format("%d", mynDut_ID); tt = tt + str;
			tt += "&msg=";

			tt += "WARNING";
			tt += URLEncode("WARNING3");
			getTIME = gettime2(); 
		
			OutputClientString(tt);//发给发送窗口作为提示
			SendBACKtoPHPserver(tt);
		}
		if ((MYbb[22] != 0) && (MYbb[23] != 1))
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

#endif
		SqlOPP2.my3UpdateSQL(mynDut_ID, bb);//

#ifdef ConfirmJS

		if (MYbb[24] == 0)//如果发现设备处于无人使用状态，那么就刷新在线标志位
		{

			if (SqlOPP2.MyQuerySQLBB(mynDut_ID) == 1)
			{
				//		AfxMessageBox("1成功");
				SqlOPP2.UpdateSQL(mynDut_ID, 0);//消费编号直接改成0写如库
			}
			else
			{
				SqlOPP2.InsertSQLPayRecord(mynDut_ID, 0);//消费编号直接改成0写如库
			}
#ifndef ADD_More_CH
			if (SqlOPP2.MyQuerySQLCC(mynDut_ID) == 1)
			{
				//		AfxMessageBox("1成功");
				SqlOPP2.UpdateSQLCCC(mynDut_ID, 0);//提示设备没人在用
			}
			else
			{
				SqlOPP2.InsertSQLPayRecordCCC(mynDut_ID, 0);//提示设备没人在用
			}
#endif
			//	AfxMessageBox("3成功");

		}
		else
		{

#ifndef ADD_More_CH 
			if (SqlOPP2.MyQuerySQLCC(mynDut_ID) == 1)
			{
				//		AfxMessageBox("1成功");
				SqlOPP2.UpdateSQLCCC(mynDut_ID, 1);//提示设备有人在用
			}
			else
			{
				SqlOPP2.InsertSQLPayRecordCCC(mynDut_ID, 1);//提示设备有人在用
			}
#endif		
		}


#ifdef ADD_More_CH

		int mLtemp = MYbb[28]; mLtemp <<= 8;
		mLtemp |= MYbb[27];


		if (SqlOPP2.MyQuerySQLCC(mynDut_ID) == 1)
		{
			//		AfxMessageBox("1成功");
			SqlOPP2.UpdateSQLCCC(mynDut_ID, mLtemp);//写入哪路在用，哪路为空
		}
		else
		{
			SqlOPP2.InsertSQLPayRecordCCC(mynDut_ID, mLtemp);//写入哪路在用，哪路为空
		}
#endif

#endif					

#ifdef ADD_More_CH
		if (MYbb[25] != 0)//不是0代表通道号
		{
			/*
			http://PHP后台接口文件（默认80端口）/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=56&msg=WARNING3
			注：”Set=56”代表下面的信息是针对编号为56的设备
			1> “msg=WARNING3”代表低液位报警
			2>“msg=CH10_Complete;”代表第10通道使用结束（比如对应充电站充电结束），要通过该运营商的公众号通知给对应的用户。
			*/
			CString strTTemp[10] = { "CH1_Complete","CH2_Complete","CH3_Complete","CH4_Complete","CH5_Complete","CH6_Complete","CH7_Complete","CH8_Complete","CH9_Complete","CH10_Complete" };
			CString strTTemp2[10] = { "CH1_Interupt","CH2_Interupt","CH3_Interupt","CH4_Interupt","CH5_Interupt","CH6_Interupt","CH7_Interupt","CH8_Interupt","CH9_Interupt","CH10_Interupt" };

			if (MYbb[26] == 1)//
			{
				OutputDebug("通道%使用完成....", MYbb[25]);
				CString tt = "http://127.0.0.1/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=";
				str.Format("%d", mynDut_ID); tt = tt + str;
				tt += "&msg=";

				// tt += "WARNING";
				tt += URLEncode(strTTemp[MYbb[25]]);

				getTIME = gettime2();

				OutputClientString(tt);//发给发送窗口作为提示
				SendBACKtoPHPserver(tt);

			}
			else  if (MYbb[26] == 2)//
			{
				OutputDebug("通道%使用中断....", MYbb[25]);
				CString tt = "http://127.0.0.1/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=";
				str.Format("%d", mynDut_ID); tt = tt + str;
				tt += "&msg=";

				// tt += "WARNING";
				tt += URLEncode(strTTemp[MYbb[25]]);

				getTIME = gettime2();

				OutputClientString(tt);//发给发送窗口作为提示
				SendBACKtoPHPserver(tt);

			}
			else  if (MYbb[26] == 3)//
			{

			}
		}
#endif
	}
	if (MYbb[3] == 0x06) //判断是卡号，10位BCD码，当控制码是06时代表的含义是上传卡号：
	{
		//					 68 01 20 06   08     00 00   0D       08 8B     00    00 06     00    04 46   00    00 32     00   00 00 00 00 00 00 00 00 00   46 14    16
		//0X00000D088B(10BCD码的卡号）
		//发送卡号给PHP服务器：get请求系统http://zizhu.sdxjyjd.com/cardcode.php?cid=9819085342&machineid=1001&md5val=kfwjeklx15151sdw23&time=201611130605

		getTIME = gettime(); //X ？？？可重入性
		MYmd5val = md5Class.md5(getTIME + "zzxt6565911");
		// CString tt = "http://zizhu.sdxjyjd.com/cardcode.php";
		CString tt = "http://127.0.0.1/cardcode.php";
		//CString tt = "http://127.0.0.1:8090"; //发给本服务器身测试用
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

		tt += "?cid=";
		tt = tt + str2;//
		tt += "&machineid=";
		str.Format("%d", mynDut_ID); tt = tt + str;

		tt += "&md5val=";
		tt = tt + MYmd5val;
		tt += "&time=";
		tt = tt + getTIME;
		OutputClientString(tt);//发给发送窗口作为提示
		SendBACKtoPHPserver(tt);
	}
	if (MYbb[3] == 0x55) //判断是手机号，11位BCD码，当控制码是55时代表的含义是上传手机号：
	{//http://zizhu.sdxjyjd.com/phonecode.php?machineid=1001&phone=6&pwd=6612151&md5val=kfwjeklx15151sdw23&time=201611130605
		//		68 01 20 55   08     00 00   0D       08 8B     00    00 06     00    04 46   00    00 32     00   00 00 00 00 00 00 00 00 00   46 14    16
		//0X00000D088B0(11BCD码的手机号） 密码0X000600
		getTIME = gettime();
		MYmd5val = md5Class.md5(getTIME + "zzxt6565911");
		//	 CString tt = "http://zizhu.sdxjyjd.com/phonecode.php";
		CString tt = "http://127.0.0.1/phonecode.php";
		//CString tt = "http://127.0.0.1:8090"; //发给本服务器身测试用

		tt += "?machineid=";
		str.Format("%d", mynDut_ID); tt = tt + str;

		tem = MYbb[5] >> 4; str.Format("%d", tem); str2 = str;//
		tem = (MYbb[5] & 0x0f); str.Format("%d", tem); str2 += str;//
		tem = MYbb[6] >> 4; str.Format("%d", tem); str2 += str; //
		tem = (MYbb[6] & 0x0f); str.Format("%d", tem); str2 += str;//
		tem = MYbb[7] >> 4; str.Format("%d", tem); str2 += str; //
		tem = (MYbb[7] & 0x0f); str.Format("%d", tem); str2 += str;//
		tem = MYbb[8] >> 4; str.Format("%d", tem); str2 += str; //
		tem = (MYbb[8] & 0x0f); str.Format("%d", tem); str2 += str;//
		tem = MYbb[9] >> 4; str.Format("%d", tem); str2 += str; //
		tem = (MYbb[9] & 0x0f); str.Format("%d", tem); str2 += str;//
		tem = MYbb[10] >> 4; str.Format("%d", tem); str2 += str; //

		tt += "&phone=";
		tt = tt + str2;//

		str2 = "";

		tem = MYbb[11] >> 4; str.Format("%d", tem); str2 = str;//
		tem = (MYbb[11] & 0x0f); str.Format("%d", tem); str2 += str;//
		tem = MYbb[12] >> 4; str.Format("%d", tem); str2 += str;//
		tem = (MYbb[12] & 0x0f); str.Format("%d", tem); str2 += str;//
		tem = MYbb[13] >> 4; str.Format("%d", tem);  str2 += str;//
		tem = (MYbb[13] & 0x0f); str.Format("%d", tem); str2 += str;//

		tt += "&pwd=";
		tt = tt + str2;//

		tt += "&md5val=";
		tt = tt + MYmd5val;
		tt += "&time=";
		tt = tt + getTIME;
		OutputClientString(tt);//发给发送窗口作为提示
		SendBACKtoPHPserver(tt);
	}
	if (MYbb[3] == 0x22) //机器完成使用后get请求：：
	{//http://zizhu.sdxjyjd.com/notify.php?machineid=1001&kk=消费金额&id=115347&md5val=kfwjeklx15151sdw23&time=201611130605&error=0
	 //68 01 20 22   08     00 0D 00       00 00  01      00 00  60        04 00  46    00 00  32      00 00 00     00 00 00     
//					 00 00 00   46 14    16
				  //本次消费金额   0X000D00 =33.28元
		getTIME = gettime();
		MYmd5val = md5Class.md5(getTIME + "zzxt6565911");

		// CString tt = "http://zizhu.sdxjyjd.com/notify.php";
#ifdef	 forSQPT_where
		CString tt = "http://xxx.xxx.xxx.xx/xxx";
#else
		CString tt = "http://127.0.0.1/notify.php";
		//CString tt = "http://127.0.0.1:8090"; //发给本服务器自身测试用
#endif

		tt += "?machineid=";
		str.Format("%d", mynDut_ID); tt = tt + str;

		// tt += "&kk=-"; //注意:这里加个"-"值，就是代表这里的余额是回收的
		tt += "&kk=";

		tem3 = MYbb[5]; tem3 = tem3 << 8;
		tem3 |= MYbb[6]; tem3 = tem3 << 8;
		tem3 |= MYbb[7];
		tem4 = tem3;
		tem4 = tem4 / 100; //后2位是小数
 ////////////////			 //给个读写标志，让其他程序比如定时器去执行写数据库的操作，这样才能符合操作系统的特点，否则容易读写数据库报错

#ifdef JavaResolvesConflicts 
		int nnMyEmpID = 0;
		unsigned char nnOpType;
		// if (SqlOPP2.MyQuerySQL(mynDut_ID) == 1) //查出对应设备ID的EmpID
		if (SqlOPP2.nnMyQuerySQL(mynDut_ID, &nnMyEmpID, &nnOpType) == 1) //查出对应设备ID的EmpID
		{
			if (nnOpType != 1)//==1留给JAVA用
			{
				//	 AfxMessageBox("92成功");
				str.Format("%f", tem4); tt = tt + str;//返回余额
				tt += "&id=";
				str.Format("%d", nnMyEmpID); tt = tt + str;//订单号						

				tt += "&md5val=";
				tt = tt + MYmd5val;
				tt += "&time=";
				tt = tt + getTIME;
				tt += "&error=";
				str.Format("%d", 0); tt = tt + str;//订单号	
												   //	 OutputDebug("myTEST908 1 NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);
				if (nnMyEmpID != 0)//只有会员才发结算命令
				{
					//	 OutputDebug("myTEST908 2  NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);

					OutputClientString(tt);//发给发送窗口作为提示
				}
				if (nnMyEmpID != 0)//只有会员才发结算命令
				{
					SendBACKtoPHPserver(tt);
				}
				OutputDebug("myTEST  NetSendMyAccount()  000..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
#ifdef ConfirmJS
				if (nnMyEmpID != 0)//
				{//清除设备在SQL数据库里面的登记或者把相应设备的订单号“MyEmpID”设置为0，
				 // OutputDebug("myTEST908 3 NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);
				 // if (nDut_ID != 11)//
					{
						if ((NetSendMyAccount(nDut_ID, nMeter_ID, 1)) == 45)//发送0X23号确认命令给设备) 加上头和尾是45个字节
						{
							OutputDebug("myTEST  NetSendMyAccount()  1..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
							SqlOPP2.UpdateSQL(mynDut_ID, 0);//把对应的硬件设备的消费编号改为0，防止下次重复扣费（如果消费机收不到确认指令）
						}
					}
					//发送0X23号确认命令给设备
					/*
					上位机得到这条信息立即返回OK确认帧给设备，确认帧一般如下：
					当控制码是23时代表结算命令发出后上位机给的确认帧：

					68 01 20 23   08     00 0D 00       00 00  01      00 00  60        04 00  46    00 00  32      00 00 00     00 00 00     00 00 00   46 14    16
					表号01             备用           确认识别        备用             备用         备用             备用         备用        备用      CRC效验
					0X000001=1
					*/


				}
				else // 如果已经删除了该设备，也就是说已经发送过0X23这个命令，这次就只发送0X23号确认命令给设备
				{
					//	if (nDut_ID !=11)//
					{
						OutputDebug("myTEST  NetSendMyAccount()  2..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
						NetSendMyAccount(nDut_ID, nMeter_ID, 1);//发送0X23号确认命令给设备
					}
				}
			}
#endif
		}
#else			
		if (SqlOPP2.MyQuerySQL(mynDut_ID) == 1) //查出对应设备ID的EmpID
		{
			//	 AfxMessageBox("92成功");
			str.Format("%f", tem4); tt = tt + str;//返回余额
			tt += "&id=";
			str.Format("%d", MyEmpID); tt = tt + str;//订单号						

			tt += "&md5val=";
			tt = tt + MYmd5val;
			tt += "&time=";
			tt = tt + getTIME;
			tt += "&error=";
			str.Format("%d", 0); tt = tt + str;//订单号	
	   //	 OutputDebug("myTEST908 1 NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);
			if (MyEmpID != 0)//只有会员才发结算命令
			{
				//	 OutputDebug("myTEST908 2  NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);

				OutputClientString(tt);//发给发送窗口作为提示
			}
			if (MyEmpID != 0)//只有会员才发结算命令
			{
				SendBACKtoPHPserver(tt);
			}
			OutputDebug("myTEST  NetSendMyAccount()  000..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
#ifdef ConfirmJS
			if (MyEmpID != 0)//
			{//清除设备在SQL数据库里面的登记或者把相应设备的订单号“MyEmpID”设置为0，
			   // OutputDebug("myTEST908 3 NetSendMyAccount()  0....MyEmpID=%d..nDut_ID=%d,nMeter_ID=%d", MyEmpID, mynDut_ID, nMeter_ID);
			   // if (nDut_ID != 11)//
				{
					if ((NetSendMyAccount(nDut_ID, nMeter_ID, 1)) == 45)//发送0X23号确认命令给设备) 加上头和尾是45个字节  //X 不可重入性
					{
						OutputDebug("myTEST  NetSendMyAccount()  1..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
						SqlOPP2.UpdateSQL(mynDut_ID, 0);//把对应的硬件设备的消费编号改为0，防止下次重复扣费（如果消费机收不到确认指令）
					}
				}
				//发送0X23号确认命令给设备
			/*
			上位机得到这条信息立即返回OK确认帧给设备，确认帧一般如下：
			当控制码是23时代表结算命令发出后上位机给的确认帧：

			68 01 20 23   08     00 0D 00       00 00  01      00 00  60        04 00  46    00 00  32      00 00 00     00 00 00     00 00 00   46 14    16
			表号01             备用           确认识别        备用             备用         备用             备用         备用        备用      CRC效验
											 0X000001=1
			*/


			}
			else // 如果已经删除了该设备，也就是说已经发送过0X23这个命令，这次就只发送0X23号确认命令给设备
			{
				//	if (nDut_ID !=11)//
				{
					OutputDebug("myTEST  NetSendMyAccount()  2..nDut_ID=%d,nMeter_ID=%d", nDut_ID, nMeter_ID);
					NetSendMyAccount(nDut_ID, nMeter_ID, 1);//发送0X23号确认命令给设备  //X 不可重入性
				}
			}
#endif
		}
#endif

	}
#ifdef ShuiQuanZuanYong
	if (MYbb[3] == 0x08) /////定时上传 瞬时流量,已用用水量,已用用电 大约1分钟一次
	{
		// http://XXX.JSP水权平台.COM/XXX?machineid=1001&flow_speed=瞬时流量&AlreadyUsed_flow_count=已用用水量&AlreadyUsed_Electrical_Quantity=已用用电量 &md5val=kfwjeklx15151sdw23&time=201611130605&error=0
		  //		68 01 20 55   08     00 00   0D       08 8B     00    00 06     00    04 46   00    00 32     00   00 00 00 00 00 00 00 00 00   46 14    16
		  //0X00000D088B0(11BCD码的手机号） 密码0X000600
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
		OutputClientString(tt);//发给发送窗口作为提示
		SendBACKtoPHPserver(tt);
	}
	if (MYbb[3] == 0x09)  ////定时上传 8个以上参数  大约10分钟一次
	{
		unsigned  long getMY_CSQ, WENDU, TDS1, TDS2, VERSION_temp;
	}
#endif
	OutputDebug("myTEST  callBack_GetDataFunction()   end..");

	return true;
}



bool callBack_GPRSFunction(int nDtu_ID, int nStatus, long lastTime)
{
	int MM, NN;long ee;
	try
	{
		MM = nDtu_ID;NN = nStatus;ee = lastTime;
	}
	catch (_com_error *e)
	{
		//  Log.Write(ex);
	}


	return true;
}
bool callBack_EXCUTEFunction(int nType, char* pText, int nTextLen)
{

	return true;
}
bool callBack_ShutDownFunction()
{
	// timer1.Enabled = true;
	return true;
}

#endif
