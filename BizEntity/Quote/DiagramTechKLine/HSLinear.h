/*******************************************************************************
 * Copyright (c)2003, 杭州恒生信息技术有限公司
 * All rights reserved.
 *
 * 文件名称：HSLinear.h
 * 文件标识：绘制操作扩充类声明与实现
 * 摘    要：对绘制操作类CGraphics进行扩充
 *
 * 当前版本：2.0
 * 作    者：俞礼石
 * 完成日期：
 *
 * 取代版本：
 * 原 作 者：俞礼石
 * 完成日期：
 * 备	 注：
 *******************************************************************************/
// HSLinear.h: interface for the CHSLinear class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YLSLINEAR_H__A1A53162_BFE4_4A25_91DF_7A59E2DA377B__INCLUDED_)
#define AFX_YLSLINEAR_H__A1A53162_BFE4_4A25_91DF_7A59E2DA377B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphics.h"

template <class T>
class CHSLinear : public CGraphics
{
public:
	CHSLinear() {}

	CHSLinear(CDrawBase* pThis):CGraphics(pThis) { }
	//绘制使用记录最大最小以及当前位置的结构
	struct DrawMaxMinParam
	{
		int nXPos;		//当前位置
		int nTop;		//最大
		int nBottom;	//最小
		DrawMaxMinParam()
		{
			memset(this,-1,sizeof(DrawMaxMinParam));
		}
		BOOL IsEmpty()
		{
			return ( (nXPos == -1) && (nTop == -1 || nBottom == -1) );
		}
	};
	/********************************************************************************
	 * 函数功能 : 设置刻度, m_Scale
	 * 函数参数 : double xmin, double ymin, double xmax, double ymax, 对应CScale结构
	 *  返回值  : 返回rue
	 * 调用依赖 : 初始化
	 * 作    者 :俞礼石
	 * 完成日期 :2003-4-23
	 * 修改记录 : 
	 *******************************************************************************/
	virtual bool  SetRange(double xmin, double ymin, double xmax, double ymax);
	/********************************************************************************
	 * 函数功能 : 计算屏幕绘图点
	 * 函数参数 : T x, T y　－　其它类型的数据坐标
	 *  返回值  : point - 屏幕绘图点
	 * 调用依赖 : 绘制或者判断鼠标区域时候调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	virtual POINT GetPoint(T x, T y);
	/********************************************************************************
	 * 函数功能 : GetPoint 的逆运算,从屏幕坐标计算数据坐标
	 * 函数参数 : POINT& pt - 屏幕坐标,引用未改变值, 
	 *			  T& x, T& y - 用于返回数据坐标
	 *  返回值  : void
	 * 调用依赖 : GetPoint 的逆运算
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void  GetValue(POINT& pt, T& x, T& y);
	//以下函数未使用
	void Lines(T* x, T* y, int n, COLORREF cr, int Index = 0, const char* Name = NULL, int LineWidth = 0, int LineStyle = PS_SOLID);
	void Markers(T* x, T* y, int n, COLORREF cr, int mode, int Index = 0, const char* Name = NULL, int size = 6);
	void Polygon(T* x, T* y, int n);
	void Polygon(CTPoint<T>* pt, int n);
	void Lines(CTPoint<T>* pt, int n, COLORREF cr, int Index = 0, const char* Name = NULL, int LineWidth = 0, int LineStyle = PS_SOLID);
	void Markers(CTPoint<T>* pt, int n, COLORREF cr, int mode, int Index = 0, const char* Name = NULL, int size = 6);
	bool FilledPolygon(CTPoint<T>* pt, int n);
	void FilledRectangle(T x1, T y1, T x2, T y2);
	bool FilledPolygon(T* x, T* y, int n);
	void FilledRectangle(CTPoint<T>&, CTPoint<T>&, COLORREF cr);
	bool FilledPolygon(CTPoint<T>*, int n, COLORREF cr);
	void FilledRectangle(T x1, T y1, T x2, T y2, COLORREF cr);
	bool FilledPolygon(T* x, T* y, int n, COLORREF cr);
	void FilledRectangle(CTPoint<T>& pt1, CTPoint<T>& pt2)	{ FilledRectangle(pt1.x, pt1.y, pt2.x, pt2.y); }
	POINT GetTPoint(CTPoint<T>& pt)							{ return GetPoint(pt.x, pt.y); }
	CTPoint<T> GetTValue(POINT& pt)							{ CTPoint<T> t; GetValue(pt, t.x, t.y); return t; }	

	/********************************************************************************
	 * 函数功能 : 移动到某点以及绘制一条直线到某点,点为数据点
	 * 函数参数 : T x,T y;CTPoint<T>& pt - 数据点
	 *  返回值  : void
	 * 调用依赖 : 绘制直线时候,组合使用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	void MoveTo(T x, T y)									{ GetPoint(x, y); m_pDC->MoveTo(m_Pt.x, m_Pt.y); }
	void LineTo(T x, T y)									{ GetPoint(x, y); m_pDC->LineTo(m_Pt.x, m_Pt.y); }
	void MoveTo(CTPoint<T>& pt)								{ MoveTo(pt.x, pt.y); }
	void LineTo(CTPoint<T>& pt)								{ LineTo(pt.x, pt.y); }
	/********************************************************************************
	 * 函数功能 : 绘制直线,连接两数据点
	 * 函数参数 : T x1, T y1, T x2, T y2 - 数据点:(x1,y1) (x2,y2)
	 *			  CTPoint<T>& pt1, CTPoint<T>& pt2 - 数据点
	 *  返回值  : void 
	 * 调用依赖 : 调用MoveTo函数
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	void Line(T x1, T y1, T x2, T y2)						{ MoveTo(x1, y1); LineTo(x2, y2); }
	void Line(CTPoint<T>& pt1, CTPoint<T>& pt2)				{ Line(pt1.x, pt1.y, pt2.x, pt2.y); }
	/********************************************************************************
	 * 函数功能 : 绘制矩形框
	 * 函数参数 : T x1, T y1, T x2, T y2 - 数据点:(x1,y1) (x2,y2)
	 *			  CTPoint<T>& pt1, CTPoint<T>& pt2 - 数据点
	 *  返回值  : void            
	 * 调用依赖 : 调用MoveTo函数  
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	void Rectangle(T x1, T y1, T x2, T y2)					{ MoveTo(x1, y1); LineTo(x2, y1); LineTo(x2, y2); LineTo(x1, y2); LineTo(x1, y1); }
	void Rectangle(CTPoint<T>& pt1, CTPoint<T>& pt2)		{ Rectangle(pt1.x, pt1.y, pt2.x, pt2.y); }
	/********************************************************************************
	 * 函数功能 : 绘制标识,例如信息地雷等
	 * 函数参数 : T x, T y; CTPoint<T>& pt - 数据点
	 *			  int mode  - 符号掩码 CROSS等
	 *			  int size - 偏移长度
	 *  返回值  : void
	 * 调用依赖 : 绘制最终调用基类的DrawMarker;除第一个函数有使用,其它几个都没有使用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	void Marker(T x, T y, int mode, int size = 6)			{ GetPoint(x, y); DrawMarker(m_Pt.x, m_Pt.y, mode, size); };
	void Marker(CTPoint<T>& pt, int mode, int size = 6)		{ if( !pt.IsMarker() ) return; Marker(pt.x, pt.y, mode, size); }
	void Marker(T x, T y, DWORD mode,int size = 6)
	{
		POINT point = GetPoint(x, y);
		CTPoint<T> pt = GetTValue(point);
		pt.m_dwMarker = mode;
		Marker(pt,size);
	}	
	void Marker(CTPoint<T>& pt,int size = 6)
	{
		if( !pt.IsMarker() ) 
			return;
		if( pt.IsMarker(CROSS) )     Marker(pt.x, pt.y, CROSS, size);
		if( pt.IsMarker(XCROSS) )    Marker(pt.x, pt.y, XCROSS, size);
		if( pt.IsMarker(STAR) )      Marker(pt.x, pt.y, STAR, size);
		if( pt.IsMarker(CIRCLE) )    Marker(pt.x, pt.y, CIRCLE, size);
		if( pt.IsMarker(TRIANGLE) )  Marker(pt.x, pt.y, TRIANGLE, size);
		if( pt.IsMarker(DIAMOND) )   Marker(pt.x, pt.y, DIAMOND, size);
		if( pt.IsMarker(SQUARE) )    Marker(pt.x, pt.y, SQUARE, size);
		if( pt.IsMarker(FCIRCLE) )   Marker(pt.x, pt.y, FCIRCLE, size);
		if( pt.IsMarker(FTRIANGLE) ) Marker(pt.x, pt.y, FTRIANGLE, size);
		if( pt.IsMarker(FDIAMOND) )  Marker(pt.x, pt.y, FDIAMOND, size);
		if( pt.IsMarker(FSQUARE) )   Marker(pt.x, pt.y, FSQUARE, size);
		if( pt.IsMarker(SELECTED) )  Marker(pt.x, pt.y, SELECTED, size);
	}
	

	/********************************************************************************
	 * 函数功能 : 绘制栅栏线,例如分时走势图的成交量图即为栅栏线
	 * 函数参数 : DWORD linetype - 线型
	 *			  CTValues<DrawPointInfo> pRefX - X方向的相对坐标,
	 *			  CTValues<DrawFenShiData> pRefPriceValues,CTValues<long> pRefPriceValues - 新价值,
	 *			  CTValues<long> pRefValues - 成交量值,
	 *			  long lMaxValue, long lMinValue - 最大最小值
	 *		      ColorProperty& crLineRiseColor - 阳线颜色属性 
	 *			  ColorProperty& crLineDownColor - 阴线颜色属性 ,
	 *			  COLORREF cr -, int Index = 0, const char* Name - 未使用	
	 *  返回值  : void
	 * 调用依赖 : 在CDrawTStockTrend::Draw中被调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void Bar(DWORD linetype,
		CTValues<DrawPointInfo> pRefX,
		CTValues<long> pRefPriceValues,
		CTValues<long> pRefValues,
		long lMaxValue, long lMinValue,
		ColorProperty& crLineRiseColor, ColorProperty& crLineDownColor,
		COLORREF cr = 0, int Index = 0, const char* Name = NULL);
	virtual void Bar(DWORD linetype,
		CTValues<DrawPointInfo> pRefX,
		CTValues<DrawFenShiData> pRefPriceValues,
		CTValues<long> pRefValues,
		long lMaxValue, long lMinValue,
		ColorProperty& crLineRiseColor, ColorProperty& crLineDownColor,
		COLORREF cr = 0, int Index = 0, const char* Name = NULL);
	/********************************************************************************
	 * 函数功能 : 绘制上下分割的栅栏线,例如买卖力道即为此线
	 * 函数参数 : DWORD linetype - 线型
     *			  CTValues<DrawPointInfo> pRefX - X方向的相对坐标,
	 *			  CTValues<long> pRefValues - 值,分正负,即为买卖,
     *			  long lMaxDiff - 最大值, 
	 *			  int nRefPos - 中线位置,
	 *		      ColorProperty& crLineRiseColor - 阳线颜色属性 
	 *			  ColorProperty& crLineDownColor - 阴线颜色属性 ,
	 *			  COLORREF cr -, int Index = 0, const char* Name - 未使用				  
	 *  返回值  : void
	 * 调用依赖 : 在CDrawTBuySellPower::Draw中有调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void BarUpDown(DWORD linetype,
		CTValues<DrawPointInfo> pRefX,
		CTValues<long> pRefValues,
		long lMaxDiff, int nRefPos,
		ColorProperty& crLineRiseColor, ColorProperty& crLineDownColor,
		COLORREF cr = 0, int Index = 0, const char* Name = NULL);
	/********************************************************************************
	 * 函数功能 : 绘制曲线,例如分时走势线等
	 * 函数参数 : DWORD linetype - 线的类型,
	 * 			  CTValues<DrawPointInfo> pRefX - X向相对坐标,
	 * 			  CTValues<long> pRefValues - Y向数据值,
	 * 			  long lMaxValue , long lMinValue, - 最大最小值
	 * 			  ColorProperty& clLine  - 线颜色属性
	 * 			  COLORREF cr - 非0表示需要绘制选中标识, 
	 *			  int Index = 0,const char* Name - 绘制扩展相关参数
	 *  返回值  : void
	 * 调用依赖 : 绘制曲线,扩展绘制:Legend()
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void DrawCurve(DWORD linetype,
		CTValues<DrawPointInfo> pRefX,
		CTValues<long> pRefValues,
		long lMaxValue, long lMinValue,
		ColorProperty& clLine,
		COLORREF cr = 0, int Index = 0,const char* Name = NULL);
	virtual void DrawCurve(DWORD linetype,
		CTValues<DrawPointInfo> pRefX,
		CTValues<DrawFenShiData> pRefValues,
		long lMaxValue, long lMinValue,
		ColorProperty& clLine,
		COLORREF cr = 0, int Index = 0,const char* Name = NULL);
	/********************************************************************************
	 * 函数功能 : 绘制Ｋ线,还有其它的主图指标
	 * 函数参数 : CKLineChartParam* pParam - K线参数指针 非NULL 包括指明什么类型线等
	 *			  CTValues<DrawPointInfo> pRefX - X向相对坐标,
	 *			  CTValues<StockDay> pRefValues - K线数据值,
	 *			  int nDistance -　横向线与线的间距,
	 *			  ColorProperty& crLineRiseColor - 阳线颜色属性, 
	 *			  ColorProperty& crLineDownColor - 阴线颜色属性,
	 *			  ColorProperty& crFillRiseColor - 阳线填充颜色属性, 
	 *			  ColorProperty& crFillDownColor - 阴线填充颜色属性,
	 *	          COLORREF cr - 未使用, 
	 *			  int Index - 未使用, 
	 *			  const char* Name - 未使用
	 *  返回值  : void
	 * 调用依赖 : 绘制K线,美国线等,包括最高最低指示
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void DrawKLine(CKLineChartParam* pParam,
		CTValues<DrawPointInfo> pRefX,
		CTValues<StockDay> pRefValues,
		int nDistance, //　横向线与线的间距
		ColorProperty& crLineRiseColor, ColorProperty& crLineDownColor,
		ColorProperty& crFillRiseColor, ColorProperty& crFillDownColor,
		COLORREF cr = 0, int Index = 0, const char* Name = NULL);

	/********************************************************************************
	 * 函数功能 : 绘制指标线
	 * 函数参数 : DWORD linetype - 线类型 CLineType
	 *			  CTValues<DrawPointInfo> pRefX - X向相对坐标,
	 *			  CTValues<HSDouble> pRefValues - K线数据值,
	 *			  int nDistance -　横向线与线的间距,
	 *			  ColorProperty& crLineRiseColor - 阳线颜色属性, 
	 *			  ColorProperty& crLineDownColor - 阴线颜色属性,
	 *			  ColorProperty& crFillRiseColor - 阳线填充颜色属性, 
	 *			  ColorProperty& crFillDownColor - 阴线填充颜色属性,
	 *	          COLORREF cr - 未使用, 
	 *			  int Index - 未使用, 
	 *			  const char* Name - 未使用
	 *  返回值  : void
	 * 调用依赖 : 绘制指标线
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void DrawTechLine(DWORD linetype,
		CTValues<DrawPointInfo> pRefX,
		CTValues<HSDouble> pRefValues,
		int nDistance, //　横向线与线的间距
		ColorProperty& crSingleLineColor,
		ColorProperty& crLineRiseColor, ColorProperty& crLineDownColor,
		ColorProperty& crFillRiseColor, ColorProperty& crFillDownColor,
		int nCurve,
		CDrawParam* pDrawParam = NULL);
		//COLORREF cr = 0, int Index = 0, const char* Name = NULL);

// 选取图表
public:
	/********************************************************************************
	 * 函数功能 : 绘制选中标志
	 * 函数参数 : int x,int y - 绘制点,
	 *			  int& nPreX - 前一个点位置,
	 *			  int nWidth - 宽高
	 *  返回值  : 成功返回TRUE, 失败返回FALSE
	 * 调用依赖 : 一般被Draw函数调用
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	virtual BOOL DrawSelMark(int x,int y,int& nPreX,int nWidth = 10);
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
	 * 函数功能 : 设置K线或者指标线选中标识
	 * 函数参数 : DWORD linetype - 线风格例如PKLINESTICK
	 *			  CPoint point - Specifies the x- and y-coordinate of the cursor
	 *			  CTValues<DrawPointInfo> pRefX - X向相对坐标,
	 *			  CTValues<StockDay> pRefValues - K线数据值,
	 *			  CTValues<HSDouble> pRefValues - 指标线数据
	 *			  int nDistance -　横向线与线的间距,
	 *			  int nSize - 未使用
	 *  返回值  : 返回K线选中索引,失败返回-1
	 * 调用依赖 : 调用了AddSelectedStyle等
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	virtual int SelectKLine(DWORD linetype,CPoint point,
					        CTValues<DrawPointInfo> pRefX,
							CTValues<StockDay> pRefValues,
						    int nDistance,int nSize = 4);
	virtual int  SelectTechLine(DWORD linetype,CPoint point,
							    CTValues<DrawPointInfo> pRefX,
								CTValues<HSDouble> pRefValues,
								CTValues<StockDay> pRefDayValues,
							    int nDistance,int nCurve,int nSize = 4,
								CLineProperty* pData = NULL,
								CDrawParam* pDrawParam = NULL);

protected:
	//未使用
	void MarkerLegend(COLORREF cr, int Index, const char* Name, int mode, int size);
//ADD BY YUHJ 2008.5.27
  int IsRiseOrFall(StockDay *pPrev, StockDay *pCur);
//END

public:
	void Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
		}
		else
		{
		}
	}
};


//////////////////////////////////////////////////////////////////////////////////
////// class CXLogYLinear
template <class T> 
class CXLogYLinear : public CHSLinear<T>
{
public:
	CXLogYLinear() { }
	CXLogYLinear(CDrawBase* pThis):CHSLinear<T>(pThis) { }

	virtual bool SetRange(double xmin, double ymin, double xmax, double ymax);
	virtual void XAxis();
	virtual void Grid();
	virtual POINT GetPoint(T x, T y);
	virtual void GetValue(POINT& pt, T& x, T& y);

};


//对数X轴类, 未使用
template <class T> 
class CXYLogLinear : public CHSLinear<T>
{
public:
	CXYLogLinear() { }
	CXYLogLinear(CDrawBase* pThis):CHSLinear<T>(pThis) { }

	virtual bool SetRange(double xmin, double ymin, double xmax, double ymax);
	virtual void YAxis();
	virtual void Grid();
	virtual POINT GetPoint(T x, T y);
	virtual void GetValue(POINT& pt, T& x, T& y);
};


//对数坐标,未使用
template <class T> 
class CXLogYLogLinear : public CHSLinear<T>
{
public:
	CXLogYLogLinear(CDrawBase* pThis):CHSLinear<T>(pThis) { }

	virtual bool SetRange(double xmin, double ymin, double xmax, double ymax);
	virtual void XAxis();
	virtual void YAxis();
	virtual void Grid();
	virtual POINT GetPoint(T x, T y);
	virtual void GetValue(POINT& pt, T& x, T& y);

};


///////////////////////////////////////////////////////////////////////////////
// CHSLinear operations
template<class T>
bool CHSLinear<T>::SetRange(double xmin, double ymin, double xmax, double ymax)
{
	//m_nAxesType		= XY;
    m_Scale.xmin	= xmin;
    m_Scale.ymin	= ymin;
    m_Scale.xmax	= xmax;
    m_Scale.ymax	= ymax;

	return true;
}

template<class T>
POINT CHSLinear<T>::GetPoint(T x, T y)
{
	return CGraphics::GetPoint(x,y);

	/*if(m_Scale.dx != 0)
	{
		m_Pt.x = (LONG)((x - m_Scale.xmin) / m_Scale.dx) + m_GL;
	}
	else
	{
		m_Pt.x = 0;
	}
	if(m_Scale.dy != 0)
	{
		m_Pt.y = (LONG)(m_GB - (y - m_Scale.ymin) / m_Scale.dy);
	}
	else
	{
		m_Pt.y = 0;
	}

	if(m_Pt.y <= m_GT)
		m_Pt.y = m_GT + 1;
	if(m_Pt.y >= m_GB)
		m_Pt.y = m_GB - 1;

	return m_Pt;*/
}

