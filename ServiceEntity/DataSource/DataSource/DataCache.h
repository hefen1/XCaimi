#pragma once
/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	datacache.h
*	文件标识：
*	摘	  要：	数据缓存头文件
*	
*	当前版本：
*	作	  者：	xudan
*	完成日期：  2010-4-6
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/

#include "HSDisk.h"

class CDataCache
{
public:
	CDataCache(void);
	virtual ~CDataCache(void);

	//读本地缓存文件
	void* ReadFile( AskData* pCurAsk ,long& lRetSize);
	//写本地缓存文件
	void WriteFile(void* pData, CodeInfo* pCode);

	//生成包头
	void MakeHeadData(DataHead*, AskData* );

	//盘后分析读缓存数据
	void* MakeDayData(AskData*& pCurAsk, long& nSize,BOOL& bSend);
	//读历史分时,
	void* MakeHisTraceData(AskData* pCurAsk,long& lSize);
	//读分时
	void* MakeTraceNewData(AskData* pCurAsk,long& lSize);
	//读分笔
	void* MakeTickData(AskData* pCurAsk,long& lSize);

	//盘后分析写缓存数据
	void WriteDayData(CodeInfo* info, int period,StockCompDayDataEx* pNewData,long nSize );
	//写历史分时，
	void WriteHisTraceData(void* pData, CodeInfo* pCode);
	//写分时
	void WriteTraceNewData(void* pData, CodeInfo* pCode);
	void WriteTickData(void* pData,CodeInfo* pCode);
	//读资讯文件
	void* MakeInfoData(AskData* pAsk, long& lRet);
	//写资讯文件
	void WriteInfoData(void* pData,CodeInfo* pCode);
	
	void SetDate(long lCurDate, long lSvrTime);
	//初始化时删除分时tick文件
	BOOL DelTraceFile();
protected:
	CHSDisk m_disk;   //读写文件
	long    m_nCurlate;  //当天时间
	long    m_nSvrTine;   //服务器时间
};
