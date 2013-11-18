/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Juraj Rojko jrojko@twist.cz
// All rights reserved
//
// ScriptEditViewFormula.cpp : implementation of the CScriptEditViewFormula class
//

#include "stdafx.h"

#include "ScriptEditViewFormula.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptEditViewFormula

int GetStringCount(const char* pStr)
{
	if( pStr == NULL )
		return 0;

	int nCount = 0;
	int nPos = strlen(pStr);
	for(int i = 0; i < nPos; )
	{
		if( (BYTE)*pStr > 0x7f )
		{
			pStr += 2;
			i	 += 2;
		}
		else
		{
			pStr += 1;
			i    += 1;
		}

		nCount++;
	}
	return nCount;
}

IMPLEMENT_DYNCREATE(CScriptEditViewFormula, baseRichView)

BEGIN_MESSAGE_MAP(CScriptEditViewFormula, baseRichView)
	//{{AFX_MSG_MAP(CScriptEditViewFormula)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, baseRichView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, baseRichView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, baseRichView::OnFilePrintPreview)

	ON_NOTIFY_REFLECT(EN_PROTECTED, OnProtected)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptEditViewFormula construction/destruction

CScriptEditViewFormula::CScriptEditViewFormula()
{
	m_chComment = 1;
	m_bCaseSensitive = FALSE;
	m_bChangeCase = TRUE;

	SetStringQuotes(_T("\""));

	SetKeywordColor(RGB(0,0,255), FALSE);
	SetConstantColor(RGB(0,0,0), TRUE);
	SetCommentColor(RGB(0,128,0), FALSE);
	SetNumberColor(RGB(255,0,255), FALSE);
	SetStringColor(RGB(255,0,255), FALSE);

	m_icOtherText.clrColor = RGB(0,0,0);
	m_icOtherText.bBold    = FALSE;
	
	m_bInForcedChange = FALSE;
	m_changeType = ctUndo;
	m_crOldSel.cpMin = m_crOldSel.cpMax = 0;
}

CScriptEditViewFormula::~CScriptEditViewFormula()
{
}

BOOL CScriptEditViewFormula::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return baseRichView::PreCreateWindow(cs);
}


void CScriptEditViewFormula::OnInitialUpdate()
{
	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));

	baseRichView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CScriptEditViewFormula printing

BOOL CScriptEditViewFormula::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


/////////////////////////////////////////////////////////////////////////////
// CScriptEditViewFormula diagnostics

#ifdef _DEBUG
void CScriptEditViewFormula::AssertValid() const
{
	baseRichView::AssertValid();
}

void CScriptEditViewFormula::Dump(CDumpContext& dc) const
{
	baseRichView::Dump(dc);
}

