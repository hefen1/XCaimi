// QuoteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QuoteReport.h"
#include "QuoteReportDlg.h"
#include "DataSourceDefine.h"
#include "WinnerApplication.h"
#include "..\QuoteComm\pageid.h"

#define IDD_TABLE 0x100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CQuoteReportApp theApp;

// CQuoteReportDlg 对话框
IMPLEMENT_DYNAMIC(CQuoteReportDlg, CHSBizDialog)

CQuoteReportDlg::CQuoteReportDlg(CWnd* pParent /*=NULL*/)
	: CHSBizDialog(CQuoteReportDlg::IDD, pParent)
{
	m_pQuoteTabCtrl = NULL;
	m_pQuoteReport = NULL;
	m_DlgBkBrush.CreateSolidBrush(RGB(0,0,0));
	m_pDataSource = NULL;
	m_nDataSourceID = 0;
	m_bSplitMoved = FALSE;
}


CQuoteReportDlg::~CQuoteReportDlg()
{
	if (m_pDataSource != NULL)
	{
		m_pDataSource->HSDataSource_DataSourceUnInit(m_nDataSourceID);
	}
	delete m_pQuoteTabCtrl;
	delete m_pQuoteReport;
	m_DlgBkBrush.DeleteObject();
}

void CQuoteReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSBizDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQuoteReportDlg, CHSBizDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_MESSAGE(QUOTETAB_NOTIFY,OnChangeTabItem)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_CLICK,IDD_DYNAMIC_CREATE+1,&CQuoteReportDlg::OnReportTableNotify)
END_MESSAGE_MAP()

void CQuoteReportDlg::SetSize()
{
	CRect rcClient;
	GetClientRect(rcClient);

	int nTemp = rcClient.Width() / 3;
	int nTabHeight = 19;
	CRect rcLeftDisp = rcClient;
	if (m_pQuoteTabCtrl 
		&& m_pQuoteTabCtrl->GetSafeHwnd() 
		&& m_pQuoteTabCtrl->IsWindowEnabled())
	{
		m_pQuoteTabCtrl->MoveWindow(rcClient.left,rcClient.top,m_rcSplit.left-rcClient.left,nTabHeight,FALSE);
		rcLeftDisp.top += nTabHeight;
	}
	if (m_wndHorzScrollWnd.GetSafeHwnd() 
		&& m_wndHorzScrollWnd.IsWindowEnabled())
	{
		m_wndHorzScrollWnd.MoveWindow(m_rcSplit.right,rcClient.top,rcClient.right-m_rcSplit.right,nTabHeight,FALSE);
	}
	if (m_wndVertScrollWnd.GetSafeHwnd() && m_wndVertScrollWnd.IsWindowEnabled())
	{
		m_wndVertScrollWnd.MoveWindow(rcClient.right - nTabHeight,rcClient.top + nTabHeight,nTabHeight,rcClient.Height() - nTabHeight,FALSE);
		rcLeftDisp.right -= nTabHeight;
	}
	if (m_pQuoteReport
		&& m_pQuoteReport->GetSafeHwnd()
		&& m_pQuoteReport->IsWindowEnabled())
	{
		m_pQuoteReport->MoveWindow(rcLeftDisp,FALSE);
	}

}

long CQuoteReportDlg::GetMarketNameByType(const PageViewInfo* pvInfo,CStringArray& strMarketNames)
{
	int nCount = 0;
	nCount = pvInfo->GetMarketTypeCount();
	WORD* markets = new WORD[nCount];
	for (int i=0,j=0; i<nCount; i++)
	{
		markets[i] = (WORD)pvInfo->GetFirstMarketType(j);
	}
	CArray<StockType> ayTypes;
	nCount = m_pDataSource->HSDataSourceEx_GetSystemMarketInfo(markets,nCount,ayTypes);
	for (int i=0; i<nCount; i++)
	{
		strMarketNames.Add(CString(ayTypes.GetAt(i).m_stTypeName.m_szName));
	}
	delete []markets;
	ayTypes.RemoveAll();
	return nCount;
}

