/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgFormulaImportExport.h
*	�ļ���ʶ��	���뵼����ʽ���ڶ��� 
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_EXPORTANDINPORTDLG_H__49D7F0DE_2849_45E4_A74F_569F4BB73C50__INCLUDED_)
#define AFX_EXPORTANDINPORTDLG_H__49D7F0DE_2849_45E4_A74F_569F4BB73C50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFormulaImportExport.h : header file
//
#include "TreeCtrlFormula.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaImportExport dialog

class CDlgFormulaMan;
class CDlgFormulaImportExport : public CDialog
{
// Construction
public:
	CDlgFormulaImportExport(CWnd* pParent = NULL,long* pThis = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFormulaImportExport)
	enum { IDD = IDD_INPORTEXPORT };
	// ��ȫ����
	CButton	m_wndAllPWD;
	// ���������
	CEdit	m_wndIdeaEdit;
	// �������
	CButton	m_wndIdeaPWD;
	// ͬ����ʽ����
	CButton	m_wndSameBut;
	// ��ʽ��������ʾ�ؼ�
	CEdit	m_wndComment;
	// ��ʽ���ؼ�
	CTreeCtrlFormula	m_wndExpTree;
	// ��ʽ������ʾ�ؼ�
	CStatic	m_wndName;
	// ��ʽע����ʾ�ؼ�
	CEdit	m_wndNote;
	// �Ƿ�ʹ������
	BOOL	m_bIdeaPWD;
	// ���������ִ�
	CString	m_strIdeaPWD;
	// �Ƿ���ȫ����
	BOOL	m_bAllPWD;
	// ��ʾ˵��
	CString	m_strWant;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFormulaImportExport)
	public:
	// ��ϢԤ����,������VK_ESCAPE,VK_RETURN,VK_F2��KEY��Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormulaImportExport)
	// "ȷ��"��ť����,����m_bInport״̬,������������ʽ����
	virtual void OnOK();
	// "ȡ��"��ť����, ɾ���������˽�й�ʽ��
	virtual void OnCancel();
	// "ͬ����ʽ����"��ť����, �˹���û��ʹ��
	afx_msg void OnFindsame();
	// �����ʼ��
	virtual BOOL OnInitDialog();
	//}}AFX_MSG


	/******************************************************************
	*	�������ܣ�	����û��ʹ��,ͬ��ʱ,Ŀǰ���õ���"��������"����ѡ����ʽ
					���ù�ʽ�����Ϣ,�������,����ʾ����Ҫ���������ʾ
	*	����������	WPARAM wParam = 0 : [in] ������
	*				LPARAM lParam = 0 : [in] CTreeGroup*������
	*	�� �� ֵ��	long : ��Ϊ0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	long OnSendAddStr(WPARAM wParam = 0, LPARAM lParam = 0);

	DECLARE_MESSAGE_MAP()

public:
	// ��ʽ����������ָ��
	CDlgFormulaMan*				     m_pFormulaMan;

	// �����Ҹ�ʽ��ʽ����
	CArray<FxjExpress*,FxjExpress*>* m_ayExpress;

	// ��ʽ����ʱ�Ĵ�����: 
	// EXTERN_EXPRESS_CHANGE_NAME	����
	// EXTERN_EXPRESS_AUTO_NAME		�Զ�����
	// EXTERN_EXPRESS_OVR			�������й�ʽ
	WORD							 m_wExternalExpress;

public:
	// ��������,��ָ��ȫ�ֵĹ�ʽ��,���������,��Ϊ�����һ��˽�б��ָ��
	CMapVariabile* m_pExternExpression;
	// ���������Ĺ�ʽ�ļ���
	CString		   m_strFileName;
	// ���䷽��: ���� TRUE, ��� FALSE
	BOOL		   m_bInport;

public:
	void AutoSetup();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTANDINPORTDLG_H__49D7F0DE_2849_45E4_A74F_569F4BB73C50__INCLUDED_)
