// MulWnd1.cpp : implementation file
//

#include "stdafx.h"
#include "EditEidolon.h"
#include "MemDC.h"
#include "Express.h"
#include "ExpPublic.h"
#include "Value.h"
#include "VarValue.h"
#include "GeneralHelper.h"
#include "RegMsg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditEidolon

#ifdef _AFXDLL
// UINT HX_USER_COMPILEDATA = ::RegisterWindowMessage(HS_COMPILE_DATA);
// UINT HX_USER_UPDATEDATA  = ::RegisterWindowMessage(HS_COMPILE_UPDATEDATA);
// UINT HX_USER_DRAGDATA    = ::RegisterWindowMessage(HS_COMPILE_DRAGDATA);
// UINT EXPRESS_HS_FIND_DATA_MSG = ::RegisterWindowMessage(HS_FIND_DATA_MSG);
// UINT HS_USER_STRATEGY = ::RegisterWindowMessage(_T("Strategy Trade Message"));
// UINT HS_USER_ZOOM		= ::RegisterWindowMessage(_T("Zoom In or Out"));	//add by lxqi 20090506	改变范围消息
#endif

IMPLEMENT_DYNCREATE(CEditEidolon, CWnd)

static CString g_strParam = "Param#";

//static CFont* CreateFont(char * cFontName,int nFontSize, int nFontStyle)
//{    
//	CFont *pNewFont = new CFont;
//	if (pNewFont == NULL)
//	{
//		return NULL;
//	}
//	if((cFontName == NULL) || cFontName[0] == '\0')
//	{
//		//pNewFont->CreateStockObject(DEVICE_DEFAULT_FONT);
//		//return pNewFont;
//		cFontName = "宋体";
//		nFontSize = 16;
//		nFontStyle = 0;
//	}
//	LOGFONT font;
//	memset(&font, 0, sizeof(font));
//	font.lfHeight = -nFontSize;
//	font.lfWeight = FW_NORMAL;
//	font.lfCharSet = DEFAULT_CHARSET;
//	strcpy(font.lfFaceName, cFontName);
//	if(!pNewFont->CreateFontIndirect(&font))
//	{
//		delete pNewFont;
//		pNewFont = NULL;
//	}
//	
//	return pNewFont;
//}

void CEditEidolon::DrawData::Draw(CDC* pDC)
{
	pDC->DrawText(m_strLeft,m_rcLeft,DT_SINGLELINE|DT_LEFT|DT_WORDBREAK);
	pDC->DrawText(m_strRight,m_rcRight,DT_SINGLELINE|DT_LEFT|DT_WORDBREAK);	
}

CEditEidolon::CEditEidolon()
{
	m_nRowCount = 0 ;
	m_nDrawHeight = 0;
	m_nRowHeight  = 0;
	m_nScrollPos  = 0;

	m_pArray = NULL;
	m_pOldParamVar = NULL;
	m_pParamVar   = NULL;
	m_bDraw		  = TRUE;
	m_cPrompt     = 0;

}

CEditEidolon::~CEditEidolon()
{
	DeleteData();
	DeleteSpinEdit();	
	m_ftChart.Free();
}


BEGIN_MESSAGE_MAP(CEditEidolon, CWnd)
	//{{AFX_MSG_MAP(CEditEidolon)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_SYSKEYDOWN()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()

	ON_REGISTERED_MESSAGE(HX_USER_UPDATEDATA, OnCompileUpdateData)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEditEidolon message handlers


