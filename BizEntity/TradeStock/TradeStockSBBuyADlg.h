/*******************************************************
  Դ��������:TradeStockSBBuyADlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ������������
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeStockBuyDlg.h"

class CTradeStockSBBuyADlg : public CTradeStockBuyDlg
{
	DECLARE_DYNAMIC(CTradeStockSBBuyADlg)
public:
	CTradeStockSBBuyADlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockSBBuyADlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};