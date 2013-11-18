#include "StdAfx.h"
#include "DrawXXDL.h"
#include "DiagramComm.h"
#include "QuoteComm\QuoteDefine.h"
#include "QuoteComm\InfoStruct.h"
#include "AnalysisTextData.h"
#include "GeneralHelper.h"
#include "HSCalculate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDrawXXDL::CDrawXXDL(IHsColor* pColor, IHsFont* pFont, IHsInfo* pInfo)
{
	SetStyle(TABLE_STYLE_SCREEN | TABLE_WIDTH_CHANGABLE);
	CreateVerScroll(HSSB_VERT | HSSB_RIGHT | HSSB_BLOCK_ALWAYSTOP);
	CreateTitle();
	m_pTitle->SetText("信息地雷");
	m_strInfoPath  = INFO_PATH_KEY_TECH;
	m_nActiveID = -1;
	m_pColor = pColor;
	m_pFont = pFont;
	m_pInfo = pInfo;
	m_pDlgShowHtml = NULL;
	m_bRequst = FALSE;
}

CDrawXXDL::~CDrawXXDL(void)
{
	if (m_pDlgShowHtml->GetSafeHwnd())
	{
		m_pDlgShowHtml->DestroyWindow();
	}
	if (m_pDlgShowHtml)
	{
		delete m_pDlgShowHtml;
	}
	DelAllInfoIndex();
}

void CDrawXXDL::CreateSelf()
{	
	SetColCount(1);
	SetRowCount(m_ayInfoIndex.GetSize());
	SetRowHeight(XXDL_ROW_HEIGHT);
	SetCellCount(m_ayInfoIndex.GetSize());

	for (int i = 0; i < m_ayInfoIndex.GetSize(); i++)
	{// 因为单元格中只能存放64个字符，为了溢出，字符串统一在绘制的画
		CreateCell("", 0,0,0,0,0,i);
	}
}

void CDrawXXDL::DelAllInfoIndex()
{
	InfoIndex* pInfoIndex = NULL;
	for (int i = 0; i < m_ayInfoIndex.GetCount(); i++)
	{
		pInfoIndex = m_ayInfoIndex.GetAt(i);
		delete pInfoIndex;
		pInfoIndex = NULL;
	}
	m_ayInfoIndex.RemoveAll();
}

CString CDrawXXDL::GetTitleString( InfoIndex* pInfoIndex )
{
	CString str = "";
	if (pInfoIndex)
	{
		str = "◎"; // 行标识
		str.AppendFormat("%04d/%02d/%02d ", pInfoIndex->m_lDate/10000, (pInfoIndex->m_lDate%10000)/100, pInfoIndex->m_lDate%100); // 时间
		str.AppendFormat("%s", pInfoIndex->m_cTitle);
	}
	return str;
}
BOOL CDrawXXDL::DrawSelf( CDC* pDC, CPoint pos, int nItemID, CRect& rcItem )
{
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	//////////////////////////////////////////////////////////////////////////
	// 获取颜色和字体
	COLORREF clrText;
	int nCellID = PointToIndex(pos);
	if (nCellID != m_nActiveID)
		clrText = m_pColor->GetRTableFixedName();
	else
		clrText = m_pColor->GetUpColor();	
	CFont* pFontText = m_pFont->GetRTableFixedFont();
	
	COLORREF oldClr = pDC->SetTextColor(clrText);
	CFont* pOldFont = pDC->SelectObject(pFontText);
	//////////////////////////////////////////////////////////////////////////
	// 获取文本内容
	int nAyID = nCellID;
	InfoIndex* pInfoIndex = m_ayInfoIndex.GetAt(nAyID);
	CString strTitle = GetTitleString(pInfoIndex);
	//////////////////////////////////////////////////////////////////////////

	pDC->DrawText(strTitle, strTitle.GetLength(), rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	//////////////////////////////////////////////////////////////////////////
	// clean GDI
	pDC->SetTextColor(oldClr);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nOldBkMode);
	return TRUE;
}

