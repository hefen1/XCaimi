/*************************************************************
*	Copyright (c)2002, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	INPLACEEDIT.H
*	文件标识：	键盘精灵中的输入框控件类( 精灵相关,暂不说明 )
*	摘	  要：	
*
*	当前版本：	2.0
*	作	  者：	俞礼石
*	更新日期：	2003年5月9日
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_INPLACEEDIT_H__ECD42821_16DF_11D1_992F_895E185F9C72__INCLUDED_)
#define AFX_INPLACEEDIT_H__ECD42821_16DF_11D1_992F_895E185F9C72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// InPlaceEdit.h : header file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright (c) 1998.
//
// The code contained in this file is based on the original
// CInPlaceEdit from http://www.codeguru.com/listview/edit_subitems.shtml
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then acknowledgement must be made to the author of this file 
// (in whatever form you wish).
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit window

class CInPlaceEdit : public CEdit
{
// Construction
public:
	CInPlaceEdit();
    CInPlaceEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                 int nRow, int nColumn, CString sInitText, UINT nFirstChar);

// Attributes
public:
 
// Operations
public:
     void EndEdit();
 
// Overrides
     // ClassWizard generated virtual function overrides
     //{{AFX_VIRTUAL(CInPlaceEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
 
// Implementation
public:
     virtual ~CInPlaceEdit();
 
// Generated message map functions
protected:
    //{{AFX_MSG(CInPlaceEdit)
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    int     m_nRow;
    int     m_nColumn;
    CString m_sInitText;
    UINT    m_nLastChar;
    BOOL    m_bExitOnArrows;

	char	m_cType;

public:
	enum Type{none,Tree,Keyboard};

public:
	UINT    GetLastChar() { return m_nLastChar; }

};
 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACEEDIT_H__ECD42821_16DF_11D1_992F_895E185F9C72__INCLUDED_)
