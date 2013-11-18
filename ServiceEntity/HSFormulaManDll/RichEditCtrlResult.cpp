#include "stdafx.h"
#include "RichEditCtrlResult.h"
#include "ExchCalcMain.h"

#define EXCH_TIP_SETTING   "EXCHTIP.INI"

extern CString GetPeriodStr(WORD dPeriod,int* nPos);
// �Ƿ��Ǻ���
extern int IsChinese(BYTE cChar,const CString& strSource,int& nPos);
extern int IsChinese(BYTE cChar,const CString& strSource,int& nPos,BOOL bChange);
extern double EXCH_POUNDAGE;

BEGIN_MESSAGE_MAP(CRichEditCtrlResult, CRichEditCtrlEx)
ON_WM_MOUSEMOVE()
ON_WM_CREATE()
END_MESSAGE_MAP()

CRichEditCtrlResult::CRichEditCtrlResult(void)
{
	
}

CRichEditCtrlResult::~CRichEditCtrlResult(void)
{
	// ��ʾ��Ϣ
}

CString CRichEditCtrlResult::GetBuyCon(void* pExchSys)
{
	if( pExchSys == NULL )
		return CString("");

	TExchSys* pExch = (TExchSys*)pExchSys;
	CString strTemp;
	CString strTech;

	strTech = pExch->m_sExchExp.GetExpTypeName();
	strTech += "-";
	strTech += pExch->m_sExchExp.m_pMainTech->GetName();
	strTech += pExch->m_sExchExp.GetParamStr();
	if( pExch->m_sExchExp.m_pMainTech->GetExpressType() == Exchange )
	{
		strTech += _T(" ��ʽ�������������");
		return strTech;
	}
	strTech += _T("ָ����:");
	strTech += pExch->m_sTExchBuy.m_strTechLeft;

	switch( pExch->m_sTExchBuy.m_SubData.m_cType )
	{
	case FormulaCompareSubData::Condition:
	case FormulaCompareSubData::Exchange:
	case FormulaCompareSubData::MoreKLine:
		strTemp += _T("��ʽ�ж������������");
		return strTemp;
		break;
	case FormulaCompareSubData::ShangChuan:	// �ϴ�
		strTemp += _T("�ϴ�");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		break;
	case FormulaCompareSubData::XiaoPo:		// ����
		strTemp += _T("����");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		break;
	case FormulaCompareSubData::DaYu: // >
		strTemp += _T("����");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		break;
	case FormulaCompareSubData::XiaoYu: // <		
		strTemp += _T("С��");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		break;
	case FormulaCompareSubData::DengYu: // =
		strTemp += _T("����");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		break;
	case FormulaCompareSubData::BiaoChi: // ����
		strTemp += _T("����");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		strTemp += _T("���ں�����");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData2;
		break;
	case FormulaCompareSubData::WeiChi:// ά��
		strTemp += _T("ά��");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		strTemp += _T("���ں��ϴ�");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData2;
		break;
	case FormulaCompareSubData::JieYu:// ����
		strTemp += _T("����");
		break;
	}
	return strTech + strTemp;
}

CString CRichEditCtrlResult::GetSellCom(void* pExchSys)
{
	if( pExchSys == NULL )
		return CString("");

	TExchSys* pExch = (TExchSys*)pExchSys;

	if( pExch->m_sExchExp.m_pMainTech->GetExpressType() == Exchange )
	{
		return CString(_T("��ʽ�ж������������"));
	}

	return CString(_T("����������"));
}

LRESULT CRichEditCtrlResult::Display(LPCSTR lpMsg /*= NULL*/)
{
	ClearScreen();

	AddItemLN(_T("------------------�������ӹɷ����޹�˾������ʾ----------------------"), HS_Rich_Text_Color_Blue);
	AddTab();
	AddItemLN(_T("�����Խ�������ο��������Ե���"), HS_Rich_Text_Color_Red);
	return 1;
}

