/*******************************************************************************
 * Copyright (c)2003, 杭州恒生信息技术有限公司
 * All rights reserved.
 *
 * 文件名称：HSDrawBaseStruct.h
 * 文件标识：绘图主要结构定义头文件
 * 摘    要：绘图主要结构定义头文件
 *
 * 当前版本：1.0
 * 作    者：
 * 完成日期：
 *
 * 备	 注：
 *******************************************************************************/

#if !defined(_YLSDRAWSTRUCT_H_)
#define _YLSDRAWSTRUCT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
//
#include "..\..\..\Common\FormulaManPublic.h"

// view类型
enum CutViewType
{
	CutViewNone,	//空
	CutViewIn,		//嵌入
	CutViewPopup	//弹出显示,如历史回忆
};

// 绘制对象类型
enum CutAtTimePageType
{
	CutNoThing,		 //无

	CutReport,		 // 报表
	CutPaiMingReport,// 排名报表

	CutFenShi,       // 个股分时
	CutTech,	     // 个股日线	

	CutBigTech,      // 大盘日线
	CutBigFenShi,    // 大盘分时
	CutBigADMFenShi, // 大盘ADM分时
	CutBigNon,
	CutBigShSz,		 // 上证深证

	CutPubPage,		 // 公共小图		 

	CutAddTech,			 // 增加技术指标
	CutChageTech,		 // change技术指标
	CutAddPKLine,		 // 增加PKLine
	CutAddByAlignTree,	 // 由停靠窗口添加
	CutAddMajor,       //yulx add 20091112叠加大盘
	CutStarField,		// 星空图


	CutGraphicFinance,	//	财务图示，杨朝惠添加
	CutGannAnalisys     //  江恩分析
};
//明细报表类型
enum DetailTableType
{
	DetailNone,
	DLFenShi,     // 分时明细
	DLRiXian,     // 日线明细
	DLBig,	      // 大盘明细
	DLFenJiaTable // 分价表
};
//明细报表数据发送
struct StockUserInfo;
struct DetailTableDataSend
{
	int   nType;	//类型
	int   lDate;	//日期 
	void* pData;	//数据指针	
	void* pSize;	//数据长度
	StockUserInfo* pStock;	//股票结构指针
	CStringArray  strHisGeneralInfo;
	CArray<char*,char*>* p_ayReqData;
	CArray<short,short>* p_ayReqLen;
	CArray<StockUserInfo*,StockUserInfo*>* p_payStock;

	DetailTableDataSend()
	{
		memset(this,0,sizeof(DetailTableDataSend));
	}
};
//切分条类型
enum SplitterType
{	
	SplitterX,   // 横向拆分
	SplitterY,   // 纵向拆分
	SplitterBoth // 纵横拆分
};
//画线工具操作
enum DrawOperate
{
	OpProperty,   // 对象属性
	OpCopy,       // 对象拷贝
	OpDelete,	  // 删除单一
	OpDeleteAll,  // 删除所有
	OpObjList,	  // 绘制列表
	OpRectInfo,   // 区域信息
	OpDrawListObj // 绘制列表信息
};
// 分析 obj 数据
class CCutTechAnalysis;
class CCutStockTrendEx;
class CDrawTechKLine;
struct AnalisysObjData
{
	BOOL			m_bRequest;			// 请求
	BOOL			m_bRequestNext; 	// 请求
	short			m_sObjtatus;		// 状态	
	CodeInfo		m_Info;				// 股票代码信息

	long			m_lTotalVol;		// 成交总量
	long			m_lTotalMeney;		// 成交总额
	int				m_nLastTransDay;	// 最后传输的天数

  union
  {
	CCutTechAnalysis* m_pParent;		// 父亲管理者
  CCutStockTrendEx* m_pTrendEx;
  };
	CDrawTechKLine*	  m_pPKLine;		// 相关图元

	int				m_nValueNum;		// 除权及插取后的数据个数
	StockDay *	    m_pData;    		// 除权后的数据

