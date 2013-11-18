#pragma once
#include "DlgLevelInput.h"

// CDlgLevelFrame �Ի���

class CDlgLevelFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLevelFrame)

public:
	CDlgLevelFrame(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLevelFrame();

// �Ի�������
	enum { IDD = IDD_DIALOG_LEVELFRAME };

public:
	void  Init();
	BOOL  IsLogin() {return m_bLogin;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnCancel();

private:
	CDlgLevelInput *m_pLevelInputDlg;
	BOOL            m_bLogin;
};
