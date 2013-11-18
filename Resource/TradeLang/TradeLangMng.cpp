/*******************************************************
  Դ��������:TradeLangMng.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ������Դ
  ����˵��:  ��Դ������ʵ��
  ��    ��:  shenglq
  ��������:  20100921
*********************************************************/
#include "stdafx.h"
#include "TradeLangMng.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTradeLang::CTradeLang()
{

}

CTradeLang::~CTradeLang()
{

}

void CTradeLang::Release()
{
	delete this;
}

HICON CTradeLang::LoadHICON(const UINT nIDResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return LoadHICON(AfxGetResourceHandle(), nIDResource);
}

HICON CTradeLang::LoadHICON(const HINSTANCE hInstance, const UINT nIDResource)
{
	return LoadIcon(hInstance, MAKEINTRESOURCE(nIDResource));
}

char* CTradeLang::LoadXML(const UINT nIDResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return LoadXML(AfxGetResourceHandle(), nIDResource);
}

char* CTradeLang::LoadXML(const HINSTANCE hInstance, const UINT nIDResource)
{
	char* pBuffer = NULL;
	//CString strModuleTempFile;
	//GetModuleFileName(hInstance,strModuleTempFile.GetBuffer(_MAX_PATH), sizeof(char)*_MAX_PATH);
	//strModuleTempFile.ReleaseBuffer();
	HRSRC hResource = FindResourceEx( hInstance, "XML", MAKEINTRESOURCE(nIDResource), 0 ); 
	if( hResource != NULL ) 
	{
		HGLOBAL hResLoad = (wchar_t *)LoadResource( hInstance, hResource ); 
		if( hResLoad != NULL )
		{ 
			long nLength = SizeofResource(hInstance, hResource); //The length
			void * pInfo = LockResource(hResLoad); 
			if(pInfo) 
			{ 
				const char * pTextInfo = (char *)pInfo;
				pBuffer = new char[nLength];
				//pBuffer[nLength] = '\0';
				memset(pBuffer,0,nLength);
				memcpy(pBuffer,pTextInfo,nLength);
				UnlockResource(hResLoad);
			} 
			FreeResource(hResLoad); //Free it 
		} 
	} 
	return pBuffer;
}

CString CTradeLang::LoadStr(const UINT nIDResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return LoadStr(AfxGetResourceHandle(), nIDResource);
}

CString CTradeLang::LoadStr(const HINSTANCE hInstance, const UINT nIDResource)
{
	CString str = "";
	LoadString(hInstance,nIDResource, str.GetBuffer(_MAX_PATH), sizeof(char)*_MAX_PATH);
	str.ReleaseBuffer();
	return str;
}




