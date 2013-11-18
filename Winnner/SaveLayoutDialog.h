#pragma once


// CSaveLayoutDialog 对话框

class CSaveLayoutDialog : public CDialog
{
	DECLARE_DYNAMIC(CSaveLayoutDialog)

public:
	CString m_strLayoutName;
	CSaveLayoutDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSaveLayoutDialog();

// 对话框数据
	enum { IDD = IDD_SAVE_LAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
