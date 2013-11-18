#pragma once
#include <afxdialogex.h>
#include "SecureEdit.h"
#include "resource.h"
// CDlgLevelInput 对话框
  
class CDlgLevelInput : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLevelInput)

public:
	CDlgLevelInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLevelInput();

// 对话框数据
	enum { IDD = IDD_DIALOG_LEVELINPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
