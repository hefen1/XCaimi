/*******************************************************
  Դ��������:TradeRQSellDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����֤ȯ��ȯ����
  ��    ��:  shenglq
  ��������:  20110121
*********************************************************/
#pragma once
#include "TradeStockBuyDlg.h"

class CTradeRQSellDlg : public CTradeStockBuyDlg
{
	DECLARE_DYNAMIC(CTradeRQSellDlg)
public:
	CTradeRQSellDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeRQSellDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void LoadWnd();
	virtual BOOL OnInit();
private:
	DECLARE_MESSAGE_MAP()

};