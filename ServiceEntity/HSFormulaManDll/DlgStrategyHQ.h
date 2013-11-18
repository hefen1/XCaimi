
#pragma once
#include "resource.h"
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"
#include "afxcmn.h"
#include <afxwin.h>
#include "KBEdit.h"
/////////////////////////////////////////////////////////////////////////////
#include "EditEidolon.h"


// CDlgStrategyParamConf 对话框

class CDlgStrategyHQ : public CDialog
{
	DECLARE_DYNAMIC(CDlgStrategyHQ)

public:
	CDlgStrategyHQ(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL);   // 标准构造函数
	virtual ~CDlgStrategyHQ();

	// 对话框数据
	enum { IDD = IDD_DIALOGStrategyHQ };

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
public:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormulaBrowser)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);

protected:
	HSOpenExpressUseData* m_pData;
protected:
	CEditEidolon m_EditEidolon;
	void		 EnableItem(BOOL bEnable);
	BOOL		 m_bSetVar; //是否重新设置了参数
public:
	HWND		 m_hParent;
	// 监控证券
	CKBEdit m_wndCode;
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
	StrategyInfo* m_pStrategy;
 	afx_msg void OnEnChangeEdit1();
	LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeCombo1();
	CEdit m_wndGap;
	CButton m_wndCheck;
	CButton m_wndXD;
};
