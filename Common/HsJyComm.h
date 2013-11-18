// -*- C++ -*-

//==========================================================================
/**
 *  @file   HsJyComm.h
 *  HsJyComm.h,v 0.1 2007/08/01 20:16:15 westwild Exp
 *
 *  @author Westwild
 */
//==========================================================================

#ifndef _Hs_Jy_Comm_H
#define _Hs_Jy_Comm_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//#define ACE_BUILD_SVC_DLL
//#include "ace/svc_export.h"

#define HSJYCOMM_HAS_DLL	1



//#include "HsJyComm_Export.h"
//#include "HandleManager.h"

/// 版本'7611'时，报文中存在的协议头
//typedef struct {
//	ACE_UINT16	Action;			// Short	2	功能号
//	ACE_UINT16	CellIndex;		// Short	2	营业部号
//	ACE_UINT32	OnlineID;		// INT	4	在线客户号
//	ACE_UINT32	OnlineToken;	// INT	4	在线令牌
//	ACE_UINT32	ReturnCode;		// INT	4	返回码（只对应答有效），0表示成功
//	ACE_UINT32	ClientAddr;		// INT	4	客户端发起的地址
//	ACE_UINT32	SenderID;		// INT	4	发送方标识，原样返回
//}TProtoHeader;

// 接口定义
typedef int	HJC_PACKET_MANAGER;		// 报文管理器
typedef int	HJC_ENV_INTERFACE;		// 通讯环境接口
typedef UINT ACE_UINT32;




#define  HsJyComm_Export __declspec (dllimport)


/// 导出函数

////////////////////////////////////////////////////////
/// 获取通讯环境模块最新的错误信息
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
const char *hjc_get_last_error();

/// 获取通讯环境模块最新的错误码
extern "C" HsJyComm_Export
int hjc_get_last_errno();

////////////////////////////////////////////////////////
// 说明
// 1. 同一报文中协议单元名字不允许重复
// 2. 协议单元名字必须为小写

// 打包器支持的协议报文类型
#define HJC_PU_NV			1		// 简单键值类型
#define HJC_PU_COMPOSE		2		// 复合单元
#define HJC_PU_BIN			3		// 二进制单元


/**
 * @brief 创建一个协议报文管理器
 *
 * @return INVALID_HHHANDLE		创建失败
 * @return !=INVALID_HHHANDLE	创建成功，返回协议报文管理器的句柄
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
HJC_PACKET_MANAGER hjcpm_create_pm (void);

extern "C" HsJyComm_Export
int HS_GetKeyState(HJC_ENV_INTERFACE hcei, char *p11);

/**
 * @brief 解析流模式报文
 *
 * @param	hpm: IN			指定进行操作的协议报文管理器句柄
 * @param	proto_buf: IN	流模式的报文数据
 * @param	buf_len: IN		指定proto_buf的有效长度
 *
 * @return	<0	报文解析失败
 * @return	=0	成功
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_parse_proto_mesg( HJC_PACKET_MANAGER hpm,
						   const char *proto_buf,
						   ACE_UINT32 buf_len );


#define HJC_PUH_ACTION_IDX			0	// 功能号
#define HJC_PUH_CELLINDEX_IDX		1	// 营业部号
#define HJC_PUH_ONLINEID_IDX		2	// 在线客户号
#define HJC_PUH_ONLINETOKEN_IDX		3	// 在线令牌
#define HJC_PUH_RETURNCODE_IDX		4	// 返回码
#define HJC_PUH_CLIENTADDR_IDX		5	// 客户端发起的地址
#define HJC_PUH_SENDERID_IDX		6	// 发送方标识，原样返回
/**
 * @breif 封装接口供客户端设置TProtoHeader的相关字段
 *
 * @param	hpm: IN		指定进行操作的协议报文管理器句柄
 * @param	idx: IN		设置字段的标示，其值可参考HJC_PUH_*的定义
 * @param	value: IN	设置字段的值
 *
 * @return	=0	成功
 * @return	-1	失败，idx（字段标示）无效
 **/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_set_header_value( HJC_PACKET_MANAGER hpm,
						   int idx,
						   ACE_UINT32 value );

/**
 * @breif 封装接口供客户端获取TProtoHeader的相关字段
 *
 * @param	hpm: IN		指定进行操作的协议报文管理器句柄
 * @param	idx: IN		获取字段的标示，其值可参考HJC_PUH_*的定义
 * @param	value: OUT	获取字段的值
 *
 * @return	=0	成功
 * @return	-1	失败，idx（字段标示）无效
 **/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_get_header_value( HJC_PACKET_MANAGER hpm,
						   int idx,
						   ACE_UINT32 &value );

