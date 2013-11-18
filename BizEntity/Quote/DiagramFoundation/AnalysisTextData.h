/*******************************************************************************
 * Copyright(c) 2010, ���ݺ����ɷ����޹�˾
 * All rights reserved.
 *
 * �ļ����ƣ�AnalysisTextData.h
 * �ļ���ʶ����ѯ������
 * ժ    Ҫ��
 *
 * ��ǰ�汾��1.0
 * ��    �ߣ�����
 * ������ڣ�2011��3��17��
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
	
	// ��ȡ���������ļ�·����
	static int GetZXFileReqName(const CodeInfo* pCode, const CString& strEntryItem, CString* strNetFileName, CString* strLocalFileName, CString strRefSection, CString strRefEntry = "", CString strRefFileName = INFO_PATH_INI_FILE, CString* strOpenType = NULL);
	static int GetZXFileReqName(const CodeInfo* pCode, InfoIndex *pInfoIndex, const CString& strEntryItem, 	CString* strNetFileName, CString* strLocalFileName,	CString strRefSection,CString strRefEntry = "", CString strRefFileName = INFO_PATH_INI_FILE, CString* strOpenType = NULL);
	// �滻�ַ���
	static CString ReplaceCfgString(CString strCfgString, const CodeInfo* pCode);
	// ͨ����Ʊ��������Ҫ�г�������
	static CString GetNameByMarketMain(CString strInfoProvider, int nMarket);
	// �õ��ϲ��ִ�,�ͺ���AnalyzeUnionString��Ӧʹ��
	// ����·�������ö���������ֵ���������ļ���
	static CString GetUnionString(CString strInfoPath, CString strRefSection, CString strRefEntry, CString strRefFileName);
	// ת����ǰ����
	static InfoIndex* GetInfoIndexData(AnsTextData* pData,int lLen,int& nCount);
};
