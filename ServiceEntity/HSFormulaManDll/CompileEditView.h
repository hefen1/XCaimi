/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CompileEditView.h
*	�ļ���ʶ��	��ʽ�༭�ؼ����༰����һЩ��ʽ�ؼ�Ҫ����Ķ���
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
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
*	�� �� �ƣ�	HSAddStrCompileEdit
*	�� �� ����	���빫ʽ����ʱʹ�õ��м����
***************************************************************/
struct HSAddStrCompileEdit
{
	short    m_nCharPos;	// m_strText�ĳ���
	CString* m_strText;     // ָ��ʽ���Ǻ������ִ�
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
*	�� �� �ƣ�	HSTreeToCompileEdit
*	�� �� ����	ɾ����ʽʱʹ�õ��м�����ṹ
***************************************************************/
struct HSTreeToCompileEdit
{
	void*	  m_pName; // ָ��CTreeGroup����
	void*	  m_pData; // ָ��HTREEITEM����
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
*	�� �� �ƣ�	CCompileEditView
*	�� �� ����	��ʽ�༭�ؼ���
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
	// ������Ϣ����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// ����Ҽ����´���
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	// ���̰��´���
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	// ����UP����
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	// ����ƶ�����
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// ���������´���
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// ������������
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// ������˫������
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// ����Ҽ�������
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	// ����Ҽ�˫������
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	// �������ͼ�괦��
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	// �ַ���Ϣ����
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	// ע��,ûʵ�� ??? 
	afx_msg void OnNote(); 
	// ��������,ûʵ�� ???
	afx_msg void OnNumen();
	// ��������, �˹���Ӧ��ȥ ??? 
	afx_msg void OnBanpieriods(); 
	// ��ʽ����
	afx_msg void OnCompile(); 
	// ���湫ʽ
	afx_msg void OnOk(); 
	// �½���ʽ����, û���κδ���
	afx_msg void OnNewExpress();

	// ����������ûʵ�� ??? 
	afx_msg void OnExpressOut();
	afx_msg void OnExpressIn();
	afx_msg void OnEngToChinese();

	// ���'+', '-', '*', '/', '(', ')', ':=', ':', '^'���Ŵ���
	afx_msg void OnSymbolMsg(UINT nID);

