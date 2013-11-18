/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	SubscriptionManager.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	�����б������
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-9-1
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "RequestResponceDataManager.h"

struct CDataSourceSubscription
{
	UINT					m_uAskType;					//�����ܺ�
	DWORD					m_dwPacketID;               //�����id
	CodeInfo				m_codeInfo;					//֤ȯ��Ϣ
	std::list<long>			m_listDataSinkID;			//�ⲿ����ģ������Ӧ��ID�б�
	DWORD					m_dwTickRequest;			//�ϴ���Ч����ʱ����¼��ʱ���������ݸ��ֶ���Ч��
	DWORD					m_dwTickResponce;			//�ϴ�Ӧ������Ӧ��ʱ���������ݸ��ֶ���Ч��
	UINT                    m_uResever;					//���� �̺������ �����������
};

//typedef CMap<CodeInfo*, CodeInfo*, BOOL, BOOL>CMapPreSubscrition;

enum eSubscriptionStatus		//�����б�״̬
{
	eSubscriptionStatus_unknow		= 0,			//δ֪����
	eSubscriptionStatus_new			= 0x0001,		//�µĶ���
	eSubscriptionStatus_add			= 0x0010,		//�϶��ģ����ӻص����
	eSubscriptionStatus_exist		= 0x0020,		//�Ѿ����ڣ�������
	eSubscriptionStatus_remove		= 0x0100,		//ɾ������

	eSubscriptionStatus_maskAdd		= 0x000F,		//����-��Ҫ������
	eSubscriptionStatus_maskExist	= 0x00F0,		//����-����Ҫ������
	eSubscriptionStatus_maskDel		= 0x0F00,		//����-��Ҫɾ��ԭ����
};

#define HS_SUBSCRIPTION_IGNORE_INTERVAL			1000				//���ĺ��Լ��ʱ�� - 3��

class CSubscriptionManager
{
protected:
	//�����б�ͬ���ź���
	CRITICAL_SECTION					m_critical;			
	//�����б����
	std::list<CDataSourceSubscription*>	m_listSubscription;
public:
	//	���캯������������
	CSubscriptionManager();
	~CSubscriptionManager();
protected:
	//��ѯ����״��������
//	int									CheckSubscriptionInfo(UINT uAskType, CodeInfo* pCodeInfo, long lDataSinkID);
	BOOL								FindCodeInfo(CodeInfo* pSource, CodeInfo* pCodeArray, int nArSize);
public:
	//��ձ��ض�������
	BOOL								RemoveAllSubscriptions();
	//���ָ�����ض�������
	BOOL								RemoveSubscriptions(UINT uAskType, CodeInfo* pCodeInfo, UINT uPacketID = 0);
	//��ȡԭ����
	CDataSourceSubscription*			GetSubscription(UINT uAskType, CodeInfo* pCodeInfo, UINT uPacketID = 0);
	
	//���ָ���Ķ���
//	int									ClearLastSubscription(UINT uAskType, long lDataSinkID);
	//�����������ͣ����룬�ص���ڲ����ȣ���ϳ������
	
	/********************************************************************************
	* �������� :����δ��������������ȡһ�����������������
	* �������� :CDataSourceReqestResponceData* pData - δ����������
	*			int& nAskCount - ��¼����������
	*  ����ֵ  :���������������	
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	char*								GetRequestPack(IN CDataSourceReqestResponceData* pData, IN OUT int& nAskCount);

	//ĳ��sink���Ƴ�
	BOOL								RemoveSink(long lSinkID);
	//����ĳ������Ķ�����Ϣ
	BOOL								UpdateOneSubscription(UINT uAskType, long lDataSinkID, AskData* pAsk = NULL);
};

