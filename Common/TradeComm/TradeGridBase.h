/*******************************************************
  源程序名称:TradeGridBase.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易GRID基类
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#pragma once
#include "..\..\Controls\CommControls\HSGridTable.h"
#include "TradeDataBaseIntf.h"
#include "TradeTablesIntf.h"

class CTradeGridBase : public CHSGridTable
{
	DECLARE_DYNAMIC(CTradeGridBase)
public:
	CTradeGridBase(int nRows = 1, int nCols = 0, int nFixedRows = 1, int nFixedCols = 0);
	virtual ~CTradeGridBase();
	int GetCurrRow();
	int GetCurrCol();
	void FillHeader(ITradeTable* pTradeTable);
	void FillGrid(IDataList* pDataList);
protected:
	ITradeTable* m_pTradeTable;
	IDataList* m_pDataList;
private:
};