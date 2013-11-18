#if !defined(AFX_GRADIENTPROGRESSCTRL_H__91ECB8BC_A7DA_42EB_B2C5_37419D31B753__INCLUDED_)
#define AFX_GRADIENTPROGRESSCTRL_H__91ECB8BC_A7DA_42EB_B2C5_37419D31B753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HSProgressCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHSProgressCtrl window

class CHSProgressCtrl : public CProgressCtrl
{
	// Construction
public:
	CHSProgressCtrl();

	//Set_function
	void SetTextColor(COLORREF color)	{m_clrText=color;}
	void SetBkColor(COLORREF color)		{m_clrBkGround=color;}
	void SetStartColor(COLORREF color)	{m_clrStart=color;}
	void SetEndColor(COLORREF color)	{m_clrEnd=color;}
	//Percent TXT showed in the Progress 
	void ShowPercent(BOOL bShowPercent = TRUE)	{m_bShowPercent=bShowPercent;}

	//Get_Function
	COLORREF GetTextColor(void)		{return m_clrText;}
	COLORREF GetBkColor(void)		{return m_clrBkGround;}
	COLORREF GetStartColor(void)	{return m_clrStart;}
	COLORREF GetEndColor(void)		{return m_clrEnd;}


	// Attributes
protected:
	float		m_fTransmited;
	COLORREF	m_clrStart,m_clrEnd,m_clrBkGround,m_clrText;
	BOOL		m_bShowPercent;
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHSProgressCtrl)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CHSProgressCtrl();

	// Generated message map functions
protected:
	void DrawGradient(CPaintDC *pDC,const RECT& rectClient,const int & nMaxWidth);
	//{{AFX_MSG(CHSProgressCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	int SetPos(float nPos);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADIENTPROGRESSCTRL_H__91ECB8BC_A7DA_42EB_B2C5_37419D31B753__INCLUDED_)