template<class T>
void CHSLinear<T>::GetValue(POINT& pt, T& x, T& y)
{               
    x = (T)(m_Scale.xmin + (pt.x - m_GL) * m_Scale.dx);
	y = (T)(m_Scale.ymin + (m_GB - pt.y) * m_Scale.dy);
}

template<class T>
void CHSLinear<T>::MarkerLegend(COLORREF cr, int Index, const char* Name, int mode, int size)
{
	m_LogFont.lfHeight = (int)(m_Size.cx / -25.0);
	if (m_LogFont.lfHeight > -10) 
		m_LogFont.lfHeight = -10;
	m_LogFont.lfWeight	   = 500;
	m_LogFont.lfOrientation= 0;
	m_LogFont.lfEscapement = 0;
	CFont font;
	if ( font.CreateFontIndirect(&m_LogFont) )
	{
		int n  = (m_Rect.right - m_GR) / 20 + 1;
		int xb = m_GR + 2 * n;
		int xe = xb + 4 * n;
		int y  = m_GT - 3 * Index * m_LogFont.lfHeight / 2;
		DrawMarker(m_GR + 4 * n, y, mode, size);
		CFont* hOldFont = m_pDC->SelectObject(&font);
		m_pDC->SetTextColor(cr); 
		SetStringAlign(LEFT, CENTER);
		PrintString(xe + n, y, 0, Name);
		m_pDC->SelectObject(hOldFont);
		font.DeleteObject();
	}
}

