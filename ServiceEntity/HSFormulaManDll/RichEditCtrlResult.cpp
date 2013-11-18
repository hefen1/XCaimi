#include "stdafx.h"
#include "RichEditCtrlResult.h"
#include "ExchCalcMain.h"

#define EXCH_TIP_SETTING   "EXCHTIP.INI"

extern CString GetPeriodStr(WORD dPeriod,int* nPos);
// 是否是汉字
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
	// 提示信息
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
		strTech += _T(" 公式定义的买入条件");
		return strTech;
	}
	strTech += _T("指标线:");
	strTech += pExch->m_sTExchBuy.m_strTechLeft;

	switch( pExch->m_sTExchBuy.m_SubData.m_cType )
	{
	case FormulaCompareSubData::Condition:
	case FormulaCompareSubData::Exchange:
	case FormulaCompareSubData::MoreKLine:
		strTemp += _T("公式中定义的买入条件");
		return strTemp;
		break;
	case FormulaCompareSubData::ShangChuan:	// 上穿
		strTemp += _T("上穿");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		break;
	case FormulaCompareSubData::XiaoPo:		// 下破
		strTemp += _T("下破");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		break;
	case FormulaCompareSubData::DaYu: // >
		strTemp += _T("大于");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		break;
	case FormulaCompareSubData::XiaoYu: // <		
		strTemp += _T("小于");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		break;
	case FormulaCompareSubData::DengYu: // =
		strTemp += _T("等于");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		break;
	case FormulaCompareSubData::BiaoChi: // 保持
		strTemp += _T("保持");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		strTemp += _T("周期后下破");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData2;
		break;
	case FormulaCompareSubData::WeiChi:// 维持
		strTemp += _T("维持");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData1;
		strTemp += _T("周期后上穿");
		strTemp += pExch->m_sTExchBuy.m_SubData.m_dData2;
		break;
	case FormulaCompareSubData::JieYu:// 介于
		strTemp += _T("介于");
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
		return CString(_T("公式中定义的卖出条件"));
	}

	return CString(_T("无卖出条件"));
}

