#include "StdAfx.h"
#include "DrawTradeStockForm.h"
#include "HSCalculate.h"
#include <atlcore.h>
#include "GeneralHelper.h"
#include "resource.h"
#include "HSMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define CLR_SELLCOUNT        RGB(0,0,255)
#define CLR_BUYCOUNT         RGB(255,0,0)
#define TRADEFORM_ROW_HIEGHT          18    // �и�
#define TRADEFORM_STRING_SPAN     "    "    // ���

CDrawTradeStockForm::CDrawTradeStockForm(IDataSourceEx* pDataSource)
{
	SetStyle(TABLE_STYLE_SCREEN);
	m_pDataSource = pDataSource;
	m_preCodeInfo.SetEmpty();
	m_nPreTotal = -1;
	m_lMaxCount = 0;
	m_stkInfo.SetEmpty();
	CreateSelf();
}

CDrawTradeStockForm::~CDrawTradeStockForm(void)
{
}

BOOL CDrawTradeStockForm::CreateSelf()
{
	WORD nSubMarket = MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
	switch (nSubMarket)
	{
	case KIND_BOND:
		return CreateBonds();
	default:
		return CreateStock();
	}

	return FALSE;
}

BOOL CDrawTradeStockForm::CreateBonds()
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // ��ԪС���ʽ
	DWORD lCellStyle(0); // ��Ԫ���ʽ

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��Դ�ַ���
	CAtlBaseModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SELL, buf, nBufLenth);
	strTitle = buf;					
	for (int i = 4; i >= 1; i--)
	{
		// ��5~��2
		::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);
	}
	// ��1
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_NUM1, buf, nBufLenth);
	strName = buf;
	CreateCell(strTitle+strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_BUY, buf, nBufLenth);
	strTitle = buf;
	for (int i = 0; i < 4; i++)
	{
		// ��1~��4
		::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);
	}
	// ��5
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_NUM1+4, buf, nBufLenth);
	strName = buf;
	CreateCell(strTitle+strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// ����
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZUOSHOU, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);

	// �ɽ�
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);
	//////////////////////////////////////////////////////////////////////////
	// �ڶ���
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_TIP;
	for (int i = 0; i < 4; i++)
	{
		//��5~��2				
		CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);
	}
	// ��1
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_BORDER_BOTTOM | CELL_TIP;
	CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_TIP;
	for (int i = 0; i < 4; i++)
	{
		// ��1~��4				
		CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);
	}
	// ��5
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_BORDER_BOTTOM | CELL_TIP;
	CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// ����
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_TIP;
	CreateCell("-", TEXT_COLOR_FLAT, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// �ɽ�
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_TIP;
	CreateCell("-", TEXT_COLOR_FLAT, TEXT_FONT_TRADE, lItemStyle, lCellStyle);
	//////////////////////////////////////////////////////////////////////////
	// ������
	lItemStyle = ITEM_TEXT_RIGHT;   
	for (int i = 0; i < 4; i++)
	{	// ��5~��2
		CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0); 
	}  
	// ��1
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_RIGHT;
	for (int i = 0; i < 4; i++)
	{   // ��1~��4
		CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);
	}
	// ��5
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// ��Ϣ
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_LIXI, buf, nBufLenth);
	strName = buf;
	CreateCell(TRADEFORM_STRING_SPAN + strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);
	// ȫ��
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_QUANJIA, buf, nBufLenth);
	strName = buf;
	CreateCell(TRADEFORM_STRING_SPAN + strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);

	//////////////////////////////////////////////////////////////////////////
	// ������
	lItemStyle = ITEM_TEXT_RIGHT;  
	for (int i = 0; i < 4; i++)
	{
		// ��5~��2
		CreateCell("-", TEXT_COLOR_TRADEXIANSHOU, TEXT_FONT_TRADE, lItemStyle, 0); 
	}
	// ��1
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("-", TEXT_COLOR_TRADEXIANSHOU, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_RIGHT;
	for (int i = 0; i < 4; i++)
	{
		// ��1~��4
		CreateCell("-", TEXT_COLOR_TRADEXIANSHOU, TEXT_FONT_TRADE, lItemStyle, 0); 
	}
	// ��5
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM ;
	CreateCell("-", TEXT_COLOR_TRADEXIANSHOU, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// ��Ϣ
	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("-", TEXT_COLOR_TRADEUP, TEXT_FONT_TRADE, lItemStyle, 0);

	// ȫ��
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_TIP;
	CreateCell("-", TEXT_COLOR_TRADEDOWN, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// ��ʼ���������
	SetColCount(4);
	InitColInfo();
	SetRowCount(m_ayCell.GetCount() / 4);
	SetRowHeight(TRADEFORM_ROW_HIEGHT);
	SetFixColCount(0);
	SetFixRowCount(0);
	

	delete[] buf;
	return TRUE;
}

BOOL CDrawTradeStockForm::CreateStock()
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // ��ԪС���ʽ
	DWORD lCellStyle(0); // ��Ԫ���ʽ

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��Դ�ַ���
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SELL, buf, nBufLenth);
	strTitle = buf;					
	for (int i = 4; i >= 1; i--)
	{
		// ��5~��2
		::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);
	}
	// ��1
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_NUM1, buf, nBufLenth);
	strName = buf;
	CreateCell(strTitle+strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_BUY, buf, nBufLenth);
	strTitle = buf;
	for (int i = 0; i < 4; i++)
	{
		// ��1~��4
		::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);
	}
	// ��5
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_NUM1+4, buf, nBufLenth);
	strName = buf;
	CreateCell(strTitle+strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// ����
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZUOSHOU, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);

	// �ɽ�
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);
	//////////////////////////////////////////////////////////////////////////
	// �ڶ���
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_TIP;
	for (int i = 0; i < 4; i++)
	{
		//��5~��2				
		CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);
	}
	// ��1
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_BORDER_BOTTOM | CELL_TIP;
	CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_TIP;
	for (int i = 0; i < 4; i++)
	{
		// ��1~��4				
		CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);
	}
	// ��5
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_BORDER_BOTTOM | CELL_TIP;
	CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// ����
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_TIP;
	CreateCell("-", TEXT_COLOR_FLAT, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// �ɽ�
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_TIP;
	CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);
	//////////////////////////////////////////////////////////////////////////
	// ������
	lItemStyle = ITEM_TEXT_RIGHT;   
	for (int i = 0; i < 4; i++)
	{	// ��5~��2
		CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0); 
	}  
	// ��1
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_RIGHT;
	for (int i = 0; i < 4; i++)
	{   // ��1~��4
		CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);
	}
	// ��5
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("-", TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// ��ͣ
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, nBufLenth);
	strName = buf;
	CreateCell(TRADEFORM_STRING_SPAN + strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);

	// ��ͣ
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_DIETING, buf, nBufLenth);
	strName = buf;
	CreateCell(TRADEFORM_STRING_SPAN + strName, TEXT_COLOR_TRADEFIXEDNAME, TEXT_FONT_TRADE, lItemStyle, 0);

	//////////////////////////////////////////////////////////////////////////
	// ������
	lItemStyle = ITEM_TEXT_RIGHT;  
	for (int i = 0; i < 4; i++)
	{
		// ��5~��2
		CreateCell("-", TEXT_COLOR_TRADEXIANSHOU, TEXT_FONT_TRADE, lItemStyle, 0); 
	}
	// ��1
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("-", TEXT_COLOR_TRADEXIANSHOU, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_RIGHT;
	for (int i = 0; i < 4; i++)
	{
		// ��1~��4
		CreateCell("-", TEXT_COLOR_TRADEXIANSHOU, TEXT_FONT_TRADE, lItemStyle, 0); 
	}
	// ��5
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM ;
	CreateCell("-", TEXT_COLOR_TRADEXIANSHOU, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// ��ͣ
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_TIP;
	CreateCell("-", TEXT_COLOR_TRADEUP, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// ��ͣ
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_TIP;
	CreateCell("-", TEXT_COLOR_TRADEDOWN, TEXT_FONT_TRADE, lItemStyle, lCellStyle);

	// ��ʼ���������
	SetColCount(4);
	InitColInfo();
	SetRowCount(m_ayCell.GetCount() / 4);
	SetRowHeight(TRADEFORM_ROW_HIEGHT);
	SetFixColCount(0);
	SetFixRowCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawTradeStockForm::DrawTableBK(CDC* pDC)
{
	COLORREF clrBK = ::GetSysColor(COLOR_BTNFACE);
	CRect rc = GetTableRect();
	pDC->FillSolidRect(&rc, clrBK);
	return TRUE;
}

BOOL CDrawTradeStockForm::DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem)
{
	if (pos.x == 2 && m_lMaxCount != 0)
	{// ������                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          

		int nWidth(0);
		if (pos.y < 5)
		{// ����  
			nWidth = (double)m_aySellCount[4-pos.y] * rcItem.Width() / m_lMaxCount;
			
			CRect rc = rcItem;
			rc.left = rc.right - nWidth;
			rc.DeflateRect(0, 15, 0, 15);
			pDC->FillSolidRect(rc, CLR_SELLCOUNT);
			return TRUE;
		}
		else if (pos.y < 10)
		{
			nWidth = (double)m_ayBuyCount[pos.y-5] * rcItem.Width() / m_lMaxCount;

			CRect rc = rcItem;
			rc.left = rc.right - nWidth;
			rc.DeflateRect(0, 15, 0, 15);
			pDC->FillSolidRect(rc, CLR_BUYCOUNT);
			return TRUE;
		}
	}
	else
	{// ������

		CDrawItem* pItem = GetItem(pos, nItemID);
		if (pItem->m_dwStyle & ITEM_TEXT_HIGHLIGHT)
		{// ���Ҫ������ʾ �ĳ� ���ο�

			double dValue = strtod(pItem->m_str, NULL);
			if (dValue > 0.0001)
			{// ���˵��۸�Ϊ0��
				rcItem.DeflateRect(1,1,1,1);
				CPen pen(PS_SOLID, 1, RGB(172, 168, 153));
				int  nBKMod = pDC->SetBkMode(TRANSPARENT);
				pDC->SelectObject(&pen);
				pDC->Rectangle(&rcItem);

				pDC->SetBkMode(nBKMod);
				pen.DeleteObject();	

				pItem->m_dwStyle &= ~ITEM_TEXT_HIGHLIGHT;
			}
			else
				return TRUE;
		}
	}
	return FALSE;
}

CString CDrawTradeStockForm::GetTipText(int nCellID, int nItemID)
{
	CString strContent("");
	GetCellText(nCellID, nItemID, strContent);

	double dValue = strtod(strContent.GetBuffer(), NULL);
	HSStockRealTime realTime;
	memset(&realTime, 0, sizeof(HSStockRealTime));

	realTime.m_lNewPrice = dValue * m_stkInfo.m_nPriceUnit;
	
	char strText[64];
	CHSCalculate::GetFudu(FALSE, &m_stkInfo, &realTime, strText);

	strContent.Format("�ǵ���: %s", strText);
	return strContent;
}

BOOL CDrawTradeStockForm::GetRequestData( IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce)
{
	//////////////////////////////////////////////////////////////////////////
	// ��һЩ�����ָ���ʼֵ
	m_nPreTotal = -1;
	m_lMaxCount = 0;
	m_ayBuyCount[0] = m_ayBuyCount[1] = 0;
	m_ayBuyCount[2] = m_ayBuyCount[3] = 0;
	m_ayBuyCount[4] = 0;

	m_aySellCount[0] = m_aySellCount[1] = 0;
	m_aySellCount[2] = m_aySellCount[3] = 0;
	m_aySellCount[4] = 0;
	//
	
	if (pStockInfo == NULL)
	{// ��Ҫ���������
		m_preCodeInfo.SetEmpty();
		RemoveAllCell();
		CreateSelf();
		CalCulateSize();
		return FALSE;
	}

	if (pStockInfo->m_ciStockCode.m_cCodeType != m_preCodeInfo.m_cCodeType)
	{// �������򵥴��� ��ʵ�ڽ������� ֻ��ծȯ�Լ���ծȯ֮��
		// ��ͬ�г���Ҫ�������ɵ�Ԫ��
		RemoveAllCell();
		CreateSelf();
		CalCulateSize();
	}
	m_preCodeInfo = pStockInfo->m_ciStockCode;

	int lAskLen = sizeof(AskData);
	char* pData = new char[lAskLen]; 
	AskData*  pAskData = (AskData*)pData;
	memset(pAskData, 0, lAskLen);
	pAskData->m_nType  = RT_REALTIME_EXT;
	pAskData->m_nSize  = 1;
	memcpy(pAskData->m_pCode, &pStockInfo->m_ciStockCode, sizeof(CodeInfo));
	pReqCollector->AddRequest((char*)pAskData, lAskLen);

	//////////////////////////////////////////////////////////////////////////
	// ��������
	pAskData->m_nType = RT_AUTOPUSH_EXT;
	pReqCollector->AddRequest((char*)pAskData, lAskLen);

	delete[] pData;
	return TRUE;
}

int CDrawTradeStockForm::OnUpdateData( DataHead* pData )
{
	if( pData->m_nType == RT_REALTIME_EXT)
	{// level1��չ����
		AnsRealTime*  pNowData  = (AnsRealTime*)pData;
		if(pNowData->m_nSize <= 0 )
		{
			return -1;
		}
		else
		{
			CommRealTimeData_Ext* pRealData = (CommRealTimeData_Ext *)pNowData->m_pnowData;
			HandleNowData( (DataHead*)pData,  pRealData, pNowData->m_nSize);
		}
	}
	else if (pData->m_nType == RT_AUTOPUSH_EXT)
	{// level1��������
		AnsHSAutoPushData* pEveryData = (AnsHSAutoPushData*)pData;
		if (pEveryData->m_nSize <= 0)
		{
			return -1;
		}
		else
		{
			CommRealTimeData_Ext* pRealData = (CommRealTimeData_Ext *)pEveryData->m_pstData;
			HandleNowData( (DataHead*)pData, pRealData, pEveryData->m_nSize);
		}
	}
	return 0;
}

void CDrawTradeStockForm::HandleNowData( DataHead* pHead, CommRealTimeData_Ext* pnowData, int nSize )
{
	if( pnowData == NULL || nSize <= 0 )
		return;

	CommRealTimeData_Ext* pData = pnowData;
	CommRealTimeData_Ext* pNextData = NULL;

	int nNowSize = 0;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}

		if (pData->m_ciStockCode.CompareCode(&m_stkInfo.m_ciStockCode))
		{
			WORD nSubMarket = MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
			if (nSubMarket == KIND_BOND)
			{// ծȯ
				if(UpdateBondsData(pData->GetShareData(), &pData->m_othData) == FALSE)
				{
					pData = pNextData;
					continue;
				}
				else
					break;
			}
			else
			{// ����
				if(UpdateStockData(pData->GetShareData(), &pData->m_othData) == FALSE)
				{
					pData = pNextData;
					continue;
				}
				else
					break;
			}
		}
		pData = pNextData;		
	}
}

