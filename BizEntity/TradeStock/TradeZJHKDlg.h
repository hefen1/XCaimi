/*******************************************************
  Դ��������:TradeZJHKDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ֤ȯֱ�ӻ������
  ��    ��:  shenglq
  ��������:  20110330
*********************************************************/
#pragma once
#include "TradeStock.h"
#include "TradeDialogBase.h"
#include "HSNumEdit.h"
#include "afxcmn.h"
#include "afxwin.h"


class CTradeZJHKDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CTradeZJHKDlg)
public:
	CTradeZJHKDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeZJHKDlg();
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);

	// �Ի�������
	enum { IDD = IDD_DIALOG_ZJHK};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	//���ί������
	virtual BOOL Validate();
	//virtual void DoEntrust();
	virtual void OnWindowFocused();
	virtual void OnHotKey(UINT nId);
	virtual void OnShow(BOOL bShow);
private:
	INodeTreeData* m_MoneyTypeList;
	void QueryEnableBalance();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbMoneyType;
	CHSNumEdit m_edBalance;
	CEdit m_edEnableBalance;
	CEdit m_edRealBalance;
	afx_msg void OnBnClickedBtOk();
	CButton m_btOK;
};