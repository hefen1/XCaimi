
#include "stdafx.h"
#include "systemfun.h"
#include "express.h"
#include "math.h"
#include "RegMsg.h"
#include "FormulaManPublic.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "HSDataSource.h"
#pragma warning( disable : 4244) 

CHSDataSource*							 g_pHxDataSource = NULL;    // 所有数据包装
CArray<ColorProperty*,ColorProperty*>*	 g_pColours = NULL;			// 技术指标缺省颜色

CSystemFun* g_pSystemFun = new CSystemFun;
//
// #ifdef _AFXDLL
// HWND  g_hParentWnd = NULL;
// #else
// extern HWND  g_hParentWnd;
// #endif

//static char m_bIntSysFun = 0;
//static CMap<int,int, COMPILEFUNCTION,COMPILEFUNCTION > m_pSysFunList;
//static void intSysFun()
//{
//	if( m_bIntSysFun )
//		return;
//
//	m_bIntSysFun = 1;
//
//	// 行情函数
//	m_pSysFunList.SetAt(FunOper_ADVANCE,g_pSystemFun->ADVANCE);    // 函数: ADVANCE(var1) 上涨家数
//
//	m_pSysFunList.SetAt(FunOper_AMOUNT,g_pSystemFun->AMOUNT);    // 函数: AMOUNT() 成交额
//
//	m_pSysFunList.SetAt(FunOper_AMOUNT,g_pSystemFun->AMOUNT);    // 函数: AMOUNT() 成交额
//
//	m_pSysFunList.SetAt(FunOper_ASKPRICE,g_pSystemFun->ASKPRICE);    // 函数: ASKPRICE(var1) 委卖价 1-3
//
//	m_pSysFunList.SetAt(FunOper_ASKVOL,g_pSystemFun->ASKVOL);    // 函数: ASKVOL(var1) 委卖量 1-3
//	
//	m_pSysFunList.SetAt(FunOper_BIDPRICE,g_pSystemFun->BIDPRICE);    // 函数: BIDPRICE(var1) 委买价 1-3
//	
//	m_pSysFunList.SetAt(FunOper_BIDVOL,g_pSystemFun->BIDVOL);    // 函数: BIDVOL(var1) 委买量 1-3
//	
//	m_pSysFunList.SetAt(FunOper_BUYVOL,g_pSystemFun->BUYVOL);    // 函数: BUYVOL() 主动性买单
//	
//	m_pSysFunList.SetAt(FunOper_CLOSE,g_pSystemFun->CLOSE);    // 函数: CLOSE() 收盘价
//	
//	m_pSysFunList.SetAt(FunOper_CLOSE,g_pSystemFun->CLOSE);		   // 函数: CLOSE() 收盘价
//	
//	m_pSysFunList.SetAt(FunOper_CLOSE,g_pSystemFun->CLOSE);		   // 函数: CLOSE() 收盘价
//	
//	m_pSysFunList.SetAt(FunOper_DECLINE,g_pSystemFun->DECLINE);    // 函数: DECLINE() 下跌家数
//	
//	m_pSysFunList.SetAt(FunOper_EXTDATA,g_pSystemFun->EXTDATA);    // 函数: EXTDATA(var1) 扩展数据1-11
//	
//	m_pSysFunList.SetAt(FunOper_HIGH,g_pSystemFun->HIGH);    // 函数: HIGH() 最高价
//	
//	m_pSysFunList.SetAt(FunOper_HIGH,g_pSystemFun->HIGH);	    // 函数: HIGH() 最高价
//	
//	m_pSysFunList.SetAt(FunOper_HIGH,g_pSystemFun->HIGH);	    // 函数: HIGH() 最高价
//	
//	m_pSysFunList.SetAt(FunOper_ISBUYORDER,g_pSystemFun->ISBUYORDER);    // 函数: ISBUYORDER() 是否主动性买单
//	
//	m_pSysFunList.SetAt(FunOper_LOW,g_pSystemFun->LOW);     // 函数: LOW() 最低价
//	
//	m_pSysFunList.SetAt(FunOper_LOW,g_pSystemFun->LOW);	   // 函数: LOW() 最低价
//	
//	m_pSysFunList.SetAt(FunOper_LOW,g_pSystemFun->LOW);	   // 函数: LOW() 最低价
//	
//	m_pSysFunList.SetAt(FunOper_OPEN,g_pSystemFun->OPEN);   // 函数: OPEN() 开盘价
//	 
//	m_pSysFunList.SetAt(FunOper_OPEN,g_pSystemFun->OPEN);	   // 函数: OPEN() 开盘价
//	
//	m_pSysFunList.SetAt(FunOper_OPEN,g_pSystemFun->OPEN);	   // 函数: OPEN() 开盘价
//	
//	m_pSysFunList.SetAt(FunOper_SELLVOL,g_pSystemFun->SELLVOL);    // 函数: SELLVOL() 主动性卖单
//	
//	m_pSysFunList.SetAt(FunOper_VOL,g_pSystemFun->VOL);     // 函数: VOL() 成交量
//	
//	m_pSysFunList.SetAt(FunOper_VOL,g_pSystemFun->VOL);       // 函数: VOL() 成交量
//	
//	m_pSysFunList.SetAt(FunOper_VOL,g_pSystemFun->VOL);       // 函数: VOL() 成交量
//	
//	
//	// 时间函数
//	m_pSysFunList.SetAt(FunOper_DATE,g_pSystemFun->hxDATE);    // 函数: DATE() 年月日
//	
//	m_pSysFunList.SetAt(FunOper_PERIOD_TYPE_DAY,g_pSystemFun->hxDAY);    // 函数: PERIOD_TYPE_DAY() 日期
//	
//	m_pSysFunList.SetAt(FunOper_HOUR,g_pSystemFun->HSOUR);    // 函数: HOUR() 小时
//	
//	m_pSysFunList.SetAt(FunOper_MINUTE,g_pSystemFun->hxMINUTE);    // 函数: MINUTE() 分钟
//	
//	m_pSysFunList.SetAt(FunOper_MONTH,g_pSystemFun->hxMONTH);    // 函数: MONTH() 月份
//	
//	m_pSysFunList.SetAt(FunOper_TIME,g_pSystemFun->hxTIME);    // 函数:  TIME() 时分秒
//	
//	m_pSysFunList.SetAt(FunOper_WEEKDAY,g_pSystemFun->hxWEEKDAY);    // 函数: WEEKDAY() 星期
//	
//	m_pSysFunList.SetAt(FunOper_YEAR,g_pSystemFun->hxYEAR);    // 函数: YEAR() 年份
//	
//	
//	//引用函数
//	m_pSysFunList.SetAt(FunOper_BACKSET,g_pSystemFun->BACKSET);    // 函数: BACKSET(var1,var2) 向前赋值
//	
//	m_pSysFunList.SetAt(FunOper_BARSCOUNT,g_pSystemFun->BARSCOUNT);    // 函数:  BARSCOUNT(var1) 有效周期数
//	
//	m_pSysFunList.SetAt(FunOper_BARSLAST,g_pSystemFun->BARSLAST);    // 函数: BARSLAST(var1) 上一次条件成立位置
//	
//	m_pSysFunList.SetAt(FunOper_BARSSINCE,g_pSystemFun->BARSSINCE);    // 函数: BARSSINCE(var1) 第一个条件成立位置
//	
//	m_pSysFunList.SetAt(FunOper_COUNT,g_pSystemFun->COUNT);    // 函数: COUNT(var1,var2) 统计总数
//	
//	m_pSysFunList.SetAt(FunOper_DMA,g_pSystemFun->DMA);    // 函数:  DMA(var1,var2) 动态移动平均
//	
//	m_pSysFunList.SetAt(FunOper_EMA,g_pSystemFun->EMA);    // 函数: EMA(var1,var2) 指数平滑移动平均
//	
//	m_pSysFunList.SetAt(FunOper_HHV,g_pSystemFun->HHV);    // 函数: HHV(var1,var2) 最高值
//	
//	m_pSysFunList.SetAt(FunOper_HHVBARS,g_pSystemFun->HHVBARS);    // 函数: HHVBARS(var1,var2) 上一高点到当前的周期数
//	
//	m_pSysFunList.SetAt(FunOper_LLV,g_pSystemFun->LLV);    // 函数:  LLV(var1,var2) 最低值
//	
//	m_pSysFunList.SetAt(FunOper_LLVBARS,g_pSystemFun->LLVBARS);    // 函数: LLVBARS(var1,var2) 上一低点到当前的周期数
//	
//	m_pSysFunList.SetAt(FunOper_MA,g_pSystemFun->MA);    // 函数: MA(var1,var2) 简单移动平均
//	
//	m_pSysFunList.SetAt(FunOper_REF,g_pSystemFun->REF);    // 函数: REF(var1,var2) 引用若干周期前的数据
//	
//	m_pSysFunList.SetAt(FunOper_SMA,g_pSystemFun->SMA);    // 函数:  SMA(var1,var2,var3) 移动平均
//	
//	m_pSysFunList.SetAt(FunOper_SUM,g_pSystemFun->SUM);    // 函数: SUM(var1,var2) 总和
//	
//	m_pSysFunList.SetAt(FunOper_SUMBARS,g_pSystemFun->SUMBARS);    // 函数: SUMBARS(var1,var2) 向前累加到指定值到现在的周期数
//	
//	
//	// 逻辑函数
//	m_pSysFunList.SetAt(FunOper_IF,g_pSystemFun->IF);    // 函数: IF(var1,var2,var3) 根据条件求不同的值
//	
//	m_pSysFunList.SetAt(FunOper_ISDOWN,g_pSystemFun->ISDOWN);    // 函数: ISDOWN() 该周期是否收阴
//	
//	m_pSysFunList.SetAt(FunOper_ISEQUAL,g_pSystemFun->ISEQUAL);    // 函数:  ISEQUAL() 该周期是否平盘
//	
//	m_pSysFunList.SetAt(FunOper_ISUP,g_pSystemFun->ISUP);    // 函数: ISUP() 该周期是否收阳
//	
//	
//	// 算术函数
//	m_pSysFunList.SetAt(FunOper_ABS,g_pSystemFun->ABS);    // 函数: ABS(var1) 求绝对值
//	
//	m_pSysFunList.SetAt(FunOper_CROSS,g_pSystemFun->CROSS);    // 函数: CROSS(var1,var2) 两条线交叉
//	
//	m_pSysFunList.SetAt(FunOper_MAX,g_pSystemFun->MAX);    // 函数:  MAX(var1,var2) 求最大值
//	
//	m_pSysFunList.SetAt(FunOper_MIN,g_pSystemFun->MIN);    // 函数: MIN(var1,var2) 求最小值
//	
//	m_pSysFunList.SetAt(FunOper_MOD,g_pSystemFun->MOD);    // 函数: MOD(var1,var2) 求模运算
//	
//	m_pSysFunList.SetAt(FunOper_NOT,g_pSystemFun->NOT);    // 函数: NOT(var1) 求逻辑非
//	
//	m_pSysFunList.SetAt(FunOper_REVERSE,g_pSystemFun->REVERSE);    // 函数:  REVERSE(var1) 求相反数
//	
//	m_pSysFunList.SetAt(FunOper_SGN,g_pSystemFun->SGN);    // 函数: SGN(var1) 求符号值
//	
//	
//	// 数学函数
//	m_pSysFunList.SetAt(FunOper_ACOS,g_pSystemFun->ACOS);    // 函数: ACOS(var1) 反余弦值
//	
//	m_pSysFunList.SetAt(FunOper_ASIN,g_pSystemFun->ASIN);    // 函数: ASIN(var1) 反正弦值
//	
//	m_pSysFunList.SetAt(FunOper_ATAN,g_pSystemFun->ATAN);    // 函数:  ATAN(var1) 反正切值
//	
//	m_pSysFunList.SetAt(FunOper_COS,g_pSystemFun->COS);    // 函数: COS(var1) 余弦值
//	
//	m_pSysFunList.SetAt(FunOper_EXP,g_pSystemFun->EXP);    // 函数: EXP(var1) 指数
//	
//	m_pSysFunList.SetAt(FunOper_LN,g_pSystemFun->LN);    // 函数: LN(var1) 求自然对数
//	
//	m_pSysFunList.SetAt(FunOper_LOG,g_pSystemFun->LOG);    // 函数:  LOG(var1) 求10为底的对数
//	
//	m_pSysFunList.SetAt(FunOper_POW,g_pSystemFun->POW);    // 函数: POW(var1,var2) 乘幂
//	
//	m_pSysFunList.SetAt(FunOper_SIN,g_pSystemFun->SIN);    // 函数: SIN(var1) 正弦值
//	
//	m_pSysFunList.SetAt(FunOper_SQRT,g_pSystemFun->SQRT);    // 函数: SQRT(var1) 开平方
//	
//	m_pSysFunList.SetAt(FunOper_TAN,g_pSystemFun->TAN);    // 函数:  TAN(var1) 正切值
//	
//	
//	// 统计函数
//	m_pSysFunList.SetAt(FunOper_AVEDEV,g_pSystemFun->AVEDEV);    // 函数: AVEDEV(var1,var2) 平均绝对偏差
//	
//	m_pSysFunList.SetAt(FunOper_DEVSQ,g_pSystemFun->DEVSQ);    // 函数: DEVSQ(var1,var2) 数据偏差平方和
//	
//	m_pSysFunList.SetAt(FunOper_FORCAST,g_pSystemFun->FORCAST);    // 函数: FORCAST(var1,var2) 线性回归预测值
//	
//	m_pSysFunList.SetAt(FunOper_SLOPE,g_pSystemFun->SLOPE);    // 函数:  SLOPE(var1,var2) 线性回归斜率
//	
//	m_pSysFunList.SetAt(FunOper_STD,g_pSystemFun->STD);    // 函数: STD(var1,var2) 估算标准差
//	
//	m_pSysFunList.SetAt(FunOper_STDP,g_pSystemFun->STDP);    // 函数: STDP(var1,var2) 总体标准差
//	
//	m_pSysFunList.SetAt(FunOper_VAR,g_pSystemFun->VAR);    // 函数: VAR(var1,var2) 估算样本方差
//	
//	m_pSysFunList.SetAt(FunOper_VARP,g_pSystemFun->VARP);    // 函数:  VARP(var1,var2) 总体样本方差
//	
//	
//	// 指数函数
//	m_pSysFunList.SetAt(FunOper_COST,g_pSystemFun->COST);    // 函数: COST(var1) 成本分布情况
//	
//	m_pSysFunList.SetAt(FunOper_PEAK,g_pSystemFun->PEAK);    // 函数: PEAK(var1,var2,var3) 前M个ZIG转向波峰值
//	
//	m_pSysFunList.SetAt(FunOper_PEAKBARS,g_pSystemFun->PEAKBARS);    // 函数: PEAKBARS(var1,var2,var3) 前M个ZIG转向波峰到当前距离
//	
//	m_pSysFunList.SetAt(FunOper_SAR,g_pSystemFun->SAR);    // 函数:  SAR(var1,var2,var3) 抛物转向
//	
//	m_pSysFunList.SetAt(FunOper_SARTURN,g_pSystemFun->SARTURN);    // 函数: SARTURN(var1,var2,var3) 抛物转向点
//	
//	m_pSysFunList.SetAt(FunOper_TROUGH,g_pSystemFun->TROUGH);    // 函数: TROUGH(var1,var2,var3) 前M个ZIG转向波谷值
//	
//	m_pSysFunList.SetAt(FunOper_TROUGHBARS,g_pSystemFun->TROUGHBARS); // 函数: TROUGHBARS(var1,var2,var3) 前M个ZIG转向波谷到当前距离
//	
//	m_pSysFunList.SetAt(FunOper_WINNER,g_pSystemFun->WINNER);    // 函数:  WINNER(var1) 获利盘比例
//	
//	m_pSysFunList.SetAt(FunOper_ZIG,g_pSystemFun->ZIG);    // 函数:  ZIG(var1,var2) 之字转向
//	
//	
//	// 大盘函数
//	m_pSysFunList.SetAt(FunOper_INDEXA,g_pSystemFun->INDEXA);    // 函数: INDEXA() 对应大盘成交额
//	
//	m_pSysFunList.SetAt(FunOper_INDEXADV,g_pSystemFun->INDEXADV);    // 函数: INDEXADV() 对应大盘上涨家数
//	
//	m_pSysFunList.SetAt(FunOper_INDEXC,g_pSystemFun->INDEXC);    // 函数:  INDEXC() 对应大盘收盘价
//	
//	m_pSysFunList.SetAt(FunOper_INDEXDEC,g_pSystemFun->INDEXDEC);    // 函数:  INDEXDEC() 对应大盘下跌家数
//	
//	m_pSysFunList.SetAt(FunOper_INDEXH,g_pSystemFun->INDEXH);    // 函数: INDEXH() 对应大盘最高价
//	
//	m_pSysFunList.SetAt(FunOper_INDEXL,g_pSystemFun->INDEXL);    // 函数: INDEXL() 对应大盘最低价
//	
//	m_pSysFunList.SetAt(FunOper_INDEXO,g_pSystemFun->INDEXO);    // 函数:  INDEXO() 对应大盘开盘价
//	
//	m_pSysFunList.SetAt(FunOper_INDEXV,g_pSystemFun->INDEXV);    // 函数:  INDEXV() 对应大盘成交量
//	
//	
//	// 常用函数
//	m_pSysFunList.SetAt(FunOper_CAPITAL,g_pSystemFun->CAPITAL);    // 函数: CAPITAL() 流通盘大小
//	
//	m_pSysFunList.SetAt(FunOper_VOLUNIT,g_pSystemFun->VOLUNIT);    // 函数: VOLUNIT() 每手股数
//	
//	m_pSysFunList.SetAt(FunOper_DYNAINFO,g_pSystemFun->DYNAINFO);    // 函数:  DYNAINFO(3) 动态行情函数: 昨收
//	
//	m_pSysFunList.SetAt(FunOper_FINANCE,g_pSystemFun->FINANCE);    // 函数:  FINANCE(1) 财务指标: 总股本(万股)
//		
//	
//	// 函数常量
//	      // 画线类型常量
//	m_pSysFunList.SetAt(FunOper_STICK,g_pSystemFun->STICK);  // 柱状线，从0到指标值间画垂直直线
//	
//	m_pSysFunList.SetAt(FunOper_COLORSTICK,g_pSystemFun->COLORSTICK);  // 彩色柱状线，当值为0是显示红色，否则显示绿色
//	
//	m_pSysFunList.SetAt(FunOper_VOLSTICK,g_pSystemFun->VOLSTICK);  // 成交量柱状线，当股价上涨时显示红色空心柱，否则显示绿色空心柱
//	
//	m_pSysFunList.SetAt(FunOper_LINESTICK,g_pSystemFun->LINESTICK);  // 同时画出柱状线和指标线
//	
//	m_pSysFunList.SetAt(FunOper_CROSSDOT,g_pSystemFun->CROSSDOT);  // 小叉线
//	
//	m_pSysFunList.SetAt(FunOper_CIRCLEDOT,g_pSystemFun->CIRCLEDOT);  // 小圆圈线
//	
//	m_pSysFunList.SetAt(FunOper_POINTDOT,g_pSystemFun->POINTDOT);  // 小圆点线
//	
//	m_pSysFunList.SetAt(FunOper_DRAW3D,g_pSystemFun->DRAW3D);    // 函数:  FINANCE(1) 财务指标: 总股本(万股)
//	
//
//	// 五彩K线颜色
//	m_pSysFunList.SetAt(FunOper_COLORRED,g_pSystemFun->COLORRED);  // 红色
//	
//	m_pSysFunList.SetAt(FunOper_COLORGREEN,g_pSystemFun->COLORGREEN);  // 绿色
//	
//	m_pSysFunList.SetAt(FunOper_COLORBLUE,g_pSystemFun->COLORBLUE);  // 蓝色
//	
//	m_pSysFunList.SetAt(FunOper_COLORBLACK,g_pSystemFun->COLORBLACK);  // 黑色
//	
//	m_pSysFunList.SetAt(FunOper_COLORWHITE,g_pSystemFun->COLORWHITE);  // 白色
//	
//	m_pSysFunList.SetAt(FunOper_COLORGRAY,g_pSystemFun->COLORGRAY);  // 灰色
//	
//	m_pSysFunList.SetAt(FunOper_COLORYELLOW,g_pSystemFun->COLORYELLOW);  // 黄色
//	
//	m_pSysFunList.SetAt(FunOper_COLORCYAN,g_pSystemFun->COLORCYAN);  // 青色
//	
//	m_pSysFunList.SetAt(FunOper_COLORMAGENTA,g_pSystemFun->COLORMAGENTA);  // 品红色
//	
//	m_pSysFunList.SetAt(FunOper_COLORBROWN,g_pSystemFun->COLORBROWN);  // 棕色
//	
//
//	m_pSysFunList.SetAt(FunOper_COLORBRGB,g_pSystemFun->hxRGB);  // RGB函数
//	
//
//	// 线风格常量
//	m_pSysFunList.SetAt(FunOper_SOLID,g_pSystemFun->hxSOLID);
//	
//	m_pSysFunList.SetAt(FunOper_DASH,g_pSystemFun->hxDASH);		     /* -------  */
//	
//	m_pSysFunList.SetAt(FunOper_DOT,g_pSystemFun->hxDOT);			     /* .......  */
//	
//	m_pSysFunList.SetAt(FunOper_DASHDOT,g_pSystemFun->hxDASHDOT);		 /* _._._._  */
//	
//	m_pSysFunList.SetAt(FunOper_DASHDOTDOT,g_pSystemFun->hxDASHDOTDOT); /* _.._.._  */
//	
//
//	// 用户绘图函数
//	m_pSysFunList.SetAt(FunOper_DRAWICON,g_pSystemFun->DRAWICON);
//	
//	m_pSysFunList.SetAt(FunOper_DRAWLINE,g_pSystemFun->DRAWLINE);
//	
//	m_pSysFunList.SetAt(FunOper_DRAWTEXT,g_pSystemFun->DRAWTEXT);
//	
//	m_pSysFunList.SetAt(FunOper_POLYLINE,g_pSystemFun->POLYLINE);
//	
//	m_pSysFunList.SetAt(FunOper_STICKLINE,g_pSystemFun->STICKLINE);
//}


