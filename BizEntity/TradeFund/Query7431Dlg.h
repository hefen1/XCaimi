/*******************************************************
  源程序名称:Query7431Dlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeStock
  功能说明:  查询基金定投委托
  作    者:  shenglq
  开发日期:  20110323
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQuery7431Dlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQuery7431Dlg)
public:
	CQuery7431Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQuery7431Dlg();
	//virtual DWORD GetDataType();
protected:
	virtual void DoDataCore(IDataList * pDataList);
	virtual void DoPopupMenuEntrust();
private:
};