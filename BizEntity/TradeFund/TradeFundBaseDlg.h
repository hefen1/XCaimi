/*******************************************************
  源程序名称:TradeFundBaseDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  基金委托窗口基类
  作    者:  shenglq
  开发日期:  20100903
*********************************************************/
#pragma once
#include "TradeFund.h"
#include "TradeDialogBase.h"
#include "HSNumEdit.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "QueryFundInfoIntf.h"
#include "afxdtctl.h"


class CTradeFundBaseDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CTradeFundBaseDlg)
public:
	CTradeFundBaseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeFundBaseDlg();
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);

	// 对话框数据
	enum { IDD = IDD_DIALOG_FUNDENTRUST};
	CStatic m_stFundCode;
	CEdit m_edFundCode;
	
	CStatic m_stFundName;	
	CStatic m_edFundName;

	CStatic m_stFundNav;
	CStatic m_edFundNav;

	CStatic m_stEnableBalance;
	CStatic m_edEnableBalance;

	CStatic m_stBalance;
	CHSNumEdit m_edBalance;

	CButton m_btOK;
protected:
	CString m_sLastFundCode;
	IFundInfoList* m_FundInfoList;
	IFundInfoItem* m_FundInfoItem;
	INodeTreeData* m_pExceedFlag;
	INodeTreeData* m_pChargeType;
	INodeTreeData* m_pDividendMethod;
	CString m_sAllotDate;
	CString m_sAllotNo;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual void OnFundCodeExit();
	//检查委托数据
	virtual BOOL Validate();
	virtual void DoEntrust();
	virtual void OnWindowFocused();
	virtual void OnHotKey(UINT nId);
	void QueryEnableBalance();
	void QueryEnableAmount();
	void Clear(int iFlag = 0);
	void LoadList(INodeTreeData* pList, CComboBox& pComboBox);
	CString GetListValue(INodeTreeData* pList, CComboBox& pComboBox);
	virtual void OnCodeChangedFocus();
	virtual void OnOK();
	virtual void OnLinkageMsg(CLinkageParam* pParam);
private:	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtOk();
	afx_msg void OnEnKillfocusEdFundcode();
	afx_msg void OnEnChangeEdFundcode();
	CComboBox m_cbChargeType;
	CStatic m_stChargeType;
	CStatic m_stDividendMethod;
	CComboBox m_cbDividendMethod;
	CStatic m_stExceedFlag;
	CComboBox m_cbExceedFlag;
	CStatic m_stTransCode;
	CEdit m_edTransCode;
	CStatic m_stHopeDate;
	CDateTimeCtrl m_dtHopeDate;
	CStatic m_stBeginDate;
	CDateTimeCtrl m_dtBeginDate;
	CStatic m_stEndDate;
	CDateTimeCtrl m_dtEndDate;
	CStatic m_stFundDate;
	CHSNumEdit m_edFundDate;
};