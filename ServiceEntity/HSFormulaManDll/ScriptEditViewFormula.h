/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	ScriptEditViewFormula.h
*	�ļ���ʶ��	��ʽ�༭�ؼ���
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
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

	// һЩRichEdit�ؼ����Ե�����
	void Initialize();

// Attributes
public:
	CDocument* GetDocument();

	// ���ô�Сд�Ƿ�����
	void SetCaseSensitive(BOOL bSensitive);
	// �����Ƿ���Ը��Ĵ�Сд
	void SetChangeCase(BOOL bChange);

	// ����ע�Ϳ�ͷ��
	void SetSLComment(TCHAR chComment, TCHAR chComment2 = 0);
	// ����ע�Ϳ�ͷ��
	void SetSLComment(LPCTSTR lpszComment);
	// �������ÿ�ͷ��
	void SetStringQuotes(LPCTSTR lpszStrQ);

	// �ӹؼ���
	virtual void AddKeywords(LPCTSTR lpszKwd);
	// �ӳ���
	virtual void AddConstants(LPCTSTR lpszKwd);
	// ����ؼ��ֱ�
	virtual void ClearKeywords();
	// ���������
	virtual void ClearConstants();

	// ���ùؼ��������ʽclrΪRGB��ɫ, bBoldΪ�Ƿ����
	void SetKeywordColor(COLORREF clr, BOOL bBold);
	// ���ó��������ʽ
	void SetConstantColor(COLORREF clr, BOOL bBold);
	// ����ע�������ʽ
	void SetCommentColor(COLORREF clr, BOOL bBold);
	// �������������ʽ
	void SetNumberColor(COLORREF clr, BOOL bBold);
	// �������������ʽ
	void SetStringColor(COLORREF clr, BOOL bBold);

