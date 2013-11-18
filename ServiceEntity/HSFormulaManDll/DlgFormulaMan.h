/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgFormulaMan.h
*	�ļ���ʶ��	��ʽ��������
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_FORMULAMAN_H__DF745EF3_1042_11D5_A105_2A829F000000__INCLUDED_)
#define AFX_FORMULAMAN_H__DF745EF3_1042_11D5_A105_2A829F000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFormulaMan.h : header file
//
#include "TreeCtrlFormula.h"
#include "DlgFormulaEdit.h"
#include "ExpPublic.h"
#include "DataSourceDefineEx.h"
#include "DataSourceDefine.h"
#include "ConfigInterface.h"
#include "HSWinner.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaMan dialog

//#include "ETSLayout.h"
#include "DlgNotesAndEidolon.h"

// SHIFT���Ƿ���
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
// CTRL���Ƿ���
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

extern void SetActiveMain(CWnd* pRefWnd);



extern CMapStringToOb *Formular_CfgTopMapOb;
extern CMapStringToOb *Formular_UserCfgMapOb;
/*************************************************************
*	�� �� �ƣ�	CDlgFormulaMan
*	�� �� ����	��ʽ������
***************************************************************/
class CDlgFormulaMan : public  CHSBaseDialog
{
	// ����˵�����ǿ�����̬����
	DECLARE_DYNAMIC(CDlgFormulaMan)
// Construction
public:
	/******************************************************************
	*	�������ܣ�	����,��ʼ��ָ������ı���
	*	����������	CWnd* pParent = NULL : [in] ������ָ��
	*				long* pThis = NULL   : [in] ָ�������ָ���ָ��
	*	�� �� ֵ��	��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	CDlgFormulaMan(CWnd* pParent = NULL,long* pThis = NULL);   // standard constructor
	~CDlgFormulaMan();

	/* DialogMgr: Add this: */
//	DECLARE_LAYOUT();
	/************************/

// Dialog Data
	//{{AFX_DATA(CDlgFormulaMan)
	enum { IDD = IDD_FORMULAMAN };
	// ����TAB�ؼ�("����ָ��", "����ϵͳ", "����ѡ��", "���K��")
	CTabCtrl	m_wndTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFormulaMan)
public:
	// ��ϢԤ����,������̵�ESC����ENTER��,ͬʱ,���õ�ǰ������ϢԤ������	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	// ���ݽ���
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// ���ں���
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	// Generated message map functions
	//{{AFX_MSG(CDlgFormulaMan)
	// ��ʼ��
	virtual BOOL OnInitDialog();
	// ��������
	afx_msg void OnDestroy();
	// ���˳�����
	virtual void OnCancel();
	// ��ȷ������
	virtual void OnOK();
	// ��ı�ѡ����
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	// ���½�����
	afx_msg void OnNew();
	// �����Ĵ���
	afx_msg void OnUpdate();
	// ��ɾ������
	afx_msg void OnDelete();
	// ��ע�ʹ���
	afx_msg void OnNotes();
	// ���������
	afx_msg void OnOut();
	// �����봦��
	afx_msg void OnIn();
	// ����ʱ���봦��
	afx_msg void OnTempin();
	// ������˵������
	afx_msg void OnParam();
	// ��С�ı䴦��
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// �����Ҵ���
	afx_msg void OnButfind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	/******************************************************************
	*	�������ܣ�	�ϷŴ���
	*	����������	WPARAM wParam : [in] ���Ϊ�ϷŹ�ʽ,���ΪHX_USER_DRAGDATA-1,������������
	*				LPARAM lParam : [in] CExpression*����,Ϊ�����ϷŵĹ�ʽ��
	*	�� �� ֵ��	LRESULT : �ɹ�(1), ʧ��(0)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��30��
	*	�޸�˵����	
	*****************************************************************/
	virtual LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);

	/******************************************************************
	*	�������ܣ�	�򸸴���g_hParentWnd��HX_USER_COMPILEDATA��Ϣ
	*	����������	WPARAM wParam : [in] ������
	*				LPARAM lParam : [in] ������
	*	�� �� ֵ��	LRESULT : ��Ϣ�����ػ���0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��30��
	*	�޸�˵����	
	*****************************************************************/
	virtual LRESULT OnExchangData(WPARAM wParam, LPARAM lParam);

	// ��Ϣѭ��,����û����
	long OnRun(WPARAM wParam, LPARAM lParam);

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

