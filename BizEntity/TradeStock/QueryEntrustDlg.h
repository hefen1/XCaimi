/*******************************************************
  源程序名称:QueryEntrustDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeStock
  功能说明:  查询当日委托
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQueryEntrustDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQueryEntrustDlg)
public:
	CQueryEntrustDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQueryEntrustDlg();
	virtual DWORD GetDataType();
protected:
	void DoEntrust();
	virtual void DoDataCore(IDataList * pDataList);
	virtual void DoPopupMenuEntrust();
private:
};