///������ͨ�ýӿڶ���
/*******************************************************
  Դ��������:hs_interface.h
  �������Ȩ:���ݺ������ӹɷ����޹�˾
  ϵͳ����:  HZHSGS���ڻ�����V1.0
  ģ������:  ����ģ��
  ����˵��: ������ͨ�ýӿڶ���,��:����������������ر��ӿڶ���;
  			0�������������Ļ�������ID����
  			1��IKnown�ӿڶ���
  			2��IAppContext�ӿڶ���
  			2����̬���ػ�����������Ľӿ�(��̬����������)����
   
            
  ����ĵ�:  ��Ҫ��ƽӿڲ���
            
             
  ��    ��:  zhangly
  ��������:  20040218
  ��    ע:  linux/unixƽ̨�£����� OS_UNIX��PLATFORM_UNIX
  			 ��windows��ע�⺯�����÷�ʽ��__stdcall
  �޸���Ա��zhangly
  �޸����ڣ�20050802
  �޸�˵�������ӶԸ߼����ݷ��ʷ���ADE��ID����
  �޸���Ա��zhangly
  �޸����ڣ�20070316
  �޸�˵�����ڶ���IKnown�ӿ�ʱ������HS_IKNOWN�����ж�
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

///PFgetxxxxInfo()����I_OK��ʾ�������鵽��������
#define I_OK 1
///PFgetxxxxInfo()����I_NONE��ʾ�������鲻����������
#define I_NONE 0

///��������Ψһ��ʶ������
#define HS_SID  int
/**@name ���⹫���Ļ���������ýӿ�Ψһ��ʶ SID ���� 
 * �ر�ע�⣺��������һ�������������޸ģ������ʹ�ѷ���ϵͳʧЧ������
 */
//@{
	///���屾������AS2ϵͳ�����еķ���ID
	#define SID_CONFIG 1
	///������־�����IDΪ2
	#define SID_LOG 2
	///���ݿ����
	#define SID_HSDB 3
	///�����������
	#define SID_DATA_COMPS 4
	///�ڴ����ݿ��ѯ
	#define SID_MDB 5
	///����չ����ӿ�(ÿ����������󶼿�����һ��)
	#define SID_MANAGED 6
	///��������
	#define SID_PACK  7	
	///�ڴ���ʱ�����
	#define SID_TEMPMDB 8
	///DBF�ļ���������
	#define SID_DBF 9
	///����֤ȯ����DBF����
	#define SID_GENHQDBF 10
	///�Ự������
	#define SID_SESSION     11
	///��ҳ��ѯ������
	#define SID_PAGESQUERY  12
	///�̼߳�ʱ��
	#define SID_TIMER  13	
	///�������������
	#define SID_AS_TASK  14	
	///�������ͷ���
	#define SID_PUSH  15	
	///��ȫ����
	#define SID_SAFE  16
	///�������ͷ���	
	#define SID_SUBSCIBE	17

	///TFTP����	
	#define SID_TFTP	19
	
	///ASͨ�ŷ���
	#define SID_AS_COMM   20
	///ASϵͳ����
	#define SID_AS_SYSTEM   21	
	///AS�û�Ӧ�ù�����
	#define SID_AS_APP_MGR   22	
	///�ӷ������
	#define SID_SUBCALL     23
	///�߼����ݷ��ʷ���ADE
	#define SID_ADE 24
	///�������ڴ�����SMDB
	#define SID_SMDB 25
	///��չϵͳ���񣨿�ƽ̨���ߵȣ�
	#define SID_SYSTEM_EX 26
			
	///������־����
	#define SID_DATA_CAPTURE 73123
	
	#define SID_AR_API		100
	#define SID_AR_OSPF		101
	#define SID_AR_MAIN		102
	///150~199 ΪAR���ʹ��
///hs_deamon �����
	#define SID_HSDAEMON    200
//@}

/**@name ���⹫����ȫ������Ψһ��ʶ SGDID ���� */
//@{
	///֤��Ᵽ������
	#define SGDID_CERT_LIB_PIN     1	
//@}

#ifndef HS_IKNOWN
#define HS_IKNOWN
///��׼�ӿڲ�ѯ,����com��׼
struct IKnown
{
	///�ӿڲ�ѯ
	/**
	 *@param const int iid �ӿ�ȫ��Ψһ��ʶ��0~1024 �ɻ�����ʹ�ã�1024�������½ӿڿ�����ʹ�ã���������Ŀ�����ڲ��ӿڹ淶�б����Է���ͻ;
	 *@param void **ppv ����iid��Ӧ�Ľӿ�ָ�루�ӿڲ���com��׼��
	 *@return I_OK �ɹ���I_NONE δ�鵽iid ��Ӧ�ӿ�
	 */
    virtual unsigned long  FUNCTION_CALL_MODE QueryInterface(const HS_SID & iid, IKnown **ppv )=0;
    virtual unsigned long  FUNCTION_CALL_MODE AddRef()=0;
    virtual unsigned long  FUNCTION_CALL_MODE Release()=0;
};
#endif

