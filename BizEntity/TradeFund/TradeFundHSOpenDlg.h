/*******************************************************
  Դ��������:TradeFundHSOpenDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ������̨������ο���������
  ��    ��:  shenglq
  ��������:  20100903
*********************************************************/
#pragma once
#include "TradeFund.h"
#include "TradeDialogBase.h"
#include "HSNumEdit.h"
#include "afxcmn.h"
#include "afxwin.h"

class CTradeFundHSOpenDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CTradeFundHSOpenDlg)
public:
	CTradeFundHSOpenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeFundHSOpenDlg();
	// �Ի�������
	enum { IDD = IDD_DIALOG_HSFUNDOPEN};
	CButton m_btOK;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnWindowFocused();
	virtual void OnHotKey(UINT nId);
	virtual void OnShow(BOOL bShow);
	virtual void OnOK();
private:	
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbFundCompany;
	afx_msg void OnBnClickedBtOk();
};