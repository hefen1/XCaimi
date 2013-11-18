
#pragma once
#include "resource.h"
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"
#include "afxcmn.h"
#include <afxwin.h>
#include "KBEdit.h"
/////////////////////////////////////////////////////////////////////////////
#include "EditEidolon.h"


// CDlgStrategyParamConf �Ի���

class CDlgStrategyHQ : public CDialog
{
	DECLARE_DYNAMIC(CDlgStrategyHQ)

public:
	CDlgStrategyHQ(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL);   // ��׼���캯��
	virtual ~CDlgStrategyHQ();

	// �Ի�������
	enum { IDD = IDD_DIALOGStrategyHQ };

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
public:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormulaBrowser)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);

protected:
	HSOpenExpressUseData* m_pData;
protected:
	CEditEidolon m_EditEidolon;
	void		 EnableItem(BOOL bEnable);
	BOOL		 m_bSetVar; //�Ƿ����������˲���
public:
	HWND		 m_hParent;
	// ���֤ȯ
	CKBEdit m_wndCode;
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
	StrategyInfo* m_pStrategy;
 	afx_msg void OnEnChangeEdit1();
	LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeCombo1();
	CEdit m_wndGap;
	CButton m_wndCheck;
	CButton m_wndXD;
};
