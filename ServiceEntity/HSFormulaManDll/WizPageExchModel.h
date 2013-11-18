/*************************************************************
*	Copyright (c)2002, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	WizPageExchModel.h
*	文件标识：	交易测试测试模型设置页
*	摘	  要：	交易测试界面相关,以后再写
*
*	当前版本：	2.0
*	作	  者：	俞礼石
*	更新日期：	2003年4月26日
*
*	修改说明：	
*
***************************************************************/
#pragma once
#include <afxwin.h>
#include "afxcmn.h"
#include "WizPageExchBase.h"
#include "KeyboardInterface.h"
#define HS_STOCK_CODE	"股票代码"
#define HS_STOCK_NAME	"股票名称"

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
	//CArray<StockArea*,StockArea*>* m_ayCheck; // 股票范围
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
	// 单股测试还是全市场测试, 0单股, 1全市场
	int m_nSigOrAll;
	// 0全部买入, 1限制买入
	int m_nAllBuyOrLimitBuy;
	// 0随机买入, 1按指标数据排序买入
	int m_nRandomBuyOrTechSort;
	// 0从大到小, 1从小到大
	int m_nBtosOrStob;
	// 排序指标
	CStatic m_wndSortTech;

public:
	//股票列表
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
