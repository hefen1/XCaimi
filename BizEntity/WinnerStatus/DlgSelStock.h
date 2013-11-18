#pragma once
#include "afxcmn.h"
#include "DlgIndexUserDef.h"
//#include "MyListCtrl.h"

// CDlgSelStock �Ի���
enum{SystemClassification = 0, SystemBlock, UserBlock};

class CDlgSelStock : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelStock)

public:
	CDlgSelStock(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSelStock();

// �Ի�������
	enum { IDD = IDD_SELECTSTOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
//˽������
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
//��������
public:
	StockUserInfo m_stockInfoSelected;
	

};
