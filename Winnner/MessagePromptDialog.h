#pragma once
#include "afxwin.h"
#include "resource.h"


// CMessagePromptDialog ¶Ô»°¿ò
class CMessagePromptDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMessagePromptDialog)

public:	
	enum { IDD = IDD_MESSAGE };
	enum MessageType {mtError,mtWaring,mtMessage};

	MessageType m_mt;
	CString m_strMsg;
	Image *m_pImg;

	CMessagePromptDialog(CWnd* pParent = NULL); 
	virtual ~CMessagePromptDialog();
	void ShowMsg(MessageType mt,CString strMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
