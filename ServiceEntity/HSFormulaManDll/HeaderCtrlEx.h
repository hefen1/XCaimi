/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	HeaderCtrlEx.h
*	�ļ���ʶ��	�Զ����б�ͷ��
*	ժ	  Ҫ��	��Ҫ��Ϊ�˶��Ҽ����б�ͷ����Ϣ��Ӧ�����������,����CBaseListCtrl��
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_HEADERCTRLEX_H__F692F0CE_106C_4B59_A96A_B5041F077FF0__INCLUDED_)
#define AFX_HEADERCTRLEX_H__F692F0CE_106C_4B59_A96A_B5041F077FF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeaderCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx window

class CHeaderCtrlEx : public CHeaderCtrl
{
// Construction
public:
	CHeaderCtrlEx(int nKey = 0);
	// ����ID(����Ϣ����ͬ����)
	// m_nKey == 0 : ������������Ŀ�޸Ĳ˵�
	// m_nKey == 1 : �������Ű������˵�( Ŀǰ�˹����Ѳ��� )
	int m_nKey; 

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeaderCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHeaderCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHeaderCtrlEx)
	// ����m_nKey������ͬ�Ĳ˵�
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	// ����ȫ�ǵ����˻���Ĵ�����
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADERCTRLEX_H__F692F0CE_106C_4B59_A96A_B5041F077FF0__INCLUDED_)
