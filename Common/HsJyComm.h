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

/// �汾'7611'ʱ�������д��ڵ�Э��ͷ
//typedef struct {
//	ACE_UINT16	Action;			// Short	2	���ܺ�
//	ACE_UINT16	CellIndex;		// Short	2	Ӫҵ����
//	ACE_UINT32	OnlineID;		// INT	4	���߿ͻ���
//	ACE_UINT32	OnlineToken;	// INT	4	��������
//	ACE_UINT32	ReturnCode;		// INT	4	�����루ֻ��Ӧ����Ч����0��ʾ�ɹ�
//	ACE_UINT32	ClientAddr;		// INT	4	�ͻ��˷���ĵ�ַ
//	ACE_UINT32	SenderID;		// INT	4	���ͷ���ʶ��ԭ������
//}TProtoHeader;

// �ӿڶ���
typedef int	HJC_PACKET_MANAGER;		// ���Ĺ�����
typedef int	HJC_ENV_INTERFACE;		// ͨѶ�����ӿ�
typedef UINT ACE_UINT32;




#define  HsJyComm_Export __declspec (dllimport)


/// ��������

////////////////////////////////////////////////////////
/// ��ȡͨѶ����ģ�����µĴ�����Ϣ
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
const char *hjc_get_last_error();

/// ��ȡͨѶ����ģ�����µĴ�����
extern "C" HsJyComm_Export
int hjc_get_last_errno();

////////////////////////////////////////////////////////
// ˵��
// 1. ͬһ������Э�鵥Ԫ���ֲ������ظ�
// 2. Э�鵥Ԫ���ֱ���ΪСд

// �����֧�ֵ�Э�鱨������
#define HJC_PU_NV			1		// �򵥼�ֵ����
#define HJC_PU_COMPOSE		2		// ���ϵ�Ԫ
#define HJC_PU_BIN			3		// �����Ƶ�Ԫ


/**
 * @brief ����һ��Э�鱨�Ĺ�����
 *
 * @return INVALID_HHHANDLE		����ʧ��
 * @return !=INVALID_HHHANDLE	�����ɹ�������Э�鱨�Ĺ������ľ��
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
HJC_PACKET_MANAGER hjcpm_create_pm (void);

extern "C" HsJyComm_Export
int HS_GetKeyState(HJC_ENV_INTERFACE hcei, char *p11);

/**
 * @brief ������ģʽ����
 *
 * @param	hpm: IN			ָ�����в�����Э�鱨�Ĺ��������
 * @param	proto_buf: IN	��ģʽ�ı�������
 * @param	buf_len: IN		ָ��proto_buf����Ч����
 *
 * @return	<0	���Ľ���ʧ��
 * @return	=0	�ɹ�
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_parse_proto_mesg( HJC_PACKET_MANAGER hpm,
						   const char *proto_buf,
						   ACE_UINT32 buf_len );


#define HJC_PUH_ACTION_IDX			0	// ���ܺ�
#define HJC_PUH_CELLINDEX_IDX		1	// Ӫҵ����
#define HJC_PUH_ONLINEID_IDX		2	// ���߿ͻ���
#define HJC_PUH_ONLINETOKEN_IDX		3	// ��������
#define HJC_PUH_RETURNCODE_IDX		4	// ������
#define HJC_PUH_CLIENTADDR_IDX		5	// �ͻ��˷���ĵ�ַ
#define HJC_PUH_SENDERID_IDX		6	// ���ͷ���ʶ��ԭ������
/**
 * @breif ��װ�ӿڹ��ͻ�������TProtoHeader������ֶ�
 *
 * @param	hpm: IN		ָ�����в�����Э�鱨�Ĺ��������
 * @param	idx: IN		�����ֶεı�ʾ����ֵ�ɲο�HJC_PUH_*�Ķ���
 * @param	value: IN	�����ֶε�ֵ
 *
 * @return	=0	�ɹ�
 * @return	-1	ʧ�ܣ�idx���ֶα�ʾ����Ч
 **/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_set_header_value( HJC_PACKET_MANAGER hpm,
						   int idx,
						   ACE_UINT32 value );

/**
 * @breif ��װ�ӿڹ��ͻ��˻�ȡTProtoHeader������ֶ�
 *
 * @param	hpm: IN		ָ�����в�����Э�鱨�Ĺ��������
 * @param	idx: IN		��ȡ�ֶεı�ʾ����ֵ�ɲο�HJC_PUH_*�Ķ���
 * @param	value: OUT	��ȡ�ֶε�ֵ
 *
 * @return	=0	�ɹ�
 * @return	-1	ʧ�ܣ�idx���ֶα�ʾ����Ч
 **/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_get_header_value( HJC_PACKET_MANAGER hpm,
						   int idx,
						   ACE_UINT32 &value );

