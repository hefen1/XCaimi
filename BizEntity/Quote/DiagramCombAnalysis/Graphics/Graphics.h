/*******************************************************************************
 * Copyright (c)2003, 杭州恒生信息技术有限公司
 * All rights reserved.
 *
 * 文件名称：Graphics.h
 * 文件标识：绘制操作类
 * 摘    要：绘制操作类
 *
 * 当前版本：2.0
 * 作    者：俞礼石
 * 完成日期：2003年05月16日 星期五 
 *
 * 取代版本：
 * 原 作 者：俞礼石
 * 完成日期：2003年05月16日 星期五 
 * 备	 注：
 *******************************************************************************/
// Graphics.h: interface for the CGraphics class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


#include <math.h>
#include "HSBaseDC.h"
#include "..\..\..\Common\QuoteComm\HSDrawBaseStruct.h"
#include "..\..\..\Common\QuoteComm\DrawBase.h"
#include "..\..\..\Common\ConfigStruct.h"
#include "..\..\..\Common\hsstructnew.h"
#include "..\..\..\Common\hsds_comudata.h"
#include "..\..\..\Common\configDefined.h"

#include "GraphicsData.h"
#define  PiV     3.14159265358979324                       
#define  ConstV  180

#define  MOVECOSTCOUNT 10

#define PKLINE_TYPE_FENSHI   		0x0001  // 分时图
#define PKLINE_TYPE_K       		0x0002  // k 线图

// 坐标轴/显示属性
/*
#define DT_LEFT             0x00000000  // 文本显示
#define DT_CENTER           0x00000001
#define DT_RIGHT            0x00000002
#define DT_VCENTER          0x00000004
#define DT_BOTTOM           0x00000008
*/
#define AXIS_LEFT			0x00000010  // 左边坐标
#define AXIS_TOP			0x00000020  // 上边坐标
#define AXIS_RIGHT			0x00000040  // 右边坐标
#define AXIS_BOTTOM			0x00000080  // 下边坐标

#define AXIS_FIRSTTEXT      0x00000100  // 顶部或左边文本
#define AXIS_LASTTEXT       0x00000200  // 下部或右边文本
#define AXIS_FENSHIPRICE    0x00000400  // 分时价格坐标
#define AXIS_DRAWBACK       0x00000800  // 图形是否画背景
#define AXIS_DRAWBOUNDARY   0x00001000  // 图形是否画边界
#define AXIS_DUOKONGMIDDLE  0x00002000  // 多空中间横线
#define AXIS_SOLIDANDDASH   0x00004000  // 分时价格坐标实线和虚线交挫绘制
#define AXIS_FENSHIHORI	    0x00008000  // 分时横坐标

#define AXIS_LEFTMARGIN		0x00010000  // 左边界
#define AXIS_TOPMARGIN		0x00020000  // 上边界
#define AXIS_RIGHTMARGIN	0x00040000  // 右边界
#define AXIS_BOTTOMMARGIN	0x00080000  // 下边界

#define AXIS_SELEXPLAINRECT	0x00100000  // 选中图形文字显示区域
#define AXIS_FORCEDMARGIN	0x00200000  // 强制使图形左右边界大小一致
#define AXIS_ACTIVE			0x00400000  // 当前是活动状态
#define AXIS_NOTDRAWFIRST	0x00800000  // 不画第一个

#define AXIS_NOTDRAWLAST	0x01000000  // 不画最后一个
#define AXIS_RIGHTDISPPRICE	0x02000000  // 右边要显示价
#define AXIS_STARHORI		0x04000000  // 星空图的横坐标
#define AXIS_HIGHFREQUENCYHORI 0x08000000 //高频横坐标

// 图形选中
#define CHART_SELPKLINE		0xF1  // 选中Ｋ线
#define CHART_SELVOLUME		0xF2  // 选中成交量
#define CHART_SELMACD		0xF3  // 选中MACD

#define CHART_MOVED			0x08  // 平移
#define CHART_LIGHT			0x10  // 加亮

//////////////////////////////////////////////////////////////////////////////////
////// class CGraphics
class CGraphics
{
public:
	CGraphics();
	CGraphics(CDrawBase* pThis)    { Defaut(); m_pThis = pThis; }
	void SetThis(CDrawBase* pThis)	 { m_pThis = pThis; }

	~CGraphics();

	void Defaut();
	/*结构
	成本分布云
	*/
	struct MoveCostConfig
	{
		char   cValid;		//标注成本分布数据是否有效

		int    nMoveCostMul;//预留
		int    nCostDay;	//成本分析数据个数
		int    nHand;		//预留
		double dLuiTongPan;	//预留
		double nMaxData;	//最大值

		void EmptyCost();

		MoveCostConfig() 
		{ 
			nMoveCostMul = 1;
			nCostDay = 500;
            nHand = 0;
			dLuiTongPan = 0;
			nMaxData = 0;
			cValid = 0; 
		}
		//返回数据有效性
		BOOL IsValid() { return cValid; }
		//成本分布,火焰山,活跃度数据
		static double dCostNowData[MOVECOSTCOUNT + 1][1000];
	};
	//激活状态结构
	struct CurrentActiveStatus
	{
		void* m_pCurObj;	//指示当前图元
		CRect m_rcAtive;	//是否激活
		CurrentActiveStatus()
		{
			m_pCurObj = NULL;
			m_rcAtive.SetRectEmpty();
		}
	};
	//边界区域参数结构
	struct CMarginParam
	{
		int m_nDef,m_nMin,m_nMax;//缺省值, 最小, 最大值

		enum marginType{left,top,right,bottom};

		CMarginParam() { memset(this,0,sizeof(CMarginParam)); }
		CMarginParam(int nDef, int nMin, int nMax)
		{
			m_nDef = nDef;
			m_nMin = nMin;
			m_nMax = nMax;			
		}
		const CMarginParam& operator=(const CMarginParam& marginSrc)
		{
			m_nDef = marginSrc.m_nDef;
			m_nMin = marginSrc.m_nMin;
			m_nMax = marginSrc.m_nMax;
			return *this;
		}
		void SetSame(int nDef)
		{
			m_nDef = nDef;
			m_nMin = nDef;
			m_nMax = nDef;
		}
	};
	//对应左 上 右 下边界
	struct CMargin { CMarginParam left,top,right,bottom; };	
	//选择状态类型定义
	enum SelectedReturnStatus { CONTINUE, FINDED = 1, BREAK };
	//特殊符号类型定义
	enum CMarkerType { CROSS  = 0x00000001, XCROSS = 0x00000002, STAR = 0x00000004,
					   CIRCLE = 0x00000008, TRIANGLE = 0x00000010, DIAMOND = 0x00000020, 
					   SQUARE = 0x00000040, FCIRCLE = 0x00000080, FTRIANGLE = 0x00000100, 
					   FDIAMOND = 0x00000200, FSQUARE = 0x00000400,
					   SELECTED = 0x00000800 ,DOWNFTRIANGLE = 0x00001000,
					   INFODATA = 0x00002000,DRAWSTRING = 0x00004000};

	enum CLineType { 
					 NONE		  = 0x00000000,
					 STICK        = 0x00000001,	  // 柱状线，从０到指标值间画垂直线
					 COLORSTICK   = 0x00000002,   // 彩色柱状线，当值为0是显示红色，否则显示绿色
					 VOLSTICK     = 0x00000004,   // 成交量柱状线，当股价上涨时显示红色空心柱，否则显示绿色空心柱
 					 LINESTICK    = 0x00000008,   // 同时画出柱状线和指标线
					 CROSSDOT     = 0x00000010,   // 小叉线
					 CIRCLEDOT    = 0x00000020,   // 小圆圈线
					 POINTDOT     = 0x00000040,   // 小圆点线

