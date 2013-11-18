/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	TreeDroptarget.h
*	�ļ���ʶ��	������ϷŹ����� ,���ﲻ��ʹ�ã�����˵��
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
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
	// Function to get the index of the item�s image and its 
	// selected image within the tree control�s image list
	void GetItemImages(HTREEITEM hSrcTItem, int &nSelItem, int &nNonSelItem);
	HTREEITEM m_hDestItem;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_TREEDROPTARGET_H)