void CEditEidolon::SetText(CStringArray* pArray,CMapVariabile* pParamVar,
						   BOOL bFocus /*= TRUE*/)
{
	BOOL bRefesh = ( !m_strOldText.IsEmpty() );
	m_strOldText.Empty();
	m_pArray = pArray;	
	if( m_pArray == NULL )
	{
		CreateSpinEdit(0);
		DeleteData();
		m_cPrompt = 1;
		Invalidate();
		return;
	}
	m_pParamVar = pParamVar;

	int iCount = ((pParamVar == NULL)?0:pParamVar->GetCount());
	CreateSpinEdit(iCount);

	if(iCount == 0)
	{
		DeleteData();
		m_cPrompt = 1;
		Invalidate();
		return;
	}

	m_cPrompt = 0;
	DeleteData();
	if(bRefesh)
	{
		Invalidate();
	}

	m_bDraw = FALSE;

	CRect rect,rcClient;
	GetClientRect(&rcClient);
	rcClient.right -= GetSystemMetrics(SM_CXHSCROLL) * 2;
	rcClient.OffsetRect(0,0-m_nScrollPos);

	rect = rcClient;

	CClientDC dc(this);
	CSize size;
	CFont* pCurFont = m_ftChart.m_pFont;
	if( pCurFont == NULL )
		pCurFont = this->GetFont();
	CFont* pFont = dc.SelectObject(pCurFont);

	int nCompParamLen = g_strParam.GetLength();
	m_nRowHeight = dc.GetTextExtent("A").cy;
	int nYDist = m_nRowHeight/3*2;

	CString strSorce,strTmp1,strTmp2;
	int nPos;
	int nY = 0;
	DrawData* pDrawData;
	int nRowCount = m_pArray->GetSize();
	m_nRowCount = 0;
	int nItem;
	int i;
	int nLen = 0;
	for(i = 0; i < nRowCount; i++)
	{
		strSorce.Format("%s",m_pArray->GetAt(i));
		nPos = strSorce.Find(g_strParam,0);
		if(nPos != -1)
		{
			strTmp1  = strSorce.Left(nPos);
			if( nLen < strTmp1.GetLength() ) 
				nLen = strTmp1.GetLength();
		}
	}

	CString strFormat;
	strFormat.Format("%%%is",nLen+5);
	for(i = 0; i < nRowCount; i++)
	{
		strSorce.Format("%s",m_pArray->GetAt(i));
		nPos = strSorce.Find(g_strParam,0);
		
		pDrawData = new DrawData();
		m_ayDrawData.Add(pDrawData);

		nY = rect.top + m_nRowHeight*m_nRowCount + nYDist*(m_nRowCount + 1);

		if(nPos != -1)
		{
			strTmp1   = strSorce.Left(nPos);
			strSorce  = strSorce.Mid(nPos);

			strTmp2.Format(strFormat,strTmp1);
			size = dc.GetTextExtent(strTmp2);
			pDrawData->m_strLeft.Format("%s",strTmp2);
			pDrawData->m_rcLeft.SetRect(rect.left,nY,rect.left + size.cx,nY + m_nRowHeight);
			
			strTmp2  = strSorce.Left(nCompParamLen);				
			if( !strTmp2.CompareNoCase(g_strParam) )
			{
				//yulx modify 参数的个数多于10个时strTmp2应该是两位数
				strTmp2 = strSorce.Mid(nCompParamLen,strSorce.Find('(')-nCompParamLen);
				//strTmp2 = strSorce.GetAt(nCompParamLen);
				nItem = atoi(strTmp2);
				pDrawData->m_pEdit = GetSpinEdit(nItem-1);
				if(pDrawData->m_pEdit != NULL)
				{
// 					pDrawData->m_pEdit->SetEditDef(pParamVar->GetAt(nItem-1));
// 					pDrawData->m_pEdit->OffSet(pDrawData->m_rcLeft.right,pDrawData->m_rcLeft.top-m_szEdit.cy/4);
				}
			}
			//yulx modify 参数的个数多于10个时应该是+2
			strTmp1 = strSorce.Mid(strSorce.Find('('));
			//strTmp1 = strSorce.Mid(nCompParamLen+1);

			size = dc.GetTextExtent(strTmp1);
			pDrawData->m_strRight.Format("%s",strTmp1);
			pDrawData->m_rcRight.SetRect(pDrawData->m_rcLeft.right + m_szEdit.cx,
				nY,pDrawData->m_rcLeft.right + m_szEdit.cx + size.cx,nY+m_nRowHeight);
		}
		else
		{
			size = dc.GetTextExtent(strSorce);
			pDrawData->m_strLeft.Format("%s",strSorce);
			pDrawData->m_rcLeft.SetRect(rect.left,nY,rect.left + size.cx,nY + m_nRowHeight);
		}
		m_nRowCount++;
	}

	dc.SelectObject(pFont);
	
	m_bDraw = TRUE;

	m_nDrawHeight = m_nRowHeight*m_nRowCount + nYDist*(m_nRowCount + 1);
	long lHeight = m_nDrawHeight + m_nRowHeight;
	if(lHeight > rcClient.Height())
	{
		SetScrollRange(0,m_nDrawHeight,false);
		SetScrollPos(SB_VERT,m_nScrollPos);
		ShowScrollBar(SB_VERT,TRUE);
	}
	else
	{
		ShowScrollBar(SB_VERT,FALSE);
		m_nScrollPos = 0;
	}

	if( bFocus )
	{
		CWnd* pWnd = GetSpinEdit(0);
		if(pWnd != NULL)
		{
			pWnd->SetFocus();
		}
	}
	//Invalidate();
}

