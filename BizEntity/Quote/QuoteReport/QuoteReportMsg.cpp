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
	case IDM_hexinSHIndex: // �Ϻ�ָ��
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | KIND_INDEX);
		break;		
	case IDM_hexinSHStockA:     // ��֤A�ɱ��۷���   
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | KIND_STOCKA);
		break;		
	case IDM_hexinSHStockB:    // ��֤B�ɱ��۷���  
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | KIND_STOCKB);
		break;	
	case IDM_hexinSHBond: // �Ϻ�ծȯ���۷���  
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | SC_StockZ);
		break; 
	case IDM_hexinSHFund:  // �Ϻ�����
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | SC_StockJ);
		break;
	case IDM_hexinSHOther: // �Ϻ�����
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SH_Bourse | KIND_BOND);
		break;
	case IDM_hexinSZIndex:  // ����ָ��
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | KIND_INDEX);
		break;
	case IDM_hexinSZStockA: // ��֤A�ɱ��۷���
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | KIND_STOCKA);
		break; 		
	case IDM_hexinSZStockB: // ��֤B�ɱ��۷���   
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | KIND_STOCKB);
		break; 		 	  	 		 	  	 
	case IDM_hexinSZBond:  // ����ծȯ���۷���  
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | SC_StockZ);
		break;
	case IDM_hexinSZFund: // ���ڻ���
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | SC_StockJ);
		break;
	case IDM_hexinSZOther: // ��������
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(SZ_Bourse | KIND_BOND);
		break;
	case IDM_hexinCYIndex:  // ��ҵָ��
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break;
	case IDM_hexinCYStockA: // ��ҵA�ɱ��۷���
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break; 		
	case IDM_hexinCYStockB: // ��ҵB�ɱ��۷���   
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break; 		 	  	 		 	  	 
	case IDM_hexinCYBond:  // ��ҵծȯ���۷���  
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break;
	case IDM_hexinCYFund: // ��ҵ����
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break;
	case IDM_hexinCYOther: // ��ҵ����
		pviInfo.m_lPageType  = PriceAnalisys1;
		pviInfo.m_nInPageTag = PT_LIST | PT_TYPE;
		pviInfo.AddMarketType(-1);
		break;
	case IDM_hexinSpacialSHIndex: // 
		break;
	case IDM_hexinSpacialSHStockA: // //��֤A���ǵ�������
		pviInfo.AddMarketType(SH_Bourse | KIND_STOCKA);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;
	case IDM_hexinSpacialSHStockB:  // //��֤B���ǵ�������
		pviInfo.AddMarketType(SH_Bourse | KIND_STOCKB);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;
	case IDM_hexinSpacialSHBond:   //��֤ծȯ�ǵ�������
		pviInfo.AddMarketType(SH_Bourse | KIND_BOND);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;
	case IDM_hexinSpacialSHFund:
		break;
	case IDM_hexinSpacialSHOther:
		break;
		//��֤A���ǵ�������
	case IDM_hexinSpacialSZStockA:
		pviInfo.AddMarketType(SZ_Bourse | KIND_STOCKA);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;
		//��֤B���ǵ�������
	case IDM_hexinSpacialSZStockB:
		pviInfo.AddMarketType(SZ_Bourse | KIND_STOCKB);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;   
		//��֤ծȯ�ǵ�������
	case IDM_hexinSpacialSZBond:
		pviInfo.AddMarketType(SZ_Bourse | KIND_BOND);
		pviInfo.m_nInPageTag = PT_TYPE | SpecialReport1;
		pviInfo.m_lPageType  = SpecialReport;
		break;	
	case IDM_PT_CW_CWTS:		//����-����ͼʾ
		return;
	case IDM_PT_CW_JBZL:		//����-��������
		/*{
		AfxMessageBox("ok!");
		}*/
		return;
	case IDM_PT_CW_GBQX:		//����-�ɱ�ȨϢ
	case IDM_PT_CW_CWSJ:		//����-��������
	case IDM_PT_CW_CWZB:		//����-����ָ��
	case IDM_PT_CW_JJZB:		//����-�����ܱ�
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
	case IDM_PST_PHASESORT://�׶�������
		{
			return; //20081018 YJT ���γɽ���ϸ
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