
#include "stdafx.h"
#include "KeyBoardStruct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HSCurKeyInfo::HSCurKeyInfo()
{
	m_nOpen = -1;

	Empty();
}

CodeInfo* HSCurKeyInfo::GetCode()
{
	return &m_cCode;
}

void HSCurKeyInfo::Empty()
{
	memset(m_szText, 0, sizeof(m_szText));
	//m_strText.Empty();
	memset(&m_cCode,0,sizeof(CodeInfo));
	memset(&m_pInfo,0,sizeof(m_pInfo));	
}

BOOL HSCurKeyInfo::IsEmpty()
{
	return (/*m_strText.IsEmpty() */m_szText[0] == 0 && 
		m_cCode.m_cCodeType == 0 && 
		(char)m_cCode.m_cCode[0] == 0);
}

void HSCurKeyInfo::Copy(const HSCurKeyInfo* pCode)
{
	if(pCode != NULL)
	{
		memcpy(m_szText, pCode->m_szText, sizeof(m_szText));
		//m_strText.Format("%s",pCode->m_strText);
		memcpy(&m_cCode,&pCode->m_cCode,sizeof(CodeInfo));
		memcpy(&m_pInfo,&pCode->m_pInfo,sizeof(m_pInfo));		
	}
	else
	{
		Empty();
	}
}

void HSCurKeyInfo::Copy(const char* strText)
{
	Empty();
	//m_strText.Format("%s",strText);
	strncpy(m_szText, strText, min(sizeof(m_szText) - 1, strlen(strText)));
}

void HSCurKeyInfo::Copy(const CodeInfo* pCode)
{
	Empty();
	if(pCode != NULL)
	{
		memcpy(&m_cCode,pCode,sizeof(CodeInfo));
	}
}

void HSCurKeyInfo::Copy(const PageViewInfo* pCode)
{
	Empty();
	if(pCode != NULL)
	{
		memcpy(&m_pInfo,pCode,sizeof(m_pInfo));
	}
}

CString HSCurKeyInfo::GetText()
{
	if( IsText() )
		return m_szText;
	return m_cCode.GetCode();
}

BOOL HSCurKeyInfo::IsText()
{
	//return m_strText.GetLength() != 0;
	return m_szText[0] != 0;
}

BOOL HSCurKeyInfo::IsBlock()
{
	if( IsText() )
	{
		return FALSE;
	}

	if( MakeMarket(m_cCode.m_cCodeType) == STOCK_MARKET 
		&& MakeMainMarket(m_cCode.m_cCodeType) > (STOCK_MARKET|SZ_BOURSE) )
	{
		return TRUE;
	}

	return FALSE;
}