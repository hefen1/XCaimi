#pragma once

#include "tinyxml.h"
#define MENUID_BEGIN        1
#define SYS_MENUID_BEGIN    MENUID_BEGIN
#define SYS_MENUID_END      MENUID_BEGIN + 99
#define USER_MENUID_BEGIN   MENUID_BEGIN + 100 
#define MENUID_END          2000  


typedef void*(WINAPI *OnMenuMessage)(const char*);

class CMenuInfo
{
private:
	HMODULE         m_handle;
	OnMenuMessage   m_pOnMenuMsg;
public:
	int       m_nId;
	CString   m_strMsgId;
	CString   m_strCaption;
	CString   m_strModuleName;
	BOOL      m_bVisable;

   CMenuInfo()
   {
	   m_handle = NULL;
	   m_pOnMenuMsg = NULL;

	   m_nId = 0;
	   m_strModuleName = _T("");
	   m_strMsgId = _T("");
	   m_strCaption = _T(""); 
	   m_bVisable = TRUE;
   }

   void OnMenuMsg();	
};

// CHSMenuBar
class CHSTopButton: public CMFCButton
{
protected:
	virtual int GetImageHorzMargin() const;
	virtual int GetVertMargin() const;
	virtual void OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState);
public:
	void ChangeImg(CString strImgFile);
};

struct TopBtnInfo
{
	int m_nID;
	CString strCaption;
	CString strImg;

	TopBtnInfo()
	{
		m_nID = 0;
		strCaption.Empty();
		strImg.Empty();
	}
};

class CHSMenuBar : public CMFCMenuBar
{
	DECLARE_DYNAMIC(CHSMenuBar)

public:
	CHSMenuBar();
	virtual ~CHSMenuBar();
	void LoadMenu(CString strMenuFile);
	void OnMenuMsg(int nId);
	CString GetActionID(int nID);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void RemoveCaptionButtons();	
	void SetCaptionButtons();
	void AddNewCaptionButton(CString strCaption,CString strMapFile,UINT nID);
	void OnTopBtnClick(UINT nID);

private:
	int  m_nMenuIdSeed;
	CTypedPtrArray<CObArray, CHSTopButton*> m_arrButtons;	
	CTypedPtrArray<CPtrArray,TopBtnInfo*> m_arrExternalInfo;
	CTypedPtrMap<CMapWordToPtr,int,CMenuInfo*> m_menuInfoMap;

	BOOL LoadMenuItemInfo(TiXmlElement *pElem,CMenuInfo* pInfo);
	BOOL BuildMenu(CMenu* pMenu,TiXmlElement* pMenuElment);
	int  CreateMenuId();
};


