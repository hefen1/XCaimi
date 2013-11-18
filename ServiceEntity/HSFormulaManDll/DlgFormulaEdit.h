/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgFormulaEdit.h
*	文件标识：	公式编辑器类定义
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/
/*#include "afxcmn.h"*/
/*#include <afxwin.h>*/
#if !defined(AFX_EDIUPDATETEXP_H__EDBF85C6_1102_11D5_A107_E07528000000__INCLUDED_)
#define AFX_EDIUPDATETEXP_H__EDBF85C6_1102_11D5_A107_E07528000000__INCLUDED_
#include "HSBaseDialog.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFormulaEdit.h : header file
//
//#include "ETSLayout.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaEdit dialog

#include "Value.h"
#include "GridCtrl.h"
class CTreeCtrlFormula;

/*************************************************************
*	类 名 称：	CDlgFormulaEdit
*	类 描 述：	公式编辑器窗口
***************************************************************/
class CDlgFormulaEdit : public /*CHSBaseDialog*/CHSBaseDialog
{
// Construction
public:
	CDlgFormulaEdit(CWnd* pParent = NULL);   // standard constructor
	~CDlgFormulaEdit();

	/* DialogMgr: Add this: */
//	DECLARE_LAYOUT();
	/************************/

// Dialog Data
	//{{AFX_DATA(CDlgFormulaEdit)
	enum { IDD = IDD_FORMULA };
	// 此三项没用, 是否去除 ??? 
	CComboBoxEx	m_wndComboxType;
	CComboBoxEx	m_wndComboxComment;
	CComboBoxEx	m_wndExpComment;

	// 是否为主图(1主图, 0副图)
	int			m_nMainChart;        
	// 公式定义汉字说明窗口
	CEdit       m_wndPromte;  //by HS

	int  m_CharType;  //记录开始时图的类型 为主图还是副图 qinhn 20090727 Add
	
	//}}AFX_DATA

	// 公式中风格设置:参见 #define HS_EXP_FINANCE 等定义
	DWORD	m_dStyle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFormulaEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CDlgFormulaEdit)
	// 初始化
	virtual BOOL OnInitDialog();
	// "确定"
	virtual void OnOK();
	// "取消"
	virtual void OnCancel();
	// "函数管理"
	afx_msg void OnInfunction();
	// "编译"
	afx_msg void OnCompile();
	// 保存 gbq add 20060427
	afx_msg void OnSave();
	// "主图"单选按钮
	afx_msg void OnMainchart();
	// "副图"单选按钮
	afx_msg void OnSubchart();
	// "密码"选择按钮
	afx_msg void OnPwd();
	// "禁用周期"
	afx_msg void OnStopperiod();
	// "用法注释"
	afx_msg void OnNotes();
	// "参数精灵"
	afx_msg void OnEidolon();
	// 密码输出框失去焦点
	afx_msg void OnKillfocusPwdedit();
	// 调出错误信息窗口(目前好象已不用)
	afx_msg void OnError();
	// "插入函数"
	afx_msg void OnButfun();
	// "插入公式"
	afx_msg void OnButexp();
	// "查找"(目前好象已不用)
	afx_msg void OnButfind();
	// m_pRichEdit(公式定义显示)控件Replace处理
	afx_msg void OnEditReplace();
	// m_pRichEdit(公式定义显示)控件COPY处理
	afx_msg void OnButcopy();
	// m_pRichEdit(公式定义显示)控件Paste处理
	afx_msg void OnEditPaste();
	// 改变窗口大小
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// 使m_pGridCtrl(参数列表)得到焦点
	void OnEditParam();
	

	/******************************************************************
	*	函数功能：	加公式处理(公式管理发送过来的消息)
	*	函数参数：	WPARAM wParam = 0 : [in] 无意义
	*				LPARAM lParam = 0 : [in] CExpression*类型,要加入的公式指针
	*	返 回 值：	long : 恒为1
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);

	// 设置窗口大小
	void SetSize();

public:
	// 公式体文本编辑控件
	class CCompileEditView*	m_pRichEdit;
	// 参数设置控件
	CGridCtrl	m_WndGridCtrl;
	// 公式管理器窗口
	class CDlgFormulaMan*		m_pFormulaMan;

	// 参数精灵窗口
	static class CDlgNotesAndEidolon* m_pDlgEidolon;
	// 注释窗口
	static class CDlgNotesAndEidolon* m_pDlgNotes;

	// 删除公式编辑器点用的所有资源
	void Delete();
	// 一般调用他是为了隐藏几个相关的窗口:注释,参数精灵,公式管理器及本身
	void Show(int nShow);

public:
	// 创建新公式处理
	void OnNewExpress();
	// 快速新建公式
	//void SpeedCreateExpress();

protected:
	// 密码
	CString	     m_strFistPWD;
	// 函数管理器窗口	
	static class CDlgFunctionMan* m_pFunctionMan;
	// 公式编辑类型(新键还是更改)
	short		  m_cType;
	// 新建公式时用
	//char		  m_cSpeedNew; 
public:
	// 类型操作
	enum Type{ 
			   none     = 0x01,
			   Update   = 0x02 /*更改公式*/,
			   NewExp   = 0x04 /*新公式*/, 
		       SpeedNew = 0x08 /*快速新建公式*/
	          };

	int  IsType(short cType)  { return (m_cType & cType); }
	void SetType(short cType) { m_cType = cType; }
	//void SetExType(char cType) { m_cSpeedNew = cType;}
		
