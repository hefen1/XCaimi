#pragma once
#include <afxdialogex.h>
#include "..\Controls\YTControl\SecureEdit.h"
#include "..\Controls\YTControl\KeyboardDlg.h"
#include "MyComBox.h"
#include "resource.h"
#include "TradeSysConfigIntf.h"
// CDlgJYInput �Ի���

class CDlgJYInput : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgJYInput)

public:
	CDlgJYInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgJYInput();

	// �Ի�������
	enum { IDD = IDD_DIALOG_JY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	BOOL OnInitDialog();
	void Init();
	void EnableInput(BOOL en);
	BOOL PreTranslateMessage(MSG* pMsg);
	void CreateValidate();   // ������֤��
	BOOL LoginJY();          // ���׵�¼
	long ConnectJYSrv();     // ���ӽ��׷�����
	CKeyboardDlg* GetKeyboardDlg() {return m_keyboardDlg;}
	BOOL MoveKeyboard();     // �ƶ������ 
	int  GetCurrentCellID(); // ��ȡ��ǰӪҵ��ID
	void ChangeJYServer();   // �ı佻�׷�������ַ
private:
	CMyComBox     m_comBoxAcc;
	CStringArray  m_ayAccInfo;

	CSecureEdit   m_sEdit;
	CKeyboardDlg* m_keyboardDlg;
	ITradeAccountTypeList* m_pAccountTypeList;
	ICellList* m_pCellList;

	BOOL    m_bSaveAcc;   // �Ƿ��¼�˺�
	BOOL    m_bAccUpdate; // �˺��Ƿ��и���
	BOOL    m_bHideAcc;   // �Ƿ������ʺ�
protected:
	HBITMAP m_BitHandle[4];
	CString strValidate;
	CString strPreValidate; //��¼ǰһ����֤�� ��ֹ�ػ�����������»���

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