BOOL CQuoteReportDlg::SetPageInfo(PageViewInfo* pInfo,CString strColGroupName /* = _T */)
{
	if (pInfo == NULL || IsBadReadPtr(pInfo,1))
	{
		return FALSE;
	}
	if (pInfo->m_lPageType < PA_BEGIN || pInfo->m_lPageType > PA_END)
	{
		return FALSE;
	}
	
	// 不同的市场则切换标签和股票列表
	if (pInfo->m_nInPageTag & PT_BLOCK)		// 切换到板块
	{
		if (pInfo->GetBlockName().IsEmpty())
		{
			return FALSE;
		}
		m_pQuoteTabCtrl->ChangeCurItem(pInfo->GetBlockName(),FALSE);

		// 更改报价表的页面属性
		m_pQuoteReport->SetPageViewInfo(pInfo,strColGroupName);
		m_pQuoteReport->RequestStockUserInfo(pInfo);

		m_CurPageInfo.EmptyMarketType();
		m_CurPageInfo = pInfo;

	} 
	else if (pInfo->m_nInPageTag & PT_TYPE)	// 切换到市场
	{
		int nCount = pInfo->GetMarketTypeCount();
		if (nCount < 1)
		{
			return FALSE;
		} 
		else if (nCount == 1)		// 单个市场
		{
			if (m_CurPageInfo.GetFirstMarketType() != pInfo->GetFirstMarketType())
			{
				CStringArray ayMarket;
				// 改变标签页
				if (GetMarketNameByType(pInfo,ayMarket) > 0)
				{
					m_pQuoteTabCtrl->ChangeCurItem(ayMarket.GetAt(0),FALSE);

					// 更改报价表的页面属性
					m_pQuoteReport->SetPageViewInfo(pInfo,strColGroupName);
					m_pQuoteReport->RequestStockUserInfo(pInfo);

					m_CurPageInfo.EmptyMarketType();
					m_CurPageInfo.AddMarketType(pInfo->GetFirstMarketType());
				}
			}
		}
		else			// 市场组合
		{
			CStringArray ayMarket;
			// 改变标签页
			GetMarketNameByType(pInfo,ayMarket);
			m_pQuoteTabCtrl->ChangeCurItem(ayMarket.GetAt(0),FALSE);

			// 更改报价表的页面属性
			m_pQuoteReport->SetPageViewInfo(pInfo);
			m_pQuoteReport->RequestStockUserInfo(pInfo);

			m_CurPageInfo.EmptyMarketType();
			int nIndex = 0;
			for (int i=0; i<pInfo->GetMarketTypeCount(); i++,nIndex++)
			{
				m_CurPageInfo.AddMarketType(pInfo->GetFirstMarketType(nIndex));
			}
		}
	}
	return TRUE;

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CQuoteReportDlg 消息处理程序

BOOL CQuoteReportDlg::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();

	//获取数据引擎接口指针
	m_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	if (m_pDataSource && !IsBadReadPtr(m_pDataSource,1))
	{
		m_nDataSourceID = m_pDataSource->HSDataSource_DataSouceInit(this,NULL);
	}

	CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价模块-开始初始化，数据引擎ID = %d",m_nDataSourceID);

	// 获取水平滚动条皮肤
	CBitmap bmp;
	bmp.LoadBitmap(IDB_HSCROLLBAR);
	BITMAP bm;
	bmp.GetBitmap(&bm);
	m_hBmpHScrollBar = (HBITMAP)bmp.Detach();

	// 获取垂直滚动条皮肤
	CBitmap bmpHor;  
	bmpHor.LoadBitmap(IDB_VSCROLLBAR);
	BITMAP bmHor;
	bmpHor.GetBitmap(&bmHor);
	m_hBmpVScrollBar = (HBITMAP)bmpHor.Detach();


	// 创建垂直滚动条
	m_wndVertScrollWnd.Create(WS_CHILD | WS_VISIBLE | SBS_VERT, CRect(0,0,0,0),
		this, IDD_TABLE+3);
	m_wndVertScrollWnd.SetBitmap(m_hBmpVScrollBar);
	EnableScrollBarCtrl(SB_VERT, FALSE);

	// 创建水平滚动条
	m_wndHorzScrollWnd.Create(WS_CHILD | WS_VISIBLE | SBS_HORZ, CRect(0,0,0,0),
		this, IDD_TABLE+2);
	m_wndHorzScrollWnd.SetBitmap(m_hBmpHScrollBar);
	EnableScrollBarCtrl(SB_HORZ, FALSE);


	// 初始化行情报价表
	if (m_pQuoteReport == NULL || IsBadReadPtr(m_pQuoteReport,1))
	{
		CWinnerApplication::SimpleLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价窗口-开始创建行情报价控件");
		m_pQuoteReport = new CQuoteReportTable(m_pDataSource,m_nDataSourceID);
		m_pQuoteReport->Create(CRect(0,0,0,0),this,IDD_DYNAMIC_CREATE + 1,WS_CHILD | WS_VISIBLE & ~WS_CAPTION);
		m_pQuoteReport->ModifyStyleEx(WS_EX_CLIENTEDGE,0,0);
		m_pQuoteReport->SetHorScrollBarRef(&m_wndHorzScrollWnd);
		m_pQuoteReport->SetVerScrollBarRef(&m_wndVertScrollWnd);
		m_pQuoteReport->SetFocus();
		CWinnerApplication::SimpleLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价窗口-创建行情报价控件完成");
	}

	// 创建标签页
	m_pQuoteTabCtrl = new CQuoteTabCtrl(m_pDataSource,m_nDataSourceID);
	if (!m_pQuoteTabCtrl->Create(NULL,NULL,WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,IDD_DYNAMIC_CREATE))
	{
		CWinnerApplication::SimpleLog(ERROR_LOG_LEVEL,QuoteReportLoggerName,"创建标签页失败！");
		return FALSE;      // fail to create
	}
	m_pQuoteTabCtrl->ChangeTabGroup();

	CRect rcClient;
	GetClientRect(rcClient);
	int nTemp = rcClient.Width() / 3;
	int nTabHeight = 19;

	m_rcSplit.SetRect(rcClient.right - nTemp,rcClient.top,rcClient.right-nTemp+QUOTETAB_SPLITERECT_WIDTH,nTabHeight);
	SetSize();



	CWinnerApplication::SimpleLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
		"行情报价模块-完成初始化");

	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CQuoteReportDlg::OnInit()
{
	m_bAutoKeyboard = TRUE;
	RegisterHotKey(QUOTEREPORT_HOTKEY_F6,0,VK_F6);
	RegisterHotKey(QUOTEREPORT_HOTKEY_ADD_TO_BLOCK,MOD_CONTROL,'Z');
	RegisterHotKey(QUOTEREPORT_HOTKEY_ADD_TO_SELFBLOCK,MOD_ALT,'Z');
	RegisterHotKey(QUOTEREPORT_HOTKEY_BLOCK_ADSCRIPTION,MOD_CONTROL,'R');
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_STRING,MOD_CONTROL,VK_OEM_3);
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_CANCEL,MOD_CONTROL,VK_DELETE);
	
	int nMarkCount = 0;
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+nMarkCount++,MOD_CONTROL,'1');
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+nMarkCount++,MOD_CONTROL,'2');
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+nMarkCount++,MOD_CONTROL,'3');
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+nMarkCount++,MOD_CONTROL,'4');
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+nMarkCount++,MOD_CONTROL,'5');
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+nMarkCount++,MOD_CONTROL,'6');
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+nMarkCount++,MOD_CONTROL,'7');
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+nMarkCount++,MOD_CONTROL,'8');
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+nMarkCount++,MOD_CONTROL,'9');
	RegisterHotKey(QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+nMarkCount++,MOD_CONTROL,'0');
	CWinnerApplication::SimpleLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
		"行情报价模块-注册热键");
	return TRUE;
}

