#pragma once
#include "LoginWnd.h"

// CFrameLoginDlg �Ի���

class CFrameLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CFrameLoginDlg)

public:
	CFrameLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFrameLoginDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FRAME };

public:
	CLoginWnd* GetLoginWnd(){return &m_LoginWnd;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnDestroy();
private:
	CLoginWnd  m_LoginWnd;
	
};
