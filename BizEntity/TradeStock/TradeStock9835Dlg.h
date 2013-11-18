/*******************************************************
  源程序名称:TradeStock9835Dlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  ETF网下现金认购
  作    者:  shenglq
  开发日期:  20110421
*********************************************************/
#pragma once
#include "TradeStockSubcribeDlg.h"

class CTradeStock9835Dlg : public CTradeStockSubcribeDlg
{
	DECLARE_DYNAMIC(CTradeStock9835Dlg)
public:
	CTradeStock9835Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeStock9835Dlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
private:
	DECLARE_MESSAGE_MAP()

};