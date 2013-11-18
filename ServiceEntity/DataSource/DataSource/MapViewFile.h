#pragma once

#include <Afxmt.h>

class CriticalGuard 
{
private:
	CCriticalSection * m_pCritical;
public:
	CriticalGuard(CCriticalSection* pCritical)
	{	
		if(pCritical != NULL)
		{
			m_pCritical = pCritical;
			m_pCritical->Lock();
		}
	}
	~CriticalGuard()
	{
		if(m_pCritical != NULL)
		{
			m_pCritical->Unlock();
		}
	}
};

class CMapViewFile
{
private:
	LPVOID					m_pMapViewStart; //the starting address of the mapped view.
	DWORD					m_dwMapViewSize;  //size of mapped 
	DWORD					m_dwDefaultExtendSize;
	CString					m_strFileName;   //映射的文件名
public:
	CCriticalSection		m_critical;      //文件访问锁
	
	CMapViewFile();
	CMapViewFile(DWORD dwDefaultExtendSize);
	~CMapViewFile(void);

	BOOL Open(CString strFileName,DWORD dwExpandSize);
	LPVOID Read(DWORD dwOffset);
	BOOL Write(DWORD dwOffset,LPVOID pBuf,DWORD dwLength);
	BOOL Clear(DWORD dwOffset,DWORD dwLength);
	BOOL ExpandFile(DWORD dwExpandSize);
	void UnMapFile();


	static BOOL MakeFile(CString strFileName);
	static BOOL CreateMultipleDirectory(char * sDirectory);
};