LRESULT CRichEditCtrlResult::Display(void* pData, BOOL bSignal, UINT uID, TExchOptimize* pOptimize/* = NULL*/)
{
	if( pData == NULL )
		return -1;

	ClearScreen();

	CExchCalcMain* pExch = (CExchCalcMain*)pData;
	CString strTemp;
	CString strTech;
	CString str;

	AddItemLN(_T("                           ϵͳ�����趨"), HS_Rich_Text_Color_Black);

	AddItem(_T("���Է�����"), HS_Rich_Text_Color_Black);
	strTech = pExch->m_sExchExp.GetExpTypeName();
	strTech += "-";
	strTech += pExch->m_sExchExp.m_pMainTech->GetName();

	strTech += pExch->GetParamStr(bSignal, uID);

	AddItemLN(strTech, HS_Rich_Text_Color_Blue);

	AddItem(_T("����ʱ�䣺"), HS_Rich_Text_Color_Black);
	strTemp.Format("%d-%d\t\t", pExch->m_sTExchBuy.m_lBeginDate, pExch->m_sTExchBuy.m_lEndDate);
	strTemp += (pExch->m_sTExchBuy.m_bNoCountIn == FALSE) ? _T("��ǿ��ƽ��") : _T("����ǿ��ƽ��");
	AddItemLN(strTemp, HS_Rich_Text_Color_Blue);

	AddItem(_T("�������ڣ�"), HS_Rich_Text_Color_Black);
	strTemp.Format("%s", GetPeriodStr(pExch->m_sExchExp.m_lCyc, NULL));
	AddItemLN(strTemp, HS_Rich_Text_Color_Blue);

	AddItem(_T("���Թ�Ʊ��"), HS_Rich_Text_Color_Black);
	int lCount = pExch->m_sTExchModel.m_ayStkList.GetSize();
	if( lCount == 1 )
	{
		StockUserInfo* pStock;

		pStock = pExch->m_sTExchModel.m_ayStkList.GetAt(0);
		if ( pStock && !IsBadReadPtr(pStock,1))
		{
			strTemp = CString(pStock->m_cStockName, sizeof(pStock->m_cStockName));
			strTemp += "\t\t";
		}
	}
	else
	{
		strTemp.Format(_T("��%d֧\t\t"), lCount);
	}
	AddItem(strTemp, HS_Rich_Text_Color_Blue);
	AddItem(_T("��ʼͶ�룺"), HS_Rich_Text_Color_Black);
	strTemp.Format("%.2fԪ", pExch->m_sTExchModel.m_lAllInvestCount);
	AddItemLN(strTemp, HS_Rich_Text_Color_Blue);

	AddItem(_T("����������"), HS_Rich_Text_Color_Black);
	strTemp = GetBuyCon(pExch);
	AddItemLN(strTemp, HS_Rich_Text_Color_Blue);

	AddTab();
	AddItem(_T("  ����������ʱ��"), HS_Rich_Text_Color_Black);
	strTemp = _T("�ڱ����ڰ������̼�");
	switch( pExch->m_sTExchBuy.m_nBuyMethod )
	{
	default:
	case BuyMethod_All_Buy:// ȫ������
		{
			strTemp += _T("ʹ��ȫ���ʽ�����");
		}
		break;
	case BuyMethod_Portion_Buy:// ��������
		{
			str.Format(_T("ʹ��%.f%%�ʽ�����"), pExch->m_sTExchBuy.m_fOneBuyCount);
			strTemp += str;
		}
		break;
	case BuyMethod_Fixation_Money:// �̶��ʽ�����
		{
			str.Format(_T("Ͷ��%.2fԪ����"), pExch->m_sTExchBuy.m_fOneBuyCount);
			strTemp += str;
		}
		break;
	case BuyMethod_Fixation_NUM:// �̶���Ʊ������
		{
			str.Format(_T("����%.f��"), pExch->m_sTExchBuy.m_fOneBuyCount);
			strTemp += str;
		}
		break;
	}
	AddItemLN(strTemp, HS_Rich_Text_Color_Blue);

	AddTab();
	AddItem(_T("���������ź�ʱ��"), HS_Rich_Text_Color_Black);
	switch( pExch->m_sTExchBuy.m_nFollowBuyMethod )
	{
	case FollowBuyMethod_NO:// ��������	
		str = _T("��������");
		break;
	case FollowBuyMethod_DJ:// �ݼ�����
		str.Format(_T("��ǰһ�źż���%.2f%%����"),pExch->m_sTExchBuy.m_fAddOrSubCount);
		break;
	case FollowBuyMethod_DL:// ��������
		str = _T("����һ�źŵ�ͬ����");
		break;
	case FollowBuyMethod_DZ:// ��������
		str.Format(_T("��ǰһ�ź�����%.2f%%����"),pExch->m_sTExchBuy.m_fAddOrSubCount);
		break;
	}
	if( pExch->m_sTExchBuy.m_nFollowBuyMethod != FollowBuyMethod_NO && pExch->m_sTExchBuy.m_bSkip == TRUE )
	{
		str += ",";
		strTemp.Format(_T("����%d�����ڵ������ź�"), pExch->m_sTExchBuy.m_nInPeriod);
		str += strTemp;
	}
	AddItemLN(str, HS_Rich_Text_Color_Blue);
	
	AddItem(_T("����������"), HS_Rich_Text_Color_Black);
	str = GetSellCom(pExch);
	AddItemLN(str, HS_Rich_Text_Color_Blue);

	AddItemLN(_T("ƽ��������(�������̼�ƽ��)"), HS_Rich_Text_Color_Black);
	CStringArray ayStr;
	GetPCCon(pExch, ayStr);
	for( int i = 0; i < ayStr.GetSize(); i+=2 )
	{
		CString str1, str2;
		AddTab();
		str1 = ayStr.GetAt(i);
		if( i+1 < ayStr.GetSize() )
		{
			str2 = ayStr.GetAt(i+1);
		}

		str.Format("%-30s%-s", str1, str2);
		AddItemLN(str, HS_Rich_Text_Color_Blue);
	}

	// ����ȫ�г�ģ�Ͳ���
	if( pExch->m_sTExchModel.m_bSingle == FALSE )
	{
		AddNewLine();
		AddItemLN(_T("����ȫ�г�ģ�Ͳ���"), HS_Rich_Text_Color_Red);

		AddTab();
		AddItem(_T("�����й�Ʊ����"), HS_Rich_Text_Color_Black);
		str.Format(_T("%d֧"), pExch->m_sTExchModel.m_nHoleStockCount );
		AddItemLN(str, HS_Rich_Text_Color_Blue);

		AddTab();
		if( pExch->m_sTExchModel.m_bBuyAll == FALSE )
		{
			AddItem(_T("��󵥴������Ʊ����"), HS_Rich_Text_Color_Black);
			str.Format(_T("%d֧"), pExch->m_sTExchModel.m_nOneBuyCount );
			AddItemLN(str, HS_Rich_Text_Color_Blue);
		}
		else
		{
			AddItem(_T("��󵥴������Ʊ����"), HS_Rich_Text_Color_Black);
			AddItemLN(_T("ȫ������"), HS_Rich_Text_Color_Blue);
		}

		//AddTab();
		//AddItem("ͬʱ����������Ʊ����", HS_Rich_Text_Color_Black);
		//str.Format("%d", pExch->m_sTExchModel.m_nOneBuyCount);
		//AddItem(str, HS_Rich_Text_Color_Blue);
		//AddItem("ֻʱ������", HS_Rich_Text_Color_Black);
		//AddItem(pExch->m_sTExchModel.m_sSortReason.m_strExp, HS_Rich_Text_Color_Blue);
		//if( pExch->m_sTExchModel.m_bSToB )
		//{
		//	AddItem("��С����", HS_Rich_Text_Color_Blue);
		//}
		//else
		//{
		//	AddItem("�ɴ�С", HS_Rich_Text_Color_Blue);
		//}
		//AddItemLN("����ѡ��", HS_Rich_Text_Color_Black);
	}

	// ϵͳ�������²��������Ż�:
	if( pOptimize == NULL )
	{
		pOptimize = &pExch->m_sTExchOptimize;
	}
	if( pOptimize->GetValidItemCount() > 0 )
	{
		AddNewLine();
		AddItem(_T("ϵͳ�������²��������Ż�:( �Ż�����"), HS_Rich_Text_Color_Red);
		str.Format("%d", pOptimize->GetCaclUionCount());
		AddItem(str, HS_Rich_Text_Color_Blue);
		AddItemLN(")", HS_Rich_Text_Color_Red);

		for( int i = 0; i < pOptimize->GetValidItemCount(); i++ )
		{
			AddTab();
			TExchOptimizeItem* pItem = pOptimize->GetValidItem(i);
			str.Format(_T(" %s����Χ( %.2f--%.2f )��������%.2f"), pItem->GetName(), pItem->m_dMin, pItem->m_dMax, pItem->m_dAdd);
			AddItemLN(str, HS_Rich_Text_Color_Blue);
		}
	}
	return 1;
}

