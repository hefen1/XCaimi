/*******************************************************
  源程序名称:TradeRZBuyDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易证券融资买入
  作    者:  shenglq
  开发日期:  20110121
*********************************************************/
#pragma once
#include "TradeStockBuyDlg.h"

class CTradeRZBuyDlg : public CTradeStockBuyDlg
{
	DECLARE_DYNAMIC(CTradeRZBuyDlg)
public:
	CTradeRZBuyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeRZBuyDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()

};