/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	InportPromptDlg.h
*	�ļ���ʶ��	���빫ʽʱ,�������������뷽ʽѡ�񴰿���
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_INPORTPROMPTDLG_H__C66A57EC_65BE_4A91_B1F3_C93E91544C06__INCLUDED_)
#define AFX_INPORTPROMPTDLG_H__C66A57EC_65BE_4A91_B1F3_C93E91544C06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InportPromptDlg.h : header file
//
#include "resource.h"
#include "HSBizDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CInportPromptDlg dialog

class CDlgFormulaImportExport;
class CInportPromptDlg : public CDialog
{
// Construction
public:
	CInportPromptDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInportPromptDlg)
	enum { IDD = IDD_PROMPTDLG };
	// ����ʱ�Ĳ�����ʽ(0����,1�Զ�����,2����)
	int		m_nType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInportPromptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInportPromptDlg)
	// ȷ������
	virtual void OnOK();
	// �˳�����
	virtual void OnCancel();
	// �Ի����ʼ������
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPORTPROMPTDLG_H__C66A57EC_65BE_4A91_B1F3_C93E91544C06__INCLUDED_)
