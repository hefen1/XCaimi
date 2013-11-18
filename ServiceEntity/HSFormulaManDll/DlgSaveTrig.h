#pragma once
#include "resource.h"
#include <afxwin.h>

// CDlgSaveTrig 对话框
class CDlgSaveTrig : public CDialog
{
	DECLARE_DYNAMIC(CDlgSaveTrig)

public:
	CDlgSaveTrig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSaveTrig();

// 对话框数据
	enum { IDD = IDD_DIALOGSaveTrig };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_wndName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL IsValid(CString str);
	CMapWordToPtr *m_pMap;
	CString m_sName;
};
