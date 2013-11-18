/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	dlgnotesAndEidolon.h
*	�ļ���ʶ��	��ʽע�ͼ��������鴰���ඨ��
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_DLGTECHEXPRESS_H__A38EA223_35F4_11D3_97B1_0000E821282D__INCLUDED_)
#define AFX_DLGTECHEXPRESS_H__A38EA223_35F4_11D3_97B1_0000E821282D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTechExpress.h : header file
//

#include "EditEidolon.h"
#include "HSBaseDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgNotesAndEidolon dialog

//#include "EditEidolon.h"

class CDlgNotesAndEidolon : public /*CDialog*/ CHSBaseDialog
{
//	DECLARE_DYNCREATE(CDlgNotesAndEidolon)

// Construction
public:
	CDlgNotesAndEidolon(CWnd* pParent = NULL);
	~CDlgNotesAndEidolon();

	// ����
	BOOL Create(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CDlgNotesAndEidolon)
	enum { IDD = IDD_NOTEANDEIDOLON };
	// ��ʽע��(�ı�)
	CEdit	m_pEditCtrl;
	// ��������(�ı�)
	CEdit	m_pEidolonCtrl;
	// �����޸�(���ı�)
	CEditEidolon m_EditEidolon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgNotesAndEidolon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CDlgNotesAndEidolon)
	// ȡ������
	virtual void OnCancel();
	// ȷ������
	virtual void OnOK();
	// ��ʼ��
	virtual BOOL OnInitDialog();
	// �ı��С����
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// ֻ�����˸���,�Ż�ʱ��ɾ�� ??? 
	afx_msg void OnDestroy();
	// ֻ�����˸���,�Ż�ʱ��ɾ�� ??? 
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG

	// ��ʽ����,ͬʱ���¹�ʽ��ص����ݼ���
	LRESULT OnCompileUpdateData(WPARAM wp,LPARAM lp);

	DECLARE_MESSAGE_MAP()

public:
	// �ؼ�����
	enum EditType {Notes/*ע��*/,EidolonEdit/*��������*/,Eidolon/*�����޸�*/};

	// ���ÿؼ�����
	void SetType(EditType nType) { m_nType = nType;}
	// �жϿؼ�����
	BOOL IsType (EditType nType) { return (m_nType == nType); }

	// ��ʽָ��,�ؼ���Ҫ��ʾ�Ĺ�ʽ
	CExpression* m_pExpression;

	/******************************************************************
	*	�������ܣ�	���ù�ʽע����Ϣ
	*	����������	CString strNotes	: [in] ע������
	*				CString strCaption  : [in] ���ڱ���
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��13��
	*	�޸�˵����	
	*****************************************************************/
	void SetNotes(CString strNotes,CString strCaption);

	/******************************************************************
	*	�������ܣ�	���ò���������Ϣ
	*	����������	CString strNotes	: [in] ����������Ϣ
	*				CString strCaption	: [in] ���ڱ���
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��13��
	*	�޸�˵����	
	*****************************************************************/
	void SetEidolonEdit(CString strNotes,CString strCaption);

	/******************************************************************
	*	�������ܣ�	���ò����޸Ľ���(���ݹ�ʽ�еľ��������ı���Ϣ��������Ϣ)
	*	����������	CString strText/CStringArray* pArray : [in] ���������ı���Ϣ
	*				CMapVariabile* pParamVar : [in] ������Ϣ
	*				CString strCaption : [in] ���ڱ���
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��13��
	*	�޸�˵����	
	*****************************************************************/
	void SetEidolon(CString strText,CMapVariabile* pParamVar,CString strCaption);
	void SetEidolon(CStringArray* pArray,CMapVariabile* pParamVar,CString strCaption);
 
	// ��յ�ǰ��ʾ����
	void	Empty();
	// �õ���ǰ��ʾ����
	CString GetText();

protected:
	// �ؼ�����
	EditType m_nType;
public:
	// �ı䴰�ڴ�Сʱͬʱ�ı�ؼ���С
	void SetSize();
	// ��ʾ�ؼ�pWnd, �����ô��ڱ���ΪstrCaption
	void Show(CString strCaption,CWnd* pWnd);
	// Ŀǰ��������,�Ż�ʱ�Ƿ��ɾ�� ??? 
	void Focus();

public:
	// Ŀǰ�����õ�ֻ�ǵ����˻����ShowWindow,�Ż�ʱ�Ƿ��ɾ�� ??? 
	virtual BOOL ShowWindow( int nCmdShow );

	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTECHEXPRESS_H__A38EA223_35F4_11D3_97B1_0000E821282D__INCLUDED_)
