/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDrawQuoteReportStruct.h
*	文件标识：
*	摘	  要：  咨询图元
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-3-17
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "hsstructnew.h"

#define		QUOTEREPORT_BLOCK_NAME_LENGTH		32 
#define		QUOTEREPORT_MARKETTYPE_COUNT		12		// 类型组合个数
//页面信息
struct PageInfo
{
	long  		m_lPageType;		// 页面类型
	long		m_lInPageTag;		// 页面标签（子类型）
	CodeInfo	m_ciCodeInfo;		// 股票代码信息

	char		m_szBlock[QUOTEREPORT_BLOCK_NAME_LENGTH];        // 板块名称
	HSMarketDataType m_sMarket[QUOTEREPORT_MARKETTYPE_COUNT];	 // 类型组合

	PageInfo()
	{
		m_lPageType = 0;
		m_lInPageTag = 0;
		memset(&m_szBlock, 0, QUOTEREPORT_BLOCK_NAME_LENGTH);
		memset(m_sMarket, 0, sizeof(m_sMarket));
	}

	CString GetCode()
	{
		return m_ciCodeInfo.GetCode();
	}

	void operator=(PageInfo* pInfo)
	{
		if(pInfo != NULL)
		{
			memcpy(this,pInfo,sizeof(PageInfo));
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
		for (int i = 0; i < QUOTEREPORT_MARKETTYPE_COUNT; i++)
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
		for (int i=nFirst; i<QUOTEREPORT_MARKETTYPE_COUNT; i++)
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
		for (int i=0; i<QUOTEREPORT_MARKETTYPE_COUNT; i++)
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
		for (int i = 0; i < QUOTEREPORT_MARKETTYPE_COUNT; i++)
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
		for (int i = 0; i < QUOTEREPORT_MARKETTYPE_COUNT; i++)
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
		for (int i = 0; i < QUOTEREPORT_MARKETTYPE_COUNT; i++)
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
		for (int i = 0; i < QUOTEREPORT_MARKETTYPE_COUNT; i++)
		{
			if (m_sMarket[i] == sMarket)
			{
				return i;
			}
		}
		return -1;
	}
};