#include "StdAfx.h"
#include "InfoData.h"
#include "GeneralHelper.h"
#include "Shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

extern void AnalyzeUnionString(const CString strSource,       // Դ�ַ���
							   CString& strInfoPath,	 // ����·��
							   CString& strRefSection,   // ���ö���
							   CString& strRefEntry,     // ����ֵ��
							   CString& strRefFileName   // �����ļ���
							   );
extern CString GetNameByMarketMain(int nMarket);
extern BOOL MakeFile(CString strFileName);
extern BOOL CreateMultipleDirectory(char * sDirectory);
CInfoData::CInfoData(void)
{
}

CInfoData::~CInfoData(void)
{
}

char* CInfoData::Read(AskData* pAsk, long& lSize)
{
	ReqTextData* pData = (ReqTextData*)pAsk->m_pCode;
	if (pData == NULL || IsBadReadPtr(pData,sizeof(ReqTextData)))
		return NULL;
	CString filepath = GetFilePath(&pData->m_sMarkData, (CodeInfo*)&pAsk->m_nPrivateKey);
	long lLen = (long)(sizeof(AnsTextData) - sizeof(char));
	CFile fp;

	if ( fp.Open(filepath,  ( CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate|CFile::typeBinary)))
	{
		long length = fp.GetLength();
		if (length <= 0)
		{
			fp.Close();
			return NULL;
		}
		lSize = length;
		char* pBuffer = new char[length];
		memset(pBuffer, 0, length);	
		fp.SeekToBegin();
		fp.Read(pBuffer, length);
		fp.Close();
		return pBuffer;
	}	
	return NULL;
}

CString CInfoData::GetFilePath( TextMarkData* pData , CodeInfo* pCode)
{
	CString strFilePath;
	CString strInfoPath;
	CString strRefSection;   // ���ö���
	CString strRefEntry;     // ����ֵ��
	CString strRefFileName;

	AnalyzeUnionString(pData->m_szInfoCfg,strInfoPath,
		strRefSection,strRefEntry,strRefFileName);

	CString strSection;
	if( !strRefSection.IsEmpty() )
	{
		strSection = strRefSection;
	}
	else
	{
		strSection = "��ɫ��Ѷ";
	}
	if( strSection.IsEmpty() )
		return 0;

	CString strEntry;
	if( strRefEntry.IsEmpty() && pCode != NULL )
	{
		strEntry.Format(strInfoPath,GetNameByMarketMain(pCode->m_cCodeType));
	}
	else
	{
		strEntry = strRefEntry;
	}

	CString strRet;
	CStringArray ayRet;
	int bRet = 0;

	strFilePath = CGeneralHelper::GetSpecifyPath(CGeneralHelper::ZIXUN_PATH);		
	strFilePath += strSection;
	strFilePath += "\\";
	strFilePath += strEntry;
	strFilePath += "\\";
	
	CreateMultipleDirectory(strFilePath.GetBuffer());
	
	CString str = pData->m_cTitle;
	//str += "-";
	str += PathFindFileName(pData->m_cFilePath);

	strFilePath += str;
	return strFilePath;

}

void CInfoData::Write( void* pData, CodeInfo* pCode)
{
	if (pData == NULL || IsBadReadPtr(pData,1))
		return;
	PacketInfo* pHead = (PacketInfo*)pData;
	AnsTextData* pAnsData = (AnsTextData*)pHead->m_pszData;
	CString filename = GetFilePath(&pAnsData->m_sMarkData, pCode);

	CFile fp;
	if ( fp.Open(filename, ( CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone |CFile::typeBinary) ))
	{
		fp.Write(pAnsData->m_cData, pAnsData->m_nSize);
		fp.Close();
	}

}