//




struct _Auto_Delete_SystemFun_
{
	~_Auto_Delete_SystemFun_()
	{
		if(g_pSystemFun != NULL)
		{
			delete g_pSystemFun;
			g_pSystemFun = NULL;
		}
	}
};
static _Auto_Delete_SystemFun_ _auto_delete_systemfun_;
static CHSDataSource* GetData(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( pCurDataStatus->m_pDataSource )
	{
		return pCurDataStatus->m_pDataSource;
	}
	else if( pCompile )
	{
		if( pCompile->GetData() != NULL )
			return pCompile->GetData();
	}
	return g_pHxDataSource;
}


CSystemFun* CSystemFun::GetSystemFunPtr(CHSDataSource* pHxDataSource)
{
	g_pHxDataSource = pHxDataSource;
	return g_pSystemFun;
}

CHSDataSource* CSystemFun::SetDataSource(CHSDataSource* pHxDataSource)
{
	CHSDataSource* pOldHxDataSource = g_pHxDataSource;
	g_pHxDataSource = pHxDataSource;
	return pOldHxDataSource;
}

int FxjUserDefCoordinate::GetOffset()
{
	int i;
	for( i = 0; i < 7; i++ )
	{
		if( m_fPoint[i] && m_fPoint[i] < 0.0000001)
			return 32;
	}
	int nValid = 0;
	for(i = 0; i < 7; i++)
	{
		if( m_fPoint[i] )
		{
			nValid++;
		}
	}
	if(nValid != m_nCount)
		return 1;

	return 0;
}

BOOL FxjEidolon::IsValid()
{
	long* p = (long*)m_strUnknown;
	if(p && *p == 9)
	{
		return TRUE;
		/*
		p++;
		if(*p == 0x06060606)
		{
			if(m_strUnknown[8] == 0x04)
			{
				return TRUE;
			}
		}
		*/
	}
	return FALSE;
}
#if 0
static int SymbolValue(CHSDataSource* pData,__int64 handle)
{
	if (!pData )
	{
		return 0;
	}
	int count = pData->GetDaySize();
	HSDouble* pCloseData = pData->GetData(hx_CLOSE_DATA);
	HSDouble* pOpenData = pData->GetData(hx_OPEN_DATA);
	HSDouble* pHighData = pData->GetData(hx_HIGH_DATA);
	HSDouble* pLowData = pData->GetData(hx_LOW_DATA);
	//HSDouble* pVolData = pData->GetData(hx_VOL_DATA);
	//HSDouble* pDateData = pData->GetData(hx_DATE_DATA);

	VARIANT_BOOL bRet;
	g_pSystemFun->m_iATIndicator->SetSymbolSize(handle, count);
	for (int i = 0; i < count; i++)
	{
		g_pSystemFun->m_iATIndicator->SetSymbolValue(handle, i, float(pOpenData[i].m_dData), float(pHighData[i].m_dData), float(pLowData[i].m_dData), float(pCloseData[i].m_dData), &bRet);
	}
	return count;
}
#endif
CSystemFun::CSystemFun()
{
	/*m_bLoadCom = FALSE;*/
}

CSystemFun::CSystemFun(CHSDataSource* pHxDataSource)
{
	g_pHxDataSource = pHxDataSource;
	/*m_bLoadCom = FALSE;*/
}

CSystemFun::~CSystemFun()
{

}
#if 0
HSDouble CSystemFun::TLOW (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					   HSCurDataInfoStatus* pCurDataStatus, int& nError)   // 函数: 
{
	if(g_pSystemFun && g_pSystemFun->m_bLoadCom)
	{
		CHSDataSource* pData = NULL;
		pData = GetData(pCompile,ayVars,pCurDataStatus,nError);
		if (!pData)
		{
			return hx_DefMinValues;
		}	
		BSTR tecName;
		g_pSystemFun->m_iATIndicator->GetObjName(0, &tecName);//获得公式名称
		__int64 hHandle;
		g_pSystemFun->m_iATIndicator->CreateObj(tecName, &hHandle);
		int count = SymbolValue(pData,hHandle);//存储数据
		if(count <= 0)
		{
			g_pSystemFun->m_iATIndicator->DeleteObj(hHandle);
			return hx_DefMinValues;
		}
		g_pSystemFun->m_iATIndicator->Calculation(hHandle, 0, count - 1);
		long lCount = 0;
		g_pSystemFun->m_iATIndicator->PlotCount(hHandle, &lCount);
		pCurDataStatus->Alloc(count);
		float dValue;
		VARIANT_BOOL bIsPlotOn;
		for (int i = 0; i < count; i++)
		{
			//for (int j = 0; j < lCount; j++)
			{
				g_pSystemFun->m_iATIndicator->IsPlotOn(hHandle, 1, i, &bIsPlotOn);
				if (bIsPlotOn)
				{
					g_pSystemFun->m_iATIndicator->GetPlotData(hHandle, 1, i, &dValue);
					pCurDataStatus->m_pData[i].m_dData = dValue;
				}
			}
		}
		g_pSystemFun->m_iATIndicator->DeleteObj(hHandle);
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}
HSDouble CSystemFun::THIGH (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						   HSCurDataInfoStatus* pCurDataStatus, int& nError)   // 函数: 
{
	if(g_pSystemFun && g_pSystemFun->m_bLoadCom)
	{
		CHSDataSource* pData = NULL;
		pData = GetData(pCompile,ayVars,pCurDataStatus,nError);
		if (!pData)
		{
			return hx_DefMinValues;
		}	
		BSTR tecName;
		g_pSystemFun->m_iATIndicator->GetObjName(0, &tecName);//获得公式名称
		__int64 hHandle;
		g_pSystemFun->m_iATIndicator->CreateObj(tecName, &hHandle);
		int count = SymbolValue(pData,hHandle);//存储数据
		if(count <= 0)
		{
			g_pSystemFun->m_iATIndicator->DeleteObj(hHandle);
			return hx_DefMinValues;
		}
		g_pSystemFun->m_iATIndicator->Calculation(hHandle, 0, count - 1);
		long lCount = 0;
		g_pSystemFun->m_iATIndicator->PlotCount(hHandle, &lCount);
		pCurDataStatus->Alloc(count);
		float dValue;
		VARIANT_BOOL bIsPlotOn;
		for (int i = 0; i < count; i++)
		{
			//for (int j = 0; j < lCount; j++)
			{
				g_pSystemFun->m_iATIndicator->IsPlotOn(hHandle, 0/*j*/, i, &bIsPlotOn);
				if (bIsPlotOn)
				{
					g_pSystemFun->m_iATIndicator->GetPlotData(hHandle, 0/*j*/, i, &dValue);
					pCurDataStatus->m_pData[i].m_dData = dValue;
				}
			}
		}
		g_pSystemFun->m_iATIndicator->DeleteObj(hHandle);
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}
#endif
// 函数: OPEN(); 开盘价	
HSDouble CSystemFun::OPEN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: HIGH(); 最高价
HSDouble CSystemFun::HIGH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	if(pRefDay == NULL)
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: LOW(); 最低价
HSDouble CSystemFun::LOW(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					     HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: CLOSE(); 收盘价
HSDouble CSystemFun::CLOSE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						   HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}
	
	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

BOOL CSystemFun::CompareCloseOpen(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{	
	return OPEN(pCompile,ayVars,pCurDataStatus,nError) <= CLOSE(pCompile,ayVars,pCurDataStatus,nError);
}

// 函数: VOL(); 成交量
HSDouble CSystemFun::VOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_VOL_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: AMOUNT(); 成交额
HSDouble CSystemFun::AMOUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_AMOUNT_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

#ifdef SUPPORT_NETVALUE
// 函数: NETVALUE(); 国债利率，基金净值
HSDouble CSystemFun::NETVALUE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_NetValue_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}
#endif

// 函数: DATEDATA(); 日期
HSDouble CSystemFun::DATEDATA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 时间函数
// 函数: DATE(); 年月日
HSDouble CSystemFun::hxDATE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);	
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) )
					return pRefDateDay[pCurDataStatus->m_nDataPos];
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) )
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = pRefDateDay[nCur];
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: PERIOD_TYPE_DAY(); 日期
HSDouble CSystemFun::hxDAY(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) )
					return ((long)pRefDateDay[pCurDataStatus->m_nDataPos]%100);
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) )
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = ((long)pRefDateDay[nCur]%100);
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: HOUR(); 小时
HSDouble CSystemFun::HSOUR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) ) // 月日时分
					return ((long)pRefDateDay[pCurDataStatus->m_nDataPos]/100)%100;
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) )
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = ((long)pRefDateDay[nCur]/100)%100;
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: MINUTE(); 分钟
HSDouble CSystemFun::hxMINUTE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) ) // 月日时分
					return (long)pRefDateDay[pCurDataStatus->m_nDataPos]%100;
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) )
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = (long)pRefDateDay[nCur]%100;
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: MONTH(); 月份
HSDouble CSystemFun::hxMONTH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) ) // 月日时分
					return ((long)pRefDateDay[pCurDataStatus->m_nDataPos]/1000000)%100;
				else if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // 年月日
					return ((long)pRefDateDay[pCurDataStatus->m_nDataPos]/100)%100;
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) )
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = ((long)pRefDateDay[nCur]/1000000)%100;
				}
			}
		}
		else if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // 年月日
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = ((long)pRefDateDay[nCur]/100)%100;
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: TIME(); 时分秒
HSDouble CSystemFun::hxTIME(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) ) // 月日时分
					return (long)pRefDateDay[pCurDataStatus->m_nDataPos]%1000000;
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) )  // 月日时分
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = (long)pRefDateDay[nCur]%1000000;
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: WEEKDAY(); 星期
HSDouble CSystemFun::hxWEEKDAY(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}

	CTime curTime;
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // 年月日
				{
					curTime = hxLongToTime((long)pRefDateDay[pCurDataStatus->m_nDataPos]);
					if(curTime != -1)
					{
						return (curTime.GetDayOfWeek() - 1);
					}
				}
				else
				{
					return 0;
				}
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // 年月日
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					curTime = hxLongToTime((long)pRefDateDay[nCur]);
					if(curTime != -1)
					{
						pCurDataStatus->m_pData[nCur] = curTime.GetDayOfWeek() - 1;
					}
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: YEAR(); 年份
HSDouble CSystemFun::hxYEAR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // 年月日
					return ((long)pRefDateDay[pCurDataStatus->m_nDataPos]/10000);
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // 年月日
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = ((long)pRefDateDay[nCur]/10000);
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

//引用函数
// 函数: BACKSET(var1,var2) 向前赋值
HSDouble CSystemFun::BACKSET(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			for( int nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues && 
					calc2.m_pData[nCur].m_dData != hx_DefMinValues)
				{
					if( calc1.m_pData[nCur] )
					{
						for(int nPos = nCur - (long)calc2.m_pData[nCur] + 1;nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
							{
								pCurDataStatus->m_pData[nPos] = 1;
							}
						}
					}
					else
					{
						pCurDataStatus->m_pData[nCur] = 0;
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues && v2 != hx_DefMinValues)
						{
							if( pData[nCur] )
							{
								for(int nPos = nCur - (long)v2 + 1;nPos >= 0 && nPos <= nCur; nPos++)
								{
									if( pData[nPos].m_dData != hx_DefMinValues )
									{
										pCurDataStatus->m_pData[nPos] = 1;
									}
								}
							}
							else
							{
								pCurDataStatus->m_pData[nCur] = 0;
							}
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues && v1 != hx_DefMinValues)
						{
							if( pData[nCur] )
							{
								for(int nPos = nCur - (long)v1 + 1;nPos >= 0 && nPos <= nCur; nPos++)
								{
									if( pData[nPos].m_dData != hx_DefMinValues )
									{
										pCurDataStatus->m_pData[nPos] = 1;
									}
								}
							}
							else
							{
								pCurDataStatus->m_pData[nCur] = 0;
							}
						}
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数:  BARSCOUNT(var1) 有效周期数
HSDouble CSystemFun::BARSCOUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		int nCount = 0;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				nCount++;
				pCurDataStatus->m_pData[nCur] = nCount;
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: BARSLAST(var1) 上一次条件成立位置
HSDouble CSystemFun::BARSLAST(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData    == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL )
	{
		int  nCount = 0;
		BOOL nFirst = FALSE;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				if( curStatus.m_pData[nCur] )
				{
					nCount = 0;
					nFirst = TRUE;
				}

				if( nFirst )
				{
					nCount++;
				}

				pCurDataStatus->m_pData[nCur] = nCount;
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: BARSSINCE(var1) 第一个条件成立位置
HSDouble CSystemFun::BARSSINCE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData    == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL )
	{
		int  nCount = 0;
		BOOL bSucc  = FALSE;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				if( !bSucc && (double)curStatus.m_pData[nCur] )
				{					
					bSucc = TRUE;
				}
				if( bSucc )
				{
					pCurDataStatus->m_pData[nCur] = nCount;
					nCount++;
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: COUNT(var1,var2) 统计总数
HSDouble CSystemFun::COUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int  nCount = 0;
		int  nCur;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			int lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,calc2.m_lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if(calc2.m_pData[nCur].m_dData != hx_DefMinValues)
				{
					lRefPeriod = (long)calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}			
				for(int nPos = ((lRefPeriod <= 0)?0:(nCur - lRefPeriod)); nPos >= 0 && nPos <= nCur;nPos++)
				{
					if( calc1.m_pData[nPos].m_dData != hx_DefMinValues && 
						(double)calc1.m_pData[nPos] )
					{
						nCount++;
					}
				}
				//---2009.11.09 litao注释掉不满足条件nCount为0时不设置数据
				//if(nCount > 0)
				//{
					pCurDataStatus->m_pData[nCur] = nCount;
				//}
				//--------------------------------------------
				nCount = 0;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{									
						for(int nPos = ((v2 <= 0)?0:(nCur-v2)); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								(double)pData[nPos] )
							{
								nCount++;
							}
						}
						//---2009.11.09 litao注释掉不满足条件nCount为0时不设置数据
						//if( nCount > 0 )
						//{
							pCurDataStatus->m_pData[nCur] = nCount;
						//}
						//---------------------------------------------		
						nCount = 0;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{
						for(int nPos = ((v1 <= 0)?0:(v1-nCur)); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								(double)pData[nPos] )
							{
								nCount++;
							}
						}
						//---2009.11.09 litao注释掉不满足条件nCount为0时不设置数据
						//if( nCount > 0 )
						//{
							pCurDataStatus->m_pData[nCur] = nCount;
						//}
						//---------------------------------------------
						nCount = 0;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数:  DMA(var1,var2) 动态移动平均
HSDouble CSystemFun::DMA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int  nCur;
		BOOL bOneTime = TRUE;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{		
			for(nCur = 0; nCur < lSize; nCur++)
			{
				if( calc1.m_pData[nCur] == hx_DefMinValues ||
					calc2.m_pData[nCur] == hx_DefMinValues )
					continue;

				if(bOneTime)
				{
					pCurDataStatus->m_lPreData = calc1.m_pData[nCur];
					bOneTime = FALSE;
				}
				
				pCurDataStatus->m_lPreData = 
							(pCurDataStatus->m_lPreData * ((double)1 - calc2.m_pData[nCur]) + 
							calc1.m_pData[nCur] * calc2.m_pData[nCur]);
				pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(nCur = 0; nCur < lSize; nCur++)
					{
						if( pData[nCur] == hx_DefMinValues )
							continue;
						
						if(bOneTime)
						{
							pCurDataStatus->m_lPreData = pData[nCur];
							bOneTime = FALSE;
						}
						
						pCurDataStatus->m_lPreData = 
							pCurDataStatus->m_lPreData * (1 - v2) + (double)pData[nCur] * v2;
						pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(nCur = 0; nCur < lSize; nCur++)
					{
						if( pData[nCur] == hx_DefMinValues )
							continue;
						
						if(bOneTime)
						{
							pCurDataStatus->m_lPreData = pData[nCur];
							bOneTime = FALSE;
						}
						
						pCurDataStatus->m_lPreData = 
							pCurDataStatus->m_lPreData * (1 - v1) + (double)pData[nCur] * v1;
						pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
					}
				}
			}
		}		
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: EMA(var1,var2) 指数平滑移动平均
HSDouble CSystemFun::EMA( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);
	
	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long N = (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||	N <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		BOOL bOneTime = TRUE;
		pCurDataStatus->Alloc(curStatus.m_lSize);		
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				pCurDataStatus->m_lPreData = 0;
				pCurDataStatus->m_pData[nCur] = 0.0;
				continue;
			}
			if(bOneTime)
			{
				pCurDataStatus->m_lPreData = ( (double)curStatus.m_pData[nCur] * (N - 1) +
											   (double)curStatus.m_pData[nCur] * 2 ) / (N + 1);
				pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
				bOneTime = FALSE;
				continue;
			}
			
			pCurDataStatus->m_lPreData = 
						( (double)pCurDataStatus->m_lPreData * (N - 1) + (double)curStatus.m_pData[nCur] * 2 ) / (N + 1);
			pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: HHV(var1,var2) 最高值
HSDouble CSystemFun::HHV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble curMax = hx_DefMaxValues;
		int  nCur;
		int  nPos;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for( nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					nPos = ((calc2.m_pData[nCur] <= 0)?0:(nCur - (double)calc2.m_pData[nCur] + 1));
					if(calc2.m_pData[nCur] > nCur)
						nPos = 0;
					for(; nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData !=hx_DefMaxValues &&
							curMax < calc1.m_pData[nPos] )
						{
							curMax = calc1.m_pData[nPos];
						}
						if( curMax.m_dData != hx_DefMaxValues )
						{
							pCurDataStatus->m_pData[nCur] = curMax;
						}
					}
				}
				curMax = hx_DefMaxValues;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						nPos = ((v2 <= 0)?0:(nCur - v2 + 1));
						if(v2 > nCur)
							nPos = 0;
						for(; nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData !=hx_DefMaxValues && 
								curMax < pData[nPos].m_dData )
							{				
								curMax = pData[nPos];
							}
							if( curMax.m_dData != hx_DefMaxValues )
							{
								pCurDataStatus->m_pData[nCur] = curMax;
							}
						}
						curMax = hx_DefMaxValues;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						nPos = ((v1 <= 0)?0:(nCur - v1 + 1));
						if(v1 > nCur)
							nPos = 0;
						for(; nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData !=hx_DefMaxValues && 
								curMax < pData[nPos] )
							{							
								curMax = pData[nPos];
							}
							if( curMax.m_dData != hx_DefMaxValues )
							{
								pCurDataStatus->m_pData[nCur] = curMax;
							}
						}
						curMax = hx_DefMaxValues;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: HHVBARS(var1,var2) 上一高点到当前的周期数
HSDouble CSystemFun::HHVBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int nCount = 0;
		int nCur;
		int nPos;
		HSDouble curMax = hx_DefMaxValues;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for( nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					for( nPos = ((calc2.m_pData[nCur] <= 0)?0:(nCur - (double)calc2.m_pData[nCur] + 1));nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues && 
							curMax < calc1.m_pData[nPos] )
						{
							curMax = calc1.m_pData[nPos];
							nCount = 0;
						}
						else if( curMax.m_dData != hx_DefMaxValues )
						{
							nCount++;
						}
					}
				}
				if( curMax.m_dData != hx_DefMaxValues )
				{
					pCurDataStatus->m_pData[nCur] = nCount;
				}
				curMax = hx_DefMaxValues;
				nCount = 0;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for( nPos = ((v2 <= 0)?0:(nCur - v2 + 1));nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								curMax < pData[nPos] )
							{
								curMax = pData[nPos];
								nCount = 0;
							}
							else if( curMax.m_dData != hx_DefMaxValues )
							{
								nCount++;
							}
						}
						if( curMax.m_dData != hx_DefMaxValues )
						{
							pCurDataStatus->m_pData[nCur] = nCount;
						}
						curMax = hx_DefMaxValues;
						nCount = 0;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for( nPos = ((v1 <= 0)?0:(nCur - v1 + 1));nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								curMax < pData[nPos] )
							{
								curMax = pData[nPos];
								nCount = 0;
							}
							else if( curMax.m_dData != hx_DefMaxValues )
							{
								nCount++;
							}
						}
						if( curMax.m_dData != hx_DefMaxValues )
						{
							pCurDataStatus->m_pData[nCur] = nCount;
						}
						curMax = hx_DefMaxValues;
						nCount = 0;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数:  LLV(var1,var2) 最低值
