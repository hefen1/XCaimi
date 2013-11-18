// DlgCombAnalysisReport.cpp : 实现文件
//

#include "stdafx.h"
#include "DiagramCombAnalysis.h"
#include "DlgCombAnalysisReport.h"
#include "WinnerApplication.h"
#include "DlgWatcherParams.h"
//
#include "ConfigInterface.h"
#include ".\QuoteComm\QuoteDefine.h"
#include ".\CombComm\CombManagerInterface.h"
#include ".\CombComm\IndicatrixObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDlgCombAnalysisReport 对话框

IMPLEMENT_DYNAMIC(CDlgCombAnalysisReport, CHSBizDialog)

CDlgCombAnalysisReport::CDlgCombAnalysisReport(CWnd* pParent /*=NULL*/)
	: CHSBizDialog(CDlgCombAnalysisReport::IDD, pParent)
{
	m_pCombGrid = NULL;
	m_pAnalysisTable = NULL;
}

CDlgCombAnalysisReport::~CDlgCombAnalysisReport()
{

	if(m_pCombGrid)
		delete m_pCombGrid;

	if(m_pAnalysisTable)
		m_pAnalysisTable->Release();
}

void CDlgCombAnalysisReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CDlgCombAnalysisReport::OnInitDialog()
{
	m_pCombGrid = new CYTGridCtrl();
	InitAnalysisGridStyle();
	if( m_pCombGrid->Create(CRect(0,0,0,0),this,Report_Grid_ID,WS_CHILD | WS_VISIBLE ) )
	{
		m_pCombGrid->ModifyStyleEx(WS_EX_CLIENTEDGE,0,0);
	}
	return FALSE;
}

BOOL CDlgCombAnalysisReport::OnInit()
{
	m_pWatcherInfoMgr = (IWatcherInfoManager*)CWinnerApplication::GetObject(OJB_WATCHERINFOMANAGER);
	m_strWatcherSetName = m_pParam->GetStringParam("watch_type");
	CString strIndicatrixTable = m_pParam->GetStringParam("indicatrix_table");
	IAnalysisTableSet *pTableSet = (IAnalysisTableSet*)CWinnerApplication::GetObject(OJB_AnalysisTableSet);
	if(pTableSet)
	{
		m_pAnalysisTable = pTableSet->GetTable(strIndicatrixTable);
	}
	if(m_pAnalysisTable == NULL)
	{
		CString strMsg;
		strMsg.Format(_T("取指标集【%s】信息失败，请检查配置信息."),strIndicatrixTable);
		MessageBox(strMsg,_T("错误"),MB_OK);
	}

	m_watcherSet.InitWatcherSet(m_strWatcherSetName);

	//初始监控Grid
	InitAnalysisGridData();
	GetCombCacheSet()->RegisterCombCacheNotifier(&m_watcherSet);
	GetCombCacheSet()->StartCalculateWork();

	SetTimer(1,   500,   0); 
	return TRUE;
}


BEGIN_MESSAGE_MAP(CDlgCombAnalysisReport, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK,Report_Grid_ID,OnAnalysisGridRClick)
	ON_NOTIFY(NM_DBLCLK,Report_Grid_ID,OnAnalysisGridDBClick)
END_MESSAGE_MAP()


// CDlgCombAnalysisReport 消息处理程序

void CDlgCombAnalysisReport::OnSize(UINT nType, int cx, int cy)
{
	CHSBizDialog::OnSize(nType, cx, cy);
	if(m_pCombGrid)
		m_pCombGrid->MoveWindow(0,0,cx,cy);
}