	/*************************************************************
	* 吕连新更改
	* 时间：  2003年9月18日
	* 描述：  接收到或是从盘中读出的原始K线数据（最基本单位K线）
	*************************************************************/
	int				m_nOldValueNum;		// 原始数据长度
	StockDay*	    m_pOldData; 		// 原始数据
	/**********吕连新更改结束*************/

	~AnalisysObjData();
	AnalisysObjData() { memset(this,0,sizeof(AnalisysObjData)); }
  //CHANGE BY YUHJ 2008.5.8
  int IsRiseOrFall(int pos,BOOL bNegative)
  {
    long lPrevClose = 0;
    StockDay *pData = m_pData + pos;

    if(pos > 0)
		  lPrevClose = Negative(m_pData[pos-1].m_lClosePrice,bNegative);

  	long	lClose;//   = m_pStock->m_lPrevClose;
	  lClose = Negative(lPrevClose,bNegative); // m_lNO1
	  if( lClose == 0 )
	  {
		  lClose = pData->m_lClosePrice;
	  }
  
    long lData = pData->m_lClosePrice ? (pData->m_lClosePrice - lClose) * 100 : 0;  

    if(!lData)
      return 0;

    return lData > 0 ? 1 : -1;
  }
  //END
	AnalisysObjData(CCutStockTrendEx* pParent,short sObjtatus,const CodeInfo* pInfo)
	{
		memset(this,0,sizeof(AnalisysObjData));

		m_pTrendEx  = pParent;
		m_bRequest = TRUE;
		m_bRequestNext = FALSE;

		m_sObjtatus = sObjtatus;
		if(pInfo != NULL)
		{
			memcpy(&m_Info,pInfo,sizeof(CodeInfo));
		}
	}

