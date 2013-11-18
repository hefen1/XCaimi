/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgIntelligentSelStock.h
*	文件标识：	智能选股类,目前不在使用,不做说明
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#pragma once
#include "afxcmn.h"
#include <afxwin.h>
#include "ExpPublic.h"
#include "resource.h"
//#include "..\HSControl\GridSelStockResult.h"

// CDlgIntelligentSelStock dialog

class CDlgIntelligentSelStock : public CDialog
{
	DECLARE_DYNAMIC(CDlgIntelligentSelStock)

public:
	CDlgIntelligentSelStock(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL,
		long* pThis = NULL);   // standard constructor
	virtual ~CDlgIntelligentSelStock();

// Dialog Data
	enum { IDD = IDD_INTELLIGENT_SELECT };

	CTabCtrl m_wndTab;
	CListCtrl m_wndCompList;
	CStatic m_wndSelectText;
	CEdit m_wndNotes;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	virtual void OnCancel();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

	LRESULT OnExchangeData(WPARAM wp,LPARAM lp);
public:
	virtual BOOL OnInitDialog();
	virtual void MyNcDestroy();
	BOOL ShowWindow( int nCmdShow );
	
protected:
	HSOpenExpressUseData* m_pData;
	CImageList  m_imgList;
//	CGridSelStockResult* m_pHSGridSelStockResult;
public:
	CStringArray m_ayGroupList;
	UnionSelCondition			  m_UnionSelCondition;			   // 组合
	SelStockCondition  m_curSelStockCondition;
public:
	StockUserInfo* CodeInfoToStockUserInfo(CodeInfo * pCodeInfo);
	
	CString GetTempletFileName(LPCTSTR pFileName);
	int GetAllSection(CStringArray& aySectionList);
	bool SaveLoad(LPCTSTR szFileName,CString strSection,
		CStringArray& ayKeyName, bool bSave = true);

/*	bool AddToIni(CNI_ReceiveData* pInfo,CString strTeam);*/
	bool ReadFromIni(CArray<SelStockResult*,
			SelStockResult*>& pSelectArray,CString strTeam);
	void LoadUnion();
	CString NameToExp(CString strName);
	void Delete();
	

	afx_msg void OnTabSelChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSaveAsBock();
	afx_msg void OnBnClickedUsernotes();
	CStatic m_wndRectGrid;
};
