#include "HSBizDialog.h"
#include "quotetabctrl.h"
#include "skinscrollbar.h"
#include "QuoteReportTable.h"
#include "DataSourceDefineEx.h"
#include "DataSourceSinkDefine.h"
#pragma once


#define QUOTETAB_SPLITERECT_WIDTH	3		// 标签页拖动条宽度

#define QUOTEREPORT_HOTKEY_F6					1	// 热键F6 ID
#define QUOTEREPORT_HOTKEY_ADD_TO_BLOCK			2	// 热键 添加到板块 ID
#define QUOTEREPORT_HOTKEY_ADD_TO_SELFBLOCK		3	// 热键 添加到自选股 ID
#define QUOTEREPORT_HOTKEY_BLOCK_ADSCRIPTION	4	// 热键 所属板块 ID
#define QUOTEREPORT_HOTKEY_MARK_STRING			5   // 热键 标记文字
#define QUOTEREPORT_HOTKEY_MARK_CANCEL			6	// 热键 取消标记
#define QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN		20   // 热键 标记图标 开始
#define QUOTEREPORT_HOTKEY_MARK_ICON_END		QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+MARK_COUNT  // 热键 标记图标 结束
// CQuoteReportDlg 对话框
/*CQuoteReportDlg class
********************************************************************************
<PRE>
类名称   : CQuoteReportDlg
功能     : 行情报价模块基础对话框类
--------------------------------------------------------------------------------
备注     : <使用该类时需要注意的问题（如果有的话）> 
典型用法 : <如果该类的使用方法较复杂或特殊，给出典型的代码例子> 
--------------------------------------------------------------------------------
作者     : <xxx>, [yyy], [zzz] ...（作者和逗号分割的修改者列表）
</PRE>
*******************************************************************************/
class CQuoteReportDlg : public CHSBizDialog,public IDataSourceSink
{
	DECLARE_DYNAMIC(CQuoteReportDlg)

public:
	CQuoteReportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQuoteReportDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_QUOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CBrush				m_DlgBkBrush;
	HBITMAP				m_hBmpHScrollBar;		// 水平滚动条图片句柄
	HBITMAP				m_hBmpVScrollBar;		// 垂直滚动条图片句柄

	CSkinScrollBar		m_wndHorzScrollWnd;		// 水平滚动条
	CSkinScrollBar		m_wndVertScrollWnd;		// 垂直滚动条
	CQuoteTabCtrl*		m_pQuoteTabCtrl;		// 报价标签页
	CQuoteReportTable*	m_pQuoteReport;			// 行情报价表
	CRect				m_rcSplit;				// 标签与滚动条位置拖动条
	BOOL				m_bSplitMoved;			// 记录是否拖动过分隔条

	IDataSourceEx*		m_pDataSource;				// 数据引擎接口指针
	long				m_nDataSourceID;			// 数据请求ID

	PageViewInfo		m_CurPageInfo;			// 当前页面信息
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnInit();
	virtual void OnKeyBoardCodeNotify(StockUserInfo* pStkInfo);
	virtual void OnWindowFocused();

	virtual long GetMarketNameByType(const PageViewInfo* pvInfo,CStringArray& strMarketNames);
	BOOL	SetPageInfo(PageViewInfo* pInfo,CString strColGroupName = _T(""));
	void	SetSize();

	BOOL HSDataSourceSink_OnCommNotify(void* pData);
	BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//消息处理函数
public:
	void			OnHotKey(UINT nId);
	void			OnQuoteReportMsg(UINT nMsgID);
	LRESULT			OnChangeTabItem(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void	OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	afx_msg void	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnPaint();
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnReportTableNotify(NMHDR* pNotifyStruct,LRESULT* result);

};
