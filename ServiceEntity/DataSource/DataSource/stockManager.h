/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	stockManager.h
*	文件标识：
*	摘	  要：	股票管理类
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-9-1
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/

#pragma once
#define _delArrayObj(p)  { for(int i = p.GetSize() - 1; i >= 0; i--) { delete p.GetAt(i); } p.RemoveAll(); }

#include "..\..\..\Common\StockInfo.h"
#include "PyjcManager.h"
class CDataSourceImpl;
class CStockManager
{
protected:
	//本地缓存股票数据队列
	CArray<CHSDSStockInfoIntact*,CHSDSStockInfoIntact*> m_ayStocks;
	CMapStringToOb                      m_mapIndex;
	//缓存股票数据互斥量
	CMutex								m_mutexStock;	
	CMapStringToPtr                     m_mapFinanceData;
	CDataSourceImpl*					m_pDataSource;
	CPyjcManager						m_pyjcManager;
	CArray<StockInfoEx *, StockInfoEx *> *  m_psiExtend; 
public:
	//	构造函数与析构函数
	CStockManager();
	virtual ~CStockManager();
	//
	void								SetDataSourceImpl(CDataSourceImpl* pImpl);
	CStringArray*						GetPyjcByHZ(char*& pHZ,int& nNext);
public:
	/**********************smal********************************************
	*	函数功能：	根据指定的单词返回相应的拼音简称
	*	函数参数：	const char* pcWord	：单词
	*				char* pcVoice		：保存相应拼音简称的缓冲
	*				int nSize			：拼音代码的字节数目
	*	返 回 值：	无 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003-4-29
	*	修改说明：	
	*********************************************************************/	
	void WordToPYJC(const char* pcWord,char* pcVoice, int nSize);
	void								MakeShortName(StockUserInfo *pStock);
	void								MakeAllShortName();
	StockInfoEx*						GetStockInfoEx(const CodeInfo *pCode);

	BOOL								CreateIndex();
	//股票信息加入到队列
	BOOL								PushStock(CHSDSStockInfoIntact* pStock);
	//获取股票信息 
	CHSDSStockInfoIntact*				GetStock(CodeInfo* pInfo);
	CHSDSStockInfoIntact*				GetStock(char* pszCode, unsigned short nMarket, unsigned short nMarketException);

	//删除某个股票信息
	BOOL								RemoveStock(CHSDSStockInfoIntact* pStock);
	BOOL								RemoveStockInfoEx();
	//清空本地股票数据
	BOOL								RemoveAllStocks();
	//清空本地某市场股票数据
	BOOL								RemoveStocksByMarket(HSMarketDataType nMarket);
	//获取指定市场的股票列表
	int									GetMarketStockList(HSMarketDataType* pMarket, int nMarketCount,CArray<StockUserInfo>& ayRef);
	//获取指定代码信息的股票列表
	int									GetStocksList(CArray<CodeInfo>* pArInfo, CArray<StockUserInfo>& stockInfo, BOOL bCompareOnlyMainMarket);
	//获取所有股票的代码及市场信息
	int									GetStockCodeInfoList(CArray<CodeInfo*>& arCodeInfo);
	//收到服务器计算涨跌停数据
	int									RecvServerCalcData(AnsSeverCalculate* pData);
	//写本地文件
	BOOL								WriteLocalFile();
	BOOL								SetFinanceDataToMap();
	//读财务数据
	int									GetStocksFinanceData(CArray<CodeInfo>* pArInfo, CArray<CurrentFinanceData>& ayFinanceData);
	//读除权数据
	int									GetStocksExrighData(CArray<CodeInfo>* pArInfo, CArray<CurrentExRightData>& ayExrightData);
	short								GetStockHand(HSMarketDataType pMarket);
	int									FindStockFromKey(StockUserInfo* pStock, const CString& strKey,int& nUnique, int& nStart,int& nEnd, CString &szMarch,int& nBetter,const int nCurIndex );
	CString								GetStockPyjc( StockUserInfo* pStock );
};