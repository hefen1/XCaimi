#pragma once
#include <afxbutton.h>
#include <afxtoolbar.h>
#include "HSBizDialog.h"
#include "TradeCoreIntf.h"
#include "WinnerApplication.h"
#include "TradeConfigObjIntf.h"
#include "TradeLangMngIntf.h"
#include "TradeLangDefined.h"
#include "LoginInterface.h"
#include "afxwin.h"

class CHSImgButton: public CMFCButton
{
protected:
	virtual int GetImageHorzMargin() const;
	virtual int GetVertMargin() const;
	virtual void OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState);
public:
	void ChangeImg(CString strImgFile);
};

// CTradeToolBar 对话框

class CTradeToolBar : public CHSBizDialog, public IAccountChange
{
	DECLARE_DYNAMIC(CTradeToolBar)

public:
	CTradeToolBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeToolBar();

// 对话框数据
	enum { IDD = IDD_TRADETOOLBAR };

	CButton			m_btRefresh;
	CEdit			m_edAccount;
	CEdit			m_edPsw;
	CComboBox		m_cbFundAccount;
	CStatic			m_stFundAccount;
	CHSImgButton	m_btMin;
	CHSImgButton	m_btMax;
	CHSImgButton	m_btClose;
	CButton			m_btLock;
	CStatic			m_stActionName;
	CMenu			m_menuMoreAccount;

public:
	virtual void OnAccountChange(IAccount* lpValue, EAccountChange nChangeType);
	virtual void OnAccountStatus(IAccount* lpValue);
	virtual void OnNoOperatTimeout();
protected:
	//CMFCButton  m_btnMenuControler; 
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInit();
	void OnLayoutStateChange();
	virtual void OnLinkageMsg(CLinkageParam* pParam);
	virtual void OnShow(BOOL bShow);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtLogin();
	afx_msg void OnLogin();
	afx_msg void OnExit();
private:
	ITradeCoreObj*		m_pTradeCoreObj;
	IAccountMng*		m_pAccountMng;
	ITradeConfigObj*	m_pTradeConfigObj;
	ITradeLang*			m_pTradeLang;
	IDoLogin*			m_pDoLogin;
public:
	afx_msg void OnBnClickedBtRefresh();
	afx_msg void OnBnClickedBtMin();
	afx_msg void OnBnClickedBtMax();
	afx_msg void OnBnClickedBtClose();
	afx_msg void OnLockTrade();
	CButton m_btMoreAccount;
	afx_msg void OnBnClickedBtMoreaccount();
	afx_msg void OnCbnSelchangeCbFundaccount();
};