void CEditEidolon::SaveOldPar(CMapVariabile *pmap)
{
	if (!m_pOldParamVar)
	{
		m_pOldParamVar = new CMapVariabile;
	}
	CString key;
	CValue *cvalue;
	POSITION pos;
	pos = pmap->GetStartPosition();
	while(pos)
	{
		pmap->GetNextAssoc(pos,key,cvalue);
		CNumericValue *pnum = new CNumericValue;
		double db = cvalue->GetValue();
		pnum->SetValue(db);
		m_pOldParamVar->SetAt(key,pnum);
	}
}

void CEditEidolon::ResetParam()
{
	if (!m_pOldParamVar)
	{
		return;
	}
	CString key;
	CValue *cvalue;
	POSITION pos;
	pos = m_pOldParamVar->GetStartPosition();
	while(pos)
	{
		m_pOldParamVar->GetNextAssoc(pos,key,cvalue);
		double db = cvalue->GetValue();
		CValue *pnew = NULL;
		if (m_pParamVar->Lookup(key,pnew))
		{
			((CNumericValue *)pnew)->SetValue(db);
		}
	}
	this->SendMessage(HX_USER_UPDATEDATA,0,0);
	pos = m_pOldParamVar->GetStartPosition();
	while(pos)
	{
		m_pOldParamVar->GetNextAssoc(pos,key,cvalue);
		delete (CNumericValue*)cvalue;
	}
	m_pOldParamVar->RemoveAll();
	m_pOldParamVar = NULL;
}

