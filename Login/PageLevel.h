#pragma once
#include "resource.h"

// CPageLevel 对话框

class CPageLevel : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageLevel)

public:
	CPageLevel();
	virtual ~CPageLevel();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_LEVEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void Init();
	BOOL OnApply();
};
