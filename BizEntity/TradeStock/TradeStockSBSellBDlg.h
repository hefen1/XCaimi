/*******************************************************
  Դ��������:TradeStockSBSellBDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ���嶨������
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeStockSBSellBDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeStockSBSellBDlg)
public:
	CTradeStockSBSellBDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockSBSellBDlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};