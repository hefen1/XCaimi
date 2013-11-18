/*************************************************************
*	Copyright (c)2002, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	WizPageExchModel.h
*	�ļ���ʶ��	���ײ��Բ���ģ������ҳ
*	ժ	  Ҫ��	���ײ��Խ������,�Ժ���д
*
*	��ǰ�汾��	2.0
*	��	  �ߣ�	����ʯ
*	�������ڣ�	2003��4��26��
*
*	�޸�˵����	
*
***************************************************************/
#pragma once
#include <afxwin.h>
#include "afxcmn.h"
#include "WizPageExchBase.h"
#include "KeyboardInterface.h"
#define HS_STOCK_CODE	"��Ʊ����"
#define HS_STOCK_NAME	"��Ʊ����"

// CWizPageExchModel dialog

class CWizPageExchModel : public CWizPageExchBase
{
//	DECLARE_DYNAMIC(CWizPageExchModel)
public:
	CWizPageExchModel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWizPageExchModel();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetIncludeStockList(CArray<StockArea*,StockArea*>* pCheck,BOOL bAdd = FALSE);
	

	DECLARE_MESSAGE_MAP()

public:
	LRESULT OnExchangeData(WPARAM wp,LPARAM lp);

public:
	//CArray<StockArea*,StockArea*>* m_ayCheck; // ��Ʊ��Χ
	int m_nRadioIndex;
	int m_nMultiBuyIndex;
	int m_nOverBuyIndex;
	int m_nBuySortIndex;

	CEdit	m_wndEditLimitCount;
	CEdit	m_wndEditLimitBuyCount;
	CButton m_wndRadioSOrM;
	CButton m_wndRadioBuyAll;
	CButton m_wndRadioBuyRandom;
	CButton m_wndRadioSToB;
	long	m_lInvestMoney;
	CStatic m_wndIncludeStock;
	// ���ɲ��Ի���ȫ�г�����, 0����, 1ȫ�г�
	int m_nSigOrAll;
	// 0ȫ������, 1��������
	int m_nAllBuyOrLimitBuy;
	// 0�������, 1��ָ��������������
	int m_nRandomBuyOrTechSort;
	// 0�Ӵ�С, 1��С����
	int m_nBtosOrStob;
	// ����ָ��
	CStatic m_wndSortTech;

public:
	//��Ʊ�б�
	CArray<StockUserInfo*, StockUserInfo*>	m_ayStkList;
	CArray<StockArea*,StockArea*>*	m_ayCheck;
	CListCtrl								m_ctlStockList;
	int										m_nAddStockIndex;
	int										m_nStockIndex;
	TExchModel								m_TExchangeModel;
	IKeyboard*								m_pKeyBoard;
	void AddStockList(CArray<StockUserInfo*, StockUserInfo*>* pDesc, 
		CArray<StockUserInfo*, StockUserInfo*>* pSour);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedHoletest();
	afx_msg void OnBnClickedSingletest();
	afx_msg void OnBnClickedAllbuy();
	afx_msg void OnBnClickedMostbuy();
	afx_msg void OnBnClickedRandom();
	afx_msg void OnBnClickedSort();
	afx_msg void OnBnClickedChangein();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnNMSetfocusStkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusStkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickStkList(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL OnKillActive(int nPos = -1);
	virtual void OnSetActive();
	virtual BOOL SetData();
	void		 PlayData();

	void		 DelStock(int nIndex);
	void		 DrawStkList();
	void		 InitStkList();
	BOOL		 SetDelStockButton(BOOL bFlag);
	BOOL		 SendAddStockMsg(MSG *pMsg);
	LRESULT		 OnUserDragData(WPARAM wParam, LPARAM lParam);
	LRESULT      OnKeyBoadrMsg(WPARAM wParam, LPARAM lParam);
	BOOL		 AddStock(StockUserInfo* pData);
	void		 SetEnableAddDelButtons(BOOL Flag);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// m_nRandomBuyOrTechSort
	void ShowRandom(int nCmd);
	// m_nAllBuyOrLimitBuy
	void ShowAllBuy(int nCmd);
	// m_nSigOrAll
	void ShowSingl(int nCmd);
	virtual BOOL  InitialData(TExchHead* pData);
};