/**
 * @brief ��Э�鱨�Ĺ�����������һ��Э�鵥Ԫ��β��
 *
 * @param	hpm: IN			ָ�����в�����Э�鱨�Ĺ��������
 * @param	type: IN		�����Э�鵥Ԫ������
 * @param	name: IN		Э�鵥Ԫ������
 * @param	value: IN		Э�鵥Ԫ��ֵ
 * @param	value_len: IN	Э�鵥Ԫ��ֵ�ĳ���
 * 
 * @return	0			�ɹ�
 * @return	-1			ʧ�ܣ��ڴ������Ѵ���ͬ����Э�鵥Ԫ
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_push_back_proto_unit( HJC_PACKET_MANAGER hpm,
							   int type,
							   const char* name,
							   const char* value,
							   ACE_UINT32 value_len );

/**
 * @brief ͨ��name��ȡ�����е�Э�鵥Ԫ
 *
 * ������ǵ�ϵͳӦ�ã�Э�鵥Ԫ��ƥ��ʱ�����Ǵ�Сд�����еģ���ʵ��������
 * ��<key_>�ѱ���Сд�����֣�����֯hash_mapʱ����<key_>Ϊ��ֵ��������ʱ��
 * �뽫�����ߴ���������ȱ�ΪСдȻ����ܽ��У�ͬʱ���ǵ������߼�������ͨ
 * ��ֱ��ָ���ľ�̬�ַ�������ʽ���е���! ����Ϊ�����Ч�ʣ���ǰ�汾��û��
 * ������ʱ�Ե������ṩ�����ֽ���Сдת�������ǰ����Ҫ����Ϊ��������! ��
 * �����뱣֤ʹ��ȫСд�����ֽ��е���!!!!!!
 *
 * @param	hpm: IN				ָ�����в�����Э�鱨�Ĺ��������
 * @param	type: IN			ָ��Ҫ��ȡ��Э�鵥Ԫ������
 * @param	name: IN			ָ��Ҫ��ȡ��Э�鵥Ԫ������
 * @param	value_buf: IN		���ڴ��Э�鵥Ԫ���ݵĻ��壬�������һ����
 *								ֵ������ý���ʱͨ��value_len����Э�鵥Ԫ��
 *								�ݵ�ʵ�ʳ���
 * @param	value_len: INOUT	����ʱ����ָ��value_buf�����ʵ����������
 *								�ý���ʱ�������ݵ�ʵ�ʳ���
 *
 * @return	>0	��ȡЭ�鵥Ԫ�ɹ�
 * @return	=0	�������ṩ�Ļ���������������ѯʵ�ʳ��ȷ���
 * @return	-1	��ȡЭ�鵥Ԫʧ�ܣ��ڱ����в����ڴ����ֵ�Э�鵥Ԫ
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_get_proto_unit( HJC_PACKET_MANAGER hpm,
						 const char *name,
						 char* value_buf,
						 ACE_UINT32 &value_len );

/**
 * @brief ������ַ���ģʽ�ı���
 *
 * ��ָ��˳������Э�鵥Ԫ������ַ���ģʽ�ı��ģ����޸��˶�����κ�Э�鵥
 * Ԫ��������µ�������ӿڲ��ܻ�ñ䶯�����ַ���ģʽ����
 *
 * @param	hpm: IN				ָ�����в�����Э�鱨�Ĺ��������
 * @param	packet_buf: IN		���ڴ���ַ���ģʽ���ĵĻ��壬�������һ����
 *								ֵ������ý���ʱͨ��buf_len���ر������ݵ�ʵ��
 *								����
 * @param	buf_len: INOUT		����ʱ����ָ��packet_buf�����ʵ������������
 *								����ʱ�������ݵ�ʵ�ʳ���
 *
 * @return	>0	����ɹ�
 * @return	=0	����������ѯ�������ʱͨ��buf_len����ʵ����Ҫ�Ļ���������
 * @return	-1	���ʧ�ܣ��ڴ����
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_compose( HJC_PACKET_MANAGER hpm,
				  char* packet_buf,
				  ACE_UINT32 &buf_len );

/**
 * @brief ����Э�鱨�Ĺ�����
 *
 * ʹ��hjcpm_create_pm��������Э�鱨�Ĺ�����ʹ����������ô˷�������
 *
 * @param	hpm: IN		ָ����Ҫ���ٵ�Э�鱨�Ĵ�����
 *
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
void hjcpm_destroy_pm( HJC_PACKET_MANAGER hpm );


///////////////////////////////////////////////////////////////////

// ֧�ֵ�ͨѶ�ӿ�����
#define HJC_EI_TCP			1		// ��ͨtcp����  Ĭ��7611Э��, �ϰ汾�Ŀ���֧��,�����޸�
#define HJC_EI_SSL			2		// ssl����		Ĭ��7611Э��, �ϰ汾�Ŀ���֧��,�����޸�
#define HJC_EI_TCP_7611		3		// ��ͨtcp����, ʹ��7611ͨѶЭ��
#define HJC_EI_TCP_7612		4		// ��ͨtcp����, ʹ��7612ͨѶЭ��
#define HJC_EI_SSL_7611		5		// SSL����, ʹ��7611ͨѶЭ��
#define HJC_EI_SSL_7612		6		// SSL����, ʹ��7612ͨѶЭ��
#define HJC_EI_HQ		7

/**
 * @brief ����ͨѶ�ӿ�
 *
 * @param type:			IN	ͨѶ�ӿ�����
 * @param host:			IN	��������ַ
 * @param port:			IN	�������˿�
 * @param catye:		IN	ͨѶ�ӿ�����ΪHJC_EI_SSLʱ��ʹ�õĿͻ���֤���ca���ͣ����Ϊ0������Ҫʱ���û�ѡ��
 * @param cert:			IN	ͨѶ�ӿ�����ΪHJC_EI_SSLʱ����Ҫʹ�õĿͻ���֤���ļ������Ϊ0������Ҫʱ���û�ѡ��
 * @param key:			IN	ͨѶ�ӿ�����ΪHJC_EI_SSLʱ����Ҫʹ�õĿͻ���˽Կ�ļ������Ϊ0������Ҫʱ���û�ѡ��
 * @param pwd:			IN	ͨѶ�ӿ�����ΪHJC_EI_SSLʱ����Ҫʹ�õ�˽Կ�ļ����룬���Ϊ0������Ҫʱ���û�ѡ��
 * @param checkca		IN	�Ƿ�һ��ҪУ��֤�����ȷ�� , ���Ϊ0��, ����У�鱾��֤���ļ�����ȷ��, ��0��ҪУ��
 * @param certserial	IN  ֤�����к�, ֤���Ψһ��ʶ
 *
 * @return INVALID_HHHANDLE		����ʧ��
 * @return !=INVALID_HHHANDLE	�����ɹ�������ͨѶ�ӿڵľ��
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

//#define HJC_PROXY_TYPE_NONE		0		// ��ʹ�ô���
//#define HJC_PROXY_TYPE_SOCKS4	1		// SOCKS4����
//#define HJC_PROXY_TYPE_SOCKS5	2		// SOCKS5��������֧���û���/����
//#define HJC_PROXY_TYPE_HTTP11	3		// HTTP�����Ӵ�������֧���û���/����
#define HJC_PROXY_TYPE_SOCKS4		0					//	ʹ��SOCKS4����Э��
#define HJC_PROXY_TYPE_HTTP11		1					//	HTTP�����Ӵ�������֧���û���/����
#define HJC_PROXY_TYPE_SOCKS5		2					//	ʹ��SOCKS5����Э��
//#define HJC_PROXY_TYPE_HTTPGET		3					//	ʹ��HTTP<Get>����Э��<binary>
//#define HJC_PROXY_TYPE_HTTPGETText	4					//	ʹ��HTTP<Get>����Э��<text>
//#define HJC_PROXY_TYPE_SOCKS4A      5                   //  ʹ��SOCKS4A����Э��
#define HJC_PROXY_TYPE_NONE			0xffffffff			//  ��ʹ�ô���

/**
 * @brief ���ô����������Ϣ
 *
 * @param	hcei: IN	hjcei_create_ei�����ɹ�������ͨѶ�ӿ�
 * @param	type: IN	�������������
 * @param	host: IN	�����������ַ
 * @param	port: IN	����������˿�
 * @param	name: IN	�����������Ҫ���û���
 * @param	pwd: IN		�����������Ҫ���û�����
 * @param	ul: IN		����������Ƿ���֤�û�������
 *
 * @return	<0	ʧ��
 * @return	=0	�ɹ�
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
 * @brief ���ӷ�����
 *
 * @param	hcei: IN		hjcei_create_ei�����ɹ�������ͨѶ�ӿ�
 * @param	timeout: IN		���ӳ�ʱʱ�䣬��λΪ�룬0��ʾһֱ�ȴ���ϵͳ����
 *
 * @return <0	����ʧ��
 * @return =0	���ӳɹ�
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_connect( HJC_ENV_INTERFACE hcei,
				  ACE_UINT32 timeout );

/**
 * @brief ��ȡ�������ĵı��Ĺ�����
 *
 * @param	hcei: IN		hjcei_create_ei�����ɹ�������ͨѶ�ӿ�
 * @param	hpm: OUT		������INVALID_HHHANDLE����Ϊ�ѽ������ı��Ĺ�����������û����븺������
 * @param	timeout: IN		û����������ʱ�ȴ���ʱ�䣬 <0 �����ڵȴ���=0 ���Ϸ��أ�>0 �ȴ�ʱ������λΪ���룩
 *
 * @return	<0	�ڴ������ϳ��ִ���
 * @return	=0	û����������
 * @return	>0	�ѽ�������������
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_get_mesg( HJC_ENV_INTERFACE hcei,
				   HJC_PACKET_MANAGER &hpm,
				   ACE_UINT32 timeout );

/**
 * @brief ��ȡ������ģʽ����
 *
 * @param	hcei: IN		hjcei_create_ei�����ɹ�������ͨѶ�ӿ�
 * @param	buf: IN			�������ṩ�Ĵ�����ݵĻ����������Ϊ0��ʾֻȡ���ĳ��ȵ�buf_len�ֶ�
 * @param	buf_len: INOUT	����ʱ����ָ��buf�ĳ��ȣ�����ʱ��ʾbuf����Ч���ݵĳ���
 * @param	timeout: IN		�����ݿɶ�ʱ�ȴ���ʱ�䣬 <0 �����ڵȴ���=0 ���Ϸ��أ�>0 �ȴ�ʱ������λΪ���룩
 *
 * @return	<0	�ڴ������ϳ��ִ���
 * @return	=0	û���������ģ���ʱ�û������ж�buf_len�ֶΣ����>0����ʾ�Ѿ������������ĵ����û������buf
 *				����Ϊ0����ָ����buf��Ч����(buf_len�Ĵ���ֵ)����
 * @return	>0	�ѽ������������ģ���ʱbuf_lenָ������Ч���ݵĳ���
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_read_mesg( HJC_ENV_INTERFACE hcei,
					char *buf,
					ACE_UINT32 &buf_len,
					ACE_UINT32 timeout );

/// ��ѯĿǰ�����˶��ٸ���������
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_get_mesg_count( HJC_ENV_INTERFACE hcei );

/**
 * @brief ��ȡ����
 *
 * ע�⣬���ʹ����read���������ƻ��˵ײ���ػ�����ʩ���Ժ��ڴ�heci��ֻ�ܱ�֤�û�������ͨ��ģʽ��read
 * �������ã�������Э����صķ���������: hjcei_get_mesg, hjcei_read_mesg�����ý�����֤��ȷ�ԡ�
 *
 * @param	hcei: IN		hjcei_create_ei�����ɹ�������ͨѶ�ӿ�
 * @param	buf: IN			�������ṩ�Ĵ�����ݵĻ�����
 * @param	buf_len: INOUT	����ʱ����ָ��buf�ĳ��ȣ�����ʱ��ʾbuf����Ч���ݵĳ���
 * @param	timeout: IN		�����ݿɶ�ʱ�ȴ���ʱ�䣬 <0 �����ڵȴ���=0 ���Ϸ��أ�>0 �ȴ�ʱ������λΪ���룩
 *
 * @return	<0	�ڴ������ϳ��ִ���
 * @return	=0	û������
 * @return	>0	��������Ч���ݵĳ���
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_read( HJC_ENV_INTERFACE heci,
			   char *buf,
			   ACE_UINT32 &buf_len,
			   ACE_UINT32 timeout );

/**
 * @brief ������������
 *
 * @param	hcei: IN	hjcei_create_ei�����ɹ�������ͨѶ�ӿ�
 * @param	hpm: IN		�����������ĵĹ��������
 * @param	timeout: IN	ָ���ɹ����͵ȴ��ĳ�ʱʱ�䣨��λΪ���룩
 *
 * @return	<=0	���ķ��ͳ������������߷��ͳ�ʱ
 * @return	>0	�������ķ��ͳɹ�
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_put_mesg( HJC_ENV_INTERFACE hcei,
				   HJC_PACKET_MANAGER hpm,
				   ACE_UINT32 timeout );

/**
 * @brief ����������������
 *
 * @param	hcei: IN		hjcei_create_ei�����ɹ�������ͨѶ�ӿ�
 * @param	buf: IN			���������ݵĻ�����
 * @param	buf_len: IN		ָ��buf����Ч���ݵĳ���
 * @param	timeout: IN		ָ���ɹ����͵ȴ��ĳ�ʱʱ�䣨��λΪ���룩
 *
 * @return	<=0	���ķ��ͳ���������󡢶Զ˹رջ��߷��ͳ�ʱ
 * @return	>0	���ݷ��ͳɹ�
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_write_mesg( HJC_ENV_INTERFACE hcei,
				char *buf,
				ACE_UINT32 buf_len,
				ACE_UINT32 timeout );

/**
 * @brief ������ģʽ����
 *
 * @param	hcei: IN		hjcei_create_ei�����ɹ�������ͨѶ�ӿ�
 * @param	buf: IN			���������ݵĻ�����
 * @param	buf_len: IN		ָ��buf����Ч���ݵĳ���
 * @param	timeout: IN		ָ���ɹ����͵ȴ��ĳ�ʱʱ�䣨��λΪ���룩
 *
 * @return	<0	���ķ��ͳ������������߶Զ˹ر�
 * @return	>=0	���ݳɹ����͵��ֽ���
 */
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcei_write( HJC_ENV_INTERFACE hcei,
				char *buf,
				ACE_UINT32 buf_len,
				ACE_UINT32 timeout );

