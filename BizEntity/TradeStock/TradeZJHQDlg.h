/*******************************************************
  Դ��������:TradeZJHQDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ֱ�ӻ�ȯ
  ��    ��:  shenglq
  ��������:  20100317
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeZJHQDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeZJHQDlg)
public:
	CTradeZJHQDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeZJHQDlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};