	AnalisysObjData(CCutTechAnalysis* pParent,short sObjtatus,const CodeInfo* pInfo)
	{
		memset(this,0,sizeof(AnalisysObjData));

		m_pParent  = pParent;
		m_bRequest = TRUE;
		m_bRequestNext = FALSE;

		m_sObjtatus = sObjtatus;
		if(pInfo != NULL)
		{
			memcpy(&m_Info,pInfo,sizeof(CodeInfo));
		}
	}
	//设置绘制图元指针
	void SetPKLine(CDrawTechKLine* pPKLine) {m_pPKLine = pPKLine;}
	//判断状态
	BOOL IsStatus(short	sObjtatus){return m_sObjtatus == sObjtatus;}
	//设置请求状态
	void SetRequest(BOOL bRequest){m_bRequest = bRequest;}
	//返回m_bRequest
	BOOL IsRequest(AnalisysObjData* pFirstData)
	{
		return m_bRequest;
	}
	//设置请求状态
	void SetRequestNext(BOOL bRequestNext){m_bRequestNext = bRequestNext;}
	//返回m_bRequestNext
	BOOL IsRequestNext()                  {return m_bRequestNext;}
	//设置股票信息
	void SetInfo(const CodeInfo* pInfo)
	{
		if(pInfo != NULL)
		{
			memcpy(&m_Info,pInfo,sizeof(CodeInfo));
		}
		else
		{
			ZeroMemory(&m_Info, sizeof(CodeInfo));
		}
	}
	//返回m_nLastTransDay
	int GetLastTransDay()
	{
		m_nLastTransDay;
	}
	//完成K线数据按照日期快速排序
	void SizeData(AnalisysObjData* pFirstData);
	/********************************************************************************
	 * 函数功能 : 复制数据
	 * 函数参数 : StockDay* pKData - K线数据指针 非NULL int nSize - 数据个数
	 *			  AnalisysObjData* pKData - 分析数据的指针 非NULL
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	void CopyData(StockDay* pKData, int nSize);
	void CopyData(AnalisysObjData* pKData);
	//释放内存
	void Free();
	//释放内存并初始化
	void SetNULL();
	/********************************************************************************
	 * 函数功能 : 生成组合请求包(与右小图), 向主窗体发送请求包
	 * 函数参数 : CodeInfo& info,short nTAnali,BOOL bNext,int nRequestCount - 历史遗留参数, 未使用
	 *  返回值  : void
	 * 调用依赖 : 用户有请求需求时候, 调用; 调用GetRequestData()取得请求包
	 * 作    者 :俞礼石 被RequestData调用
	 * 完成日期 :2003-5-1
	 * 修改记录 : 
	 *******************************************************************************/
	void RequestData(short nTAnali = -1,BOOL bNext = FALSE,int nRequestCount = -1);
	//返回股票名称
	CString GetName();
	/********************************************************************************
	 * 函数功能 : 读取数据, 包括除权数据等
	 * 函数参数 : void* pAsk - 数据请求包 非NULL
	 *			  AnalisysObjData* pPreData - 前一分析数据指针 非NULL
	 *  返回值  : 成功返回TRUE, 失败返回FALSE
	 * 调用依赖 : 
	 * 作    者 :俞礼石
	 * 完成日期 :2003-5-7
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL    ReadData(void* pAsk,AnalisysObjData* pPreData);
	//判断数据是阴线还是阳线
	BOOL IsUp(int nPosition)
	{
		if( m_pData == NULL ||
			nPosition < 0   ||
			nPosition >= m_nValueNum)
			return FALSE;
		return (m_pData[nPosition].m_lOpenPrice < m_pData[nPosition].m_lClosePrice
			|| (m_pData[nPosition].m_lOpenPrice == m_pData[nPosition].m_lClosePrice
			   && m_pData[nPosition].m_lClosePrice >= m_pData[(nPosition - 1)>0?(nPosition - 1) : 0].m_lClosePrice));
	}
	//临时复制保存数据
	void SaveCur();
	void LoadCur();
  void Append(StockDay &day);//ADD BY YUHJ 2008.6
	BOOL IsSave();
	//返回原始的数据
	StockDay* GetSrcData();

	// 数据的倒数
	void Convert(int nPriceUnit);
};



/////////////////////////////////////////////////////////////////////////////
// CDoublePoint - A 2-D point, similar to Windows POINT structure.

class CDoublePoint;
class CDoubleRect;
//高精度点结构
class CDoublePoint : public CObject
{
public:

// Constructors
	CDoublePoint() { x = 0 ; y = 0; }
	CDoublePoint(double initX, double initY)
	{
		x = initX;
		y = initY;
	}
	CDoublePoint(CDoublePoint& point)
	{
		x = point.x;
		y = point.y;
	}
// Operations
	//偏移
	void Offset(double xOffset, double yOffset) { x += xOffset; y += yOffset; }
	//操作符=重载
	void operator=(const CDoublePoint& point)
	{
		x = point.x;
		y = point.y;
	}
	//操作符==重载
	BOOL operator==(CDoublePoint point) const
	{
		return (x == point.x && y == point.y); 
	}
	//操作符!=重载
	BOOL operator!=(CDoublePoint point) const
	{ 
		return (x != point.x || y != point.y); 
	}
	//操作符+=重载
	void operator+=(CDoublePoint point)	{ x += point.x; y += point.y; }
	//操作符-=重载
	void operator-=(CDoublePoint point) { x -= point.x; y -= point.y; }
	//操作符+重载
	CDoublePoint operator+(CDoublePoint point) const
	{ 
		return CDoublePoint(x + point.x, y + point.y); 
	}
	//操作符-重载
	CDoublePoint operator-(CDoublePoint point) const
	{ 
		return CDoublePoint(x - point.x, y - point.y); 
	}
	//操作符-重载
	CDoublePoint operator-() const 	{ return CDoublePoint(-x, -y); }

public:
	double	x, y;
};

/////////////////////////////////////////////////////////////////////////////
// CDoubleRect - A 2-D rectangle, similar to Windows RECT structure.
//高精度矩形区域
class CDoubleRect : public tagRECT
{
public:

// Constructors
	CDoubleRect()
	{
		left = right = top = bottom = 0;
	}
	CDoubleRect(double l, double t, double r, double b)
	{ 
		left = l; top = t; right = r; bottom = b; 
	}
	CDoubleRect(const CDoubleRect& srcRect)	{ CopyRect(&srcRect); }
	CDoubleRect(CDoublePoint point, SIZE size)
	{ 
		right = (left = point.x) + size.cx; 
		bottom = (top = point.y) + size.cy; 
	}
	CDoubleRect(CDoublePoint topLeft, CDoublePoint bottomRight)
	{ 
		left = topLeft.x; 
		top = topLeft.y;
		right = bottomRight.x; 
		bottom = bottomRight.y; 
	}

// Attributes (in addition to RECT members)
	//返回宽度,高度
	double Width() const { return right - left; }
	double Height() const{ return bottom - top; }
	//返回(TOP,LEFT)
	CDoublePoint& TopLeft() { return *((CDoublePoint*)this); }
	//返回(BOTTOM, RIGHT)
	CDoublePoint& BottomRight() { return *((CDoublePoint*)this+1); }
	//返回(TOP,LEFT)
	const CDoublePoint& TopLeft() const { return *((CDoublePoint*)this); }
	//返回(BOTTOM, RIGHT)
	const CDoublePoint& BottomRight() const { return *((CDoublePoint*)this+1); }
	//返回中点
	CDoublePoint CenterPoint() const { return CDoublePoint((left+right)/2, (top+bottom)/2); }
	//判断是否为空
	BOOL IsRectEmpty() const
	{
		return (left >= right || top >= bottom);
	}
	//判断区域是否为空
	BOOL IsRectNull() const
	{ 
		return (left == 0 && right == 0 && top == 0 && bottom == 0); 
	}
	//判断点是否在区域内
	BOOL PtInRect(CDoublePoint point) const
	{
		return (point.x >= left && point.x < right && point.y >= right && point.y < bottom);
	}

// Operations
	//设置区域
	void SetRect(double x1, double y1, double x2, double y2)
	{
		left = x1; top = y1;
		right = x2; bottom = y2;
	}
	//设置区域
	void SetRect(CDoublePoint topLeft, CDoublePoint bottomRight)
	{
		SetRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
	}
	//区域置空
	void SetRectEmpty()
	{
		left = top = right = bottom = 0;
	}
	//复制区域
	void CopyRect(const CDoubleRect* lpSrcRect)
	{
		memcpy(this, lpSrcRect, sizeof(CDoubleRect));
	}
	//判断区域是否相等
	BOOL EqualRect(CDoubleRect* lpRect) const
	{
		return memcmp(this, lpRect, sizeof(CDoubleRect)) == 0;
	}
	//Increases the width and height of CRect
	void InflateRect(double x, double y)
	{
		left -= x; right += x;
		top  -= y; bottom += y;
	}
	//Increases the width and height of CRect
	void InflateRect(SIZE size) { InflateRect((double)size.cx, (double)size.cy); }
	//Moves CRect by the specified offsets
	void OffsetRect(double x, double y)
	{
		left += x; right += x;
		top  += y; bottom += y;
	}
	//Moves CRect by the specified offsets
	void OffsetRect(SIZE size) { OffsetRect((double)size.cx, (double)size.cy); }
	//Moves CRect by the specified offsets
	void OffsetRect(CDoublePoint point) { OffsetRect(point.x, point.y); }
	//Standardizes the height and width of CRect
	void NormalizeRect();
	// Additional Operations
	operator RECT()
	{
		CRect rect;
		rect.left   = (int)left;
		rect.right  = (int)right;
		rect.top    = (int)top;
		rect.bottom = (int)bottom;
		return rect;
	}
	//Copies the dimensions of a rectangle to CRect
	void operator=(const RECT& srcRect)
	{
		left   = (double)srcRect.left;
		right  = (double)srcRect.right;
		top    = (double)srcRect.top;
		bottom = (double)srcRect.bottom;
	}
	//Copies the dimensions of a rectangle to CRect
	void operator=(const CDoubleRect& srcRect)
	{
		CopyRect(&srcRect);
	}
public:
	double left, top, right, bottom;
};

// 区间分析数据
struct InterzoneData
{
	int		m_nDateBegin;		// 开始日期
	int		m_nDateEnd;			// 结束日期
	int		m_nTimeBegin;		// 开始时间
	int		m_nTimeEnd;			// 结束时间
	
	CodeInfo	m_tagCode;		// 代码
	int		m_nCount;		// 数据组数
	long	m_lOpenPrice;		//开
	time_t  m_tOpen;			

	long	m_lMaxPrice;		//高
	time_t  m_tMax;

	long	m_lMinPrice;		//低
	time_t  m_tMin;

	long	m_lClosePrice;		//收
	time_t  m_tClose;

	long	m_lPreClosePrice;		//昨收
	
	double		m_dbMoney;			//成交金额
	double		m_dbTotal;			//成交量   单位：股
	double		m_dbSumClose;		//收盘价和

	HSDrawData	m_dbCAPITAL;		//流通盘
};
// Serialization
/********************************************************************************
* 函数功能 : 函数使用于基本结构的串行化实现                                                                    	 
* 函数参数 : CArchive& ar - 串行化类         
*			  CDoublePoint point - 高精度点
*			CDoubleRect& rect - 高精度区域
*  返回值  :  CArchive&  - 串行化类                                                                   	 
* 调用依赖 :                                                                     	 
* 作    者 :俞礼石                                                               	 
* 完成日期 :2003-5-7                                                             	 
* 修改记录 :                                                                     	 
*******************************************************************************/ 	 
inline CArchive& AFXAPI operator<<(CArchive& ar, CDoublePoint point)
{ 
	ar.Write(&point, sizeof(CDoublePoint)); 
	return ar; 
}
inline CArchive& AFXAPI operator<<(CArchive& ar, const CDoubleRect& rect)
{ 
	ar.Write(&rect, sizeof(CDoubleRect)); 
	return ar; 
}
inline CArchive& AFXAPI operator>>(CArchive& ar, CDoublePoint& point)
{ 
	ar.Read(&point, sizeof(CDoublePoint)); 
	return ar; 
}
inline CArchive& AFXAPI operator>>(CArchive& ar, CDoubleRect& rect)
{ 
	ar.Read(&rect, sizeof(CDoubleRect)); 
	return ar; 
}


// 拖放数据,用于接收外部托入的数据
class CDragAndDropData
{
public:
	CutAtTimePageType m_lType;			 //页面类型
	void*			  m_pIdentifier;	 //标识,指示到视图指针
	void*			  m_pData;			 //指示数据,例如公式,股票等数据指针
	char			  m_cUserType;		 //自定义类型,枚举
	char			  m_cAddKlineAndDel; //叠加后删除原来的指标

public:
	enum {none,MouseMove,DbClicked};

