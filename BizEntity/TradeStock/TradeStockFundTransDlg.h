/*******************************************************
  Դ��������:TradeStockFundTransDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  �����̺�ҵ��ת��
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeStockFundTransDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeStockFundTransDlg)
public:
	CTradeStockFundTransDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockFundTransDlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};