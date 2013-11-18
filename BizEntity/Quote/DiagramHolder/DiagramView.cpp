// DiagramView.cpp : 实现文件
//

#include "stdafx.h"
#include <afxcontrolbarutil.h>
#include "DiagramHolder.h"
#include "DiagramView.h"
#include "DlgDiagramHolder.h"
#include "KeyBoardStruct.h"


BEGIN_MESSAGE_MAP(CDiagramView, CView)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SYSKEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


CRequestCollector::CRequestCollector()
{
	
}

CRequestCollector::~CRequestCollector()
{
	for(int i = 0; i < m_arrReqData.GetCount(); i++)
	{
		char* pAsk = m_arrReqData[i];
		short lLen = m_arrReqLen[i];
		if( pAsk != NULL && lLen > 0 )
		{
			delete[] pAsk;
		}		
	}
}

void CRequestCollector::AddRequest(char* pAskData,short lAskLen)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if( pAskData != NULL && lAskLen > 0)
	{
		char* pData = new char[lAskLen];
		memcpy(pData,pAskData,lAskLen);
		m_arrReqData.Add(pData);
		m_arrReqLen.Add(lAskLen);
	}
}

void CRequestCollector::DoRequest(IDataSourceEx* pDataSource,long nDataSourceID, BOOL bAutoClear)
{
	UINT nSendOption = bAutoClear?e_DataSouceSend_Clear|e_DataSouceSend_ReadDisk:e_DataSouceSend_ReadDisk;
	for(int i = 0; i < m_arrReqData.GetCount(); i++)
	{
		char* pAsk = m_arrReqData[i];
		short lLen = m_arrReqLen[i];
		if( pAsk != NULL && lLen > 0 )
		{
			pDataSource->HSDataSource_RequestAsyncData(nDataSourceID, pAsk, lLen, 0, nSendOption);
		}		
	}
}

void CRequestCollector::Release()
{
	delete this;
}

// CDiagramView

IMPLEMENT_DYNAMIC(CDiagramView, CView)

CDiagramView::CDiagramView()
{
	m_currentStockIndex = 0;
	m_pLayout = NULL;
	m_bFixedLayoutMode = FALSE;
	m_currentStockCode.SetEmpty();
}

CDiagramView::~CDiagramView()
{
}

BOOL CDiagramView::CreateView(CHSBizDialog* pBizDlg)
{
	m_pHolder = pBizDlg;
	CCreateContext context;
	context.m_pNewViewClass = RUNTIME_CLASS(CDiagramView);

	if (!this->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0,0,0,0), pBizDlg, AFX_IDW_PANE_FIRST, &context))
	{
		TRACE(traceAppMsg, 0, "Warning: could not create CDiagramView for frame.\n");
		return FALSE;
	}
	m_LayerSelector.SetHostWnd(this);
	InitKeyboradCmd();
	return TRUE;
}

