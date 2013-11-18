#include "StdAfx.h"
#include "DrawRStockTitle.h"
#include "ConfigInterface.h"
#include "QuoteLangMngInterFace.h"
#include "WinnerApplication.h"
#include "..\..\..\Resource\QuoteLang\Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDrawRStockTitle::CDrawRStockTitle(IHsUserDataCfg* pUserDefCfg, IHsColor* pColor, IHsFont* pFont)
{
	ASSERT(pUserDefCfg && pColor && pFont);
	m_iUserDefCfg = pUserDefCfg;
	m_pColor = pColor;
	m_pFont = pFont;

	SetStyle(TABLE_STYLE_SCREEN | TABLE_WIDTH_CHANGABLE | TABLE_HEIGHT_CHANGABLE);
	SetRowCount(1);
	SetColCount(1);

	CreateCell("", 0, 0, 0, CELL_TIP);

	ReadFile();
	m_pImageList = NULL;

}

CDrawRStockTitle::~CDrawRStockTitle()
{
	if (m_pImageList)
	{
		m_pImageList->DeleteImageList();
		delete m_pImageList;
	}
}

void CDrawRStockTitle::ReadFile()
{
	m_clrBK  =  m_pColor->GetBlackgroundColor();
	m_clrCode = m_pColor->GetRTitleCode();
	m_clrCodeName = m_pColor->GetRTitleCodeName();
	m_pFontCode = m_pFont->GetRTableTitleCodeFont();
	m_pFontCodeName = m_pFont->GetRTableTitleNameFont();
	
}

int CDrawRStockTitle::DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect &rcItem)
{
	if (m_pImageList == NULL)
	{
		// ����ͼƬ��Դ
		IQuoteLang* pQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
		HANDLE handle = pQuoteLang->LoadHImage(IDB_ALL_ICON, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		if (handle)
		{
			CBitmap bit;
			bit.Attach((HBITMAP)handle);
			m_pImageList = new CImageList;
			m_pImageList->Create(16, 16, ILC_COLOR32 | ILC_MASK, MARK_COUNT + 1, 3); // ��ȡһ�� ���һ��Ϊ���ֱ�ʶ��ͼ��
			BOOL bRet = m_pImageList->Add(&bit, RGB(0, 128, 128));
			bit.DeleteObject();
		}
	}
	
	//��䱳��
	CRect textRect = rcItem;
	pDC->FillSolidRect(&textRect, m_clrBK);

	if (m_stkInfo.m_ciStockCode.m_cCodeType == 0)
	{
		return -1;
	}

	//�ֱ�������ļ���ȡ��Ϣ���ڹ�Ʊ�������ƺ�����ʾ�Ĵ������ͺ��Ǽ�(��ʾ���ٿ���)
	short nCodeMarket = m_stkInfo.m_ciStockCode.m_cCodeType;
	char *pCodeType = {"�������ʹ���"};
	char *pStarGrade = {"�Ǽ��ȼ�"};
	char pCodemarket[6]={0};
	itoa(nCodeMarket,pCodemarket,16);


	int nBkMode = pDC->SetBkMode(TRANSPARENT);	

	CString str(pCodemarket);
	CString strCode = m_stkInfo.m_ciStockCode.GetCode();
	CString strName = m_stkInfo.GetName();

	COLORREF lColor = pDC->SetTextColor(m_clrCode);
	CFont* pOldFont = pDC->SelectObject(m_pFontCode); 

	CRect rc = textRect;
	strCode = " " + strCode;
	rc.right = rc.left + pDC->GetTextExtent(strCode).cx + 1;//textRect.Width()/2 - 4;
	pDC->DrawText(strCode, strCode.GetLength(), &rc,  DT_VCENTER | DT_SINGLELINE|DT_CENTER);

	////���ƺʹ���ʹ�ò�ͬ����ɫ����
	pDC->SelectObject(m_pFontCodeName);
	pDC->SetTextColor(m_clrCodeName);
	
	//���ƹ�Ʊ����
	strName = " " + strName;
	rc.left  = rc.right + 4;
	rc.right = rc.left + pDC->GetTextExtent(strName).cx + 1;
	pDC->DrawText(strName, strName.GetLength(), &rc, DT_VCENTER | DT_SINGLELINE); 

	//////////////////////////////////////////////////////////////////////////
	// ���Ʊ��
	CString strKey;
	strKey.Format("%04x%s",m_stkInfo.m_ciStockCode.m_cCodeType, m_stkInfo.GetCode());
	const HsCodeMark* pMark = m_iUserDefCfg->SearchMark(strKey);
	if (pMark)
	{
		int nImage(-1);
		if (pMark->m_nType == -1)
		{// ���ֱ�ʶ
			nImage = 10; // ȡ��ʮ��ͼƬ
		}
		else
		{// ��ͨͼƬ��ʶ
			nImage = pMark->m_nType - 1;
		}
		rc.left  = rc.right + 4;
		rc.right = textRect.right;
		rc.top  += rc.Height() / 4;
		m_pImageList->Draw(pDC, nImage, CPoint(rc.left, rc.top), ILD_TRANSPARENT);
	}

	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nBkMode);
	pDC->SetTextColor(lColor);
	return TRUE;
}

CString CDrawRStockTitle::GetTipText(int nCellID, int nItemID)
{
	CString strText("");
	CString strKey("");
	strKey.Format("%04x%s",m_stkInfo.m_ciStockCode.m_cCodeType, m_stkInfo.GetCode());
	const HsCodeMark* pMark = m_iUserDefCfg->SearchMark(strKey);
	if (pMark)
	{
		if (pMark->m_nType == -1)
		{// ���ֱ�ʶ
			strText = pMark->m_strContent;
		}
	}
	return strText;
}

BOOL CDrawRStockTitle::GetRequestData( IRequestCollector* pReqCollector,StockUserInfo* pStockInfo,BOOL bForce)
{
	if(pStockInfo == NULL)
		return FALSE;

	m_pRegion->Invalidate(&m_rect, FALSE);
	return TRUE;
}