/// ����ͨѶ�ӿ�
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
void hjcei_destroy_ei( HJC_ENV_INTERFACE hcei );

//��������ǩ��ɢ�м�������
#define  HJC_EVP_MD5	1
#define  HJC_EVP_SHA1	2


///��������ǩ��
/**
 * @brief ��������ǩ��
 *
 * @brief	type: IN	���ķֽ⺯��������,Ŀǰ�����ֿ���ѡ��:MD5, sha1
 * @brief	key : IN	��Կ�ļ�����
 * @brief	userdata: IN �û��ṩ�Ĺ����ķֽ⺯����ѧҪ�ֽ��ֵ,�������κ�ֵ,��Ϊ������MDF��Ψһ��
 * @brief	datalen: IN	 �û��ṩ���ݵĳ���
 * @brief   md	: IN	��������ǩ������Ҫ���������, �Ƽ�����Ϊ65, Ŀǰ����������ǩ���̶Ⱦ�Ϊ64
 * @return  <0	��������ǩ�����̳���
 * @return  >0	�����ɹ�����������ǩ���ĳ���
*/
extern "C" HsJyComm_Export
int hjcei_create_digitalsign(int type, const char *key,const char *userdata,unsigned int datalen, unsigned char *md, const char *passwd);

///��֤����ǩ��
/**
* @brief ��֤����ǩ��
*
* @brief	type: IN	���ķֽ⺯��������,Ŀǰ�����ֿ���ѡ��:MD5, sha1
* @brief	cert: IN	֤���ļ�����
* @brief	userdata: IN �û��ṩ�Ĺ����ķֽ⺯����ѧҪ�ֽ��ֵ
* @brief	mdlen: IN	����ǩ���ĳ���
* @brief   md	: IN	����ǩ�����������
* @return  <0	��֤ʧ��
* @return  =0	��֤�ɹ�
*/
extern "C" HsJyComm_Export
int hjcei_verify_digitalsign(int type, const char *cert, const char *userdata,unsigned int datalen,unsigned int mdlen,unsigned char *md );

