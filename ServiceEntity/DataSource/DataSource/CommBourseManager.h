/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	CommBourseManager.h
*	文件标识：
*	摘	  要：	市场管理类
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
class CDataSourceImpl;
class CHsBouseManager 
{
protected:
	//本地缓存市场数据队列
	std::list<BourseInfo*>				m_listBourseInfo;
	//缓存市场数据互斥量
	CMutex								m_mutexBourse;	
	CDataSourceImpl*					m_pDataSource;
public:
	//	构造函数与析构函数
	CHsBouseManager();
	~CHsBouseManager();
protected:
	BOOL								LookupMainMarket(unsigned short* pMarket, int nMarketCount, unsigned short nMarketSource);
	BOOL								LookupSameMarket(unsigned short* pMarket, int nMarketCount, unsigned short nMarketSource);
	short								RetainGetPriceUnit(unsigned short nMarketCode);
	short								RetainGetPriceDecimal(unsigned short nMarketCode);
public:
	//
	void								SetDataSourceImpl(CDataSourceImpl* pImpl);
	//获取市场个数
	int									GetBouserSize();
	//市场信息加入到队列
	BOOL								PushBourse(BourseInfo* pBourse);
	//获取市场信息 
	BourseInfo*							GetBourse(unsigned short nMarket);
	//删除某个市场信息
	BOOL								RemoveBourse(BourseInfo* pBourse);
	//删除某个市场
	BOOL								RemoveBourse(HSMarketDataType nMarket );
	//清空本地市场数据
	BOOL								RemoveAllBourse();
	
	//获取二级市场信息
	int									GetSystemMarketInfo(unsigned short* pMarket, int nMarketCount, CArray<StockType>& blockArray);
	//获取二级市场
	StockType*							GetStockType(unsigned short nMarketCode);
	//获取市场（板块）价格单位	
	short								GetMarketBlockPriceUnit(unsigned short nMarketCode);
	//获取市场（板块）价格精度
	short								GetStockPriceDecimal(unsigned short nMarketCode);
	//同时获取价格单位及精度
	BOOL								GetStockPriceUnitDecimal(unsigned short nMarketCode, short& nPriceUnit, short& nDecimal);
	//写本地文件
	BOOL								WriteLocalFile();
};