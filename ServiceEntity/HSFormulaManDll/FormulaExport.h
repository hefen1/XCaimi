
/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	FormulaExport.h
*	文件标识：	
*	摘	  要：	公式管理模块的接口类定义
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/

#if !defined(AFX_COMPEXPORT_H__FC0269FD_A095_4EE3_823A_54DF4E9D6518__INCLUDED_)
#define AFX_COMPEXPORT_H__FC0269FD_A095_4EE3_823A_54DF4E9D6518__INCLUDED_

#include "FormulaManInterFace.h"
#include "CompileInterFace.h"
#include "FormulaStruct.h"
#include "ExpPublic.h"
#include "ConfigInterface.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*************************************************************
*	类 名 称：	CFormulaExport
*	类 描 述：	公式管理模块的接口类
***************************************************************/
class CFormulaExport: public IFormulaMan
{
public:
	/******************************************************************
	*	函数功能：	构造
	*	函数参数：	long lExe = 0 : 本模块是否以Exe的形式存在
	*	返 回 值：	无
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	CFormulaExport(long lExe = 0);
	virtual ~CFormulaExport();

public:
	/******************************************************************
	*	函数功能：	打开公式管理器(并同时返回公式管理器窗口句柄)
	*	函数参数：	HWND hwndNotify : [in] 公式管理器的父窗口
	*				long** pThis	: [in] 无意义
	*	返 回 值：	long : 公式管理器窗口句柄
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	long OpenCompile(HWND hwndNotify,long** pThis);

	/******************************************************************
	*	函数功能：	关闭(退出公式管理窗口的消息循环及销毁本模块占用的资源)
	*	函数参数：	HWND hwndNotify : [in] 无意义
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	void CloseCompile(HWND hwndNotify);

	/******************************************************************
	*	函数功能：	公式管理模块操作接口
	*	函数参数：	WPARAM wParam : [in] 与lParam相关,看lParam说明
	*				LPARAM lParam : [in] HSOpenExpressUseData*类型数据,操作方式根据HSOpenExpressUseData::m_wType&0xF00决定
										hxOpenExpress_Edit	  : 打开修改公式窗口, wp无意义, 返回0
										hxOpenExpress_Notes   : 打开注释窗口, wp无意义
										hxOpenExpress_Eidolon : 打开参数修改窗口, wp无意义
										hxShowAlignWindow	  : 得到指标树(传入的窗口(CWnd*)HSOpenExpressUseData::m_pData上创建)窗口, wp无意义, 返回指标树指针
										hxShowKeyboardWindow  : 显示键盘精灵窗口, wp 显示或隐藏窗口(0或非0), 返回0
										hxInitKeyboardWindow  : 初始化键盘精灵窗口, wp无意义, 返回0
										hxShowByExpressType   : 由公式类型来显示公式, wp无意义, 返回0
										hxShowReturnByExpressType : 由公式类型来显示公式,并返回选中公式, wp无意义
										hxOpenExpressByConditionStockType : 选股, wp无意义, 返回0
										hxOpenExpressBySortStockType : 指标排序, wp无意义, 返回0
										hxOpenBlockAnalysis   : 板块分析, wp无意义, 返回0
										hxOpenUniteAnalysis   : 联动分析, wp无意义, 返回0
										hxOpenAlarmManagerDlg : 打开预警窗口, wp无意义, 返回预警窗口指针
										hxOpenFindDataDlg     : 打开公式查找窗口, wp无意义, 返回0
										hxOpenAddStkToAlarm   : 打开加股票到预警窗口, wp无意义, 返回0
											
	*	返 回 值：	long : 与lParam相关,看lParam说明
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	long OpenExpress( WPARAM wParam,LPARAM lParam );

	/******************************************************************
	*	函数功能：	将消息发送到公式管理窗口进行处理
	*	函数参数：	UINT message : [in] 消息ID
	*				WPARAM wParam: [in] 消息分类
	*				LPARAM lParam: [in] 消息数据
	*	返 回 值：	long : 各种消息的返回值各不相同,具体在消息处理函数里说明
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	long OnMessage( UINT message ,WPARAM wParam,LPARAM lParam);

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
 	virtual BOOL CalcValue(  CString strName,DWORD style ,CArray<CLineProperty*,CLineProperty*>* pAyData
		
		/*,HSCurDataInfoStatus* pCurDataStatus = NULL,*/
		/*CTValues<void>* pRefCalcValues = NULL*/) ;
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
	virtual BOOL	SetDayData(StockDay* pDay,long lCurSize,
							   int nVolumeUnit,
							   int nMoneyUnit,short nMoneyDecimal,
							   int nPriceUnit,short nPriceDecimal,CodeInfo* pCodeinfo = NULL,
							   WORD nKey = 0);
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
	virtual int GetLineNum(CString strName,DWORD style) ;
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
	virtual CString GetExpressVar(CString strName,DWORD style	) ;
	/******************************************************************
	*	函数功能：	获取指标线相关属性
	*	函数参数：	CString strname             : [in] 指标名称
	*				CArray<CLineProperty*,CLineProperty*>* pAyData  [out] 返回计算结果
	*				DWORD style					: [in] 指标类型
	*				
	*	返 回 值：	bool : 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/ 
	 BOOL GetLineProperty(CString strName, DWORD style,CArray<CLineProperty*,CLineProperty*>*& pAyData);

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
   void	AddStyle(long dwStyle);
	 	/******************************************************************
	*	函数功能：	根据名称的某个字查找匹配的所有指标,用于键盘精灵查找公式
	*	函数参数：	CArray<ExpPropery*,ExpPropery*>& ayNameProp
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/ 
	void  GetExpProp(CArray<ExpPropery*,ExpPropery*>& ayNameProp);
	/******************************************************************
	*	函数功能：	获取实时数据
	*	函数参数：	CodeInfo* pCode [in] 股票代码
					CArray<int> ayID [in] 实时数据的id
	*				CArray<DOUBLE>& ayData [out] 实时数据值
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/ 
	void	GetNowData(CodeInfo* pCode,CArray<int> ayID, CArray<DOUBLE>& ayData);

	//初始化公式
	void initExpress(HWND hwndNotify,long** pThis);

	//打开公式窗口
	void  OpenExpWindow(CString strName, DWORD dwStyle, DWORD dwType);

	long ShowExpression( WPARAM wParam );
	long FindExpression( WPARAM wParam );
	long PreTranslateMsgDlg(MSG* pMsg); 

private:
	IHsColor*  m_pHSColor;     //颜色配置指针
	CArray<CLineProperty*,CLineProperty*> m_AyData;   //计算结果
	CMapStringToPtr m_mapDataSource;                  //数据源
};


#endif // !defined(AFX_COMPEXPORT_H__FC0269FD_A095_4EE3_823A_54DF4E9D6518__INCLUDED_)