///��������֤������
/**
* @brief ��������֤������
*
* @brief	pPass		: IN	��Կ�ļ�����Կ, ��ʽ: pass:888888, ����888888����Կ,���޸�
* @brief	pCsrout		: IN	�����ļ����·��, ��:e:\hs_cer.pem
* @brief	pKeyout		: IN	��Կ�ļ����·��, ��:e:\hs_key.pem
* @brief	pConfigfile	: IN	�����ļ�·��
* @return  =0	���ɳɹ�
* @return  !=0	����ʧ��
*/
extern "C" HsJyComm_Export
int  HS_CreateCsr(char *pPass,char *pCsrout,char *pKeyout,char *pConfigfile);


///
/**
* @brief 
*
* @brief	pP7			: IN	pP7�ļ�, 
* @brief	pPem		: IN	ת����pPem�ļ���Ϣ
* @return  =1	���ɳɹ�
* @return  =0	����ʧ��
*/
extern "C" HsJyComm_Export
int  HS_P7_PEM(char *pP7,char *pPem);

///
/**
* @brief 
*
* @brief	pP7			: IN	pP7�ļ�, 
* @brief	pPem		: IN	ת����pPem�ļ���Ϣ
* @return  =1	���ɳɹ�
* @return  =0	����ʧ��
*/
extern "C" HsJyComm_Export
int  HS_PEM_P12(char *pPem,char *pPkey,char *pPassin,char *pPassout,char *pP12);

