/*******************************************************
  Դ��������:TradeStock9837Dlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ETF���¹ɷݻ���
  ��    ��:  shenglq
  ��������:  20100421
*********************************************************/
#include "stdafx.h"
#include "TradeStock9837Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStock9837Dlg, CTradeStockFundTransDlg)

CTradeStock9837Dlg::CTradeStock9837Dlg(CWnd* pParent /* = NULL */)
	: CTradeStockFundTransDlg(pParent)
{

}

CTradeStock9837Dlg::~CTradeStock9837Dlg()
{

}


BEGIN_MESSAGE_MAP(CTradeStock9837Dlg, CTradeStockFundTransDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStock9837Dlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "M";
		m_stEnableAmount.SetWindowText("�ɻ�����");
		m_stTransCode.SetWindowText("ETF ����");
		return TRUE;
	}
	else
		return FALSE;
}

