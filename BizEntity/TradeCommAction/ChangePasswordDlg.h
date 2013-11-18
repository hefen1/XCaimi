/*******************************************************
  源程序名称:ChangePasswordDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  修改密码窗口基类
  作    者:  shenglq
  开发日期:  20110315
*********************************************************/
#pragma once
#include "TradeCommAction.h"
#include "TradeDialogBase.h"
#include "afxwin.h"
#include "SecureEdit.h"
#include "KeyboardDlg.h"
#include "HSNumEdit.h"

class CChangePasswordDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CChangePasswordDlg)
public:
	CChangePasswordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangePasswordDlg();
	virtual void SetSize();
	// 对话框数据
	enum { IDD = IDD_DIALOG_CHANGEPASSWORD};
protected:
	CString m_sDirection;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual void OnShow(BOOL bShow);
	virtual void OnWindowFocused();
	virtual BOOL Validate();
	virtual void LoadWnd();
	virtual void OnHotKey(UINT nId);
	BOOL MoveKeyboard(CWnd* pWnd);     //移动软键盘 
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	CKeyboardDlg* m_keyboardDlg;
	IModifyPassowrdTypeList* m_PswTypeList;
public:
	CButton m_btOK;
	CComboBox m_cbPswType;
	CSecureEdit m_edOldPsw;
	CSecureEdit m_edNewPsw;
	CSecureEdit m_edConfirmPsw;
	CButton m_btOld;
	CButton m_btNew;
	CButton m_btConfirm;
	afx_msg void OnBnClickedBtOk();
	afx_msg void OnBnClickedBtOld();
	afx_msg void OnBnClickedBtNew();
	afx_msg void OnBnClickedBtConfirm();
	afx_msg void OnEnSetfocusEdOldpsw();
	afx_msg void OnEnSetfocusEdNewpsw();
	afx_msg void OnEnSetfocusEdConfirmpsw();
};