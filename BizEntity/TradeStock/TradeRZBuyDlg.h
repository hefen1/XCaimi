/*******************************************************
  Դ��������:TradeRZBuyDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����֤ȯ��������
  ��    ��:  shenglq
  ��������:  20110121
*********************************************************/
#pragma once
#include "TradeStockBuyDlg.h"

class CTradeRZBuyDlg : public CTradeStockBuyDlg
{
	DECLARE_DYNAMIC(CTradeRZBuyDlg)
public:
	CTradeRZBuyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeRZBuyDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void LoadWnd();
private:
	DECLARE_MESSAGE_MAP()

};