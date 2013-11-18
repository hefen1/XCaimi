/*******************************************************
  源程序名称:as_packet.h
  软件著作权:杭州恒生电子股份有限公司
  系统名称:  HZHSGS金融基础件V1.0
  模块名称:  AS模块
  功能说明:  打包解包服务组件
             1、IPackService 服务组件接口
             2、IPacker 打包接口
			 3、TResultSet 解包接口(由as2_ResultSet.h定义)
             
           
  相关文档:  概要设计接口部分
             
  作    者:  zhangly
  开发日期:  20040223
  备    注:  平台无关，支持多种包格式
  修改人员：
  修改日期：
  修改说明：
*********************************************************/
#ifndef AS_PACKET_H
#define AS_PACKET_H

#include <stdio.h>
#include <stdlib.h>
#include "hs_interface.h"
#include "as_ResultSet.h"

#ifndef PLATFORM_EXPORT
#define PLATFORM_EXPORT
#endif

struct IPacker;
struct IUnPacker;
struct IResultSet;

///打包解包服务接口 
struct IPackService : IKnown
{
	///返回业务包格式版本
	/**根据业务包首字符判断业务包格式版本,业务包版本识别规则:
	 *
     *  V1版首部放的是字符串型的列数，所以第一个字节>=0x30;
     *
     *  V2版第一个字节放的是版本号，目前值为0x20，兼容性改升时该值最大升到0x2F
     *
	 *@param void * lpBuffer  业务包数据(必须是指向合法业务包的首字节)
	 *@return int  业务包格式版本(1: V1版,0x20~0x2F V2版版本号)
	 */
    virtual int FUNCTION_CALL_MODE GetVersion(const void * lpBuffer) = 0;
	
	///取一个业务包打包器,获得打包器必须通过FreePacker()释放;
	/**
	 *@param int iVersion 业务包格式版本(取值:1 字串版,其他值 0x20版)
	 *@return IPacker * 打包器接口指针
	 */
    virtual IPacker * FUNCTION_CALL_MODE GetPacker(int iVersion) = 0;
	///释放打包器
	/**
	 *@param IPacker * 要释放的打包器接口指针
	 */
    virtual void FUNCTION_CALL_MODE FreePacker(IPacker * lpPacker) = 0;

	///取一个业务包解包器,获得解包器必须通过FreePacker()释放;
	/**调用GetVersion
	 *@param void * lpBuffer 要解包的数据（不含AR通信包头）
	 *@param unsigned int iLen 数据长度
	 *@return IUnPacker * 结果集操作接口指针
	 */
    virtual IUnPacker *  FUNCTION_CALL_MODE GetUnPacker(void * lpBuffer, unsigned int iLen ) = 0;

	///释放解包器
    virtual void FUNCTION_CALL_MODE FreeUnPacker(IUnPacker * lpUnPacker) = 0;
};

///多结果集打包接口(一个包可有多个异构结果集)
/**执行序列:
 *
 * 0、准备： SetBuffer(),如果打包缓存区由调用者提供,则必须在BeginPack()之前准备;
 * 1、开始:  BeginPack(),打包器复位;
 *
 * 2、第一个结果集打包：
 *
 *(a)添加字段名列表域：AddField()
 *
 *(b)按照结果集二维表顺序，逐字段，逐条记录添加内容：AddValue()
 *
 * 3、设置第一个结果集的返回码(可选，如果返回码为0则不用设置) SetFirstRetCode()
 *
 * 4、打下一个结果集(可选) NewDataSet()，此处同时设置了该结果集的返回码；
 * 
 * 5、参考第2步实现一个结果集打包；
 *
 * 6、结束：EndPack(),重复调用会导致加入空结果集;
 *
 * 7、取打包结果(缓存区，缓存区大小，数据长度)
 *    打包结果也可以直接解包UnPack()返回解包接口
 *
 *使用注意事项:IPacker所使用的内存缓存区，均由调用者控制；
 *             结果集附带的返回码，只有在包格式版本0x20以上时有效；
 */
struct IPacker
{
    ///打包器初始化(使用调用者的缓存区)
	/** 第一次使用打包器时，可先使用本方法设置好缓冲区(数据长度被置为iDataLen)
	 *@param  char * pBuf  缓冲区地址
 	 *@param  int iBufSize  缓冲区空间
 	 *@param  int iDataLen  已有数据长度，新增数据加在已有数据之后（只对V1.0格式的包有效） 	 
 	 */
	virtual void FUNCTION_CALL_MODE SetBuffer(void * pBuf,int iBufSize,int iDataLen=0 )=0;

	///复位，重新开始打另一个包(字段数与记录数置为0行0例)
	/**
	 * 功能：开始打包，把包长度清零(重复使用已有的缓存区空间)
	 *@return 无
	 */
	virtual void FUNCTION_CALL_MODE BeginPack(void)=0;

	///开始打一个结果集
	/**在打单结果集的包时，可以不调用本方法,均取默认值
	 *@param const char *szDatasetName 0x20版打包需要指明结果集名字
	 *@param int iReturnCode           0x20版打包需要为每个结果集指明返回值
	 */
	virtual int FUNCTION_CALL_MODE NewDataset(const char *szDatasetName, int iReturnCode = 0)=0;

