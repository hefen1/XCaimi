/*******************************************************
  Դ��������:TradeZQHSDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ծȯ����
  ��    ��:  shenglq
  ��������:  20100317
*********************************************************/
#pragma once
#include "TradeZQToStockDlg.h"

class CTradeZQHSDlg : public CTradeZQToStockDlg
{
	DECLARE_DYNAMIC(CTradeZQHSDlg)
public:
	CTradeZQHSDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeZQHSDlg();
protected:	
	virtual BOOL OnInit();
private:
	DECLARE_MESSAGE_MAP()
};