#include "StdAfx.h"
#include "DrawRValue.h"
#include "Resource.h"
#include "GeneralHelper.h"
#include "HSCalculate.h"
#include "FormulaManPublic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDrawRValue::CDrawRValue(IDataSourceEx* pDataSource)
{
	SetStyle(TABLE_STYLE_SCREEN | TABLE_WIDTH_CHANGABLE);
	CreateSelf();
	CreateVerScroll(HSSB_VERT | HSSB_RIGHT);
	m_pDataSource = pDataSource;
	m_ptagAnsValueData = NULL;
}

CDrawRValue::~CDrawRValue(void)
{
	if (m_ptagAnsValueData)
	{
		delete m_ptagAnsValueData;
	}
}

void CDrawRValue::CreateSelf()
{
	SetRowCount(RValue_ROW_COUNT);
	SetRowHeight(RValue_ROW_HEIGHT);
	SetColCount(RValue_COL_COUNT);
	SetFixColCount(0);
	SetFixRowCount(0);
	
	DWORD dwItemStyle(0); // ��ԪС���ʽ
	DWORD dwCellStyle(0); // ��Ԫ���ʽ

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��Դ�ַ���
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//
	CString strName;

	/////////////////////////////// ��һ�� /////////////////////////////////////////
	// ��ͣ
	dwItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, nBufLenth);    
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// ��ͣ
	::LoadString(instance, IDS_STRING_DIETING, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// ��ӯ
	::LoadString(instance, IDS_STRING_SHIYING, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// ����
	::LoadString(instance, IDS_STRING_HUANSHOU, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// ��ͨ��
	::LoadString(instance, IDS_STRING_LIUTONGPAN, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// �ܽ��
	::LoadString(instance, IDS_STRING_ZONGJINE, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// �ɽ�����
	::LoadString(instance, IDS_STRING_CHENGJIAOBISHU, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// ÿ�ʾ���
	::LoadString(instance, IDS_STRING_MEIBIJUNLIANG, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// ÿ�ʾ���
	::LoadString(instance, IDS_STRING_MEIBIJUNE, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// ���Ӿ���
	::LoadString(instance, IDS_STRING_FENZHONGJUNLIANG, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// ���Ӿ���
	::LoadString(instance, IDS_STRING_FENZHONGJUNE, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// 10���ֵ
	::LoadString(instance, IDS_STRING_TENJUNZHI, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// 20���ֵ
	::LoadString(instance, IDS_STRING_TWENTYJUNZHI, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// 50���ֵ
	::LoadString(instance, IDS_STRING_FIFTYJUNZHI, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// һ�������
	::LoadString(instance, IDS_STRING_MONTHMAX, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// һ�������
	::LoadString(instance, IDS_STRING_MONTHMIN, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// һ�����
	::LoadString(instance, IDS_STRING_YEARMAX, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	// һ�����
	::LoadString(instance, IDS_STRING_YEARMIN, buf, nBufLenth);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);

	/////////////////////////////// �ڶ��� /////////////////////////////////////////
	dwItemStyle = ITEM_TEXT_RIGHT;
	for (int i = 0; i < RValue_ROW_COUNT; i++)
	{
		CreateCell("", TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, dwItemStyle);
	}

}

BOOL CDrawRValue::GetRequestData( IRequestCollector* pReqCollector, StockUserInfo* pStockInfo, BOOL bForce )
{
	if(pStockInfo == NULL)
		return FALSE;


	m_bIsShow = FALSE;
	//////////////////////////////////////////////////////////////////////////
	// ��ȡStockType;
	CArray<StockType> ayStockType;
	m_pDataSource->HSDataSourceEx_GetSystemMarketInfo((unsigned short*)&m_stkInfo.m_ciStockCode.m_cCodeType, 1, ayStockType);
	if (ayStockType.GetCount() < 1)
	{
		return FALSE;
	}
	m_stkType = ayStockType.GetAt(0);
	//////////////////////////////////////////////////////////////////////////
	// ����Value
	long lAskLen = sizeof(AskData);
	AskData* pAskData = (AskData*)(new char[lAskLen]);
	memset(pAskData, 0, lAskLen);

	pAskData->m_nType  = RT_VALUE;
	pAskData->m_nSize  = 1;
	memcpy(pAskData->m_pCode, &m_stkInfo.m_ciStockCode, sizeof(CodeInfo));
	pReqCollector->AddRequest((char*)pAskData, lAskLen);

	pAskData->m_nType = RT_AUTOPUSH_EXT;
	pReqCollector->AddRequest((char*)pAskData, lAskLen);
	
	delete[] pAskData;
	return TRUE;
}

int CDrawRValue::OnUpdateData( DataHead* pData )
{
	if(pData->m_nType == RT_VALUE)
	{
		m_bIsShow = TRUE;

		AnsValueData* pValData = (AnsValueData*)pData;
		if(m_ptagAnsValueData == NULL)
		{
			m_ptagAnsValueData = new AnsValueData;
			memset(m_ptagAnsValueData , 0, sizeof(AnsValueData));
		}
		memcpy(m_ptagAnsValueData, pValData, sizeof(AnsValueData));
		HandleValueData(m_ptagAnsValueData);
	}
	else if (pData->m_nType == RT_AUTOPUSH_EXT)
	{
		AnsHSAutoPushData* pEveryData = (AnsHSAutoPushData*)pData;
		if (pEveryData->m_nSize <= 0)
		{
			return -1;
		}
		else
		{
			CommRealTimeData_Ext* pRealData = (CommRealTimeData_Ext *)pEveryData->m_pstData;
			CommRealTimeData_Ext* pNextData = NULL;

			int nNowSize = 0;
			for(int i = 0; i < pEveryData->m_nSize; i++)
			{
				pNextData = pRealData->GetNext(&pRealData->m_ciStockCode,nNowSize);
				if( pNextData == pRealData )
				{
					pNextData = pRealData->GetNext(&pData->m_nPrivateKey.m_pCode,nNowSize);
				}

				if (pRealData->m_ciStockCode.CompareCode(&m_stkInfo.m_ciStockCode))
				{
					if(m_ptagAnsValueData == NULL)
					{
						m_ptagAnsValueData = new AnsValueData;
						memset(m_ptagAnsValueData , 0, sizeof(AnsValueData));
					}

					HSStockRealTime* pRealTime = &pRealData->GetShareData()->m_nowDataExt.m_stockRealTime;
					if(pRealTime->m_lTotal < m_ptagAnsValueData->m_lTotal )
					{
						return FALSE;		
					}
					
					m_ptagAnsValueData->m_lTickCount++;		
					m_ptagAnsValueData->m_lTotal = pRealTime->m_lTotal;
					m_ptagAnsValueData->m_fAvgPrice = pRealTime->m_fAvgPrice;
					m_ptagAnsValueData->m_lNewPrice = pRealTime->m_lNewPrice;
					m_ptagAnsValueData->m_nTime = 0;//m_stkType.m_nCurTime; // ��ʱȡ0
					HandleValueData(m_ptagAnsValueData);
				}
				pRealData = pNextData;
			}
		}
	}
	return TRUE;
}


void CDrawRValue::HandleValueData( AnsValueData* pData )
{
	if (pData == NULL)
		return;

	long  nUnit = m_stkInfo.m_nPriceUnit;
	short nDecimal = m_stkInfo.m_nDecimal;
	int   nHand = m_stkInfo.m_nHandNum;
	double dValue(0.0);
	char   strText[64];
	CString strName;
	

	//////////////////////////////////////////////////////////////////////////
	// �õ���������
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	HSStockRealTime stkRealTime;
	stkRealTime.m_lNewPrice = pData->m_lNewPrice;
	stkRealTime.m_lTotal = pData->m_lTotal;
	//////////////////////////////////////////////////////////////////////////
	// �ڶ���
	// ��ͣ��
	CPoint pos(1,0);
	pos.y = 0 - GetMove().y;
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText, GetColor(dValue, m_stkInfo.m_lPrevClose));

	// ��ͣ��
	pos.y ++;
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText, GetColor(dValue, m_stkInfo.m_lPrevClose));

	// ��ӯ��
	pos.y ++;
	dValue = CHSCalculate::GetShiying(&m_stkInfo, &stkRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	// ������
	pos.y ++;
	dValue = CHSCalculate::GetHuanshou(&m_stkInfo, &stkRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	// ��ͨ��
	pos.y ++;
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	SetCellText(pos, 0, strText);

	// �ܽ��
	pos.y ++;
	dValue = pData->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 1, 2, strText, LTOA_MASK_SHOWEX_UNITAUTO | LTOA_MASK_FIXEDLEN_IFZERO);
	SetCellText(pos, 0, strText);

	// �ɽ�����
	pos.y ++;
	strName.Format("%d��", pData->m_lTickCount);
	strncpy(strText, strName.GetString(), strName.GetLength() + 1);
	SetCellText(pos, 0, strText);

	// ÿ�ʾ���
	pos.y ++;
	strName.Format("%d��", (pData->m_lTotal / pData->m_lTickCount) / 100);
	strncpy(strText, strName.GetString(), strName.GetLength() + 1);
	SetCellText(pos, 0, strText);

	// ÿ�ʾ���
	pos.y ++;
	dValue = (double)pData->m_fAvgPrice / (double)pData->m_lTickCount;
	CGeneralHelper::LongToString(dValue, 0, 1, 2, strText, LTOA_MASK_SHOWEX_UNITWAN | LTOA_MASK_FIXEDLEN_IFZERO);
	SetCellText(pos, 0, strText);

	// ���Ӿ���
	pos.y ++;
	if(pData->m_nTime != -1)
		strName.Format( "%d��", pData->m_lTotal / (pData->m_nTime + 1) / 100);		
	else
		strName.Format( "%d��", pData->m_lTotal  / 100);
	strncpy(strText, strName.GetString(), strName.GetLength() + 1);
	SetCellText(pos, 0, strText);

	// ���Ӿ���
	pos.y ++;
	if(pData->m_nTime != -1)
		strName.Format( "%.2f��",(((double)(pData->m_fAvgPrice))/ (pData->m_nTime + 1))/10000); 
	else
		strName.Format( "%.2f��",(((double)(pData->m_fAvgPrice)))/10000); 
	strncpy(strText, strName.GetString(), strName.GetLength() + 1);
	SetCellText(pos, 0, strText);

	// 10���ֵ
	pos.y ++;
	dValue = pData->m_lMa10;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText, GetColor(dValue, m_stkInfo.m_lPrevClose));

	// 20���ֵ
	pos.y ++;
	dValue = pData->m_lMa20;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText, GetColor(dValue, m_stkInfo.m_lPrevClose));

	// 50���ֵ
	pos.y ++;
	dValue = pData->m_lMa50;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText, GetColor(dValue, m_stkInfo.m_lPrevClose));

	// һ�������
	pos.y ++;
	dValue = pData->m_lMonthMax;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText, GetColor(dValue, m_stkInfo.m_lPrevClose));

	// һ�������
	pos.y ++;
	dValue = pData->m_lMonthMin;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText, GetColor(dValue, m_stkInfo.m_lPrevClose));

	// һ�����
	pos.y ++;
	dValue = pData->m_lYearMax;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText, GetColor(dValue, m_stkInfo.m_lPrevClose));

	// һ�����
	pos.y ++;
	dValue = pData->m_lYearMin;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText, GetColor(dValue, m_stkInfo.m_lPrevClose));

	if (pFinanceData)
	{
		delete pFinanceData;
	}

	InValidate(NULL, FALSE);
}

WORD CDrawRValue::GetColor(double dNewValue, double dPreValue)
{
	if (dNewValue > dPreValue)
	{
		return TEXT_COLOR_UP;
	}
	else if (dNewValue == dPreValue)
	{
		return TEXT_COLOR_FLAT;
	}
	else
	{
		return TEXT_COLOR_DOWN;
	}
}