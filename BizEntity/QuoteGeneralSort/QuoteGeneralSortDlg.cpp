

#include "stdafx.h"
#include "Resource.h"
#include "QuoteGeneralSortDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif     
void DrawBKImageEx(CDC* pDC,int nLeft,int nTop,int nWidth,int nHeight,CString sBMPFile)
{
	CDC sdc;
	BITMAP bm;
	CBitmap bmp;
	char buffer[200];
	GetCurrentDirectory(200, buffer);
	CString BmpPath;
	CString strImgDir = CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH);
	BmpPath.Format("%s%s",strImgDir,sBMPFile);
	if (BmpPath.IsEmpty() )
	{
		//CHSTools::WriteRunLog("要加载的按钮图片不存在");
		return ;
	}
	if ( _access(BmpPath.GetBuffer(), 0) != -1 )
	{
		bmp.Attach(LoadImage(GetModuleHandle(NULL), BmpPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		sdc.CreateCompatibleDC(pDC);
		CBitmap *pOld = sdc.SelectObject(&bmp);
		bmp.GetBitmap(&bm);

		pDC->StretchBlt(nLeft, nTop, nWidth, nHeight, &sdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

		sdc.SelectObject(pOld);
	}
	
}
CArray<StockUserInfo*,StockUserInfo*>	g_pStock;
StockUserInfo* GetStockUserInfo(const char* code, HSMarketDataType type)
{
	char bufffer[7];
	memset(bufffer, 0, sizeof(bufffer));
	strncpy_s(bufffer, code, 6);
	CString cCode = (CString)bufffer;

	StockUserInfo* pTemp;
	for (int i=0; i<g_pStock.GetCount(); i++)
	{
		pTemp = g_pStock.GetAt(i);
		if ( !pTemp->GetCode().Compare(cCode) && pTemp->m_ciStockCode.m_cCodeType == type)
			return pTemp;
	}
	return NULL;
}


short g_ayGeneralSortType[] = {RT_RISE,RT_FALL,RT_5_RISE,RT_5_FALL,RT_AHEAD_COMM,RT_AFTER_COMM,RT_AHEAD_PRICE,RT_AHEAD_VOLBI,RT_AHEAD_MONEY};
#ifdef SUPPORT_NEW_GENERALGeneralSort
static char g_szSpeedRise[_MAX_PATH] = "快速涨幅排名";
static char g_szSpeedFall[_MAX_PATH] = "快速跌幅排名";
static QuoteTableCtrlGeneralSort::TableFlag m_strMultReportTitle[] = 
{ 
	{"今日涨幅排名",0,RT_RISE,NULL},
	{"快速涨幅排名",1,RT_5_RISE,NULL},
	{"今日委比前几名",2,RT_AHEAD_COMM,NULL},
	{"今日跌幅排名",3,RT_FALL,NULL},
	{"快速跌幅排名",4,RT_5_FALL,NULL},
	{"今日委比后几名",5,RT_AFTER_COMM,NULL},
	{"今日振幅排名",6,RT_AHEAD_PRICE,NULL},
	{"今日量比排名",7,RT_AHEAD_VOLBI,NULL},
	{"今日总金额排名",8,RT_AHEAD_MONEY,NULL}
};

void ChangeTitle(ReqGeneralSortEx* pAnsData)
{
	if( pAnsData == NULL )
	{
		return;
	}
	if( pAnsData->m_nMinuteCount == 0 )
	{
		sprintf(m_strMultReportTitle[1].strTitle, HS_LANGUAGE("快速涨幅排名"));
		sprintf(m_strMultReportTitle[4].strTitle, HS_LANGUAGE("快速跌幅排名"));
	}
	else
	{
		sprintf(m_strMultReportTitle[1].strTitle, HS_LANGUAGE("%d分钟涨幅排名"), pAnsData->m_nMinuteCount);
		sprintf(m_strMultReportTitle[4].strTitle, HS_LANGUAGE("%d分钟跌幅排名"), pAnsData->m_nMinuteCount);
	}
}
#else
static QuoteTableCtrlGeneralSort::TableFlag m_strMultReportTitle[] = 
{ 
	{"今日涨幅排名",0,RT_RISE,NULL},
	{"快速涨幅排名",1,RT_5_RISE,NULL},
	{"今日委比前几名",2,RT_AHEAD_COMM,NULL},
	{"今日跌幅排名",3,RT_FALL,NULL},
	{"快速跌幅排名",4,RT_5_FALL,NULL},
	{"今日委比后几名",5,RT_AFTER_COMM,NULL},
	{"今日振幅排名",6,RT_AHEAD_PRICE,NULL},
	{"今日量比排名",7,RT_AHEAD_VOLBI,NULL},
	{"今日总金额排名",8,RT_AHEAD_MONEY,NULL}
};
#endif

void RefreashOtherWnd(int nIndex)
{
	if ( nIndex >=0 && nIndex <9)
	{
		QuoteTableCtrlGeneralSort* pTable = (QuoteTableCtrlGeneralSort*)(m_strMultReportTitle[nIndex].pTable);
		if (pTable)
		{
			pTable->RefreshWnd();
		}
	}

}

IMPLEMENT_DYNCREATE(QuoteGeneralSortDlg, CHSBizDialog)

QuoteGeneralSortDlg::QuoteGeneralSortDlg(CWnd* pParent):CHSBizDialog(QuoteGeneralSortDlg::IDD, pParent),
						m_nRowCount(3),m_nColCount(3)
{
	m_szTitle = CSize(0,20);

}
QuoteGeneralSortDlg::~QuoteGeneralSortDlg()
{

}
void QuoteGeneralSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSBizDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_wndTitle);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_wndClose);
}