template<class T>
void CHSLinear<T>::Markers(T* x, T* y, int n, COLORREF cr, int mode, int Index, const char* Name, int size)
{
	switch(mode)
	{
		case CROSS:
		case XCROSS:
		case STAR:
		case CIRCLE:
		case TRIANGLE:
		case DIAMOND:
		case SQUARE:
		{
			CPen hPen;
			hPen.CreatePen(PS_SOLID, 0, cr);
			CPen* hOldPen  = m_pDC->SelectObject(&hPen);
			for(int i = 0; i < n; i ++)
				Marker(x[i], y[i], mode, size);
			if (m_bEnableLegend && Index > 0 && Name)
				MarkerLegend(cr, Index, Name, mode, size);
			m_pDC->SelectObject(hOldPen);
			hPen.DeleteObject();
			break;
		}
		case FCIRCLE:
		case FTRIANGLE:
		case FDIAMOND:
		case FSQUARE:
		{
			CBrush hBrush;
			hBrush.CreateSolidBrush(cr);
			CBrush* hBrold = m_pDC->SelectObject(&hBrush);
			for(int i = 0; i < n; i ++)
				Marker(x[i], y[i], mode, size);
			if (m_bEnableLegend && Index > 0 && Name)
				MarkerLegend(cr, Index, Name, mode, size);
			m_pDC->SelectObject(hBrold);
			hBrush.DeleteObject();
			break;
		}
	}
}

template<class T>
void CHSLinear<T>::Markers(CTPoint<T>* pt, int n, COLORREF cr, int mode, int Index, const char* Name, int size)
{
	switch(mode)
	{
		case CROSS:
		case XCROSS:
		case STAR:
		case CIRCLE:
		case TRIANGLE:
		case DIAMOND:
		case SQUARE:
		{
			CPen hPen;
			hPen.CreatePen(PS_SOLID, 1, cr);
			CPen* hOldPen  = m_pDC->SelectObject(&hPen);
			for(int i = 0; i < n; i ++)
				Marker(pt[i].x, pt[i].y, mode, size);
			if (m_bEnableLegend && Index > 0 && Name)
				MarkerLegend(cr, Index, Name, mode, size);
			m_pDC->SelectObject(hOldPen);
			hPen.DeleteObject();
			break;
		}
		case FCIRCLE:
		case FTRIANGLE:
		case FDIAMOND:
		case FSQUARE:
		{
			CBrush hBrush;
			hBrush.CreateSolidBrush(cr);
			CBrush* hBrold = m_pDC->SelectObject(&hBrush);
			for(int i = 0; i < n; i ++)
				Marker(pt[i].x, pt[i].y, mode, size);
			if (m_bEnableLegend && Index > 0 && Name)
				MarkerLegend(cr, Index, Name, mode, size);
			m_pDC->SelectObject(hBrold);
			hBrush.DeleteObject();
			break;
		}
	}
}

template<class T>
void CHSLinear<T>::FilledRectangle(CTPoint<T>& pt1, CTPoint<T>& pt2, COLORREF cr)
{
    CBrush hBrush;
	hBrush.CreateSolidBrush(cr);
    CBrush* hBrold = m_pDC->SelectObject(&hBrush);
	FilledRectangle(pt1, pt2);
	m_pDC->SelectObject(hBrold);
    hBrush.DeleteObject();
}

template<class T>
void CHSLinear<T>::Polygon(CTPoint<T>* pt, int n)
{
	MoveTo(pt[0].x, pt[0].y);
	for(int i = 1; i < n; i ++)
		LineTo(pt[i].x, pt[i].y);
	LineTo(pt[0].x, pt[0].y);
}

template<class T>
bool CHSLinear<T>::FilledPolygon(CTPoint<T>* pt, int n)
{
	POINT *p = new POINT[n];
	if (!p) return false;
	for(int i = 0; i < n; i ++)
		p[i] = GetTPoint(pt[i]);
	m_pDC->Polygon(p, n );
	delete []p;

	return true;
}

template<class T>
bool CHSLinear<T>::FilledPolygon(CTPoint<T>* pt, int n, COLORREF cr)
{
    CBrush hBrush;
	hBrush.CreateSolidBrush(cr);
    CBrush* hBrold = m_pDC->SelectObject(&hBrush);
	bool bl = FilledPolygon(pt, n);
	m_pDC->SelectObject(hBrold);
    hBrush.DeleteObject();
	return bl;
}

template<class T>
void CHSLinear<T>::Lines(CTPoint<T>* pt, int n, COLORREF cr, int Index, const char* Name, int LineWidth, int LineStyle)
{
	if (m_bPrinting)	LineWidth *= m_nPrintScale;
	CPen hPen;
	hPen.CreatePen(LineStyle, LineWidth, cr);
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	MoveTo(pt[0].x, pt[0].y);
	Marker(pt[0]);  // 绘制标签

	for(int i = 1; i < n; i ++)
	{
		Marker(pt[i]); // 绘制标签
		LineTo(pt[i].x, pt[i].y);
	}

	if (m_bEnableLegend && Index > 0 && Name)
		Legend(cr, Index, Name);

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
}

template<class T>
void CHSLinear<T>::BarUpDown(DWORD linetype,
						   CTValues<DrawPointInfo> pRefX,CTValues<long> pRefValues,
						   long lMaxDiff, int nRefPos,
						   ColorProperty& crLineRiseColor, ColorProperty& crLineDownColor,
						   COLORREF cr, int Index /*= 0*/, const char* Name /*= NULL*/)
{
	if( pRefValues.m_lSize < 0 )
	    return;

	CPen hRisePen;
	hRisePen.CreatePen(crLineRiseColor.m_nPenStyle, crLineRiseColor.m_nWidth, crLineRiseColor.m_lColor);
	CPen hDownPen;
	hDownPen.CreatePen(crLineDownColor.m_nPenStyle, crLineDownColor.m_nWidth, crLineDownColor.m_lColor);
	CPen* hOldPen  = m_pDC->SelectObject(&hRisePen);

	DrawPointInfo* pXPoint      = pRefX.m_pValue;
	long*		   pValues      = pRefValues.m_pValue;

	if(m_YGridTicks != 0)
	{
		nRefPos = m_GT + (int)(1.0 * m_YGridTicks/2 * (m_GB-m_GT) / m_YGridTicks); // 中间线
	}
	else
	{
		nRefPos = m_GT + (int)(1.0 * m_YGridTicks/2 * (m_GB-m_GT) / 2);
	}

	for (int i = 0; i < pRefValues.m_lSize; i++)
	{
		if(pValues[i] == hx_DefMinValues)
			continue;
		if(lMaxDiff == 0)
			break;

		if( !IsValue( &pXPoint[i].m_fCoordinate,NULL ) )
			continue;

		if ( pValues[i] != 0 )
		{
			//MoveTo(float(pXPoint[i]) - m_GL, nRefPos);
			m_pDC->MoveTo(float(pXPoint[i]), nRefPos);			
			int nEnd = nRefPos - (int)((float)m_PY * pValues[i] / lMaxDiff + 0.5);

			if (pValues[i] > 0)
			{
				m_pDC->SelectObject(&hRisePen);
				m_pDC->LineTo(float(pXPoint[i]), nEnd - 1);
				//LineTo(float(pXPoint[i]) - m_GL, nEnd - 1);
			}
			else if (pValues[i] < 0)
			{
				m_pDC->SelectObject(&hDownPen);
				m_pDC->LineTo(float(pXPoint[i]), nEnd + 1);
				//LineTo(float(pXPoint[i]) - m_GL, nEnd + 1);
			}
		}
	}
	m_pDC->SelectObject(hOldPen);
	hRisePen.DeleteObject();
	hDownPen.DeleteObject();

}

template<class T>
void CHSLinear<T>::Bar(DWORD linetype,
					 CTValues<DrawPointInfo> pRefX,
					 CTValues<DrawFenShiData> pRefPriceValues,
					 CTValues<long> pRefValues,
					 long lMaxValue, long lMinValue,
					 ColorProperty& crLineRiseColor, ColorProperty& crLineDownColor,
					 COLORREF cr /*= 0*/, int Index /*= 0*/, const char* Name /*= NULL*/)
{                
	if( lMaxValue <= lMinValue ||
		pRefValues.m_lSize <= 0 )
	    return;

	//if (m_bPrinting)
	//	LineWidth *= m_nPrintScale;
//	HPEN hPen	  = ::CreatePen(LineStyle, LineWidth, cr);
//	HPEN hOldPen  = (HPEN)::SelectObject(m_pDC, hPen);

	CPen hRisePen;
	hRisePen.CreatePen(crLineRiseColor.m_nPenStyle, crLineRiseColor.m_nWidth, 
		crLineRiseColor.m_lColor);
	CPen hDownPen;
	hDownPen.CreatePen(crLineDownColor.m_nPenStyle, crLineDownColor.m_nWidth,
		crLineDownColor.m_lColor);
	CPen* hOldPen  = m_pDC->SelectObject(&hRisePen);

	DrawPointInfo*  pXPoint      = pRefX.m_pValue;
	DrawFenShiData*	pPriceValues = pRefPriceValues.m_pValue;
	long*		    pValues      = pRefValues.m_pValue;

	BOOL bRise = TRUE;
	for (int i = 0; i < pRefValues.m_lSize; i++)
	{
		if( !IsValue( &pXPoint[i].m_fCoordinate,NULL ) )
			continue;

		if( pPriceValues != NULL &&
			i >= 1 )
		{
			if(pPriceValues[i].m_lValue >= pPriceValues[i-1].m_lValue)
			{
				if(!bRise)
        		{
        			m_pDC->SelectObject(&hRisePen);
        			bRise = TRUE;
        		}
			}
			else if(bRise)
			{
        		m_pDC->SelectObject(&hDownPen);
        		bRise = FALSE;
            }
		}

		if ( (pValues[i] - lMinValue) != 0 && (lMaxValue - lMinValue) != 0 )
		{
			MoveTo(float(pXPoint[i]) - m_GL, 1);
			LineTo(float(pXPoint[i]) - m_GL, 
				(int)((float)m_PY * 
				(pValues[i] - lMinValue) / (lMaxValue - lMinValue) + 0.5) - 1);
		}
	}
    
	m_pDC->SelectObject(hOldPen);
	hRisePen.DeleteObject();
	hDownPen.DeleteObject();
}

