/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	ScriptEditViewFormula.h
*	文件标识：	公式编辑控件类
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/

#if !defined(_HXSCRIPTEDITVIEW_H_)
#define _HXSCRIPTEDITVIEW_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CompileEditView.h"
#ifndef baseRichView
#define baseRichView CCompileEditView
#endif

class CScriptEditViewFormula : public baseRichView
{
public: // create from serialization only
	CScriptEditViewFormula();
	DECLARE_DYNCREATE(CScriptEditViewFormula)

	// 一些RichEdit控件属性的设置
	void Initialize();

// Attributes
public:
	CDocument* GetDocument();

	// 设置大小写是否敏感
	void SetCaseSensitive(BOOL bSensitive);
	// 设置是否可以更改大小写
	void SetChangeCase(BOOL bChange);

	// 设置注释开头附
	void SetSLComment(TCHAR chComment, TCHAR chComment2 = 0);
	// 设置注释开头符
	void SetSLComment(LPCTSTR lpszComment);
	// 设置引用开头符
	void SetStringQuotes(LPCTSTR lpszStrQ);

	// 加关键字
	virtual void AddKeywords(LPCTSTR lpszKwd);
	// 加常量
	virtual void AddConstants(LPCTSTR lpszKwd);
	// 清除关键字表
	virtual void ClearKeywords();
	// 清除常量表
	virtual void ClearConstants();

	// 设置关键字字体格式clr为RGB颜色, bBold为是否粗体
	void SetKeywordColor(COLORREF clr, BOOL bBold);
	// 设置常量字体格式
	void SetConstantColor(COLORREF clr, BOOL bBold);
	// 设置注释字体格式
	void SetCommentColor(COLORREF clr, BOOL bBold);
	// 设置数字字体格式
	void SetNumberColor(COLORREF clr, BOOL bBold);
	// 设置引用字体格式
	void SetStringColor(COLORREF clr, BOOL bBold);

// Operations
public:
	// 格式化所有文本
	void FormatAll();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptEditViewFormula)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScriptEditViewFormula();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	/******************************************************************
	*	函数功能：	字符ch是否存在于m_strStringQuotes中
	*	函数参数：	TCHAR ch : [in] 要查找的字符
	*	返 回 值：	BOOL : 存在(TRUE), 不存在(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	BOOL IsStringQuote(TCHAR ch);

	/******************************************************************
	*	函数功能：	单词lpszSymbol是否存在于m_strKeywordsLower中
	*	函数参数：	LPCTSTR lpszSymbol : [in] 要查找的单词
	*	返 回 值：	int : 小于0没有, 其它值则为有  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	int  IsKeyword(LPCTSTR lpszSymbol);
	
	/******************************************************************
	*	函数功能：	单词lpszSymbol是否存在于m_strConstantsLower中
	*	函数参数：	LPCTSTR lpszSymbol : [in] 要查找的单词
	*	返 回 值：	int : 小于0没有, 其它值则为有
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	int  IsConstant(LPCTSTR lpszSymbol);


	/******************************************************************
	*	函数功能：	设置字体色彩及是否为粗体
	*	函数参数：	int nStart : [in] 要操作文字的开始索引号
	*				int nEnd   : [in] 要操作文字的结束索引号
	*				BOOL bBold : [in] 是否为粗体
	*				COLORREF clr : [in] 字本色彩
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	void SetFormatRange(int nStart, int nEnd, BOOL bBold, COLORREF clr);

	/******************************************************************
	*	函数功能：	格式化从nStart到nEnd的文本显示格式
	*	函数参数：	int nStart : [in] 文本开始
	*				int nEnd   : [in] 文本结束
	*				BOOL bAll = FALSE : [in] 是否为所有文本
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	void FormatTextRange(int nStart, int nEnd,BOOL bAll = FALSE);

	/******************************************************************
	*	函数功能：	格式化从nStart所在的行到nEnd所在行的文本显示格式
	*	函数参数：	int nStart : [in] 文本开始
	*				int nEnd   : [in] 文本结束
	*	返 回 值：	void 
	*	调用依赖：	FormatTextRange
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	void FormatTextLines(int nStart, int nEnd);

	/******************************************************************
	*	函数功能：	改变从nStart到nEnd的文本为lpszStr
	*	函数参数：	int nStart : [in] 文本开始
	*				int nEnd   : [in] 文本结束
	*				LPCTSTR lpszStr : [in] 要替换的文本
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	void ChangeCase(int nStart, int nEnd, LPCTSTR lpszStr);

	// 字体属性
	struct SymbolColor 
	{
		COLORREF clrColor; // RGB色值
		BOOL bBold;        // 是否使用粗体
	};

	enum ChangeType {ctUndo/*取消*/, ctUnknown/*不确定的原因*/, ctReplSel/*代替选择*/, ctDelete/*按下Del删除*/, ctBack/*按下back space删除*/, ctCut/*剪切*/, ctPaste/*粘贴*/, ctMove/*移动*/};

	// 大小写敏感
	BOOL  m_bCaseSensitive;
	// 可以改变大小写
	BOOL  m_bChangeCase;
	// 注释开头符
	TCHAR m_chComment;
	// 注释开头符
	TCHAR m_chComment2;
	// 注释开头符
	CString m_strComment;
	// 引用开头符
	CString m_strStringQuotes;
	// 关键字列表
	CString m_strKeywords;
	// 小写的关键字列表
	CString m_strKeywordsLower;
	// 常量列表
	CString m_strConstants;
	// 小写的常量列表
	CString m_strConstantsLower;

	// 注释字体格式
	SymbolColor m_icComment;
	// 数字字体格式
	SymbolColor m_icNumber;
	// 引用字体格式
	SymbolColor m_icString;
	// 关键字字体格式
	SymbolColor m_icKeyword;
	// 常量字体格式
	SymbolColor m_icConstant;
	// 其它字体格式
	SymbolColor m_icOtherText;

	// 是否在转化格式
	BOOL		m_bInForcedChange;
	// 更改类型
	ChangeType  m_changeType;	
	// 上一步的选择
	CHARRANGE   m_crOldSel;

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

	/******************************************************************
	*	函数功能：	得到鼠标所在点的单词
	*	函数参数：	CString strText : [in] 当前鼠标所在行的行文本
	*				int nBegin      : [in/out] 开始位置,返回单词开始位置
	*				int nEnd        : [in/out] 结束位置,返回单词结束位置
	*				CPoint point    : [in] 鼠标所在的点
	*	返 回 值：	CString : 鼠标所在点的单词
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual CString	GetTipText(CString strText,int& nBegin,int& nEnd,CPoint point);

// Generated message map functions
protected:
	//{{AFX_MSG(CScriptEditViewFormula)
	// 编辑消息处理
	afx_msg void OnChange();
	// 创建控件消息处理
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// WM_SETTEXT消息响应
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	// EN_PROTECTED消息响应
	afx_msg void    OnProtected(NMHDR*, LRESULT* pResult);

protected:
	// 没做处理
	virtual HRESULT QueryAcceptData(LPDATAOBJECT, CLIPFORMAT*, DWORD,BOOL, HGLOBAL);
};

#ifndef _DEBUG  // debug version in ScriptEditViewFormula.cpp
inline CDocument* CScriptEditViewFormula::GetDocument()
   { return (CDocument*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_HXSCRIPTEDITVIEW_H_)