///Ӧ�������Ľӿ�(ͨ�����ӿڿ��Ի�ȡ��ǰӦ�õ��ڲ�����ӿ���ȫ������)
/**����������ṩ��̬���ع���ȫ�����ڲ�����ӿڵĻ���(���������)
 * 
 *���������ڲ�ģ������һ���ڳ�ʼ��ʱ�յ�Ӧ�ÿ�ܴ���ı��ӿ�ָ�룻
 *
 *�Ӷ�ͨ�����ӿڼ̳е�IKnown�ӿ��µ�QueryInterface��������ID��ȡ�����������ӿڻ��ڲ�����ӿھ����
 *
 *Ҳ����ͨ�����ӿڣ���ȡӦ�ü�ȫ�����ݣ�
 *
 *�����������ʵ�ֱ��ӿ�ʱ����̬���غͳ�ʼ���Ļ�������ȫ�����ݵȣ�
 */
struct IAppContext:IKnown
{
	///ȡ���ӿڰ汾
	/**����������Ϊ���ӿڵĵ�һ������,���ӿڰ汾��һ�»ᵼ�²���Ԥ�ڵĽ�����Ǽ����޸�Ҫ�����±������������
	 *@return int ���ӿڰ汾yyyymmdd
	 */
    virtual int FUNCTION_CALL_MODE getVersion()=0;
    
	///ȡAR/AS����
	/**
	 *@return const char * ����AR/AS��������
	 */
    virtual const char * FUNCTION_CALL_MODE getName()=0;	

	///ȡ���
	/**
	 *@return int ����AR/AS�������ڱ��
	 */
    virtual int  FUNCTION_CALL_MODE getID()=0;	

	///ȡӦ������ʱ�ĳ�ʼ��ѡ��,��ѡ����ȡ����ѡ��ֵ
	/**
	 *@param const char * szParamName ��������������ѡ����Ϊ��׼�����ַ���
	 *@param const char * szDefaultValue    ���ָ������û��ͨ�������л򻷾������ṩֵ���򷵻�ȱʡֵ
	 *@return const char * ����ֵ
	 */    
	virtual const char * FUNCTION_CALL_MODE getOption(const char * szParamName,const char * szDefaultValue)=0;
    
    ///����ȫ������ID������ָ��ȫ������ָ��
    /**
     *@param int iGlbDataID  Ӧ������ID
     *@return void * �ɹ�������Ӧ��������ָ�룬������NULL
     */
    virtual void *  FUNCTION_CALL_MODE getGlbData(int iGlbDataID)=0;

	///����ȫ������ID������ָ��ȫ������ָ��
    /**
     *@param int iGlbDataID  ȫ������ID
     *@param const void * pData      ����ָ��(����ΪNULL)
     *@return int �ɹ�����I_OK��������I_NONE
     */
    virtual int  FUNCTION_CALL_MODE setGlbData(int iGlbDataID,void * pData)=0;

	///ȡ���õİ�ȫ�ȼ�
	/**
	 *@return int ��ȫ�ȼ�(0����У�����ǩ����1 У�����ǩ��)
	 */
    virtual int FUNCTION_CALL_MODE getSafeLevel()=0;
    
};

struct tagBaseServiceInfo;

//�������������������������ָ��(��̬����ʱʹ��)������PFgetxxxxInfo�Ǳ���ġ�
///���ع���ʵ������ָ��,���ڳ�ʼ�����̲����������������������ڴ���ʱ��ʼ����
typedef IKnown * (FUNCTION_CALL_MODE * PFgetxxxxInstance ) (IAppContext * );
///��ʼ������ָ�루�������л����������̬���غ�ŵ��ã���������������ȡAS/AR����ڲ���������Ҫ�ڹ���ӿ���Ӧϵͳ�¼�MSG_ONSTART��
typedef int (FUNCTION_CALL_MODE * PFinitxxxx ) (IAppContext * );
///�������������Ϣ��ѯ�������ߴ�iIndex=0��ʼ�����ѯ��ֱ������I_NONEΪֹ;
typedef int (FUNCTION_CALL_MODE * PFgetxxxxInfo ) (int iIndex, tagBaseServiceInfo * ppv );

///�������������Ϣ(��̬����ʱʹ��)
struct tagBaseServiceInfo
{
	///��������ID
	int iServiceID;
	///������������Ϣyyyymmdd
	int iVersion;
	///�������������ʼ������ָ��
    PFinitxxxx  xxxxInit;
	///��ȡ���������������ʵ������ָ��
    PFgetxxxxInstance  getxxxxInstance;
	///�����������ʼ��ʱ��������������������ID(��������������ȳ�ʼ����)
	int iDependServices;
	///����������Ϣ
	const char * Caption;
};

#endif