template<class T>
void CHSLinear<T>::Bar(DWORD linetype,
					 CTValues<DrawPointInfo> pRefX,CTValues<long> pRefPriceValues,
					 CTValues<long> pRefValues,
					 long lMaxValue, long lMinValue,
					 ColorProperty& crLineRiseColor, ColorProperty& crLineDownColor,
					 COLORREF cr /*= 0*/, int Index /*= 0*/, const char* Name /*= NULL*/)
{                
	if( lMaxValue <= lMinValue ||
		pRefValues.m_lSize <= 0 )
	    return;

	//if (m_bPrinting)
	//	LineWidth *= m_nPrintScale;
//	HPEN hPen	  = ::CreatePen(LineStyle, LineWidth, cr);
//	HPEN hOldPen  = (HPEN)::SelectObject(m_pDC, hPen);

	CPen hRisePen;
	hRisePen.CreatePen(crLineRiseColor.m_nPenStyle, crLineRiseColor.m_nWidth, crLineRiseColor.m_lColor);
	CPen hDownPen;
	hDownPen.CreatePen(crLineDownColor.m_nPenStyle, crLineDownColor.m_nWidth, crLineDownColor.m_lColor);
	CPen* hOldPen  = m_pDC->SelectObject(&hRisePen);

	DrawPointInfo* pXPoint      = pRefX.m_pValue;
	long*		   pPriceValues = pRefPriceValues.m_pValue;
	long*		   pValues      = pRefValues.m_pValue;

	BOOL bRise = TRUE;
	for (int i = 0; i < pRefValues.m_lSize; i++)
	{
		if( !IsValue( &pXPoint[i].m_fCoordinate,NULL ) )
			continue;

		if( pPriceValues != NULL &&
			i >= 1 )
		{
			if(pPriceValues[i] >= pPriceValues[i-1])
			{
				if(!bRise)
        		{
        			m_pDC->SelectObject(&hRisePen);
        			bRise = TRUE;
        		}
			}
			else if(bRise)
			{
        		m_pDC->SelectObject(&hDownPen);
        		bRise = FALSE;
            }
		}

		if ( (pValues[i] - lMinValue) != 0 && (lMaxValue - lMinValue) != 0 )
		{
			MoveTo(float(pXPoint[i]) - m_GL, 1);
			LineTo(float(pXPoint[i]) - m_GL, 
				(int)((float)m_PY * 
				(pValues[i] - lMinValue) / (lMaxValue - lMinValue) + 0.5) - 1);
		}
	}
    
	m_pDC->SelectObject(hOldPen);
	hRisePen.DeleteObject();
	hDownPen.DeleteObject();

}

template<class T>
void CHSLinear<T>::DrawCurve(DWORD linetype,
						   CTValues<DrawPointInfo> pRefX,CTValues<DrawFenShiData> pRefValues,
						   long lMaxValue, long lMinValue,
						   ColorProperty& clLine,
						   COLORREF cr /*= 0*/, int Index /*= 0*/,const char* Name /*= NULL*/)
{
	if(pRefValues.m_lSize <= 0)
	{
		return;
	}

	DrawPointInfo*		pXPoint = pRefX.m_pValue;
	DrawFenShiData*		pValues = pRefValues.m_pValue;

	if (pValues == NULL)
	{
		return;
	}
//	if (m_bPrinting)	LineWidth *= m_nPrintScale;
	CPen hPen;
	hPen.CreatePen(clLine.m_nPenStyle, clLine.m_nWidth, clLine.m_lColor);
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	int nPreX = 0;

	CPoint pt;
	int nFormerPos = -1;
	int nCurPos;
  long LastVal = (lMinValue + lMaxValue) >> 1;

	for (int i = 0; i < pRefValues.m_lSize; i++)
	{
		if(pValues[i].m_lValue == hx_DefMinValues)
		{
			nFormerPos = -1;
			continue;
		}
		if( (lMaxValue - lMinValue) == 0 )
			break;

		if( !IsValue( &pValues[i].m_lValue,NULL ) )
			continue;

		nCurPos = (int)((float)m_PY * (((double)(pValues[i].m_lValue ? pValues[i].m_lValue : LastVal) - (double)lMinValue) / ((double)lMaxValue - (double)lMinValue)) /*+ 0.5*/);		
		
		if ((i > 0) && (LastVal/*pValues[i-1].m_lValue*/ != hx_DefMinValues))
		{    		
			if (nFormerPos < 0)
			{
				nFormerPos = (int)((double)m_PY * 
					(((double)LastVal/*pValues[i - 1].m_lValue*/ - (double)lMinValue) / ((double)lMaxValue - (double)lMinValue)) /*+ 0.5*/);				
			}
			if(i == 1)
			{
				MoveTo(float(pXPoint[i-1]) - m_GL,nFormerPos);
			}
			LineTo(float(pXPoint[i]) - m_GL,nCurPos);

			// yulx delete
// 			if( pValues[i - 1].m_nMessageMask & SD_NEWS )
// 			{
// 				Marker(float(pXPoint[i]) - m_GL,m_GT, INFODATA, 10);
// 			}

			if( cr != 0 )
			{
				DrawSelMark(float(pXPoint[i]) - m_GL,nCurPos,nPreX,m_PX/10);
			}
		}
/*		if( pValues[i].m_nMessageMask & SD_NEWS )//yulx add
		{
			if (i < pRefValues.m_lSize - 1)
			{
				Marker(float(pXPoint[i + 1]) - m_GL,m_GT, INFODATA, 10);
			}
			else
				Marker(float(pXPoint[i]) + float(pXPoint[1]) - float(pXPoint[0]) - m_GL,m_GT, INFODATA, 10);
		}*/
		if(pValues[i].m_lValue)
			LastVal = pValues[i].m_lValue;

		nFormerPos = nCurPos;
	}
	
	if(linetype & CURVELINETOEND)
	{
		LineTo(m_GR, nCurPos);
	}

	if (m_bEnableLegend && Index > 0 && Name)
		Legend(cr, Index, Name);

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
}
//
template<class T>
void CHSLinear<T>::DrawCurve(DWORD linetype,
						   CTValues<DrawPointInfo> pRefX,CTValues<long> pRefValues,
						   long lMaxValue, long lMinValue,
						   ColorProperty& clLine,
						   COLORREF cr /*= 0*/, int Index /*= 0*/,const char* Name /*= NULL*/)
{
	if(pRefValues.m_lSize <= 0)
	{
		return;
	}

	DrawPointInfo* pXPoint = pRefX.m_pValue;
	long*		   pValues = pRefValues.m_pValue;

	if (pValues == NULL)
	{
		return;
	}
//	if (m_bPrinting)	LineWidth *= m_nPrintScale;
	CPen hPen;
	hPen.CreatePen(clLine.m_nPenStyle, clLine.m_nWidth, clLine.m_lColor);
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	CPoint pt;
	int nFormerPos = -1;
	int nCurPos;
  long lastVal = (lMinValue + lMaxValue) >> 1;
	for (int i = 0; i < pRefValues.m_lSize; i++)
	{
		if(pValues[i] == hx_DefMinValues)
		{
			nFormerPos = -1;
			continue;
		}
		if( (lMaxValue - lMinValue) == 0 )
			break;

		if( !IsValue( &pValues[i],NULL ) )
			continue;

		nCurPos = (int)((float)m_PY * (((double)(pValues[i] ? pValues[i] : lastVal) - (double)lMinValue) / ((double)lMaxValue - (double)lMinValue)) /*+ 0.5*/);		

		if ((i > 0) && (lastVal/*pValues[i-1]*/ != hx_DefMinValues))
		{    		
			if (nFormerPos < 0)
			{
				nFormerPos = (int)((double)m_PY * 
					(((double)lastVal/*pValues[i - 1]*/ - (double)lMinValue) / ((double)lMaxValue - (double)lMinValue)) /*+ 0.5*/);				
			}
			if(i == 1)
			{
				MoveTo(float(pXPoint[i-1]) - m_GL,nFormerPos);
			}
			LineTo(float(pXPoint[i]) - m_GL,nCurPos);
		}
		
    if(pValues[i])
      lastVal = pValues[i];

		nFormerPos = nCurPos;
	}
	
	if(linetype & CURVELINETOEND)
	{
		LineTo(m_GR, nCurPos);
	}

	if (m_bEnableLegend && Index > 0 && Name)
		Legend(cr, Index, Name);

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
}

//ADD BY YUHJ 2008.5.15
template<class T>
int CHSLinear<T>::IsRiseOrFall(StockDay *pPrev, StockDay *pCur)
{
  long lPrevClose = 0;

	//yangdl 2008.03.11 特殊处理价格
	BOOL bNegative = FALSE;

//#ifdef Support_XHX_Dll
//	bNegative = MakeNegativeIndex(pCurAnalisysData->m_Info.m_cCodeType);
//#endif

  if(pPrev)
   lPrevClose = Negative(pPrev->m_lClosePrice,bNegative);

	long	lClose;//   = m_pStock->m_lPrevClose;
  lClose = Negative(lPrevClose,bNegative); // m_lNO1
  if( lClose == 0 )
  {
	  lClose = pCur->m_lClosePrice;
  }

  long lData = pCur->m_lClosePrice ? (pCur->m_lClosePrice - lClose) * 100 : 0;  

  if(!lData)
    return 0;

  return lData > 0 ? 1 : -1;
}
//END

