/*******************************************************
  Դ��������:TradeZQToStockDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ծȯת��
  ��    ��:  shenglq
  ��������:  20100317
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeZQToStockDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeZQToStockDlg)
public:
	CTradeZQToStockDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeZQToStockDlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};