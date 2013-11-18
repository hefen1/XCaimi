/*******************************************************
  源程序名称:QuerySBQuoteDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeStock
  功能说明:  查询三板协议行情
  作    者:  shenglq
  开发日期:  20110323
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQuerySBQuoteDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQuerySBQuoteDlg)
public:
	CQuerySBQuoteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQuerySBQuoteDlg();
	//virtual DWORD GetDataType();
protected:
	//virtual void DoGridDBClick();
	virtual void DoDataCore(IDataList * pDataList);
private:
};