#pragma once
#include "resource.h"
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
#ifndef CDlgBase
#define CDlgBase CYlsBaseDialog
#endif

// CDlgStrategyParamConf �Ի���

class CDlgStrategyParamConf : public CDlgBase
{
	DECLARE_DYNAMIC(CDlgStrategyParamConf)

public:
	CDlgStrategyParamConf(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL);   // ��׼���캯��
	virtual ~CDlgStrategyParamConf();

// �Ի�������
	enum { IDD = IDD_DIALOGSTRATEGY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	CExpTabCtrl m_wndTab;

	CStatic   m_wndPeriodStc;
	CComboBox m_wndPeriod;

	CStatic   m_wndConNotes;
	CComboBox m_wndCon;
	CStatic   m_wndStaticCon;

	CComboBox m_wndCompare;
	CComboBox m_wndData2;
	CStatic   m_wndAndOr;
	CComboBox m_wndData1;
	CStatic   m_wndMiddle;
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormulaBrowser)
	virtual void OnCancel();
	virtual void OnOK();
// 	afx_msg void OnButfind();
// 	afx_msg void OnUpdate();
/*	afx_msg void OnNotes();*/
	virtual BOOL OnInitDialog();
 	afx_msg void OnSelchangeCon();
 	afx_msg void OnSelchangeCompare();
	virtual void PostNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	virtual LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);

protected:
	HSOpenExpressUseData* m_pData;
protected:
	CEditEidolon m_EditEidolon;
	void		 EnableItem(BOOL bEnable);
public:
	SingleSelCondition m_sSingleSel; // ����
	BOOL		 m_nType;      // 1.ѡ��
	HWND		 m_hParent;
	// ���֤ȯ
	CEdit m_wndCode;
	// ��������
	CComboBox m_wndCycle;
	// ����������
	CEdit m_wndDay;
	// ���뼸�κ�ֹͣ����
	CEdit m_wndBuyTimes;
	// �������κ�ֹͣ����
	CEdit m_wndSellTimes;
	// ���������봥��
	CEdit m_wndTriggerTime;
	// ���δ��󴥷�
	CEdit m_wndTrigerNum;
};
