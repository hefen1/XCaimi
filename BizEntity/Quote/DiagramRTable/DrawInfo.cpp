#include "StdAfx.h"
#include "DrawInfo.h"
#include "AnalysisTextData.h"
#include <shlwapi.h>
#include "..\..\..\Resource\QuoteLang\Resource.h"
#include "GeneralHelper.h"
#include "DiagramComm.h"
#include "QuoteLangMngInterFace.h"
#include "WinnerApplication.h"
#include "HSMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CDrawInfo::m_nActiveCellID = 2; // Ĭ�ϵ�һ�� �ڶ���ΪĬ��ѡ�е�Ԫ��
CDrawInfo::CDrawInfo(IHsColor* pColor, IHsInfo* pInfo)
{
	SetStyle(TABLE_STYLE_SCREEN);
	m_strInfoPath = INFO_PATH_KEY_F10;
	m_pViewInfo = new CHSViewInfo;
	m_pViewInfo->SetViewNotify(this);
	m_pColor = pColor;
	m_pInfo  = pInfo;
	m_preCodeInfo.SetEmpty();

	// ����ͼƬ��Դ
	IQuoteLang* pQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
	HANDLE handle = pQuoteLang->LoadHImage(IDB_ZIXUN_ICON, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	if (handle)
	{
		CBitmap bit;
		bit.Attach((HBITMAP)handle);
		m_lstImage.Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 3);
		BOOL bRet = m_lstImage.Add(&bit, RGB(255,0,255));
		bit.DeleteObject();
	}
}

CDrawInfo::~CDrawInfo(void)
{
	if (m_pViewInfo)
	{
		delete m_pViewInfo;
		m_pViewInfo = NULL;
	}

	if (m_lstImage.m_hImageList)
	{
		m_lstImage.DeleteImageList();
	}
}

BOOL CDrawInfo::SetRect( CRect rc )
{
	//////////////////////////////////////////////////////////////////////////
	// �����п�
	SetColCount(INFO_COL_COUNT);
	if (GetColCount() > 1)
	{
		ColInfoStruct sColInfo;
		sColInfo.nWidth = (rc.Width() - INFO_LAST_COL_WIDTH) / (GetColCount() - 1);	
		for (int i = 0; i < GetColCount() - 1; i++)
		{
			SetColInfo(i, &sColInfo);
		}

		sColInfo.nWidth = rc.Width() - sColInfo.nWidth * (GetColCount() - 1);
		SetColInfo(GetColCount()-1, &sColInfo);
	}
	//////////////////////////////////////////////////////////////////////////
	CDrawTableBase::SetRect(rc);

	rc.top = rc.top + GetRowCount() * INFO_ROW_HEIGHT + 1;
	m_pViewInfo->SetRect(rc);

	return TRUE;
}

BOOL CDrawInfo::Draw(CDC* pDC)
{
	BOOL bRet = __super::Draw(pDC);
	bRet = m_pViewInfo->Draw(pDC);
	
	return bRet;
}

