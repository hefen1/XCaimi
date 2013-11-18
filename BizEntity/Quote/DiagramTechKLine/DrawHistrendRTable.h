#pragma once 
/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	DrawBox.h
*	文件标识：
*	摘	  要：  信息提示框
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-4-28
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#include "..\DiagramFoundation\DrawTableBase.h"

class CDrawHisTrendRTable : public CDrawTableBase
{
protected:
	int					m_nRowCount;
	int					m_nColCount;
public:
	CDrawHisTrendRTable();
	virtual ~CDrawHisTrendRTable();

//interface
	virtual BOOL DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem);	

//属性
protected:
	BOOL		CreateTable();
public:
	BOOL		UpdateData(StockUserInfo* pStock, StockDay* pStockDayData, long lPreClose, long lFinaceTotalHand);
};