CDocument* CScriptEditViewFormula::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));
	return (CDocument*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScriptEditViewFormula message handlers
void CScriptEditViewFormula::Initialize() 
{
	PARAFORMAT2 pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_TABSTOPS ;
	pf.cTabCount = MAX_TAB_STOPS;
	for( int itab = 0 ; itab < pf.cTabCount  ; itab++ )
		pf.rgxTabs[itab] = (itab + 1) * 1440/5 ;

	//PARAFORMAT2 ;
	SetParaFormat( pf );

	CHARFORMAT cfDefault;
	cfDefault.cbSize = sizeof(cfDefault);
	cfDefault.dwEffects = CFE_PROTECTED; 
	cfDefault.dwMask = CFM_BOLD | CFM_FACE | CFM_SIZE | CFM_CHARSET | CFM_PROTECTED;
	cfDefault.yHeight = 200;
	cfDefault.bCharSet = 0xEE; 
	strcpy(cfDefault.szFaceName, _T("Courier New")); 

	GetRichEditCtrl().SetDefaultCharFormat(cfDefault);
	
	GetRichEditCtrl().SetEventMask(ENM_CHANGE | ENM_SELCHANGE | ENM_PROTECTED);
}

void CScriptEditViewFormula::SetSLComment(TCHAR chComment, TCHAR chComment2)
{
	m_chComment  = '{';//chComment;
	m_chComment2 = '}';//chComment2;
}

void CScriptEditViewFormula::SetSLComment(LPCTSTR lpszComment)
{
	m_strComment = "//";//lpszComment;
}

void CScriptEditViewFormula::AddKeywords(LPCTSTR lpszKwd)
{
	m_strKeywords = m_strKeywords + lpszKwd;
	m_strKeywordsLower = m_strKeywords;
	if (!m_bCaseSensitive)
		m_strKeywordsLower.MakeLower();
}

void CScriptEditViewFormula::ClearKeywords()
{
	m_strKeywords.Empty();
	m_strKeywordsLower.Empty();
}							  

void CScriptEditViewFormula::AddConstants(LPCTSTR lpszConst)
{
	m_strConstants = m_strConstants + lpszConst;
	m_strConstantsLower = m_strConstants;
	if (!m_bCaseSensitive)
		m_strConstantsLower.MakeLower();
}

void CScriptEditViewFormula::ClearConstants()
{
	m_strConstants.Empty();
	m_strConstantsLower.Empty();
}							  

void CScriptEditViewFormula::SetCaseSensitive(BOOL bSensitive)
{
	m_bCaseSensitive = bSensitive;
}

void CScriptEditViewFormula::SetChangeCase(BOOL bChange)
{
	m_bChangeCase = bChange;
}

void CScriptEditViewFormula::SetStringQuotes(LPCTSTR lpszStrQ)
{
	m_strStringQuotes = lpszStrQ;
}

void CScriptEditViewFormula::SetKeywordColor(COLORREF clr, BOOL bBold)
{
	m_icKeyword.clrColor = clr;
	m_icKeyword.bBold = bBold;
}

void CScriptEditViewFormula::SetConstantColor(COLORREF clr, BOOL bBold)
{
	m_icConstant.clrColor = clr;
	m_icConstant.bBold = bBold;
}

void CScriptEditViewFormula::SetCommentColor(COLORREF clr, BOOL bBold)
{
	m_icComment.clrColor = clr;
	m_icComment.bBold = bBold;
}

void CScriptEditViewFormula::SetNumberColor(COLORREF clr, BOOL bBold)
{
	m_icNumber.clrColor = clr;
	m_icNumber.bBold = bBold;
}

void CScriptEditViewFormula::SetStringColor(COLORREF clr, BOOL bBold)
{
	m_icString.clrColor = clr;
	m_icString.bBold = bBold;
}
int CScriptEditViewFormula::IsKeyword(LPCTSTR lpszSymbol)
{
	CString strSymbol; strSymbol.Format(" %s ", lpszSymbol);
	if (!m_bCaseSensitive) 
		strSymbol.MakeLower();
				
	return m_strKeywordsLower.Find(strSymbol);
}

int CScriptEditViewFormula::IsConstant(LPCTSTR lpszSymbol)
{
	CString strSymbol; strSymbol.Format(" %s ", lpszSymbol);
	if (!m_bCaseSensitive) 
		strSymbol.MakeLower();
				
	return m_strConstantsLower.Find(strSymbol);
}

BOOL CScriptEditViewFormula::IsStringQuote(TCHAR ch)
{
	return (m_strStringQuotes.Find(ch) >= 0);
}


void CScriptEditViewFormula::SetFormatRange(int nStart, int nEnd, BOOL bBold, COLORREF clr)
{
	if (nStart >= nEnd)
		return;

	GetRichEditCtrl().SetSel(nStart, nEnd);

	DWORD dwEffects = bBold?CFE_BOLD:0;

	CHARFORMAT cfm;
	cfm.cbSize = sizeof(cfm);
    GetRichEditCtrl().GetSelectionCharFormat(cfm);
	
	if( (cfm.dwMask & CFM_COLOR)  && 
		(cfm.crTextColor == clr)  && 
		(cfm.dwMask & CFM_BOLD)   &&
		((cfm.dwEffects & CFE_BOLD) == dwEffects) )
		return;

	cfm.dwEffects	= dwEffects;
	cfm.crTextColor = clr;
	cfm.dwMask = CFM_BOLD | CFM_COLOR;

	GetRichEditCtrl().SetSelectionCharFormat(cfm);
}

void CScriptEditViewFormula::ChangeCase(int nStart, int nEnd, LPCTSTR lpsz)
{
	ASSERT((nEnd - nStart) == (int)_tcslen(lpsz));

	if (!m_bCaseSensitive && m_bChangeCase) {
		GetRichEditCtrl().SetSel(nStart, nEnd);
		GetRichEditCtrl().ReplaceSel(lpsz);
	}
}

void CScriptEditViewFormula::FormatTextRange(int nStart, int nEnd,BOOL bAll /*= FALSE*/)
{
	if (nStart >= nEnd)
	{
		OnEdit(0,0,"",TRUE);
		m_bInputChar = FALSE;
		return;
	}

	m_bInForcedChange = TRUE;

	CHARRANGE crOldSel;

	GetRichEditCtrl().GetSel(crOldSel);
	LockWindowUpdate();
	GetRichEditCtrl().HideSelection(TRUE, FALSE);

	CString strRet;

	TCHAR *pBuffer = NULL;
	
	try 
	{
		GetRichEditCtrl().SetSel(nStart, nEnd);

		CString strSel = GetRichEditCtrl().GetSelText();
		long nLen = strSel.GetLength();
		if( nLen <= 0 )
		{
			GetRichEditCtrl().SetSel(crOldSel);
			GetRichEditCtrl().HideSelection(FALSE, FALSE);
			UnlockWindowUpdate();
			
			m_bInForcedChange = FALSE;
			return;
		}

		pBuffer = new TCHAR[nLen + 1];
		memset(pBuffer,0,sizeof(char)*(nLen + 1));
		strncpy(pBuffer,strSel,sizeof(char)*nLen);

		//ASSERT(nLen <= nEnd - nStart);
		/*
		if( nLen <= nEnd - nStart - 2 )
		{
			GetRichEditCtrl().SetSel(crOldSel);
			GetRichEditCtrl().HideSelection(FALSE, FALSE);
			UnlockWindowUpdate();
			
			m_bInForcedChange = FALSE;
			delete[] pBuffer;
			return;
		}
		*/

		//pBuffer[nLen] = 0;
		
		if(m_bInputChar)
		{
			strRet = pBuffer;
		}
		
		TCHAR *pStart, *pPtr;
		pStart = pPtr = pBuffer;

		TCHAR* pSymbolStart = NULL;
		SymbolColor ic;

		int nFormatBegin;

		while (*pPtr != 0) 
		{
			TCHAR ch = *pPtr;

			if ( ch == '{' )  // ×¢ÊÍ {}
			{
				if( !bAll )
				{
					GetRichEditCtrl().SetSel(crOldSel);
					GetRichEditCtrl().HideSelection(FALSE, FALSE);
					UnlockWindowUpdate();
					m_bInForcedChange = FALSE;
					if( pBuffer != NULL )
					{
						delete [] pBuffer;
					}
					FormatTextRange(0,GetTextLength(),TRUE);
					return;
				}

				pSymbolStart = pPtr;
				do 
				{
					ch = *(++pPtr);
				}
				while (ch != 0 && ch != '}');
				if( strlen(pPtr) > 0 )
				{
					pPtr++;
				}
				ic = m_icComment;
			}
			else if ( (ch == '/') && (strlen(pPtr) > 0 && pPtr[1] == '*') )  // ×¢ÊÍ /**/
			{
				if( !bAll )
				{
					GetRichEditCtrl().SetSel(crOldSel);
					GetRichEditCtrl().HideSelection(FALSE, FALSE);
					UnlockWindowUpdate();
					m_bInForcedChange = FALSE;
					if( pBuffer != NULL )
					{
						delete [] pBuffer;
					}
					FormatTextRange(0,GetTextLength(),TRUE);
					return;
				}

				pSymbolStart = pPtr;
				do 
				{
					ch = *(++pPtr);
				}
				while ( ch != 0 && strlen(pPtr) > 0 && pPtr[1] != '*' && ch != '/' );
				while( strlen(pPtr) > 0 )
				{
					pPtr++;
				}
				ic = m_icComment;
			}
			else if ( (ch == '/') && (strlen(pPtr) > 0 && pPtr[1] == '/') )  // ×¢ÊÍ //
			{
				pSymbolStart = pPtr;
				do 
				{
					ch = *(++pPtr);
				}
				while (ch != 0 && ch != '\r');

				ic = m_icComment;
			}		
			else if (IsStringQuote(ch))   // ×Ö·û´® strings
			{ 
				pSymbolStart = pPtr;
				TCHAR ch1 = ch;
				do
				{
					ch = *(++pPtr);
				} 
				while (ch != 0 && ch != ch1 && ch != '\r');
				if (ch == ch1) pPtr++;
				ic = m_icString;
			} 
			else if (_istdigit(ch)) // Êý×Ö numbers
			{ 
				pSymbolStart = pPtr;
				_tcstod(pSymbolStart, &pPtr);
				ic = m_icNumber;
			} 
			else if (_istalpha(ch) || ch == '_') // ¹Ø¼ü×Ö/³£Á¿ keywords
			{ 
				pSymbolStart = pPtr;
				do
				{
					ch = *(++pPtr);
				}
				while (_istalnum(ch) || ch == '_');
				*pPtr = 0;
				int nPos = IsKeyword(pSymbolStart);
				if (nPos >= 0) 
				{
					nFormatBegin = GetStringCount(pBuffer);

					ChangeCase(nStart + nFormatBegin - GetStringCount(pSymbolStart), 
						nStart + nFormatBegin - GetStringCount(pPtr), 
						m_strKeywords.Mid(nPos+1, pPtr - pSymbolStart));
					
					ic = m_icKeyword;
				}
				else 
				{
					nPos = IsConstant(pSymbolStart);
					if (nPos >= 0) 
					{
						nFormatBegin = GetStringCount(pBuffer);

						ChangeCase(nStart + nFormatBegin - GetStringCount(pSymbolStart),
							nStart + nFormatBegin - GetStringCount(pPtr), 
							m_strConstants.Mid(nPos+1, pPtr - pSymbolStart));

						ic = m_icConstant;
					}
					else 
					{
						pSymbolStart = NULL;
					}
				}
				*pPtr = ch;
			} 
			else 
			{
				pPtr++;
			}

			if ( pSymbolStart != NULL ) 
			{
				ASSERT(pSymbolStart < pPtr);

				nFormatBegin = GetStringCount(pBuffer);

				SetFormatRange(nStart + nFormatBegin - GetStringCount(pStart), 
					nStart + nFormatBegin - GetStringCount(pSymbolStart), 
					m_icOtherText.bBold,m_icOtherText.clrColor);
				SetFormatRange(nStart + nFormatBegin - GetStringCount(pSymbolStart), 
					nStart + nFormatBegin - GetStringCount(pPtr), 
					ic.bBold, ic.clrColor);

				pStart = pPtr;
				pSymbolStart = 0;
			}
			else if (*pPtr == 0)
			{
				nFormatBegin = GetStringCount(pBuffer);

				SetFormatRange(nStart + nFormatBegin - GetStringCount(pStart), 
					nStart + nFormatBegin - GetStringCount(pPtr), 
					m_icOtherText.bBold,m_icOtherText.clrColor);
			}

		}
	} 
	catch(...){}

	GetRichEditCtrl().SetSel(crOldSel);
	GetRichEditCtrl().HideSelection(FALSE, FALSE);
	UnlockWindowUpdate();

	m_bInForcedChange = FALSE;

	if(m_bInputChar)
	{
		strRet = GetTipText(strRet,nStart, nEnd,GetRichEditCtrl().GetCaretPos());
		OnEdit(nStart,nEnd,strRet,TRUE);
		m_bInputChar = FALSE;
	}
	
	if( pBuffer != NULL )
	{
		delete [] pBuffer;
	}

}

void CScriptEditViewFormula::FormatTextLines(int nLineStart, int nLineEnd)
{
	long nStart = GetRichEditCtrl().LineIndex(GetRichEditCtrl().LineFromChar(nLineStart));
	long nEnd = GetRichEditCtrl().LineIndex(GetRichEditCtrl().LineFromChar(nLineEnd));
	nEnd += GetRichEditCtrl().LineLength(nLineEnd);

	FormatTextRange(nStart, nEnd);
}


void CScriptEditViewFormula::FormatAll()
{
	FormatTextRange(0, GetTextLength());
}

LRESULT CScriptEditViewFormula::OnSetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT res = Default();
	//FormatAll();
	return res;	
}

