#pragma once
#include "..\..\HelperLib\WinMgr.h"
#include "..\..\Common\HSBizDialog.h"

// CShareHolderList �Ի���

class CShareHolderList: public CHSBizDialog
{
	DECLARE_DYNAMIC(CShareHolderList)
public:
	CShareHolderList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShareHolderList();

// �Ի�������
	enum { IDD = IDD_SHARTHOLDER };

protected:
	CWinMgr m_winMgr;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInit();
	virtual void OnHotKey(UINT nId);
	virtual void OnShow(BOOL bShow);

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void FillTestData();
};
