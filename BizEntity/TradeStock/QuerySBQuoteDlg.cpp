/*******************************************************
  Դ��������:QuerySBQuoteDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeStock
  ����˵��:  ��ѯ����Э������
  ��    ��:  shenglq
  ��������:  20110323
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