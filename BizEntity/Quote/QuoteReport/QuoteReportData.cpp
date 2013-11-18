#include "stdafx.h"
#include "QuoteReport.h"
#include "QuoteReportDlg.h"
#include "..\QuoteComm\QuoteBaseDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////收到数据
BOOL CQuoteReportDlg::HSDataSourceSink_OnCommNotify(void* pData)
{
	return TRUE;
}
BOOL CQuoteReportDlg::HSDataSourceSink_OnRecvData(void* pData, int nLen)
{
	if (!pData || IsBadReadPtr(pData,1))
	{
		return FALSE;
	}
	DataHead* pHead = (DataHead*)pData;

	m_pQuoteReport->OnRecvData(pData, nLen);

	return TRUE;
}
