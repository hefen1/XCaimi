/*******************************************************
  Դ��������:TradeStockSBSellCDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����ȷ������
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeStockSBSellCDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeStockSBSellCDlg)
public:
	CTradeStockSBSellCDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockSBSellCDlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};