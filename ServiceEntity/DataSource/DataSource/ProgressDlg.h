#pragma once

#include "Resource.h"
// CProgressDlg �Ի���

class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProgressDlg();

	virtual BOOL OnInitDialog();
	virtual BOOL Create(CWnd* pParentWnd);
// �Ի�������
	enum { IDD = IDD_PROGRESSDLG };

protected:
	CBrush*			m_pBrush;
	CStatic			m_wndText;  
	CProgressCtrl   m_wndProgressCtrl;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void   SetText(CString strText);
	void   SetPos(int nPos);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