public:	
	// �����ģ����ռ�õ�����˽�˿ռ�
	void Delete();
	// �������й�ʽ
	void Save();
	
	// ��ʽ���ؼ�����
	CArray<CTreeCtrlFormula*,CTreeCtrlFormula*> m_pAyTree;
public:	
	// TAB��ͼ��
	static CImageList	m_tabImages; 
	// TREE��ѡ�еĹ�ʽ��ͼ��
	static CImageList	m_expressCheckImages;
	// TREE��Ĭ�ϵĹ�ʽ��ͼ��
	static CImageList	m_expressImages;


public:	

	/******************************************************************
	*	�������ܣ�	��������,�õ���(��������˹�ʽ)
	*	����������	int nIndex = -1 : [in] ������,Ϊ-1ʱ����Ҫ�õ���ǰѡ��TAB����
	*	�� �� ֵ��	CTreeCtrlFormula* : �õ��Ĺ�ʽ��,ָ������ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	CTreeCtrlFormula* GetCurTree(int nIndex = -1);

	/******************************************************************
	*	�������ܣ�	��������,�õ���(��������˹�ʽ)
	*	����������	WORD nMask   : [in] ������(��Tech,Condition,Exchange,MoreKLine,Union,ExpressBin,none��)
	*	�� �� ֵ��	CTreeCtrlFormula* : �õ��Ĺ�ʽ��,ָ������ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	CTreeCtrlFormula* GetCurTree(WORD nMask);

	/******************************************************************
	*	�������ܣ�	��������,�õ���(��������˹�ʽ)
	*	����������	CString strName : [in] ����
	*	�� �� ֵ��	CTreeCtrlFormula*    : �õ��Ĺ�ʽ��,ָ������ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	CTreeCtrlFormula* GetCurTree(CString strName);

	/******************************************************************
	*	�������ܣ�	�õ���ʽpExpresie���ڵĹ�ʽ��
	*	����������	CExpression* pExpresie : [in] ��ʽ
	*	�� �� ֵ��	CTreeCtrlFormula*		   : [in] �õ��Ĺ�ʽ��,ָ������ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	CTreeCtrlFormula* GetCurTree(CExpression* pExpresie);

	/******************************************************************
	*	�������ܣ�	��ʾ�����ع�ʽ��pCurTree����
	*	����������	CTreeCtrlFormula* pCurTree : [in] Ҫ�����Ĺ�ʽ��
	*				BOOL bShow = TRUE	  : [in] ��ʾ(TRUE), ����(FALSE)
	*	�� �� ֵ��	void		 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	void		 ShowTree(CTreeCtrlFormula* pCurTree,BOOL bShow = TRUE);

// ��ʽ�����Ϣ
public:
	static CMapVariabile*  m_pExternExpression;      // �ⲿ���ʽ��
	static CMapVariabile*  m_pExternVariabile;		 // �ⲿ������
	static CMapVariabile*  m_pExternFunction;		 // �ⲿ������

	static HSFunAddDefaultTechAnysis m_pOpeDefTechFun;		 // ȱʡָ�����(����ָ��)
	static class CDlgFormulaEdit*	  m_pEdiUpdatetExp;		 // ��ʽ�༭����(����ָ��)
	static class CDlgNotesAndEidolon* m_pDlgNotes;			 // ��ʽע�ʹ���(����ָ��)
	static class CDlgNotesAndEidolon* m_pDlgParam;			 // �����޸Ĵ���(����ָ��)	

	/******************************************************************
	*	�������ܣ�	���ó���ָ�����
	*	����������	CExpression* pExpression : [in] Ҫ���õ�ָ�깫ʽָ��
	*				char Operator			 : [in] ������, '+' ����Ϊ����
															'-' ȡ������ָ�������
															'd' ��ճ���ָ���б�
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	static BOOL  SetDefaultTech(CExpression* pExpression,char Operator);

	// �õ���ʽ�༭����ָ��
	class CDlgFormulaEdit*		GetExpDlg(CExpression* pExpression = NULL);
	// �õ���ʽע�ʹ���ָ��
	class CDlgNotesAndEidolon* 	GetDlgNotes();
	// �õ������޸Ĵ���ָ��
	class CDlgNotesAndEidolon* 	GetDlgParam();

	/******************************************************************
	*	�������ܣ�	���ù�ʽע�ʹ���m_pDlgNotes�Ĺ�ʽ�������ݼ����ڱ����ı�
	*	����������	CString strNotes   : [in] ��ʽ�����ı�
	*				CString strCaption : [in] ���ڱ����ı�
	*	�� �� ֵ��	void  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	void  SetNotes(CString strNotes,CString strCaption);

public:
	// �ı䴰�ڴ�С
	void	SetSize();

	/******************************************************************
	*	�������ܣ�	��ʼ��,�õ�����ģ���з����˿ռ���ⲿ���ʽ��,�ⲿ��
					����,�ⲿ������,ͼƬ,��ͷ����ͼ���ݿռ���亯��ָ��,
					ͬʱ��ʼ���ⲿ������
	*	����������	HWND hParentWnd = NULL : [in] �����ھ��, ΪNULLʱ���ı䵱ǰ�ĸ����ھ��
	*	�� �� ֵ��	void  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	void    InitData(HWND hParentWnd = NULL);

	/******************************************************************
	*	�������ܣ�	���ڿؼ�����Ч����
	*	����������	BOOL bEnable : [in] �Ƿ���Ч
	*				HSOpenExpressUseData* pData = NULL : [in] ��ǰѡ�еĹ�ʽ
	*	�� �� ֵ��	void  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	void    Enable(BOOL bEnable,HSOpenExpressUseData* pData = NULL);
	// ���ڿؼ�(����,ɾ��,ע�ͼ�������ť)����Ч����
	void	EnableItem(BOOL bEnable);

	//
protected:
	// ������(Ŀǰ����û����ʲô�ط����õ���???)
	// class CWndKeyboard* m_pIndividualInfo;
	// ���̾��鴰��
	//class  CWndKeyboard* m_pKeyboard;

	IHsColumnProperty* m_pColProt;   //�����ֶ�ָ��

public:
	/******************************************************************
	*	�������ܣ�	��ʽ����ģ������ӿ�
	*	����������	HSOpenExpressUseData* pData : [in] ������ʽ����HSOpenExpressUseData::m_wType&0xF00����
							hxShowByExpressType   : �ɹ�ʽ��������ʾ��ʽ, ����0
							hxShowReturnByExpressType : �ɹ�ʽ��������ʾ��ʽ,������ѡ�й�ʽ
							hxOpenExpressByConditionStockType : ѡ��, ����0
							hxOpenExpressBySortStockType : ָ������, ����0
							hxOpenBlockAnalysis   : ������, ����0
							hxOpenUniteAnalysis   : ��������, ����0
							hxOpenAlarmManagerDlg : ��Ԥ������, ����Ԥ������ָ��
							hxOpenFindDataDlg     : �򿪹�ʽ���Ҵ���, ����0
							hxOpenAddStkToAlarm   : �򿪼ӹ�Ʊ��Ԥ������,  ����0
						
	*	�� �� ֵ��	long : ��pData���,��pData˵��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��29��
	*	�޸�˵����	
	*****************************************************************/
	long Open(HSOpenExpressUseData* pData);
	long Open(StrategyInfo* pData);//yulx add ���Խ��ײ�������
	long Open(StrategyHis * pData);//yulx add ���Խ��ײ���
	long StrategyManage();//yulx add ���Թ���
	BOOL  ReadStrategy(CMapWordToPtr *& pmap);//��ȡ����
	void  SerializeExp(CArchive & ar,CExpValue *& exp);
	// ������
