#include "hsstructnew.h"
#pragma once

#define		BLOCK_NAME_LENGTH		32 
#define		MARKETTYPE_COUNT		12		// 类型组合个数
// 页面信息
struct  PageViewInfo 
{ 
	long  		m_lPageType;		// 页面类型
	long		m_nInPageTag;		// 页面标签（子类型）
	CodeInfo	m_ciCodeInfo;		// 股票代码信息

	char		m_szBlock[BLOCK_NAME_LENGTH]; // 板块名称

	HSMarketDataType m_sMarket[MARKETTYPE_COUNT];	// 类型组合

	PageViewInfo()
	{
		m_lPageType  = 0;
		m_nInPageTag = 0;

		memset(&m_szBlock,0,BLOCK_NAME_LENGTH);
		memset(m_sMarket,0,sizeof(m_sMarket));
	}

	CString GetCode()
	{
		return m_ciCodeInfo.GetCode();
	}

	void operator=(PageViewInfo* pInfo)
	{
		if(pInfo != NULL)
		{
			memcpy(this,pInfo,sizeof(PageViewInfo));
		}
	}

	void Serialize(CArchive& ar)
	{
		if( ar.IsStoring() )
		{
			ar << m_lPageType;
			ar << m_nInPageTag;
			ar << m_ciCodeInfo.m_cCodeType;			// 证券类型
			ar << m_ciCodeInfo.GetCode();

			int i;
			for( i = 0; i < _countof(m_szBlock); i++)
			{
				ar << m_szBlock[i];
			}

			for( i = 0; i < _countof(m_sMarket); i++)
			{
				ar << m_sMarket[i];
			}		
		}
		else
		{
			ar >> m_lPageType;
			ar >> m_nInPageTag;
			ar >> m_ciCodeInfo.m_cCodeType;			// 证券类型

			//
			CString str;
			ar >> str;
			strncpy(m_ciCodeInfo.m_cCode,str,sizeof(m_ciCodeInfo.m_cCode));

			// 板块名称
			for( int i = 0; i < _countof(m_szBlock); i++)
			{
				ar >> m_szBlock[i];
			}

			for( int i = 0; i < _countof(m_sMarket); i++)
			{
				ar >> m_sMarket[i];
			}
		}
	}

	// block
	void    SetBlockName(CString strBlock)
	{
		strncpy(m_szBlock,strBlock,sizeof(m_szBlock));
	}
	CString GetBlockName()
	{
		return CString(m_szBlock).Left(sizeof(m_szBlock));
	}

	// 添加一个包含的市场
	int	AddMarketType(HSMarketDataType sMarket)
	{
		if (sMarket <= 0)
		{
			return -1;
		}
		for (int i = 0; i < MARKETTYPE_COUNT; i++)
		{
			if (m_sMarket[i] == 0)
			{
				m_sMarket[i] = sMarket;
				return i;
			}
			else if (m_sMarket[i] == sMarket)
			{
				return -1;
			}
		}
		return -1;
	}

	// 获取第一个包含的有效市场
	HSMarketDataType GetFirstMarketType(int& nFirst) const
	{
		for (int i=nFirst; i<MARKETTYPE_COUNT; i++)
		{
			if (m_sMarket[i] != 0)
			{
				nFirst = i;
				return m_sMarket[i];
			}
		}
		return 0;
	}

	HSMarketDataType GetFirstMarketType() const
	{
		for (int i=0; i<MARKETTYPE_COUNT; i++)
		{
			if (m_sMarket[i] != 0)
			{
				return m_sMarket[i];
			}
		}
		return 0;
	}

	// 清空所有已包含的市场
	int EmptyMarketType()
	{
		int nCount = 0;
		for (int i = 0; i < MARKETTYPE_COUNT; i++)
		{
			if (m_sMarket[i] != 0)
			{
				m_sMarket[i] = 0;
				nCount++;
			}
		}
		return nCount;
	}

	// 删除指定市场
	BOOL DeleteMarketType(HSMarketDataType sMarket)
	{
		for (int i = 0; i < MARKETTYPE_COUNT; i++)
		{
			if (m_sMarket[i] == sMarket)
			{
				m_sMarket[i] = 0;
				return TRUE;
			}
		}
		return FALSE;
	}

	// 获取当前组合的市场个数
	int GetMarketTypeCount() const
	{
		int nCount = 0;
		for (int i = 0; i < MARKETTYPE_COUNT; i++)
		{
			if (m_sMarket[i] != 0)
			{
				nCount++;
			}
		}
		return nCount;
	}

	// 查找指定市场
	int	FindMarketType(HSMarketDataType sMarket) const
	{
		for (int i = 0; i < MARKETTYPE_COUNT; i++)
		{
			if (m_sMarket[i] == sMarket)
			{
				return i;
			}
		}
		return -1;
	}
};  