void CDiagramView::InitKeyboradCmd()
{
	//行情报价
	m_aReportKeyboardCmd.Add(KeyShangAQuote		);//上A报价
	m_aReportKeyboardCmd.Add(KeyShangBQuote		);//上B报价
	m_aReportKeyboardCmd.Add(KeyShengAQuote		);//深A报价
	m_aReportKeyboardCmd.Add(KeyShengBQuote		);//深B报价
	m_aReportKeyboardCmd.Add(KeyShangBondQuote);//上债报价
	m_aReportKeyboardCmd.Add(KeyShengBondQuote);//深债报价
	//m_aReportKeyboardCmd.Add(KeyLOFQuote	  );//lof报价
	m_aReportKeyboardCmd.Add(KeyShangFundQuote); //上债报价
	m_aReportKeyboardCmd.Add(KeyShengFundQuote); //深债报价
	m_aReportKeyboardCmd.Add(KeySmallStocksQuote);//中小盘
	m_aReportKeyboardCmd.Add(KeyGEM           );//创业板
	m_aReportKeyboardCmd.Add(KeyGEMRange	  );//创业板涨幅
	m_aReportKeyboardCmd.Add(KeyARange		  );//A股涨幅
	m_aReportKeyboardCmd.Add(KeyAGeneral	  );//A股综合			
	m_aReportKeyboardCmd.Add(KeyShangARange	  );//上A涨幅
	m_aReportKeyboardCmd.Add(KeyShangBRange	  );//上B涨幅
	m_aReportKeyboardCmd.Add(KeyShengARange	  );//深A涨幅
	m_aReportKeyboardCmd.Add(KeyShengBRange	  );//深B涨幅
	m_aReportKeyboardCmd.Add(KeyShangBondRange);//上债涨幅
	m_aReportKeyboardCmd.Add(KeyShengBondRange);//深债涨幅	
	m_aReportKeyboardCmd.Add(KeyShangFundRange);//上金涨幅
	m_aReportKeyboardCmd.Add(KeyShengFundRange);//深金涨幅
	m_aReportKeyboardCmd.Add(KeySmallStocksRange);//中小盘涨幅

	//技术分析
	m_aTechAnalysisKeyboardCmd.Add(KeyOneMinute		);//1分钟线
	m_aTechAnalysisKeyboardCmd.Add(KeyFiveMinute	);//5分钟线
	m_aTechAnalysisKeyboardCmd.Add(Key15Minute		);//15分钟线
	m_aTechAnalysisKeyboardCmd.Add(Key30Minute		);//30分钟
	m_aTechAnalysisKeyboardCmd.Add(Key60Minute		);//60分钟
	m_aTechAnalysisKeyboardCmd.Add(KeyDayData		);//日线
	m_aTechAnalysisKeyboardCmd.Add(KeyWeekData		);//周线
	m_aTechAnalysisKeyboardCmd.Add(KeyYearData		);//年线
	m_aTechAnalysisKeyboardCmd.Add(KeyMultiDayData  );//多日线

	//功能键
	m_aFunctionKeyboardCmd.Add(KeyF1  );
	m_aFunctionKeyboardCmd.Add(KeyF2  );
	m_aFunctionKeyboardCmd.Add(KeyF3  );
	m_aFunctionKeyboardCmd.Add(KeyF4  );
	m_aFunctionKeyboardCmd.Add(KeyF5  );
	m_aFunctionKeyboardCmd.Add(KeyF6  );
	m_aFunctionKeyboardCmd.Add(KeyF7  );
	m_aFunctionKeyboardCmd.Add(KeyF8  );
	m_aFunctionKeyboardCmd.Add(KeyCtrlX  );
	m_aFunctionKeyboardCmd.Add(KeyF10  );
}

BOOL CDiagramView::IsQuoteReprotKeyboradCmd(int nCmd)
{
	for(int i = 0; i < m_aReportKeyboardCmd.GetCount(); i++)
	{
		if( nCmd == m_aReportKeyboardCmd[i] )
			return TRUE;
	}
	return FALSE;
}

BOOL CDiagramView::IsTechAnalysisKeyboradCmd(int nCmd)
{
	for(int i = 0; i < m_aTechAnalysisKeyboardCmd.GetCount(); i++)
	{
		if( nCmd == m_aTechAnalysisKeyboardCmd[i] )
			return TRUE;
	}
	return FALSE;
}

BOOL CDiagramView::IsFunctionKeyboradCmd(int nCmd)
{
	for(int i = 0; i < m_aFunctionKeyboardCmd.GetCount(); i++)
	{
		if( nCmd == m_aFunctionKeyboardCmd[i] )
			return TRUE;
	}
	return FALSE;
}

void CDiagramView::SetDataSource(IDataSourceEx* pDataSource,long nSourceID)
{
	this->m_pDataSource = pDataSource;
	this->m_nDataSourceID = nSourceID;
}