BOOL CDrawTradeStockForm::UpdateBondsData( ShareRealTimeData_Ext* pStock, StockOtherData* pOther )
{
	if ( pStock == NULL || ::IsBadReadPtr(pStock, 1) || pOther == NULL || ::IsBadReadPtr(pOther, 1))
	{
		return FALSE;
	}

	HSStockRealTime_Ext* pRealTimeExt = &pStock->m_nowDataExt;
	if (pRealTimeExt == NULL || ::IsBadReadPtr(pRealTimeExt,1))
	{
		return FALSE;
	}

	HSStockRealTime* pRealTime = &pRealTimeExt->m_stockRealTime;
	if (pRealTime == NULL || ::IsBadReadPtr(pRealTime,1))
	{
		return FALSE;
	}

	int nHand = m_stkInfo.m_nHandNum; // ծȯnHand=10
	double dTotal = (double)pRealTime->m_lTotal / (double)nHand;
	if (dTotal == m_nPreTotal)
		return FALSE;
	m_nPreTotal = dTotal;

	BOOL bIsStop = TRUE;//�Ƿ�ͣ��
	if (pRealTimeExt->m_stockOther.m_lStopFlag == 0)
	{
		bIsStop = FALSE;
	}

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;
	double  dValue = 0.0;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	m_ayBuyCount[0] = pRealTime->m_lBuyCount1; m_ayBuyCount[1] = pRealTime->m_lBuyCount2;
	m_ayBuyCount[2] = pRealTime->m_lBuyCount3; m_ayBuyCount[3] = pRealTime->m_lBuyCount4;
	m_ayBuyCount[4] = pRealTime->m_lBuyCount5;
	
	m_aySellCount[0] = pRealTime->m_lSellCount1; m_aySellCount[1] = pRealTime->m_lSellCount2;
	m_aySellCount[2] = pRealTime->m_lSellCount3; m_aySellCount[3] = pRealTime->m_lSellCount4;
	m_aySellCount[4] = pRealTime->m_lSellCount5;
	m_lMaxCount = FindMaxCount();

	long BuyPrice[] = {pRealTime->m_lBuyPrice1, pRealTime->m_lBuyPrice2, pRealTime->m_lBuyPrice3,
		pRealTime->m_lBuyPrice4, pRealTime->m_lBuyPrice5};
	long SellPrice[] = {pRealTime->m_lSellPrice1, pRealTime->m_lSellPrice2, pRealTime->m_lSellPrice3,
		pRealTime->m_lSellPrice4, pRealTime->m_lSellPrice5};
	//////////////////////////////////////////////////////////////////////////
	// �ڶ���
	pos.x = 1;
	pos.y = -1;
	//��5~��1 ��
	for (int i = 4; i >= 0; i--)
	{
		dValue = SellPrice[i];
		CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		pos.y++;
		lClr = GetTextColor(dValue, nClose);
		SetCellText(pos, 0, strText, lClr);
	}

	//��1~��5 ��
	for (int i = 0; i < 5; i++)
	{
		dValue = BuyPrice[i];
		CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		pos.y++;
		lClr = GetTextColor(dValue, nClose);
		SetCellText(pos, 0, strText, lClr);
	}

	// ����
	dValue = m_stkInfo.m_lPrevClose;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	// �ɽ�
	dValue = pRealTime->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//////////////////////////////////////////////////////////////////////////
	// ������
	pos.x = 3;
	pos.y = -1;

	//��5 ~ ��1��
	for (int i = 4; i >= 0; i--)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, m_aySellCount[i], strText);		
		SetCellText(pos, 0, strText);
	}

	// ��1 ~ ��5��
	for (int i = 0; i < 5; i++)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, m_ayBuyCount[i], strText);
		SetCellText(pos, 0, strText);
	}

	// ��Ϣ
	dValue = pRealTime->m_lNationalDebtRatio;
	CGeneralHelper::LongToString(dValue, 0, 1000, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	// ȫ��
	dValue = CHSCalculate::GetQuanjia(&m_stkInfo, pRealTime, strText);
	pos.y++;
	SetCellText(pos, 0, strText, GetTextColor(dValue, nClose));

	InValidate(NULL, FALSE);
	return TRUE;
}

