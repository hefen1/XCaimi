/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	RequestResponceDataManager.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	����(Ӧ��)�б������
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

#define HS_REQUEST_IGNORE_INTERVAL			1000		//������Լ��ʱ�� - 1��

enum eDataSourceRequestResponceStauts				//��������Ӧ�������״̬
{
	eDataSourceRequestResponceStauts_jypacknofiy		= 0x0001,	//��������ͬ��֪ͨ
	eDataSourceRequestResponceStauts_CommNotify         = 0x0002,   //ͨѶ֪ͨ

	eDataSourceRequestResponceStauts_notprocess			= 0x0011,	//δ����
	eDataSourceRequestResponceStauts_processing			= 0x0012,	//���ڷ���
	eDataSourceRequestResponceStauts_sended				= 0x0013,	//�ѷ��͵�������

	eDataSourceRequestResponceStauts_partreaddisk       = 0x0021,   //���ֶ�ȡ����
	eDataSourceRequestResponceStauts_allreaddisk        = 0x0022,   //ȫ����ȡ���� 
	 
	eDataSourceRequestResponceStauts_recvnotPorcess		= 0x0101,	//�ѽ���δ����
	eDataSourceRequestResponceStauts_recvPorcess		= 0x0102,	//���պ�����
	eDataSourceRequestResponceStauts_complete			= 0x0103,	//������ɲ����ظ��ͻ���

	eDataSourceRequestResponceStauts_loacal				= 0,		//���ش�������

	eDataSourceRequestResponceStauts_masksend			= 0x00F0,	//����--��������
	eDataSourceRequestResponceStauts_maskrecv			= 0x0F00,	//����--��������

};

struct CDataSourceReqestResponceData
{
	UINT		m_uStatus;						//���ݰ�����״̬
	long		m_lDataSinkID;					//�ص�
	UINT		m_uOption;						//���ݰ�����
	int			m_nTimeOut;						//��ʱʱ��
	int			m_nAskDataLen;					//����/Ӧ���ԭʼ����
	char*		m_pRequestResponceData;			//����/Ӧ�������ָ��
	CDataSourceReqestResponceData()
	{
		memset(this, 0, sizeof(CDataSourceReqestResponceData));
	}
};
