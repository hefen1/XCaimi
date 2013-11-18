///////////////////////////////////////////////////////////////////////////////////////////
// 文件名:			DlgBlockPicAnalysis.cpp
// 创建者：				
// 创建时间：		2002-3-6
// 内容描述：		"板块分析"功能模块---"图形分析"
///////////////////////////////////////////////////////////////////////////////
// HSPicAnalyseDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "DlgBlockPicAnalysis.h"
#include "DlgBlockAnalysis.h"
#include "ExpPublic.h"
#include "hsstructnew.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockPicAnalysis dialog
extern HWND g_hParentWnd;

CDlgBlockPicAnalysis::CDlgBlockPicAnalysis(CWnd* pParent /*=NULL*/, TYPEANDBLOCK* pList, 
									 TabStockSortData* pIndex, int iItem, int iSubItem)
	: CDialog(CDlgBlockPicAnalysis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBlockPicAnalysis)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pList		= pList;
	m_pIndex	= pIndex;
	m_iCurLine	= iItem;
	m_iCurCol	= iSubItem - 2;
	m_dMinVal	= HS_MAX_VAL;
	m_dMaxVal	= 0 - HS_MAX_VAL;
	m_iMaxBlockX= 0;
	m_iMaxListX = 0;
	m_iCurStock = 0;
	m_colorref	= -1;
	Create(IDD, pParent);
}


void CDlgBlockPicAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBlockPicAnalysis)
	DDX_Control(pDX, IDC_LIST_BOX, m_ListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBlockPicAnalysis, CDialog)
	//{{AFX_MSG_MAP(CDlgBlockPicAnalysis)
	ON_LBN_DBLCLK(IDC_LIST_BOX, OnDblclkListBox)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockPicAnalysis message handlers


void CDlgBlockPicAnalysis::GetMinMaxVal()
{
	int	z, zEnd = m_pList->ayCalcStock.GetSize();						//类股票数		

	for(z = 0; z < zEnd; z ++)		//每一个列表中,股票的数量
	{
		if( m_pList->ayCalcRet[z]->ayValues.GetSize() <= 0)
			continue;
		if(m_dMinVal > m_pList->ayCalcRet[z]->ayValues[m_iCurCol])
			m_dMinVal = m_pList->ayCalcRet[z]->ayValues[m_iCurCol];
		if(m_pList->ayCalcRet[z]->ayValues[m_iCurCol] > m_dMaxVal)
			m_dMaxVal = m_pList->ayCalcRet[z]->ayValues[m_iCurCol];
	}
	m_dMaxVal += m_dMaxVal / 100;
}


void CDlgBlockPicAnalysis::GetMaxX()
{
	int i;

	for(i = 0; i < HS_PIC_ROWNUM; i ++)
	{
		if(m_iMaxBlockX < ayBlockData[i])
			m_iMaxBlockX = ayBlockData[i];
		if(m_iMaxListX < ayListData[i])
			m_iMaxListX = ayListData[i];
	}
	m_iMaxListX += 1 + m_iMaxListX / 10;
	m_iMaxBlockX += 1 + + m_iMaxBlockX / 10;
}


void CDlgBlockPicAnalysis::Init()		//设置m_iY, m_...X, m_DrawRect
{
	CWnd *pwnd = GetDlgItem(IDC_PIC);

	pwnd->GetWindowRect(&m_DrawRect);
	ScreenToClient(&m_DrawRect);

	m_iY = ( m_DrawRect.Height() - 2 * HS_ROW_PIX ) * 1.0 / HS_PIC_ROWNUM;
	m_iX = ( m_DrawRect.Width() - HS_COL_PIX ) * 1.0 / HS_PIC_COLNUM;

	GetMinMaxVal();
	InitListData();
	InitBlockData();
	GetMaxX();
}


void CDlgBlockPicAnalysis::InitListData()
{

	double	dTab = (m_dMaxVal - m_dMinVal) / HS_PIC_ROWNUM;
	ayListData.SetSize(HS_PIC_ROWNUM);
	int	i, iEnd = m_pList->ayCalcStock.GetSize();
	int	j, iTemp;
	BlockCalcRet *pCalRet;

	for(i = 0; i < iEnd; i++ )
	{
		pCalRet = m_pList->ayCalcRet.GetAt(i);
		if( pCalRet->ayValues.GetSize() <= 0 )
		{
			continue;
		}
		for(j = 0; j < HS_PIC_ROWNUM; j ++)
		{
			double dTemp = pCalRet->ayValues.GetAt(m_iCurCol);
			if((dTemp >= (j * dTab + m_dMinVal)) && ( dTemp < ( ( j + 1 )* dTab + m_dMinVal ) ) )
			{
				iTemp = ayListData.GetAt(j);
				iTemp ++;
				ayListData.SetAt(j, iTemp);
				break;
			}
		}
	}
}


