#pragma once


// CHSAboutDlg 对话框

class CHSAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CHSAboutDlg)

public:
	CHSAboutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHSAboutDlg();

	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_ABOUTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CListCtrl  m_list;
	DECLARE_MESSAGE_MAP()
};
