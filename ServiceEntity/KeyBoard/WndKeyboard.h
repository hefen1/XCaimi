/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	WndKeyboard.h
*	�ļ���ʶ��	���̾��鴰����(�������,�Ժ���д)
*	ժ	  Ҫ��	�������˸������Ĵ�����,
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_INDIVIDUALINFO_H__9E29AC76_D944_11D4_904C_5254AB285D5A__INCLUDED_)
#define AFX_INDIVIDUALINFO_H__9E29AC76_D944_11D4_904C_5254AB285D5A__INCLUDED_

#include <afxbutton.h>
#include "KeyButton.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_ENTER_CHARS			6	  // ���������ַ�����
#define SUPPORT_2004_03_23_CHANGE     // �ļ��̾���
									  // ������볤��Ϊ6
									  // ���������ҽ��Ϊ0ʱ�������ϴβ��ҽ����

#define DS_DependenceParent 0x00000010 // �͸����ڴ�Сһ��ͣ��
#define DS_NotBerth			0x00000020 // ��ͣ��
#define DS_BYTREE			0x00000100 // ����ʽ
#define DS_BYKEYBOARD		0x00000200 // ���̾���
#define DS_ALWAYSSHOW		0x00000400 // ������ʾ
#define DS_AUTOSHOWHIDE		0x00000800 // ʧȥ����ʱ�Զ�Ӱ��

#define hxUSER_EDITMSG		WM_USER + 10

class CTreeCtrlFormula;
class CKeyboardDataList;
class CWndKeyboard : public CWnd
{
// Construction
public:
	CWndKeyboard();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndKeyboard)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWndKeyboard();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndKeyboard)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	void OnChangeEdit();
	void OnShowButton();
	void OnCloseButton();
	LRESULT OnEditMsg(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	enum Align {none,left,top,right,bottom};
	BOOL	   Create(CWnd* pParentWnd,CRect rect);

protected:
	Align    m_nAlign;
	Align    m_nOldAlign;

	CBrush*  m_pBrush;
	void	 Initialise(CString strClass);
	
	Align	 AdjustPos(CRect* lpRect);	
	void	 SetSize(BOOL bRestore);
	void	 RestoreWindow();

	BOOL	 MouseDown(UINT nFlags, CPoint point);

	CRect    m_oldRect;
	CRect    m_floatRect;
	BOOL	 m_bOperator;
	BOOL	 m_bMoveWindow;
	BOOL	 m_bEnter;

	DWORD	 m_dwStyle;
public:
	void AddStyle(DWORD	dwStyle)	{ m_dwStyle |= dwStyle; }
	void RemoveStyle(DWORD dwStyle) { m_dwStyle &= (~dwStyle); }
	void SetStyle(DWORD dwStyle)	{ m_dwStyle = dwStyle; }
	BOOL IsStyle(DWORD dwStyle)		{ return (m_dwStyle & dwStyle); }
	void PaintBK(CDC* pDC);
protected:
	CImageList		   m_contentImages;

protected:
	CKeyboardDataList*  m_pKeyboard;
	CEdit*				m_pEdit;
 	CKeyButton*			m_pAlwaysShow;
    CKeyButton*   	m_pCloseBtn;
	CFont*				m_pFont;
	CFont*              m_pEditFont;

public:

	CKeyboardDataList*  GetKeyboard()	{ return m_pKeyboard;	 }
	CEdit*				GetEdit()		{ return m_pEdit;        }
	CKeyboardDataList*  GetList()		{ return m_pKeyboard;    }

public:
	BOOL ShowWindow( int nCmdShow );

	static HHOOK			m_hookProc;
	static LRESULT CALLBACK KeyboardHookCallback(int code, WPARAM wParam, LPARAM lParam);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDIVIDUALINFO_H__9E29AC76_D944_11D4_904C_5254AB285D5A__INCLUDED_)
