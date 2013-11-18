/*******************************************************
  源程序名称:TradeFundHSOpenDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  恒生柜台基金二次开户窗口类
  作    者:  shenglq
  开发日期:  20100903
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
	CTradeFundHSOpenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeFundHSOpenDlg();
	// 对话框数据
	enum { IDD = IDD_DIALOG_HSFUNDOPEN};
	CButton m_btOK;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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