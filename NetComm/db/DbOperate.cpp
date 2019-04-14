#include "stdafx.h"
#include "afxdb.h"
#include "DbOperate.h"
#include "../../MyDefine.h"

int MyEmpID = 0;
CDbOperate::CDbOperate()
{ //用IP=127.0.0.1链接有时候连接不上
	//m_sqlAddr = "Provider=SQLOLEDB.1;Server=127.0.0.1;DATABASE=WXData;UID=sa;PWD=123;";//自己测试电脑上的配置
  // m_sqlAddr = "Provider=SQLOLEDB.1;Server=127.0.0.1;DATABASE=wxdata;UID=wxdata;PWD=3239508152;"; //云平台上的密码  118.190.86.76
	m_sqlAddr = "Provider=SQLOLEDB.1;Server=127.0.0.1;DATABASE=WXData;UID=sa;PWD=xh6565911;";  //云平台上的密码   115.28.78.161上面的
   //m_sqlAddr = "Provider=SQLOLEDB.1;Server=iZm5e727lmif5mZ;DATABASE==WXData;UID=sa;PWD=xh6565911;"; //云平台上的密码
   //	m_sqlAddr = "Provider=SQLOLEDB.1;Server=101.200.204.188;DATABASE=WXData;UID=sa;PWD=xh6565911;"; //云平台上的密码
	//  m_sqlAddr = "Provider=SQLOLEDB.1;Server=127.0.0.1;DATABASE=WXData;UID=sa;PWD=xh6565911;"; //云平台上的密码  118.190.86.76	//需要打开远程数据库上的对外连接

   //	  m_sqlAddr = "Provider=SQLOLEDB.1;Server=localhost;DATABASE=wxdata;UID=sa;PWD=xh6565911;"; //云平台上的密码  118.190.86.76	//需要打开远程数据库上的对外连接
   //	  m_sqlAddr = "Provider=SQLOLEDB.1;Server=47.104.168.94;DATABASE=wxdata;UID=sa;PWD=xh6565911;";

	m_pSqlCon = &m_db;
	m_accAddr = "Provider=Microsoft.Jet.OLEDB.4.0;	Data Source = MonitorData.mdb";
	m_sqlUser.user_name = "";
	m_sqlUser.user_pass = "";

	m_accUser.user_name = "";
	m_accUser.user_pass = "";
}

CDbOperate::~CDbOperate()
{

}

//打开SQL数据库
bool CDbOperate::OpenSql()
{
	HRESULT hr;
	OutputDebug("myTEST  CDbOperate::OpenSql()  start..");

	try
	{
		m_pSqlCon->Open(_T("mengHX"), FALSE, FALSE, _T("ODBC;UID=sa;PWD=xh6565911"));
		OutputDebug("myTEST  CDbOperate::OpenSql()   end..");

		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		errorMsg.Format("连接数据库失败!\r\n错误信息:%s", e.ErrorMessage());
		MessageBox(NULL, errorMsg, "错误", MB_OK);
		return false;
	}
}
//关闭SQL数据库
bool CDbOperate::CloseSql()
{
	try
	{
		m_pSqlCon->Close(); ///如果已经打开了连接则关闭它
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		return false;
	}
}

bool CDbOperate::OpenAccess()
{
	return true;
}

bool CDbOperate::CloseAccess()
{
	return true;
}

