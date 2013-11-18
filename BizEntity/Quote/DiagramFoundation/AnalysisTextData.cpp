#include "StdAfx.h"
#include "AnalysisTextData.h"
#include <shlwapi.h>
#include "DiagramComm.h"
#include "..\..\..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CAnalysisTextData::GetZXFileReqName( const CodeInfo* pCode, const CString& strEntryItem, CString* strNetFileName, CString* strLocalFileName, CString strRefSection /*= ""*/, CString strRefEntry /*= ""*/, CString strRefFileName /*= INFO_PATH_INI_FILE*/, CString* strOpenType /*= NULL*/ )
{
	if( strRefSection.IsEmpty() )
		return 0;

	CString strEntry;
	if( strRefEntry.IsEmpty() && pCode != NULL )
		strEntry.Format(strEntryItem, GetNameByMarketMain(strRefSection, pCode->m_cCodeType));
	else
		strEntry = strRefEntry;
	if( strEntry.IsEmpty() )
		return 0;

	CString strRet;
	CStringArray ayRet;
	CString strScrollTextFileName = CGeneralHelper::GetAppPath() + _T("Setting\\") + strRefFileName;
	CGeneralHelper::ReadConfigString(strRefSection, strEntry, strRet, strScrollTextFileName);

	short nCount;
	nCount = CGeneralHelper::AnalyseStr(strRet,";",ayRet);
	if( nCount <= 0 )
		return 0;

	if(strNetFileName)
	{
		strRet = ayRet.GetAt(0); 
		CString strCfgString, strTmp;
		strCfgString = ReplaceCfgString(strRet, pCode);
		if(pCode && strCfgString.Find("%s") != -1)
		{
			strRet = CString(pCode->m_cCode,sizeof(pCode->m_cCode));
			strRet.Trim();

			strTmp.Format(strCfgString, strRet);	
			strCfgString = strTmp;
		}
		*strNetFileName = strCfgString;
	}

	if(strOpenType && nCount>= 3)
	{
		*strOpenType = ayRet.GetAt(2); // 
	}

	if( strLocalFileName ) // ����
	{
		*strLocalFileName = CGeneralHelper::GetAppPath() + _T("InfoData\\");		
		*strLocalFileName += strRefSection;
		*strLocalFileName += "\\";
		*strLocalFileName += strEntry;
		*strLocalFileName += "\\";
	}

	return 1;
}

int CAnalysisTextData::GetZXFileReqName( const CodeInfo* pCode, InfoIndex *pInfoIndex, const CString& strEntryItem, CString* strNetFileName, CString* strLocalFileName, CString strRefSection /*= ""*/,CString strRefEntry /*= ""*/, CString strRefFileName /*= INFO_PATH_INI_FILE*/, CString* strOpenType /*= NULL*/ )
{
	if(strRefSection.IsEmpty() )
		return 0;

	CString strEntry;  
	if( strRefEntry.IsEmpty() && pCode != NULL )
		strEntry.Format(strEntryItem,GetNameByMarketMain(strRefSection, pCode->m_cCodeType));
	else
		strEntry = strRefEntry;
	if( strEntry.IsEmpty() )
		return 0;

	CString strRet;
	CStringArray ayRet;
	short nCount;

	int bRet = 0;
	if( strNetFileName || strOpenType) // ����
	{
		CString strScrollTextFileName = CGeneralHelper::GetAppPath() + _T("Setting\\") + strRefFileName;
		CGeneralHelper::ReadConfigString(strRefSection, strEntry, strRet, strScrollTextFileName);

		nCount = CGeneralHelper::AnalyseStr(strRet,";",ayRet);
		if( nCount <= 1 )
			return 0;

		if(strNetFileName && pInfoIndex)
		{
			strRet = ayRet.GetAt(1); // 
			CString strCfgString = ReplaceCfgString(strRet, pCode);
			if(strCfgString.Find("%s") != -1)		
			{
				strRet.Format(strCfgString, strRefFileName.IsEmpty() ? pInfoIndex->m_cFilePath : ::PathFindFileName(pInfoIndex->m_cFilePath));
				strCfgString = strRet;			
			}
			*strNetFileName = strCfgString;
		}
		if(strOpenType && nCount >= 3)
		{
			*strOpenType = ayRet.GetAt(2); // 
		}
	}

	if( strLocalFileName ) // ����
	{
		*strLocalFileName = CGeneralHelper::GetAppPath() + _T("InfoData\\");		
		*strLocalFileName += strRefSection;
		*strLocalFileName += "\\";
		*strLocalFileName += strEntry;
		*strLocalFileName += "\\";
	}

	return 1;
}

/* �滻�ַ���
$jys -> shase,sznse,hk,exchange,future
$code����%s-> ��Ʊ����*/
CString CAnalysisTextData::ReplaceCfgString(CString strCfgString, const CodeInfo* pCode)
{
	if(pCode != NULL)
	{
		strCfgString.MakeLower();
		CString strJYSRep, strTmp;

		HSMarketDataType nSubMark= MakeMainMarket(pCode->m_cCodeType);
		HSMarketDataType  nMark = MakeMarket(pCode->m_cCodeType);
		if(nMark == STOCK_MARKET)
		{
			if(nSubMark == SH_Bourse)
			{
				strJYSRep = "shase";
			}
			else if(nSubMark == SZ_Bourse)
			{
				strJYSRep = "sznse";
			}
// 			else if(nSubMark == HK_Bourse)
// 			{
// 				strJYSRep = "hk";
// 			}
		}
		else if(nMark == FUTURES_MARKET)
		{
			strJYSRep = "future";
		}
		else if(nMark == FOREIGN_MARKET)
		{
			strJYSRep = "exchange";
		}

		strCfgString.Replace("$jys", strJYSRep);
		strJYSRep = CString(pCode->m_cCode,sizeof(pCode->m_cCode));
		strJYSRep.Trim();
		strCfgString.Replace("$code", strJYSRep);		
	}
	return strCfgString;
}

