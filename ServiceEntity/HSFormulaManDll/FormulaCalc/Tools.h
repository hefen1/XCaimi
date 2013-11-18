/*******************************************************************************
 * Copyright (c)2003, 杭州恒生信息技术有限公司
 * All rights reserved.
 *
 * 文件名称：tools.h
 * 文件标识：
 * 摘    要：
 *
 * 当前版本：2.0
 * 作    者：
 * 完成日期：
 *
 * 取代版本：
 * 原 作 者：
 * 完成日期：
 *******************************************************************************/
//tools.h
#ifndef INC_TOOLS_H
#define INC_TOOLS_H

#pragma	pack(1)
#if !defined(_WIN32_WCE)
	#include <time.h>
#endif

#define	QS_INT			-1
#define	QS_UNINT		-2
#define	QS_LONG			-3
#define	QS_UNLONG		-4
#define	QS_SHORT		-5
#define	QS_WORD		    -6
#define	QS_UNSHORT		-6
#define	QS_FLOAT		-7
#define	QS_DOUBLE		-8

struct KQuickSort
{
	void*	m_pData;
	short	m_nDataLen;
	short	m_nKeyOffset;
	short	m_nKeyLen;
	long	m_nLow;
	long	m_nHigh;
};

//functions for quick sort and search
int smartcmp(const void* pBuff1,const void* pBuff2,short count);
int	BinarySearch(struct KQuickSort* pQSData,void* pKeySearchFor, BOOL bAscend = TRUE);
void QuickSortAscend(struct KQuickSort* pQSData);
void QuickSortDescend(struct KQuickSort* pQSData);
void QuickSortAscend2(struct KQuickSort* pQSData);
void QuickSortDescend2(struct KQuickSort* pQSData);
//
double _atof(const TCHAR* str,int nLen = -1);
long _atol(const TCHAR* str,int nLen = -1);
int  _strlen(const TCHAR* str, int nLen = -1);
TCHAR* _strncpy(TCHAR* string1, const TCHAR* string2, int nCount);
TCHAR* strcpyn(TCHAR* strDest, int nDest, const TCHAR* strSource, int nSource = -1, BOOL bTrim = FALSE);
int wstrcpyn(char* strDest, int nDest, const TCHAR* strSource, int nSource = -1);
int _wcmemcmp(const void* buf1, const void* buf2, size_t count1 = -1, size_t count2 = -1);
int _mwcstrcmp(const TCHAR* psz1, const TCHAR* psz2, TCHAR cToken = ';');

void* smemcpy(void* dest, int nDest, const void* src, int count);

#if !defined(_WIN32_WCE)
int _smkdir(const TCHAR* szPath);
time_t LongToTime(long lDate);
time_t StringToTime(const TCHAR* pszTime);
TCHAR*  TimeToString(time_t tm);
#endif

const TCHAR* RTrim(const TCHAR* strSrc);
const TCHAR* LTrim(const TCHAR* strSrc);
const TCHAR* AllTrim(const TCHAR* strSrc);

//others functions
TCHAR* _tempfname(const TCHAR* dir, const TCHAR* fhead, const TCHAR* ext);
TCHAR* _strtok(TCHAR* s1,TCHAR* s2);
void strins(TCHAR* strPos, int c);
void InsertNumberSpace(TCHAR* strNumber,int cSpace = ',');
void regulatePathString(TCHAR* pszPath);

void* DebugGlobalAllocPtr(UINT fuAlloc, DWORD cbAlloc);
BOOL  DebugGlobalFreePtr(void *p);

TCHAR* EncodeString(const TCHAR *pszSrc);
TCHAR* DecodeString(const TCHAR *pszSrc);

#if defined(_WIN32_WCE)
unsigned long time(unsigned long* ltime);
#endif

#pragma pack()
#endif
