#pragma once
#include "LoginWnd.h"

// CFrameLoginDlg 对话框

class CFrameLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CFrameLoginDlg)

public:
	CFrameLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFrameLoginDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FRAME };

public:
	CLoginWnd* GetLoginWnd(){return &m_LoginWnd;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnDestroy();
private:
	CLoginWnd  m_LoginWnd;
	
};
