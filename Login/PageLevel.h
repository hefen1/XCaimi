#pragma once
#include "resource.h"

// CPageLevel �Ի���

class CPageLevel : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageLevel)

public:
	CPageLevel();
	virtual ~CPageLevel();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_LEVEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void Init();
	BOOL OnApply();
};
