/*******************************************************
  Դ��������:TradeFundSubcribeDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  �����Ϲ�
  ��    ��:  shenglq
  ��������:  20110216
*********************************************************/
#pragma once
#include "TradeFundBaseDlg.h"

class CTradeFundSubcribeDlg : public CTradeFundBaseDlg
{
	DECLARE_DYNAMIC(CTradeFundSubcribeDlg)
public:
	CTradeFundSubcribeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeFundSubcribeDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnFundCodeExit();
	virtual void DoEntrust();
	virtual void LoadWnd();
	virtual BOOL Validate();

	DECLARE_MESSAGE_MAP()

};