/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	ExpPublic.h
*	文件标识：	
*	摘	  要：	定义了公式中重要的CMapVariabile类及一些ID
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
***************************************************************/

#if !defined(_EXPPUBLIC_H_)
#define _EXPPUBLIC_H_
#include "hsds_comudata.h"
#include "hsstructnew.h"
#include "ConfigStruct.h"
#include "QuoteStuctDefine.h"

#include "ExportDll.h"
#include "CompileInterFace.h"
#include "FormulaStruct.h"
#include "windowsx.h"
#include <math.h>
#include <ctype.h>
#include <io.h>
#include <direct.h>
#include <mbstring.h>
#include <afxcmn.h>

//******************************以下是litao新增*****************************************
#include <afxmt.h>
#define COMPILE_NO 1
#define EXECUTE_NO 2
#define NOHAVE_ID 0
#define HAVE_ID 1
#define MORELOOPERROR -999
#define MAXLOOPTIMES  1000
//******************************以上是litao新增*****************************************


#define HS_GETSTOCK_BYCODE				    0x00000100
#define HS_GETSTOCK_BYMAKET					0x00000200 

// 选股dlg使用 ref DlgCondition.cpp
#define  CDlgCondition_Text_Prompt				  _T("选中数: %i,选中比例: %.2f%%")
#define  CDlgCondition_Not_Select_Error			  _T("请设置合理的条件!")
#define  CDlgCondition_But_Stop					  _T("中止选股&Z")
#define  CDlgCondition_But_Run					  _T("执行选股&Z")
#define  CDlgCondition_But_Request				  _T("是否中止选股？")
#define  CDlgCondition_Default_Name				  _T("条件选股")

#define  CDlgCondition_MessageBox_Caption		  _T("选股")
#define  CDlgCondition_Data_Prompt				  _T("选股在盘后下载的日线中进行，请确保数据完整!")

// CDlgSetStockAndTime ref DlgSetStockAndTime.cpp
// 选股配置参数保存/装入
#define  CDlgSetStockAndTime_Section			  _T("选股-时间-股票范围-选项")
#define  CDlgSetStockAndTime_Time				  _T("时间")
#define  CDlgSetStockAndTime_Stock				  _T("股票范围")
#define  CDlgSetStockAndTime_Option				  _T("选项")

#define  CDlgSetStockAndTime_Begin_End_Date_Error _T("起始时间设置错误。")

// CDlgUnionConSelStock ref DlgUnionConSelStock.cpp
#define  CDlgUnionConSelStock_Ok_Prompt			  _T("组合条件为空，确认是否删除？")
#define  CDlgUnionConSelStock_Not_Name			  _T("名称不能为空！")

#define  CDlgUnionConSelStock_Cond_Or			  _T("下列条件之一成立")
#define  CDlgUnionConSelStock_Cond_And			  _T("下列条件同时成立")
#define  CDlgUnionConSelStock_Cond_Or_Pre     	  _T("向前%i-%i天之间,下列条件之一成立")
#define  CDlgUnionConSelStock_Cond_And_Pre	      _T("向前%i-%i天之间,下列条件同时成立")

// 公式自动安装参数
#define EXTERN_EXPRESS_OVR		    0x0001  // 覆盖已有公式
#define EXTERN_EXPRESS_AUTO_NAME	0x0002  // 自动命名
#define EXTERN_EXPRESS_CHANGE_NAME  0x0004  // 更名
#define EXTERN_EXPRESS_SAME_NAME	0x0008  // 是否重名
#define EXTERN_EXPRESS_AUTO_SETUP	0x0010  // 自动安装公式

#define  SelectingStock     0x100       //选股过程中
#define  SelectStockOver    0x101       //选股结束

#define BLOCK_NAME_LENGTH		32			// 板块名称长度
#define _delObject(p)    { if(p) {delete p;   p = NULL; } }
#define _delArray(p)	 { if(p) {delete[] p; p = NULL; } }
#define _delArrayObj(p)  { for(int i = p.GetSize() - 1; i >= 0; i--) { delete p.GetAt(i); } p.RemoveAll(); }

// 符号操作
#define Operator_Begin  150
#define Operator_End    255
		// 数学函数
#define Operator_SIN		 (Operator_Begin+1) // 正弦函数 SIN
#define Operator_COS		 (Operator_Begin+2) // 余弦
#define Operator_EXP		 (Operator_Begin+3) // 指数
#define Operator_SQRT		 (Operator_Begin+4) // 平方根
#define Operator_LOG		 (Operator_Begin+5) // 对数
#define Operator_TG			 (Operator_Begin+6) // 正切
#define Operator_CTG		 (Operator_Begin+7) // 余切
#define Operator_ASIN		 (Operator_Begin+8) // 反正弦函数
#define Operator_ACOS		 (Operator_Begin+9) // 反余弦
#define Operator_ATG		 (Operator_Begin+10) // 反正切
        // 操作符
#define Operator_LargerEqual (Operator_Begin+11) // >=     大于等于
#define Operator_SmallEqual  (Operator_Begin+12) // <=     小于等于
#define Operator_NotEqual    (Operator_Begin+13) // != <>  不等于
#define Operator_OR          (Operator_Begin+14) // || or  或
#define Operator_AND         (Operator_Begin+15) // && and 并且
#define Operator_Semicolon   (Operator_Begin+16) // ;  分号(即按行编译)
#define Operator_Komma		 (Operator_Begin+17) // ,  逗号
#define Operator_Evaluate	 (Operator_Begin+18) // := 赋值
#define Operator_Colon		 (Operator_Begin+19) // :  冒号(绘图)
#define Operator_Function	 (Operator_Begin+20) // 函数
#define Operator_Express	 (Operator_Begin+21) // 引用公式
#define Operator_InportData	 (Operator_Begin+22) // 引用数据
#define Operator_While		 (Operator_Begin+23) // WHILE

#define Operator_Error       Operator_End // 操作符错误

// 错误号
#define hxError_Invalid				 (-1000)			    // 未知错误
#define hxError_DIVISION_BY_0		 (hxError_Invalid -  1) // 被 0 除
#define hxError_ILEGAL_OPERATION	 (hxError_Invalid -  2)
#define hxError_UNDEFINED_VARIABLE   (hxError_Invalid -  3) // 变量未定义
#define hxError_INVALID_DOMAIN	     (hxError_Invalid -  4) // log 参数错
#define hxError_Fun_Exist			 (hxError_Invalid - 5)  // 变量已经存在
#define hxError_Var_Exist			 (hxError_Invalid - 6)  // 函数已经存在
#define hxError_Nod_CloneFail		 (hxError_Invalid - 7)  // 操作单元复制失败
#define hxError_InvalidateVars       (hxError_Invalid - 8)  // 无效变量
#define hxError_NotFunction	         (hxError_Invalid - 9)  // 函数不存在
#define hxError_NotVariable	         (hxError_Invalid - 10) // 变量不存在
#define hxError_Operator	         (hxError_Invalid - 11) // 操作符错误
#define hxError_Fun_param			 (hxError_Invalid - 12) // 函数参数错误
#define hxError_Identification		 (hxError_Invalid - 13) // 不能识别的字符

#define hxError_AddRight		     (hxError_Invalid - 14) // +加号右边错误
#define hxError_SubRight		     (hxError_Invalid - 15) // -减号右边错误
#define hxError_RideRight		     (hxError_Invalid - 16) // +乘号右边错误
#define hxError_DivideRight		     (hxError_Invalid - 17) // -除号右边错误
#define hxError_PowerRight		     (hxError_Invalid - 18) // ^乘方右边错误
#define hxError_BracketLeft		     (hxError_Invalid - 19) // (左括号错误
#define hxError_BracketRight	     (hxError_Invalid - 21) // )右括号错误
#define hxError_AbsLeft	     	     (hxError_Invalid - 22) // | |绝对值左边错误
#define hxError_AbsRight     	     (hxError_Invalid - 23) // | |绝对值右边错误
#define hxError_LogicalRight     	 (hxError_Invalid - 24) // 逻辑操作符右边错误(< > = == >= <= <> !=)
#define hxError_CommaRight     	     (hxError_Invalid - 25) // 逗号右边错误
#define hxError_OrRight     	     (hxError_Invalid - 26) // or || 逻辑或右边错误
#define hxError_AndRight     	     (hxError_Invalid - 27) // and && 逻辑并且右边错误
#define hxError_DefinedRight  	     (hxError_Invalid - 28) // :=赋值表达式右边错误
#define hxError_DrawRight	  	     (hxError_Invalid - 29) // :绘图操作符右边错误
#define hxError_ReverseRight	  	 (hxError_Invalid - 30) // !取反右边错误
#define hxError_Name_Exist			 (hxError_Invalid - 31)  // 标识符已经存在
#define hxError_NotName_Exist		 (hxError_Invalid - 32)  // 标识符未定义
#define hxError_Notes_Invalidate	 (hxError_Invalid - 33)  // 注释错误/**/ //
#define hxError_AtLeastHasExp		 (hxError_Invalid - 34)  // 至少需要一个表达式
#define hxError_AtLeastHasDrawExp	 (hxError_Invalid - 35)  // 至少需要一个绘图表达式
#define hxError_WantInvertedComma	 (hxError_Invalid - 36)  // 缺少引用符号
#define hxError_Exp_param			 (hxError_Invalid - 37)  // 缺少引用参数
#define hxError_unknowPeriod		 (hxError_Invalid - 38)  // 不能识别的周期
//******************************以下是litao新增*****************************************
#define hxError_MoreLoop			 (hxError_Invalid - 39)  // 循环次数太多
//******************************以上是litao新增*****************************************

// 函数操作
#define FunOper_Begin 100
#define FunOper_End   1000

// 用户定义函数
#define FunOper_UserDefFun_Begin (FunOper_Begin+1)
#define FunOper_UserDefFun_End   (FunOper_UserDefFun_Begin+100)

// 行情函数
#define FunOper_QUOTES_Begin    (FunOper_UserDefFun_End + 1)
#define FunOper_QUOTES_End      (FunOper_QUOTES_Begin + 20)

