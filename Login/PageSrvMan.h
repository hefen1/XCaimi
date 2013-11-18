#pragma once
#include "afxcmn.h"
#include "NetEngineInterface.h"


// CPageSrvMan 对话框

class CPageSrvMan : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageSrvMan)

public:
	CPageSrvMan(Server_type nID = CEV_Connect_HQ, int m_nCellID = -1);
	virtual ~CPageSrvMan();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_SRVMAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	Server_type  m_nCurID;
	int          m_nCellID;
public:
	BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonDel();
	CListCtrl m_cListSrv;
	CImageList	 m_imageList;
	afx_msg void OnNMDblclkListSrv(NMHDR *pNMHDR, LRESULT *pResult);
private:
	void Init();
public:
	void SetDefault(int index);
	afx_msg void OnNMClickListSrv(NMHDR *pNMHDR, LRESULT *pResult);
};
