/*******************************************************
  源程序名称:TradeDialogBase.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易窗口基类实现
  作    者:  shenglq
  开发日期:  20100903
*********************************************************/
#include "stdafx.h"
#include "TradeDialogBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeDialogBase, CHSBizDialog)

CTradeDialogBase::CTradeDialogBase(UINT nID, CWnd* pParent /* = NULL */)
	: CHSBizDialog(nID, pParent)
{
	m_pTradeConfigObj = (ITradeConfigObj*) CWinnerApplication::GetObject(OBJ_TRADECONFIG);
	m_pTradeCoreObj = (ITradeCoreObj*) CWinnerApplication::GetObject(OBJ_TRADECORE);
	m_pAccount = m_pTradeCoreObj->GetAccountMng()->GetCurrentAccount(atTrade);
	m_pBusinessLogic = m_pAccount->GetBusinessLogic();
	m_pDataSourceEx = (IDataSourceEx*) CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	m_DataSourceHandle = m_pDataSourceEx->HSDataSource_DataSouceInit(this);
	m_pTradeLang = (ITradeLang*) CWinnerApplication::GetObject(OBJ_TRADELANG);
	m_iAction = 0;
	m_pXMLDataDoc = NULL;
	m_pAccountParamList = m_pAccount->GetAccountParamList();
}

CTradeDialogBase::~CTradeDialogBase()
{
	/*if (m_pXMLDataDoc)
	{
		delete m_pXMLDataDoc;
	}	*/
	m_pDataSourceEx->HSDataSource_DataSourceUnInit(m_DataSourceHandle);
	if (m_pXMLDataDoc)
	{
		delete m_pXMLDataDoc;
	}
}