//插入数据
bool CDbOperate::InsertSQLPayRecord(int deviceID, int EmpIDNUM)
{
	OutputDebug("myTEST  InsertSQLPayRecord()  start..");

	try
	{
		CString strTemp;
		CString strSQL;
		COleDateTime dt = COleDateTime::GetCurrentTime();

		//	strSQL.Format("INSERT INTO stuTable(birth) VALUES('%s')", dt.Format("%Y-%m-%d %H:%M:%S"));
		//自己修改字段名即可
		//	strTemp.Format("insert into dbo.Dev_NetState(SetID,EmpID) values(%d,%d)", deviceID, EmpIDNUM);
		strTemp.Format("insert into dbo.Dev_NetState(SetID,EmpID,ConnectDate) values(%d,%d,'%s')", deviceID, EmpIDNUM, (dt.Format("%Y-%m-%d %H:%M:%S")));
		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  InsertSQLPayRecord()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
	}
	OutputDebug("myTEST  InsertSQLPayRecord()   end..");
	return false;
}
//插入数据
bool CDbOperate::nnInsertSQLPayRecord(int deviceID, int EmpIDNUM, unsigned char mt)
{
	OutputDebug("myTEST  InsertSQLPayRecord()  start..");

	try
	{
		CString strTemp;
		CString strSQL;
		COleDateTime dt = COleDateTime::GetCurrentTime();

		//	strSQL.Format("INSERT INTO stuTable(birth) VALUES('%s')", dt.Format("%Y-%m-%d %H:%M:%S"));
		//自己修改字段名即可
		//	strTemp.Format("insert into dbo.Dev_NetState(SetID,EmpID) values(%d,%d)", deviceID, EmpIDNUM);
		if (mt == 0)
		{
			strTemp.Format("insert into dbo.Dev_NetState(SetID,EmpID,OpType,ConnectDate) values(%d,%d,%s,'%s')", deviceID, EmpIDNUM, NULL, (dt.Format("%Y-%m-%d %H:%M:%S")));

		}
		else
		{
			strTemp.Format("insert into dbo.Dev_NetState(SetID,EmpID,OpType,ConnectDate) values(%d,%d,%d,'%s')", deviceID, EmpIDNUM, mt, (dt.Format("%Y-%m-%d %H:%M:%S")));
		}
		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  InsertSQLPayRecord()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
	}
	OutputDebug("myTEST  InsertSQLPayRecord()   end..");
	return false;
}
//插入数据
bool CDbOperate::InsertSQLPayRecordCCC(int deviceID, int EmpIDNUM)
{
	try
	{
		CString strTemp;
		CString strSQL;
		COleDateTime dt = COleDateTime::GetCurrentTime();

		//	strSQL.Format("INSERT INTO stuTable(birth) VALUES('%s')", dt.Format("%Y-%m-%d %H:%M:%S"));
		//自己修改字段名即可
		//	strTemp.Format("insert into dbo.Dev_NetState(SetID,EmpID) values(%d,%d)", deviceID, EmpIDNUM);
		strTemp.Format("insert into dbo.Dev_UseState(SetID,status) values(%d,%d)", deviceID, EmpIDNUM);
		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		//	OutputDebug("myTEST  InsertSQLPayRecord()   end..");
		return true;
	}
	catch (_com_error   e)
	{
	}
	//	OutputDebug("myTEST  InsertSQLPayRecord()   end..");
	return false;
}
bool CDbOperate::InsertSQLPayRecord2(int deviceID)
{
	OutputDebug("myTEST  InsertSQLPayRecord2()  start..");

	try
	{
		CString strTemp;
		CString strSQL;
		COleDateTime dt = COleDateTime::GetCurrentTime();

		//	strSQL.Format("INSERT INTO stuTable(birth) VALUES('%s')", dt.Format("%Y-%m-%d %H:%M:%S"));
		//自己修改字段名即可
		//	strTemp.Format("insert into dbo.Dev_NetState(SetID,EmpID) values(%d,%d)", deviceID, EmpIDNUM);
		strTemp.Format("insert into Dbo.WK_SetState(SetID) values(%d)", deviceID);
		//	strTemp.Format("Update Dbo.WK_SetState set state_1=%d,state_2=%d,state_3=%d,state_4=%d,state_5=%d,state_6=%d,BuildDate='%s' where SetID=%d", mdata[0], mdata[1], mdata[2], mdata[3], mdata[4], mdata[5], dt.Format("%Y-%m-%d %H:%M:%S"), DevID);//注意字符串就用'%s',数字用%d

		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  InsertSQLPayRecord2()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		//	errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		//	MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	OutputDebug("myTEST  InsertSQLPayRecord2()   end..");
	return false;
}

#ifdef DaoShenFlowmeter
//插入数据
bool CDbOperate::InsertSQLPayRecord_CANSHU(int deviceID, unsigned long tep1, unsigned long tep2, unsigned long tep3, unsigned long tep4, unsigned long tep5, unsigned long tep6)
{


	try
	{
		CString strTemp;
		CString strSQL;
		COleDateTime dt = COleDateTime::GetCurrentTime();

		//	strSQL.Format("INSERT INTO stuTable(birth) VALUES('%s')", dt.Format("%Y-%m-%d %H:%M:%S"));
		//自己修改字段名即可
		//	strTemp.Format("insert into dbo.Dev_NetState(SetID,EmpID) values(%d,%d)", deviceID, EmpIDNUM);
		//strTemp.Format("insert into Dbo.WK_SetState(SetID) values(%d)", deviceID);
		strTemp.Format("Update Dbo.WK_SetState set state_1=%d,state_2=%d,state_3=%d,state_4=%d,state_5=%d,state_6=%d,BuildDate='%s' where SetID=%d", tep1, tep2, tep3, tep4, tep5, tep6, dt.Format("%Y-%m-%d %H:%M:%S"), deviceID);//注意字符串就用'%s',数字用%d

		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  InsertSQLPayRecord2()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		//	errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		//	MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}

	return false;
}

#endif

//更新数据
bool CDbOperate::UpdateSQL(int devID, int EmpIDNUM)

{
	OutputDebug("myTEST  myOUTerr()  start..");

	try
	{
		CString strTemp;
		//自己修改字段名即可
		//strTemp.Format("Update dbo.Dev_NetState set DevIP=%d,EmpID='%s' )", deviceID, id);
		//strTemp.Format("Update dbo.Dev_NetState set ConnectOK=%d,EmpID=%d ", id,deviceID);
		strTemp.Format("Update dbo.Dev_NetState set EmpID=%d where SetID=%d ", EmpIDNUM, devID);//注意字符串就用'%s',数字用%d
		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  UpdateSQL()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		//	errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		//	MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	OutputDebug("myTEST  UpdateSQL()   end..");
	return false;
}

//更新数据
bool CDbOperate::nnUpdateSQL(int devID, int EmpIDNUM, unsigned char mt)

{
	//OutputDebug("myTEST  myOUTerr()  start..");

	try
	{
		CString strTemp;
		//自己修改字段名即可
		//strTemp.Format("Update dbo.Dev_NetState set DevIP=%d,EmpID='%s' )", deviceID, id);
		//strTemp.Format("Update dbo.Dev_NetState set ConnectOK=%d,EmpID=%d ", id,deviceID);
		if (mt == 0)
		{
			strTemp.Format("Update dbo.Dev_NetState set EmpID=%d, OpType=%s where SetID=%d ", EmpIDNUM, NULL, devID);//注意字符串就用'%s',数字用%d

		}
		else
		{
			strTemp.Format("Update dbo.Dev_NetState set EmpID=%d, OpType=%d where SetID=%d ", EmpIDNUM, mt, devID);//注意字符串就用'%s',数字用%d
		}
		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  UpdateSQL()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		//	errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		//	MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	//OutputDebug("myTEST  UpdateSQL()   end..");
	return false;
}
//更新数据
bool CDbOperate::UpdateSQLCCC(int devID, int EmpIDNUM)

{
	//OutputDebug("myTEST  myOUTerr()  start..");

	try
	{
		CString strTemp;
		//自己修改字段名即可
		//strTemp.Format("Update dbo.Dev_NetState set DevIP=%d,EmpID='%s' )", deviceID, id);
		//strTemp.Format("Update dbo.Dev_UseState set setid=%d,EmpID=%d ", id,deviceID);
		strTemp.Format("Update dbo.Dev_UseState set status=%d where SetID=%d ", EmpIDNUM, devID);//注意字符串就用'%s',数字用%d
		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  UpdateSQL()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		//	errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		//	MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	//	OutputDebug("myTEST  UpdateSQL()   end..");
	return false;
}
//临时先假定设备都上线，把链接状态都改为上线状态
bool CDbOperate::myUpdateSQL(int ConOK)
{
	OutputDebug("myTEST  myUpdateSQL()  start..");

	try
	{
		CString strTemp;
		//自己修改字段名即可
		//strTemp.Format("Update dbo.Dev_NetState set DevIP=%d,EmpID='%s' )", deviceID, id);
		//strTemp.Format("Update dbo.Dev_NetState set ConnectOK=%d,EmpID=%d ", id,deviceID);
		strTemp.Format("Update dbo.Dev_NetState set ConnectOK=%d ", ConOK);//注意字符串就用'%s',数字用%d
		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  myUpdateSQL()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		//	errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		//	MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	OutputDebug("myTEST  myUpdateSQL()   end..");
	return false;
}
bool CDbOperate::my2UpdateSQL(int DevID, int ConOK)
{
	OutputDebug("myTEST  my2UpdateSQL()  start..");

	try
	{
		CString strTemp;
		CString strSQL;
		COleDateTime dt = COleDateTime::GetCurrentTime();

		//自己修改字段名即可
		//strTemp.Format("Update dbo.Dev_NetState set DevIP=%d,EmpID='%s' )", deviceID, id);
		//strTemp.Format("Update dbo.Dev_NetState set ConnectOK=%d,EmpID=%d ", id,deviceID);

		strTemp.Format("Update dbo.Dev_NetState set ConnectOK=%d,ConnectDate='%s' where SetID=%d", ConOK, dt.Format("%Y-%m-%d %H:%M:%S"), DevID);//注意字符串就用'%s',数字用%d
		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  my2UpdateSQL()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		//	errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		//	MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	OutputDebug("myTEST  my2UpdateSQL()   end..");
	return false;
}
bool CDbOperate::my3UpdateSQL(int DevID, unsigned int *mdata)
{
	OutputDebug("myTEST  my3UpdateSQL()  start..");

	try
	{
		CString strTemp;
		CString strSQL;
		COleDateTime dt = COleDateTime::GetCurrentTime();

		//自己修改字段名即可
		//strTemp.Format("Update dbo.Dev_NetState set DevIP=%d,EmpID='%s' )", deviceID, id);
		//strTemp.Format("Update dbo.Dev_NetState set ConnectOK=%d,EmpID=%d ", id,deviceID);

		strTemp.Format("Update Dbo.WK_SetState set state_1=%d,state_2=%d,state_3=%d,state_4=%d,state_5=%d,state_6=%d,BuildDate='%s' where SetID=%d", mdata[0], mdata[1], mdata[2], mdata[3], mdata[4], mdata[5], dt.Format("%Y-%m-%d %H:%M:%S"), DevID);//注意字符串就用'%s',数字用%d
		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  my3UpdateSQL()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		//	errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		//	MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	OutputDebug("myTEST  my3UpdateSQL()   end..");
	return false;
}

bool CDbOperate::myUpdateDown_DataSQL(int deviceID, unsigned int*Down_Data, unsigned int*CheckBox) //是可从入函数
{
	unsigned int bigDown_Data[15]; unsigned int bigCheckBox;
	OutputDebug("myTEST  myUpdateDown_DataSQL()  start..");
	bigDown_Data[0] = Down_Data[0]; bigDown_Data[0] = bigDown_Data[0] << 16;
	bigDown_Data[0] |= Down_Data[1];
	bigDown_Data[1] = Down_Data[2]; bigDown_Data[1] = bigDown_Data[1] << 16;
	bigDown_Data[1] |= Down_Data[3];
	bigDown_Data[2] = Down_Data[4]; bigDown_Data[2] = bigDown_Data[2] << 16;
	bigDown_Data[2] |= Down_Data[5];
	bigDown_Data[3] = Down_Data[6]; bigDown_Data[3] = bigDown_Data[3] << 16;
	bigDown_Data[3] |= Down_Data[7];
	bigDown_Data[4] = Down_Data[8]; bigDown_Data[4] = bigDown_Data[4] << 16;
	bigDown_Data[4] |= Down_Data[9];
	bigDown_Data[5] = Down_Data[10]; bigDown_Data[5] = bigDown_Data[5] << 16;
	bigDown_Data[5] |= Down_Data[11];
	bigDown_Data[6] = Down_Data[12]; bigDown_Data[6] = bigDown_Data[6] << 16;
	bigDown_Data[6] |= Down_Data[13];
	bigDown_Data[7] = Down_Data[14]; bigDown_Data[7] = bigDown_Data[7] << 16;
	bigDown_Data[7] |= Down_Data[15];
	bigDown_Data[8] = Down_Data[16]; bigDown_Data[8] = bigDown_Data[8] << 16;
	bigDown_Data[8] |= Down_Data[17];
	bigDown_Data[9] = Down_Data[18]; bigDown_Data[9] = bigDown_Data[9] << 16;
	bigDown_Data[9] |= Down_Data[19];
	bigDown_Data[10] = Down_Data[20]; bigDown_Data[10] = bigDown_Data[10] << 16;
	bigDown_Data[10] |= Down_Data[21];
	bigDown_Data[11] = Down_Data[22]; bigDown_Data[11] = bigDown_Data[11] << 16;
	bigDown_Data[11] |= Down_Data[23];
	bigDown_Data[12] = Down_Data[24]; bigDown_Data[12] = bigDown_Data[12] << 16;
	bigDown_Data[12] |= Down_Data[25];
	bigDown_Data[13] = Down_Data[26]; bigDown_Data[13] = bigDown_Data[13] << 16;
	bigDown_Data[13] |= Down_Data[27];
	bigDown_Data[14] = Down_Data[28]; bigDown_Data[14] = bigDown_Data[14] << 16;
	bigDown_Data[14] |= Down_Data[29];

	bigCheckBox = CheckBox[0]; bigCheckBox = bigCheckBox << 16;
	bigCheckBox |= CheckBox[1];

	try
	{
		CString strTemp;
		CString strSQL;
		COleDateTime dt = COleDateTime::GetCurrentTime();
		//自己修改字段名即可


		//	strTemp.Format("Update Dbo.WK_SetDownData set Down_Data1=%d,Down_Data2=%d,Down_Data3=%d,Down_Data4=%d,Down_Data5=%d,Down_Data6=%d, Down_Data7=%d, Down_Data8=%d, Down_Data9=%d, Down_Data10=%d, Down_Data11=%d, Down_Data12=%d, Down_Data13=%d, Down_Data14=%d, Down_Data15=%d where SetID=%d ", bigDown_Data[0], bigDown_Data[1], bigDown_Data[2], bigDown_Data[3], bigDown_Data[4], bigDown_Data[5], bigDown_Data[6], bigDown_Data[7], bigDown_Data[8], bigDown_Data[9], bigDown_Data[10], bigDown_Data[11], bigDown_Data[12], bigDown_Data[13], bigDown_Data[14],  deviceID);//注意字符串就用'%s',数字用%d
		strTemp.Format("Update Dbo.WK_SetDownData set Down_Data1=%d,Down_Data2=%d,Down_Data3=%d,Down_Data4=%d,Down_Data5=%d,Down_Data6=%d, Down_Data7=%d, Down_Data8=%d, Down_Data9=%d, Down_Data10=%d, Down_Data11=%d, Down_Data12=%d, Down_Data13=%d, Down_Data14=%d, Down_Data15=%d,Down_CheckBox=%d where SetID=%d ", bigDown_Data[0], bigDown_Data[1], bigDown_Data[2], bigDown_Data[3], bigDown_Data[4], bigDown_Data[5], bigDown_Data[6], bigDown_Data[7], bigDown_Data[8], bigDown_Data[9], bigDown_Data[10], bigDown_Data[11], bigDown_Data[12], bigDown_Data[13], bigDown_Data[14], bigCheckBox, deviceID);//注意字符串就用'%s',数字用%d

		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  myUpdateDown_DataSQL()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		//	MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	OutputDebug("myTEST  myUpdateDown_DataSQL()   end..");
	return false;

}
//查询示例
void CDbOperate::QuerySQL()
{
	//_RecordsetPtr  pRecordset;
	OutputDebug("myTEST  QuerySQL()  start..");

	//pRecordset.CreateInstance(__uuidof(Recordset));
	CString strData;
	strData.Format("select SetID,EmpID from dbo.Dev_NetState");
	_bstr_t sql = strData.GetBuffer(0);
	CRecordset  pRecordset(&m_db);

	try
	{
		pRecordset.Open(CRecordset::dynamic, sql);
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		//	errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		//	MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}

	if (pRecordset.IsBOF())
	{
		return;
	}
	else {
		pRecordset.MoveFirst();
	}

	while (!pRecordset.IsEOF())
	{
		//循环读取每条记录
		CString  TEMP;
		pRecordset.GetFieldValue("EmpID", TEMP);
		DWORD   dtu_id = atoi(TEMP);  //atoi()字符串转数据类型

		pRecordset.GetFieldValue("SetID", TEMP);
		DWORD   id = atoi(TEMP);  //atoi()字符串转数据类型

		pRecordset.MoveNext();
	}
	pRecordset.Close();
	OutputDebug("myTEST  QuerySQL()   end..");
}
//查询示例---强烈注意里面有全局变量MyEmpID，本函数是非可重入函数。
unsigned char CDbOperate::MyQuerySQL(int deviceID)//强烈注意里面有全局变量MyEmpID，本函数是非可重入函数。
{
	unsigned long ii; _variant_t var;
	OutputDebug("myTEST  1CDbOperate:: MyQuerySQL()  start..");

	ii = 0;
	CRecordset  pRecordset(&m_db);
	try
	{
		CString strData;
		strData.Format("select *  from dbo.Dev_NetState where SetID = %d", deviceID);
		_bstr_t sql = strData.GetBuffer(0);
		pRecordset.Open(CRecordset::dynamic, sql);
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	try
	{
		if (!(pRecordset.IsBOF()))
		{
			pRecordset.MoveFirst();
		}
		else
		{ // AfxMessageBox("表内数据为空");
			goto sttt2;
		}
		while (!(pRecordset.IsEOF()))
		{
			//循环读取每条记录
			//	DWORD   dtu_id = pRecordset->GetCollect("EmpID");

			CString   TEMP;
			//pRecordset.GetFieldValue("SetID", TEMP);
			//	DWORD   id = atoi(TEMP);

			//CString   TEMP;
			pRecordset.GetFieldValue("EmpID", TEMP);
			MyEmpID = atoi(TEMP);
			ii++;
			pRecordset.MoveNext();
		}
		//	pRecordset->Close();
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	OutputDebug("myTEST  2CDbOperate:: 2MyQuerySQL()   end..");
sttt2:
	if (ii > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
#ifdef updataOnOffDTU
//查询示例---强烈注意里面有全局变量
int CDbOperate::MyQuerySQLconnectTime(void)//
{
	unsigned long ii; _variant_t var; CString   olgTime;
	CString   TEMP;
	OutputDebug("myTEST  1CDbOperate:: MyQuerySQL()  start..");
	olgTime = "";

	ii = 0;


	CRecordset  pRecordset(&m_db);
	try
	{

		CString strData;
		strData.Format("select *  from dbo.Dev_NetState ");
		_bstr_t sql = strData.GetBuffer(0);			
		pRecordset.Open(CRecordset::dynamic, sql);
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	try
	{
		if (!(pRecordset.IsBOF()))
		{
			pRecordset.MoveFirst();
		}
		else
		{ // AfxMessageBox("表内数据为空");
			goto sttt2;
		}
		while (!(pRecordset.IsEOF()))
		{
			//循环读取每条记录
			//	DWORD   dtu_id = pRecordset->GetCollect("EmpID");

			pRecordset.GetFieldValue("ID", TEMP);
			pRecordset.GetFieldValue("ConnectDate", olgTime);

			DWORD   devNo = atoi(TEMP);

			//CString   TEMP;
			//pRecordset.GetFieldValue("EmpID", TEMP);
			//MyEmpID = atoi(TEMP);

			CDbOperate::devTimeMap[devNo] = olgTime;
			//CDbOperate::devTimeMap.insert((devNo, olgTime));

			ii++;
			pRecordset.MoveNext();
		}
		//	pRecordset->Close();
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	//OutputDebug("myTEST  2CDbOperate:: 2MyQuerySQL()   end..");
sttt2:

	if (ii > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}



}

bool CDbOperate::UpdateSQLconnectTime(long id, int ConOK) //更新设备的联网状态
{
	OutputDebug("myTEST  my2UpdateSQL()  start..");

	try
	{
		CString strTemp;
		CString strSQL;
		COleDateTime dt = COleDateTime::GetCurrentTime();

		//自己修改字段名即可
		//strTemp.Format("Update dbo.Dev_NetState set DevIP=%d,EmpID='%s' )", deviceID, id);
		//strTemp.Format("Update dbo.Dev_NetState set ConnectOK=%d,EmpID=%d ", id,deviceID);

		strTemp.Format("Update dbo.Dev_NetState set ConnectOK=%d where ID=%d", ConOK, id);//注意字符串就用'%s',数字用%d
		_bstr_t sql = strTemp.GetBuffer(0);
		m_pSqlCon->ExecuteSQL(sql);
		OutputDebug("myTEST  my2UpdateSQL()   end..");
		return true;
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		//	errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		//	MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	OutputDebug("myTEST  my2UpdateSQL()   end..");
	return false;
}
#endif
//查询示例---强烈注意里面有全局变量MyEmpID，本函数是可重入函数。
unsigned char CDbOperate::nnMyQuerySQL(int deviceID, int * PnnMyEmpID, unsigned char *OpType)//强烈注意里面有全局变量MyEmpID，本函数是可重入函数。
{
	unsigned long ii; _variant_t var;
	OutputDebug("myTEST  1CDbOperate:: MyQuerySQL()  start..");

	ii = 0;


	CRecordset  pRecordset(&m_db);
	try
	{

		CString strData;
		strData.Format("select *  from dbo.Dev_NetState where SetID = %d", deviceID);

		_bstr_t sql = strData.GetBuffer(0);
		pRecordset.Open(CRecordset::dynamic, sql);
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	try
	{
		if (!(pRecordset.IsBOF()))
		{
			pRecordset.MoveFirst();
		}
		else
		{ // AfxMessageBox("表内数据为空");
			goto sttt2;
		}
		while (!(pRecordset.IsEOF()))
		{
			//循环读取每条记录
			//	DWORD   dtu_id = pRecordset->GetCollect("EmpID");

			CString   TEMP;
			//pRecordset.GetFieldValue("SetID", TEMP);
			//	DWORD   id = atoi(TEMP);

			//CString   TEMP;
			pRecordset.GetFieldValue("EmpID", TEMP);
			*PnnMyEmpID = atoi(TEMP);
			pRecordset.GetFieldValue("OpType", TEMP);
			*OpType = atoi(TEMP);

			ii++;
			pRecordset.MoveNext();
		}
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	OutputDebug("myTEST  2CDbOperate:: 2MyQuerySQL()   end..");
sttt2:
	if (ii > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
//---查询一下设备是否有人再使用，也就是流水EmpID是否不为0，为0就是没人使用。
unsigned char CDbOperate::MyQuerySQL3(int deviceID)//---查询一下设备是否有人再使用，也就是流水EmpID是否不为0，为0就是没人使用。
{
	unsigned long ii; _variant_t var; int jj = 0;
	OutputDebug("myTEST  CDbOperate:: MyQuerySQL3()  start..");

	ii = 0; jj = 0;
	CRecordset  pRecordset(&m_db);
	try
	{

		CString strData;
		strData.Format("select *  from dbo.Dev_NetState where SetID = %d", deviceID);
		_bstr_t sql = strData.GetBuffer(0);
		pRecordset.Open(CRecordset::dynamic, sql);
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	try
	{
		if (!(pRecordset.IsBOF()))
		{
			pRecordset.MoveFirst();
		}
		else
		{ // AfxMessageBox("表内数据为空");
			goto sttt2;
		}
		while (!(pRecordset.IsEOF()))
		{
			//循环读取每条记录
			CString  TEMP;
			pRecordset.GetFieldValue("EmpID", TEMP);
			jj = atoi(TEMP);
			ii++;
			pRecordset.MoveNext();
		}
		//	pRecordset->Close();
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	OutputDebug("myTEST  CDbOperate:: MyQuerySQL3()   end..");
sttt2:
	if (jj > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
//查询示例BB
unsigned char CDbOperate::MyQuerySQLBB(int deviceID)
{

	unsigned long ii; _variant_t var;
	OutputDebug("myTEST  3CDbOperate:: MyQuerySQL()  start..");

	ii = 0;
	CRecordset  pRecordset(&m_db);
	try
	{

		CString strData;
		strData.Format("select *  from dbo.Dev_NetState where SetID = %d", deviceID);
		_bstr_t sql = strData.GetBuffer(0);
		pRecordset.Open(CRecordset::dynamic, sql);
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	try
	{
		if (!(pRecordset.IsBOF()))
		{
			pRecordset.MoveFirst();
		}
		else
		{ // AfxMessageBox("表内数据为空");
			goto sttt2;
		}
		while (!(pRecordset.IsEOF()))
		{
			//循环读取每条记录

			ii++;
			pRecordset.MoveNext();
		}
		//	pRecordset->Close();
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	OutputDebug("myTEST  4CDbOperate:: MyQuerySQL()   end..");
sttt2:
	if (ii > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
unsigned char CDbOperate::MyQuerySQLCC(int deviceID)
{
	unsigned long ii; _variant_t var;
	OutputDebug("myTEST  5CDbOperate:: MyQuerySQL()  start..");

	ii = 0;
	CRecordset  pRecordset(&m_db);
	try
	{

		CString strData;
		strData.Format("select *  from dbo.Dev_UseState where SetID = %d", deviceID);
		_bstr_t sql = strData.GetBuffer(0);
		pRecordset.Open(CRecordset::dynamic, sql);
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	try
	{
		if (!(pRecordset.IsBOF()))
		{
			pRecordset.MoveFirst();
		}
		else
		{ // AfxMessageBox("表内数据为空");
			goto sttt2;
		}
		while (!(pRecordset.IsEOF()))
		{
			//循环读取每条记录

			ii++;
			pRecordset.MoveNext();
		}
		//	pRecordset->Close();
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	OutputDebug("myTEST  6CDbOperate:: MyQuerySQL()   end..");
sttt2:
	if (ii > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
unsigned char CDbOperate::MyQuerySQL2(int deviceID)
{

	unsigned long ii; _variant_t var;
	OutputDebug("myTEST  MyQuerySQL2()  start..");

	ii = 0;
	CRecordset  pRecordset(&m_db);
	try
	{
		CString strData;
		strData.Format("select *  from Dbo.WK_SetState where SetID = %d", deviceID);//注意字符串就用'%s',数字用%d
		_bstr_t sql = strData.GetBuffer(0);	
		pRecordset.Open(CRecordset::dynamic, sql);
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	try
	{
		if (!(pRecordset.IsBOF()))
		{
			pRecordset.MoveFirst();
		}
		else
		{ // AfxMessageBox("表内数据为空");
			goto sttt2;
		}
		while (!(pRecordset.IsEOF()))
		{
			//循环读取每条记录
			ii++;
			pRecordset.MoveNext();
		}
		//	pRecordset->Close();
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	OutputDebug("myTEST  MyQuerySQL2()   end..");
sttt2:
	if (ii > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

//查询设置的参数是哪些，-本函数是可重入函数。
unsigned char CDbOperate::MyQueryDown_DataSQL(int deviceID, unsigned int*Down_Data, unsigned int*CheckBox)
{
	unsigned long ii; _variant_t var; unsigned long tempDD;
	OutputDebug("myTEST  CDbOperate:: MyQueryDown_DataSQL()  start..");

	ii = 0;
	CRecordset  pRecordset(&m_db);
	try
	{

		CString strData;
		strData.Format("select *  from dbo.WK_SetDownData where SetID = %d", deviceID);
		_bstr_t sql = strData.GetBuffer(0);
		pRecordset.Open(CRecordset::dynamic, sql);
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		//下面的错误提示，在调试时可以打开，观察错误点
		errorMsg.Format("MyQuerySQL数据库!\r\n错误信息:%s", e.ErrorMessage());
		MessageBox(NULL, errorMsg, "错误", MB_OK);
		//	return false;
	}
	try
	{
		if (!(pRecordset.IsBOF()))
		{
			pRecordset.MoveFirst();
		}
		else
		{ // AfxMessageBox("表内数据为空");
			goto sttt2;
		}
		while (!(pRecordset.IsEOF()))
		{
			//循环读取每条记录

			CString  TEMP;
			pRecordset.GetFieldValue("Down_Data1", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data2", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data3", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data4", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data5", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data6", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data7", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data8", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data9", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data10", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data11", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data12", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data13", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data14", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_Data15", TEMP);
#ifdef downLoadPam
			TEMP.Replace(",", "");
#endif
			tempDD = atoi(TEMP);
			Down_Data[ii] = tempDD >> 16;
			ii++;
			Down_Data[ii] = tempDD & 0xFFFF;
			ii++;
			pRecordset.GetFieldValue("Down_CheckBox", TEMP);

			tempDD = atoi(TEMP);
			CheckBox[0] = tempDD >> 16;
			CheckBox[1] = tempDD & 0xFFFF;

			pRecordset.MoveNext();
		}
		//	pRecordset->Close();
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	OutputDebug("myTEST  CDbOperate:: MyQueryDown_DataSQL()   end..");
sttt2:
	if (ii > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

bool CDbOperate::InsertAccessState(int dtu_id, bool online)
{
	return true;
}

//更新数据
bool CDbOperate::UpdateAccess(int deviceID, CString str)
{
	//和SQL的一样
	return false;
}
//查询示例
void CDbOperate::QueryAccess()
{
	//和SQL的一样
}