HSDouble CSystemFun::LLV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble curMin = hx_DefMinValues;
		int  nCur;
		int  nPos;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for( nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					nPos = ((calc2.m_pData[nCur] <= 0)?0:(nCur - (double)calc2.m_pData[nCur] + 1));
					if(calc2.m_pData[nCur] > nCur)
						nPos = 0;
					for( ; nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues && 
							curMin > calc1.m_pData[nPos] )
						{							
							curMin = calc1.m_pData[nCur];
						}
						if(curMin.m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[nCur] = curMin;
						}
					}
				}
				curMin = hx_DefMinValues;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						nPos = ((v2 <= 0)?0:(nCur - v2 + 1));
						if(v2 > nCur)
							nPos = 0;
						for(;nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && curMin > pData[nPos] )
							{							
								curMin = pData[nPos];
							}
							if(curMin.m_dData != hx_DefMinValues)
							{
								pCurDataStatus->m_pData[nCur] = curMin;
							}
						}
						curMin = hx_DefMinValues;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						nPos = ((v1 <= 0)?0:(nCur - v1 + 1));
						if(v1 > nCur)
							nPos = 0;
						for(; nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && curMin > pData[nPos] )
							{
								curMin = pData[nPos];
							}
							if(curMin.m_dData != hx_DefMinValues)
							{
								pCurDataStatus->m_pData[nCur] = curMin;
							}
						}
						curMin = hx_DefMinValues;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: LLVBARS(var1,var2) 上一低点到当前的周期数
HSDouble CSystemFun::LLVBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{ 
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int nCount = 0;
		int nCur;
		HSDouble curMin = hx_DefMinValues;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for( nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					for(int nPos = ((calc2.m_pData[nCur] <= 0)?0:(nCur - (double)calc2.m_pData[nCur] + 1));nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues && 
							curMin > calc1.m_pData[nPos] )
						{
							curMin = calc1.m_pData[nPos];
							nCount = 0;
						}
						else if( curMin.m_dData != hx_DefMinValues )
						{
							nCount++;
						}
					}
				}
				if( curMin.m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = nCount;
				}
				curMin = hx_DefMinValues;
				nCount = 0;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for(int nPos = ((v2 <= 0)?0:(nCur - v2 + 1));nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								curMin > pData[nPos] )
							{
								curMin = pData[nPos];
								nCount = 0;
							}
							else if( curMin.m_dData != hx_DefMinValues )
							{
								nCount++;
							}
						}
						if( curMin.m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[nCur] = nCount;
						}
						curMin = hx_DefMinValues;
						nCount = 0;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for(int nPos = ((v1 <= 0)?0:(nCur - v1 + 1));nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								curMin > pData[nPos] )
							{
								curMin = pData[nPos];
								nCount = 0;
							}
							else if( curMin.m_dData != hx_DefMinValues )
							{
								nCount++;
							}
						}
						if( curMin.m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[nCur] = nCount;
						}
						curMin = hx_DefMinValues;
						nCount = 0;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: MA(var1,var2) 简单移动平均
HSDouble CSystemFun::MA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{

	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble curSum = 0;
		int  nCur,nPos;
		BOOL bValid = FALSE;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			long lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			nPos = GetDataValidPos(calc1.m_pData,lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					lRefPeriod = calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}
				lRefPeriod = (((lRefPeriod <= 1)?1:lRefPeriod) - 1);
				if( lRefPeriod <= 0 )
				{
					pCurDataStatus->m_pData[nCur] = calc1.m_pData[nCur];
				}
				else
				{
					for(nPos = (lRefPeriod <= 0)?0:(nCur - lRefPeriod); nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
						{
							curSum += calc1.m_pData[nPos];
							bValid = TRUE;
						}
					}
					if( bValid && lRefPeriod != hx_DefMinValues )
					{
						pCurDataStatus->m_pData[nCur] = curSum/lRefPeriod;
					}
					curSum = 0;
					bValid = FALSE;
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{
						if( v2 <= 0 )
						{
							if( pData[nCur].m_dData != hx_DefMinValues )
							{
								pCurDataStatus->m_pData[nCur] = pData[nCur];
							}
						}
						else
						{
							for( nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos].m_dData != hx_DefMinValues )
								{
									curSum += pData[nPos];
									bValid = TRUE;
								}
							}
							if( bValid )
							{
								pCurDataStatus->m_pData[nCur] = curSum/(v2+1);
							}
							curSum = 0;
							bValid = FALSE;
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{	
						if( v1 <= 0 )
						{
							if( pData[nCur].m_dData != hx_DefMinValues )
							{
								pCurDataStatus->m_pData[nCur] = pData[nCur];
							}
						}
						else
						{
							for( nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos].m_dData != hx_DefMinValues )
								{
									curSum += pData[nPos];
									bValid = TRUE;
								}
							}
							if( bValid )
							{
								pCurDataStatus->m_pData[nCur] = curSum/(v1+1);
							}
							curSum = 0;
							bValid = FALSE;
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;

	/*
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pVar0 = ayVars->GetAt(0);
	LPNODELEMENT pRefData = pVar0->left;
	long lRefPeriod = (long)pCompile->vexp(pVar0->right,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pRefData    == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus;
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		HSDouble curAverage = hx_DefMinValues;
		int  nNext = 0,nCur,nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);

		nCur = GetDataValidPos(curStatus.m_pData,curStatus.m_lSize);
		if(nCur > 0)
		{
			nNext += nCur;
		}
		for( nCur = nCur + lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for( nPos = nNext; nPos >= 0 && nPos <= nCur; nPos++ )
			{
				if( curStatus.m_pData[nPos] != hx_DefMinValues )
				{
					if( curAverage == hx_DefMinValues )
					{
						curAverage = 0;
					}
					curAverage += curStatus.m_pData[nPos];
				}
			}
			if(curAverage != hx_DefMinValues)
			{
				pCurDataStatus->m_pData[nCur] = curAverage/lRefPeriod;
			}

			curAverage = hx_DefMinValues;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
	*/
}

// 函数: REF(var1,var2) 引用若干周期前的数据
HSDouble CSystemFun::REF(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			for( int nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					if( nCur - (long)calc2.m_pData[nCur] >= 0 )
					{
						pCurDataStatus->m_pData[nCur] = calc1.m_pData[nCur - (long)calc2.m_pData[nCur]];
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( nCur - (long)v2 >= 0 )
						{
							pCurDataStatus->m_pData[nCur] = calc1.m_pData[nCur - (long)v2];
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( nCur - (long)v1 >= 0 )
						{
							pCurDataStatus->m_pData[nCur] = calc1.m_pData[nCur - (long)v1];
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数:  SMA(var1,var2,var3) 移动平均
HSDouble CSystemFun::SMA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	LPNODELEMENT pParam2  = ayVars->GetParam(2);

	if(pParam0 == NULL || pParam1 == NULL || pParam2 == NULL)
	{
		return hx_DefMinValues;
	}

	long N = (long)pCompile->vexp(pParam1,pCurDataStatus);
	long M = (long)pCompile->vexp(pParam2,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		N <= 0				  ||
		N < M				  ||
		M <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL)
	{
		BOOL bOneTime = TRUE;
		pCurDataStatus->Alloc(curStatus.m_lSize);		
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
				continue;

			if(bOneTime)
			{
				pCurDataStatus->m_lPreData = ( curStatus.m_pData[nCur] * (N - M) +
											   curStatus.m_pData[nCur] * M ) / N;

				bOneTime = FALSE;
				continue;
			}
			pCurDataStatus->m_lPreData = ( pCurDataStatus->m_lPreData * (N - M) + curStatus.m_pData[nCur] * M ) / N;
			pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: SUM(var1,var2) 总和
HSDouble CSystemFun::SUM(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble curSum = 0;
		int  nCur;
		BOOL bValid = FALSE;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			long lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					lRefPeriod = calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}
				for(int nPos = ((lRefPeriod <= 0)?0:(nCur - lRefPeriod)); nPos >= 0 && nPos <= nCur; nPos++)
				{
					if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
					{
						curSum += calc1.m_pData[nPos];
						bValid = TRUE;
					}
				}
				if( bValid )
				{
					pCurDataStatus->m_pData[nCur] = curSum;
				}
				curSum = 0;
				bValid = FALSE;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{
						for(int nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								curSum += pData[nPos];
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							pCurDataStatus->m_pData[nCur] = curSum;
						}
						curSum = 0;
						bValid = FALSE;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{		
						for(int nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								curSum += pData[nPos];
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							pCurDataStatus->m_pData[nCur] = curSum;
						}
						curSum = 0;
						bValid = FALSE;
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: SUMBARS(var1,var2) 向前累加到指定值到现在的周期数
HSDouble CSystemFun::SUMBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int nCur;
		int nCount = 0;
		HSDouble curSum = 0;
		int nPos;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for( nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					for( nPos = nCur; nPos >= 0; nPos-- )
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
						{
							curSum += calc1.m_pData[nPos];
							nCount++;
						}
						if( curSum >= calc2.m_pData[nCur] )
						{
							pCurDataStatus->m_pData[nCur] = nCount;//-1;
							break;
						}
					}
				}
				nCount = 0;
				curSum = 0;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for( nPos = nCur; nPos >= 0; nPos-- )
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								curSum += pData[nPos];
								nCount++;
							}
							if( curSum >= v2 )
							{
								pCurDataStatus->m_pData[nCur] = nCount;//-1;
								break;
							}
						}
						nCount = 0;
						curSum = 0;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for( nPos = nCur; nPos >= 0; nPos-- )
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								curSum += pData[nPos];
								nCount++;
							}
							if( curSum >= v1 )
							{
								pCurDataStatus->m_pData[nCur] = nCount;//-1;
								break;
							}
						}
						nCount = 0;
						curSum = 0;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: IF(var1,var2,var3) 根据条件求不同的值
HSDouble CSystemFun::IF(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	LPNODELEMENT pParam2  = ayVars->GetParam(2);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL ||
		pParam0  == NULL ||
		pParam1 == NULL ||
		pParam2 == NULL)
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v0 = pCompile->vexp(pParam0,&calc0);
	double v1 = pCompile->vexp(pParam1,&calc1);
	double v2 = pCompile->vexp(pParam2,&calc2);

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		v0 = pCompile->vexp(pParam0,pCurDataStatus);
		v1 = pCompile->vexp(pParam1,pCurDataStatus);
		v2 = pCompile->vexp(pParam2,pCurDataStatus);

		if( v0 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return ( v0?v1:v2 );
	}

	if( (calc0.m_pData != NULL) || (calc1.m_pData != NULL) || (calc2.m_pData != NULL) ) // 一对多，多对一，多对多,
	{
		int i;
		long lSize = max(max(calc0.m_lSize,calc1.m_lSize),calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc0.m_pData != NULL && calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc0.m_pData[i].m_dData != hx_DefMinValues && 
					calc1.m_pData[i].m_dData != hx_DefMinValues &&
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?calc1.m_pData[i]:calc2.m_pData[i]);
				}
			}
		}
		else if( (calc0.m_pData != NULL) && (calc1.m_pData != NULL || calc2.m_pData != NULL) )
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{	
					for(i = 0; i < lSize; i++)
					{
						if( pData[i].m_dData != hx_DefMinValues && 
							calc0.m_pData[i].m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?pData[i]:v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if( pData[i].m_dData != hx_DefMinValues &&
							calc0.m_pData[i].m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?v1:pData[i]);
						}
					}
				}
			}
		}
		else if( (calc0.m_pData == NULL) && (calc1.m_pData != NULL || calc2.m_pData != NULL) )
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v0 != hx_ReturnPointValues && v2 != hx_ReturnPointValues )
				{	
					for(i = 0; i < lSize; i++)
					{
						if( pData[i].m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[i] = (v0?pData[i]:v2);
						}
					}
				}
				else if( v0 != hx_ReturnPointValues && v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if( pData[i].m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[i] = (v0?v1:pData[i]);
						}
					}
				}
			}
		}
		else if( (calc0.m_pData != NULL) && (calc1.m_pData == NULL) && (calc2.m_pData == NULL) )
		{
			if( v1 != hx_ReturnPointValues && v2 != hx_ReturnPointValues )
			{	
				for(i = 0; i < lSize; i++)
				{
					if( calc0.m_pData[i].m_dData != hx_DefMinValues )
					{
						pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?v1:v2);
					}
				}
			}
		}
		
		return hx_ReturnPointValues;
	}
	else
	{
		if( v0 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return ( v0?v1:v2 );
	}

	return hx_DefMinValues;
}

// 函数: ISDOWN();  该周期是否收阴
HSDouble CSystemFun::ISDOWN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSDouble* pRefOpenDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);	
	if(pRefOpenDay == NULL || pRefCloseDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefCloseDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues && 
				pRefOpenDay[pCurDataStatus->m_nDataPos].m_dData  != hx_DefMinValues )
			{
				return pRefCloseDay[pCurDataStatus->m_nDataPos] < pRefOpenDay[pCurDataStatus->m_nDataPos];
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc(GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize());
		for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
		{
			if( pRefCloseDay[nCur].m_dData != hx_DefMinValues && 
				pRefOpenDay[nCur].m_dData  != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = (pRefCloseDay[nCur] < pRefOpenDay[nCur]);
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: ISEQUAL(); 该周期是否平盘
HSDouble CSystemFun::ISEQUAL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSDouble* pRefOpenDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);	
	if(pRefOpenDay == NULL || pRefCloseDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefCloseDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues && 
				pRefOpenDay[pCurDataStatus->m_nDataPos].m_dData  != hx_DefMinValues )
			{
				return (pRefCloseDay[pCurDataStatus->m_nDataPos] == pRefOpenDay[pCurDataStatus->m_nDataPos]);
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc(GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize());
		for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
		{
			if( pRefCloseDay[nCur].m_dData != hx_DefMinValues && 
				pRefOpenDay[nCur].m_dData  != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = (pRefCloseDay[nCur] == pRefOpenDay[nCur]);
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: ISUP();    该周期是否收阳
HSDouble CSystemFun::ISUP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSDouble* pRefOpenDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);	
	if(pRefOpenDay == NULL || pRefCloseDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefCloseDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues && 
				pRefOpenDay[pCurDataStatus->m_nDataPos].m_dData  != hx_DefMinValues )
			{
				return pRefCloseDay[pCurDataStatus->m_nDataPos] > pRefOpenDay[pCurDataStatus->m_nDataPos];
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc(GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize());
		for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
		{
			if( pRefCloseDay[nCur].m_dData != hx_DefMinValues && 
				pRefOpenDay[nCur].m_dData  != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = (pRefCloseDay[nCur] > pRefOpenDay[nCur]);
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}


// 函数: ABS(var1) 求绝对值
HSDouble CSystemFun::ABS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return fabs( var1 );
	}

	// 返回一串数据
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = fabs(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数:  MAX(var1,var2) 求最大值
HSDouble CSystemFun::MAX(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL || pParam1 == NULL )
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	
	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		v1 = pCompile->vexp(pParam0,pCurDataStatus);
		v2 = pCompile->vexp(pParam1,pCurDataStatus);
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return __max( v1,v2 );
	}

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = __max(calc1.m_pData[i],calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = __max(pData[i],v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = __max(v1,pData[i]);
						}
					}
				}
			}
		}
		
		return hx_ReturnPointValues;
	}
	else
	{
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return __max( v1,v2 );
	}

	return hx_DefMinValues;
}

// 函数: MIN(var1,var2) 求最小值
HSDouble CSystemFun::MIN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		v1 = pCompile->vexp(pParam0,pCurDataStatus);
		v2 = pCompile->vexp(pParam1,pCurDataStatus);
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return __min( v1,v2 );
	}

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = __min(calc1.m_pData[i],calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = __min(pData[i],v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = __min(v1,pData[i]);
						}
					}
				}
			}
		}
		
		return hx_ReturnPointValues;
	}
	else
	{
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return __min( v1,v2 );
	}
	
	return hx_DefMinValues;
}

// 统计函数

// 函数: AVEDEV(var1,var2) 平均绝对偏差
HSDouble CSystemFun::AVEDEV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curAvg = 0;
		HSDouble sum = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = lRefPeriod - 1; nCur > 0 && nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				for(nPos = nNext; nPos <= nCur; nPos++)
				{
					if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
					{
						curAvg += curStatus.m_pData[nPos];
					}
				}

				if( curAvg.m_dData != 0 )
				{
					curAvg /= lRefPeriod;
					for(nPos = nNext; nPos <= nCur; nPos++)
					{
						if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
						{
							sum += fabs(curStatus.m_pData[nPos] - curAvg);
						}
					}

					if( sum.m_dData != 0 )
					{ 
						sum /= lRefPeriod;
						pCurDataStatus->m_pData[nCur] = sum;
					}
				}
				
				curAvg = 0;
				sum = 0;
			}
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}

// 函数: DEVSQ(var1,var2) 数据偏差平方和
HSDouble CSystemFun::DEVSQ(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curAvg = 0;
		HSDouble curTotal = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for(nPos = nNext; nPos <= nCur; nPos++)
			{
				if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
				{
					curAvg += curStatus.m_pData[nPos];
				}
			}
			curAvg /= lRefPeriod;
			if( curAvg.m_dData != 0 )
			{
				for(nPos = nNext; nPos <= nCur; nPos++)
				{
					if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
					{
						curTotal += (curStatus.m_pData[nPos] - curAvg)*(curStatus.m_pData[nPos] - curAvg);
					}
				}
				pCurDataStatus->m_pData[nCur] = curTotal;
			}

			curTotal = 0;//curAvg;
			curAvg = 0;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}

//	FORCAST		();    // 函数: FORCAST(var1,var2) 线性回归预测值
//	SLOPE		();    // 函数:  SLOPE(var1,var2) 线性回归斜率

// 函数: STD(var1,var2) 估算标准差
HSDouble CSystemFun::STD(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curTotalofSqr = 0;
		HSDouble curTotal = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		
		int nCur = GetDataValidPos(curStatus.m_pData,curStatus.m_lSize);
		if( nCur > 0)
		{
			nNext += nCur;
		}
		for( nCur = nCur + lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for(nPos = nNext; nPos <= nCur; nPos++)
			{
				if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
				{
					curTotalofSqr += curStatus.m_pData[nPos]*curStatus.m_pData[nPos];
					curTotal	  += curStatus.m_pData[nPos];
				}
			}
			if( curTotalofSqr.m_dData != 0 )
			{
				curTotalofSqr *= lRefPeriod; // 平方和*周期
			}
			if( curTotal.m_dData != 0 )
			{
				curTotal *= curTotal;		 // 和的平方
			}

			if( curTotal.m_dData != 0 && curTotalofSqr.m_dData != 0 )
			{
				pCurDataStatus->m_pData[nCur] = sqrt(abs((double)(curTotalofSqr - curTotal)) / (lRefPeriod*(lRefPeriod - 1)) );
			}

			curTotalofSqr = 0;
			curTotal	  = 0;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}

// 函数: STDP(var1,var2) 总体标准差
HSDouble CSystemFun::STDP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curTotalofSqr = 0;
		HSDouble curTotal = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for(nPos = nNext; nPos <= nCur; nPos++)
			{
				if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
				{
					curTotalofSqr += curStatus.m_pData[nPos]*curStatus.m_pData[nPos];
					curTotal	  += curStatus.m_pData[nPos];
				}
			}
			if( curTotalofSqr.m_dData != 0 )
			{
				curTotalofSqr *= lRefPeriod; // 平方和*周期
			}
			if( curTotal.m_dData != 0 )
			{
				curTotal *= curTotal;		 // 和的平方
			}

			if( curTotal.m_dData != 0 && curTotalofSqr.m_dData != 0 )
			{
				pCurDataStatus->m_pData[nCur] = sqrt(abs((double)(curTotalofSqr - curTotal)) / (lRefPeriod*lRefPeriod) );
			}

			curTotalofSqr = 0;
			curTotal	  = 0;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}

