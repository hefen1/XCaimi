/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	DrawRValue.h
*	文件标识：
*	摘	  要：  右小图值
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

#define RValue_ROW_HEIGHT     20
#define RValue_ROW_COUNT      18
#define RValue_COL_COUNT       2

class CDrawRValue :
	public CDrawTableBase
{
public:
	CDrawRValue(IDataSourceEx* pDataSource);
	~CDrawRValue(void);

	void CreateSelf();
	void HandleValueData(AnsValueData* pData); // 处理应答
	
	WORD GetColor(double dNewValue, double dPreValue);
// interface
	virtual	BOOL    GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
private: 
	IDataSourceEx*          m_pDataSource;    // 数据引擎指针
 	AnsValueData*			m_ptagAnsValueData; // 存储的当前数据
	StockType               m_stkType;
};