void CQuoteReportDlg::OnKeyBoardCodeNotify(StockUserInfo* pStkInfo)
{
	if(pStkInfo == NULL)
		return;
	CLinkageParam param;
	CString strMarkeType;
	strMarkeType.Format("%d",pStkInfo->m_ciStockCode.m_cCodeType);
	param.AddParam(PARAM_CODE_CHANGE_MARK_TYPE,strMarkeType);
	param.AddParam(PARAM_CODE_CHANGE_STOCK_CODE,pStkInfo->m_ciStockCode.GetCode());
	SendLinkageMsg(QUOTE_MESSAGE_CODE_CHANGE,&param);
}

void CQuoteReportDlg::OnWindowFocused()
{
	if(m_pQuoteTabCtrl)
		m_pQuoteTabCtrl->SetFocus();
}

void CQuoteReportDlg::OnSize(UINT nType, int cx, int cy)
{
	CHSBizDialog::OnSize(nType, cx, cy);

	if (!m_bSplitMoved)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		int nTabHeight = 19;

		m_rcSplit.SetRect(rcClient.right - rcClient.Width() / 3,rcClient.top,rcClient.right-rcClient.Width()/3+QUOTETAB_SPLITERECT_WIDTH,nTabHeight);
	}

	SetSize();
}

