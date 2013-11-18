#include "StdAfx.h"
#include "DiagramTest.h"

CDiagramTest::CDiagramTest(void)
{
	memset(&m_realTime,0,sizeof(m_realTime));
}

CDiagramTest::~CDiagramTest(void)
{
	
}

void CDiagramTest::SetRegionNode(IRegionNode* pNode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_pRegion = pNode;
	HWND hwnd = m_pRegion->GetHostHandle();
	if(m_pInfoForm.m_hWnd == NULL)
	{
		CWnd* pParent = CWnd::FromHandle(hwnd);
		m_pInfoForm.Create(CInfomationForm::IDD,pParent);
	}
}

BOOL CDiagramTest::SetRect(CRect rc)
{
	if(::IsWindow(m_pInfoForm.m_hWnd))
	{
		m_pInfoForm.MoveWindow(&rc);
	}
	return TRUE;
}

int CDiagramTest::Draw(CDC* pDC)
{
	//m_pInfoForm.ShowWindow(SW_SHOW);
	return 0;
}

BOOL CDiagramTest::MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/)
{
	//m_pRegion->SetCursor( IDC_HAND);
	return TRUE;
}

BOOL CDiagramTest::MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/)
{
	return TRUE;
}

BOOL CDiagramTest::GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo)
{
// 	int lAskLen = sizeof(AskData);
// 	char* pData = new char[lAskLen]; 
// 	AskData*  pAskData = (AskData*)pData;
// 	memset(pAskData, 0, lAskLen);
// 	pAskData->m_nType  = RT_REALTIME_EXT;
// 	pAskData->m_nSize  = 1;
// 	memcpy(pAskData->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
// 	pReqCollector->AddRequest((char*)pAskData,lAskLen);
// 	delete[] pData;
 	return TRUE;
}

int	CDiagramTest::OnUpdateData(DataHead* pData)
{
// 	if( pData->m_nType == RT_REALTIME_EXT)
// 	{
// 		AnsRealTime*  pNowData  = (AnsRealTime*)pData;
// 		if(pNowData->m_nSize <= 0)
// 		{
// 			return FALSE;
// 		}
// 		//memcpy(&m_realTime,pNowData->m_pnowData,sizeof(m_realTime));
// 	}
 	return 0;
}

void CDiagramTest::OnActiveChange(BOOL bShow)
{
	m_pInfoForm.ShowWindow(bShow);
}