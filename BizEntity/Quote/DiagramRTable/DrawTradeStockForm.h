/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	DrawTradeStockForm.h
*	文件标识：
*	摘	  要：  交易五档行情图元
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2010-12-6
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
#include "DataSourceDefineEx.h"

class CDrawTradeStockForm :
	public CDrawTableBase
{
public:
	CDrawTradeStockForm(IDataSourceEx* pDataSource);
	~CDrawTradeStockForm(void);

	BOOL	 CreateSelf();  // 创建五档
	BOOL     CreateBonds(); // 创建债券五档
	BOOL     CreateStock(); // 创建股票五档(其余类型也采用该构造函数)

	void     HandleNowData(DataHead* pHead, CommRealTimeData_Ext* pnowData, int nSize); // 处理实时数据
	BOOL     UpdateBondsData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);    // 更新债券数据
	BOOL     UpdateStockData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);    // 更新股票数据

	void     InitColInfo();    // 初始化列属性
	long     FindMaxCount();   // 从买盘量和卖盘量中 找到最大量
	WORD     GetTextColor(double newValue, double preValue);
//interface
	virtual  BOOL    DrawTableBK(CDC* pDC);
	virtual  BOOL    DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem);
	virtual  CString GetTipText(int nCellID, int nItemID);
	virtual  BOOL	 GetRequestData(IRequestCollector* pReqCollector, StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual  int     OnUpdateData(DataHead* pData);
	virtual  BOOL    MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
private:
	double m_nPreTotal;     // 之前的总手用来过滤 两个相同的主推包
	long   m_lMaxCount;     // 最大量 
	long   m_ayBuyCount[5]; // 买盘量
	long   m_aySellCount[5];// 卖盘量

	CodeInfo                   m_preCodeInfo;   // 图元之前的股票信息   
	IDataSourceEx*	           m_pDataSource;   // 数据引擎接口指针
};
