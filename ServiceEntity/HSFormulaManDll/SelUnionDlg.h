/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	SelUnionDlg.h
*	文件标识：	
*	摘	  要：	组合条件选择窗口
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
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
	// 初始化,并将所有组合条件在窗口列表中显示出来
	virtual BOOL OnInitDialog();
protected:
	// "确定"按键处理
	virtual void OnOK();
public:
	// 组合条件列表框
	CListBox m_wndUnionList;
	// 删除按钮
	afx_msg void OnBnClickedUnionDelete();
	// 双击处理::OnOK();
	afx_msg void OnLbnDblclkUnionList();
};
