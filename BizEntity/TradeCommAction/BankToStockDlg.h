/*******************************************************
  Դ��������:BankToStockDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����ת֤ȯ������
  ��    ��:  shenglq
  ��������:  20110310
*********************************************************/
#pragma once
#include "BankTransferBaseDlg.h"

class CBankToStockDlg : public CBankTransferBaseDlg
{
	DECLARE_DYNAMIC(CBankToStockDlg)
public:
	CBankToStockDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBankToStockDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void ShowPassword();
private:
	DECLARE_MESSAGE_MAP()

};