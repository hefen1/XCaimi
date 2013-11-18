#pragma once

#include "Resource.h"

// CInfomationForm 对话框

class CInfomationForm : public CDialog
{
	DECLARE_DYNAMIC(CInfomationForm)

public:
	CInfomationForm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInfomationForm();

// 对话框数据
	enum { IDD = IDD_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
