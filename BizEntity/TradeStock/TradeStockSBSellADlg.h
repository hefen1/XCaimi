/*******************************************************
  Դ��������:TradeStockSBSellADlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ������������
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeStockSBSellADlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeStockSBSellADlg)
public:
	CTradeStockSBSellADlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockSBSellADlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};