/**
 * @brief 向协议报文管理器中推入一个协议单元到尾部
 *
 * @param	hpm: IN			指定进行操作的协议报文管理器句柄
 * @param	type: IN		加入的协议单元的类型
 * @param	name: IN		协议单元的名字
 * @param	value: IN		协议单元的值
 * @param	value_len: IN	协议单元的值的长度
 * 
 * @return	0			成功
 * @return	-1			失败，内存错误或已存在同名的协议单元
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_push_back_proto_unit( HJC_PACKET_MANAGER hpm,
							   int type,
							   const char* name,
							   const char* value,
							   ACE_UINT32 value_len );

/**
 * @brief 通过name获取报文中的协议单元
 *
 * 针对我们的系统应用，协议单元在匹配时必须是大小写不敏感的，在实现上增加
 * 了<key_>已保存小写的名字，在组织hash_map时是以<key_>为键值。在搜索时必
 * 须将调用者传入的名字先变为小写然后才能进行，同时考虑到调用者几乎都是通
 * 过直接指定的静态字符串的形式进行调用! 所以为了提高效率，当前版本并没有
 * 在搜索时对调用者提供的名字进行小写转换，而是把这个要求作为调用条件! 用
 * 户必须保证使用全小写的名字进行调用!!!!!!
 *
 * @param	hpm: IN				指定进行操作的协议报文管理器句柄
 * @param	type: IN			指定要获取的协议单元的类型
 * @param	name: IN			指定要获取的协议单元的名字
 * @param	value_buf: IN		用于存放协议单元数据的缓冲，如果传入一个空
 *								值，则调用结束时通过value_len返回协议单元数
 *								据的实际长度
 * @param	value_len: INOUT	调用时用于指定value_buf缓冲的实际容量，调
 *								用结束时返回数据的实际长度
 *
 * @return	>0	获取协议单元成功
 * @return	=0	调用者提供的缓冲区容量不足或查询实际长度返回
 * @return	-1	获取协议单元失败，在报文中不存在此名字的协议单元
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_get_proto_unit( HJC_PACKET_MANAGER hpm,
						 const char *name,
						 char* value_buf,
						 ACE_UINT32 &value_len );

/**
 * @brief 输出到字符流模式的报文
 *
 * 按指定顺序将所有协议单元输出到字符流模式的报文，在修改了对象的任何协议单
 * 元后必须重新调用这个接口才能获得变动过的字符流模式报文
 *
 * @param	hpm: IN				指定进行操作的协议报文管理器句柄
 * @param	packet_buf: IN		用于存放字符流模式报文的缓冲，如果传入一个空
 *								值，则调用结束时通过buf_len返回报文数据的实际
 *								长度
 * @param	buf_len: INOUT		调用时用于指定packet_buf缓冲的实际容量，调用
 *								结束时返回数据的实际长度
 *
 * @return	>0	输出成功
 * @return	=0	容量不足或查询输出。此时通过buf_len返回实际需要的缓冲区容量
 * @return	-1	输出失败，内存错误
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_compose( HJC_PACKET_MANAGER hpm,
				  char* packet_buf,
				  ACE_UINT32 &buf_len );

/**
 * @brief 销毁协议报文管理器
 *
 * 使用hjcpm_create_pm所创建的协议报文管理器使用完后必须调用此方法销毁
 *
 * @param	hpm: IN		指定需要销毁的协议报文处理器
 *
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
void hjcpm_destroy_pm( HJC_PACKET_MANAGER hpm );


///////////////////////////////////////////////////////////////////

// 支持的通讯接口类型
#define HJC_EI_TCP			1		// 普通tcp连接  默认7611协议, 老版本的可以支持,不用修改
#define HJC_EI_SSL			2		// ssl连接		默认7611协议, 老版本的可以支持,不用修改
#define HJC_EI_TCP_7611		3		// 普通tcp连接, 使用7611通讯协议
#define HJC_EI_TCP_7612		4		// 普通tcp连接, 使用7612通讯协议
#define HJC_EI_SSL_7611		5		// SSL连接, 使用7611通讯协议
#define HJC_EI_SSL_7612		6		// SSL连接, 使用7612通讯协议
#define HJC_EI_HQ		7

/**
 * @brief 创建通讯接口
 *
 * @param type:			IN	通讯接口类型
 * @param host:			IN	服务器地址
 * @param port:			IN	服务器端口
 * @param catye:		IN	通讯接口类型为HJC_EI_SSL时，使用的客户端证书的ca类型，如果为0，则需要时由用户选择
 * @param cert:			IN	通讯接口类型为HJC_EI_SSL时，需要使用的客户端证书文件，如果为0，则需要时由用户选择
 * @param key:			IN	通讯接口类型为HJC_EI_SSL时，需要使用的客户端私钥文件，如果为0，则需要时由用户选择
 * @param pwd:			IN	通讯接口类型为HJC_EI_SSL时，需要使用的私钥文件密码，如果为0，则需要时由用户选择
 * @param checkca		IN	是否一定要校验证书的正确性 , 如果为0则, 不需校验本地证书文件的正确性, 非0需要校验
 * @param certserial	IN  证书序列号, 证书的唯一标识
 *
 * @return INVALID_HHHANDLE		创建失败
 * @return !=INVALID_HHHANDLE	创建成功，返回通讯接口的句柄
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
HJC_ENV_INTERFACE hjcei_create_ei( int type,
								  const char *host,
								  int port,
								  const char *catype,
								  const char *cert,
								  const char *key,
								  const char *pwd,
								  int   checkca = 0
								 );

//#define HJC_PROXY_TYPE_NONE		0		// 不使用代理
//#define HJC_PROXY_TYPE_SOCKS4	1		// SOCKS4代理
//#define HJC_PROXY_TYPE_SOCKS5	2		// SOCKS5代理，可以支持用户名/密码
//#define HJC_PROXY_TYPE_HTTP11	3		// HTTP长连接代理，可以支持用户名/密码
#define HJC_PROXY_TYPE_SOCKS4		0					//	使用SOCKS4代理协议
#define HJC_PROXY_TYPE_HTTP11		1					//	HTTP长连接代理，可以支持用户名/密码
#define HJC_PROXY_TYPE_SOCKS5		2					//	使用SOCKS5代理协议
//#define HJC_PROXY_TYPE_HTTPGET		3					//	使用HTTP<Get>代理协议<binary>
//#define HJC_PROXY_TYPE_HTTPGETText	4					//	使用HTTP<Get>代理协议<text>
//#define HJC_PROXY_TYPE_SOCKS4A      5                   //  使用SOCKS4A代理协议
#define HJC_PROXY_TYPE_NONE			0xffffffff			//  不使用代理

/**
 * @brief 设置代理服务器信息
 *
 * @param	hcei: IN	hjcei_create_ei方法成功创建的通讯接口
 * @param	type: IN	代理服务器类型
 * @param	host: IN	代理服务器地址
 * @param	port: IN	代理服务器端口
 * @param	name: IN	代理服务器需要的用户名
 * @param	pwd: IN		代理服务器需要的用户密码
 * @param	ul: IN		代理服务器是否验证用户名密码
 *
 * @return	<0	失败
 * @return	=0	成功
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_set_proxy( HJC_ENV_INTERFACE hcei,
					int	type,
					const char *host,
					int prot,
					const char *name,
					const char *pwd,
					int ul );

/**
 * @brief 连接服务器
 *
 * @param	hcei: IN		hjcei_create_ei方法成功创建的通讯接口
 * @param	timeout: IN		连接超时时间，单位为秒，0表示一直等待到系统返回
 *
 * @return <0	连接失败
 * @return =0	连接成功
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_connect( HJC_ENV_INTERFACE hcei,
				  ACE_UINT32 timeout );

/**
 * @brief 获取完整报文的报文管理器
 *
 * @param	hcei: IN		hjcei_create_ei方法成功创建的通讯接口
 * @param	hpm: OUT		不等于INVALID_HHHANDLE的则为已解析过的报文管理器句柄，用户必须负责销毁
 * @param	timeout: IN		没有完整报文时等待的时间， <0 无限期等待，=0 马上返回，>0 等待时长（单位为毫秒）
 *
 * @return	<0	在此连接上出现错误
 * @return	=0	没有完整报文
 * @return	>0	已解析到完整报文
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_get_mesg( HJC_ENV_INTERFACE hcei,
				   HJC_PACKET_MANAGER &hpm,
				   ACE_UINT32 timeout );

/**
 * @brief 获取完整流模式报文
 *
 * @param	hcei: IN		hjcei_create_ei方法成功创建的通讯接口
 * @param	buf: IN			调用者提供的存放数据的缓冲区，如果为0表示只取报文长度到buf_len字段
 * @param	buf_len: INOUT	调用时用于指定buf的长度，返回时标示buf中有效数据的长度
 * @param	timeout: IN		无数据可读时等待的时间， <0 无限期等待，=0 马上返回，>0 等待时长（单位为毫秒）
 *
 * @return	<0	在此连接上出现错误
 * @return	=0	没有完整报文，此时用户必须判断buf_len字段，如果>0，表示已经有了完整报文但是用户传入的buf
 *				参数为0或者指定的buf有效容量(buf_len的传入值)不够
 * @return	>0	已解析到完整报文，此时buf_len指明了有效数据的长度
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_read_mesg( HJC_ENV_INTERFACE hcei,
					char *buf,
					ACE_UINT32 &buf_len,
					ACE_UINT32 timeout );

/// 查询目前缓存了多少个完整报文
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_get_mesg_count( HJC_ENV_INTERFACE hcei );

/**
 * @brief 读取数据
 *
 * 注意，如果使用了read方法，则破坏了底层相关缓存设施，以后在次heci上只能保证用户进行普通流模式的read
 * 方法调用，其他与协议相关的方法（例如: hjcei_get_mesg, hjcei_read_mesg）调用将不保证正确性。
 *
 * @param	hcei: IN		hjcei_create_ei方法成功创建的通讯接口
 * @param	buf: IN			调用者提供的存放数据的缓冲区
 * @param	buf_len: INOUT	调用时用于指定buf的长度，返回时标示buf中有效数据的长度
 * @param	timeout: IN		无数据可读时等待的时间， <0 无限期等待，=0 马上返回，>0 等待时长（单位为毫秒）
 *
 * @return	<0	在此连接上出现错误
 * @return	=0	没有数据
 * @return	>0	读到的有效数据的长度
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_read( HJC_ENV_INTERFACE heci,
			   char *buf,
			   ACE_UINT32 &buf_len,
			   ACE_UINT32 timeout );

/**
 * @brief 发送完整报文
 *
 * @param	hcei: IN	hjcei_create_ei方法成功创建的通讯接口
 * @param	hpm: IN		包含完整报文的管理器句柄
 * @param	timeout: IN	指定成功发送等待的超时时间（单位为毫秒）
 *
 * @return	<=0	报文发送出错，网络错误或者发送超时
 * @return	>0	完整报文发送成功
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_put_mesg( HJC_ENV_INTERFACE hcei,
				   HJC_PACKET_MANAGER hpm,
				   ACE_UINT32 timeout );

/**
 * @brief 发送完整报文数据
 *
 * @param	hcei: IN		hjcei_create_ei方法成功创建的通讯接口
 * @param	buf: IN			待发送数据的缓冲区
 * @param	buf_len: IN		指定buf中有效数据的长度
 * @param	timeout: IN		指定成功发送等待的超时时间（单位为毫秒）
 *
 * @return	<=0	报文发送出错，网络错误、对端关闭或者发送超时
 * @return	>0	数据发送成功
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_write_mesg( HJC_ENV_INTERFACE hcei,
				char *buf,
				ACE_UINT32 buf_len,
				ACE_UINT32 timeout );

/**
 * @brief 发送流模式数据
 *
 * @param	hcei: IN		hjcei_create_ei方法成功创建的通讯接口
 * @param	buf: IN			待发送数据的缓冲区
 * @param	buf_len: IN		指定buf中有效数据的长度
 * @param	timeout: IN		指定成功发送等待的超时时间（单位为毫秒）
 *
 * @return	<0	报文发送出错，网络错误或者对端关闭
 * @return	>=0	数据成功发送的字节数
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_write( HJC_ENV_INTERFACE hcei,
				char *buf,
				ACE_UINT32 buf_len,
				ACE_UINT32 timeout );

/// 销毁通讯接口
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
void hjcei_destroy_ei( HJC_ENV_INTERFACE hcei );

//定义数字签名散列加密类型
#define  HJC_EVP_MD5	1
#define  HJC_EVP_SHA1	2


///创建数字签名
/**
 * @brief 创建数字签名
 *
 * @brief	type: IN	报文分解函数的类型,目前有两种可以选择:MD5, sha1
 * @brief	key : IN	密钥文件名字
 * @brief	userdata: IN 用户提供的供报文分解函数所学要分解的值,可以是任何值,因为产生的MDF是唯一的
 * @brief	datalen: IN	 用户提供数据的长度
 * @brief   md	: IN	产生数字签名后所要保存的数组, 推荐长度为65, 目前产生的数字签名程度均为64
 * @return  <0	创建数字签名过程出错
 * @return  >0	创建成功，返回数字签名的长度
*/
extern "C" HsJyComm_Export
int hjcei_create_digitalsign(int type, const char *key,const char *userdata,unsigned int datalen, unsigned char *md, const char *passwd);