void CDlgCombAnalysisReport:: InitAnalysisGridStyle()
{
	if(m_pCombGrid == NULL)
		return;
	
	m_pCombGrid->SetEditable(FALSE);
	m_pCombGrid->SetRowResize(FALSE);
	m_pCombGrid->SetListMode();//将Grid设置成排序模式
	m_pCombGrid->SetGridLines(GVL_NONE);
	m_pCombGrid->SetDoubleBuffering(TRUE);
	m_pCombGrid->SetFrameFocusCell(FALSE);
	m_pCombGrid->EnableTitleTips(FALSE);

	// 设置单元格字体
	IHsFont*    pFontProt = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
	LOGFONT logfont;
	CFont* pFont = (CFont*)pFontProt->GetQuoteColHeadFont();
	if (pFont)
	{
		ZeroMemory(&logfont,sizeof(LOGFONT));
		pFont->GetLogFont(&logfont);
		m_pCombGrid->GetDefaultCell(TRUE,FALSE)->SetFont(&logfont);
		m_pCombGrid->GetDefaultCell(FALSE,TRUE)->SetFont(&logfont);
		m_pCombGrid->GetDefaultCell(TRUE,TRUE)->SetFont(&logfont);
	}
	pFont = (CFont*)pFontProt->GetQuoteDynamicFont();
	if (pFont)
	{
		ZeroMemory(&logfont,sizeof(LOGFONT));
		pFont->GetLogFont(&logfont);
		m_pCombGrid->GetDefaultCell(FALSE,FALSE)->SetFont(&logfont);
	}

	// 设置单元格背景
	IHsColor*  pColorProt = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	m_pCombGrid->SetBkColor(pColorProt->GetBlackgroundColor());
	//背景色
	COLORREF clrFixedRowColBK = pColorProt->GetQuoteFixedRowColBKColor();
	COLORREF clrFixedRowBK = pColorProt->GetQuoteFixedRowBKColor(); 
	COLORREF clrFixedColBK = pColorProt->GetQuoteFixedColBKColor();
	COLORREF clrBlackground = pColorProt->GetBlackgroundColor();
	m_pCombGrid->GetDefaultCell(TRUE,TRUE)->SetBackClr(clrFixedRowColBK);//固定行列背景
	m_pCombGrid->GetDefaultCell(TRUE,FALSE)->SetBackClr(clrFixedRowBK);//固定行背景
	m_pCombGrid->GetDefaultCell(FALSE,TRUE)->SetBackClr(clrFixedColBK);////固定列背景
	m_pCombGrid->GetDefaultCell(FALSE,FALSE)->SetBackClr(clrBlackground); //非固定单元格
	//前景色
	COLORREF clrFixedCodeColor = pColorProt->GetFixedCodeColor();
	COLORREF clrFixedNameColor = pColorProt->GetFixedNameColor();
	COLORREF clrEqualColor = pColorProt->GetEqualColor();
	m_pCombGrid->GetDefaultCell(TRUE,TRUE)->SetTextClr(clrFixedCodeColor);
	m_pCombGrid->GetDefaultCell(TRUE,FALSE)->SetTextClr(clrFixedCodeColor);
	m_pCombGrid->GetDefaultCell(FALSE,TRUE)->SetTextClr(clrFixedNameColor);
	m_pCombGrid->GetDefaultCell(FALSE,FALSE)->SetTextClr(clrEqualColor); //非固定单元格

	m_pCombGrid->SetDefCellHeight(25);
	m_pCombGrid->SetDefCellMargin(2);	
 }

