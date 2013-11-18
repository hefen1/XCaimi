#pragma once
#include "afxcmn.h"
#include "DlgIndexUserDef.h"
//#include "MyListCtrl.h"

// CDlgSelStock 对话框
enum{SystemClassification = 0, SystemBlock, UserBlock};

class CDlgSelStock : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelStock)

public:
	CDlgSelStock(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSelStock();

// 对话框数据
	enum { IDD = IDD_SELECTSTOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnLvnItemchangedStocktype(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedStocklist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkStocklist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeSystemblockentry(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

	void requestStockTypeInfo( int CurSel );
	void requestStockInfo(int listIndex);

	void InsertListStock(const CArray<StockUserInfo>& m_stockInfo);
//私有属性
private:
	CTabCtrl m_tabctrl;
	CListCtrl m_listStock;
	CListCtrl m_listType;
	CTreeCtrl m_treeCtrl;
	CArray<StockType> m_stockType;
	int m_nTypeLastIndex;
	CArray<StockUserInfo> m_ayStkInfo;
	int m_nInfoLastIndex;
	CImageList m_listImage;
//公有属性
public:
	StockUserInfo m_stockInfoSelected;
	

};
