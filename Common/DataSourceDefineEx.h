/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	DataSourceDefineEx.h
*	文件标识：
*	摘	  要：	数据引擎接口扩展定义头文件
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-9-7
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/

#pragma once

#include "DataSourceDefine.h"
#include "StockInfo.h"

class IDataSourceEx : public IDataSource
{
public:
	/********************************************************************************
	* 函数功能 :从数据引擎获取单个股票信息
	* 函数参数 :CodeInfo* pInfo - 含有股票代码、市场的信息
	*			 StockUserInfo& stockInfo; 股票信息的引用
	*  返回值  :TRUE 成功		
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual BOOL	HSDataSourceEx_GetStockUserInfo(IN CodeInfo* pInfo, IN OUT StockUserInfo& stockInfo) = 0;
	/********************************************************************************
	* 函数功能 :从数据引擎获取服务器时间
	* 函数参数 :
	*			 long &lDate, long &lTime; lData，服务器当前日期，格式为YYYYMMDD；lTime，服务器当前时间，格式为hhmmss
	*  返回值  :TRUE 成功		
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 : 20110308 由于数据引擎不做登录动作，所以该接口功能变更：lDate返回为服务器上一次初始化日期，如20110308。lTime为0
	*******************************************************************************/
	virtual BOOL	HSDataSourceEx_GetServerDataTime(IN OUT long &lDate, IN OUT long &lTime) = 0;

	/********************************************************************************
	* 函数功能 :从数据引擎获取某个市场股票信息表
	* 函数参数 :
	*			 unsigned short nMarket; 股票信息的引用
	*  返回值  : 列表大小	
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual int	HSDataSourceEx_GetMarketStocksInfo(IN unsigned short nMarket, IN OUT CArray<StockUserInfo>& stockArray) = 0;

	/********************************************************************************
	* 函数功能 :从数据引擎获取系统二级市场信息
	* 函数参数 : unsigned short* pMarket - 市场大类数组 如果为空，返回所有
	*			  int nMarketCount	市场类型数组大小
	*			CArray<StockType>& stockArray 存放系统板块信息的数组
	*  返回值  : 列表大小	
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual int	HSDataSourceEx_GetSystemMarketInfo(IN unsigned short* pMarket, IN int nMarketCount, IN OUT CArray<StockType>& blockArray) = 0;

	/********************************************************************************
	* 函数功能 :从数据引擎获取单个股票信息
	* 函数参数 :char* pszCode - 股票代码
	*			unsigned short nMarket 市场大类，如股票，期货等
	*			 StockUserInfo& stockInfo; 股票信息的引用
	*			unsigned short nMarketException 需要排除的小类,如排除指数等
	*  返回值  :TRUE 成功		
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual BOOL	HSDataSourceEx_GetStockUserInfo(IN char* pszCode, IN unsigned short nMarket, IN OUT StockUserInfo& stockInfo, IN unsigned short nMarketException = 0) = 0;

	/********************************************************************************
	* 函数功能 :从数据引擎获取多个股票信息-如板块股票信息
	* 函数参数 :CArray<CodeInfo> arInfo 股票代码信息的数组	; 空数组则返回全部股票	
	*			 CArray<StockUserInfo>& stockInfo; 股票信息数组的引用
	*			BOOL bCompareOnlyMainMarket; 只比较主市场标示
	*  返回值  :数组大小		
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual int	HSDataSourceEx_GetStocksUserInfo(IN CArray<CodeInfo>& arInfo, IN OUT CArray<StockUserInfo>& stockInfo, BOOL bCompareOnlyMainMarket = FALSE) = 0;

	/********************************************************************************
	* 函数功能 :从数据引擎获取多个股票财务-
	* 函数参数 :CArray<CodeInfo> arInfo 股票代码信息的数组	
	*			 CArray<CurrentFinanceData>& stockInfo; 股票财务数据数组的引用
	*			
	*  返回值  :数组大小		
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual int	HSDataSourceEx_GetStocksFinanceData(IN CArray<CodeInfo>& arInfo, IN OUT CArray<CurrentFinanceData>& ayFinanceData) = 0;

	/********************************************************************************
	* 函数功能 :从数据引擎获取多个股票除权数据
	* 函数参数 :CArray<CodeInfo> arInfo 股票代码信息的数组	
	*			 CArray<CurrentExRightData*>& ayExrightData; 股票除权数据数组指针的引用
	*			
	*  返回值  :数组大小		
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	* 备注*	   : CurrentExRightData 数组总的数据，需要调用Release() 去释放
	*******************************************************************************/
	virtual int	HSDataSourceEx_GetStocksExrightData(IN CArray<CodeInfo>& arInfo, IN OUT CArray<CurrentExRightData>& ayExrightData) = 0;
	/********************************************************************************
	* 函数功能:公式调用数据引擎，获取本地k线数据
	* 函数参数:
	*			:void* pAsk  - AskData结构地址
	*			int nMask - 未使用	
	*			int nExRight - 除权状态 0 不除权  HS_HxDataSource_Left_ChuQuan 前除权 HS_HxDataSource_Back_ChuQuan 后除权
	*			
	*			
	*			
	*  返回值 : void* 返回读取的文件内容
	* 调用依赖: 	
	* 作   者:
	* 完成日期:
	* 修改记录:
	*******************************************************************************/
	virtual void* HSDataSource_GetDayData(IN void* pAsk, IN int nMask = 0,IN int nExRight = 0) = 0;
	/********************************************************************************
	* 函数功能:写本地文件
	* 函数参数:	codeinfo  - 股票代码
	*			int period - 周期 类型
	*			StockCompDayDataEx* pNewData - 日线数据内容
	*			int nSize                -  StockCompDayDataEx的个数
	*			
	*			
	*  返回值 : 成功返回true 失败false
	* 调用依赖: 	
	* 作   者:
	* 完成日期:
	* 修改记录:
	*******************************************************************************/
	virtual BOOL HSDataSource_WriteDayData(CodeInfo* info,int period,StockCompDayDataEx* pNewData,int nSize ) = 0;
	/********************************************************************************
	* 函数功能:键盘精灵调用，用于字符匹配计算
	* 函数参数:
	*			
	*			
	*			
	*			
	*  返回值 : 当前key值的位置
	* 调用依赖: HSDataSource_FindStockFromKey
	* 作   者:
	* 完成日期:
	* 修改记录:
	*******************************************************************************/
	virtual int HSDataSource_FindStockFromKey(StockUserInfo* pStock, const CString& strKey,int& nUnique,
									int& nStart,int& nEnd, CString &szMarch,int& nBetter,const int nCurIndex) = 0;
	/********************************************************************************
	* 函数功能:键盘精灵调用，获取拼音简称
	* 函数参数:
	*			
	*			
	*			
	*			
	*  返回值 : 当前key值的位置
	* 调用依赖: HSDataSource_GetStockPyjc
	* 作   者:
	* 完成日期:
	* 修改记录:
	*******************************************************************************/
	virtual CString HSDataSource_GetStockPyjc(StockUserInfo* pStock) = 0;
};