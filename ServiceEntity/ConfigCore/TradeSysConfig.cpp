/*******************************************************
  源程序名称:TradeSysConfig.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  配置中心
  功能说明:  交易系统配置
  作    者:  shenglq
  开发日期:  20101012
*********************************************************/
#include "stdafx.h"
#include "TradeSysConfig.h"
#include "TradeLangDefined.h"
#include "TradeConfigObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CTradeConfigObj gTradeConfigObj;

CTradeMarketItem::CTradeMarketItem()
{

}

CTradeMarketItem::~CTradeMarketItem()
{

}

CString CTradeMarketItem::GetMarketName()
{
	return CNodeData::GetValue("Name");
}

CString CTradeMarketItem::GetDispName()
{
	return CNodeData::GetValue("DisplayName");
}


EMoneyType CTradeMarketItem::GetMoneyType()
{
	CString sValue = CNodeData::GetValue("MoneyType");
	if (sValue.IsEmpty())
	{
		sValue = "0";
	}
	int iValue = atoi(sValue);
	return EMoneyType(iValue);
}

CString CTradeMarketItem::GetAccountType()
{
	return CNodeData::GetValue("AccountType");
}


//////////////////////////////////////////////////////////////////////////
CTradeMarketList::CTradeMarketList()
{

}

CTradeMarketList::~CTradeMarketList()
{

}

ITradeMarketItem* CTradeMarketList::GetItems(const int iIndex)
{
	return dynamic_cast<ITradeMarketItem*>(CNodeTreeData::GetItems(iIndex));
}

ITradeMarketItem* CTradeMarketList::GetItemsByName(const CString sName)
{
	return dynamic_cast<ITradeMarketItem*>(CNodeTreeData::GetItemsByName(sName));
}

