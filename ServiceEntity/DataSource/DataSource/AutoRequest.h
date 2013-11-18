/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	AutoRequest.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	���������е��Զ�������
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
#include "NetEngineInterface.h"

#pragma once

class CDataSourceImpl;
class CConfigManager;

#define TEMP_DATASOURCE_ID						1					//��ʱʹ�ã����ط���ʼ����־

enum	eDataSourceAutoInitStatus			//�����������Զ���ʼ��״̬
{
	InitStatus_succ = 0,			//��ʼ�����
	InitStatus_login = 1,			//��¼
	InitStatus_config = 2,			//��������
	InitStatus_exright = 3,			//��Ȩ����
	InitStatus_currentfinancedata = 4, //��������
	InitStatus_blockdata = 5,		//�������
	InitStatus_userblockdata = 6,	//�û����
	InitStatus_init = 7,			//��ʼ���г�����Ʊ
	InitStatus_servercalc = 8,		//�����ǵ�ͣ
};

enum eDataSourceAutoConfigType
{
	ConfigType_bystatus = 1,
	ConfigType_byRequest = 2,
};

class CAutoRequest
{
protected:
	CDataSourceImpl*		m_pDataSourceImpl;
	CConfigManager*			m_pConfigManager;

	CConfigManager*			GetConfigManager();
public:
	//	���캯������������
	CAutoRequest();
	~CAutoRequest();

public:
	//���������������ָ��
	void				SetDataSourceImpl(CDataSourceImpl* pImpl);
	//��¼�ɹ�����Ӧ
	BOOL				OnLoginSuccess(Server_type svrType);
	//��ʼ������
	BOOL				ReqServerInitInfo(Server_type svrType);
	//�ļ�����
	BOOL				RequestConfFile(CString strFileName, BOOL bForceUpdate = FALSE);
	// �������������������Ȩ����
	BOOL				RequestIncrementData(long lType);
	//ת��״̬����
	CString				GetStatus(DWORD dwStatus);
	//ת��������ļ��ڵ�����
	CString				GetSectionName(int nType, DWORD dwStatus);
	//�����Ʊ�����������ǵ�ͣ����
	BOOL				AutoRequestServerCalc(char cSvrType);
	//�����������Ϣ
	BOOL				ReqServerInfo();		
};