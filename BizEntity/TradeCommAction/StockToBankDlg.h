/*******************************************************
  Դ��������:StockToBankDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ֤ȯת���д�����
  ��    ��:  shenglq
  ��������:  20110310
*********************************************************/
#pragma once
#include "BankTransferBaseDlg.h"

class CStockToBankDlg : public CBankTransferBaseDlg
{
	DECLARE_DYNAMIC(CStockToBankDlg)
public:
	CStockToBankDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStockToBankDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void ShowPassword();
private:
	DECLARE_MESSAGE_MAP()

};