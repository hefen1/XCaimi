#pragma once

#include "resource.h"
#include "DataSourceDefine.h"
#include "afxcmn.h"

// CHSDownloadData 对话框

#define INVALIDE_VALUE  -99999 

class CriticalGuard 
{
private:
	CCriticalSection * m_pCritical;
public:
	CriticalGuard(CCriticalSection* pCritical)
	{	
		if(pCritical != NULL)
		{
			m_pCritical = pCritical;
			m_pCritical->Lock();
		}
	}
	~CriticalGuard()
	{
		if(m_pCritical != NULL)
		{
			m_pCritical->Unlock();
		}
	}
};

struct TaskItem
{
	CString m_TaskName;
	long m_lBeignTime; //开始下载时间
	int m_DayCount;
	int m_Min1Count;
	int m_Min5Count;
	unsigned int m_nTaskMask;//按位:第1位日线,第2位5分钟线,第3位1分钟线

	char*          m_localData;
	int			   m_localDataCount;

	TaskItem()
	{
		m_TaskName = "";
		m_lBeignTime = 0;
		m_DayCount = 0;
		m_Min1Count = 0;
		m_Min1Count = 0;
		m_nTaskMask = 0xFFFF;

		m_localDataCount = 0;
		m_localData = NULL;
	}

	void Free()
	{
		if(m_localData != NULL && m_localDataCount > 0)
		{
			StockDay* pDel = (StockDay*)m_localData;
			if (pDel)
			{
				delete[] pDel;
				m_localData = NULL;
				pDel = NULL;
			}
		}
		m_localData = NULL;
		m_localData = 0;
	}

	~TaskItem()
	{
		Free();
	}

	BOOL IsFinishedDAYTask()	
	{
		return m_nTaskMask & 0x0001;
	}

	void SetDayTaskFinished()
	{
		m_nTaskMask = m_nTaskMask | 0x0001;
	}

	BOOL IsFinishedMinute1Task()
	{
		return m_nTaskMask & 0x0004;
	}

	void SetMinute1TaskFinished()
	{
		m_nTaskMask = m_nTaskMask | 0x0004;
	}

	BOOL IsFinishedMinute5Task()
	{
		return m_nTaskMask & 0x0002;
	}

	void SetMinute5TaskFinished()
	{
		m_nTaskMask = m_nTaskMask | 0x0002;
	}

	void UnFinishedDayTask()
	{
		m_nTaskMask &= ~0x0001;
	}
	void UnFinishedMinite1Task()
	{
		m_nTaskMask &= ~0x0004;
	}

	void UnFinishedMinte5Task()
	{
		m_nTaskMask &= ~0x0002;
	}
};

class CDownloadThread : public CWinThread
{
	DECLARE_DYNCREATE(CDownloadThread)

protected:
	CDownloadThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CDownloadThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	static CDownloadThread* m_currentDownloadThread;
	static CWnd*  m_pMain;

protected:
//	DECLARE_MESSAGE_MAP()
};


class CHSDownloadData : public CDialog, public IDataSourceSink
{
	DECLARE_DYNAMIC(CHSDownloadData)

public:
	CHSDownloadData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHSDownloadData();

	enum { IDD = IDD_DOWNLOADDATA };

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL DownLoadData();
	//组请求包
	char* MakeReqPacket(CodeInfo* info, short nPeriod, long& lLen);

	//获取下一只代码
	BOOL GetNextCode(StockUserInfo **pStock,short &nPeriod);

	//处理请求返回数据(排序、过滤、写本地缓存)
	void DealTechDataAns(DataHead *pHead);

	//取本地缓存的日线数据:存到放m_localData和m_localDataCount中
	void GetLocalData(CodeInfo *pCode,short nPeriod,TaskItem* pTask);

	//获取当前时间点nCurrentDate的前一根数据线的时间点
	long GetPreDataDate(CodeInfo *pCode,long nCurrentDate,int nPeriod);

	//获取以时间lBeginDate为起点点，往后挑过nSpillDays个K数据根数的新时间点
	long GetChangedBeginDate(CodeInfo *pCode,long lBeginDate,int nSpillDays,int nPeriod);

	//获取lCurrentDate时间点在nPeriodType的周期类型时，nDays之前的时间点
	long GetPreDate(long lCurrentDate,int nDays,int nPeriodType);

	//判断当天是否已经下载过数据,下载过返回True否则返回False
	BOOL IsDownLoaded(long lDate,CodeInfo *pCode,short nPeriod);

	void InitListView();
	//数据引擎消息接受接口
	BOOL	HSDataSourceSink_OnCommNotify(void* pData);
	BOOL	HSDataSourceSink_OnRecvData(void* pData, int nLen);
	void    SetTaskMask(int nType);
	void    UpdateStockListView(CodeInfo *pCode,TaskItem *pTask,int nPeriod,BOOL bAllExist,BOOL &bWriteDownLoadDate);
private:
	CArray<StockUserInfo>    m_ayStock;          //要下载的代码
	HANDLE             m_hDloadThread;			 //下载数据线程
	IDataSourceEx*	   m_pDataSource;			 //数据引擎接口 
	IChannelManager*   m_pManager;				 //通讯库连接管理接口
	TaskItem*          m_pTaskItem;				 //数据下载任务节点
	long               m_lHandle;				 //行情连接句柄
	long			   m_nDataSourceID;			 //数据请求ID
	int				   m_nPos;                   //当前下载的点
	short              m_nCulType;               //当前周期类型
	unsigned int       m_nTaskMask;              //周期掩码
	int				   m_DownLoadCount;
	int				   m_nLimitedDay;
	int				   m_StockCount;
	int				   m_nReloadDegree;
	CCriticalSection   m_critical;

protected:
	CProgressCtrl      m_progress;
	CListCtrl		   m_lcStock;
	CTreeCtrl          m_tcDataType;
	  
};
