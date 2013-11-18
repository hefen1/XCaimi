#pragma once

/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	GraphicsData.h
*	文件标识：
*	摘	  要：  画线工具数据存储类
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2011-3-1
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#include "..\..\..\Common\ConfigStruct.h"

#define GL_HighLightText					0x00000001	// K线最大最小值是否加亮（透明）显示
#define GL_DayHorAxes						0x00000002	// 日线横坐标
#define GL_DayVerAxes						0x00000004	// 日线纵坐标
#define GL_FenShiHorAxes					0x00000008	// 分时横坐标
#define GL_FenShiVerAxes					0x00000010	// 分时纵坐标
#define GL_RIGHTSTARTZOOM					0x00000020	// 日线从右边开始缩放
#define GL_BOTTOMSTARTZOOM					0x00000040	// 日线纵向从底部开始缩放
#define GL_STOCK_INFO_SHOW					0x00000080	// 右边股票信息
#define GL_WHOLE_SCREEN_SHOW				0x00000100	// 全屏显示
#define GL_CROSS_CURSOR						0x00000200	// 十字光标
#define GL_ONE_CURSOR						0x00000400	// 一字光标
#define GL_TREND_RIGHT_AXES					0x00000800	// 分时图右边坐标

// 日线
#define GL_PERIOD_TYPE_DAY_CHART_DRAG		0x00001000	// 曲线任意拖放
#define GL_KLine_TopLine					0x00002000	// 日线顶部横线
#define GL_PERIOD_TYPE_DAY_CHART_DELETE		0x00004000	// 可删除副图
#define GL_PERIOD_TYPE_DAY_CHART_ASIX_TIP	0x00008000	// 纵向年月日星期显示
#define GL_PERIOD_TYPE_DAY_TIP_DATA			0x00010000	// 当前点需要数据提示
#define GL_PERIOD_TYPE_DAY_CHUQUAN_TEXT		0x00020000	// 显示除权详细文本
#define GL_PERIOD_TYPE_DAY_AUTO_CHUQUAN		0x00040000	// 自动除权
#define GL_PERIOD_TYPE_DAY_RIGHT_CHUQUAN	0x00080000	// 向后除权
#define GL_PERIOD_TYPE_DAY_RIGHT_AXES		0x00100000	// 右边纵坐标

#define GL_PERIOD_TYPE_DAY_INDRAWK_COST		0x00200000	// 成本分布在K线内绘制
#define GL_PERIOD_TYPE_DAY_SHOWHIDE_COST	0x00400000	// 是否显示成本分布

#define GL_PERIOD_TYPE_DAY_DEFCOST_COST		0X00800000	// 成本分布
#define GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST	0X01000000	// 火焰
#define GL_PERIOD_TYPE_DAY_HUOYAODU_COST	0X02000000	// 活跃度

#define GL_TITLE_ADVISE_ICON_SHOW			0X04000000	// 标题条的建议图标
#define GL_TITLE_JIAOYI_ICON_SHOW			0X08000000	// 标题条的交易图标

#define GL_ACTIVE_HSHELP					0X10000000	// 是否选中了帮助
#define GL_STOCK_INFO_SHOW_EX				0X20000000  // 是否手工切换右小图显示

#define FONT_NORMAL				0x01  // 正常
#define FONT_BOLD				0x02  // 粗体
#define FONT_ITALIC				0x04  // 斜体
#define FONT_UNDERLINE			0x08  // 下划线

class CImageListEx : public CImageList
{
public:
	int Draw(CDC* pDC, int nImage, POINT pt, UINT nStyle,CRect rcFocus = CRect(0,0,0,0));
	BOOL DrawEx(CDC* pDC, int nImage, POINT pt, SIZE sz, COLORREF clrBk, COLORREF clrFg, UINT nStyle);
};

struct CGraphicsDataSettting
{
	DWORD m_dwStyle;	//风格

	CGraphicsDataSettting()
	{
		memset(this,0,sizeof(CGraphicsDataSettting));
	}
	//添加,删除,判断,设置,取得风格值
	void    AddStyle(DWORD dwStyle)	   { m_dwStyle |= dwStyle; }
	void    RemoveStyle(DWORD dwStyle) { m_dwStyle &= ~dwStyle; }
	BOOL    IsStyle(DWORD dwStyle)     { return BOOL(m_dwStyle & dwStyle); }
	void    SetStyle(DWORD dwStyle)	   { m_dwStyle = dwStyle; }
	double  GetStyle()								   { return m_dwStyle; }
	//操作符=
	void operator=(const CGraphicsDataSettting& dwStyle)
	{
		m_dwStyle = dwStyle.m_dwStyle;
	}
	//操作符=
	void operator=(const long dwStyle)
	{
		m_dwStyle = dwStyle;
	}
};

