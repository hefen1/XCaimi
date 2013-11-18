/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	SystemFun.h
*	文件标识：	底层系统函数定义
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
***************************************************************/

#if !defined(_SYSTEMFUN_H_)
#define _SYSTEMFUN_H_

/*#include "..\comm\HSexportbase.h"*/
#include "exppublic.h"
#include "HSDataSource.h"
#include "ExpValue.h"
#include "hsds_comudata.h"

#if 0
#import "C:\Documents and Settings\yulaixin\桌面\AbleTrend.SH\_bin\AblesysIndicators.exe"
using namespace AblesysIndicatorsLib;
#endif
#define HS_Return_Double -10000
#define HS_Return_Long	  -20000
#define HS_Return_STRING -30000
#define HS_Return_ZGB -40000

//--2010.10.8litao #import "AblesysIndicators.exe"
//--using namespace AblesysIndicatorsLib;

#define  CommRealTimeDataEmpty CommRealTimeData
/*************************************************************
*	类 名 称：	CSystemFun
*	类 描 述：	将所有的系统函数组织起来,全为静态函数,无属性
***************************************************************/
class CExpressionBase;
class CExpValue;

class BCGCONTROLBARDLLEXPORT CSystemFun : public CObject
{
//--2010.10.8litao public:
//	CComPtr<IATIndicator> m_iATIndicator;
//	HRESULT m_hrATIndicator;

public:
#if 0
	CComPtr<IATIndicator> m_iATIndicator;
	BOOL m_bLoadCom;
#endif
	CSystemFun();
	// 初始化全局的数据源变量g_pHxDataSource
	CSystemFun(CHSDataSource* pHxDataSource);
	~CSystemFun();
	// 
public:
	static HSDouble TLOW (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	static HSDouble THIGH (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 行情函数
	/******************************************************************
	*	函数功能：	ADVANCE(var1) 上涨家数
	*	函数参数：	CExpressionBase* pCompile				: [in] 公式基类,在这里主要是为了取得数据源
	*				FormulaGeneralParam* ayVars					: [in] 通用参数
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] 输入状态及返回数据用
	*				int& nError								: [out] 计算错误信息
	*	返 回 值：	HSDouble								: 计算结果
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	static HSDouble ADVANCE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    // 函数: 

	// 以下函数,所有参数与上述相同,其意义也是一般.请参考上面!!!
	// 开盘价
	static HSDouble OPEN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    // 函数: 	

	// 最高价
	static HSDouble HIGH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 函数: LOW(); 最低价
	static HSDouble LOW(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 函数: CLOSE(); 收盘价
	static HSDouble CLOSE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 函数: VOL(); 成交量
	static HSDouble VOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 函数: AMOUNT(); 成交额
	static HSDouble AMOUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
#ifdef SUPPORT_NETVALUE
	// 函数: NETVALUE(); 国债利率，基金净值
	static HSDouble NETVALUE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
#endif

	// 函数: DATEDATA(); 日期
	static HSDouble DATEDATA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    


	/******************************************************************
	*	函数功能：
	*	函数参数：	CExpressionBase* pCompile
	*				FormulaGeneralParam* ayVars
	*				HSCurDataInfoStatus* pCurDataStatus
	*				int& nError
	*	返 回 值：	HSDouble 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	static BOOL CompareCloseOpen(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);


	// 函数: ASKPRICE(var1) 委卖价 1-3
	static HSDouble ASKPRICE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 函数: ASKVOL(var1) 委卖量 1-3
	static HSDouble ASKVOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// BIDPRICE(var1) 委买价 1-3
	static HSDouble BIDPRICE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    // 函数: 

	// 函数: BIDVOL(var1) 委买量 1-3
	static HSDouble BIDVOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  

	// 函数: BUYVOL(); 主动性买单
	static HSDouble BUYVOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 函数: DECLINE(); 下跌家数
	static HSDouble DECLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 函数: EXTDATA(var1) 扩展数据1-11
	static HSDouble EXTDATA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 函数: ISBUYORDER(); 是否主动性买单
	static HSDouble ISBUYORDER(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 函数: SELLVOL(); 主动性卖单
	static HSDouble SELLVOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    


	// 时间函数
	// 函数: DATE(); 年月日
	static HSDouble hxDATE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// 函数: PERIOD_TYPE_DAY(); 日期
	static HSDouble hxDAY(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);   
	// 函数: HOUR(); 小时
	static HSDouble HSOUR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: MINUTE(); 分钟
	static HSDouble hxMINUTE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: MONTH(); 月份
	static HSDouble hxMONTH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: TIME(); 时分秒
	static HSDouble hxTIME(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: WEEKDAY(); 星期
	static HSDouble hxWEEKDAY(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: YEAR(); 年份
	static HSDouble hxYEAR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	//引用函数
	// 函数: BACKSET(var1,var2) 向前赋值
	static HSDouble BACKSET(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  BARSCOUNT(var1) 有效周期数
	static HSDouble BARSCOUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: BARSLAST(var1) 上一次条件成立位置
	static HSDouble BARSLAST(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: BARSSINCE(var1) 第一个条件成立位置
	static HSDouble BARSSINCE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);   
	// 函数: COUNT(var1,var2) 统计总数
	static HSDouble COUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  DMA(var1,var2) 动态移动平均
	static HSDouble DMA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 函数: EMA(var1,var2) 指数平滑移动平均
	static HSDouble EMA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError );    

	// 函数: HHV(var1,var2) 最高值
	static HSDouble HHV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: HHVBARS(var1,var2) 上一高点到当前的周期数
	static HSDouble HHVBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  LLV(var1,var2) 最低值
	static HSDouble LLV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: LLVBARS(var1,var2) 上一低点到当前的周期数
	static HSDouble LLVBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: MA(var1,var2) 简单移动平均
	static HSDouble MA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: REF(var1,var2) 引用若干周期前的数据
	static HSDouble REF(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  SMA(var1,var2,var3) 移动平均
	static HSDouble SMA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: SUM(var1,var2) 总和
	static HSDouble SUM(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: SUMBARS(var1,var2) 向前累加到指定值到现在的周期数
	static HSDouble SUMBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 逻辑函数
	// 函数: IF(var1,var2,var3) 根据条件求不同的值
	static HSDouble IF(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: ISDOWN();  该周期是否收阴
	static HSDouble ISDOWN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: ISEQUAL(); 该周期是否平盘
	static HSDouble ISEQUAL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: ISUP();    该周期是否收阳
	static HSDouble ISUP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 算术函数
	// 函数: ABS(var1) 求绝对值
	static HSDouble ABS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: CROSS(var1,var2) 两条线交叉
	static HSDouble CROSS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  MAX(var1,var2) 求最大值
	static HSDouble MAX(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);   
	// 函数: MIN(var1,var2) 求最小值
	static HSDouble MIN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);   
	// 函数: MOD(var1,var2) 求模运算
	static HSDouble MOD		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: NOT(var1) 求逻辑非
	static HSDouble NOT		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  REVERSE(var1) 求相反数
	static HSDouble REVERSE	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// 函数: SGN(var1) 求符号值
	static HSDouble SGN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 数学函数
	// 函数: ACOS(var1) 反余弦值
	static HSDouble ACOS	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: ASIN(var1) 反正弦值
	static HSDouble ASIN	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// 函数:  ATAN(var1) 反正切值
	static HSDouble ATAN	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: COS(var1) 余弦值
	static HSDouble COS		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: EXP(var1) 指数
	static HSDouble EXP		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// 函数: LN(var1) 求自然对数
	static HSDouble LN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  LOG(var1) 求10为底的对数
	static HSDouble LOG		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: POW(var1,var2) 乘幂
	static HSDouble POW		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: SIN(var1) 正弦值
	static HSDouble SIN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: SQRT(var1) 开平方
	static HSDouble SQRT	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  TAN(var1) 正切值
	static HSDouble TAN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 统计函数
	// 函数: AVEDEV(var1,var2) 平均绝对偏差
	static HSDouble AVEDEV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: DEVSQ(var1,var2) 数据偏差平方和
	static HSDouble DEVSQ(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: FORCAST(var1,var2) 线性回归预测值
	static HSDouble FORCAST		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  SLOPE(var1,var2) 线性回归斜率
	static HSDouble SLOPE		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: STD(var1,var2) 估算标准差
	static HSDouble STD(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: STDP(var1,var2) 总体标准差	
	static HSDouble STDP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);   
	// 函数: VAR(var1,var2) 估算样本方差
	static HSDouble VAR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  VARP(var1,var2) 总体样本方差
	static HSDouble VARP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);     

	// 指数函数
	// 函数: COST(var1) 成本分布情况
	static HSDouble COST		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: PEAK(var1,var2,var3) 前M个ZIG转向波峰值
	static HSDouble PEAK		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: PEAKBARS(var1,var2,var3) 前M个ZIG转向波峰到当前距离
	static HSDouble PEAKBARS	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  SAR(var1,var2,var3) 抛物转向
	static HSDouble SAR			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: SARTURN(var1,var2,var3) 抛物转向点
	static HSDouble SARTURN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// 函数: TROUGH(var1,var2,var3) 前M个ZIG转向波谷值
	static HSDouble TROUGH		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: TROUGHBARS(var1,var2,var3) 前M个ZIG转向波谷到当前距离
	static HSDouble TROUGHBARS	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  WINNER(var1) 获利盘比例
	static HSDouble WINNER		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  ZIG(var1,var2) 之字转向
	static HSDouble ZIG			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  LWINNER(var1) 近期获利盘比例
	static HSDouble LWINNER		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  PWINNER(var1) 远期获利盘比例
	static HSDouble PWINNER		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 大盘函数
	// 函数: INDEXA(); 对应大盘成交额
	static HSDouble INDEXA			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: INDEXADV(); 对应大盘上涨家数
	static HSDouble INDEXADV		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  INDEXC(); 对应大盘收盘价
	static HSDouble INDEXC			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  INDEXDEC(); 对应大盘下跌家数
	static HSDouble INDEXDEC		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: INDEXH(); 对应大盘最高价
	static HSDouble INDEXH			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: INDEXL(); 对应大盘最低价
	static HSDouble INDEXL			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  INDEXO(); 对应大盘开盘价
	static HSDouble INDEXO			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数:  INDEXV(); 对应大盘成交量
	static HSDouble INDEXV			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 常用函数
	// 函数: CAPITAL(); 流通盘大小
	static HSDouble CAPITAL		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// 函数: VOLUNIT(); 每手股数
	static HSDouble VOLUNIT		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// 动态行情函数
	static HSDouble DYNAINFO	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// 函数:  DYNAINFO(3) 动态行情函数: 昨收
	// 函数:  DYNAINFO(4) 动态行情函数: 今开
	// 函数:  DYNAINFO(5) 动态行情函数: 最高
	// 函数:  DYNAINFO(6) 动态行情函数: 最低
	// 函数:  DYNAINFO(7) 动态行情函数: 最新
	// 函数:  DYNAINFO(8) 动态行情函数: 总手
	// 函数:  DYNAINFO(9) 动态行情函数: 现手
	// 函数:  DYNAINFO(10) 动态行情函数: 总额
	// 函数:  DYNAINFO(11) 动态行情函数: 均价
	// 函数:  DYNAINFO(12) 动态行情函数: 涨跌
	// 函数:  DYNAINFO(13) 动态行情函数: 振幅
	// 函数:  DYNAINFO(14) 动态行情函数: 涨幅
	// 函数:  DYNAINFO(15) 动态行情函数: 委比
	// 函数:  DYNAINFO(16) 动态行情函数: 委差
	// 函数:  DYNAINFO(17) 动态行情函数: 量比
	// 函数:  DYNAINFO(18) 动态行情函数: 委买
	// 函数:  DYNAINFO(19) 动态行情函数: 委卖
	// 函数:  DYNAINFO(20) 动态行情函数: 委买价
	// 函数:  DYNAINFO(21) 动态行情函数: 委卖价
	// 函数:  DYNAINFO(22) 动态行情函数: 内盘
	// 函数:  DYNAINFO(23) 动态行情函数: 外盘
	// 函数:  DYNAINFO(24) 动态行情函数: 涨速
	// 函数:  DYNAINFO(25) 动态行情函数: 买一量
	// 函数:  DYNAINFO(26) 动态行情函数: 买二量
	// 函数:  DYNAINFO(27) 动态行情函数: 买三量
	// 函数:  DYNAINFO(28) 动态行情函数: 买一价
	// 函数:  DYNAINFO(29) 动态行情函数: 买二价
	// 函数:  DYNAINFO(30) 动态行情函数: 买三价
	// 函数:  DYNAINFO(31) 动态行情函数: 卖一量
	// 函数:  DYNAINFO(32) 动态行情函数: 卖二量
	// 函数:  DYNAINFO(33) 动态行情函数: 卖三量
	// 函数:  DYNAINFO(34) 动态行情函数: 卖一价
	// 函数:  DYNAINFO(35) 动态行情函数: 卖二价
	// 函数:  DYNAINFO(36) 动态行情函数: 卖三价
	// 函数:  DYNAINFO(37) 动态行情函数: 换手率
	// 函数:  DYNAINFO(38) 动态行情函数: 5日均量
	// 函数:  DYNAINFO(39) 动态行情函数: 市盈率
	// 函数:  DYNAINFO(40) 动态行情函数: 成交方向

	// 财务指标
	static HSDouble FINANCE	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 函数:  FINANCE(1) 财务指标: 总股本(万股)
	// 函数:  FINANCE(2) 财务指标: 国家股
	// 函数:  FINANCE(3) 财务指标: 发起人法人股
	// 函数:  FINANCE(4) 财务指标: 法人股
	// 函数:  FINANCE(5) 财务指标: B股
	// 函数:  FINANCE(6) 财务指标: H股
	// 函数:  FINANCE(7) 财务指标: 流通A股
	// 函数:  FINANCE(8) 财务指标: 职工股
	// 函数:  FINANCE(9) 财务指标: A2转配股
	// 函数:  FINANCE(10) 财务指标: 总资产(千元)
	// 函数:  FINANCE(11) 财务指标: 流动资产
	// 函数:  FINANCE(12) 财务指标: 固定资产
	// 函数:  FINANCE(13) 财务指标: 无形资产
	// 函数:  FINANCE(14) 财务指标: 长期投资
	// 函数:  FINANCE(15) 财务指标: 流动负债
	// 函数:  FINANCE(16) 财务指标: 长期负债
	// 函数:  FINANCE(17) 财务指标: 资本公积金
	// 函数:  FINANCE(18) 财务指标: 每股公积金
	// 函数:  FINANCE(19) 财务指标: 股东权益
	// 函数:  FINANCE(20) 财务指标: 主营收入
	// 函数:  FINANCE(21) 财务指标: 主营利润
	// 函数:  FINANCE(22) 财务指标: 其他利润
	// 函数:  FINANCE(23) 财务指标: 营业利润
	// 函数:  FINANCE(24) 财务指标: 投资收益
	// 函数:  FINANCE(25) 财务指标: 补贴收入
	// 函数:  FINANCE(26) 财务指标: 营业外收支
	// 函数:  FINANCE(27) 财务指标: 上年损益调整
	// 函数:  FINANCE(28) 财务指标: 利润总额
	// 函数:  FINANCE(29) 财务指标: 税后利润
	// 函数:  FINANCE(30) 财务指标: 净利润
	// 函数:  FINANCE(31) 财务指标: 未分配利润
	// 函数:  FINANCE(32) 财务指标: 每股未分配
	// 函数:  FINANCE(33) 财务指标: 每股收益
	// 函数:  FINANCE(34) 财务指标: 每股净资产
	// 函数:  FINANCE(35) 财务指标: 调整每股净资
	// 函数:  FINANCE(36) 财务指标: 股东权益比
	// 函数:  FINANCE(37) 财务指标: 净资收益率

	// 函数常量
	// 画线类型常量
	// 柱状线，从0到指标值间画垂直直线
	static HSDouble STICK				(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 彩色柱状线，当值为0是显示红色，否则显示绿色
	static HSDouble COLORSTICK	   	    (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 成交量柱状线，当股价上涨时显示红色空心柱，否则显示绿色空心柱
	static HSDouble VOLSTICK			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 同时画出柱状线和指标线
	static HSDouble LINESTICK			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 小叉线
	static HSDouble CROSSDOT			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 小圆圈线
	static HSDouble CIRCLEDOT			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 小圆点线
	static HSDouble POINTDOT			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 3d
	static HSDouble DRAW3D( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  

	// 画3D曲线
	static HSDouble LINEDRAW3D( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 

	// 当前曲线不绘制
	static HSDouble NOTDRAWLINE( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  

	// 当前文本不绘制
	static HSDouble NOTDRAWTEXT( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  

	// 当前曲线文本都不绘制
	static HSDouble NOTDRAWLINETEXT( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	
	static HSDouble NOTDRAW( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);	

	static HSDouble NOTTABLE( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);	

	// 不计算最大最小值,在绘图时使用
	static HSDouble NOTMAXMIN( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	

	// 五彩K线颜色
	// 红色
	static HSDouble COLORRED	    (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 绿色
	static HSDouble COLORGREEN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 蓝色
	static HSDouble COLORBLUE		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 黑色
	static HSDouble COLORBLACK		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 白色
	static HSDouble COLORWHITE		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 灰色
	static HSDouble COLORGRAY		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 黄色
	static HSDouble COLORYELLOW	    (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 青色
	static HSDouble COLORCYAN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 品红色
	static HSDouble COLORMAGENTA	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 棕色	
	static HSDouble COLORBROWN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 指定色
	static HSDouble hxRGB	    	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// 线风格常量
	// 实线
	static HSDouble hxSOLID(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	/* -------  */
	static HSDouble hxDASH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);       
	/* .......  */
	static HSDouble hxDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);        
	/* _._._._  */
	static HSDouble hxDASHDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	/* _.._.._  */
	static HSDouble hxDASHDOTDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError); 

	// 用户绘图函数
	// 画图标
	static HSDouble DRAWICON(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 画线
	static HSDouble DRAWLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);       /*   */
	// 画文本
	static HSDouble DRAWTEXT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);        /*   */
	// 画线
	static HSDouble POLYLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);    /*   */
	// 
	static HSDouble STICKLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);    /*  */

	// 其他内部计算函数
	// 得到数据源
	static CHSDataSource* GetDataSource(CExpressionBase* pCompile);
	// 得到五日量
	static HSDouble Get5DayVol(CHSDataSource* pHxData, int nDayPos);

	// 强度分析用
	// 得到历史量比
	static HSDouble GetHisLIANGBI(CHSDataSource* pHxData, int nDayPos);
	// n日强度
	static HSDouble QIANGDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 涨跌幅度=
	static HSDouble ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 换手率
	static HSDouble HUANSHOULV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 量比幅度"
	static HSDouble LIANGBIFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 震荡幅度"
	static HSDouble ZHENGDANGFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// 区间分析用
	// 区间市场比例
	static HSDouble BetweenShiChangBiLi(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 区间开盘
	static HSDouble BetweenOpen(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 区间收盘
	static HSDouble BetweenClose(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 区间最高
	static HSDouble BetweenHigh(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 区间最低
	static HSDouble BetweenLow(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 区间成交量
	static HSDouble BetweenVol(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 区间成交额
	static HSDouble BetweenAmount(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 重心价
	static HSDouble CenterPrice(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 开始时间
	static HSDouble BetweenBeginDate(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 结速时间
	static HSDouble BetweenEndDate(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// 板块分析用
	// 涨跌幅
	static HSDouble BK_ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 权涨跌幅
	static HSDouble BK_Q_ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 总成交量
	static HSDouble BK_Vol(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 成交额
	static HSDouble BK_Amount(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 成交比例
	static HSDouble BK_Deal(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 换手率
	static HSDouble BK_HuanShouLv(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 市盈率
	static HSDouble BK_ShiYing(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 令涨股
	static HSDouble BK_LingZhang(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 领跌股
	static HSDouble BK_LingDie(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 股票数
	static HSDouble BK_Num(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 股票数
	static HSDouble BK_UpRatio(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
public:
	/******************************************************************
	*	函数功能：	将YYYMMDD的时间格式转换成基于1970为基点的毫秒数
	*	函数参数：	long lDate	: [in] 以YYYMMDD为表示的时间
	*	返 回 值：	time_t	    : 基于1970为基点的毫秒数
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	static	time_t	   hxLongToTime(long lDate);

	/******************************************************************
	*	函数功能：	设置数据源的同时返回g_pSystemFun全局变量
	*	函数参数：	CHSDataSource* pHxDataSource	: [in] 数据源
	*	返 回 值：	CSystemFun*						: 全局的系统函数管理类对象
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	static CSystemFun*    GetSystemFunPtr(CHSDataSource* pHxDataSource);

	/******************************************************************
	*	函数功能：	设置数据源
	*	函数参数：	CHSDataSource* pHxDataSource : [in] 数据源
	*	返 回 值：	CHSDataSource*				 : 旧的数据源
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	static CHSDataSource* SetDataSource(CHSDataSource* pHxDataSource);

	/******************************************************************
	*	函数功能：	有效数据的偏移量
	*	函数参数：	HSDouble* pData : [in] 要检查的变量数组
	*				int nSize		: [in] 数组长度
	*	返 回 值：	int				: 第一个有效数据的索引号
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	static int		      GetDataValidPos(HSDouble* pData,int nSize);


	// 指定类型的专业财务数据
	static HSDouble PROFFIN	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 指定日期的专业财务数据
	static HSDouble PROFFINON	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 输出字符串
	static HSDouble STROUT	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 日期转换函数
	static HSDouble DATETOSTR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	
	static HSDouble BLOCKCALC	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	
	

	//2.	PROFSTR				专业财务字符串数据
	static HSDouble PROFSTR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//PROFSTR(N,M)
	//表示取得M个报告期之前的第N号专业财务字符串数据,
	//例如PROFSTR(4001,0)表示最近一期第一大股东名称，N的取值请参阅下表

	//3.	PROFFINLAST			专业财务数据截止期位置
	static HSDouble PROFFINLAST	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//PROFFINLAST(N)
	//表示最近报告期的第N号专业财务数据距离现在的周期数，N的取值请参阅下表

	
	//5.	PROFSTRON			指定日期的专业字符串财务数据
	static HSDouble PROFSTRON	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//PROFFINON(N,Y,MD)
	//表示取得Y年M月D日的第N号专业财务数据，
	//如PROFSTRON(5001,2000,0101)取得2000年1月1日的第一大股东名称，N的取值请参阅下表

	//6.	PROFFINTERM		专业财务数据报告期
	static HSDouble PROFFINTERM	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//PROFFINTERM(M)
	//表示取得M个报告期之前的财务报表是年报、中报还是季报
	//返回1：第一季度季报，2：中报，3：第三季度季报，4：年报
	//例如：若PROFFINTERM(0)等于4，则表示最近一期财务报表是年报。

	//7.	SETPROFFIN			设置专业财务数据
	static HSDouble SETPROFFIN	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//SETPROFFIN(X)
	//在本函数后的其它专业财务函数将受到本次属性设置的影响，表示将专业财务数据属性设定为X，X从低到高每一位表示一个含义
	//第1位：包含年报，
	//第2位：包含中报，
	//第3位：包含季报，
	//第4位：包含最新财务指标，
	//第5位：调整中报、季报财务指标，
	//例如SETPROFFIN(01011)表示取得年报、中报，最新一期数据，无论是那个报告期都要包括在内，中报不作调整
	//如果不调用本函数，系统默认值为01111


	// 弘历通-彩虹
	static HSDouble HLTCH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// 弘历通-红白圈
	static HSDouble HLTHBQ(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
    // 弘历通-红白圈
	static HSDouble HLTHBQ1(long lRefPeriod, long lRefCount,HSCurDataInfoStatus* pCurDataStatus,
				 HSDouble* pRefHighDay,HSDouble* pRefLowDay,HSDouble* pRefCloseDay, int& nError);
    // 弘历通-红白圈
    static HSDouble HLTHBQ2(long lRefPeriod, long lRefCount,HSCurDataInfoStatus* pCurDataStatus,
				 HSDouble* pRefHighDay,HSDouble* pRefLowDay,HSDouble* pRefCloseDay, int& nError);

	// 循环相关
public:
	static HSDouble FOR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	static HSDouble CONTINUE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	static HSDouble BREAK	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	static HSDouble OPERATOR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	//******************************以下是litao新增*****************************************
	//函数：返回不绘制的数据类型
	static HSDouble DRAWNULL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError)
	{
		return hx_DefMinValues;
	}
	// 过滤函数，满足条件的N个周期后的数据设置为0
	static HSDouble FILTER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// WHILE (条件表达式) DO  [循环内语句] END LOOP
	static HSDouble WHILE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// 函数: CEILING(var1) 向上舍入取整
	static HSDouble CSystemFun::CEILING(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 函数: FLOOR(var1) 向下舍入取整
	static HSDouble CSystemFun::FLOOR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 函数: ROUND(var1) 四舍五入取整
	static HSDouble CSystemFun::ROUND(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// 函数: RBTREND() AbleTrend的红绿线标志
	//--2010.10.8static HSDouble CSystemFun::RBTREND(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
	//	HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//******************************以上是litao新增*****************************************
};
#endif
