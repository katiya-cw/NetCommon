#pragma once
#include "stdafx.h"
#include <afxdb.h>

#include<map>
#include<string>

struct USER_INFO
{
	//�û���
	CString user_name;
	//�û�����
	CString user_pass;
};
//���ݿ����
/*
provider=SQLOLEDB.1����ʹ�õ���SQL Server���ݿ⡣
Server=HUOWEI\SQL2005�����ݿ�ķ�������ΪHUOWEI\SQL2005��
Uid=sa;PWD=5706512�����SQL Server���ݿ���ʹ�õĵ�¼��ʽ��SQL Server�����֤ģʽ�û�����sa ;����Ϊ��5706512��
Database=book������ʹ�õ����ݿ���book���ݿ�

*/
class CDbOperate
{
public:
	CDbOperate();
	~CDbOperate();
	
	//��SQL���ݿ�
	bool OpenSql();
	//�ر�SQL���ݿ�
	bool CloseSql();
	//��������
	bool InsertSQLPayRecord(int deviceID, int EmpIDNUM);
	bool nnInsertSQLPayRecord(int deviceID, int EmpIDNUM, unsigned char mt);
	bool InsertSQLPayRecordCCC(int deviceID, int EmpIDNUM);

	bool InsertSQLPayRecord_CANSHU(int deviceID, unsigned long tep1, unsigned long tep2, unsigned long tep3, unsigned long tep4, unsigned long tep5, unsigned long tep6);
	
	//��������
	bool UpdateSQL(int devID, int EmpIDNUM);
	bool nnUpdateSQL(int devID, int EmpIDNUM, unsigned char mt);
	bool myUpdateSQL(int ConOK);
	bool myUpdateDown_DataSQL(int deviceID, unsigned int*Down_Data, unsigned int*CheckBox);
	bool UpdateSQLCCC(int devID, int EmpIDNUM);
	bool UpdateSQLconnectTime(long id, int ConOK);  //�����豸������״̬
	//��ѯʾ��
	void QuerySQL();
	//��ѯ��û�����Ӳ��
	unsigned char MyQuerySQL(int deviceID);
	unsigned char nnMyQuerySQL(int deviceID, int * PnnMyEmpID, unsigned char *OpType);
	unsigned char MyQuerySQLBB(int deviceID);
	unsigned char MyQuerySQL3(int deviceID);//��ѯʾ��---��ѯһ���豸�Ƿ�������ʹ�ã�Ҳ������ˮEmpID�Ƿ�Ϊ0��Ϊ0����û��ʹ�á�
	int MyQuerySQLconnectTime(void);//
	bool my2UpdateSQL(int DevID, int ConOK);
	unsigned char MyQueryDown_DataSQL(int deviceID, unsigned int*Down_Data, unsigned int*CheckBox);
	unsigned char MyQuerySQLCC(int deviceID);
	//Access���ݿ����
	bool OpenAccess();
	bool CloseAccess();
	bool InsertAccessState(int dtu_id,bool online);
	//��������
	bool UpdateAccess(int deviceID,CString str);
	bool my3UpdateSQL(int DevID, unsigned int *mdata);
	bool InsertSQLPayRecord2(int deviceID);
	unsigned char MyQuerySQL2(int deviceID);
	//��ѯʾ��
	void QueryAccess();
	
	typedef std::map <DWORD, CString>   devTimeMap2;

	devTimeMap2 devTimeMap;
	

private: 
	//SQL��ַ
	CString m_sqlAddr;
	//Access·��
	CString m_accAddr;
	//�û���Ϣ
	USER_INFO m_sqlUser;
	USER_INFO m_accUser;
	//ADO������Ϣ
//	_ConnectionPtr m_pSqlCon;
	_ConnectionPtr m_pAccCon;
	//ODBC����
	CDatabase m_db;
	CDatabase* m_pSqlCon;
};