void CScriptEditViewFormula::OnChange() 
{
	if (m_bInForcedChange)
		return;

	CHARRANGE crCurSel; 
	GetRichEditCtrl().GetSel(crCurSel);

	if (m_changeType == ctMove &&
		crCurSel.cpMin == crCurSel.cpMax) 
	{
		// cut was canceled, so this is paste operation
		m_changeType = ctPaste;
	}

	switch(m_changeType) 
	{
	case ctReplSel:// old=(x,y) -> cur=(x+len,x+len)
	case ctPaste:  // old=(x,y) -> cur=(x+len,x+len)
		FormatTextLines(m_crOldSel.cpMin, crCurSel.cpMax);
		break;
	case ctDelete: // old=(x,y) -> cur=(x,x)
	case ctBack:   // old=(x,y) -> cur=(x,x), newline del => old=(x,x+1) -> cur=(x-1,x-1)
	case ctCut:    // old=(x,y) -> cur=(x,x)
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		break;
	case ctUndo:   // old=(?,?) -> cur=(x,y)
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		break;
	case ctMove:   // old=(x,x+len) -> cur=(y-len,y) | cur=(y,y+len)
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		if (crCurSel.cpMin > m_crOldSel.cpMin) // move after
			FormatTextLines(m_crOldSel.cpMin, m_crOldSel.cpMin);
		else // move before
			FormatTextLines(m_crOldSel.cpMax, m_crOldSel.cpMax);
		break;
	default:
		//m_changeType = ctReplSel;
		//FormatTextLines(0, this->GetRichEditCtrl().GetLineCount());
		FormatAll();
		break;
	}

	//undo action does not call OnProtected, so make it default
	m_changeType = ctUndo;
}

