/*******************************************************
  Դ��������:TradeSetDividendDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ���÷ֺ췽ʽ
  ��    ��:  shenglq
  ��������:  20110420
*********************************************************/
#pragma once
#include "TradeFundBaseDlg.h"

class CTradeSetDividendDlg : public CTradeFundBaseDlg
{
	DECLARE_DYNAMIC(CTradeSetDividendDlg)
public:
	CTradeSetDividendDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeSetDividendDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnFundCodeExit();
	virtual void DoEntrust();
	virtual void LoadWnd();
	virtual BOOL Validate();

	DECLARE_MESSAGE_MAP()

};