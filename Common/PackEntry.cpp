/*******************************************************
  源程序名称:PackEntry.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  打包解包器
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "PackEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPackEntry::CPackEntry()
{

}
CPackEntry::~CPackEntry()
{

}

void CPackEntry::Error()
{
	CString sError = GetLastError();
	char* szError = new char[sError.GetLength() + 1];
	memcpy(szError, sError, sError.GetLength() + 1);
	THROW(szError);
}
CString CPackEntry::GetDLLFileName()
{
	CString sDLLFileName = "HsJyCommd.dll";
	return sDLLFileName;
}
PACKET_HANDLE CPackEntry::CreatePM()
{
	typedef PACKET_HANDLE(_cdecl *EntryFunc)();
	PACKET_HANDLE iPM = 0;
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjcpm_create_pm");
	if (myFunc != NULL)
	{
		iPM = myFunc();
	}
	return iPM;
}

void CPackEntry::DestroyPM(const PACKET_HANDLE hPM)
{
	typedef void(_cdecl *EntryFunc)(PACKET_HANDLE hPM);
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjcpm_destroy_pm");
	if (myFunc != NULL)
	{
		myFunc(hPM);
	}

}

BOOL CPackEntry::GetHeader(const PACKET_HANDLE hPM, const EWholePacketHeade iIndex, unsigned int& iValue)
{
	BOOL bTmp = FALSE;
	typedef int(_cdecl *EntryFunc)(PACKET_HANDLE hPM, int iIndex, unsigned int& iValue);
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjcpm_get_header_value");
	if (myFunc != NULL)
	{
		bTmp = (myFunc(hPM, (int)iIndex, iValue) == 0);
	}
	return bTmp;
}

BOOL CPackEntry::SetHeader(const PACKET_HANDLE hPM, const EWholePacketHeade iIndex, unsigned int iValue)
{
	BOOL bTmp = FALSE;
	typedef int(_cdecl *EntryFunc)(PACKET_HANDLE hPM, int iIndex, unsigned int iValue);
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjcpm_set_header_value");
	if (myFunc != NULL)
	{
		bTmp = (myFunc(hPM, int(iIndex), iValue) == 0);
	}
	return bTmp;
}

CString CPackEntry::GetLastError()
{
	CString sTmp = "";
	typedef const char*(_cdecl *EntryFunc)();
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjc_get_last_error");
	if (myFunc != NULL)
	{
		sTmp = myFunc();
	}
	return sTmp;
}

BOOL CPackEntry::ParseData(const PACKET_HANDLE hPM, char * lpPackData, const int iDataLen)
{
	BOOL bTmp = FALSE;
	typedef int(_cdecl *EntryFunc)(PACKET_HANDLE hPM, const char * lpPackData, int iDataLen);
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjcpm_parse_proto_mesg");
	if (myFunc != NULL)
	{
		bTmp = (myFunc(hPM, lpPackData, iDataLen) == 0);
	}
	return bTmp;
}

char* CPackEntry::GetData(const PACKET_HANDLE hPM, const CString sName, int& iDataLen)
{
	//CString sTmp = "";
	typedef int(_cdecl *EntryFunc)(PACKET_HANDLE hPM, const char * lpName, char * lpDataBuf, int& iDataLen);
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjcpm_get_proto_unit");
	char* lpDataBuf = NULL;
	if (myFunc != NULL)
	{
		//char* lpName =(LPTSTR)(LPCTSTR)sName;
		//int iDataLen = 0;
		if (myFunc(hPM, sName, lpDataBuf, iDataLen) >= 0)
		{
			lpDataBuf = new char[iDataLen];
			if (myFunc(hPM, sName, lpDataBuf, iDataLen) <= 0)
			{
				//sTmp = lpDataBuf;
				Error();
			}
		}

	}
	return lpDataBuf;
}

BOOL CPackEntry::SetData(const PACKET_HANDLE hPM, const EPacketType eType, const CString sName, char* lpData, int iDataLen)
{
	BOOL bTmp = FALSE;
	typedef int(_cdecl *EntryFunc)(PACKET_HANDLE hPM, int iType, const char * lpName, const char * lpValue, int iDataLen);
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjcpm_push_back_proto_unit");
	if (myFunc != NULL)
	{
		//char* lpName = (LPTSTR)(LPCTSTR)sName;
		bTmp = (myFunc(hPM, int(eType), sName, lpData, iDataLen) == 0);		
	}
	return bTmp;
}

BOOL CPackEntry::DataExist(const PACKET_HANDLE hPM, const CString sName)
{
	BOOL bTmp = FALSE;
	typedef int(_cdecl *EntryFunc)(PACKET_HANDLE hPM, const char * lpName, char * lpDataBuf, int& iDataLen);
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjcpm_get_proto_unit");
	if (myFunc != NULL)
	{
		//char* lpName = sName.GetString();
		char* lpDataBuf = NULL;
		int iDataLen = 0;
		bTmp = ((myFunc(hPM, sName, lpDataBuf, iDataLen) >= 0) &&(iDataLen > 0));
	}
	return bTmp;
}

char* CPackEntry::GetPackData(const PACKET_HANDLE hPM, int& iDataLen)
{
	typedef int(_cdecl *EntryFunc)(PACKET_HANDLE hPM, char* lpPackBuf, int& iPackLen);
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjcpm_compose");
	char* lpPackBuf = NULL;
	int iPackLen = 0;
	if (myFunc != NULL)
	{			
		if (myFunc(hPM, lpPackBuf, iPackLen) >= 0)
		{
			lpPackBuf = new char[iPackLen];
			if (myFunc(hPM, lpPackBuf, iPackLen) <= 0)
			{
				//sTmp = lpPackBuf;
				Error();
			}
		}
	}
	iDataLen = iPackLen;
	return lpPackBuf;
}

int CPackEntry::GetNVCount(const PACKET_HANDLE hPM)
{
	typedef int(_cdecl *EntryFunc)(PACKET_HANDLE hPM);
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjcpm_get_nv_size");
	if (myFunc != NULL)
	{
		return myFunc(hPM);
	}
	else
		return 0;
}

char* CPackEntry::GetNVNameByIndex(const PACKET_HANDLE hPM, const int iIndex, int& iDataLen, int& iType)
{
	typedef int(_cdecl *EntryFunc)(PACKET_HANDLE hPM,int iIndex, char* lpPackBuf, int& iPackLen, int& iType);
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("hjcpm_get_nv_name");
	char* lpPackBuf = NULL;
	int iPackLen = 0;
	if (myFunc != NULL)
	{			
		if (myFunc(hPM, iIndex, lpPackBuf, iPackLen, iType) >= 0)
		{
			lpPackBuf = new char[iPackLen];
			if (myFunc(hPM, iIndex, lpPackBuf, iPackLen, iType) <= 0)
			{
				//sTmp = lpPackBuf;
				Error();
			}
		}
	}
	iDataLen = iPackLen;
	return lpPackBuf;
}

//==========================================================================
/*
CAr2PackEntry::CAr2PackEntry()
{

}

CAr2PackEntry::~CAr2PackEntry()
{

}

CString CAr2PackEntry::GetDLLFileName()
{
	CString sDLLFileName = "fsc_pack";
	return sDLLFileName;
}

IPackService * CAr2PackEntry::GetPackServiceInstance()
{
	IPackService* lpTmp = NULL;
	typedef IPackService*(FUNCTION_CALL_MODE *EntryFunc)(IAppContext * p);
	EntryFunc myFunc = (EntryFunc)CDLLEntryBase::GetLibProcAddress("getPackServiceInstance");
	if (myFunc != NULL)
	{
		lpTmp = myFunc(NULL);
	}
	return lpTmp;	 
}*/