BOOL CDrawTradeStockForm::UpdateStockData( ShareRealTimeData_Ext* pStock, StockOtherData* pOther )
{
	if ( pStock == NULL || ::IsBadReadPtr(pStock, 1) || pOther == NULL || ::IsBadReadPtr(pOther, 1))
	{
		return FALSE;
	}

	HSStockRealTime_Ext* pRealTimeExt = &pStock->m_nowDataExt;
	if (pRealTimeExt == NULL || ::IsBadReadPtr(pRealTimeExt,1))
	{
		return FALSE;
	}

	HSStockRealTime* pRealTime = &pRealTimeExt->m_stockRealTime;
	if (pRealTime == NULL || ::IsBadReadPtr(pRealTime,1))
	{
		return FALSE;
	}

	int nHand = m_stkInfo.m_nHandNum; // ��ƱnHand=10
	double dTotal = (double)pRealTime->m_lTotal / (double)nHand;
	if (dTotal == m_nPreTotal)
		return FALSE;
	m_nPreTotal = dTotal;

	BOOL bIsStop = TRUE;//�Ƿ�ͣ��
	if (pRealTimeExt->m_stockOther.m_lStopFlag == 0)
	{
		bIsStop = FALSE;
	}

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;
	double  dValue = 0.0;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	m_ayBuyCount[0] = pRealTime->m_lBuyCount1; m_ayBuyCount[1] = pRealTime->m_lBuyCount2;
	m_ayBuyCount[2] = pRealTime->m_lBuyCount3; m_ayBuyCount[3] = pRealTime->m_lBuyCount4;
	m_ayBuyCount[4] = pRealTime->m_lBuyCount5;

	m_aySellCount[0] = pRealTime->m_lSellCount1; m_aySellCount[1] = pRealTime->m_lSellCount2;
	m_aySellCount[2] = pRealTime->m_lSellCount3; m_aySellCount[3] = pRealTime->m_lSellCount4;
	m_aySellCount[4] = pRealTime->m_lSellCount5;
	m_lMaxCount = FindMaxCount();

	long BuyPrice[] = {pRealTime->m_lBuyPrice1, pRealTime->m_lBuyPrice2, pRealTime->m_lBuyPrice3,
		pRealTime->m_lBuyPrice4, pRealTime->m_lBuyPrice5};
	long SellPrice[] = {pRealTime->m_lSellPrice1, pRealTime->m_lSellPrice2, pRealTime->m_lSellPrice3,
		pRealTime->m_lSellPrice4, pRealTime->m_lSellPrice5};
	//////////////////////////////////////////////////////////////////////////
	// �ڶ���
	pos.x = 1;
	pos.y = -1;
	//��5~��1 ��
	for (int i = 4; i >= 0; i--)
	{
		dValue = SellPrice[i];
		CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		pos.y++;
		lClr = GetTextColor(dValue, nClose);
		SetCellText(pos, 0, strText, lClr);
	}

	//��1~��5 ��
	for (int i = 0; i < 5; i++)
	{
		dValue = BuyPrice[i];
		CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		pos.y++;
		lClr = GetTextColor(dValue, nClose);
		SetCellText(pos, 0, strText, lClr);
	}

	// ����
	dValue = m_stkInfo.m_lPrevClose;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	// �ɽ�
	dValue = pRealTime->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//////////////////////////////////////////////////////////////////////////
	// ������
	pos.x = 3;
	pos.y = -1;

	//��5 ~ ��1��
	for (int i = 4; i >= 0; i--)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, m_aySellCount[i], strText);		
		SetCellText(pos, 0, strText);
	}

	// ��1 ~ ��5��
	for (int i = 0; i < 5; i++)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, m_ayBuyCount[i], strText);
		SetCellText(pos, 0, strText);
	}

	// ��ͣ
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, 1000, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	// ��ͣ
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	InValidate(NULL, FALSE);
	return TRUE;
}

