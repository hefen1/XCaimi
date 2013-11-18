#pragma once
#include "afxwin.h"
#include "DlgJYInput.h"
#include "DlgLevelInput.h"
#include <afxdialogex.h>
#include "LoginInterface.h"
#include "TradeAccountMngIntf.h"
#include "DataSourceDefine.h"
#include "../HelperLib/MyButton.h"
// CDlgLogin 对话框

#define KeyBoardMoveTimer 50
class CDlgLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLogin)

public:
	CDlgLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLogin();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRadioHqjy();
	afx_msg void OnBnClickedRadioJy();
	afx_msg void OnBnClickedRadioHq();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonTuoji();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonGuide();
	afx_msg void OnBnClickedButtonComm();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	LoginRet m_nLoginType; 
	CDlgJYInput *m_pJYInputDlg;
	CDlgLevelInput *m_pLevelInputDlg;
	CStatic m_csProgress;
	
	BOOL m_bLogin;
	LoginType m_eDlgType;  // 登录框呈现方式 
	int  m_nTimer;
public:
	void CommMan();
	void SetProgressMsg(CDataSourceInitDataStauts *msg);
	void SetSpecialLogin(LoginType eType){m_eDlgType = eType;}
	BOOL GetSpecialLogin() {return m_eDlgType;}
	LoginRet  GetLoginType(){return m_nLoginType;}
	BOOL IsLoginSuccess(){return m_bLogin;}

	BOOL QuoteInit();
	BOOL FutureInit();
	BOOL Level2Init(const CString &strAccName = "", const CString &strAccPwd = "");
	void Init();
	BOOL HasLevel2();
	void EnableButtens(BOOL en);

public:
	BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	CMyButton m_cBtnQuit;
	CMyButton m_cBtnLogin;
	CMyButton m_cBtnGuide;
	CMyButton m_cBtnComm;
	CMyButton m_cBtnSpeed;
	CMyButton m_cBtnOff;

	afx_msg void OnPaint();
};