//==========================================================================

CPacker::CPacker()
{
	m_hPM = 0;
	m_lpPackEntry = new CPackEntry;
	m_hPM = m_lpPackEntry->CreatePM();
}
CPacker::~CPacker()
{
	m_lpPackEntry->DestroyPM(m_hPM);
	delete(m_lpPackEntry);
	m_lpPackEntry = NULL;
}

unsigned int CPacker::GetHeader(const EWholePacketHeade iIndex)
{
	unsigned int iValue = 0;
	m_lpPackEntry->GetHeader(m_hPM, iIndex, iValue);
	return iValue;
}

BOOL CPacker::SetHeader(const EWholePacketHeade iIndex, unsigned int iValue)
{
	return m_lpPackEntry->SetHeader(m_hPM, iIndex, iValue);
}

CString CPacker::GetLastError()
{
	return m_lpPackEntry->GetLastError();
}

BOOL CPacker::ParseData(char* lpPackData, int iDataLen)
{
	return m_lpPackEntry->ParseData(m_hPM, lpPackData, iDataLen);
}

char* CPacker::GetData(CString sName, int& iDataLen)
{
	return m_lpPackEntry->GetData(m_hPM, sName, iDataLen);
}

BOOL CPacker::SetData(const EPacketType eType, const CString sName, char* lpData, int iDataLen)
{
	return m_lpPackEntry->SetData(m_hPM, eType, sName, lpData, iDataLen);
}

BOOL CPacker::DataExist(const CString sName)
{
	return m_lpPackEntry->DataExist(m_hPM, sName);
}

char* CPacker::GetPackData(int& iDataLen)
{
	return m_lpPackEntry->GetPackData(m_hPM, iDataLen);
}

int CPacker::GetNVCount()
{
	return m_lpPackEntry->GetNVCount(m_hPM);
}

CString CPacker::GetNVNameByIndex(const int iIndex, int& iType)
{
	CString sReturn = "";
	int nLen = 0;
	int nType = 0;
	char* pBuf = m_lpPackEntry->GetNVNameByIndex(m_hPM, iIndex, nLen, nType);
	if (nLen > 0)
	{
		char* pStr = new char[nLen + 1];
		memcpy(pStr, pBuf, nLen);
		memcpy(pStr + nLen, "\0", 1);
		sReturn = pStr;
		delete[] pBuf;
		pBuf = NULL;
		delete[] pStr;
		pStr = NULL;
	}
	iType = nType;
	return sReturn;
}

CString CPacker::GetStr(CString sName)
{
	CString sReturn = "";
	int nLen = 0;
	char* pBuf = m_lpPackEntry->GetData(m_hPM, sName, nLen);
	if (nLen > 0)
	{
		char* pStr = new char[nLen + 1];
		memcpy(pStr, pBuf, nLen);
		memcpy(pStr + nLen, "\0", 1);
		sReturn = pStr;
		delete[] pBuf;
		pBuf = NULL;
		delete[] pStr;
		pStr = NULL;
	}
	return sReturn;
}