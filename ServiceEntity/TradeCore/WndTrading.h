/*******************************************************
  源程序名称:WndTrading.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  同步请求窗体类定义
  作    者:  shenglq
  开发日期:  20101025
*********************************************************/
#pragma once

#define WM_TRADE_THREAD_ASYNC		WM_USER + 1
#define WM_ACCOUNT_STATUS			WM_USER + 2

class CWndTrading : public CWnd
{
	DECLARE_DYNAMIC(CWndTrading)
public:
	CWndTrading();
	~CWndTrading();

protected:
	DECLARE_MESSAGE_MAP()
	LRESULT OnThreadAsync(WPARAM wParam, LPARAM lParam);
	LRESULT OnAccountStatus(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};