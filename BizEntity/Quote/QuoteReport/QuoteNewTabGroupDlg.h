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

	// �Ի�������
	enum { IDD = IDD_NEW_TAB_GROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();

	CString m_strGroupName;
protected:
	virtual void OnOK();
};
