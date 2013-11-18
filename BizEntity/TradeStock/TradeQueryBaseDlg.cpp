/*******************************************************
  源程序名称:TradeQueryBase.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  查询窗口基类
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#include "stdafx.h"
#include "TradeQueryBaseDlg.h"
#include "HSMessage.h"

#define _GRID_ID_ 100
#define  IDC_POPUPMENU_BEGIN		1000
#define  IDC_POPUPMENU_END			1100

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CTradeQueryBaseDlg, CTradeDialogBase)

CTradeQueryBaseDlg::CTradeQueryBaseDlg(CWnd* pParent /* = NULL */)
: CTradeDialogBase(CTradeQueryBaseDlg::IDD, pParent)
{
	m_pTradeTable = NULL;
	m_pGrid = NULL;
	m_pDataList = NULL;
	m_nQueryBegin = 0;
	m_EntrustStatusList = NULL;
}

CTradeQueryBaseDlg::~CTradeQueryBaseDlg()
{
	//m_pAccount->GetTradeDataCore()->CancelSubscribe(this);
	if (m_pTradeTable)
	{
		m_pTradeTable->Release();
	}
	if (m_pGrid)
	{
		delete m_pGrid;
	}
	/*if (m_pDataList)
	{
		m_pDataList->Release();
	}*/
	if (m_EntrustStatusList)
	{
		m_EntrustStatusList->Release();
	}
	UnregisterLinkageMsg(TRADE_MESSAGE_REFRESH);	
	UnregisterLinkageMsg(TRADE_MESSAGE_WITHDRAW);
}

BOOL CTradeQueryBaseDlg::OnInit()
{
	if (__super::OnInit())
	{
		//TODO:
		RegisterLinkageMsg(TRADE_MESSAGE_REFRESH);
		RegisterLinkageMsg(TRADE_MESSAGE_WITHDRAW);

		m_EntrustStatusList = m_pAccountParamList->GetEntrustStatusList();
		if (NULL == m_pTradeTable)
		{
			m_pTradeTable = m_pTradeCoreObj->NewColumsByAtionEx(m_iAction);
		}
		if (NULL == m_pGrid)
		{
			m_pGrid = new CTradeGridBase();
			m_pGrid->Create(CRect(0,0,0,0),this,_GRID_ID_,WS_CHILD | WS_VISIBLE & ~WS_BORDER);
			m_pGrid->FillHeader(m_pTradeTable);			
			SetSize();			
		}
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeQueryBaseDlg::OnShow(BOOL bShow)
{
	if (bShow)
	{
		m_pAccount->GetTradeDataCore()->Subscribe(this);
		AsyncQuery();
	}
	else
	{
		m_pAccount->GetTradeDataCore()->CancelSubscribe(this);
	}
}

void CTradeQueryBaseDlg::OnLinkageMsg(CLinkageParam* pParam)
{
	switch (pParam->m_nMsg)
	{
	case TRADE_MESSAGE_REFRESH:
		{
			if (IsShow())
				AsyncQuery();
			break;
		}
	case TRADE_MESSAGE_WITHDRAW:
		{
			if(IsShow())
				DoPopupMenuEntrust();
			break;
		}
		
	}
}

void CTradeQueryBaseDlg::TradingAsyncCallBack(ITrading* pTrading, int iAction)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pTrading->GetSuccessful() && pTrading->HaveData())
	{
		/*if (m_pDataList)
		{
			m_pDataList->Release();
		}
		m_pDataList = pTrading->GetDataList();
		FillGrid();*/
		DoDataCore(pTrading->GetDataList());
		SendCount(pTrading->GetDataList()->GetCount());
	}
	else
	{
		pTrading->FreeDataList();
		m_pGrid->SetRowCount(1);
		SendCount(0);
	}
		
}

void CTradeQueryBaseDlg::SetSize()
{
	CRect rcClient;
	GetClientRect(rcClient);
	if (m_pGrid
		&& m_pGrid->GetSafeHwnd()
		&& m_pGrid->IsWindowEnabled())
	{
		m_pGrid->MoveWindow(rcClient);
	}
}

void CTradeQueryBaseDlg::DoGridDBClick()
{
	int iRow = m_pGrid->GetCurrRow();
	CString sMsg = "";
	if ((iRow > 0) && (m_pGrid->GetRowCount() > 1))
	{
		LPARAM pParam = m_pGrid->GetItemData(iRow, 0);
		if (pParam)
		{
			IDataItem* pItem = (IDataItem*)pParam;
			CString sStockCode = pItem->GetStrByName(STOCKCODE);
			sStockCode.Trim();
			if (!sStockCode.IsEmpty())
			{
				CLinkageParam param(TRUE);
				param.AddParam(_T(STOCKCODE), sStockCode);
				SendLinkageMsg(TRADE_MESSAGE_STOCKCODE,&param);
			}
		}
	}
}

void CTradeQueryBaseDlg::AsyncQuery()
{
	DWORD nQueryEnd = GetTickCount();
	if ((nQueryEnd - m_nQueryBegin) > (5 * 1000))
	{
		m_nQueryBegin = nQueryEnd;
		ITrading* pTrading = m_pTradeCoreObj->NewTradingByAction(m_iAction);
		pTrading->Clear();	
		pTrading->SetAccount(m_pAccount);
		pTrading->TradingAsync(this);
	}
}

void CTradeQueryBaseDlg::FillGrid()
{
	if (IsShow())
	{
		m_pGrid->FillGrid(m_pDataList);
	}
	
}

void CTradeQueryBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//if (m_pGrid)
	//{
	//	DDX_Control(pDX, _GRID_ID_, *m_pGrid);
	//}
}

BEGIN_MESSAGE_MAP(CTradeQueryBaseDlg, CTradeDialogBase)
	//
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, _GRID_ID_, &CTradeQueryBaseDlg::OnGridRClick)
	ON_NOTIFY(NM_DBLCLK, _GRID_ID_, &CTradeQueryBaseDlg::OnGridDBClick)
	ON_COMMAND_RANGE(IDC_POPUPMENU_BEGIN, IDC_POPUPMENU_END, &CTradeQueryBaseDlg::OnClickPopupMenu)
END_MESSAGE_MAP()


void CTradeQueryBaseDlg::OnSize(UINT nType, int cx, int cy)
{
	CHSBizDialog::OnSize(nType, cx, cy);
	SetSize();
}

void CTradeQueryBaseDlg::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	//AfxMessageBox("Right Click");
	DoGridRClick();
}

void CTradeQueryBaseDlg::OnGridDBClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	DoGridDBClick();
}

//BOOL CTradeQueryBaseDlg::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
//{
//	return CTradeDialogBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//}



//LRESULT CTradeQueryBaseDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CTradeDialogBase::WindowProc(message, wParam, lParam);
//}

//BOOL CTradeQueryBaseDlg::OnInitDialog()
//{
//	CTradeDialogBase::OnInitDialog();
//
//	// TODO:  Add extra initialization here
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//	// EXCEPTION: OCX Property Pages should return FALSE
//}
DWORD CTradeQueryBaseDlg::GetDataType()
{
	return 0;
}

void CTradeQueryBaseDlg::OnReflash(IDataList * pDataList, DWORD dDataType)
{
	if (m_pDataList != pDataList)
	{
		m_pDataList = pDataList;
		FillGrid();
	}	
}

void CTradeQueryBaseDlg::OnChange(IDataItem * pDataItem, EDataChangeType dtChangeType, DWORD dDataType)
{

}

void CTradeQueryBaseDlg::DoDataCore(IDataList * pDataList)
{

}

void CTradeQueryBaseDlg::DoGridRClick()
{	
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	/*CCellID cell = m_pGrid->GetCellFromPt(point);
	if ((cell.row >= 0) && (cell.row > m_pGrid->GetFixedRowCount()) && (m_pGrid->GetCurrRow() > 0) && m_pXMLDataDoc)*/
	if ((m_pGrid->GetCurrRow() > 0) && m_pXMLDataDoc)
	{		
		TiXmlElement* pRoot = m_pXMLDataDoc->FirstChildElement("PopupMenu");
		if (pRoot)
		{
			CMenu popMenu;
			popMenu.CreatePopupMenu();
			TiXmlElement* pNode = pRoot->FirstChildElement("Item");
			while(pNode)
			{
				CString sCaption = pNode->Attribute(_T("caption"));
				CString sID = pNode->Attribute(_T("id"));
				int nID = atoi((LPCTSTR)sID);
				popMenu.AppendMenu(MF_STRING, nID, sCaption);
				pNode = pNode->NextSiblingElement();
			}
			ClientToScreen(&point);
			popMenu.TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
			popMenu.DestroyMenu();
		}
	}	
}

void CTradeQueryBaseDlg::OnClickPopupMenu(UINT Id)
{
	if (m_pXMLDataDoc)
	{		
		TiXmlElement* pRoot = m_pXMLDataDoc->FirstChildElement("PopupMenu");
		if (pRoot)
		{
			TiXmlElement* pNode = pRoot->FirstChildElement("Item");
			while(pNode)
			{
				CString sID = pNode->Attribute(_T("id"));
				int nID = atoi(sID);
				if (Id == nID)
				{
					CString sEntrustFlag = pNode->Attribute(_T("entrustflag"));
					if ("1" == sEntrustFlag)
					{
						DoPopupMenuEntrust();
					}
					else
					{
						CString sOpenLayout = pNode->Attribute(_T("openlayout"));
						OpenLayOutMsg(sOpenLayout);
					}
					
					//MessageBoxA(sOpenLayout, "提示");
					break;
				}
				pNode = pNode->NextSiblingElement();
			}
		}
	}	
}

void CTradeQueryBaseDlg::OpenLayOutMsg(CString sLayout)
{
	//this->SendLinkageMsg(TRADE_MESSAGE_OPENLAYOUT, 0, sLayout);
	CLinkageParam param;
	param.AddParam(_T("layout"),sLayout);
	int iRow = m_pGrid->GetCurrRow();
	if (iRow > 0)
	{
		LPARAM pParam = m_pGrid->GetItemData(iRow, 0);
		if (pParam)
		{
			IDataItem* pItem = (IDataItem*)pParam;
			CString sStockCode = pItem->GetStrByName(STOCKCODE);
			if (!sStockCode.IsEmpty())
			{
				param.AddParam(_T(STOCKCODE), sStockCode);
				SendLinkageMsg(TRADE_MESSAGE_OPENLAYOUT,&param);
			}
		}
	}
}

void CTradeQueryBaseDlg::DoPopupMenuEntrust()
{

}

void CTradeQueryBaseDlg::SendCount(int nCount)
{
	if (IsShow())
	{
		CLinkageParam param;
		CString sValue;
		sValue.Format("%d", nCount);
		param.AddParam(TRADE_PARAM_COUNT, sValue);
		//SendLinkageMsg(TRADE_MESSAGE_QUERYCOUNT, &param);
	}
}