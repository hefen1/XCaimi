#pragma once
#include "resource.h"
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
#ifndef CDlgBase
#define CDlgBase CYlsBaseDialog
#endif

// CDlgStrategyParamConf 对话框

class CDlgStrategyParamConf : public CDlgBase
{
	DECLARE_DYNAMIC(CDlgStrategyParamConf)

public:
	CDlgStrategyParamConf(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL);   // 标准构造函数
	virtual ~CDlgStrategyParamConf();

// 对话框数据
	enum { IDD = IDD_DIALOGSTRATEGY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	CExpTabCtrl m_wndTab;

	CStatic   m_wndPeriodStc;
	CComboBox m_wndPeriod;

	CStatic   m_wndConNotes;
	CComboBox m_wndCon;
	CStatic   m_wndStaticCon;

	CComboBox m_wndCompare;
	CComboBox m_wndData2;
	CStatic   m_wndAndOr;
	CComboBox m_wndData1;
	CStatic   m_wndMiddle;
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormulaBrowser)
	virtual void OnCancel();
	virtual void OnOK();
// 	afx_msg void OnButfind();
// 	afx_msg void OnUpdate();
/*	afx_msg void OnNotes();*/
	virtual BOOL OnInitDialog();
 	afx_msg void OnSelchangeCon();
 	afx_msg void OnSelchangeCompare();
	virtual void PostNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	virtual LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);

protected:
	HSOpenExpressUseData* m_pData;
protected:
	CEditEidolon m_EditEidolon;
	void		 EnableItem(BOOL bEnable);
public:
	SingleSelCondition m_sSingleSel; // 单个
	BOOL		 m_nType;      // 1.选股
	HWND		 m_hParent;
	// 监控证券
	CEdit m_wndCode;
	// 分析周期
	CComboBox m_wndCycle;
	// 多日线天数
	CEdit m_wndDay;
	// 买入几次后停止买入
	CEdit m_wndBuyTimes;
	// 卖出几次后停止卖出
	CEdit m_wndSellTimes;
	// 持续多少秒触发
	CEdit m_wndTriggerTime;
	// 几次达标后触发
	CEdit m_wndTrigerNum;
};