public:
	// ��������������, û������ʲô�ط���
	void				   CreateIndividualInfo();

	/******************************************************************
	*	�������ܣ�	�õ�������,���û��,�򴴽�
	*	����������	void* pData = NULL : [in] HSOpenExpressUseData*������
											  HSOpenExpressUseData::m_pData��CWnd*����,Ϊ�������ĸ�����ָ��
	*	�� �� ֵ��	CTreeCtrlFormula* : �õ��ĸ�����
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	class CTreeCtrlFormula*	   GetIndividualInfo(void* pData = NULL);

	// ���̾���
public:
	// �������̾��鴰��
// 	void				   CreateKeyboard();
// 	// �õ����̾��鴰��
// 	class CWndKeyboard* GetKeyboard();
// 
// 	/******************************************************************
// 	*	�������ܣ�	��ʼ�����̾���,��CKeyboardDataList::InitKeyboard����˵��
// 	*	����������	WPARAM wParam : [in] ��CKeyboardDataList::InitKeyboard����
// 	*				LPARAM lParam : [in] ��CKeyboardDataList::InitKeyboard����
// 	*	�� �� ֵ��	void				  
// 	*	����������	CKeyboardDataList::InitKeyboard
// 	*	��    �ߣ�	
// 	*	������ڣ�	2003��5��6��
// 	*	�޸�˵����	
// 	*****************************************************************/
// 	void				   InitKeyboard(WPARAM wParam, LPARAM lParam);
// 
// 	/******************************************************************
// 	*	�������ܣ�	��ʾ�������ؼ��̾��鴰��
// 	*	����������	WPARAM wParam : [in] ��ʾ(SW_SHOW), ����(SW_HIDE)
// 	*				LPARAM lParam : [in] HSOpenExpressUseData*��������
// 	*	�� �� ֵ��	void				  
// 	*	����������	
// 	*	��    �ߣ�	
// 	*	������ڣ�	2003��5��6��
// 	*	�޸�˵����	
// 	*****************************************************************/
// 	long				   ShowKeyboard(WPARAM wParam, LPARAM lParam);