int CRichEditCtrlResult::GetPCCon(void* pExchSys, CStringArray& ayStr)
{
	if( pExchSys == NULL )
		return 0;

	CString str;
	TExchSys* pExch = (TExchSys*)pExchSys;

	if( pExch->m_sTExchSell.m_bMaidPeriod )
	{
		str.Format(_T("����%.f����ƽ��"), pExch->m_sTExchSell.m_nMaidPeriod);
		ayStr.Add(str);
	}

	if( pExch->m_sTExchSell.m_bWinPeriod )
	{
		str.Format(_T("����ﵽ%.f%%ֹӮ"), pExch->m_sTExchSell.m_nWinPeriod);
		ayStr.Add(str);
	}

	if( pExch->m_sTExchSell.m_bLosePeriod )
	{
		str.Format(_T("����ﵽ%.f%%ֹ��"), pExch->m_sTExchSell.m_nLosePeriod);
		ayStr.Add(str);
	}

	if( pExch->m_sTExchSell.m_bFallPeriod )
	{
		str.Format(_T("%d�����ڻ������ﵽ%d%%ƽ��"), pExch->m_sTExchSell.m_nFallPeriodCount, pExch->m_sTExchSell.m_nFallPeriod);
		ayStr.Add(str);        
	}

	if( pExch->m_sTExchSell.m_bUpPeriod )
	{
		str.Format(_T("%d�����ں��̣��������%d%%ƽ��"), pExch->m_sTExchSell.m_nUpPeriodCount, pExch->m_sTExchSell.m_nUpPeriod);
		ayStr.Add(str);        
	}
    
	return 1;
}