#define FunOper_ADVANCE			(FunOper_QUOTES_Begin+1)    // 函数: ADVANCE(var1) 上涨家数
#define FunOper_AMOUNT			(FunOper_QUOTES_Begin+2)    // 函数: AMOUNT() 成交额
#define FunOper_ASKPRICE		(FunOper_QUOTES_Begin+3)    // 函数: ASKPRICE(var1) 委卖价 1-3
#define FunOper_ASKVOL			(FunOper_QUOTES_Begin+4)    // 函数: ASKVOL(var1) 委卖量 1-3
#define FunOper_BIDPRICE		(FunOper_QUOTES_Begin+5)    // 函数: BIDPRICE(var1) 委买价 1-3
#define FunOper_BIDVOL			(FunOper_QUOTES_Begin+6)    // 函数: BIDVOL(var1) 委买量 1-3
#define FunOper_BUYVOL			(FunOper_QUOTES_Begin+7)    // 函数: BUYVOL() 主动性买单
#define FunOper_CLOSE			(FunOper_QUOTES_Begin+8)    // 函数: CLOSE() 收盘价
#define FunOper_DECLINE			(FunOper_QUOTES_Begin+9)    // 函数: DECLINE() 下跌家数
#define FunOper_EXTDATA			(FunOper_QUOTES_Begin+10)    // 函数: EXTDATA(var1) 扩展数据1-11
#define FunOper_HIGH			(FunOper_QUOTES_Begin+11)    // 函数: HIGH() 最高价
#define FunOper_ISBUYORDER		(FunOper_QUOTES_Begin+12)    // 函数: ISBUYORDER() 是否主动性买单
#define FunOper_LOW				(FunOper_QUOTES_Begin+13)    // 函数: LOW() 最低价
#define FunOper_OPEN			(FunOper_QUOTES_Begin+14)    // 函数: OPEN() 开盘价
#define FunOper_SELLVOL			(FunOper_QUOTES_Begin+15)    // 函数: SELLVOL() 主动性卖单
#define FunOper_VOL				(FunOper_QUOTES_Begin+16)    // 函数: VOL() 成交量
#ifdef SUPPORT_NETVALUE
#define FunOper_NETVALUE		(FunOper_QUOTES_Begin+17)    // 函数: NETVALUE() 国债利率,基金净值（单位：元)
#endif

// 时间函数
#define FunOper_DateTime_Begin    (FunOper_QUOTES_End + 1)
#define FunOper_DateTime_End      (FunOper_DateTime_Begin + 10)

#define FunOper_DATE			(FunOper_DateTime_Begin+1)    // 函数: DATE() 年月日
#define FunOper_PERIOD_TYPE_DAY				(FunOper_DateTime_Begin+2)    // 函数: PERIOD_TYPE_DAY() 日期
#define FunOper_HOUR			(FunOper_DateTime_Begin+3)    // 函数: HOUR() 小时
#define FunOper_MINUTE			(FunOper_DateTime_Begin+4)    // 函数: MINUTE() 分钟
#define FunOper_MONTH			(FunOper_DateTime_Begin+5)    // 函数: MONTH() 月份
#define FunOper_TIME			(FunOper_DateTime_Begin+6)    // 函数:  TIME() 时分秒
#define FunOper_WEEKDAY			(FunOper_DateTime_Begin+7)    // 函数: WEEKDAY() 星期
#define FunOper_YEAR			(FunOper_DateTime_Begin+8)    // 函数: YEAR() 年份

//引用函数
#define FunOper_Refer_Begin    (FunOper_DateTime_End + 1)
#define FunOper_Refer_End      (FunOper_Refer_Begin + 20)

#define FunOper_BACKSET			(FunOper_Refer_Begin+1)    // 函数: BACKSET(var1,var2) 向前赋值
#define FunOper_BARSCOUNT		(FunOper_Refer_Begin+2)    // 函数:  BARSCOUNT(var1) 有效周期数
#define FunOper_BARSLAST		(FunOper_Refer_Begin+3)    // 函数: BARSLAST(var1) 上一次条件成立位置
#define FunOper_BARSSINCE		(FunOper_Refer_Begin+4)    // 函数: BARSSINCE(var1) 第一个条件成立位置
#define FunOper_COUNT			(FunOper_Refer_Begin+5)    // 函数: COUNT(var1,var2) 统计总数
#define FunOper_DMA				(FunOper_Refer_Begin+6)    // 函数:  DMA(var1,var2) 动态移动平均
#define FunOper_EMA				(FunOper_Refer_Begin+7)    // 函数: EMA(var1,var2) 指数平滑移动平均
#define FunOper_HHV				(FunOper_Refer_Begin+8)    // 函数: HHV(var1,var2) 最高值
#define FunOper_HHVBARS			(FunOper_Refer_Begin+9)    // 函数: HHVBARS(var1,var2) 上一高点到当前的周期数
#define FunOper_LLV				(FunOper_Refer_Begin+10)    // 函数:  LLV(var1,var2) 最低值
#define FunOper_LLVBARS			(FunOper_Refer_Begin+11)    // 函数: LLVBARS(var1,var2) 上一低点到当前的周期数
#define FunOper_MA				(FunOper_Refer_Begin+12)    // 函数: MA(var1,var2) 简单移动平均
#define FunOper_REF				(FunOper_Refer_Begin+13)    // 函数: REF(var1,var2) 引用若干周期前的数据
#define FunOper_SMA				(FunOper_Refer_Begin+14)    // 函数:  SMA(var1,var2,var3) 移动平均
#define FunOper_SUM				(FunOper_Refer_Begin+15)    // 函数: SUM(var1,var2) 总和
#define FunOper_SUMBARS			(FunOper_Refer_Begin+16)    // 函数: SUMBARS(var1,var2) 向前累加到指定值到现在的周期数

// 逻辑函数
#define FunOper_Logical_Begin    (FunOper_Refer_End + 1)
#define FunOper_Logical_End      (FunOper_Logical_Begin + 10)

#define FunOper_IF				 (FunOper_Logical_Begin+1)    // 函数: IF(var1,var2,var3) 根据条件求不同的值
#define FunOper_ISDOWN			 (FunOper_Logical_Begin+2)    // 函数: ISDOWN() 该周期是否收阴
#define FunOper_ISEQUAL			 (FunOper_Logical_Begin+3)    // 函数:  ISEQUAL() 该周期是否平盘
#define FunOper_ISUP			 (FunOper_Logical_Begin+4)    // 函数: ISUP() 该周期是否收阳

//******************************以下是litao新增*****************************************
#define FunOper_WHILE			 (FunOper_Logical_Begin+5)    // 函数: 循环语句
#define FunOper_FILTER			 (FunOper_Logical_Begin+6)    // 函数: 过滤语句
//******************************以上是litao新增*****************************************

// 算术函数
#define FunOper_Arithmetic_Begin    (FunOper_Logical_End + 1)
#define FunOper_Arithmetic_End      (FunOper_Arithmetic_Begin + 15)

#define FunOper_ABS					(FunOper_Arithmetic_Begin+1)    // 函数: ABS(var1) 求绝对值
#define FunOper_CROSS				(FunOper_Arithmetic_Begin+2)    // 函数: CROSS(var1,var2) 两条线交叉
#define FunOper_MAX					(FunOper_Arithmetic_Begin+3)    // 函数:  MAX(var1,var2) 求最大值
#define FunOper_MIN					(FunOper_Arithmetic_Begin+4)    // 函数: MIN(var1,var2) 求最小值
#define FunOper_MOD					(FunOper_Arithmetic_Begin+5)    // 函数: MOD(var1,var2) 求模运算
#define FunOper_NOT					(FunOper_Arithmetic_Begin+6)    // 函数: NOT(var1) 求逻辑非
#define FunOper_REVERSE				(FunOper_Arithmetic_Begin+7)    // 函数:  REVERSE(var1) 求相反数
#define FunOper_SGN					(FunOper_Arithmetic_Begin+8)    // 函数: SGN(var1) 求符号值
//*************************************************************************************************
#define FunOper_CEILING				(FunOper_Arithmetic_Begin+9)    // 函数: CEILING(var1) 向上取整
#define FunOper_FLOOR				(FunOper_Arithmetic_Begin+10)   // 函数: FLOOR(var1) 向下取整
#define FunOper_ROUND				(FunOper_Arithmetic_Begin+11)   // 函数: ROUND(var1) 四舍五入取整
//*************************************************************************************************


// 数学函数
#define FunOper_Maths_Begin			(FunOper_Arithmetic_End + 1)
#define FunOper_Maths_End			(FunOper_Maths_Begin + 15)

#define FunOper_ACOS				(FunOper_Maths_Begin+1)    // 函数: ACOS(var1) 反余弦值
#define FunOper_ASIN				(FunOper_Maths_Begin+2)    // 函数: ASIN(var1) 反正弦值
#define FunOper_ATAN				(FunOper_Maths_Begin+3)    // 函数:  ATAN(var1) 反正切值
#define FunOper_COS					(FunOper_Maths_Begin+4)    // 函数: COS(var1) 余弦值
#define FunOper_EXP					(FunOper_Maths_Begin+5)    // 函数: EXP(var1) 指数
#define FunOper_LN					(FunOper_Maths_Begin+6)    // 函数: LN(var1) 求自然对数
#define FunOper_LOG					(FunOper_Maths_Begin+7)    // 函数:  LOG(var1) 求10为底的对数
#define FunOper_POW					(FunOper_Maths_Begin+8)    // 函数: POW(var1,var2) 乘幂
#define FunOper_SIN					(FunOper_Maths_Begin+9)    // 函数: SIN(var1) 正弦值
#define FunOper_SQRT				(FunOper_Maths_Begin+10)    // 函数: SQRT(var1) 开平方
#define FunOper_TAN					(FunOper_Maths_Begin+11)    // 函数:  TAN(var1) 正切值

// 统计函数
#define FunOper_Statistic_Begin     (FunOper_Maths_End + 1)
#define FunOper_Statistic_End       (FunOper_Statistic_Begin + 10)

