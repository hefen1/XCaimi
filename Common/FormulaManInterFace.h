/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	FormulaManinterface.h
*	文件标识：	公式管理模块接口
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/
#ifndef FORMULAMANINTERQFACE
#define FORMULAMANINTERQFACE

#include "hsds_comudata.h"
#include "FormulaManPublic.h"
/*#include "ExpPublic.h"*/
#define OBJ_FORMULAMAN	_T("FormulaMan.FormulaManager")

interface IFormulaMan
{
	/******************************************************************
	*	函数功能：	设置日线数据到chsdatasorce数据列表m_ayDayData中
	*	函数参数：	StockDay* pDay		: [in] 日线数据
	*				long lCurSize		: [in] 日线数据长度
	*				int nVolumeUnit		: [in] 成交量单位
	*				int nMoneyUnit		: [in] 成交额单位
	*				short nMoneyDecimal	: [in] 成交额小数位
	*				int nPriceUnit		: [in] 价格单位
	*				short nPriceDecimal	: [in] 价格小数位
	*				WORD nKey = 0		: [in] 为0时,表示设置所有的K线,否则,只设置一个对应nKey键值的数据
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	virtual BOOL SetDayData(StockDay* pDay,long lCurSize,
		int nVolumeUnit,
		int nMoneyUnit,short nMoneyDecimal,
		int nPriceUnit,short nPriceDecimal,CodeInfo* pCodeinfo = NULL,
		WORD nKey = 0) = 0;

	/******************************************************************
	*	函数功能：	公式计算
	*	函数参数：	CString strname             : [in] 指标名称
	*				CArray<CLineProperty*,CLineProperty*>* pAyData  [out] 返回计算结果
	*				DWORD style					: [in] 指标类型
	*				void* data                  : [out] 返回结果
	*				HSCurDataInfoStatus* pCurDiataStatus = NULL : [out] 只有在表达式引用时有用,存放计算结果
	*				CTValues<void>* pRefCalcValues = NULL : [out] 只有在表达式引用时有用,存放计算结果位置(起点及终点)
	*	返 回 值：	BOOL  : 参数不正确(FALSE), 其它均为(TRUE)
	*	返 回 值：	long : 各种消息的返回值各不相同,具体在消息处理函数里说明
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
 	virtual BOOL CalcValue(  CString strName,DWORD style,CArray<CLineProperty*,CLineProperty*>* pAyData
		/*DWORD style ,*/
		/*,HSCurDataInfoStatus* pCurDataStatus = NULL,*/
		/*CTValues<void>* pRefCalcValues = NULL*/) = 0;
	/******************************************************************
	*	函数功能：	获取指标线个数
	*	函数参数：	CString strname             : [in] 指标名称
	*				CArray<CLineProperty*,CLineProperty*>* pAyData  [out] 返回计算结果
	*				DWORD style					: [in] 指标类型
	*				
	*	返 回 值：	int : 线的个数
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/ 
	virtual int GetLineNum(CString strName,DWORD style	) = 0;
	/******************************************************************
	*	函数功能：	获取指标参数
	*	函数参数：	CString strname             : [in] 指标名称
	*			
	*				DWORD style					: [in] 指标类型
	*				
	*	返 回 值：	char*   [out] 返回计算结果
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/ 
	virtual CString GetExpressVar(CString strName,DWORD style	) = 0;
	/******************************************************************
	*	函数功能：	设置数据处理风格，如左边除权,向后除权等
	*	函数参数：	Dword style   : [in] 数据处理风格
	*			
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/ 
	virtual void	AddStyle(long dwStyle) =0;
	/******************************************************************
	*	函数功能：	根据名称的某个字查找匹配的所有指标,用于键盘精灵查找公式
	*	函数参数：	
	*				CArray<ExpPropery*,ExpPropery*> [out]存放查找结果
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/ 
	virtual void	GetExpProp(CArray<ExpPropery*,ExpPropery*>& ayNameProp) =0;
	/******************************************************************
	*	函数功能：	弹出公式窗口
	*	函数参数：	CString strname             : [in] 指标名称
	*			    DWORD style					: [in] 指标类型
	*				DWORD dwType 窗口类型
	*	
	*					 hxOpenExpressByConditionStockType	 0x0900 // 条件选股打开
	*					 hxOpenExpressByConditionDingZhi		 0x0910 // 定制选股打开
	*					 hxOpenExpressByConditionZhiBiao		 0x0920 // 指标选股打开
	*					 hxOpenExpressByConditionZhiNeng		 0x0930 // 智能选股打开
	*					 hxOpenExpressByConditionChaJian		 0x0940 // 插件选股打开
	*					 hxOpenExchangeSys            		 0x0950 // 交易系统
	*					 hxOpenExchangeYouSel				 0x0960	// 交易优选
	*					 hxOpenExpressBySortStockType		 0x0A00 // 指标排序打开
	*					 hxOpenBlockAnalysis					 0x0B00 // 板块分析
	*					 hxOpenUniteAnalysis					 0x0C00 // 联动分析
	*					 hxOpenFindDataDlg					 0x0D00 // 打开查找dlg
	*					 hxOpenAlarmManagerDlg				 0x0E00 // 打开预警管理对话框
	*					 hxOpenAddStkToAlarm					 0x1000 // 打开加入到预警对话框
	*				
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/ 
	virtual void  OpenExpWindow(CString strName, DWORD dwStyle, DWORD dwType) = 0;
	
};


#endif