ITradeMarketItem* CTradeMarketList::GetItemsFull(CXML xPath, int iIndex /* = -1 */)
{

	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(xPath, m_pRoot, iIndex);
	if (pNode)
	{
		CTradeMarketItem* pNodeData = new CTradeMarketItem();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	else
		return NULL;
	
}
//////////////////////////////////////////////////////////////////////////
//账号类型相关类
CTradeAccountTypeItem::CTradeAccountTypeItem()
{

}

CTradeAccountTypeItem::~CTradeAccountTypeItem()
{

}

CString CTradeAccountTypeItem::GetAccountTypeName()
{
	return CNodeData::GetValue("Name");
}

CString CTradeAccountTypeItem::GetAccountType()
{
	return CNodeData::GetValue("Type");
}

CString CTradeAccountTypeItem::GetBackType()
{
	return CNodeData::GetValue("BackType");
}

CString CTradeAccountTypeItem::GetShortName()
{
	return CNodeData::GetValue("ShortName");
}

CString CTradeAccountTypeItem::GetMarketType()
{
	return CNodeData::GetValue("MarketType");
}

CString CTradeAccountTypeItem::GetVisibleName()
{
	CString sValue = CNodeData::GetValue("VisibleName");
	if (sValue.IsEmpty())
	{
		sValue = CNodeData::GetValue("Name");
	}
	return sValue;
}


//////////////////////////////////////////////////////////////////////////
CTradeAccountTypeList::CTradeAccountTypeList()
{

}

CTradeAccountTypeList::~CTradeAccountTypeList()
{

}

ITradeAccountTypeItem* CTradeAccountTypeList::GetItems(const int iIndex)
{
	return dynamic_cast<ITradeAccountTypeItem*>(CNodeTreeData::GetItems(iIndex));
}

ITradeAccountTypeItem* CTradeAccountTypeList::GetItemsByName(const CString sName)
{
	return dynamic_cast<ITradeAccountTypeItem*>(CNodeTreeData::GetItemsByName(sName));
}

CString CTradeAccountTypeList::GetBackTypeByAccountType(const CString sName)
{
	for(int i = 0; i < CNodeTreeData::GetCount(); i++)
	{
		ITradeAccountTypeItem *pItem = GetItems(i);
		if(pItem && pItem->GetAccountTypeName().Compare(sName) == 0)
		{
			CString sBackType = pItem->GetBackType();
			pItem->Release();
			return sBackType;
		}
		pItem->Release();
	}
	return "1"; //如果没有找到，给一个默认股票类型的
}

CString CTradeAccountTypeList::GetMarketTypeByAccountType(const CString sName)
{

	CString sAccountType = "";
	for (int i = 0; i < CNodeTreeData::GetCount(); i++)
	{//通过账户类型名称 查询市场类型
		ITradeAccountTypeItem *pAccountItem = GetItems(i);
		if(pAccountItem && pAccountItem->GetAccountTypeName().Compare(sName) == 0)
		{
			sAccountType = pAccountItem->GetAccountType();
			pAccountItem->Release();
			break;
		}
		pAccountItem->Release();
	}

	ITradeAccountTypeItem *pAccountItem = GetItemsByName(sName);
	if (pAccountItem)
	{
		return pAccountItem->GetMarketType();
	}
	else
		return "0";
}

ITradeAccountTypeItem* CTradeAccountTypeList::GetItemsFull(CXML xPath, int iIndex )
{

	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(xPath, m_pRoot, iIndex);
	if (pNode)
	{
		CTradeAccountTypeItem* pNodeData = new CTradeAccountTypeItem();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}

	TRACE("账号类型:GetItemsFull函数出错,请检查配置文件中是否正确\n");
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
CModifyPassowrdTypeItem::CModifyPassowrdTypeItem()
{

}

CModifyPassowrdTypeItem::~CModifyPassowrdTypeItem()
{

}

CString CModifyPassowrdTypeItem::GetOldCharType()
{
	return CNodeData::GetValue("OldCharType");
}

int CModifyPassowrdTypeItem::GetOldMaxLen()
{
	CString sValue = CNodeData::GetValue("OldMaxLen");
	if (sValue.IsEmpty())
	{
		sValue = "0";
	}
	int iValue = atoi(sValue);
	return iValue;
}

int CModifyPassowrdTypeItem::GetOldMinLen()
{
	CString sValue = CNodeData::GetValue("OldMinLen");
	if (sValue.IsEmpty())
	{
		sValue = "0";
	}
	int iValue = atoi(sValue);
	return iValue;
}

CString CModifyPassowrdTypeItem::GetNewCharType()
{
	return CNodeData::GetValue("NewCharType");
}

int CModifyPassowrdTypeItem::GetNewMaxLen()
{
	CString sValue = CNodeData::GetValue("NewMaxLen");
	if (sValue.IsEmpty())
	{
		sValue = "0";
	}
	int iValue = atoi(sValue);
	return iValue;
}

int CModifyPassowrdTypeItem::GetNewMinLen()
{
	CString sValue = CNodeData::GetValue("NewMinLen");
	if (sValue.IsEmpty())
	{
		sValue = "0";
	}
	int iValue = atoi(sValue);
	return iValue;
}



//////////////////////////////////////////////////////////////////////////
CModifyPassowrdTypeList::CModifyPassowrdTypeList()
{

}

CModifyPassowrdTypeList::~CModifyPassowrdTypeList()
{

}

IModifyPassowrdTypeItem* CModifyPassowrdTypeList::GetItems(const int iIndex)
{
	return dynamic_cast<IModifyPassowrdTypeItem*>(CNodeTreeData::GetItems(iIndex));
}

IModifyPassowrdTypeItem* CModifyPassowrdTypeList::GetItemsByName(const CString sName)
{
	return dynamic_cast<IModifyPassowrdTypeItem*>(CNodeTreeData::GetItemsByName(sName));
}

IModifyPassowrdTypeItem* CModifyPassowrdTypeList::GetItemsFull(CXML xPath, int iIndex /* = -1 */)
{

	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(xPath, m_pRoot, iIndex);
	if (pNode)
	{
		CModifyPassowrdTypeItem* pNodeData = new CModifyPassowrdTypeItem();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	else
		return NULL;
}

//////////////////////////////////////////////////////////////////////////
CFundRiskQuestionItem::CFundRiskQuestionItem()
{

}

CFundRiskQuestionItem::~CFundRiskQuestionItem()
{

}

CString CFundRiskQuestionItem::GetIDPropName()
{
	return "code";
}

CString CFundRiskQuestionItem::GetCode()
{
	return CNodeData::GetValue("code");
}

CString CFundRiskQuestionItem::GetTitle()
{
	return CNodeData::GetValue("title");
}

CString CFundRiskQuestionItem::GetChoice()
{
	return CNodeData::GetValue("choice");
}

CString CFundRiskQuestionItem::GetChoiceText()
{
	return CNodeData::GetValue("choicetext");
}

int CFundRiskQuestionItem::GetChoiceScore()
{
	CString sValue = CNodeData::GetValue("choicescore");
	if (sValue.IsEmpty())
	{
		sValue = "0";
	}
	int iValue = atoi(sValue);
	return iValue;
}

CString CFundRiskQuestionItem::GetRemark()
{
	return CNodeData::GetValue("remark");
}
//////////////////////////////////////////////////////////////////////////
CFundRiskQuestionList::CFundRiskQuestionList()
{

}

CFundRiskQuestionList::~CFundRiskQuestionList()
{

}

IFundRiskQuestionItem* CFundRiskQuestionList::GetItems(const int iIndex)
{
	return dynamic_cast<IFundRiskQuestionItem*>(CNodeTreeData::GetItems(iIndex));
}

IFundRiskQuestionItem* CFundRiskQuestionList::GetItemsByName(const CString sName)
{
	return dynamic_cast<IFundRiskQuestionItem*>(CNodeTreeData::GetItemsByName(sName));
}

IFundRiskQuestionItem* CFundRiskQuestionList::GetItemsFull(CXML xPath, int iIndex /* = -1 */)
{

	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(xPath, m_pRoot, iIndex);
	if (pNode)
	{
		CFundRiskQuestionItem* pNodeData = new CFundRiskQuestionItem();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	else
		return NULL;
}


//////////////////////////////////////////////////////////////////////////
CCommParamList::CCommParamList()
{
	m_pNextParamList = NULL;
	m_sCellID = "";
	m_sAccount = "";
	m_sID = "";
}
CCommParamList::~CCommParamList()
{
	if (m_pNextParamList)
	{
		m_pNextParamList->Release();
	}
}

CXML CCommParamList::GetListPath()
{
	return "";
}

void CCommParamList::SetNextParamList(ICCommParamList* pParamlist)
{
	m_pNextParamList = pParamlist;
}

void CCommParamList::SetCellAndAccount(const CString sCellID, const CString sAccount)
{
	m_sCellID = m_sCellID;
	m_sAccount = sAccount;
	m_sID = sCellID;
	if (!sAccount.IsEmpty())
	{
		m_sID = sCellID + "-" + m_sAccount;
	}
}

TiXmlNode* CCommParamList::GetChildNodeByIDName(const CString sID, const CString sName)
{
	TiXmlNode* pChildNode = NULL;
	TiXmlNode* pNode = m_pRoot;
	CString sListPath = GetListPath();
	if (!sListPath.IsEmpty())
	{
		CString sNodeID = sID;
		if (sNodeID.IsEmpty())
			sNodeID = m_sID;
		if (!sNodeID.IsEmpty())
		{
			CXML xPath = "";
			xPath.Format(GetListPath(), sNodeID);
			pNode = m_pXMLParser->SelectSingleNode(xPath, m_pRoot);
		}	
	}
	if (pNode)
	{
		pChildNode = pNode->FirstChildElement(sName);
		if(!pChildNode)
		{
			//是否配置有模板
			CString sConfig = m_pXMLParser->GetNodeValue("@config", pNode);
			if (!sConfig.IsEmpty())
			{
				sConfig = "//" + sConfig;
				pChildNode = m_pXMLParser->SelectSingleNode(sConfig, sName);
			}
		}			
	}
	return pChildNode;	
}

INodeTreeData* CCommParamList::GetNVListByName(const CString sNodeName, const CString sID /* = */ )
{
	TiXmlNode* pChildNode = GetChildNodeByIDName(sID, sNodeName);
	if (pChildNode)
	{
		CNodeTreeData* pNodeData = new CNodeTreeData();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pChildNode);
		return pNodeData;
	}
	else
	{
		if (m_pNextParamList)
		{
			return	m_pNextParamList->GetNVListByName(sNodeName, sID);
		}
	}
	//TRACE("%s没有找到,请检查配置文件中是否正确\n",xPath);
	return NULL;
}

IModifyPassowrdTypeList* CCommParamList::GetModifyPassowrdTypeList()
{
	TiXmlNode* pChildNode = GetChildNodeByIDName("", "ModifyPassWordType");
	if (pChildNode)
	{
		CModifyPassowrdTypeList* pNodeData = new CModifyPassowrdTypeList();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pChildNode);
		return pNodeData;
	}	
	else
	{
		if (m_pNextParamList)
		{
			return	m_pNextParamList->GetModifyPassowrdTypeList();
		}
	}
	//TRACE("密码类型:%s没有找到,请检查配置文件中是否正确\n",xPath);
	return NULL;
}

ITradeAccountTypeList* CCommParamList::GetAccountTypeList(const CString sID /* = */ )
{
	TiXmlNode* pChildNode = GetChildNodeByIDName(sID, "AccountType");
	if (pChildNode)
	{
		CTradeAccountTypeList* pNodeData = new CTradeAccountTypeList();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pChildNode);
		return pNodeData;
	}
	else
	{
		if (m_pNextParamList)
		{
			return	m_pNextParamList->GetAccountTypeList(sID);
		}
	}
	//TRACE("帐号类型:%s没有找到,请检查配置文件中是否正确\n",xPath);
	return NULL;
}

ITradeMarketList* CCommParamList::GetMarketList()
{
	TiXmlNode* pChildNode = GetChildNodeByIDName("", "MarketType");
	if (pChildNode)
	{
		CTradeMarketList* pNodeData = new CTradeMarketList();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pChildNode);
		return pNodeData;
	}
	else
	{
		if (m_pNextParamList)
		{
		    return	m_pNextParamList->GetMarketList();
		}
	}
	//TRACE("市场类型:%s没有找到,请检查配置文件中是否正确\n",xPath);
	return NULL;
}