///验证数字签名
/**
* @brief 验证数字签名
*
* @brief	type: IN	报文分解函数的类型,目前有两种可以选择:MD5, sha1
* @brief	cert: IN	证书文件名字
* @brief	userdata: IN 用户提供的供报文分解函数所学要分解的值
* @brief	mdlen: IN	数字签名的长度
* @brief   md	: IN	数字签名保存的数组
* @return  <0	验证失败
* @return  =0	验证成功
*/
extern "C" HsJyComm_Export
int hjcei_verify_digitalsign(int type, const char *cert, const char *userdata,unsigned int datalen,unsigned int mdlen,unsigned char *md );

///生成数字证书请求
/**
* @brief 生成数字证书请求
*
* @brief	pPass		: IN	密钥文件的密钥, 格式: pass:888888, 其中888888是密钥,可修改
* @brief	pCsrout		: IN	请求文件存放路径, 如:e:\hs_cer.pem
* @brief	pKeyout		: IN	密钥文件存放路径, 如:e:\hs_key.pem
* @brief	pConfigfile	: IN	配置文件路径
* @return  =0	生成成功
* @return  !=0	生成失败
*/
extern "C" HsJyComm_Export
int  HS_CreateCsr(char *pPass,char *pCsrout,char *pKeyout,char *pConfigfile);


