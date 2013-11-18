/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgTailorSelStock.h
*	�ļ���ʶ��	����ѡ���ඨ�� 
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_TAILORSELECTSTOCK_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_)
#define AFX_TAILORSELECTSTOCK_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HSCondition.h : header file
//
#include "resource.h"
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"
#include "DlgUnionList.h"
#include "ExpPublic.h"
// CDlgTailorSelStock dialog

class CDlgTailorSelStock : public CDialog
{	
	DECLARE_DYNAMIC(CDlgTailorSelStock)

public:
	CDlgTailorSelStock(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL,
		long* pThis = NULL);   // standard constructor
	virtual ~CDlgTailorSelStock();

// Dialog Data
	enum { IDD = IDD_TAILORSELECTSTOCK };

	CStatic	m_wndUnionDlgPos;
	CButton	m_wndExpNotes;
	CStatic	m_wndMiddle;
	CComboBox	m_wndData2;
	CComboBox	m_wndData1;
	CComboBox	m_wndCompare;
	CStatic		m_wndAndOr;
	CExpTabCtrl	m_wndTab;
	CStatic		m_wndSelectStock;
	CStatic		m_wndTotalStock;
	CStatic		m_wndIncludeStock;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	

	// Generated message map functions
	//{{AFX_MSG(CHSCondition)

	// "��Χ"��ť����
	afx_msg void OnAdvance();
	// ȡ������
	virtual void OnCancel();
	// "ע��"��ť����
	afx_msg void OnUsernotes();
	// "ִ��"�ť����
	virtual void OnOK();
	// "ѡ�ɵ����"��ť����
	afx_msg void OnToblock();
	// "�޸Ĺ�ʽ"��ť����
	afx_msg void OnButexpress();
	// "�޸Ĳ���"��ť����
	afx_msg void OnButparam();
	// ��ʽ�б�TAB�ı䴦��
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	// ����"�ر�"��ť����
	afx_msg void OnBnClickedClose();
	// �ı�ȽϷ�ʽ����
	afx_msg void OnSelchangeCompare();	
    //}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	/******************************************************************
	*	�������ܣ�	��ʽ������ʱ,��������Ϣ����Ӧ����
	*	����������	WPARAM wParam = 0 : [in] TEMPUNION_NEWCONDITION -- ������������ lParamΪCDlgUnionList*��������, ����0
												TEMPUNION_SAVEUNION -- ������ϴ��� lParamΪpTempUnion*��������, ����0
												 TEMPUNION_DELUNION -- ɾ����ϴ��� lParamΪUnionSelCondition*����, ����0
												   HX_UNION_EXPRESS -- ��Ϲ�ʽ���� û����ȷ��������, lParamΪUnionSelCondition*����, ����0 
											CTreeCtrlFormula::HSBrowser -- ��Ϲ�ʽ����,ͬʱ�����޸�ָ��,�޸Ĺ�ʽ,ע��˵����ť, lParamΪCTreeGroup*��������, ����0
									CCompileEditView::UpdateExpress -- ��ʽ��������Ϣ����, ͬʱ���Լ�����HX_EXPRESSSENTENCE����Ϣ, lParam����, ����0
												 HX_EXPRESSSENTENCE -- ѡ�й�ʽ����,lParamΪCExpression*����, ����0
	*				LPARAM lParam = 0 : [in] ��wParam���
	*	�� �� ֵ��	long : ��Ϊ0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��9��
	*	�޸�˵����	
	*****************************************************************/
	long    LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	/******************************************************************
	*	�������ܣ�	HX_USER_DRAGDATA�û���Ϣ����
	*	����������	WPARAM wParam : [in] ����ΪHX_USER_DRAGDATA-1��������
	*				LPARAM lParam : [in] ΪCExpression*����,��ǰѡ �еĹ�ʽ 
	*	�� �� ֵ��	LRESULT : ���˴���(1), ��������(0)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��9��
	*	�޸�˵����	
	*****************************************************************/
	virtual LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);

	/******************************************************************
	*	�������ܣ�	HX_USER_COMPILEDATA�û���Ϣ��Ӧ
	*	����������	WPARAM wp : [in] 2	--	�ı���������,ͬʱֹͣ��ʼѡ��(ȡԭ״̬�ķ�״̬), 
											lp ΪCString*������,ָ�����õİ����
											����0
								     3	--	���ص�ǰѡ�й�Ʊ
											lp ������
									 4	--	ѡ�ɽ���,����0
									 5	--	���õ�ǰ��Ʊ��
											lp ΪCNI_ReceiveData*������,pInfo2->m_nStatusΪѡ����, pInfo2->m_lDataTotalΪ�ܹ�Ʊ��
											����0
									 6	--	�������б����ҵĹ�Ʊ��
											lp Ϊint������, ָ���б����ҵĹ�Ʊ��
											����0
	*				LPARAM lp : [in] ��WP���,�뿴WP˵��
	*	�� �� ֵ��	LRESULT : ��WP���,�뿴WP˵��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��9��
	*	�޸�˵����	
	*****************************************************************/
	LRESULT OnExchangeData(WPARAM wp,LPARAM lp);

