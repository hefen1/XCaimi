#pragma once
#include "resource.h"
#include <afxwin.h>

// CDlgSaveTrig �Ի���
class CDlgSaveTrig : public CDialog
{
	DECLARE_DYNAMIC(CDlgSaveTrig)

public:
	CDlgSaveTrig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSaveTrig();

// �Ի�������
	enum { IDD = IDD_DIALOGSaveTrig };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_wndName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL IsValid(CString str);
	CMapWordToPtr *m_pMap;
	CString m_sName;
};
