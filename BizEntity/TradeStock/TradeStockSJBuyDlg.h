/*******************************************************
  源程序名称:TradeStockSJBuyDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易证券市价买入
  作    者:  shenglq
  开发日期:  20110121
*********************************************************/
#pragma once
#include "TradeStockBuyDlg.h"

class CTradeStockSJBuyDlg : public CTradeStockBuyDlg
{
	DECLARE_DYNAMIC(CTradeStockSJBuyDlg)
public:
	CTradeStockSJBuyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeStockSJBuyDlg();
	//virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//virtual BOOL OnInit();
	//virtual void OnStockCodeExit();
	//virtual void OnEntrustPriceExit();
	//virtual void DoEntrust();
	virtual void LoadWnd();
private:
	//void QueryEnableBalance();
	//void QueryEnableAmount();

	DECLARE_MESSAGE_MAP()

};