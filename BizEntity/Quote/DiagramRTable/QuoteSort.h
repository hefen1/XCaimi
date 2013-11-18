#pragma once
#include "hsstructnew.h"
#pragma	pack(1)

#define	QS_INT			-1
#define	QS_UNINT		-2
#define	QS_LONG			-3
#define	QS_UNLONG		-4
#define	QS_SHORT		-5
#define	QS_WORD		    -6
#define	QS_UNSHORT		-6
#define	QS_FLOAT		-7
#define	QS_DOUBLE		-8

struct Qentry{
	int left;
	int right;
};


struct KQuickSort
{
	void*	m_pData;
	short	m_nDataLen;
	short	m_nKeyOffset;
	short	m_nKeyLen;
	long	m_nLow;
	long	m_nHigh;
};

struct HSSortData
{
	StockUserInfo*  m_pKey;
	double			m_dValue;
	HSSortData()
	{
		m_pKey = NULL;
		m_dValue = 0.0;
		//memset(this,0,sizeof(HSSortData));
	}

	HSSortData(StockUserInfo* pKey,double dValue)
	{
		m_pKey   = pKey;
		m_dValue = dValue;
	}
};

struct HSSortDataStr
{
	StockUserInfo*  m_pKey;
	char			m_szValue[64];
	HSSortDataStr()
	{
		m_pKey = NULL;
		memset(m_szValue,0,64);
	}

	HSSortDataStr(StockUserInfo* pKey,char* szValue)
	{
		m_pKey   = pKey;
		Copy(szValue);
	}
	void Copy(const char* szValue)
	{
		if( szValue == NULL )
			return;
		memset(m_szValue,0,sizeof(m_szValue));
		strncpy(m_szValue,szValue,64);
	}
};


//functions for quick sort and search
int smartcmp(const void* pBuff1,const void* pBuff2,short count);
int	BinarySearch(struct KQuickSort* pQSData,void* pKeySearchFor, BOOL bAscend = TRUE);
void QuickSortAscend(struct KQuickSort* pQSData);
void QuickSortDescend(struct KQuickSort* pQSData);
void QuickSortAscend2(struct KQuickSort* pQSData);
void QuickSortDescend2(struct KQuickSort* pQSData);

void FindStockPos(CArray<StockUserInfo*,StockUserInfo*>& payStock,CodeInfo& codeinfo,int& nPos);

#pragma	pack()