/*******************************************************
  Դ��������:TradeStockSellDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����֤ȯ����
  ��    ��:  shenglq
  ��������:  20100903
*********************************************************/
#pragma once
#include "TradeStockBaseDlg.h"

class CTradeStockSellDlg : public CTradeStockBaseDlg
{
	DECLARE_DYNAMIC(CTradeStockSellDlg)
public:
	CTradeStockSellDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockSellDlg();
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);
protected:	
	int m_nQueryEnableAction;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnStockCodeExit();
	virtual void DoEntrust();
	virtual void LoadWnd();
private:
	void QueryEnableAmount();

	DECLARE_MESSAGE_MAP()


};