#define FunOper_AVEDEV				(FunOper_Statistic_Begin+1)    // 函数: AVEDEV(var1,var2) 平均绝对偏差
#define FunOper_DEVSQ				(FunOper_Statistic_Begin+2)    // 函数: DEVSQ(var1,var2) 数据偏差平方和
#define FunOper_FORCAST				(FunOper_Statistic_Begin+3)    // 函数: FORCAST(var1,var2) 线性回归预测值
#define FunOper_SLOPE				(FunOper_Statistic_Begin+4)    // 函数:  SLOPE(var1,var2) 线性回归斜率
#define FunOper_STD					(FunOper_Statistic_Begin+5)    // 函数: STD(var1,var2) 估算标准差
#define FunOper_STDP				(FunOper_Statistic_Begin+6)    // 函数: STDP(var1,var2) 总体标准差
#define FunOper_VAR					(FunOper_Statistic_Begin+7)    // 函数: VAR(var1,var2) 估算样本方差
#define FunOper_VARP				(FunOper_Statistic_Begin+8)    // 函数:  VARP(var1,var2) 总体样本方差

// 指数函数
#define FunOper_Index_Begin			(FunOper_Statistic_End + 1)
#define FunOper_Index_End			(FunOper_Index_Begin + 12)

#define FunOper_COST		(FunOper_Index_Begin+1)    // 函数: COST(var1) 成本分布情况
#define FunOper_PEAK		(FunOper_Index_Begin+2)    // 函数: PEAK(var1,var2,var3) 前M个ZIG转向波峰值
#define FunOper_PEAKBARS	(FunOper_Index_Begin+3)    // 函数: PEAKBARS(var1,var2,var3) 前M个ZIG转向波峰到当前距离
#define FunOper_SAR			(FunOper_Index_Begin+4)    // 函数:  SAR(var1,var2,var3) 抛物转向
#define FunOper_SARTURN		(FunOper_Index_Begin+5)    // 函数: SARTURN(var1,var2,var3) 抛物转向点
#define FunOper_TROUGH		(FunOper_Index_Begin+6)    // 函数: TROUGH(var1,var2,var3) 前M个ZIG转向波谷值
#define FunOper_TROUGHBARS	(FunOper_Index_Begin+7)    // 函数: TROUGHBARS(var1,var2,var3) 前M个ZIG转向波谷到当前距离
#define FunOper_WINNER		(FunOper_Index_Begin+8)    // 函数:  WINNER(var1) 获利盘比例
#define FunOper_ZIG			(FunOper_Index_Begin+9)    // 函数:  ZIG(var1,var2) 之字转向
#define FunOper_LWINNER		(FunOper_Index_Begin+10)    // 函数:  LWINNER(var1,var2) 近期获利盘比例
#define FunOper_PWINNER		(FunOper_Index_Begin+11)    // 函数:  PWINNER(var1,var2) 远期获利盘比例

// 大盘函数
#define FunOper_BigIndex_Begin    (FunOper_Index_End + 1)
#define FunOper_BigIndex_End      (FunOper_BigIndex_Begin + 10)

#define FunOper_INDEXA			(FunOper_BigIndex_Begin+1)    // 函数: INDEXA() 对应大盘成交额
#define FunOper_INDEXADV		(FunOper_BigIndex_Begin+2)    // 函数: INDEXADV() 对应大盘上涨家数
#define FunOper_INDEXC			(FunOper_BigIndex_Begin+3)    // 函数:  INDEXC() 对应大盘收盘价
#define FunOper_INDEXDEC		(FunOper_BigIndex_Begin+4)    // 函数:  INDEXDEC() 对应大盘下跌家数
#define FunOper_INDEXH			(FunOper_BigIndex_Begin+5)    // 函数: INDEXH() 对应大盘最高价
#define FunOper_INDEXL			(FunOper_BigIndex_Begin+6)    // 函数: INDEXL() 对应大盘最低价
#define FunOper_INDEXO			(FunOper_BigIndex_Begin+7)    // 函数:  INDEXO() 对应大盘开盘价
#define FunOper_INDEXV			(FunOper_BigIndex_Begin+8)    // 函数:  INDEXV() 对应大盘成交量

// 常用函数
#define FunOper_InCommonUse_Begin    (FunOper_BigIndex_End + 1)
#define FunOper_InCommonUse_End      (FunOper_InCommonUse_Begin + 30)

#define FunOper_CAPITAL		(FunOper_InCommonUse_Begin+1)    // 函数: CAPITAL() 流通盘大小
#define FunOper_VOLUNIT		(FunOper_InCommonUse_Begin+2)    // 函数: VOLUNIT() 每手股数

#define FunOper_DYNAINFO	(FunOper_InCommonUse_Begin+3) // 动态行情函数

#define FunOper_FINANCE		(FunOper_InCommonUse_Begin+4)  // 财务指标

// 专业财务数据
#define FunOper_PROFFIN		(FunOper_InCommonUse_Begin+5)  // 专业财务数据
#define FunOper_PROFSTR		(FunOper_InCommonUse_Begin+6)  // 专业财务字符串数据
#define FunOper_PROFFINLAST	(FunOper_InCommonUse_Begin+7)  // 专业财务数据截止期位置
#define FunOper_PROFFINON	(FunOper_InCommonUse_Begin+8)  // 指定日期的专业财务数据
#define FunOper_PROFSTRON	(FunOper_InCommonUse_Begin+9)  // 指定日期的专业字符串财务数据
#define FunOper_PROFFINTERM	(FunOper_InCommonUse_Begin+10) // 专业财务数据报告期
#define FunOper_SETPROFFIN	(FunOper_InCommonUse_Begin+11) // 设置专业财务数据

#define FunOper_FOR			(FunOper_InCommonUse_Begin+12) // for 循环
#define FunOper_CONTINUE	(FunOper_InCommonUse_Begin+13) // continue
#define FunOper_BREAK		(FunOper_InCommonUse_Begin+14) // break
#define FunOper_OPERATOR	(FunOper_InCommonUse_Begin+15) // operator
#define FUNOPER_STROUT		(FunOper_InCommonUse_Begin+16) // strout
#define FUNOPER_DATETOSTR   (FunOper_InCommonUse_Begin+17) // DATETOSTR // 日期转换到字符串
#define FUNOPER_BLOCKCALC   (FunOper_InCommonUse_Begin+18) // 板块计算

// 函数常量
#define FunOper_Constant_Begin    (FunOper_InCommonUse_End + 1)
#define FunOper_Constant_End      (FunOper_Constant_Begin + 35)

		// 画线类型常量
#define FunOper_STICK			(FunOper_Constant_Begin+1)  // 柱状线，从0到指标值间画垂直直线
#define FunOper_COLORSTICK		(FunOper_Constant_Begin+2)  // 彩色柱状线，当值为0是显示红色，否则显示绿色
#define FunOper_VOLSTICK		(FunOper_Constant_Begin+3)  // 成交量柱状线，当股价上涨时显示红色空心柱，否则显示绿色空心柱
#define FunOper_LINESTICK		(FunOper_Constant_Begin+4)  // 同时画出柱状线和指标线
#define FunOper_CROSSDOT		(FunOper_Constant_Begin+5)  // 小叉线
#define FunOper_CIRCLEDOT		(FunOper_Constant_Begin+6)  // 小圆圈线
#define FunOper_POINTDOT		(FunOper_Constant_Begin+7)  // 小圆点线
#define FunOper_DRAW3D			(FunOper_Constant_Begin+8)  // 3d
#define FunOper_LINEDRAW3D		(FunOper_Constant_Begin+9)  // 3d曲线
#define FunOper_NOTDRAWLINE		(FunOper_Constant_Begin+10) // 当前曲线不绘制
#define FunOper_NOTDRAWTEXT		(FunOper_Constant_Begin+11) // 当前文本不绘制
#define FunOper_NOTDRAWLINETEXT	(FunOper_Constant_Begin+12) // 当前曲线文本都不绘制
#define FunOper_NOTDRAW			(FunOper_Constant_Begin+13) // 不绘制图形
#define FunOper_NOTTABLE		(FunOper_Constant_Begin+14) // 不在表格里显示
#define FunOper_NOTMAXMIN		(FunOper_Constant_Begin+15) // 不计算最大最小值,在绘图时使用

	// 五彩K线颜色
#define FunOper_COLORRED	    (FunOper_Constant_Begin+16)  // 红色
#define FunOper_COLORGREEN		(FunOper_Constant_Begin+17)  // 绿色
#define FunOper_COLORBLUE		(FunOper_Constant_Begin+18)  // 蓝色
#define FunOper_COLORBLACK		(FunOper_Constant_Begin+19)  // 黑色
#define FunOper_COLORWHITE		(FunOper_Constant_Begin+20)  // 白色
#define FunOper_COLORGRAY		(FunOper_Constant_Begin+21)  // 灰色
#define FunOper_COLORYELLOW		(FunOper_Constant_Begin+22)  // 黄色
#define FunOper_COLORCYAN		(FunOper_Constant_Begin+23)  // 青色
#define FunOper_COLORMAGENTA	(FunOper_Constant_Begin+24)  // 品红色
#define FunOper_COLORBROWN		(FunOper_Constant_Begin+25)  // 棕色
#define FunOper_COLORBRGB		(FunOper_Constant_Begin+26)  // RGB

	// 线风格常量
#define FunOper_SOLID            (FunOper_Constant_Begin+27)
#define FunOper_DASH             (FunOper_Constant_Begin+28)       /* -------  */
#define FunOper_DOT              (FunOper_Constant_Begin+29)       /* .......  */
#define FunOper_DASHDOT          (FunOper_Constant_Begin+30)       /* _._._._  */
#define FunOper_DASHDOTDOT       (FunOper_Constant_Begin+31)       /* _.._.._  */

#define FunOper_DRAWNULL		 (FunOper_Constant_Begin+32) // 返回不绘制类型
//--2010.10.8 litao #define FunOper_RBTREND		     (FunOper_Constant_Begin+33) // 返回不绘制类型



