#pragma once


// CScrollNews 对话框

class CScrollNews : public CDialog
{
	DECLARE_DYNAMIC(CScrollNews)

public:
	CScrollNews(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScrollNews();

	void SetStatusBar(CWnd* p) { m_statusBar = p; }
// 对话框数据
	enum { IDD = IDD_SCROLL_NEWS };

protected:
	CWnd* m_statusBar;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
