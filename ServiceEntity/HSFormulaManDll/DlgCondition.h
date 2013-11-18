
/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgCondition.h
*	�ļ���ʶ��	ѡ�����Ի���
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/

#define HS_SELECTSTOCK_ANALYSIS  0x100  // ����ѡ��
#define HS_SELECTSTOCK_DingZhi   0x101  // ����ѡ��
#define HS_SELECTSTOCK_ZhiBiao   0x102  // ָ��ѡ��
#define HS_SELECTSTOCK_ZhiNeng   0x103  // ����ѡ��

#define HS_SORT_ANALYSIS         0x200   //ָ������
#define HS_ADD_SORT_ANALYSIS     0x201   //���ָ������

// #define GB_GUBEN_NAME _T("�ɱ�����")
// #define Cj_GUBEN_NAME _T("��������")
// #define	HQ_BASE_NAME	 _T("��Ʊ�����ֶ�")

#if !defined(AFX_HSCONDITION_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_)
#define AFX_HSCONDITION_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HSCondition.h : header file
//
#include "resource.h"
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"
#include "ExpPublic.h"
#include "CHSProgressCtrl.h"
#include "EditEidolon.h"
#include "HSBaseDialog.h"
#include "HSShadeButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgCondition dialog

/*#define CButton     CHSShadeButton*/


class CDlgCondition : public /*CDialog*/ CHSBaseDialog
{
	DECLARE_DYNAMIC(CDlgCondition)
public:
	enum { ConditionType,Sort };
	
// Construction
public:
	CDlgCondition(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL,
				  char cType = ConditionType,
				  long* pThis = NULL,
				  int nID = IDD_CONDITION_STOCK);   // standard constructor
	~CDlgCondition();
	virtual INT_PTR DoModal();
	//virtual void ExitRunModalLoop(DWORD dwFlags = 0);
	int			 RunModalLoop(DWORD dwFlags = 0);
// Dialog Data
	//{{AFX_DATA(CDlgCondition)
	enum { IDD = IDD_CONDITION_STOCK };
	
	CStatic	m_wndBottomPos;
	CStatic	m_wndConditionPos;
	CStatic	m_wndSelStockResult;
	CStatic	m_wndStaticCon;
	CStatic	m_wndMiddle;
	CStatic	m_wndPrompt;
	CStatic		m_wndPeriodStc;
	CComboBox	m_wndPeriod;
	CStatic		m_wndNotes;
	CComboBox	m_wndData2;
	CComboBox	m_wndData1;
	CStatic		m_wndConNotes;
	CComboBox	m_wndCon;
	CComboBox	m_wndCompare;
	CStatic		m_wndAndOr;
	CExpTabCtrl	m_wndTab;
	CStatic		m_wndParam;
// 	CHSShadeButton  m_wndSwitchBut;
// 	CHSShadeButton	m_wndNewUnion;
// 	CHSShadeButton	m_wndUpdateUnion;
// 	CHSShadeButton  m_wndDeleteUnion;
// 	CHSShadeButton	m_wndExpNotes;
	//}}AFX_DATA

	CStatic			    m_wndPromptText;
	CHSProgressCtrl	m_cProgress;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCondition)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDlgCondition)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAdvance();
	virtual void OnCancel();
	afx_msg void OnUsernotes();
	afx_msg void OnPresucc();
	virtual void OnOK();
	afx_msg void OnToblock();
	afx_msg void OnAutoSelect();
	afx_msg void OnUpDateNowData();
	afx_msg void OnSelchangeCompare();
	afx_msg void OnSelchangeCon();
	afx_msg void OnNewunion();
	afx_msg void OnUpdateunion();
	afx_msg void OnButhideleft();
	afx_msg void OnSaveasblock();
	afx_msg void OnButexpress();
	afx_msg void OnButparam();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedButfind(); // ���ҹ�ʽ

	// �Զ�����Ϣ�����ݽ���
protected:
	long			LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT			OnExchangeData(WPARAM wp,LPARAM lp);
	LRESULT			OnUserDragData(WPARAM wParam, LPARAM lParam);

	LRESULT			OnCommNotify(WPARAM wParam, LPARAM lParam);

	// ѡ����ز���
public:
	CArray<StockArea*,StockArea*> m_ayCheck;              // ��Ʊ��Χ
	CString				          m_strBlock;             // �����

	SingleSelCondition		      m_sSingleSel;	          // ����
	UnionSelCondition			  m_pHSUnion;			  // ���

	SelStockCondition             m_curSelStockCondition; // ��ǰ�������� 

	CArray<StockUserInfo*,StockUserInfo*> m_AyLastStock;  // ���һ��ѡ�ɽ��
	// 
protected:
	CEditEidolon m_EditEidolon; // �����޸�

	// �Կռ����û�ȡ������
protected:
	void EnableItem(BOOL bEnable);
	void EnableBut(BOOL bEnable);

	// Ԥ����/ִ��ѡ��
protected:
	int  AutoSelect();
	int  Select(); // ִ��
	void Init();   // ��ʼ��

	// ����-�ⲿ��ֱ�ӵ���
public:
	static CString  GetNameByKey(CString& strFindKeyName);
	static void		InitCWData(CComboBox* pBox,CString strName);
	static void		InitCondition(CComboBox* pBox,int nType);
	static CString  GetDescribe(UnionSelCondition* pHSUnion);

	// ɾ��
public:
	void		 Delete();
	virtual void MyNcDestroy();

	// �����С
public:
	void SetSize();

	// �����������
public:
	static BOOL SaveLoad(UINT nMode,CExpTabCtrl* pwndTab,CString strFileName = "",DWORD dStyle = 0,CDlgCondition* pCDlgCondition = NULL);	
	static void Serialize(CArchive& ar,int& nCountRef,CExpTabCtrl*	pwndTab);

	// ѡ�й�Ʊ����
protected:
	short   m_nSelStockCount; 

	// ��ͬ���ʹ���
protected:
	char    m_cType;
	int		m_nID;

public:
	void SetType(char cType); // �������ͣ�ѡ�ɺ�����
	BOOL IsType(char cType)  { return (m_cType == cType); }

	// ��ǰ����Ƿ���Ч
public:
	BOOL IsValidateUnion(UnionSelCondition* pHSUnion);
	int  DoSelectStock( SelStockCondition*, int style);

public:
	long m_lRefWnd;  

	// ��Ϣ����,��ǰ������Ϣ�Ĵ���,һ��ָ��:CHSChildFrame
	HWND m_hMsgWnd;
	HWND m_hParentWnd;

	// gbq add 20060209, �������Ʋ��ҹ�ʽ
public:
	CString m_strExpression;
	CExpression* FindExpression(CString strName);
	CExpression* ShowExpression(CString strName);
	CWnd*		 GetParentEx();
	BOOL		 ShowWindow( int nCmdShow );
	// gbq add end;

protected:
	CArray<UnionSelCondition*,UnionSelCondition*> m_ayUnionSelCondition;
	int											  m_nCurPos;
	int											  m_nStopAutoRun;
	int											  m_nDelayTimer;
	int											  m_nDelayTaskTimer;   // ����֮����ʱ��
	CString										  m_strSelectSavePath; // �Զ�ѡ�ɽ��·��

	int	 WriteBlock();

public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedDeleteunion();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSCONDITION_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_)
