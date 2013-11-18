/*******************************************************
  源程序名称:TradeLockDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  锁屏
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#pragma once
#include "Transaction.h"
#include "HSBizDialog.h"
#include "TradeCoreIntf.h"
#include "WinnerApplication.h"
#include "TradeConfigObjIntf.h"
#include "TradeLangMngIntf.h"
#include "TradeLangDefined.h"


// TradeLockDlg 对话框

class CTradeLockDlg : public CHSBizDialog
{
	DECLARE_DYNAMIC(CTradeLockDlg)

public:
	CTradeLockDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeLockDlg();

	// 对话框数据
	enum { IDD = IDD_TRADE_LOCK };

	//CButton			m_btRefresh;
	CEdit			m_edAccount;
	CEdit			m_edPsw;
	CComboBox		m_cbFundAccount;
	CStatic			m_stFundAccount;
	CStatic			m_stHint;
	CStatic			m_stPsw;
	//CHSImgButton	m_btMin;
	//CHSImgButton	m_btMax;
	//CHSImgButton	m_btClose;
	CButton			m_btOK;
	CButton			m_btCancel;
	CButton			m_btExit;
	CStatic			m_stErrorHint;
protected:
	//CMFCButton  m_btnMenuControler; 
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInit();
	virtual void OnShow(BOOL bShow);
	virtual void OnWindowFocused();

	DECLARE_MESSAGE_MAP()
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	ITradeCoreObj*		m_pTradeCoreObj;
	IAccountMng*		m_pAccountMng;
	ITradeConfigObj*	m_pTradeConfigObj;
	ITradeLang*			m_pTradeLang;
public:
	afx_msg void OnBnClickedBtExit();
	afx_msg void OnBnClickedBtCancel();
	afx_msg void OnBnClickedBtOk();
};