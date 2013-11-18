/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	CompileEditView.h
*	文件标识：	公式编辑控件基类及其它一些公式控件要关类的定义
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_COMPILEEDITVIEW_H__174390D7_AB9C_11D4_A12C_00E09833EFFA__INCLUDED_)
#define AFX_COMPILEEDITVIEW_H__174390D7_AB9C_11D4_A12C_00E09833EFFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CompileEditView.h : header file
//
#include "ExpPublic.h"

/*************************************************************
*	类 名 称：	HSAddStrCompileEdit
*	类 描 述：	插入公式或函数时使用的中间变量
***************************************************************/
struct HSAddStrCompileEdit
{
	short    m_nCharPos;	// m_strText的长度
	CString* m_strText;     // 指向公式或是函数名字串
	HSAddStrCompileEdit()
	{
		memset(this,0,sizeof(HSAddStrCompileEdit));
	}
	~HSAddStrCompileEdit()
	{
		if(m_strText != NULL)
		{
			delete m_strText;
			m_strText = NULL;
		}
	}
};

/*************************************************************
*	类 名 称：	HSTreeToCompileEdit
*	类 描 述：	删除公式时使用的中间变量结构
***************************************************************/
struct HSTreeToCompileEdit
{
	void*	  m_pName; // 指向CTreeGroup对象
	void*	  m_pData; // 指向HTREEITEM对象
	HSTreeToCompileEdit(void* pName,void* pData)
	{
		m_pName = pName;
		m_pData = pData;
	}	
};

#define SAVEMENU_BEGIN WM_USER + 1000
#define SAVEMENU_END   WM_USER + 2000

class CRichEditDocEx : public CRichEditDoc
{
public:
	CRichEditDocEx(CView* pView = NULL);
	~CRichEditDocEx();
public:
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo = NULL) const;
	virtual CRichEditView* GetView() const { return (CRichEditView*)m_pView; }
protected:
	CView*			   m_pView;
	CRichEditCntrItem* m_pRichEditCntrItem;
};

class COnlineList;