///
/**
* @brief 
*
* @brief	pP7			: IN	pP7文件, 
* @brief	pPem		: IN	转换后pPem文件信息
* @return  =1	生成成功
* @return  =0	生成失败
*/
extern "C" HsJyComm_Export
int  HS_P7_PEM(char *pP7,char *pPem);

///
/**
* @brief 
*
* @brief	pP7			: IN	pP7文件, 
* @brief	pPem		: IN	转换后pPem文件信息
* @return  =1	生成成功
* @return  =0	生成失败
*/
extern "C" HsJyComm_Export
int  HS_PEM_P12(char *pPem,char *pPkey,char *pPassin,char *pPassout,char *pP12);

/**
* @brief 修改证书密码(如果是usb则就该usb的pin密码)
*
* @brief	type			: IN	"1": 文件类型, "usb" : usbKEY证书 
* @brief	key				: IN	如果是文件证书,传入证书存放路径, 如果是usb证书则是证书序列号 
* @brief	oldpasswd		: IN	旧密码
* @brief	newpasswd		: IN	新密码
* @return  =0	生成成功
* @return  =-1	生成失败
*/
extern "C" HsJyComm_Export
int  hjcei_ChPasswd(char *type, char *key, char *oldpasswd, char *newpasswd, char *p11);


