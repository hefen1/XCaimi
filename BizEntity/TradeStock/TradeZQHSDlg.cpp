/*******************************************************
  Դ��������:TradeZQHSDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ծȯ����
  ��    ��:  shenglq
  ��������:  20100317
*********************************************************/
#include "stdafx.h"
#include "TradeZQHSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeZQHSDlg, CTradeZQToStockDlg)

CTradeZQHSDlg::CTradeZQHSDlg(CWnd* pParent /* = NULL */)
	: CTradeZQToStockDlg(pParent)
{

}

CTradeZQHSDlg::~CTradeZQHSDlg()
{

}


BEGIN_MESSAGE_MAP(CTradeZQHSDlg, CTradeZQToStockDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeZQHSDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "8";
		return TRUE;
	}
	else
		return FALSE;
}

