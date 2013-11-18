#pragma once
#include "..\..\Common\HSBizDialog.h"
#include ".\MenuTree.h"
#include "..\..\Controls\CommControls\HSButton.h"
#include "TradeLangMngIntf.h"
#include "WinnerApplication.h"
#include "TradeAccountMngIntf.h"
#include "TradeCoreIntf.h"

typedef CTypedPtrArray<CPtrArray, CMenuTree*> CMenuTreeList;
typedef CTypedPtrArray<CPtrArray, CHSButton*> CButtonList;

class CAccountMenuTree
{
public:
	CAccountMenuTree();
	~CAccountMenuTree();
	void SetAccount(IAccount* pAccount){m_pAccount = pAccount;}
	IAccount* GetAccount(){return m_pAccount;}
	CMenuTreeList m_MenuTreeList;
	CButtonList	  m_ButtonList;
	CMenuTree*	m_pCurrMenuTree;
	CHSButton*  m_pCurrButton;
protected:
	IAccount* m_pAccount;
};
// CMenuTreeDlg 对话框
typedef CTypedPtrArray<CPtrArray, CAccountMenuTree*> CAccountMenuTreeList;

class CMenuTreeDlg : public CHSBizDialog, public IAccountChange
{
	DECLARE_DYNAMIC(CMenuTreeDlg)

public:
	CMenuTreeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMenuTreeDlg();

// 对话框数据
	enum { IDD = IDD_MENUTREE };
public:
	virtual void OnAccountChange(IAccount* lpValue, EAccountChange nChangeType);
	virtual void OnAccountStatus(IAccount* lpValue);
protected:
	CAccountMenuTreeList m_AccountMenuTreeList;
	CAccountMenuTree* m_CurrAccountMenuTree;
	CMenuTreeList m_MenuTreeList;
	CButtonList	m_ButtonList;
	CPoint m_lastClickPoint;
	CImageList m_ImageList;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInit();
	void OnShow(BOOL bShow);
	void OnLinkageMsg(CLinkageParam* pParam);

	void SendLingLayoutChangeMsg(const CString &strCaption);
	void OpenLayOut();
	void OpenLayOut(CString sLayOut, IHSParam* pParam = NULL);
	void OnSizeEx();
	void OnHotKey(UINT nId);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnNMClickMenuTree(UINT Id,NMHDR *pNMHDR, LRESULT *pResult);
	void OnMenuItemChange(UINT Id, NMHDR *pNMHDR, LRESULT *pResult);
	void OnNMSetfocusTree(UINT Id, NMHDR *pNMHDR, LRESULT *pResult);
	void OnNMKillfocusTree(UINT Id, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnButtonClicked(UINT Id); 
private:
	IAccountMng*		m_pAccountMng;
	ITradeCoreObj*		m_pTradeCoreObj;
	ITradeConfigObj*	m_pTradeConfigObj;
	int					m_nLastConIndex;
	void NewMenuTree(IAccount* pAccount);
};
