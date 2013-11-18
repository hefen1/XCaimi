/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	DrawRStockForm.h
*	文件标识：
*	摘	  要：  五档行情图元
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
#include "FormulaManPublic.h"

class CDrawRStockForm :
	public CDrawTableBase
{
public:
	CDrawRStockForm(IDataSourceEx* pDataSource, BOOL bLevel = FALSE);
	virtual ~CDrawRStockForm();

	BOOL	 CreateSelf();  // 创建五档
	BOOL     CreateBonds(); // 创建债券五档
	BOOL     CreateStock(); // 创建股票五档
	BOOL     CreateFund();  // 创建基金 lof五档
	BOOL     CreateEFT();   // 创建ETF五档
	BOOL     CreateIndex(); // 创建指数
	BOOL     CreateFuture();// 创建期货
	/********************************************************************************
	 * 函数功能 : 返回单元格文本格式, 
	 * 函数参数 : double newVelue - 当前值 
	 *			  double preValue - 之前值
	 *  返回值  : COLORREF 颜色
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-12-7
	 * 修改记录 : 
	 *******************************************************************************/
	WORD  GetTextColor(double newValue, double preValue);

	/********************************************************************************
	 * 函数功能 : 获取闪电下单的买卖方向
	 * 函数参数 : CPoint pos - 当前单元格下标 
	 *  返回值  : BOOL - 1: 表示买 0: 表示卖
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-12-7
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL  GetQuickTradeDirection(CPoint pos, double dPrice); 

	void  HandleNowData(DataHead* pHead, CommRealTimeData_Ext* pnowData, int nSize); // 处理实时数据
	BOOL  UpdateFundData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);     // 更新基金 lof数据
	BOOL  UpdateETFData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);      // 更新EFT数据
	BOOL  UpdateBondsData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);    // 更新债券数据
	BOOL  UpdateStockData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);    // 更新股票数据
	BOOL  UpdateIndexData(HSIndexRealTime* pIndex);    // 更新股票数据

	//////////////////////////////////////////////////////////////////////////
	// 以下是leve2处理函数
	BOOL  UpdateFundData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData);  // 更新基金 lof数据
	BOOL  UpdateETFData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData);   // 更新EFT数据
	BOOL  UpdateBondsData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData); // 更新债券数据
	BOOL  UpdateStockData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData); // 更新股票数据

//interface
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
	virtual BOOL    MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
protected:
	void    InitPriceMap(CMap<long,long,long,long> &lMap, long lPrice[5]);

	WORD    GetSellChange(long lSellPrice, long lNowSellCount, char* strText); // 计算卖量变化
	WORD    GetBuyChange(long lBuyPrice, long lNowBuyCount, char* strText);    // 计算买量变化
private:
	CMap<long,long,long,long> m_buyMap;  // 买1~5量
	CMap<long,long,long,long> m_sellMap; // 卖1~5量
	long   m_nNewPrice; // 当前股票最新价

	CodeInfo                   m_preCodeInfo;   // 图元之前的股票信息   
	IDataSourceEx*	           m_pDataSource;   // 数据引擎接口指针
	BOOL                       m_bLevel;        // 是否为level2
	RealTimeDataLevel          m_sLevelData;    // 用来过滤相同的level2主推包
	HSStockRealTime            m_sRealTime;     // 用来过滤相同的level1主推包
};