LRESULT CRichEditCtrlResult::Display(void* pData, int nIdx, UINT uID, BOOL bIsSingal)
{
	if( pData == NULL )
		return -1;

	ClearScreen();

	LRESULT lRet;
	CExchCalcMain* pExch = (CExchCalcMain*)pData;
	TCaclOneStockResult* pResult = NULL;
	TCaclResult* pCaclRet = pExch->GetOneResult(uID);
	if( pCaclRet == NULL )
		return -1;
	
	if( nIdx < 0 || nIdx >= pCaclRet->m_ayAllStock.GetSize() )
	{
		pResult = &pCaclRet->m_sTotal;
	}	
	else
	{
		pResult = pCaclRet->m_ayAllStock.GetAt(nIdx);
	}

	if( bIsSingal )
	{
		lRet = DisplaySingal(pResult, pExch);
	}
	else
	{
		lRet = DisplayMulti(pResult, pExch);
	}

	LineScroll(0);

	return lRet;
}

LRESULT CRichEditCtrlResult::DisplaySingal(void* pData, void* pExchData)
{
	if( pData == NULL || pExchData == NULL )
		return -1;

	TExchSys* pExch = (TExchSys*)pExchData;
	TCaclOneStockResult* pResult = (TCaclOneStockResult*)pData;
	CString strOut;
	CString strTemp;
	CString str1, str2, str3, str4;

	AddItemLN(_T("------------------------------ϵͳ����ժҪ-----------------------------------"), HS_Rich_Text_Color_Red);

	str1.Format("%s:", _T("���Թ�Ʊ"));
	char* cTest = (char*)&pResult->m_lStockNum;
	if( *(cTest+sizeof(long)) != 0 )
	{
		str2 = CString(pResult->m_sCode.m_cCode, sizeof(pResult->m_sCode.m_cCode));
	}
	else
	{
		str2.Format(_T("��%d֧"), pResult->m_lStockNum);
	}
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format("%s:", _T("��ر���"));
	str2.Format("%.2f%%", pResult->m_dYearReturnPercent);
	str3.Format("%s:", _T("������״���"));
	str4.Format(_T("%.2f��"), pResult->m_lYearDealNum);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format("%s:", _T("ʤ��"));
	str2.Format("%.2f%%", pResult->m_dWinPercent);
	str3.Format("%s:", _T("�ɹ���"));
	str4.Format("%.2f%%", pResult->m_dSucessPercent);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format("%s:", _T("ƽ������"));
	str2.Format(_T("%.2fԪ"), pResult->m_dNetProfit/(pResult->m_lDealNum == 0 ? 1 : pResult->m_lDealNum));
	str3.Format("%s:",_T("����ź�����"));
	double dTemp1 = pResult->m_lEndDate/10000 - pResult->m_lBeginDate/10000;
	if( dTemp1 == 0 )
		dTemp1 = 1;
	str4.Format(_T("%.2f��"), pResult->m_dSignalNum / dTemp1);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format("%s:", _T("��󵥴�ӯ��"));
	str2.Format(_T("%.2fԪ"), pResult->m_dMaxSingleProfit);
	str3.Format("%s:", _T("��󵥴ο���"));
	str4.Format(_T("%.2fԪ"), pResult->m_dMaxSingleDificit);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);
	
	str1.Format("%s:", _T("���״���"));
	str2.Format(_T("%d��"), pResult->m_lDealNum);
	str3.Format("%s:", _T("ӯ�����״���"));
	str4.Format(_T("%d��"), pResult->m_lProfitDealNum);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue, FALSE);

	str1.Format(_T("  (ռ%.2f%%)"), pResult->m_lProfitDealNum*100.0/(pResult->m_lDealNum == 0 ? 1 : pResult->m_lDealNum));
	AddItemLN(strOut, HS_Rich_Text_Color_Red);

	return 1;
}

