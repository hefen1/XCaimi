/*******************************************************
  Դ��������:TradeWarrantEntrustDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  Ȩ֤��Ȩ
  ��    ��:  shenglq
  ��������:  20110317
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeWarrantEntrustDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeWarrantEntrustDlg)
public:
	CTradeWarrantEntrustDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeWarrantEntrustDlg();
protected:	
	virtual BOOL OnInit();
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()
};