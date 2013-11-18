#include "stdafx.h"
#include "CHSStatic.h"
extern void DrawBKImageEx(CDC* pDC,int nLeft,int nTop,int nWidth,int nHeight,CString sBMPFile);

 
BEGIN_MESSAGE_MAP(CStaticTitle, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

CStaticTitle::CStaticTitle()
{
	m_TitleColor = RGB(0,0,0);
	m_strBKImage = "";
	m_strFrontImage = "";
}

BOOL CStaticTitle::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CStatic::OnEraseBkgnd(pDC);
}

void CStaticTitle::OnPaint()
{
	OutputDebugString("CStaticTitle::OnPaint()=====>\n");
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()

	CRect rectClient;
	GetClientRect(&rectClient);

	CString str;
	this->GetWindowText(str);

	CSize size;
	CFont* pFont = new CFont();
	VERIFY(pFont->CreateFont(16,0, 0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("宋体")));
	CFont* pOldFont = dc.SelectObject(pFont);

	CString strBkImage = m_strBKImage.IsEmpty()?"内容导航背景.bmp":m_strBKImage;
	DrawBKImageEx(&dc,rectClient.left,rectClient.top,rectClient.Width(),rectClient.Height(),strBkImage);
	if (!m_strFrontImage.IsEmpty())
	{
		GetTextExtentPoint32(dc.m_hDC,str.GetBuffer(),str.GetLength(),&size);
		DrawBKImageEx(&dc,rectClient.left,rectClient.top,size.cx + 10, 20,m_strFrontImage);
	}

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_TitleColor);
	rectClient.DeflateRect(7,0,0,0);
	dc.DrawText(str,-1,rectClient,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	dc.SelectObject(pOldFont);
	pFont->DeleteObject();
}

BOOL CStaticTitle::SetBkImage(CString strBKImage)
{
	if (strBKImage.IsEmpty())
	{
		return FALSE;
	} 
	else
	{
		m_strBKImage = strBKImage;
	}
	return TRUE;
}

BOOL CStaticTitle::SetFrontImage(CString strFrontImage)
{
	m_strFrontImage = strFrontImage;
	if (strFrontImage.IsEmpty())
	{
		return FALSE;
	} 
	else
	{
		return TRUE;
	}
}

BOOL CStaticTitle::SetTitleColor(COLORREF col)
{
	m_TitleColor = col;
	return TRUE;
}




BEGIN_MESSAGE_MAP(CQuoteButton, CMFCButton)
// 	ON_WM_ERASEBKGND()
// 	ON_WM_PAINT()
// 	ON_WM_DELETEITEM()
END_MESSAGE_MAP()

CQuoteButton::CQuoteButton():m_strBKImage("")
{
}

CQuoteButton::~CQuoteButton()
{

}
// BOOL CQuoteButton::OnEraseBkgnd(CDC* pDC)
// {
// 	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 
// 	return CButton::OnEraseBkgnd(pDC);
// }
// 
// void CQuoteButton::OnPaint()
// {
// 	OutputDebugString("CStaticTitle::OnPaint()=====>\n");
// 	CPaintDC dc(this); // device context for painting
// 	// TODO: 在此处添加消息处理程序代码
// 	// 不为绘图消息调用 CStatic::OnPaint()
// 
// 	CRect rectClient;
// 	GetClientRect(&rectClient);
// 	dc.SetBkMode(TRANSPARENT);
// 	CString strBkImage = m_strBKImage;
// 	DrawBKImageEx(&dc,rectClient.left,rectClient.top,rectClient.Width(),rectClient.Height(),strBkImage);
// }

BOOL CQuoteButton::SetBkImage(CString strBKImage)
{
	if (strBKImage.IsEmpty())
	{
		return FALSE;
	} 
	else
	{
		CImage img;
		img.Load(strBKImage);
		if( !img.IsNull() )
		{
			HBITMAP hBitmap = img.Detach();
			this->SetImage(hBitmap);
		}
	}
	return TRUE;
 }
// void CQuoteButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
// {
// 	return;
// }