#pragma once


// CSaveLayoutDialog �Ի���

class CSaveLayoutDialog : public CDialog
{
	DECLARE_DYNAMIC(CSaveLayoutDialog)

public:
	CString m_strLayoutName;
	CSaveLayoutDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSaveLayoutDialog();

// �Ի�������
	enum { IDD = IDD_SAVE_LAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