#define HS_DES_ENCRYPT	1	//加密
#define HS_DES_DECRYPT	0	//解密
/// 使用DES加解密数据
/**
* @brief 使用DES加解密数据
*
* @brief	type	加解密类型:1 加密, 0 解密
* @brief	In   	需要加/解密的数据
* @brief	nInLen  In的数据长度
* @brief	Out 	加/解密后数据, 当Out=NULL时， nOutLen返回加解所以缓冲区长度
* @brief    nOutLen 加/解密后数据长度,密文长度应该是nKeyLen的整数倍并大于nInLen, 返回加密后数据长度
* @return  <0	加密失败
* @return  =0	加密成功
* @return  =1   表示输出Out的空间不够, nOutLen保存需要的长度
*/
extern "C" HsJyComm_Export
int hjcei_DES_crypt(int type,  char *In, int nInLen, char *Out, int &nOutLen);

#define FORMAT_CTCA     1
#define FORMAT_SHECA    2
#define FORMAT_CWCA     3
#define FORMAT_HSCA     4			//恒生CA
#define FORMAT_ZJCA     5
#define	FORMAT_P7_PEM	6			//p7格式转化成PEM
#define	FORMAT_PEM_P12	7			//PEM文件转化成p7格式
/**
* @brief 证书格式格式化
*
* @brief	pPem			需格式化的证书文件内容
* @brief	nPemLen			证书内容长度
* @brief	pOutPath   		格式化后证书路径以及文件名 , 如e:\p.pem
* @brief	nPemType		格式化类型
* @brief	pPkey 			证书对应的密钥文件
* @brief    pPassin			密钥文件对应的密钥
* @brief    pPassin			格式化p12对应的文件的密钥
* @return  <0				格式化失败
* @return  =0				格式化成功
*/
extern "C" HsJyComm_Export
int	hjcei_Format_Cert(char *pPem, char *pOutPath, int nPemType,char *pPkey,char *pPassin,char *pPassout);