void CDrawTradeStockForm::InitColInfo()
{
	ColInfoStruct col(30);
	SetColInfo(0, &col);

	col.nWidth = 40;
	SetColInfo(1, &col);
	
	col.nWidth = 40;
	SetColInfo(2, &col);
	
	col.nWidth = 50;
	SetColInfo(3, &col);
}

long CDrawTradeStockForm::FindMaxCount()
{
	long lMax(0);
	for (int i = 0; i < 5; i++)
		lMax = lMax<m_ayBuyCount[i]?m_ayBuyCount[i]:lMax;

	for (int i = 0; i < 5; i++)
		lMax = lMax<m_aySellCount[i]?m_aySellCount[i]:lMax;

	return lMax;
}

WORD CDrawTradeStockForm::GetTextColor( double newValue, double preValue )
{
	double diff = newValue - preValue;
	if (diff > 0)
	{
		return TEXT_COLOR_TRADEUP;   // �ǵ���ɫ
	}
	else if (diff < 0)
	{
		return TEXT_COLOR_TRADEDOWN; // ������ɫ
	}
	else
	{
		return TEXT_COLOR_TRADEFIXEDNAME; // ����ƽ����ɫ
	}
}

BOOL CDrawTradeStockForm::MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData )
{
	int nActiveCellID = GetActiveCellID();
	CDrawCell* pCell = GetCell( nActiveCellID );
	if ( pCell && GetSignal() == NEED_TEXTHIGHLIGHT)
	{
		CString str("");
		GetCellText(GetActiveCellID(), 0, str);
		double dPrice = strtod(str, NULL);
		if (dPrice - 0.0001 > 0)
		{// �۸�Ϊ0 
			//////////////////////////////////////////////////////////////////////////
			// ��������Ϣ
			CString strFormat("%");
			strFormat.AppendFormat(".%df", m_stkInfo.m_nDecimal);
			str.Format(strFormat, dPrice);

			CDrawBaseParam param;
			param.AddParam(PARAM_TRADE_PRICE, str);
			param.AddParam(PARAM_TRADE_CODE, m_stkInfo.GetCode());
			m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_TRADE, &param);
		}
	}
	return TRUE;
}