/*******************************************************
  Դ��������:TradeFundHopeAllotDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����ԤԼ�깺
  ��    ��:  shenglq
  ��������:  20110216
*********************************************************/
#pragma once
#include "TradeFundBaseDlg.h"

class CTradeFundHopeAllotDlg : public CTradeFundBaseDlg
{
	DECLARE_DYNAMIC(CTradeFundHopeAllotDlg)
public:
	CTradeFundHopeAllotDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeFundHopeAllotDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnFundCodeExit();
	virtual void DoEntrust();
	virtual void LoadWnd();
	virtual BOOL Validate();

	DECLARE_MESSAGE_MAP()

};