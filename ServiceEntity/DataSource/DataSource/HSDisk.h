/*******************************************************************************
* Copyright (c)2003, 杭州恒生信息技术有限公司
* All rights reserved.
*
* 文件名称：HSDisk.h
* 文件标识：DISK文件操作主类
* 摘    要：DISK文件操作主类
*
* 当前版本：
* 作    者：
* 完成日期：
*
* 取代版本：
* 原 作 者：
* 完成日期：
* 参考资料: DISK数据请求包定义.DOC
*******************************************************************************/
#if !defined(AFX_HSDISK_H__27C28C1C_61E2_4180_BC44_16F55261C145__INCLUDED_)
#define AFX_HSDISK_H__27C28C1C_61E2_4180_BC44_16F55261C145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrendFile.h"
#include "DataSourceDefineEx.h"
#include "FormulaManPublic.h"
#include "KLineDataFile.h"

#define  AnalisysFor5             0x0002  //五分钟
#define  AnalisysFor15            0x0003  //十五分钟
#define  AnalisysFor30            0x0004  //三十分钟
#define  AnalisysFor60            0x0005  //六十分钟
#define  AnalisysForDay           0x0006  //日线
#define  AnalisysForWeek          0x0007  //周线
#define  AnalisysForMonth         0x0008  //月线
#define  AnalisysForMoreDay       0x0009  //多日线

class  CHSDisk 
{
public:
	

public:
	CHSDisk();
	CHSDisk(HWND hWnd,UINT	nNotifyMessage);
	virtual ~CHSDisk();
protected:



public:
	static int CalcChuQuan(unsigned long lPreDate,int nDayType,StockDay& pDay,
		CArray<ChuQuanData*,ChuQuanData*>& ayChuQuanData,
		long lMask,long nPriceUnit);

	static int  CaclExRight(StockDay* pDay, int nLen, CodeInfo* pCodeInfo, int nExRight/* = 0*/);




public:
	
