/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgUnionList.h
*	文件标识：	选股公式列表管理类定义
*	摘	  要：	对选股条件的增加删除修改或是更改几个条件之间的与还是或关系
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_TEMPUNION_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_)
#include "ExpPublic.h"

#define TEMPUNION_NEWCONDITION 0x99
#define TEMPUNION_SAVEUNION    0x98
#define TEMPUNION_DELUNION    0x97


#define AFX_TEMPUNION_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include <afxwin.h>

// CDlgUnionList dialog

//added by HS
class CDlgUnionList : public CDialog
{
	DECLARE_DYNAMIC(CDlgUnionList)

	struct HSUnionSub
	{
		CArray<UnionSelCondition*,UnionSelCondition*> m_ayArray;

		HSUnionSub()
		{
		}

		~HSUnionSub()
		{
			Free();
		}

		void Free()
		{
			UnionSelCondition* pHSUnion;
			for(int i = 0; i < m_ayArray.GetSize(); i++)
			{
				pHSUnion = m_ayArray.GetAt(i);

				if(pHSUnion)
				{
					delete pHSUnion;
				}
			}
			m_ayArray.RemoveAll();
		}
	};

public:
	CDlgUnionList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUnionList();



// Dialog Data
	//{{AFX_DATA(CDlgUnionList)
	enum { IDD = IDD_TEMPUNION };
	CTreeCtrl	m_wndTree;
	//}}AFX_DATA

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual void PostNcDestroy();
	afx_msg void OnNew();
	afx_msg void OnDelete();
	afx_msg void OnAnd();
//	virtual void OnCancel();
//	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdate();
	afx_msg void OnOr();
//	afx_msg void OnClickTree(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	

public:

	enum{AddButIndex,UpdateButIndex,DeleteButIndex};
	SingleSelCondition* m_pSingleSel;	
	UnionSelCondition*	  m_pHSUnion;
	HSUnionSub   m_ayUnion;
//	BOOL	  m_bCreate;

public:
	void Insert(CTreeCtrl* pTree,UnionSelCondition* pHSUnion);
	int	 DeleteItemData(UnionSelCondition* pHSUnion,void* pDelData);
	HTREEITEM AddUnionGroup(UnionSelConditionSub* pUnionGroup);

public:
	void AddUnionArray(UnionSelCondition* pUnion);
	UnionSelCondition* GetUnion();
	void SetUnion(UnionSelCondition* pUnion);
	void SetSingleUnion(SingleSelCondition* pUnionData);
	
	void EnableBut(int nIndex ,bool bShowOrHide = true);
	static bool JudgeConditionEmpty(UnionSelCondition* pHSUnion);
	
	afx_msg void OnSaveUnion();
	afx_msg void OnDblclkTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDelete(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedLoadunion();

public:
	BOOL SaveLoad(UINT nMode,CString strFileName = "",DWORD dStyle = 0);	
	virtual void Serialize(CArchive& ar,int& nCountRef);
	BOOL ShowWindow( int nCmdShow );

public:
	CString GetDefaultUnionName(UnionSelCondition* pHSUnion);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEMPUNION_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_)

