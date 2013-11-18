/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDrawRFinance.h
*	文件标识：
*	摘	  要：  右小图财务数据
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-01-07
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "DrawTableBase.h"


#define RFinance_ROW_HEIGHT     22
#define RFinance_COL            2
#define RFinance_Text_Color     RGB(128, 128, 128)

class CDrawRFinance :
	public CDrawTableBase
{
public:
	CDrawRFinance(IDataSourceEx* pDataSource);
	~CDrawRFinance(void);

	BOOL	CreateSelf(); 
	virtual	BOOL    GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
protected:
	void    OnUpdateFinaceData(); // 处理财务数据
private:
	CArray<CurrentFinanceData> m_ayFinanceData; // 保存请求回来的财务数据
	CUIntArray                 m_ayTitleInt;    // 需要显示的财务项宏定义
	CStringArray               m_ayTitleStr;    // 需要显示的财务项字符串 

	IDataSourceEx*	           m_pDataSource;   // 数据引擎接口指针
};
