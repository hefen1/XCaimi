/*******************************************************
  Դ��������:TradeStockSetCostPriceDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ���óɱ��۴��ڻ���
  ��    ��:  shenglq
  ��������:  20100903
*********************************************************/
#pragma once
#include "TradeStock.h"
#include "TradeDialogBase.h"
#include "QueryStockAccountIntf.h"
#include "HSNumEdit.h"
#include "afxcmn.h"
#include "afxwin.h"


class CTradeStockSetCostPriceDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CTradeStockSetCostPriceDlg)
public:
	CTradeStockSetCostPriceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockSetCostPriceDlg();
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);

	// �Ի�������
	enum { IDD = IDD_DIALOG_SETCOSTPRICE};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnStockCodeExit();
	//���ί������
	virtual BOOL Validate();
	virtual void DoEntrust();
	virtual void OnWindowFocused();
	virtual void OnHotKey(UINT nId);
	virtual void OnLinkageMsg(CLinkageParam* pParam);
private:
	CString m_sLastStockCode;
	IStockAccountList* m_pStockAccountList;
	ITradeMarketList* m_pTradeMarketList;
	IStockHoldList* m_pCurrHoldList;
	void Clear(int iFlag = 0);
	void SelectStockAccount();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbStockAccount;
	CEdit m_edStockCode;
	CHSNumEdit m_edPrice;
	CButton m_btOK;
	CStatic m_edStockName;
	afx_msg void OnBnClickedBtOk();
	afx_msg void OnEnKillfocusEdStockcode();
	afx_msg void OnEnChangeEdStockcode();
};