public:
	// 错误信息界面
	static class CDlgError* m_pErrorDlg;

public:
	/******************************************************************
	*	函数功能：	根据公式类型及公式名称设置窗口标题
	*	函数参数：	int nType		: [in] 公式类型
	*				CString strName : [in] 公式名称
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	void SetTitle(int nType,CString strName);

	//
protected:
	// 插入公式或函数时显示的树
	CTreeCtrlFormula*  m_pEditTree;

	/******************************************************************
	*	函数功能：	插入公式或函数处理 
	*	函数参数：	WPARAM wParam = 0 : [in] 加入公式CTreeCtrlFormula::EditExpRef, 加入函数CTreeCtrlFormula::EditFunRef
	*				LPARAM lParam = 0 : [in] 加入函数时为CString*, 加入公式时为CValue*
	*	返 回 值：	long : 恒为0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	afx_msg long OnSendAddStr(WPARAM wParam = 0, LPARAM lParam = 0);

public:
	// 创建加入公式或函数时显示的树控件m_pEditTree
	CTreeCtrlFormula* CreateEditTree();

	/******************************************************************
	*	函数功能：	从外部函数表或公式表中得到函数或公式
	*	函数参数：	CString strName : [in] 函数或公式名
	*				short& nRet		: [in/out] CTreeCtrlFormula::EditFunRef在外部函数表里找, 同时将nRet赋为CExpression::ExpExternFun
											   CTreeCtrlFormula::EditExpRef在外部公式表里找, 同时将nRet赋为CExpression::ExpExternExp
	*	返 回 值：	CValue*  : 找不到则为NULL, 否则为找到的公式或函数的指针
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	CValue*  GetVariable(CString strName,short& nRet);

	/******************************************************************
	*	函数功能：	显示公式或函数的树
	*	函数参数：	CRect& RefRect : [in] 显示的大小及位置
	*				int nCmdShow   : [in] 显示(SW_SHOW)还是隐藏(SW_HIDE)
	*				int nType	   : CExpression::ExpExternFun显示函数树, 其它则显示公式树
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	void ShowEditTree(CRect& RefRect,int nCmdShow,int nType);

	/******************************************************************
	*	函数功能：	将外部函数表中所有函数类型为nType的函数都加入到树pTree是strRoot节点下
	*	函数参数：	CString strRoot		: [in] 要操作的树pTree的根节点
	*				int nType			: [in] 函数类型
	*				CTreeCtrlFormula* pTree  : [in] 要操作的树
	*	返 回 值：	void 
	*	调用依赖：	CTreeCtrlFormula::InputFunItem()
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	void AddFunByType(CString strRoot,int nType,CTreeCtrlFormula* pTree);
	 //添加参数列表
	void AddToParamerList(CExpression* pExp);

public:
	// 公式类型
	int		m_nExpressType;
	// 旧的公式名
	CString m_strOldName;
	// 是否按"确定"按钮退出窗口
	BOOL	m_bOkFlag;
	HACCEL m_hAccelTable;

	static bool bNeedChangeStyle;  //qinhn 20070727 Add 
	static BOOL bEscMsgOfDlgEdit;
	
	// begin by HS 2002.3.28
protected:
	// 使用默认公式定义
	afx_msg void OnDefault();

	// 默认公式处理相关结构
	struct ReloadDefault
	{
		CString m_strName;	// 公式名
		CString m_strSource;// 公式体定义

		void Clear() 
		{
			m_strName   = "";
			m_strSource = "";
		}
		// 串行化,bStore是否写操作
		void Serialize(bool bStore);
	};
	
	// 默认操作数据
	ReloadDefault m_RDDefault;
	// end by HS 2002.3.28
	
public:
	// WIN消息处理,得到控件画刷
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// 公式属组列表控件
	CComboBox m_wndComBox;
	// ??? 不在使用,建议删除
	HTREEITEM m_hComboxItem;
	// 公式属组列表控件改变选择处理
	afx_msg void OnComBoxSelChange();
	// 编译结果及动态翻译TAB改变选择处理
	afx_msg void OnTcnSelchangeTabout(NMHDR *pNMHDR, LRESULT *pResult);
	// 编译结果及动态翻译TAB点击处理
	afx_msg void OnNMClickTabout(NMHDR *pNMHDR, LRESULT *pResult);
	// 编译结果及动态翻译TAB控件
	CTabCtrl m_wndTabOut;

public:
	enum { Translate/*动态翻译*/,CompileResult/*编译结果*/ };

	/******************************************************************
	*	函数功能：	显示动态翻译或编译结果处理
	*	函数参数：	int nIndex : [in] Translate显示动态翻译, CompileResult则显示编译结果
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	void ShowResult(int nIndex);

public:
	void OnMenu(UINT nID);
	void OnUpdateMenu(CCmdUI* pCmdUI);

	void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//表格初始化
	void initGridCtrl(int nRow=100);
	BOOL ShowWindow( int nCmdShow );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDIUPDATETEXP_H__EDBF85C6_1102_11D5_A107_E07528000000__INCLUDED_)
