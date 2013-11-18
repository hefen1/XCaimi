/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDrawQuoteReportStruct.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��ѯͼԪ
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-3-17
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "hsstructnew.h"

#define		QUOTEREPORT_BLOCK_NAME_LENGTH		32 
#define		QUOTEREPORT_MARKETTYPE_COUNT		12		// ������ϸ���
//ҳ����Ϣ
struct PageInfo
{
	long  		m_lPageType;		// ҳ������
	long		m_lInPageTag;		// ҳ���ǩ�������ͣ�
	CodeInfo	m_ciCodeInfo;		// ��Ʊ������Ϣ

	char		m_szBlock[QUOTEREPORT_BLOCK_NAME_LENGTH];        // �������
	HSMarketDataType m_sMarket[QUOTEREPORT_MARKETTYPE_COUNT];	 // �������

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

	// ���һ���������г�
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

	// ��ȡ��һ����������Ч�г�
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

	// ��������Ѱ������г�
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

	// ɾ��ָ���г�
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

	// ��ȡ��ǰ��ϵ��г�����
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

	// ����ָ���г�
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