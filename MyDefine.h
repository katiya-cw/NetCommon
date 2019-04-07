
#ifndef MyDefine_h
#define MyDefine_h
#define ConfirmJS   //增加结算确认逻辑

#define SENDforOK
#define SENDTIMES_LENS 999999  //定义1000个设备，暂时这样做
#define SENDTIMES 5    //如果无法上线，针对非会员就多发几次保证付款成功

//-------------------------放在"115.28.78.161"上面必须关闭的2个宏定义---START--------------------------------------
#define GaiDongIPaddress //变化IP地址,定义后使用127.0.0.1即本地IP来做传递和接收
#define WarningFlg //低水位报警方式，定义为常开，不定义为常闭  204.188的不定义，非洗车的要定义
//-------------------------放在"115.28.78.161"上面必须关闭的2个宏定义---END--------------------------------------


#define SetPAM  //设置每个设备参数
//#define LinShi1 //会员金额小于1元时，不做处理，后来硬件里面加了余额提醒后，就不再需要
//#define  myDebugTXT   //定义用自己的写MYLOG.TXT，这个一般不需要，增加MYLOG.TXT占用的空间

#define NewFaction //ID号从3000到6999之间的，按分拆来读写，比如3335，下传是拆分为333+表号5，上传时拼接就可以



#define PreventMemoryOverflow  //防止内存溢出 

#define   DaoShenFlowmeter //使用深圳一克拉家电有限公司的TDS传感器 多功能水质检测模块

#define ADD_More_CH  //多通道，充电站等 包括10路状态上传写入数据库， 充电是否正常完成报警等 含有数据库断开就自动崩溃的逻辑


#define  downLoadPam //下载参数，读出数据库里面的对应INT类型值带",",用REPLACE（）函数去掉才能下载

////////////----------农业水权云端计算扣费相关------------START--------------------------------
//#define ShuiQuanZuanYong //农业水权云端计算扣费版本
//#define forSQPT_where //对接，数据转发地址
////////////----------农业水权云端计算扣费相关------------END--------------------------------


#define outdebugGetText //加上GET返回信息的输出，方便调试

#define JavaResolvesConflicts //解决JAVA和PHP共用时，结算命令清空EMID时互相冲突的问题 ！！！！同样的这个宏不同的使用PHP和JAVA: 强烈注意里面函数参数设置的不一样


#define UseFloatCanSHU //允许使用小数直接单次付费，售货机可能用---只改了会员付款部分，


#define httpRevUsePort 8090 //确定接收PHP发来的HTTP命令使用的是什么端口  以前默认的都是8090

//修改打开关闭数据库的位置：只在接收数据的线程UINT ______AFX_CHECK_METER(LPVOID pParam)里面打开和关闭即可


#define updataOnOffDTU //增加对设备在线设备状态的在线更新 注意用含有STD:THREAD 启用的线程的函数不允许后面退出函数，否则出现崩溃
                      //应该大于8分钟比较安全  但是在线的时候设备表现为掉线，不发07的命令，这点特别注意


//修改下面的 保持和前端通信一致 2018-9-27
/*
//	if(send(g_GetData.sSocket,strS,1,0) <= 0)
if (send(g_GetData.sSocket, strS, 4, 0) <= 0) //为了配上服务端按4个字节判断类型，这里故意发4个字节时间更新命令
//有时候用远程调试或者用WINDBG调试出现中断，解决方法：

这个地方重复 close 了，所以才会导致一直 WINDBG 中断
LogicCenter.cpp 里面处理完后有 CloseHandle

只去掉 /test/DLL.h里面的		if (hEvent)
{
CloseHandle(hEvent);
}
*/
/*查询SQL并发数
select value from  master.dbo.sysconfigures where [config]=103
*/



///////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////
extern int pascal NetSendMyAccount(int DtuID, unsigned char nMeter_ID, int OKorNO);//对自助设备发送结算确认命令
extern  int pascal MENGsendDown_Data(int DtuID, unsigned char nMeter_ID, unsigned int* Down_Data, unsigned char ttm);
//extern unsigned int CheckBox[2];


















#endif
