/*******************************************************
  Դ��������:TradeStockBaseDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ֤ȯί�д��ڻ���
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
#include "HSStatic.h"
#include "HSButton.h"


class CTradeStockBaseDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CTradeStockBaseDlg)
public:
	CTradeStockBaseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeStockBaseDlg();
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);
	virtual BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen);

	// �Ի�������
	enum { IDD = IDD_DIALOG_STOCKENTRUST};
	CComboBox m_cbStockAccount;
	CEdit m_edStockCode;
	CStatic m_edStockName;
	CHSNumEdit m_edPrice;
	CHSStatic m_stEnableBalance;
	CEdit m_edEnableBalance;
	CHSStatic m_stEnableAmount;
	CEdit m_edEnableAmount;
	CHSStatic m_stAmount;
	CHSNumEdit m_edAmount;
	CHSButton m_btOK;
protected:
	CString m_sLastStockCode;
	CString m_sLasStockCodePriceExit;
	double m_dLastPrice;
	EBSDirection m_eBSDirection;
	CString m_sEntrustProp;
	IStockAccountList* m_pStockAccountList;
	ITradeMarketList* m_pTradeMarketList;
	long m_lCurrPrevClose;
	INodeTreeData* m_pSHSJType;
	INodeTreeData* m_pSZSJType;
	INodeTreeData* m_StockEntrustType;
	CString m_sEntrustType;
	BOOL m_bQuickTrade;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnShow(BOOL bShow);
	virtual void OnStockCodeExit();
	virtual void OnEntrustPriceExit();
	//���ί������
	virtual BOOL Validate();
	virtual void DoEntrust();
	virtual void OnWindowFocused();
	virtual void OnHotKey(UINT nId);
	CString GetEntrustProp();
	virtual void OnAmountEnter();
	void Clear(int iFlag = 0);
	virtual void OnLinkageMsg(CLinkageParam* pParam);
	virtual void OnOK();
	virtual void OnParamChanged(IHSParam* pParam);
private:
	void SelectStockAccount();
	void RequestHQ(StockUserInfo sStockInfo);
	void UnRequestHQ();
	void LoadSJType();
	void InitColor();
	void MessageFillCodePrice(CString sCodeField, CString sPriceField, CLinkageParam* pParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtOk();
	afx_msg void OnEnKillfocusEdStockcode();
	afx_msg void OnEnKillfocusEdPrice();
	afx_msg void OnEnChangeEdStockcode();
	afx_msg void OnStnClickedEdEnableamount();
	CSpinButtonCtrl m_spPrice;
	CSpinButtonCtrl m_spAmount;
	afx_msg void OnDeltaposSpPrice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpAmount(NMHDR *pNMHDR, LRESULT *pResult);
	CHSStatic m_stStockAccount;
	CHSStatic m_stStockCode;
	CHSStatic m_stPrice;
	CHSStatic m_stStockName;
	CHSStatic m_stSJType;
	CComboBox m_cbSJType;
	afx_msg void OnCbnKillfocusCbSjtype();
	afx_msg void OnCbnSelchangeCbStockaccount();
	afx_msg void OnEnSetfocusEdAmount();
	CHSStatic m_stSeatNo;
	CEdit m_edSeatNo;
	CHSStatic m_stConferNo;
	CEdit m_edConferNo;
	CHSStatic m_stTransCode;
	CEdit m_edTransCode;
};