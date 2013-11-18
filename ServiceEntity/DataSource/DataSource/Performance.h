/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	performance.h
*	文件标识：
*	摘	  要：	数据引擎性能测试数据定义
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-10-18
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/

#pragma once

#ifdef _SUPPORT_PERFORMANCE_TEST		//支持性能测试

#define IDUM_INIT_HANDLE		1			//初始化窗口句柄

#define PERFORMANCE_DATATRANSFER_CHECK		0x68756e64			//"hund(sun)前四个字母ASCII的值"

enum ePerformanceInfoType
{
	epitype_thread			= 0x01,				//线程信息
	epitype_function		= 0x02,				//过程（函数）信息
};

enum ePerformanceDrawType
{
	epdtype_histogram3D		= 0,				//三维直方图
	epdtype_histogram		= 1,				//二维直方图	
	epdtype_linegraph3D		= 2,				//三维折线图
	epdtype_linegraph		= 3,				//二维折线图
	epdtype_areagraph3D		= 4,				//三维面积图
	epdtype_areagraph		= 5,				//二维面积图
	epdtype_pie				= 14,				//饼图
};

struct  CPerformanceInfomation
{
	UINT		m_uType;					//统计类型
	UINT		m_uDrawType;				//描述统计图类型
	long		m_nPeriod;					//期间(毫秒)
	WORD		m_wTotalCount;				//总统计数量
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
	DWORD			m_dwTickStart;					//开始时间
	char			m_szFunctionName[32];			//函数名称
	DWORD			m_dwTickSpend;					//函数消耗时钟数
	UINT			m_uAskType;						//请求类型
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