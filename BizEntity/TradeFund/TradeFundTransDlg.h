/*******************************************************
  Դ��������:TradeFundTransDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����ת��
  ��    ��:  shenglq
  ��������:  20110216
*********************************************************/
#pragma once
#include "TradeFundBaseDlg.h"

class CTradeFundTransDlg : public CTradeFundBaseDlg
{
	DECLARE_DYNAMIC(CTradeFundTransDlg)
public:
	CTradeFundTransDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeFundTransDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnFundCodeExit();
	virtual void DoEntrust();
	virtual void LoadWnd();
	virtual BOOL Validate();

	DECLARE_MESSAGE_MAP()

};