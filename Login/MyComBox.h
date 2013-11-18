/*******************************************************
  源程序名称: MyComBox.h
  软件著作权: 恒生电子股份有限公司
  系统名称:   投资赢家金融理财终端V1.0
  模块名称:   Login
  功能说明:   下拉框控件
  作    者:   王超
  开发日期:   2010-11-16
*********************************************************/
#pragma once
#include "SecureEdit.h"
// CMyComBox

#define STR_DIVIDE   "-"
class CMyComBox : public CComboBox
{
	DECLARE_DYNAMIC(CMyComBox)

public:
	CMyComBox();
	virtual ~CMyComBox();
	
	void    SetHide(BOOL bIsHide);  
	CString GetEditText();          // 获取编辑框中的文本
	void    ShowText();
	void    RemoveAllItem();        // 移出所有Item项

	int     SetCurSel(int nSelect);
	int     AddString(LPCTSTR lpszString);
	void    ResetContent();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH  OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnDropDown();

	int     GetNextSel(int nDiff);   // 获取下一个或者上一个下标
	void    RecalcText();
private:
	BOOL m_bTextUpdated;
	int  m_nCurIndex;
	// The subclassed COMBOLBOX window (notice the 'L')
	HWND m_hListBox;

	CStringArray  m_ayText;     // 保存原始的文本内容
	CStringArray  m_ayHideText; // 保存隐藏之后的文本内容
	CSecureEdit   m_sEdit;

	// The string containing the text to display
	CString m_strText;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