class CGraphicsData
{
	//颜色部分
public:
	// 图形页面
	ColorProperty clPageBK;			// 背景
	ColorProperty clMouseMoveMarkBK;// 鼠标移动标记背景
	ColorProperty clUp;				// 上涨
	ColorProperty clDown;			// 下跌
	ColorProperty clEqual;			// 平盘
	ColorProperty clPreClose;	    // 昨收
	ColorProperty clHorLine;		// 横格线
	ColorProperty clVerLine;		// 竖格线
	ColorProperty clGridLine;		// 图形边框线
	ColorProperty clChartBK;		// 图形背景
	ColorProperty clChartTick;		// 图形坐标线
	ColorProperty clChartTickTXT;   // 图形坐标文本
	ColorProperty clChartTitle;		// 图形标题	
	ColorProperty clHorTXT;			// 横坐标文本
	ColorProperty clFixedTXT;		// 固定文本
	ColorProperty clFixedNameTXT;	// 名称标题文本
	ColorProperty clFixedCodeTXT;	// 代码标题文本
	ColorProperty clInChartTitleTXT; // 图形内标题文本
	ColorProperty clMoveLine;
	ColorProperty clVerDayTXT;		// 日线纵坐标文本
	ColorProperty clVerAmountTXT;   // 成交量纵坐标文本
	ColorProperty clVerLiangBiTXT;  // 量比纵坐标文本
	ColorProperty clVerBuySellTXT;	// 买卖力道纵坐标文本
	ColorProperty clVerADLTXT;		// ADL纵坐标文本
	ColorProperty clVerDouKongTXT;	// 多空纵坐标文本
	ColorProperty	clHorAmountTXT;		// 成交量横坐标文本
	ColorProperty 	clHorLiangBiTXT;	// 量比横坐标文本
	ColorProperty 	clHorBuySellTXT;	// 买卖力道横坐标文本
	ColorProperty 	clHorADLTXT;		// ADL横坐标文本
	ColorProperty 	clHorDouKongTXT;	// 多空横坐标文本
	// 即时分析/大盘分析
	ColorProperty clJsPriceLine;	// 价格线
	ColorProperty clJsAvgLine;		// 均线
	ColorProperty clJsPriceMiddleLine;	// 价格线的中间横线
	///////////////////////////////////////////////////////////////////////////////////
	ColorProperty clJsPriceMiddleVerLine;	// 价格线的中间竖线
	ColorProperty clJsPriceHourLine;	// 价格线的小时线
	ColorProperty clJsPriceHalfHourLine;	// 价格线的半小时线
	//上面的价格线的中间竖线（上下午分割线），小时线，半小时线的添加绘画由覃宏念在2008-11-4完成
	ColorProperty clJsLiangBiLine;  // 量比指标
	ColorProperty clJsDownVolLine;	// 成交量跌
	ColorProperty clJsUpVolLine;	// 成交量涨
	ColorProperty clJsADLLine;		// 大盘ADL
	ColorProperty clJsDouKongLine;	// 大盘多空
	ColorProperty clJsBuyLine;		// 买量
	ColorProperty clJsSellLine;		// 卖量
	ColorProperty crDiffValueColor; // 买卖差值
	// 日线
	ColorProperty clDayUpLine;		 // 上涨线(阳线)
	ColorProperty clDayDownLine;	 // 下跌线(阴线)
	ColorProperty clDayUpFillLine;   // 上涨填充(填充阳线)
	ColorProperty clDayDownFillLine; // 下跌填充(填充阴线)
	ColorProperty clDayZeroHorLine;  // 零点横线
	ColorProperty clDayAmericaLine;	 // 美国线
	ColorProperty clDayTrendLine;	 // 趋势线
	ColorProperty clDayGridVerLine;	 // 日线网格竖线
	ColorProperty clDotHorLine;      // 分时图，网格横线
	ColorProperty clDotVerLine;      // 分时图，网格竖线
	ColorProperty clDayHorLine;      // 日线图，网格横线
	ColorProperty clDayVerLine;      // 日线图，网格竖线
	ColorProperty clDayTopLine;      // qinhn2008-11-7添加，日线图中左边垂直方向的三个方框的顶部横线，即如MA，VOL字下面的横线
	ColorProperty clMaxMinValueBk;   //日线图最大最小值加亮背景色
	ColorProperty clMaxMinValueFK;   //日线图最大最小值加亮前景色
	ColorProperty clJsIOPVLine;   // IOPV线