void CEditEidolon::SetText(CString strRefText,CMapVariabile* pParamVar,
						   BOOL bFocus /*= TRUE*/)
{
	BOOL bRefesh = (m_pArray != NULL);
	m_pArray = NULL;
	
	CString strSorce;
	if( m_strOldText.CompareNoCase(strRefText) )
	{		
		strSorce.Format("%s",strRefText);
		CStringArray ayRet;
		int nCount = AnalyzeStr(strSorce,"\n\r",ayRet);
		strSorce.Empty();
		for(int i = 0; i < nCount; i++)
		{
			strSorce += ayRet.GetAt(i);
		}
		m_strOldText.Format("%s",strSorce);
	}
	else
	{
		strSorce.Format("%s",strRefText);
	}

	if(strSorce.IsEmpty())
	{
		CreateSpinEdit(0);
		DeleteData();
		m_cPrompt = 1;
		Invalidate();
		return;
	}

	m_pParamVar = pParamVar;
	SaveOldPar(pParamVar);
	int iCount = ((pParamVar == NULL)?0:pParamVar->GetCount());
	CreateSpinEdit(iCount);

	if(iCount == 0)
	{
		DeleteData();
		m_cPrompt = 1;
		Invalidate();
		return;
	}
	if(bRefesh)
	{
		Invalidate();
	}

	m_cPrompt = 0;
	DeleteData();

	m_bDraw = FALSE;

	int nLen = strSorce.GetLength();
	CRect rect,rcClient;
	GetClientRect(&rcClient);
	rcClient.right -= GetSystemMetrics(SM_CXHSCROLL) * 2;
//	m_nScrollPos = GetScrollPos(SB_VERT);

	rcClient.OffsetRect(0,0-m_nScrollPos);

	rect = rcClient;
	CClientDC dc(this);
	CSize size;
	CFont* pCurFont = m_ftChart.m_pFont;
	if( pCurFont == NULL )
		pCurFont = this->GetFont();
	CFont* pFont = dc.SelectObject(pCurFont);
	
	int nCompParamLen = g_strParam.GetLength();
	int nPos = 0;
	CString strTmp1;
	CString strTmp2;
	CString pstrLeft;
	CString strAdjust;
	DrawData* pDrawData = NULL;
	m_nRowCount = 0;
	int nY = 0,nX = 0;
	int nWidth;
	int nChars;
	int nItem = 0;
	m_nRowHeight = dc.GetTextExtent("A").cy;
	int nYDist = m_nRowHeight/3*2;
	while(nPos != -1)
	{
		nPos = strSorce.Find(g_strParam,0);
		if(nPos != -1)
		{
			if(nX >= rcClient.right) // 下一行
			{
				nX = 0;
				m_nRowCount++;
			}

			strTmp1   = strSorce.Left(nPos);
			strSorce  = strSorce.Mid(nPos);
			
			pDrawData = new DrawData();
			m_ayDrawData.Add(pDrawData);

			size = dc.GetTextExtent(strTmp1);

			nY = rect.top + m_nRowHeight*m_nRowCount + nYDist*(m_nRowCount + 1);

			nWidth = nX + rect.left + size.cx + m_szEdit.cx - rcClient.right;
			if( nWidth > 0 && !strTmp1.IsEmpty() )
			{
				nChars = strTmp1.GetLength();
				nWidth = nWidth / ( size.cx / nChars );
				nChars = nChars - nWidth - 1;
				
				if(strTmp1.GetLength() > nCompParamLen)
					nChars += nCompParamLen;
				if(nChars > strTmp1.GetLength())
					nChars = strTmp1.GetLength();
				
				if(nChars <= 0)
				{
					nChars = 2;
				}
				strAdjust = ::AdjustStr(strTmp1,nChars,&pstrLeft);
				strSorce  = pstrLeft + strSorce; // 
				
				size = dc.GetTextExtent(strAdjust);
				pDrawData->m_strLeft.Format("%s",strAdjust);
				pDrawData->m_rcLeft.SetRect(rect.left,nY,rect.left + size.cx,nY + m_nRowHeight);

				m_nRowCount++;
				nX = 0;
			}
			else
			{
				if(nWidth > 0)
				{
					m_nRowCount++;
					nX = 0;
					nY = rect.top + m_nRowHeight*m_nRowCount + nYDist*(m_nRowCount + 1);
				}

				pDrawData->m_strLeft.Format("%s",strTmp1);
				pDrawData->m_rcLeft.SetRect(nX + rect.left,nY,nX + rect.left + size.cx,nY + m_nRowHeight);

				strTmp2  = strSorce.Left(nCompParamLen);				
				if( !strTmp2.CompareNoCase(g_strParam) )
				{
					strTmp2 = strSorce.GetAt(nCompParamLen);
					nItem = atoi(strTmp2);
					pDrawData->m_pEdit = GetSpinEdit(nItem-1);
					CNumericValue* pValue = (CNumericValue*)(pParamVar->GetAt(nItem-1));
					if(pDrawData->m_pEdit != NULL)
					{
						pDrawData->m_pEdit->SetEditDef(pParamVar->GetAt(nItem-1));
						pDrawData->m_pEdit->OffSet(pDrawData->m_rcLeft.right,pDrawData->m_rcLeft.top-m_szEdit.cy/4);
					}
				}

				strSorce = strSorce.Mid(nCompParamLen+1);

				// 下一个
				nPos = strSorce.Find(g_strParam,0);
				if(nPos != -1)
				{
					strTmp1   = strSorce.Left(nPos);
					strSorce  = strSorce.Mid(nPos);
					
					size = dc.GetTextExtent(strTmp1);
					
					nWidth = pDrawData->m_rcLeft.right + m_szEdit.cx + size.cx - rcClient.right;
					if( nWidth > 0 && !strTmp1.IsEmpty() )
					{
						nChars = strTmp1.GetLength();
						nWidth = nWidth / ( size.cx / nChars );
						nChars = nChars - nWidth - 1;
						
						if(nWidth < nCompParamLen)
							nChars += nCompParamLen;
						if(nChars > strTmp1.GetLength())
							nChars = strTmp1.GetLength();
						
						if(nChars <= 2)
						{
							strSorce = strTmp1 + strSorce;
						}
						else
						{							
							strAdjust = AdjustStr(strTmp1,nChars,&pstrLeft);
							strSorce = pstrLeft + strSorce; // 
							
							size = dc.GetTextExtent(strAdjust);
							pDrawData->m_strRight.Format("%s",strAdjust);
							pDrawData->m_rcRight.SetRect(pDrawData->m_rcLeft.right + m_szEdit.cx,
								nY,pDrawData->m_rcLeft.right + m_szEdit.cx + size.cx,nY+m_nRowHeight);
						}
						
						nX = 0;
						m_nRowCount++;
					}
					else
					{
						if(nWidth > 0)
						{
							m_nRowCount++;
							nX = 0;
							nY = rect.top + m_nRowHeight*m_nRowCount + nYDist*(m_nRowCount + 1);
						}
						size = dc.GetTextExtent(strTmp1);
						pDrawData->m_strRight.Format("%s",strTmp1);
						pDrawData->m_rcRight.SetRect(pDrawData->m_rcLeft.right + m_szEdit.cx,
							nY,pDrawData->m_rcLeft.right + m_szEdit.cx + size.cx,nY+m_nRowHeight);
						
						nX += pDrawData->m_rcRight.right;
					}								
				}
				else
				{
					nX = pDrawData->m_rcLeft.right + m_szEdit.cx;
					if(nX >= rect.right)
					{
						nX = 0;
						m_nRowCount++;
					}
					break;
				}
			}
		}
	}

	while( !strSorce.IsEmpty() )
	{
		pDrawData = new DrawData();
		m_ayDrawData.Add(pDrawData);

		strTmp1.Format("%s",strSorce);
		size = dc.GetTextExtent(strTmp1);		

		nY = rect.top + m_nRowHeight*m_nRowCount + nYDist*(m_nRowCount + 1);

		nWidth = nX + size.cx - rcClient.right;
		if( nWidth > 0 )
		{
			nChars = strTmp1.GetLength();
			nWidth = nWidth / ( size.cx / nChars );
			nChars = nChars - nWidth - 1;
			if(nChars <= 2)
				nChars = 4;
			strAdjust = AdjustStr(strTmp1,nChars,&pstrLeft);
			strSorce = pstrLeft; // 
			
			size = dc.GetTextExtent(strAdjust);
			pDrawData->m_strRight.Format("%s",strAdjust);
			pDrawData->m_rcRight.SetRect(nX,nY,nX + size.cx,nY+m_nRowHeight);

			nX = 0;
			m_nRowCount++;			
		}
		else
		{
			size = dc.GetTextExtent(strTmp1);
			pDrawData->m_strRight.Format("%s",strTmp1);
			pDrawData->m_rcRight.SetRect(nX,nY,nX + size.cx,nY+m_nRowHeight);
			break;
		}
	}

	dc.SelectObject(pFont);

	m_bDraw = TRUE;

	m_nDrawHeight = m_nRowHeight*m_nRowCount + nYDist*(m_nRowCount + 1);
	long lHeight = m_nDrawHeight + m_nRowHeight;
	if(lHeight > rcClient.Height())
	{
		SetScrollRange(0,m_nDrawHeight,false);
		SetScrollPos(SB_VERT,m_nScrollPos);
		ShowScrollBar(SB_VERT,TRUE);
	}
	else
	{
		ShowScrollBar(SB_VERT,FALSE);
		m_nScrollPos = 0;
	}

	if( bFocus )
	{
		CWnd* pWnd = GetSpinEdit(0);
		if(pWnd != NULL)
		{
			pWnd->SetFocus();
		}
	}

	//Invalidate();
}

