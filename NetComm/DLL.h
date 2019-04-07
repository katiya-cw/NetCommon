#if !defined(________H_________)
#define ________H_________

//下载过程回调函数
typedef bool (*CALL_BACK_DOWNLOAD)(int nType,char* pText,int nTextLen);
/*
nType 类型 0 下载完成  1  发送错误  2 接收错误 3 版本错误  4 下载错误 5 正在下载 9 超时
pText 描述
nTextLen 描述的长度
*/
/*
//表的通信结构
struct _DATA_METER
{	
	int nDTU_ID;//DTU编号
	int nID;//ID 设备编号
	
	int nType1;//类型 == 1

	int nUedE;//已用电量 度 2位小数
	BYTE nUedE_E;//已用电量 报警： 1报警 0不报警
	
	int nLeftE;//剩余电量 度 2位小数
	BYTE nLeftE_E;//剩余电量 报警： 1报警 0不报警
	
	int nUsedM;//已用钱 元 2位小数
	BYTE nUsedM_E;//已用钱 报警： 1报警 0不报警
	
	int nLeftM;//剩余钱 元
	BYTE nLeftM_E;//剩余钱 报警： 1报警 0不报警
	
	int nRate;//单价 元/度 2位小数
	BYTE nRate_E;//单价 报警： 1报警 0不报警
	
	int nFlow;//流量 立方米 2位小数
	BYTE nFlow_E;//流量 报警： 1报警 0不报警
	
	int nFlow2;//瞬间流量 立方米/小时 2位小数
	BYTE nFlow2_E;//瞬间流量  报警： 1报警 0不报警
	
	int nUID;//当前用户ID
	BYTE nUID_E;//当前用户ID 报警： 1报警 0不报警
	
	long lTime;//最后上传的时间 从19
};

struct _DATA_METER_2//2.0版本 3个字节是数 没有报警
{	
	int nDTU_ID;//DTU编号
	int nID;//ID 设备编号
	
	int nType2;//类型 == 2

	int nUedE;//已用电量 度 2位小数
	BYTE nUedE_E;// == 0
	
	int nLeftE;//剩余电量 度 2位小数
	BYTE nLeftE_E;// == 0
	
	int nUsedM;//已用钱 元 2位小数
	BYTE nUsedM_E;// == 0
	
	int nLeftM;//剩余钱 元 2位小数
	BYTE nLeftM_E;// == 0
	
	int nRate;//单价 元/度 2位小数
	BYTE nRate_E;// == 0
	
	int nFlowTotal;//累积流量 立方米
	BYTE nFlowTotal_E;// == 0
	
	int nFlow2;//瞬间流量 立方米/小时 2位小数
	BYTE nFlow2_E;// == 0
	
	int nUID;//当前用户ID
	BYTE nUID_E;// == 0
	
	long lTime;//最后上传的时间 从19
};

struct _DATA_RTU
{	
	int nDTU_ID;//DTU编号

	int nID;//ID 设备编号

	int nType3;//类型 == 3
	
	int nFlow;//流量 立方米 2位小数
	BYTE nFlow_E;// == 0
	
	int nWater;//水位
	BYTE nWater_E;// == 0
	
	int nTempr;//温度  2位小数
	BYTE nTempr_E;//==0
	
	int nHumidity;//湿度
	BYTE nHumidity_E;//==0
	
	int nOther1;//其他1
	BYTE nOther1_E;//==0
	
	int nOther2;//其他2
	BYTE nOther2_E;//==0
	
	int nOther3;//其他3
	BYTE nOther3_E;//==0
	
	int nOther4;//其他4
	BYTE nOther4_E;//==0
	
	long lTime;//最后上传的时间 从19
};
*/
//实时通信回调函数,实时回调通信服务器的表的状态数据
//typedef bool (*CALL_BACK_GET_DATA)(_DATA_METER* pData);

//实时通信回调函数,实时回调通信服务器的DTU的状态数据
//typedef bool (*CALL_BACK_GET_DATA_GPRS)(int id,int nStatus,long lastTime);

//实时通信回调函数,实时回调通信服务器的表的状态数据
//typedef bool (*CALL_BACK_GET_DATA2)(_DATA_METER_CHAR* pData);
/*
pData 实时数据
*/

//数据库执行回调函数
typedef bool (*CALL_BACK_EXCUTE)(int nType,char* pText,int nTextLen);
/*
nType 0 操作成功，其他为错误
pText 错误描述
nTextLen 错误描述的长度
*/

