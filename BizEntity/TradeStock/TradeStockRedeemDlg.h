/*******************************************************
  Դ��������:TradeStockRedeemDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ���ڻ������
  ��    ��:  shenglq
  ��������:  20110316
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeStockRedeemDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeStockRedeemDlg)
public:
	CTradeStockRedeemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockRedeemDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void LoadWnd();
	virtual BOOL Validate();
private:
	DECLARE_MESSAGE_MAP()

};