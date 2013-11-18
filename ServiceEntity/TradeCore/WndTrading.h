/*******************************************************
  Դ��������:WndTrading.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ͬ���������ඨ��
  ��    ��:  shenglq
  ��������:  20101025
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