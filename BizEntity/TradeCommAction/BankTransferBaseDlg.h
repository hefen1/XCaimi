/*******************************************************
  Դ��������:BankTransferBaseDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ��֤ת�ʴ��ڻ���
  ��    ��:  shenglq
  ��������:  20110310
*********************************************************/
#pragma once
#include "TradeCommAction.h"
#include "TradeDialogBase.h"
#include "afxwin.h"
#include "SecureEdit.h"
#include "KeyboardDlg.h"
#include "HSNumEdit.h"

class CBankTransferBaseDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CBankTransferBaseDlg)
public:
	CBankTransferBaseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBankTransferBaseDlg();
	virtual void SetSize();

	// �Ի�������
	enum { IDD = IDD_DIALOG_BANKTRANSFER};
protected:
	IBankList* m_BankList;
	INodeTreeData* m_MoneyTypeList;
	CString m_sDirection;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnShow(BOOL bShow);
	virtual void OnWindowFocused();
	virtual BOOL Validate();
	virtual void LoadWnd();
	virtual void OnHotKey(UINT nId);
	virtual void ShowPassword();
	BOOL MoveKeyboard(CWnd* pWnd);     //�ƶ������ 
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CComboBox m_cbBankNo;
	CComboBox m_cbMoneyType;
	CSecureEdit m_edBankPsw;
	CSecureEdit m_edFundPsw;
	CHSNumEdit m_edBalance;
	CButton m_btOK;
	CStatic m_stBankNo;
	CStatic m_stMoneyType;
	CStatic m_stBankPsw;
	CStatic m_stFundPsw;
	CStatic m_stBalance;
	afx_msg void OnCbnSelchangeCbBankno();
	afx_msg void OnBnClickedBtOk();
	CButton m_btBank;
	CButton m_btFund;
private:
	CKeyboardDlg* m_keyboardDlg;
public:
	afx_msg void OnBnClickedBtBank();
	afx_msg void OnBnClickedBtFund();
	afx_msg void OnEnSetfocusEdFundpsw();
	afx_msg void OnEnSetfocusEdBankpsw();
};