BOOL CDrawInfo::DrawSelf( CDC* pDC, CPoint pos, int nItemID, CRect& rcItem )
{
	int nAyIndex = GetArrayIndexFromPoint(pos);
	int nCellIndex = PointToIndex(pos);
	if (nAyIndex != -1)
	{// ��ǰ������ڵ���Ѷ������Ԫ��  
		
		if (m_nActiveCellID == nCellIndex)
		{
			// �������
			rcItem.DeflateRect(1,1,1,1);
			pDC->FillSolidRect(&rcItem, m_pColor->GetDiagramLayoutActiveFillColor());

			// ���ü���ǰ��ɫ
			SetCellText(nCellIndex, nItemID, NULL, TEXT_COLOR_ACTIVEFIXEDNAME);
		}
		else
		{
			// ����Ĭ��ǰ��ɫ
			SetCellText(nCellIndex, nItemID, NULL, TEXT_COLOR_FIXEDNAME);
		}	
	}
	else
	{// ������Ѷ������Ԫ��
		if (pos == CPoint(GetColCount()-1, 0))
		{// ����ǵ�һ�У����һ��

			//////////////////////////////////////////////////////////////////////////
			// ����ȫ�����߻�ԭͼ������
			CalcImageRect(&m_rcMaxScreen, &rcItem, TRUE);
			// ����ͼƬ
			if (!m_bSizeMax)
				m_lstImage.Draw(pDC, 0, CPoint(m_rcMaxScreen.left, m_rcMaxScreen.top), ILD_TRANSPARENT);
			else
				m_lstImage.Draw(pDC, 4, CPoint(m_rcMaxScreen.left, m_rcMaxScreen.top), ILD_TRANSPARENT);
			//////////////////////////////////////////////////////////////////////////
			// ����ر�ͼ������
			CalcImageRect(&m_rcClose, &rcItem, FALSE);
			// ����ͼƬ
			m_lstImage.Draw(pDC, 1, CPoint(m_rcClose.left, m_rcClose.top), ILD_TRANSPARENT);
			return TRUE;
		}
		else if (pos == CPoint(GetColCount()-1, 1))
		{
			//////////////////////////////////////////////////////////////////////////
			// �������Ϸ���ͼ������
			CalcImageRect(&m_rcUp, &rcItem, TRUE);
			// ����ͼƬ
			m_lstImage.Draw(pDC, 2, CPoint(m_rcUp.left, m_rcUp.top), ILD_TRANSPARENT);

			//////////////////////////////////////////////////////////////////////////
			// �������·���ͼ������
			CalcImageRect(&m_rcDown, &rcItem, FALSE);
			// ����ͼƬ
			m_lstImage.Draw(pDC, 3, CPoint(m_rcDown.left, m_rcDown.top), ILD_TRANSPARENT);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDrawInfo::GetRequestData( IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce )
{ 
	if (pStockInfo == NULL)
	{
		return FALSE;
	}
	if (pStockInfo->m_ciStockCode.CompareCode(&m_preCodeInfo))
	{
		return FALSE;
	}
	m_preCodeInfo = pStockInfo->m_ciStockCode;
	m_bIsShow = FALSE;

	AskData* pakData = NULL;
	short  lAskLen = 0;

	int nCodeCount = sizeof(ReqTextData) / sizeof(CodeInfo) + 
		((sizeof(ReqTextData) % sizeof(CodeInfo))?1:0);

	lAskLen = sizeof(AskData) + (nCodeCount - 1)* sizeof(CodeInfo);
	pakData = (AskData*)new char[lAskLen];
	memset(pakData, 0, lAskLen);

	//	pakData->m_nIndex = nPacketIndex;//GetPacketIndex(1);
	pakData->m_nSize  = nCodeCount;
	pakData->m_nType  = RT_TEXTDATAWITHINDEX_NEGATIVE;
	pakData->m_nPrivateKey.m_pCode = m_preCodeInfo;
	//	pakData->m_cOperator = GET_SRVTYPE_BYNAME(strRefFileName);	
	//	pakData->m_lKey   = lThis;

	ReqTextData* pDataReq = (ReqTextData*)pakData->m_pCode;
	// ��ȡinfoPath.ini�ļ�
	CString strFileName,strLocalPath,strLocalFileName; // �ļ���
	CString strCFg;
	CAnalysisTextData::GetZXFileReqName(&m_stkInfo.m_ciStockCode, m_strInfoPath, &strFileName,&strLocalPath, m_pInfo->GetInfoProvider()); 
	// ���ļ���ȡ�����ݿ������������
	strncpy(pDataReq->m_sMarkData.m_cFilePath, strFileName, sizeof(pDataReq->m_sMarkData.m_cFilePath));

	strCFg = CAnalysisTextData::GetUnionString(m_strInfoPath, "", "", INFO_PATH_INI_FILE); 
	strncpy(pDataReq->m_sMarkData.m_szInfoCfg, strCFg, sizeof(pDataReq->m_sMarkData.m_szInfoCfg));

	pReqCollector->AddRequest((char*)pakData,lAskLen);
	delete[] pakData;

	return TRUE;
}

int CDrawInfo::OnUpdateData( DataHead* pData )
{
	if (pData->m_nType == RT_TEXTDATAWITHINDEX_NEGATIVE)
	{
		m_bIsShow = TRUE;
		AnsTextData* pTextData = (AnsTextData*)pData;
		if (pTextData->m_nSize <= 0)
		{
			return 0;
		}
		//////////////////////////////////////////////////////////////////////////
		// ��ʼ��һЩ����
		RemoveAllCell();
		DelAllInfoIndex();
		m_pViewInfo->SetText("");
		//////////////////////////////////////////////////////////////////////////
		int nCount(0);
		InfoIndex* pRefIndex = CAnalysisTextData::GetInfoIndexData(pTextData, pTextData->m_nSize, nCount);
		if (pRefIndex)
		{
			// ���뵽ָ��
			InfoIndex* pIndex = NULL;
			for(int i = 0; i < nCount; i++,pRefIndex++ )
			{
				pIndex = new InfoIndex;
				memcpy(pIndex, pRefIndex, sizeof(InfoIndex));
				m_ayInfoIndex.Add(pIndex);
			}
			CreateSelf();
			
			CPoint pos = IndexToPoint(m_nActiveCellID);
			RequestText(GetArrayIndexFromPoint(pos));  
		}
	}
	else if (pData->m_nType == RT_BYINDEXRETDATA)
	{
		AnsTextData* pTextData = (AnsTextData*)pData;
		CString strText(pTextData->m_cData, pTextData->m_nSize);

		m_pViewInfo->SetText(strText);
		SetRect(m_rect);
		InValidate(NULL, FALSE);
	}
	return TRUE;
}

BOOL CDrawInfo::Create(IDrawBaseParam *pParam)
{
	m_bSizeMax = pParam->GetIntegerParam("Size_max");
	return TRUE;
}

void CDrawInfo::CreateSelf()
{
	// ��һ�е�һ��Ϊ��Ʊ���� �ڶ��еڶ���Ϊ��Ʊ����
	int nCellCount = m_ayInfoIndex.GetCount() + 2;
	int nRowCount = nCellCount / INFO_COL_COUNT + ((nCellCount%INFO_COL_COUNT)==0?0:1); // ����ȡ��
	SetColCount(INFO_COL_COUNT);
	SetRowCount(nRowCount);
	SetRowHeight(INFO_ROW_HEIGHT);
	nCellCount = INFO_COL_COUNT * nRowCount;
	SetCellCount(nCellCount);

	int nCellIndex(0);
	int nAyIndex(0);
	CString str;
	InfoIndex* pInfoIndex = NULL;
	//////////////////////////////////////////////////////////////////////////
	// ��һ��
	str = m_stkInfo.m_cStockName;   // ��Ʊ����
	CreateCell(str, TEXT_COLOR_STOCKCODE, TEXT_FONT_FIXED, ITEM_TEXT_CENTER, CELL_BORDER_RIGHT | CELL_BORDER_BOTTOM, 0, nCellIndex++);
	
	str = m_stkInfo.m_ciStockCode.GetCode(); // ��������
	CreateCell(str, TEXT_COLOR_STOCKNAME, TEXT_FONT_FIXED, ITEM_TEXT_CENTER, CELL_BORDER_RIGHT | CELL_BORDER_BOTTOM, 0, nCellIndex++);
	
	//////////////////////////////////////////////////////////////////////////
	// ��ӷ��ص�����
	for (int y = 0; y < GetRowCount(); y++)
	{
		for (int x = 1; x < GetColCount() - 1; x++)
		{
			nCellIndex = PointToIndex(CPoint(x,y));
			str = "";
			if (nAyIndex < m_ayInfoIndex.GetCount())
			{
				pInfoIndex = m_ayInfoIndex.GetAt(nAyIndex++);
				str = pInfoIndex->m_cTitle;
			}
			CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, ITEM_TEXT_CENTER, CELL_BORDER_RIGHT | CELL_BORDER_BOTTOM, 0, nCellIndex);
		}
		// ÿһ�����һ�е�Ԫ���ǰ�ť
		nCellIndex = PointToIndex(CPoint(GetColCount()-1, y));
		CreateCell("", TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, ITEM_TEXT_CENTER, CELL_BORDER_RIGHT | CELL_BORDER_BOTTOM, 0, nCellIndex);
	}
}

void CDrawInfo::DelAllInfoIndex()
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

int CDrawInfo::GetArrayIndexFromPoint(CPoint pos)
{
	if (pos.x < 1 || pos.x >= GetColCount() - 1 || pos.y < 0 || pos.y >= GetRowCount())
	{
		return -1;
	}
	return pos.y * (GetColCount() - 2) + pos.x - 1;
}

void CDrawInfo::RequestText(int nAyIndex)
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
	pakData->m_nPrivateKey.m_pCode = m_preCodeInfo;
	
	ReqTextData* pDataReq = (ReqTextData*)pakData->m_pCode;
	// ��ȡinfoPath.ini�ļ�
	CString strFileName,strLocalPath,strLocalFileName; // �ļ���
	CAnalysisTextData::GetZXFileReqName(&m_stkInfo.m_ciStockCode, pInfoIndex, m_strInfoPath, &strFileName, &strLocalPath, m_pInfo->GetInfoProvider()); 
	// ���ļ���ȡ�����ݿ������������
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
	}

	delete[] pakData;

}