#define FunOper_Draw_Begin       (FunOper_Constant_End + 1)
#define FunOper_Draw_End	     (FunOper_Draw_Begin + 30)

// 用户绘图函数
#define FunOper_DRAWICON         (FunOper_Draw_Begin+1)
#define FunOper_DRAWLINE         (FunOper_Draw_Begin+2)
#define FunOper_DRAWTEXT         (FunOper_Draw_Begin+3)
#define FunOper_POLYLINE         (FunOper_Draw_Begin+4)
#define FunOper_STICKLINE        (FunOper_Draw_Begin+5)

// 当前数据状态
#define hx_EXCUTE_PROMPT_ERROR   0x00000001 // 执行时，如果有错误，则添加到错误列表。(缺省不添加)
#define hx_GETLINE_DRAWEXPRESS   0x00000002 // 得到一行绘图表达式
#define hx_GetCurrentPosData     0x00000004 // 得到当前位置数据
#define hx_StrOutData			 0x00000008 // 当前输出是字符串数据

#define hx_CURRENTDATENOTDELETE  0x00010000 // 当前数据不能删除

#define HX_USERDRAWFUN_EXPRESS	 0x00000100  // 使用绘图函数,参见CGraphics::USERDRAWLINE (Graphics.h)

#define HX_USER_COMPILE_INFO	WM_USER + 101 // 编译时发送的消息
#define HX_USER_INITFUN			WM_USER + 102 // 初始化函数消息
#define HX_USER_SENDADDSTR		WM_USER + 103 // 向编辑公式view发送添加字符串
#define HX_USER_LOADCUR 		WM_USER + 104 // 装入当前公式（缺省）
#define HX_USER_DELETECUR 		WM_USER + 105 // 删除当前公式（缺省）
// 禁用周期
#define HX_PERIOD_ONEDEAL         0x0001 // 分笔成交
#define HX_PERIOD_ONEMINUTE       0x0002 // 1分钟线
#define HX_PERIOD_FIVEMINUTE      0x0004 // 5分钟线
#define HX_PERIOD_FIFTEENMINUTE   0x0008 // 15分钟线
#define HX_PERIOD_THIRTYMINUTE    0x0010 // 30分钟线
#define HX_PERIOD_SIXTYMINUTE     0x0020 // 60分钟线
#define HX_PERIOD_PERIOD_TYPE_DAY             0x0040 // 日线
#define HX_PERIOD_WEEK            0x0080 // 周线
#define HX_PERIOD_MONTH           0x0100 // 月线
#define HX_PERIOD_MOREDAY         0x0200 // 多日线

// 默认周期字符串
#define STRING_PERIOD_ONEDEAL          _T("分笔成交")
#define STRING_PERIOD_ONEMINUTE        _T("1分钟线" )
#define STRING_PERIOD_FIVEMINUTE       _T("5分钟线" )
#define STRING_PERIOD_FIFTEENMINUTE    _T("15分钟线")
#define STRING_PERIOD_THIRTYMINUTE     _T("30分钟线")
#define STRING_PERIOD_SIXTYMINUTE      _T("60分钟线")
#define STRING_PERIOD_PERIOD_TYPE_DAY  _T("日线"    )
#define STRING_PERIOD_WEEK             _T("周线"    )
#define STRING_PERIOD_MONTH            _T("月线"    )
#define STRING_PERIOD_MOREDAY          _T("多日线"  )

//end 

#ifdef VC_7_0

template<class BASE_CLASS, class KEY, class VALUE>

#else

#define BASE_CLASS CMapStringToOb
template<class _CLASS,class KEY, class VALUE>

#endif

class CHSTypedPtrMap : public BASE_CLASS
{
protected:
	CArray<class CValue*,class CValue*>* m_ayObj;
public:
// Construction
	CHSTypedPtrMap(int nBlockSize = 10)
		: BASE_CLASS(nBlockSize) 
	{
		m_ayObj = NULL;
	}
	
	~CHSTypedPtrMap()
	{
		if(m_ayObj != NULL)
		{
			delete m_ayObj;
			m_ayObj = NULL;
		}
	}

	void RemoveAll()
	{
		BASE_CLASS::RemoveAll();
		
		if(m_ayObj != NULL)
		{
			m_ayObj->RemoveAll();
			delete m_ayObj;
			m_ayObj = NULL;
		}
	}

	// Lookup
	BOOL Lookup(BASE_CLASS::BASE_ARG_KEY key, VALUE& rValue) const
		{ return BASE_CLASS::Lookup(key, (BASE_CLASS::BASE_VALUE&)rValue); }

	// Lookup and add if not there
	VALUE& operator[](BASE_CLASS::BASE_ARG_KEY key)
		{ return (VALUE&)BASE_CLASS::operator[](key); }

	void DeleteAt(CValue* pValoare)
	{
		if(m_ayObj == NULL)
			return;
		for(int i = 0; i < m_ayObj->GetSize();i++)
		{
			if(m_ayObj->GetAt(i) == pValoare)
			{
				m_ayObj->RemoveAt(i);
				i--; //20090122 YJT
			}
		}
	}

	// add a new key (key, value) pair
	void SetAt(KEY key, VALUE newValue)
	{
		CValue* valoare;
		if ( Lookup(key,valoare) )
		{
			DeleteAt(valoare);

			delete valoare;
			valoare = NULL;
		}

		BASE_CLASS::SetAt(key, newValue);

		if(m_ayObj == NULL)
		{
			m_ayObj = new CArray<class CValue*,class CValue*>;
		}
		m_ayObj->Add((CValue*)newValue);
	}

	void InsertAt(KEY key, VALUE newValue,int nIndex)
	{
		CValue* valoare;
		if ( Lookup(key,valoare) )
		{
			DeleteAt(valoare);

			delete valoare;
			valoare = NULL;
		}

		BASE_CLASS::SetAt(key, newValue);

		if(m_ayObj == NULL)
		{
			m_ayObj = new CArray<class CValue*,class CValue*>;
		}
		m_ayObj->InsertAt(nIndex,(CValue*)newValue);
	}

	void Add(KEY key, VALUE newValue)
	{
		CValue* valoare;
		if ( Lookup(key,valoare) )
		{
			DeleteAt(valoare);
			RemoveKey(key);
		}
		BASE_CLASS::SetAt(key, newValue);

		if(m_ayObj == NULL)
		{
			m_ayObj = new CArray<class CValue*,class CValue*>;
		}
		m_ayObj->Add((CValue*)newValue);
	}

	// removing existing (key, ?) pair
	BOOL RemoveKey(KEY key)
	{
		return BASE_CLASS::RemoveKey(key); 
	}

	// iteration
	void GetNextAssoc(POSITION& rPosition, KEY& rKey, VALUE& rValue) const
	{ 
		BASE_CLASS::GetNextAssoc(rPosition, (BASE_CLASS::BASE_KEY&)rKey,
			(BASE_CLASS::BASE_VALUE&)rValue); 
	}

public:
	CString GetObjectName(CObject* pObject)
	{
		POSITION pos = GetStartPosition();
		CString sName;
		CValue *pVal;
		while (pos != NULL)
		{	
			GetNextAssoc(pos, sName, pVal);
			if(pVal != NULL && pVal == pObject)
			{
				return sName;
			}
		}
		return "";
	}

	CArray<class CValue*,class CValue*>* GetObjArray()
	{
		return m_ayObj; 
	}

	CValue* GetAt(int nPos)
	{
		if(m_ayObj == NULL)
			return NULL;
		if( nPos >= 0 && nPos < m_ayObj->GetSize() )
		{
			return m_ayObj->GetAt(nPos);
		}
		return NULL;
	}

	short GetDrawCount(short& nDrawCount)
	{		
		nDrawCount = 0;
		if(m_ayObj == NULL)
			return 0;

		short nNotDraw = m_ayObj->GetSize();
		for(short i = 0; i < nNotDraw;i++)
		{
			if( m_ayObj->GetAt(i)->IsStyle(HX_EXPRESSDRAW) )
			{
				nDrawCount++;
			}
		}
		return (nNotDraw - nDrawCount);
	}
};
typedef CHSTypedPtrMap<CMapStringToOb,CString,class CValue*> CHSMapVariabile;

 // 执行时标识的序列号

struct ExecObjNoList
{
	CString strName;
	int iObjNo;
	CCriticalSection *pcsObjNo;				//临界区变量
	ExecObjNoList(){pcsObjNo = new CCriticalSection(); }
	~ExecObjNoList(){ delete pcsObjNo;}
};
typedef struct ExecObjNoList  EXECOBJNO;
typedef struct ExecObjNoList* LPEXECOBJNO;

struct HSExpressAutoSetup
{
	CString  m_strFileName; // 文件名称
	DWORD	 m_dwStye;      // 引入类型，定义 EXTERN_EXPRESS_*
	void*	 m_pData;       // 保留
	HSExpressAutoSetup()
	{
		m_dwStye = 0;
		m_pData  = NULL;
	}
};

/*************************************************************
*	类 名 称：	CMapVariabile
*	类 描 述：	表(StringToOb)中同时包含了数组
***************************************************************/
class CExpression;
class BCGCONTROLBARDLLEXPORT CMapVariabile : public CHSMapVariabile
{
public:
	CMapVariabile();
	~CMapVariabile();
public:
	// Lookup

	/******************************************************************
	*	函数功能：	根据Key得到相应的rValue值
	*	函数参数：	CString key    : 针对表中的键值
	*				CValue*& rValue: 返回的数据
	*	返 回 值：	BOOL : 为真则表示找到Key的相应值,否则没找到
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月10日
	*	修改说明：	
	*****************************************************************/
	BOOL Lookup(CString key, CValue*& rValue) const;

	/******************************************************************
	*	函数功能：	根据Key及cType得到相应的rValue值
	*	函数参数：	CString key : 针对表中的键值
	*				char cType  : 公式类型, Tech, Exchange...
	*				CValue*& rValue : 返回的数据
	*	返 回 值：	BOOL  : 为真则表示找到Key的相应值,否则没找到
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月10日
	*	修改说明：	
	*****************************************************************/
	BOOL Lookup(CString key,char cType, CValue*& rValue) const;

