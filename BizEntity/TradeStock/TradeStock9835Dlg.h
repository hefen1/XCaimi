/*******************************************************
  Դ��������:TradeStock9835Dlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ETF�����ֽ��Ϲ�
  ��    ��:  shenglq
  ��������:  20110421
*********************************************************/
#pragma once
#include "TradeStockSubcribeDlg.h"

class CTradeStock9835Dlg : public CTradeStockSubcribeDlg
{
	DECLARE_DYNAMIC(CTradeStock9835Dlg)
public:
	CTradeStock9835Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStock9835Dlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
private:
	DECLARE_MESSAGE_MAP()

};