BOOL CDrawXXDL::GetRequestData( IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce )
{
	if (pStockInfo == NULL)
	{
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	// 初始化变量
	m_bIsShow = FALSE;
	m_lTextIndexCount = 0;
	//////////////////////////////////////////////////////////////////////////

	AskData* pakData = NULL;
	short  lAskLen = 0;

	int nCodeCount = sizeof(ReqTextData) / sizeof(CodeInfo) + 
		((sizeof(ReqTextData) % sizeof(CodeInfo))?1:0);

	lAskLen = sizeof(AskData) + (nCodeCount - 1)* sizeof(CodeInfo);
	pakData = (AskData*)new char[lAskLen];
	memset(pakData, 0, lAskLen);

	CString strRefName = INFO_PATH_INI_FILE;
	//pakData->m_nIndex = nPacketIndex;//GetPacketIndex(1);
	pakData->m_nSize  = nCodeCount;
	pakData->m_nType  = RT_TEXTDATAWITHINDEX_NEGATIVE;
	pakData->m_cOperator = GET_SRVTYPE_BYNAME(strRefName);	
	pakData->m_nPrivateKey.m_pCode = pStockInfo->m_ciStockCode;
	//pakData->m_lKey   = (long)this;

	ReqTextData* pDataReq = (ReqTextData*)pakData->m_pCode;
	// 读取infoPath.ini文件
	CString strFileName,strLocalPath,strLocalFileName; // 文件名
	CString strCFg;
	CAnalysisTextData::GetZXFileReqName(&m_stkInfo.m_ciStockCode, m_strInfoPath, &strFileName, &strLocalPath, m_pInfo->GetInfoProvider()); 
	// 从文件读取的数据拷贝到请求包中
	strncpy(pDataReq->m_sMarkData.m_cFilePath, strFileName, sizeof(pDataReq->m_sMarkData.m_cFilePath));

	strCFg = CAnalysisTextData::GetUnionString(m_strInfoPath, "", "", strRefName); 
	strncpy(pDataReq->m_sMarkData.m_szInfoCfg, strCFg, sizeof(pDataReq->m_sMarkData.m_szInfoCfg));

	pReqCollector->AddRequest((char*)pakData,lAskLen);
	delete[] pakData;
	return TRUE;
}
int CDrawXXDL::OnUpdateData( DataHead* pData )
{
	if (pData->m_nType == RT_TEXTDATAWITHINDEX_NEGATIVE)
	{
		AnsTextData* pTextData = (AnsTextData*)pData;
		if (pTextData->m_nSize <= 0)
			return 0;
		if (pTextData->m_nSize == m_lTextIndexCount)
			return 0;
		m_lTextIndexCount = pTextData->m_nSize;
		
		//////////////////////////////////////////////////////////////////////////
		// 初始化一些变量
		DelAllInfoIndex();
		RemoveAllCell();
		m_nActiveID = -1;
		m_bIsShow = TRUE;
		//////////////////////////////////////////////////////////////////////////
		int nCount(0);
		InfoIndex* pRefIndex = CAnalysisTextData::GetInfoIndexData(pTextData, pTextData->m_nSize, nCount);
		if (pRefIndex)
		{
			// 加入到指针
			InfoIndex* pIndex = NULL;
			for(int i = 0; i < nCount; i++,pRefIndex++ )
			{
				pIndex = new InfoIndex;
				memcpy(pIndex, pRefIndex, sizeof(InfoIndex));
				m_ayInfoIndex.InsertAt(0, pIndex);
			}
		}
		CreateSelf();
		CalCulateSize();
		InValidate(NULL, FALSE);
	}
	else if (pData->m_nType == RT_BYINDEXRETDATA)
	{
		AnsTextData* pTextData = (AnsTextData*)pData;
		if (pTextData->m_nSize <= 0 || m_nActiveID < 0 || m_nActiveID >= m_ayInfoIndex.GetCount() || m_bRequst == FALSE)
		{
			return FALSE;
		}

		InfoIndex* pInfoIndex = m_ayInfoIndex.GetAt(m_nActiveID);
		CString strText(pTextData->m_cData, pTextData->m_nSize);
		CString strUrl = CHSCalculate::CreateHtml(strText, pInfoIndex);
		
		if (!strUrl.IsEmpty())
		{
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//		::OleInitialize(NULL);  // 为了使用CDHtmlDialog
	//		HWND hwnd = m_pRegion->GetHostHandle();
	//		CWnd* pWnd = CWnd::FromHandle(hwnd);
	//		CDHtmlShowDlg dlg(pWnd);

			//////////////////////////////////////////////////////////////////////////
			// 创建html对话框
			if (m_pDlgShowHtml == NULL)
			{
				HWND hwnd = m_pRegion->GetHostHandle();
				CWnd* pWnd = CWnd::FromHandle(hwnd);
				m_pDlgShowHtml = new CDHtmlShowDlg(pWnd);
			}
			//////////////////////////////////////////////////////////////////////////
			m_pDlgShowHtml->SetUserUrl(strUrl);
			m_pDlgShowHtml->SetCaption(pInfoIndex->m_cTitle);
			m_pDlgShowHtml->Show();
			
			m_bRequst = FALSE;
// 			dlg.SetUserUrl(strUrl);
// 			dlg.SetCaption(pInfoIndex->m_cTitle);
// 			dlg.DoModal();
		}
	}
	return TRUE;
}

void CDrawXXDL::RequestText(int nAyIndex)
{
	if (nAyIndex < 0 || nAyIndex >= m_ayInfoIndex.GetCount())
	{
		return;
	}

	InfoIndex* pInfoIndex = m_ayInfoIndex.GetAt(nAyIndex);

	AskData* pakData = NULL;
	short  lAskLen = 0;
	int    nCodeCount = sizeof(ReqTextData) / sizeof(CodeInfo) + 
		((sizeof(ReqTextData) % sizeof(CodeInfo))?1:0);

	lAskLen = sizeof(AskData) + (nCodeCount - 1)* sizeof(CodeInfo);
	pakData = (AskData*)new char[lAskLen];
	memset(pakData, 0, lAskLen);

	pakData->m_nSize  = nCodeCount;
	pakData->m_nType  = RT_BYINDEXRETDATA;
	pakData->m_nPrivateKey.m_pCode = m_stkInfo.m_ciStockCode;	

	ReqTextData* pDataReq = (ReqTextData*)pakData->m_pCode;
	// 读取infoPath.ini文件
	CString strFileName,strLocalPath,strLocalFileName; // 文件名
	CAnalysisTextData::GetZXFileReqName(&m_stkInfo.m_ciStockCode, pInfoIndex, m_strInfoPath, &strFileName, &strLocalPath, m_pInfo->GetInfoProvider()); 
	// 从文件读取的数据拷贝到请求包中
	strncpy(pDataReq->m_sMarkData.m_cTitle, pInfoIndex->m_cTitle, sizeof(pDataReq->m_sMarkData.m_cTitle));
	strncpy(pDataReq->m_sMarkData.m_cFilePath, strFileName, sizeof(pDataReq->m_sMarkData.m_cFilePath));

	CString strCFg = CAnalysisTextData::GetUnionString(m_strInfoPath, "", "", INFO_PATH_INI_FILE);
	strncpy(pDataReq->m_sMarkData.m_szInfoCfg, strCFg, sizeof(pDataReq->m_sMarkData.m_szInfoCfg));

	pDataReq->m_sMarkData.m_lBeginPos = pInfoIndex->m_lOffset;
	pDataReq->m_sMarkData.m_lEndPos   = pInfoIndex->m_lLength + pInfoIndex->m_lOffset;

	IRequestCollector* pReqCollector = m_pRegion->GetRequestCollector();
	if (pReqCollector)
	{
		pReqCollector->AddRequest((char*)pakData,lAskLen);
		m_pRegion->SendRequest(pReqCollector);
		pReqCollector->Release();
		m_bRequst = TRUE;
	}

	delete[] pakData;
}

BOOL CDrawXXDL::MouseDBDown( UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/ )
{	
	CDrawTableBase::MouseDBDown(nFlags, point, pData);
	
	m_nActiveID = GetActiveCellID();
	RequestText(m_nActiveID);  
	InValidate(NULL, FALSE);
	return TRUE;
}
