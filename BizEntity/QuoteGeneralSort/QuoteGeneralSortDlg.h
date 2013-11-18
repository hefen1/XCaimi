/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	QuoteTableCtrlGeneralSort.h
*	文件标识：
*	摘	  要：	行情综合排名表对话框
*	
*	当前版本：
*	作	  者：	
*	完成日期：
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
****************************************************************/
#ifndef QUOTEGeneralSortDLG
#define QUOTEGeneralSortDLG

#include "CHSStatic.h"
#include "QuoteGeneralSortTabCtrl.h"

#include "HSBizDialog.h"
#include "DataSourceDefine.h"
#include "QuoteSplitterWnd.h"

#define QuoteReportGeneralSortLoggerName  _T("QUOTEGENERALGeneralSort")
#define RequestGeneralGeneralSortTimerID   0x2000
#define RequestGeneralGeneralSortTimer		5000
typedef CArray<StockUserInfo*, StockUserInfo*> CAyStock;

class QuoteGeneralSortDlg: public CHSBizDialog,public IDataSourceSink
{
    DECLARE_DYNCREATE(QuoteGeneralSortDlg)

public:
	QuoteGeneralSortDlg(CWnd* pParent = NULL);
	~QuoteGeneralSortDlg();
	// 对话框数据
	enum { IDD = IDD_QUOTEGeneralSort_DIALOG};

public:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持 
	BOOL	OnInitDialog();
	void	RequestData();
	void	Draw();
	void    SetSize();
	void	SetMulitStock();
	QuoteTableCtrlGeneralSort* GetWndByAskType(int nAskType,int& nWidIndex);
	void	DrawTitle(CDC* pDC,CRect rect);
	void    RemoveCArray();
	void    SetMarketType(HSMarketDataType type) { m_cGeneralSortRequestType = type;}
	void    SetTitleText(CString strText);
	//消息接受接口
	BOOL	HSDataSourceSink_OnCommNotify(void* pData);
	BOOL	HSDataSourceSink_OnRecvData(void* pData, int nLen);

	//请求股票列表信息
	void RequestStockUserInfo();
	//处理股票列表信息
	void HandleStockUserInfo(void* pData, int nlen);
	//添加股票信息
	void AddStock(StockUserInfo* pStock,int nTotal);
	StockUserInfo* GetStockUserInfo(const char*, HSMarketDataType type);

	DECLARE_MESSAGE_MAP()

private:
	CArray<QuoteTableCtrlGeneralSort*,QuoteTableCtrlGeneralSort*> m_ayQuoteSort;
	IDataSource*	    m_pDataSource;				          // 数据引擎接口指针
	long				m_nDataSourceID;			          // 数据请求ID
	HSMarketDataType	m_cGeneralSortRequestType;             //80-89市场类型				 
	CStaticTitle		m_wndTitle;								//标题框
	CQuoteButton		m_wndClose;
	CSize				m_szTitle;								//标题大小
	int					m_nRowCount;							//多图分割的行数
	int					m_nColCount;							//多图分割的列数	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();


	afx_msg void OnBnClickedButtonClose();
};

#endif