void CScriptEditViewFormula::OnProtected(NMHDR* pNMHDR, LRESULT* pResult)
{
	ENPROTECTED* pEP = (ENPROTECTED*)pNMHDR;

	// determine type of change will occur

	switch (pEP->msg) {
	case WM_KEYDOWN:
		switch (pEP->wParam) {
		case VK_DELETE:
			m_changeType = ctDelete;
			break;
		case VK_BACK:
			m_changeType = ctBack;
			break;
		default:
			m_changeType = ctUnknown;
			break;
		}
		break;
	case EM_REPLACESEL:
	case WM_CHAR:
		m_changeType = ctReplSel;
		break;
	case WM_PASTE:
		m_changeType = (m_changeType == ctCut)?ctMove:ctPaste;
		break;
	case WM_CUT:
		m_changeType = ctCut;
		break;
	case EM_SETCHARFORMAT:
		// Ignore this
		break;
	default:
		m_changeType = ctUnknown;
		break;
	};

	if (pEP->msg != EM_SETCHARFORMAT && m_changeType != ctMove)
		m_crOldSel = pEP->chrg;

	*pResult = FALSE;
}

int CScriptEditViewFormula::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (baseRichView::OnCreate(lpCreateStruct) == -1)
		return -1;

//	Initialize();

	SetCaseSensitive(FALSE);

	SetStringQuotes(_T("\""));
	SetSLComment(_T('\''));
	SetSLComment(_T("rem"));

	return 0;
}

