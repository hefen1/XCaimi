/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgFormulaBrowser.h
*	文件标识：	公式浏览器(可以修改参数数值)类定义
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_HSBROWSER_H__B9E17DAB_CF2E_41FA_AA83_690C9CC54510__INCLUDED_)
#define AFX_HSBROWSER_H__B9E17DAB_CF2E_41FA_AA83_690C9CC54510__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFormulaBrowser.h : header file
//

#include "resource.h"
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"
#include "ExpPublic.h"
#include "EditEidolon.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaBrowser dialog


class CDlgFormulaBrowser : public CDialog
{
// Construction
public:
	CDlgFormulaBrowser(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL);   // standard constructor

//	/* DialogMgr: Add this: */
//	DECLARE_LAYOUT();
//	/************************/

// Dialog Data
	//{{AFX_DATA(CDlgFormulaBrowser)
	enum { IDD = IDD_BROWSEFORMULA };
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
	afx_msg void OnUpdate();
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
	BOOL ShowWindow( int nCmdShow );
public:
	SingleSelCondition m_sSingleSel; // 单个
	BOOL		 m_nType;      // 1.选股
	HWND		 m_hParent;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSBROWSER_H__B9E17DAB_CF2E_41FA_AA83_690C9CC54510__INCLUDED_)