int CEditEidolon::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetFont("GB321",14,0);

	return 0;
}

void CEditEidolon::SetFont(CFont* pFont, BOOL bRedraw)
{
	if( pFont )
	{
		LOGFONT logfont;
		pFont->GetLogFont(&logfont);

		if( m_ftChart.m_pFont )
		{
			delete m_ftChart.m_pFont;
			m_ftChart.m_pFont = NULL;
		}
		CFont* pNewFont = new CFont;
		if( pNewFont->CreateFontIndirect(&logfont) )
		{
			m_ftChart.m_pFont = pNewFont;
		}
		else
		{
			delete pNewFont;
			pNewFont = NULL;
		}

		if( pNewFont )
		{
			CClientDC dc(this);
			CFont* pOldFont = dc.SelectObject(m_ftChart.m_pFont);
			m_szEdit = dc.GetTextExtent(g_strParam);
			m_szEdit.cx += m_szEdit.cx/2;
			dc.SelectObject(pOldFont);
		}
	}

	CWnd::SetFont( pFont,  bRedraw);
}

void CEditEidolon::SetFont(char * cFontName,int nFontSize, int nFontStyle)
{
	if( m_ftChart.m_pFont )
	{
		delete m_ftChart.m_pFont;
		m_ftChart.m_pFont = NULL;
	}

	CFont* pCurFont = m_ftChart.m_pFont;
	if( pCurFont == NULL )
	{
		m_ftChart.m_pFont = CGeneralHelper::CreateFont(cFontName,nFontSize, nFontStyle);//CHSTools::CreateFont("GB321",14,0);//CreateFont("GB321",14,0);
	}

	CWnd::SetFont( m_ftChart.m_pFont );

	CClientDC dc(this);
	if(pCurFont == NULL)
		pCurFont = this->GetFont();
	CFont* pFont = dc.SelectObject(pCurFont);
	m_szEdit = dc.GetTextExtent(g_strParam);
	m_szEdit.cx += m_szEdit.cx/2;
	dc.SelectObject(pFont);
}