LRESULT CRichEditCtrlResult::DisplayMulti(void* pData, void* pExchData)
{
	if( pData == NULL || pExchData == NULL )
		return -1;

	TExchSys* pExch = (TExchSys*)pExchData;
	TCaclOneStockResult* pResult = (TCaclOneStockResult*)pData;
	CString str1, str2, str3, str4;
	CString strTemp;

	AddItemLN(_T("------------------------------ϵͳ������ϸ���-----------------------------------"), HS_Rich_Text_Color_Red);

	str1.Format("%s:", _T("���Թ�Ʊ"));
	char* cTest = (char*)&pResult->m_lStockNum;
	if( *(cTest+sizeof(long)) != 0 )
	{
		str2 = CString(pResult->m_sCode.m_cCode, sizeof(pResult->m_sCode.m_cCode));
	}
	else
	{
		str2.Format(_T("��%d֧"), pResult->m_lStockNum);
	}
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("������:");
	str3 = _T("��������:");
	str2.Format(_T("%.2fԪ"), pResult->m_dNetProfit);
	str4.Format("%.2f%%", pResult->m_dNPPercent);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("��ӯ��:");
	str3 = _T("�ܿ���:");
	str2.Format(_T("%.2fԪ"), pResult->m_dTotalProfit);
	str4.Format(_T("%.2fԪ"), pResult->m_dTotalDificit);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("���״���:");
	str3 = _T("ʤ��:");
	str2.Format(_T("%d��"), pResult->m_lDealNum);
	str4.Format("%.2f%%", pResult->m_dWinPercent);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("������״���:");
	str3 = _T("ӯ��/�����״���:");
	str2.Format(_T("%.2f��"), pResult->m_lYearDealNum);
	str4.Format("%d/%d", pResult->m_lProfitDealNum, pResult->m_lDificitDealNum);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("��󵥴�ӯ��:");
	str3 = _T("��󵥴ο���:");
	str2.Format(_T("%.2fԪ"), pResult->m_dMaxSingleProfit);
	str4.Format(_T("%.2fԪ"), pResult->m_dMaxSingleDificit);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	double dTemp1, dTemp2;
	dTemp1 = pResult->m_dTotalProfit/(pResult->m_lProfitDealNum == 0 ? 1 : pResult->m_lProfitDealNum);
	dTemp2 = pResult->m_dTotalDificit/(pResult->m_lDificitDealNum == 0 ? 1 : pResult->m_lDificitDealNum);
	str1 = _T("ƽ��ӯ��:");
	str3 = _T("ƽ������:");
	str2.Format(_T("%.2fԪ"), dTemp1);
	str4.Format(_T("%.2fԪ"), dTemp2);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("ƽ������:");
	str3 = _T("ƽ��ӯ��/ƽ������:");
	str2.Format(_T("%.2fԪ"), pResult->m_dNetProfit/(pResult->m_lDealNum == 0 ? 1 : pResult->m_lDealNum));
	str4.Format("%.2f", fabs(dTemp1 / (dTemp2 == 0 ? 1 : dTemp2)));
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("�������ӯ������:");
	str3 = _T("��������������:");
	str2.Format(_T("%d��"), pResult->m_lMaxContinuProfitNum);
	str4.Format(_T("%d��"), pResult->m_lMaxContinuDificitNum);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format(_T("����ƽ��������:"));
	str2.Format("%.2f", pResult->m_dDealAverageCyc);
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("ӯ������ƽ������:");
	str3 = _T("������ƽ������:");
	str2.Format("%.2f", pResult->m_dProfiltAverageCyc);
	str4.Format("%.2f", pResult->m_dDiticitAverageCyc);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format(_T("ӯ��ϵ��:"));
	if( pResult->m_dTotalDificit > 0 )
	{
		dTemp1 = pResult->m_dTotalProfit + pResult->m_dTotalDificit;
		if( dTemp1 == 0 )
			dTemp1 = 1;
		str2.Format("%.2f", (pResult->m_dTotalProfit - pResult->m_dTotalDificit) /  dTemp1);
	}
	else
	{
		dTemp1 = pResult->m_dTotalProfit - pResult->m_dTotalDificit;
		if( dTemp1 == 0 )
			dTemp1 = 1;
		str2.Format("%.2f", (pResult->m_dTotalProfit + pResult->m_dTotalDificit) / dTemp1 );
	}
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("��󸡶�ӯ��:");
	str3 = _T("��󸡶�����:");
	str2.Format(_T("%.2fԪ"), pResult->m_dMaxFloatProfit);
	str4.Format(_T("%.2fԪ"), pResult->m_dMaxFloatDificit);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format(_T("��󸡶�ӯ����:"));
	str2.Format("%.2f", pResult->m_dMaxFloatProfit - pResult->m_dMaxFloatDificit);
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("��Ͷ��:");
	str3 = _T("��ЧͶ��:");
	str2.Format(_T("%.2fԪ"), pResult->m_dAllJumpIn);
	str4.Format(_T("%.2fԪ"), pResult->m_dValidJumpIn*(1+EXCH_POUNDAGE));
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("��ر���:");
	str3 = _T("����Ч�ر���:");
	str2.Format("%.2f%%", pResult->m_dYearReturnPercent);
	str4.Format("%.2f%%", pResult->m_dYearValidReturnPercent);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("��������лر�:");
	str3 = _T("���������ر���:");
	str2.Format("%.2f%%", pResult->m_dSimpleBuyHaveReturnPercent);
	str4.Format("%.2f%%", pResult->m_dBuyHaveYearPercent);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("�ܽ��׶�:");
	str3 = _T("���׷�:");
	str2.Format(_T("%.2fԪ"), pResult->m_dAllDealAmount);
	str4.Format(_T("%.2fԪ"), pResult->m_dAllDealCost);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("���׷�/����:");
	str2.Format("%.2f", pResult->m_dAllDealCost/(pResult->m_dNetProfit == 0 ? 1 : pResult->m_dNetProfit));
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("����ʱ��:");
	str3 = _T("����������:");
	str2.Format("%d-%d", pResult->m_lBeginDate, pResult->m_lEndDate);
	str4.Format("%d", pResult->m_lTestCycNum);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("ƽ����λ:");
	str3 = _T("����λ:");
	str2.Format("%.2f%%", pResult->m_dAverageCW);
	str4.Format("%.2f%%", pResult->m_dMaxCW);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("ƽ���ֲ���:");
	str3 = _T("���ֲ���:");
	str2.Format(_T("%.2f��"), pResult->m_dAverageCCL);
	str4.Format(_T("%.2f��"), pResult->m_dMaxCCL);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	AddItemLN(_T("------------------------------�����ź�ͳ��-----------------------------------"), HS_Rich_Text_Color_Red);
	AddItemLN(_T("(ͳ�����������źŵ�����������ǽ��ײ������ʽ𼰲�����ɵ��ź�ɾ������)"), HS_Rich_Text_Color_Black);

	str1 = _T("�ɹ���:");
	str2.Format("%.2f%%", pResult->m_dSucessPercent);
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("�ź�����:");
	str3 = _T("����ź�����:");
	str2.Format("%.0f", pResult->m_dSignalNum);
	dTemp1 = pResult->m_lTestCycNum * 1.0 / 250;
	if( fabs(dTemp1) < 0.001 )
	{
		dTemp1 = 1;
	}
	str4.Format(_T("%.2f��/��"), pResult->m_dSignalNum / dTemp1);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("���ջ�������:");
	str3 = _T("ʮ�ջ�������:");
	str2.Format("%.2f%%", pResult->m_d5DayProfitProbability);
	str4.Format("%.2f%%", pResult->m_d10DayProfitProbability);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("��ʮ�ջ�������:");
	str3 = _T("Ŀ�����ڻ�������:");
	str2.Format("%.2f%%", pResult->m_d20DayProfitProbability);
	str4.Format("%.2f%%", pResult->m_dAimCycProfitProbability);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	SetSel(0, 0);
	//SetScrollPos(SB_VERT, 0);
	return 1;
}

