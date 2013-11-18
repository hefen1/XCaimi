#include "StdAfx.h"
#include "QuoteLangMng.h"

CQuoteLang::CQuoteLang(void)
{
}

CQuoteLang::~CQuoteLang(void)
{
}


void CQuoteLang::Release()
{
	delete this;
}

HICON CQuoteLang::LoadHICON(const UINT nIDResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return LoadHICON(AfxGetResourceHandle(), nIDResource);
}

HICON CQuoteLang::LoadHICON(const HINSTANCE hInstance, const UINT nIDResource)
{
	return LoadIcon(hInstance, MAKEINTRESOURCE(nIDResource));
}

char* CQuoteLang::LoadXML(const UINT nIDResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return LoadXML(AfxGetResourceHandle(), nIDResource);
}

char* CQuoteLang::LoadXML(const HINSTANCE hInstance, const UINT nIDResource)
{
	char* pBuffer = NULL;
	CString strModuleTempFile;
	GetModuleFileName(hInstance,strModuleTempFile.GetBuffer(_MAX_PATH), sizeof(char)*_MAX_PATH);
	strModuleTempFile.ReleaseBuffer();

	HRSRC hResource = FindResourceEx( hInstance, _T("XML"), MAKEINTRESOURCE(nIDResource), 0); 
	if( hResource != NULL ) 
	{
		HGLOBAL hResLoad = (wchar_t *)LoadResource( hInstance, hResource ); 
		if( hResLoad != NULL )
		{ 
			DWORD nLength = SizeofResource(hInstance, hResource); //The length
			void * pInfo = LockResource(hResLoad); 
			if(pInfo) 
			{ 
				const char * pTextInfo = (char *)pInfo;
				pBuffer = new char[nLength];
				memset(pBuffer,0,nLength);
				memcpy(pBuffer,pTextInfo,nLength);
				UnlockResource(hResLoad);
			} 
			FreeResource(hResLoad); //Free it 
		} 
	} 
	return pBuffer;
}

CString CQuoteLang::LoadStr(const UINT nIDResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return LoadStr(AfxGetResourceHandle(), nIDResource);
}

CString CQuoteLang::LoadStr(const HINSTANCE hInstance, const UINT nIDResource)
{
	CString str = "";
	LoadString(hInstance,nIDResource, str.GetBuffer(_MAX_PATH), sizeof(char)*_MAX_PATH);
	str.ReleaseBuffer();
	return str;
}

// 获取图片
HANDLE CQuoteLang::LoadHImage(UINT nIDResource,
						   UINT uType,
						   int cxDesired,
						   int cyDesired,
						   UINT fuLoad)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return LoadHImage(AfxGetResourceHandle(), nIDResource, uType, cxDesired, cyDesired, fuLoad);
}
HANDLE CQuoteLang::LoadHImage(HINSTANCE hinst,
						   UINT nIDResource,
						   UINT uType,
						   int cxDesired,
						   int cyDesired,
						   UINT fuLoad)
{
	return LoadImage(hinst, MAKEINTRESOURCE(nIDResource), uType, cxDesired, cyDesired, fuLoad);
}

//获取菜单资源
HMENU CQuoteLang::LoadMenu(const UINT nIDResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return LoadMenu(AfxGetResourceHandle(), nIDResource);
}

HMENU CQuoteLang::LoadMenu(const HINSTANCE hInstance, const UINT nIDResource)
{
	return ::LoadMenu(hInstance, MAKEINTRESOURCE(nIDResource));
}
