/*******************************************************
  Դ��������:TradeFundRedeemDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  �������
  ��    ��:  shenglq
  ��������:  20110216
*********************************************************/
#pragma once
#include "TradeFundBaseDlg.h"

class CTradeFundRedeemDlg : public CTradeFundBaseDlg
{
	DECLARE_DYNAMIC(CTradeFundRedeemDlg)
public:
	CTradeFundRedeemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeFundRedeemDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnFundCodeExit();
	virtual void DoEntrust();
	virtual void LoadWnd();
	virtual BOOL Validate();

	DECLARE_MESSAGE_MAP()

};