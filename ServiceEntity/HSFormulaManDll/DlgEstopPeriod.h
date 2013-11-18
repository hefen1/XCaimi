/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgEstopPeriod.h
*	文件标识：	禁用周期设置窗口类
*	摘	  要：	目前此功能还没实现,暂不写 ??? 
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_ESTOPPERIODDLG_H__CFBF07A4_C3FD_11D4_902B_5254AB285D5A__INCLUDED_)
#define AFX_ESTOPPERIODDLG_H__CFBF07A4_C3FD_11D4_902B_5254AB285D5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEstopPeriod.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEstopPeriod dialog

class CDlgEstopPeriod : public CDialog
{
// Construction
public:
	CDlgEstopPeriod(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEstopPeriod)
	enum { IDD = IDD_ESTOPPERIOD };
	CComboBox	m_oDefaultPeriod;
	BOOL	m_bDay;
	BOOL	m_bFifteenMinute;
	BOOL	m_bFiveMinute;
	BOOL	m_bMonth;
	BOOL	m_bMoreDay;
	BOOL	m_bOneDeal;
	BOOL	m_bOneMinute;
	BOOL	m_bSixtyMinute;
	BOOL	m_bThirtyMinute;
	BOOL	m_bWeek;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEstopPeriod)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CDlgEstopPeriod)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void    Empty();
	short	GetEstopPeriod();   // 禁用周期
	short	GetDefaultPeriod(); // 缺省周期
	
	afx_msg void OneDeal();
	void DefaultPeriod(void);
	afx_msg void OneMinute();
	afx_msg void FiveMinute();
	afx_msg void FifteenMinute();
	afx_msg void ThiryMinute();
	afx_msg void SixtyMinute();
	afx_msg void Day();
	afx_msg void Week();
	afx_msg void Month();
	afx_msg void MoreDay();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ESTOPPERIODDLG_H__CFBF07A4_C3FD_11D4_902B_5254AB285D5A__INCLUDED_)