// 绘制Ｋ线
template<class T>
void CHSLinear<T>::DrawKLine(CKLineChartParam* pParam,
						   CTValues<DrawPointInfo> pRefX,
						   CTValues<StockDay> pRefValues,
						   int nDistance, //　横向线与线的间距
						   ColorProperty& crLineRiseColor, ColorProperty& crLineDownColor,
						   ColorProperty& crFillRiseColor, ColorProperty& crFillDownColor,
						   COLORREF cr /*= 0*/, int Index /*= 0*/, const char* Name /*= NULL*/)
{
	if(pRefValues.m_lSize <= 0 || pRefX.m_lSize <= 0 )
	{
		return;
	}
	int nEnd = pRefValues.m_lStart + pRefValues.m_lSize;
	if( (pRefValues.m_lStart >= nEnd) || (pRefValues.m_lStart < 0) )
		return;

	CPen RisePen(crLineRiseColor.m_nPenStyle,crLineRiseColor.m_nWidth,crLineRiseColor.m_lColor);
	CPen DownPen(crLineDownColor.m_nPenStyle,crLineDownColor.m_nWidth,crLineDownColor.m_lColor);
	
	CBrush RiseBrush(crFillRiseColor);
	CBrush DownBrush(crFillDownColor);
	
//	BOOL bRise = TRUE;
	CPen*	pOldPen	  = m_pDC->SelectObject(&RisePen);
	CBrush* pOldBrush = m_pDC->SelectObject(&RiseBrush);
	
	DrawPointInfo* pXPoint = pRefX.m_pValue;
	StockDay*	   pValues = pRefValues.m_pValue;
	CDispMaxMinValue*   pCurMin		    = NULL;
	CDispMaxMinValue*   pCurMax		    = NULL;
	CTechKLineImp* pPKLine = GetPKLine();
	if( pPKLine != NULL && !pPKLine->IsAddLine())
	{
		pCurMin	= GetMaxMin(FALSE);
		pCurMax	= GetMaxMin(TRUE);
	}
/*	CDragObjManager* pDragObjManager = NULL;	

	if( !(pParam->m_dwStyle & (ADDPKLINE | NOTDRAWMSGMARK)))//CHANGE BY YUHJ
	{
		pDragObjManager = GetDragObjManager();	
		pCurMin	= GetMaxMin(FALSE);
		pCurMax	= GetMaxMin(TRUE);
	}*/

	int nTop,nBottom;
	POINT pt1,pt2,pt3,pt4;
	CRect preRect,rect;
	int   k;
	int   nPreX = 0;
	
	DrawMaxMinParam min,max;
	//nDistance /= 2;

	//int nPreStartPos = -1;
	for (int i = pRefValues.m_lStart; i < nEnd; i++)
	{
		k = i - pRefValues.m_lStart;
		if( k < 0 )
		{
			continue;
		}

		if( !IsValue( &pValues[i].m_lDate,NULL) )
			continue;

		if( !IsValue( &pValues[i].m_lClosePrice,NULL) ) // 收盘价无效
			continue;

        nTop     = ValueToVertPos(pValues[i].m_lMaxPrice);
        nBottom  = ValueToVertPos(pValues[i].m_lMinPrice);
        
		rect.top    = ValueToVertPos(pValues[i].m_lOpenPrice);
	    rect.bottom = ValueToVertPos(pValues[i].m_lClosePrice);
	
    //ADD BY YUHJ 2008.7
		if( IsSelectedStyle(CHART_SELPKLINE) && !(pParam->m_dwStyle & NOTDRAWMSGMARK)) // 选中
		{
			DrawSelMark(float(pXPoint[k]) - m_GL,nTop,nPreX,m_PX/10);
		}

		// 转换
		pt1 = GetPoint(float(pXPoint[k]) - m_GL,nTop);
		pt2 = GetPoint(float(pXPoint[k]) - m_GL,nBottom);
		pt3 = GetPoint(float(pXPoint[k]) - m_GL - nDistance/2,rect.top);
		pt4 = GetPoint(float(pXPoint[k]) - m_GL + nDistance/2,rect.bottom);
		
		rect.SetRect(pt3.x,pt3.y,pt4.x,pt4.y);
		rect.NormalizeRect();

		if( preRect.left  == rect.left  && preRect.top    == rect.top &&
			preRect.right == rect.right && preRect.bottom == rect.bottom )
		{
			goto _CurMaxMin_;
		}
		preRect = rect;

		BOOL bUp = IsUp(i);

/*    if(pParam->IsType(IDR_hexinOXChart))
    {
      int d = rect.Width();

      POINT pt = GetPoint(float(pXPoint[k]) - m_GL,ValueToVertPos(m_YRefParam.max));

      while(pt.y < pt1.y && d > 0)		//modify by lxqi 20100113 加入d>0条件，防止死循环
        pt.y += d;

      int cnt = (int)((pt2.y - pt.y) / d) + 1;

      pt1.y = pt.y - d / 2;
      pt2.y = pt1.y + d * cnt;
      
      bUp = IsRiseOrFall(i ? pValues + (i - 1) : NULL, pValues + i) > 0 ? TRUE : FALSE;
    }*/

//END

	//modify by lxqi 20090507	为策略交易测试添加，判断是否画特殊线
	BOOL bIsSpecial = FALSE;
/*	if (pValues[i].m_lDate == g_hxData.m_nCurSelTrade)
	{
		bIsSpecial = TRUE;
	}*/
        //绘制k线，美国线，压缩线等图
		DrawPKByMark(  pParam, 
					   pValues[i],
					   NULL,
					   NULL,NULL,
					   //pValues[i].m_lOpenPrice <= pValues[i].m_lClosePrice,
					   bUp,  // gbq modify 20060327,该句改为上面这句,注意测试
					   i,pt1.x,pt1.y,pt2.y,rect,
					   &RisePen,&RiseBrush,&DownPen,&DownBrush,
					   bIsSpecial);		//add by lxqi 20090507

_CurMaxMin_:
		;
		if( pCurMin != NULL && pCurMin->IsFind(i) && min.IsEmpty() )
		{
			min.nXPos   = pt1.x - 3;
			min.nTop    = pt1.y + 6;
			min.nBottom = pt2.y + 6;

			DrawMaxMin(min.nXPos,min.nTop,min.nBottom,rect,pCurMin,NULL);

		}
		if( pCurMax != NULL && pCurMax->IsFind(i) && max.IsEmpty() )
		{
			max.nXPos   = pt1.x - 3;
			max.nTop    = pt1.y - 5;
			max.nBottom = pt2.y - 5;

			DrawMaxMin(max.nXPos,max.nTop,max.nBottom,rect,NULL,pCurMax);
		}
	}
	m_pDC->SelectObject(pOldPen);
	m_pDC->SelectObject(pOldBrush);
}

template<class T>
long CHSLinear<T>::IsSelected(DWORD linetype,CPoint& point,CRect& rect,
							   short nCurve,void* pMask,
							   CLineProperty* pData /*= NULL*/)
{
	POINT pt1 = GetPoint(rect.left, rect.top);
	POINT pt2 = GetPoint(rect.right, rect.bottom);

	if( rect.left   == -1 ) rect.left = -1;	  else rect.left   = pt1.x;
	if( rect.top    == -1 ) rect.top = -1;	  else rect.top    = pt1.y;
	if( rect.right  == -1 ) rect.right = -1;  else rect.right  = pt2.x;
	if( rect.bottom == -1 ) rect.bottom = -1; else rect.bottom = pt2.y;
	
	return CGraphics::IsSelected(linetype,point,rect,nCurve,pMask,pData);
}

template<class T>
BOOL CHSLinear<T>::DrawSelMark(int x,int y,int& nPreX,int nWidth /*= 10*/)
{
	if( (x - nPreX) > nWidth )
	{
		Marker(x, y, SELECTED);
		nPreX = x;
		return TRUE;
	}
	return FALSE;
}

template<class T>
int CHSLinear<T>::SelectKLine(DWORD linetype,CPoint point,
					        CTValues<DrawPointInfo> pRefX,CTValues<StockDay> pRefValues,
						    int nDistance,int nSize /*= 4*/)
{
//	if(!(linetype & NOTSELMARK))
//	{
//		RemoveSelectedStyle();
//	}

	if(pRefValues.m_lSize <= 0 || pRefX.m_lSize <= 0 )
	{
		return -1;
	}
	if( pRefValues.m_lStart < 0 )
	{
		return -1;
	}

	DrawPointInfo* pXPoint = pRefX.m_pValue;
	StockDay*	   pValues = pRefValues.m_pValue;

	CRect rect;
	long nRet;
	for (int i = pRefValues.m_lStart; i < pRefValues.m_lSize; i++)
	{
		if( !IsValue( &pValues[i].m_lDate,NULL) )
			continue;

		rect.left   = float(pXPoint[i]) - m_GL - nDistance/2;
		rect.right  = rect.left + nDistance;
        rect.top    = ValueToVertPos(pValues[i].m_lOpenPrice);
        rect.bottom = ValueToVertPos(pValues[i].m_lClosePrice);

		nRet = IsSelected(linetype,point,rect,nSize,&pValues[i]);
		if( nRet == BREAK )
		{
			return -1;
		}
		else if( nRet == FINDED )
		{
			if(!(linetype & NOTSELMARK))
			{
				AddSelectedStyle(CHART_SELPKLINE);
			}
			return i;
		}	
	}

	return -1;
}


