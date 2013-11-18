/*******************************************************
  源程序名称:TradeStockSubcribeDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  场内基金认购
  作    者:  shenglq
  开发日期:  20110316
*********************************************************/
#pragma once
#include "TradeStockAllotDlg.h"

class CTradeStockSubcribeDlg : public CTradeStockAllotDlg
{
	DECLARE_DYNAMIC(CTradeStockSubcribeDlg)
public:
	CTradeStockSubcribeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeStockSubcribeDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual BOOL Validate();
private:
	DECLARE_MESSAGE_MAP()

};