BEGIN_MESSAGE_MAP(QuoteGeneralSortDlg, CHSBizDialog)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &QuoteGeneralSortDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()

int QuoteGeneralSortDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CHSBizDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO: Add your specialized creation code here	

	return 0;
}

BOOL QuoteGeneralSortDlg::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();
	this->ShowWindow(SW_HIDE);
	CRect rc(0,0,680,535);
	this->MoveWindow(&rc);
	m_pDataSource = (IDataSource*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	if (m_pDataSource && !IsBadReadPtr(m_pDataSource,1))
	{
		m_nDataSourceID = m_pDataSource->HSDataSource_DataSouceInit(this,NULL);
	}
	m_cGeneralSortRequestType = SH_BOURSE|KIND_STOCKA | STOCK_MARKET;

 	CRect rect;
 	this->GetClientRect(rect);
 	rect.bottom = 21;
 	if (IsWindow(m_wndTitle.GetSafeHwnd()))
 	{
 		m_wndTitle.SetTitleColor(RGB(255,255,255));
 		m_wndTitle.SetWindowText("综合排名");
 		m_wndTitle.SetBkImage("title_bg.bmp");			// 设置标题背景图片
 		m_wndTitle.MoveWindow(rect);
 	}
	if (IsWindow(m_wndClose.GetSafeHwnd()))
	{
		this->GetClientRect(rect);
		rect.left = rect.right-21;
		CString strImgDir = CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH);
		CString filename; 
		filename.Format("%sclose_main.bmp", strImgDir);
		m_wndClose.SetBkImage(filename);
		m_wndClose.MoveWindow(rect.left,rect.top, 20,20);
		m_wndClose.ShowWindow(SW_SHOW);
	
	}
	SetSize();
 	SetMulitStock();
   	RequestStockUserInfo();  

	this->ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
}


//请求综合排名信息
void QuoteGeneralSortDlg::RequestData()
{
	QuoteTableCtrlGeneralSort* pQuoteGeneralSortTable=NULL; 
	char* pAskData = NULL;
	int nRetLen;
	for (int i=0; i< _countof(m_strMultReportTitle);i++)
	{
		QuoteTableCtrlGeneralSort::TableFlag tableflag = m_strMultReportTitle[i];
		pQuoteGeneralSortTable = (QuoteTableCtrlGeneralSort*)(m_strMultReportTitle[i].pTable);
		pAskData = pQuoteGeneralSortTable->GetRequestData(m_strMultReportTitle[i].nType, nRetLen, m_cGeneralSortRequestType);
		break;
	}
	if (pAskData)
	{
		m_pDataSource->HSDataSource_RequestAsyncData(m_nDataSourceID, pAskData,  nRetLen, 0, e_DataSouceSend_Clear);
	}
}
void QuoteGeneralSortDlg::AddStock(StockUserInfo* pStock,int nTotal)
{
	if( !pStock || IsBadReadPtr(pStock,1) || nTotal <= 0 )
		return;

	g_pStock.RemoveAll();
	StockUserInfo* pTemp = NULL;

	for(int i = 0; i < nTotal; i++)
	{
		pTemp = new StockUserInfo;
		pTemp->Copy(&pStock[i]);
		g_pStock.Add(pTemp);
	}
}

void QuoteGeneralSortDlg::HandleStockUserInfo(void* pInfo, int nlen)
{
	
	AnsStockInfoList* pStockList = (AnsStockInfoList*)pInfo;
	if (pStockList == NULL || IsBadReadPtr(pStockList,1))
	{
		return ;
	}
	AddStock(pStockList->m_pnowData, pStockList->m_nSize);
	RequestData();
	SetTimer(RequestGeneralGeneralSortTimerID, RequestGeneralGeneralSortTimer,NULL);
}

