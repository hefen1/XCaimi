#include "resource.h"
#include "afxwin.h"

#pragma once

// CMarkContentDlg 对话框

class CMarkContentDlg : public CDialog
{
	DECLARE_DYNAMIC(CMarkContentDlg)

public:
	CMarkContentDlg(CPoint& point,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMarkContentDlg();

// 对话框数据
	enum { IDD = IDD_MARK_CONTENT };

	CString GetContent();
	void SetSize();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnOK();

	CPoint m_point;
	CString m_strContent;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEdit m_wndContent;
	CButton m_wndOK;
	CButton m_wndCancel;
};
