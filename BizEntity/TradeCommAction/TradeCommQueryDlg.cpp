/*******************************************************
  源程序名称:TradeCommQueryDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  查询窗口基类
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#include "stdafx.h"
#include "TradeCommQueryDlg.h"
#include "HSMessage.h"

#define _GRID_ID_ 100

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(TradeCommQueryDlg, CTradeDialogBase)

TradeCommQueryDlg::TradeCommQueryDlg(CWnd* pParent /* = NULL */)
: CTradeDialogBase(TradeCommQueryDlg::IDD, pParent)
{
	m_pTradeTable = NULL;
	m_pGrid = NULL;
	m_pDataList = NULL;
	m_nQueryBegin = 0;
	m_sBeginDate = "";
	m_sEndDate = "";
	CTime tTime = CTime::GetCurrentTime();
	DWORD nBegin = tTime.GetYear() * 10000 + tTime.GetMonth() * 100 + tTime.GetDay();
	m_sBeginDate.Format("%d", nBegin);
	m_sEndDate = m_sBeginDate;
	m_bShowQuery = TRUE;
}

TradeCommQueryDlg::~TradeCommQueryDlg()
{
	UnregisterLinkageMsg(TRADE_MESSAGE_REFRESH);
	if (m_pTradeTable)
	{
		m_pTradeTable->Release();
	}
	if (m_pGrid)
	{
		delete m_pGrid;
	}
	if (m_pDataList)
	{
		m_pDataList->Release();
	}
	
}

BOOL TradeCommQueryDlg::OnInit()
{
	if (__super::OnInit())
	{
		//TODO:
		RegisterLinkageMsg(TRADE_MESSAGE_REFRESH);

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
		m_bShowQuery = m_pParam->GetStringParam("ShowQuery") != "0";
		return TRUE;
	}
	else
		return FALSE;
}

void TradeCommQueryDlg::OnShow(BOOL bShow)
{
	if (bShow && m_bShowQuery)
	{
		AsyncQuery();
	}
}

void TradeCommQueryDlg::OnLinkageMsg(CLinkageParam* pParam)
{
	switch (pParam->m_nMsg)
	{
	case TRADE_MESSAGE_REFRESH:
		{
			if (IsShow())
			{
				CString sTmp;
				sTmp = pParam->GetStringField("begin_date");
				if (sTmp != _T(""))
				{
					m_sBeginDate = sTmp;
				}
				sTmp = pParam->GetStringField("end_date");
				if (sTmp != _T(""))
				{
					m_sEndDate = sTmp;
				}
				if (pParam->m_bPost)
				{
					AsyncQuery();
				}
				else
					SyncQuery();				
			}
			break;
		}
		
	}
}

void TradeCommQueryDlg::TradingAsyncCallBack(ITrading* pTrading, int iAction)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pTrading->GetSuccessful() && pTrading->HaveData())
	{
		if (m_pDataList)
		{
			m_pDataList->Release();
		}
		m_pDataList = pTrading->GetDataList();
		FillGrid();
		if (m_pDataList)
		{
			SendCount(m_pDataList->GetCount());
		}
	}
	else
	{
		pTrading->FreeDataList();
		m_pGrid->SetRowCount(1);
		SendCount(0);
	}
		
}

void TradeCommQueryDlg::SetSize()
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

BOOL TradeCommQueryDlg::Validate()
{	
	return TRUE;
}

void TradeCommQueryDlg::DoEntrust()
{

}

void TradeCommQueryDlg::SyncQuery()
{
	IReqQueryBase* pReqQueryBase = dynamic_cast<IReqQueryBase*>(m_pTradeCoreObj->NewTradingByAction(m_iAction));
	pReqQueryBase->Clear();	
	if (m_sBeginDate != _T(""))
	{
		pReqQueryBase->SetBeginDate(m_sBeginDate);
	}
	if (m_sEndDate != _T(""))
	{
		pReqQueryBase->SetEndDate(m_sEndDate);
	}
	pReqQueryBase->SetAccount(m_pAccount);
	if(pReqQueryBase->TradingSync())
	{
		CString sMsg;
		if(pReqQueryBase->HaveError(sMsg))
		{
			MessageBox(sMsg, "提示", MB_ICONINFORMATION);
			pReqQueryBase->FreeDataList();
			m_pGrid->SetRowCount(1);
			SendCount(0);
		}
		else
		{
			if (m_pDataList)
			{
				m_pDataList->Release();
			}
			m_pDataList = pReqQueryBase->GetDataList();
			SendCount(m_pDataList->GetCount());
			FillGrid();
		}
	}
	else
	{
		MessageBox(pReqQueryBase->GetLastError(), "提示", MB_ICONINFORMATION);
	}
}

void TradeCommQueryDlg::AsyncQuery()
{
	DWORD nQueryEnd = GetTickCount();
	if ((nQueryEnd - m_nQueryBegin) > (5 * 1000))
	{
		m_nQueryBegin = nQueryEnd;
		IReqQueryBase* pReqQueryBase = dynamic_cast<IReqQueryBase*>(m_pTradeCoreObj->NewTradingByAction(m_iAction));
		pReqQueryBase->Clear();	
		if (m_sBeginDate != _T(""))
		{
			pReqQueryBase->SetBeginDate(m_sBeginDate);
		}
		if (m_sEndDate != _T(""))
		{
			pReqQueryBase->SetEndDate(m_sEndDate);
		}
		pReqQueryBase->SetAccount(m_pAccount);
		pReqQueryBase->TradingAsync(this);
	}
}

void TradeCommQueryDlg::FillGrid()
{
	if (IsShow())
	{
		m_pGrid->FillGrid(m_pDataList);
	}
	
}

void TradeCommQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//if (m_pGrid)
	//{
	//	DDX_Control(pDX, _GRID_ID_, *m_pGrid);
	//}
}

BEGIN_MESSAGE_MAP(TradeCommQueryDlg, CTradeDialogBase)
	//
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, _GRID_ID_, &TradeCommQueryDlg::OnGridRClick)
	ON_NOTIFY(NM_DBLCLK, _GRID_ID_, &TradeCommQueryDlg::OnGridDBClick)
END_MESSAGE_MAP()


void TradeCommQueryDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	SetSize();
}

void TradeCommQueryDlg::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	//AfxMessageBox("Right Click");
}

void TradeCommQueryDlg::OnGridDBClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	if (Validate())
	{
		DoEntrust();
	}
}

void TradeCommQueryDlg::SendCount(int nCount)
{
	if (IsShow())
	{
		CLinkageParam param(TRUE);
		CString sValue;
		sValue.Format("%d", nCount);
		param.AddParam(TRADE_PARAM_COUNT, sValue);
		SendLinkageMsg(TRADE_MESSAGE_QUERYCOUNT, &param);
	}
}

//BOOL TradeCommQueryDlg::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
//{
//	return CTradeDialogBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//}



//LRESULT TradeCommQueryDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CTradeDialogBase::WindowProc(message, wParam, lParam);
//}

//BOOL TradeCommQueryDlg::OnInitDialog()
//{
//	CTradeDialogBase::OnInitDialog();
//
//	// TODO:  Add extra initialization here
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//	// EXCEPTION: OCX Property Pages should return FALSE
//}
