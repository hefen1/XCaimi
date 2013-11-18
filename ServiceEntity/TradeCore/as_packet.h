/*******************************************************
  Դ��������:as_packet.h
  �������Ȩ:���ݺ������ӹɷ����޹�˾
  ϵͳ����:  HZHSGS���ڻ�����V1.0
  ģ������:  ASģ��
  ����˵��:  �������������
             1��IPackService ��������ӿ�
             2��IPacker ����ӿ�
			 3��TResultSet ����ӿ�(��as2_ResultSet.h����)
             
           
  ����ĵ�:  ��Ҫ��ƽӿڲ���
             
  ��    ��:  zhangly
  ��������:  20040223
  ��    ע:  ƽ̨�޹أ�֧�ֶ��ְ���ʽ
  �޸���Ա��
  �޸����ڣ�
  �޸�˵����
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

///����������ӿ� 
struct IPackService : IKnown
{
	///����ҵ�����ʽ�汾
	/**����ҵ������ַ��ж�ҵ�����ʽ�汾,ҵ����汾ʶ�����:
	 *
     *  V1���ײ��ŵ����ַ����͵����������Ե�һ���ֽ�>=0x30;
     *
     *  V2���һ���ֽڷŵ��ǰ汾�ţ�ĿǰֵΪ0x20�������Ը���ʱ��ֵ�������0x2F
     *
	 *@param void * lpBuffer  ҵ�������(������ָ��Ϸ�ҵ��������ֽ�)
	 *@return int  ҵ�����ʽ�汾(1: V1��,0x20~0x2F V2��汾��)
	 */
    virtual int FUNCTION_CALL_MODE GetVersion(const void * lpBuffer) = 0;
	
	///ȡһ��ҵ��������,��ô��������ͨ��FreePacker()�ͷ�;
	/**
	 *@param int iVersion ҵ�����ʽ�汾(ȡֵ:1 �ִ���,����ֵ 0x20��)
	 *@return IPacker * ������ӿ�ָ��
	 */
    virtual IPacker * FUNCTION_CALL_MODE GetPacker(int iVersion) = 0;
	///�ͷŴ����
	/**
	 *@param IPacker * Ҫ�ͷŵĴ�����ӿ�ָ��
	 */
    virtual void FUNCTION_CALL_MODE FreePacker(IPacker * lpPacker) = 0;

	///ȡһ��ҵ��������,��ý��������ͨ��FreePacker()�ͷ�;
	/**����GetVersion
	 *@param void * lpBuffer Ҫ��������ݣ�����ARͨ�Ű�ͷ��
	 *@param unsigned int iLen ���ݳ���
	 *@return IUnPacker * ����������ӿ�ָ��
	 */
    virtual IUnPacker *  FUNCTION_CALL_MODE GetUnPacker(void * lpBuffer, unsigned int iLen ) = 0;

	///�ͷŽ����
    virtual void FUNCTION_CALL_MODE FreeUnPacker(IUnPacker * lpUnPacker) = 0;
};

///����������ӿ�(һ�������ж���칹�����)
/**ִ������:
 *
 * 0��׼���� SetBuffer(),�������������ɵ������ṩ,�������BeginPack()֮ǰ׼��;
 * 1����ʼ:  BeginPack(),�������λ;
 *
 * 2����һ������������
 *
 *(a)����ֶ����б���AddField()
 *
 *(b)���ս������ά��˳�����ֶΣ�������¼������ݣ�AddValue()
 *
 * 3�����õ�һ��������ķ�����(��ѡ�����������Ϊ0��������) SetFirstRetCode()
 *
 * 4������һ�������(��ѡ) NewDataSet()���˴�ͬʱ�����˸ý�����ķ����룻
 * 
 * 5���ο���2��ʵ��һ������������
 *
 * 6��������EndPack(),�ظ����ûᵼ�¼���ս����;
 *
 * 7��ȡ������(����������������С�����ݳ���)
 *    ������Ҳ����ֱ�ӽ��UnPack()���ؽ���ӿ�
 *
 *ʹ��ע������:IPacker��ʹ�õ��ڴ滺���������ɵ����߿��ƣ�
 *             ����������ķ����룬ֻ���ڰ���ʽ�汾0x20����ʱ��Ч��
 */