void CDlgCombAnalysisReport::InitAnalysisGridData()
{
	if(m_pAnalysisTable == NULL)
		return;
	//初始列
	m_pCombGrid->SetColumnCount(m_pAnalysisTable->GetColumnCount());
	m_pCombGrid->SetRowCount(1);
	m_pCombGrid->SetFixedRowCount(1);
	m_pCombGrid->SetFixedColumnCount(1);
	CClientDC dc(this);
	short tmLen = (short)dc.GetTextExtent("0").cx;
	for(int i = 0 ; i < m_pAnalysisTable->GetColumnCount(); i++)
	{
		IAnalysisColumn* pCol =  m_pAnalysisTable->GetColumn(i);
		CString strTmp = pCol->GetDispName();
		int nLen = strTmp.GetLength();
		GV_ITEM Item;
		Item.mask = GVIF_TEXT | GVIF_FORMAT;
		Item.row =0;
		Item.col = i;
		Item.strText = strTmp;
		Item.nFormat = DT_CENTER|DT_VCENTER;
		m_pCombGrid->SetItem(&Item);	
		m_pCombGrid->SetColumnWidth(i, pCol->GetColWidth());
		m_pCombGrid->SetItemData(0,i,(LPARAM)pCol);
	}

	//初始行
	m_pCombGrid->SetRowCount(m_watcherSet.GetCount() + 1);
	for( int i = 0; i < m_watcherSet.GetCount(); i++ )
	{
		CWatcher* pWatcher = m_watcherSet.GetWatcher(i);
		m_pCombGrid->SetItemText(i + 1,0,pWatcher->GetName());
		m_pCombGrid->SetItemData(i + 1,0,(LPARAM)pWatcher);
		//
		for(int j= 0 ; j < m_pAnalysisTable->GetColumnCount(); j++)
		{
			IAnalysisColumn* pCol =  m_pAnalysisTable->GetColumn(j);
			IIndicatrixObj *pIndicatrixObj = (IIndicatrixObj*)CWinnerApplication::GetObject(OBJ_INDICATRIXOBJ);
			CString strExp = pCol->GetExpression();
			if(!strExp.IsEmpty())
			{
				CMapStringToString mapParams;
				IWatcherParams* pWatcherParams= pWatcher->GetParams();
				pIndicatrixObj->SetIndicatrixName(pCol->GetName());
				pIndicatrixObj->BuildExpressionObj(strExp,pWatcherParams);
				pWatcher->AddIndicatrixObj(pCol->GetName(),pIndicatrixObj);
			}
		}
		pWatcher->LoadLastIndicatriData();
	}
}

void CDlgCombAnalysisReport::ReflashGrid()
{
	IHsColor*  pColorProt = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	m_pCombGrid->SetSelectClr(pColorProt->GetQuoteHighLightBKColor());
	for( int i = 0; i < m_watcherSet.GetCount(); i++ )
	{
		CWatcher* pWatcher = m_watcherSet.GetWatcher(i);
		for(int j = 0 ; j< m_pAnalysisTable->GetColumnCount(); j++)
		{
			IAnalysisColumn* pCol =  m_pAnalysisTable->GetColumn(j);
			CString strColName = pCol->GetName();
			CString strExp = pCol->GetExpression();
			if( (!strColName.IsEmpty()) && (!strExp.IsEmpty()) )
			{
				double dValue = pWatcher->GetLastIndicatrixObjValue(strColName);
				CString strDispFormat = pCol->GetDispFormat();
				CString strValue;
				if(!strDispFormat.IsEmpty())
					strValue.Format(strDispFormat,dValue);	
				else
					strValue.Format("%.3f",dValue);

				GV_ITEM Item;
				Item.mask = GVIF_TEXT | GVIF_FORMAT;
				Item.row = i + 1;
				Item.col = j;
				Item.strText = strValue;
				Item.nFormat = DT_RIGHT|DT_VCENTER;
				m_pCombGrid->SetItem(&Item);			
			}
		}
	}
	m_pCombGrid->Invalidate();
	NotifyDragramRefresh();
}

void CDlgCombAnalysisReport::NotifyDragramRefresh()
{
	CString strDiagramMsgId;
	strDiagramMsgId.Format("%d",DiagramMsg_Refresh_Indicatrix_Line);

	CLinkageParam param;
	param.m_bPost = TRUE;
	param.m_nMsg = QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT;
	//param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_CombAnalysis); // 通知的图层
	param.AddParam(PARAM_DIAGRAMMSGID,strDiagramMsgId); // 通知的图元系统消息号
	SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT,&param);
}

void CDlgCombAnalysisReport::NotifyWatcherChanged(CWatcher* pWatcher)
{
	CString strDiagramMsgId;
	strDiagramMsgId.Format("%d",DiagramMsg_WatcherObj_Change);
	CString strWatcherObj;
	strWatcherObj.Format("%d",pWatcher);

	CLinkageParam param;
	param.m_bPost = TRUE;
	param.m_nMsg = QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT;
	param.AddParam(PARAM_DIAGRAMMSGID,strDiagramMsgId); // 通知的图元系统消息号
	param.AddParam(Param_WatcherObj,strWatcherObj); // 通知的图层
	param.AddParam(Param_Change_Flag,Param_Change_Flag_Del); 
	SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT,&param);
}