BOOL CDiagramView::HSDataSourceSink_OnCommNotify(void* pData)
{
	if (pData == NULL)
	{
		return FALSE;
	}

	CommNotify* pNotify = (CommNotify*)pData;
	
	if (pNotify->m_uType == eDataSource_ReConnect)
	{// 断线重连需要重新请求一遍

		if(m_pLayout)
		{
			CRequestCollector reqCollector;
			m_pLayout->GetRequestData(&reqCollector, &m_currentStockCode, TRUE);
			reqCollector.DoRequest(m_pDataSource,m_nDataSourceID);
		}
	}
	else if (pNotify->m_uType == eDataSource_Init)
	{// 服务器初始化 需要重新请求一下维护的股票列表
		
		SetNeighbourStockInfo(&m_currentStockCode);
		if (!m_neighbourStockCodes.IsEmpty())
			m_currentStockCode = m_neighbourStockCodes.GetAt(m_currentStockIndex);
		
		if (m_pLayout)
		{
			CRequestCollector reqCollector;
			m_pLayout->GetRequestData(&reqCollector, &m_currentStockCode, TRUE);
			reqCollector.DoRequest(m_pDataSource, m_nDataSourceID);
		}
	}

	return TRUE;
}

BOOL CDiagramView::HSDataSourceSink_OnRecvData(void* pData, int nLen)
{
	if( pData == NULL || nLen == 0 )
		return FALSE;
	DataHead* pHead = (DataHead*)pData;
	m_pLayout->OnUpdateData(pHead);
	return TRUE;
}

void CDiagramView::GetRequestData(StockUserInfo* pStockInfo)
{
	CRequestCollector reqCollector;
	if(&m_currentStockCode != pStockInfo )
	{
		m_currentStockCode.Copy(pStockInfo);
	}
	m_pLayout->GetRequestData(&reqCollector,pStockInfo);
	reqCollector.DoRequest(m_pDataSource,m_nDataSourceID);
	this->Invalidate();
}

void CDiagramView::SetNeighbourStockInfo(StockUserInfo* pTargetCode)
{
	if(pTargetCode == NULL)
		return;

	m_neighbourStockCodes.RemoveAll();
	CodeInfo* pCode = &pTargetCode->m_ciStockCode;
	CArray<StockUserInfo> stockArray;
	m_pDataSource->HSDataSourceEx_GetMarketStocksInfo(pCode->m_cCodeType,
		m_neighbourStockCodes);
	m_currentStockIndex = 0;
	for(int i = 0; i < m_neighbourStockCodes.GetCount(); i++)
	{
		CodeInfo* pTemp = &(m_neighbourStockCodes[i].m_ciStockCode);
		if(pTemp->m_cCodeType == pCode->m_cCodeType && pTemp->GetCode() == pCode->GetCode())
		{
			m_currentStockIndex = i;
			break;
		}
	}
}

BOOL CDiagramView::IsActiveDiagramLayer(CString strLayoutName)
{
	if(m_pLayout)
	{
		if( m_pLayout->GetLayoutName() == strLayoutName )
			return TRUE;
	}
	return FALSE;
}

BOOL CDiagramView::OnKeyBoardNotify(WPARAM wParam, LPARAM lParam)
{
	BOOL bDone = FALSE;
	if(m_pLayout)
	{
		OnKeyCommand(wParam,lParam);
		bDone =m_pLayout->OnKeyBoardNotify(wParam,lParam);
	}
	return bDone;
}

BOOL CDiagramView::OnKeyCommand(WPARAM wParam, LPARAM lParam)
{
	if(wParam != HSCurKeyInfo::KeyCommand) //特殊
		return FALSE;
	int nCmd = int(lParam);
	if(IsQuoteReprotKeyboradCmd(nCmd))
	{
		if(!IsActiveDiagramLayer(Layout_QuoteReport))
		{	
			StockUserInfo emptyCode;
			emptyCode.SetEmpty();
			ChangeDiagramLayer(Layout_QuoteReport,&emptyCode);
		}
	}
	else if( IsTechAnalysisKeyboradCmd(nCmd) )
	{
		if(!IsActiveDiagramLayer(Layout_TechAnalysis))
		{
			ChangeDiagramLayer(Layout_QuoteReport,NULL);
		}
	}
	else if( IsFunctionKeyboradCmd(nCmd) )
	{
		OnFunctionKeyboradCmd(nCmd);
	}
	return FALSE;
}

