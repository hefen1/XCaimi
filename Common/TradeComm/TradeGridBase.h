/*******************************************************
  Դ��������:TradeGridBase.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����GRID����
  ��    ��:  shenglq
  ��������:  20100910
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