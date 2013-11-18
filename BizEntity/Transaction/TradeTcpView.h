#pragma once
#include "..\..\HelperLib\WinMgr.h"
#include "..\..\Common\HSBizDialog.h"
#include "TradeCoreIntf.h"
#include "SplitterControl.h"
#include "afxcmn.h"
#include "afxwin.h"


#define WM_TRADE_CALLBACK		WM_USER + 1


// CTcpView 对话框

class CTradePackData
{
public:
	CTradePackData();
	~CTradePackData();
	DWORD m_nTickCount;
	DWORD m_nThreadId;
	int m_nAction;
	long m_nConnectHandle;
	char* m_pBuf;
	UINT m_nLen;
	int m_nType;
};

typedef CTypedPtrArray<CPtrArray, CTradePackData*> CTradePackDataList;

class CTcpView : public CHSBizDialog, public ITradeTcpView
{
	DECLARE_DYNAMIC(CTcpView)
public:
	CTcpView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTcpView();

	enum { IDD = IDD_TCPVIEW };

protected:
	ITradeCoreObj* m_pTradeCoreObj;
	CTradePackDataList m_TradePackDataList;
	//CSplitterControl m_wndSplitter;
	int m_nLastIndex;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void DoResize(int delta);
	DECLARE_MESSAGE_MAP()

	void OnShow(BOOL bShow);
	LRESULT OnTradeCallBack(WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL OnInit();
	virtual void OnHotKey(UINT nId);
	virtual void OnLinkageMsg(CLinkageParam* pParam);
	virtual void CallBackData(char* pBuf, UINT nLen, long nConnectHandle, int nAction, int nType);

	virtual BOOL OnInitDialog();
	void FillTestData();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CTreeCtrl m_trData;
	CListCtrl m_lsEntrust;
	afx_msg void OnLvnItemchangedEntrust(NMHDR *pNMHDR, LRESULT *pResult);
	CSplitterControl m_stSplitter;
};
