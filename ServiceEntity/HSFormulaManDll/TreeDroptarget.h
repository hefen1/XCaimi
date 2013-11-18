/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	TreeDroptarget.h
*	文件标识：	树鼠标拖放管理类 ,这里不再使用，不做说明
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
// TreeDropTarget.h

#if !defined(_TREEDROPTARGET_H)
#define _TREEDROPTARGET_H

#include <afxole.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeDropTarget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeDropTarget


class CTreeCtrlFormula;
class CTreeDropTarget :public COleDropTarget	
{
	// Overrides
public:
	virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, 
		DWORD dwKeyState, CPoint point );
	virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, 
		DWORD dwKeyState, CPoint point );
	virtual void OnDragLeave( CWnd* pWnd );
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point);
	// Members
public:
	//Static variable to store the window handle of source tree control
	static HWND m_shWndTreeCtrl;
private:
	// Pointer to source tree control
	CTreeCtrlFormula *m_pSourceTreeCtrl;
	// Pointer to destination tree  control
	CTreeCtrlFormula *m_pDestTreeCtrl;
	// Recursive Function for adding items from source tree to destination tree
	void AddItem(HTREEITEM hSrcTItem,HTREEITEM hDestTItem );
	// Function to get the index of the item抯 image and its 
	// selected image within the tree control抯 image list
	void GetItemImages(HTREEITEM hSrcTItem, int &nSelItem, int &nNonSelItem);
	HTREEITEM m_hDestItem;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_TREEDROPTARGET_H)