void CDiagramView::OnFunctionKeyboradCmd(int nCmd)
{
	switch(nCmd)
	{
	case KeyF1:
		OnF1();
		break;
	case KeyF2:
		OnF2();
		break;
	case KeyF3:
		OnF3();
		break;
	case KeyF4:
		OnF4();
		break;
	case KeyF5:
		OnF5();
		break;
	case KeyF6:
		OnF6();
		break;
	case KeyF7:
		OnF7();
		break;
	case KeyF8:
		OnF8();
		break;
	case KeyCtrlX:
		break;
	case KeyF10:
		OnF10();
		break;
	default:
		break;
	}
}

void CDiagramView::OpenFixedLayout(CString strFixedLayoutName)
{
	m_bFixedLayoutMode = TRUE;
	m_strFixedLayoutName = strFixedLayoutName;
	m_pLayout = new CDiagramLayer(strFixedLayoutName);
	m_pLayout->SetDataSource(m_pDataSource,m_nDataSourceID);
	m_pLayout->SetBizDialog(m_pHolder);
	
	
	strFixedLayoutName = m_LayerSelector.GetStandardFileName(strFixedLayoutName,NULL);
	m_pLayout->LoadLayout(this,strFixedLayoutName);
}

CDiagramLayer* CDiagramView::ChangeDiagramLayer(CString strLayoutName,StockUserInfo* pStockInfo)
{
	CodeInfo codeInfo;
	if(pStockInfo != NULL)
		codeInfo = pStockInfo->m_ciStockCode;
	else
		codeInfo = m_currentStockCode.m_ciStockCode;
	CDiagramLayer*  pLayout = NULL;
	if(strLayoutName == Layout_QuoteReport)
	{
		StockUserInfo emptyCode;
		emptyCode.SetEmpty();
		pLayout = m_LayerSelector.GetDiagramLayer(strLayoutName,&emptyCode.m_ciStockCode);
	}
	else
	{
		pLayout = m_LayerSelector.GetDiagramLayer(strLayoutName,&codeInfo);
	}

	if(pLayout == NULL)
		return NULL;
	//更改visable
	if(m_pLayout)
		m_pLayout->SetVisable(FALSE);//将原先显示的图层设置为隐藏
	m_pLayout = pLayout; 
	m_pLayout->SetVisable(TRUE);//设置现有图层属性为显示
	m_pLayout->SetDataSource(m_pDataSource,m_nDataSourceID);
	m_pLayout->SetBizDialog(m_pHolder);
	CRect rc;
	GetClientRect(&rc);
	m_pLayout->ResetRegionNodeActiveDiagram();
	m_pLayout->RecalcLayout(0,0,rc.Width(),rc.Height());

	this->Invalidate(FALSE);
	return m_pLayout;
}

StockUserInfo* CDiagramView::GetNeighbourStockCode(int nIndex)
{
	if( nIndex < 0 || nIndex >= m_neighbourStockCodes.GetCount() )
		return NULL;
	else
	{
		m_currentStockIndex = nIndex;
		return &m_neighbourStockCodes[nIndex];
	}
}


// CDiagramView 消息处理程序

void CDiagramView::OnDraw(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	if(m_pLayout)
	{
		CMemDC dc(*pDC,this);
		m_pLayout->OnDraw(&dc.GetDC());
	}
}

void CDiagramView::OnSize(UINT nType, int cx, int cy)
{
	if(m_pLayout == NULL)
		return;

	m_pLayout->RecalcLayout(0,0,cx,cy);
}

