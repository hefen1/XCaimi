/*******************************************************
  源程序名称:TradeStockSetCostPriceDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  重置成本价窗口基类
  作    者:  shenglq
  开发日期:  20100903
*********************************************************/
#pragma once
#include "TradeStock.h"
#include "TradeDialogBase.h"
#include "QueryStockAccountIntf.h"
#include "HSNumEdit.h"
#include "afxcmn.h"
#include "afxwin.h"


class CTradeStockSetCostPriceDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CTradeStockSetCostPriceDlg)
public:
	CTradeStockSetCostPriceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeStockSetCostPriceDlg();
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);

	// 对话框数据
	enum { IDD = IDD_DIALOG_SETCOSTPRICE};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual void OnStockCodeExit();
	//检查委托数据
	virtual BOOL Validate();
	virtual void DoEntrust();
	virtual void OnWindowFocused();
	virtual void OnHotKey(UINT nId);
	virtual void OnLinkageMsg(CLinkageParam* pParam);
private:
	CString m_sLastStockCode;
	IStockAccountList* m_pStockAccountList;
	ITradeMarketList* m_pTradeMarketList;
	IStockHoldList* m_pCurrHoldList;
	void Clear(int iFlag = 0);
	void SelectStockAccount();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbStockAccount;
	CEdit m_edStockCode;
	CHSNumEdit m_edPrice;
	CButton m_btOK;
	CStatic m_edStockName;
	afx_msg void OnBnClickedBtOk();
	afx_msg void OnEnKillfocusEdStockcode();
	afx_msg void OnEnChangeEdStockcode();
};