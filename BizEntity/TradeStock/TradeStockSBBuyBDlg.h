/*******************************************************
  Դ��������:TradeStockSBBuyBDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ���嶨������
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeStockBuyDlg.h"

class CTradeStockSBBuyBDlg : public CTradeStockBuyDlg
{
	DECLARE_DYNAMIC(CTradeStockSBBuyBDlg)
public:
	CTradeStockSBBuyBDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockSBBuyBDlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};