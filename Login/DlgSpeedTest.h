#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <afxmt.h>
#include "resource.h"
#include "NetEngineInterface.h"
// CDlgSpeedTest 对话框

class CDlgSpeedTest : public CDialog
{
	DECLARE_DYNAMIC(CDlgSpeedTest)

public:
	CDlgSpeedTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSpeedTest();

// 对话框数据
	enum { IDD = IDD_DIALOG_SPEEDTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CCriticalSection m_cCritical;
	int m_nTotalCount;
	int m_nNotiCount;
	BOOL m_bIsTesting;
	Server_type m_cType;
	CTabCtrl m_cTabCtl;
	CListCtrl m_cList;
	CButton m_cBtnTest;
	TradeCert m_tcInfo;
	CArray<SpeedTestResult,SpeedTestResult>m_ayTestResult;
	void TestSpeed();
	void StopTest();
	BOOL Notify(SpeedTestResult *pVoid);
	BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTcnSelchangeTabSpeet(NMHDR *pNMHDR, LRESULT *pResult);
	void Init();
	void Sort();
	BOOL FindRes(CString name);
	void ShowRes();
};
