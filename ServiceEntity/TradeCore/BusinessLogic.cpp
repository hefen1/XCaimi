/*******************************************************
  源程序名称:Businesslogic.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易业务逻辑
  			1、委托接口实现
  			2、查询接口实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "Businesslogic.h"
#include "TradeCore.h"
#include "TradeStockEntrustIntf.h"
#include "TradeStockWithDrawIntf.h"
#include "TradeAction.h"
#include "TradeFundEntrustIntf.h"
#include "BankStockTransferIntf.h"
#include "ChangePasswordIntf.h"
#include "TradeSetStockCostPriceIntf.h"
#include "TradeStockZJHKIntf.h"
#include "TradeStockZJHQIntf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern ITradeCoreObj* pTradeCoreObj;

CBusinessLogic::CBusinessLogic()
{
	m_StockAccountList = NULL;	
	m_FundInfoList = NULL;
	m_BankList = NULL;
	m_UserInfoList = NULL;
	m_StockFundInfoList = NULL;
	m_FundStockAccountList = NULL;
	m_FundCompanyList = NULL;
}

CBusinessLogic::~CBusinessLogic()
{
	
	if (m_StockAccountList)
	{
		m_StockAccountList->Release();
	}
	if (m_FundInfoList)
	{
		m_FundInfoList->Release();
	}
	if (m_BankList)
	{
		m_BankList->Release();
	}
	if (m_UserInfoList)
	{
		m_UserInfoList->Release();
	}
	if (m_StockFundInfoList)
	{
		m_StockFundInfoList->Release();
	}
	if (m_FundStockAccountList)
	{
		m_FundStockAccountList->Release();
	}
	if (m_FundCompanyList)
	{
		m_FundCompanyList->Release();
	}
	
}

void CBusinessLogic::Release()
{
	delete this;
}

void CBusinessLogic::SetAccount(IAccount* lpAccount)
{
	m_Account = lpAccount;
}

IStockAccountList* CBusinessLogic::GetStockAccount(CString& sMsg)
{
	if (m_StockAccountList)
	{
		return m_StockAccountList;
	}
	try
	{
		IReqQueryStockAccount* pIntf = dynamic_cast<IReqQueryStockAccount*>(pTradeCoreObj->NewTradingByAction(TRADE_STOCK_QUERYACCOUNT));
		pIntf->Clear();
		pIntf->SetExchangeType("");
		pIntf->SetStockAccount("");
		pIntf->SetAccount(m_Account);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						m_StockAccountList = dynamic_cast<IStockAccountList*>(pList);
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
						pIntf->FreeDataList();
						
					}
				}
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return m_StockAccountList;
}

BOOL CBusinessLogic::StockEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqStockEntrust* pIntf = dynamic_cast<IReqStockEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_STOCK_ENTRUST));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetExchangeType(sExchangeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetStockCode(sStockCode);
		pIntf->SetEntrustBs(eEntrustBs);
		pIntf->SetEntrustPrice(dEntrustPrice);
		pIntf->SetEntrustAmount(iEntrustAmount);
		pIntf->SetEntrustProp(sEntrustProp);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetEntrustNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}

				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::StockWithDraw(const CString sExchangeType, const CString sStockAccount, const CString sEntrustNo, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqStockWithDraw* pIntf = dynamic_cast<IReqStockWithDraw*>(pTradeCoreObj->NewTradingByAction(TRADE_STOCK_WITHDRAW));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetExchangeType(sExchangeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetEntrustNo(sEntrustNo);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg = pItem->GetEntrustNo();
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}

				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

//////////////////////////////////////////////////////////////////////////

IFundInfoList* CBusinessLogic::GetFundInfo(CString& sMsg)
{
	if (m_FundInfoList)
	{
		return m_FundInfoList;
	}
	try
	{
		IReqQueryFundInfo* pIntf = dynamic_cast<IReqQueryFundInfo*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_QUERYFUNDINFO));
		pIntf->Clear();
		pIntf->SetFundCompany("");
		pIntf->SetFundCode("");
		pIntf->SetAccount(m_Account);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						m_FundInfoList = dynamic_cast<IFundInfoList*>(pList);
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
						pIntf->FreeDataList();
					}
				}
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}
	return m_FundInfoList;
}
//////////////////////////////////////////////////////////////////////////

IFundStockAccountList* CBusinessLogic::GetFundStockAccount(CString& sMsg, BOOL bQueryFlag )
{
	if (bQueryFlag && m_FundStockAccountList)
	{
		m_FundStockAccountList->Release();
		m_FundStockAccountList = NULL;
	}

	if (m_FundStockAccountList)
	{
		return m_FundStockAccountList;
	}
	try
	{
		IReqQueryFundStockAccount* pIntf = dynamic_cast<IReqQueryFundStockAccount*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_QUERYSTOCKACCOUNT));
		pIntf->Clear();
		pIntf->SetFundCompany("");
		pIntf->SetAccount(m_Account);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						m_FundStockAccountList = dynamic_cast<IFundStockAccountList*>(pList);
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
						pIntf->FreeDataList();
					}
				}
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}
	return m_FundStockAccountList;
}
//////////////////////////////////////////////////////////////////////////
IFundCompanyList* CBusinessLogic::GetFundCompany(CString& sMsg)
{
	if (m_FundCompanyList)
	{
		return m_FundCompanyList;
	}
	try
	{
		IReqQueryFundCompany* pIntf = dynamic_cast<IReqQueryFundCompany*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_QUERYFUNDCOMPANY));
		pIntf->Clear();
		pIntf->SetFundCompany("");
		pIntf->SetAccount(m_Account);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						m_FundCompanyList = dynamic_cast<IFundCompanyList*>(pList);
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
						pIntf->FreeDataList();
					}
				}
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}
	return m_FundCompanyList;
}
//////////////////////////////////////////////////////////////////////////

BOOL CBusinessLogic::FundSubcribe(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		CString sTmp;
		IFundStockAccountList* pFundStockAccountList = GetFundStockAccount(sTmp, FALSE);
		CString sStockAccount = "";
		CString sTransAccount = "";
		if (pFundStockAccountList)
		{
			IFundStockAccountItem* pItem = pFundStockAccountList->GetByFundCompany(sFundCompany);
			if (pItem)
			{
				sStockAccount = pItem->GetStockAccount();
				sTransAccount = pItem->GetTransAccount();
			}
		}
		IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_SUBSCRIBE));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetFundCompany(sFundCompany);
		pIntf->SetFundCode(sFundCode);
		pIntf->SetBalance(dBalance);
		pIntf->SetChargeType(sChargeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetTransAccount(sTransAccount);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetSerialNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::FundAllot(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		CString sTmp;
		IFundStockAccountList* pFundStockAccountList = GetFundStockAccount(sTmp, FALSE);
		CString sStockAccount = "";
		CString sTransAccount = "";
		if (pFundStockAccountList)
		{
			IFundStockAccountItem* pItem = pFundStockAccountList->GetByFundCompany(sFundCompany);
			if (pItem)
			{
				sStockAccount = pItem->GetStockAccount();
				sTransAccount = pItem->GetTransAccount();
			}
		}
		IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_ALLOT));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetFundCompany(sFundCompany);
		pIntf->SetFundCode(sFundCode);
		pIntf->SetBalance(dBalance);
		pIntf->SetChargeType(sChargeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetTransAccount(sTransAccount);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetSerialNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::FundRedeem(const CString sFundCompany, const CString sFundCode, const double dAmount, const CString sAllotDate, const CString sAllotNo, const CString sExceedFlag, const CString sChargeType, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		CString sTmp;
		IFundStockAccountList* pFundStockAccountList = GetFundStockAccount(sTmp, FALSE);
		CString sStockAccount = "";
		CString sTransAccount = "";
		if (pFundStockAccountList)
		{
			IFundStockAccountItem* pItem = pFundStockAccountList->GetByFundCompany(sFundCompany);
			if (pItem)
			{
				sStockAccount = pItem->GetStockAccount();
				sTransAccount = pItem->GetTransAccount();
			}
		}
		IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_REDEEM));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetFundCompany(sFundCompany);
		pIntf->SetFundCode(sFundCode);
		pIntf->SetAmount(dAmount);
		pIntf->SetChargeType(sChargeType);
		pIntf->SetAllotDate(sAllotDate);
		pIntf->SetAllotNo(sAllotNo);
		pIntf->SetExceedFlag(sExceedFlag);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetTransAccount(sTransAccount);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetSerialNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::FundWithDraw(const CString sFundCompany, const CString sFundCode, const CString sEntrustNo, const CString sAllotDate, const CString sAllotNo,CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_WITHDRAW));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetFundCompany(sFundCompany);
		pIntf->SetFundCode(sFundCode);
		pIntf->SetAllotDate(sAllotDate);
		pIntf->SetAllotNo(sAllotNo);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg = pItem->GetSerialNo();
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::FundSetDividend(const CString sFundCompany, const CString sFundCode, const CString sDividendMethod, const CString sChargeType, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		CString sTmp;
		IFundStockAccountList* pFundStockAccountList = GetFundStockAccount(sTmp, FALSE);
		CString sStockAccount = "";
		CString sTransAccount = "";
		if (pFundStockAccountList)
		{
			IFundStockAccountItem* pItem = pFundStockAccountList->GetByFundCompany(sFundCompany);
			if (pItem)
			{
				sStockAccount = pItem->GetStockAccount();
				sTransAccount = pItem->GetTransAccount();
			}
		}
		IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_SETDIVIDEND));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetFundCompany(sFundCompany);
		pIntf->SetFundCode(sFundCode);
		pIntf->SetDividendMethod(sDividendMethod);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetTransAccount(sTransAccount);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托受理成功，合同号：%s", pItem->GetSerialNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
return bReturn;
}

BOOL CBusinessLogic::FundTrans(const CString sFundCompany, const CString sFundCode,const CString sTransCode, const double dTransAmount, const CString sAllotDate, const CString sAllotNo, const CString sChargeType, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		CString sTmp;
		IFundStockAccountList* pFundStockAccountList = GetFundStockAccount(sTmp, FALSE);
		CString sStockAccount = "";
		CString sTransAccount = "";
		if (pFundStockAccountList)
		{
			IFundStockAccountItem* pItem = pFundStockAccountList->GetByFundCompany(sFundCompany);
			if (pItem)
			{
				sStockAccount = pItem->GetStockAccount();
				sTransAccount = pItem->GetTransAccount();
			}
		}
		IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_TRANS));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetFundCompany(sFundCompany);
		pIntf->SetFundCode(sFundCode);
		pIntf->SetTransCode(sTransCode);
		pIntf->SetTransAmount(dTransAmount);
		pIntf->SetChargeType(sChargeType);
		pIntf->SetAllotDate(sAllotDate);
		pIntf->SetAllotNo(sAllotNo);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetTransAccount(sTransAccount);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetSerialNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::FundSetRiskLevel(const CString sRiskLevel, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_SETRISKLEVEL));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetClientRiskLevel(sRiskLevel);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("设置成功！");
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::FundHopeAllot(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, const CString sHopeDate, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		CString sTmp;
		IFundStockAccountList* pFundStockAccountList = GetFundStockAccount(sTmp, FALSE);
		CString sStockAccount = "";
		CString sTransAccount = "";
		if (pFundStockAccountList)
		{
			IFundStockAccountItem* pItem = pFundStockAccountList->GetByFundCompany(sFundCompany);
			if (pItem)
			{
				sStockAccount = pItem->GetStockAccount();
				sTransAccount = pItem->GetTransAccount();
			}
		}
		IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_HOPEALLOT));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetFundCompany(sFundCompany);
		pIntf->SetFundCode(sFundCode);
		pIntf->SetBalance(dBalance);
		pIntf->SetChargeType(sChargeType);
		pIntf->SetHopeDate(sHopeDate);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetTransAccount(sTransAccount);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetSerialNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::FundHopeRedeem(const CString sFundCompany, const CString sFundCode, const double dAmount, const CString sAllotDate, const CString sAllotNo, const CString sExceedFlag, const CString sChargeType, const CString sHopeDate, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		CString sTmp;
		IFundStockAccountList* pFundStockAccountList = GetFundStockAccount(sTmp, FALSE);
		CString sStockAccount = "";
		CString sTransAccount = "";
		if (pFundStockAccountList)
		{
			IFundStockAccountItem* pItem = pFundStockAccountList->GetByFundCompany(sFundCompany);
			if (pItem)
			{
				sStockAccount = pItem->GetStockAccount();
				sTransAccount = pItem->GetTransAccount();
			}
		}
		IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_HOPEREDEEM));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetFundCompany(sFundCompany);
		pIntf->SetFundCode(sFundCode);
		pIntf->SetAmount(dAmount);
		pIntf->SetChargeType(sChargeType);
		pIntf->SetAllotDate(sAllotDate);
		pIntf->SetAllotNo(sAllotNo);
		pIntf->SetExceedFlag(sExceedFlag);
		pIntf->SetHopeDate(sHopeDate);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetTransAccount(sTransAccount);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetSerialNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::Fund7432(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, const CString sBeginDate, const CString sEndDate, const CString sFundDate, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		CString sTmp;
		IFundStockAccountList* pFundStockAccountList = GetFundStockAccount(sTmp, FALSE);
		CString sStockAccount = "";
		CString sTransAccount = "";
		if (pFundStockAccountList)
		{
			IFundStockAccountItem* pItem = pFundStockAccountList->GetByFundCompany(sFundCompany);
			if (pItem)
			{
				sStockAccount = pItem->GetStockAccount();
				sTransAccount = pItem->GetTransAccount();
			}
		}
		IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_FUND_7432));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetFundCompany(sFundCompany);
		pIntf->SetFundCode(sFundCode);
		pIntf->SetBalance(dBalance);
		pIntf->SetChargeType(sChargeType);
		pIntf->SetBeginDate(sBeginDate);
		pIntf->SetEndDate(sEndDate);
		pIntf->SetFundDate(sFundDate);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetTransAccount(sTransAccount);
		pIntf->SetRequestARData("product_type", "00");
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetSerialNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}


IBankList* CBusinessLogic::GetBankInfo(CString& sMsg)
{
	if (m_BankList)
	{
		return m_BankList;
	}
	try
	{
		IReqQueryBank* pIntf = dynamic_cast<IReqQueryBank*>(pTradeCoreObj->NewTradingByAction(TRADE_QUERYBANK));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						m_BankList = dynamic_cast<IBankList*>(pList);
					}
					else
					{
						pIntf->FreeDataList();
						sMsg = pItem->GetErrorInfo();
						if (sMsg.IsEmpty())
						{
							sMsg = "没有存管银行！";
						}
					}
				}
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return m_BankList;
}

BOOL CBusinessLogic::BankStockTransfer(const EMoneyType mtValue, const CString sBankNo, const CString sBankPsw, const CString sFundPsw, const CString sDirection, const double dBalance, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		int nAction = TRADE_BANKTRANSFER;
		if ("3" == sDirection)
		{
			nAction = TRADE_QUERYBANKBALANCE;
		}
		IReqBankStockTransfer* pIntf = dynamic_cast<IReqBankStockTransfer*>(pTradeCoreObj->NewTradingByAction(nAction));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetMoneyType(mtValue);
		pIntf->SetBankNo(sBankNo);
		pIntf->SetBankPassword(sBankPsw);
		pIntf->SetFundPassword(sFundPsw);
		pIntf->SetTransferDirection(sDirection);
		pIntf->SetOccurBalance(dBalance);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg = "请求已发送成功，请查询转帐流水！";
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::ChangePassword(const CString sPasswordType, const CString sOldPassword, const CString sNewPassword, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqChangePassword* pIntf = dynamic_cast<IReqChangePassword*>(pTradeCoreObj->NewTradingByAction(TRADE_CHANGEPASSWORD));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetPasswordType(sPasswordType);
		pIntf->SetOldPassword(sOldPassword);
		pIntf->SetNewPassword(sNewPassword);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg = "密码修改成功！";
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::SetStockCostPrice(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, const double dCostPrice, const CString sSeat, const double dEntrustBalance, const CString sRemark, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqSetStockCostPrice* pIntf = dynamic_cast<IReqSetStockCostPrice*>(pTradeCoreObj->NewTradingByAction(TRADE_STOCK_SETCOSTPRICE));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetExchangeType(sExchangeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetStockCode(sStockCode);
		pIntf->SetCostPrice(dCostPrice);
		pIntf->SetSeat(sSeat);
		pIntf->SetEntrustBalance(dEntrustBalance);
		pIntf->SetRemark(sRemark);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg = "成本价重置成功！";
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}
				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

IUserInfoItem* CBusinessLogic::GetUserInfo(CString& sMsg, BOOL bQueryFlag /* = FALSE */)
{
	if (bQueryFlag && m_UserInfoList)
	{
		m_UserInfoList->Release();
		m_UserInfoList = NULL;
	}
	if (m_UserInfoList)
	{
		return m_UserInfoList->GetItems(0);
	}
	try
	{
		IReqQueryUserInfo* pIntf = dynamic_cast<IReqQueryUserInfo*>(pTradeCoreObj->NewTradingByAction(TRADE_QUERYUSERINFO));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						m_UserInfoList = dynamic_cast<IUserInfoList*>(pList);
						return m_UserInfoList->GetItems(0);
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
						pIntf->FreeDataList();
					}
				}
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}
	return NULL;
}