BOOL QuoteGeneralSortDlg::HSDataSourceSink_OnCommNotify(void* pData)
{
	return TRUE;
}

BOOL QuoteGeneralSortDlg::HSDataSourceSink_OnRecvData(void* pData, int nLen)
{
	if (!pData || IsBadReadPtr(pData,1))
	{
		return FALSE;
	}
	DataHead* pHead = (DataHead*)pData;

	// 股票列表信息应答
	if (pHead->m_nType == RT_STOCKINFOLIST)
	{
		HandleStockUserInfo(pData, nLen);
		return 0;
	}
	//综合排名应答信息
	if (pHead->m_nType ==  RT_GENERALSORT_EX)
	{
		AnsGeneralSortEx * pReportTotalData = (AnsGeneralSortEx*)pData;
		if(pReportTotalData->m_nSortType <= 0) 
		{
			return 0;
		}

	//#ifdef SUPPORT_NEW_GENERALGeneralSort
	//	ChangeTitle(pReportTotalData);
	//#endif
		//bTimeRequest = (pReportTotalData->m_nSortType&RT_AHEAD_MONEY);

		long lSize = sizeof(AnsGeneralSortEx)+(pReportTotalData->m_nSize-1)*sizeof(GeneralSortData);
		AnsGeneralSortEx* pTempBuffer = (AnsGeneralSortEx*)new char[lSize];
		if( pTempBuffer == NULL )
		{
			return 0;
		}
		ZeroMemory(pTempBuffer, lSize);

		memcpy(pTempBuffer, pReportTotalData, sizeof(AnsGeneralSortEx));
		int nCurPos = 0;
		int count = _countof(g_ayGeneralSortType);
		for( int i = 0; i < _countof(g_ayGeneralSortType); i++ )
		{
			// 存在此排列类型
			if( !(pReportTotalData->m_nSortType & g_ayGeneralSortType[i]) )
			{
				continue;
			}
			pTempBuffer->m_nSortType = g_ayGeneralSortType[i];			
			memcpy(pTempBuffer->m_prptData, &pReportTotalData->m_prptData[nCurPos], pReportTotalData->m_nSize*sizeof(GeneralSortData));
			nCurPos += pReportTotalData->m_nSize;
			int nWidIndex;
			QuoteTableCtrlGeneralSort* pWnd = GetWndByAskType(pTempBuffer->m_nSortType,nWidIndex);

			if(pWnd != NULL)
			{
				pWnd->HandleRecvData(pTempBuffer,nWidIndex);
			} 			
		}
	}
	return TRUE;
}

//请求股票列表信息
void QuoteGeneralSortDlg::RequestStockUserInfo()
{

	AskData*  paskData;
	int nRetLen = sizeof(AskData) + sizeof(ReqStockInfoList)+sizeof(CodeInfo);
	paskData = (AskData*)new char[nRetLen];
	if( paskData == NULL )
	{
		return;
	}
	memset(paskData,0,nRetLen);

	paskData->m_nType  = RT_STOCKINFOLIST;
	paskData->m_nSize  = 1;

	ReqStockInfoList* pMarketList = (ReqStockInfoList*)paskData->m_pCode;
	pMarketList->m_nMarketType[0] = m_cGeneralSortRequestType;
	pMarketList->m_nSize = 1;

	m_pDataSource->HSDataSource_RequestAsyncData(m_nDataSourceID, (char*)paskData, nRetLen, 0, e_DataSouceSend_Clear);
	delete paskData;
}

void QuoteGeneralSortDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case RequestGeneralGeneralSortTimerID:
		{
			RequestData();
			break;
		}
	default:
		break;
	}
}

void  QuoteGeneralSortDlg::SetTitleText(CString strText)
{
	if (IsWindow(m_wndTitle.GetSafeHwnd()))
		m_wndTitle.SetWindowText(strText.GetBuffer());
}
//设置分割后小图的大小
void QuoteGeneralSortDlg::SetSize()
{	
	CRect rect,rcclient;
	GetClientRect(rect);
	rect.DeflateRect(0,21,0,0);
	int nRowCount = m_nRowCount;
	int nColCount = m_nColCount;
	int nRowHeight = (rect.Height()-2) / nRowCount;
	int nColWidth  = (rect.Width()-2)  / nColCount;
	int offleft = 0;
	int offtop = rect.top;
	for( int i = 0; i < _countof(g_ayGeneralSortType); i++ )
	{
		QuoteTableCtrlGeneralSort* pTable = new QuoteTableCtrlGeneralSort();
		pTable->Create(CRect(0,0,0,0), this, i);
		pTable->ModifyStyleEx(WS_EX_CLIENTEDGE,0,0);
	
		m_ayQuoteSort.Add(pTable);
		

		if (i %3 == 0 && i>0)
		{
			offtop += nRowHeight+1;
			offleft = 0;
		}
		pTable->SetWindowPos(NULL,offleft,offtop, nColWidth, nRowHeight,0);

		pTable->ShowWindow(SW_SHOW);
		offleft += nColWidth+1;
		

	}
}

