#pragma once
#include "DlgLevelInput.h"

// CDlgLevelFrame 对话框

class CDlgLevelFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLevelFrame)

public:
	CDlgLevelFrame(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLevelFrame();

// 对话框数据
	enum { IDD = IDD_DIALOG_LEVELFRAME };

public:
	void  Init();
	BOOL  IsLogin() {return m_bLogin;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnCancel();

private:
	CDlgLevelInput *m_pLevelInputDlg;
	BOOL            m_bLogin;
};
