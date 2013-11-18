#include "stdafx.h"
#include "QuoteReport.h"
#include "QuoteReportDlg.h"
#include "..\QuoteComm\QuoteBaseDefine.h"
#include "..\QuoteComm\pageid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CQuoteReportDlg::OnQuoteReportMsg(UINT nMsgID)
{
	PageViewInfo pviInfo;

	switch(nMsgID)
	{
	case IDM_hexinSHIndex: // 上海指数
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | KIND_INDEX);
		break;		
	case IDM_hexinSHStockA:     // 上证A股报价分析   
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | KIND_STOCKA);
		break;		
	case IDM_hexinSHStockB:    // 上证B股报价分析  
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | KIND_STOCKB);
		break;	
	case IDM_hexinSHBond: // 上海债券报价分析  
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | SC_StockZ);
		break; 
	case IDM_hexinSHFund:  // 上海基金
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | SC_StockJ);
		break;
	case IDM_hexinSHOther: // 上海其他
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | KIND_BOND);
		break;
	case IDM_hexinSZIndex:  // 深圳指数
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | KIND_INDEX);
		break;
	case IDM_hexinSZStockA: // 深证A股报价分析
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | KIND_STOCKA);
		break; 		
	case IDM_hexinSZStockB: // 深证B股报价分析   
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | KIND_STOCKB);
		break; 		 	  	 		 	  	 
	case IDM_hexinSZBond:  // 深圳债券报价分析  
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | SC_StockZ);
		break;
	case IDM_hexinSZFund: // 深圳基金
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | SC_StockJ);
		break;
	case IDM_hexinSZOther: // 深圳其他
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | KIND_BOND);
		break;
	case IDM_hexinCYIndex:  // 创业指数
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break;
	case IDM_hexinCYStockA: // 创业A股报价分析
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break; 		
	case IDM_hexinCYStockB: // 创业B股报价分析   
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break; 		 	  	 		 	  	 
	case IDM_hexinCYBond:  // 创业债券报价分析  
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break;
	case IDM_hexinCYFund: // 创业基金
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break;
	case IDM_hexinCYOther: // 创业其他
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break;
	case IDM_hexinSpacialSHIndex: // 
		break;
	case IDM_hexinSpacialSHStockA: // //上证A股涨跌幅排名
		pviInfo.AddMarketType(SH_Bourse | KIND_STOCKA);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;
	case IDM_hexinSpacialSHStockB:  // //上证B股涨跌幅排名
		pviInfo.AddMarketType(SH_Bourse | KIND_STOCKB);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;
	case IDM_hexinSpacialSHBond:   //上证债券涨跌幅排名
		pviInfo.AddMarketType(SH_Bourse | KIND_BOND);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;
	case IDM_hexinSpacialSHFund:
		break;
	case IDM_hexinSpacialSHOther:
		break;
		//深证A股涨跌幅排名
	case IDM_hexinSpacialSZStockA:
		pviInfo.AddMarketType(SZ_Bourse | KIND_STOCKA);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;
		//深证B股涨跌幅排名
	case IDM_hexinSpacialSZStockB:
		pviInfo.AddMarketType(SZ_Bourse | KIND_STOCKB);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;   
		//深证债券涨跌幅排名
	case IDM_hexinSpacialSZBond:
		pviInfo.AddMarketType(SZ_Bourse | KIND_BOND);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;	
	case IDM_PT_CW_CWTS:		//财务-财务图示
		return;
	case IDM_PT_CW_JBZL:		//财务-基本资料
		/*{
		AfxMessageBox("ok!");
		}*/
		return;
	case IDM_PT_CW_GBQX:		//财务-股本权息
	case IDM_PT_CW_CWSJ:		//财务-财务数据
	case IDM_PT_CW_CWZB:		//财务-财务指标
	case IDM_PT_CW_JJZB:		//财务-基金周报
		{
			pviInfo.m_nInPageTag = (nMsgID == IDM_PT_CW_GBQX)?PT_CW_GBQX:
				(nMsgID == IDM_PT_CW_CWSJ)?PT_CW_CWSJ:
				(nMsgID == IDM_PT_CW_CWZB)?PT_CW_CWZB:
				(nMsgID == IDM_PT_CW_JJZB)?PT_CW_JJZB:0;

		pviInfo.m_nInPageTag |= PT_LIST;

		pviInfo.m_lPageType  = PriceAnalisys2;
		pviInfo.AddMarketType(SH_Bourse | KIND_STOCKA);
		}
		break;
	case IDM_PST_PHASESORT://阶段排序结果
		{
			return; //20081018 YJT 屏蔽成交明细
			//pviInfo.m_lPageType = StockMaiMaiChart;
			//if( IsTableValid() && m_pDispTable)
			//{
			//	const StockUserInfo* pStockInfo = m_pDispTable->GetStock();
			//	if( pStockInfo )
			//	{
			//		pviInfo.m_ciCodeInfo = pStockInfo->m_ciStockCode;
			//	}
			//}
			//AddTable(&pviInfo,IDR_DETAIL_MENU);
			//return;
		}
		break;
	default:
		return;
	}
}