struct IPacker
{
    ///�������ʼ��(ʹ�õ����ߵĻ�����)
	/** ��һ��ʹ�ô����ʱ������ʹ�ñ��������úû�����(���ݳ��ȱ���ΪiDataLen)
	 *@param  char * pBuf  ��������ַ
 	 *@param  int iBufSize  �������ռ�
 	 *@param  int iDataLen  �������ݳ��ȣ��������ݼ�����������֮��ֻ��V1.0��ʽ�İ���Ч�� 	 
 	 */
	virtual void FUNCTION_CALL_MODE SetBuffer(void * pBuf,int iBufSize,int iDataLen=0 )=0;

	///��λ�����¿�ʼ����һ����(�ֶ������¼����Ϊ0��0��)
	/**
	 * ���ܣ���ʼ������Ѱ���������(�ظ�ʹ�����еĻ������ռ�)
	 *@return ��
	 */
	virtual void FUNCTION_CALL_MODE BeginPack(void)=0;

	///��ʼ��һ�������
	/**�ڴ򵥽�����İ�ʱ�����Բ����ñ�����,��ȡĬ��ֵ
	 *@param const char *szDatasetName 0x20������Ҫָ�����������
	 *@param int iReturnCode           0x20������ҪΪÿ�������ָ������ֵ
	 */
	virtual int FUNCTION_CALL_MODE NewDataset(const char *szDatasetName, int iReturnCode = 0)=0;

	/**
	 * ���ܣ��������ֶ�
	 *
	 *��ִ�д���Ҫ��:�� NewDataset()��Reset(),SetBuffer()֮��,����ֶΰ�˳�����;
	 *
	 *@param szFieldName���ֶ���
	 *@param cFieldType ���ֶ�����:I������F��������C�ַ���S�ַ�����R�������������
	 *@param iFieldWidth ���ֶο�ȣ���ռ����ֽ�����
	 *@param iFieldScale ���ֶξ���,��cFieldType='F'ʱ��С��λ��(ȱʡΪ4λС��)
	 *@return ������ʾʧ�ܣ�����ΪĿǰ���ĳ���
	 */
	virtual int FUNCTION_CALL_MODE AddField(const char *szFieldName,char cFieldType ='S',int iFieldWidth=255,int iFieldScale=4)=0;

	/**
	 * ���ܣ��������ַ�������
     * ��ִ�д���Ҫ��:�����������ֶ�������֮��,����ֶΰ�˳�����;
	 *@param       szValue���ַ�������
	 *@return ������ʾʧ�ܣ�����ΪĿǰ���ĳ���
	 */
	virtual int FUNCTION_CALL_MODE AddValue(const char *szValue)=0;

	/**
     * ���ܣ���������������
 	 *@param       iValue����������
	 *@return ������ʾʧ�ܣ�����ΪĿǰ���ĳ���
	 */
	virtual int FUNCTION_CALL_MODE AddValue(int iValue)=0;
	
    /**
	 * ���ܣ������Ӹ�������
	 *@param       fValue����������
	 *@return ������ʾʧ�ܣ�����ΪĿǰ���ĳ���
	 */
	virtual int FUNCTION_CALL_MODE AddValue(double fValue)=0;

	/**
	 * ���ܣ�������һ���ַ�
	 *@param		 cValue���ַ�
	 *@return ������ʾʧ�ܣ�����ΪĿǰ���ĳ���
	 */
	virtual int FUNCTION_CALL_MODE AddValue(char cValue)=0;

	/**
	 * ���ܣ�������һ�������
	 *@param	void * lpBuff ������
	 *@param	int iLen  ���ݳ���	 
	 *@return ������ʾʧ�ܣ�����ΪĿǰ���ĳ���
	 */
	virtual int FUNCTION_CALL_MODE AddValue(void * lpBuff,int iLen)=0;

    ///�������
	virtual void FUNCTION_CALL_MODE EndPack()=0;
 
	/**
     * ���ܣ�ȡ������ָ��
	 *@return ������ָ��
     */
	virtual void * FUNCTION_CALL_MODE PackBuf(void) = 0;
	