/*************************************************************
*	类 名 称：	CCompileEditView
*	类 描 述：	公式编辑控件类
***************************************************************/
class CCompileEditView : public CRichEditView
{
// Construction
public:
	CCompileEditView();
	DECLARE_DYNCREATE(CCompileEditView)


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompileEditView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCompileEditView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCompileEditView)
	// 创建消息处理
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// 鼠标右键按下处理
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	// 键盘按下处理
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	// 键盘UP处理
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	// 鼠标移动处理
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// 鼠标左键按下处理
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// 鼠标左键弹起处理
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// 鼠标左键双击处理
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// 鼠标右键弹起处理
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	// 鼠标右键双击处理
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	// 设置鼠标图标处理
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	// 字符消息处理
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	// 注释,没实现 ??? 
	afx_msg void OnNote(); 
	// 参数精灵,没实现 ???
	afx_msg void OnNumen();
	// 禁用周期, 此功能应禁去 ??? 
	afx_msg void OnBanpieriods(); 
	// 公式编译
	afx_msg void OnCompile(); 
	// 保存公式
	afx_msg void OnOk(); 
	// 新建公式处理, 没做任何处理
	afx_msg void OnNewExpress();

	// 以下三个均没实现 ??? 
	afx_msg void OnExpressOut();
	afx_msg void OnExpressIn();
	afx_msg void OnEngToChinese();

	// 添加'+', '-', '*', '/', '(', ')', ':=', ':', '^'符号处理
	afx_msg void OnSymbolMsg(UINT nID);

	/******************************************************************
	*	函数功能：	加编译信息到错误列表窗口
	*	函数参数：	WPARAM wParam = 0 : [in] 无意义
	*				LPARAM lParam = 0 : [in] nodErrorInfo*类型数据,错误信息
	*	返 回 值：	long : 恒为0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	afx_msg long AddCompileInfotoList(WPARAM wParam = 0, LPARAM lParam = 0);

	/******************************************************************
	*	函数功能：	插入公式或函数操作
	*	函数参数：	WPARAM wParam = 0 : [in] HX_EXPRESSSENTENCE 插入公式, 否则是插入函数
	*				LPARAM lParam = 0 : [in] HSAddStrCompileEdit*类型数据,HSAddStrCompileEdit::m_strText为插入的公式指针或函数名指针
	*	返 回 值：	long : 恒为0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	afx_msg long OnSendAddStr(WPARAM wParam = 0, LPARAM lParam = 0);
	// HX_USER_LOADCUR消息响应,没有任何功能
	afx_msg long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);

	/******************************************************************
	*	函数功能：	HX_USER_DELETECUR消息响应,从外部公式表中删除公式
	*	函数参数：	WPARAM wParam = 0
	*				LPARAM lParam = 0 : [in] HSTreeToCompileEdit*类型数据
											 HSTreeToCompileEdit::m_pName为CTreeGroup*类型数据
											 它包含了要删除的公式的信息
	*	返 回 值：	long : 输入删除公式的密码有误(2), 其它情况恒为0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	afx_msg long DeleteCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	
	/******************************************************************
	*	函数功能：	此功能没使用,它实现了从菜单中选择行情函数,加入公式定义体中
	*	函数参数：	UINT nID : [in] 加函数菜单ID
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	afx_msg void OnFunListMsg(UINT nID);

	// 没有本功能(菜单以不存在) ??? 
	afx_msg void SaveDataMenu(UINT nID);

	// 没有实现( ??? )
	afx_msg void OnProperty();

	// 响应菜单"拷贝"功能
	afx_msg void OnCopy();

	// 只调用了基类功能
	int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);

public:
	CMapVariabile*  m_pExternExpression;     // 外部表达式表
	CMapVariabile*  m_pExternVariabile;		 // 外部变量表
	CMapVariabile*  m_pExternFunction;		 // 外部函数表
	
public:
	// 初始化外部表达式表,外部变量表,外部函数表及初始化已定义的函数
	void InitData();
	// 得到外部表达式表指针
	CMapVariabile*  GetExpression() { return m_pExternExpression; }     // 外部表达式表

protected:
	// 不再使用,优化后应删除 ??? 
	CDialogBar*		     m_pwndDlgMainBar;
	// 不再使用,优化后应删除 ??? 
	CListCtrl*			 m_pListCtrl;
	// 不再使用,优化后应删除 ??? 
	class CWorkspaceWnd* m_pWorkspaceWnd;
	// 不再使用,优化后应删除 ??? 
	class CGridCtrl*	 m_pVarGridCtrl;

	// 右键弹出菜单
	CMenu*				 m_pPopupMenu;
	// 控件类型(定义了四种),其实能正确使用的只有COMPILE类型
	int					 m_nCompileEdit;
	// 没用使用, 优化后应删除 ??? 
	CString				 m_strCurExpressName;

	// 鼠标指针类型, TRUE使用IDC_ARROW, FALSE使用IDC_IBEAM
	BOOL				 m_bArrowCur;

protected:
	// 由于全局菜单g_pSaveMenu不存在,所以,此成员不起什么做用, 优化后应删除 ??? 
	HTREEITEM			m_hExpressInTreePos;	
	// 没用使用,优化后应删除 ??? 
	struct CTreeGroup*	m_pCurEditTreeGroup;

protected:
	// 初始化已定义的函数
	void  InitSysFunction();
	// 一些RichEdit控件属性的设置
	void  Initialize();
	// 由于全局菜单g_pSaveMenu不存在,所以,此成员不起什么做用, 优化后应删除 ??? 
	BOOL  CreateSaveMenu();

public:
	// 不再使用,优化后应删除 ??? 
	void  Atatch(CListCtrl* pListCtrl){m_pListCtrl = pListCtrl;}
	// 不再使用,优化后应删除 ??? 
	/*void  Atatch(class CWorkspaceWnd* pFunListView);*/
	// 不再使用,优化后应删除 ??? 
	void  Atatch(CDialogBar* pwndDlgMainBar)   {m_pwndDlgMainBar = pwndDlgMainBar;}
	// 不再使用,优化后应删除 ??? 
	void  Atatch(class CGridCtrl* pVarGridCtrl){m_pVarGridCtrl = pVarGridCtrl;}	

	// 设置控件类型
	void  SetCompileEdit(int nCompileEdit);
	// 得到控件类型(编辑器还是HELP)
	BOOL  IsCompileEdit(int nType) {return (m_nCompileEdit&nType);}

public:
	// 装入公式数据,目前以没有使用,优化后应否删除 ??? 
	void Init(short nType);

	/******************************************************************
	*	函数功能：	编译所定义的公式
	*	函数参数：	CDlgFormulaEdit* pDlg : [in] 公式管理器窗口
	*				LPARAM lParam = 0    : [in] 无意义
	*	返 回 值：	long 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	long Compile(class CDlgFormulaEdit* pDlg, LPARAM lParam = 0);

	// 装入公式数据,目前以没有使用,优化后应否删除 ??? 
	BOOL SaveLoadMap(BOOL bSave,CMapVariabile& ayVarMap,CString strFileName = "");	


	// 控件类型
	enum hxEditType {COMPILE = 0X0001/*公式编辑*/,HELP = 0X0002/*帮助*/,NOTES = 0X0004/*注释*/,EIDOLON = 0X0008/*参数精灵*/};