void CDlgCombAnalysisReport::OnDestroy()
{
	__super::OnDestroy();
	m_watcherSet.Clear();
	GetCombCacheSet()->StartCalculateWork(FALSE);
}

void CDlgCombAnalysisReport::OnTimer(UINT_PTR nIDEvent)
{
	ReflashGrid();
	__super::OnTimer(nIDEvent);
}

void CDlgCombAnalysisReport::OnAnalysisGridRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu popMenu;
	popMenu.CreatePopupMenu();
	popMenu.AppendMenu(MF_STRING | MF_BYPOSITION,GRID_MENU_AddWatcher,_T("增加监控项"));
	popMenu.AppendMenu(MF_STRING | MF_BYPOSITION,GRID_MENU_ModifyWatcher,_T("修改监控项"));
	popMenu.AppendMenu(MF_STRING | MF_BYPOSITION,GRID_MENU_DelWathcher,_T("删除监控项"));
	popMenu.AppendMenu(MF_STRING | MF_BYPOSITION|MF_SEPARATOR,0,_T("_"));
	popMenu.AppendMenu(MF_STRING | MF_BYPOSITION,GRID_MENU_Refresh,_T("刷新"));

	NM_GRIDVIEW * pMNGird = (NM_GRIDVIEW*)pNMHDR;
	CPoint point(0,0);
	::GetCursorPos(&point);
	int nMenuID = popMenu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,point.x,point.y, this);
	if(nMenuID == GRID_MENU_AddWatcher)
	{
		IWatcherInfo* pWatcherInfo = m_pWatcherInfoMgr->CreateWathcerInfo();
		CDlgWatcherParams dlg;
		dlg.SetAnalysisTableObj(m_pAnalysisTable);
		dlg.m_pWatcherInfo = pWatcherInfo;
		if( dlg.DoModal() == IDOK)
		{
			//保存到文件
			m_pWatcherInfoMgr->AddWatcherInfo(pWatcherInfo,m_strWatcherSetName);
			AddWatcher(pWatcherInfo);
		}
		else
			pWatcherInfo->Release();
	}
	else if(nMenuID == GRID_MENU_ModifyWatcher)
	{
		CWatcher* pWatcher = (CWatcher*)m_pCombGrid->GetItemData(pMNGird->iRow,0);
		CDlgWatcherParams dlg;
		dlg.SetAnalysisTableObj(m_pAnalysisTable);
		dlg.m_pWatcherInfo = pWatcher->GetWatcherInfo();
		if ( dlg.DoModal() == IDOK )
		{
			m_watcherSet.ModifyWatcher(pWatcher);
			m_pWatcherInfoMgr->ModifyWatcherInfo(pWatcher->GetWatcherInfo(),m_strWatcherSetName);
		}
	}
	else if(nMenuID == GRID_MENU_DelWathcher)
	{
		//保存到文件
		CWatcher* pWatcher = (CWatcher*)m_pCombGrid->GetItemData(pMNGird->iRow,0);
		m_pWatcherInfoMgr->DelWatcherInfo(pWatcher->GetName(),m_strWatcherSetName);
		RemoveWatcher(pMNGird->iRow);
		ReflashGrid();
	}
	else if( nMenuID == GRID_MENU_Refresh )
	{
		GetCombCacheSet()->DoCalculateWork(TRUE);
		ReflashGrid();
	}
}

void CDlgCombAnalysisReport::OnAnalysisGridDBClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW * pMNGird = (NM_GRIDVIEW*)pNMHDR;
	if(pMNGird->iRow > 0)
		NotifySingleIndicatrixChange(pMNGird);
	else if(pMNGird->iRow == 0)
	{
		NotifyMultiIndicatrixChange(pMNGird);
	}	
}

