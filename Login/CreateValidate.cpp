#include "stdafx.h"
#include "CreateValidate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCreateValidate::CCreateValidate(void)
{

	memset(&m_logFont, 0, sizeof(LOGFONT));   
	m_logFont.lfHeight = 20;     
	m_logFont.lfWidth = 15;
	lstrcpy(m_logFont.lfFaceName, _T("新宋体"));  

	m_FontArray[0] = "Arial";
	m_FontArray[1] = "Arial Narrow";
	m_FontArray[2] = "Batang";
	m_FontArray[3] = "BatangChe";
	m_FontArray[4] = "Book Antiqua";
	m_FontArray[5] = "Bookman Old Style";
	m_FontArray[6] = "Dotum";
	m_FontArray[7] = "Garamond";
	m_FontArray[8] = "Gulim";
	m_FontArray[9] = "Kartika";
	m_FontArray[10] = "Monotype Corsiva";
	m_FontArray[11] = "System";
	m_FontArray[12] = "方正姚体";
	m_FontArray[13] = "仿宋_GB2312";
	m_FontArray[14] = "黑体";
	m_FontArray[15] = "华文新魏";
	m_FontArray[16] = "华文行楷";
	m_FontArray[17] = "隶书";
	m_FontArray[18] = "楷体_GB2312";
	m_FontArray[19] = "新宋体";
	m_FontArray[20] = "幼圆";
	m_FontArray[21] = "Lucida Console";
	m_FontArray[22] =  "Monotype Corsiva"; 

	m_Color[0] = RGB(255,0,0);
	m_Color[1] = RGB(0,0,0);
	m_Color[2] = RGB(0,0,255);
	m_Color[3] = RGB(255,0,128);
	m_Color[4] = RGB(128,0,0);
	m_Color[5] = RGB(255,0,255);
	m_Color[6] = RGB(0,128,255);
	m_Color[7] = RGB(277,277,0);
	m_Color[8] = RGB(0,64,128);
	m_Color[9] = RGB(0,0,64);


}

CCreateValidate::~CCreateValidate(void)
{
}

HBITMAP CCreateValidate::CreateBMP( const HWND &hHwnd, const CString & strCode, const int nHeight, const int nWidth)
{
	HDC dc   =  ::GetDC(hHwnd);   
	HDC dcMem = ::CreateCompatibleDC(dc);

	HBITMAP hBitmap = ::CreateCompatibleBitmap(dc,nWidth,nHeight);   
	HBITMAP hOldBm  = (HBITMAP)::SelectObject(dcMem, hBitmap); 

	CDC* pDC = CDC::FromHandle(dcMem);
	if(pDC)
	{
		pDC->FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));
	}

	SetBkMode(dcMem, TRANSPARENT);
	FontDisturb(nHeight,nWidth);
	HFONT hFont = CreateFontIndirect(&m_logFont); 
	SelectObject(dcMem, hFont);

	ColorDisturb();
	SetTextColor(dcMem, ColorDisturb());

	CRect rect;
	rect.left = rect.top = 0;
	rect.right = rect.left + nWidth;
	rect.bottom = rect.top + nHeight;
	DrawText(dcMem,strCode,strCode.GetLength(),rect,DT_VCENTER|DT_SINGLELINE);
	//TextOut(dcMem,0,0,strCode,strCode.GetLength());

	::DeleteDC(dcMem);  
	::ReleaseDC(hHwnd, dc);
	::DeleteObject(hFont);

	return hBitmap;
}

//让字体变化以造成干扰
void CCreateValidate::FontDisturb(const int nHeight, const int nWidth )
{
	//字体随机
	lstrcpy(m_logFont.lfFaceName,_T(m_FontArray[rand()%22]));   

	//字体大小随机
	m_logFont.lfHeight = (int)(nHeight*0.5) + (rand()%((int)(nHeight*0.4)));     
	m_logFont.lfWidth  = (int)(nWidth*0.5) + (rand()%((int)(nWidth*0.4)));
	m_logFont.lfWeight = rand()%1000;//Specifies the weight of the font in the range 0 through 1000. For example, 400 is normal and 700 is bold. If this value is zero, a default weight is used. 
	m_logFont.lfItalic = (rand()%2 == 1)?TRUE:FALSE;
	m_logFont.lfEscapement = rand()%80;
}

COLORREF CCreateValidate::ColorDisturb()
{
	int i = rand()%10;
	return m_Color[i];

}