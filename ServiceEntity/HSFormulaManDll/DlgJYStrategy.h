#pragma once
#include "resource.h"
#include "afxdtctl.h"
#include "afxwin.h"

// CDlgJYStrategy 对话框

class CDlgJYStrategy : public CDialog
{
	DECLARE_DYNAMIC(CDlgJYStrategy)

public:
	CDlgJYStrategy(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgJYStrategy();

// 对话框数据
	enum { IDD = 0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
