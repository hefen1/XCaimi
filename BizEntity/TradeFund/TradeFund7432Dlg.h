/*******************************************************
  Դ��������:TradeFund7432Dlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����Ͷ
  ��    ��:  shenglq
  ��������:  20110216
*********************************************************/
#pragma once
#include "TradeFundBaseDlg.h"

class CTradeFund7432Dlg : public CTradeFundBaseDlg
{
	DECLARE_DYNAMIC(CTradeFund7432Dlg)
public:
	CTradeFund7432Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeFund7432Dlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnFundCodeExit();
	virtual void DoEntrust();
	virtual void LoadWnd();
	virtual BOOL Validate();
	virtual void OnCodeChangedFocus();

	DECLARE_MESSAGE_MAP()

};