/**
* @brief �޸�֤������(�����usb��͸�usb��pin����)
*
* @brief	type			: IN	"1": �ļ�����, "usb" : usbKEY֤�� 
* @brief	key				: IN	������ļ�֤��,����֤����·��, �����usb֤������֤�����к� 
* @brief	oldpasswd		: IN	������
* @brief	newpasswd		: IN	������
* @return  =0	���ɳɹ�
* @return  =-1	����ʧ��
*/
extern "C" HsJyComm_Export
int  hjcei_ChPasswd(char *type, char *key, char *oldpasswd, char *newpasswd, char *p11);


#define HS_DES_ENCRYPT	1	//����
#define HS_DES_DECRYPT	0	//����
/// ʹ��DES�ӽ�������
/**
* @brief ʹ��DES�ӽ�������
*
* @brief	type	�ӽ�������:1 ����, 0 ����
* @brief	In   	��Ҫ��/���ܵ�����
* @brief	nInLen  In�����ݳ���
* @brief	Out 	��/���ܺ�����, ��Out=NULLʱ�� nOutLen���ؼӽ����Ի���������
* @brief    nOutLen ��/���ܺ����ݳ���,���ĳ���Ӧ����nKeyLen��������������nInLen, ���ؼ��ܺ����ݳ���
* @return  <0	����ʧ��
* @return  =0	���ܳɹ�
* @return  =1   ��ʾ���Out�Ŀռ䲻��, nOutLen������Ҫ�ĳ���
*/
extern "C" HsJyComm_Export
int hjcei_DES_crypt(int type,  char *In, int nInLen, char *Out, int &nOutLen);

