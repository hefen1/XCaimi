#pragma once
#include "Resource.h"
#include "DiagramFoundationDef.h"
#include <afxdhtml.h>

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CDHtmlShowDlg 对话框

class HS_EXT_CLASS CDHtmlShowDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDHtmlShowDlg)

public:
	CDHtmlShowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDHtmlShowDlg();

// 对话框数据
	enum { IDD = IDD_SHOWINFO, IDH = 0};
	
	void SetUserUrl(CString str);
	void SetCaption(CString str);
	void Show();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()

private:
	CString    m_strUserUrl;
	CString    m_strUserCaption;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
