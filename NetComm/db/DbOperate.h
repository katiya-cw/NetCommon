#pragma once
#include "stdafx.h"
#include <afxdb.h>

#include<map>
#include<string>

struct USER_INFO
{
	//用户名
	CString user_name;
	//用户密码
	CString user_pass;
};
//数据库操作
/*
provider=SQLOLEDB.1：你使用的是SQL Server数据库。
Server=HUOWEI\SQL2005：数据库的服务器名为HUOWEI\SQL2005。
Uid=sa;PWD=5706512：你的SQL Server数据库所使用的登录方式是SQL Server身份验证模式用户名：sa ;密码为：5706512。
Database=book：你所使用的数据库是book数据库

*/
class CDbOperate
{
public:
	CDbOperate();
	~CDbOperate();
	
	//打开SQL数据库
	bool OpenSql();
	//关闭SQL数据库
	bool CloseSql();
	//插入数据
	bool InsertSQLPayRecord(int deviceID, int EmpIDNUM);
	bool nnInsertSQLPayRecord(int deviceID, int EmpIDNUM, unsigned char mt);
	bool InsertSQLPayRecordCCC(int deviceID, int EmpIDNUM);

	bool InsertSQLPayRecord_CANSHU(int deviceID, unsigned long tep1, unsigned long tep2, unsigned long tep3, unsigned long tep4, unsigned long tep5, unsigned long tep6);
	
	//更新数据
	bool UpdateSQL(int devID, int EmpIDNUM);
	bool nnUpdateSQL(int devID, int EmpIDNUM, unsigned char mt);
	bool myUpdateSQL(int ConOK);
	bool myUpdateDown_DataSQL(int deviceID, unsigned int*Down_Data, unsigned int*CheckBox);
	bool UpdateSQLCCC(int devID, int EmpIDNUM);
	bool UpdateSQLconnectTime(long id, int ConOK);  //更新设备的联网状态
	//查询示例
	void QuerySQL();
	//查询有没有这个硬件
	unsigned char MyQuerySQL(int deviceID);
	unsigned char nnMyQuerySQL(int deviceID, int * PnnMyEmpID, unsigned char *OpType);
	unsigned char MyQuerySQLBB(int deviceID);
	unsigned char MyQuerySQL3(int deviceID);//查询示例---查询一下设备是否有人再使用，也就是流水EmpID是否不为0，为0就是没人使用。
	int MyQuerySQLconnectTime(void);//
	bool my2UpdateSQL(int DevID, int ConOK);
	unsigned char MyQueryDown_DataSQL(int deviceID, unsigned int*Down_Data, unsigned int*CheckBox);
	unsigned char MyQuerySQLCC(int deviceID);
	//Access数据库操作
	bool OpenAccess();
	bool CloseAccess();
	bool InsertAccessState(int dtu_id,bool online);
	//更新数据
	bool UpdateAccess(int deviceID,CString str);
	bool my3UpdateSQL(int DevID, unsigned int *mdata);
	bool InsertSQLPayRecord2(int deviceID);
	unsigned char MyQuerySQL2(int deviceID);
	//查询示例
	void QueryAccess();
	
	typedef std::map <DWORD, CString>   devTimeMap2;

	devTimeMap2 devTimeMap;
	

private: 
	//SQL地址
	CString m_sqlAddr;
	//Access路径
	CString m_accAddr;
	//用户信息
	USER_INFO m_sqlUser;
	USER_INFO m_accUser;
	//ADO连接信息
//	_ConnectionPtr m_pSqlCon;
	_ConnectionPtr m_pAccCon;
	//ODBC连接
	CDatabase m_db;
	CDatabase* m_pSqlCon;
};

