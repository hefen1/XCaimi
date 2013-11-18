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
	m_ayTitleInt.Add(COLUMN_FINANCE_REPORT_DATE);         //38 ������
	m_ayTitleStr.Add("������");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_TOTAL);        //1 �ܹɱ�����ɣ�
	m_ayTitleStr.Add("�ܹɱ�");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_PASS_A);	      //7 ��ͨA�� 
	m_ayTitleStr.Add("��ͨA��");
	m_ayTitleInt.Add(COLUMN_FINANCE_ASSETS_YIELD); 	      //37 ����������
	m_ayTitleStr.Add("����������");
	m_ayTitleInt.Add(COLUMN_FINANCE_MAIN_INCOME);  	      //20 ��Ӫ����
	m_ayTitleStr.Add("��Ӫ����");

	m_ayTitleInt.Add(COLUMN_FINANCE_TOTAL_PROFIT);	      //28 �����ܶ�
	m_ayTitleStr.Add("�����ܶ�");
	m_ayTitleInt.Add(COLUMN_FINANCE_PER_UNPAID);   	      //32 ÿ��δ����
	m_ayTitleStr.Add("ÿ��δ����");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_NATIONAL);	  //2 ���ҹ�
	m_ayTitleStr.Add("���ҹ�");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_INITIATOR);	  //3 �����˷��˹�
	m_ayTitleStr.Add("�����˷��˹�");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_CORPORATION);  //4 ���˹� 
	m_ayTitleStr.Add("���˹�");

	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_B);            //5 B��
	m_ayTitleStr.Add("B��");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_H);			  //6 H��
	m_ayTitleStr.Add("H��");
	m_ayTitleInt.Add(COLUMN_CAPITALIZATION_EMPLOYEE);     //8 ְ����
	m_ayTitleStr.Add("ְ����");
	m_ayTitleInt.Add(COLUMN_FINANCE_TOTAL_ASSETS);   	  //10 ���ʲ�
	m_ayTitleStr.Add("���ʲ�");
	m_ayTitleInt.Add(COLUMN_FINANCE_CURRENT_ASSETS);	  //11 �����ʲ�  
	m_ayTitleStr.Add("�����ʲ�");

	m_ayTitleInt.Add(COLUMN_FINANCE_CAPITAL_ASSETS);	  //12 �̶��ʲ�  
	m_ayTitleStr.Add("�̶��ʲ�");
	m_ayTitleInt.Add(COLUMN_FINANCE_UNBODIED_ASSETS);	  //13 �����ʲ�  
	m_ayTitleStr.Add("�����ʲ�");
	m_ayTitleInt.Add(COLUMN_FINANCE_CURRENT_LIABILITIES); //15 ������ծ 
	m_ayTitleStr.Add("������ծ");
	m_ayTitleInt.Add(COLUMN_FINANCE_LONG_LIABILITIES);	  //16 ���ڸ�ծ 
	m_ayTitleStr.Add("���ڸ�ծ");
	m_ayTitleInt.Add(COLUMN_FINANCE_CAPITAL_ACCFUND);	  //17 �ʱ�������
	m_ayTitleStr.Add("�ʱ�������");

	m_ayTitleInt.Add(COLUMN_FINANCE_PERSTOCK_ACCFUND);	  //18 ÿ�ɹ�����
	m_ayTitleStr.Add("ÿ�ɹ�����");
	m_ayTitleInt.Add(COLUMN_FINANCE_PARTNER_RIGHT);	      //19 �ɶ�Ȩ��  
	m_ayTitleStr.Add("�ɶ�Ȩ��");
	m_ayTitleInt.Add(COLUMN_FINANCE_TAKING_PROFIT);	      //23 Ӫҵ����  
	m_ayTitleStr.Add("Ӫҵ����");
	m_ayTitleInt.Add(COLUMN_FINANCE_YIELD);   	          //24 Ͷ������  
	m_ayTitleStr.Add("Ͷ������");
	m_ayTitleInt.Add(COLUMN_FINANCE_OTHER_INCOME);	      //26 Ӫҵ����֧
	m_ayTitleStr.Add("Ӫҵ����֧");

	m_ayTitleInt.Add(COLUMN_FINANCE_SCOT_PROFIT);	       //29 ˰������
	m_ayTitleStr.Add("˰������");
	m_ayTitleInt.Add(COLUMN_FINANCE_RETAINED_PROFITS);	   //30 ������
	m_ayTitleStr.Add("������");
	m_ayTitleInt.Add(COLUMN_FINANCE_PER_INCOME);	       //33 ÿ������
	m_ayTitleStr.Add("ÿ������");
	m_ayTitleInt.Add(COLUMN_FINANCE_PER_ASSETS);		   //34 ÿ�ɾ��ʲ�
	m_ayTitleStr.Add("ÿ�ɾ��ʲ�");
	m_ayTitleInt.Add(COLUMN_FINANCE_PARTNER_RIGHT_RATIO);  //36 �ɶ�Ȩ���
	m_ayTitleStr.Add("�ɶ�Ȩ���");

	DWORD lItemStyle(-1);
	DWORD lCellStyle(-1);
	////////////////////////////  ��һ��  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	for (int i = 0; i < m_ayTitleInt.GetCount(); i++)
	{
		CreateCell(m_ayTitleStr.GetAt(i),  TEXT_COLOR_FIXEDNAME, TEXT_FONT_FINANCE, lItemStyle);
	}

	////////////////////////////  �ڶ���  //////////////////////////////////////
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
	// ���ݸ��´ӵڶ��п�ʼ
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