#define FORMAT_CTCA     1
#define FORMAT_SHECA    2
#define FORMAT_CWCA     3
#define FORMAT_HSCA     4			//����CA
#define FORMAT_ZJCA     5
#define	FORMAT_P7_PEM	6			//p7��ʽת����PEM
#define	FORMAT_PEM_P12	7			//PEM�ļ�ת����p7��ʽ
/**
* @brief ֤���ʽ��ʽ��
*
* @brief	pPem			���ʽ����֤���ļ�����
* @brief	nPemLen			֤�����ݳ���
* @brief	pOutPath   		��ʽ����֤��·���Լ��ļ��� , ��e:\p.pem
* @brief	nPemType		��ʽ������
* @brief	pPkey 			֤���Ӧ����Կ�ļ�
* @brief    pPassin			��Կ�ļ���Ӧ����Կ
* @brief    pPassin			��ʽ��p12��Ӧ���ļ�����Կ
* @return  <0				��ʽ��ʧ��
* @return  =0				��ʽ���ɹ�
*/
extern "C" HsJyComm_Export
int	hjcei_Format_Cert(char *pPem, char *pOutPath, int nPemType,char *pPkey,char *pPassin,char *pPassout);

/**
*	@brief	��ѯ֤��ʣ���������
*	
*	@brief	hcei			���Ӿ��
*
*	@return	>= 0			��ѯ�ɹ�, ����ʣ������
*	@return < 0				��ѯʧ��
*/



extern "C" HsJyComm_Export
int HS_SSL_GetValidDays(HJC_ENV_INTERFACE hcei);


/**
*	@brief	֤�鵼��/��������
*	@brief	type			���ܱ�ʶ: 0 ����  1 ����
*	@brief	pSurPath		Դ�ļ����·��
*   @brief	pDesPath		Ŀ���ļ����·��
*	@brief	usepasswd		�Ƿ�ʹ��Ĭ����Կ����:	1 ���� 0 ������
*
*	@return		0	   �ɹ�
	@return     <0     ʧ��
*/

#define		CERT_OUTPORT		0	//����
#define		CERT_INPORT			1	//����

extern "C" HsJyComm_Export
int hjcei_cert_inport(int type, char *pSurPath, char *pDesPath);


/**
*	@brief	֤�鵼��/������������
*	@brief	type				���ܱ�ʶ: 0 ����  1 ����
*	@brief	pCertSurPath		Դ֤���ļ����·��
*   @brief	pCertDesPath		Ŀ��֤���ļ����·��
*	@brief	pKeySurPath			Դ��Կ�ļ����·��
*   @brief	pKeyDesPath			Ŀ����Կ�ļ�·��
*	@brief	passwd				��Կ�ļ�������:	��"111111"
*
*	@return		0	   �ɹ�
	@return     <0     ʧ��
*/

extern "C" HsJyComm_Export
int hjcei_certkey_inport(int type, char *pCertSurPath, char *pCertDesPath, char *pKeySurPath, char *pKeyDesPath,  char *passwd);


/**
*	@brief	��ѯ֤�����к�
*	
*	@brief	hcei			���Ӿ��
*	@brief	cert_no			�������кŻ�����
*	@brief	nolen			����������
*	@return	>= 0			��ѯ�ɹ�
*	@return < 0				��ѯʧ��
*/


extern "C" HsJyComm_Export
int hjcei_get_certno(HJC_ENV_INTERFACE hcei, char *cert_no, int nolen);


/**
*	@brief	��ȡusb֤������
*	
*	@brief	pCertContext		����֤����Ϣ�ṹ��
*	@brief	nCertNum			����ṹ�建������С
*	@return > 0					����ʵ��֤�����
*	@return <0					��ȡʧ��
*/

extern class STCertContext;

extern "C" HsJyComm_Export
int hjcei_get_cert_context(STCertContext *pCertContext, int nCertNum, char *p11);



