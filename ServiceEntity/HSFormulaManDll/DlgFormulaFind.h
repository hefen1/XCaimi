/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgFormulaFind.h
*	�ļ���ʶ��	��ʽ���Ҵ�����
*	ժ	  Ҫ��	ʵ���ڹ�ʽ���ҵĹ���
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#pragma once
#include <afxcmn.h>
#include <afxwin.h>

#include "HSBaseDialog.h"

class CDlgFormulaFind : public /*CDialog*/ CHSBaseDialog
{
	DECLARE_DYNAMIC(CDlgFormulaFind)

public:
	CDlgFormulaFind(DWORD dwMask = 0xFFFF,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFormulaFind();

	// Dialog Data
	//{{AFX_DATA(CDlgFormulaMan)
	enum { IDD = IDD_COMPFIND };
	// ����
	CAnimateCtrl m_wndAnimate;
	// Ҫ�����ִ�������
	CComboBox m_wndKeyCombo;
//	CComboBox m_wndRangeCombo;
	// ���ҽ���б�
	CListCtrl m_wndResultList;
	
	// ���ҷ�Χ
	BOOL m_bCompDiscribe; // ����
	BOOL m_bCompEdolon;	  // ��������	
	BOOL m_bCompName;	  // ����
	BOOL m_bCompSource;	  // �����ִ�(��ʽ)
	BOOL m_bCompNotes;    // ע��
	BOOL m_bCompAll;	// ��������
	//}}AFX_DATA

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	// "ֹͣ"�ť����
	afx_msg void OnBnClickedStopbut();
	// "��ʼ"��ť����
	afx_msg void OnBnClickedStartbutton();
	// ѡ��ı��¼�����,����û��ʲô��
	afx_msg void OnCbnSelchangeKeycombo();
	// �������.��֪��д���������ʲô�� ??? 
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	// ˫���ҵ��Ĺ�ʽ�б�,���ڣ��ߴ���ʾ��Ϣ
	afx_msg void OnLvnItemActivateResultlist(NMHDR *pNMHDR, LRESULT *pResult);	

public:
	DWORD m_dwMask;		// ���ҷ�Χ(��ʽ����)
	CWnd* m_pParentWnd; // ������,һ����������
	short m_nFindMask;  // ���ҷ�Χ(��ʽ��,ע��,����,����,����˵����)
	BOOL m_bThreadRun;  // �߳�����״̬(TRUE ������,FALSE ֹͣ)

public:

	/******************************************************************
	*	�������ܣ�	�����̴߳�����
	*	����������	LPVOID pParameter : [in] CDlgFormulaFind����ָ��
	*	�� �� ֵ��	DWORD : ��Ϊ0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	static DWORD WINAPI  FindComp(LPVOID pParameter);
	static BOOL bEscMsgOfDlgFind;
public:
	// ��Ϣ����Ԥ����
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	// ��ʼ��,��ʽ�б�������ʼ��
	virtual BOOL OnInitDialog();
	// OK����
	virtual void OnOK();
	// "�˳�"����
	virtual void OnCancel();
	// delete �����Լ�
	virtual void PostNcDestroy();
	

public:
	// �Ӹ�BOOLֵ��֯��m_nFindMask
	short GetFindMask(void);
	BOOL ShowWindow( int nCmdShow );	
	afx_msg void OnBnClickedCompall();
	afx_msg void OnBnClickedCompname();
	afx_msg void OnBnClickedCompdiscribe();
	afx_msg void OnBnClickedCompnotes();
	afx_msg void OnBnClickedCompedolon();
	afx_msg void OnBnClickedCompsource();
};