//以下是实时表数据的返回与说明
/*
nDut_ID		DTU编号
nMeter_ID	表编号
nType		表协议类型 1 2 3
tCommn		通信的时间
pData		实时数据，二进制
nDataLen	实时数据长度
*/
//=====================协议V1
/*
0x68,表号，数据长度（总包长度），控制码03，通道数量（1-255），各通道的数据，各通道的数据（从第1个开始，每通道三字节，前两个是数据，高位在前，最后一个是报警状态，1报警，0不报警），  CSL，      CSH，16H

读回的数据是：（2位小数点）
68 07 20 03   08     00 0D   00       08 8B     00    00 06     00    04 46   00    00 32     00 00 00 00 00 00 00 00 00 00   46 14    16
                     已用电量         剩余电量         已用钱数       剩余钱数       电价                                     CRC效验
                     0X000D =0.13    0X088B=21.87    0X0006=0.06     0X0446=1094    0X0032=0.50
                      度             度               元               元            元
*/
//======================协议V2
/*
如下收到数据 07是表号
0x68,表号，数据长度（总包长度），控制码03，通道数量（1-255），各通道的数据，各通道的数据（从第1个开始，每通道三字节，都是数据，高位在前，低位在后），  CSL，      CSH，16H

68 07 20 03   08     00 0D  01       00 01 01       00 06  00      00 03 01    00 00 32       00 01 20     00 02 02   00 00 11    46 14    16

                     已用电量         剩余电量         已用钱数       剩余钱数       电价        累积流量    瞬时流量     户号    CRC效验
                     0X000D01 =33.29 0X000101=2.57   0X000600=15.36  0X000301=7.69 0X000032=0.50  0X000120=  0X000202=    17
                    度              度               元               元            元         288米3     5.14米3/小时  户	
*/
//=======================协议V3(RTU)
/*
0x68,表号，数据长度（总包长度），控制码03，通道数量（1-255），各通道的数据，各通道的数据（从第1个开始，每通道三字节，前两个是数据，高位在前，最后一个是报警状态，1报警，0不报警），  CSL，      CSH，16H

读回的数据是：
68 07 20 03   08     00 0D   00       08 8B     00    00 06     00    04 46   00    00 32     00 00 00 00 00 00 00 00 00 00   46 14    16
                     通道1流量         水位             温度            湿度         其他                                     CRC效验
                     0X000D =0.13    0X088B=21.87    0X0006=0.06     0X0446=1094    0X0032=0.50
                    立方米            
*/
typedef bool (*CALL_BACK_GET_DATA)(int nDut_ID,int nMeter_ID,int nType,long tCommn,BYTE* pData,int nDataLen);

typedef bool (*CALL_BACK_GET_DATA_GPRS)(int nDtu_ID,int nStatus,long lastTime);

//通信连接断开回调
typedef bool (*CALL_BACK_SHUTDOWN)();

//下载数据库
int __stdcall DownLoadFile(char* pServerIP,int nPort,char* strSaveFile,CALL_BACK_DOWNLOAD callBack);
/*
返回 
0 连接准备开始下载
1 本地错误
2 绑定错误
3 连接错误

参数
pServerIP 服务器地址
nPort     服务器端口
strSaveFile 需要保存的文件名包含路径
CALL_BACK_DOWNLOAD 下载过程回调
*/

//实时通信接口
//int __stdcall NetComm(char* pServerIP,int nPort,CALL_BACK_GET_DATA callGetBack,CALL_BACK_GET_DATA_GPRS callGetBack2,CALL_BACK_EXCUTE callExcuteBack,CALL_BACK_SHUTDOWN callShutdownBack);
int __stdcall NetComm(char* pServerIP,int nPort,CALL_BACK_GET_DATA callGetBack,CALL_BACK_GET_DATA_GPRS callGetBack2,CALL_BACK_EXCUTE callExcuteBack,CALL_BACK_SHUTDOWN callShutdownBack);
 
/*
返回 
0 连接成功
1 本地错误
2 绑定错误
3 连接错误

 参数
pServerIP, 服务器地址
nPort, 服务器端口

callGetBack, 实时数据回调
callGetBack2, 实时数据回调
callExcuteBack, 执行数据库回调
callShutdownBack 连接断开
*/

//对数据库的操作，标准SQL语句，完成后或失败后会通过回调显示
int __stdcall NetSend(char *pData,int nLen);
/*
返回 返回发送的数据长度 -2未连接

  参数
pData 需要发送的字符串
nLen  字符串长度
*/
//用于上位机发送指定长度的数据给设备终端
int __stdcall NetSendMyData(int DtuID,unsigned char MepID,unsigned char *pData,int nLen);







//用于上位机发送指定钱数到设备终端
int __stdcall NetSendMyDataTrue(int DtuID,unsigned char nMeter_ID, int money,int delayTime  );
/*
返回 发送成功后返回发送成功的字节数
  参数
DtuID  终端设备编号
money  支付多少钱,单位分,比如7609就是76元零9分

*/

//软件注册,返回1表示成功
int pascal regMYdevice(void);

//对数据库的操作，标准SQL语句，完成后或失败后会通过回调显示
int __stdcall GetQueryData(char *pData,int nLen,char* pRetData,int& nRetLen);


int __stdcall myfction(int data1);
#endif
