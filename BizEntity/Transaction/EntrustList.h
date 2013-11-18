#pragma once
#include "..\..\HelperLib\WinMgr.h"
#include "..\..\Common\HSBizDialog.h"

// CEntrustList 对话框

class CEntrustList : public CHSBizDialog
{
	DECLARE_DYNAMIC(CEntrustList)
public:
	CEntrustList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEntrustList();

	enum { IDD = IDD_ENTRUST_LIST };

protected:
	CWinMgr m_winMgr;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

	void OnShow(BOOL bShow);

public:
	virtual BOOL OnInit();
	virtual void OnHotKey(UINT nId);
	virtual void OnLinkageMsg(CLinkageParam* pParam);

	virtual BOOL OnInitDialog();
	void FillTestData();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