/**
*	@brief	查询证书剩余可用天数
*	
*	@brief	hcei			连接句柄
*
*	@return	>= 0			查询成功, 返回剩余天数
*	@return < 0				查询失败
*/



extern "C" HsJyComm_Export
int HS_SSL_GetValidDays(HJC_ENV_INTERFACE hcei);


/**
*	@brief	证书导入/导出功能
*	@brief	type			功能标识: 0 导出  1 导入
*	@brief	pSurPath		源文件存放路径
*   @brief	pDesPath		目的文件存放路径
*	@brief	usepasswd		是否使用默认密钥加密:	1 加密 0 不加密
*
*	@return		0	   成功
	@return     <0     失败
*/

#define		CERT_OUTPORT		0	//导出
#define		CERT_INPORT			1	//导入

extern "C" HsJyComm_Export
int hjcei_cert_inport(int type, char *pSurPath, char *pDesPath);


/**
*	@brief	证书导入/导出子主功能
*	@brief	type				功能标识: 0 导出  1 导入
*	@brief	pCertSurPath		源证书文件存放路径
*   @brief	pCertDesPath		目的证书文件存放路径
*	@brief	pKeySurPath			源密钥文件存放路径
*   @brief	pKeyDesPath			目的密钥文件路径
*	@brief	passwd				密钥文件的密码:	如"111111"
*
*	@return		0	   成功
	@return     <0     失败
*/

extern "C" HsJyComm_Export
int hjcei_certkey_inport(int type, char *pCertSurPath, char *pCertDesPath, char *pKeySurPath, char *pKeyDesPath,  char *passwd);


/**
*	@brief	查询证书序列号
*	
*	@brief	hcei			连接句柄
*	@brief	cert_no			保存序列号缓冲区
*	@brief	nolen			缓冲区长度
*	@return	>= 0			查询成功
*	@return < 0				查询失败
*/


extern "C" HsJyComm_Export
int hjcei_get_certno(HJC_ENV_INTERFACE hcei, char *cert_no, int nolen);


/**
*	@brief	获取usb证书内容
*	
*	@brief	pCertContext		返回证书信息结构体
*	@brief	nCertNum			传入结构体缓冲区大小
*	@return > 0					返回实际证书个数
*	@return <0					读取失败
*/

extern class STCertContext;

extern "C" HsJyComm_Export
int hjcei_get_cert_context(STCertContext *pCertContext, int nCertNum, char *p11);



/**
*	@brief	显示证书内容
*	
*	@brief	type				证书类型: “1”: 文件证书 "usb": usb证书
*	@brief	cert				证书路径或者序列号
*	@brief	pCertContext		证书内容缓冲区
*	@return = 0					获取成功
*	@return < 0					获取失败
*/
extern "C" HsJyComm_Export
int hjcei_get_cert(const char *type,const char *cert,STCertContext *pCertContext , char *p11);

/**********************************************************************
函数名称：HS_Sign_ZXData
函数功能：签名
处理过程：

1.循环读取原数据加密后保存到输出缓冲区中。
参数说明：
nAlg_ID:				[IN]  int 		签名类型 detached：PKCS7签名格式，1代表detached类型，0代表attached类型；
pDataIn:				[IN]  char[]	待签名内容
nDataLen:				[IN]  int		内容长度
pDataOut:				[OUT] int		签名后输出缓冲区
nDataOutLen:			[OUT] int		签名后密文长度
返回值：成功返回0,否则返回-1,	nDataOutLen保存签名长度
备注说明：
	当传入pDataOut=NULL或pDataOutLen=0时,nDataOutLen返回需要保存签名缓冲区长度
	Detached类型：此类型的P7包中不包含签名原文，对签名进行验证时必须输入签名原文做参数；
	Attached类型：此类型的P7包中包含签名原文，对签名进行验证时不需要输入签名原文做参数

**********************************************************************/
extern "C" HsJyComm_Export
int	HS_Sign_ZXData(int  nAlgID, const char *certno, const char *pDataIn, int nDataInLen, char *pDataOut, int &nDataOutLen);

