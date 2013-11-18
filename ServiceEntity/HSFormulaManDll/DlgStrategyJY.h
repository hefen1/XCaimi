#pragma once
#include "afxcmn.h"
#include "DlgStrategy.h"
#include "resource.h"
#include <afxwin.h>
#include "DlgJYStrategy.h"
// CDlgStrategyJY �Ի���

class CDlgStrategyJY : public CDialog
{
	DECLARE_DYNAMIC(CDlgStrategyJY)

public:
	CDlgStrategyJY(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL);   // ��׼���캯��
	virtual ~CDlgStrategyJY();

// �Ի�������
	enum { IDD = IDD_STRATEGY_JY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
	HWND m_hTrade;
	CString m_sCode;
public:	
	HSOpenExpressUseData* m_pData;
	CDlgStrategy *m_pStrategy;
	StrategyInfo *m_pInfo;
	CMapWordToPtr *m_pMap;//�������еĴ�������
	HWND	m_hParent;
	CTabCtrl m_wndTab;
	CKBEdit m_wndCode;
	CComboBox m_wndStartegyCF;
	CButton m_wndCheck;
	BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabhq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtontest();
	afx_msg void OnBnClickedButtonsave();
	afx_msg void OnBnClickedButtonok();
	afx_msg void OnBnClickedButtoncancel();
	BOOL  GetParam();
	void SetParam();
	afx_msg void OnEnChangeEditcode();
	LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);
	LRESULT OnStrategyTrade(WPARAM wp, LPARAM lp);
	BOOL LoadStrategy();//ȡ�����еĴ������ԣ�������
	afx_msg void OnCbnSelchangeCombocf();
	void ResetTrig();
	afx_msg void OnBnClickedButtonsv();
	BOOL IsTradeValid();
	void CloseTradeWin();
	void OnClose();
	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsStrategyChg(BOOL NoName = TRUE);
	afx_msg void OnPaint();
};
