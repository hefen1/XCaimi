#pragma once


// CScrollNews �Ի���

class CScrollNews : public CDialog
{
	DECLARE_DYNAMIC(CScrollNews)

public:
	CScrollNews(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScrollNews();

	void SetStatusBar(CWnd* p) { m_statusBar = p; }
// �Ի�������
	enum { IDD = IDD_SCROLL_NEWS };

protected:
	CWnd* m_statusBar;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
