/*******************************************************
  源程序名称:QueryHoldDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeFund
  功能说明:  查询持仓
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQueryHoldDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQueryHoldDlg)
public:
	CQueryHoldDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQueryHoldDlg();
	virtual DWORD GetDataType();
protected:
	virtual void DoDataCore(IDataList * pDataList);
private:
};