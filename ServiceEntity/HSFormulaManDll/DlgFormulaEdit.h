/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgFormulaEdit.h
*	�ļ���ʶ��	��ʽ�༭���ඨ��
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
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
*	�� �� �ƣ�	CDlgFormulaEdit
*	�� �� ����	��ʽ�༭������
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
	// ������û��, �Ƿ�ȥ�� ??? 
	CComboBoxEx	m_wndComboxType;
	CComboBoxEx	m_wndComboxComment;
	CComboBoxEx	m_wndExpComment;

	// �Ƿ�Ϊ��ͼ(1��ͼ, 0��ͼ)
	int			m_nMainChart;        
	// ��ʽ���庺��˵������
	CEdit       m_wndPromte;  //by HS

	int  m_CharType;  //��¼��ʼʱͼ������ Ϊ��ͼ���Ǹ�ͼ qinhn 20090727 Add
	
	//}}AFX_DATA

	// ��ʽ�з������:�μ� #define HS_EXP_FINANCE �ȶ���
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
	// ��ʼ��
	virtual BOOL OnInitDialog();
	// "ȷ��"
	virtual void OnOK();
	// "ȡ��"
	virtual void OnCancel();
	// "��������"
	afx_msg void OnInfunction();
	// "����"
	afx_msg void OnCompile();
	// ���� gbq add 20060427
	afx_msg void OnSave();
	// "��ͼ"��ѡ��ť
	afx_msg void OnMainchart();
	// "��ͼ"��ѡ��ť
	afx_msg void OnSubchart();
	// "����"ѡ��ť
	afx_msg void OnPwd();
	// "��������"
	afx_msg void OnStopperiod();
	// "�÷�ע��"
	afx_msg void OnNotes();
	// "��������"
	afx_msg void OnEidolon();
	// ���������ʧȥ����
	afx_msg void OnKillfocusPwdedit();
	// ����������Ϣ����(Ŀǰ�����Ѳ���)
	afx_msg void OnError();
	// "���뺯��"
	afx_msg void OnButfun();
	// "���빫ʽ"
	afx_msg void OnButexp();
	// "����"(Ŀǰ�����Ѳ���)
	afx_msg void OnButfind();
	// m_pRichEdit(��ʽ������ʾ)�ؼ�Replace����
	afx_msg void OnEditReplace();
	// m_pRichEdit(��ʽ������ʾ)�ؼ�COPY����
	afx_msg void OnButcopy();
	// m_pRichEdit(��ʽ������ʾ)�ؼ�Paste����
	afx_msg void OnEditPaste();
	// �ı䴰�ڴ�С
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// ʹm_pGridCtrl(�����б�)�õ�����
	void OnEditParam();
	

	/******************************************************************
	*	�������ܣ�	�ӹ�ʽ����(��ʽ�����͹�������Ϣ)
	*	����������	WPARAM wParam = 0 : [in] ������
	*				LPARAM lParam = 0 : [in] CExpression*����,Ҫ����Ĺ�ʽָ��
	*	�� �� ֵ��	long : ��Ϊ1
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��29��
	*	�޸�˵����	
	*****************************************************************/
	long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);

	// ���ô��ڴ�С
	void SetSize();

public:
	// ��ʽ���ı��༭�ؼ�
	class CCompileEditView*	m_pRichEdit;
	// �������ÿؼ�
	CGridCtrl	m_WndGridCtrl;
	// ��ʽ����������
	class CDlgFormulaMan*		m_pFormulaMan;

	// �������鴰��
	static class CDlgNotesAndEidolon* m_pDlgEidolon;
	// ע�ʹ���
	static class CDlgNotesAndEidolon* m_pDlgNotes;

	// ɾ����ʽ�༭�����õ�������Դ
	void Delete();
	// һ���������Ϊ�����ؼ�����صĴ���:ע��,��������,��ʽ������������
	void Show(int nShow);

public:
	// �����¹�ʽ����
	void OnNewExpress();
	// �����½���ʽ
	//void SpeedCreateExpress();

protected:
	// ����
	CString	     m_strFistPWD;
	// ��������������	
	static class CDlgFunctionMan* m_pFunctionMan;
	// ��ʽ�༭����(�¼����Ǹ���)
	short		  m_cType;
	// �½���ʽʱ��
	//char		  m_cSpeedNew; 
public:
	// ���Ͳ���
	enum Type{ 
			   none     = 0x01,
			   Update   = 0x02 /*���Ĺ�ʽ*/,
			   NewExp   = 0x04 /*�¹�ʽ*/, 
		       SpeedNew = 0x08 /*�����½���ʽ*/
	          };

	int  IsType(short cType)  { return (m_cType & cType); }
	void SetType(short cType) { m_cType = cType; }
	//void SetExType(char cType) { m_cSpeedNew = cType;}
		
public:
	// ������Ϣ����
	static class CDlgError* m_pErrorDlg;

public:
	/******************************************************************
	*	�������ܣ�	���ݹ�ʽ���ͼ���ʽ�������ô��ڱ���
	*	����������	int nType		: [in] ��ʽ����
	*				CString strName : [in] ��ʽ����
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��29��
	*	�޸�˵����	
	*****************************************************************/
	void SetTitle(int nType,CString strName);

	//
