/*******************************************************
  源程序名称:TradeDataCore.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易数据数据中心
        1、订阅、取消订阅数据实现
  		2、读取、设置数据接口实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "TradeDataCore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTradeDataCore::CTradeDataCore()
{
	m_pFundList = NULL;
	m_pStockHoldList = NULL;
	m_pStockEntrustList = NULL;
	m_pStockDealList = NULL;
	m_pFundHoldList = NULL;
	m_pFundEntrustList = NULL;
}

CTradeDataCore::~CTradeDataCore()
{

}

void CTradeDataCore::Release()
{
	if (m_pFundList)
	{
		m_pFundList->Release();
	}
	if (m_pStockHoldList)
	{
		m_pStockHoldList->Release();
	}
	if (m_pStockEntrustList)
	{
		m_pStockEntrustList->Release();
	}
	if (m_pStockDealList)
	{
		m_pStockDealList->Release();
	}

	if (m_pFundHoldList)
	{
		m_pFundHoldList->Release();
	}
	if (m_pFundEntrustList)
	{
		m_pFundEntrustList->Release();
	}
	m_SubscribeList.RemoveAll();
	delete this;
}

void CTradeDataCore::Subscribe(ITradeDataCallBack * pOwner)
{
	DWORD nDataType = pOwner->GetDataType();
	if (nDataType)
	{
		m_SubscribeList.Add(pOwner);
		if(m_pFundList && (TRADE_SUB_FUND == (nDataType & TRADE_SUB_FUND)))
		{
			pOwner->OnReflash(m_pFundList, TRADE_SUB_FUND);
		}
		if(m_pStockHoldList && (TRADE_SUB_STOCK_HOLD == (nDataType & TRADE_SUB_STOCK_HOLD)))
		{
			pOwner->OnReflash(m_pStockHoldList, TRADE_SUB_STOCK_HOLD);
		}
		if(m_pStockEntrustList && (TRADE_SUB_STOCK_ENTRUST == (nDataType & TRADE_SUB_STOCK_ENTRUST)))
		{
			pOwner->OnReflash(m_pStockEntrustList, TRADE_SUB_STOCK_ENTRUST);
		}
		if(m_pStockDealList && (TRADE_SUB_STOCK_DEAL == (nDataType & TRADE_SUB_STOCK_DEAL)))
		{
			pOwner->OnReflash(m_pStockDealList, TRADE_SUB_STOCK_DEAL);
		}
		if(m_pFundHoldList && (TRADE_SUB_FUND_HOLD == (nDataType & TRADE_SUB_FUND_HOLD)))
		{
			pOwner->OnReflash(m_pFundHoldList, TRADE_SUB_FUND_HOLD);
		}
		if(m_pFundEntrustList && (TRADE_SUB_FUND_ENTRUST == (nDataType & TRADE_SUB_FUND_ENTRUST)))
		{
			pOwner->OnReflash(m_pFundEntrustList, TRADE_SUB_FUND_ENTRUST);
		}
	}
	
}

void CTradeDataCore::CancelSubscribe(ITradeDataCallBack * pOwner)
{
	if (pOwner->GetDataType())
	{
		for (int i = 0; i < m_SubscribeList.GetCount(); i++)
		{
			ITradeDataCallBack* pItem = m_SubscribeList.GetAt(i);
			if (pOwner == pItem)
			{
				m_SubscribeList.RemoveAt(i);
				break;
			}
		}
	}	
}

IFundList* CTradeDataCore::GetFund()
{
	return m_pFundList;
}
void CTradeDataCore::SetFund(IFundList * pDataList)
{
	for (int i = 0; i < m_SubscribeList.GetCount(); i++)
	{
		ITradeDataCallBack* pItem = m_SubscribeList.GetAt(i);
		if(TRADE_SUB_FUND == (pItem->GetDataType() & TRADE_SUB_FUND))
		{
			pItem->OnReflash(pDataList, TRADE_SUB_FUND);
		}
	}
	if (m_pFundList)
	{
		m_pFundList->Release();
	}
	m_pFundList = pDataList;
}

IStockHoldList* CTradeDataCore::GetStockHold()
{
	return m_pStockHoldList;
}

void CTradeDataCore::SetStockHold(IStockHoldList * pDataList)
{
	for (int i = 0; i < m_SubscribeList.GetCount(); i++)
	{
		ITradeDataCallBack* pItem = m_SubscribeList.GetAt(i);
		if(TRADE_SUB_STOCK_HOLD == (pItem->GetDataType() & TRADE_SUB_STOCK_HOLD))
		{
			pItem->OnReflash(pDataList, TRADE_SUB_STOCK_HOLD);
		}
	}
	if (m_pStockHoldList)
	{
		m_pStockHoldList->Release();
	}
	m_pStockHoldList = pDataList;
}

IStockEntrustList* CTradeDataCore::GetStockEntrust()
{
	return m_pStockEntrustList;
}

void CTradeDataCore::SetStockEntrust(IStockEntrustList * pDataList)
{
	for (int i = 0; i < m_SubscribeList.GetCount(); i++)
	{
		ITradeDataCallBack* pItem = m_SubscribeList.GetAt(i);
		if(TRADE_SUB_STOCK_ENTRUST == (pItem->GetDataType() & TRADE_SUB_STOCK_ENTRUST))
		{
			pItem->OnReflash(pDataList, TRADE_SUB_STOCK_ENTRUST);
		}
	}
	if (m_pStockEntrustList)
	{
		m_pStockEntrustList->Release();
	}
	m_pStockEntrustList = pDataList;
}

IStockDealList* CTradeDataCore::GetStockDeal()
{
	return m_pStockDealList;
}

void CTradeDataCore::SetStockDeal(IStockDealList * pDataList)
{
	for (int i = 0; i < m_SubscribeList.GetCount(); i++)
	{
		ITradeDataCallBack* pItem = m_SubscribeList.GetAt(i);
		if(TRADE_SUB_STOCK_DEAL == (pItem->GetDataType() & TRADE_SUB_STOCK_DEAL))
		{
			pItem->OnReflash(pDataList, TRADE_SUB_STOCK_DEAL);
		}
	}
	if (m_pStockDealList)
	{
		m_pStockDealList->Release();
	}
	m_pStockDealList = pDataList;
}



IFundEntrustList* CTradeDataCore::GetFundEntrust()
{
	return m_pFundEntrustList;
}

void CTradeDataCore::SetFundEntrust(IFundEntrustList * pDataList)
{
	for (int i = 0; i < m_SubscribeList.GetCount(); i++)
	{
		ITradeDataCallBack* pItem = m_SubscribeList.GetAt(i);
		if(TRADE_SUB_FUND_ENTRUST == (pItem->GetDataType() & TRADE_SUB_FUND_ENTRUST))
		{
			pItem->OnReflash(pDataList, TRADE_SUB_FUND_ENTRUST);
		}
	}
	if (m_pFundEntrustList)
	{
		m_pFundEntrustList->Release();
	}
	m_pFundEntrustList = pDataList;
}

IFundHoldList* CTradeDataCore::GetFundHold()
{
	return m_pFundHoldList;
}

void CTradeDataCore::SetFundHold(IFundHoldList * pDataList)
{
	for (int i = 0; i < m_SubscribeList.GetCount(); i++)
	{
		ITradeDataCallBack* pItem = m_SubscribeList.GetAt(i);
		if(TRADE_SUB_FUND_HOLD == (pItem->GetDataType() & TRADE_SUB_FUND_HOLD))
		{
			pItem->OnReflash(pDataList, TRADE_SUB_FUND_HOLD);
		}
	}
	if (m_pFundHoldList)
	{
		m_pFundHoldList->Release();
	}
	m_pFundHoldList = pDataList;
}