HBRUSH CQuoteReportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CHSBizDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	if(nCtlColor ==CTLCOLOR_DLG)
		return (HBRUSH)m_DlgBkBrush;   //返加页面背景刷子
	return hbr;
}

void CQuoteReportDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush* pBrush = CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE));
	dc.FillRect(m_rcSplit,pBrush);
	pBrush->DeleteObject();
}

BOOL CQuoteReportDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CHSBizDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CQuoteReportDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pQuoteReport->SendMessage(WM_HSCROLL,(WPARAM)MAKELONG(nSBCode,nPos),(LPARAM)(pScrollBar->GetSafeHwnd()));
	//CHSBizDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CQuoteReportDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pQuoteReport->SendMessage(WM_VSCROLL,(WPARAM)MAKELONG(nSBCode,nPos),(LPARAM)(pScrollBar->GetSafeHwnd()));
	//CHSBizDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CQuoteReportDlg::OnChangeTabItem(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)wParam;
	TabItemData* ItemData = (TabItemData*)lParam;
	if (ItemData)
	{
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价-标签页切换到第 %d 项,开始",nIndex);
		SetPageInfo(&ItemData->m_PageInfo,ItemData->m_strColGroupName);
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价-标签页切换到第 %d 项,结束",nIndex);
		return 1;
	}
	return 0;
}
BOOL CQuoteReportDlg::PreTranslateMessage(MSG* pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

void CQuoteReportDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CQuoteReportDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CQuoteReportDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_rcSplit.PtInRect(point))
	{
		SetCapture();
		SetCursor(::LoadCursor(NULL, (LPCTSTR)IDC_SIZEWE));
	}
	else
	{
		ReleaseCapture();
		SetCursor(::LoadCursor(NULL, (LPCTSTR)IDC_ARROW));
	}
	__super::OnMouseMove(nFlags,point);
}

void CQuoteReportDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rcSplit.PtInRect(point))
	{
		SetCapture();
		SetCursor(::LoadCursor(NULL, (LPCTSTR)IDC_SIZEWE));

		CRect rcInvert = m_rcSplit;
		CDC* pDC = GetDC();
		CPoint  pt;
		CPoint ptPriv = point;

		if (pDC)
		{
			pDC->InvertRect(rcInvert);
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this)
					break;

				switch (msg.message) 
				{
				case WM_MOUSEMOVE:
					{
						if (!m_bSplitMoved)
						{
							m_bSplitMoved = TRUE;
						}
						pDC->InvertRect(rcInvert);
						pt.x = (int)(short)LOWORD(msg.lParam);
						pt.y = (int)(short)HIWORD(msg.lParam);
						m_rcSplit.SetRect(pt.x,m_rcSplit.top,pt.x+QUOTETAB_SPLITERECT_WIDTH,m_rcSplit.bottom);
						rcInvert = m_rcSplit;
						pDC->InvertRect(rcInvert);
						SetSize();
						m_wndHorzScrollWnd.Invalidate();
						m_pQuoteTabCtrl->Invalidate();
						break;
					}
				case WM_LBUTTONUP:
				case WM_RBUTTONDOWN:
					{
						pDC->InvertRect(rcInvert);
						goto LoopEnd;
					}
				default:
					{
						DispatchMessage(&msg);
						break;
					}
				}
			}
		}