					 PKLINESTICK  = 0x00000080,   // K线
					 USERDRAWLINE = 0x00000100,   // 使用绘图函数

					 DRAW3D		  = 0x00000200,	  // 用3D方式
					 DRAWZEROLINE = 0x00001000,   // 画零点横线
					 ADDPKLINE	  = 0x00002000,   // 添加K线
					 NOTSELMARK	  = 0x00004000,   // 不画选中标志
					 LINEDRAW3D   = 0x00008000,   // 画3D曲线

					 NOTDRAWLINE  = 0x00010000,    // 当前曲线不绘制
					 NOTDRAWTEXT  = 0x00020000,    // 当前文本不绘制
					 NOTDRAWLINETEXT  = NOTDRAWLINE | NOTDRAWTEXT, // 当前曲线文本都不绘制
					 SELECTMARK       = 0x00040000,  // 交易系统、选股指示、五彩k线
/*
					#define HX_NOTTABLE			   0x00080000  // 为绘图表达式,但是不在表格里显示
					#define HX_NOTEXPRESSDRAW	   0x00100000  // 为绘图表达式,但是不绘图
					#define HX_EXPRESSDRAW		   0x00200000  // 为绘图表达式
					#define HX_EXPRESSDRAW_NONAME  0x00400000  // 为绘图表达式(没有名称)

					#define HX_EXPRESSSENTENCE	   0x00800000  // 为语句表达式
					#define HX_EXPRESSNUMBER	   0x01000000  // 为数值表达式
					#define HX_EXPRESSFUNCTION	   0x02000000  // 为函数表达式
					#define HX_EXPRESSREFERENCE	   0x04000000  // 为引用语句表达式
					#define HX_UNION_EXPRESS	   0x08000000  // 为组合表达式
					#define HX_UNKNOWFUNPARAM	   0x10000000  // 为函数表达式,其参数不定
*/
					CURVELINETOEND       = 0x20000000,    // 曲线延伸绘制
					//HX_NOTCALCMAXMIN     0x40000000,    // 不计算最大最小值
//ADD BY YUHJ
          NOTDRAWMSGMARK       = 0x80000000
//END
	};

	/********************************************************************************
	 * 函数功能 :设置缩进比率,算法参考CRatio结构定义 
	 * 函数参数 : double xmin ,ymin ,xmax ,ymax 对应 CRatio 结构成员
	 *  返回值  : void
	 * 调用依赖 : 被Default()调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-22
	 * 修改记录 : 
	 *******************************************************************************/
	void SetRatio(double xmin = 0, double ymin = 0, double xmax = 1, double ymax = 1);
	/********************************************************************************
	 * 函数功能 : 设置X,Y坐标轴(m_XRefParam, m_YRefParam)相关参数
	 * 函数参数 : double min ,max,  unit ,ref,int nDiv , nDecimal , lineWidth -
	 *			   对应CRefParam结构参数
	 *			  CHSBaseDC* pDC - 未使用
	 *  返回值  : void
	 * 调用依赖 : 窗口Reset Size将调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-22
	 * 修改记录 : 
	 *******************************************************************************/
	void SetYRefParam(double min = 0, double max = 0,
		double unit = 1, double ref = 0,
		int nDiv = 1, int nDecimal = 0,
		int lineWidth = 0,
		CHSBaseDC* pDC = NULL);

	// 自定义坐标
	CArray<double,double>* GetUsrDefAxis() { return &m_ayUsrDefAxis; }
	void AddUsrDefAxis(	double dwAxis,BOOL bDelAll = FALSE );

