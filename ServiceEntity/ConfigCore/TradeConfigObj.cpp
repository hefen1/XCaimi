/*******************************************************
  源程序名称:TradeConfigObj.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易交易配置
  功能说明:  
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "TradeConfigObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CTradeConfigObj::CTradeConfigObj()
{
	m_pTradeTables = NULL;
	m_pConfigDef = NULL;
	m_pConfigCell = NULL;
	m_pConfigAccount = NULL;
}

CTradeConfigObj::~CTradeConfigObj()
{
	if (m_pTradeTables)
	{
		m_pTradeTables->Release();
	}	
	if (m_pConfigDef)
	{
		m_pConfigDef->Release();
	}
	if (m_pConfigCell)
	{
		m_pConfigCell->Release();
	}
	if (m_pConfigAccount)
	{
		m_pConfigAccount->Release();
	}
}

void CTradeConfigObj::Release()
{
	delete this;
}

BOOL CTradeConfigObj::LoadTradeTables()
{
	if (NULL == m_pTradeTables)
	{
		m_pTradeTables = new CTradeTables();
		if (m_pTradeTables->InitData())
		{
			return TRUE;
		}
		else
		{
			m_pTradeTables->Release();
			m_pTradeTables = NULL;
			return FALSE;
		}
	}
	else
		return TRUE;
}

BOOL CTradeConfigObj::LoadTradeSysConfigs()
{
	BOOL bReturn = FALSE;
	if (NULL == m_pConfigDef)
	{
		m_pConfigDef = new CTradeConfigDef();
		if (m_pConfigDef->InitData())
		{
			bReturn = TRUE;
		}
		else
		{
			m_pConfigDef->Release();
			m_pConfigDef = NULL;
			bReturn = FALSE;
		}
	}
	else
		bReturn = TRUE;

	if (bReturn)
	{
		if (NULL == m_pConfigCell)
		{
			m_pConfigCell = new CTradeConfigCell();
			if (m_pConfigCell->InitData())
			{
				bReturn = TRUE;
			}
			else
			{
				m_pConfigCell->Release();
				m_pConfigCell = NULL;
				bReturn = FALSE;
			}
		}
		else
			bReturn = TRUE;
	}
	if (bReturn)
	{
		if (NULL == m_pConfigAccount)
		{
			m_pConfigAccount = new CTradeConfigAccount();
			if (m_pConfigAccount->InitData())
			{
				bReturn = TRUE;
			}
			else
			{
				m_pConfigAccount->Release();
				m_pConfigAccount = NULL;
				bReturn = FALSE;
			}
		}
		else
			bReturn = TRUE;
	}
	return bReturn;
}

ICellList* CTradeConfigObj::GetCellList()
{
	if (LoadTradeSysConfigs())
	{
		ICellList* pCellList = m_pConfigCell->GetCellList();
		ICCommParamList* pConfigDefList = m_pConfigDef->GetDefaultConfigList();
		pCellList->SetNextParamList(pConfigDefList);
		return pCellList;
	}
	else
		return NULL;	
}

ITradeTable* CTradeConfigObj::GetTradeTableData(const int iAction)
{
	if (LoadTradeTables())
	{
		CString sName = "";
		sName.Format("%d", iAction);
		return m_pTradeTables->GetItemsByName(sName);
	}
	else
		return NULL;	
}

ITradeDict* CTradeConfigObj::GetTradeDict(CString sName)
{
	if (LoadTradeTables())
		return m_pTradeTables->GetTradeDictByName(sName);
	else
		return NULL;
}


INodeTreeData* CTradeConfigObj::GetReqParamList(const int iAction)
{
	if (LoadTradeTables())
	{
		return m_pTradeTables->GetReqParamByAction(iAction);
	}
	else
		return NULL;	
}

TiXmlElement* CTradeConfigObj::GetFunctionTree(CString sCellID, CString sAccount /* = */ )
{
	if (LoadTradeSysConfigs())
	{
		return	m_pConfigCell->GetCellList()->GetItemsByName(sCellID)->GetFunctionTree();
	}
	else
		return NULL;
}

ICCommParamList* CTradeConfigObj::GetAccountParamList(CString sCellID, CString sAccount)
{
	if (LoadTradeSysConfigs())
	{
		IAccountConfigList* pAccountParamList = m_pConfigAccount->GetAccountConfigList();
		pAccountParamList->SetCellAndAccount(sCellID, sAccount);
		ICellList* pCellList = m_pConfigCell->GetCellList();
		pCellList->SetCellAndAccount(sCellID, "");
		ICCommParamList* pConfigDefList = m_pConfigDef->GetDefaultConfigList();
		pAccountParamList->SetNextParamList(pCellList);
		pCellList->SetNextParamList(pConfigDefList);
		return pAccountParamList;
	}
	else
		return NULL;
}