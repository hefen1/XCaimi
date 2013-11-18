#include "afxwin.h"
#include "Resource.h"
#include "..\..\..\Resource\QuoteLang\Resource.h"
#pragma once

class CQuoteNewTabGroupDlg :
	public CDialog
{
	DECLARE_DYNAMIC(CQuoteNewTabGroupDlg)

public:
	CQuoteNewTabGroupDlg(CWnd* pParent = NULL);
	virtual ~CQuoteNewTabGroupDlg(void);

	// 对话框数据
	enum { IDD = IDD_NEW_TAB_GROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();

	CString m_strGroupName;
protected:
	virtual void OnOK();
};