BOOL CBusinessLogic::StockSBEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, const CString sSeatNo, const CString sConferNo, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqStockEntrust* pIntf = dynamic_cast<IReqStockEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_STOCK_SBENTRUST));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetExchangeType(sExchangeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetStockCode(sStockCode);
		pIntf->SetEntrustBs(eEntrustBs);
		pIntf->SetEntrustPrice(dEntrustPrice);
		pIntf->SetEntrustAmount(iEntrustAmount);
		pIntf->SetEntrustProp(sEntrustProp);
		pIntf->SetOppoSeatNo(sSeatNo);
		pIntf->SetConferNo(sConferNo);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetEntrustNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}

				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::StockSBWithDraw(const CString sExchangeType, const CString sStockAccount, const CString sEntrustNo, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqStockWithDraw* pIntf = dynamic_cast<IReqStockWithDraw*>(pTradeCoreObj->NewTradingByAction(TRADE_STOCK_SBWITHDRAW));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetExchangeType(sExchangeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetEntrustNo(sEntrustNo);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg = pItem->GetEntrustNo();
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}

				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::StockFundEntrust(const int iAction, const CString sExchangeType, const CString sStockAccount, const CString sStockCode, const int iEntrustAmount, const CString sTransCode, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqStockEntrust* pIntf = dynamic_cast<IReqStockEntrust*>(pTradeCoreObj->NewTradingByAction(iAction));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetExchangeType(sExchangeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetStockCode(sStockCode);
		pIntf->SetEntrustAmount(iEntrustAmount);
		pIntf->SetTransCode(sTransCode);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetEntrustNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}

				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::StockFundWithDraw(const CString sExchangeType, const CString sStockCode, const CString sEntrustNo, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqStockWithDraw* pIntf = dynamic_cast<IReqStockWithDraw*>(pTradeCoreObj->NewTradingByAction(TRADE_STOCK_SBWITHDRAW));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetExchangeType(sExchangeType);
		pIntf->SetStockCode(sStockCode);
		pIntf->SetEntrustNo(sEntrustNo);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg = pItem->GetEntrustNo();
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}

				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

IStockFundInfoList* CBusinessLogic::GetStockFundInfo(CString& sMsg)
{
	if (m_StockFundInfoList)
	{
		return m_StockFundInfoList;
	}
	try
	{
		IReqQueryStockFundInfo* pIntf = dynamic_cast<IReqQueryStockFundInfo*>(pTradeCoreObj->NewTradingByAction(TRADE_STOCK_QUERYFUNDINFO));
		pIntf->Clear();
		pIntf->SetExchangeType("");
		pIntf->SetAccount(m_Account);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						m_StockFundInfoList = dynamic_cast<IStockFundInfoList*>(pList);
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
						pIntf->FreeDataList();

					}
				}
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return m_StockFundInfoList;
}

BOOL CBusinessLogic::RZRQEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, const CString sEntrustType, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqStockEntrust* pIntf = dynamic_cast<IReqStockEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_RZRQ_ENTRUST));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetExchangeType(sExchangeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetStockCode(sStockCode);
		pIntf->SetEntrustBs(eEntrustBs);
		pIntf->SetEntrustPrice(dEntrustPrice);
		pIntf->SetEntrustAmount(iEntrustAmount);
		pIntf->SetEntrustProp(sEntrustProp);
		pIntf->SetEntrustType(sEntrustType);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetEntrustNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}

				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::RZRQZJHK(const CString sMoneyType, const double dEntrustBalance, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqStockZJHK* pIntf = dynamic_cast<IReqStockZJHK*>(pTradeCoreObj->NewTradingByAction(TRADE_RZRQ_ZJHK));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetMoneyType(sMoneyType);
		pIntf->SetOccurBalance(dEntrustBalance);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("还款成功，流水号：%s", pItem->GetSerialNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}

				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::RZRQZJHQ(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, const int iEntrustAmount, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqStockZJHQ* pIntf = dynamic_cast<IReqStockZJHQ*>(pTradeCoreObj->NewTradingByAction(TRADE_RZRQ_ZJHQ));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetExchangeType(sExchangeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetStockCode(sStockCode);
		pIntf->SetEntrustAmount(iEntrustAmount);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetEntrustNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}

				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::ETF9835(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, const int iEntrustAmount,const CString sEntrustProp, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqStockEntrust* pIntf = dynamic_cast<IReqStockEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_STOCK_9835));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetExchangeType(sExchangeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetStockCode(sStockCode);
		pIntf->SetEntrustAmount(iEntrustAmount);
		pIntf->SetEntrustPrice(1);
		pIntf->SetEntrustBs(bsBuy);
		pIntf->SetEntrustProp(sEntrustProp);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetEntrustNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}

				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}

