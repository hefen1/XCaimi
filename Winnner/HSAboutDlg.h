#pragma once


// CHSAboutDlg �Ի���

class CHSAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CHSAboutDlg)

public:
	CHSAboutDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHSAboutDlg();

	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_ABOUTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CListCtrl  m_list;
	DECLARE_MESSAGE_MAP()
};