template<class T>
void CHSLinear<T>::DrawTechLine(DWORD linetype,
							  CTValues<DrawPointInfo> pRefX,CTValues<HSDouble> pRefValues,
							  int nDistance, //　横向线与线的间距
							  ColorProperty& crSingleLineColor,
							  ColorProperty& crLineRiseColor, ColorProperty& crLineDownColor,
							  ColorProperty& crFillRiseColor, ColorProperty& crFillDownColor,
							  int nCurve,
							  CDrawParam* pDrawParam /* = NULL*/)
							  //COLORREF cr /*= 0*/, int Index /*= 0*/, const char* Name /*= NULL*/)
{	
	if( (linetype & NOTDRAWLINE) )
	{
		return;
	}

	//
	if(pRefValues.m_lSize <= 0 || pRefX.m_lSize <= 0 )
	{
		return;
	}
	int nEnd = pRefValues.m_lStart + pRefValues.m_lSize;
	if( (pRefValues.m_lStart >= nEnd) || (pRefValues.m_lStart < 0) )
		return;

	CPen SinglePen(crSingleLineColor.m_nPenStyle,crSingleLineColor.m_nWidth,crSingleLineColor.m_lColor);
	CPen RisePen(crLineRiseColor.m_nPenStyle,crLineRiseColor.m_nWidth,crLineRiseColor.m_lColor);
	CPen DownPen(crLineDownColor.m_nPenStyle,crLineDownColor.m_nWidth,crLineDownColor.m_lColor);
	
	CBrush RiseBrush(crFillRiseColor);
	CBrush DownBrush(crFillDownColor);
	
	BOOL bRise = TRUE;
	CPen*	pOldPen	  = m_pDC->SelectObject( ((linetype & VOLSTICK) || 
											  (linetype & COLORSTICK))?&RisePen:&SinglePen );
	CBrush* pOldBrush = NULL;
	if( ((linetype & VOLSTICK) || (linetype & COLORSTICK)) )
	{
		pOldBrush = m_pDC->SelectObject(&RiseBrush);
	}

	DrawPointInfo* pXPoint = pRefX.m_pValue;
	HSDouble*	   pValues = pRefValues.m_pValue;

	CRect preRect,rect;
	
	CPoint pt1;
	CPoint pt2;

	int nCurValue;
	int nPrePos = -1;
	int nPreX = 0;
	preRect.SetRectEmpty();
	BOOL bFirst = TRUE;
//	int  nPreStartPos = -1;
	for (int i = pRefValues.m_lStart; i < nEnd; i++)
	{
		if(::IsBadReadPtr(&pValues[i], sizeof(CTValues<HSDouble>)))
			break;
		if(pValues[i] == hx_DefMinValues)
		{
			nPrePos = -1; // by yls add 2007.08.30
			continue;
		}
		
		if( !IsValue( &pXPoint[i].m_fCoordinate,NULL ) )
			continue;

		rect.left   = float(pXPoint[i]) - m_GL - nDistance/2;
		rect.right  = rect.left + nDistance;

		if( pDrawParam && nCurve )
		{
			rect.OffsetRect(pDrawParam->m_nSingleWidth*nCurve,0);
		}

//		if( nDistance == 0 )
//		{
//			if( rect.left == rect.left && rect.left == nPreStartPos )
//			{
//				continue;
///			}
//			nPreStartPos = rect.left;
//		}

		nCurValue   = rect.top = ValueToVertPos(pValues[i]);
        rect.bottom = ValueToVertPos(0);
		rect.NormalizeRect();
		rect.bottom++;
		if( preRect.left  == rect.left  && preRect.top    == rect.top &&
			preRect.right == rect.right && preRect.bottom == rect.bottom )
		{
			nPrePos = nCurValue;
			goto _DrawSelMark_;
		}
		
		if ( (linetype & USERDRAWLINE) )
		{
			if( pValues[i].m_pOtherData != NULL )
			{
				HSDrawOtherData* pOtherData = (HSDrawOtherData*)pValues[i].m_pOtherData;
				if( pOtherData->m_bType1 ==	HS_DRAWLINE )
				{
					short nDist = MAKEWORD(pOtherData->m_bType2,pOtherData->m_bType3); // 间隔点数
					if( (i + nDist) < nEnd /*&& i < nDist*/ )
					{ 
						pt1 = GetPoint(float(pXPoint[i]) - m_GL, nCurValue);
						pt2 = GetPoint(float(pXPoint[i + nDist]) - m_GL, ValueToVertPos(pOtherData->m_dValue) );
					}
					else
					{
						continue;
					}
				}
				else if( pOtherData->m_bType1 == HS_DRAWSL )
				{
					short nDist = MAKEWORD(pOtherData->m_bType2,pOtherData->m_bType3); // 间隔点数
					if( (i + nDist) < nEnd )
					{ 
						pt1 = GetPoint(float(pXPoint[i]) - m_GL, nCurValue);
						pt2 = GetPoint(float(pXPoint[i + nDist]) - m_GL, nCurValue );
					}
					else
					{
						continue;
					}
				}
				else if( pOtherData->m_bType1 == HS_STICKLINE )
				{
					pt1 = GetPoint(float(pXPoint[i]) - m_GL, nCurValue);
					pt2 = GetPoint(float(pXPoint[i]) - m_GL, ValueToVertPos(pOtherData->m_dValue) );
				}
				else if( pOtherData->m_bType1 == HS_POLYLINE )
				{
					pt1 = GetPoint(float(pXPoint[i]) - m_GL, nCurValue);
					if( bFirst )
					{
						pOtherData->m_bType2 = 1;
						bFirst = FALSE;
					}
				}
				else 
				{					
					pt2 = GetPoint(float(pXPoint[i]) - m_GL, nCurValue);
					if( nPrePos != -1 && i > 0 )
					{
						pt1 = GetPoint(float(pXPoint[i - 1]) - m_GL, nPrePos);						
					}
					else
					{
						pt1 = pt2;
					}

					nPrePos = nCurValue;
				}
				DrawAdvance(linetype,nCurve,crSingleLineColor,
							pt1,pt2,pValues[i],pOtherData,i,nDistance);
			}
		}
		else if( (linetype & VOLSTICK) || (linetype & COLORSTICK) )
		{
			if( ( (linetype & COLORSTICK) && pValues[i] >= 0) || 
				( (linetype & VOLSTICK) && IsUp((pRefX).m_lStart+i) ) )
			{
				if(!bRise)
				{
					m_pDC->SelectObject(&RisePen);
					m_pDC->SelectObject(&RiseBrush);
					bRise = TRUE;
				}
			}
			else if( bRise )
			{
				m_pDC->SelectObject(&DownPen);
				m_pDC->SelectObject(&DownBrush);
				bRise = FALSE;
			}
			
			if(linetype & VOLSTICK) // 成交量
			{
				pt1 = GetPoint(rect.left,rect.top);
				pt2 = GetPoint(rect.right,rect.bottom);
				if(rect.IsRectEmpty())
				{
					m_pDC->MoveTo(pt1.x,pt1.y);
					m_pDC->LineTo(pt2.x, pt1.y); 
					m_pDC->LineTo(pt2.x,pt2.y); 
					m_pDC->LineTo(pt1.x, pt2.y); 
					m_pDC->LineTo(pt1.x,pt1.y);
					//Rectangle(rect.left,rect.top,rect.right,rect.bottom);
				}
				else
				{
					if( pt1.y == pt2.y )
					{
						m_pDC->MoveTo(pt1.x, pt1.y-1);
					    m_pDC->LineTo(pt2.x, pt2.y-1); 
					}
					else
					{
					    m_pDC->Rectangle(pt1.x,pt1.y,pt2.x,pt2.y);
					}
					//FilledRectangle(rect.left,rect.top,rect.right,rect.bottom);
				}
			}		
			else if(linetype & COLORSTICK) // 
			{
				pt1 = GetPoint(float(pXPoint[i]) - m_GL, rect.top);
				pt2 = GetPoint(float(pXPoint[i]) - m_GL, rect.bottom);
				m_pDC->MoveTo(pt1.x,pt1.y);
				m_pDC->LineTo(pt2.x,pt2.y);
			}
		}
		else if( linetype & DRAW3D ) //画3D
		{

			/*if( IsUp((pRefX).m_lStart+i) )
			{
				if(!bRise)
				{
					bRise = TRUE;
				}
			}
			else if( bRise )
			{
				bRise = FALSE;
			}*/

			rect.NormalizeRect();
			pt1 = GetPoint(rect.left,rect.bottom);
			pt2 = GetPoint(rect.right,rect.top);
			if(rect.IsRectEmpty())
			{
				m_pDC->MoveTo(pt1.x,pt1.y);
				m_pDC->LineTo(pt2.x, pt1.y); 
				m_pDC->LineTo(pt2.x,pt2.y); 
				m_pDC->LineTo(pt1.x, pt2.y); 
				m_pDC->LineTo(pt1.x,pt1.y);

				pt1.Offset(-2,-2);
				pt2.Offset(-2,-2);
				m_pDC->MoveTo(pt1.x,pt1.y);
				m_pDC->LineTo(pt2.x, pt1.y); 
				m_pDC->LineTo(pt2.x,pt2.y); 
				m_pDC->LineTo(pt1.x, pt2.y); 
				m_pDC->LineTo(pt1.x,pt1.y);
				//Rectangle(rect.left,rect.top,rect.right,rect.bottom);
			}
			else
			{
				if( pt1.y == pt2.y )
				{
					m_pDC->MoveTo(pt1.x, pt1.y-1);
					m_pDC->LineTo(pt2.x, pt2.y-1); 
				}
				else
				{
					//Draw3DStick(pt1,pt2,crSingleLineColor);
					
					DrawMarker(pt1,pt2,crSingleLineColor);

					/*if( bRise )
					{
						DrawMarker(pt1,pt2,crLineRiseColor);
					}
					else
					{
						DrawMarker(pt1,pt2,crLineDownColor);
					}*/
				}
			}
		}
		else if(linetype & LINESTICK) // 
		{
			pt1 = GetPoint(float(pXPoint[i]) - m_GL, rect.top);
			pt2 = GetPoint(float(pXPoint[i]) - m_GL, rect.bottom);
			m_pDC->MoveTo(pt1.x,pt1.y);
			m_pDC->LineTo(pt2.x,pt2.y);
			if(nPrePos != -1 && i > 0)
			{
				MoveTo(float(pXPoint[i - 1]) - m_GL, nPrePos);
				LineTo(float(pXPoint[i]) - m_GL, nCurValue);
			}
			nPrePos = nCurValue;
		}
		else if(linetype & STICK) // 
		{
			pt1 = GetPoint(float(pXPoint[i]) - m_GL, nCurValue);
			pt2 = GetPoint(float(pXPoint[i]) - m_GL, ValueToVertPos(0) );
			m_pDC->MoveTo(pt1.x,pt1.y);
			m_pDC->LineTo(pt2.x,pt2.y);
		}
		else
		{			
			if( nPrePos != -1 && i > 0 )
			{
				if(linetype & CROSSDOT) // 
				{
					pt1 = GetPoint(float(pXPoint[i - 1]) - m_GL, nPrePos);
					DrawMarker(pt1.x,pt1.y, CROSS,4);
				}
				else if(linetype & CIRCLEDOT) // 
				{
					pt1 = GetPoint(float(pXPoint[i - 1]) - m_GL, nPrePos);
					DrawMarker(pt1.x,pt1.y, CIRCLE,nDistance);//3);
				}
				else if(linetype & POINTDOT) // 
				{
					pt1 = GetPoint(float(pXPoint[i - 1]) - m_GL, nPrePos);
					DrawMarker(pt1.x,pt1.y, FCIRCLE,1);
				}
				else if( linetype & LINEDRAW3D ) //画3D
				{
					pt1 = GetPoint(float(pXPoint[i - 1]) - m_GL, nPrePos);
					pt2 = GetPoint(float(pXPoint[i]) - m_GL, nCurValue);
				
					if( pt1.y == pt2.y )
					{
						m_pDC->MoveTo(pt1.x, pt1.y-1);
						m_pDC->LineTo(pt2.x, pt2.y-1); 
					}
					else
					{
						DrawMarker(pt1,pt2,crSingleLineColor);
					}

					//m_pDC->MoveTo(pt1.x,pt1.y);
					//m_pDC->LineTo(pt2.x,pt2.y);
				}
				else
				{
					pt1 = GetPoint(float(pXPoint[i - 1]) - m_GL, nPrePos);
					pt2 = GetPoint(float(pXPoint[i]) - m_GL, nCurValue);
					m_pDC->MoveTo(pt1.x,pt1.y);
					m_pDC->LineTo(pt2.x,pt2.y);
				}
			}
			nPrePos = nCurValue;
		}
		preRect = rect;
		
_DrawSelMark_:
		if( IsSelectedStyle(nCurve) ) // 选中
		{
			DrawSelMark(rect.left,nCurValue,nPreX,m_PX/10);
		}
	}

	if(linetype & DRAWZEROLINE) // 绘制零点横线
	{
		CPen zeroPen(crSingleLineColor.m_nPenStyle,
			crSingleLineColor.m_nWidth,crSingleLineColor.m_lColor);
		m_pDC->SelectObject(&zeroPen);
		MoveTo(0, ValueToVertPos(0));
		LineTo(m_GR, ValueToVertPos(0));
	}

	m_pDC->SelectObject(pOldPen);
	if(pOldBrush != NULL)
	{
		m_pDC->SelectObject(pOldBrush);
	}
}

