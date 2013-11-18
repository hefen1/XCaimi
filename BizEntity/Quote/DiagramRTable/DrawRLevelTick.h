/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	DrawRLevelTick.h
*	文件标识：
*	摘	  要：  绘制逐笔(LeveL2)
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-04-19
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

#define RLEVELTICK_ROW   9

class CDrawRLevelTick :
	public CDrawTableBase
{
public:
	CDrawRLevelTick(IDataSourceEx* pDataSource);
	virtual ~CDrawRLevelTick(void);

	BOOL	CreateSelf(); 
// interface
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo);
	virtual int		OnUpdateData(DataHead* pData);
private:
	// 处理实时
	void            HandleStockTrace(StockTick* pData);
// 变量
private:
	BOOL m_bDispFour;           // 是否显示第四列 现阶段上海不显示 深圳显示
	BOOL m_bFullTime;           // 配置开关: 是否显示完整时间

	long m_lBuyPrice;           // 买价
	long m_lSellPrice;          // 卖价
	unsigned long m_lCurrentTotal; // 当前总笔
	unsigned long m_nHand;         // 手数
	long          m_lPreTime;	   // 前一时间
	unsigned short m_nLineNum;     // 当前更新的行数
	long	       m_lPreNewPrice; // 前一成交价

	StockType       m_StockType;     // 获取当前股票的一些信息 用于计算时间
	CodeInfo        m_preCodeInfo;   // 图元之前的股票信息   
	IDataSourceEx*	m_pDataSource;   // 数据引擎接口指针
};