// 函数: VAR(var1,var2) 估算样本方差
HSDouble CSystemFun::VAR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod = (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curTotalofSqr = 0;
		HSDouble curTotal = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for(nPos = nNext; nPos <= nCur; nPos++)
			{
				if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
				{
					curTotalofSqr += curStatus.m_pData[nPos]*curStatus.m_pData[nPos];
					curTotal	  += curStatus.m_pData[nPos];
				}
			}
			if( curTotalofSqr.m_dData != 0 )
			{
				curTotalofSqr *= lRefPeriod; // 平方和*周期
			}
			if( curTotal.m_dData != 0 )
			{
				curTotal *= curTotal;		 // 和的平方
			}

			if( curTotal.m_dData != 0 && curTotalofSqr.m_dData != 0 )
			{
				pCurDataStatus->m_pData[nCur] = (double)(curTotalofSqr - curTotal) / (lRefPeriod*(lRefPeriod - 1));
			}

			curTotalofSqr = 0;
			curTotal	  = 0;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}

// 函数:  VARP(var1,var2) 总体样本方差
HSDouble CSystemFun::VARP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	if(pParam0 == NULL || pParam0 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curTotalofSqr = 0;
		HSDouble curTotal = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for(nPos = nNext; nPos <= nCur; nPos++)
			{
				if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
				{
					curTotalofSqr += curStatus.m_pData[nPos]*curStatus.m_pData[nPos];
					curTotal	  += curStatus.m_pData[nPos];
				}
			}
			if( curTotalofSqr.m_dData != 0 )
			{
				curTotalofSqr *= lRefPeriod; // 平方和*周期
			}
			if( curTotal.m_dData != 0 )
			{
				curTotal *= curTotal;		 // 和的平方
			}

			if( curTotal.m_dData != 0 && curTotalofSqr.m_dData != 0 )
			{
				pCurDataStatus->m_pData[nCur] = (curTotalofSqr - curTotal) / (lRefPeriod*lRefPeriod);
			}

			curTotalofSqr = 0;
			curTotal	  = 0;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}


// 函数常量
// 画线类型常量
HSDouble CSystemFun::STICK(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 柱状线，从0到指标值间画垂直直线
{
	return (HSDouble)0x00000001;
}

HSDouble CSystemFun::COLORSTICK(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 彩色柱状线，当值为0是显示红色，否则显示绿色
{
	return (HSDouble)0x00000002;
}

HSDouble CSystemFun::VOLSTICK(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 成交量柱状线，当股价上涨时显示红色空心柱，否则显示绿色空心柱
{
	return (HSDouble)0x00000004;
}

HSDouble CSystemFun::LINESTICK(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 同时画出柱状线和指标线
{
	return (HSDouble)0x00000008;
}

HSDouble CSystemFun::CROSSDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 小叉线
{
	return (HSDouble)0x00000010;
}

HSDouble CSystemFun::CIRCLEDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 小圆圈线
{
	return (HSDouble)0x00000020;
}

HSDouble CSystemFun::POINTDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 小圆点线
{
	return (HSDouble)0x00000040;
}

HSDouble CSystemFun::DRAW3D(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 3d
{
	return (HSDouble)0x00000200;
}

HSDouble CSystemFun::LINEDRAW3D(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 3d
{
	return (HSDouble)0x00008000;
}

HSDouble CSystemFun::NOTDRAWLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 3d
{
	return (HSDouble)0x00010000;
}

// 当前文本不绘制
HSDouble CSystemFun::NOTDRAWTEXT( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return (HSDouble)0x00020000;
}

// 当前曲线文本都不绘制
HSDouble CSystemFun::NOTDRAWLINETEXT( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return (HSDouble)(0x00010000 | 0x00020000);
}  

HSDouble CSystemFun::NOTDRAW( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return (HSDouble)(HX_NOTEXPRESSDRAW);
}  

HSDouble CSystemFun::NOTTABLE( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return (HSDouble)(HX_NOTTABLE);
}  

HSDouble CSystemFun::NOTMAXMIN( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return (HSDouble)(HX_NOTCALCMAXMIN);
}  

// 五彩K线颜色
HSDouble CSystemFun::COLORRED(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 红色
{
	return RGB(255,0,0);
}

HSDouble CSystemFun::COLORGREEN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 绿色
{
	return RGB(0,255,0);
}

HSDouble CSystemFun::COLORBLUE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 蓝色
{
	return RGB(0,0,255);
}

HSDouble CSystemFun::COLORBLACK(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 黑色
{
	return 0;
}

HSDouble CSystemFun::COLORWHITE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 白色
{
	return RGB(255,255,255);
}

HSDouble CSystemFun::COLORGRAY(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 灰色
{
	return RGB(192,192,192);
}

HSDouble CSystemFun::COLORYELLOW(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 黄色
{
	return RGB(255,255,0);
}

HSDouble CSystemFun::COLORCYAN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 青色
{
	return RGB(0,128,192);
}

HSDouble CSystemFun::COLORMAGENTA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 品红色
{
	return RGB(255,128,255);
}

HSDouble CSystemFun::COLORBROWN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 棕色
{
	return RGB(128,0,0);
}

HSDouble CSystemFun::hxRGB		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)

{
	if(ayVars == NULL || ayVars->GetSize() <= 1)
	{
		nError = 0;
		return 0;
	}

	LPNODELEMENT pVar0 = ayVars->GetParam(0);
	LPNODELEMENT pVar1 = ayVars->GetParam(1);
	LPNODELEMENT pVar2 = ayVars->GetParam(2);

	if( pVar0 == NULL ||
		pVar1 == NULL ||
		pVar2 == NULL )
		return 0;

	return RGB( (BYTE)pCompile->vexp(pVar0,pCurDataStatus),
				(BYTE)pCompile->vexp(pVar1,pCurDataStatus),
				(BYTE)pCompile->vexp(pVar2,pCurDataStatus) );
}

	// 线风格常量
HSDouble CSystemFun::hxSOLID(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return PS_SOLID;
}

HSDouble CSystemFun::hxDASH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)       /* -------  */
{
	return PS_DASH;
}

HSDouble CSystemFun::hxDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)        /* .......  */
{
	return PS_DOT;
}

HSDouble CSystemFun::hxDASHDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)    /* _._._._  */
{
	return PS_DASHDOT;
}

HSDouble CSystemFun::hxDASHDOTDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError) /* _.._.._  */
{
	return PS_DASHDOTDOT;
}

int CSystemFun::GetDataValidPos(HSDouble* pData,int nSize)
{
	if(pData == NULL || nSize == 0) 
		return 0;
	int nCurPos;
	for( nCurPos = 0; nCurPos < nSize; nCurPos++ )
	{
		if( pData[nCurPos].m_dData != hx_DefMinValues )
		{
			break;
		}
	}
	return nCurPos;
}

time_t CSystemFun::hxLongToTime(long lDate)
{
	struct tm atm;
	memset(&atm, 0, sizeof(tm));
	TCHAR szTime[32];
	_stprintf(szTime, _T("%ld"), lDate);
	int nLen = _tcslen(szTime);
	if(nLen <= 8)
	{
		atm.tm_year = int(lDate/10000);
 		if(atm.tm_year < 1900)
 		{
			atm.tm_mon    = atm.tm_year/100 - 1;
			atm.tm_mday   = atm.tm_year%100;
			atm.tm_hour   = (int)((lDate%10000)/100);
			atm.tm_min    = (int)(lDate%100);
			time_t ct     = time(NULL);
			atm.tm_year   = localtime(&ct)->tm_year + 1900;
		}
		else
		{
			atm.tm_mon    = (int)((lDate%10000)/100) - 1;
			atm.tm_mday   = (int)(lDate%100);
			atm.tm_hour   = 0;
			atm.tm_min    = 0;
		}
	}
	else if(nLen > 8)
	{
		atm.tm_year = (int)(lDate/100000000 + 1990);
		int nTemp   = (int)(lDate%100000000)/10000;
		atm.tm_mon  = nTemp/100 - 1;
		atm.tm_mday = nTemp%100;
		atm.tm_hour = (int)((lDate%10000)/100);
		atm.tm_min  = (int)(lDate%100);
	}
	atm.tm_year -= 1900;
	if(atm.tm_mday < 1 || atm.tm_mday > 31 || 
	   atm.tm_mon  < 0 || atm.tm_mon  > 11 ||
	   atm.tm_hour < 0 || atm.tm_hour > 23 ||
	   atm.tm_min  < 0 || atm.tm_min  > 59 ||
	   atm.tm_sec  < 0 || atm.tm_sec  > 59)
	{
		return -1;
	}
 	return mktime(&atm);
}

HSDouble CSystemFun::ADVANCE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::ASKPRICE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ASKPRICE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	switch( (int)v1 )
	{
	case 1: v1 = 34; break;
	case 2: v1 = 35; break;
	case 3: v1 = 36; break;
	//----2009.11.06 litao增加对买卖4、5档盘口的判断
	case 4: v1 = 45; break;
	case 5: v1 = 46; break;
	//---------------------------------------
	default: return hx_DefMinValues;
	}

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)v1);
	
	//nError = -1;
	//return 0;
}

HSDouble CSystemFun::ASKVOL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	switch( (int)v1 )
	{
	case 1: v1 = 31; break;
	case 2: v1 = 32; break;
	case 3: v1 = 33; break;
	//----2009.11.06 litao增加对买卖4、5档盘口的判断
	case 4: v1 = 47; break;
	case 5: v1 = 48; break;
	//---------------------------------------
	default: return hx_DefMinValues;
	}

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)v1);
	
	//nError = -1;
	//return 0;
}
HSDouble CSystemFun::BIDPRICE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	switch( (int)v1 )
	{
	case 1: v1 = 28; break;
	case 2: v1 = 29; break;
	case 3: v1 = 30; break;
	//----2009.11.06 litao增加对买卖4、5档盘口的判断
	case 4: v1 = 41; break;
	case 5: v1 = 42; break;
	//---------------------------------------
	default: return hx_DefMinValues;
	}

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)v1);
	
	//nError = -1;
	//return 0;
}
HSDouble CSystemFun::BIDVOL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	switch( (int)v1 )
	{
	case 1: v1 = 25; break;
	case 2: v1 = 26; break;
	case 3: v1 = 27; break;
	//----2009.11.06 litao增加对买卖4、5档盘口的判断
	case 4: v1 = 43; break;
	case 5: v1 = 44; break;
	//---------------------------------------
	default: return hx_DefMinValues;
	}

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)v1);
	
	//nError = -1;
	//return 0;
}

HSDouble CSystemFun::BUYVOL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;
	
	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)23);
	
	//nError = -1;
	//return 0;
}

HSDouble CSystemFun::DECLINE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::EXTDATA (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::ISBUYORDER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::SELLVOL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;
	
	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)22);
	
	//nError = -1;
	//return 0;
}

HSDouble CSystemFun::CROSS (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int  nCur,nPos;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			nPos = GetDataValidPos(calc1.m_pData,lSize);
			nCur = __max(nCur,nPos);
			nCur++; // 下一个
			//if( nCur <= 0 )
			//	nCur = 1;
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur-1].m_dData != hx_DefMinValues &&
					calc1.m_pData[nCur-1].m_dData != hx_DefMinValues &&
					calc2.m_pData[nCur].m_dData   != hx_DefMinValues &&
					calc1.m_pData[nCur].m_dData   != hx_DefMinValues )
				{
					if( (calc1.m_pData[nCur-1] < calc2.m_pData[nCur-1]) &&
						(calc1.m_pData[nCur]   > calc2.m_pData[nCur]) )
					{
						pCurDataStatus->m_pData[nCur] = 1;
					}
					else
					{
						pCurDataStatus->m_pData[nCur] = 0;
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					nCur++;
					for( ; nCur < lSize; nCur++ )
					{
						if( pData[nCur-1].m_dData != hx_DefMinValues &&
							pData[nCur].m_dData   != hx_DefMinValues )
						{
							if( (pData[nCur-1] < v2) &&
								(pData[nCur]   > v2) )
							{
								pCurDataStatus->m_pData[nCur] = 1;
							}
							else
							{
								pCurDataStatus->m_pData[nCur] = 0;
							}
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					nCur++;
					for( ; nCur < lSize; nCur++ )
					{
						if( pData[nCur-1].m_dData != hx_DefMinValues &&
							pData[nCur].m_dData   != hx_DefMinValues )
						{
							if( (pData[nCur-1] > v1) &&
								(pData[nCur]   < v1) )
							{
								pCurDataStatus->m_pData[nCur] = 1;
							}
							else
							{
								pCurDataStatus->m_pData[nCur] = 0;
							}
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;

//	nError = -1;
//	return 0;
}

HSDouble GetCurrentMod(HSDouble v1,HSDouble v2)
{
	return ((long)((double)v1) % (long)((double)v2)) ;
}

HSDouble CSystemFun::MOD (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL ||
		pParam0 == NULL || 
		pParam1 == NULL )
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	
	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		v1 = pCompile->vexp(pParam0,pCurDataStatus);
		v2 = pCompile->vexp(pParam1,pCurDataStatus);
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		if( v2 == 0 )
		{
			return hx_DefMinValues;
		}

		return GetCurrentMod( v1 ,v2 );
	}

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues &&
					calc2.m_pData[i].m_dData != 0 )
				{
					pCurDataStatus->m_pData[i] = GetCurrentMod(calc1.m_pData[i] ,calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues && v2 != 0)
						{
							pCurDataStatus->m_pData[i] = GetCurrentMod( pData[i] ,v2 );
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues && pData[i].m_dData != 0 )
						{
							pCurDataStatus->m_pData[i] = GetCurrentMod( v1 , pData[i] );
						}
					}
				}
			}
		}
		
		return hx_ReturnPointValues;
	}
	else
	{
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}

		if( v2 == 0 )
		{
			return hx_DefMinValues;
		}

		return GetCurrentMod( v1,v2 );
	}

	return hx_DefMinValues;

	//nError = -1;
	//return 0;
}

HSDouble CSystemFun::NOT (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pRefData,&curStatus);

	if( curStatus.m_pData != NULL )
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = !curStatus.m_pData[nCur];
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::REVERSE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pRefData,&curStatus);

	if( curStatus.m_pData != NULL )
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = (0 - (double)curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::SGN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pRefData,&curStatus);

	if( curStatus.m_pData != NULL )
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = curStatus.m_pData[nCur] > 0?1:
												curStatus.m_pData[nCur] < 0?-1:0;
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::ACOS (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return acos(var1);
	}

	// 返回一串数据
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = acos(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::ASIN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return asin(var1);
	}

	// 返回一串数据
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = asin(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::ATAN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return atan(var1);
	}

	// 返回一串数据
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = atan(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::COS (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return cos(var1);
	}

	// 返回一串数据
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = cos(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::EXP (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return exp(var1);
	}

	// 返回一串数据
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = exp(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::LN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	// e = 2.71828
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pRefData = ayVars->GetParam(0);
	
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pRefData,&curStatus);
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			pCurDataStatus->m_pData[nCur] = log(curStatus.m_pData[nCur])/log(2.71828);
		}
		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if (v1 <= 0)
	{
		return hx_DefMinValues;
	}
	
	return ( log( v1 )/log(2.71828) );
}

HSDouble CSystemFun::LOG (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pRefData = ayVars->GetParam(0);
	
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pRefData,&curStatus);
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			pCurDataStatus->m_pData[nCur] = log((double)curStatus.m_pData[nCur].m_dData)/log((float)10);
		}
		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if (v1 <= 0)
	{
		return hx_DefMinValues;
	}
	
	return ( log( v1 )/log((float)10) );
	//return 0;
}

HSDouble CSystemFun::POW (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	if(pParam0 == NULL || pParam0 == NULL)
		return hx_DefMinValues;

	long lRefPeriod = (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL)
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&curStatus);
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = pow(curStatus.m_pData[nCur],(double)lRefPeriod);
			}
		}
		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	
	return pow(v1,lRefPeriod);
}