template<class T>
int CHSLinear<T>::SelectTechLine(DWORD linetype,CPoint point,
							  CTValues<DrawPointInfo> pRefX,
							  CTValues<HSDouble> pRefValues,
							  CTValues<StockDay> pRefDayValues,
							  int nDistance,int nCurve,int nSize /*= 4*/,
							  CLineProperty* pData /*= NULL*/,
							  CDrawParam* pDrawParam /*= NULL*/)
{
	if( (linetype & NOTDRAWLINE) )
	{
		return -1;
	}

//	if( !(linetype & NOTSELMARK) )
//	{
//		RemoveSelectedStyle();
//	}
	
	if( pRefValues.m_lSize <= 0 || pRefX.m_lSize <= 0 )
	{
		return -1;
	}

	if( pRefValues.m_lStart < 0 )
		return -1;

	DrawPointInfo* pXPoint = pRefX.m_pValue;
	HSDouble*	   pValues = pRefValues.m_pValue;

	StockDay*	   pDayValues = pRefDayValues.m_pValue;

	CRect rect;
	int   nCurValue;
	int   nPrePos = -1;
	long  nRet;
	//int   k;

	for (int i = pRefValues.m_lStart; i < pRefValues.m_lSize; i++)
	{
		if(pValues[i] == hx_DefMinValues)
		{
			nPrePos = -1;
			continue;
		}

		if( !this->IsValue( &pXPoint[i].m_fCoordinate,NULL ) )
			continue;

		// 交易系统、选股指示、五彩k线
		if( linetype & SELECTMARK )
		{
			if( pDayValues == NULL )
				break;
		
		/*	k = i - pRefValues.m_lStart;
			if( k < 0 )
				continue;*/

			if( !pValues[i].m_dData )
				continue;
		
			rect.left    = float(pXPoint[i]) - m_GL - nDistance/2;
			rect.right   = rect.left + nDistance;
			rect.top     = ValueToVertPos(pDayValues[i].m_lMaxPrice);
			rect.bottom  = ValueToVertPos(pDayValues[i].m_lMinPrice);

			nRet = IsSelected(linetype,point,rect,nCurve,0,pData);
			if( nRet == BREAK )
			{
				return -1;
			}
			else if( nRet == FINDED )
			{
				if(!(linetype & NOTSELMARK))
				{
					AddSelectedStyle(nCurve);
				}
				return i;
			}

			continue;
		}

		rect.left   = float(pXPoint[i]) - m_GL - nDistance/2;
		rect.right  = rect.left + nDistance;
		nCurValue   = rect.top = ValueToVertPos(pValues[i]);
        rect.bottom = ValueToVertPos(0);

		if( (linetype & VOLSTICK) || (linetype & DRAW3D) ) // 成交量
		{
			if(rect.bottom < 0)
			{
				rect.bottom = 0;
			}

			if( pDrawParam && nCurve && linetype & DRAW3D)
			{
				rect.OffsetRect(pDrawParam->m_nSingleWidth*nCurve,0);
	/*			rect.NormalizeRect();

				CPoint ptStart	= GetPoint(rect.left,rect.bottom);
				CPoint ptEnd	= GetPoint(rect.right,rect.top);	
				rect = 	CRect(ptStart,ptEnd);
	*/		}			
		}		
		else if(linetype & COLORSTICK) // 
		{
			rect.left  = float(pXPoint[i]) - m_GL;
			rect.right = float(pXPoint[i]) - m_GL;
		}
		else if(linetype & LINESTICK) // 
		{
			rect.left   = float(pXPoint[i]) - m_GL;
			rect.right  = float(pXPoint[i]) - m_GL;

			nRet = IsSelected(linetype,point,rect,nCurve,0);
			if( nRet == BREAK )
			{
				return -1;
			}
			else if( nRet == FINDED )
			{
				if(!(linetype & NOTSELMARK))
				{
					AddSelectedStyle(nCurve);
				}
				return i;
			}

			if(nPrePos != -1 && i > 0) // 曲线
			{
				rect.left   = float(pXPoint[i - 1]) - m_GL;
				rect.right  = nPrePos;
				rect.top    = float(pXPoint[i]) - m_GL;
				rect.bottom = nCurValue;

				nRet = IsSelected(0,point,rect,nCurve,0);
				if( nRet == BREAK )
				{
					return -1;
				}
				else if( nRet == FINDED )
				{
					if(!(linetype & NOTSELMARK))
					{
						AddSelectedStyle(nCurve);
					}
					return i;
				}
				nPrePos = nCurValue;
				continue;
			}
			nPrePos = nCurValue;
		}
		else if(linetype & STICK) // 
		{
			rect.left   = float(pXPoint[i]) - m_GL;
			rect.right  = float(pXPoint[i]) - m_GL;
			rect.top    = nCurValue;
			rect.bottom = ValueToVertPos(0);
		}		
		else
		{
			if( nPrePos != -1 && i > 0 )
			{
				rect.left   = float(pXPoint[i - 1]) - m_GL;
				rect.top    = nPrePos;
				if( (linetype & CROSSDOT) || (linetype & CIRCLEDOT) || (linetype & POINTDOT) )
				{					
					rect.right  = -1;
					rect.bottom = -1;
				}
				else 
				{
					rect.right  = float(pXPoint[i]) - m_GL;
					rect.bottom = nCurValue;
				}

				nPrePos = nCurValue;
			}
			else
			{
				nPrePos = nCurValue;
				continue;
			}
		}

		nRet = IsSelected(linetype,point,rect,nCurve,0);
		if( nRet == BREAK )
		{
			return -1;
		}
		else if( nRet == FINDED )
		{
			if(!(linetype & NOTSELMARK))
			{
				AddSelectedStyle(nCurve);
			}
			return i;
		}
	}

	return -1;
}

template<class T>
void CHSLinear<T>::FilledRectangle(T x1, T y1, T x2, T y2)
{
	POINT pt1 = GetPoint(x1, y1);
	POINT pt2 = GetPoint(x2, y2);
	m_pDC->Rectangle(pt1.x, pt1.y, pt2.x, pt2.y);
}

template<class T>
void CHSLinear<T>::FilledRectangle(T x1, T y1, T x2, T y2, COLORREF cr)
{
    CBrush hBrush;
	hBrush.CreateSolidBrush(cr);
    CBrush* hBrold = m_pDC->SelectObject(&hBrush);
	FilledRectangle(x1, y1, x2, y2);
	m_pDC->SelectObject(hBrold);
    hBrush.DeleteObject();
}

template<class T>
void CHSLinear<T>::Polygon(T* x, T* y, int n)
{
	MoveTo(x[0], y[0]);
	for(int i = 1; i < n; i ++)
		LineTo(x[i], y[i]);
	LineTo(x[0], y[0]);
}

template<class T>
bool CHSLinear<T>::FilledPolygon(T* x, T* y, int n)
{
	POINT *pt = new POINT[n];
	if (!pt) return false;
	for(int i = 0; i < n; i ++)
		pt[i] = GetPoint(x[i], y[i]);
	m_pDC->Polygon(pt, n );
	delete []pt;

	return true;
}

template<class T>
bool CHSLinear<T>::FilledPolygon(T* x, T* y, int n, COLORREF cr)
{
    CBrush hBrush;
	hBrush.CreateSolidBrush(cr);
    CBrush* hBrold = m_pDC->SelectObject(&hBrush);
	bool bl = FilledPolygon(x, y, n);
	m_pDC->SelectObject(hBrold);
    hBrush.DeleteObject();

	return bl;
}

template<class T>
void CHSLinear<T>::Lines(T* x, T* y, int n, COLORREF cr, int Index, const char* Name, int LineWidth, int LineStyle)
{
	if (m_bPrinting)	LineWidth *= m_nPrintScale;
	CPen hPen;
	hPen.CreatePen(LineStyle, LineWidth, cr);
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	MoveTo(x[0], y[0]);
	for(int i = 1; i < n; i ++)
		LineTo(x[i], y[i]);

	if (m_bEnableLegend && Index > 0 && Name)
		Legend(cr, Index, Name);

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
} 


///////////////////////////////////////////////////////////////////////////////
// CXLogYLinear operations
template<class T>
bool CXLogYLinear<T>::SetRange(double xmin, double ymin, double xmax, double ymax)
{
	m_nAxesType	 = XLOG;
	if (xmin < 0.000001 || xmax < 0.000001)
	{
		::MessageBox(NULL, "The X minimum value must be larger than 0.000001", "Warning", MB_ICONWARNING);
		return false;
	}

    m_Scale.xmin	= log10(xmin);
    m_Scale.ymin	= ymin;
    m_Scale.xmax	= log10(xmax);
    m_Scale.ymax	= ymax;
	m_nXStep		= (int)(m_Scale.xmax - m_Scale.xmin);

	return true;
}

template<class T>
void CXLogYLinear<T>::XAxis()
{
	int	  xb, yb, xe, ye;   
    int   i, j, k = 1;                                  
	char  str[32];
	float value;

	yb = m_GB;
	for(i = 1; i <= m_nXStep; i ++)
	{
		for(j = 1; j <= 10; j ++)
		{
			if (k == 0)   
				ye = m_GB + m_rcMargin.bottom / 7;
			else
				ye = m_GB + m_rcMargin.bottom / 14;
			xb = xe = (int)(log10(j) * m_PX / m_nXStep + 1.0 * (i-1) * m_PX / m_nXStep + m_GL);
			if (j == 1)
			{
				value = (float)(pow(10.0, m_Scale.xmin) * pow(10.0, i - 1));
				Format(m_nXDecimal, str, value);
				PrintString(xb, m_GB + m_rcMargin.bottom / 5, 0, str);
			}
			if ((i != 1 || j != 1) && (i != m_nXStep || j != 10))
				DrawLine(xb, yb, xe, ye);
			k = 1;
		}
		k=0;
	}
	value = (float)(pow(10.0, m_Scale.xmin) * pow(10.0, i - 1));
	Format(m_nXDecimal, str, value);
	PrintString(m_GR, m_GB + m_rcMargin.bottom / 5, 0, str);
}