	CDragAndDropData(){memset(this,0,sizeof(CDragAndDropData));}
	CDragAndDropData(CutAtTimePageType lType,void* pIdentifier,void* pData)
	{
		m_lType = lType;
		m_pIdentifier = pIdentifier;
		m_pData = pData;
	}
	~CDragAndDropData(){}
	//数据有效性
	BOOL IsValidate()
	{
		return ( (m_pIdentifier != NULL) && (m_pData != NULL) && (m_lType != 0) );
	}
	//比较标识
	BOOL Compare(void* pIdentifier)
	{
		if(!IsValidate())
			return FALSE;
		return (m_pIdentifier == pIdentifier);
	}
	//返回数据指针
	void* GetData() {return m_pData;}
	//返回标识指针
	void* GetIdentifier() {return m_pIdentifier;}
	//返回页面类型
	CutAtTimePageType GetType() {return m_lType;}

};

//K线数据接收结构
struct StockDay;
struct CStockDayDataSequence
{
	short m_nType;	//请求类型
	int	  m_nSize;	//数据个数
	int   m_nStart;	//数据起始位置
	StockDay * m_pData;	//数据指针

	CStockDayDataSequence()
	{
		memset(this,0,sizeof(CStockDayDataSequence));
	}
};


//////////////////////////////////////////////////////////////////////////////////
////// struct CTPoint
//不定类型点
template <class T> 
struct CTPoint
{
	T x;
	T y;
	DWORD  m_dwMarker;	// 标签类型,可多种组合（参见CMarkerType）