protected:
	HSOpenExpressUseData* m_pData; // ����û��( ??? )
	bool m_bSelStockState ; // ѡ��״̬
public:

	CDlgUnionList m_wndTempUnionDlg;  // ��������б�ؼ�

	CArray<StockArea*,StockArea*> m_ayCheck; // ��Ʊ��Χ
	CString				  m_strBlock;              // �����

	SingleSelCondition		  m_sSingleSel;			   // ����
	UnionSelCondition			  m_pHSUnion;			   // ���

	SelStockCondition  m_curSelStockCondition; // ����ѡ�ɽṹ
	// �Ƿ�ʹ�ó�Ȩ�����ݼ���
	BOOL m_bCheckChuQuan;

protected:
	// ��ʼ����ֹͣѡ��
	int  Select();
	// ��ʼ����ʽ��
	void Init(int nCopy = CTreeCtrlFormula::CopyDelete);
	int  DoSelectStock( SelStockCondition*, int style);


public:
	// ��ʼ���Ի���
	virtual BOOL OnInitDialog();
	// û��
	virtual void MyNcDestroy();

	/******************************************************************
	*	�������ܣ�	������ʱʹ��,�ӻ���������ID�õ�����(�ж������)
	*	����������	CString& strFindKeyName : [in] Ӧ��һ�������ִ�,��Ӧ���������ݵ�ID��
	*	�� �� ֵ��	CString : �õ���ӦID���ݵ�����
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��9��
	*	�޸�˵����	
	*****************************************************************/
	static CString  GetNameByKey(CString& strFindKeyName);
	// ���������ݻ��ǻ������ݰ���ʽ�����뵽��ʽ����
	void		InitCWData(CString strName);
	//	��ʼ���ȽϷ�ʽ����
	static void		InitCondition(CComboBox* pBox,int nType);

public:
	// ���湫ʽ��ɾ������˽�пռ�
	void Delete();
	//void SetSize();

public:
	/******************************************************************
	*	�������ܣ�	װ�����д�����������ʽ����
	*	����������	UINT nMode : [in] װ�뻹��д��
	*				CString strFileName = "" : [in] �ļ���
	*				DWORD dStyle = 0 : [in] ��� 
	*	�� �� ֵ��	BOOL 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��9��
	*	�޸�˵����	
	*****************************************************************/
	BOOL SaveLoad(UINT nMode,CString strFileName = "",DWORD dStyle = 0);	
	// ��������Ϲ�ʽ
	virtual void Serialize(CArchive& ar,int& nCountRef);
	BOOL ShowWindow( int nCmdShow );

protected:
	// ��ʽ���� ����ý�û��ѽ? �Ҳ�֪�����ʱ���������Ա��ʲô�� ???
	CArray<CTreeGroup*,CTreeGroup*> m_ayTreeGroup;

public:
	// ����"����"��ť����
	afx_msg void OnBnClickedButfind();
};




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAILORSELECTSTOCK_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_)

