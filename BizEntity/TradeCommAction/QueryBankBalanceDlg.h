/*******************************************************
  Դ��������:QueryBankBalanceDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ��ѯ����������
  ��    ��:  shenglq
  ��������:  20110310
*********************************************************/
#pragma once
#include "BankTransferBaseDlg.h"

class CQueryBankBalanceDlg : public CBankTransferBaseDlg
{
	DECLARE_DYNAMIC(CQueryBankBalanceDlg)
public:
	CQueryBankBalanceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryBankBalanceDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void LoadWnd();
	virtual void ShowPassword();
private:
	DECLARE_MESSAGE_MAP()

};