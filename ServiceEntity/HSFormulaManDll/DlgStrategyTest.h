// #if !defined(AFX_HSBROWSER_H__B9E17DAB_CF2E_41FA_AA83_690C9CC54510__INCLUDED_)
// #define AFX_HSBROWSER_H__B9E17DAB_CF2E_41FA_AA83_690C9CC54510__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFormulaBrowser.h : header file
//

#include "resource.h"
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"
#include <afxwin.h>
#include "afxdtctl.h"
#include "KBEdit.h"
#include "EditEidolon.h"
#include "ExpPublic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaBrowser dialog

// CDlgStrategyTest 对话框

class CDlgStrategyTest : public CDialog
{
public:
	CDlgStrategyTest(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL);   // 标准构造函数

// 对话框数据
	enum { IDD = IDD_DIALOGStrategyTest };

	CExpTabCtrl	m_wndTab;
	//}}AFX_DATA

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



	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFormulaBrowser)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormulaBrowser)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnButfind();
	afx_msg void OnNotes();
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
	HWND		 m_hParent;
	StrategyHis  *m_pStrategyHis;
	CKBEdit m_wndCode;
	CEdit m_wndMoney;
	CComboBox m_wndPeri;
	CEdit m_wndDay;
	CDateTimeCtrl m_wndDayStart;
	CDateTimeCtrl m_wndTimeStart;
	CDateTimeCtrl m_wndDayEnd;
	CDateTimeCtrl m_wndTimeEnd;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnCbnSelchangeComboper();
	BOOL GetParam();
	
protected:
	CStatic m_wndStaticUnit;
	CComboBox m_wndBuyType;
	CEdit m_wndBuyCount;
	BOOL m_bCKMaxGain;
	BOOL m_bCKMaxDeficit;
	CEdit m_wndGainNum;
	CEdit m_wndDeficitNum;
	CEdit m_wndFees;
public:
	afx_msg void OnBnClickedCkMaxgain();
	afx_msg void OnBnClickedCkMaxdeficit();
	afx_msg void OnCbnSelchangeStrategyBuytype();
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

// #endif // !defined(AFX_HSBROWSER_H__B9E17DAB_CF2E_41FA_AA83_690C9CC54510__INCLUDED_)
