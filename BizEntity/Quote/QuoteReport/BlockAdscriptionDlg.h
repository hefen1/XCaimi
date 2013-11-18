#pragma once
#include "afxwin.h"
#include "..\..\..\Resource\QuoteLang\Resource.h"
#include "HSBizDialog.h"
#include "hsstructnew.h"
#include "QuoteLangMngInterFace.h"
#include "ConfigInterface.h"
#include "DataSourceDefineEx.h"

class CBlockAdscriptionDlg :
	public CHSBizDialog
{
	DECLARE_DYNAMIC(CBlockAdscriptionDlg)

public:
	CBlockAdscriptionDlg(CWnd* pParent = NULL);
	virtual ~CBlockAdscriptionDlg(void);

	enum { IDD = IDD_BLOCK_ADSCRIPTION };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	DECLARE_MESSAGE_MAP()
	virtual void OnLinkageMsg(CLinkageParam* pParam);
	long	Reset();
public:
	virtual BOOL OnInitDialog();
	void    SetStockInfo(CodeInfo* pCodeInfo);

	CListCtrl		m_wndBlockList;
	CButton			m_wndRefresh;
	StockUserInfo   m_stockInfo;

	IBlockManager*	m_iBlock;
	IQuoteLang*		m_iQuoteLang;
	IDataSourceEx*  m_pDataSource;     // 数据引擎接口指针
protected:
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnNMDblclkBlockList(NMHDR *pNMHDR, LRESULT *pResult);
};
