/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgAddProjectToBlock.h
*	�ļ���ʶ��	"������"����ģ��--"��Ŀ����"
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/

#if !defined(AFX_HSADDPROJECTDLG_H__F0CAF6BA_9733_4924_8918_DA94B3EBB806__INCLUDED_)
#define AFX_HSADDPROJECTDLG_H__F0CAF6BA_9733_4924_8918_DA94B3EBB806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddProjectToBlock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddProjectToBlock dialog
#include "resource.h"
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"

class CDlgFormulaEdit;
class CExpTabCtrl;

class CDlgAddProjectToBlock : public CDialog
{
// Construction
public:
	enum { Condition,Sort };

	/******************************************************************
	*	�������ܣ�	
	*	����������	CWnd* pParent = NULL : [in] ������
	*				int iFormat = 1		 : [in] ��ʾ��ʽ 
	*				int iType = 0		 : [in] ͳ�Ʒ�ʽ
	*				LPCSTR strColName = "" : [in] ����
	*				LPCSTR strDesc = ""	   : [in] ����
	*				LPCSTR strWinText = "" : [in] ������
    *	�� �� ֵ��	��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	CDlgAddProjectToBlock(CWnd* pParent = NULL, 
		int iFormat = 1, 
		int iType = 0, 
		LPCSTR strColName = "", 
		LPCSTR strDesc = "", 
		LPCSTR strWinText = ""
		);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAddProjectToBlock)
	enum { IDD = IDD_ADDPROJECT };
	// ��ʾ��ʽ��Ͽ�ؼ�
	CComboBox	m_comboFormat;
	// ͳ�Ʒ�ʽ��Ͽ�ؼ�
	CComboBox	m_conboTotal;
	// ����
	CString	m_name;
	// ������
	CString	m_desc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddProjectToBlock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddProjectToBlock)
	virtual BOOL OnInitDialog();
	// OK��������
	virtual void OnOK();
	// OnCancle����
	afx_msg void OnExit();
	// ѡ��ʽ��������
	afx_msg void OnSelectExpress();
	// ��ʽ��Ͽ�ؼ��ı�ѡ����
	afx_msg void OnSelchangeComFormat();
	// ͳ�Ʒ�ʽ��Ͽ�ؼ��ı�ѡ����
	afx_msg void OnSelchangeComTotal();
	// �ı���������
	afx_msg void OnChangeName();
	// ��������ʱ����
	afx_msg void OnDestroy();
	// �½���ť����
	afx_msg void OnNewExpress();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	// ָ��ʽ������
	CWnd	*m_Form;
	// ��ǰ����
	CString m_strCurName;
	// ��ǰ������
	CString m_strCurDesc;
	// ���ڱ���
	CString m_strWinText;
	// ��ǰ��ʽ
	int		m_iCurFormat;
	// ��ǰͳ������
	int		m_iCurType;

	// ??? û��ʲô�ô�,���ȥ��
	CExpTabCtrl	m_wndTab;
	// ??? û��ʲô�ô�,���ȥ��
	void Init();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSADDPROJECTDLG_H__F0CAF6BA_9733_4924_8918_DA94B3EBB806__INCLUDED_)