void CDiagramView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_pLayout == NULL)
		return;
	m_pLayout->OnMouseAction(CDiagramLayer::MouseMove,nFlags,point);
}

void CDiagramView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_pLayout == NULL)
		return;
	m_pLayout->OnMouseAction(CDiagramLayer::MouseDown,nFlags,point);
}


void CDiagramView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_pLayout == NULL)
		return;
	m_pLayout->OnMouseAction(CDiagramLayer::MouseUp,nFlags,point);
}

void CDiagramView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_pLayout == NULL)
		return;
	m_pLayout->OnMouseAction(CDiagramLayer::MouseDBDown,nFlags,point);
}

void CDiagramView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_pLayout == NULL)
		return;
	m_pLayout->OnMouseAction(CDiagramLayer::RButtonDown,nFlags,point);
}

BOOL CDiagramView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(m_pLayout == NULL)
		return FALSE;

	if(m_pLayout->OnMouseWheel(nFlags,zDelta,pt) != 0)
		return TRUE;

//	if(m_pLayout->IsLayout(Layout_TechAnalysis) )
	{
		if(zDelta > 0)
		{
			SendMessage(WM_KEYDOWN,(WPARAM)VK_PRIOR,0);
		}
		else
		{
			SendMessage(WM_KEYDOWN,(WPARAM)VK_NEXT,0);
		}
		return TRUE;
	}

	return FALSE;
}


BOOL CDiagramView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CDiagramView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWndUpdateLocker(this);

	int nRet = 0;
	if(m_pLayout)
		nRet = m_pLayout->OnKey(nChar,nRepCnt,nFlags);
	if(nRet)
		return;

	if( ((CDlgDiagramHolder*)m_pHolder)->IsFixedMode() )
		return;

	if(nChar == VK_ESCAPE)
	{
		OnEsc();
	}
	else if ( nChar == VK_PRIOR  )//page up
	{
		OnPrior();
	}
	else if( nChar == VK_NEXT  )//page down
	{
		OnNext();
	}
	else if(nChar == VK_F1)
	{
		OnF1();
	}
	else if(nChar == VK_F2)
	{
		OnF2();	
	}
	else if(nChar == VK_F3)
	{
		OnF3();	
	}
	else if(nChar == VK_F4)
	{
		OnF4();	
	}
	else if(nChar == VK_F5)
	{
		OnF5();	
	}
	else if(nChar == VK_F6)
	{
		OnF6();	
	}
	else if(nChar == VK_F7)
	{
		OnF7();
	}
	else if(nChar == VK_F8)
	{
		OnF8();
	}
}

void CDiagramView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWndUpdateLocker(this);

	if( ((CDlgDiagramHolder*)m_pHolder)->IsFixedMode() )
		return;

	if(nChar == VK_F10)
	{	
		OnF10();
	}
	if(m_pLayout)
		m_pLayout->OnKey(nChar,nRepCnt,nFlags);
}

void CDiagramView::OnPrior()
{
	StockUserInfo* pStkInfo = GetNeighbourStockCode(m_currentStockIndex -1);
	if(pStkInfo)
	{
		this->GetRequestData(pStkInfo);
	}
}

void CDiagramView::OnNext()
{
	StockUserInfo* pStkInfo = GetNeighbourStockCode(m_currentStockIndex + 1);
	if(pStkInfo)
	{
		this->GetRequestData(pStkInfo);
	}
}

void CDiagramView::OnEsc()
{
	if(!m_pLayout->IsLayout(Layout_QuoteReport))
	{
		StockUserInfo emptyCode;
		emptyCode.SetEmpty();
		ChangeDiagramLayer(Layout_QuoteReport,&emptyCode);
		GetRequestData(NULL);
	}
}