/**********************************************************************
函数名称：HS_P12_Dispatch
函数功能：P12证书分拆组合
处理过程：

参数说明：
type:					[IN]  int 			分拆类型: 0: 拆分 1: 合并
pfx:					[IN]  char[]		pfx文件路径
cert:					[IN]  char[]		证书路径
key:					[IN]  char[]		私钥路径
passin:					[IN]  char[]		输入密码: 如: 111111
passout:				[IN]  char[]		输出密码: 如: 222222
返回值：成功返回0,否则返回-1
备注说明：
	
**********************************************************************/

extern "C" HsJyComm_Export
int	HS_P12_Dispatch(int  type, const char *pfx, const char *certDes, const char *keyDes, const char *passin, const char *passout);

extern "C" HsJyComm_Export
int	HS_Encrypt_Data(int  nAlgID, const char *pDataIn, int nDataInLen, char *pDataOut, int &nDataOutLen, const char *pPasswd);

extern "C" HsJyComm_Export
int	HS_Decrypt_Data(int  nAlgID, const char *pDataIn, int nDataInLen, char *pDataOut, int &nDataOutLen, const char *pPasswd);


//extern "C" HsJyComm_Export
//unsigned int	HS_Get_ConnLocalIP(HJC_ENV_INTERFACE hcei);

extern "C" HsJyComm_Export
int		HS_USB_InitializeSlot(int nType,const char *p11, const char *pAdminPasswd, const char *pUserPasswd);

extern "C" HsJyComm_Export
int		HS_USB_GenerateKeyPair(int nType,const char *p11,  const char *pUserPasswd, const char *pCertNo, const char *pSN,
							   char *pReqOut, int nReqLen);


extern "C" HsJyComm_Export
int	HS_USB_ImportCertificate(int nType,const char *p11,  const char *pUserPasswd,const char *pCertNo,const char *pCert );
/**
*	@brief USBKEY数字签名
*
*
* @param	nType:			IN  int  签名类型 0:中信证券签名  1:飞天标准接口 2:天津文交所签名 
* @param   p11				IN  char[]	具体驱动名称，个别签名方式可以不传该驱动名称
* @param	certno:			IN  int  证书号： 如果是天津文交所的，可以不送
* @param   pPasswd:		IN	char[] USB密码
* @param	pDataIn:		IN  char[] 待签名内容
* @param	nDataLen:		IN  int 内容长度
* @param	pDataOut:		OUT int 签名后输出缓冲区
* @param	nDataOutLen:	OUT int 签名后密文长度
*
* @return 成功返回0, nDataOutLen保存签名长度
* @return 否则返回-1
*
*/
extern "C" HsJyComm_Export
int HS_USB_SignData(int  nType,const char *p11, const char *certno, const char *pPasswd,const char *pDataIn, int nDataInLen, char *pDataOut, int &nDataOutLen);



/**
*	@brief	获取usb证书内容
*	
*	@param  nType				1: 飞天标准接口
*	@pram   p11					驱动接口名称
*	@param	pCertContext		返回证书信息结构体
*	@param	nCertNum			传入结构体缓冲区大小
*	@return > 0					返回实际证书个数
*	@return <0					读取失败
*/
extern "C" HsJyComm_Export
int HS_USB_GetCertContext(int nType ,const char *p11,STCertContext *pCertContext, int nCertNum);





/**
* @brief 修改证书密码(如果是usb则就该usb的pin密码)
*
* @param	type			: IN	1: 飞天标准接口 2: 文件证书 
* @param    p11				: IN    驱动名称，如是文件证书可不穿
* @param	key				: IN	如果是文件证书,传入证书存放路径, 如果是usb证书则是证书序列号 
* @param	oldpasswd		: IN	旧密码
* @param	newpasswd		: IN	新密码
* @return  =0	生成成功
* @return  =-1	生成失败
*/
extern "C" HsJyComm_Export
int  HS_USB_ChangePasswd(int nType, char *p11,char *key, char *oldpasswd, char *newpasswd);


