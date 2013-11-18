#pragma once
#include "..\..\HelperLib\WinMgr.h"
#include "..\..\Common\HSBizDialog.h"

// CShareHolderList 对话框

class CShareHolderList: public CHSBizDialog
{
	DECLARE_DYNAMIC(CShareHolderList)
public:
	CShareHolderList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShareHolderList();

// 对话框数据
	enum { IDD = IDD_SHARTHOLDER };

protected:
	CWinMgr m_winMgr;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInit();
	virtual void OnHotKey(UINT nId);
	virtual void OnShow(BOOL bShow);

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void FillTestData();
};
