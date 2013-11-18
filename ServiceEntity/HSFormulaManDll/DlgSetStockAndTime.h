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
// ѡ�����ò�������/װ��
//#define  CDlgSetStockAndTime_Section			  _T("ѡ��-ʱ��-��Ʊ��Χ-ѡ��")
//#define  CDlgSetStockAndTime_Time				  _T("ʱ��")
//#define  CDlgSetStockAndTime_Stock				  _T("��Ʊ��Χ")
//#define  CDlgSetStockAndTime_Option				  _T("ѡ��")

//#define  CDlgSetStockAndTime_Begin_End_Date_Error _T("��ʼʱ�����ô���")

// CDlgUnionConSelStock ref DlgUnionConSelStock.cpp
//#define  CDlgUnionConSelStock_Ok_Prompt			  _T("�������Ϊ�գ�ȷ���Ƿ�ɾ����")
//#define  CDlgUnionConSelStock_Not_Name			  _T("���Ʋ���Ϊ�գ�")
//
//#define  CDlgUnionConSelStock_Cond_Or			  _T("��������֮һ����")
//#define  CDlgUnionConSelStock_Cond_And			  _T("��������ͬʱ����")
//#define  CDlgUnionConSelStock_Cond_Or_Pre     	  _T("��ǰ%i-%i��֮��,��������֮һ����")
//#define  CDlgUnionConSelStock_Cond_And_Pre	      _T("��ǰ%i-%i��֮��,��������ͬʱ����")

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
	CArray<StockArea*,StockArea*>* m_ayCheck; // ��Ʊ��Χ
	SelStockCondition*			     m_curSelStockCondition;

protected:
	void SetIncludeStockList(CArray<StockArea*,StockArea*>* pCheck,BOOL bAdd = FALSE);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSINCLUDESTOCKANDTIMEDLG_H__96361090_74FC_4239_9F2B_E22423E07E62__INCLUDED_)