LRESULT CRichEditCtrlResult::Display(LPCSTR lpMsg /*= NULL*/)
{
	ClearScreen();

	AddItemLN(_T("------------------恒生电子股份有限公司友情提示----------------------"), HS_Rich_Text_Color_Blue);
	AddTab();
	AddItemLN(_T("本测试结果仅供参考，风险自担！"), HS_Rich_Text_Color_Red);
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

	AddItemLN(_T("                           系统测试设定"), HS_Rich_Text_Color_Black);

	AddItem(_T("测试方法："), HS_Rich_Text_Color_Black);
	strTech = pExch->m_sExchExp.GetExpTypeName();
	strTech += "-";
	strTech += pExch->m_sExchExp.m_pMainTech->GetName();

	strTech += pExch->GetParamStr(bSignal, uID);

	AddItemLN(strTech, HS_Rich_Text_Color_Blue);

	AddItem(_T("测试时间："), HS_Rich_Text_Color_Black);
	strTemp.Format("%d-%d\t\t", pExch->m_sTExchBuy.m_lBeginDate, pExch->m_sTExchBuy.m_lEndDate);
	strTemp += (pExch->m_sTExchBuy.m_bNoCountIn == FALSE) ? _T("计强制平仓") : _T("不计强制平仓");
	AddItemLN(strTemp, HS_Rich_Text_Color_Blue);

	AddItem(_T("测试周期："), HS_Rich_Text_Color_Black);
	strTemp.Format("%s", GetPeriodStr(pExch->m_sExchExp.m_lCyc, NULL));
	AddItemLN(strTemp, HS_Rich_Text_Color_Blue);

	AddItem(_T("测试股票："), HS_Rich_Text_Color_Black);
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
		strTemp.Format(_T("共%d支\t\t"), lCount);
	}
	AddItem(strTemp, HS_Rich_Text_Color_Blue);
	AddItem(_T("初始投入："), HS_Rich_Text_Color_Black);
	strTemp.Format("%.2f元", pExch->m_sTExchModel.m_lAllInvestCount);
	AddItemLN(strTemp, HS_Rich_Text_Color_Blue);

	AddItem(_T("买入条件："), HS_Rich_Text_Color_Black);
	strTemp = GetBuyCon(pExch);
	AddItemLN(strTemp, HS_Rich_Text_Color_Blue);

	AddTab();
	AddItem(_T("  当条件满足时："), HS_Rich_Text_Color_Black);
	strTemp = _T("在本周期按照收盘价");
	switch( pExch->m_sTExchBuy.m_nBuyMethod )
	{
	default:
	case BuyMethod_All_Buy:// 全部买入
		{
			strTemp += _T("使用全部资金买入");
		}
		break;
	case BuyMethod_Portion_Buy:// 部分买入
		{
			str.Format(_T("使用%.f%%资金买入"), pExch->m_sTExchBuy.m_fOneBuyCount);
			strTemp += str;
		}
		break;
	case BuyMethod_Fixation_Money:// 固定资金买入
		{
			str.Format(_T("投入%.2f元购买"), pExch->m_sTExchBuy.m_fOneBuyCount);
			strTemp += str;
		}
		break;
	case BuyMethod_Fixation_NUM:// 固定股票数买入
		{
			str.Format(_T("买入%.f股"), pExch->m_sTExchBuy.m_fOneBuyCount);
			strTemp += str;
		}
		break;
	}
	AddItemLN(strTemp, HS_Rich_Text_Color_Blue);

	AddTab();
	AddItem(_T("出现连续信号时："), HS_Rich_Text_Color_Black);
	switch( pExch->m_sTExchBuy.m_nFollowBuyMethod )
	{
	case FollowBuyMethod_NO:// 不再买入	
		str = _T("不再买入");
		break;
	case FollowBuyMethod_DJ:// 递减买入
		str.Format(_T("较前一信号减少%.2f%%买入"),pExch->m_sTExchBuy.m_fAddOrSubCount);
		break;
	case FollowBuyMethod_DL:// 等量买入
		str = _T("与上一信号等同买入");
		break;
	case FollowBuyMethod_DZ:// 递增买入
		str.Format(_T("较前一信号增加%.2f%%买入"),pExch->m_sTExchBuy.m_fAddOrSubCount);
		break;
	}
	if( pExch->m_sTExchBuy.m_nFollowBuyMethod != FollowBuyMethod_NO && pExch->m_sTExchBuy.m_bSkip == TRUE )
	{
		str += ",";
		strTemp.Format(_T("忽略%d周期内的连续信号"), pExch->m_sTExchBuy.m_nInPeriod);
		str += strTemp;
	}
	AddItemLN(str, HS_Rich_Text_Color_Blue);
	
	AddItem(_T("卖出条件："), HS_Rich_Text_Color_Black);
	str = GetSellCom(pExch);
	AddItemLN(str, HS_Rich_Text_Color_Blue);

	AddItemLN(_T("平仓条件：(按照收盘价平仓)"), HS_Rich_Text_Color_Black);
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

	// 采用全市场模型测试
	if( pExch->m_sTExchModel.m_bSingle == FALSE )
	{
		AddNewLine();
		AddItemLN(_T("采用全市场模型测试"), HS_Rich_Text_Color_Red);

		AddTab();
		AddItem(_T("最大持有股票数："), HS_Rich_Text_Color_Black);
		str.Format(_T("%d支"), pExch->m_sTExchModel.m_nHoleStockCount );
		AddItemLN(str, HS_Rich_Text_Color_Blue);

		AddTab();
		if( pExch->m_sTExchModel.m_bBuyAll == FALSE )
		{
			AddItem(_T("最大单次买入股票数："), HS_Rich_Text_Color_Black);
			str.Format(_T("%d支"), pExch->m_sTExchModel.m_nOneBuyCount );
			AddItemLN(str, HS_Rich_Text_Color_Blue);
		}
		else
		{
			AddItem(_T("最大单次买入股票数："), HS_Rich_Text_Color_Black);
			AddItemLN(_T("全部买入"), HS_Rich_Text_Color_Blue);
		}

		//AddTab();
		//AddItem("同时满足条件股票多于", HS_Rich_Text_Color_Black);
		//str.Format("%d", pExch->m_sTExchModel.m_nOneBuyCount);
		//AddItem(str, HS_Rich_Text_Color_Blue);
		//AddItem("只时，按照", HS_Rich_Text_Color_Black);
		//AddItem(pExch->m_sTExchModel.m_sSortReason.m_strExp, HS_Rich_Text_Color_Blue);
		//if( pExch->m_sTExchModel.m_bSToB )
		//{
		//	AddItem("由小到大", HS_Rich_Text_Color_Blue);
		//}
		//else
		//{
		//	AddItem("由大到小", HS_Rich_Text_Color_Blue);
		//}
		//AddItemLN("排序选择", HS_Rich_Text_Color_Black);
	}

	// 系统将对以下参数进行优化:
	if( pOptimize == NULL )
	{
		pOptimize = &pExch->m_sTExchOptimize;
	}
	if( pOptimize->GetValidItemCount() > 0 )
	{
		AddNewLine();
		AddItem(_T("系统将对以下参数进行优化:( 优化次数"), HS_Rich_Text_Color_Red);
		str.Format("%d", pOptimize->GetCaclUionCount());
		AddItem(str, HS_Rich_Text_Color_Blue);
		AddItemLN(")", HS_Rich_Text_Color_Red);

		for( int i = 0; i < pOptimize->GetValidItemCount(); i++ )
		{
			AddTab();
			TExchOptimizeItem* pItem = pOptimize->GetValidItem(i);
			str.Format(_T(" %s：范围( %.2f--%.2f )，步长：%.2f"), pItem->GetName(), pItem->m_dMin, pItem->m_dMax, pItem->m_dAdd);
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
		str.Format(_T("买入%.f周期平仓"), pExch->m_sTExchSell.m_nMaidPeriod);
		ayStr.Add(str);
	}

	if( pExch->m_sTExchSell.m_bWinPeriod )
	{
		str.Format(_T("利润达到%.f%%止赢"), pExch->m_sTExchSell.m_nWinPeriod);
		ayStr.Add(str);
	}

	if( pExch->m_sTExchSell.m_bLosePeriod )
	{
		str.Format(_T("亏损达到%.f%%止损"), pExch->m_sTExchSell.m_nLosePeriod);
		ayStr.Add(str);
	}

	if( pExch->m_sTExchSell.m_bFallPeriod )
	{
		str.Format(_T("%d周期内回落亏损达到%d%%平仓"), pExch->m_sTExchSell.m_nFallPeriodCount, pExch->m_sTExchSell.m_nFallPeriod);
		ayStr.Add(str);        
	}

	if( pExch->m_sTExchSell.m_bUpPeriod )
	{
		str.Format(_T("%d周期内横盘，振幅低于%d%%平仓"), pExch->m_sTExchSell.m_nUpPeriodCount, pExch->m_sTExchSell.m_nUpPeriod);
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

	AddItemLN(_T("------------------------------系统测试摘要-----------------------------------"), HS_Rich_Text_Color_Red);

	str1.Format("%s:", _T("测试股票"));
	char* cTest = (char*)&pResult->m_lStockNum;
	if( *(cTest+sizeof(long)) != 0 )
	{
		str2 = CString(pResult->m_sCode.m_cCode, sizeof(pResult->m_sCode.m_cCode));
	}
	else
	{
		str2.Format(_T("共%d支"), pResult->m_lStockNum);
	}
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format("%s:", _T("年回报率"));
	str2.Format("%.2f%%", pResult->m_dYearReturnPercent);
	str3.Format("%s:", _T("年均交易次数"));
	str4.Format(_T("%.2f次"), pResult->m_lYearDealNum);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format("%s:", _T("胜率"));
	str2.Format("%.2f%%", pResult->m_dWinPercent);
	str3.Format("%s:", _T("成功率"));
	str4.Format("%.2f%%", pResult->m_dSucessPercent);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format("%s:", _T("平均利润"));
	str2.Format(_T("%.2f元"), pResult->m_dNetProfit/(pResult->m_lDealNum == 0 ? 1 : pResult->m_lDealNum));
	str3.Format("%s:",_T("年均信号数量"));
	double dTemp1 = pResult->m_lEndDate/10000 - pResult->m_lBeginDate/10000;
	if( dTemp1 == 0 )
		dTemp1 = 1;
	str4.Format(_T("%.2f次"), pResult->m_dSignalNum / dTemp1);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format("%s:", _T("最大单次盈利"));
	str2.Format(_T("%.2f元"), pResult->m_dMaxSingleProfit);
	str3.Format("%s:", _T("最大单次亏损"));
	str4.Format(_T("%.2f元"), pResult->m_dMaxSingleDificit);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);
	
	str1.Format("%s:", _T("交易次数"));
	str2.Format(_T("%d次"), pResult->m_lDealNum);
	str3.Format("%s:", _T("盈利交易次数"));
	str4.Format(_T("%d次"), pResult->m_lProfitDealNum);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue, FALSE);

	str1.Format(_T("  (占%.2f%%)"), pResult->m_lProfitDealNum*100.0/(pResult->m_lDealNum == 0 ? 1 : pResult->m_lDealNum));
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

	AddItemLN(_T("------------------------------系统测试详细结果-----------------------------------"), HS_Rich_Text_Color_Red);

	str1.Format("%s:", _T("测试股票"));
	char* cTest = (char*)&pResult->m_lStockNum;
	if( *(cTest+sizeof(long)) != 0 )
	{
		str2 = CString(pResult->m_sCode.m_cCode, sizeof(pResult->m_sCode.m_cCode));
	}
	else
	{
		str2.Format(_T("共%d支"), pResult->m_lStockNum);
	}
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("净利润:");
	str3 = _T("净利润率:");
	str2.Format(_T("%.2f元"), pResult->m_dNetProfit);
	str4.Format("%.2f%%", pResult->m_dNPPercent);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("总盈利:");
	str3 = _T("总亏损:");
	str2.Format(_T("%.2f元"), pResult->m_dTotalProfit);
	str4.Format(_T("%.2f元"), pResult->m_dTotalDificit);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("交易次数:");
	str3 = _T("胜率:");
	str2.Format(_T("%d次"), pResult->m_lDealNum);
	str4.Format("%.2f%%", pResult->m_dWinPercent);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("年均交易次数:");
	str3 = _T("盈利/亏损交易次数:");
	str2.Format(_T("%.2f次"), pResult->m_lYearDealNum);
	str4.Format("%d/%d", pResult->m_lProfitDealNum, pResult->m_lDificitDealNum);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("最大单次盈利:");
	str3 = _T("最大单次亏损:");
	str2.Format(_T("%.2f元"), pResult->m_dMaxSingleProfit);
	str4.Format(_T("%.2f元"), pResult->m_dMaxSingleDificit);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	double dTemp1, dTemp2;
	dTemp1 = pResult->m_dTotalProfit/(pResult->m_lProfitDealNum == 0 ? 1 : pResult->m_lProfitDealNum);
	dTemp2 = pResult->m_dTotalDificit/(pResult->m_lDificitDealNum == 0 ? 1 : pResult->m_lDificitDealNum);
	str1 = _T("平均盈利:");
	str3 = _T("平均亏损:");
	str2.Format(_T("%.2f元"), dTemp1);
	str4.Format(_T("%.2f元"), dTemp2);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("平均利润:");
	str3 = _T("平均盈利/平均亏损:");
	str2.Format(_T("%.2f元"), pResult->m_dNetProfit/(pResult->m_lDealNum == 0 ? 1 : pResult->m_lDealNum));
	str4.Format("%.2f", fabs(dTemp1 / (dTemp2 == 0 ? 1 : dTemp2)));
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("最大连续盈利次数:");
	str3 = _T("最大连续亏损次数:");
	str2.Format(_T("%d次"), pResult->m_lMaxContinuProfitNum);
	str4.Format(_T("%d次"), pResult->m_lMaxContinuDificitNum);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format(_T("交易平均周期数:"));
	str2.Format("%.2f", pResult->m_dDealAverageCyc);
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("盈利交易平均周期:");
	str3 = _T("亏损交易平均周期:");
	str2.Format("%.2f", pResult->m_dProfiltAverageCyc);
	str4.Format("%.2f", pResult->m_dDiticitAverageCyc);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format(_T("盈利系数:"));
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
	str1 = _T("最大浮动盈利:");
	str3 = _T("最大浮动亏损:");
	str2.Format(_T("%.2f元"), pResult->m_dMaxFloatProfit);
	str4.Format(_T("%.2f元"), pResult->m_dMaxFloatDificit);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1.Format(_T("最大浮动盈亏差:"));
	str2.Format("%.2f", pResult->m_dMaxFloatProfit - pResult->m_dMaxFloatDificit);
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("总投入:");
	str3 = _T("有效投入:");
	str2.Format(_T("%.2f元"), pResult->m_dAllJumpIn);
	str4.Format(_T("%.2f元"), pResult->m_dValidJumpIn*(1+EXCH_POUNDAGE));
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("年回报率:");
	str3 = _T("年有效回报率:");
	str2.Format("%.2f%%", pResult->m_dYearReturnPercent);
	str4.Format("%.2f%%", pResult->m_dYearValidReturnPercent);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("简单买入持有回报:");
	str3 = _T("买入持有年回报率:");
	str2.Format("%.2f%%", pResult->m_dSimpleBuyHaveReturnPercent);
	str4.Format("%.2f%%", pResult->m_dBuyHaveYearPercent);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("总交易额:");
	str3 = _T("交易费:");
	str2.Format(_T("%.2f元"), pResult->m_dAllDealAmount);
	str4.Format(_T("%.2f元"), pResult->m_dAllDealCost);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("交易费/利润:");
	str2.Format("%.2f", pResult->m_dAllDealCost/(pResult->m_dNetProfit == 0 ? 1 : pResult->m_dNetProfit));
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	str1 = _T("测试时间:");
	str3 = _T("测试周期数:");
	str2.Format("%d-%d", pResult->m_lBeginDate, pResult->m_lEndDate);
	str4.Format("%d", pResult->m_lTestCycNum);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("平均仓位:");
	str3 = _T("最大仓位:");
	str2.Format("%.2f%%", pResult->m_dAverageCW);
	str4.Format("%.2f%%", pResult->m_dMaxCW);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("平均持仓量:");
	str3 = _T("最大持仓量:");
	str2.Format(_T("%.2f股"), pResult->m_dAverageCCL);
	str4.Format(_T("%.2f股"), pResult->m_dMaxCCL);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	AddNewLine();
	AddItemLN(_T("------------------------------买入信号统计-----------------------------------"), HS_Rich_Text_Color_Red);
	AddItemLN(_T("(统计所有买入信号点情况，不考虑交易测试中资金及策略造成的信号删除问题)"), HS_Rich_Text_Color_Black);

	str1 = _T("成功率:");
	str2.Format("%.2f%%", pResult->m_dSucessPercent);
	AddLineSingleItem(str1, str2, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("信号数量:");
	str3 = _T("年均信号数量:");
	str2.Format("%.0f", pResult->m_dSignalNum);
	dTemp1 = pResult->m_lTestCycNum * 1.0 / 250;
	if( fabs(dTemp1) < 0.001 )
	{
		dTemp1 = 1;
	}
	str4.Format(_T("%.2f次/年"), pResult->m_dSignalNum / dTemp1);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("五日获利概率:");
	str3 = _T("十日获利概率:");
	str2.Format("%.2f%%", pResult->m_d5DayProfitProbability);
	str4.Format("%.2f%%", pResult->m_d10DayProfitProbability);
	AddLineDoubleItem(str1, str2, str3, str4, HS_Rich_Text_Color_Black, HS_Rich_Text_Color_Blue);

	str1 = _T("二十日获利概率:");
	str3 = _T("目标周期获利概率:");
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
/*	CWriteReadStandardConfig::ReadConfigString(_T_N("设置"), strKey.GetBuffer(), strRet, szPath); */
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
