/*******************************************************
  Դ��������:TradeStockSubcribeDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ���ڻ����Ϲ�
  ��    ��:  shenglq
  ��������:  20110316
*********************************************************/
#pragma once
#include "TradeStockAllotDlg.h"

class CTradeStockSubcribeDlg : public CTradeStockAllotDlg
{
	DECLARE_DYNAMIC(CTradeStockSubcribeDlg)
public:
	CTradeStockSubcribeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockSubcribeDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual BOOL Validate();
private:
	DECLARE_MESSAGE_MAP()

};