	CTPoint()							   { m_dwMarker = 0; x = 0; y = 0; }
	CTPoint(T initX, T initY)			   { x = initX; y = initY; }
	void operator = (const CTPoint& pt)	   { x = pt.x;	y = pt.y;  }

	// 标签操作:添加,删除,判断
	void AddMarker(DWORD dwMarkerType)     { m_dwMarker |= dwMarkerType;    }	
	void RemoveMarker(DWORD dwMarkerType)  { m_dwMarker &= (~dwMarkerType); }
	BOOL IsMarker()						   { return m_dwMarker != 0; }
	BOOL IsMarker(DWORD dwMarkerType)      { return (m_dwMarker & dwMarkerType);  }

};
//不定类型的文本类,主要用于注释标题
class CTechKLineImp;
template <class ToptextType>
struct CToptextType
{
	CString		 m_strName;		//文本内容
	ToptextType  m_Value;		//值
	COLORREF	 m_cl;			//文本颜色
	short		 m_nDecimal;	//小数位数
	BYTE		 m_bArrow;		//绘制箭头类型
	short		 m_nImageIndex;	//绘制imagelist
	CTechKLineImp*	m_pTechLine;

	CToptextType() {m_Value = 0;m_cl = 0;m_bArrow = 0;}
	CToptextType(CString strName, ToptextType Value, COLORREF cl, CTechKLineImp* pImp, short nDecimal = 0,BYTE bArrow = 0, short nImageIndex = -1)
	{
		m_strName  = strName;
		m_Value    = Value;
		m_cl       = cl;
		m_nDecimal = nDecimal;
		m_bArrow   = bArrow;
		m_nImageIndex = nImageIndex;
		m_pTechLine = pImp;
	}
	~CToptextType()
	{
	}
	//判断绘制箭头类型是否为箭头
	BOOL IsArrow() { return (BOOL)(m_bArrow&01); }
	//判断绘制箭头类型是否为方块
	BOOL IsAddHig() { return (BOOL)(m_bArrow&02); }
};


// 横坐标
struct DrawPointInfo
{
	float m_fCoordinate;	//对应的屏幕像素点
	int	  m_lPoint;			//数据索引点