	/******************************************************************
	*	�������ܣ�	�ӱ�����Ϣ�������б���
	*	����������	WPARAM wParam = 0 : [in] ������
	*				LPARAM lParam = 0 : [in] nodErrorInfo*��������,������Ϣ
	*	�� �� ֵ��	long : ��Ϊ0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	afx_msg long AddCompileInfotoList(WPARAM wParam = 0, LPARAM lParam = 0);

	/******************************************************************
	*	�������ܣ�	���빫ʽ��������
	*	����������	WPARAM wParam = 0 : [in] HX_EXPRESSSENTENCE ���빫ʽ, �����ǲ��뺯��
	*				LPARAM lParam = 0 : [in] HSAddStrCompileEdit*��������,HSAddStrCompileEdit::m_strTextΪ����Ĺ�ʽָ�������ָ��
	*	�� �� ֵ��	long : ��Ϊ0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	afx_msg long OnSendAddStr(WPARAM wParam = 0, LPARAM lParam = 0);
	// HX_USER_LOADCUR��Ϣ��Ӧ,û���κι���
	afx_msg long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);

	/******************************************************************
	*	�������ܣ�	HX_USER_DELETECUR��Ϣ��Ӧ,���ⲿ��ʽ����ɾ����ʽ
	*	����������	WPARAM wParam = 0
	*				LPARAM lParam = 0 : [in] HSTreeToCompileEdit*��������
											 HSTreeToCompileEdit::m_pNameΪCTreeGroup*��������
											 ��������Ҫɾ���Ĺ�ʽ����Ϣ
	*	�� �� ֵ��	long : ����ɾ����ʽ����������(2), ���������Ϊ0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	afx_msg long DeleteCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	
	/******************************************************************
	*	�������ܣ�	�˹���ûʹ��,��ʵ���˴Ӳ˵���ѡ�����麯��,���빫ʽ��������
	*	����������	UINT nID : [in] �Ӻ����˵�ID
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	afx_msg void OnFunListMsg(UINT nID);

	// û�б�����(�˵��Բ�����) ??? 
	afx_msg void SaveDataMenu(UINT nID);

	// û��ʵ��( ??? )
	afx_msg void OnProperty();

	// ��Ӧ�˵�"����"����
	afx_msg void OnCopy();

	// ֻ�����˻��๦��
	int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);

public:
	CMapVariabile*  m_pExternExpression;     // �ⲿ���ʽ��
	CMapVariabile*  m_pExternVariabile;		 // �ⲿ������
	CMapVariabile*  m_pExternFunction;		 // �ⲿ������
	
public:
	// ��ʼ���ⲿ���ʽ��,�ⲿ������,�ⲿ��������ʼ���Ѷ���ĺ���
	void InitData();
	// �õ��ⲿ���ʽ��ָ��
	CMapVariabile*  GetExpression() { return m_pExternExpression; }     // �ⲿ���ʽ��

protected:
	// ����ʹ��,�Ż���Ӧɾ�� ??? 
	CDialogBar*		     m_pwndDlgMainBar;
	// ����ʹ��,�Ż���Ӧɾ�� ??? 
	CListCtrl*			 m_pListCtrl;
	// ����ʹ��,�Ż���Ӧɾ�� ??? 
	class CWorkspaceWnd* m_pWorkspaceWnd;
	// ����ʹ��,�Ż���Ӧɾ�� ??? 
	class CGridCtrl*	 m_pVarGridCtrl;

	// �Ҽ������˵�
	CMenu*				 m_pPopupMenu;
	// �ؼ�����(����������),��ʵ����ȷʹ�õ�ֻ��COMPILE����
	int					 m_nCompileEdit;
	// û��ʹ��, �Ż���Ӧɾ�� ??? 
	CString				 m_strCurExpressName;

	// ���ָ������, TRUEʹ��IDC_ARROW, FALSEʹ��IDC_IBEAM
	BOOL				 m_bArrowCur;

protected:
	// ����ȫ�ֲ˵�g_pSaveMenu������,����,�˳�Ա����ʲô����, �Ż���Ӧɾ�� ??? 
	HTREEITEM			m_hExpressInTreePos;	
	// û��ʹ��,�Ż���Ӧɾ�� ??? 
	struct CTreeGroup*	m_pCurEditTreeGroup;

protected:
	// ��ʼ���Ѷ���ĺ���
	void  InitSysFunction();
	// һЩRichEdit�ؼ����Ե�����
	void  Initialize();
	// ����ȫ�ֲ˵�g_pSaveMenu������,����,�˳�Ա����ʲô����, �Ż���Ӧɾ�� ??? 
	BOOL  CreateSaveMenu();

public:
	// ����ʹ��,�Ż���Ӧɾ�� ??? 
	void  Atatch(CListCtrl* pListCtrl){m_pListCtrl = pListCtrl;}
	// ����ʹ��,�Ż���Ӧɾ�� ??? 
	/*void  Atatch(class CWorkspaceWnd* pFunListView);*/
	// ����ʹ��,�Ż���Ӧɾ�� ??? 
	void  Atatch(CDialogBar* pwndDlgMainBar)   {m_pwndDlgMainBar = pwndDlgMainBar;}
	// ����ʹ��,�Ż���Ӧɾ�� ??? 
	void  Atatch(class CGridCtrl* pVarGridCtrl){m_pVarGridCtrl = pVarGridCtrl;}	

	// ���ÿؼ�����
	void  SetCompileEdit(int nCompileEdit);
	// �õ��ؼ�����(�༭������HELP)
	BOOL  IsCompileEdit(int nType) {return (m_nCompileEdit&nType);}

public:
	// װ�빫ʽ����,Ŀǰ��û��ʹ��,�Ż���Ӧ��ɾ�� ??? 
	void Init(short nType);

	/******************************************************************
	*	�������ܣ�	����������Ĺ�ʽ
	*	����������	CDlgFormulaEdit* pDlg : [in] ��ʽ����������
	*				LPARAM lParam = 0    : [in] ������
	*	�� �� ֵ��	long 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	long Compile(class CDlgFormulaEdit* pDlg, LPARAM lParam = 0);

	// װ�빫ʽ����,Ŀǰ��û��ʹ��,�Ż���Ӧ��ɾ�� ??? 
	BOOL SaveLoadMap(BOOL bSave,CMapVariabile& ayVarMap,CString strFileName = "");	


	// �ؼ�����
	enum hxEditType {COMPILE = 0X0001/*��ʽ�༭*/,HELP = 0X0002/*����*/,NOTES = 0X0004/*ע��*/,EIDOLON = 0X0008/*��������*/};

public:
	// �趨�ؼ��ּ�����
	void SetKeywordsConstants();	
	// �ӵ��ؼ����ִ���
	virtual void AddKeywords(LPCTSTR lpszKwd)  {}
	// �ӵ������ִ���
	virtual void AddConstants(LPCTSTR lpszKwd) {}
	// ��չؼ����ִ�
	virtual void ClearKeywords()  {}
	// ��ճ����ִ�
	virtual void ClearConstants() {}

public:
	// �������ڴ���,Ŀǰ���ٴ��дι���, �Ż���Ӧɾ�� ???
	class CDlgEstopPeriod* m_pEstopPeriodDlg;

public:
	// û��ʵ��ʲô����, �Ż���Ӧɾ�� ???
	virtual BOOL OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );
