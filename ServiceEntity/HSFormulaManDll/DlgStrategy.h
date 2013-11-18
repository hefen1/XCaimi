#pragma once
#include "resource.h"
#include "KBEdit.h"
#include "afxdtctl.h"
#include <afxwin.h>
#include "ExpPublic.h"
#include "TreeCtrlFormula.h"
#include "exptabctrl.h"
#include "EditEidolon.h"
// CDlgStrategy �Ի���
class CDlgStrategy : public CDialog
{
	DECLARE_DYNAMIC(CDlgStrategy)

public:
	CDlgStrategy(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL);   // ��׼���캯��
	virtual ~CDlgStrategy();

// �Ի�������
	enum { IDD = IDD_TAB_STRATEGY_HQ };
	BOOL OnInitDialog();
	HWND	m_hParent;
	long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	void EnableItem(BOOL bEnable);
	BOOL GetTrigData(StrategyTrig* pTrig,BOOL IsByDay = FALSE);
	void SetTrigData(StrategyTrig* pTrig);
	void SetParam();
	void SetRDOnly(BOOL ro=TRUE);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	HSOpenExpressUseData* m_pData;
	DECLARE_MESSAGE_MAP()
	CDateTimeCtrl m_wndEffectStart;
	CDateTimeCtrl m_wndEffectEnd;
	CComboBox m_wndPeriodType;
	CEdit m_wndPeriodNum;
	CButton m_wndFuQ;
	CEdit m_wndMaxBuy;
	CEdit m_wndMaxSell;
	CEdit m_wndGap;
	CEditEidolon m_EditEidolon;
	StrategyTrig *m_pTrig;
public:
	CExpTabCtrl m_wndTab;
	CStatic   m_wndConNotes;
	CComboBox m_wndCon;
	CStatic   m_wndStaticCon;

	CComboBox m_wndCompare;
	CComboBox m_wndData2;
	CStatic   m_wndAndOr;
	CComboBox m_wndData1;
	CStatic   m_wndMiddle;
	afx_msg void OnCbnSelchangeCombocycle();
	void Serialize(CArchive & ar,CExpValue *& exp);//������д��ʽ
	BOOL SaveAllTrig(CMapWordToPtr *m_map);//����m_map�е����в���
	static BOOL IsEqual(StrategyTrig *trig1,StrategyTrig *trig2);//�ж����������Ƿ�һ��
	static WORD GetStrategyTrigKey(CMapWordToPtr *&m_map,char* TrigName);
	static void* GetStrategyTrig(CMapWordToPtr *&m_map,char* TrigName);
	static WORD AddStrategyTrig(CMapWordToPtr *&m_map,void* pTrig);
	static BOOL DelStrategyTrig(CMapWordToPtr *&m_map,WORD TrigID);
	BOOL PreTranslateMessage(MSG* pMsg);
};