void CDlgBlockPicAnalysis::InitBlockData()
{
	ayBlockData.SetSize(HS_PIC_ROWNUM);
	BlockInfo	*pCurBlock = m_pList->ayBlock.GetAt(m_iCurLine);
	CArray<CodeInfo*, CodeInfo*>	*payStock = &(pCurBlock->ayStockList);
	CArray<StockUserInfo*, StockUserInfo*>	*payCalStock = &(m_pList->ayCalcStock);
	CodeInfo *pStock;
	CString	strStockCode;
	BlockCalcRet	*pCalRet;
	int i, iEnd = m_pList->ayCalcStock.GetSize();
	int	j, jEnd = payStock->GetSize();

	double dTab = (m_dMaxVal - m_dMinVal) / HS_PIC_ROWNUM;

	for( j = 0; j < jEnd; j ++ )
	{
		pStock = payStock->GetAt(j);
		strStockCode.Format("%s", pStock->m_cCode);
		
		i = ((CDlgBlockAnalysis *)(this->GetParent()))->GetStockIndex(m_pIndex, strStockCode);

		if(i == -1)					//如果没有
			continue;

		pCalRet = m_pList->ayCalcRet.GetAt(i);
		double dTemp;
		CString strTemp;

		if( pCalRet->ayValues.GetSize() <= 0 )
		{
			dTemp = 0;
		}
		else
		{
			dTemp = pCalRet->ayValues.GetAt(m_iCurCol);
		}

		strTemp.Format("%s (%.3f)", pStock->m_cCode, dTemp);
		m_ListBox.AddString(strTemp);			//在列表盒中加入
		for(i = 0; i < HS_PIC_ROWNUM; i ++)
		{
			if((dTemp >= (i * dTab + m_dMinVal)) && ( dTemp < ( ( i + 1 )* dTab + m_dMinVal ) ) )
			{
				int iTemp;

				iTemp = ayBlockData.GetAt(i);
				iTemp ++;
				ayBlockData.SetAt(i, iTemp);
				break;
			}
		}
	}
}


