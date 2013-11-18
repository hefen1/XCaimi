#pragma once
#include <afxdialogex.h>
#include "..\Controls\YTControl\SecureEdit.h"
#include "..\Controls\YTControl\KeyboardDlg.h"
#include "MyComBox.h"
#include "resource.h"
#include "TradeSysConfigIntf.h"
// CDlgJYInput 对话框

class CDlgJYInput : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgJYInput)

public:
	CDlgJYInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgJYInput();

	// 对话框数据
	enum { IDD = IDD_DIALOG_JY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	BOOL OnInitDialog();
	void Init();
	void EnableInput(BOOL en);
	BOOL PreTranslateMessage(MSG* pMsg);
	void CreateValidate();   // 创建验证码
	BOOL LoginJY();          // 交易登录
	long ConnectJYSrv();     // 连接交易服务器
	CKeyboardDlg* GetKeyboardDlg() {return m_keyboardDlg;}
	BOOL MoveKeyboard();     // 移动软键盘 
	int  GetCurrentCellID(); // 获取当前营业部ID
	void ChangeJYServer();   // 改变交易服务器地址
private:
	CMyComBox     m_comBoxAcc;
	CStringArray  m_ayAccInfo;

	CSecureEdit   m_sEdit;
	CKeyboardDlg* m_keyboardDlg;
	ITradeAccountTypeList* m_pAccountTypeList;
	ICellList* m_pCellList;

	BOOL    m_bSaveAcc;   // 是否记录账号
	BOOL    m_bAccUpdate; // 账号是否有更新
	BOOL    m_bHideAcc;   // 是否隐藏帐号
protected:
	HBITMAP m_BitHandle[4];
	CString strValidate;
	CString strPreValidate; //记录前一个验证码 防止重绘过程中又重新绘制

	void LoadAccountType(const CString& strType = "");
	void SaveAccInfo(CString strAcc, CString strType, CString strCell);

	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonSoftkeyboard();	
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCheckHide();
	afx_msg void OnBnClickedCheckRemember();
	afx_msg void OnCbnSelchangeComboCell();
	afx_msg void OnCbnSelchangeComboAccount();
public:
	afx_msg void OnCbnSelchangeComboAccounttype();
	afx_msg void OnCbnSelendokComboAccounttype();
};