	/**
     * ���ܣ�ȡ����������
     *@return ����������
	 */
	virtual int FUNCTION_CALL_MODE PackLen(void) = 0;
	
	/**
	 * ���ܣ�ȡ��������������С
     *@return ��������������С
	 */
	virtual int FUNCTION_CALL_MODE PackBufSize(void) = 0;
	
	/**
	 * ���ܣ�ȡ�����ʽ�汾
     *@return �汾
	 */
	virtual int FUNCTION_CALL_MODE getVersion(void) = 0;
	
	///���ý�����ķ�����(0x20������Ҫ��)������������Ҫ����
	/**������ȡȱʡֵ0�������ã�������ã��������EndPack()֮ǰ����
	 * ���ܣ�ȡ�����ʽ�汾
     *@return �汾
	 */
	virtual void FUNCTION_CALL_MODE SetReturnCode(unsigned long dwRetCode) = 0;

	/**
     * ���ܣ�ֱ�ӷ��ص�ǰ�������Ľ���ӿ�,������EndPack()֮����ܵ���,�ڴ�����ͷ�ʱ��Ӧ�Ľ����ʵ��Ҳ�ͷ�
     *
     *@return ������ӿڣ��˽���ӿڲ��ܵ���destroy()���ͷ�
	 */
	virtual IUnPacker * FUNCTION_CALL_MODE UnPack(void) = 0;

};

///������ӿ�
struct IUnPacker : IResultSet
{
	/**
	 * ���ܣ�ȡ�����ʽ�汾
     *@return �汾
	 */
	virtual int FUNCTION_CALL_MODE getVersion(void) = 0;

	/**
     * ���ܣ�ȡ������ݳ���
     *@return ������ݳ���
	 */
	virtual int FUNCTION_CALL_MODE Open(void * lpBuffer,unsigned int iLen) = 0;

    ///ȡ���������(0x20���ϰ汾֧��)
    virtual int FUNCTION_CALL_MODE GetDatasetCount()=0;

    ///���õ�ǰ�����(0x20���ϰ汾֧��)
    /**
	 *@param  int nIndex				��������
	 *@return int						1��ʾ�ɹ�������Ϊʧ��
	 */
    virtual int FUNCTION_CALL_MODE SetCurrentDataset(int nIndex)=0;

    ///���õ�ǰ����� (0x20���ϰ汾֧��)
    /**
	 *@param  const char *szDatasetName	���������
	 *@return int						1 ��ʾ�ɹ�������Ϊʧ��
	 */
    virtual int FUNCTION_CALL_MODE SetCurrentDataset(const char *szDatasetName)=0;

	/**
     * ���ܣ�ȡ���������ָ��
	 *@return ������ָ��
     */
	virtual void * FUNCTION_CALL_MODE PackBuf(void) = 0;

	/**
     * ���ܣ�ȡ������ݳ���
     *@return ������ݳ���
	 */
	virtual unsigned int FUNCTION_CALL_MODE PackLen(void) = 0;
	
};

//----------��Ϊ�ɶ�̬���صĻ���������������붨��������������---------

#ifdef __cplusplus
extern "C" { 
#endif

///�����ʼ��xxxxInit()
/**
 *@return int �汾�� yyyymmdd��ʽ
 */
int  FUNCTION_CALL_MODE PackServiceInit(IAppContext * p);

///ȡ����ӿھ�� getxxxxInstance()
/**
 *@return void * ���������� 
 */
IKnown * FUNCTION_CALL_MODE getPackServiceInstance(IAppContext * p);

///����ͳһ��AS�������������ѯ�ӿ� getxxxInfo()
/**���������������̬����ʱ��ͨ�����ӿڻ��Ի�ȡ������Ϣ(����xxxxInit(),getxxxxInterface()����ָ��)
 *@param void ** ppv    �������������Ϣ(�������)��
 *@return ����0��ʾ�鵽��Ҫ�Ľӿڣ����򷵻ظ���
 */
int FUNCTION_CALL_MODE getPackServiceInfo(int iIndex, tagBaseServiceInfo * ppv );

#ifdef __cplusplus
  }
#endif
 
#endif