	/**
	 * 功能：向包添加字段
	 *
	 *有执行次序要求:在 NewDataset()或Reset(),SetBuffer()之后,逐个字段按顺序添加;
	 *
	 *@param szFieldName：字段名
	 *@param cFieldType ：字段类型:I整数，F浮点数，C字符，S字符串，R任意二进制数据
	 *@param iFieldWidth ：字段宽度（所占最大字节数）
	 *@param iFieldScale ：字段精度,即cFieldType='F'时的小数位数(缺省为4位小数)
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddField(const char *szFieldName,char cFieldType ='S',int iFieldWidth=255,int iFieldScale=4)=0;

	/**
	 * 功能：向包添加字符串数据
     * 有执行次序要求:必须在所有字段增加完之后,逐个字段按顺序添加;
	 *@param       szValue：字符串数据
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddValue(const char *szValue)=0;

	/**
     * 功能：向包添加整数数据
 	 *@param       iValue：整数数据
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddValue(int iValue)=0;
	
    /**
	 * 功能：向包添加浮点数据
	 *@param       fValue：浮点数据
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddValue(double fValue)=0;

	/**
	 * 功能：向包添加一个字符
	 *@param		 cValue：字符
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddValue(char cValue)=0;

	/**
	 * 功能：向包添加一个大对象
	 *@param	void * lpBuff 数据区
	 *@param	int iLen  数据长度	 
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddValue(void * lpBuff,int iLen)=0;

    ///结束打包
	virtual void FUNCTION_CALL_MODE EndPack()=0;
 
	/**
     * 功能：取打包结果指针
	 *@return 打包结果指针
     */
	virtual void * FUNCTION_CALL_MODE PackBuf(void) = 0;
	
	/**
     * 功能：取打包结果长度
     *@return 打包结果长度
	 */
	virtual int FUNCTION_CALL_MODE PackLen(void) = 0;
	
	/**
	 * 功能：取打包结果缓冲区大小
     *@return 打包结果缓冲区大小
	 */
	virtual int FUNCTION_CALL_MODE PackBufSize(void) = 0;
	
	/**
	 * 功能：取打包格式版本
     *@return 版本
	 */
	virtual int FUNCTION_CALL_MODE getVersion(void) = 0;
	
	///设置结果集的返回码(0x20版以上要求)，错误结果集需要设置
	/**返回码取缺省值0，则不设置，如果设置，则必须在EndPack()之前调用
	 * 功能：取打包格式版本
     *@return 版本
	 */
	virtual void FUNCTION_CALL_MODE SetReturnCode(unsigned long dwRetCode) = 0;

	/**
     * 功能：直接返回当前打包结果的解包接口,必须在EndPack()之后才能调用,在打包器释放时相应的解包器实例也释放
     *
     *@return 解包器接口，此解包接口不能调用destroy()来释放
	 */
	virtual IUnPacker * FUNCTION_CALL_MODE UnPack(void) = 0;

};

///解包器接口
struct IUnPacker : IResultSet
{
	/**
	 * 功能：取打包格式版本
     *@return 版本
	 */
	virtual int FUNCTION_CALL_MODE getVersion(void) = 0;

	/**
     * 功能：取解包数据长度
     *@return 解包数据长度
	 */
	virtual int FUNCTION_CALL_MODE Open(void * lpBuffer,unsigned int iLen) = 0;

    ///取结果集个数(0x20以上版本支持)
    virtual int FUNCTION_CALL_MODE GetDatasetCount()=0;

    ///设置当前结果集(0x20以上版本支持)
    /**
	 *@param  int nIndex				结果集编号
	 *@return int						1表示成功，否则为失败
	 */
    virtual int FUNCTION_CALL_MODE SetCurrentDataset(int nIndex)=0;

    ///设置当前结果集 (0x20以上版本支持)
    /**
	 *@param  const char *szDatasetName	结果集名称
	 *@return int						1 表示成功，否则为失败
	 */
    virtual int FUNCTION_CALL_MODE SetCurrentDataset(const char *szDatasetName)=0;

	/**
     * 功能：取解包数据区指针
	 *@return 数据区指针
     */
	virtual void * FUNCTION_CALL_MODE PackBuf(void) = 0;

	/**
     * 功能：取解包数据长度
     *@return 解包数据长度
	 */
	virtual unsigned int FUNCTION_CALL_MODE PackLen(void) = 0;
	
};

//----------作为可动态加载的基础服务组件，必须定义以下引出函数---------

#ifdef __cplusplus
extern "C" { 
#endif

///组件初始化xxxxInit()
/**
 *@return int 版本号 yyyymmdd形式
 */
int  FUNCTION_CALL_MODE PackServiceInit(IAppContext * p);

///取服务接口句柄 getxxxxInstance()
/**
 *@return void * 基础服务句柄 
 */
IKnown * FUNCTION_CALL_MODE getPackServiceInstance(IAppContext * p);

///返回统一的AS基础服务组件查询接口 getxxxInfo()
/**当基础服务组件动态加载时，通过本接口或以获取服务信息(包括xxxxInit(),getxxxxInterface()函数指针)
 *@param void ** ppv    基础服务组件信息(输出参数)，
 *@return 返回0表示查到所要的接口，否则返回负数
 */
int FUNCTION_CALL_MODE getPackServiceInfo(int iIndex, tagBaseServiceInfo * ppv );

#ifdef __cplusplus
  }
#endif
 
#endif
