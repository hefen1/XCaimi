#pragma once
#include "resource.h"
#include "TreeCtrlFormula.h"
#include "afxcmn.h"
#include <afxwin.h>
#include "DlgStrategyHQ.h"
#include "ExpPublic.h"

// CDlgStrategyMain �Ի���

class CDlgStrategyMain : public CDialog
{
	DECLARE_DYNAMIC(CDlgStrategyMain)

public:
	CDlgStrategyMain(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL);   // ��׼���캯��
	virtual ~CDlgStrategyMain();

// �Ի�������
	enum { IDD = IDD_DIALOGStrategyMain };
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_wndTab;
	CDlgStrategyHQ* m_nStrHqTab;
	StrategyInfo* m_pStrategy;
	BOOL		 m_nType;      // 1.ѡ��
	HWND		 m_hParent;

	BOOL  m_bHQOK;
	BOOL  m_bJYOK;
protected:
	HSOpenExpressUseData* m_pData;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
};
