#pragma once
#include <afxdialogex.h>
#include "SecureEdit.h"
#include "resource.h"
// CDlgLevelInput �Ի���
  
class CDlgLevelInput : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLevelInput)

public:
	CDlgLevelInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLevelInput();

// �Ի�������
	enum { IDD = IDD_DIALOG_LEVELINPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	void Init();
	BOOL LoginLevel(const CString &strAccName = "", const CString &strAccPwd = "");
	void EnableInput(BOOL bEnable);

private:
	CSecureEdit   m_sEdit;
};
