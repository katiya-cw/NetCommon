
#ifndef MyDefine_h
#define MyDefine_h
#define ConfirmJS   //���ӽ���ȷ���߼�

#define SENDforOK
#define SENDTIMES_LENS 999999  //����1000���豸����ʱ������
#define SENDTIMES 5    //����޷����ߣ���Էǻ�Ա�Ͷ෢���α�֤����ɹ�

//-------------------------����"115.28.78.161"�������رյ�2���궨��---START--------------------------------------
#define GaiDongIPaddress //�仯IP��ַ,�����ʹ��127.0.0.1������IP�������ݺͽ���
#define WarningFlg //��ˮλ������ʽ������Ϊ������������Ϊ����  204.188�Ĳ����壬��ϴ����Ҫ����
//-------------------------����"115.28.78.161"�������رյ�2���궨��---END--------------------------------------


#define SetPAM  //����ÿ���豸����
//#define LinShi1 //��Ա���С��1Ԫʱ��������������Ӳ���������������Ѻ󣬾Ͳ�����Ҫ
//#define  myDebugTXT   //�������Լ���дMYLOG.TXT�����һ�㲻��Ҫ������MYLOG.TXTռ�õĿռ�

#define NewFaction //ID�Ŵ�3000��6999֮��ģ����ֲ�����д������3335���´��ǲ��Ϊ333+���5���ϴ�ʱƴ�ӾͿ���



#define PreventMemoryOverflow  //��ֹ�ڴ���� 

#define   DaoShenFlowmeter //ʹ������һ�����ҵ����޹�˾��TDS������ �๦��ˮ�ʼ��ģ��

#define ADD_More_CH  //��ͨ�������վ�� ����10·״̬�ϴ�д�����ݿ⣬ ����Ƿ�������ɱ����� �������ݿ�Ͽ����Զ��������߼�


#define  downLoadPam //���ز������������ݿ�����Ķ�ӦINT����ֵ��",",��REPLACE��������ȥ����������

////////////----------ũҵˮȨ�ƶ˼���۷����------------START--------------------------------
//#define ShuiQuanZuanYong //ũҵˮȨ�ƶ˼���۷Ѱ汾
//#define forSQPT_where //�Խӣ�����ת����ַ
////////////----------ũҵˮȨ�ƶ˼���۷����------------END--------------------------------


#define outdebugGetText //����GET������Ϣ��������������

#define JavaResolvesConflicts //���JAVA��PHP����ʱ�������������EMIDʱ�����ͻ������ ��������ͬ��������겻ͬ��ʹ��PHP��JAVA: ǿ��ע�����溯���������õĲ�һ��


#define UseFloatCanSHU //����ʹ��С��ֱ�ӵ��θ��ѣ��ۻ���������---ֻ���˻�Ա����֣�


#define httpRevUsePort 8090 //ȷ������PHP������HTTP����ʹ�õ���ʲô�˿�  ��ǰĬ�ϵĶ���8090

//�޸Ĵ򿪹ر����ݿ��λ�ã�ֻ�ڽ������ݵ��߳�UINT ______AFX_CHECK_METER(LPVOID pParam)����򿪺͹رռ���


#define updataOnOffDTU //���Ӷ��豸�����豸״̬�����߸��� ע���ú���STD:THREAD ���õ��̵߳ĺ�������������˳�������������ֱ���
                      //Ӧ�ô���8���ӱȽϰ�ȫ  �������ߵ�ʱ���豸����Ϊ���ߣ�����07���������ر�ע��


//�޸������ ���ֺ�ǰ��ͨ��һ�� 2018-9-27
/*
//	if(send(g_GetData.sSocket,strS,1,0) <= 0)
if (send(g_GetData.sSocket, strS, 4, 0) <= 0) //Ϊ�����Ϸ���˰�4���ֽ��ж����ͣ�������ⷢ4���ֽ�ʱ���������
//��ʱ����Զ�̵��Ի�����WINDBG���Գ����жϣ����������

����ط��ظ� close �ˣ����ԲŻᵼ��һֱ WINDBG �ж�
LogicCenter.cpp ���洦������� CloseHandle

ֻȥ�� /test/DLL.h�����		if (hEvent)
{
CloseHandle(hEvent);
}
*/
/*��ѯSQL������
select value from  master.dbo.sysconfigures where [config]=103
*/



///////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////
extern int pascal NetSendMyAccount(int DtuID, unsigned char nMeter_ID, int OKorNO);//�������豸���ͽ���ȷ������
extern  int pascal MENGsendDown_Data(int DtuID, unsigned char nMeter_ID, unsigned int* Down_Data, unsigned char ttm);
//extern unsigned int CheckBox[2];


















#endif