	/******************************************************************
	*	函数功能：	当前的键值是否存在
	*	函数参数：	CString key : 针对表中的键值
	*				CValue*& rValue : 返回的数据
	*	返 回 值：	BOOL  : 为真则表示找到Key的相应值,否则不存在
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月10日
	*	修改说明：	
	*****************************************************************/
	BOOL IsExist(CString key, CValue*& rValue) const;

	/******************************************************************
	*	函数功能：	向表中加一个公式
	*	函数参数：	CString key     : 公式名
	*				CValue* rValue  : 公式体
	*	返 回 值：	CValue*			: 不成功则返回NULL, 否则返回rValue
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月10日
	*	修改说明：	
	*****************************************************************/
	CValue* AddExpress(CString key,CValue* rValue);

	/******************************************************************
	*	函数功能：	根据key+cType组成键值,将其从表中删除
	*	函数参数：	CString key : 键的后部分键值 
	*				char cType  : 根据公式类型决定键的第一位数据
	*	返 回 值：	BOOL        : TRUE是表示删除成功,否则失败
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月10日
	*	修改说明：	
	*****************************************************************/
	BOOL    RemoveKey(CString key,char cType);

	/******************************************************************
	*	函数功能：	同上,只是公式类型从pExp里面得到这点区别
	*	函数参数：	CString key
	*				CExpression* pExp : 公式体,可用pExp->GetExpressType()得到公式类型
	*	返 回 值：	BOOL  
	*	调用依赖：	RemoveKey(CString key,char cType);
	*	作    者：	
	*	完成日期：	2003年4月10日
	*	修改说明：	
	*****************************************************************/
	BOOL    RemoveKey(CString key,CExpression* pExp);

	/******************************************************************
	*	函数功能：	返回键值为Key的数据,没有则加一个
	*	函数参数：	CString key : 键值
	*	返 回 值：	CValue*&    : 返回的数据
	*	调用依赖：	调用CHSMapVariabile::operator[]
	*	作    者：	
	*	完成日期：	2003年4月10日
	*	修改说明：	
	*****************************************************************/
	CValue*& operator[](CString key);
};
/******************************************************************
*	函数功能：	添加一个编译错误
*	函数参数：	struct nodErrorInfo* pError : 错误信息
*	返 回 值：	void 
*	调用依赖：	
*	作    者：	
*	完成日期：	2003年4月10日
*	修改说明：	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern void AddError(struct nodErrorInfo* pError);  // 

/******************************************************************
*	函数功能：	删除所有编译错误
*	函数参数：	（无）
*	返 回 值：	void 
*	调用依赖：	
*	作    者：	
*	完成日期：	2003年4月10日
*	修改说明：	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern void DeleteError();                          // 

/******************************************************************
*	函数功能：	得到编译错误总数
*	函数参数：	（无）
*	返 回 值：	int  
*	调用依赖：	
*	作    者：	
*	完成日期：	2003年4月10日
*	修改说明：	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern int  GetErrorCount();                        // 

/******************************************************************
*	函数功能：	由错误号得到错误信息
*	函数参数：	UINT nID
*	返 回 值：	CString  
*	调用依赖：	
*	作    者：	
*	完成日期：	2003年4月10日
*	修改说明：	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern CString  hxGetErrorInfoByErrorNum(UINT nID); // 

/******************************************************************
*	函数功能：	替换回车换行符
*	函数参数：	CString strExp : 源字串
*				CString seps   : 要替换的所有字符串
*				CString* pstrCur = NULL : 要将这符seps[i]替换掉的字串
*	返 回 值：	CString  : 替换后的结果, 如strExp = "a,b.c" seps=",." pstrCur = "A", 则结果为"aAbAc"
*	调用依赖：	
*	作    者：	
*	完成日期：	2003年4月10日
*	修改说明：	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern CString  hxRelaceReturnKey(CString strExp,CString seps,CString* pstrCur = NULL);  // 

/******************************************************************
*	函数功能：	删除列表
*	函数参数：	CMapVariabile* pVarsMap : 要删除的列表
*				BOOL bSelf              : 是否删除自己,如果为真则要运行delete  pVarsMap;
*	返 回 值：	CMapVariabile*          : pVarsMap
*	调用依赖：	
*	作    者：	
*	完成日期：	2003年4月10日
*	修改说明：	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern  CMapVariabile* DeleteVarsMap(CMapVariabile* pVarsMap,BOOL bSelf); // 

/******************************************************************
*	函数功能：	在表pVarsMap中得到一个唯一的名称
*	函数参数：	CString strOldName : 名称的初始值,得到的结果为以其为前缀,后面加一整数如 strOldName = "AVC",则结果可能为 "AVC120"
*				CMapVariabile* pVarsMap : 目标表
*	返 回 值：	CString	: 得到的唯一名称
*	调用依赖：	
*	作    者：	
*	完成日期：	
*	修改说明：	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern  CString		  GetIdentifierExpressName(CString strWholeName,CString strOldName,CMapVariabile* pVarsMap); // 

// 目前不用
BCGCONTROLBARDLLEXPORT extern int FindInMap(const CString& strKey,
											CMapVariabile* pVarsMap,
											DWORD dwFindMask = 0,
											CArray<CValue*,CValue*>* pArray = NULL,
											class CListCtrl* pCtrl = NULL);

/******************************************************************
*	函数功能：	初始化已定义的函数并加入表pExternFunction中
*	函数参数：	CMapVariabile*& pExternFunction : 将初始化好后的系统函数加入此表中
*	返 回 值：	void  
*	调用依赖：	
*	作    者：	
*	完成日期：	
*	修改说明：	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern  void   InitSysFunction(CMapVariabile*& pExternFunction);
// 存储已编译的公式信息
typedef BOOL ( *HSFunAddDefaultTechAnysis)(void* pExpress,BOOL bFind,void* pData,char cOperater);
typedef BOOL ( *HSFunAddToTree)(void* pExpress,void* pData,char cOperater);												 
BCGCONTROLBARDLLEXPORT extern  BOOL   SaveLoadMap(BOOL bSave,CMapVariabile*& pExternExpression,
												  CMapVariabile* pExternVariabile,CMapVariabile*  pExternFunction,
												  CComboBox* pBox = NULL,char* strFileName = NULL,
												  HSFunAddDefaultTechAnysis pFun = NULL,
												  HSFunAddToTree pFunAyTree = NULL);

BCGCONTROLBARDLLEXPORT extern void    SetDefaultValues(HSDouble* pData,long lSize,HSDouble values);
BCGCONTROLBARDLLEXPORT extern void    SetDefaultValues(long* pData,long lSize);
BCGCONTROLBARDLLEXPORT extern CString ReplaceNumber(CString& strSource);

// 分解字串
BCGCONTROLBARDLLEXPORT extern int	  AnalyzeStr(CString strResource,const char* strDelimit,CStringArray& ayRet);
// 是否添加最后一个分号
BCGCONTROLBARDLLEXPORT extern void    RelaceLastChar(CString& strText);
// 使文本不是半个汉字
BCGCONTROLBARDLLEXPORT extern CString AdjustStr(CString pOrg,int& nReqLen,CString* pstrLeft = NULL);
// 是否是汉字
BCGCONTROLBARDLLEXPORT extern int     IsChinese(BYTE cChar,const CString& strSource,int& nPos);
BCGCONTROLBARDLLEXPORT extern int     IsChinese(BYTE cChar,const CString& strSource,int& nPos,BOOL bChange);

// 数据及技术指标缺省颜色
class CHSDataSource;
BCGCONTROLBARDLLEXPORT extern void    SetHxDataSource( CHSDataSource* pData,CArray<struct ColorProperty*,struct ColorProperty*>* pDefColours);
BCGCONTROLBARDLLEXPORT extern CString hxSubZero(CString str);

//
// 函数分类
enum  FunctionType
{	
	FunctionNone,
	FunctionQuotes,		 // 行情
	FunctionDateTime,	 // 时间
	FunctionReference,   // 引用
	FunctionLogical,	 // 逻辑
	FunctionArithmetic,	 // 算术
	FunctionMath,		 // 数学
	FunctionStat,		 // 统计
	FunctionTarget,		 // 指标
	FunctionBig,		 // 大盘
	FunctionInCommonUse, // 常用
	FunctionDraw,	     // 绘图

	FunctionUserDefine,  // 自定义
	FunctionConstant     // 此函数是常量
};

#define DataTypeOperStr		  0x01  // 在 nodElement 中 valoarestr成员 有效。
#define DataTypeOperCompile   0x02  // 编译方式
#define DataFunParam		  0x04  // 函数参数
#define DataExpParam		  0x08  // 引用参数
#define DataInport  		  0x10  // 引用数据
#define DataText  		      0x20  // 文本数据


// 错误信息
struct nodErrorInfo
{
	int		  m_nError;     // 错误号
	int		  m_nBegin;     // 错误开始位置
	int       m_nEnd;       // 错误结束位置
	CString*  m_pError;     // 错误信息
	CString*  m_pHelp;      // 提示、帮助

	nodErrorInfo();
	/*nodErrorInfo(int nError,int nBegin = -1,int nEnd = -1, CString pError = "",CString pHelp = "");*/
	nodErrorInfo(int nError,int nBegin = -1,int nEnd = -1, 
		const char* pError = NULL, const char* pHelp=NULL);

	CString  GetError();     // 错误信息
	CString  GetHelp();      // 提示、帮助
	
	~nodErrorInfo();
};

// 函数参数
struct FormulaGeneralParam;
struct FunctionParam
{
	void*			 pFun;	// 函数指针
	FormulaGeneralParam* ayVars; // 函数参数列表指针

	FunctionParam();
	~FunctionParam();
	
	virtual void  Serialize(class CExpression* pExpress,struct nodElement* nod,CArchive& ar);
};

// 引用公式参数
struct ExpressParam
{
	void*		   pValue; // 引用的公式指针 
	CString		   strSub; // 引用的公式中的输出项
	void*		   pOther; // 一般为空,不用