	/********************************************************************************
	* 函数功能 :根据请求包信息,返回经过周期抽取的数据(例如合成周线等)	
	* 函数参数 :void* pAsk  - AskData结构地址
				long lRetSize - 请求的StockCompDayDataEx数据个数 
								等于0或者绝对值大于文件数据个数,被置文件数据个数
								小于0且绝对值小于文件数据个数, 从文件末尾向前偏移|lRetSize | 个数据
								大于0且绝对值大于文件数据个数 
				int nMask - 1 : 表示读取指定文件, 即打开即关闭, 文件指针在文件头
							0 : 表示读取指定文件, 文件指针被保存, 不关闭文件
				int nExRight  : 除权状态 0 不除权  HS_HxDataSource_Left_ChuQuan 前除权 HS_HxDataSource_Back_ChuQuan 后除权
	*  返回值  :RetData * 指向数据的地址(数据被StockCompDayDataEx填充), 失败返回NULL	
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/ 
	void*   GetDayData(void* pAsk,long lRetSize,int nMask = 0, int nExRight = 0);

	/********************************************************************************
	* 函数功能 :根据请求包信息,返回经过周期抽取的数据(例如合成周线等)	
	* 函数参数 :void* pAsk  - AskData结构地址
				int nMask - 未使用	
				int nExRight - 除权状态 0 不除权  HS_HxDataSource_Left_ChuQuan 前除权 HS_HxDataSource_Back_ChuQuan 后除权
	*  返回值  :合成后数据指针RetData *,失败返回NULL	
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/ 
	void*   GetDayData(void* pAsk,int nMask = 0, int nExRight = 0);

	//获取分时数据	
	void*   GetTraceData(void* pAsk,long& nSize,long lDate);  
	//获取历史分时数据
	void*   GetHisTraceData(void* pAsk,long& nSize);  
	//分笔
	void*   GetTickData(void* pAsk,long& nSize,long lDate);
	/********************************************************************************
	* 函数功能 :调用InitPath();InitData();	
	* 函数参数 :	
	*  返回值  :	
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	* 不足之处 : 考虑去掉
	*******************************************************************************/ 
	static void			Init();
	/********************************************************************************
	* 函数功能 :初始化所有程序需要使用到的数据路径 包括行情,期货,资讯数据路径
				根路径将从配置文件读取用户设置行情,资讯主路径,否则默认路径
	* 函数参数 :void	
	*  返回值  :void	
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/ 
	static void			InitPath();

	/********************************************************************************
	* 函数功能 :取得指向指定文件的文件指针	
	* 函数参数 :CFile*& pFile - 指针,新分配指针将保存到此
				CString& strFileName- 文件名 不需要引用
	*  返回值  :成功TRUE 失败FALSE	
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	* 不足之处 :此函数未使用   考虑去掉
	*******************************************************************************/ 
	static BOOL			InitFile(CFile*& pFile, CString& strFileName);


public:
	/********************************************************************************
	* 函数功能 :写历史数据			
	* 函数参数 :DataHead* pHead - 数据地址	
	*			类型:
	*			RT_STOCKTICK - 无处理
	*			RT_MAJORINDEXTICK - BigTraceData*  写StockHistoryData数据
	*			RT_BUYSELLORDER   - 无处理
	*			RT_TREND		 - 数据头要求AnsTrendData, 写StockHistoryData
	*			RT_BUYSELLPOWER  - 买卖力道数据  数据头地址DOWNCAST(BuySellData)	写StockHistoryData
	*			HISBUYSELLDATA - DOWNCAST(NewBuySellDataOld),写StockHistoryData
	*			RT_MAJORINDEXTREND	- DOWNCAST(AnsMajorIndexTrend), 写IndexRealTime
	*			RT_MAJORINDEXDBBI - 多空指标 DOWNCAST(BigDataDuoKongOld) 写StockHistoryData
	*			RT_MAJORINDEXADL - 大盘ADL DOWNCAST(BigDataADLOld) 写StockHistoryData
	*			RT_MAJORINDEXBUYSELL - 大盘买卖力道 DOWNCAST(BigDataBuySellOld) 写StockHistoryData
	*			RT_LEAD   - 领先指标DOWNCAST(AnsLeadData) 写HSIndexRealTime
	*			RT_ADDTREND - 多股同列 DOWNCAST(MultiStockHisDataOld) NowData & OtherData
	*			HISTEND - 未处理			
	*  返回值  :void	
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/ 
	void WriteHistoryData(DataHead* pHead);
	/********************************************************************************
	* 函数功能 :写日线数据	
	* 函数参数 :AskData* pCurAsk - 如果为NULL,将指向当前请求,
	*			StockCompDayDataEx* pNewData - 数据指针
	*			int nSize	- 数据个数
	*  返回值  :BOOL 成功返回TRUE 失败返回FALSE		
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/ 
	BOOL WriteDayDataNew(CodeInfo* info,int period,StockCompDayDataEx* pNewData,long nSize);

	//写历史分时 
	BOOL WriteHisTraceData(CodeInfo* pCode,long lDate, void* pData, long lSize);
	//写分时数据
	BOOL WriteNewTraceData(CodeInfo* pCode, void* pData, long lSize,long lDate);
	//写分笔
	BOOL WriteTickData(CodeInfo* pCode, void* pData, long lSize,long lDate);
	//
protected:
	//安装数据用,用来指示当前的数据路径
	static CString	m_strCurPath;

public:
	static short g_nCurServerID;
	
private:
	CMap<CString,LPCTSTR,SeverCalculateData,SeverCalculateData>  m_calculateMap;
	CMapWordToPtr  m_mapTrendfile;
	CMapStringToPtr  m_mapHisTrendfile;
	CMapStringToPtr  m_mapTickFile;
public:
	static void InitRiseFallData(char* pData);
	void MakeServerCalculate(AskData* pAsk);
	RetDiskData* MakePeriodData(StockCompDayDataEx* pDataStartPos, UINT nCount, 
		ReqDayData* pAskData,
		int nMask /*= 0*/, int nExRight/* = 0*/);
};

#endif // !defined(AFX_HSDISK_H__27C28C1C_61E2_4180_BC44_16F55261C145__INCLUDED_)



