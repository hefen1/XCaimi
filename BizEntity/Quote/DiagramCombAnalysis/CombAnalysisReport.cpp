#include "StdAfx.h"
#include "CombAnalysisReport.h"

CCombAnalysisReport::CCombAnalysisReport(void)
{
}

CCombAnalysisReport::~CCombAnalysisReport(void)
{
}

BOOL CCombAnalysisReport::Create(IDrawBaseParam *pParam)
{
	//表格基本属性
	SetStyle(TABLE_STYLE_SCREEN | TABLE_ROW_SELECT);
	SetRowHeight(COMBANALYSISREPOR_ROW_HEIGHT);
	//垂直滚动条
	CreateVerScroll(HSSB_VERT | HSSB_RIGHT);
	m_pVerScrollBar->SetSize(20);
	//水平滚动条
	CreateHorScroll(HSSB_HORZ | HSSB_DOWN );
	m_pHorScrollBar->SetSize(20);
	InitReport();

	return TRUE;
}

void CCombAnalysisReport::InitReport()
{
	SetRowCount(10);
	ColInfoStruct sInfo;
	sInfo.nWidth = 100;
	sInfo.nID = 0;
	for(int i = 0; i < 7; i ++)
	{
		CString str;
		str.Format("column_%d",i+1);
		InsertCol(GetColCount(),FALSE,str,TEXT_COLOR_TIME,TEXT_FONT_FIXED);
		SetColInfo(i, &sInfo);
	}
	SetFixRowCount(1);
	SetFixColCount(1);
}