	DrawPointInfo()
	{
		memset(this,0,sizeof(DrawPointInfo));
	}
	//操作符	
	void operator = (float fCoordinate)
	{
		m_fCoordinate = fCoordinate;
	}
	//操作符
	void operator = (int lPoint)
	{
		m_lPoint = lPoint;
	}
	//操作符
	operator float() { return m_fCoordinate; }
	//操作符
	operator int()	 { return (int)m_fCoordinate; }

};

//实时数据-绘图
struct DrawFenShiData
{
	long   m_lValue;		//值
	short  m_nMessageMask;	//消息掩码
	union
	{
		long  m_lTime;		//时间
		long* m_pDataEx;	//扩展数据指针
	};

	DrawFenShiData()
	{
		memset(this,0,sizeof(DrawFenShiData));
	}
	//赋值操作符
	void operator = (long lValue)
	{
		m_lValue = lValue;
	}
	void operator = (unsigned long lValue)
	{
		m_lValue = lValue;
	}
	//赋值操作符
	void operator = (int lPoint)
	{
		m_lValue = lPoint;
	}
	//强制类型转换操作符
	operator long()		   { return m_lValue; }
	operator int()		   { return (int)m_lValue; }
	operator double long() { return (double long)m_lValue; }
};

//缩放数据
struct ZoomInAndOutData
{
	char  m_nType;			//缩放类型,枚举值
	float m_nCountPoint;	//当前屏幕的数据点总数	
	float m_nCurPoint;		//当前参考点
	ZoomInAndOutData* m_pPreZoom;//指向前一个缩放数据指针
	//? 枚举含义？？
	enum ZoomType {none,BigIn,BigOut,SmallIn,SmallOut,DelAll,ZoomOut = WM_USER,ZoomIn,MoreChartUnion};
	ZoomInAndOutData()
	{
		memset(this,0,sizeof(ZoomInAndOutData));
	}
	~ZoomInAndOutData()
	{
		if(m_pPreZoom != NULL)
		{
			delete m_pPreZoom;
			m_pPreZoom = NULL;
		}
	}
	ZoomInAndOutData(char nType,float nCountPoint,float nCurPoint)
	{
		m_nType		  = nType;
		m_nCountPoint = nCountPoint;
		m_nCurPoint   = nCurPoint;
		m_pPreZoom    = NULL;
	}
	//设置数值
	void Set(char nType,float nCountPoint,float nCurPoint)
	{
		m_nType		  = nType;
		m_nCountPoint = nCountPoint;
		m_nCurPoint   = nCurPoint;
		//m_pPreZoom    = NULL;
	}
	//判断类型
	BOOL IsType(char nType) { return (m_nType == nType); }
};

//鼠标消息类型结构
struct hxRefMouseMsg
{
	char m_cType;		//类型