template<class T>
void CXLogYLinear<T>::Grid()
{
	int i, j, k;
                
	CPen hPen;
	hPen.CreatePen(PS_DOT, 0, m_nGridColor);
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	for(i = 1; i < m_nXStep; i ++)
		for(k = 2; k <= 10; k ++)
	{
		j = (int)(log10(k) * m_PX / m_nXStep + 1.0 * (i - 1) * m_PX / m_nXStep + m_GL);
		DrawLine(j, m_GT + 1, j, m_GB - 1);
	}
	for(k = 2; k < 10; k ++)
	{
		j = (int)(log10(k) * m_PX / m_nXStep + 1.0 * (m_nXStep - 1) * m_PX / m_nXStep + m_GL);
		DrawLine(j, m_GT + 1, j, m_GB - 1);
	}

	for(i = 1; i < m_YGridTicks; i ++) 
	{
		j = m_GT + (int)(1.0 * i * (m_GB-m_GT) / m_YGridTicks);
		DrawLine(m_GL + 1, j, m_GR - 1, j);
	}

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
}

template<class T>
POINT CXLogYLinear<T>::GetPoint(T x, T y)
{
	if(m_Scale.dx != 0)
	{
		m_Pt.x = (LONG)((log10(x) - m_Scale.xmin) / m_Scale.dx) + m_GL;
	}
	else
	{
		m_Pt.x = 0;
	}
    if(m_Scale.dy != 0)
	{
		m_Pt.y = (LONG)(m_GB - (y - m_Scale.ymin) / m_Scale.dy);
	}
	else
	{
		m_Pt.y = 0;
	}
	return m_Pt;
}

template<class T>
void CXLogYLinear<T>::GetValue(POINT& pt, T& x, T& y)
{               
    x = (T)pow(10, m_Scale.xmin + (pt.x - m_GL) * m_Scale.dx);
	y = (T)(m_Scale.ymin + (m_GB - pt.y) * m_Scale.dy);
}


///////////////////////////////////////////////////////////////////////////////
// CXYLogLinear operations
template<class T>
bool CXYLogLinear<T>::SetRange(double xmin, double ymin, double xmax, double ymax)
{
	return SetRange_LogY(xmin,  ymin,  xmax, ymax);

	/*m_nAxesType	 = YLOG;
	if (ymin < 0.000001 || ymax < 0.000001)
	{
		::MessageBox(NULL, "The Y minimum value must be larger than 0.000001", "Warning", MB_ICONWARNING);
		return false;
	}

    m_Scale.xmin	= xmin;
    m_Scale.ymin	= log10(ymin);
    m_Scale.xmax	= xmax;
    m_Scale.ymax	= log10(ymax);
	m_nYStep		= (int)(m_Scale.ymax - m_Scale.ymin);

	return true;*/
}

template<class T>
void CXYLogLinear<T>::YAxis()
{
	YAxis_LogY();	
}

template<class T>
void CXYLogLinear<T>::Grid()
{
	Grid_LogY();

	/*
	int i, j, k;
                
	CPen hPen;
	hPen.CreatePen(PS_DOT, 0, m_nGridColor);
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	for(i = 1; i < m_XGridTicks; i ++)
	{
		j = m_GL + (int)(1.0 * i * (m_GR-m_GL) / m_XGridTicks);
		DrawLine(j, m_GT + 1, j, m_GB - 1);
	}

	for(i = 1; i < m_nYStep; i ++)
		for(k = 2; k <= 10; k ++)
	{
		j = m_GB - (int)(log10(k) * m_PY / m_nYStep + 1.0 * (i - 1) * m_PY / m_nYStep);
		DrawLine(m_GL + 1, j, m_GR - 1, j);
	}
	for(k = 2; k < 10; k ++)
	{
		j = m_GB - (int)(log10(k) * m_PY / m_nYStep + 1.0 * (m_nYStep - 1) * m_PY / m_nYStep);
		DrawLine(m_GL + 1, j, m_GR - 1, j);
	}
	
	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
	*/
}

template<class T>
POINT CXYLogLinear<T>::GetPoint(T x, T y)
{
	return GetPoint_LogY(x,y);

	/*if(m_Scale.dx != 0)
	{
		m_Pt.x = (LONG)((x - m_Scale.xmin) / m_Scale.dx) + m_GL;
	}
	else
	{
		m_Pt.x = 0;
	}
    if(m_Scale.dy != 0)
	{
		m_Pt.y = (LONG)(m_GB - (log10(y) - m_Scale.ymin) / m_Scale.dy);
	}
	else
	{
		m_Pt.y = 0;
	}
	return m_Pt;*/
}

template<class T>
void CXYLogLinear<T>::GetValue(POINT& pt, T& x, T& y)
{               
    x = (T)(m_Scale.xmin + (pt.x - m_GL) * m_Scale.dx);
	y = (T)pow((double)10, (m_Scale.ymin + (m_GB - pt.y) * m_Scale.dy));
}

///////////////////////////////////////////////////////////////////////////////
// CXLogYLogLinear operations
template<class T>
bool CXLogYLogLinear<T>::SetRange(double xmin, double ymin, double xmax, double ymax)
{
	m_nAxesType	 = XYLOG;
	if (xmin < 0.000001 || xmax < 0.000001)
	{
		::MessageBox(NULL, "The X minimum value must be larger than 0.000001", "Warning", MB_ICONWARNING);
		return false;
	}

	if (ymin < 0.000001 || ymax < 0.000001)
	{
		::MessageBox(NULL, "The Y minimum value must be larger than 0.000001", "Warning", MB_ICONWARNING);
		return false;
	}

    m_Scale.xmin	= log10(xmin);
    m_Scale.ymin	= log10(ymin);
    m_Scale.xmax	= log10(xmax);
    m_Scale.ymax	= log10(ymax);
	m_nXStep		= (int)(m_Scale.xmax - m_Scale.xmin);
	m_nYStep		= (int)(m_Scale.ymax - m_Scale.ymin);

	return true;
}

template<class T>
void CXLogYLogLinear<T>::XAxis()
{
	int	  xb, yb, xe, ye;   
    int   i, j, k = 1;                                  
	char  str[32];
	float value;

	yb = m_GB;
	for(i = 1; i <= m_nXStep; i ++)
	{
		for(j = 1; j <= 10; j ++)
		{
			if (k == 0)   
				ye = m_GB + m_rcMargin.bottom / 7;
			else
				ye = m_GB + m_rcMargin.bottom / 14;
			xb = xe = (int)(log10(j) * m_PX / m_nXStep + 1.0 * (i-1) * m_PX / m_nXStep + m_GL);
			if (j == 1)
			{
				value = (float)(pow(10.0, m_Scale.xmin) * pow(10.0, i - 1));
				Format(m_nXDecimal, str, value);
				PrintString(xb, m_GB + m_rcMargin.bottom / 5, 0, str);
			}
			if ((i != 1 || j != 1) && (i != m_nXStep || j != 10))
				DrawLine(xb, yb, xe, ye);
			k = 1;
		}
		k=0;
	}
	value = (float)(pow(10.0, m_Scale.xmin) * pow(10.0, i - 1));
	Format(m_nXDecimal, str, value);
	PrintString(m_GR, m_GB + m_rcMargin.bottom / 5, 0, str);
}

template<class T>
void CXLogYLogLinear<T>::YAxis()
{
	int	  xb, yb, xe, ye;   
    int   i, j, k = 1;                                  
	char  str[32];
	float value;

	xe = m_GL;
	for(i = 1; i <= m_nYStep; i ++)
	{
		for(j = 1; j <= 10; j ++)
		{
			if (k == 0)   
				xb = m_GL - m_rcMargin.left / 10;
			else
				xb = m_GL - m_rcMargin.left / 20;
			yb = ye = m_GB - (int)(log10(j) * m_PY / m_nYStep + 1.0 * (i - 1) * m_PY / m_nYStep);
			if (j == 1)
			{
				value = (float)(pow(10.0, m_Scale.ymin) * pow(10.0, i - 1));
				Format(m_nYDecimal, str, value);
				PrintString(m_GL - m_rcMargin.left / 6, yb, 0, str);
			}
			if ((i != 1 || j != 1) && (i != m_nYStep || j != 10))
				DrawLine(xb, yb, xe, ye);
			k = 1;
		}
		k=0;
	}

	value = (float)(pow(10.0, m_Scale.ymin) * pow(10.0, i - 1));
	Format(m_nYDecimal, str, value);
	PrintString(m_GL - m_rcMargin.left / 6, yb, 0, str);
}

template<class T>
void CXLogYLogLinear<T>::Grid()
{
	int i, j, k;
                
	CPen hPen;
	hPen.CreatePen(PS_DOT, 0, m_nGridColor);
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	for(i = 1; i < m_nXStep; i ++)
		for(k = 2; k <= 10; k ++)
	{
		j = (int)(log10(k) * m_PX / m_nXStep + 1.0 * (i - 1) * m_PX / m_nXStep + m_GL);
		DrawLine(j, m_GT + 1, j, m_GB - 1);
	}
	for(k = 2; k < 10; k ++)
	{
		j = (int)(log10(k) * m_PX / m_nXStep + 1.0 * (m_nXStep - 1) * m_PX / m_nXStep + m_GL);
		DrawLine(j, m_GT + 1, j, m_GB - 1);
	}

	for(i = 1; i < m_nYStep; i ++)
		for(k = 2; k <= 10; k ++)
	{
		j = m_GB - (int)(log10(k) * m_PY / m_nYStep + 1.0 * (i - 1) * m_PY / m_nYStep);
		DrawLine(m_GL + 1, j, m_GR - 1, j);
	}
	for(k = 2; k < 10; k ++)
	{
		j = m_GB - (int)(log10(k) * m_PY / m_nYStep + 1.0 * (m_nYStep - 1) * m_PY / m_nYStep);
		DrawLine(m_GL + 1, j, m_GR - 1, j);
	}
	
	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
}

template<class T>
POINT CXLogYLogLinear<T>::GetPoint(T x, T y)
{
	if(m_Scale.dx != 0)
	{
		m_Pt.x = (LONG)((log10(x) - m_Scale.xmin) / m_Scale.dx) + m_GL;
	}
	else
	{
		m_Pt.x = 0;
	}
    if(m_Scale.dy != 0)
	{
		m_Pt.y = (LONG)(m_GB - (log10(y) - m_Scale.ymin) / m_Scale.dy);
	}
	else
	{
		m_Pt.y = 0;
	}
	return m_Pt;
}

template<class T>
void CXLogYLogLinear<T>::GetValue(POINT& pt, T& x, T& y)
{               
    x = (T)pow(10, (m_Scale.xmin + (pt.x - m_GL) * m_Scale.dx));
	y = (T)pow(10, (m_Scale.ymin + (m_GB - pt.y) * m_Scale.dy));
}

#endif // !defined(AFX_YLSLINEAR_H__A1A53162_BFE4_4A25_91DF_7A59E2DA377B__INCLUDED_)
