#pragma once
#include "afxcmn.h"
#include "DlgStrategy.h"
#include "resource.h"
#include <afxwin.h>
#include "kbedit.h"
#include "afxdtctl.h"
#include "DlgJYStrategy.h"
// CDlgTestStrategy 对话框

class CDlgTestStrategy : public CDialog
{
	DECLARE_DYNAMIC(CDlgTestStrategy)

public:
	CDlgTestStrategy(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL);   // 标准构造函数
	virtual ~CDlgTestStrategy();

// 对话框数据
	enum { IDD = IDD_STRATEGY_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	HSOpenExpressUseData* m_pData;
	CDlgStrategy *m_pStrategy;
	HWND m_hTrade;
	CMapWordToPtr *m_pMap;
	CString m_sCode;
public:
	StrategyHis  *m_pStrategyHis;
	HWND	m_hParent;
	CKBEdit m_wndCode;
	CEdit m_wndMoney;
	CEdit m_wndFee;
	CDateTimeCtrl m_wndDayStart;
	CDateTimeCtrl m_wndTimeStart;
	CDateTimeCtrl m_wndDayEnd;
	CDateTimeCtrl m_wndTimeEnd;
	CComboBox m_wndStategyTrig;
	CTabCtrl m_wndTab;
	BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonadd();
	afx_msg void OnBnClickedButtonsave();
	afx_msg void OnBnClickedButtonok();
	afx_msg void OnBnClickedButtoncancel();
	afx_msg void OnTcnSelchangeTabhq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditCode();
	LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);
	LRESULT OnStrategyTrade(WPARAM wp, LPARAM lp);
	BOOL GetParam(BOOL isByDay = FALSE);
	void SetParam();
	afx_msg void OnCbnSelchangeCombocf();
	BOOL LoadStrategy();
	void ResetTrig();
	BOOL IsTradeValid();
	afx_msg void OnBnClickedButtonsv();
	void CloseTradeWin();
	void OnClose();
	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsStrategyChg(BOOL NoName = TRUE);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonOPT();
	afx_msg void OnBnClickedButtonbyday();
};