void CDlgBlockPicAnalysis::DrawPic()
{
	CClientDC *pDC = (CClientDC *)this->GetDC();

	CPen	BgPen, *pOldPen;
	CBrush	BgBrush(RGB(255,255, 255)), *pOldBrush;

	BgPen.CreatePen(0, 1, RGB(255, 255, 255));
	pDC->GetBkColor();
	CDC *pdc = this->GetDC();
	pDC->SetBkColor(m_colorref);
	pDC->SetTextColor(RGB(0, 0, 0));
	pOldPen = pDC->SelectObject(&BgPen);
	pOldBrush = pDC->SelectObject(&BgBrush);
	RECT	rect;
	rect.left = m_DrawRect.left + HS_COL_PIX;
	rect.right = m_DrawRect.right - HS_RIGHT_W;
	rect.top = m_DrawRect.top + HS_ROW_PIX;
	rect.bottom = m_DrawRect.bottom - HS_ROW_PIX;
	pDC->Rectangle(&rect);
	pDC->DrawFocusRect(&rect);
	BgPen.DeleteObject();
	BgPen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	pDC->SelectObject(&BgPen);
	int		iTab = m_iX * HS_PIC_COLNUM / HS_X_NUM;
	int i;
	for( i = 0; i < HS_X_NUM - 1; i++ )
	{
		pDC->MoveTo(rect.left + iTab * (i + 1), rect.bottom);
		pDC->LineTo(rect.left + iTab * (i + 1), rect.top);
	}
	UINT oldF = pDC->SetTextAlign(TA_LEFT|TA_TOP);
	for(i = 0; i <= HS_X_NUM; i++)
	{
		if( i % 2 != 0 )
			continue;
		CString strTemp;
		if( i == 2 )
		{
			pDC->SetTextColor(RGB(255, 0, 0));
			strTemp.Format("%.1f",  m_iMaxBlockX * 1.0 * i / HS_X_NUM);
			strTemp.TrimRight('0');
			strTemp.TrimRight('.');
			pDC->TextOut(rect.left + iTab * i, rect.top - HS_ROW_PIX, strTemp);
			pDC->SetTextColor(RGB(0, 255, 0));
			strTemp.Format("%.1f",  m_iMaxListX * 1.0 * i / HS_X_NUM);
			strTemp.TrimRight('0');
			strTemp.TrimRight('.');
			pDC->TextOut(rect.left + iTab * i, rect.bottom + 2, strTemp);	
		}
		else
		{
			pDC->SetTextColor(RGB(255, 0, 0));
			strTemp.Format("%.0f",  m_iMaxBlockX * 1.0 * i / HS_X_NUM);
			pDC->TextOut(rect.left + iTab * i, rect.top - HS_ROW_PIX, strTemp);
			pDC->SetTextColor(RGB(0, 255, 0));
			strTemp.Format("%.0f",  m_iMaxListX * 1.0 * i / HS_X_NUM);
			pDC->TextOut(rect.left + iTab * i, rect.bottom + 2, strTemp);	
		}
	}
	pDC->SetTextColor(RGB(0, 0, 0));

	iTab = m_iY * HS_PIC_ROWNUM / HS_Y_NUM;
	for(i = 0; i < HS_Y_NUM - 1; i ++)
	{
		pDC->MoveTo(rect.left, rect.top + iTab * (i + 1));
		pDC->LineTo(rect.right, rect.top + iTab * (i + 1));
	}

	double dTemp = ( m_dMaxVal - m_dMinVal ) / HS_Y_NUM;
	double dMax = m_dMaxVal, dMin = m_dMinVal;
	int		nOne = 0, nTwo = 0;
	while( abs( (long)dMax ) > 99 )
	{
		dMax /= 10;
		nOne++;
	}
	while(abs( (long)dMin ) > 99 )
	{
		dMin /= 10;
		nTwo++;
	}

	long nUnit = 1;
	nOne = nOne > nTwo ? nOne : nTwo;
	while( nOne )
	{
		nOne--;
		nUnit *= 10;
	}
	pDC->SetTextAlign(TA_RIGHT|TA_BASELINE);

	for(i = 0; i <= HS_Y_NUM; i++)
	{
		CString strTemp;
		strTemp.Format("%.1f",  ( m_dMinVal + dTemp * i) / nUnit );
		pDC->TextOut(rect.left - 2, rect.bottom - iTab * i, strTemp);
	}
	CString strTemp;
	if( nUnit >= 100000000 ) // 亿
	{
		strTemp.Format(_T("X%d亿"), nUnit/100000000);
	}
	else if( nUnit >= 1000000 ) // 百万
	{
		strTemp.Format(_T("X%d百万"), nUnit/1000000);
	}
	else if( nUnit >= 10000 ) // 万
	{
		strTemp.Format(_T("X%d万"), nUnit/10000);
	}
	else if( nUnit >= 1000 ) //千
	{
		strTemp.Format(_T("X%d千"), nUnit/1000);
	}
	else if( nUnit >= 100 ) // 百
	{
		strTemp.Format(_T("X%d百"), nUnit/100);
	}
	else
	{
		strTemp.Format(_T("X%d"), nUnit);
	}
	
	pDC->TextOut(rect.left - 2, m_DrawRect.bottom, strTemp);

	int iEnd = ayBlockData.GetSize();
	int	iTemp;
	CPen penTemp;

	penTemp.CreatePen(0, 2, RGB(0, 255, 0));
	pDC->SelectObject(&penTemp);
	dTemp = HS_PIC_COLNUM * 1.0 / m_iMaxListX * m_iX;
	for(i = 0; i < iEnd; i++)
	{
		iTemp = ayListData.GetAt(i);
		if(iTemp != 0)
		{
			pDC->MoveTo(rect.left, rect.bottom - i * m_iY);
			pDC->LineTo(rect.left + iTemp * dTemp, rect.bottom - i * m_iY);
		}
	}

	penTemp.DeleteObject();
	penTemp.CreatePen(0, 1, RGB(255, 0, 0));
	pDC->SelectObject(&penTemp);
	dTemp = HS_PIC_COLNUM * 1.0 / m_iMaxBlockX * m_iX;
	for(i = 0; i < iEnd; i++)
	{
		iTemp = ayBlockData.GetAt(i);
		if(iTemp != 0)
		{
			pDC->MoveTo(rect.left, rect.bottom - i * m_iY);
			pDC->LineTo(rect.left + iTemp * dTemp, rect.bottom - i * m_iY);
		}
	}

	SelectCurStock();

	pDC->SetTextAlign(oldF);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}