	ExpressParam();
	~ExpressParam();
};

// 引用公式参数
struct InportDataParam
{
	CHSDataSource*	m_pDataSrc;	// 引用的数据源

	InportDataParam();
	~InportDataParam();
};

typedef struct StockDay* LPSTOCKDAY; // 日线数据

// 绘图返回数据
struct CFormulaDrawParam
{
	char	  m_strName[256];      // 名称
	char      m_strExpParam[256];  // 参数描述

	long      m_lStyle;		// 风格

	long      m_lColor;	   /*线颜色*/
	int		  m_nPenStyle; /*线风格*/
	int		  m_nWidth;	   /*线宽度*/
	CFormulaDrawParam();
};

struct BlockCurDispCalc
{
	double m_dValue;		//统计结果
};

struct BlockOtherData
{
	double m_dCapital; // 流通盘大小
};

// 计算时传入外部参数
//struct CCalcExternParam
//{
//	long		m_nBeginDate; // 开始日期,如19901020
//	long	    m_nEndDate;   
//	CodeInfo    m_sCode;	  // 代码
//	char 		m_szBlock[BLOCK_NAME_LENGTH]; // 板块名称
//
//	CCalcExternParam()
//	{
//		EmptyBlock();
//	}
//
//	void    EmptyBlock() { memset(m_szBlock,0,sizeof(m_szBlock)); }
//	CString GetBlockName() { return CString(m_szBlock).Left(BLOCK_NAME_LENGTH); }
//	void    SetBlockName(CString pData)
//	{
//		EmptyBlock();
//		if( pData.IsEmpty() )
//			return;
//
//		strncpy(m_szBlock,pData,sizeof(m_szBlock));
//	}
//};

/*************************************************************
*	类 名 称：	CLineProperty
*	类 描 述：	绘图数据(在盘后分析里面用到)
***************************************************************/
class  CExpValue;
/*************************************************************
*	类 名 称：	BlockCalcRet
*	类 描 述：	板块计算结果
***************************************************************/
struct BlockCalcRet
{
	CArray<double,double> ayValues;		//从第3列开始的计算结果,与ayColName对应
};

/*************************************************************
*	类 名 称：	SelStockResult
*	类 描 述：	选投结果数据中间变量
***************************************************************/
struct SelStockResult
{
	HSDouble dData;
	void*	 pStock;
	void*    pAyStock; // 
 };
/*************************************************************
*	类 名 称：	BlockTypeInfo
*	类 描 述：	板块类型信息
***************************************************************/
struct BlockTypeInfo
{
	CString		strTypeName;	//类型名
	// 本类所有的板块信息
	CArray<BlockInfo*,BlockInfo*>	ayBlock; 
	// 所有要计算的股票信息
	CArray<StockUserInfo*,StockUserInfo*> ayCalcStock;		
	// 要返回的其它数据,与ayCalcStock对应
	CArray<BlockOtherData*,BlockOtherData*> ayOtherData;		
	//计算结果,与ayCalcStock对应
	CArray<BlockCalcRet*,BlockCalcRet*> ayCalcRet;			
	// 公式数组
	void*	m_pColName;        

	// 清空资源
	~BlockTypeInfo();
	void Free();
};
typedef struct BlockTypeInfo TYPEANDBLOCK;
/*************************************************************
*	类 名 称：	CDataSourceUseData
*	类 描 述：	在数据源类(CHxDataSoures)里使用,一个对象表示一个类型的数据串
***************************************************************/
class CDataSourceUseData
{
public:
	HSDouble*    m_pData;    // 数据
	int			 m_nSize;
public:
	// 初始化处理
	CDataSourceUseData();
	// 清空处理
	~CDataSourceUseData();
	// 返回m_pData
	HSDouble* GetData();
	// 空间是否为空
	BOOL      IsEmpty();
	// 分配置空间
	CDataSourceUseData* Alloc(int nCurSize,int nOldSize,long pLastDay = 0);
	// 清空空间
	void Free();
};
// 数据源里的项
typedef CTypedPtrMap<CMapWordToPtr,WORD,class CDataSourceUseData*> CMapDataSource;

// 日线数据项ID
#define hx_OPEN_DATA   0x01 // 开盘价数据 
#define hx_HIGH_DATA   0x02 // 最高价数据 
#define hx_LOW_DATA    0x03 // 最低价数据 
#define hx_CLOSE_DATA  0x04 // 收盘价数据 
#define hx_VOL_DATA    0x05 // 成交量数据 
#define hx_AMOUNT_DATA 0x06 // 成交额数据 
#define hx_DATE_DATA   0x07 // 日期数据  格式为19910101

#ifdef SUPPORT_NETVALUE
#define hx_NetValue_DATA 0x08 // 国债利率，基金净值
#endif

// 数据类型
#define hx_PERIOD_TYPE_DAY_DATA		  0x00000001 // 分析周期：>=日
#define hx_MINUTE_DATA	              0x00000002 // 分析周期：>=分钟

/*************************************************************
*	类 名 称：	FormulaCompareSubData
*	类 描 述：	要比较的数据结构
***************************************************************/
struct FormulaCompareSubData
{
	enum 
	{
		DaYu,		//  大于
		XiaoYu,		//	小于
		DengYu,		//	等于
		JieYu,		//	介于
		ShangChuan, //	上穿
		XiaoPo,		//	下破
		WeiChi,		//	维持
		BiaoChi,	//	保持

		Condition, // 条件选股
		Exchange,  // 交易系统
		MoreKLine,  // 五彩K线
		Strategy   // 策略交易  //20090302 YJT 增加 Strategy
	};

	char     m_cType; // 操作类型
	CString m_dData1; // 操作符右边1
	CString m_dData2; // 操作符右边2
	
	void Copy(FormulaCompareSubData* pData);
	FormulaCompareSubData();
	void Serialize(CArchive& ar);
};


struct StockArea
{
	CString strText; // 板块股
	DWORD	lData;   // 分类股

	static CArray<StockUserInfo*,StockUserInfo*> ayStock;
	static void AddStock(DWORD market);
	StockArea();
};

/*************************************************************
*	类 名 称：	SingleSelCondition
*	类 描 述：	单条件选股
***************************************************************/
class SingleSelCondition
{
public:
	CString			m_strExp;		// 条件/公式名称
	int				m_nType;		// 公式类型

	short			m_nPeriod;		// 周期
    CString			m_strCondition; // 技术指标指:哪一个指标线;交易系统指:哪种信号;基本面指:财务指标ID
	FormulaCompareSubData m_SubData;		// 要与m_strCondition指标计算结果比较的指标或数据

	SingleSelCondition();

	void Copy(SingleSelCondition* pData);
	void Serialize(CArchive& ar);

#ifndef _HSDiskDll_
	virtual CString GetDescribe(void* pData = NULL);
	virtual BOOL	Open(void* pData = NULL);
#endif

};
/*************************************************************
*	类 名 称：	UnionSelConditionSub
*	类 描 述：	组合条件选股里的一项
***************************************************************/
struct UnionSelConditionSub
{
	CString m_strName;		// 名称
	short	m_nBeginDate;	// 开始时间
	short	m_nEndDate;		// 结束时间

	short	m_lDateCount;	// 周期数
	long	m_nCalcEndDate; // 计算结果时间

	char    m_cAndOr;		// 条件项是与还是或的关系
    CArray<SingleSelCondition*,SingleSelCondition*>* m_aySub; // 单个选股条件

	void Free();
	void Copy(UnionSelConditionSub* pData);
	UnionSelConditionSub();
	~UnionSelConditionSub();
	void Serialize(CArchive& ar);
};

struct UnionSelCondition;
struct SelStockCondition
{
	WORD				  m_dStyle;					// 风格
	CArray<StockArea*,StockArea*>* m_ayCheck; // 股票范围
	CString*			  m_strBlock;               // 板块名

	SingleSelCondition*		  m_sSingleSel;			    // 单个选股条件
	UnionSelCondition*			  m_pHSUnion;			    // 组合选股条件

	unsigned long				  m_lEndDate;				// 计算终止日期  //--2010.11.12 litao修正2012时间问题
	long				  m_lDateCount;				// 计算日期长度

	void* m_pData;									// 其它附带参数(备用)

	SelStockCondition();
};

/*************************************************************
*	类 名 称：	UnionSelCondition
*	类 描 述：	组合条件选股m_aySub.GetSize()目前都为1
***************************************************************/

struct UnionSelCondition
{
	CString m_strName; // 组合条件名称
    CArray<UnionSelConditionSub*,UnionSelConditionSub*>* m_aySub; // 组和条件项

	void Free();
	void Copy(UnionSelCondition* pData);
	UnionSelCondition();
	~UnionSelCondition();
	void Serialize(CArchive& ar);
};


struct SuccResult  // 选股成功信息
{
	unsigned long	m_lDate;  // 时间
	long	m_nPos;   // 当前k线位置

#ifndef HS_SUPPORT_UNIX
	static CString m_strTip; // 选股成功信息提示

	SuccResult(unsigned long lDate,long nPos)
	{
		m_lDate = lDate; // 时间
		m_nPos  = nPos;
	}
#endif

};
		

struct HSGridBlockData
{
	CString		m_strTypeName;						// 类型名
	CUIntArray  *m_payIdx;							// 本类型含有哪些板块（为m_payAllBlock中的索引数据数组）

	static CArray<BlockInfo*,BlockInfo*> *m_payAllBlock; // 外部空间，所有板块列表，在此不能删除

	HSGridBlockData()
	{
		m_payIdx = NULL;
		m_payAllBlock = NULL;
	}

	~HSGridBlockData()
	{
		if( m_payIdx != NULL )
		{
			m_payIdx->RemoveAll();
			delete m_payIdx;
			m_payIdx = NULL;
		}
	}
};
struct HSOpenExpressUseData
{
	CString* m_strName; // 表达式名称
	void*	 m_pData;	// 公式指针
	DWORD	 m_wType;   // 类型

	DWORD	m_dStyle;	// 公式风格，指向CExpression:m_dStyle,参照：HS_EXP_FINANCE 等定义
	// 过滤公式用，如财务数据公式，只要=HS_EXP_FINANCE即可.

