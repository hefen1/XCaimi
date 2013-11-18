/*******************************************************************************
 * Copyright(c) 2010, 杭州恒生股份有限公司
 * All rights reserved.
 *
 * 文件名称：AnalysisTextData.h
 * 文件标识：咨询工具类
 * 摘    要：
 *
 * 当前版本：1.0
 * 作    者：王超
 * 完成日期：2011年3月17日
 * 
 *******************************************************************************/
#pragma once
#include "..\..\..\Common\hsstructnew.h"
#include "..\..\..\Common\QuoteComm\InfoStruct.h"
#include "..\..\..\Common\QuoteComm\QuoteDefine.h"
#include "..\..\..\Common\hsds_comudata.h"
#include "DiagramFoundationDef.h"

class HS_EXT_CLASS CAnalysisTextData
{
public:
	
	// 获取资料请求文件路径名
	static int GetZXFileReqName(const CodeInfo* pCode, const CString& strEntryItem, CString* strNetFileName, CString* strLocalFileName, CString strRefSection, CString strRefEntry = "", CString strRefFileName = INFO_PATH_INI_FILE, CString* strOpenType = NULL);
	static int GetZXFileReqName(const CodeInfo* pCode, InfoIndex *pInfoIndex, const CString& strEntryItem, 	CString* strNetFileName, CString* strLocalFileName,	CString strRefSection,CString strRefEntry = "", CString strRefFileName = INFO_PATH_INI_FILE, CString* strOpenType = NULL);
	// 替换字符串
	static CString ReplaceCfgString(CString strCfgString, const CodeInfo* pCode);
	// 通过股票代码获得主要市场的名称
	static CString GetNameByMarketMain(CString strInfoProvider, int nMarket);
	// 得到合并字串,和函数AnalyzeUnionString对应使用
	// 配置路径，配置段名，配置值名，配置文件名
	static CString GetUnionString(CString strInfoPath, CString strRefSection, CString strRefEntry, CString strRefFileName);
	// 转换当前索引
	static InfoIndex* GetInfoIndexData(AnsTextData* pData,int lLen,int& nCount);
};