public:
	// 设定关键字及常量
	void SetKeywordsConstants();	
	// 加到关键字字串中
	virtual void AddKeywords(LPCTSTR lpszKwd)  {}
	// 加到常量字串中
	virtual void AddConstants(LPCTSTR lpszKwd) {}
	// 清空关键字字串
	virtual void ClearKeywords()  {}
	// 清空常量字串
	virtual void ClearConstants() {}

public:
	// 禁用周期窗口,目前不再带有次功能, 优化后应删除 ???
	class CDlgEstopPeriod* m_pEstopPeriodDlg;

public:
	// 没有实现什么功能, 优化后应删除 ???
	virtual BOOL OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );
public:
	// 只有UpdateExpress看到使用,其它好象都没有什么做用
	enum LoadExpressType { None/*正常*/,TreeMsg/*树消息*/,UpdateExpress/*更新公式*/,ComboxMsg/*下接列表消息*/,UpdateNotes/*更新注释*/};

public:
	// 公式编译器窗口指针
	static class CDlgFormulaEdit* m_pParentDlg;

protected:
	// 提示信息窗口
	/*CXInfoTip		m_pDatatip;*/
	// 创建提示信息窗口
	BOOL   		    IsDataTip();

	/******************************************************************
	*	函数功能：	处到鼠标指针所在点的TIP帮助文本
	*	函数参数：	CString& strTitle : [out] 得到的指针坐标所在单词
	*				UINT nFlags		  : [in] 铵钮FLAGS
	*				CPoint point	  : [in] 指针坐标
	*				int* nBegin = NULL: [out] 指针坐标所在单词开始索引号(在全文索引中)
	*				int* nEnd = NULL  : [out] 指针坐标所在单词结束索引号(在全文索引中)
	*	返 回 值：	CString : 得到的TIP文本
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual CString	GetTipText(CString& strTitle,UINT nFlags, CPoint point,int* nBegin = NULL,int* nEnd = NULL);

	//	无用, 是否要删除 ???
	virtual CString	GetTipText(CString strText,int nBegin,int nEnd,CPoint point) { return ""; }

	/******************************************************************
	*	函数功能：	根据函数名得到注释信息
	*	函数参数：	CString strKey : [in] 函数名
	*	返 回 值：	CString : 注释信息
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual CString	GetKeyHelp(CString strKey);

	/******************************************************************
	*	函数功能：	得到strText中,strText[nStart]字符所在的单词(连续的字母或中文字串)
	*	函数参数：	CString strText : [in] 要处理的字串
	*				int& nStart     : [in] 要处理的点(索引号)
	*				int nCount      : [in] 字符长度
	*	返 回 值：	CString : strText[nStart]字符所在的单词(连续的字母或中文字串), 否则为空
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual CString	FindWord(CString strText,int& nStart,int nCount);

	/******************************************************************
	*	函数功能：	得到strText左边或右边连继的字符串(字母或中文)
	*	函数参数：	CString strText : [in] 要处理的字串
	*				BOOL bLeft : [in] 得到左边字串(TRUE), 得到右边字串(FALSE)
	*	返 回 值：	CString : 得到的单词
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual CString	CutWord(CString strText,BOOL bLeft);
public:
	// 掩藏TIP提示
	void	HideDataTip();

protected:
	// 在线翻译相关,目前此功能还没有实现, 暂不说明
	BOOL         m_bInputChar;
	// 在线翻译相关,翻译结果显示
	COnlineList* m_pOnlineList;
	// 创建翻译结果显示控件
	COnlineList* CreateOnlineList();

public:
	// 显示在线翻译,在线翻译相关,目前此功能还没有实现, 暂不说明
	void		 ShowOnline(int nCmd,BOOL bForce = FALSE);
	// 是否显示在线翻译,在线翻译相关,目前此功能还没有实现, 暂不说明
	BOOL		 IsOnlineShow();
	// 以去除此消息,优化时是否应删除 ???
	BOOL		 OnReturn(MSG* pMsg);
	// 在线翻译帮助, 在线翻译相关,目前此功能还没有实现, 暂不说明
	void		 OnEditOnlineHelp();
	// 显示在线翻译,在线翻译相关,目前此功能还没有实现, 暂不说明
	void		 ShowOnlineList(CPoint& point,int nCmdShow,CString strText = "",
								BOOL bEdit = FALSE,int nBegin = 0,int nEnd = 0);
	// 编辑在线翻译,在线翻译相关,目前此功能还没有实现, 暂不说明
	virtual void OnEdit(int nBegin,int nEnd,CString strText,BOOL bEdit = FALSE);

protected:
	// 调用了基类的OnEditProperties函数,所以,所有参数均无意义
	virtual HRESULT QueryAcceptData(LPDATAOBJECT, CLIPFORMAT*, DWORD,BOOL, HGLOBAL);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPILEEDITVIEW_H__174390D7_AB9C_11D4_A12C_00E09833EFFA__INCLUDED_)