	void SetXRefParam(double min = 0, double max = 0, double unit = 1, 
		double ref = 0,int nDiv = 1, int nDecimal = 0,
		int lineWidth = 0,CHSBaseDC* pDC = NULL);
	/********************************************************************************
	 * 函数功能 : 设置刻度相关参数 (m_TickLineLen)
	 * 函数参数 : unsigned char lm ,tm ,rm , bm  - 对应结构CTickLineLen参数
	 *  返回值  : void
	 * 调用依赖 : 初始化
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void SetTickLineLen(unsigned char lm = 3,unsigned char tm = 3,unsigned char rm = 3,unsigned char bm = 4);
	//返回边界区域指针
	CRect* GetMargin() { return &m_rcMargin; }
	/********************************************************************************
	 * 函数功能 : 返回边界区域指针;返回区域边界区域偏移(PKLINE_TYPE_FENSHI偏移1,否则3)
	 *			  如果指定了画图设备,将首先根据字体调整区域;
	 * 函数参数 : CRect& rect - 返回边界区域top偏移
	 *			  DWORD dwStyle - 样式PKLINE_TYPE_FENSHI等
	 *			  CDC* pDC - 绘图设备指针
	 *  返回值  : CRect*	边界区域指针 &m_rcMargin
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	CRect* GetMargin(CRect& rect,DWORD dwStyle = 0,CDC* pDC = NULL);   // margin
	/********************************************************************************
	 * 函数功能 : 设置边界区域 根据风格AXIS_LEFTMARGIN,AXIS_TOPMARGIN,AXIS_RIGHTMARGIN,AXIS_BOTTOMMARGIN
	 *			  决定是否设置值或者置0	
	 * 函数参数 : CRect rect - 区域
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void		  SetMargin(CRect rect);
	/********************************************************************************
	 * 函数功能 : 设置边界区域	
	 * 函数参数 : CMarginParam margin - 值
	 *			  char cType - 设置的类型, 取值为CMarginParam::top等枚举
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void		  SetMargin(CMarginParam margin,char cType);
	/********************************************************************************
	 * 函数功能 : 取得边界区域结构指针
	 * 函数参数 : char cType - 设置的类型, 取值为CMarginParam::top等枚举
	 *  返回值  : CGraphics::CMarginParam* 边界区域结构指针,失败返回NULL
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	CGraphics::CMarginParam* GetMargin(char cType);
	/********************************************************************************
	 * 函数功能 : 设置打印刻度
	 * 函数参数 : HDC& hDC - 打印绘图设置句柄
	 *			  RECT& rect - 打印区域
	 *			  引用未改变值
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void SetPrintScale(HDC& hDC, RECT& rect);
	/********************************************************************************
	 * 函数功能 : 重新计算区域,包括客户区域,边界区域,实际绘图区域等
	 * 函数参数 : RECT& rt - 指定客户区域
	 *  返回值  : void
	 * 调用依赖 : 此函数比较重要,打印,绘图设置Size都将调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void RecalcRects(RECT& rt);
	/********************************************************************************
	 * 函数功能 : 画图初始,将填充背景与分界线
	 * 函数参数 : CHSBaseDC* pDC - 绘图设备指针
	 *  返回值  : void
	 * 调用依赖 : DrawBkGround(), DrawBoundary()
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void BeginDraw(CHSBaseDC* pDC);
	/********************************************************************************
	 * 函数功能 : 画图终止, 如果是打印,将置打印标识为终结
	 * 函数参数 : CHSBaseDC* pDC - 绘图设备指针
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void EndDraw(CHSBaseDC* pDC);
	/********************************************************************************
	 * 函数功能 : 相关颜色操作 设置或者取得颜色设置结构
	 *			  背景;网格线颜色;边框颜色;刻度线颜色;刻度文本颜色;标题颜色
	 * 函数参数 : ColorProperty* cr - 颜色结构指针;
	 *			  ColorProperty* crVer,ColorProperty* crHor - 纵横颜色结构指针
	 *  返回值  : void
	 *			  ColorProperty 颜色设置结构
	 * 调用依赖 : 改变颜色时候 , 将调用, 但不引起重画
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	
	void SetBackColor(ColorProperty* cr)						  { *m_pBackColor = cr; }
	ColorProperty GetBackColor()								  { return **m_pBackColor; }
	
	void SetGridColor(ColorProperty* crVer,ColorProperty* crHor)  { *m_pVerLineColor = crVer; *m_pHorLineColor = crHor; }
	ColorProperty GetGridVerColor()							{ return **m_pVerLineColor; }
	ColorProperty GetGridHorColor()							{ return **m_pHorLineColor; }
	
	void SetBorderColor(ColorProperty* cr)					{ *m_pGridLineColor = cr; }
	ColorProperty GetBorderColor()							{ return **m_pGridLineColor; }
	
	void SetTickLineColor(ColorProperty* cr)				{ *m_pChartTickColor = cr; }
	ColorProperty GetTickLineColor()						{ return **m_pChartTickColor; }
	
	void SetTickTXTColor(ColorProperty* cr)					{ *m_pChartTickTXTColor = cr; }
	ColorProperty GetTickTXTColor()							{ return **m_pChartTickTXTColor; }

	void SetTitleColor(ColorProperty* cr)					{ *m_pChartTitleColor = cr; }
	ColorProperty GetTitleColor()							{ return **m_pChartTitleColor; }
	//取得当前画图设备指针 直接返回m_pDC
	CHSBaseDC* GetBaseDC()						  { return m_pDC; }
	/********************************************************************************
	 * 函数功能 : 设置X轴Y轴刻度划分个数, 网格个数
	 * 函数参数 : int x, y - X轴Y轴刻度划分个数, 网格个数
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void SetXNumOfTicks(int x = 50);
	void SetYNumOfTicks(int y = 50);
  int  GetYNumOfTicks();
	void SetXNumOfGridTicks(int x = 10);
	void SetYNumOfGridTicks(int y = 10);
	/********************************************************************************
	 * 函数功能 : 取得,设置纵横收缩单位
	 * 函数参数 : int decimal - 收缩单位
	 *  返回值  : void, int
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 * 不足之处 : 函数未使用, 对应变量未使用,扩展使用
	 *******************************************************************************/
	void SetXDecimal(int decimal)				{ m_nXDecimal = decimal; }
	int  GetXDecimal()							{ return m_nXDecimal; }
	void SetYDecimal(int decimal)				{ m_nYDecimal = decimal; }
	int  GetYDecimal()							{ return m_nYDecimal; }
	/********************************************************************************
	 * 函数功能 : 设置文本对齐方式
	 * 函数参数 : DWORD x - 横向 DWORD y - 纵向
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void SetStringAlign(DWORD x, DWORD y)		{ m_StrAlign.HAlign = x; m_StrAlign.VAlign = y; }
	/********************************************************************************
	 * 函数功能 :设置字体 
	 * 函数参数 : const char* FontName - 字体名称, 非空字府串
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void SetFontName(const char* FontName)		{ strcpy(m_LogFont.lfFaceName, FontName); }
	/********************************************************************************
	 * 函数功能 : 使能边框风格以及边框阴影
	 * 函数参数 : bool bFlag - 使能标识
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 * 不足之处 : 目前未使用, 作扩展用
	 *******************************************************************************/
	void EnableLegend(bool bFlag = true)		{ m_bEnableLegend = bFlag; }
	void EnableLegendShadow(bool bFlag = true)	{ m_bLegendShadow = bFlag; }
	//使能双缓冲
	void EnableMemoryDraw(bool bFlag = false)	{ m_bMemoryDraw = bFlag; }
	/********************************************************************************
	 * 函数功能 : 画边界 
	 * 函数参数 : ColorProperty& cr - 边界线型等属性
	 *			  int nMargin - 打印时候使用, 设置缩小比率
	 *  返回值  : void
	 * 调用依赖 : 必须是AXIS_DRAWBOUNDARY风格
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
    void DrawBoundary(ColorProperty& cr, int nMargin);
	/********************************************************************************
	 * 函数功能 : 画阴影
	 * 函数参数 : int n - 偏移量
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void DrawShadow(int n);
	/********************************************************************************
	 * 函数功能 :输出标题 顶部或者底部 X轴或者Y轴等
	 * 函数参数 :const char* Title - 文本, int Pos -  位置 DT_TOP等
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 * 备注		:作扩充用, 未使用
	 *******************************************************************************/
	void Title(const char* Title, int Pos = DT_TOP);
	void XAxisTitle(const char* Title, int Pos = DT_BOTTOM);
	void YAxisTitle(const char* Title, int Pos = DT_LEFT);
	/********************************************************************************
	 * 函数功能 : 绘制图形边框以及刻度线 刻度文本
	 * 函数参数 : void
	 *  返回值  : void
	 * 调用依赖 : DrawBorder
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void Axes();
	/********************************************************************************
	 * 函数功能 : 绘制图形边框
	 * 函数参数 : void
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void DrawBorder();
	/********************************************************************************
	 * 函数功能 :	用于绘制图形内标题,例如深发展A..., 滑块指示文本等
	 * 函数参数 :	CToptextType<double>* Value,... - 标题文本结构, 包含文本,颜色等 多个
	 *				CArray< CToptextType<double>*,CToptextType<double>* >& Value
	 *					- 标题文本结构数组, 包含文本,颜色等 多个
	 *				class CDrawBase* pCurObj- 当前绘制图元, 用于取得前一图元
	 *				CNameProperty& strExpName-公式结构,包含文本,颜色属性等,要求文本非空字府串
	 *				CArray<CLineProperty*,CLineProperty*>* ayData -
	 *				int nCurDataPos-当前光标指示的数据位置
	 *				int nDiv- 未使用
	 *				short nDecimal- 数据缩放倍数(除数)
	 *				BYTE bArrow- 是否画箭头
	 *				DWORD dStyle 线状态, PKLINE_TYPE_ACTIVE_CHART
	 *				DWORD dObjectStyle  - 公式类型CExpression::Condition等
	 *  返回值  : int
	 * 调用依赖 : 画标题
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	int DrawVarTitle(CToptextType<double>* Value,...);
	int DrawVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& Value, int nStartPositon = 0);
	int DrawVarTitle(class CDrawBase* pCurObj,CNameProperty& strExpName,CArray<CLineProperty*,CLineProperty*>* ayData,int nCurDataPos,
					  int nDiv,short nDecimal,BYTE bArrow,DWORD dStyle = 0,DWORD dObjectStyle = 0);
	//设置范围 纯虚函数 
	virtual bool SetRange(double xmin, double ymin, double xmax, double ymax) = 0;
	bool SetRange_LogY(double xmin, double ymin, double xmax, double ymax);

	/********************************************************************************
	 * 函数功能 : 画X,Y轴坐标,刻度与坐标文本
	 * 函数参数 : void
	 *  返回值  : void
	 * 调用依赖 : 绘制调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void XAxis();
	virtual void YAxis();
	void YAxis_LogY();

	/********************************************************************************
	 * 函数功能 : 绘制日线图右边纵坐标刻度线以及文本
	 * 函数参数 : void
	 *  返回值  : void
	 * 调用依赖 : 绘制调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void RiXiaRightYAxis();
	/********************************************************************************
	 * 函数功能 : 分时走势图,左边纵轴刻度与文本
	 * 函数参数 :  void    
	 *  返回值  :  void    
	 * 调用依赖 :  绘制调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void FenShiYAxis();
	/********************************************************************************
	 * 函数功能 : 分时走势图,右边纵轴刻度与文本
	 * 函数参数 :  void    
	 *  返回值  :  void    
	 * 调用依赖 :  绘制调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void FenShiRightYTick();
	/********************************************************************************
	 * 函数功能 : 绘制网格线(包括分时与日线)
	 * 函数参数 : void
	 *  返回值  : void
	 * 调用依赖 : DrawMiddleLine
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void Grid();
	void		 Grid_LogY();
	/********************************************************************************
	 * 函数功能 : 绘制图形内标题的高亮选中状态
	 * 函数参数 : void
	 *  返回值  : void
	 * 调用依赖 : 目前已经取消, 函数直接返回
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void DrawExplainRect();
	//绘制焦点边框, 目前已经取消, 代码被注释
	virtual void DrawActive(BOOL bActive = FALSE);
	/********************************************************************************
	 * 函数功能 : 绘制中间的分割线
	 * 函数参数 : int x,int y,int x1,int y1- 直线的两个点
	 *			  int LineWidth , int LineStyle  - 直线的样式与宽度
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void DrawMiddleLine(int x,int y,int x1,int y1,int LineWidth = 2, int LineStyle = PS_SOLID);
	/********************************************************************************
	* 函数功能 : 绘制中间的分割竖线，即上下午分割线的绘画，管理
	 * 函数参数 : int x,int y,int x1,int y1- 直线的两个点
	 *			  int LineWidth , int LineStyle  - 直线的样式与宽度
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :覃宏念
	 * 完成日期 :2008-11-4
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void DrawMiddleVerLine(int x,int y,int x1,int y1,int LineWidth = 2, int LineStyle = PS_SOLID);
	/*******************************************************************************
     * 函数功能 : 绘制管理分时图中的小时线
	 * 函数参数 : int x,int y,int x1,int y1- 直线的两个点
	 *			  int LineWidth , int LineStyle  - 直线的样式与宽度
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :覃宏念
	 * 完成日期 :2008-11-4
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void DrawHourLine(int x,int y,int x1,int y1,int LineWidth = 1, int LineStyle = PS_SOLID);
	/********************************************************************************
	* 函数功能 : 绘制管理分时图中的半小时虚线
	 * 函数参数 : int x,int y,int x1,int y1- 直线的两个点
	 *			  int LineWidth , int LineStyle  - 直线的样式与宽度
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :覃宏念
	 * 完成日期 :2008-11-4
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void DrawHalfHourLine(int x,int y,int x1,int y1,int LineWidth = 1, int LineStyle = PS_DOT);
	/********************************************************************************
	* 函数功能 : 绘制管理日线图中的左边垂直方向三个方框的顶部横线，即MA，VOL，MACD等字列下面的横线
	 * 函数参数 : int x,int y,int x1,int y1- 直线的两个点
	 *			  int LineWidth , int LineStyle  - 直线的样式与宽度
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :覃宏念
	 * 完成日期 :2008-11-7
	 * 修改记录 : 
	 *******************************************************************************/ 
	virtual void DrawKLineTopLine(int x,int y,int x1,int y1,int LineWidth = 1, int LineStyle = PS_DOT);  
	/********************************************************************************  qinhn
	/********************************************************************************
	 * 函数功能 : 判断点是否在区域中
	 * 函数参数 : DWORD linetype - 线风格例如PKLINESTICK
	 *			  CPoint& point- 点
	 *			  CRect& rect-区域
	 *			  short nCurve,void* pMask - 未使用
	 *  返回值  : 判断成功返回FINDED, 否则CONTINUE
	 * 调用依赖 : 调用了IsOnLine
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	virtual long IsSelected(DWORD linetype,CPoint& point,CRect& rect,short nCurve,void* pMask,
						    CLineProperty* pData = NULL);
	/********************************************************************************
	 * 函数功能 : 轴风格的相关操作, 设置, 添加, 删除, 判断
	 * 函数参数 : DWORD dwAxesStyle - 轴风格
	 *  返回值  : void, BOOL:成功返回TRUE, 失败返回FALSE
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void SetAxesStyle( DWORD dwAxesStyle)	 { m_dwAxesStyle = dwAxesStyle; }
	void AddAxesStyle( DWORD dwAxesStyle)	 { m_dwAxesStyle |= dwAxesStyle; }
	void RemoveAxesStyle( DWORD dwAxesStyle) { m_dwAxesStyle &= ~dwAxesStyle; }
	BOOL IsAxesStyle ( DWORD dwAxesStyle)	 { return (m_dwAxesStyle & dwAxesStyle); }
	/********************************************************************************
	 * 函数功能 : 选择状态的相关操作, 设置, 添加, 删除, 判断
	 * 函数参数 : short cStyle - 选择状态
	 *  返回值  : void, BOOL:成功返回TRUE, 失败返回FALSE
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void SetSelectedStyle(short cStyle)	     { m_cSelectedStyle = cStyle; }
	void AddSelectedStyle(short cStyle)	     { m_cSelectedStyle = cStyle; }
	void RemoveSelectedStyle();
	BOOL IsSelectedStyle(short cStyle)		 { return (m_cSelectedStyle == cStyle); }
	BOOL IsSelectedStyle()					 { return (m_cSelectedStyle != -1); }
	/********************************************************************************
	 * 函数功能 : 设置关联列表以及拖动容器指针, 直接指针赋值m_ayDrawList,m_ayDrag
	 * 函数参数 : CArray<class CHSDrawObj*, class CHSDrawObj*>* payDrawList 关联列表指针
	 *			  CArray<class CDragObjManager*,class CDragObjManager*>* ayDrag 拖动容器指针
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void SetRelating(CArray<class CHSDrawObj*, class CHSDrawObj*>* payDrawList,
					 CArray<class CDragObjManager*,class CDragObjManager*>* ayDrag);
	//返回 绘图关联列表指针
	CArray<class CHSDrawObj*, class CHSDrawObj*>*			   GetDrawList() { return m_ayDrawList; }
	//返回 拖放目标管理指针
	CArray<class CDragObjManager*,class CDragObjManager*>* GetDragObj()  { return m_ayDrag; }	 

	/********************************************************************************
	 * 函数功能 : 取得指定图元在拖放链表中的前一个或者后一个图元
	 * 函数参数 : CDrawBase* pCurObj- 图元指针 , 非NULL
	 *			  BOOL bNext - 向前一个或后一个
	 *  返回值  : 成功返回图元指针, 失败返回NULL
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	CDrawBase* GetPrevObj(CDrawBase* pCurObj,BOOL bNext);
	CDrawBase* GetPrevObjHaveShow(CDrawBase* pCurObj,BOOL bNext);
	//返回文本注释区域
	CRect  GetExplainTextRect();
	
	/********************************************************************************
	 * 函数功能 : 返回页边区域
	 * 函数参数 : char cType - 页边区域在左边时候, 1表示绘图用, 2表示操作用, 返回不同的区域 
	 *  返回值  : CRect 页边区域, 失败返回空区域
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	CRect  GetOperatorRect(char cType = 0);
	/********************************************************************************
	 * 函数功能 : 判断点是否在页边区域内
	 * 函数参数 : CPoint pointm - 点 DWORD dwStyle - 页面类型 PKLINE_TYPE_TECH_LIST等
	 *  返回值  : 成功返回对应的页面类型, 失败返回-1
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	int	   IsOperatorRect(CPoint pointm,DWORD dwStyle = 0);
	/********************************************************************************
	 * 函数功能 : 画操作图标, 目前以及取消
	 * 函数参数 : DWORD dwStyle - 页面样式
	 *  返回值  : void
	 * 调用依赖 : 绘制页边时候会调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void   DrawOperatorRect(DWORD dwStyle);

	//返回真正的绘制区域(可以用来绘制K线等的区域)
	CRect  GetChartRect();
	/********************************************************************************
	 * 函数功能 : 取得页边区域, 绘制页边区域
	 * 函数参数 : nMask - 表示上下左右哪个位置的页边距 AXIS_LEFTMARGIN等
	 *  返回值  : CRect - 区域, 失败返回空区域; void
	 * 调用依赖 : 例如后复权有高亮填充右页边区域
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	CRect  GetMarginRect(int nMask);
	void   DrawMarginRect(int nMask);
	//返回选中注释(标题)绘制区域
	CRect  GetExplainRect() { return m_rcExplain; }
	//返回选中注释(标题)扩展菜单图标的绘制区域
	CRect  GetExplainMenuRect();
	//返回-1, 无处理, 不足:考虑去掉
	int    GetAxisRect(CPoint& pt);
	/********************************************************************************
	 * 函数功能 : 设置注释位置
	 * 函数参数 : CRect rect-区域
	 *			  CDrawBase* pThis 图元指针
	 *			  BOOL bAuto - 操作键值 3:直接赋值 2:直接返回, 0: 赋值并添加风格AXIS_SELEXPLAINRECT
	 *						非2,3,0:原区域有效的前提下赋值
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void   SetExplainRect( CRect rect,CDrawBase* pThis,BOOL bAuto = FALSE);
	//清空注释区域, 删除相关图元风格
	void   SetExplainEmpty();
	/********************************************************************************
	 * 函数功能 : 绘制一个日线(美国线等, 需要指定参数类型)
	 * 函数参数 : CKLineChartParam* sChartParam - 参数结构指针 非NULL
	 *			  StockDay&	pValues, - 日线数据, 用于画收盘价线等
	 *			  CDragObjManager* pDragObjManager, - 拖放容器指针
	 *			  CDispMaxMinValue* pCurMin- 指示最小值日线数据位置等信息的结构指针
	 *			  CDispMaxMinValue* pCurMax- 指示最大值日线数据位置等信息的结构指针
	 *			  BOOL bUp - 阳线还是阴线
	 *			  int nDataPos- 数据位置(相对全部)
	 *			  int x  - 中间线的位置
	 *			  int top - 高价位置
	 *			  int bottom- 低价位置
	 *			  CRect rect-开收组成的四方区域
	 *			  CPen* pUpPen,CBrush* pUpBrush - 阳线的线以及画刷
	 *			  CPen* pDownPen,CBrush* pDownBrush - 阴线的线以及画刷
	 *			  BOOL bSpecial - 是否为特殊标志 add by lxqi 20090507
	 *  返回值  : 成功返回TRUE, 失败返回FLASE
	 * 调用依赖 : 此函数作为日线绘制的入口函数, 由参数指定类型, 再分支操作
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL DrawPKByMark(CKLineChartParam* sChartParam,StockDay&	pValues,
					  CDragObjManager* pDragObjManager,
					  CDispMaxMinValue* pCurMin,CDispMaxMinValue* pCurMax,
					  BOOL bUp,int nDataPos,int x,int top,int bottom,CRect rect,
					  CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush,
					  BOOL bSpecial = FALSE);
	/********************************************************************************
	 * 函数功能 : 绘制K线
	 * 函数参数 : BOOL bUp - 阳线还是阴线
	 *			  int nDataPos- 数据位置(相对全部)
	 *			  int x  - 中间线的位置
	 *			  int top - 高价位置
	 *			  int bottom- 低价位置
	 *			  CRect rect-开收组成的四方区域
	 *			  CPen* pUpPen,CBrush* pUpBrush - 阳线的线以及画刷
	 *			  CPen* pDownPen,CBrush* pDownBrush - 阴线的线以及画刷
	 *  返回值  : void
	 * 调用依赖 : 被DrawPKByMark所调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void DrawPKline( BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					 CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	/********************************************************************************
	 * 函数功能 : 绘制美国线
	 * 函数参数 : BOOL bUp - 阳线还是阴线
	 *			  int nDataPos- 数据位置(相对全部)
	 *			  int x  - 中间线的位置
	 *			  int top - 高价位置
	 *			  int bottom- 低价位置
	 *			  CRect rect-开收组成的四方区域
	 *			  CPen* pUpPen,CBrush* pUpBrush - 阳线的线以及画刷
	 *			  CPen* pDownPen,CBrush* pDownBrush - 阴线的线以及画刷
	 *  返回值  : void
	 * 调用依赖 : 被DrawAmericaByMark所调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void DrawAmerica( BOOL& bUp,int& nX,int& nTop,int& nBottom,CRect& rect,
		CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	/********************************************************************************
	 * 函数功能 : 绘制收盘价线DrawClose, 普通压缩线DrawCNDEQV 线性压缩线DrawKEQV 宝塔线DrawTowerline
	 *				OX线DrawNPline
	 * 函数参数 : CKLineChartParam* sChartParam - 参数结构指针 非NULL
	 *			  StockDay&	pValues, - 日线数据, 用于画收盘价线等
	 *			  CDragObjManager* pDragObjManager, - 拖放容器指针
	 *			  CDispMaxMinValue* pCurMin- 指示最小值日线数据位置等信息的结构指针
	 *			  CDispMaxMinValue* pCurMax- 指示最大值日线数据位置等信息的结构指针
	 *			  BOOL bUp - 阳线还是阴线
	 *			  int nDataPos- 数据位置(相对全部)
	 *			  int x  - 中间线的位置
	 *			  int top - 高价位置
	 *			  int bottom- 低价位置
	 *			  CRect rect-开收组成的四方区域
	 *			  CPen* pUpPen,CBrush* pUpBrush - 阳线的线以及画刷
	 *			  CPen* pDownPen,CBrush* pDownBrush - 阴线的线以及画刷
	 *  返回值  : void                 
	 * 调用依赖 : 被DrawPKByMark所调用 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void DrawClose(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	void DrawCNDEQV(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	void DrawEQV(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	void DrawKEQV(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	void DrawTowerline(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	void DrawNPline(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
  //ADD BY YUHJ 2008.5.14
	void DrawOXline(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					        CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
  //END

	//函数只有声明无实现 不足,考虑去掉
	BOOL DrawAmericaByMark(StockDay&	pValues,
					  CDragObjManager* pDragObjManager,
					  CDispMaxMinValue* pCurMin,CDispMaxMinValue* pCurMax,
					  BOOL bUp,int nDataPos,int x,int top,int bottom,CRect rect,
					  CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);

	/********************************************************************************
	 * 函数功能 : 绘制收盘价
	 * 函数参数 : StockDay&	pValues, - 日线数据, 用于画收盘价线等
	 *			  CDragObjManager* pDragObjManager, - 拖放容器指针
	 *			  CDispMaxMinValue* pCurMin- 指示最小值日线数据位置等信息的结构指针
	 *			  CDispMaxMinValue* pCurMax- 指示最大值日线数据位置等信息的结构指针
	 *			  BOOL bUp - 阳线还是阴线
	 *			  int nDataPos- 数据位置(相对全部)
	 *			  int x  - 中间线的位置
	 *			  int top - 高价位置
	 *			  int bottom- 低价位置
	 *			  CRect rect-开收组成的四方区域
	 *			  CPen* pUpPen,CBrush* pUpBrush - 阳线的线以及画刷
	 *			  CPen* pDownPen,CBrush* pDownBrush - 阴线的线以及画刷
	 *  返回值  : void                 
	 * 调用依赖 : 无被调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL DrawCloseByMark(StockDay&	pValues,
					  CDragObjManager* pDragObjManager,
					  CDispMaxMinValue* pCurMin,CDispMaxMinValue* pCurMax,
					  BOOL bUp,int nDataPos,int x,int top,int bottom,CRect rect,
					  CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	/********************************************************************************
	 * 函数功能 : 绘制相应的信息, 例如除权标识,信息地雷等
	 * 函数参数 : int x  - X位置
	 *			  int y - Y位置
	 *			  CRect rect-开收组成的四方区域
	 *			  StockDay&	pValues, - 日线数据 未使用
	 *  返回值  : void
	 * 调用依赖 : 画完K线后绘制
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void DrawPKMessageMask(int x,int y,CRect& rect,StockDay& pValues);
	/********************************************************************************
	 * 函数功能 : 绘制最高最低标识
	 * 函数参数 :  int x  - 中间线的位置
	 *			  int top - 高价位置
	 *			  int bottom- 低价位置
	 *			  CRect rect-开收组成的四方区域
	 *			  CDispMaxMinValue* pCurMin- 指示最小值日线数据位置等信息的结构指针
	 *			  CDispMaxMinValue* pCurMax- 指示最大值日线数据位置等信息的结构指针
	 *  返回值  : void
	 * 调用依赖 : 绘制完所以K线后绘制
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void DrawMaxMin(int nXPos,int nTop,int nBottom,CRect rect,CDispMaxMinValue* pCurMin,CDispMaxMinValue* pCurMax);
	//返回此图元指针, 相当于this
	class CDrawBase* GetPKLine();
	/********************************************************************************
	 * 函数功能 : 判断数据是阴还是阳
	 * 函数参数 : int nPosition - 数据位置 
	 *  返回值  : 阳:TRUE, 阴:FALSE
	 * 调用依赖 : 绘制K线使用, 实际上调用AnalisysObjData:IsUp()完成
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL  IsUp(int nPosition);
	/********************************************************************************
	 * 函数功能 : 取得屏幕K线最大或者最小位置数据信息结构指针
	 * 函数参数 : BOOL bMax - 取最大还是最小, TRUE最大, 反之取最小
	 *  返回值  : CDispMaxMinValue* , 失败返回NULL
	 * 调用依赖 : 在绘制日线前调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	CDispMaxMinValue* GetMaxMin(BOOL bMax);
	/********************************************************************************
	 * 函数功能 :返回滑块文本
	 * 函数参数 : MouseTipTextInfo* pMouseTipTextInfo - 鼠标TIP文本结构指针, 取得文本将保存到结构
	 *			  int nPosition  - 未使用 CPoint& point - 当前鼠标点
	 *  返回值  : 成功返回TRUE, 失败返回FALSE
	 * 调用依赖 : 鼠标移动时候触发
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL GetTipText(MouseTipTextInfo* pMouseTipTextInfo,int nPosition,CPoint& point);
	int DrawAdvance(DWORD& linetype,int& nCurve,ColorProperty& crSingleLineColor,
					POINT& pt1,POINT& pt2,
					HSDouble& pCurData,HSDrawOtherData* pOtherData,int& nPos,
					int nDistance);
	/********************************************************************************
	 * 函数功能 : 判断是否三点一线
	 * 函数参数 : CPoint& ptFrom  CPoint& ptTo, CPoint& point  - 起始终止点, 中间点
	 *  返回值  : 成功返回TRUE, 失败返回FALSE
	 * 调用依赖 : 判断选中时候调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	static BOOL IsOnLine(CPoint& ptFrom, CPoint& ptTo, CPoint& point);
	static BOOL IsOnDot(CPoint& ptFrom, CPoint& ptTo, CPoint& point);


public:
	/********************************************************************************
	 * 函数功能 : 判断值的有效性 有効区间[hx_DefMinValues hx_DefMaxValues]
	 * 函数参数 : long* lValue,...; float* lValue,... - 待判断数据 多个
	 *  返回值  : int 数据有效返回TRUE 否则返回FALSE
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	int IsValue(long* lValue,...);
	int IsValue(float* lValue,...);
	int IsValue(double* lValue,...);
	/********************************************************************************
	 * 函数功能 : 根据数值,返回对应在屏幕的位置, 例如用于取得K线高点与低点的位置
	 * 函数参数 : double lValue  - 数值
	 *  返回值  : int , 屏幕的位置
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	int ValueToVertPos(double lValue);

	void SetAxesType(int nAxesType);
	int	 IsAxesType(int nAxesType);

public:
	//图元激活信息
	static CurrentActiveStatus m_curAtiveStatus;
public:
	//有効绘制区域
	RECT		m_PlotRect;
	//使能内存双缓冲绘制
	bool		m_bMemoryDraw;
	//是否正在打印
	bool		m_bPrinting;
	//打印模式
	int			m_nPreMode;
	//打印比例
	int			m_nPrintScale;
public:
	//坐标样式 分别表示普通XY, 对数X, 对数Y, 对数XY
  //CHANGE BY YUHJ 2008.3.24
	enum   CAxesType	{ XY, XLOG, XYLOG, YDEF, YLOG, YLN, YPER, YGLD, YEQU };
  //END
protected:
	//刻度结构 分别表示 X轴刻度最小单位, 轴刻度最小单位, X轴最小位置, X轴最大位置,Y轴最小位置, Y轴最大位置
	struct CScale		{ double dx, dy, xmin, ymin, xmax, ymax; };
	/*结构	缩进比率
	缩进算法(Rect):
	left  += xmin * WIDTH;
	top	  += ymin * HEIGHT;
	right  -= (1 - xmax) * WIDTH;
	bottom -= (1 - ymax) * HEIGHT;
	*/
	struct CRatio		{ double xmin, ymin, xmax, ymax; };
	//文本对齐结构 分别表示横向对齐样式, 纵向对齐样式
	struct StringAlign	{ DWORD HAlign, VAlign; };
	//坐标数据参数结构 分别表示最小值, 最大值, 一单位长度, 昨收盘, 整除除数, 小数位数; 线宽
	struct CRefParam	{ double min, max, unit, ref;int nDiv, nDecimal;int lineWidth; 
						  //数据是否有效	
						  BOOL IsValid() {return (min != 0 || max != 0);} 
						};
	//刻度线毛刺长度结构, 分别表示左,上, 右,底部毛刺长度
	struct CTickLineLen	{ unsigned char lm,tm,rm,bm; };	
	//坐标轴及文本显示风格
	DWORD      m_dwAxesStyle;	
	//图形选中风格
	short	   m_cSelectedStyle;