protected:
	// ���빫ʽ����ʱ��ʾ����
	CTreeCtrlFormula*  m_pEditTree;

	/******************************************************************
	*	�������ܣ�	���빫ʽ�������� 
	*	����������	WPARAM wParam = 0 : [in] ���빫ʽCTreeCtrlFormula::EditExpRef, ���뺯��CTreeCtrlFormula::EditFunRef
	*				LPARAM lParam = 0 : [in] ���뺯��ʱΪCString*, ���빫ʽʱΪCValue*
	*	�� �� ֵ��	long : ��Ϊ0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��29��
	*	�޸�˵����	
	*****************************************************************/
	afx_msg long OnSendAddStr(WPARAM wParam = 0, LPARAM lParam = 0);

public:
	// �������빫ʽ����ʱ��ʾ�����ؼ�m_pEditTree
	CTreeCtrlFormula* CreateEditTree();

	/******************************************************************
	*	�������ܣ�	���ⲿ�������ʽ���еõ�������ʽ
	*	����������	CString strName : [in] ������ʽ��
	*				short& nRet		: [in/out] CTreeCtrlFormula::EditFunRef���ⲿ����������, ͬʱ��nRet��ΪCExpression::ExpExternFun
											   CTreeCtrlFormula::EditExpRef���ⲿ��ʽ������, ͬʱ��nRet��ΪCExpression::ExpExternExp
	*	�� �� ֵ��	CValue*  : �Ҳ�����ΪNULL, ����Ϊ�ҵ��Ĺ�ʽ������ָ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��29��
	*	�޸�˵����	
	*****************************************************************/
	CValue*  GetVariable(CString strName,short& nRet);

	/******************************************************************
	*	�������ܣ�	��ʾ��ʽ��������
	*	����������	CRect& RefRect : [in] ��ʾ�Ĵ�С��λ��
	*				int nCmdShow   : [in] ��ʾ(SW_SHOW)��������(SW_HIDE)
	*				int nType	   : CExpression::ExpExternFun��ʾ������, ��������ʾ��ʽ��
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��29��
	*	�޸�˵����	
	*****************************************************************/
	void ShowEditTree(CRect& RefRect,int nCmdShow,int nType);

	/******************************************************************
	*	�������ܣ�	���ⲿ�����������к�������ΪnType�ĺ��������뵽��pTree��strRoot�ڵ���
	*	����������	CString strRoot		: [in] Ҫ��������pTree�ĸ��ڵ�
	*				int nType			: [in] ��������
	*				CTreeCtrlFormula* pTree  : [in] Ҫ��������
	*	�� �� ֵ��	void 
	*	����������	CTreeCtrlFormula::InputFunItem()
	*	��    �ߣ�	
	*	������ڣ�	2003��4��29��
	*	�޸�˵����	
	*****************************************************************/
	void AddFunByType(CString strRoot,int nType,CTreeCtrlFormula* pTree);
	 //��Ӳ����б�
	void AddToParamerList(CExpression* pExp);

public:
	// ��ʽ����
	int		m_nExpressType;
	// �ɵĹ�ʽ��
	CString m_strOldName;
	// �Ƿ�"ȷ��"��ť�˳�����
	BOOL	m_bOkFlag;
	HACCEL m_hAccelTable;

	static bool bNeedChangeStyle;  //qinhn 20070727 Add 
	static BOOL bEscMsgOfDlgEdit;
	
	// begin by HS 2002.3.28
protected:
	// ʹ��Ĭ�Ϲ�ʽ����
	afx_msg void OnDefault();

	// Ĭ�Ϲ�ʽ������ؽṹ
	struct ReloadDefault
	{
		CString m_strName;	// ��ʽ��
		CString m_strSource;// ��ʽ�嶨��

		void Clear() 
		{
			m_strName   = "";
			m_strSource = "";
		}
		// ���л�,bStore�Ƿ�д����
		void Serialize(bool bStore);
	};
	
	// Ĭ�ϲ�������
	ReloadDefault m_RDDefault;
	// end by HS 2002.3.28
	
public:
	// WIN��Ϣ����,�õ��ؼ���ˢ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// ��ʽ�����б�ؼ�
	CComboBox m_wndComBox;
	// ??? ����ʹ��,����ɾ��
	HTREEITEM m_hComboxItem;
	// ��ʽ�����б�ؼ��ı�ѡ����
	afx_msg void OnComBoxSelChange();
	// ����������̬����TAB�ı�ѡ����
	afx_msg void OnTcnSelchangeTabout(NMHDR *pNMHDR, LRESULT *pResult);
	// ����������̬����TAB�������
	afx_msg void OnNMClickTabout(NMHDR *pNMHDR, LRESULT *pResult);
	// ����������̬����TAB�ؼ�
	CTabCtrl m_wndTabOut;

public:
	enum { Translate/*��̬����*/,CompileResult/*������*/ };

	/******************************************************************
	*	�������ܣ�	��ʾ��̬��������������
	*	����������	int nIndex : [in] Translate��ʾ��̬����, CompileResult����ʾ������
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��29��
	*	�޸�˵����	
	*****************************************************************/
	void ShowResult(int nIndex);

public:
	void OnMenu(UINT nID);
	void OnUpdateMenu(CCmdUI* pCmdUI);

	void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//����ʼ��
	void initGridCtrl(int nRow=100);
	BOOL ShowWindow( int nCmdShow );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDIUPDATETEXP_H__EDBF85C6_1102_11D5_A107_E07528000000__INCLUDED_)
