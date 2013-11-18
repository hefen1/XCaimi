/*******************************************************
  源程序名称:TradeStock9837Dlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  ETF网下股份换购
  作    者:  shenglq
  开发日期:  20100421
*********************************************************/
#pragma once
#include "TradeStockFundTransDlg.h"

class CTradeStock9837Dlg : public CTradeStockFundTransDlg
{
	DECLARE_DYNAMIC(CTradeStock9837Dlg)
public:
	CTradeStock9837Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeStock9837Dlg();
protected:	
	virtual BOOL OnInit();
private:
	DECLARE_MESSAGE_MAP()
};