/*******************************************************
  Դ��������:TradeStockSJBuyDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����֤ȯ�м�����
  ��    ��:  shenglq
  ��������:  20110121
*********************************************************/
#pragma once
#include "TradeStockBuyDlg.h"

class CTradeStockSJBuyDlg : public CTradeStockBuyDlg
{
	DECLARE_DYNAMIC(CTradeStockSJBuyDlg)
public:
	CTradeStockSJBuyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockSJBuyDlg();
	//virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//virtual BOOL OnInit();
	//virtual void OnStockCodeExit();
	//virtual void OnEntrustPriceExit();
	//virtual void DoEntrust();
	virtual void LoadWnd();
private:
	//void QueryEnableBalance();
	//void QueryEnableAmount();

	DECLARE_MESSAGE_MAP()

};