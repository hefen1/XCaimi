/*******************************************************
  Դ��������:TradeMQHQDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����֤ȯ��ȯ��ȯ
  ��    ��:  shenglq
  ��������:  20110121
*********************************************************/
#pragma once
#include "TradeStockBuyDlg.h"

class CTradeMQHQDlg : public CTradeStockBuyDlg
{
	DECLARE_DYNAMIC(CTradeMQHQDlg)
public:
	CTradeMQHQDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeMQHQDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void LoadWnd();
	virtual BOOL OnInit();
private:
	DECLARE_MESSAGE_MAP()

};