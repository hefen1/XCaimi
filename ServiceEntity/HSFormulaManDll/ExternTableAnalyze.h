/*************************************************************
*	Copyright (c)2002, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	ExternTableAnalyze.h
*	文件标识：	用于盘后区间分析数据处理
*	摘	  要：	目前以不用.
*
*	当前版本：	2.0
*	作	  者：	熊钟彬
*	更新日期：	2003年4月18日
*
***************************************************************/

#if !defined(AFX_EXTERNTABLEANALYZE_H__6762347239057439875)
#define AFX_EXTERNTABLEANALYZE_H__6762347239057439875

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\comm\tableheadstruct.h"

class CExternTableAnalyze
{
public:
	CExternTableAnalyze();
	~CExternTableAnalyze();
protected:
	//时间区间
	long m_lEnd;//格式:20021010
	long m_lCount;//天数
protected:
	//根据输入的盘后数据,分解出基本行情数据(开高收低等)
	StockDay* GetPeriodStockDay(StockDay* pStockDay, short nPeriod);
	//从盘后数据文件读取指定时间区域的日线数据
	int GetData(RetDiskData* pData, StockUserInfo* pStock, long lEnd, long lCount);
	//获得左区间
	inline long GetBeginDate(long lEnd, long lCount);
	//取得时间区间在文件中的位置
	BOOL GetDataPosition(CFile *pSrcFile, long lSeekBeginDate, long *plBeginPos,
		long lSeekEndDate = -1, long *plEndPos  = NULL);
protected:
	void LongToFloat(long lData, long lClose, long lUnit, short nDecimal, float &fData);
};
#endif/*AFX_EXTERNTABLEANALYZE_H__6762347239057439875*/

long CExternTableAnalyze::GetBeginDate(long lEnd, long lCount)
{
	CTime tEnd(lEnd / 10000, (lEnd % 10000) / 100, lEnd % 100, 0, 0, 0);
	CTime tBegin = tEnd - CTimeSpan(lCount, 0, 0, 0);
	return (tBegin.GetYear() * 10000 + tBegin.GetMonth() * 100 + tBegin.GetDay()); 
}