	enum Type 
	{
		none,
		RMove,RMoveDone,	// 移动
		RSel,RSelDone,		// 选中

		StartPlanMove,		// 启动平移
		CancelPlanMove,		// 取消平移
		DBClickPlanMove,	// 双击平移
		VerPlanMove,		// 纵向平移
		RightHoriPlanMove,	// 右键横向平移
		RightHoriPlanMoveDone,	// 右键横向平移结束
		RightVerPlanMove,	    // 右键纵向平移

		RightZoomIn,		// 右键放大
		RightZoomRect,		// 得到右键放大区域
		ClickInExplainRect, // 点击在图形数字显示区域
		ClickInExplainDownRect,
		NextRefresh,		// 下次需要刷新
		LButtonMove,		// 左键按下移动(在非图形区域)
		RButtonMove,		    // 右键按下移动(在非图形区域)
		LButtonRightOperator    // 图表标题操作
	};

	hxRefMouseMsg()
	{
		m_cType = -1;
	}
	hxRefMouseMsg(Type cType)
	{
		m_cType = (char)cType;
	}
	//判断类型
	BOOL IsType(Type cType) { return (m_cType == cType); }

};

// 绘图参数
struct CDrawParam
{
	short m_nValidCount;     // 有效绘图总数
	short m_nSingleWidth;    // 单个绘图宽度
	CDrawParam()
	{
		memset(this,0,sizeof(CDrawParam));
	}
};

struct CLineProperty;
//当前选择的指标线结构
struct yls_CurSelLineData
{
	int				m_nSelPos;	//当前选中,指示横向选中数据的下标
	CLineProperty*  m_pData;	//指针
	yls_CurSelLineData()
	{
	}
	yls_CurSelLineData(int nSelPos,CLineProperty* pData)
	{
		m_nSelPos = nSelPos;
		m_pData   = pData;
	}
};

#define TimeCurrentData StockOtherData

//// 短格式的other数据
//struct TimeCurrentData
//{
//   long  m_nTime;		// 现在时间
//   long  m_lCurrent;    // 现在总手
//   long	 m_lInside;		// 内盘
//   long	 m_lOutside;	// 外盘
//   TimeCurrentData(StockOtherData* pOtherData)
//   {
//	   m_nTime = pOtherData->m_nTime;
//	   m_lCurrent = pOtherData->m_lCurrent;
//	   m_lInside  =	pOtherData->m_lInside;
//	   m_lOutside =	pOtherData->m_lOutside;
//   };
//   TimeCurrentData()
//   {
//	   memset(this, 0, sizeof(TimeCurrentData));
//   };
//};
//数据接收结构
struct CMulQuantitySequence
{
	int  m_nSize;	//数据大小
	int  m_nStart;	//起始
	long m_lReference;//中线参考
	long m_lMax;		//最大
	long m_lMin;		//最小
	int  m_nSeqs;		/*表示应答数据个数,
						0表示只有DrawFenShiData,指针m_ppValues[0],
						1表示m_ppValues[1]为领先数据指针,
						2表示m_ppValues[2]为趋势数据指针*/
	long **m_ppValues;	//数据指针
};
//分价图数据结构
struct CFenJiaTuData
{
	long m_lPrice;		//新价
	long m_lBuyVol;		//买量
	long m_lSellVol;	//卖量
	CFenJiaTuData()
	{
		m_lPrice    = 0;
		m_lBuyVol   = 0;
		m_lSellVol  = 0;
	};
	
};
//分笔数据
struct CStockTraceDataSequence
{
	int m_nSize;		//数据个数
	int m_nDataType;	//类型
	StockTick  *m_pData;	//数据指针 
};
struct DrawFenShiData;
//上证深证走势数据结构
struct CMyBigMainIndexFigInfo
{
	long  m_lTotalVol;				//成交总量
	long  m_lRefPrice;				//中线参考价格
	int   m_nValueNum;				//数据个数
	DrawFenShiData* m_pPriceValues;	//分时数据指针
	long* m_pVolValues;				//成交量数据指针
	int   m_nPoints;				//点数
};
//上证深证分类指数数据结构
struct CMyBigIndexFigInfo
{
	long  m_lTotalVol;				//成交总量     
	long  m_lRefPrice;				//中线参考价格 
	int   m_nValueNum;				//数据个数     
	DrawFenShiData* m_pPriceValues;	//分时数据指针 
	int   m_nPoints;				//点数
};
//鼠标提示信息数据结构
struct MouseTipTextInfo
{
	DWORD	m_dwMask;	//掩码