/**
*	@brief	��ʾ֤������
*	
*	@brief	type				֤������: ��1��: �ļ�֤�� "usb": usb֤��
*	@brief	cert				֤��·���������к�
*	@brief	pCertContext		֤�����ݻ�����
*	@return = 0					��ȡ�ɹ�
*	@return < 0					��ȡʧ��
*/
extern "C" HsJyComm_Export
int hjcei_get_cert(const char *type,const char *cert,STCertContext *pCertContext , char *p11);

/**********************************************************************
�������ƣ�HS_Sign_ZXData
�������ܣ�ǩ��
������̣�

1.ѭ����ȡԭ���ݼ��ܺ󱣴浽����������С�
����˵����
nAlg_ID:				[IN]  int 		ǩ������ detached��PKCS7ǩ����ʽ��1����detached���ͣ�0����attached���ͣ�
pDataIn:				[IN]  char[]	��ǩ������
nDataLen:				[IN]  int		���ݳ���
pDataOut:				[OUT] int		ǩ�������������
nDataOutLen:			[OUT] int		ǩ�������ĳ���
����ֵ���ɹ�����0,���򷵻�-1,	nDataOutLen����ǩ������
��ע˵����
	������pDataOut=NULL��pDataOutLen=0ʱ,nDataOutLen������Ҫ����ǩ������������
	Detached���ͣ������͵�P7���в�����ǩ��ԭ�ģ���ǩ��������֤ʱ��������ǩ��ԭ����������
	Attached���ͣ������͵�P7���а���ǩ��ԭ�ģ���ǩ��������֤ʱ����Ҫ����ǩ��ԭ��������

**********************************************************************/
extern "C" HsJyComm_Export
int	HS_Sign_ZXData(int  nAlgID, const char *certno, const char *pDataIn, int nDataInLen, char *pDataOut, int &nDataOutLen);

/**********************************************************************
�������ƣ�HS_P12_Dispatch
�������ܣ�P12֤��ֲ����
������̣�

����˵����
type:					[IN]  int 			�ֲ�����: 0: ��� 1: �ϲ�
pfx:					[IN]  char[]		pfx�ļ�·��
cert:					[IN]  char[]		֤��·��
key:					[IN]  char[]		˽Կ·��
passin:					[IN]  char[]		��������: ��: 111111
passout:				[IN]  char[]		�������: ��: 222222
����ֵ���ɹ�����0,���򷵻�-1
��ע˵����
	
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
*	@brief USBKEY����ǩ��
*
*
* @param	nType:			IN  int  ǩ������ 0:����֤ȯǩ��  1:�����׼�ӿ� 2:����Ľ���ǩ�� 
* @param   p11				IN  char[]	�����������ƣ�����ǩ����ʽ���Բ�������������
* @param	certno:			IN  int  ֤��ţ� ���������Ľ����ģ����Բ���
* @param   pPasswd:		IN	char[] USB����
* @param	pDataIn:		IN  char[] ��ǩ������
* @param	nDataLen:		IN  int ���ݳ���
* @param	pDataOut:		OUT int ǩ�������������
* @param	nDataOutLen:	OUT int ǩ�������ĳ���
*
* @return �ɹ�����0, nDataOutLen����ǩ������
* @return ���򷵻�-1
*
*/
extern "C" HsJyComm_Export
int HS_USB_SignData(int  nType,const char *p11, const char *certno, const char *pPasswd,const char *pDataIn, int nDataInLen, char *pDataOut, int &nDataOutLen);



/**
*	@brief	��ȡusb֤������
*	
*	@param  nType				1: �����׼�ӿ�
*	@pram   p11					�����ӿ�����
*	@param	pCertContext		����֤����Ϣ�ṹ��
*	@param	nCertNum			����ṹ�建������С
*	@return > 0					����ʵ��֤�����
*	@return <0					��ȡʧ��
*/
extern "C" HsJyComm_Export
int HS_USB_GetCertContext(int nType ,const char *p11,STCertContext *pCertContext, int nCertNum);





/**
* @brief �޸�֤������(�����usb��͸�usb��pin����)
*
* @param	type			: IN	1: �����׼�ӿ� 2: �ļ�֤�� 
* @param    p11				: IN    �������ƣ������ļ�֤��ɲ���
* @param	key				: IN	������ļ�֤��,����֤����·��, �����usb֤������֤�����к� 
* @param	oldpasswd		: IN	������
* @param	newpasswd		: IN	������
* @return  =0	���ɳɹ�
* @return  =-1	����ʧ��
*/
extern "C" HsJyComm_Export
int  HS_USB_ChangePasswd(int nType, char *p11,char *key, char *oldpasswd, char *newpasswd);


int  HS_USB_ChangePasswd(int nType, char *p11,char *pKey, char *poldpasswd, char *pnewpasswd);