void CRichEditCtrlResult::AddLineDoubleItem(CString str1, CString str2, CString str3, CString str4, COLORREF cl1, COLORREF cl2, BOOL bLN)
{
	CString strOut;

	strOut.Format("%18s", str1);
	AddItem(strOut, cl1);
	strOut.Format("%-18s", str2);
	AddItem(strOut, cl2);
	AddItem("    ", HS_Rich_Text_Color_White);
	strOut.Format("%18s", str3);
	AddItem(strOut, cl1);
	strOut.Format("%-18s", str4);
	if( bLN )
		AddItemLN(strOut, cl2);
	else
		AddItem(strOut, cl2);
}

void CRichEditCtrlResult::AddLineSingleItem(CString str1, CString str2, COLORREF cl1, COLORREF cl2, BOOL bLN)
{
	CString strOut;

	strOut.Format("%18s", str1);
	AddItem(strOut, cl1);
	strOut.Format("%-18s", str2);
	if( bLN )
		AddItemLN(strOut, cl2);
	else
		AddItem(strOut, cl2);
}

CString	CRichEditCtrlResult::GetTipText(CString& strTitle,UINT nFlags, CPoint point,
									  int* nBegin /*= NULL*/,int* nEnd /*= NULL*/)
{
	CClientDC dc(this);
	CSize size = dc.GetTextExtent("A");
	size.cx -= 1;
	CPoint pt;

	int nLine = 0,lChar = 0,lCountChar = 0;
	int nLineCount = GetLineCount();
	int nFirstVisible = GetFirstVisibleLine();
	if(nFirstVisible > 0)
		nLine = nFirstVisible;
	if(nLine > 0)
		lChar = LineIndex(nLine);
	CString str;
	GetWindowText(str);
	nLineCount = str.GetLength();
	for(; lChar < nLineCount; lChar++)
	{
		pt    = GetCharPos( lChar ) ;
		nLine = LineFromChar( lChar ) ;
		if( pt.y < point.y - size.cy)
		{
			nLine++;
			lCountChar = LineIndex(nLine);
			if(lCountChar > lChar)
			{
				lChar = lCountChar;
				continue;
			}
		}
		
		if( pt.y > point.y )
		{
			return "";
		}
		else if( point.x >= pt.x && point.x <= pt.x+ size.cx &&
				 point.y >= pt.y && point.y <= pt.y+ size.cy)
		{			
			CString strText;
			
			if( nBegin != NULL )
				*nBegin = lChar;

			nLineCount = LineFromChar(lChar);
			int nIndex = LineIndex(nLineCount);
			nIndex = abs(lChar - nIndex);
			if(nIndex < 0)
				return "";

			int nLineLength = 1024;
			nLineCount = GetLine(nLineCount, strText.GetBuffer(nLineLength),nLineLength);
			strText.ReleaseBuffer();
			if(nLineCount <= 0)
				return "";
			
			CString strRet;
			strTitle.Format("%s",FindWord(strText,nIndex,strText.GetLength()));
			if(strTitle.IsEmpty())
				return "";

			if(nFlags & MK_RBUTTON)
			{
				if( nEnd != NULL )
					*nEnd = lChar + strTitle.GetLength();
				return strTitle;
			}
			else
			{
				strRet.Format("%s",GetKeyHelp(strTitle));
			}

			if(strRet.IsEmpty())
				return "";
			return strRet;
		}
	}

	return "";
}

