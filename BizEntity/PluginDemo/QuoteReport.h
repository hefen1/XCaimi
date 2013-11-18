#pragma once

#include "../../Common/HSBizDialog.h"

// CQuoteReport 对话框

class CQuoteReport : public CHSBizDialog
{
	DECLARE_DYNAMIC(CQuoteReport)

public:
	CQuoteReport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQuoteReport();

// 对话框数据
	enum { IDD = IDD_QUOTEREPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
