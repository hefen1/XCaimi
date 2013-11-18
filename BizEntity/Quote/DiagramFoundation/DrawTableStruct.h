/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	DrawTableStruct.h
*	文件标识：
*	摘	  要：  绘制主要结构以及全局变量 定义文件
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2010-12-1
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/

#pragma once
#include "DiagramFoundation.h"
#include "DiagramFoundationDef.h"
extern CDiagramFoundationApp theFoundationApp;

#define  g_lDefautColor RGB(0,0,0)
//#define  g_lHighLightColor RGB(1,37,218)

//////////////////////////////////////////////////////////////////////////
// 光标样子
#define  MOUSE_SIZEWE   0x0001 // 鼠标光标西东
#define  MOUSE_ARROW    0x0002 // 鼠标光标原样
#define  MOUSE_HAND     0x0004 // 鼠标光标手形
// 当前需要操作的标志
#define  NEED_NOTHING       0x0001 // 不需要做任何操作
#define  NEED_RESIZE        0x0002 // 需要调整大小
#define  NEED_MOVE          0x0004 // 需要左右移动
#define  NEED_TEXTHIGHLIGHT 0x0008 // 文本高亮显示
#define  NEED_TIP           0x0010 // 需要显示Tip
// Tip
#define  DELAY_SHOWTIP_TIMESPAN 500    // Tip延迟显示时间
#define  OFFSET_CX               10    // Tip显示的起始位置偏移当前鼠标坐标 X轴
#define  OFFSET_CY               10    // Tip显示的起始位置偏移当前鼠标坐标 Y轴

// 颜色
#define  TEXT_COLOR_UP				0x0001 // 上涨颜色
#define  TEXT_COLOR_DOWN			0x0002 // 下跌颜色
#define  TEXT_COLOR_FLAT			0x0003 // 平盘颜色
#define  TEXT_COLOR_TIME			0x0004 // 时间颜色
#define  TEXT_COLOR_FIXEDNAME		0x0005 // 字段名称颜色
#define  TEXT_COLOR_ACTIVEFIXEDNAME 0x0006 // 字段名称激活颜色
#define  TEXT_COLOR_XIANHAND        0x0007 // 现手颜色
#define  TEXT_COLOR_ZONGBI          0x0008 // 总笔颜色 与 现手颜色一样
#define  TEXT_COLOR_JINGZHI         0x0008 // 净值颜色 与 现手颜色一样
#define  TEXT_COLOR_CHENGJIAO       0x0009 // 成交额颜色
#define  TEXT_COLOR_ZONGE           0x0009 // 总额颜色 与 成交额一样
#define  TEXT_COLOR_HUANSHOU        0x000A // 换手颜色
#define  TEXT_COLOR_LIANGBI         0x000B // 量比颜色
#define  TEXT_COLOR_STOCKCODE       0x000C // 股票代码颜色
#define  TEXT_COLOR_STOCKNAME       0x000D // 股票名称颜色
#define  TEXT_COLOR_STOCKNAME_SELF  0x000E // 自选股股票名称颜色
#define  TEXT_COLOR_TOTALHAND		0x000F // 总手颜色
#define  TEXT_COLOR_MEIBI           0x000F // 每笔颜色 与 总手颜色一样
#define  TEXT_COLOR_WEIBUY          0x0010 // 委买量颜色
#define  TEXT_COLOR_BUYCOUNT        0x0010 // 买量颜色 与 委买量颜色一样
#define  TEXT_COLOR_JUNLIANG        0x0010 // 5日均量颜色 与 委买量颜色一样
#define  TEXT_COLOR_WEISELL         0x0020 // 委卖量颜色
#define  TEXT_COLOR_SELLCOUNT       0x0020 // 卖量颜色 与 委卖量颜色一样
#define  TEXT_COLOR_TIPFG           0x0030 // Tip文本颜色
#define  TEXT_COLOR_TIPBK           0x0040 // Tip背景颜色
#define  TEXT_COLOR_TRADEFIXEDNAME  0x0050 // 交易图元固定文本颜色
#define  TEXT_COLOR_TRADEUP         0x0060 // 交易上涨颜色
#define  TEXT_COLOR_TRADEDOWN       0x0070 // 交易下跌颜色
#define  TEXT_COLOR_TRADEXIANSHOU   0x0080 // 交易现手颜色

// 字体
#define  TEXT_FONT_FIXED      0x0001 // 静态字段字体       
#define  TEXT_FONT_DYNAMICAL  0x0002 // 动态字段字体
#define  TEXT_FONT_FINANCE    0x0003 // 配财值页文本字体
#define  TEXT_FONT_TIME       0x0004 // 时间列文本字体
#define  TEXT_FONT_TITLE      0x0005 // 表格标题字体
#define  TEXT_FONT_TIP        0x0006 // TIP字体
#define  TEXT_FONT_TRADE      0x0007 // 交易图元字体
#define  TEXT_FONT_QUOTE_CODE 0x0008 // 行情报价代码字体
#define  TEXT_FONT_QUOTE_NAME 0x0009 // 行情报价名称字体
//////////////////////////////////////////////////////////////////////////
//画笔结构
struct HsLOGPEN// :public LOGPEN
{
public:
	enum HorVer {hor,ver};

	UINT        lopnStyle;	//线风格
	POINT       lopnWidth;	//线宽度
	COLORREF    lHorColor;	//横向线颜色指针
	COLORREF    lVerColor;  //纵向线颜色指针

	HsLOGPEN()
	{
		Empty();
	}
	//设置缺省
	void Empty()
	{
		lHorColor = lVerColor = g_lDefautColor;
	}
	//赋值操作符
	void operator=(HsLOGPEN logpen)
	{
		lopnStyle = logpen.lopnStyle;
		lopnWidth = logpen.lopnWidth;
		lHorColor = logpen.lHorColor;
		lVerColor = logpen.lVerColor;		
	}
	LOGPEN GetPen(HorVer horver)
	{
		LOGPEN pen;
		pen.lopnStyle = lopnStyle;
		pen.lopnWidth = lopnWidth;
		pen.lopnColor = (horver == hor)?lHorColor:lVerColor;
		return pen;
	}
};

//////////////////////////////////////////////////////////////////////////
// 
class HS_EXT_CLASS DrawTool
{
public:
	static COLORREF GetColor(WORD wClr);
	static CFont* GetFont(WORD wFont);

private:
	static CMap<WORD,WORD&, DWORD, DWORD&>  m_mapColor;
	static CMapWordToPtr                    m_mapFont;
};

//////////////////////////////////////////////////////////////////////////
struct ItemConstructionStruct
{// 构造单元小格时 属性结构体
	char   str[64];     // 文本内容
	WORD   wClr;        // 颜色ID
	WORD   wFont;       // 字体ID
	DWORD  dwItemStyle; // 单元格属性
	double dWidth;      // 所占宽度百分比
	DWORD  dwExtent;    // 扩展
};
//////////////////////////////////////////////////////////////////////////

struct ColInfoStruct
{// 列属性信息
	int   nWidth;       // 列宽
	int   nID;          // 列ID

	ColInfoStruct(int width = 0, int ID = -1)
	{
		nWidth = width;
		nID    = ID;
	}

	void Copy(ColInfoStruct* pColInfo)
	{
		if (pColInfo)
		{
			nWidth = pColInfo->nWidth;
			nID    = pColInfo->nID;
		}
	}

	void operator=(ColInfoStruct* pColInfo)
	{
		Copy(pColInfo);
	}

	void operator=(ColInfoStruct pColInfo)
	{
		Copy(&pColInfo);
	}

};
