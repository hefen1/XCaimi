
/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgCondition.h
*	文件标识：	选股主对话框
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/

#define HS_SELECTSTOCK_ANALYSIS  0x100  // 条件选股
#define HS_SELECTSTOCK_DingZhi   0x101  // 定制选股
#define HS_SELECTSTOCK_ZhiBiao   0x102  // 指标选股
#define HS_SELECTSTOCK_ZhiNeng   0x103  // 智能选股

#define HS_SORT_ANALYSIS         0x200   //指标排序
#define HS_ADD_SORT_ANALYSIS     0x201   //添加指标排序

// #define GB_GUBEN_NAME _T("股本数据")
// #define Cj_GUBEN_NAME _T("财务数据")
// #define	HQ_BASE_NAME	 _T("股票行情字段")

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

	afx_msg void OnBnClickedButfind(); // 查找公式

	// 自定义消息－数据交换
protected:
	long			LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT			OnExchangeData(WPARAM wp,LPARAM lp);
	LRESULT			OnUserDragData(WPARAM wParam, LPARAM lParam);

	LRESULT			OnCommNotify(WPARAM wParam, LPARAM lParam);

	// 选股相关参数
public:
	CArray<StockArea*,StockArea*> m_ayCheck;              // 股票范围
	CString				          m_strBlock;             // 板块名

	SingleSelCondition		      m_sSingleSel;	          // 单个
	UnionSelCondition			  m_pHSUnion;			  // 组合

	SelStockCondition             m_curSelStockCondition; // 当前交换变量 

	CArray<StockUserInfo*,StockUserInfo*> m_AyLastStock;  // 最近一次选股结果
	// 
protected:
	CEditEidolon m_EditEidolon; // 参数修改

	// 对空件禁用或取消禁用
protected:
	void EnableItem(BOOL bEnable);
	void EnableBut(BOOL bEnable);

	// 预处理/执行选股
protected:
	int  AutoSelect();
	int  Select(); // 执行
	void Init();   // 初始化

	// 操作-外部可直接调用
public:
	static CString  GetNameByKey(CString& strFindKeyName);
	static void		InitCWData(CComboBox* pBox,CString strName);
	static void		InitCondition(CComboBox* pBox,int nType);
	static CString  GetDescribe(UnionSelCondition* pHSUnion);

	// 删除
public:
	void		 Delete();
	virtual void MyNcDestroy();

	// 重设大小
public:
	void SetSize();

	// 保存组合条件
public:
	static BOOL SaveLoad(UINT nMode,CExpTabCtrl* pwndTab,CString strFileName = "",DWORD dStyle = 0,CDlgCondition* pCDlgCondition = NULL);	
	static void Serialize(CArchive& ar,int& nCountRef,CExpTabCtrl*	pwndTab);

	// 选中股票总数
protected:
	short   m_nSelStockCount; 

	// 不同类型处理
protected:
	char    m_cType;
	int		m_nID;

public:
	void SetType(char cType); // 设置类型：选股和排序
	BOOL IsType(char cType)  { return (m_cType == cType); }

	// 当前组合是否有效
public:
	BOOL IsValidateUnion(UnionSelCondition* pHSUnion);
	int  DoSelectStock( SelStockCondition*, int style);

public:
	long m_lRefWnd;  

	// 消息窗口,当前接收消息的窗口,一般指向:CHSChildFrame
	HWND m_hMsgWnd;
	HWND m_hParentWnd;

	// gbq add 20060209, 根据名称查找公式
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
	int											  m_nDelayTaskTimer;   // 任务之间间隔时间
	CString										  m_strSelectSavePath; // 自动选股结果路径

	int	 WriteBlock();

public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedDeleteunion();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSCONDITION_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_)
