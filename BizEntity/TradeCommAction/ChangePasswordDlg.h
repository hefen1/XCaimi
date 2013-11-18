/*******************************************************
  Դ��������:ChangePasswordDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  �޸����봰�ڻ���
  ��    ��:  shenglq
  ��������:  20110315
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
	CChangePasswordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChangePasswordDlg();
	virtual void SetSize();
	// �Ի�������
	enum { IDD = IDD_DIALOG_CHANGEPASSWORD};
protected:
	CString m_sDirection;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnShow(BOOL bShow);
	virtual void OnWindowFocused();
	virtual BOOL Validate();
	virtual void LoadWnd();
	virtual void OnHotKey(UINT nId);
	BOOL MoveKeyboard(CWnd* pWnd);     //�ƶ������ 
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