/*******************************************************
  Դ��������:TradeStock9837Dlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ETF���¹ɷݻ���
  ��    ��:  shenglq
  ��������:  20100421
*********************************************************/
#pragma once
#include "TradeStockFundTransDlg.h"

class CTradeStock9837Dlg : public CTradeStockFundTransDlg
{
	DECLARE_DYNAMIC(CTradeStock9837Dlg)
public:
	CTradeStock9837Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStock9837Dlg();
protected:	
	virtual BOOL OnInit();
private:
	DECLARE_MESSAGE_MAP()
};