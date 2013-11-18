#include "StdAfx.h"
#include "DrawRLevelForm.h"
#include <atlcore.h>
#include "GeneralHelper.h"
#include "Resource.h"
#include "HSCalculate.h"
#include "HSMessage.h"
#include "HSWinner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �Ҽ��˵�ID
#define ID_QUICKTRADE_BUY   0x0001
#define ID_QUICKTRADE_SELL  0x0002

CDrawRLevelForm::CDrawRLevelForm(IDataSourceEx* pDataSource)
{
	SetStyle(TABLE_STYLE_SCREEN | TABLE_HEIGHT_CHANGABLE | TABLE_WIDTH_CHANGABLE);
	m_sLevelData.m_ciStockCode.SetEmpty();
	m_pDataSource = pDataSource;
	CreateSelf();
}

CDrawRLevelForm::~CDrawRLevelForm(void)
{
}

BOOL CDrawRLevelForm::CreateSelf()
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

	//////////////////////////////////////////////////////////////////////////
	//��һ��

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SELL, buf, nBufLenth);
	strTitle = buf;	
	for (int i = 9; i >= 1; i--)
	{
		// ��10~��2
		::LoadString(instance, IDS_STRING_NUM1 + i, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
	}
	// ��1
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_NUM1, buf, nBufLenth);
	strName = buf;
	CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_BUY, buf, nBufLenth);
	strTitle = buf;
	for (int i = 0; i < 9; i++)
	{
		// ��1~��9
		::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
	}
	// ��10
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_NUM1+9, buf, nBufLenth);
	strName = buf;
	CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	// ����
	::LoadString(instance, IDS_STRING_SELLAVG, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	// ���
	::LoadString(instance, IDS_STRING_BUYAVG, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//////////////////////////////////////////////////////////////////////////
	// �ڶ���
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	for (int i = 0; i < 9; i++)
	{
		//��10~��2				
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
	}
	// ��1
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	for (int i = 0; i < 9; i++)
	{
		// ��1~��9			
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
	}
	// ��10
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

	// ����
	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);

	// ���
	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);

	//////////////////////////////////////////////////////////////////////////
	// ������
	// ��ʾʮ�����鱨��
		lItemStyle = ITEM_TEXT_RIGHT;   
	for (int i = 0; i < 9; i++)
	{	// ��10~��2
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
	}  
	// ��1
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_RIGHT ;
	for (int i = 0; i < 9; i++)
	{   // ��1~��9
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
	}
	// ��10
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

	// ����
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SELLTOTAL, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	// ����
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_BUYTOTAL, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//////////////////////////////////////////////////////////////////////////
	// ������
	//��ʾί�����仯
	lItemStyle = ITEM_TEXT_RIGHT;  
	for (int i = 0; i < 9; i++)
	{
		// ��10~��2
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
	}
	// ��1
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_RIGHT;
	for (int i = 0; i < 9; i++)
	{
		// ��1~��9
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
	}
	// ��10
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM ;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	
	// ����
	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_CHENGJIAO, TEXT_FONT_DYNAMICAL, lItemStyle, 0);

	// ����
	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_CHENGJIAO, TEXT_FONT_DYNAMICAL, lItemStyle, 0);

	// ��ʼ���������
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixRowCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawRLevelForm::GetRequestData( IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce)
{
	if (pStockInfo == NULL)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��һЩ�����ָ���ʼֵ
	m_sellMap.RemoveAll();
	m_buyMap.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	int lAskLen = sizeof(AskData);
	char* pData = new char[lAskLen]; 
	AskData*  pAskData = (AskData*)pData;
	memset(pAskData, 0, lAskLen);
	pAskData->m_nType  = RT_LEVEL_REALTIME;
	pAskData->m_nSize  = 1;
	memcpy(pAskData->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
	pReqCollector->AddRequest((char*)pAskData,lAskLen);

	pAskData->m_nType = RT_LEVEL_AUTOPUSH;
	pReqCollector->AddRequest((char*)pAskData,lAskLen);
	delete[] pData;

	return TRUE;
}

int CDrawRLevelForm::OnUpdateData( DataHead* pData )
{
	if (pData->m_nType == RT_LEVEL_AUTOPUSH || pData->m_nType == RT_LEVEL_REALTIME)
	{
		// level2��չ���� �� ���ư�
		AnsHSAutoPushLevel* pLevelAuto = (AnsHSAutoPushLevel*)pData;
		if (pLevelAuto->m_nSize <= 0)
		{
			return -1;
		}
		else
		{
			if (memcmp(&m_sLevelData, pLevelAuto->m_pstData, sizeof(RealTimeDataLevel)) == 0)
				return 0;

			memcpy(&m_sLevelData, pLevelAuto->m_pstData, sizeof(RealTimeDataLevel));
			CodeInfo* pCodeInfo = &(pLevelAuto->m_pstData->m_ciStockCode);
			if (pCodeInfo->CompareCode(&m_stkInfo.m_ciStockCode))
			{
				LevelRealTime* pLevelReal = &pLevelAuto->m_pstData->m_sLevelRealTime;
				LevelStockOtherData* pOtherData = &pLevelAuto->m_pstData->m_othData;
				UpdateData(pLevelReal, pOtherData);
			}
		}
	}
	return 1;
}

BOOL CDrawRLevelForm::UpdateData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData)
{
	if (pLevelData == NULL || pOtherData == NULL)
		return FALSE;

	long BuyCount[] = {pLevelData->m_lBuyCount1, pLevelData->m_lBuyCount2, pLevelData->m_lBuyCount3,
		pLevelData->m_lBuyCount4, pLevelData->m_lBuyCount5, pLevelData->m_lBuyCount6, pLevelData->m_lBuyCount7,
		pLevelData->m_lBuyCount8, pLevelData->m_lBuyCount9, pLevelData->m_lBuyCount10};
	long SellCount[] = {pLevelData->m_lSellCount1, pLevelData->m_lSellCount2, pLevelData->m_lSellCount3,
		pLevelData->m_lSellCount4, pLevelData->m_lSellCount5, pLevelData->m_lSellCount6, pLevelData->m_lSellCount7,
	    pLevelData->m_lSellCount8, pLevelData->m_lSellCount9, pLevelData->m_lSellCount10};
	long BuyPrice[] = {pLevelData->m_lBuyPrice1, pLevelData->m_lBuyPrice2, pLevelData->m_lBuyPrice3,
		pLevelData->m_lBuyPrice4, pLevelData->m_lBuyPrice5, pLevelData->m_lBuyPrice6, pLevelData->m_lBuyPrice7,
		pLevelData->m_lBuyPrice8, pLevelData->m_lBuyPrice9, pLevelData->m_lBuyPrice10};
	long SellPrice[] = {pLevelData->m_lSellPrice1, pLevelData->m_lSellPrice2, pLevelData->m_lSellPrice3,
		pLevelData->m_lSellPrice4, pLevelData->m_lSellPrice5, pLevelData->m_lSellPrice6, pLevelData->m_lSellPrice7,
		pLevelData->m_lSellPrice8, pLevelData->m_lSellPrice9, pLevelData->m_lSellPrice10};
	// ��ʼ���۸�map
	InitPriceMap(m_buyMap,  BuyPrice);
	InitPriceMap(m_sellMap, SellPrice);
	
	short   nHand    = m_stkInfo.m_nHandNum;
	long    lClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;
	//////////////////////////////////////////////////////////////////////////
	CPoint pos(1,-1); // �ӵڶ��п�ʼ��������
	double dValue(0.0);
	char   strText[64];
	WORD   lClr(0);

	//��10~��1 ��
	for (int i = 9; i >=0; i--)
	{
		dValue = SellPrice[i];
		CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		pos.y++;
		lClr = GetTextColor(dValue, lClose);
		SetCellText(pos, 0, strText, lClr);
	}

	//��1~��10 ��
	for (int i = 0; i < 10; i++)
	{
		dValue = BuyPrice[i];
		CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		pos.y++;
		lClr = GetTextColor(dValue, lClose);
		SetCellText(pos, 0, strText, lClr);
	}

	// ����
	dValue = CHSCalculate::GetSellAvg(&m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, lClose/(double)nUnit);
	SetCellText(pos, 0, strText, lClr);

	// ���
	dValue = CHSCalculate::GetBuyAvg(&m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, lClose/(double)nUnit);
	SetCellText(pos, 0, strText, lClr);

	//////////////////////////////////////////////////////////////////////////
	// ������
	pos.x = 2; 
	pos.y = -1;
	//��10 ~ ��1��
	for (int i = 9; i >= 0; i--)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, SellCount[i], strText);		
		SetCellText(pos, 0, strText);

		// ͬʱ����õ��ĳɽ���
		lClr = GetSellChange(SellPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	// ��1 ~ ��10��
	for (int i = 0; i < 10; i++)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, BuyCount[i], strText);
		SetCellText(pos, 0, strText);

		// ͬʱ����õ��ĳɽ���
		lClr = GetBuyChange(BuyPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	//////////////////////////////////////////////////////////////////////////
	// ������
	pos.x = 3; 
	pos.y = -1;
	//��10 ~ ��1 ���仯
	for (int i = 9; i >= 0; i--)
		pos.y++;

	//��1 ~ ��10 ���仯
	for (int i = 0; i < 10; i++)
		pos.y++;

	// ����
	dValue = pLevelData->m_fSellTotal;
	CGeneralHelper::LongToString(dValue, 0, 1, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);
	pos.y++;
	SetCellText(pos, 0, strText);

	// ����
	dValue = pLevelData->m_fBuyTotal;
	CGeneralHelper::LongToString(dValue, 0, 1, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);
	pos.y++;
	SetCellText(pos, 0, strText);

	InValidate(NULL, FALSE);
	return TRUE;
}