public:

	/******************************************************************
	*	�������ܣ�	�¼���ʽ
	*	����������	WORD nMask : [in] �¼���ʽ����(ָ��,����,���׻����)
	*	�� �� ֵ��	CDlgFormulaEdit* : [in] ��ʽ�༭����ָ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	CDlgFormulaEdit* NewExpress(WORD nMask,short nType = CDlgFormulaEdit::NewExp);

	/******************************************************************
	*	�������ܣ�	��ע�ʹ���
	*	����������	WPARAM wParam : [in] ������
	*				LPARAM lParam : [in] ΪHSOpenExpressUseData*����
										 HSOpenExpressUseData::m_pDataΪ��ʽָ��(CExpression*)
	*	�� �� ֵ��	long : �ɹ�(1), ʧ��(0)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	long OnNotes(WPARAM wParam,LPARAM lParam);

	/******************************************************************
	*	�������ܣ�	�򿪲����޸Ĵ���
	*	����������	WPARAM wParam : [in] ������
	*				LPARAM lParam : [in] ΪHSOpenExpressUseData*����
										 HSOpenExpressUseData::m_pDataΪ��ʽָ��(CExpression*)
	*	�� �� ֵ��	long : �ɹ�(1), ʧ��(0)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	long OnParam(WPARAM wParam,LPARAM lParam);

public:
	/******************************************************************
	*	�������ܣ�	�����б�m_pAyTree��,����ʽnCopy��������Ŀ����pTree��
	*	����������	CTreeCtrlFormula* pTree = NULL : [in] Ŀ����,ΪNULLָ������m_wndFavorate
	*				int nCopy = CTreeCtrlFormula::CopyNone : [in] ������ʽ
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	void CopyTree(CTreeCtrlFormula* pTree = NULL,int nCopy = CTreeCtrlFormula::CopyNone);

	void CopyToFloatTree(int nCopy = CTreeCtrlFormula::CopyNone);

protected:

	/******************************************************************
	*	�������ܣ�	���ļ��Ի����еõ���ʽ�ļ���
	*	����������	BOOL bOpenFileDialog : [in] �򿪻��Ǳ���
	*				CString& strFileName : [out] �õ����ļ���
	*	�� �� ֵ��	int : �ļ��Ի�DoModal�ķ���
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	int		OpenExpressFile(BOOL bOpenFileDialog,CString& strFileName);

	/******************************************************************
	*	�������ܣ�	���û�ָ���ļ��ж�����
	*	����������	int& lLength		: [out] �ļ�����(Ҳ���Ƿ��ص�char*�Ŀռ䳤��)
	*				CString& fileName   : [out] �û�ָ�����ļ���
	*	�� �� ֵ��	char* : ���û�ָ���ļ��ж�ȡ������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	char*	GetInportBuffer(int& lLength,CString& fileName);

	/******************************************************************
	*	�������ܣ�	��������ҹ�ʽ
	*	����������	char* pBuffer : [in] �����ҹ�ʽ�ļ�����
	*				long lLength  : [in] pBuffer����
	*	�� �� ֵ��	int : ����ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	int 	InportFxjExpress(char* pBuffer,long lLength);

	/******************************************************************
	*	�������ܣ�	���ִ�pBuffer�����ɷ����ҹ�ʽ���ŵ�����ayExpress��
	*	����������	char* pBuffer : [in] Ҫ������ִ�
	*				long lCurPos  : [in] ��ʼλ��
	*				long lNextPos : [in] �¸����ݿ�ʼλ��
	*				CArray<FxjExpress*, FxjExpress*>& ayExpress : [out] �������,�����ҹ�ʽ����
	*				int lLength   : [in] pBuffer����
	*	�� �� ֵ��	int : �ɹ�(1), ʧ��(-1)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	int		GetFxjExpress(char* pBuffer,long lCurPos,long lNextPos,
						  CArray<FxjExpress*,FxjExpress*>& ayExpress,int lLength );
	// ��ǰ���Ƿ���Ч(nCurPosС��nSubCount��nMainCount)
	BOOL	IsValid(int nCurPos,int nSubCount,int nMainCount);

public:

	/******************************************************************
	*	�������ܣ�	�ӹ�ʽ(�ӷ����ҹ�ʽ)
	*	����������	CMapVariabile* pExternExpression : [in] �ⲿ��ʽ�б�
	*				WORD wStyle						 : [in] ��ʽ���뷽ʽ,EXTERN_EXPRESS_OVR�򸲸����й�ʽ
	*				FxjExpress* pFxjExpress          : [in] �����ҹ�ʽ 
	*				CTreeCtrlFormula* pTree = NULL		 : [in] Ҫ�����Ĺ�ʽ��
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	void	AddExpress(CMapVariabile* pExternExpression,WORD wStyle,FxjExpress* pFxjExpress,CTreeCtrlFormula* pTree = NULL);

	/******************************************************************
	*	�������ܣ�	����ʽpLoadData::pExpValue�����ⲿ��ʽ��,ͬ�๫ʽ����Ĭ��ָָ�����
	*	����������	CTreeCtrlFormula::LoadItemData* pLoadData : [in] ��ʽ�����Ϣ
	*				WORD wStyle : ��ʽ���뷽ʽ,EXTERN_EXPRESS_OVR�򸲸����й�ʽ
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	void	AddExpress(CTreeCtrlFormula::LoadItemData* pLoadData,WORD wStyle);

	// ������pTree������ͼ�꼰ѡ��ͼ�����m_pFormulaManָ��ǰ��ʽ����������
	void	SetTree(CTreeCtrlFormula* pTree);
protected:
	// ��ʽ���뷽ʽ
	WORD			m_wExternalExpress;
public:
	IMainWnd*		m_pMainWnd;    //����ܽӿ�
public:

	/******************************************************************
	*	�������ܣ�	��������
	*	����������	UINT nKey : [in] ��ֵ
	*	�� �� ֵ��	BOOL : ����ɹ�(TRUE), û������(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	BOOL	Key(UINT nKey);

protected:
	
	struct ExternParam
	{
		int		m_nCopy/* = CopyNone*/;
		DWORD	m_dwStyle;
		DWORD	m_dwType;   // ����
		ExternParam()
		{
			memset(this,0,sizeof(ExternParam));
		}
	};

	CTypedPtrMap<CMapPtrToPtr,CTreeCtrlFormula*,ExternParam*> m_ayExtern;
	
	//CArray<CTreeCtrlFormula*,CTreeCtrlFormula*> m_ayExtern; // �ⲿʹ�� //m_wndFavorate;
	//CTreeCtrlFormula* GetExternTree(int nPos);