	ColorProperty crHightBkColor;		// 加亮背景颜色
	ColorProperty clBoxLine;     /////DrawBox边框颜色
	ColorProperty clBoxLineWidth;     /////DrawBox边框线条颜色和大小
	ColorProperty clDrawBoxValue;     ///DrawBox文本颜色

	ColorProperty clBottomBK;		///底部时间显示背景颜色
	ColorProperty clBottomChoose;	///底部日线（分钟线等）切换文本颜色
	ColorProperty clInfoLine;		///信息地雷边框颜色
	ColorProperty clInfoBk;			/////信息地雷背景
	ColorProperty clInfoHilight;     ///信息地雷高亮
	ColorProperty clSplitLine;     ///分割线
	ColorProperty clInChartText;    //  图元内文本
	
	// -----------------------------------
	// 字体部分
	// -----------------------------------	 
	FontProperty ftXAxis;			// 横向坐标
	FontProperty ftYAxis;			// 纵向坐标
	FontProperty ftMulti;		    // 放大倍数
	FontProperty ftDrawVarTitle;	// 顶部说明
	FontProperty ftDrawInText;		// 图形内标题文本
	FontProperty ftPopupText;		// 图形弹出文本
	FontProperty ftPeriod;			// K线周期字体设置 modify by lxqi 20100305
	FontProperty ftTab;				// tab

	CImageListEx*	m_expressImages;		 //全局的图标列表

//////////////////////////////////////////////////////////////////////////
private:
	CArray<ColorProperty*,ColorProperty*>	m_ayColorProperty;		// 当前使用的颜色
	CArray<FontProperty*,FontProperty*>		m_ayFontProperty;		// 当前使用的字体
	
	CArray<ColorProperty*,ColorProperty*> m_ayDefTechColor;			// 缺省指标颜色

	CGraphicsDataSettting					m_sStyle;	
public:
	CGraphicsData();
	~CGraphicsData();

	BOOL			 Init();
	/********************************************************************************
	* 函数功能 : 全局风格操作,包括:添加,删除,判断,设置,取得
	* 函数参数 : YlsStyleSettting_Type dwStyle - 全局风格
	*  返回值  : void
	*			  成功返回TRUE, 失败返回FALSE
	* 调用依赖 : 全局风格相关
	* 作    者 :俞礼石
	* 完成日期 :2003-5-7
	* 修改记录 : 
	*******************************************************************************/
	void    AddStyle(DWORD dwStyle)    { m_sStyle.AddStyle(dwStyle); }
	void    RemoveStyle(DWORD dwStyle) { m_sStyle.RemoveStyle(dwStyle); }
	BOOL    IsStyle(DWORD dwStyle)	   { return m_sStyle.IsStyle(dwStyle); }
	void    SetStyle(DWORD dwStyle)	   { m_sStyle.SetStyle(dwStyle); }
	void    SetStyle(CGraphicsDataSettting oStyle)	       { m_sStyle = oStyle; }
	double  GetStyle()								   { return m_sStyle.GetStyle(); }

	CFont*			CreateFont(char * cFontName,int nFontSize, int nFontStyle);
	/********************************************************************************
	* 函数功能 : 判断值的有效性 有効区间[hx_DefMinValues hx_DefMaxValues]
	* 函数参数 : long* lValue,...; float* lValue,... - 待判断数据 多个
	*  返回值  : int 数据有效返回TRUE 否则返回FALSE
	* 调用依赖 : 
	* 作    者 :
	* 完成日期 :
	* 修改记录 : 
	*******************************************************************************/
	BOOL			IsValue(double* lValue,...);
	long			HsGetYearMothDay(long lDate);
	ColorProperty*	GetDefTechColor(short nIndex);
};

extern CGraphicsData g_hsGraphicsData;