void CDrawRLevelForm::InitPriceMap( CMap<long,long,long,long> &lMap, long lPrice[10] )
{
	long lKey = 0;
	long lValue = 0;
	// ȥ��map�й�ʱ�ļ۸�
	POSITION pos = lMap.GetStartPosition();
	while(pos)
	{
		lMap.GetNextAssoc(pos, lKey, lValue);

		BOOL bExist = FALSE;
		for (int i = 0; i < 9; i++)
		{
			if (lKey == lPrice[i])
			{
				bExist = TRUE;
				break;
			}
		}

		if (bExist == FALSE)
			lMap.RemoveKey(lKey);
	}
}

WORD CDrawRLevelForm::GetSellChange(long lSellPrice, long lNowSellCount, char* strText)
{
	double dValue = 0.0;
	if (lSellPrice)
	{// ���˼۸�Ϊ0
		long lPreSellCount = -1;
		if (m_sellMap.Lookup(lSellPrice, lPreSellCount))
			dValue = lNowSellCount - lPreSellCount;
		m_sellMap.SetAt(lSellPrice, lNowSellCount);
	}

	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_NOTHING_IFZERO);
	return GetTextColor(dValue, 0);
}
WORD CDrawRLevelForm::GetBuyChange(long lBuyPrice, long lNowBuyCount, char* strText)
{
	double dValue = 0.0;
	if (lBuyPrice)
	{// ���˼۸�Ϊ0
		long lPreBuyCount = -1;
		if (m_buyMap.Lookup(lBuyPrice, lPreBuyCount))
			dValue = lNowBuyCount - lPreBuyCount;
		m_buyMap.SetAt(lBuyPrice, lNowBuyCount);
	}

	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_NOTHING_IFZERO);
	return GetTextColor(dValue, 0);
}

