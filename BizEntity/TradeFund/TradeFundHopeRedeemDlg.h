/*******************************************************
  Դ��������:TradeFundHopeRedeemDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����ԤԼ���
  ��    ��:  shenglq
  ��������:  20110216
*********************************************************/
#pragma once
#include "TradeFundBaseDlg.h"

class CTradeFundHopeRedeemDlg : public CTradeFundBaseDlg
{
	DECLARE_DYNAMIC(CTradeFundHopeRedeemDlg)
public:
	CTradeFundHopeRedeemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeFundHopeRedeemDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnFundCodeExit();
	virtual void DoEntrust();
	virtual void LoadWnd();
	virtual BOOL Validate();

	DECLARE_MESSAGE_MAP()

};