	CString m_strTitle; //标题
	CString m_strData;  //内容

	CString m_strX;		//X轴提示
	CString m_strY;		//Y轴提示
	
	CPoint* m_pPoint;	//点指针
	MouseTipTextInfo()
	{
		m_dwMask = 0;
		m_pPoint = NULL;
	}
	//清空文本
	void EmptyTip()
	{
		m_strTitle = "";
		m_strData  = "";
	}
};
//向导对话框
class CExpression;
struct YlsWizardData
{
	CString		 m_pMainTech;	//主指标组合名称
	CStringArray m_AySub;		//副指标组合名称
	YlsWizardData(){}
	YlsWizardData( CExpression* pMainTech,CArray<CExpression*,CExpression*>* AySub );
};

//K线参数数据结构
struct CKLineChartParam
{ 
	DWORD   m_dwStyle;	//风格
	DWORD   m_dwType;	//类型
	double  m_nValues;  // sum vol,close first 用于画和成交量有关的线
	CPoint  m_pointFistpoint;	//用于画收盘价线，等量线
	int		m_nPreUpDown;	//用于记录前天的行情的涨跌，－1为第一点，1为上涨，0为下跌
	int		m_nPreHigh;		//昨天高点
	int		m_nPreLow;		//昨天低点
	float   m_fChange;		//新价线的设置点
	CKLineChartParam()
	{
		m_dwStyle = 0;
		m_dwType  = -1;
		m_nValues = 0;
		m_pointFistpoint.x = -1;
		m_pointFistpoint.y = -1;
		m_nPreUpDown = -1;
		m_nPreHigh   = 0;
		m_nPreLow    = 0;
		m_fChange    = (float)0.05;
	}
	//判断类型
	BOOL IsType(DWORD dwType) { return (dwType == m_dwType); }
	
};
/*
结构声明
结构名称:StarFieldItem
功能描述:星空图一颗星星的相关数据项
作	  者:熊钟彬
日    期:2003-08-12
*/

struct StarFieldItem
{	
	HSDouble		m_dbX;		// X轴数据
	HSDouble		m_dbY;		// Y轴数据
	
	int				m_nPos;		// 数据在X向的索引位置

	char			m_cStatus;	// 涨跌标志
	short			m_nMask;	// 掩码, 标志星星类别,例如选中状态等
    
	const StockUserInfo*		m_pStockInfo;	// 星星指针,指向股票代码
public:	
	enum {Up, Down, Holding};	// 涨跌标志
	enum {Selected = 0x0100};	// 掩码取值 	
public:
	StarFieldItem()
	{
		memset(this, 0, sizeof(StarFieldItem));
	};
	int GetImage();
};
#endif // _YLSDRAWSTRUCT_H_