/*******************************************************
  Դ��������:TradeStockSJSellDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����֤ȯ�м�����
  ��    ��:  shenglq
  ��������:  20110121
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeStockSJSellDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeStockSJSellDlg)
public:
	CTradeStockSJSellDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockSJSellDlg();
	//virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//virtual BOOL OnInit();
	//virtual void OnStockCodeExit();
	//virtual void DoEntrust();
	virtual void LoadWnd();
private:
	//void QueryEnableAmount();

	DECLARE_MESSAGE_MAP()

};