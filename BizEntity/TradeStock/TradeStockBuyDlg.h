/*******************************************************
  Դ��������:TradeStockBuyDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����֤ȯ����
  ��    ��:  shenglq
  ��������:  20100903
*********************************************************/
#pragma once
#include "TradeStockBaseDlg.h"

class CTradeStockBuyDlg : public CTradeStockBaseDlg
{
	DECLARE_DYNAMIC(CTradeStockBuyDlg)
public:
	CTradeStockBuyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockBuyDlg();
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);
protected:	
	int m_nQueryEnableAction;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnStockCodeExit();
	virtual void OnEntrustPriceExit();
	virtual void DoEntrust();
	virtual void LoadWnd();
	void QueryEnableBalance();
	void QueryEnableAmount();
private:
	DECLARE_MESSAGE_MAP()

};