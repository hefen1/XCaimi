/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	DrawRTick.h
*	文件标识：
*	摘	  要：  绘制分笔(LeveL1)
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2010-12-31
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

#define RTICK_ROW         9
#define RTICK_ZHUBI_ROW   20
#define RTICK_ROW_HEIGHT  18

class CDrawRTick 
	: public CDrawTableBase
{
public:
	CDrawRTick(IDataSourceEx* pDataSource);
	virtual ~CDrawRTick(void);

	BOOL	CreateSelf(); 
	void    InitFlag();      // 初始化标记值
// interface
	virtual BOOL    Create(IDrawBaseParam *pParam);
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
	virtual BOOL	MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual void    OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam);
protected:
	void    EmptyCellHighLight();  // 清空所有高亮属性
	void    InitData(StockTick* pTick);           // 初始化数据
private:
	// 处理实时
	void    HandleStockTrace(StockTick* pData);
	// 处理主推
	void    HandleNowData(DataHead* pHead, CommRealTimeData_Ext* pnowData, int nSize);  // LEVEL1
	void    HandleNowData(DataHead *pHead, HSLevelTransaction *pData, int nSize);       // LEVEL2
	// 一些功能函数
	unsigned int GetLineNum(); // 获取显示的行号
	WORD    GetTextColorID(double newValue, double preValue); // 比较前后两个值，获取颜色ID
// 变量
private:
	BOOL m_bDispFour;           // 是否显示第四列 现阶段上海不显示 深圳显示
	BOOL m_bFullTime;           // 配置开关: 是否显示完整时间

	long m_lStockTickCount;     // tick包中数量
	unsigned long m_lTotalVol;	// 总手
	long	m_lPreNewPrice;     // 前一成交价

	unsigned long m_nTotalHand;	// 总持仓量
	long  m_lBuyPrice;		    // 买价
	long  m_lSellPrice;		    // 卖价
	long  m_lPreTime;		    // 前一时间
	long  m_lPreSecond;         // 前一时间的秒

	unsigned short   m_nDataLineNum;  // 当前有数据的行数
	unsigned short*  m_ayRowFlag;     // 记录每一列的标记值
	unsigned short   m_nFlag;         // 标记值

	BOOL m_bLevel;              // 是否为Level2
	BOOL m_bZhuBi;              // 是否为逐笔
	
	HSLevelTransaction   m_sLevelDataAuto; // level2包中的第一项 用来区别相同的主推包      

	StockType       m_StockType;     // 获取当前股票的一些信息 用于计算时间
	IDataSourceEx*	m_pDataSource;   // 数据引擎接口指针
};
