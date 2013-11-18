///基础件通用接口定义
/*******************************************************
  源程序名称:hs_interface.h
  软件著作权:杭州恒生电子股份有限公司
  系统名称:  HZHSGS金融基础件V1.0
  模块名称:  公共模块
  功能说明: 基础件通用接口定义,如:基础服务组件即遵守本接口定义;
  			0、基础件公开的基础服务ID定义
  			1、IKnown接口定义
  			2、IAppContext接口定义
  			2、动态加载基础服务组件的接口(动态库引出函数)定义
   
            
  相关文档:  概要设计接口部分
            
             
  作    者:  zhangly
  开发日期:  20040218
  备    注:  linux/unix平台下，定义 OS_UNIX或PLATFORM_UNIX
  			 在windows下注意函数调用方式：__stdcall
  修改人员：zhangly
  修改日期：20050802
  修改说明：增加对高级数据访问服务ADE的ID定义
  修改人员：zhangly
  修改日期：20070316
  修改说明：在定义IKnown接口时，增加HS_IKNOWN定义判断
*********************************************************/

#ifndef HS_INTERFACE_H
#define HS_INTERFACE_H

#ifndef FUNCTION_CALL_MODE
#ifdef  OS_UNIX 
	#define FUNCTION_CALL_MODE  
#else
	#ifdef PLATFORM_UNIX
		#define FUNCTION_CALL_MODE 
	#else
		#define FUNCTION_CALL_MODE	__stdcall
	#endif	 
#endif
#endif

///PFgetxxxxInfo()返回I_OK表示按索引查到基础服务
#define I_OK 1
///PFgetxxxxInfo()返回I_NONE表示按索引查不到基础服务
#define I_NONE 0

///基础服务唯一标识的类型
#define HS_SID  int
/**@name 对外公开的基础服务调用接口唯一标识 SID 定义 
 * 特别注意：下述常量一但发布，不得修改，否则会使已发布系统失效！！！
 */
//@{
	///定义本服务在AS2系统服务中的服务ID
	#define SID_CONFIG 1
	///定义日志服务的ID为2
	#define SID_LOG 2
	///数据库访问
	#define SID_HSDB 3
	///数据组件服务
	#define SID_DATA_COMPS 4
	///内存数据库查询
	#define SID_MDB 5
	///可扩展管理接口(每个被管理对象都可以有一个)
	#define SID_MANAGED 6
	///打包解包器
	#define SID_PACK  7	
	///内存临时表服务
	#define SID_TEMPMDB 8
	///DBF文件操作服务
	#define SID_DBF 9
	///创建证券行情DBF服务
	#define SID_GENHQDBF 10
	///会话管理器
	#define SID_SESSION     11
	///分页查询管理器
	#define SID_PAGESQUERY  12
	///线程计时器
	#define SID_TIMER  13	
	///主动任务管理器
	#define SID_AS_TASK  14	
	///订阅推送服务
	#define SID_PUSH  15	
	///安全服务
	#define SID_SAFE  16
	///订阅推送服务	
	#define SID_SUBSCIBE	17

	///TFTP服务	
	#define SID_TFTP	19
	
	///AS通信服务
	#define SID_AS_COMM   20
	///AS系统对象
	#define SID_AS_SYSTEM   21	
	///AS用户应用管理器
	#define SID_AS_APP_MGR   22	
	///子服务调用
	#define SID_SUBCALL     23
	///高级数据访问服务ADE
	#define SID_ADE 24
	///单主键内存表服务SMDB
	#define SID_SMDB 25
	///扩展系统服务（跨平台工具等）
	#define SID_SYSTEM_EX 26
			
	///数据日志服务
	#define SID_DATA_CAPTURE 73123
	
	#define SID_AR_API		100
	#define SID_AR_OSPF		101
	#define SID_AR_MAIN		102
	///150~199 为AR插件使用
///hs_deamon 对象号
	#define SID_HSDAEMON    200
//@}

/**@name 对外公开的全局数据唯一标识 SGDID 定义 */
//@{
	///证书库保护密码
	#define SGDID_CERT_LIB_PIN     1	
//@}