CString	CRichEditCtrlResult::GetKeyHelp(CString strKey)
{
	CString strRet;
	
	char szPath[_MAX_PATH];
	sprintf(szPath, "%s%s",  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) , EXCH_TIP_SETTING);
/*	CWriteReadStandardConfig::ReadConfigString(_T_N("����"), strKey.GetBuffer(), strRet, szPath); */
	return strRet;
}

void CRichEditCtrlResult::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if ( !nFlags  )
	{
		CString strTitle;
		CString strText = GetTipText(strTitle,nFlags,point);
		if(!strText.IsEmpty())
		{
		/*	m_pDatatip.Show(strTitle,strText);*/
		}
	}
	CRichEditCtrlEx::OnMouseMove(nFlags, point);
}

void CRichEditCtrlResult::InitialTip()
{
// 	if(m_pDatatip != NULL)
// 		return;
// 
// 	m_pDatatip = new CHSDataTip();
// 	m_pDatatip->Create(this);
// 	m_pDatatip->SetDelay(50);
}

CString	CRichEditCtrlResult::FindWord(CString strText,int& nStart,int nCount)
{
	if( strText.IsEmpty() || nStart > nCount )
		return "";

	CString strLeft  = strText.Left(nStart);
	CString strRight = strText.Mid(nStart);

	strLeft  = CutWord(strLeft,TRUE);
	nStart -= strLeft.GetLength();
	strRight.TrimLeft();
	strRight = CutWord(strRight,FALSE);

	return strLeft+strRight;
}

