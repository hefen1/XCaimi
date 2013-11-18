/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	InputPassPwD.h
*	�ļ���ʶ��	�����������(Ŀǰû�еط�����,�Ƿ�Ӧɾ�� ??? )
*	ժ	  Ҫ��	û����,����˵��
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_INPUTPASSPWD_H__4B8A68F6_C2A1_11D4_9029_5254AB285D5A__INCLUDED_)
#define AFX_INPUTPASSPWD_H__4B8A68F6_C2A1_11D4_9029_5254AB285D5A__INCLUDED_

#include "Express.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputPassPwD.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CInputPassPwD dialog

class CInputPassPwD : public CDialog
{
// Construction
public:
	CInputPassPwD(CWnd* pParent = NULL,
		CString strTitle = "",CString strPrompt = "",
		char cType = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputPassPwD)
	enum { IDD = IDD_PASSWORD };
	CString	m_strCurPWD;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputPassPwD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	virtual BOOL OnInitDialog() ;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputPassPwD)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_strTitle,m_strPrompt;	
	char    m_cType;

public:
	CExpression* m_pExp;
	CString m_strFistPWD;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTPASSPWD_H__4B8A68F6_C2A1_11D4_9029_5254AB285D5A__INCLUDED_)
