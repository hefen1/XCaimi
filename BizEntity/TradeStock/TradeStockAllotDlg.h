/*******************************************************
  Դ��������:TradeStockAllotDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ���ڻ����깺
  ��    ��:  shenglq
  ��������:  20110316
*********************************************************/
#pragma once
#include "TradeStockBuyDlg.h"

class CTradeStockAllotDlg : public CTradeStockBuyDlg
{
	DECLARE_DYNAMIC(CTradeStockAllotDlg)
public:
	CTradeStockAllotDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockAllotDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void LoadWnd();
	virtual void OnAmountEnter();
	virtual BOOL Validate();
private:
	DECLARE_MESSAGE_MAP()

};