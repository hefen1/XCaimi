/*******************************************************
  Դ��������:TradeStockSBBuyCDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����ȷ������
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeStockBuyDlg.h"

class CTradeStockSBBuyCDlg : public CTradeStockBuyDlg
{
	DECLARE_DYNAMIC(CTradeStockSBBuyCDlg)
public:
	CTradeStockSBBuyCDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockSBBuyCDlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};