//设置多图的信息
void QuoteGeneralSortDlg::SetMulitStock()
{
	int nRow,nCol;

	int nTotal = 0;
	QuoteTableCtrlGeneralSort* pTable;
	for(nRow = 0;nRow < m_nRowCount;nRow++)
	{
		for(nCol = 0;nCol < m_nColCount;nCol++)
		{
			//分割后的子图关联到m_strMultReportTitle数组中的ptable			
			pTable = m_ayQuoteSort.GetAt(nRow*m_nRowCount+nCol); 
			pTable->SetFlag(&m_strMultReportTitle[nRow*m_nRowCount+nCol]);
			pTable->ModifyStyleEx(WS_EX_CLIENTEDGE,0,0);
			pTable->InitColData();
			pTable->DrawTitle();
		}
	}

	
}

QuoteTableCtrlGeneralSort* QuoteGeneralSortDlg::GetWndByAskType(int nAskType,int& nWidIndex)
{
	for( int i = 0; i < _countof(m_strMultReportTitle); i++ )
	{
		if( m_strMultReportTitle[i].nType == nAskType )
		{
			nWidIndex = i;
			return (QuoteTableCtrlGeneralSort*)m_strMultReportTitle[i].pTable;
		}
	}

	return NULL;
}

StockUserInfo* QuoteGeneralSortDlg::GetStockUserInfo(const char* code, HSMarketDataType type)
{
// 	char bufffer[7];
// 	memset(bufffer, 0, sizeof(bufffer));
// 	strncpy_s(bufffer, code, 6);
// 	CString cCode = (CString)bufffer;
// 
// 	StockUserInfo* pTemp;
// 	for (int i=0; i<m_pStock.GetCount(); i++)
// 	{
// 		pTemp = m_pStock.GetAt(i);
// 		if ( !pTemp->GetCode().Compare(cCode) && pTemp->m_ciStockCode.m_cCodeType == type)
// 			return pTemp;
// 	}
 	return NULL;
}

void QuoteGeneralSortDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	__super::OnLButtonDown(nFlags, point);
}
void QuoteGeneralSortDlg::OnDestroy()
{
	KillTimer(RequestGeneralGeneralSortTimerID);
	if (m_pDataSource && !IsBadReadPtr(m_pDataSource,1))
	{
		m_pDataSource->HSDataSource_DataSourceUnInit(m_nDataSourceID);
	}
	RemoveCArray();
}
void QuoteGeneralSortDlg::OnPaint()
{
  	CPaintDC dc(this);
	CRect rect, closerect;
  	GetClientRect(rect);
	COLORREF colorline = RGB(200,0,0);
	int nRowCount = m_nRowCount;
	int nColCount = m_nColCount;

  
	rect.DeflateRect(0,21,0,0);

	int nRowHeight = (rect.Height()-2) / nRowCount;
	int nColWidth  = (rect.Width()-2)  / nColCount;
	int offleft = 0;
	int offtop = rect.top;
	for( int i = 0; i < nColCount-1; i++ )
	{
		rect.left +=nColWidth;
		rect.right = rect.left+1;
		dc.Draw3dRect(rect,colorline,colorline);
		rect.left =rect.right;
	} 
	GetClientRect(rect);
	rect.DeflateRect(0,21,0,0);
	for( int i = 0; i < nRowCount-1; i++ )
	{
		rect.top +=nRowHeight;
		rect.bottom = rect.top+1;
		dc.Draw3dRect(rect,colorline,colorline);
		rect.top = rect.bottom;
	}


  	/*dc.FillSolidRect(rect, RGB(255,0,0));*/
}
void  QuoteGeneralSortDlg::RemoveCArray()
{
	
	for (int i=0; i<g_pStock.GetCount();i++)
	{
		StockUserInfo* pTemp = g_pStock.GetAt(i);
		if (pTemp)
			delete pTemp;
	}
	g_pStock.RemoveAll();
	for (int i=0; i<m_ayQuoteSort.GetCount();i++)
	{
		QuoteTableCtrlGeneralSort* pTemp = m_ayQuoteSort.GetAt(i);
		if (pTemp && !IsBadReadPtr(pTemp,1))
		{
			delete pTemp;		
		}
	}
	m_ayQuoteSort.RemoveAll();
}
void QuoteGeneralSortDlg::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	
	OnDestroy();
	this->CloseWindow();
}