#ifndef HS_IKNOWN
#define HS_IKNOWN
///标准接口查询,参照com标准
struct IKnown
{
	///接口查询
	/**
	 *@param const int iid 接口全局唯一标识，0~1024 由基础件使用，1024以上由新接口开发者使用，基础件项目组在内部接口规范中备案以防冲突;
	 *@param void **ppv 返回iid对应的接口指针（接口参照com标准）
	 *@return I_OK 成功，I_NONE 未查到iid 相应接口
	 */
    virtual unsigned long  FUNCTION_CALL_MODE QueryInterface(const HS_SID & iid, IKnown **ppv )=0;
    virtual unsigned long  FUNCTION_CALL_MODE AddRef()=0;
    virtual unsigned long  FUNCTION_CALL_MODE Release()=0;
};
#endif

///应用上下文接口(通过本接口可以获取当前应用的内部对象接口与全局数据)
/**基础件框架提供动态加载管理全局性内部对象接口的机制(如基础服务)
 * 
 *基础件的内部模块或组件一般在初始化时收到应用框架传入的本接口指针；
 *
 *从而通过本接口继承的IKnown接口下的QueryInterface函数，按ID获取各类基础服务接口或内部对象接口句柄；
 *
 *也可以通过本接口，存取应用级全局数据；
 *
 *基础件框架在实现本接口时，动态加载和初始化的基础服务及全局数据等；
 */
struct IAppContext:IKnown
{
	///取本接口版本
	/**本方法必须为本接口的第一个方法,本接口版本不一致会导致不可预期的结果，非兼容修改要求重新编译所有组件；
	 *@return int 本接口版本yyyymmdd
	 */
    virtual int FUNCTION_CALL_MODE getVersion()=0;
    
	///取AR/AS名字
	/**
	 *@return const char * 对于AR/AS返回组名
	 */
    virtual const char * FUNCTION_CALL_MODE getName()=0;	

	///取编号
	/**
	 *@return int 对于AR/AS返回组内编号
	 */
    virtual int  FUNCTION_CALL_MODE getID()=0;	

	///取应用启动时的初始化选项,按选项名取启动选项值
	/**
	 *@param const char * szParamName 参数名（命令行选项名为标准，单字符）
	 *@param const char * szDefaultValue    如果指定参数没有通过命令行或环境变量提供值，则返回缺省值
	 *@return const char * 参数值
	 */    
	virtual const char * FUNCTION_CALL_MODE getOption(const char * szParamName,const char * szDefaultValue)=0;
    
    ///根据全局数据ID，返回指定全局数据指针
    /**
     *@param int iGlbDataID  应用数据ID
     *@return void * 成功返回相应的数据区指针，出错返回NULL
     */
    virtual void *  FUNCTION_CALL_MODE getGlbData(int iGlbDataID)=0;

	///根据全局数据ID，保存指定全局数据指针
    /**
     *@param int iGlbDataID  全局数据ID
     *@param const void * pData      数据指针(不能为NULL)
     *@return int 成功返回I_OK，出错返回I_NONE
     */
    virtual int  FUNCTION_CALL_MODE setGlbData(int iGlbDataID,void * pData)=0;

	///取配置的安全等级
	/**
	 *@return int 安全等级(0，不校验组件签名，1 校验组件签名)
	 */
    virtual int FUNCTION_CALL_MODE getSafeLevel()=0;
    
};

struct tagBaseServiceInfo;

//基础服务组件引出的三个函数指针(动态加载时使用)，其中PFgetxxxxInfo是必须的。
///返回孤子实例函数指针,对于初始化过程不依赖于其他组件的组件，在创建时初始化；
typedef IKnown * (FUNCTION_CALL_MODE * PFgetxxxxInstance ) (IAppContext * );
///初始化函数指针（会在所有基础组件被动态加载后才调用，若基础组件需求获取AS/AR框架内部对象，则需要在管理接口响应系统事件MSG_ONSTART）
typedef int (FUNCTION_CALL_MODE * PFinitxxxx ) (IAppContext * );
///基础服务组件信息查询，调用者从iIndex=0开始逐个轮询，直到返回I_NONE为止;
typedef int (FUNCTION_CALL_MODE * PFgetxxxxInfo ) (int iIndex, tagBaseServiceInfo * ppv );

///基础服务组件信息(动态加载时使用)
struct tagBaseServiceInfo
{
	///基础服务ID
	int iServiceID;
	///基础服务版号信息yyyymmdd
	int iVersion;
	///基础服务组件初始化函数指针
    PFinitxxxx  xxxxInit;
	///获取基础服务组件孤子实例函数指针
    PFgetxxxxInstance  getxxxxInstance;
	///本服务组件初始化时所依赖的其他基础服务ID(即其他服务必须先初始化好)
	int iDependServices;
	///服务描述信息
	const char * Caption;
};

#endif