void CDlgCombAnalysisReport::NotifySingleIndicatrixChange(NM_GRIDVIEW * pMNGird)
{
	if(pMNGird->iRow <=0  || pMNGird->iColumn <=0 )
		return;
	CWatcher* pWatcher =  (CWatcher*)m_pCombGrid->GetItemData(pMNGird->iRow,0);
	if(pWatcher == NULL)
		return;
	IAnalysisColumn* pCol =  (IAnalysisColumn*)m_pCombGrid->GetItemData(0,pMNGird->iColumn);
	if(pCol == NULL)
		return;

	CString strIndicatrix = pCol->GetName();
	IIndicatrixObj* pObj = pWatcher->GeIndicatrixOb(strIndicatrix);

	CString strDiagramMsgId,strObjAddress,strLineName;
	strDiagramMsgId.Format("%d",DiagramMsg_Comb_Indicatrix_Sel);
	strObjAddress.Format("%d",pObj);
	strLineName=pWatcher->GetName() + _T("-") + pCol->GetDispName();

	CLinkageParam param;
	param.m_bPost = FALSE;
	param.m_nMsg = QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT;
	param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_CombAnalysis); // 通知的图层
	param.AddParam(PARAM_DIAGRAMMSGID,strDiagramMsgId); // 通知的图元系统消息号
	param.AddParam(Param_Comb_IndicatrixObj,strObjAddress);
	param.AddParam(Param_Indicatrix_Name,strLineName);
	SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT,&param);
}

void CDlgCombAnalysisReport::NotifyMultiIndicatrixChange(NM_GRIDVIEW * pMNGird)
{
	if(pMNGird->iRow !=0  || pMNGird->iColumn <=0 )
		return;

	IAnalysisColumn* pCol =  (IAnalysisColumn*)m_pCombGrid->GetItemData(0,pMNGird->iColumn);
	if(pCol == NULL)
		return;

	CString strDiagramMsgId,strObjAddress;
	strDiagramMsgId.Format("%d",DiagramMsg_Multi_Comb_Indicatrix_Sel);
	strObjAddress.Format("%d",&m_watcherSet);
	CLinkageParam param;
	param.m_bPost = FALSE;
	param.m_nMsg = QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT;
	param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_MultiCombAnalysis); // 通知的图层
	param.AddParam(PARAM_DIAGRAMMSGID,strDiagramMsgId); // 通知的图元系统消息号
	param.AddParam(Param_WatchSetObj,strObjAddress);
	param.AddParam(Param_Indicatrix_Name,pCol->GetName());
	param.AddParam(Param_Indicatrix_DispName,pCol->GetDispName());
	SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT,&param);
}

void CDlgCombAnalysisReport::AddWatcher(IWatcherInfo* pWatcherInfox)
{
	CWatcher* pWatcherObj = new CWatcher();
	pWatcherObj->SetWatcherInfo(pWatcherInfox);
	
	for(int j= 0 ; j < m_pAnalysisTable->GetColumnCount(); j++)
	{
		IAnalysisColumn* pCol =  m_pAnalysisTable->GetColumn(j);
		IIndicatrixObj *pIndicatrixObj = (IIndicatrixObj*)CWinnerApplication::GetObject(OBJ_INDICATRIXOBJ);
		CString strExp = pCol->GetExpression();
		if(!strExp.IsEmpty())
		{
			CMapStringToString mapParams;
			IWatcherParams* pWatcherParams= pWatcherObj->GetParams();
			pIndicatrixObj->SetIndicatrixName(pCol->GetName());
			pIndicatrixObj->BuildExpressionObj(strExp,pWatcherParams);
			pWatcherObj->AddIndicatrixObj(pCol->GetName(),pIndicatrixObj);
		}
	}
	int nRow = m_pCombGrid->InsertRow(pWatcherObj->GetName());
	m_pCombGrid->SetItemData(nRow,0,(LPARAM)pWatcherObj);
	m_watcherSet.AddWatcher(pWatcherObj);
}

void CDlgCombAnalysisReport::RemoveWatcher(int nRow)
{
	CWatcher* pWatcher = (CWatcher*)m_pCombGrid->GetItemData(nRow,0);
	if(pWatcher)
	{
		NotifyWatcherChanged(pWatcher);
		m_pCombGrid->DeleteRow(nRow);
		m_watcherSet.RemoveWatcher(pWatcher);
	}
}