INodeTreeData* CCommParamList::GetDefaultReqDataList(CString sID /* = */ )
{
	return GetNVListByName("DefaultReqData", sID);
}

INodeTreeData* CCommParamList::GetSHSJEntrustTypeList()
{
	return GetNVListByName("SHSJWTType");
}

INodeTreeData* CCommParamList::GetSZSJEntrustTypeList()
{
	return GetNVListByName("SZSJWTType");
}

INodeTreeData* CCommParamList::GetExceedFlagList()
{
	return GetNVListByName("ExceedFlag");
}

INodeTreeData* CCommParamList::GetChargeTypeList()
{
	return GetNVListByName("ChargeType");
}

INodeTreeData* CCommParamList::GetDividendMethodList()
{
	return GetNVListByName("DividendMethod");
}

INodeTreeData* CCommParamList::GetMoneyTypeList()
{
	return GetNVListByName("MoneyType");
}

INodeTreeData* CCommParamList::GetStockEntrustType()
{
	return GetNVListByName("StockEntrustType");
}

INodeTreeData* CCommParamList::GetIDKindList()
{
	return GetNVListByName("IDKind");
}

INodeTreeData* CCommParamList::GetEntrustStatusList()
{
	return GetNVListByName("EntrustStatus");
}

IFundRiskQuestionList* CCommParamList::GetFundRiskQuestionList()
{
	TiXmlNode* pChildNode = GetChildNodeByIDName("", "QuestionParam");
	if (pChildNode)
	{
		CFundRiskQuestionList* pNodeData = new CFundRiskQuestionList();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pChildNode);
		return pNodeData;
	}
	else
	{
		if (m_pNextParamList)
		{
			return	m_pNextParamList->GetFundRiskQuestionList();
		}
	}
	//TRACE("市场类型:%s没有找到,请检查配置文件中是否正确\n",xPath);
	return NULL;
}