void CEditEidolon::OnPaint() 
{
	/*
	const MSG *msg = GetCurrentMessage();
	DefWindowProc( msg->message, msg->wParam, msg->lParam );
	CClientDC cdc(this); // device context for painting

	CDC* pDC = &cdc;
	CBaseDC  dc;
	CBaseDC  pDCEx(pDC);
	CBaseDC* pDrawDC = pDCEx;
	CBitmap  bitmap;
	CBitmap* pOldBitmap;

	CRect rcClient,client,rect;
	GetClientRect(rcClient);
	pDCEx->GetClipBox(client);
	rect.IntersectRect(rcClient,client);
	if( (rect.left >= rect.right) || 
		(rect.top  >= rect.bottom) )
	{
		return;
	}

	client.InflateRect(5,5,5,5);
	rect = client;

	// draw to offscreen bitmap for fast looking repaints
	if (dc.CreateCompatibleDC(pDCEx))
	{
		if (bitmap.CreateCompatibleBitmap(pDCEx, rect.Width(), rect.Height()))
		{
			pDrawDC = &dc;
			
			// offset origin more because bitmap is just piece of the whole drawing
			dc.OffsetViewportOrg(-rect.left, -rect.top);
			pOldBitmap = dc.SelectObject(&bitmap);
			dc.SetBrushOrg(rect.left % 8, rect.top % 8);
			
			// might as well clip to the same rectangle
			dc.IntersectClipRect(client);
		}
	}

	int nPreMode = pDrawDC->SetBkMode(TRANSPARENT);

	CFont* pFont = pDrawDC->SelectObject(g_hxData.ftChart.m_pFont);	
	CBrush brush;
	if ( !brush.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)) )
		return;
	brush.UnrealizeObject();
	
	pDrawDC->FillRect(client, &brush);
	brush.DeleteObject();
	
	if(m_bDraw)
	{
		DrawData* pData;
		for(int i = 0; i < m_ayDrawData.GetSize(); i++)
		{
			pData = m_ayDrawData.GetAt(i);
			pData->Draw(pDrawDC);
		}		
	}
	pDrawDC->SelectObject(pFont);

	pDrawDC->SetBkMode(nPreMode);

	if ( pDrawDC != pDCEx  )
	{
		pDCEx->SetViewportOrg(0, 0);
		pDCEx->SetWindowOrg(0,0);
		pDCEx->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);

		pDCEx->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBitmap);
		dc.DeleteDC();
		bitmap.DeleteObject();
	}
	*/

	CPaintDC memdc(this);
	//CClientDC memdc(this);
	//CMemDC memdc(&cdc);
	CFont* pCurFont = m_ftChart.m_pFont;
	if( pCurFont == NULL )
		pCurFont = this->GetFont();
	CFont* pFont = memdc.SelectObject(pCurFont);
	
	CBrush brush;
	COLORREF color = RGB(255,255,255);
	if ( !brush.CreateSolidBrush(color) )
		return;
	brush.UnrealizeObject();
	
	CRect client;
	GetClientRect(&client);
	
	memdc.FillRect(client, &brush);
	brush.DeleteObject();
	
	memdc.SetBkMode(TRANSPARENT);
	color = RGB(0,0,0);
	memdc.SetTextColor(color);
	if(m_bDraw)
	{
		DrawData* pData;
		for(int i = 0; i < m_ayDrawData.GetSize(); i++)
		{
			pData = m_ayDrawData.GetAt(i);
			pData->Draw(&memdc);
		}

		if(m_cPrompt)
		{
			CSize size = memdc.GetTextExtent("无参数配置！");
			memdc.TextOut(client.left+(client.Width() - size.cx)/2,client.top+(client.Height() - size.cy)/2,"无参数配置！");
		}
	}

	memdc.SelectObject(pFont);
	
	// Do not call CWnd::OnPaint() for painting messages
}

