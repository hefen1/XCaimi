/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	SelUnionDlg.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	�������ѡ�񴰿�
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#pragma once
#include <afxwin.h>
#include "resource.h"

// CSelUnionDlg dialog

class CSelUnionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelUnionDlg)

public:
	CSelUnionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelUnionDlg();

// Dialog Data
	enum { IDD = IDD_SEL_UNION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// ��ʼ��,����������������ڴ����б�����ʾ����
	virtual BOOL OnInitDialog();
protected:
	// "ȷ��"��������
	virtual void OnOK();
public:
	// ��������б��
	CListBox m_wndUnionList;
	// ɾ����ť
	afx_msg void OnBnClickedUnionDelete();
	// ˫������::OnOK();
	afx_msg void OnLbnDblclkUnionList();
};