CString	CScriptEditViewFormula::GetTipText(CString& strTitle,UINT nFlags, CPoint point,
									  int* nBegin /*= NULL*/,int* nEnd /*= NULL*/)
{
	CClientDC dc((CWnd*)&GetRichEditCtrl());
	CSize size = dc.GetTextExtent("A");

	CPoint pt;

	int nLine = 0,lChar = 0,lCountChar = 0;
	int nLineCount = GetRichEditCtrl().GetLineCount();
	int nFirstVisible = GetRichEditCtrl().GetFirstVisibleLine();
	if(nFirstVisible > 0)
		nLine = nFirstVisible;
	if(nLine > 0)
		lChar = GetRichEditCtrl().LineIndex(nLine);
	CString str;
	GetRichEditCtrl().GetWindowText(str);
	nLineCount = str.GetLength();
	for(; lChar < nLineCount; lChar++)
	{
		pt    = GetRichEditCtrl().GetCharPos( lChar ) ;
		nLine = GetRichEditCtrl().LineFromChar( lChar ) ;
		if( pt.y < point.y - size.cy)
		{
			nLine++;
			lCountChar = GetRichEditCtrl().LineIndex(nLine);
			if(lCountChar > lChar)
			{
				lChar = lCountChar;
				continue;
			}
		}
		
		if( pt.y > point.y )
		{
			return "";
		}
		else if( point.x >= pt.x && point.x <= pt.x+ size.cx &&
				 point.y >= pt.y && point.y <= pt.y+ size.cy)
		{			
			CString strText;
			
			if( nBegin != NULL )
				*nBegin = lChar;

			nLineCount = GetRichEditCtrl().LineFromChar(lChar);
			int nIndex = GetRichEditCtrl().LineIndex(nLineCount);
			nIndex = abs(lChar - nIndex);
			if(nIndex < 0)
				return "";

			int nLineLength = 1024;
			nLineCount = GetRichEditCtrl().GetLine(nLineCount, strText.GetBuffer(nLineLength),nLineLength);
			strText.ReleaseBuffer();
			if(nLineCount <= 0)
				return "";
			
			CString strRet;
//			strText.TrimLeft();
//			strText.TrimRight();
			strTitle.Format("%s",FindWord(strText,nIndex,strText.GetLength()));
			if(strTitle.IsEmpty())
				return "";

			if(nFlags & MK_RBUTTON)
			{
				if( nEnd != NULL )
					*nEnd = lChar + strTitle.GetLength();
				return strTitle;
			}
			else
			{
				strRet.Format("%s",GetKeyHelp(strTitle));
			}

			if(strRet.IsEmpty())
				return "";
			return strRet;
		}
	}

	return "";
}