public:
	/******************************************************************
	*	�������ܣ�	��ʾ��ʽ�༭������
	*	����������	CDlgFormulaEdit* Dialog : [in] ��ʽ�༭������
	*				WPARAM wParam = 0 : [in] ������
	*				LPARAM lParam = 0 : [in] ������
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	void ShowFomulaDlg(CDlgFormulaEdit* Dialog,WPARAM wParam = 0, LPARAM lParam = 0);

	/******************************************************************
	*	�������ܣ�	��ʽ����
	*	����������	DWORD dwMask = 0xFFFF : [in] ���ҷ�Χ
	*				CWnd* pWnd = NULL : [in] ���Ҵ��ڵĸ�����
	*	�� �� ֵ��	long : ���Ҵ��ڵ�ָ��(CDlgFormulaFind*)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	long Find(DWORD dwMask = 0xFFFF,CWnd* pWnd = NULL);

	//������Ϣ�����
	void SendLinkMsg(WPARAM wParam ,LPARAM lPaeam);
	
		// ���ٴ�����ʽ
	//long SpeedCreateTech();
public:
	// ����
	virtual void MyNcDestroy();

	// ��ʽ�Զ���װ
public:
	void AutoSetupExpress(HSOpenExpressUseData* pData);

	// gbq add 20060209, �������Ʋ��ҹ�ʽ
public:
	CExpression* FindExpression(CString strName);
	CExpression* ShowExpression(CString strName);
	void  GetColumnProt();
	BOOL ShowWindow( int nCmdShow );

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULAMAN_H__DF745EF3_1042_11D5_A105_2A829F000000__INCLUDED_)
