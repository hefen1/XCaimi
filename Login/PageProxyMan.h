#pragma once
#include "afxwin.h"
#include "resource.h"
#include "NetEngineInterface.h"

// CPageProxyMan 对话框

class CPageProxyMan : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageProxyMan)

public:
	CPageProxyMan();
	virtual ~CPageProxyMan();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_PROXYMAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckProxy();
	afx_msg void OnBnClickedCheckChennal();
	afx_msg void OnBnClickedButtonTest();
	CIPAddressCtrl m_cProxy;
	CEdit m_cPort;
	CEdit m_cUser;
	CEdit m_cPWD;
	CButton m_cCheckProxy;
	CButton m_cCheckChannel;
	CButton m_cCheckAuto;

public:
	virtual BOOL OnInitDialog();
	BOOL GetProxyInfo(ProxyInfo *proxy);
	void Init();
	void Enable(BOOL benable);
	BOOL OnApply();
	afx_msg void OnBnClickedRadioSocket4();
	afx_msg void OnBnClickedRadioHttp();
	afx_msg void OnBnClickedRadioSocket5();
};
