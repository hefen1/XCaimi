/*******************************************************
  Դ��������:TradeFundAllotDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  �����깺
  ��    ��:  shenglq
  ��������:  20110216
*********************************************************/
#pragma once
#include "TradeFundBaseDlg.h"

class CTradeFundAllotDlg : public CTradeFundBaseDlg
{
	DECLARE_DYNAMIC(CTradeFundAllotDlg)
public:
	CTradeFundAllotDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeFundAllotDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnFundCodeExit();
	virtual void DoEntrust();
	virtual void LoadWnd();
	virtual BOOL Validate();

	DECLARE_MESSAGE_MAP()

};