CString	CRichEditCtrlResult::CutWord(CString strText,BOOL bLeft)
{
	int nBegin,nEnd;
	int nPos;
	CString strRet;
	if(bLeft)
	{
		nEnd   = strText.GetLength();
		nBegin = nEnd - 1;
		nEnd = 0;
		while ( (nBegin >= 0) && (IsChinese(strText[nBegin],strText,nPos,TRUE) ||
				  (isalnum(strText[nBegin])&&strText[nBegin]!=':' && strText[nBegin]!=' ')) )
		{
			nBegin--;
			nEnd++;
		}
		strRet = strText.Right(nEnd);
	}
	else
	{
		nBegin   = 0;
		nEnd = strText.GetLength();
		while ( (nBegin < nEnd) && (strText[nBegin] == '/' || IsChinese(strText[nBegin],strText,nPos,TRUE) ||
				  (isalnum(strText[nBegin]) && strText[nBegin]!=':' && strText[nBegin]!=' ')) )
		{
			nBegin++;
		}
		if( nBegin < 0 ) nBegin = 0;
		strRet = strText.Mid(0,nBegin);
	}
	
	return strRet;
}
int CRichEditCtrlResult::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditCtrlEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	UINT uOptions = GetOptions();
	uOptions &= ~ECO_AUTOHSCROLL;
	SetOptions(ECOOP_SET, uOptions);

	return 0;
}