HSDouble CSystemFun::SIN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::SQRT (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::TAN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::FORCAST (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble sumx = 0,sumn = 0,sumxn = 0,sum2n = 0,averagex,averagen;
		int  nCur,nPos;
		BOOL bValid = FALSE;
		HSDouble slope,b;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);

		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			long lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			nPos = GetDataValidPos(calc1.m_pData,lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					lRefPeriod = calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}

				lRefPeriod = (((lRefPeriod <= 1)?1:lRefPeriod) - 1);
				for(nPos = (lRefPeriod <= 0)?0:(nCur - lRefPeriod); nPos >= 0 && nPos <= nCur; nPos++)
				{
					if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
					{
						sumx  += calc1.m_pData[nPos];
						sumn  += nPos;
						sumxn += nPos*(double)calc1.m_pData[nPos];
						sum2n += nPos*nPos;
						bValid = TRUE;
					}
				}

				if( bValid && lRefPeriod != hx_DefMinValues && lRefPeriod != 0 )
				{
					averagex = sumx / lRefPeriod;
					averagen = sumn / lRefPeriod;
					slope = (lRefPeriod * sumxn - sumx * sumn) / \
						    (lRefPeriod * sum2n - sumn * sumn);

					/*
					sumx  = 0;
					sumn  = 0;
					bValid = FALSE;
					for(nPos = (lRefPeriod <= 0)?0:(nCur - lRefPeriod); nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos] != hx_DefMinValues )
						{
							sumx += calc1.m_pData[nPos];
							sumn += nPos;
							bValid = TRUE;
						}
					}
					*/
					if( bValid )
					{
						//b = (sumx / lRefPeriod) - slope * ( sumn / lRefPeriod );
						b = averagex - slope * averagen;
						pCurDataStatus->m_pData[nCur] = slope.m_dData * nCur + b;//(double)slope * ( lRefPeriod - 1 ) + b;
					}
				}

				sumx  = 0;
				sumn  = 0;
				sumxn = 0;
				sum2n = 0;
				bValid = FALSE;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{
						for( nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								sumx  += pData[nPos];
								sumn  += nPos;
								sumxn += nPos*(double)pData[nPos];
								sum2n += nPos*nPos;
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							averagex = sumx / (v2+1);
							averagen = sumn / (v2+1);
							slope = ((v2+1) * sumxn - sumx * sumn) / \
									((v2+1) * sum2n - sumn * sumn);

							/*
							sumx  = 0;
							sumn  = 0;
							bValid = FALSE;
							for( nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos] != hx_DefMinValues )
								{
									sumx += pData[nPos];
									sumn += nPos;
									bValid = TRUE;
								}
							}
							*/
							if( bValid )
							{
								//b = (sumx / (v2+1)) - slope * ( sumn / (v2+1) );
								b = averagex - slope * averagen;
								pCurDataStatus->m_pData[nCur] = slope.m_dData * nCur + b;
							}
						}
						
						sumx  = 0;
						sumn  = 0;
						sumxn = 0;
						sum2n = 0;
						bValid = FALSE;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{		
						for( nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								sumx  += pData[nPos];
								sumn  += nPos;
								sumxn += nPos*(double)pData[nPos];
								sum2n += nPos*nPos;
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							averagex = sumx / (v1+1);
							averagen = sumn / (v1+1);
							slope = ((v1+1) * sumxn - sumx * sumn) / \
									((v1+1) * sum2n - sumn * sumn);

							/*
							sumx  = 0;
							sumn  = 0;
							bValid = FALSE;
							for( nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos] != hx_DefMinValues )
								{
									sumx += pData[nPos];
									sumn += nPos;
									bValid = TRUE;
								}
							}
							*/

							if( bValid )
							{
								//b = (sumx / (v1+1)) - slope * ( sumn / (v1+1) );
								b = averagex - slope * averagen;
								pCurDataStatus->m_pData[nCur] = slope.m_dData * nCur + b;//slope * ( (v1+1) - 1 ) + b;
							}
						}

						sumx  = 0;
						sumn  = 0;
						sumxn = 0;
						sum2n = 0;
						bValid = FALSE;
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::SLOPE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble sumx = 0,sumn = 0,sumxn = 0,sum2n = 0,averagex,averagen;
		int  nCur,nPos;
		BOOL bValid = FALSE;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);

		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			long lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			nPos = GetDataValidPos(calc1.m_pData,lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					lRefPeriod = calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}

				lRefPeriod = (((lRefPeriod <= 1)?1:lRefPeriod) - 1);
				for(nPos = (lRefPeriod <= 0)?0:(nCur - lRefPeriod); nPos >= 0 && nPos <= nCur; nPos++)
				{
					if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
					{
						sumx  += calc1.m_pData[nPos];
						sumn  += nPos;
						sumxn += nPos*(double)calc1.m_pData[nPos];
						sum2n += nPos*nPos;
						bValid = TRUE;
					}
				}

				if( bValid && lRefPeriod != hx_DefMinValues && lRefPeriod != 0 )
				{
					//averagex = sumx / lRefPeriod;
					//averagen = sumn / lRefPeriod;
					pCurDataStatus->m_pData[nCur] = (lRefPeriod * sumxn - sumx * sumn) / \
												    (lRefPeriod * sum2n - sumn * sumn);
				}

				sumx  = 0;
				sumn  = 0;
				sumxn = 0;
				sum2n = 0;
				bValid = FALSE;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{
						for( nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								sumx  += pData[nPos];
								sumn  += nPos;
								sumxn += nPos*(double)pData[nPos];
								sum2n += nPos*nPos;
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							/*averagex = sumx / (v2+1);
							averagen = sumn / (v2+1);*/
							pCurDataStatus->m_pData[nCur] = ((v2+1) * sumxn - sumx * sumn) / \
															((v2+1) * sum2n - sumn * sumn);
						}
						
						sumx  = 0;
						sumn  = 0;
						sumxn = 0;
						sum2n = 0;
						bValid = FALSE;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{		
						for( nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								sumx  += pData[nPos];
								sumn  += nPos;
								sumxn += nPos*(double)pData[nPos];
								sum2n += nPos*nPos;
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							//averagex = sumx / (v1+1);
							//averagen = sumn / (v1+1);
							pCurDataStatus->m_pData[nCur] = ((v1+1) * sumxn - sumx * sumn) / \
															((v1+1) * sum2n - sumn * sumn);
						}

						sumx  = 0;
						sumn  = 0;
						sumxn = 0;
						sum2n = 0;
						bValid = FALSE;
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::COST (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::PEAK (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::PEAKBARS (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::SARTURN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::TROUGH (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::TROUGHBARS (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

//-------2009.12.02 litao新增按平均分布计算的成本价---------------------------------------------------
HSDouble CSystemFun::WINNER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	
	if(pParam0 == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;

	HSDouble* pHighDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);
	HSDouble* pVolDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_VOL_DATA);

	//取数据的长度
	int lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
	if (lSize < 1 ) return hx_DefMinValues;

	lSize = max(lSize,curStatus.m_lSize) ;

	//取总股本
    double lCapital = GetData(pCompile,ayVars,pCurDataStatus, nError)->CAPITAL()/10000.0;
	
	pCurDataStatus->Alloc(lSize);

	//定义存放获利量的数组
	//CArray<long*, long*> *payCostData;
	double fSumVol=0.0; double fWinVol=0.0;

	if(curStatus.m_pData != NULL)
	{
		//计算第一天的获利盘
		if(pHighDay[0].m_dData < curStatus.m_pData[0])
			pCurDataStatus->m_pData[0] = 1.0;
		else if(pLowDay[0].m_dData > curStatus.m_pData[0])
			pCurDataStatus->m_pData[0] = 0.0;
		else
		{ 
			if((pHighDay[0].m_dData - pLowDay[0].m_dData)>0.00001)
				pCurDataStatus->m_pData[0] = (curStatus.m_pData[0]-pLowDay[0].m_dData)/(pHighDay[0].m_dData - pLowDay[0].m_dData) ;
			else
				pCurDataStatus->m_pData[0] = 1.0;
		}

		for( int nCur = 1; nCur < lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
				continue;

			fSumVol = 0.0; fWinVol = 0.0;

			//从第二天开始持续计算,计算的开始是从累计换手率达到100%的日期起
			for(int i=nCur; i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < curStatus.m_pData[nCur])
					fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;
				else if(pLowDay[i].m_dData > curStatus.m_pData[nCur])
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(curStatus.m_pData[nCur] - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if (fSumVol > lCapital)
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}
	else  //当参数为常数时
	{
		double nPrice = double(pCompile->vexp(pParam0,&curStatus));

		if (nPrice < 0.0001) return hx_DefMinValues;

		//计算第一天的获利盘
		if(pHighDay[0].m_dData < nPrice)
			pCurDataStatus->m_pData[0] = 1.0;
		else if(pLowDay[0].m_dData > nPrice)
			pCurDataStatus->m_pData[0] = 0.0;
		else
		{ 
			if((pHighDay[0].m_dData - pLowDay[0].m_dData)>0.00001)
				pCurDataStatus->m_pData[0] = (nPrice-pLowDay[0].m_dData)/(pHighDay[0].m_dData - pLowDay[0].m_dData) ;
			else
				pCurDataStatus->m_pData[0] = 1.0;
		}

		for( int nCur = 1; nCur < lSize; nCur++ )
		{
			fSumVol = 0.0; fWinVol = 0.0;

			//从当天依次向前开始持续计算
			for(int i=nCur; i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < nPrice)
					fWinVol = fWinVol + pVolDay[i]/10000.0;
				else if(pLowDay[i].m_dData > nPrice)
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(nPrice - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if (fSumVol > lCapital)
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

//近期获利盘比例
HSDouble CSystemFun::LWINNER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);

	if((pParam0 == NULL ) || (pParam1 == NULL ))
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pParam1,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;
	
	int nDays = int(pCompile->vexp(pParam0,&curStatus));
	if(nDays <= 0) 
		return hx_DefMinValues;

	HSDouble* pHighDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);
	HSDouble* pVolDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_VOL_DATA);

	//取数据的长度
	int lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
	if (lSize < 1 ) return hx_DefMinValues;

	lSize = max(lSize,curStatus.m_lSize) ;

	//取总股本
    double lCapital = GetData(pCompile,ayVars,pCurDataStatus, nError)->CAPITAL()/10000.0;
	
	pCurDataStatus->Alloc(lSize);

	//定义存放获利量的数组
	//CArray<long*, long*> *payCostData;
	double fSumVol=0.0; double fWinVol=0.0;

	if(curStatus.m_pData != NULL)
	{
		//计算第一天的获利盘
		if(pHighDay[0].m_dData < curStatus.m_pData[0])
			pCurDataStatus->m_pData[0] = 1.0;
		else if(pLowDay[0].m_dData > curStatus.m_pData[0])
			pCurDataStatus->m_pData[0] = 0.0;
		else
		{ 
			if((pHighDay[0].m_dData - pLowDay[0].m_dData)>0.00001)
				pCurDataStatus->m_pData[0] = (curStatus.m_pData[0]-pLowDay[0].m_dData)/(pHighDay[0].m_dData - pLowDay[0].m_dData) ;
			else
				pCurDataStatus->m_pData[0] = 1.0;
		}

		for( int nCur = 1; nCur < lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
				continue;

			fSumVol = 0.0; fWinVol = 0.0;

			//从第二天开始持续计算,计算的开始是从累计nDays
			for(int i=nCur; i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < curStatus.m_pData[nCur])
					fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;
				else if(pLowDay[i].m_dData > curStatus.m_pData[nCur])
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(curStatus.m_pData[nCur] - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if ((nCur-i+1) >= (nDays))
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}
	else  //当参数为常数时
	{
		double nPrice = double(pCompile->vexp(pParam0,&curStatus));

		if (nPrice < 0.0001) return hx_DefMinValues;

		//计算第一天的获利盘
		if(pHighDay[0].m_dData < nPrice)
			pCurDataStatus->m_pData[0] = 1.0;
		else if(pLowDay[0].m_dData > nPrice)
			pCurDataStatus->m_pData[0] = 0.0;
		else
		{ 
			if((pHighDay[0].m_dData - pLowDay[0].m_dData)>0.00001)
				pCurDataStatus->m_pData[0] = (nPrice-pLowDay[0].m_dData)/(pHighDay[0].m_dData - pLowDay[0].m_dData) ;
			else
				pCurDataStatus->m_pData[0] = 1.0;
		}

		for( int nCur = 1; nCur < lSize; nCur++ )
		{
			fSumVol = 0.0; fWinVol = 0.0;

			//从当天依次向前开始持续计算
			for(int i=nCur; i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < nPrice)
					fWinVol = fWinVol + pVolDay[i]/10000.0;
				else if(pLowDay[i].m_dData > nPrice)
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(nPrice - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if ((nCur-i+1) >= (nDays))
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

//远期获利盘比例
HSDouble CSystemFun::PWINNER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);

	if((pParam0 == NULL ) || (pParam1 == NULL ))
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pParam1,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;
	
	int nDays = int(pCompile->vexp(pParam0,&curStatus));
	if(nDays < 0) 
		return hx_DefMinValues;

	HSDouble* pHighDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);
	HSDouble* pVolDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_VOL_DATA);

	//取数据的长度
	int lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
	if (lSize < 1 ) return hx_DefMinValues;

	lSize = max(lSize,curStatus.m_lSize) ;

	//取总股本
    double lCapital = GetData(pCompile,ayVars,pCurDataStatus, nError)->CAPITAL()/10000.0;
	
	pCurDataStatus->Alloc(lSize);

	//定义存放获利量的数组
	//CArray<long*, long*> *payCostData;
	double fSumVol=0.0; double fWinVol=0.0;

	if(curStatus.m_pData != NULL)
	{
		//前N前内初始化不显示数据
		for ( int i = 0; i < nDays; i++ )
		{
			if (i >= lSize) break;
			pCurDataStatus->m_pData[i] = hx_DefMinValues ;
		}

		for( int nCur = nDays; nCur < lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
				continue;

			fSumVol = 0.0; fWinVol = 0.0;

			//从第二天开始持续计算,计算的开始是从累计换手率达到100%的日期起
			for(int i=(nCur-nDays); i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < curStatus.m_pData[nCur])
					fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;
				else if(pLowDay[i].m_dData > curStatus.m_pData[nCur])
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(curStatus.m_pData[nCur] - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if (fSumVol > lCapital)
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}
	else  //当参数为常数时
	{
		double nPrice = double(pCompile->vexp(pParam0,&curStatus));

		if (nPrice < 0.0001) return hx_DefMinValues;

		//前N天内初始化不显示数据
		for ( int i = 0; i < nDays; i++ )
		{
			if (i >= lSize) break;
			pCurDataStatus->m_pData[i] = hx_DefMinValues ;
		}

		for( int nCur = nDays; nCur < lSize; nCur++ )
		{
			fSumVol = 0.0; fWinVol = 0.0;

			//从当天依次向前开始持续计算
			for(int i=(nCur-nDays); i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < nPrice)
					fWinVol = fWinVol + pVolDay[i]/10000.0;
				else if(pLowDay[i].m_dData > nPrice)
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(nPrice - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if (fSumVol > lCapital)
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}
//---------------------------------------------------------------------------------------------------------------------------------

HSDouble CSystemFun::ZIG (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXA (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXADV (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXC (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXDEC (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXH (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXO (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXV (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::CAPITAL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	// CAPITAL，返回流通盘大小，单位为手。对于A股得到流通A股，B股得到B股总股本，指数为0

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->CAPITAL();
}

HSDouble CSystemFun::VOLUNIT (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::DYNAINFO (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)v1);
	//return 0;
}

HSDouble CSystemFun::FINANCE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: ADVANCE(var1) 上涨家数
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetFinaceDataByPos((int)v1);
}

// 用户绘图函数
HSDouble CSystemFun::DRAWICON(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 1)
	{
		nError = 0;
		return 0;
	}

	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);
	LPNODELEMENT pParam2 = ayVars->GetParam(2);

	if( pParam0 == NULL ||
		pParam1 == NULL ||
		pParam2 == NULL )
		return 0;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	double v3 = pCompile->vexp(pParam2,&calc3);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDrawOtherData* pHSDrawOtherData;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;

		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			memcpy(pCurDataStatus->m_pData,calc2.m_pData,sizeof(*pCurDataStatus->m_pData)*lSize);

			for( int nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					if( calc1.m_pData[nCur] )
					{
						pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
						//pHSDrawOtherData = new HSDrawOtherData();
						pHSDrawOtherData->m_bType1 = HS_DRAWICON;
						pHSDrawOtherData->m_bType2 = v3;
						pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues )
						{
							if( pData[nCur] )
							{
								pCurDataStatus->m_pData[nCur] = v2;
								
								pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
								//pHSDrawOtherData = new HSDrawOtherData();
								pHSDrawOtherData->m_bType1 = HS_DRAWICON;
								pHSDrawOtherData->m_bType2 = v3;
								pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
							}
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					memcpy(pCurDataStatus->m_pData,pData,sizeof(*pCurDataStatus->m_pData)*lSize);

					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues )
						{
							pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
							//pHSDrawOtherData = new HSDrawOtherData();
							pHSDrawOtherData->m_bType1 = HS_DRAWICON;
							pHSDrawOtherData->m_bType2 = v3;
							pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
						}
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::DRAWLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 1)
	{
		nError = 0;
		return 0;
	}

	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);
	LPNODELEMENT pParam2 = ayVars->GetParam(2);
	LPNODELEMENT pParam3 = ayVars->GetParam(3);
	LPNODELEMENT pParam4 = ayVars->GetParam(4);

	if( pParam0 == NULL ||
		pParam1 == NULL ||
		pParam2 == NULL ||
		pParam3 == NULL ||
		pParam4 == NULL)
		return 0;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	HSCurDataInfoStatus calc4(pCurDataStatus);
	HSCurDataInfoStatus calc5(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	double v3 = pCompile->vexp(pParam2,&calc3);
	double v4 = pCompile->vexp(pParam3,&calc4);
	double v5 = pCompile->vexp(pParam4,&calc5);

	if( calc1.m_pData != NULL && calc2.m_pData != NULL &&
		calc3.m_pData != NULL && calc4.m_pData != NULL )
	{
		HSDrawOtherData* pHSDrawOtherData;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;

		for( int nCur = 0; nCur < lSize; nCur++ ) 
		{
			if( calc1.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				if( (double)calc1.m_pData[nCur] && 
					calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = calc2.m_pData[nCur];
					
					pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
					pHSDrawOtherData->m_bType1 = HS_DRAWLINE;
					pHSDrawOtherData->m_bType4 = (BYTE)v5;    // 线型:0.都不延长；1.延长起始点；2.延长终止点；3.起始点终止点同时延长
					pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
					
					nCur++;
					for( ; nCur < lSize; nCur++ ) 
					{
						if( calc3.m_pData[nCur].m_dData != hx_DefMinValues )
						{
							if( (double)calc3.m_pData[nCur] && 
								calc3.m_pData[nCur].m_dData != hx_DefMinValues )
							{
								pHSDrawOtherData->m_bType2 = LOBYTE((short)nCur);            // 间隔点数
								pHSDrawOtherData->m_bType3 = HIBYTE((short)nCur);
								pHSDrawOtherData->m_dValue = calc4.m_pData[nCur];
								break;
							}
						}
					}

				}
			}
		}
		
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::DRAWTEXT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 1)
	{
		nError = 0;
		return 0;
	}

	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);
	LPNODELEMENT pParam2 = ayVars->GetParam(2);

	if( pParam0 == NULL ||
		pParam1 == NULL ||
		pParam2 == NULL )
		return 0;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	double v3 = pCompile->vexp(pParam2,&calc3);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDrawOtherData* pHSDrawOtherData;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;

		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			memcpy(pCurDataStatus->m_pData,calc2.m_pData,sizeof(*pCurDataStatus->m_pData)*lSize);

			for( int nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					if( calc1.m_pData[nCur] )
					{
						pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
						//pHSDrawOtherData = new HSDrawOtherData();
						pHSDrawOtherData->m_bType1 = HS_DRAWTEXT;
						if( (pParam2->cDataType & DataTypeOperStr) && 
							(pParam2->cDataType & DataText) &&
							pParam2->valoarestr )
						{
							//if( HSDrawData::m_pHSExternAlloc )
                               // pHSDrawOtherData->m_strText = (CString*)(*HSDrawData::m_pHSExternAlloc)((long)pParam2->valoarestr,HSCStringCopy);//new CString( *pParam2->valoarestr );
							//pHSDrawOtherData->m_strText.Format("%s", *(pParam2->valoarestr));	
							memcpy(pHSDrawOtherData->m_cText, pParam2->valoarestr->GetBuffer(),min(sizeof(pHSDrawOtherData->m_cText)-1,pParam2->valoarestr->GetLength()));
						}

						pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues )
						{
							if( pData[nCur] )
							{
								pCurDataStatus->m_pData[nCur] = v2;
								
								pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
								//pHSDrawOtherData = new HSDrawOtherData();
								pHSDrawOtherData->m_bType1 = HS_DRAWTEXT;
								if( (pParam2->cDataType & DataTypeOperStr) && 
									(pParam2->cDataType & DataText) &&
									pParam2->valoarestr )
								{
									//if( HSDrawData::m_pHSExternAlloc )
									//	pHSDrawOtherData->m_strText = (CString*)(*HSDrawData::m_pHSExternAlloc)((long)pParam2->valoarestr,HSCStringCopy);
									//pHSDrawOtherData->m_strText.Format("%s", *(pParam2->valoarestr));
									memcpy(pHSDrawOtherData->m_cText, pParam2->valoarestr->GetBuffer(),min(sizeof(pHSDrawOtherData->m_cText)-1,pParam2->valoarestr->GetLength()));
								}
								
								pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
							}
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					memcpy(pCurDataStatus->m_pData,pData,sizeof(*pCurDataStatus->m_pData)*lSize);

					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues )
						{
							pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
							//pHSDrawOtherData = new HSDrawOtherData();
							pHSDrawOtherData->m_bType1 = HS_DRAWTEXT;
							if( (pParam2->cDataType & DataTypeOperStr) && 
								(pParam2->cDataType & DataText) &&
								pParam2->valoarestr )
							{
								//if( HSDrawData::m_pHSExternAlloc )
								//	pHSDrawOtherData->m_strText = (CString*)(*HSDrawData::m_pHSExternAlloc)((long)pParam2->valoarestr,HSCStringCopy);
								//pHSDrawOtherData->m_strText.Format("%s", *(pParam2->valoarestr));	
								memcpy(pHSDrawOtherData->m_cText, pParam2->valoarestr->GetBuffer(),min(sizeof(pHSDrawOtherData->m_cText)-1,pParam2->valoarestr->GetLength()));
							}
							
							pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
						}
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::POLYLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;

		HSDrawOtherData* pHSDrawOtherData;
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			for( int nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues && 
					calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					if( calc1.m_pData[nCur] )
					{
						pCurDataStatus->m_pData[nCur] = calc2.m_pData[nCur];
						pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
						//pHSDrawOtherData = new HSDrawOtherData();
						pHSDrawOtherData->m_bType1 = HS_POLYLINE;
						pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues && 
							v2 != hx_DefMinValues )
						{
							if( pData[nCur] )
							{
								pCurDataStatus->m_pData[nCur] = v2;
								pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
								//pHSDrawOtherData = new HSDrawOtherData();
								pHSDrawOtherData->m_bType1 = HS_POLYLINE;
								pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
							}
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues && v1 != hx_DefMinValues)
						{
							if( pData[nCur] )
							{
								pCurDataStatus->m_pData[nCur] = v1;
								pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
								//pHSDrawOtherData = new HSDrawOtherData();
								pHSDrawOtherData->m_bType1 = HS_POLYLINE;
								pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
							}
						}
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::STICKLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				   HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 1)
	{
		nError = 0;
		return 0;
	}

	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);
	LPNODELEMENT pParam2 = ayVars->GetParam(2);
	LPNODELEMENT pParam3 = ayVars->GetParam(3);
	LPNODELEMENT pParam4 = ayVars->GetParam(4);

	if( pParam0 == NULL ||
		pParam1 == NULL ||
		pParam2 == NULL ||
		pParam3 == NULL ||
		pParam4 == NULL)
		return 0;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	HSCurDataInfoStatus calc4(pCurDataStatus);
	HSCurDataInfoStatus calc5(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	double v3 = pCompile->vexp(pParam2,&calc3);
	double v4 = pCompile->vexp(pParam3,&calc4);
	double v5 = pCompile->vexp(pParam4,&calc5);

	if( calc1.m_pData != NULL )
	{
		HSDrawOtherData* pHSDrawOtherData;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;

		if( calc2.m_pData != NULL || calc3.m_pData != NULL )
		{
			HSDouble* pData = ((calc2.m_pData != NULL)?calc2.m_pData:calc3.m_pData);
			for( int nCur = 0; nCur < lSize; nCur++ ) 
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues && (double)calc1.m_pData[nCur] )
				{
					if( calc2.m_pData != NULL && calc3.m_pData != NULL )
					{
						if( calc2.m_pData[nCur].m_dData != hx_DefMinValues &&
							calc3.m_pData[nCur].m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[nCur] = calc2.m_pData[nCur];
							
							pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
							//pHSDrawOtherData = new HSDrawOtherData();
							pHSDrawOtherData->m_bType1 = HS_STICKLINE;
							pHSDrawOtherData->m_bType2 = (BYTE)v4;
							pHSDrawOtherData->m_bType3 = (BYTE)v5;
							pHSDrawOtherData->m_dValue = calc3.m_pData[nCur];
							pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
						}
					}
					else
					{
						pCurDataStatus->m_pData[nCur] = pData[nCur];
						
						pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
						//pHSDrawOtherData = new HSDrawOtherData();
						pHSDrawOtherData->m_bType1 = HS_STICKLINE;
						pHSDrawOtherData->m_bType2 = (BYTE)v4;
						pHSDrawOtherData->m_bType3 = (BYTE)v5;
						pHSDrawOtherData->m_dValue = (calc2.m_pData != NULL)?v3:v2;
						pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
					}
				}
			}
		}
		else
		{
			for( int nCur = 0; nCur < lSize; nCur++ ) 
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues && (double)calc1.m_pData[nCur] )
				{
					pCurDataStatus->m_pData[nCur] = (nCur%2)?v2:v3;
					
					pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
					//pHSDrawOtherData = new HSDrawOtherData();
					pHSDrawOtherData->m_bType1 = HS_STICKLINE;
					pHSDrawOtherData->m_bType2 = (BYTE)v4;
					pHSDrawOtherData->m_bType3 = (BYTE)v5;
					pHSDrawOtherData->m_dValue = (nCur%2)?v3:v2;
					pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
				}
			}
		}
		
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

CHSDataSource* CSystemFun::GetDataSource(CExpressionBase* pCompile)
{
	if( pCompile == NULL )
		return NULL;
	return pCompile->GetData();
}

#define Day_Len_SectionCacl		5
#define Vol_OneDay_SectionCacl	0
HSDouble CSystemFun::Get5DayVol(CHSDataSource* pHxData, int nDayPos)
{
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	if( nDayPos < Day_Len_SectionCacl )
		return Vol_OneDay_SectionCacl;

	HSDouble* pVol = pHxData->GetData(hx_VOL_DATA);
	int nCount = pHxData->GetDaySize();
	if( pVol == NULL || nCount <= Day_Len_SectionCacl 
		|| nDayPos >= nCount )
		return (double)hx_DefMinValues;

	HSDouble* pCurPos = pVol + nDayPos;
	int nEnd = Day_Len_SectionCacl;

	HSDouble dReturn = 0;
	for( int i = 0; i < nEnd && pCurPos != NULL; i++, pCurPos--)
	{
		dReturn += pCurPos->m_dData;
	}

	return dReturn.m_dData / nEnd;
}

HSDouble CSystemFun::GetHisLIANGBI(CHSDataSource* pHxData, int nDayPos)
{
	HSDouble d5DayVol = Get5DayVol(pHxData, nDayPos-1);
	if( d5DayVol == (double)hx_DefMinValues )
		return 0;

	HSDouble* pVol = pHxData->GetData(hx_VOL_DATA);

	if(d5DayVol > 0 && fabs(d5DayVol.m_dData) > 0.0001 )
	{
		return (pVol[nDayPos].m_dData/d5DayVol.m_dData);
	}
	else
	{
		return 0;
	}
}

//  n日强度
HSDouble CSystemFun::QIANGDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	int nDays = 5;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;
		}
	}

	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nDayCount = pHxData->GetDaySize();
	if( nDayCount <= nDays )
	{
		nDays = 0;
	}
	else
	{
		nDays = nDayCount - nDays;
	}
	HSDouble* pClose   = pHxData->GetData(hx_CLOSE_DATA);
	HSDouble dPreClose = -1;
	double dUpAmount = 0;
	double dDownAmount = 0;

	for( ; nDays < nDayCount && pClose != NULL; nDays++ )
	{
		if( pClose[nDays].m_dData != (double)hx_DefMinValues )
		{
			if( dPreClose == -1 )
			{
				dPreClose = pClose[nDays];
			}
			else
			{
				double dTemp = pClose[nDays].m_dData - dPreClose.m_dData;
				if( dTemp > 0 )
				{
					dUpAmount += dTemp;
				}
				else
				{
					dDownAmount += dTemp;
				}
				dPreClose = pClose[nDays];
			}
		}
	}

	if( fabs(dUpAmount-dDownAmount) < 0.0001 ) 
	{
		return (double)hx_DefMinValues;
	}

	return (dUpAmount+dDownAmount) / (dUpAmount-dDownAmount) * 100;
	return dUpAmount / (dUpAmount-dDownAmount) * 100;
}

//   涨跌幅度"
HSDouble CSystemFun::ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	HSDouble dOpen = BetweenOpen(pCompile, ayVars, pCurDataStatus, nError);
	HSDouble dClose = BetweenClose(pCompile, ayVars, pCurDataStatus, nError);
	if( dOpen == (double)hx_DefMinValues 
		|| dClose == (double)hx_DefMinValues 
		|| fabs(dOpen.m_dData) <= 0.0001 )
	{
		return (double)hx_DefMinValues;
	}

	return (dClose.m_dData - dOpen.m_dData)*100/dOpen.m_dData;
}

//   换手率
HSDouble CSystemFun::HUANSHOULV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	HSDouble dRetCapital = pHxData->CAPITAL();
	if ( dRetCapital == (double)hx_DefMaxValues || fabs(dRetCapital.m_dData) <= 0.0001 )
	{
		return (double)hx_DefMinValues;
	}

	HSDouble dBetweenVol = BetweenVol(pCompile, ayVars, pCurDataStatus, nError);
	if( dBetweenVol == (double)hx_DefMaxValues )
	{
		return (double)hx_DefMinValues;
	}

	return (100.0*dBetweenVol.m_dData/dRetCapital.m_dData);
}

//   量比幅度"
HSDouble CSystemFun::LIANGBIFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nDays = pHxData->GetDaySize();
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;
		}
	}
	HSDouble dLiangBi;
	HSDouble dAmount = 0;
	int nDayCount = pHxData->GetDaySize();
	int nCount = min(nDayCount, nDays);
	int nValidDays = 0;
	nDayCount--;

	for( int i = 0; i < nCount; i++ )
	{
		dLiangBi = GetHisLIANGBI(pHxData, nDayCount-i);
		if( dLiangBi.m_dData != 0 )
		{
			dAmount += dLiangBi;
			nValidDays++;
		}
	}

	if( nValidDays == 0 )
	{
		return (double)hx_DefMinValues;
	}

	dLiangBi = dAmount.m_dData / (nValidDays);

	return dLiangBi;
}

//   震荡幅度"
HSDouble CSystemFun::ZHENGDANGFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

#ifndef Super_V1_50_ZZF
	HSDouble dOpenPrice = BetweenOpen(pCompile, ayVars, pCurDataStatus, nError);
#endif
	HSDouble dMaxPrice = BetweenHigh(pCompile, ayVars, pCurDataStatus, nError);
	HSDouble dMinPrice = BetweenLow(pCompile, ayVars, pCurDataStatus, nError);

	if( dMaxPrice == (double)hx_DefMinValues || (double)hx_DefMinValues == dMinPrice )
		return (double)hx_DefMinValues;

#ifndef Super_V1_50_ZZF
	if( dOpenPrice == (double)hx_DefMinValues 
		|| fabs(dOpenPrice.m_dData) <= 0.0001 )
		return (double)hx_DefMinValues;
#else
	if( fabs(dMinPrice.m_dData) <= 0.0001 )
		return (double)hx_DefMinValues;
#endif

#ifndef Super_V1_50_ZZF
	return (dMaxPrice.m_dData - dMinPrice.m_dData)/(dOpenPrice.m_dData)*100;
#else
	return (dMaxPrice.m_dData - dMinPrice.m_dData)/(dMinPrice.m_dData)*100;
#endif
}

//   区间市场比例
HSDouble CSystemFun::BetweenShiChangBiLi(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	HSDouble dFinace = pHxData->CAPITAL();
	HSDouble dBetweenVol = BetweenVol(pCompile, ayVars, pCurDataStatus, nError);
	if( (double)hx_DefMaxValues == dFinace 
		|| dFinace == (double)hx_DefMinValues 
		|| dBetweenVol == (double)hx_DefMinValues 
		|| fabs(dFinace.m_dData) <= 0.0001 )
	{
		return (double)hx_DefMinValues;
	}

#ifdef _DEBUG
	if( dFinace.m_dData < 0 )
	{
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T("区间市场比例 流通盘不正确"));
	}
	if( dBetweenVol.m_dData < 0 )
	{
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T("区间市场比例 区间成交量不正确"));
	}
#endif

	return (dBetweenVol.m_dData)*100/dFinace.m_dData;
}

//   区间开盘
HSDouble CSystemFun::BetweenOpen(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nDayCount = pHxData->GetDaySize();
	int nDays = nDayCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;
		}
	}

	int nReturnDays = nDayCount - nDays;

	if( nReturnDays < 0 )
		return (double)hx_DefMinValues;

	if( nDayCount > nReturnDays )
	{
		HSDouble* pOpen   = pHxData->GetData(hx_OPEN_DATA);
		if( pOpen != NULL )
			return pOpen[nReturnDays];
	}

	return (double)hx_DefMinValues;
}

//   区间收盘
HSDouble CSystemFun::BetweenClose(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nDayCount = pHxData->GetDaySize(); //10
	//int nDays = nDayCount;//10
	//if( ayVars != NULL && ayVars->GetSize() >= 1 )
	//{
	//	LPNODELEMENT pItem = ayVars->GetParam(0);
	//	if( pItem != NULL )
	//	{
	//		nDays = (int)pItem->valoare;//1
	//	}
	//}
	//else
	//{
	//	nDays = 1;
	//}

	int nReturnDays = nDayCount - 1;//9

	if( nReturnDays < 0 )
		return (double)hx_DefMinValues;

	if( nDayCount > nReturnDays )
	{
		HSDouble* pOpen   = pHxData->GetData(hx_CLOSE_DATA);
		if( pOpen != NULL )
			return pOpen[nReturnDays];
	}

	return (double)hx_DefMinValues;
}

//   区间最高
HSDouble CSystemFun::BetweenHigh(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();//10
	int nDays = nCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;//1
		}
	}

	nDays = min(nDays, nCount);
	HSDouble dReturn = (double)hx_DefMinValues;
	HSDouble* pHigh   = pHxData->GetData(hx_HIGH_DATA);
	pHigh += (nCount-1);

	for( int i = 0; i < nDays && pHigh != NULL; i++ )
	{
		if( (dReturn < *pHigh || dReturn == (double)hx_DefMinValues) && *pHigh != (double)hx_DefMinValues )
		{
			dReturn = *pHigh;
		}
		pHigh--;
	}

	return dReturn;
}

//   区间最低
HSDouble CSystemFun::BetweenLow(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();//10
	int nDays = nCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;//1
		}
	}
	nDays = min(nDays, nCount);
	HSDouble dReturn = (double)hx_DefMinValues;
	HSDouble* pHigh   = pHxData->GetData(hx_LOW_DATA);
	pHigh += (nCount-1);

	for( int i = 0; i < nDays && pHigh != NULL; i++ )
	{
		if( (dReturn > *pHigh || dReturn == (double)hx_DefMinValues) && *pHigh != (double)hx_DefMinValues )
		{
			dReturn = *pHigh;
		}
		pHigh--;
	}

	return dReturn;
}

