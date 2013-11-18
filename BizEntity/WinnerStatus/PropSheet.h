#pragma once

#include "prop.h"

// CPropSheet

class CPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSheet)

public:
	CPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropSheet();

protected:
	DECLARE_MESSAGE_MAP()
//公共变量
public:
	CProp m_prop; //表单变量
public:
	virtual BOOL OnInitDialog();
};
