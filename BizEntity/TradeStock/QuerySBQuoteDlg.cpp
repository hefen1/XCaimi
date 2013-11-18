/*******************************************************
  源程序名称:QuerySBQuoteDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeStock
  功能说明:  查询三板协议行情
  作    者:  shenglq
  开发日期:  20110323
*********************************************************/
#include "stdafx.h"
#include "QuerySBQuoteDlg.h"
#include "TradeLangDefined.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CQuerySBQuoteDlg, CTradeQueryBaseDlg)

CQuerySBQuoteDlg::CQuerySBQuoteDlg(CWnd* pParent /* = NULL */)
: CTradeQueryBaseDlg(pParent)
{

}

CQuerySBQuoteDlg::~CQuerySBQuoteDlg()
{
	if(m_pDataList)
	{
		m_pDataList->Release();
		m_pDataList = NULL;
	}

}

void CQuerySBQuoteDlg::DoDataCore(IDataList * pDataList)
{
	if (m_pDataList)
	{
		m_pDataList->Release();
		m_pDataList = NULL;
	}
	m_pDataList = pDataList;
	FillGrid();
}