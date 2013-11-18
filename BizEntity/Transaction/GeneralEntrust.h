#pragma once
#include <afxbutton.h>
#include "..\..\Common\HSBizDialog.h"

// CGeneralEntrust 对话框

class CGeneralEntrust : public CHSBizDialog
{
	DECLARE_DYNAMIC(CGeneralEntrust)

public:
	CGeneralEntrust(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGeneralEntrust();

	enum { IDD = IDD_GENERAL_ENSTRUST };
	CEdit m_edStockCode;

	virtual BOOL OnInit();
	virtual void OnHotKey(UINT nId);
	virtual void OnWindowFocused();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnEntrust();
public:
	CButton m_btnEntrust;
	virtual BOOL OnInitDialog();
};
