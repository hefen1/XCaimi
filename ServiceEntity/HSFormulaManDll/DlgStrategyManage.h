#pragma once
#include "resource.h"
#include "afxdtctl.h"
#include <afxwin.h>
#include "DlgStrategy.h"
#include "afxcmn.h"
#include "DlgJYStrategy.h"
// CDlgStrategyManage 对话框

class CDlgStrategyManage : public CDialog
{
	DECLARE_DYNAMIC(CDlgStrategyManage)

public:
	CDlgStrategyManage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStrategyManage();

// 对话框数据
	enum { IDD = IDD_DIALOGStgyMng };
	BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CDlgStrategy *m_pStrategy;
	HWND m_hTrade;
	void Show(BOOL bshow = TRUE);
	void ShowTab(BOOL bshow = TRUE);
	CMapWordToPtr *m_pMap;
	BOOL m_bAdd;
public:
	afx_msg void OnBnClickedButtonadd();
	CTabCtrl m_wndTab;
	afx_msg void OnTcnSelchangeTabtrig(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtoncancel();
	CEdit m_wndName;
	CListCtrl m_wndList;
	afx_msg void OnBnClickedButtondelete();
	afx_msg void OnBnClickedButtonmodify();
	BOOL LoadStrategy();
	afx_msg void OnNMClickListtrig(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnStrategyTrade(WPARAM wp, LPARAM lp);
	void SetRDOnly(BOOL ro=TRUE);
	afx_msg void OnBnClickedButtonsave();
	afx_msg void OnBnClickedButtoncc();
	void CloseTradeWin();
	void OnClose();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	//CEdit m_wndExplain;
};
