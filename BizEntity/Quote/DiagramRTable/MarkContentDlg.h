#include "resource.h"
#include "afxwin.h"

#pragma once

// CMarkContentDlg �Ի���

class CMarkContentDlg : public CDialog
{
	DECLARE_DYNAMIC(CMarkContentDlg)

public:
	CMarkContentDlg(CPoint& point,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMarkContentDlg();

// �Ի�������
	enum { IDD = IDD_MARK_CONTENT };

	CString GetContent();
	void SetSize();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnOK();

	CPoint m_point;
	CString m_strContent;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEdit m_wndContent;
	CButton m_wndOK;
	CButton m_wndCancel;
};
