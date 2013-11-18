// zipFileDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "zipFileDll.h"
#include "ZipArchive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern "C"
int WINAPI ZipFile(const char *pszPath,const char *pszZipFile)
{
	try
	{
		CZipArchive zip;//压缩

		CString strZip,strPath;

		strZip.Format("%s",pszZipFile);
		strPath.Format("%s",pszPath);

		zip.Open(strZip, CZipArchive::create, 0  );
		if(zip.AddNewFile(pszPath, 8) == FALSE)
		{
			zip.Close();
			return 0;
		}
		zip.Close();

	}
	catch(...)
	{
		return 0;
	}
	return 1;
}

extern "C"
int WINAPI UnZipFile(const char *pszZipFile,const char *pszPath)
{
	if( pszZipFile == NULL || pszPath == NULL )
		return 0;

	CString strZip,strPath;
	strZip.Format("%s",pszZipFile);
	strPath.Format("%s",pszPath);

	CZipArchive zip;
	zip.Open(strZip, CZipArchive::open);
	for (int i = 0; i < zip.GetNoEntries(); i++)//输出文件(!!!没有文件夹)
	{
		if( !zip.ExtractFile(WORD(i) , strPath) )
		{ 
			break;
		}
	}
	zip.Close();

	return 1;
}
