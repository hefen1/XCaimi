/*******************************************************
  源程序名称:QueryFundInfoDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeFund
  功能说明:  查询基金信息
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#include "stdafx.h"
#include "QueryFundInfoDlg.h"
#include "TradeLangDefined.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CQueryFundInfoDlg, CTradeQueryBaseDlg)

CQueryFundInfoDlg::CQueryFundInfoDlg(CWnd* pParent /* = NULL */)
: CTradeQueryBaseDlg(pParent)
{
	m_pFundInfoList = NULL;
}

CQueryFundInfoDlg::~CQueryFundInfoDlg()
{
	if (m_pFundInfoList)
	{
		m_pFundInfoList->Release();
	}
}

BOOL CQueryFundInfoDlg::OnInit()
{
	if (__super::OnInit())
	{
		CString sMsg;
		CString sFilterStatus = m_pParam->GetStringParam("filterstatus");
		IFundInfoList* pList = m_pAccount->GetBusinessLogic()->GetFundInfo(sMsg);
		if (pList)
		{
			if (sFilterStatus.IsEmpty())
			{
				m_pGrid->FillGrid(pList);
			}
			else
			{
				sFilterStatus = "," + sFilterStatus + ",";
				m_pFundInfoList = dynamic_cast<IFundInfoList*>(pList->Clone());
				for (int i = (m_pFundInfoList->GetCount() - 1); i >= 0 ; i--)
				{
					CString sStatus = m_pFundInfoList->GetItems(i)->GetStatus();
					sStatus = "," + sStatus + ",";
					int nPos = sFilterStatus.Find(sStatus);
					if (nPos >= 0)
					   m_pFundInfoList->DeleteItem(i);
				}
				m_pGrid->FillGrid(m_pFundInfoList);
			}
			
		}		
		return TRUE;
	}
	else
		return FALSE;
}

void CQueryFundInfoDlg::OnShow(BOOL bShow)
{

}

void CQueryFundInfoDlg::OnLinkageMsg(CLinkageParam* pParam)
{

}