#if !defined(AFX_KEYBOARDDATALIST_H__A31A8E43_E047_11D4_9054_5254AB285D5A__INCLUDED_)
#define AFX_KEYBOARDDATALIST_H__A31A8E43_E047_11D4_9054_5254AB285D5A__INCLUDED_


#include "KeyBoardStruct.h"
#include "FormulaManInterFace.h"
#include "FormulaManPublic.h"
#include "DataSourceDefineEx.h"
#include "DataSourceSinkDefine.h"
#include "ConfigInterface.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyboardDataList.h : header file
//

#define HS_CHART_BEGIN           100
#define HS_FENBI				 (HS_CHART_BEGIN)      // 分笔成交(&0)",						   32771
#define HS_ONEMINUTE			 (HS_CHART_BEGIN+1)    // MENUITEM "1分钟图(&1)",                 32772
#define HS_FIVEMINUTE			 (HS_CHART_BEGIN+2)    // MENUITEM "5分钟图(&5)",                 32773
#define HS_FIFTYMINUTE			 (HS_CHART_BEGIN+3)    // MENUITEM "15分钟图(&Q)",                32774
#define HS_THIRTYMINUTE			 (HS_CHART_BEGIN+4)    // MENUITEM "30分钟图(&3)",                32775
#define HS_SIXTYMINUTE			 (HS_CHART_BEGIN+5)    // MENUITEM "60分钟图(&6)",                32776
#define HS_PERIOD_TYPE_DAY		 (HS_CHART_BEGIN+6)    // MENUITEM "日线图(&D)",                  32777
#define HS_WEEK				     (HS_CHART_BEGIN+7)    // MENUITEM "周线图(&W)",                  32778
#define HS_MONTH				 (HS_CHART_BEGIN+8)    // MENUITEM "月线图(&M)",                  32779
#define HS_MOREDAY				 (HS_CHART_BEGIN+9)    // MENUITEM "多日图(&U)",                  32780
#define HS_180MINUTE			 (HS_CHART_BEGIN+10)   // MENUITEM "180分钟图(&6)",
#define HS_ADDMAJOR				 (HS_CHART_BEGIN+60)	
#define ID_WELCOMETIP            (HS_CHART_BEGIN+100)	
#define ID_BOTTOM_STOCK_ALERT    (HS_CHART_BEGIN+101)	
#define ID_KEYBOARD_NOWBUY       (HS_CHART_BEGIN+102)	
#define ID_KEYBOARD_NOWSELL		 (HS_CHART_BEGIN+103)	



//请求页面 分别对应HSCurKeyInfo中枚举页面类型
#define KEYINFO_STOCK            0x00010         //股票
#define KEYINFO_EXPRESS          0x00020         //公式
#define KEYINFO_USERINFO	     0x00030         //外部定义
#define KEYINFO_COMMAND			 0x00040         //特殊页面
#define KEYINFO_QUICKDATA        0x00050         //其他快键

#define MsgInitStocks            WM_USER+100
/////////////////////////////////////////////////////////////////////////////
extern HSShowKeyboardInfo m_sHSShowKeyboardInfo;


struct HSShortCutInfo
{
	CString m_strKey;
	CString m_strMap;
};
/*************************************************************
*	类 名 称：	HSStockPosition
*	类 描 述：	股票定位信息
***************************************************************/
struct HSStockPosition
{
	long m_lBegin;
	long m_lEnd;

	CString m_strName;

	HSMarketDataType m_market; // 市场类别

	CArray<StockUserInfo>*	m_psiInfo;

	HSStockPosition();
	HSStockPosition(long lBegin,long lEnd,
		CArray<StockUserInfo>* psiInfo,CString strName,HSMarketDataType market);

	void Copy(HSStockPosition* pData);
};
inline
HSStockPosition::HSStockPosition()
{
	m_lBegin   = 0;
	m_lEnd     = 0;

	m_psiInfo  = NULL;

	m_market = 0;
}
inline
HSStockPosition::HSStockPosition(long lBegin,long lEnd,
								 CArray<StockUserInfo>* psiInfo,
								 CString strName,
								 HSMarketDataType market)
{
	m_lBegin   = lBegin;
	m_lEnd     = lEnd;

	m_psiInfo = psiInfo;

	m_market = market;

	//m_strName.Format("%s",strName);

}
inline
void HSStockPosition::Copy(HSStockPosition* pData)
{
	m_lBegin   = pData->m_lBegin;
	m_lEnd     = pData->m_lEnd;

	m_psiInfo  = pData->m_psiInfo;

	m_market  = pData->m_market;

	//m_strName.Format("%s",pData->m_strName);
}




// 添加到键盘精灵
struct HSDefaultKeyInfo
{
	CString m_strKey;
	CString m_strNotes;

	PageViewInfo* m_pInfo; // 对应的页面打开
	UINT    m_CmdID;