//   区间成交量
HSDouble CSystemFun::BetweenVol(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();//10
	int nDays = nCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;//1
		}
	}

	nDays = nCount - nDays;
	if( nDays < 0 )
		nDays = 0;
	HSDouble dReturn = 0;
	HSDouble* pVol = pHxData->GetData(hx_VOL_DATA);
	pVol += nDays;
	for( int i = nDays; i < nCount && pVol != NULL; i++, pVol++ )
	{
		if( *pVol != (double)hx_DefMinValues )
			dReturn += *pVol;
	}

#ifdef _DEBUG
	if( dReturn.m_dData < 0 )
	{
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T("区间成交量 少于0不正确"));
	}
#endif

	return dReturn;
}

//   区间成交额
HSDouble CSystemFun::BetweenAmount(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();//10
	int nDays = nCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;//1
		}
	}

	nDays = nCount - nDays;
	if( nDays < 0 ) nDays = 0;
	HSDouble dReturn = 0;
	HSDouble* pVol = pHxData->GetData(hx_AMOUNT_DATA);
	pVol += nDays;
	for( int i = nDays; i < nCount && pVol != NULL; i++, pVol++ )
	{
		if( *pVol != (double)hx_DefMinValues )
			dReturn += *pVol;
	}

	//dReturn.m_dData *= 1000;

#ifdef _DEBUG
	if( dReturn.m_dData < 0 )
	{
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T("区间成交额 少于0不正确"));
	}
#endif

	return dReturn;
}

// 重心价
HSDouble CSystemFun::CenterPrice(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	return (double)hx_DefMinValues;
}

// 开始时间
HSDouble CSystemFun::BetweenBeginDate(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_STRING;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();
	int nDays = nCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;//1
		}
	}

	nDays = nCount - nDays;
	if( nDays < 0 ) nDays = 0;
	HSDouble dReturn = (double)hx_DefMinValues;
	HSDouble* pVol = pHxData->GetData(hx_DATE_DATA);
	if( pVol != NULL )
	{
		dReturn.m_dData = pVol[nDays].m_dData;
	}

	return dReturn;
}

// 结速时间
HSDouble CSystemFun::BetweenEndDate(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_STRING;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();//10
	if( nCount <= 0 )
	{
		return (double)hx_DefMinValues;
	}

	HSDouble dReturn = (double)hx_DefMinValues;
	HSDouble* pVol = pHxData->GetData(hx_DATE_DATA);
	if( pVol != NULL )
	{
		dReturn.m_dData = pVol[nCount-1].m_dData;
	}

	return dReturn;
}

// 涨跌幅
HSDouble CSystemFun::BK_ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	long lCountHQ;
	if( payNowData == NULL || (lCountHQ = payNowData->GetSize()) <= 0 || pHxData == NULL )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;

	double dTemp = 0;
	long lValid = 0;

	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		StockUserInfo* pInfo = (StockUserInfo*)pHxData->SendDataToWnd((void*)&pItem->m_ciStockCode.GetCode(), 
			HS_COMPILE_DATA_WPARAM_GetStockUserInfo);

		if( pInfo != NULL )
		{
			dTemp += (pShareData->m_nowData.m_lNewPrice - pInfo->m_lPrevClose) * 100.0 / ( pInfo->m_lPrevClose == 0 ? 1 : pInfo->m_lPrevClose);
		}
		else
		{
			dTemp += (pShareData->m_nowData.m_lNewPrice - pShareData->m_nowData.m_lOpen) * 100.0 / ( pShareData->m_nowData.m_lOpen == 0 ? 1 : pShareData->m_nowData.m_lOpen);
		}

		lValid++;
	}
	
	if( lValid == 0 )
	{
		return (double)hx_DefMinValues;
	}

	return dTemp / (lValid);
}

// 权涨跌幅
HSDouble CSystemFun::BK_Q_ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	long lCountHQ;
	if( payNowData == NULL || (lCountHQ = payNowData->GetSize()) <= 0 || pHxData == NULL )
	{
		return (double)hx_DefMinValues;
	}

	CommRealTimeDataEmpty* pItem;
	double dTemp = 0;
	double dCapital;
	double dCapitalAll = 0;
	int lValid = 0;

	AskData sAsk;
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		StockUserInfo* pInfo = (StockUserInfo*)pHxData->SendDataToWnd((void*)&pItem->m_ciStockCode.GetCode(), 
			HS_COMPILE_DATA_WPARAM_GetStockUserInfo);

		memcpy(sAsk.m_pCode, &pItem->m_ciStockCode, sizeof(pItem->m_ciStockCode));
		pHxData->SetAsk(&sAsk);
		pHxData->SetFinaceData(&pItem->m_ciStockCode);
		dCapital = pHxData->CAPITAL();
		if( dCapital == 0 || (double)hx_DefMaxValues == dCapital )
		{
			// 调试日志
			//WRITE_LOG(__FILE__, __LINE__, _T("权涨跌幅 流通盘数据不正确"));
			dCapital = 1;
		}
		if( pInfo != NULL )
		{//2010.10.28 修改为流通市值
			dTemp += dCapital* pShareData->m_nowData.m_lNewPrice * ( pShareData->m_nowData.m_lNewPrice - pInfo->m_lPrevClose ) * 100.0 / (pInfo->m_lPrevClose == 0? 1:pInfo->m_lPrevClose);
		}
		else
		{//2010.10.28 修改为流通市值
			dTemp += dCapital* pShareData->m_nowData.m_lNewPrice * (pShareData->m_nowData.m_lNewPrice - pShareData->m_nowData.m_lOpen) * 100.0 / (pShareData->m_nowData.m_lOpen == 0 ? 1:pShareData->m_nowData.m_lOpen);
		}
		dCapitalAll += dCapital* pShareData->m_nowData.m_lNewPrice;  //2010.10.28 修改为流通市值
		lValid++;
	}

	if( fabs(dCapitalAll) <= 0.0001 )
	{
		return (double)hx_DefMinValues;
	}

	if( lValid <= 0 )
	{
		return (double)hx_DefMinValues;
	}

	return dTemp / dCapitalAll;
}

// 总成交量
HSDouble CSystemFun::BK_Vol(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	long lCountHQ;
	if( payNowData == NULL || (lCountHQ = payNowData->GetSize()) <= 0 )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;

	double dTemp = 0;
	int lValid = 0;

	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}

		if( pShareData->m_nowData.m_nHand <= 0 )
		{
			dTemp += pShareData->m_nowData.m_lTotal/100;
		}
		else
		{
			dTemp += pShareData->m_nowData.m_lTotal;
		}
		lValid++;
	}

#ifdef _DEBUG
	if( dTemp < 0 )
	{
		// 调试日志
		/*WRITE_LOG(__FILE__, __LINE__, _T("总成交量 dTemp < 0"));*/
	}
#endif

	if( lValid <= 0 )
	{
		return (double)hx_DefMinValues;
	}

	return dTemp;
}

// 成交额
HSDouble CSystemFun::BK_Amount(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	if( payNowData == NULL || payNowData->GetSize() <= 0 )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;

	double dTemp = 0;
	int lValid = 0;

	long lCountHQ = payNowData->GetSize();
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		dTemp += pShareData->m_nowData.m_fAvgPrice;
		lValid++;
	}

#ifdef _DEBUG
	if( dTemp < 0 )
	{
		// 调试日志
	/*	WRITE_LOG(__FILE__, __LINE__, _T("成交额 dTemp < 0"));*/
	}
#endif
	if( lValid <= 0 )
	{
		return (double)hx_DefMinValues;
	}
	return dTemp;
}

// 成交比例
HSDouble CSystemFun::BK_Deal(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;
	double dTemp = 0;
	double dCapital;
	double dCapitalAll = 0;
	int lValid = 0;

	long lCountHQ = payNowData->GetSize();
	AskData sAsk;
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}

		memcpy(sAsk.m_pCode, &pItem->m_ciStockCode, sizeof(pItem->m_ciStockCode));
		pHxData->SetAsk(&sAsk);
		pHxData->SetFinaceData(&pItem->m_ciStockCode);
		dCapital = pHxData->CAPITAL();

		if( fabs(dCapital) <= 0.0001 || (double)hx_DefMaxValues == dCapital )
		{
			//return (double)hx_DefMinValues;  //2010.10.28 litao修改当取不到总股本时不返回，也不进行累加
		}
		else
		{
			dCapitalAll += dCapital;
			dTemp += pShareData->m_nowData.m_lTotal;
			lValid++;
		}
	}

	if( fabs(dCapitalAll) <= 0.0001 )
	{
		return (double)hx_DefMinValues;
	}
	if( lValid <= 0 )
	{
		return (double)hx_DefMinValues;
	}

