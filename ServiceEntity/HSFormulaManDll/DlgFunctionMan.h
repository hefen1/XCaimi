/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgFunctionMan.h
*	�ļ���ʶ��	������������
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_FUNCTIONMAN_H__99136E56_137F_11D5_A10C_000000000000__INCLUDED_)
#define AFX_FUNCTIONMAN_H__99136E56_137F_11D5_A10C_000000000000__INCLUDED_

#include "ExpPublic.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFunctionMan.h : header file
//
//#include "ETSLayout.h"
#include "BaseListCtrl.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFunctionMan dialog

#define USERDEF_UPDATEDATA (WM_USER+101)

/*************************************************************
*	�� �� �ƣ�	CDlgFunctionMan
*	�� �� ����	������������
***************************************************************/
class CDlgFunctionMan : public CDialog
{
// Construction
public:

	/******************************************************************
	*	�������ܣ�	����,��ʼ��������CWnd���Create(������ģʽ�Ի���)
	*	����������	CWnd* pParent = NULL : [in] ������
	*				CMapVariabile*  pExternFunction = NULL : [in] �ⲿ������
	*				CMapVariabile*	pExternExpression = NULL : [in]	�ⲿ��ʽ��
	*				long*  pThis = NULL : [in] ����ָ���ָ��
	*	�� �� ֵ��	��
	*	����������	CWnd::Create()
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	CDlgFunctionMan(CWnd* pParent = NULL,CMapVariabile*  pExternFunction = NULL,
		CMapVariabile*	pExternExpression = NULL,
		long*  pThis = NULL );   // standard constructor

	/* DialogMgr: Add this: */
//	DECLARE_LAYOUT();
	/************************/

// Dialog Data
	//{{AFX_DATA(CDlgFunctionMan)
	enum { IDD = IDD_FUNCTIONMAN };
	// ע���ı��� 
	CEdit			m_wndNotes;
	// �����б�ؼ�
	CBaseListCtrl   m_wndTypeList;//m_wndTabCtrl;
	// ��ǰ�������й�ʽ���ؼ�
	CBaseListCtrl	m_wndListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFunctionMan)
public:
	// ��Ϣ����Ԥ����
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	// �Ի������ݽ���
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CDlgFunctionMan)
	virtual void OnOK();		// "ȷ��"��ť���� ??? 
	virtual void OnCancel();	// "�ر�"��ť����
	afx_msg void OnNew();		// "�¼�"��ť���� ???
	afx_msg void OnUpdate();	// ""
	afx_msg void OnInsert();	// "����"��ť����
	virtual BOOL OnInitDialog();// �Ի����ʼ������
	// ���������б�ѡ��ı���Ϣ����
	afx_msg void OnItemchangedType(NMHDR* pNMHDR, LRESULT* pResult);
	// �����б�ѡ��ı���Ϣ����
	afx_msg void OnItemchangedFunlist(NMHDR* pNMHDR, LRESULT* pResult);
	// �ı��С��Ϣ����
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG


	/******************************************************************
	*	�������ܣ�	USERDEF_UPDATEDATA�û���Ϣ����
	*	����������	WPARAM wParam = 0 : [in] ������
	*				LPARAM lParam = 0 : [in] �ڴ�ָ��,�������С���ж�ΪCFunExpresion*����CExpValue*��
	*	�� �� ֵ��	long : ��Ϊ0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	long OnUserDefUpdateData(WPARAM wParam = 0, LPARAM lParam = 0);

	DECLARE_MESSAGE_MAP()
protected:

	/******************************************************************
	*	�������ܣ�	��ָ�����͵ĺ����ں����б�ؼ�����ʾ����
	*	����������	short nType : [in] ��������(0-11)
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void AddFunByType(short nType);

	/******************************************************************
	*	�������ܣ�	��ָ�����͵Ĺ�ʽ�ڹ�ʽ�б�ؼ�����ʾ����(Ŀǰ�������� ??? )
	*	����������	short nType : [in] ��ʽ����
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void AddExpressByType(short nType);

protected:
	class CDlgFormulaEdit* m_pParent;			// ���ñ�ģ��Ĺ�ʽ�༭������ָ��
	CMapVariabile*		  m_pExternFunction;	// �ⲿ������
	CMapVariabile*		  m_pExternExpression;  // �ⲿ���ʽ��

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNCTIONMAN_H__99136E56_137F_11D5_A10C_000000000000__INCLUDED_)