BOOL CBusinessLogic::ETF9837(const CString sExchangeType, const CString sStockAccount, const CString sComponentCode, const int iEntrustAmount, const CString sETFCode, const CString sEntrustProp, CString& sMsg)
{
	BOOL bReturn = FALSE;
	try
	{
		IReqStockEntrust* pIntf = dynamic_cast<IReqStockEntrust*>(pTradeCoreObj->NewTradingByAction(TRADE_STOCK_9837));
		pIntf->Clear();
		pIntf->SetAccount(m_Account);
		pIntf->SetExchangeType(sExchangeType);
		pIntf->SetStockAccount(sStockAccount);
		pIntf->SetComponentCode(sComponentCode);
		pIntf->SetStockCode(sETFCode);
		pIntf->SetEntrustAmount(iEntrustAmount);
		pIntf->SetEntrustPrice(1);
		pIntf->SetEntrustBs(bsBuy);
		pIntf->SetEntrustProp(sEntrustProp);
		if (pIntf->TradingSync())
		{
			IDataList* pList = pIntf->GetDataList();
			if (pList)
			{
				IDataItem* pItem = pList->GetItems(0);
				if (pItem)
				{
					int iErrorNo = pItem->GetErrorNo();
					if ( 0 == iErrorNo)
					{	
						sMsg.Format("委托成功，委托编号：%s", pItem->GetEntrustNo());
						bReturn = TRUE;
					}
					else
					{
						sMsg = pItem->GetErrorInfo();
					}

				}
				pIntf->FreeDataList();
			}		
		}
		else
		{
			sMsg = pIntf->GetLastError();
		}
		pIntf->Release();
	}
	catch (...)
	{
		sMsg = "获取接口错误！";
	}	
	return bReturn;
}