	HWND	m_hParent;

	short    m_ShowShortKey;   //标记使用，为了在报价界面放开21 23快捷键

	HSOpenExpressUseData();
	HSOpenExpressUseData(WORD wType);
	BOOL IsType(WORD wType);
};

//add by lxqi 20090311
//策略交易监视信息
struct StrategyInfo
{
	//参数////////////////////////////////////////////////////////////////////////////
	short			m_nID;				// 监控ID
	char			m_cUserNo[20];		// 用户资金账号		

	short			m_cCodeType;		// 证券类型（可以指向市场大类）
	char			m_cCode[6];			// 证券代码
	char			m_sCodeName[10];	// 证券名称
	char			m_sExpName[32];		// 公式名称
	//----------------------------------------------------------
	char			m_cTrigName[32];			//触发策略名称
	int				m_lStartTime;		//有效时间起始
	int				m_lEndTime;			//有效时间止
	char			m_cTradeName[32];	// 交易策略名称
	//==========================================================
	short			m_nPeriodType;		// 指标周期 参考 BaseDefine.h中的PERIOD_TYPE_DAY等
	short			m_nPeriodNumber;	// 任意周期时，指定周期
	short			m_nStatus;			// 监控状态（0停止，1启动）

	short			m_nBuyTimes;		// 买入次数，达到限制次数后不再发送买入请求。
	short			m_nMaxBuyTimes;		// 买入次数限制，买入次数达到此限制次数后不再买入。-1表示不控制
	short			m_nSellTimes;		// 卖出次数，达到次数后不再发送卖出请求。-1表示不控制
	short			m_nMaxSellTimes;	// 卖出次数限制，卖出次数达到此限制次数后不再卖出。-1表示不控制
	short			m_nTrigTimes;		// 连续触发次数，连续触发m_nMaxTrigTimes次后，进行交易
	short			m_nMaxTrigTimes;	// 触发次数上限，达到此上限则触发交易
	short			m_nDuration;		// 触发持续时间（秒），触发后m_nDuration秒内仍然触发，则进行交易
	short			m_nGap;				//连续委托间隔第一次触发交易之后，即停止对该股票的监控，直到“连续委托间隔(比如30秒)”之后再开始重新监控该股票，
										//“连续委托间隔”默认30秒。当连续间隔设置为-1，表示当日只进行一次建仓操作，则发生一次自动建仓交易后停止自动建仓。
	long			m_nLastTrigTime;	//上次触发的时间
	short			m_bFuQuan;			//是否复权
	short			m_bXD;			//下单时是否提示
	//////////////////////////////////////////////////////////////////////////
	//行情数据、公式
	short			m_nSize;			// 公式及数据个数
	CLineProperty*	m_pData;			// 监控相关数据（包括原始数据、监控结果和计算公式）
	StrategyInfo()
	{
		m_nID = 0;			
		m_cCodeType = 0;	

		ZeroMemory(m_cCode,6);
		ZeroMemory(m_sCodeName,10);
		ZeroMemory(m_sExpName,32);	
		ZeroMemory(m_cTradeName,32);
		ZeroMemory(m_cTrigName,32);
		m_nSize = 0;		
		m_nPeriodType = 0;	
		m_nStatus = 0;		
		m_nPeriodNumber = 0;
		
		m_lStartTime = 930;		//有效时间起始
		m_lEndTime = 1500;			//有效时间止
		m_nMaxBuyTimes = -1;
		m_nMaxSellTimes = -1;
		m_nBuyTimes = 0;	
		m_nSellTimes = 0;	
		m_nTrigTimes = 0;	
		m_nDuration = 0;	
		m_nGap = 0;
		m_nLastTrigTime = 0;
		m_bFuQuan = 1;
		m_nGap = 30;
		m_pData = NULL;
		m_bXD = 0;
	}
};

//add by lxqi 20090425	触发策略模板结构
struct StrategyTrig
{
	char			m_cName[32];			//触发策略名称
	//char			m_cExplain[64];		// 触发策略描述
	short			m_nReserve;			// 保留

	int				m_lStartTime;		//有效时间起始
	int				m_lEndTime;			//有效时间止
	short			m_nPeriodType;		// 指标周期 参考 BaseDefine.h中的PERIOD_TYPE_DAY等
	short			m_nPeriodNumber;	// 任意周期时，指定周期

	short			m_nMaxBuyTimes;		// 买入次数限制，买入次数达到此限制次数后不再买入。-1表示不控制
	short			m_nMaxSellTimes;	// 卖出次数限制，卖出次数达到此限制次数后不再卖出。-1表示不控制
	//short			m_nMaxTrigTimes;	// 触发次数上限，达到此上限则触发交易
	//short			m_nDuration;		// 触发持续时间（秒），触发后m_nDuration秒内仍然触发，则进行交易
	short			m_nGap;				// 连续委托间隔第一次触发交易之后，即停止对该股票的监控，直到“连续委托间隔(比如30秒)”之后再开始重新监控该股票，
										// “连续委托间隔”默认30秒。当连续间隔设置为-1，表示当日只进行一次建仓操作，则发生一次自动建仓交易后停止自动建仓。
	short			m_bFuQuan;			// 是否复权
	CExpValue*		m_pExpValue;		// 存放公式相关的指针
	StrategyTrig()
	{
		ZeroMemory(m_cName,32);
//		ZeroMemory(m_cExplain,64);
		m_lStartTime = 930;
		m_lEndTime = 1500;
		m_nPeriodType = 0;
		m_nPeriodNumber = 0;
		m_nMaxBuyTimes = -1;
		m_nMaxSellTimes = -1;
		//m_nMaxTrigTimes = 0;
		//m_nDuration = 0;
		m_nGap = 30;
		m_bFuQuan = 1;
		m_pExpValue = NULL;
	}
};
//end by lxqi 20090425

//add by lxqi 20090311	策略交易监视列表类型
typedef CTypedPtrMap<CMapWordToPtr,WORD,StrategyInfo*> CMapStrategyInfo;
//end by lxqi
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------

struct StrategyHis//策略交易历史重演
{
	CodeInfo		*m_pCode;			// 当前测试的股票信息
	char			m_sCodeName[16];	// 证券名称
	char			m_Tradecode[6];		// 交易代码
	short			m_TradeCodeType;	//交易代码类型
	//--------------------------------------------------
	char			m_cTrigName[32];		// 触发策略名称
	//=====================================================
	char			m_cTradeName[32];	// 交易策略名称
	float			m_nMaxGain;			// 盈利上限（百分比），当价格高于当前成本价达到盈利上限时，触发卖出全部持仓，-1不止盈
	float			m_nMaxDeficit;		// 亏损上限（百分比），当价格地狱当前成本价达到亏损上限时，触发卖出全部持仓，-1不止损
	short			m_nMaxBuyTimes;		// 买入次数限制，达到此限制后强制卖出所有，-1不限制
	short			m_nMaxSellTimes;	// 卖出次数限制，卖出次数达到此限制次数后不再卖出。-1表示不控制
	short			m_nBuyType;			// 买入类型，0：全部资金买入，1：部分资金买入(按比例)，2：固定资金买入，3：固定股数买入（手）
	double			m_nBuyNum;			// 买入数量，根据m_nBuyType判断具体数据涵义
	short			m_nSellType;		// 卖出类型，0：全部资金卖出，1：部分资金卖出(按比例)，2：固定资金卖出，3：固定股数卖出（手）
	double			m_nSellNum;			// 卖出数量，根据m_nSellType判断具体数据涵义
	
	short			m_nGap;				// 连续委托间隔第一次触发交易之后，即停止对该股票的监控，直到“连续委托间隔(比如30秒)”之后再开始重新监控该股票，
										// “连续委托间隔”默认30秒。当连续间隔设置为-1，表示当日只进行一次建仓操作，则发生一次自动建仓交易后停止自动建仓。
	short			m_bFuQuan;			// 是否复权
	short			m_bOpt;             // 是否是调优
	short			m_bByDay;			// 是否是每日分析
	double			m_nFees;			// 手续费（百分比），按照每次交易的金额计算

	double			m_nMoney;			// 总金额
	float			m_fCost;			// 已用金额
	long			m_lHolding;			// 持股数（股）
	
	short   m_nStop;//是否止损止盈
	short   m_nDynStop;//是否动态止损止盈
	double  m_lSellStopLossRange;//卖出止损幅度
	double  m_lSellStopProfitRange;//卖出止盈幅度

