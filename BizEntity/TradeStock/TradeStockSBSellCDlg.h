/*******************************************************
  源程序名称:TradeStockSBSellCDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  三板确定卖出
  作    者:  shenglq
  开发日期:  20110323
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeStockSBSellCDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeStockSBSellCDlg)
public:
	CTradeStockSBSellCDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeStockSBSellCDlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};