CString	CScriptEditViewFormula::GetTipText(CString strText,int& nBegin,int& nEnd,CPoint point)
{
	CClientDC dc((CWnd*)&GetRichEditCtrl());
	CSize size = dc.GetTextExtent("A");

	CPoint pt = GetRichEditCtrl().GetCaretPos();
	for(; nBegin < nEnd; nBegin++)
	{
		pt = GetRichEditCtrl().GetCharPos( nBegin ) ;
		if( point.x >= pt.x && point.x <= pt.x+ size.cx )
		{
			int nIndex = GetRichEditCtrl().LineFromChar(nBegin);
			nIndex = GetRichEditCtrl().LineIndex(nIndex);
			nEnd = nIndex;
			nIndex = abs(nBegin - nIndex);
//			if(nIndex < 0)
//				return "";

			CString strRet;
			strRet.Format("%s",FindWord(strText,nIndex,strText.GetLength()));

			//nBegin  = GetRichEditCtrl().LineFromChar(nBegin);
			//nBegin  = GetRichEditCtrl().LineIndex(nBegin);
			nBegin  = nEnd;
			nBegin += nIndex;

			nBegin -= (strText.GetLength() - GetStringCount(strText));
			nEnd    = nBegin + strRet.GetLength();

			return strRet;
			
		}
	}

	return "";
}

HRESULT CScriptEditViewFormula::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR * lpcfFormat,
										   DWORD dwReco, BOOL bReally, HGLOBAL hMetaFile) 
{ 
	return S_OK;//CRichEditView::QueryAcceptData(lpdataobj, lpcfFormat,dwReco, bReally, hMetaFile);
}

