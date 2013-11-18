#pragma once

#include "resource.h"
#include "DataSourceDefine.h"
#include "afxcmn.h"

// CHSDownloadData �Ի���

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
	long m_lBeignTime; //��ʼ����ʱ��
	int m_DayCount;
	int m_Min1Count;
	int m_Min5Count;
	unsigned int m_nTaskMask;//��λ:��1λ����,��2λ5������,��3λ1������

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
	CDownloadThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	CHSDownloadData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHSDownloadData();

	enum { IDD = IDD_DOWNLOADDATA };

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL DownLoadData();
	//�������
	char* MakeReqPacket(CodeInfo* info, short nPeriod, long& lLen);

	//��ȡ��һֻ����
	BOOL GetNextCode(StockUserInfo **pStock,short &nPeriod);

	//�������󷵻�����(���򡢹��ˡ�д���ػ���)
	void DealTechDataAns(DataHead *pHead);

	//ȡ���ػ������������:�浽��m_localData��m_localDataCount��
	void GetLocalData(CodeInfo *pCode,short nPeriod,TaskItem* pTask);

	//��ȡ��ǰʱ���nCurrentDate��ǰһ�������ߵ�ʱ���
	long GetPreDataDate(CodeInfo *pCode,long nCurrentDate,int nPeriod);

	//��ȡ��ʱ��lBeginDateΪ���㣬��������nSpillDays��K���ݸ�������ʱ���
	long GetChangedBeginDate(CodeInfo *pCode,long lBeginDate,int nSpillDays,int nPeriod);

	//��ȡlCurrentDateʱ�����nPeriodType����������ʱ��nDays֮ǰ��ʱ���
	long GetPreDate(long lCurrentDate,int nDays,int nPeriodType);

	//�жϵ����Ƿ��Ѿ����ع�����,���ع�����True���򷵻�False
	BOOL IsDownLoaded(long lDate,CodeInfo *pCode,short nPeriod);

	void InitListView();
	//����������Ϣ���ܽӿ�
	BOOL	HSDataSourceSink_OnCommNotify(void* pData);
	BOOL	HSDataSourceSink_OnRecvData(void* pData, int nLen);
	void    SetTaskMask(int nType);
	void    UpdateStockListView(CodeInfo *pCode,TaskItem *pTask,int nPeriod,BOOL bAllExist,BOOL &bWriteDownLoadDate);
private:
	CArray<StockUserInfo>    m_ayStock;          //Ҫ���صĴ���
	HANDLE             m_hDloadThread;			 //���������߳�
	IDataSourceEx*	   m_pDataSource;			 //��������ӿ� 
	IChannelManager*   m_pManager;				 //ͨѶ�����ӹ���ӿ�
	TaskItem*          m_pTaskItem;				 //������������ڵ�
	long               m_lHandle;				 //�������Ӿ��
	long			   m_nDataSourceID;			 //��������ID
	int				   m_nPos;                   //��ǰ���صĵ�
	short              m_nCulType;               //��ǰ��������
	unsigned int       m_nTaskMask;              //��������
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
