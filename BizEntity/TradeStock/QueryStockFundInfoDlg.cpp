/*******************************************************
  Դ��������:QueryStockFundInfoDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeFund
  ����˵��:  ��ѯ�����̺�ҵ�������Ϣ
  ��    ��:  shenglq
  ��������:  20100910
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