	HSDefaultKeyInfo()
	{
		m_CmdID = 0;
		m_pInfo = NULL;
	}
	~HSDefaultKeyInfo()
	{
		if ( m_pInfo && !IsBadReadPtr(m_pInfo,1))
			delete m_pInfo;
		m_pInfo = NULL;
	}

	HSDefaultKeyInfo(CString strKey,CString strNotes)
	{
		m_CmdID    = 0;
		m_pInfo    = NULL;
		m_strKey   = strKey;
		m_strNotes = strNotes;
	}
	int IsFind(CString strKey)
	{
		int nIndex = m_strKey.Find(strKey);
		if( nIndex != -1 )
			return nIndex;

		return -1;
	}
};
struct CutShortInfo
{
	HSDefaultKeyInfo* m_pKeyInfo;
	int m_nPageID;
	CutShortInfo()
	{
		m_pKeyInfo = NULL;
		m_nPageID =0 ;
	}
};

class CListBoxKeyboard : public CListBox
{
protected:
	BOOL m_bFirstEraseBk;
public:
	CListBoxKeyboard();
	~CListBoxKeyboard();

	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#define CKeyboardListBase CListBoxKeyboard//CListCtrl

class CKeyboardDataList : public CKeyboardListBase, public IDataSourceSink
{
// Construction
	
public:
	CKeyboardDataList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyboardDataList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKeyboardDataList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKeyboardDataList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
public:
	void InitKeyboard();
	void GetStocks();
	int  AddUsrShortKey(CString strKey,CString strNotes,BOOL bDel = FALSE);
	int  AddUsrShortKeyEx(CString strKey,CString strNotes,UINT CmdID,BOOL bDel = FALSE);
	int  FindUsrShortKey(CString strKey,CString strNotes);
    int  FindShortCut(CString strKey);
    void AddShortCut(CString strKey, CString strMap);

	void FilterData();
	void FilterData_Dll();
	void RemoveMap();

protected:
	CArray<HSDefaultKeyInfo*,HSDefaultKeyInfo*> m_ayKey;
    CArray<HSShortCutInfo *, HSShortCutInfo *> m_ayShortCut;
	IFormulaMan*     m_pFormulaMan;                         //公式接口指针
	IDataSourceEx*   m_pDataSource ;                        //数据引擎接口指针 
	long			 m_lSinkID;
	CArray<HSStockPosition*,HSStockPosition*> m_ayStockPos;   //市场股票代码
	CArray<ExpPropery*,ExpPropery*>				m_ayExpProp;  //公式指标
	CArray<StockUserInfo*,StockUserInfo*> m_ayStockInfo;	//存放查找到得股票结果
	CArray<ExpPropery*,ExpPropery*> m_ayNameProp;           //存放查找到的公式结果
	CArray<CutShortInfo,CutShortInfo> m_ayCutShort;         //存放查找到得快键结果
	CStringArray m_ayOrder;                                 //市场顺序
// 	CMapStringToPtr m_mapStockIndex;
// 	CMapStringToPtr m_mapExp;                 
     
protected:
// 	CMapVariabile* GetExpressMap();
// 	CMapVariabile* m_pExpressMap;

	BOOL	ExitDataThread();
	BOOL	m_bWaitFind;
	BOOL	m_bNextFind;

	WORD	m_dStyle;

	CFont*  m_pFont;
	IHsKeyboardCfg*  m_pKeyboardCfg;
// 线程
public:
	int		m_nStopThread;
	BOOL    CreateDataThread(CString strKey);
//	CString	GetKey()	 { return m_strKey; }
	BOOL	IsExitFind() { return m_bWaitFind; }

public:
	BOOL	IsStyle(WORD dStyle)		{ return (m_dStyle & dStyle); }
	void	AddStyle(WORD dStyle)		{ m_dStyle |= dStyle;         }
	void	RemoveStyle(WORD dStyle)	{ m_dStyle &= (~dStyle);      }
	void    SetStyle(WORD dStyle)		{ m_dStyle = dStyle;          }

protected:
  int   FindStockIndex(int type);
	// 大小
public:
	CSize GetSize();
	
	void  AddItem(int nItem, LPCTSTR lpszItem, int nImage,LPARAM lParam,
				  BOOL bFirst,int nAddPos = -1);
	void  AddStock(int nItem, LPCTSTR lpszItem, int nImage,LPARAM lParam,
				         BOOL bFirst,int nAddPos = -1);
	void  AddExpress(int nItem, LPCTSTR lpszItem, int nImage,LPARAM lParam,
				           BOOL& bFirst,int nAddPos = -1);
	void  AddExpress(int nItem, const CArray<ExpPropery*,ExpPropery*> &ayName);
	void  RemoveItem(int nItem);
	void  RemoveArray();
	virtual BOOL HSDataSourceSink_OnCommNotify(void* pData) ;
	virtual BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen) ;
	
public:
	BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );


	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnInitStocks(WPARAM wParam, LPARAM lParam);
};

////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDDATALIST_H__A31A8E43_E047_11D4_9054_5254AB285D5A__INCLUDED_)
