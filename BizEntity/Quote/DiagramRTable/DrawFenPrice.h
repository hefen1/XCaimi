/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	DrawTableBase.h
*	文件标识：
*	摘	  要：  分价表
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-03-11
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

#define FENPRICE_ROW_HEIGHT      22  // 行高
#define FENPRICE_COL_COUNT        4  // 列数
#define FENPRICE_COL_WIDTH_NUM1  45  // 第一列列宽
#define FENPRICE_COL_WIDTH_NUM2  50  // 第二列列宽
#define FENPRICE_COL_WIDTH_NUM3  55  // 第三列列宽
#define FENPRICE_COL_WIDTH_NUM4   0  // 第四列列宽 第四列是非固定列 大小在基类里面会自动计算好

struct CFenPriceData
{
	long m_lPrice;      //  价格
	long m_lBuyVolume;	//	买量
	long m_lSellVolume;	//	卖量
	long m_lTurnover;   //  成交笔数
};


class CDrawFenPrice :
	public CDrawTableBase
{
public:
	CDrawFenPrice(IDataSourceEx* pDataSource, IHsColor* pColor, IHsFont* pFont);
	~CDrawFenPrice(void);

	void CreatSelf();  // 创建固定列
	void InitSize(int nRowCount);   // 设置行列属性
	void InsertArray(StockTick* pStockTick); // 解包 并且按价格从大到小排列插入到数组中
	void RemoveAllData(); // 从数组中删除所有数据
	void UpdateCell();    // 通过得到的数据 更新单元格
	long FindMaxVolume(); // 从数组中找到某一个价格最大的成交量
//interface
	virtual int     Draw( CDC* pDC );
	virtual BOOL    DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem);	
	virtual CString GetTipText(int nCellID, int nItemID);
	virtual	BOOL    GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int     OnUpdateData( DataHead* pData );
private:
	IDataSourceEx*	          m_pDataSource;   // 数据引擎接口指针
	IHsColor*                 m_pColor;        // 颜色接口指针 
	IHsFont*                  m_pFont;         // 字体接口指针

	StockType                 m_StockType;     // 当前股票的对应一些信息
	CArray<CFenPriceData*, CFenPriceData*&>    m_ayFenPriceData;// 分价数据数组
	
	COLORREF m_clrTitleBK;    // 标题背景色

	long    m_lTotalVol;      // 总成交量
	long    m_lPreNewPrice;   // 前一个包的成交价
	long    m_lPreSellPrice;  // 前一卖价
	long    m_lPreBuyPrice;   // 前一买价
	long    m_lStockTickCount;// 记录STOCKTICK包的size
};