#ifdef _DEBUG
	if( dTemp < 0 )
	{
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T("成交比例 dTemp < 0"));
	}
	if( dCapitalAll < 0 )
	{
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T("成交比例 dCapitalAll < 0"));
	}
#endif

	return dTemp / dCapitalAll;
}

// 换手率
HSDouble CSystemFun::BK_HuanShouLv(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;
	double dTemp = 0;
	HSDouble dCapital;
	int iCount = 0;

	long lCountHQ = payNowData->GetSize();
	AskData sAsk;
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		memcpy(sAsk.m_pCode, &pItem->m_ciStockCode, sizeof(pItem->m_ciStockCode));
		pHxData->SetAsk(&sAsk);
		pHxData->SetFinaceData(&pItem->m_ciStockCode);
		dCapital = pHxData->CAPITAL();

		if( fabs(dCapital.m_dData) <= 0.0001 || (double)hx_DefMaxValues == dCapital.m_dData )
		{
			//return (double)hx_DefMinValues;//2010.10.28 litao修改当取不到总股本时不返回，也不进行累加
		}
		else
		{
			dTemp += pShareData->m_nowData.m_lTotal/ dCapital.m_dData;
			iCount++;
		}
	}

	if( iCount <= 0 )
	{
		return (double)hx_DefMinValues;
	}

	return dTemp / iCount;
}

// 市盈率
HSDouble CSystemFun::BK_ShiYing(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL )
		return (double)hx_DefMinValues;

	double dReturn = 0;
	int iValid = 0;

	long lCountHQ = payNowData->GetSize();
	AskData sAsk;
	for( int i = 0; i < lCountHQ; i++ )
	{
		CommRealTimeDataEmpty* pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem != NULL )
			pShareData = pItem->GetShareData();
		if( pItem != NULL && pShareData->m_nowData.m_lNewPrice > 0)
		{
			memcpy(sAsk.m_pCode, &pItem->m_ciStockCode, sizeof(pItem->m_ciStockCode));
			pHxData->SetAsk(&sAsk);
			pHxData->SetFinaceData(&pItem->m_ciStockCode);
			HSDouble dRet = pHxData->GetFinaceDataByID(COLUMN_FINANCE_PER_INCOME);

			double nQuarter = pHxData->GetFinaceDataByID(0); //20090605 YJT 修改市盈率算法
			//20090605 YJT 修改市盈率算法
			if (abs(nQuarter - 1) < 0.0001)
				nQuarter = 0.25;
			else if (abs(nQuarter - 2) < 0.0001)
				nQuarter = 0.5;
			else if (abs(nQuarter - 3) < 0.0001)
				nQuarter = 0.75;
			else
				nQuarter = 1;

			short lUnit = (short)pHxData->SendDataToWnd(&pItem->m_ciStockCode.m_cCodeType, HS_COMPILE_DATA_WPARAM_GetStockPriceUnit);
			if ( dRet.m_dData == 0 || dRet.m_dData == (double)hx_DefMaxValues )
			{
				continue;
			}
			dRet.m_dData *= (lUnit == 0 ? 1 : lUnit);

			dReturn += pShareData->m_nowData.m_lNewPrice/dRet.m_dData * nQuarter; //20090605 YJT 修改市盈率算法
			iValid++;
		}
	}

	if( iValid <= 0 )
	{
		return (double)hx_DefMinValues;
	}

	//2010.11.05 litao 增加如果亏损，则不计算市盈率
	if (dReturn < 0.0)
		return (double)hx_DefMinValues;

	return (double)dReturn/iValid;
}

// 令涨股
HSDouble CSystemFun::BK_LingZhang(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = 0;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;
	double dTemp = 0;
	double dTempPre = 0;
	double dRet = -1;
	
	long lCountHQ = payNowData->GetSize();
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem != NULL)
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		StockUserInfo* pInfo = (StockUserInfo*)pHxData->SendDataToWnd((void*)&pItem->m_ciStockCode.GetCode(), 
			HS_COMPILE_DATA_WPARAM_GetStockUserInfo);

		if( pInfo != NULL )
		{
			dTemp = (pShareData->m_nowData.m_lNewPrice - pInfo->m_lPrevClose) * 100.0 / pInfo->m_lPrevClose;
		}
		else
		{
			dTemp = (pShareData->m_nowData.m_lNewPrice - pShareData->m_nowData.m_lOpen) * 100.0 / pShareData->m_nowData.m_lOpen;
		}

		if( dTempPre < dTemp && pShareData->m_nowData.m_lNewPrice > 0 )
		{
			dTempPre = dTemp;
			dRet = i;
		}
	}

	return dRet;
}

// 领跌股
HSDouble CSystemFun::BK_LingDie(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = 0;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL)
		return (double)hx_DefMinValues;

	long lCountHQ = payNowData->GetSize();
	CommRealTimeDataEmpty* pItem;
	double dTemp = 0;
	double dTempPre = 0;
	double dRet = -1;
	
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		StockUserInfo* pInfo = (StockUserInfo*)pHxData->SendDataToWnd((void*)&pItem->m_ciStockCode.GetCode(), 
			HS_COMPILE_DATA_WPARAM_GetStockUserInfo);

		if( pInfo != NULL )
		{
			dTemp = (pShareData->m_nowData.m_lNewPrice - pInfo->m_lPrevClose) * 100.0 / pInfo->m_lPrevClose;
		}
		else
		{
			dTemp = (pShareData->m_nowData.m_lNewPrice - pShareData->m_nowData.m_lOpen) * 100.0 / pShareData->m_nowData.m_lOpen;
		}

		if( dTempPre > dTemp && pShareData->m_nowData.m_lNewPrice > 0 )
		{
			dTempPre = dTemp;
			dRet = i;
		}
	}

	return dRet;
}

// 股票数
HSDouble CSystemFun::BK_Num(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Long;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	if( payNowData == NULL || payNowData->GetSize() <= 0 )
		return (double)hx_DefMinValues;

	return payNowData->GetSize();
}

// 涨股比， 用double型表示，低位的5位表示板块总数，大于5位的高位表示上涨的家数
HSDouble CSystemFun::BK_UpRatio(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_ZGB;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL)
		return (double)hx_DefMinValues;

	long lCountHQ = payNowData->GetSize();
	long lUpNum = 0; //上涨股票家数
	CommRealTimeDataEmpty* pItem;

	double dRet = -1;
	
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		StockUserInfo* pInfo = (StockUserInfo*)pHxData->SendDataToWnd((void*)&pItem->m_ciStockCode.GetCode(), 
			HS_COMPILE_DATA_WPARAM_GetStockUserInfo);

		if( pInfo != NULL )
		{
			if(pShareData->m_nowData.m_lNewPrice - pInfo->m_lPrevClose > 0.0)
				lUpNum++;
		}
	}
	dRet = lUpNum * 100000.0 + lCountHQ;
	return dRet;
}


// 指定类型的专业财务数据
// PROFFIN(ID,Begin,End,type,code)
// (ID,类型,开始年份,终止年份,股票代码)
// 共五个参数，必须有两个参数
// 类型：1：第一季度季报，2：中报，3：第三季度季报，4：年报
HSDouble CSystemFun::PROFFIN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = 0;
	return hx_DefMinValues;

	/*
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	if( GetData(pCompile,ayVars,pCurDataStatus,nError) == NULL )
		return hx_DefMinValues;

	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	LPNODELEMENT pParam2  = ayVars->GetParam(2);
	LPNODELEMENT pParam3  = ayVars->GetParam(3);
	LPNODELEMENT pParam4  = ayVars->GetParam(4);

	if( pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	double v0 = 0,v1 = 0,v2 = 0,v3 = 0;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);

	// 财务id或简称
	CString strID;
	if( pParam0 )
	{
		if(pParam0->cDataType & DataTypeOperStr && pParam0->valoarestr)
		{
			strID = *pParam0->valoarestr;
		}
	}

	// 类型
	if( pParam1 )
		v1 = pCompile->vexp(pParam1,&calc1);

	// 开始日期
	if( pCurDataStatus->m_pCalcParam )
		v2 = pCurDataStatus->m_pCalcParam->m_nBeginDate;
	else if( pParam2 )
		v2 = pCompile->vexp(pParam2,&calc2);

	// 终止日期
	if( pCurDataStatus->m_pCalcParam )
		v3 = pCurDataStatus->m_pCalcParam->m_nEndDate;
	else if( pParam3 )
		v3 = pCompile->vexp(pParam2,&calc3);

	// 股票代码
	CString strCode;
	if( pCurDataStatus->m_pCalcParam )
	{
		strCode = pCurDataStatus->m_pCalcParam->m_sCode.GetCode();
	}
	else if( pParam4 )
	{
		if(pParam4->cDataType & DataTypeOperStr && pParam4->valoarestr)
		{
			strCode = *pParam4->valoarestr;
		}
	}

	return GetData(pCompile,ayVars,pCurDataStatus,nError)->PROFFIN(strID,v1,v2,v3,strCode,
		pCurDataStatus,nError);
		*/
}

// 指定日期的专业财务数据
// PROFFINON(ID,前一参照,Begin,End,code)
// (ID,前一参照,开始时间,终止时间,股票代码)
HSDouble CSystemFun::PROFFINON (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	if( GetData(pCompile,ayVars,pCurDataStatus,nError) == NULL )
		return hx_DefMinValues;

	LPNODELEMENT pParam0  = ayVars->GetParam(0); // 字段
	LPNODELEMENT pParam1  = ayVars->GetParam(1); // 开始日期
	LPNODELEMENT pParam2  = ayVars->GetParam(2); // 终止日期
	LPNODELEMENT pParam3  = ayVars->GetParam(3); // 参照数据
	LPNODELEMENT pParam4  = ayVars->GetParam(4); // 偏移日期数
	LPNODELEMENT pParam5  = ayVars->GetParam(5); // 代码

	if( pCompile == NULL ||
		pParam0  == NULL )
		return hx_DefMinValues;

	double v0 = 0,v1 = 0,v2 = 0,v3 = 0,v4 = 0;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	HSCurDataInfoStatus calc4(pCurDataStatus);
	HSCurDataInfoStatus calc5(pCurDataStatus);

	CString strID;	// 财务id或简称
	if( pParam0 )
	{
		if(pParam0->cDataType & DataTypeOperStr && pParam0->valoarestr)
		{
			strID = *pParam0->valoarestr;
		}
	}
	
	// 开始日期
	if( pCurDataStatus->m_pCalcParam )
		v1 = pCurDataStatus->m_pCalcParam->m_nBeginDate;
	else if( pParam1 )
		v1 = pCompile->vexp(pParam1,&calc1);

	// 终止日期
	if( pCurDataStatus->m_pCalcParam )
		v2 = pCurDataStatus->m_pCalcParam->m_nEndDate;
	else if( pParam2 )
		v2 = pCompile->vexp(pParam2,&calc2);

	if( pParam3 )  // 参照数据
	{
		v3 = pCompile->vexp(pParam3,&calc3);
	}

	if( pParam4 )  // 偏移日期数
	{
		v4 = pCompile->vexp(pParam4,&calc4);
	}

	// 股票代码
	CString strCode;
	CString strBlockName; // 板块
	if( pCurDataStatus->m_pCalcParam )
	{
		strCode      = pCurDataStatus->m_pCalcParam->m_sCode.GetCode();
		strBlockName = pCurDataStatus->m_pCalcParam->GetBlockName();
	}
	else if( pParam5 )
	{
		if(pParam5->cDataType & DataTypeOperStr && pParam5->valoarestr)
		{
			strCode = *pParam5->valoarestr;
		}
	}

	if( strBlockName.IsEmpty() ) // 不是板块计算
	{
		return GetData(pCompile,ayVars,pCurDataStatus,nError)->PROFFINON(strID,
			v1,v2,&calc3,v4,strCode,
			pCurDataStatus,nError);
	}

	return GetData(pCompile,ayVars,pCurDataStatus,nError)->BlockCalc(strID,
		strBlockName,
		v1,v2,
		&calc3,
		v4,
		pCurDataStatus,nError); 
}


//2.	PROFSTR				专业财务字符串数据
//PROFSTR(N,M)
//表示取得M个报告期之前的第N号专业财务字符串数据,
//例如PROFSTR(4001,0)表示最近一期第一大股东名称，N的取值请参阅下表
HSDouble CSystemFun::PROFSTR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 0;
}


//3.	PROFFINLAST			专业财务数据截止期位置
//PROFFINLAST(N)
//表示最近报告期的第N号专业财务数据距离现在的周期数，N的取值请参阅下表
HSDouble CSystemFun::PROFFINLAST	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 0;
}


//5.	PROFSTRON			指定日期的专业字符串财务数据
//PROFFINON(N,Y,MD)
//表示取得Y年M月D日的第N号专业财务数据，
//如PROFSTRON(5001,2000,0101)取得2000年1月1日的第一大股东名称，N的取值请参阅下表
HSDouble CSystemFun::PROFSTRON	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 0;
}

//6.	PROFFINTERM		专业财务数据报告期
//PROFFINTERM(M)
//表示取得M个报告期之前的财务报表是年报、中报还是季报
//返回1：第一季度季报，2：中报，3：第三季度季报，4：年报
//例如：若PROFFINTERM(0)等于4，则表示最近一期财务报表是年报。
HSDouble CSystemFun::PROFFINTERM	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 0;
}

//7.	SETPROFFIN			设置专业财务数据
//SETPROFFIN(X)
//在本函数后的其它专业财务函数将受到本次属性设置的影响，表示将专业财务数据属性设定为X，X从低到高每一位表示一个含义
//第1位：包含年报，
//第2位：包含中报，
//第3位：包含季报，
//第4位：包含最新财务指标，
//第5位：调整中报、季报财务指标，
//例如SETPROFFIN(01011)表示取得年报、中报，最新一期数据，无论是那个报告期都要包括在内，中报不作调整
//如果不调用本函数，系统默认值为01111
HSDouble CSystemFun::SETPROFFIN	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::FOR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	/*
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	LPNODELEMENT pParam2  = ayVars->GetParam(2);
	if(pParam0 == NULL || pParam1 == NULL || pParam2 == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);

	double v0 = pCompile->vexp(pParam0,&calc0);
	double v1 = pCompile->vexp(pParam1,&calc1);
	//double v2 = pCompile->vexp(pParam2,&calc2);

	if( v1 == hx_DefMinValues )
		return hx_DefMinValues;

	int nCount = ayVars->GetSize();

	for( int i = 3; v1 && i < nCount; i++ )
	{
		HSCurDataInfoStatus calc(pCurDataStatus);
		LPNODELEMENT pParam  = ayVars->GetParam(i);

		pCompile->vexp(pParam,&calc);
	}

	*/
	nError = -1;
	return 0;
}

HSDouble CSystemFun::CONTINUE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	//nError = -1;
	return 0;
}

HSDouble CSystemFun::BREAK	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	//nError = -1;
	return 1;
}

HSDouble CSystemFun::OPERATOR   (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 1;
	/*
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	LPNODELEMENT pParam2  = ayVars->GetParam(2);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( //GetData(pCompile,ayVars,pCurDataStatus,nError)->GetDaySize() <= 0 || 
		pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v0 = pCompile->vexp(pParam0,&calc0);
	double v1 = pCompile->vexp(pParam1,&calc1);
	double v2 = pCompile->vexp(pParam2,&calc2);

	// 一对多，多对一，多对多,
	if( (calc0.m_pData != NULL) || (calc1.m_pData != NULL) || (calc2.m_pData != NULL) ) 
	{
	}
	else if( v0 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}

	switch( (int)v2 )
	{
	case 0: // +
		return (v0 + v2);
	case 1: // -
		return (v0 - v2);
	case 2: // *
		return (v0 * v2);
	case 3: // /
		if( v2 )
			return (v0 / v2);
		else
			break;
	case 4: // =
		v0 = v2;
		return v0;
	}

	return hx_DefMinValues;

	//return hx_DefMinValues;
	*/
}

// 字符串连接输出：如果是数字则转换成字符串,只能是字符串和数字
HSDouble CSystemFun::STROUT	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(pCurDataStatus == NULL || pCompile == NULL || ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	pCurDataStatus->Alloc(ayVars->GetSize());

	HSDrawOtherData* pHSDrawOtherData;
	LPNODELEMENT pParam;

	for( int i = 0; i < ayVars->GetSize(); i++ )
	{
		pParam = ayVars->GetParam(i);
		if( pParam == NULL ||
			pParam->valoarestr == NULL ||
			!((pParam->cDataType & DataTypeOperStr) && (pParam->cDataType & DataText)) )
			continue;

		pHSDrawOtherData = pCurDataStatus->m_pData[i].AllocOther(NULL,TRUE);
		if( pHSDrawOtherData )
			//pHSDrawOtherData->m_strText.Format("%s", *(pParam->valoarestr));
			memcpy(pHSDrawOtherData->m_cText,pParam->valoarestr->GetBuffer(),min(sizeof(pHSDrawOtherData->m_cText)-1,pParam->valoarestr->GetLength()));
	}

	return hx_ReturnPointValues;
}