// Operations
public:
	// ��ʽ�������ı�
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
	*	�������ܣ�	�ַ�ch�Ƿ������m_strStringQuotes��
	*	����������	TCHAR ch : [in] Ҫ���ҵ��ַ�
	*	�� �� ֵ��	BOOL : ����(TRUE), ������(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	BOOL IsStringQuote(TCHAR ch);

	/******************************************************************
	*	�������ܣ�	����lpszSymbol�Ƿ������m_strKeywordsLower��
	*	����������	LPCTSTR lpszSymbol : [in] Ҫ���ҵĵ���
	*	�� �� ֵ��	int : С��0û��, ����ֵ��Ϊ��  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	int  IsKeyword(LPCTSTR lpszSymbol);
	
	/******************************************************************
	*	�������ܣ�	����lpszSymbol�Ƿ������m_strConstantsLower��
	*	����������	LPCTSTR lpszSymbol : [in] Ҫ���ҵĵ���
	*	�� �� ֵ��	int : С��0û��, ����ֵ��Ϊ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	int  IsConstant(LPCTSTR lpszSymbol);


	/******************************************************************
	*	�������ܣ�	��������ɫ�ʼ��Ƿ�Ϊ����
	*	����������	int nStart : [in] Ҫ�������ֵĿ�ʼ������
	*				int nEnd   : [in] Ҫ�������ֵĽ���������
	*				BOOL bBold : [in] �Ƿ�Ϊ����
	*				COLORREF clr : [in] �ֱ�ɫ��
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	void SetFormatRange(int nStart, int nEnd, BOOL bBold, COLORREF clr);

	/******************************************************************
	*	�������ܣ�	��ʽ����nStart��nEnd���ı���ʾ��ʽ
	*	����������	int nStart : [in] �ı���ʼ
	*				int nEnd   : [in] �ı�����
	*				BOOL bAll = FALSE : [in] �Ƿ�Ϊ�����ı�
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	void FormatTextRange(int nStart, int nEnd,BOOL bAll = FALSE);

	/******************************************************************
	*	�������ܣ�	��ʽ����nStart���ڵ��е�nEnd�����е��ı���ʾ��ʽ
	*	����������	int nStart : [in] �ı���ʼ
	*				int nEnd   : [in] �ı�����
	*	�� �� ֵ��	void 
	*	����������	FormatTextRange
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	void FormatTextLines(int nStart, int nEnd);

	/******************************************************************
	*	�������ܣ�	�ı��nStart��nEnd���ı�ΪlpszStr
	*	����������	int nStart : [in] �ı���ʼ
	*				int nEnd   : [in] �ı�����
	*				LPCTSTR lpszStr : [in] Ҫ�滻���ı�
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	void ChangeCase(int nStart, int nEnd, LPCTSTR lpszStr);

	// ��������
	struct SymbolColor 
	{
		COLORREF clrColor; // RGBɫֵ
		BOOL bBold;        // �Ƿ�ʹ�ô���
	};

	enum ChangeType {ctUndo/*ȡ��*/, ctUnknown/*��ȷ����ԭ��*/, ctReplSel/*����ѡ��*/, ctDelete/*����Delɾ��*/, ctBack/*����back spaceɾ��*/, ctCut/*����*/, ctPaste/*ճ��*/, ctMove/*�ƶ�*/};

	// ��Сд����
	BOOL  m_bCaseSensitive;
	// ���Ըı��Сд
	BOOL  m_bChangeCase;
	// ע�Ϳ�ͷ��
	TCHAR m_chComment;
	// ע�Ϳ�ͷ��
	TCHAR m_chComment2;
	// ע�Ϳ�ͷ��
	CString m_strComment;
	// ���ÿ�ͷ��
	CString m_strStringQuotes;
	// �ؼ����б�
	CString m_strKeywords;
	// Сд�Ĺؼ����б�
	CString m_strKeywordsLower;
	// �����б�
	CString m_strConstants;
	// Сд�ĳ����б�
	CString m_strConstantsLower;

	// ע�������ʽ
	SymbolColor m_icComment;
	// ���������ʽ
	SymbolColor m_icNumber;
	// ���������ʽ
	SymbolColor m_icString;
	// �ؼ��������ʽ
	SymbolColor m_icKeyword;
	// ���������ʽ
	SymbolColor m_icConstant;
	// ���������ʽ
	SymbolColor m_icOtherText;

	// �Ƿ���ת����ʽ
	BOOL		m_bInForcedChange;
	// ��������
	ChangeType  m_changeType;	
	// ��һ����ѡ��
	CHARRANGE   m_crOldSel;

	/******************************************************************
	*	�������ܣ�	�������ָ�����ڵ��TIP�����ı�
	*	����������	CString& strTitle : [out] �õ���ָ���������ڵ���
	*				UINT nFlags		  : [in] �ťFLAGS
	*				CPoint point	  : [in] ָ������
	*				int* nBegin = NULL: [out] ָ���������ڵ��ʿ�ʼ������(��ȫ��������)
	*				int* nEnd = NULL  : [out] ָ���������ڵ��ʽ���������(��ȫ��������)
	*	�� �� ֵ��	CString : �õ���TIP�ı�
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual CString	GetTipText(CString& strTitle,UINT nFlags, CPoint point,int* nBegin = NULL,int* nEnd = NULL);

	/******************************************************************
	*	�������ܣ�	�õ�������ڵ�ĵ���
	*	����������	CString strText : [in] ��ǰ��������е����ı�
	*				int nBegin      : [in/out] ��ʼλ��,���ص��ʿ�ʼλ��
	*				int nEnd        : [in/out] ����λ��,���ص��ʽ���λ��
	*				CPoint point    : [in] ������ڵĵ�
	*	�� �� ֵ��	CString : ������ڵ�ĵ���
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual CString	GetTipText(CString strText,int& nBegin,int& nEnd,CPoint point);

// Generated message map functions
protected:
	//{{AFX_MSG(CScriptEditViewFormula)
	// �༭��Ϣ����
	afx_msg void OnChange();
	// �����ؼ���Ϣ����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// WM_SETTEXT��Ϣ��Ӧ
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	// EN_PROTECTED��Ϣ��Ӧ
	afx_msg void    OnProtected(NMHDR*, LRESULT* pResult);

protected:
	// û������
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
