/*******************************************************
  Դ��������:TradeStock9835Dlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ETF�����ֽ��Ϲ�
  ��    ��:  shenglq
  ��������:  20110421
*********************************************************/
#include "stdafx.h"
#include "TradeStock9835Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStock9835Dlg, CTradeStockSubcribeDlg)

CTradeStock9835Dlg::CTradeStock9835Dlg(CWnd* pParent /* = NULL */)
	: CTradeStockSubcribeDlg(pParent)
{

}

CTradeStock9835Dlg::~CTradeStock9835Dlg()
{

}

void CTradeStock9835Dlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeStock9835Dlg, CTradeStockSubcribeDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStock9835Dlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "M";
		//m_stEnableAmount.SetWindowText("�Ϲ�����");
		//m_stAmount.SetWindowText("�Ϲ����");
		return TRUE;
	}
	else
		return FALSE;	
}
