#pragma once
#include "Resource.h"

//#include "..\..\Bin\HSWinner\face\Login\Resource.h"
#include "DlgJYInput.h"
#include "DlgLevelInput.h"

// CLoginWnd
enum{LoginError = -1, HQ, JY, HQJY};
#define KeyBoardMoveTimer 50

class CLoginWnd : public CUIWnd
{
	DECLARE_DYNAMIC(CLoginWnd)
	void WindowID() {IDD = IDW_LOGIN;}
protected:
	int m_nLoginType; 
	CDlgJYInput *m_cJYInput;
	CDlgLevelInput *m_cLevelInput;

	BOOL m_bLogin;
	BOOL m_bSpecialLogin;  //true表示特殊的登录框呈现 

public:
	CLoginWnd();
	virtual ~CLoginWnd();
	
	CDlgJYInput* GetJYInput() {return m_cJYInput;}
	void SetProgressMsg(CDataSourceInitDataStauts *msg);
	void SetSpecialLogin(BOOL SpecialLogin){m_bSpecialLogin = SpecialLogin;}
	BOOL GetSpecialLogin() {return m_bSpecialLogin;}
	int  GetLoginType(){return m_nLoginType;}
	BOOL IsLoginSuccess(){return m_bLogin;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedRadioHqjy();
	afx_msg void OnBnClickedRadioJy();
	afx_msg void OnBnClickedRadioHq();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonTuoji();
	afx_msg void OnBnClickedButtonGuide();
	afx_msg void OnBnClickedButtonComm();
	afx_msg void OnBnClickedButtonTest();

	afx_msg void OnTimer(UINT_PTR nIDEvent); 
	BOOL PreTranslateMessage(MSG* pMsg);
public:
	void Init();
	BOOL HasLevel2();
	BOOL QuoteInit();
	void EnableButtens(BOOL en);
	void Close();
	long ConnectServer();
	void CommMan();

private:
	CButton m_cBtnQuit;
	CButton m_cBtnLogin;
	CButton m_cBtnGuide;
	CButton m_cBtnComm;
	CButton m_cBtnSpeed;
	CButton m_cBtnOff;
	int m_nTimer;
	IAccount* pQHAccount;
};


