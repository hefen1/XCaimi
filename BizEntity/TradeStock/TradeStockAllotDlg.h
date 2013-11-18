/*******************************************************
  源程序名称:TradeStockAllotDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  场内基金申购
  作    者:  shenglq
  开发日期:  20110316
*********************************************************/
#pragma once
#include "TradeStockBuyDlg.h"

class CTradeStockAllotDlg : public CTradeStockBuyDlg
{
	DECLARE_DYNAMIC(CTradeStockAllotDlg)
public:
	CTradeStockAllotDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeStockAllotDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual void LoadWnd();
	virtual void OnAmountEnter();
	virtual BOOL Validate();
private:
	DECLARE_MESSAGE_MAP()

};