BOOL CTradeDialogBase::OnInit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (CHSBizDialog::OnInit())
	{
		//TODO:
		CString sXMLData = m_pParam->GetXMLData();
		if (!sXMLData.IsEmpty())
		{
			m_pXMLDataDoc = new TiXmlDocument();
			if (!m_pXMLDataDoc->Parse(sXMLData))
			{
				delete m_pXMLDataDoc;
				m_pXMLDataDoc = NULL;
				return FALSE;
			}
		}
		int nAction = m_pParam->GetIntegerParam("action");
		if (nAction)
		{
			m_iAction = nAction;
		}
		LoadWnd();
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeDialogBase::PostRefreshMsg()
{
	//CString sParam = "";
	CLinkageParam param(TRUE);
	this->PostLinkageMsg(TRADE_MESSAGE_REFRESH, &param);
}

void CTradeDialogBase::SetAction(const int iAction)
{
	m_iAction = iAction;
}

void CTradeDialogBase::TradingAsyncCallBack(ITrading* pTrading, int iAction)
{

}

BOOL CTradeDialogBase::HSDataSourceSink_OnCommNotify(void* pData)
{
	return TRUE;
}

BOOL CTradeDialogBase::HSDataSourceSink_OnRecvData(void* pData, int nLen)
{
	return TRUE;
}

void CTradeDialogBase::DoDataExchange(CDataExchange* pDX)
{
	CHSBizDialog::DoDataExchange(pDX);
}

void CTradeDialogBase::LoadWnd()
{
//
}

void CTradeDialogBase::AdjustLayout(int nHeight,int x /* = 0 */, int y /* = 0 */, int z /* = 0 */,EAdjustDirection adValue /* = adTToB */)
{
	CRect rcLast;
	switch (adValue)
	{
	case adTToB:
		for (int i = 0; i < m_paCWnd.GetCount(); i++)
		{
			CWnd* pWnd = (CWnd*)m_paCWnd.GetAt(i);
			CRect rcTmp;
			pWnd->GetWindowRect(rcTmp);
			//pWnd->GetClientRect(rcTmp);
			CPoint ptBR;
			CPoint ptTL;
			int iWidth = rcTmp.Width();
			//int iHeigth = rcTmp.Height();
			if (0 == i)
			{
				CRect rcClient;
				//GetClientRect(rcClient);
				GetWindowRect(rcClient);
				ptTL.y = rcClient.TopLeft().y + y;
				ptTL.x = rcClient.TopLeft().x + x;	
				ptBR.y = ptTL.y + nHeight;
				ptBR.x = ptTL.x + iWidth;				
			}
			else
			{
				ptTL.y = rcLast.TopLeft().y + rcLast.Height() + z;
				ptTL.x = rcLast.TopLeft().x;	
				ptBR.y = ptTL.y + nHeight;
				ptBR.x = ptTL.x + iWidth;
			}
			rcTmp.SetRect(ptTL, ptBR);
			rcLast = rcTmp;
			ScreenToClient(&rcTmp);
			pWnd->MoveWindow(rcTmp);
			pWnd->ShowWindow(SW_SHOW);

		}
		break;
	case adBToT:
		break;
	case adLToR:
		break;
	case adRToL:
		break;	
	}
	
}
void CTradeDialogBase::AdjustLayoutEx(CWnd* pMain, CWnd* pDetail, int x /* = 0 */, int y /* = 0 */, EAdjustDirectionEx adxValue /* = adxRightTop */)
{
	CRect rcTmp;
	pMain->GetWindowRect(rcTmp);
	int iWidth = rcTmp.Width();
	int iHeigth = rcTmp.Height();
	CPoint ptBR = rcTmp.BottomRight();
	CPoint ptTL = rcTmp.TopLeft();
	pDetail->GetWindowRect(rcTmp);
	int iWidth1 = rcTmp.Width();
	int iHeigth1 = rcTmp.Height();
	switch (adxValue)
	{
	case adxRightTop:
		ptTL.y = ptTL.y + y;
		ptTL.x = ptTL.x + iWidth + x;	
		ptBR.x = ptTL.x + iWidth1;
		ptBR.y = ptTL.y + iHeigth1;
		break;
	case adxRightBottom:
		break;
	case adxBottomLeft:
		ptTL.x = ptTL.x + x;
		ptBR.x = ptTL.x + iWidth1;
		ptTL.y = ptBR.y + y;
		ptBR.y = ptTL.y + iHeigth1;
		break;
	case adxBottomRight:
		ptBR.x = ptBR.x + x;
		ptTL.x = ptBR.x - iWidth1;
		ptTL.y = ptBR.y + y;
		ptBR.y = ptTL.y + iHeigth1;		
		break;
	}
	rcTmp.SetRect(ptTL, ptBR);
	ScreenToClient(&rcTmp);
	pDetail->MoveWindow(rcTmp);
	//pDetail->ShowWindow(SW_SHOW);
}

void CTradeDialogBase::SetComoboxHeight(CComboBox* pComoboBox, int nHeight /* = 100 */)
{
	pComoboBox->ModifyStyle(0, CBS_NOINTEGRALHEIGHT); 
	CRect rect;
	pComoboBox->GetWindowRect(rect);
	rect.bottom = rect.bottom + nHeight;
	ScreenToClient(&rect);
	pComoboBox->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
}

BEGIN_MESSAGE_MAP(CTradeDialogBase, CHSBizDialog)
//
END_MESSAGE_MAP()

DWORD CTradeDialogBase::GetDataType()
{
	return 0;
}

void CTradeDialogBase::OnReflash(IDataList * pDataList, DWORD dDataType)
{

}

void CTradeDialogBase::OnChange(IDataItem * pDataItem, EDataChangeType dtChangeType, DWORD dDataType)
{

}

void CTradeDialogBase::NotifyQuoteChangeCode(CString strStkCode)
{
	StockUserInfo sStockInfo;
	if (strStkCode.IsEmpty())
	{
		CLinkageParam param;
		param.AddParam(PARAM_CODE_CHANGE_MSG_FROM,PARAM_CODE_CHANGE_MSG_FROM_TRADE);
		SendLinkageMsg(QUOTE_MESSAGE_CODE_CHANGE,&param);	
	}
	else
	{
		if( m_pDataSourceEx != NULL
			&& m_pDataSourceEx->HSDataSourceEx_GetStockUserInfo(strStkCode.GetBuffer(), STOCK_MARKET, sStockInfo))
		{
			CLinkageParam param;
			CString strMarkeType;
			strMarkeType.Format("%d",sStockInfo.m_ciStockCode.m_cCodeType);
			param.AddParam(PARAM_CODE_CHANGE_MARK_TYPE,strMarkeType);
			param.AddParam(PARAM_CODE_CHANGE_STOCK_CODE,sStockInfo.GetCode());
			param.AddParam(PARAM_CODE_CHANGE_MSG_FROM,PARAM_CODE_CHANGE_MSG_FROM_TRADE);
			SendLinkageMsg(QUOTE_MESSAGE_CODE_CHANGE,&param);
		}
	}
}

void CTradeDialogBase::AdjustLayoutEx(CWnd* pWnd, int x, EAdjustDirectionEx adxValue /* = adxCenter */)
{
	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcComm;
	pWnd->GetClientRect(rcComm);
	CPoint ptBR = rcComm.BottomRight();
	CPoint ptTL = rcComm.TopLeft();
	int iWidth = rcComm.Width();
	int iHeight = rcComm.Height();
	int iTop = (rcClient.Height() - iHeight) / 2;
	if (iTop < 0)
	{
		iTop = 0;
	}
	ptBR.x = rcClient.BottomRight().x - x;
	ptTL.x = ptBR.x - iWidth;
	ptTL.y = rcClient.TopLeft().y + iTop;
	ptBR.y = ptTL.y  + iHeight;
	rcComm.SetRect(ptTL, ptBR);
	pWnd->MoveWindow(rcComm);
}