/**
* @brief �޸�֤������(�����usb��͸�usb��pin����)
*
* @param	type			: IN	1: �����׼�ӿ� 2: �ļ�֤�� 
* @param    p11				: IN    �������ƣ������ļ�֤��ɲ���
* @param	pCertNo			: IN	֤�����к� 

* @return  > 0	������Ч����
* @return  <0	��ȡʧ��
*/
extern "C" HsJyComm_Export
int HS_USB_GetValidDays(int nType, const char *p11, const char *pCertNo);



extern "C" HsJyComm_Export
int HS_USB_GetKeyStatus(int nType, const char *p11);

extern "C" HsJyComm_Export
int wangj_test(int nType, const char *pBuf, int nInBufLen, char *pBufOut, int &nOutLen);



/**
* @brief ���в�����¼�������ݰ�
*
* @param	pType:IN			��������:  0:���е�½ǩ��
* @param	pInput: IN			���������������
* @param	pOutBuf: IN			������ݻ�����
* @param	pOutBufLen: IN		�������������
*
* @return	<0	��������-1 ��ʾ�û�ȡ����-2 ��ʾ���BUFFER���Ȳ���������Ϊ����
* @return	=0	�ɹ�
*/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int HS_CreateSSOPkg(int pType,const char * pInput, char * pOutBuf, int &pOutBufLen);

/**
* @brief ������֤��¼����
*
* @param	pSSORes: IN			������֤��������
* @param	pType:IN			�������� : 0 ������֤ǩ��
*
* @return	<0	����
* @return	=0	�ɹ�
*/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int HS_VerifySSOToken(int pType,const char * pSSORes);

/**
* @brief ���в���ǩ������
*
* @param	pInput: IN			���������������
* @param	pOutBuf: IN			������ݻ�����
* @param	pOutBufLen: IN		�������������
*
* @return	<0	��������-1 ��ʾ�û�ȡ����-2 ��ʾ���BUFFER���Ȳ���������Ϊ����
* @return	=0	�ɹ�
*/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int HS_CreateCMBSSOTokenSign(const char* pInput, char* pOutBuf, int* pOutBufLen);


/**
* @brief ��ȡ��ǰ���ӵ�mac��ip��ַ
*
* @param    hcei			: IN    ei
* @param	nType			: IN	2: mac  
* @param    pBuf			: OUT   ���������
* @param    nBudLen			: OUT   ����������

* @return  > 1	: ��ȡ�ɹ�
* @return  = 0  : ���뻺��������̫С��nBufLen������Ҫ�ĳ���
* @return  < 0  : ��ȡʧ��
*/
extern "C" HsJyComm_Export
int HS_GetConnInfo(HJC_ENV_INTERFACE hcei,int nType, char *pBuf, int &nBufLen);



/**
* @breif ��ȡЭ���ֶθ���
*
* @param	hpm: IN		ָ�����в�����Э�鱨�Ĺ��������
*
* @return	>=  0	�ɹ�,���ؾ���ضθ���
* @return	< 0	    ʧ��
**/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_get_nv_size( HJC_PACKET_MANAGER hpm);

/**
* @brief ͨ��������ȡ�����е�Э�鵥Ԫ
*	NV_UNIT			= 1,		// A=content\n
*	HE_UNIT			= 2,		// <A>{len}content
*	BIN_UNIT		= 3,		// [A]{len}content
*	RAW_UNIT		= 4,		// ��¼ԭʼ���ַ���ģʽ��Э�鵥Ԫ
*
* @param	hpm: IN				ָ�����в�����Э�鱨�Ĺ��������
* @param    idx:IN				�ֶ�����ֵ
* @param	name: IN			ָ��Ҫ��ȡ��Э�鵥Ԫ������
* @param	value_buf: IN		���ڴ��Э�鵥Ԫ���ݵĻ��壬���Ȳ���ʱ����ý���ʱͨ��value_len����Э�鵥Ԫ���ݵ�ʵ�ʳ���
* @param	value_len: INOUT	����ʱ����ָ��value_buf�����ʵ������
*								
* @param	val_type:OUT		�ֶ�����
*
* @return	>0	��ȡЭ�鵥Ԫ�ɹ������ֶγ���
* @return	=0	�������ṩ�Ļ���������������ѯʵ�ʳ��ȷ���
* @return	-1	��ȡЭ�鵥Ԫʧ�ܣ��ڱ����в����ڴ����ֵ�Э�鵥Ԫ
*/
//extern "C" ACE_Svc_Export
extern "C" HsJyComm_Export
int hjcpm_get_nv_name( HJC_PACKET_MANAGER hpm,
						 int idx,
						 char* value_buf,
						 ACE_UINT32 &value_len,
						 int &val_type);

#endif /*_Hs_Jy_Comm_H*/