	/*year-month-day ,example: 19960616
	分钟数据的表示方法如下：yymmddhhnn(年月日时分)
	yy指的是year - 1990，故年份表达范围：1990 - 2011
	如0905131045，指的是：1999年5月13号10点45分。*/
	long  			m_nStartDate;		// 开始时间
	long	 		m_nEndDate;			// 结束时间
	long			m_lEffectStartTime;	// 有效时间起始
	long			m_lEffectEndTime;	// 有效时间止
	short			m_nPeriodType;		// 指标周期 参考 BaseDefine.h中的PERIOD_TYPE_DAY等
	short			m_nPeriodNumber;	// 任意周期时，指定周期
	char			m_sExpName[32];		// 公式名称
	short			m_nSize;			// 公式及数据个数
	CLineProperty*	m_pData;			// 监控相关数据（包括原始数据、监控结果和计算公式）
	CHSDataSource*  m_pTradeDataSrc;    // 用于交易的数据
	CMap<CString,LPCTSTR,double,double> m_mapVar;//参数
	StrategyHis()
	{
		m_pCode = NULL;
		ZeroMemory(m_sCodeName,16);
		ZeroMemory(m_sExpName,32);	
		ZeroMemory(m_cTradeName,32);
		ZeroMemory(m_cTrigName,32);
		ZeroMemory(m_Tradecode,6);
		m_bFuQuan = 1;
		m_fCost = 0;
		m_lHolding = 0;
		m_nMoney = 0;		
		m_nPeriodType = 0;	
		m_nPeriodNumber = 0;
		m_nStartDate = 0;	
		m_nEndDate = 0;	
		m_nSize = 0;	
		m_nMaxBuyTimes = -1;
		m_nMaxSellTimes = -1;
		m_nMaxGain=-1;
		m_nMaxDeficit=-1;
		m_nBuyType=0;
		m_nBuyNum=0;
		m_nFees=0;
		m_nGap =30;
		m_lEffectStartTime = 930;		//有效时间起始
		m_lEffectEndTime = 1500;			//有效时间止
		m_pData = NULL;
		m_pTradeDataSrc = NULL;
		m_nStop = 0;
		m_nDynStop = 0;
		m_lSellStopLossRange = 0;
		m_lSellStopProfitRange = 0;
		m_bOpt = 0;
		m_bByDay = 0;
	}
	void Free()
	{
		if (m_pCode)
		{
			ZeroMemory(m_pCode->m_cCode,6);
			m_pCode->m_cCodeType = 0;
		}
		ZeroMemory(m_sCodeName,16);
		ZeroMemory(m_sExpName,32);	
		ZeroMemory(m_cTradeName,32);
		ZeroMemory(m_cTrigName,32);
		ZeroMemory(m_Tradecode,6);
		m_bFuQuan = 1;
		m_fCost = 0;
		m_lHolding = 0;
		m_nMoney = 0;		
		m_nPeriodType = 0;	
		m_nPeriodNumber = 0;
		m_nStartDate = 0;	
		m_nEndDate = 0;	
		m_nMaxBuyTimes = -1;
		m_nMaxSellTimes = -1;
		m_nMaxGain=-1;
		m_nMaxDeficit=-1;
		m_nBuyType=0;
		m_nBuyNum=0;
		m_nFees=0;
		m_nGap =30;
		m_lEffectStartTime = 930;		//有效时间起始
		m_lEffectEndTime = 1500;			//有效时间止
		m_nStop = 0;
		m_nDynStop = 0;
		m_lSellStopLossRange = 0;
		m_lSellStopProfitRange = 0;
		m_bOpt = 0;
		m_bByDay = 0;
		if(m_nSize > 0)
		{
			delete[] m_pData;
			m_pData = NULL;
			m_nSize = 0;
		}
	}
};
//客户端数据包报头
struct CNI_General
{
	int	 m_nSize;	// 数据包长度
	long m_lCommID;	// 通讯属主
	int	 m_nEvent;	// 事件, CEV_...

	long m_nLoginSrvType;	// 参见RT_LOGIN定义
};
struct CNI_ReceiveData
{
	struct CNI_General m_hd;	//数据报头
	int m_nStatus;				// 状态CLX_TS_...
	BOOL m_bFile;				//是否为文件
	long m_lDataTotal;			//数据长度
	long m_lDataTransmited;		//已经传送的数据长度
	const char* m_pszData;		// if m_bFile is TRUE, m_pszData points to file name
};

#define FINANCIAL_DATA_LENGTH	118 /* 对应版本 20040401-2 */
/* 财务数据 */
struct UpdatedFinancialData
{
	char		m_strCode[6];	 	/* 股票代码	 */
	long		m_lBiuldDate;		/* 发生日期  */
	long		m_lModifyDate;		/* 修改日期	 */
	float		m_fData[FINANCIAL_DATA_LENGTH]; /* 财务数据项 */
};
/* 财务报表分析的ID属性结构 */
struct FinancialColProperty
{
	//DWORD	m_dwStyle;				/* 风格, 保留未使用 */
	DWORD	m_dwID;					/* 字段ID */

	CString	m_strSimpleEnglish;		/* 英文简称 */
	//CString	m_strSimpleChinese;	/* 中文简称 */

	FinancialColProperty()
	{
		m_dwID = 0;		
	}

	FinancialColProperty(DWORD	dwID,	LPCTSTR	lszSimpleEnglish,
		LPCTSTR	lpszSimpleChinese = NULL, DWORD	dwStyle = 0)
	{
		ASSERT(lszSimpleEnglish );

		//m_dwStyle	= dwStyle;
		m_dwID		= dwID;

		m_strSimpleEnglish.Format("%s", lszSimpleEnglish);	

		/*if(lpszSimpleChinese)
		{
			m_strSimpleChinese.Format("%s", lpszSimpleChinese);		
		}*/
	};

	CString GetSimpleChinese()
	{
		CString strChinese;
		ASSERT(strChinese.LoadString(m_dwID));
		return strChinese;
	}
};
class AllFinancialData : public HSRetFileData
{
public:
	AllFinancialData() ;
	~AllFinancialData() ;

	HSDouble GetDataByName(UpdatedFinancialData* pData, CString nID);
	//	根据财务数据的ID号返回相对应的财务数据
	HSDouble GetDataByID(UpdatedFinancialData* pData, UINT nID);
	//	根据财务数据的相对位置返回相对应的财务数据
	HSDouble GetDataByPos(UpdatedFinancialData* pData, int nPos);

	//	检测数据是否有效
	BOOL IsEmpty(CString code); 	

protected:
	CCalcExternParam* m_pCalcExternParam;
public:
	BOOL IsMyBlockData(int nBegin, int nEnd, CString strBlock);
	void SetMyBlockData(int nBegin, int nEnd, CString strBlock);
public:
	/* 取得当前数据长度 */

	long GetSize(){return m_lSize / sizeof(UpdatedFinancialData);}

	/* 通过数据索引位置, 返回数据指针 */

	UpdatedFinancialData* GetData(int nPos);

	/* 根据日期,返回该数据属于哪一期期 1,2,3,4*/
	int GetType(long lDate);	

public:
	void Find(int Begin,int End, int &nBeginPos, int &nEndPos);

	/* 根据日期折半查找, 返回数据索引位置 模糊查找或准确查找*/
	int Find(long lDate, long nLow = -1, long nHigh = -1, BOOL bBlur = TRUE,
	BOOL bBinarySearch = TRUE);	
public:
	void ReSort();
public:
	/* 财务报表分析财务数据ID的哈希表 */
	static CMapStringToPtr* m_pMapStringToID;
public:
	static CMapStringToPtr* GetMap();
	static void DeleteMap();
public:	
	/* 
	通过英文简称, 返回该ID(如果pProperty不为空, 则复制数据到pProperty)
	*/
	int GetFinColProperty(CString strSimpleEn, FinancialColProperty* pProperty = NULL);
};


class CFinanceStatmentFile
{
public:
	CFinanceStatmentFile(void);
	~CFinanceStatmentFile(void);
public:
	/* 安装数据文件 文件结构见通讯协议*/
	CString	Setup(void* pParamData , int* pThreadStatus = NULL); 
	/* 安装更新的数据 */
	BOOL	Setup(UpdatedFinancialData* pData, const short nSize, const char cAppend = TRUE); 
public:
	/* 从文件中取得数据 */
	static BOOL	Load(LPCTSTR lpszCode, AllFinancialData* pAllFinancialData);

public:
	/* 取得代码数据文件路径(客户端) */
	static CString GetFilePath(LPCTSTR lpszCode);
public:
	/* 将映射表的文件缓冲写盘,关闭文件,释放映射表 */
	BOOL	Flush();
protected:
	/* 股票代码与文件指针的映射表*/
	CMapStringToPtr* m_mapPosition;
	BOOL Lookup( LPCTSTR key, CFile*& rValue);
};


/*
请求类型: RT_TECHDATA / RT_TECHDATA_EX
功能说明: 盘后分析
*/

///*请求结构*/
//struct ReqDayData 	
//{
//	short				m_nPeriodNum;		// 周期长度,服务器不用
//	unsigned short		m_nSize;			// 本地数据当前已经读取数据起始个数,服务器不用
//	long				m_lBeginPosition;	// 起始个数，-1 表示当前位置。 （服务器端已经返回的个数）
//	unsigned short		m_nDay;				// 申请的个数
//	short				m_cPeriod;			// 周期类型
//	CodeInfo			m_ciCode;			// 申请的股票代码
//
//	void To(char cIntelToComputer )
//	
//	}
//};



template <class THSMapBaseType>
class CMapStringToPtrEx : public CMapStringToPtr
{
public:
	// add a new (key, value) pair
	void SetAt(LPCTSTR key, void* newValue)
	{
		void* pData;
		if( this->Lookup(key,pData) ) // 是否存在
		{
			return;
		}
		CMapStringToPtr::SetAt(key, newValue);	
	}

	BOOL SetAt(LPCTSTR key, THSMapBaseType newValue)
	{
		void* pData;
		if( this->Lookup(key,pData) ) // 是否存在
		{
			return FALSE;
		}
		CMapStringToPtr::SetAt(key, (void*)newValue);
		return TRUE;
	}

	BOOL SetAt(CodeInfo* pCode, THSMapBaseType newValue)
	{
		if( pCode == NULL )
			return FALSE;
		CString key;
		key.Format("%s",pCode->GetKey());
		void* pData;
		if( this->Lookup(key,pData) ) // 是否存在
		{
			return FALSE;
		}
		CMapStringToPtr::SetAt(key, (void*)newValue);
		return TRUE;
	}

	// Lookup
	BOOL Lookup(LPCTSTR key, void*& rValue) const
	{ return CMapStringToPtr::Lookup(key, rValue); }

	BOOL Lookup(CodeInfo* pCode, char* strKey,THSMapBaseType& rValue) const
	{ 		
		return CMapStringToPtr::Lookup( pCode->GetKey(strKey), (void*&)rValue ); 
	}

	void Free()
	{
		POSITION pos = this->GetStartPosition();
		CString	sName;
		void *pVal;
		while (pos != NULL)
		{
			this->GetNextAssoc (pos, sName, pVal);
			if(pVal != NULL)
			{
				delete (THSMapBaseType)pVal;
			}
		}
		this->RemoveAll();
	}
};

// 将pCode加入全局的选股结果列表中,并返回对应的返股成功数据数组
extern CArray<SuccResult*,SuccResult*>* AddSuccDataToMap(CodeInfo* pCode);
extern CTime LongToDate(unsigned long lDate);


//========================================================================
#endif
