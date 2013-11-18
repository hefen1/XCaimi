/*******************************************************
  源程序名称:TradeWarrantEntrustDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  权证行权
  作    者:  shenglq
  开发日期:  20110317
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeWarrantEntrustDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeWarrantEntrustDlg)
public:
	CTradeWarrantEntrustDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeWarrantEntrustDlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};