INodeTreeData* CCommParamList::GetFundAccRiskTypes()
{
	return GetNVListByName("FundAccRiskTypes");
}
//////////////////////////////////////////////////////////////////////////
CCellItem::CCellItem()
{

}

CCellItem::~CCellItem()
{

};

CString CCellItem::GetCellID()
{
	return CNodeData::GetValue("id");
}

CString CCellItem::GetCaption()
{
	return CNodeData::GetValue("caption");
}

CString CCellItem::GetType()
{
	return CNodeData::GetValue("type");
}

CString CCellItem::GetTypeName()
{
	return CNodeData::GetValue("typename");
}

int CCellItem::GetServerID()
{
	CString sValue = CNodeData::GetValue("serverid");
	if (sValue.IsEmpty())
	{
		sValue = GetCellID();
	}
	int iValue = atoi(sValue);
	return iValue;
}

CString CCellItem::GetBranchNo()
{
	return CNodeData::GetValue("branchno");
}

CString CCellItem::GetIDPropName()
{
	return "id";
}

TiXmlElement* CCellItem::GetFunctionTree()
{
	CString sConfig = CNodeData::GetValue("config");
	//sConfig.Format("//%s/FunctionTree", CNodeData::GetValue("config"));
	return m_pParent->Parent()->FirstChildElement(sConfig)->FirstChildElement("FunctionTree");
}
//////////////////////////////////////////////////////////////////////////
CCellList::CCellList()
{

}

CCellList::~CCellList()
{

}

CXML CCellList::GetListPath()
{
	return "Cell[@id=%s]";
}

ICellItem* CCellList::GetItems(const int iIndex)
{
	return dynamic_cast<ICellItem*>(CNodeTreeData::GetItems(iIndex));
}

