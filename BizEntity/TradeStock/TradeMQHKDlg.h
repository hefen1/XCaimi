/*******************************************************
  Դ��������:TradeMQHKDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����֤ȯ��ȯ����
  ��    ��:  shenglq
  ��������:  20110121
*********************************************************/
#pragma once
#include "TradeStockSellDlg.h"

class CTradeMQHKDlg : public CTradeStockSellDlg
{
	DECLARE_DYNAMIC(CTradeMQHKDlg)
public:
	CTradeMQHKDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeMQHKDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void LoadWnd();
	virtual BOOL OnInit();
private:
	DECLARE_MESSAGE_MAP()

};