// 日期转换函数:(date,format)
HSDouble CSystemFun::DATETOSTR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(pCompile == NULL || pCurDataStatus == NULL || ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( pParam0 == NULL || pParam1 == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
//	double v2 = pCompile->vexp(pParam1,&calc2);

	if( pParam1->valoarestr == NULL ||
		!((pParam1->cDataType & DataTypeOperStr) && (pParam1->cDataType & DataText)) )
		return hx_DefMinValues;

	HSDrawOtherData* pHSDrawOtherData;
	CString strFormat = *pParam1->valoarestr;
	CTime tm;
	long lDate;
	if( calc1.m_pData != NULL )
	{
		long lSize = calc1.m_lSize;
		pCurDataStatus->Alloc(lSize);
		int nCur = GetDataValidPos(calc1.m_pData,lSize);
		for( ; nCur < lSize; nCur++ )
		{
			pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
			if( pHSDrawOtherData )
			{
				pCurDataStatus->m_pData[nCur].m_dData = calc1.m_pData[nCur].m_dData;
				lDate = calc1.m_pData[nCur].m_dData;
				tm    = LongToDate(lDate);
				if( tm > 0 )
				{
					CString strText;
					strText.Format("%s", tm.Format(strFormat));
					memcpy(pHSDrawOtherData->m_cText, strText.GetBuffer(),min(sizeof(pHSDrawOtherData->m_cText)-1,strText.GetLength()));
					//pHSDrawOtherData->Alloc(&tm.Format(strFormat));
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::BLOCKCALC(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError)
{

	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	if( GetData(pCompile,ayVars,pCurDataStatus,nError) == NULL )
		return hx_DefMinValues;

	LPNODELEMENT pParam0  = ayVars->GetParam(0); // 字段
	LPNODELEMENT pParam1  = ayVars->GetParam(1); // 板块名称
	LPNODELEMENT pParam2  = ayVars->GetParam(2); // 开始日期
	LPNODELEMENT pParam3  = ayVars->GetParam(3); // 终止日期
	LPNODELEMENT pParam4  = ayVars->GetParam(4); // 参照数据
	LPNODELEMENT pParam5  = ayVars->GetParam(5); // 偏移日期数
	LPNODELEMENT pParam6  = ayVars->GetParam(6); // 代码
	LPNODELEMENT pParam7  = ayVars->GetParam(7); // 操作

	if( pCompile == NULL ||
		pParam0  == NULL )
		return hx_DefMinValues;

	double v0 = 0,v1 = 0,v2 = 0,v3 = 0,v4 = 0,v5 = 0,v6 = 0;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	HSCurDataInfoStatus calc4(pCurDataStatus);
	HSCurDataInfoStatus calc5(pCurDataStatus);
	HSCurDataInfoStatus calc6(pCurDataStatus);
	HSCurDataInfoStatus calc7(pCurDataStatus);

	// 财务id或简称
	CString strID;	
	if( pParam0 )
	{
		if(pParam0->cDataType & DataTypeOperStr && pParam0->valoarestr)
		{
			strID = *pParam0->valoarestr;
		}
	}

	// 板块名称
	CString strBlockName;
	if( pParam1 )
	{
		if(pParam1->cDataType & DataTypeOperStr && pParam1->valoarestr)
		{
			strBlockName = *pParam1->valoarestr;
		}
	}
	if( strBlockName.IsEmpty() )
	{
		if( pCurDataStatus->m_pCalcParam )
			strBlockName = pCurDataStatus->m_pCalcParam->GetBlockName();
	}
	if( strBlockName.IsEmpty() )
	{
		return hx_DefMinValues;
	}
	
	// 开始日期
	if( pCurDataStatus->m_pCalcParam )
		v2 = pCurDataStatus->m_pCalcParam->m_nBeginDate;
	else if( pParam1 )
		v2 = pCompile->vexp(pParam2,&calc2);

	// 终止日期
	if( pCurDataStatus->m_pCalcParam )
		v3 = pCurDataStatus->m_pCalcParam->m_nEndDate;
	else if( pParam3 )
		v3 = pCompile->vexp(pParam3,&calc3);

	if( pParam4 )  // 参照数据
	{
		v4 = pCompile->vexp(pParam4,&calc4);
	}

	if( pParam5 )  // 偏移日期数
	{
		v5 = pCompile->vexp(pParam5,&calc5);
	}

	// 操作
	if( pParam6 )  
	{
		v6 = pCompile->vexp(pParam6,&calc6);
	}


	return GetData(pCompile,ayVars,pCurDataStatus,nError)->BlockCalc(strID,
		strBlockName,
		v2,v3,
		&calc4,
		v5,
		pCurDataStatus,nError,
		v6);	
}


// 弘历通-彩虹 
HSDouble CSystemFun::HLTCH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble curSum = 0;
		int  nCur,nPos;
		int  i,j;
		i = 0;
		j = 0;
		BOOL bValid = FALSE;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			long lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			nPos = GetDataValidPos(calc1.m_pData,lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					lRefPeriod = calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}
				lRefPeriod = (((lRefPeriod <= 1)?1:lRefPeriod) - 1);
				if( lRefPeriod <= 0 )
				{
					pCurDataStatus->m_pData[nCur] = calc1.m_pData[nCur];
				}
				else
				{
					i = 0;
					j = 0;
					for(nPos = (lRefPeriod <= 0)?0:(nCur - lRefPeriod); nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
						{
							i = i + 1;
							curSum += (double)i * calc1.m_pData[nPos];
							bValid = TRUE;
							j = j + i;
						}
					}
					if( bValid && lRefPeriod != hx_DefMinValues )
					{
						pCurDataStatus->m_pData[nCur] = curSum / (double)j;
					}
					curSum = 0;
					bValid = FALSE;
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{
						if( v2 <= 0 )
						{
							if( pData[nCur].m_dData != hx_DefMinValues )
							{
								pCurDataStatus->m_pData[nCur] = pData[nCur];
							}
						}
						else
						{
							i = 0;
					        j = 0;
							for( nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos].m_dData != hx_DefMinValues )
								{
									i = i + 1;
									curSum += ((double)i * pData[nPos]);
									bValid = TRUE;
									
						            j = j + i;
								}
							}
							if( bValid )
							{
								pCurDataStatus->m_pData[nCur] =  curSum / (double)j;
							}
							curSum = 0;
							bValid = FALSE;
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{	
						if( v1 <= 0 )
						{
							if( pData[nCur].m_dData != hx_DefMinValues )
							{
								pCurDataStatus->m_pData[nCur] = pData[nCur];
							}
						}
						else
						{
                            i = 0;
					        j = 0;
							for( nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos].m_dData != hx_DefMinValues )
								{
									i = i + 1;
									curSum += ((double)i * pData[nPos]);
									bValid = TRUE;
									j = j + i;
								}
							}
							if( bValid )
							{
								pCurDataStatus->m_pData[nCur] =  curSum/(double)j;
							}
							curSum = 0;
							bValid = FALSE;
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 弘历通-红白圈
HSDouble CSystemFun::HLTHBQ(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

    HSDouble* pRefHighDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pRefLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);	
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);

	if( curStatus.m_pData != NULL )
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;
		return HLTHBQ1(lRefPeriod,curStatus.m_lSize,pCurDataStatus,pRefHighDay,pRefLowDay,pRefCloseDay,nError);
	}
	return hx_DefMinValues;
}

// 弘历通-红白圈
HSDouble CSystemFun::HLTHBQ1(long lRefPeriod, long lRefCount,HSCurDataInfoStatus* pCurDataStatus,
				 HSDouble* pRefHighDay,HSDouble* pRefLowDay,HSDouble* pRefCloseDay, int& nError)
{
	HSDrawOtherData* pHSDrawOtherData;

	HSDouble curSAR = 0;
	HSDouble curXK = 0;
	for( int nCur = lRefPeriod ; nCur < lRefCount; nCur++ )
	{
		if( nCur < 3 )
		{
			curSAR = pRefHighDay[nCur];
            pCurDataStatus->m_pData[nCur] = pRefHighDay[nCur];

			if( pCurDataStatus->m_pData[nCur].m_pOtherData == NULL )
			{
				pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
				//pHSDrawOtherData = new HSDrawOtherData();
				pHSDrawOtherData->m_bType1 = HS_HLTHBQ;
				pHSDrawOtherData->m_bType2 = 2;
				pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
			}

			continue;
		}
        
		if( nCur == lRefPeriod)
		{
            curSAR = pRefHighDay[nCur];
			for( int k = 1; k <= 3; k ++)
			{
				if( pRefHighDay[nCur - k] > curSAR )
					curSAR = pRefHighDay[nCur - k];
			}
            continue;
			//pCurDataStatus->m_pData[nCur] = curSAR;
		}
        
		curXK = curXK + 0.02;
		if (curXK > 0.2 )
			curXK = 0.2;
        curSAR = curSAR + (curXK * (pRefLowDay[nCur - 1] - curSAR));
        pCurDataStatus->m_pData[nCur] = curSAR;

		if( pCurDataStatus->m_pData[nCur].m_pOtherData == NULL )
		{
			pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
			//pHSDrawOtherData = new HSDrawOtherData();
			pHSDrawOtherData->m_bType1 = HS_HLTHBQ;
			if( lRefPeriod == (nCur - 1) )
				pHSDrawOtherData->m_bType2 = 2;
			else
				pHSDrawOtherData->m_bType2 = 2;
			pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
		}

        if(curSAR < pRefCloseDay[nCur])
		{
			return HLTHBQ2(nCur,lRefCount,pCurDataStatus,pRefHighDay,pRefLowDay,pRefCloseDay,nError);
		}
	}
	return hx_ReturnPointValues;
}


// 弘历通-红白圈2
HSDouble CSystemFun::HLTHBQ2(long lRefPeriod, long lRefCount,HSCurDataInfoStatus* pCurDataStatus,
				 HSDouble* pRefHighDay,HSDouble* pRefLowDay,HSDouble* pRefCloseDay, int& nError)
{
	HSDrawOtherData* pHSDrawOtherData;
	HSDouble curSAR = 0;
	HSDouble curXK = 0;
	for( int nCur = lRefPeriod ; nCur < lRefCount; nCur++ )
	{
		if( nCur < 3 )
		{
			curSAR = pRefHighDay[nCur];
            pCurDataStatus->m_pData[nCur] = pRefHighDay[nCur];

			if( pCurDataStatus->m_pData[nCur].m_pOtherData == NULL )
			{
				pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
				//pHSDrawOtherData = new HSDrawOtherData();
				pHSDrawOtherData->m_bType1 = HS_HLTHBQ;
				pHSDrawOtherData->m_bType2 = 1;
				pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
			}

			continue;
		}
        
		if( nCur == lRefPeriod)
		{
            curSAR = pRefLowDay[nCur];
			for( int k = 1; k <= 3; k ++)
			{
				if( pRefLowDay[nCur - k] < curSAR )
					curSAR = pRefLowDay[nCur - k];
			}

			if( pCurDataStatus->m_pData[nCur].m_pOtherData == NULL )
			{
				pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
				//pHSDrawOtherData = new HSDrawOtherData();
				pHSDrawOtherData->m_bType1 = HS_HLTHBQ;
				pHSDrawOtherData->m_bType2 = 1;
				pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
			}
			pCurDataStatus->m_pData[nCur] = curSAR;

            continue;
		}
        
		curXK = curXK + 0.02;
		if (curXK > 0.2 )
			curXK = 0.2;
        curSAR = curSAR + (curXK * (pRefHighDay[nCur - 1] - curSAR));
        pCurDataStatus->m_pData[nCur] = curSAR;

		//
		if( pCurDataStatus->m_pData[nCur].m_pOtherData == NULL )
		{
			pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
			//pHSDrawOtherData = new HSDrawOtherData();
			pHSDrawOtherData->m_bType1 = HS_HLTHBQ;
			pHSDrawOtherData->m_bType2 = 1;
			pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
		}

        if(curSAR > pRefCloseDay[nCur])
		{
			return HLTHBQ1(nCur,lRefCount,pCurDataStatus,pRefHighDay,pRefLowDay,pRefCloseDay,nError);
		}
	}
	return hx_ReturnPointValues;
}

//******************************以下是litao新增*****************************************
HSDouble CSystemFun::SAR (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // 函数: 抛物线转向
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pParam0  = ayVars->GetParam(0);	//周期
	LPNODELEMENT pParam1  = ayVars->GetParam(1);	//调整系数
	LPNODELEMENT pParam2  = ayVars->GetParam(2);	//调整系数上限

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);

	long lRefPeriod =  (long)pCompile->vexp(pParam0,&calc0);
	if (lRefPeriod < 1)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	double AF =  (double)pCompile->vexp(pParam1,&calc1)/100.0;
	double MaxAF =  (double)pCompile->vexp(pParam2,&calc2)/100.0;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;

	HSDouble* pRefHighDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pRefLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);	
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);
	HSDouble* pRefOpenDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);


	long lSize;
	HSDouble curSAR = 0.0;
	double curEP = 0.0;double curAF = AF;
	int i,j; bool bUp;  //当前计算是上涨周期还是下跌周期 true为上涨周期,的false为下跌周期

	//取数据的长度
	lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() ;

	if(lSize>lRefPeriod)
	{
		//lSize = calc0.m_lSize;
		pCurDataStatus->Alloc(lSize+1);

		//初始化
		for (i = 0;i<lSize;i++)
		{
			pCurDataStatus->m_pData[i] = hx_DefMinValues;
		}

		//判断上升或下跌周期，计算SAR(0)
		if (pRefCloseDay[lRefPeriod-1] < pRefCloseDay[0])  //看涨
		{
			bUp = true;
			pCurDataStatus->m_pData[lRefPeriod] = 9999999.0;
			//前n日的最低价
			for (i = 0;i<lRefPeriod;i++)
			{
				if (pRefLowDay[i] < pCurDataStatus->m_pData[lRefPeriod])
					pCurDataStatus->m_pData[lRefPeriod] = pRefLowDay[i];
			}

		}
		else	//看跌
		{
			bUp = false;
			pCurDataStatus->m_pData[lRefPeriod] = 0.0;
			//前n日的最高价
			for (i = 0;i<lRefPeriod;i++)
			{
				if (pRefHighDay[i] > pCurDataStatus->m_pData[lRefPeriod])
					pCurDataStatus->m_pData[lRefPeriod] = pRefHighDay[i];
			}

		}

		for (i = lRefPeriod+1;i<lSize;i++)  //依次计算各个SAR值
		{

			//如果(n-1)日是低开高收的，则EP(n-1)便取其最高价，否则取最低价
			if (pRefCloseDay[i-1] >= pRefOpenDay[i-1])
				curEP = pRefHighDay[i-1];
			else
				curEP = pRefLowDay[i-1];

			//上涨趋势如果当天的最高价比前天的最高价还高,则AF递增0.02,反之为下跌趋势
			if ((i-2) >= 0)
			{	
				if (((pRefHighDay[i] > pRefHighDay[i-2]) && (bUp)) || ((pRefLowDay[i] < pRefLowDay[i-2]) && (!bUp)))
					curAF = min(MaxAF,curAF + 0.02);
			}

			pCurDataStatus->m_pData[i] = pCurDataStatus->m_pData[i-1] + curAF * (curEP - pCurDataStatus->m_pData[i-1]);


			//如果在看涨行情中，计算出某日的SAR值比当日的最低价高，则应以当日的最低价为该日的SAR值；
			//如果在看跌行情中，计算出的某日的SAR值比当日的最高价低，则应以当日的最高价为某日的SAR值。
			if ((bUp) && (pRefLowDay[i] <= pCurDataStatus->m_pData[i])) //上升周期突破
			{
				////pCurDataStatus->m_pData[i] = min(pCurDataStatus->m_pData[i],pRefLowDay[i]);
				curAF = AF; 
				bUp = !bUp;

				//重新计算下一个SAR(0)
				pCurDataStatus->m_pData[i+1] = 0.0;
				//前n日的最高价
				for (j = i - lRefPeriod +1;j< i ;j++)
				{
					if (pRefHighDay[j] > pCurDataStatus->m_pData[i+1])
						pCurDataStatus->m_pData[i+1] = pRefHighDay[j];
				}
				i++;
			}
			else if	((!bUp) && (pRefHighDay[i] >= pCurDataStatus->m_pData[i])) //上升周期突破
			{
				////pCurDataStatus->m_pData[i] = max(pCurDataStatus->m_pData[i],pRefHighDay[i]);
				curAF = AF; 
				bUp = !bUp;

				//重新计算下一个SAR(0) 
				pCurDataStatus->m_pData[i+1] = 9999999.0;
				for (j = i - lRefPeriod +1;j< i ;j++)
				{
					if (pRefLowDay[j] < pCurDataStatus->m_pData[i+1])
						pCurDataStatus->m_pData[i+1] = pRefLowDay[j];
				}
				i++;
			}			

		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}


//过滤函数，满足条件的N个周期后的数据设置为0
HSDouble CSystemFun::FILTER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL ||
		pParam0  == NULL ||
		pParam1 == NULL )
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);

	double v0 = pCompile->vexp(pParam0,&calc0);
	int N1 = 0;
	N1 = (int)pCompile->vexp(pParam1,&calc1);

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		v0 = pCompile->vexp(pParam0,pCurDataStatus);

		if( v0 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}	
		return ( v0?1:0);
	}

	if( (calc0.m_pData != NULL)) 
	{
		int i,j;
		long lSize =calc0.m_lSize; 
		pCurDataStatus->Alloc(lSize);
		if(( calc0.m_pData != NULL) && (N1 > 0))
		{
			i = 0;
			while( i < lSize)
			{
				if( calc0.m_pData[i].m_dData != hx_DefMinValues)
				{
					pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?1:0);
					if (calc0.m_pData[i])  //满足条件则后面N周期的置0
					{
						for(j = 1; j <= N1; j++)
						{
							if ((i+j) < lSize)
								pCurDataStatus->m_pData[i+j] = (calc0.m_pData[i]?0:0);
						}
						i = i + N1+1;
					}
					else
						i++;
				}
				else
					i++;
			}
		}
		else if( (calc0.m_pData == NULL) && (N1 <= 0) )
		{
			i = 0;
			while( i < lSize)
			{
				if( calc0.m_pData[i].m_dData != hx_DefMinValues)
				{
					pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?1:0);
					if (calc0.m_pData[i])
					{
						for(j = i+1; j < lSize; j++)
						{
							pCurDataStatus->m_pData[j] = 0;
						}
						i = lSize;
						break;
					}	
				}
				i++;
			}
		}		
		return hx_ReturnPointValues;
	}
	else
	{
		if( v0 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return ( v0?1:0);
	}

	return hx_DefMinValues;
}


//函数的说明在..\truck\bin\HQClient\Setting\FunctionHelp.cfg
//由于参数中不能访问内部绘图表，以函数形式实现WHILE有困难
HSDouble CSystemFun::WHILE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CExpression* pExpression = ((CExpression*)pCompile);

	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pParam0  = ayVars->GetParam(0);  //条件表达式计算根节点
	LPNODELEMENT pParam1  = ayVars->GetParam(1);  //循环内语句第一个输出的位置
	LPNODELEMENT pParam2  = ayVars->GetParam(2);  //循环内语句最后一个输出的位置

	//无数据/条件判断语句退出
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL ||
		pParam0  == NULL || pParam1  == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);

	double v0 = pExpression->vexp(pParam0,&calc0);
	if( v0 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}

	int N1 = -1; int N2 = -1; 
	if (NULL  != pParam1) 
		N1 = (int)pCompile->vexp(pParam1,&calc1);

	if (NULL  != pParam2) 	
		N2 = (int)pCompile->vexp(pParam2,&calc2);


	try
	{	

		CMapVariabile* pVar = pExpression->GetExpDraw();
		//CMapVariabile* pVar = g_hxData.GetExpDraw("test1",m_strExpName.m_dStyle & 0x00FF);

		if (NULL == pVar)
			pVar = (pExpression->m_pRefExpress)->GetExpDraw();

		if( pVar == NULL ||
			pVar->GetObjArray() == NULL )
		{
			AddError(new nodErrorInfo(hxError_Fun_param));
			return hx_DefMinValues;
		}

		while (v0) 
		{
			// 一个一个中间标识进行计算
			CArray<CLineProperty*,CLineProperty*> ayData;
			CExpValue* pCurVal;
			CLineProperty* pData;
			for(int nObj = N1; nObj <= N2; nObj++)
			{
				pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
				if( pCurVal != NULL )
				{
					pData = new CLineProperty;
					if( pCurDataStatus )
					{
						pData->m_pDataSource = pCurDataStatus->m_pDataSource;
					}
					ayData.Add(pData);

					pCurVal->CalcValue(pData,0,0,NULL);

				}
			}
			_delArrayObj(ayData);

			//重新计算条件值
			v0 = pCompile->vexp(pParam0,&calc0);
		}

		return 0;
	}
	catch(...)
	{
	}

	return hx_DefMinValues; // return 0;

}


// 函数: CEILING(var1) 向上舍入取整
HSDouble CSystemFun::CEILING(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return ceil( var1 );
	}

	// 返回一串数据
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = ceil(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: FLOOR(var1) 向下舍入取整
HSDouble CSystemFun::FLOOR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						   HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return floor( var1 );
	}

	// 返回一串数据
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = floor(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// 函数: ROUND(var1) 四舍五入取整
HSDouble CSystemFun::ROUND(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						   HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// 返回单值
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return (int)( var1 );
	}

	// 返回一串数据
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = (int)(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

/*HSDouble CSystemFun::RBTREND(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						   HSCurDataInfoStatus* pCurDataStatus, int& nError)
{

	if (FAILED(g_pSystemFun->m_hrATIndicator))
    {
		nError = 0;
		return hx_DefMinValues;
	}

	__int64 hHandle;
    g_pSystemFun->m_iATIndicator->CreateObj("T1", &hHandle);

	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	// 返回一串数据
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	
	LPNODELEMENT pParam0 = ayVars->GetParam(0);

	long lRefPeriod =  (long)pCompile->vexp(pParam0,&curStatus);
	if (lRefPeriod < 1)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);

	HSDouble* pRefHighDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pRefLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);	
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);
	HSDouble* pRefOpenDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);

	long lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() ;


        VARIANT_BOOL bRet;
        g_pSystemFun->m_iATIndicator->SetSymbolSize(hHandle, lSize + 1);
        for (int i = 0; i < lSize; i++)
        {
			g_pSystemFun->m_iATIndicator->SetSymbolValue(hHandle, i, pRefOpenDay[i].m_dData, pRefHighDay[i].m_dData, pRefLowDay[i].m_dData, pRefCloseDay[i].m_dData, &bRet);
        }


    g_pSystemFun->m_iATIndicator->Calculation(hHandle, 0, lSize-1);

    long lCount = 0;
    g_pSystemFun->m_iATIndicator->PlotCount(hHandle, &lCount);


        float dValue;
        VARIANT_BOOL bIsPlotOn;
        for (int i = 0; i < lSize; i++)
        {
			g_pSystemFun->m_iATIndicator->IsPlotOn(hHandle, 0, i, &bIsPlotOn);
            if (bIsPlotOn)
            {
                g_pSystemFun->m_iATIndicator->GetPlotData(hHandle, 0, i, &dValue);
				pCurDataStatus->m_pData[i] = dValue;
            }
            else
            {
				pCurDataStatus->m_pData[i] = hx_DefMinValues;
            }
		}

		g_pSystemFun->m_iATIndicator->DeleteObj(hHandle);
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}*/

//******************************以上是litao新增*****************************************