LoopEnd:
		ReleaseCapture();
		SetCursor(::LoadCursor(NULL, (LPCTSTR)IDC_ARROW));
		ReleaseDC(pDC);
	}

	__super::OnLButtonDown(nFlags,point);
}

void CQuoteReportDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags,point);
}

void CQuoteReportDlg::OnReportTableNotify(NMHDR* pNotifyStruct,LRESULT* result)
{
	//MessageBox(_T("Oh no!"));
}
LRESULT CQuoteReportDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return __super::WindowProc(message, wParam, lParam);
}

void CQuoteReportDlg::OnHotKey(UINT nId)
{
	switch(nId)
	{
	case QUOTEREPORT_HOTKEY_F6:
		{
			CString strSelfBlock = ((IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG))->LoadStr(IDS_SELF_BLOCK);
			m_pQuoteTabCtrl->ChangeCurItem(strSelfBlock);
			break;
		}
	case QUOTEREPORT_HOTKEY_ADD_TO_BLOCK:
		{
			m_pQuoteReport->OnAddToBlock();
			break;
		}
	case QUOTEREPORT_HOTKEY_ADD_TO_SELFBLOCK:
		{
			m_pQuoteReport->OnAddToSelfBlock();
			break;
		}
	case QUOTEREPORT_HOTKEY_BLOCK_ADSCRIPTION:
		{
			m_pQuoteReport->OnBlockAdscription();
			break;
		}
	case QUOTEREPORT_HOTKEY_MARK_STRING:
		{
			m_pQuoteReport->OnMarkCodeByString();
			break;
		}
	case QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN:
		{
			m_pQuoteReport->OnMarkCodeByIcon(ID_MARK_TAG_BEGIN);
				break;
		}
	case QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+1:
		{
			m_pQuoteReport->OnMarkCodeByIcon(ID_MARK_TAG_BEGIN+1);
				break;
		}
	case QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+2:
		{
			m_pQuoteReport->OnMarkCodeByIcon(ID_MARK_TAG_BEGIN+2);
				break;
		}
	case QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+3:
		{
			m_pQuoteReport->OnMarkCodeByIcon(ID_MARK_TAG_BEGIN+3);
				break;
		}
	case QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+4:
		{
			m_pQuoteReport->OnMarkCodeByIcon(ID_MARK_TAG_BEGIN+4);
				break;
		}
	case QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+5:
		{
			m_pQuoteReport->OnMarkCodeByIcon(ID_MARK_TAG_BEGIN+5);
				break;
		}
	case QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+6:
		{
			m_pQuoteReport->OnMarkCodeByIcon(ID_MARK_TAG_BEGIN+6);
				break;
		}
	case QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+7:
		{
			m_pQuoteReport->OnMarkCodeByIcon(ID_MARK_TAG_BEGIN+7);
				break;
		}
	case QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+8:
		{
			m_pQuoteReport->OnMarkCodeByIcon(ID_MARK_TAG_BEGIN+8);
				break;
		}
	case QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+9:
		{
			m_pQuoteReport->OnMarkCodeByIcon(ID_MARK_TAG_BEGIN+9);
			break;
		}
	case QUOTEREPORT_HOTKEY_MARK_CANCEL:
		{
			m_pQuoteReport->OnCancelMarkCode();
			break;
		}
	}
}