void CEditEidolon::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(pScrollBar != NULL)
		return;

	CRect rect;
	GetClientRect(&rect);
	int iSub = m_nDrawHeight;// - rect.Height();
	BOOL bScroll = TRUE;
	switch(nSBCode)
	{
	case SB_PAGEDOWN:
		{
			if(m_nScrollPos == iSub)
			{
				SetScrollPos(SB_VERT,m_nScrollPos);
				return;
			}
			m_nScrollPos += rect.Height();
			if(m_nScrollPos > iSub)
			{
				m_nScrollPos = iSub;
			}
			SetScrollPos(SB_VERT,m_nScrollPos);
			break;
		}
	case SB_LINEDOWN:
		{
			if(m_nScrollPos == iSub)
			{
				SetScrollPos(SB_VERT,m_nScrollPos);
				return;
			}
			m_nScrollPos += m_nRowHeight;
			if(m_nScrollPos > iSub - m_nRowHeight)
			{
				m_nScrollPos = iSub;
			}
			SetScrollPos(SB_VERT,m_nScrollPos);		
			break;
		}
	case SB_PAGEUP:
		{
			if(m_nScrollPos == 0 )
			{
				SetScrollPos(SB_VERT,m_nScrollPos);
				return;
			}
			m_nScrollPos -= rect.Height();
			if(m_nScrollPos < 0)
			{
				m_nScrollPos = 0;
			}
			SetScrollPos(SB_VERT,m_nScrollPos);	
			break;
		}
	case SB_LINEUP:
		{
			if(m_nScrollPos == 0)
			{
				SetScrollPos(SB_VERT,m_nScrollPos);
				return;
			}
			m_nScrollPos -= m_nRowHeight;
			if(m_nScrollPos < m_nRowHeight)
			{
				m_nScrollPos = 0;
			}
			SetScrollPos(SB_VERT,m_nScrollPos);				
			break;
		}
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		{
			if((int)nPos == m_nScrollPos)
				return;
			m_nScrollPos = (int)nPos;
			SetScrollPos(SB_VERT,(int)nPos);
			break;
		}
	default:
		bScroll = FALSE;
		break;
	}

	if(bScroll)
	{
		if(m_pArray != NULL)
		{
			SetText(m_pArray,m_pParamVar);
		}
		else
		{
			SetText(m_strOldText,m_pParamVar);
		}
		Invalidate();
	}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CEditEidolon::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	/*
	if( nChar == VK_TAB)
	{
		CSpinEditEx* pSpinEdit = GetSpinEdit(0);
		if(pSpinEdit != NULL)
		{
			pSpinEdit->m_Edit.SetFocus();
		}
	}
	*/
	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CEditEidolon::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CEditEidolon::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && 
		(pMsg->wParam == VK_TAB || pMsg->wParam == VK_RETURN) )
	{
		int nCount = m_payEditCtrl.GetSize();
		if(nCount > 0)
		{
			int iFocus;
			CWnd* pwndFocus = this->GetFocus();
			for(iFocus = 0;iFocus < nCount;iFocus++)
			{
				CSpinEditEx* pSpinEdit = GetSpinEdit(iFocus);
				if( pSpinEdit != NULL && 
					pSpinEdit->IsWindowVisible() &&
					(CWnd*)pSpinEdit == pwndFocus )
					break;
			}
			if( ++iFocus >= nCount )
			{
				iFocus = 0;
			}
			CSpinEditEx* pSpinEdit = GetSpinEdit(iFocus);
			if( pSpinEdit != NULL )
			{
				pSpinEdit->SetFocus();
				//MakeVisible();
			}
			return true;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CEditEidolon::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	static BOOL bSize = FALSE;
	if( !bSize && m_pParamVar != NULL)
	{
		bSize = TRUE;
		if(m_pArray != NULL)
		{
			SetText(m_pArray,m_pParamVar);
		}
		else
		{
			SetText(m_strOldText,m_pParamVar);
		}
		bSize = FALSE;
	}
}

void CEditEidolon::Empty()
{
	m_pParamVar = NULL;
	m_strOldText.Empty();
}

void CEditEidolon::CreateSpinEdit(int nCount)
{
	int nSize = m_payEditCtrl.GetSize();
	int i;
	CSpinEditEx* pSpinEdit;
	if(nCount > nSize)
	{
		for(i = nSize; i < nCount;i++)
		{
			pSpinEdit = new CSpinEditEx;
			CRect rect(0,0,m_szEdit.cx,m_szEdit.cy + 4/*+m_szEdit.cy/2 - 4*/);
			pSpinEdit->Create(this,rect,i);
			pSpinEdit->SetFont(m_ftChart.m_pFont);
			m_payEditCtrl.Add(pSpinEdit);
		}
	}
	else
	{
		while(nSize > 0 && nCount < nSize)
		{
			nSize--;
			delete m_payEditCtrl.GetAt(nSize);
			m_payEditCtrl.RemoveAt(nSize);		
		}
	}
	for(i = 0; i < nCount;i++)
	{
		pSpinEdit = m_payEditCtrl.GetAt(i);
		pSpinEdit->ShowWindow(SW_HIDE);
	}
}

void CEditEidolon::DeleteSpinEdit()
{
	for(int i = 0; i < m_payEditCtrl.GetSize();i++)
	{
		delete m_payEditCtrl.GetAt(i);
	}
	m_payEditCtrl.RemoveAll();
}

CSpinEditEx* CEditEidolon::GetSpinEdit(int nPos)
{
	if(nPos >= 0 && nPos < m_payEditCtrl.GetSize())
	{
		return m_payEditCtrl.GetAt(nPos);
	}
	return NULL;
}

LRESULT CEditEidolon::OnCompileUpdateData(WPARAM wp,LPARAM lp)
{
	if(GetParent() != NULL)
	{
		return GetParent()->SendMessage(HX_USER_UPDATEDATA,wp,lp);
	}
	return 0;
}

void CEditEidolon::DeleteData()
{
	m_bDraw = FALSE;
	for(int i = 0; i < m_ayDrawData.GetSize(); i++)
	{
		delete m_ayDrawData.GetAt(i);
	}
	m_ayDrawData.RemoveAll();
	m_bDraw = TRUE;
}

BOOL CEditEidolon::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CEditEidolon::MoveWindow(int x, int y, int nWidth, int nHeight,
				BOOL bRepaint /*= TRUE*/)
{

	CWnd::MoveWindow(x,y,nWidth,nHeight,FALSE);

	CRect rect(x,y,x+nWidth,y+nHeight);
	this->InvalidateRect(rect);

	CWnd* pWnd = this->GetParent();
	if( pWnd != NULL )
	{
		pWnd->GetClientRect(rect);
		pWnd->InvalidateRect(rect);
	}
	/*
	int cx = ::GetSystemMetrics(SM_CXEDGE);
	int cy = ::GetSystemMetrics(SM_CYEDGE);
	rect.SetRect(x,y,x+cx,y+cy);
	InvalidateRect(rect);
	rect.SetRect(x,y,x+cx,y+cy);
	InvalidateRect(rect);
	*/
}

void CEditEidolon::MoveWindow(LPCRECT lpRect, BOOL bRepaint /*= TRUE*/)
{
	MoveWindow( lpRect->left, lpRect->top, abs(lpRect->right - lpRect->left),
			    abs(lpRect->bottom - lpRect->top),bRepaint );
}

void CEditEidolon::MakeVisible() // ???
{
	CWnd* pWnd = this->GetFocus();
	if( pWnd != NULL )
	{
		CEditEidolon::DrawData* pData = GetDrawData(pWnd);
		if(pData == NULL)
			return;

		CRect rect,rcClient,rc;
		pWnd->GetClientRect(rect);
		this->ClientToScreen(rect);
		rc = rect;
		this->ScreenToClient(rc);
		this->GetWindowRect(rcClient);
		if( !rcClient.PtInRect(rect.TopLeft()) ||
			!rcClient.PtInRect(CPoint(rect.top,rect.right)) ||
			!rcClient.PtInRect(CPoint(rect.left,rect.bottom)) ||
			!rcClient.PtInRect(rect.BottomRight()) )
		{			
			m_nScrollPos = pData->m_rcLeft.top + 4;
			if(m_pArray != NULL)
			{
				SetText(m_pArray,m_pParamVar);
			}
			else
			{
				SetText(m_strOldText,m_pParamVar);
			}
		}
	}
}

CEditEidolon::DrawData* CEditEidolon::GetDrawData(CWnd* pWnd)
{
	if(pWnd == NULL)
		return NULL;
	DrawData* pData;
	for(int i = 0; i < m_ayDrawData.GetSize(); i++)
	{
		pData = m_ayDrawData.GetAt(i);
		if( pData->m_pEdit != NULL &&
			pData->m_pEdit== pWnd)
		{
			return pData;
		}
	}
	return NULL;
}