BOOL CDrawInfo::MouseDown( UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/ )
{		
	if (m_rcDown.PtInRect(point))
	{// ���·�
		CDrawBaseParam param;
		param.AddParam(PARAM_CODE_PAGE_UP_DOWN_DIRECTION, PARAM_CODE_PAGE_UP_DOWN_DIRECTION_DOWN);
		m_pRegion->SendLinkageMsg(QUOTE_MESSAGE_CODE_PAGE_UP_DOWN, &param);
		return TRUE;
	}
	else if (m_rcUp.PtInRect(point))
	{// ���Ϸ�
		CDrawBaseParam param;
		param.AddParam(PARAM_CODE_PAGE_UP_DOWN_DIRECTION, PARAM_CODE_PAGE_UP_DOWN_DIRECTION_UP);
		m_pRegion->SendLinkageMsg(QUOTE_MESSAGE_CODE_PAGE_UP_DOWN, &param);
		return TRUE;
	}
	else if (m_rcClose.PtInRect(point))
	{// �ر�
		CDrawBaseParam param;
		CString strType;
		strType.Format("%d", m_stkInfo.m_ciStockCode.m_cCodeType);
		param.AddParam(Param_Open_MarketType, strType);
		param.AddParam(Param_Open_StockCode, m_stkInfo.GetCode());
		param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_TechAnalysis);
		CString strMsgID;
		strMsgID.Format("%d", DiagramMsg_OpenLayout);
		param.AddParam(PARAM_DIAGRAMMSGID, strMsgID);
		m_pRegion->SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT, &param);
		
		return TRUE;
	}
	else if (m_rcMaxScreen.PtInRect(point))
	{// ȫ�����߻�ԭ

		CString strLayoutName("");
		if (m_bSizeMax)
			strLayoutName = Layout_F10Info;
		else
			strLayoutName = Layout_MaxSize_F10Info;
		
		CDrawBaseParam param;
		CString strType;
		strType.Format("%d", m_stkInfo.m_ciStockCode.m_cCodeType);
		param.AddParam(Param_Open_MarketType, strType);
		param.AddParam(Param_Open_StockCode, m_stkInfo.GetCode());
		param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, strLayoutName);
		CString strMsgID;
		strMsgID.Format("%d", DiagramMsg_OpenLayout);
		param.AddParam(PARAM_DIAGRAMMSGID, strMsgID);
		m_pRegion->SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT, &param);

		return TRUE;
	}

	GetCellID(point);
	if (GetActiveCellID() == m_nActiveCellID)
	{// ����û�������λ�� û�з����ı� ֱ�ӷ���
		return TRUE;
	}
	CPoint pos = IndexToPoint(GetActiveCellID());
	int nAyIndex = GetArrayIndexFromPoint(pos);

	if (nAyIndex != -1)
	{// Ҫ������Ѷ��������
		m_nActiveCellID = GetActiveCellID();
		RequestText(nAyIndex);
	}
	else
	{
		m_pViewInfo->MouseDown(nFlags, point);
	}

	return TRUE;
}