ICellItem* CCellList::GetItemsByName(const CString sName)
{
	return dynamic_cast<ICellItem*>(CNodeTreeData::GetItemsByName(sName));
}

ICellItem* CCellList::GetItemsFull(CXML xPath, int iIndex /* = -1 */)
{

	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(xPath, m_pRoot, iIndex);
	if (pNode)
	{
		CCellItem* pNodeData = new CCellItem();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	else
		return NULL;
}

CXML CCellList::GetChildNodeName()
{
	return "Cell";
}

CString CCellList::GetIDPropName()
{
	return "id";
}



//////////////////////////////////////////////////////////////////////////
CAccountConfigItem::CAccountConfigItem()
{

}

CAccountConfigItem::~CAccountConfigItem()
{

}

CString CAccountConfigItem::GetID()
{
	return CNodeData::GetValue(GetIDPropName());
}

CString CAccountConfigItem::GetIDPropName()
{
	return "id";
}

//////////////////////////////////////////////////////////////////////////
CAccountConfigList::CAccountConfigList()
{

}

CAccountConfigList::~CAccountConfigList()
{

}

CXML CAccountConfigList::GetListPath()
{
	return "Account[@id=%s]";
}

IAccountConfigItem* CAccountConfigList::GetItems(const int iIndex)
{
	return dynamic_cast<IAccountConfigItem*>(CNodeTreeData::GetItems(iIndex));
}

IAccountConfigItem* CAccountConfigList::GetItemsByName(const CString sName)
{
	return dynamic_cast<IAccountConfigItem*>(CNodeTreeData::GetItemsByName(sName));
}

IAccountConfigItem* CAccountConfigList::GetItemsFull(CXML xPath, int iIndex /* = -1 */)
{

	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(xPath, m_pRoot, iIndex);
	if (pNode)
	{
		CAccountConfigItem* pNodeData = new CAccountConfigItem();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	else
		return NULL;
}

CXML CAccountConfigList::GetChildNodeName()
{
	return "Account";
}

CString CAccountConfigList::GetIDPropName()
{
	return "id";
}

//////////////////////////////////////////////////////////////////////////

CTradeConfigDef::CTradeConfigDef()
{

}

CTradeConfigDef::~CTradeConfigDef()
{

}
ICCommParamList* CTradeConfigDef::GetDefaultConfigList()
{
	CXML xPath = "DefaultConfig";
	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(GetRootPath(), xPath);
	if (pNode)
	{
		CCommParamList* pNodeData = new CCommParamList();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	return NULL;
}

UINT CTradeConfigDef::GetIDResource()
{
	return GNV_TRADE_XML_SYSCONFIGS;
}
//////////////////////////////////////////////////////////////////////////
CTradeConfigCell::CTradeConfigCell()
{

}

CTradeConfigCell::~CTradeConfigCell()
{

}

ICellList* CTradeConfigCell::GetCellList()
{
	CXML xPath = "CellConfig";
	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(GetRootPath(), xPath);
	if (pNode)
	{
		CCellList* pNodeData = new CCellList();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		//IModifyPassowrdTypeList* pList = pNodeData->GetModifyPassowrdTypeListByID("1");
		//int nCount = pList->GetCount();
		//nCount++;
		//pList = pNodeData->GetModifyPassowrdTypeListByID("2");
		//nCount = pList->GetCount();
		//nCount++;
		return pNodeData;
	}

	TRACE("营业部列表:%s没有找到,请检查配置文件中是否正确\n",xPath);
	return NULL;
}

CString CTradeConfigCell::GetFileName()
{
	return CGeneralHelper::GetAppPath() + _T("config\\TradeConfig.xml");
}
//////////////////////////////////////////////////////////////////////////
CTradeConfigAccount::CTradeConfigAccount()
{

}

CTradeConfigAccount::~CTradeConfigAccount()
{

}
IAccountConfigList* CTradeConfigAccount::GetAccountConfigList()
{
	CXML xPath = "AccountConfig";
	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(GetRootPath(), xPath);
	if (!pNode)
	{
		pNode = new TiXmlElement(xPath);
		m_pRoot->LinkEndChild(pNode);
		m_pXMLParser->SaveFile(GetFileName());
	}	
	CAccountConfigList* pNodeData = new CAccountConfigList();
	pNodeData->SetXMLParser(m_pXMLParser);
	pNodeData->SetRoot(pNode);
	return pNodeData;
}

CString CTradeConfigAccount::GetFileName()
{
	return CGeneralHelper::GetAppPath() + _T("config\\TradeConfigUser.xml");
}
//////////////////////////////////////////////////////////////////////////