int  HS_USB_ChangePasswd(int nType, char *p11,char *pKey, char *poldpasswd, char *pnewpasswd);

/**
* @brief 修改证书密码(如果是usb则就该usb的pin密码)
*
* @param	type			: IN	1: 飞天标准接口 2: 文件证书 
* @param    p11				: IN    驱动名称，如是文件证书可不穿
* @param	pCertNo			: IN	证书序列号 

* @return  > 0	返回有效天数
* @return  <0	获取失败
*/
extern "C" HsJyComm_Export
int HS_USB_GetValidDays(int nType, const char *p11, const char *pCertNo);



extern "C" HsJyComm_Export
int HS_USB_GetKeyStatus(int nType, const char *p11);

extern "C" HsJyComm_Export
int wangj_test(int nType, const char *pBuf, int nInBufLen, char *pBufOut, int &nOutLen);



/**
* @brief 招行产生登录请求数据包
*
* @param	pType:IN			输入类型:  0:招行登陆签名
* @param	pInput: IN			输入请求包缓冲区
* @param	pOutBuf: IN			输出数据缓冲区
* @param	pOutBufLen: IN		输出缓冲区长度
*
* @return	<0	出错，其中-1 表示用户取消，-2 表示输出BUFFER长度不够，其他为错误
* @return	=0	成功
*/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int HS_CreateSSOPkg(int pType,const char * pInput, char * pOutBuf, int &pOutBufLen);

/**
* @brief 招行验证登录请求
*
* @param	pSSORes: IN			输入验证包缓冲区
* @param	pType:IN			输入类型 : 0 招行验证签名
*
* @return	<0	出错
* @return	=0	成功
*/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int HS_VerifySSOToken(int pType,const char * pSSORes);

/**
* @brief 招行产生签名数据
*
* @param	pInput: IN			输入请求包缓冲区
* @param	pOutBuf: IN			输出数据缓冲区
* @param	pOutBufLen: IN		输出缓冲区长度
*
* @return	<0	出错，其中-1 表示用户取消，-2 表示输出BUFFER长度不够，其他为错误
* @return	=0	成功
*/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int HS_CreateCMBSSOTokenSign(const char* pInput, char* pOutBuf, int* pOutBufLen);


/**
* @brief 获取当前连接的mac或ip地址
*
* @param    hcei			: IN    ei
* @param	nType			: IN	2: mac  
* @param    pBuf			: OUT   输出缓冲区
* @param    nBudLen			: OUT   缓冲区长度

* @return  > 1	: 获取成功
* @return  = 0  : 传入缓冲区长度太小，nBufLen返回需要的长度
* @return  < 0  : 获取失败
*/
extern "C" HsJyComm_Export
int HS_GetConnInfo(HJC_ENV_INTERFACE hcei,int nType, char *pBuf, int &nBufLen);



/**
* @breif 获取协议字段个数
*
* @param	hpm: IN		指定进行操作的协议报文管理器句柄
*
* @return	>=  0	成功,返回具体地段个数
* @return	< 0	    失败
**/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_get_nv_size( HJC_PACKET_MANAGER hpm);

/**
* @brief 通过索引获取报文中的协议单元
*	NV_UNIT			= 1,		// A=content\n
*	HE_UNIT			= 2,		// <A>{len}content
*	BIN_UNIT		= 3,		// [A]{len}content
*	RAW_UNIT		= 4,		// 记录原始的字符流模式的协议单元
*
* @param	hpm: IN				指定进行操作的协议报文管理器句柄
* @param    idx:IN				字段索引值
* @param	name: IN			指定要获取的协议单元的名字
* @param	value_buf: IN		用于存放协议单元数据的缓冲，长度不够时则调用结束时通过value_len返回协议单元数据的实际长度
* @param	value_len: INOUT	调用时用于指定value_buf缓冲的实际容量
*								
* @param	val_type:OUT		字段类型
*
* @return	>0	获取协议单元成功返回字段长度
* @return	=0	调用者提供的缓冲区容量不足或查询实际长度返回
* @return	-1	获取协议单元失败，在报文中不存在此名字的协议单元
*/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_get_nv_name( HJC_PACKET_MANAGER hpm,
						 int idx,
						 char* value_buf,
						 ACE_UINT32 &value_len,
						 int &val_type);

#endif /*_Hs_Jy_Comm_H*/
