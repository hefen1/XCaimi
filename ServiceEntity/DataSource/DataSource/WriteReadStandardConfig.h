/*************************************************************
*	Copyright (c)2003, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	WriteReadStandardConfig.h
*	文件标识：	
*	摘	  要：	读写标准私有配置文件的类描述
*
*	当前版本：	2.0
*	作	  者：	
*	更新日期：	2003-4-27
*
***************************************************************/
#ifndef	__WRITEREADSTANDARDCONFIG_H__
#define	__WRITEREADSTANDARDCONFIG_H__

#pragma once

//#ifndef Have_defineMapOb

//#define  Have_defineMapOb   0x01
//#endif


//	added by hlw 2002.0815
//	读写私有配置文件类
class CWriteReadStandardConfig
{
public:
	//	构造函数与析构函数
	CWriteReadStandardConfig();
	~CWriteReadStandardConfig(void);
	
	//	读写私有配置文件
	
	/******************************************************************
	*	函数功能：	读取指定名称的配置文件中指定段的所有字符串键，
	*				并把各字符串键值存放在指定的字符串数组中
	*	函数参数：	const char* strSection	：指定的段名称
	*				CStringArray& strArray	：保存结构的字符串数组
	*				const char* strFileName	：待读写的文件名称
	*	返 回 值：	BOOL					：操作成功返回TRUE，失败时返回FALSE
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003-4-27
	*	修改说明：	
	*****************************************************************/
	static BOOL ReadConfigSectionEntry(const char* strSection, CStringArray& strArray,const char* strFileName = NULL);
	static BOOL ReadConfigSectionString(const char* strSection, CStringArray& strArray,const char* strFileName  = NULL);
	
	static BOOL ReadConfigSectionEntry(const char* pstrSection, 
		const char* pstrEntry, 
		CString* pstrItem, 
		CStringArray* pstrArray, 
		const char* pData, 
		const int nLength);
	/******************************************************************
	*	函数功能：	读取指定段内指定名称的整型键值
	*	函数参数：	const char *strSection	：指定的段名
	*				const char* strEntry	：待读取键的名称
	*				CUIntArray &UIntArray	：存放各整型键值的数组
	*				const char* strFileName ：待读写的文件名称
	*	返 回 值：	BOOL					：操作成功返回TRUE，失败时返回FALSE				
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003-4-27
	*	修改说明：	
	*****************************************************************/
	
	static BOOL ReadSectionInt(const char *strSection,const char* strEntry,CUIntArray &UIntArray,const char* strFileName = NULL);
	
	/******************************************************************
	*	函数功能：	读取指定段内指定名称的字符串键值
	*	函数参数：	const char* strSection	：指定的段名
	*				const char* strEntry	：待读取的键的名称
	*				CString& strItem		：保存所读取键的值的缓冲
	*				const char* strFileName ：待读写的文件名称
	*	返 回 值：	BOOL 					：操作成功返回TRUE，失败时返回FALSE。
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003-4-27
	*	修改说明：	
	*****************************************************************/
	
	static BOOL ReadConfigString(const char* strSection,const char* strEntry,CString& strItem,const char* strFileName = NULL);
	
	/******************************************************************
	*	函数功能：	读取指定段内指定名称的整型键值
	*	函数参数：	const char* strSection	：指定的段名
	*				const char* strEntry	：待读取的键的名称
	*				int& nValue				：保存所读取键的值
	*				const char* strFileName ：待读写的文件名称
	*	返 回 值：	BOOL 					：操作成功返回TRUE，失败时返回FALSE。
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003-4-27
	*	修改说明：	
	*****************************************************************/
	
	static BOOL ReadConfigInt(const char* strSection,const char* strEntry,int& nValue,const char* strFileName = NULL);
	
	/******************************************************************
	*	函数功能：	更新指定段内指定名称的字符串键的值。
	*	函数参数：	const char* strSection	：指定的段名
	*				const char* strEntry	：待更新的键的名称
	*				const char* strItem		：欲更新键的新值
	*				const char* strFileName ：待读写的文件名称
	*	返 回 值：	BOOL 					：操作成功返回TRUE，失败时返回FALSE。
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003-4-27
	*	修改说明：	
	*****************************************************************/
	
	static BOOL WriteConfigString(const char* strSection,const char* strEntry, const char* strItem,const char* strFileName = NULL);
	
	/******************************************************************
	*	函数功能：	更新指定段内指定名称的整型键的值。
	*	函数参数：	const char* strSection	：指定的段名
	*				const char* strEntry	：待更新的键的名称
	*				CUIntArray &UIntArray	：欲更新键的新值。
	*				const char* strFileName ：待读写的文件名称
	*	返 回 值：	BOOL 					：操作成功返回TRUE，失败时返回FALSE。
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003-4-27
	*	修改说明：	
	*****************************************************************/

	
	static BOOL WriteSectionInt(const char *strSection,const char* strEntry,CUIntArray &UIntArray,const char* strFileName = NULL);
	static BOOL WriteSectionInt(const char *strSection,const char* strEntry,long lValue,const char* strFileName = NULL);
	
	
	/******************************************************************
	*	函数功能：	用新的段名来更新指定的段名
	*	函数参数：	const char *strSectionNew	：指定的新段名
	*				const char* strSectionOld	：待更新段的段名
	*				const char* strFileName		：待更新的文件名称
	*	返 回 值：	BOOL						：操作成功返回TRUE；失败时返回FALSE。
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003-4-27
	*	修改说明：	
	*****************************************************************/
	
	static BOOL ChangeSection(const char *strSectionNew,const char* strSectionOld,const char* strFileName = NULL);
	
	static CString GetContentsByHelpID(CString strFile,CString strHelpID);

};


#endif	//__WRITEREADSTANDARDCONFIG_H__