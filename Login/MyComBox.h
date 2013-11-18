/*******************************************************
  Դ��������: MyComBox.h
  �������Ȩ: �������ӹɷ����޹�˾
  ϵͳ����:   Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:   Login
  ����˵��:   ������ؼ�
  ��    ��:   ����
  ��������:   2010-11-16
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
	CString GetEditText();          // ��ȡ�༭���е��ı�
	void    ShowText();
	void    RemoveAllItem();        // �Ƴ�����Item��

	int     SetCurSel(int nSelect);
	int     AddString(LPCTSTR lpszString);
	void    ResetContent();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH  OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnDropDown();

	int     GetNextSel(int nDiff);   // ��ȡ��һ��������һ���±�
	void    RecalcText();
private:
	BOOL m_bTextUpdated;
	int  m_nCurIndex;
	// The subclassed COMBOLBOX window (notice the 'L')
	HWND m_hListBox;

	CStringArray  m_ayText;     // ����ԭʼ���ı�����
	CStringArray  m_ayHideText; // ��������֮����ı�����
	CSecureEdit   m_sEdit;

	// The string containing the text to display
	CString m_strText;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