CString CAnalysisTextData::GetNameByMarketMain(CString strInfoProvider, int nMarket )
{
	CString strText;
	const short nMainMarket = MakeMarket(nMarket);
	if (strInfoProvider.Compare("�޳���Ѷ") == 0)
	{
		if(nMainMarket == HK_MARKET)
		{
			strText = "�۹�";
		}
		else if(nMainMarket == STOCK_MARKET)
		{
			if(MakeMainMarket(nMarket) == SH_Bourse)
			{
				if(MakeSubMarket(nMarket) == KIND_BOND)
					strText = "��֤ծȯ";
				else if(MakeSubMarket(nMarket) == KIND_QuanZhen)
					strText = "��֤Ȩ֤";
				else if(MakeSubMarket(nMarket) == KIND_FUND)
					strText = "��֤����";
				else
					strText = "��֤";
			}
			else if(MakeMainMarket(nMarket) == SZ_Bourse)
			{
				if(MakeSubMarket(nMarket) == KIND_BOND)
					strText = "��֤ծȯ";
				else if(MakeSubMarket(nMarket) == KIND_QuanZhen)
					strText = "��֤Ȩ֤";
				else if(MakeSubMarket(nMarket) == KIND_FUND)
					strText = "��֤����";
				else
					strText = "��֤";
			}
			else if (MakeMainMarket(nMarket) == (STOCK_MARKET|SH_BOURSE|SZ_BOURSE))		
			{
				strText = "���";
			}
			else
			{
				strText = "��Ʊ";
			}
		}
		else if(nMainMarket == FUTURES_MARKET)
		{
			strText =  "�ڻ�";
		}
		else if(nMainMarket == FOREIGN_MARKET)
		{
			strText = "���";
		}
		else if(nMainMarket == WP_MARKET)
		{
			strText = "����";
		}
		else
		{
			strText = "δ֪";
		}
	}
	else
	{
		if(nMainMarket == HK_MARKET)
		{
			strText = "�۹�";
		}
		else if(nMainMarket == STOCK_MARKET)
		{
			if(MakeMainMarket(nMarket) == SH_Bourse)
			{
				strText = "��֤";
			}
			else if(MakeMainMarket(nMarket) == SZ_Bourse)
			{
				strText = "��֤";
			}
			else if (MakeMainMarket(nMarket) == (STOCK_MARKET|SH_BOURSE|SZ_BOURSE))		
			{
				strText = "���";
			}
			else
			{
				strText = "��Ʊ";
			}
		}
		else if(nMainMarket == FUTURES_MARKET)
		{
			strText =  "�ڻ�";
		}
		else if(nMainMarket == FOREIGN_MARKET)
		{
			strText = "���";
		}
		else if(nMainMarket == WP_MARKET)
		{
			strText = "����";
		}
		else
		{
			strText = "δ֪";
		}
	}
	return strText;
}

CString CAnalysisTextData::GetUnionString( CString strInfoPath, CString strRefSection, CString strRefEntry, CString strRefFileName )
{
	CString strRet;
	if( strInfoPath.IsEmpty() )
		strInfoPath = " ";
	if( strRefSection.IsEmpty() )
		strRefSection = " ";
	if( strRefEntry.IsEmpty() )
		strRefEntry = " ";
	if( strRefFileName.IsEmpty() )
		strRefFileName = " ";

	strRet  = strInfoPath;
	strRet += ";";
	strRet += strRefSection;
	strRet += ";";
	strRet += strRefEntry;
	strRet += ";";
	strRet += strRefFileName;

	return strRet;
}


InfoIndex* CAnalysisTextData::GetInfoIndexData(AnsTextData* pData,int lLen,int& nCount)
{
	InfoIndex* pRefIndex = NULL;
	nCount = 0;

	char* pStr = pData->m_cData;
	if( !strncmp(pStr,"hsinfo",6) || 
		!strncmp(pStr,"QIANLONG",8) || 
		!strncmp(pStr,"NewsSend",8)) // sgzx ���
	{
		nCount    = (lLen - 16)/ sizeof(InfoIndex);
		if( ((lLen - 16) % sizeof(InfoIndex)) != 0 )
			return NULL;

		pRefIndex = (InfoIndex*)&pStr[16];
	}
	else // wst ά����
	{
		nCount = lLen / sizeof(InfoIndex);

		// Ϊ16ͷ
		if( (lLen % sizeof(InfoIndex)) == 16 )
		{
			nCount = (lLen - 16) / sizeof(InfoIndex);
			pRefIndex = (InfoIndex*)&pData[16];			
		}
		else if( (lLen % sizeof(InfoIndex)) != 0 )
			return NULL;
		else
			pRefIndex = (InfoIndex*)pData;
	}

	return pRefIndex;
}