	//Y轴坐标数据参数结构 
	CRefParam			  m_YRefParam;
	CArray<double,double> m_ayUsrDefAxis;

	//X轴坐标数据参数结构 
	CRefParam   m_XRefParam;
	//颜色属性指针的指针
	ColorProperty**	m_pBackColor;		//背景
	ColorProperty**	m_pVerLineColor;	//纵线
	ColorProperty**	m_pHorLineColor;	//横线
	ColorProperty**	m_pGridLineColor;	//网格线
	ColorProperty**	m_pChartTickColor;	//刻度线
	ColorProperty**	m_pChartTickTXTColor;//刻度文本
	ColorProperty**	m_pChartTitleColor;	//图形内标题文本
	//刻度线毛刺长度结构数据
	CScale		m_Scale;
	//缩进比率
	CRatio		m_Ratio;	
	//字体
	static LOGFONT	m_LogFont;
	//用于保存字体可拉伸高度, 无实际使用
	SIZE		m_Size;
	//文本对齐样式
	StringAlign m_StrAlign;
	//扩展使用, 目前未使用
	int			m_nXDecimal;
	//扩展使用, 目前未使用
	int			m_nYDecimal;
	//使能边框
	bool		m_bEnableLegend;
	//使能阴影
	bool		m_bLegendShadow;
	//坐标风格 如XY等
	int			m_nAxesType;
	//扩展使用, 目前未使用
	int			m_nXStep;
	//扩展使用, 目前未使用
	int			m_nYStep;
	//边界区域
	CRect		m_rcMargin;
	//上 下 左右 边界相关参数
	CMargin		m_rcDefMargin;
	//绘图关联列表指针 直接指向
	CArray<class CHSDrawObj*, class CHSDrawObj*>*			     m_ayDrawList; 
	//拖放目标管理指针 直接指向
	CArray<class CDragObjManager*,class CDragObjManager*>* m_ayDrag;
	//画操作属主指针
	CDrawBase*	 m_pThis;
	//有効区域
	RECT		m_Rect;
	//剪裁区域
	RECT		m_ClipBox;
	//客户区域
	RECT		m_ClientRect;
	//绘图设备指针
	CHSBaseDC*	m_pDC;
	//扩展使用, 继承类使用
	POINT		m_Pt;
	//注释区域
	CRect				m_rcExplain;
	//小图标列表
	static CImageList	m_MenuBitmap;
	//小图标大小
	static SIZE			m_MenuSize;
	//对应左,右, 上, 下,宽, 搞
	int m_GL, m_GR, m_GT, m_GB, m_PX, m_PY;
	//X, Y刻度总数目, X,Y刻度大单位
	int m_XTicks, m_YTicks, m_XGridTicks, m_YGridTicks;
	//刻度线毛刺长度结构
	CTickLineLen m_TickLineLen;
	//X,Y刻度大单位里拆分小单位个数
	int	   m_XTicksUnit,m_YTicksUnit; 

protected:	
	/********************************************************************************
	 * 函数功能 : 绘制厚边框以及文本 
	 * 函数参数 : COLORREF cr- 文本颜色int Index 偏移索引 const char* Name - 文本内容
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void Legend(COLORREF cr, int Index, const char* Name);
	//返回m_Rect
	void GetPixelRect(RECT& rt);
	//设置Pixel, 并按照缩放比率赋值
	void SetPixelRect(RECT rt);
	//绘制背景
	void DrawBkGround();
	/********************************************************************************
	 * 函数功能 : 绘制圆, 填充圆DrawFilledCircle
	 * 函数参数 : int x, int y, int radius - 点, 半径
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void DrawCircle(int x, int y, int radius);
	void DrawFilledCircle(int x, int y, int radius);
	//绘制四方区域 (left,top), (right, bottom)
	void DrawRectangle(int x1, int y1, int x2, int y2);
	/********************************************************************************
	 * 函数功能 : 绘制特殊符号, 如上下箭头等
	 * 函数参数 : int x, int y - 原点 int mode  - 符号掩码 CROSS等
	 *			  int size - 偏移长度
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void DrawMarker(int x, int y, int mode, int size = 6,CString strText = "");
	void DrawMarker( CPoint ptStart, CPoint ptEnd, ColorProperty& crColor );
	/********************************************************************************
	 * 函数功能 : 浮点数格式化输出到字府串
	 * 函数参数 : int decimal - 小数位数 
	 *		      CString str, 保存字府串 
	 *			  float value 数据
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void Format(int decimal, CString& str, double value);
	//文本输出特殊样式 浮点数 , 带边框
	enum { PrintString_Float = 0x01,PrintString_Rect = 0x02 };
	/********************************************************************************
	 * 函数功能 : 向屏幕输出文本
	 * 函数参数 : int x, int y - 输出位置 
	 *			  int theta, 半径
	 *			  const char* fmt, 文本
     *			  UINT nFormat 对齐
	 *			  short sType 特殊样式
	 *			  CFont* pCurFont 字体
	 *  返回值  : void
	 * 调用依赖 : 常用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void PrintString(int x, int y, int theta, const char* fmt,
					 UINT nFormat = DT_SINGLELINE|DT_END_ELLIPSIS|DT_WORDBREAK,
					 short sType = PrintString_Float,
					 CFont* pCurFont = NULL);
	//通过位置取得时间显示字府串 样式%02d:%02d
	void GetFenShiTimeStr(int nPos,CString& strBuf,BOOL bLast = FALSE);
	void GetHighFrequencyTimeStr(int nPos,CString& strBuf,BOOL bLast = FALSE);
	//分页面,绘制刻度
	void Ticks();
	//绘制纵向刻度
	void RightYTick();
	
public:
	/********************************************************************************
	 * 函数功能 : 画线
	 * 函数参数 : int x1, int y1, int x2 ,int y2,- 线两点
	 *			  char bUser 是否使用自定义直线画法,此画法为:使用点,连接成连,
	 *			  ColorProperty* pColor 线属性指针 非自定义画法有効
	 *  返回值  : void
	 * 调用依赖 : 常用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void DrawLine(int x1, int y1, int x2, int y2,char bUser = FALSE,ColorProperty* pColor = NULL);
protected:
	//画线
	void DrawLine(POINT& pt1, POINT& pt2,char bUser = FALSE,ColorProperty* pColor = NULL)	{ DrawLine(pt1.x, pt1.y, pt2.x, pt2.y,bUser,pColor); }
	//画圆
	void DrawCircle(POINT& pt, int radius)					{ DrawCircle(pt.x, pt.y, radius); }
	//填充圆
	void DrawFilledCircle(POINT& pt, int radius)			{ DrawFilledCircle(pt.x, pt.y, radius); }
	//画矩形
	void DrawRectangle(POINT& pt1, POINT& pt2)				{ DrawRectangle(pt1.x, pt1.y, pt2.x, pt2.y); }
	//画特殊符号
	void DrawMarker(POINT& pt, int mode, int size = 6)		{ DrawMarker(pt.x, pt.y, mode, size); }
	//画字府串
	void PrintString(POINT& pt, int theta, const char* fmt)	{ PrintString(pt.x, pt.y, theta, fmt); }
  //画交叉线
  //ADD BY YUHJ 2008.5.14
  void DrawCross(POINT &pt, int radius);
  //END
protected:
	//各文字对应字体大小,此处定义文字分类
	enum    FontSizeType { ftTitle,ftXAxisTitle,ftYAxisTitle,ftLegend,
						   ftXAxis,ftYAxis,ftFenShiYAxis,ftAxes,
						   ftRightYTick,ftFenShiRightYTick,ftMulti,
						   ftDrawVarTitle,ftDrawMaxMin };
	//按照文字分类设置字体大小
	int		GetFontSize(char cType);
	//按照文字分类取得字体
	CFont*	GetFont(char cType);
	//用于保存前一左右最大边界区域宽度
	int		m_nPreLeftMaxMargin;
	int		m_nPreRightMaxMargin;
public:
	/********************************************************************************
	 * 函数功能 : 返回左边坐标文本最大宽度, 右边坐标文本最大宽度GetYRightMaxWidth
	 * 函数参数 : CHSBaseDC* pDC -  绘图设备指针 非NULL
	 *			  BOOL bOneCharWidth - 是否计算字符宽度
	 *  返回值  : 最大宽度
	 * 调用依赖 : 被CalcLeftRightWidth调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	int		GetYLeftMaxWidth(CHSBaseDC* pDC,BOOL bOneCharWidth = FALSE);
	int		GetYRightMaxWidth(CHSBaseDC* pDC);
	/********************************************************************************
	 * 函数功能 : 重新调整左右边界坐标
	 * 函数参数 : CHSBaseDC* pDC -  绘图设备指针 非NULL
	 *  返回值  : void
	 * 调用依赖 : 调用了GetYLeftMaxWidth,GetYRightMaxWidth
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void    CalcLeftRightWidth(CHSBaseDC* pDC);
	/********************************************************************************
	 * 函数功能 : 根据数据返回相应的扩大倍数字府串描述 例如"Ⅹ10"
	 * 函数参数 : int nValue - 扩大倍数
	 *  返回值  : CString , 扩大倍数字府串描述,失败返回"Over"
	 * 调用依赖 : 在日线右坐标通常回有此类文本出现
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	CString ConvertIntToStr(int nValue);
	/********************************************************************************
	 * 函数功能 : 刷新, 在此类函数中重新调整了Size, 未重画
	 * 函数参数 : CDC* pDC  - 绘图设备指针 WPARAM wp = 0,LPARAM lp = 0 - 备用
	 *  返回值  : void
	 * 调用依赖 : 用于颜色设置改变时候刷新
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	void	Refresh(CDC* pDC,WPARAM wp = 0,LPARAM lp = 0);
	//响应字体改变后对应的区域改变
	void	SetSizeByFont(CDC* pDC);
public:
	// 绘制星星群
	virtual void DrawStars(DWORD linetype,
		CTValues<DrawPointInfo> pRefX,
		CTValues<StarFieldItem> pRefValues,
		ColorProperty& clLine,
		COLORREF cr = 0, int Index = 0, const char* Name = NULL);
public:
	//串行化
	virtual void	Serialize(CArchive& ar);
public:
	//	成本分布云相关结构
	struct MoveCostParam
	{
		char		  cPos;		//分类索引
		COLORREF	  m_lColor; //主色
		//根据分类索引, 分配颜色
		void SetColor(BYTE r,BYTE g,BYTE b);
	};
	//成本分布颜色相关参数
	struct MoveCostParamArray
	{
		ColorProperty lColor;							//主色
		CArray<MoveCostParam*,MoveCostParam*> ayParam;	//子类颜色
		~MoveCostParamArray()
		{
			for(int i = 0; i < ayParam.GetSize(); i++)
			{
				delete ayParam.GetAt(i);
			}
		}
	};
	//成本分别需要使用的相关数值
	struct MoveCostDetail
	{
		int		nData;				//日期，成本分布日期
		float	fInterestRatio;		//获利盘 百分比小数值, 套牢盘应为:1-fInterestRatio
		float	fInterestPan;		//指示获利价位位置(下标)
		float	fEvenCost;			//预留
		float	fPercent90Up;		//预留
		float	fPercent90Down;		//预留
		float	fPercent90Focus;	//预留	
		float	fPercent70Up;		//预留
		float	fPercent70Down;		//预留
		float	fPercent70Focus;	//预留	
		float   fHYS[MOVECOSTCOUNT];	//火焰山成本占总成本的比值
		float	fHYD[MOVECOSTCOUNT];    //活跃度成本占总成本的比值

		MoveCostDetail()
		{
			nData = 0;
			fInterestRatio = 0;
			fInterestPan = 0;
			fEvenCost = 0;
			fPercent90Up = 0;
			fPercent90Down = 0;
			fPercent90Focus = 0;
			fPercent70Up = 0;
			fPercent70Down = 0;
			fPercent70Focus = 0;

			for( int i = 0; i < MOVECOSTCOUNT; i++ )
			{
				fHYS[i] = 0;
				fHYD[i] = 0;
			}
		}
		/*将数据全部清零 
		why not use : memset(this, 0, sizeof(this));? by xiongzb 
		*/
		void DeleteAll()
		{
			nData = 0;
			fInterestRatio = 0;
			fInterestPan = 0;
			fEvenCost = 0;
			fPercent90Up = 0;
			fPercent90Down = 0;
			fPercent90Focus = 0;
			fPercent70Up = 0;
			fPercent70Down = 0;
			fPercent70Focus = 0;

			for( int i = 0; i < MOVECOSTCOUNT; i++ )
			{
				fHYS[i] = 0;
				fHYD[i] = 0;
			}
		}
	};
	//
	struct HxhMoveCostData
	{
		HxhMoveCostData();
		int	  m_nCalcType;					//用于记录是三角还是平均:1是三角，2是平均
		float m_fReduce;					//用于记录历史衰减系数
		float m_fFocus;						//集中度
		BOOL  m_bRefine;					//精细计算
		float m_fDefaultHand;				//没有流通盘是的的换手率
		int   m_ayMoveCost[MOVECOSTCOUNT];  //移动成本的显示情况
		int   m_ayHouYanShan[MOVECOSTCOUNT]; //火焰山的显示情况
		int   m_ayHuoYueDu[MOVECOSTCOUNT];	 //活跃度的显示情况
		int	  m_nHYSCount;					 //火焰山个数
		int   m_nHYDCount;					 //活跃度个数

		static int Load();	//从配置文件加载
		static int Save();	//保存到配置文件
	};
	/********************************************************************************
	 * 函数功能 : 绘制成本布云等
	 * 函数参数 : CGraphics::MoveCostConfig& moveCost  - 配置
	 *		      MoveCostParamArray& ayParam, - 参数配置
	 *			  CRect& clipBox, - 剪裁区域
	 *			  AnalisysObjData* pAnalisysData, 分析数据指针
	 *			  int nCurPos - 当前数据位置
	 *  返回值  : 成功返回1, 失败返回0
	 * 调用依赖 : 初始绘制
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	int  PaintMoveCost(CGraphics::MoveCostConfig& moveCost,MoveCostParamArray& ayParam, 
		CRect& clipBox,AnalisysObjData* pAnalisysData,int nCurPos);
	/********************************************************************************
	 * 函数功能 : 刷新成本分布等
	 * 函数参数 : AnalisysObjData* pAnalisysData - 分析数据指针
	 *			 ,CGraphics::MoveCostConfig& moveCost, 配置
	 *			  int nStart, 数据起始位置 
	 *			  int nEnd, 数据终止位置
	 *			  int nCurPos - 当前数据位置
	 *  返回值  : 成功返回1, 失败返回0
	 * 调用依赖 : 刷新绘制
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	int  CalcMoveCost( AnalisysObjData* pAnalisysData,CGraphics::MoveCostConfig& moveCost,
					  int nStart, int nEnd, int nCurPos );

	// 是否为主图，并且其中是否包含K线（后面再加）
	BOOL IsMainChart();

public:
	virtual POINT GetPoint(double x, double y);
	POINT GetPoint_LogY(double x, double y);
};
/********************************************************************************
 * 函数功能 : 设置成本分布云参数
 * 函数参数 : void
 *  返回值  : void
 * 调用依赖 : 对应:设置对话框OK, 系统设置改变导致的刷新
 * 作    者 :俞礼石
 * 完成日期 :2003-4-23
 * 修改记录 : 
 *******************************************************************************/
extern void SetMoveCostParam();
/********************************************************************************
 * 函数功能 : 返回共享数据结构指针
 * 函数参数 : void
 *  返回值  : CGraphics::HxhMoveCostData* 
 * 调用依赖 : 
 * 作    者 :俞礼石
 * 完成日期 :2003-4-23
 * 修改记录 : 
 *******************************************************************************/
extern CGraphics::HxhMoveCostData* GetHxhMoveData();