WORD CDrawRLevelForm::GetTextColor( double newValue, double preValue )
{
	double diff = newValue - preValue;
	if (diff > 0)
	{
		return TEXT_COLOR_UP;   // �ǵ���ɫ
	}
	else if (diff < 0)
	{
		return TEXT_COLOR_DOWN; // ������ɫ
	}
	else
	{
		return TEXT_COLOR_FLAT; // ƽ����ɫ
	}
}

BOOL CDrawRLevelForm::MouseDBDown( UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/ )
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
			CPoint pos = IndexToPoint(nActiveCellID);
			BOOL bBuyDirection = 0;
			if (pos.y >= 0 && pos.y < 10)
			{// ����
				bBuyDirection = 1;
			}

			CString strFormat("%");
			strFormat.AppendFormat(".%df", m_stkInfo.m_nDecimal);
			if (bBuyDirection)
			{// ��������
				//////////////////////////////////////////////////////////////////////////
				// ��Ӳ���
				CDrawBaseParam param;
				str.Format(strFormat, dPrice);
				param.AddParam(PARAM_QUICKTRADE_PRICE, str);
				param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
				m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_BUY);
			}
			else
			{// ��������
				//////////////////////////////////////////////////////////////////////////
				// ��Ӳ���
				CDrawBaseParam param;
				str.Format(strFormat, dPrice);
				param.AddParam(PARAM_QUICKTRADE_PRICE, str);
				param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
				m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_SELL);
			}
		}
	}
	return TRUE;
}

BOOL CDrawRLevelForm::RButtonDown( UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData /*= NULL*/ )
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

			ResetCell(pCell);
			//////////////////////////////////////////////////////////////////////////
			// ��ȡ��Դ�ַ���
			CAtlBaseModule _Module;
			//CComModule _Module;
			HINSTANCE instance = _Module.GetResourceInstance();
			char* buf = new char[1000];
			int   nBufLenth = 1000;
			//

			CMenu popMenu;
			popMenu.CreatePopupMenu();
			::LoadString(instance, IDS_STRING_QUICKBUY, buf, nBufLenth);  // ��������
			str = buf;
			popMenu.AppendMenu(0, ID_QUICKTRADE_BUY, str);

			::LoadString(instance, IDS_STRING_QUICKSELL, buf, nBufLenth); // ��������
			str = buf;
			popMenu.AppendMenu(0, ID_QUICKTRADE_SELL, str);

			int nMenuID(-1);
			if (popMenu.GetMenuItemCount() > 0)
			{
				::ClientToScreen(hWndParent, &point);
				CWnd* pWnd = CWnd::FromHandle(hWndParent);
				nMenuID = popMenu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,point.x,point.y, pWnd);
			}

			if (nMenuID > 0)
			{
				BOOL bBuyDirection = (nMenuID == ID_QUICKTRADE_BUY)?0:1;

				CString strFormat("%");
				strFormat.AppendFormat(".%df", m_stkInfo.m_nDecimal);
				if (bBuyDirection)
				{// ��������
					//////////////////////////////////////////////////////////////////////////
					// ��Ӳ���
					CDrawBaseParam param;
					str.Format(strFormat, dPrice);
					param.AddParam(PARAM_QUICKTRADE_PRICE, str);
					param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
					m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_BUY);
				}
				else
				{// ��������
					//////////////////////////////////////////////////////////////////////////
					// ��Ӳ���
					CDrawBaseParam param;
					str.Format(strFormat, dPrice);
					param.AddParam(PARAM_QUICKTRADE_PRICE, str);
					param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
					m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_SELL);
				}
			}
			popMenu.DestroyMenu();
			delete[] buf;	
		}
	}
	return TRUE;
}