BOOL CDlgBlockPicAnalysis::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	BlockInfo	*pCurBlock = m_pList->ayBlock.GetAt(m_iCurLine);
	m_strWinText = pCurBlock->strBlockName;
	this->SetWindowText(m_strWinText);
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBlockPicAnalysis::OnDblclkListBox() 
{
	CString	strTemp, strStockName;
	int	i, iEnd = m_pList->ayCalcStock.GetSize();
	StockUserInfo	*pStock;
	BlockCalcRet	*pCalRet;

	m_ListBox.GetText(m_ListBox.GetCurSel(), strTemp);

	strTemp.TrimLeft();
	strStockName = strTemp.Left(8);

	for(i = 0; i < iEnd; i++)
	{
		pStock = m_pList->ayCalcStock.GetAt(i);

		if(strStockName.Compare(pStock->GetName()) == 0)
		{
			break;
		}
	}
	if(i >= iEnd)
		return;

	HSCurKeyInfo keyInfo;
	keyInfo.Copy(&pStock->m_ciStockCode);
	::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
							HX_USER_DRAGDATA,(LPARAM)&keyInfo);
	pCalRet = m_pList->ayCalcRet.GetAt(i);
	if( pCalRet->ayValues.GetSize() <= 0 )
	{
		return;
	}

	double dTemp = pCalRet->ayValues.GetAt(m_iCurCol);
	double dTab = (m_dMaxVal - m_dMinVal) / HS_PIC_ROWNUM;
	int j;
	for( j = 0; j < HS_PIC_ROWNUM; j ++)
	{
		if((dTemp >= (j * dTab + m_dMinVal)) && ( dTemp < ( ( j + 1 )* dTab + m_dMinVal ) ) )
			break;
	}
	if(j >= HS_PIC_ROWNUM)
		return;

	m_iCurStock = j;
	SelectCurStock();
}

void CDlgBlockPicAnalysis::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if( m_colorref == -1 )
	{
		m_colorref = dc.GetPixel(1,1);
	}
	DrawPic();	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgBlockPicAnalysis::SelectCurStock()
{
	CClientDC *pDC = (CClientDC *)this->GetDC();
	CBrush	BgBrush(RGB(255,0, 0)), *pOldBrush;
	CPen    *oldPen, Pen(0, 1, RGB(255, 255, 255));
	pOldBrush = pDC->SelectObject(&BgBrush);
	RECT	rect;

	rect.left = m_DrawRect.right  - HS_RIGHT_W;
	rect.right = m_DrawRect.right;
	rect.top = m_DrawRect.top + HS_ROW_PIX - 2;
	rect.bottom = m_DrawRect.bottom - HS_ROW_PIX;
	pDC->Rectangle(&rect);
	
	CBrush brush(RGB(255, 255, 255));
	pDC->SelectObject(&brush);
	oldPen = pDC->SelectObject(&Pen);
	rect.top = rect.bottom - (int)(m_iCurStock * m_iY) - 4;
	rect.bottom -= (int)(m_iCurStock * m_iY);
	pDC->Rectangle(&rect);

	pDC->SelectObject(oldPen);
	rect.left = m_DrawRect.left + HS_COL_PIX;
	rect.right = m_DrawRect.right;
	rect.top = m_DrawRect.top + HS_ROW_PIX - HS_TOP_W;
	rect.bottom = m_DrawRect.top + HS_ROW_PIX;
	pDC->SelectObject(&BgBrush);
	pDC->Rectangle(&rect);

	pDC->SelectObject(&Pen);
	double dTemp = HS_PIC_COLNUM * 1.0 / m_iMaxBlockX * m_iX;
	pDC->SelectObject(&brush);
	rect.left += (int)(ayBlockData.GetAt(m_iCurStock) * dTemp);
	rect.right = rect.left + 4;
	pDC->Rectangle(&rect);

	pDC->SelectObject(oldPen);
	rect.left = m_DrawRect.left + HS_COL_PIX;
	rect.right = m_DrawRect.right;
	rect.top = m_DrawRect.bottom - HS_ROW_PIX;
	rect.bottom = rect.top + HS_TOP_W;
	pDC->SelectObject(&BgBrush);
	pDC->Rectangle(&rect);

	pDC->SelectObject(&Pen);
	dTemp = HS_PIC_COLNUM * 1.0 / m_iMaxListX * m_iX;
	pDC->SelectObject(&brush);
	rect.left += (int)(ayListData.GetAt(m_iCurStock) * dTemp);
	rect.right = rect.left + 4;
	pDC->Rectangle(&rect);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(pOldBrush);
}

void CDlgBlockPicAnalysis::OnCancel()
{
	if( ::IsWindow( m_hWnd ) )
	{
		DestroyWindow();
	}
}

void CDlgBlockPicAnalysis::PostNcDestroy()
{
	delete this;
}
