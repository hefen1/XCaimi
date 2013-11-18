#if !defined(AFX_HSINCLUDESTOCKANDTIMEDLG_H__96361090_74FC_4239_9F2B_E22423E07E62__INCLUDED_)
#define AFX_HSINCLUDESTOCKANDTIMEDLG_H__96361090_74FC_4239_9F2B_E22423E07E62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetStockAndTime.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSetStockAndTime dialog
#include "ExpPublic.h"
// CDlgSetStockAndTime ref DlgSetStockAndTime.cpp
// 选股配置参数保存/装入
//#define  CDlgSetStockAndTime_Section			  _T("选股-时间-股票范围-选项")
//#define  CDlgSetStockAndTime_Time				  _T("时间")
//#define  CDlgSetStockAndTime_Stock				  _T("股票范围")
//#define  CDlgSetStockAndTime_Option				  _T("选项")

//#define  CDlgSetStockAndTime_Begin_End_Date_Error _T("起始时间设置错误。")

// CDlgUnionConSelStock ref DlgUnionConSelStock.cpp
//#define  CDlgUnionConSelStock_Ok_Prompt			  _T("组合条件为空，确认是否删除？")
//#define  CDlgUnionConSelStock_Not_Name			  _T("名称不能为空！")
//
//#define  CDlgUnionConSelStock_Cond_Or			  _T("下列条件之一成立")
//#define  CDlgUnionConSelStock_Cond_And			  _T("下列条件同时成立")
//#define  CDlgUnionConSelStock_Cond_Or_Pre     	  _T("向前%i-%i天之间,下列条件之一成立")
//#define  CDlgUnionConSelStock_Cond_And_Pre	      _T("向前%i-%i天之间,下列条件同时成立")

class CDlgSetStockAndTime : public CDialog
{
// Construction
public:
		CDlgSetStockAndTime(CWnd* pParent = NULL,
		CArray<StockArea*,StockArea*>* ayCheck = NULL,
		SelStockCondition* curSelStockCondition = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetStockAndTime)
	enum { IDD = IDD_INCLUDESTOCKANDTIMEDLG };
	CDateTimeCtrl	m_wndBeginDate;
	CDateTimeCtrl	m_wndEndDate;
	CStatic	m_wndIncludeStock;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetStockAndTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetStockAndTime)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnChangein();
	virtual BOOL OnInitDialog();
	afx_msg void OnHisdatecheck();
	afx_msg void OnUnioncheck();
	BOOL ShowWindow( int nCmdShow );
	//}}AFX_MSG
	void OnCheckChuquan();

	DECLARE_MESSAGE_MAP()

	LRESULT OnExchangeData(WPARAM wp,LPARAM lp);

public:
	CArray<StockArea*,StockArea*>* m_ayCheck; // 股票范围
	SelStockCondition*			     m_curSelStockCondition;

protected:
	void SetIncludeStockList(CArray<StockArea*,StockArea*>* pCheck,BOOL bAdd = FALSE);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSINCLUDESTOCKANDTIMEDLG_H__96361090_74FC_4239_9F2B_E22423E07E62__INCLUDED_)
