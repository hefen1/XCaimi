/*******************************************************
  Դ��������:TradeStockFundMergeSplitDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  �����̺�ҵ��ϲ����
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeStockFundMergeSplitDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeStockFundMergeSplitDlg)
public:
	CTradeStockFundMergeSplitDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockFundMergeSplitDlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};