BOOL CDrawInfo::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	return m_pViewInfo->OnMouseWheel(nFlags, zDelta, pt);
}

int	CDrawInfo::OnKey(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return m_pViewInfo->OnKey(nChar, nRepCnt, nFlags);
}

void CDrawInfo::ViewInfoInvalidate( CRect* pRect, BOOL bErase /*= TRUE*/ )
{
	m_pRegion->Invalidate(pRect, bErase);
}

HWND CDrawInfo::ViewInfoGetHandle()
{
	return m_pRegion->GetHostHandle();
}

void CDrawInfo::CalcImageRect( CRect* pRcIamge, CRect* pRcItem, BOOL bIsLeft /*= TRUE*/ )
{
	if (pRcIamge == NULL || pRcItem == NULL)
		return;
	
	*pRcIamge = *pRcItem;
	int nMargin = pRcItem->Width() / 10;
	if (bIsLeft)
	{
		pRcIamge->DeflateRect(nMargin, 5, 0, 0);
		pRcIamge->bottom = pRcIamge->top + 16;
		pRcIamge->right = pRcIamge->left + 16;
	}
	else
	{
		pRcIamge->DeflateRect(0, 5, nMargin, 0);
		pRcIamge->bottom = pRcIamge->top + 16;
		pRcIamge->left = pRcIamge->right - 16;
	}
}
