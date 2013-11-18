/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	performance.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	�����������ܲ������ݶ���
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-10-18
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/

#pragma once

#ifdef _SUPPORT_PERFORMANCE_TEST		//֧�����ܲ���

#define IDUM_INIT_HANDLE		1			//��ʼ�����ھ��

#define PERFORMANCE_DATATRANSFER_CHECK		0x68756e64			//"hund(sun)ǰ�ĸ���ĸASCII��ֵ"

enum ePerformanceInfoType
{
	epitype_thread			= 0x01,				//�߳���Ϣ
	epitype_function		= 0x02,				//���̣���������Ϣ
};

enum ePerformanceDrawType
{
	epdtype_histogram3D		= 0,				//��άֱ��ͼ
	epdtype_histogram		= 1,				//��άֱ��ͼ	
	epdtype_linegraph3D		= 2,				//��ά����ͼ
	epdtype_linegraph		= 3,				//��ά����ͼ
	epdtype_areagraph3D		= 4,				//��ά���ͼ
	epdtype_areagraph		= 5,				//��ά���ͼ
	epdtype_pie				= 14,				//��ͼ
};

struct  CPerformanceInfomation
{
	UINT		m_uType;					//ͳ������
	UINT		m_uDrawType;				//����ͳ��ͼ����
	long		m_nPeriod;					//�ڼ�(����)
	WORD		m_wTotalCount;				//��ͳ������
	CPerformanceInfomation()
	{
		memset(this, 0, sizeof(CPerformanceInfomation));
	}
};

struct CPerformanceThread : public CPerformanceInfomation
{
	char		m_szThreadName[32];
	long		m_lKernelTime;
	long		m_lUserTime;

	CPerformanceThread()
	{
		memset(this, 0, sizeof(CPerformanceThread));
	}

	void Analysis(char* pszName, HANDLE hThread = NULL)
	{
		strncpy(m_szThreadName, pszName, min(sizeof(m_szThreadName) - 1, strlen(pszName)));
		m_uType = epitype_thread;
		m_uDrawType = epdtype_histogram;
		
		HANDLE hCurThread = hThread;
		if (hCurThread == NULL)
		{
			hCurThread = GetCurrentThread();
		}
		FILETIME	ftCreateTime;			//the creation time of the thread
		FILETIME	ftExitTime;
		FILETIME	ftKernelTime;
		FILETIME	ftUserTime;
		::GetThreadTimes(hCurThread, &ftCreateTime, &ftExitTime, &ftKernelTime, &ftUserTime);
		m_lKernelTime = ftKernelTime.dwLowDateTime / 10000;// + ftKernelTime.dwHighDateTime * 
		m_lUserTime = ftUserTime.dwLowDateTime / 10000;
	}
};

struct CPerformanceFunction : public CPerformanceInfomation
{
	DWORD			m_dwTickStart;					//��ʼʱ��
	char			m_szFunctionName[32];			//��������
	DWORD			m_dwTickSpend;					//��������ʱ����
	UINT			m_uAskType;						//��������
	CPerformanceFunction()
	{
		memset(this, 0, sizeof(CPerformanceFunction));
		m_wTotalCount = 6;
	}

	void Start(char* pszName)
	{
		strncpy(m_szFunctionName, pszName, min(sizeof(m_szFunctionName) - 1, strlen(pszName)));
		m_uType = epitype_function;
		m_uDrawType = epdtype_linegraph;
		m_dwTickStart = GetTickCount();
	}

	void End()
	{
		m_dwTickSpend = GetTickCount() - m_dwTickStart;
	}
};
#endif