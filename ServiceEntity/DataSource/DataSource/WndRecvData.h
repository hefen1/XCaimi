#pragma once


// CWndRecvData
class CDataSourceImpl;

#define WM_THREAD_ASYNC		WM_USER + 1
#define WM_THREAD_STATUS	WM_USER + 2
class CWndRecvData : public CWnd
{
	DECLARE_DYNAMIC(CWndRecvData)
protected:
	CDataSourceImpl*		m_pDataSourceImpl;
public:
	CWndRecvData(CDataSourceImpl* pDataSourceImpl);
	virtual ~CWndRecvData();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnThreadAsync(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStatusNotify(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnClose();
};