public:
	// ֻ��UpdateExpress����ʹ��,��������û��ʲô����
	enum LoadExpressType { None/*����*/,TreeMsg/*����Ϣ*/,UpdateExpress/*���¹�ʽ*/,ComboxMsg/*�½��б���Ϣ*/,UpdateNotes/*����ע��*/};

public:
	// ��ʽ����������ָ��
	static class CDlgFormulaEdit* m_pParentDlg;

protected:
	// ��ʾ��Ϣ����
	/*CXInfoTip		m_pDatatip;*/
	// ������ʾ��Ϣ����
	BOOL   		    IsDataTip();

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

	//	����, �Ƿ�Ҫɾ�� ???
	virtual CString	GetTipText(CString strText,int nBegin,int nEnd,CPoint point) { return ""; }

	/******************************************************************
	*	�������ܣ�	���ݺ������õ�ע����Ϣ
	*	����������	CString strKey : [in] ������
	*	�� �� ֵ��	CString : ע����Ϣ
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual CString	GetKeyHelp(CString strKey);

	/******************************************************************
	*	�������ܣ�	�õ�strText��,strText[nStart]�ַ����ڵĵ���(��������ĸ�������ִ�)
	*	����������	CString strText : [in] Ҫ������ִ�
	*				int& nStart     : [in] Ҫ����ĵ�(������)
	*				int nCount      : [in] �ַ�����
	*	�� �� ֵ��	CString : strText[nStart]�ַ����ڵĵ���(��������ĸ�������ִ�), ����Ϊ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual CString	FindWord(CString strText,int& nStart,int nCount);

	/******************************************************************
	*	�������ܣ�	�õ�strText��߻��ұ����̵��ַ���(��ĸ������)
	*	����������	CString strText : [in] Ҫ������ִ�
	*				BOOL bLeft : [in] �õ�����ִ�(TRUE), �õ��ұ��ִ�(FALSE)
	*	�� �� ֵ��	CString : �õ��ĵ���
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual CString	CutWord(CString strText,BOOL bLeft);
public:
	// �ڲ�TIP��ʾ
	void	HideDataTip();

protected:
	// ���߷������,Ŀǰ�˹��ܻ�û��ʵ��, �ݲ�˵��
	BOOL         m_bInputChar;
	// ���߷������,��������ʾ
	COnlineList* m_pOnlineList;
	// ������������ʾ�ؼ�
	COnlineList* CreateOnlineList();

public:
	// ��ʾ���߷���,���߷������,Ŀǰ�˹��ܻ�û��ʵ��, �ݲ�˵��
	void		 ShowOnline(int nCmd,BOOL bForce = FALSE);
	// �Ƿ���ʾ���߷���,���߷������,Ŀǰ�˹��ܻ�û��ʵ��, �ݲ�˵��
	BOOL		 IsOnlineShow();
	// ��ȥ������Ϣ,�Ż�ʱ�Ƿ�Ӧɾ�� ???
	BOOL		 OnReturn(MSG* pMsg);
	// ���߷������, ���߷������,Ŀǰ�˹��ܻ�û��ʵ��, �ݲ�˵��
	void		 OnEditOnlineHelp();
	// ��ʾ���߷���,���߷������,Ŀǰ�˹��ܻ�û��ʵ��, �ݲ�˵��
	void		 ShowOnlineList(CPoint& point,int nCmdShow,CString strText = "",
								BOOL bEdit = FALSE,int nBegin = 0,int nEnd = 0);
	// �༭���߷���,���߷������,Ŀǰ�˹��ܻ�û��ʵ��, �ݲ�˵��
	virtual void OnEdit(int nBegin,int nEnd,CString strText,BOOL bEdit = FALSE);

protected:
	// �����˻����OnEditProperties����,����,���в�����������
	virtual HRESULT QueryAcceptData(LPDATAOBJECT, CLIPFORMAT*, DWORD,BOOL, HGLOBAL);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPILEEDITVIEW_H__174390D7_AB9C_11D4_A12C_00E09833EFFA__INCLUDED_)
