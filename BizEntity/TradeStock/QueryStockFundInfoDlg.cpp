/*******************************************************
  源程序名称:QueryStockFundInfoDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeFund
  功能说明:  查询基金盘后业务基金信息
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#include "stdafx.h"
#include "QueryStockFundInfoDlg.h"
#include "TradeLangDefined.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CQueryStockFundInfoDlg, CTradeQueryBaseDlg)

CQueryStockFundInfoDlg::CQueryStockFundInfoDlg(CWnd* pParent /* = NULL */)
: CTradeQueryBaseDlg(pParent)
{

}

CQueryStockFundInfoDlg::~CQueryStockFundInfoDlg()
{

}

BOOL CQueryStockFundInfoDlg::OnInit()
{
	if (__super::OnInit())
	{
		CString sMsg;
		IStockFundInfoList* pList = m_pAccount->GetBusinessLogic()->GetStockFundInfo(sMsg);
		if (pList)
		{
			m_pGrid->FillGrid(pList);
		}		
		return TRUE;
	}
	else
		return FALSE;
}

void CQueryStockFundInfoDlg::OnShow(BOOL bShow)
{

}

void CQueryStockFundInfoDlg::OnLinkageMsg(CLinkageParam* pParam)
{

}

void CQueryStockFundInfoDlg::DoGridDBClick()
{
	
}