void CDiagramView::OnF1()
{
	if(!IsActiveDiagramLayer(Layout_DealDetail))
	{
		if(m_currentStockCode.GetName().IsEmpty() || IsActiveDiagramLayer(Layout_QuoteReport))
			// 当前代码为空 或者 当前是行情报价界面
			m_pLayout->OnKey(VK_F1,0,0);
		else
		{
			ChangeDiagramLayer(Layout_DealDetail,NULL);
			GetRequestData(&m_currentStockCode);
		}
	}
	else
	{	
		ChangeDiagramLayer(Layout_TechAnalysis,NULL);
		GetRequestData(&m_currentStockCode);
	}	
}
void CDiagramView::OnF2()
{

	if(!IsActiveDiagramLayer(Layout_PriceBook))
	{
		if(m_currentStockCode.GetName().IsEmpty() || IsActiveDiagramLayer(Layout_QuoteReport))
			// 当前代码为空 或者 当前是行情报价界面
			m_pLayout->OnKey(VK_F2,0,0);
		else
		{
			ChangeDiagramLayer(Layout_PriceBook,NULL);
			GetRequestData(&m_currentStockCode);
		}
	}
	else
	{	
		ChangeDiagramLayer(Layout_TechAnalysis,NULL);
		GetRequestData(&m_currentStockCode);
	}	
}

void CDiagramView::OnF3()
{
	CodeInfo code;
	code.m_cCodeType = STOCK_MARKET|SH_BOURSE;
	strncpy(code.m_cCode,_T("1A0001"),6);
	StockUserInfo stockUserInfo;
	if( !m_pDataSource->HSDataSourceEx_GetStockUserInfo(&code,stockUserInfo) )
		return;
	ChangeDiagramLayer(Layout_TechAnalysis,&stockUserInfo);
	GetRequestData(&stockUserInfo);
}

void CDiagramView::OnF4()
{
	CodeInfo code;
	code.m_cCodeType = STOCK_MARKET|SZ_BOURSE;
	strncpy(code.m_cCode,_T("2A01"),6);
	StockUserInfo stockUserInfo;
	if( !m_pDataSource->HSDataSourceEx_GetStockUserInfo(&code,stockUserInfo) )
		return;
	ChangeDiagramLayer(Layout_TechAnalysis,&stockUserInfo);
	GetRequestData(&stockUserInfo);
}

void CDiagramView::OnF5()
{
	if(!IsActiveDiagramLayer(Layout_TechAnalysis))
	{
		if(m_currentStockCode.GetName().IsEmpty() || IsActiveDiagramLayer(Layout_QuoteReport))
			// 当前代码为空 或者 当前是行情报价界面
			m_pLayout->OnKey(VK_F5,0,0);
		else
		{
			ChangeDiagramLayer(Layout_TechAnalysis,NULL);
			GetRequestData(&m_currentStockCode);
		}
	}
	m_pLayout->OnKey(VK_F5,0,1);//指定打开分时
}

void CDiagramView::OnF6()
{
	if(!IsActiveDiagramLayer(Layout_QuoteReport))
	{
		ChangeDiagramLayer(Layout_QuoteReport,NULL);
	}
	m_pLayout->OnKey(VK_F6,0,0);
}

void CDiagramView::OnF7()
{
	if(m_pLayout)
		m_pLayout->OnKey(VK_F7,0,0);
}

void CDiagramView::OnF8()
{
	if(m_pLayout)
		m_pLayout->OnKey(VK_F8,0,0);
}

void CDiagramView::OnF10()
{
	if(!IsActiveDiagramLayer(Layout_F10Info))
	{
		if(m_currentStockCode.GetName().IsEmpty() || IsActiveDiagramLayer(Layout_QuoteReport))
			// 当前代码为空 或者 当前是行情报价界面
			m_pLayout->OnKey(VK_F10,0,0);
		else
		{
			ChangeDiagramLayer(Layout_F10Info,NULL);
			GetRequestData(&m_currentStockCode);
		}
	}
}


LRESULT CDiagramView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CView::WindowProc(message, wParam, lParam);
}
