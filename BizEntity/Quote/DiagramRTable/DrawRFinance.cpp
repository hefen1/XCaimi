#include "StdAfx.h"
#include "DrawRFinance.h"
#include "hsothercoldefine.h"
#include <complex>
#include "HSCalculate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDrawRFinance::CDrawRFinance(IDataSourceEx* pDataSource)
:CDrawTableBase()
{
	SetStyle(TABLE_STYLE_SCREEN | TABLE_WIDTH_CHANGABLE);
	CreateSelf();
	CreateVerScroll(HSSB_VERT | HSSB_RIGHT);
	m_pDataSource = pDataSource;	
}

CDrawRFinance::~CDrawRFinance(void)
{

}

BOOL CDrawRFinance::CreateSelf()
{
	m_ayTitleInt.RemoveAll();
	m_ayTitleStr.RemoveAll();
	m_ayTitleInt.Add(COLUMN_FINANCE_REPORT_DATE);         //38 报告期
	m_ayTitleStr.Add("报告期");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_TOTAL);        //1 总股本（万股）
	m_ayTitleStr.Add("总股本");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_PASS_A);	      //7 流通A股 
	m_ayTitleStr.Add("流通A股");
	m_ayTitleInt.Add(COLUMN_FINANCE_ASSETS_YIELD); 	      //37 净资收益率
	m_ayTitleStr.Add("净资收益率");
	m_ayTitleInt.Add(COLUMN_FINANCE_MAIN_INCOME);  	      //20 主营收入
	m_ayTitleStr.Add("主营收入");

	m_ayTitleInt.Add(COLUMN_FINANCE_TOTAL_PROFIT);	      //28 利润总额
	m_ayTitleStr.Add("利润总额");
	m_ayTitleInt.Add(COLUMN_FINANCE_PER_UNPAID);   	      //32 每股未分配
	m_ayTitleStr.Add("每股未分配");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_NATIONAL);	  //2 国家股
	m_ayTitleStr.Add("国家股");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_INITIATOR);	  //3 发起人法人股
	m_ayTitleStr.Add("发起人法人股");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_CORPORATION);  //4 法人股 
	m_ayTitleStr.Add("法人股");

	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_B);            //5 B股
	m_ayTitleStr.Add("B股");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_H);			  //6 H股
	m_ayTitleStr.Add("H股");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_EMPLOYEE);     //8 职工股
	m_ayTitleStr.Add("职工股");
	m_ayTitleInt.Add(COLUMN_FINANCE_TOTAL_ASSETS);   	  //10 总资产
	m_ayTitleStr.Add("总资产");
	m_ayTitleInt.Add(COLUMN_FINANCE_CURRENT_ASSETS);	  //11 流动资产  
	m_ayTitleStr.Add("流动资产");

	m_ayTitleInt.Add(COLUMN_FINANCE_CAPITAL_ASSETS);	  //12 固定资产  
	m_ayTitleStr.Add("固定资产");
	m_ayTitleInt.Add(COLUMN_FINANCE_UNBODIED_ASSETS);	  //13 无形资产  
	m_ayTitleStr.Add("无形资产");
	m_ayTitleInt.Add(COLUMN_FINANCE_CURRENT_LIABILITIES); //15 流动负债 
	m_ayTitleStr.Add("流动负债");
	m_ayTitleInt.Add(COLUMN_FINANCE_LONG_LIABILITIES);	  //16 长期负债 
	m_ayTitleStr.Add("长期负债");
	m_ayTitleInt.Add(COLUMN_FINANCE_CAPITAL_ACCFUND);	  //17 资本公积金
	m_ayTitleStr.Add("资本公积金");

	m_ayTitleInt.Add(COLUMN_FINANCE_PERSTOCK_ACCFUND);	  //18 每股公积金
	m_ayTitleStr.Add("每股公积金");
	m_ayTitleInt.Add(COLUMN_FINANCE_PARTNER_RIGHT);	      //19 股东权益  
	m_ayTitleStr.Add("股东权益");
	m_ayTitleInt.Add(COLUMN_FINANCE_TAKING_PROFIT);	      //23 营业利润  
	m_ayTitleStr.Add("营业利润");
	m_ayTitleInt.Add(COLUMN_FINANCE_YIELD);   	          //24 投资收益  
	m_ayTitleStr.Add("投资收益");
	m_ayTitleInt.Add(COLUMN_FINANCE_OTHER_INCOME);	      //26 营业外收支
	m_ayTitleStr.Add("营业外收支");

	m_ayTitleInt.Add(COLUMN_FINANCE_SCOT_PROFIT);	       //29 税后利润
	m_ayTitleStr.Add("税后利润");
	m_ayTitleInt.Add(COLUMN_FINANCE_RETAINED_PROFITS);	   //30 净利润
	m_ayTitleStr.Add("净利润");
	m_ayTitleInt.Add(COLUMN_FINANCE_PER_INCOME);	       //33 每股收益
	m_ayTitleStr.Add("每股收益");
	m_ayTitleInt.Add(COLUMN_FINANCE_PER_ASSETS);		   //34 每股净资产
	m_ayTitleStr.Add("每股净资产");
	m_ayTitleInt.Add(COLUMN_FINANCE_PARTNER_RIGHT_RATIO);  //36 股东权益比
	m_ayTitleStr.Add("股东权益比");

	DWORD lItemStyle(-1);
	DWORD lCellStyle(-1);
	////////////////////////////  第一列  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	for (int i = 0; i < m_ayTitleInt.GetCount(); i++)
	{
		CreateCell(m_ayTitleStr.GetAt(i),  TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, lItemStyle);
	}

	////////////////////////////  第二列  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	for (int i = 0; i < m_ayTitleInt.GetCount(); i++)
	{
		CreateCell("", TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, lItemStyle);
	}

	SetRowCount(m_ayTitleInt.GetCount());
	SetColCount(RFinance_COL);
	SetFixColCount(0);
	SetFixRowCount(0);
	SetRowHeight(RFinance_ROW_HEIGHT);
	
	return TRUE;
}

BOOL CDrawRFinance::GetRequestData( IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce)
{
	if(pStockInfo == NULL)
		return FALSE;

	CArray<CodeInfo> ayCode;
	ayCode.Add(pStockInfo->m_ciStockCode);
	m_ayFinanceData.RemoveAll();
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, m_ayFinanceData);
	if (m_ayFinanceData.GetCount() == ayCode.GetCount())
	{
		OnUpdateFinaceData();
	}
	return TRUE;
}

void CDrawRFinance::OnUpdateFinaceData()
{
	// 数据更新从第二列开始
	int index = m_ayCell.GetCount() / 2;
	char strText[64];
	memset(strText, 0, sizeof(strText));
	for (int i = 0; i < m_ayTitleInt.GetCount(); i++, index++)
	{	
		int nIndex = m_ayTitleInt.GetAt(i);
		CHSCalculate::GetTextByID(nIndex, &m_ayFinanceData.GetAt(0), strText);
		SetCellText(index, 0, strText, 0, 0, 0, 0, FALSE);
	}
	InValidate(&GetTableRect(), FALSE);
}
