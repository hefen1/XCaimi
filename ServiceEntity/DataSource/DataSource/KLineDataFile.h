#pragma once

#include "hsds_comudata.h"
#include "MapViewFile.h"

#define KLINEDATAFILE_VERSION_1       1;
#define KLINEDATAFILE_EXPAND          1024*1024*8

struct KLineDataFileHead
{
	unsigned int m_nVersion;//版本号
	unsigned int m_nIndexCount;//索引数
	unsigned int m_nIndexCapability; //单个索引容量
	unsigned int m_nPageCapability;  //单页容量
	unsigned int m_nPageNO;           //当前页数
};


struct PageInfo
{
	unsigned int m_nPageNO;//编号
	CodeInfo	 m_code;   //存放的代码
	int			 m_count;  //页中存放的日线个数
};

struct IndexInfo  //索引
{
	CodeInfo m_CodeInfo;
	PageInfo m_pages[1];
	
	BOOL IsEmptyIndex()
	{
		if(m_CodeInfo.m_cCodeType == 0)
			return TRUE;
		else
			return FALSE;
	}
};


class CKLineDataFile
{
public:
	CKLineDataFile();
	CKLineDataFile(DWORD dwDefaultExpand);
	~CKLineDataFile(void);
	
	//映射索引文件
    BOOL OpenIndexData(CString filename);
	//获取索引信息
	 static DWORD GetIndexData(CodeInfo* pCode,int peroid,CString indexfilename,KLineDataFileHead& head, CArray<PageInfo,PageInfo>&);
	
	 static CKLineDataFile* GetIndexFile(CString strKey,short nPeriod);
	//取文件头信息配置
	 static void GetFileHeadOption(KLineDataFileHead *pHead,short nPeriod);
	//取对应的K线文件  qinhn 20101010 添加默认参数bReleaseFile，为了在数据下载时释放映射文件
	 static CKLineDataFile* GetKLineDataFile(CString strKey,BOOL bReleaseFile = FALSE);
	//写K线文件
	 static void WriteKLineData(CodeInfo *pCode,short nPeriod,char* buf,long nBufLen);
	//读K线文件
	 static BOOL ReadKLineData(CodeInfo *pCode,short nPeriod,char** buf,int &nDataCount);	
	
	 static void WriteKLineDataEx(CodeInfo *pCode,short nPeriod,char* buf,long nBufLen);  
	 static  BOOL ReadKLineDataEx(CodeInfo *pCode,short nPeriod,char** buf,int &nDataCount);


	 static BOOL IsSameCode(const CodeInfo* pSrc,const CodeInfo* pDes);
	static void RemoveMap();
	//qinhn 释放映射文件 避免其占用过大内存
	 void ReleaseMapFile(CodeInfo *pCode,short nPeriod);
	//取数据文件 索引文件路径
	static BOOL GetFilePath(CodeInfo* pCode,  int nPeroid, CString& indexPath, CString& dataPath);
	//打开文件
	BOOL Open(CString strFileName);
	//初始化文件--写头信息
	BOOL InitFile();
	//取代码的索引偏移
	DWORD GetIndexOffset(CodeInfo *pCode);
	//取索引信息
	BOOL  GetCodePages(DWORD dwIndexOffset,CArray<PageInfo,PageInfo> *codePages);
	//清空页数据
	BOOL ClearOnePage(PageInfo* pPage);
	//写页数据
	BOOL WriteOnePage(PageInfo* pPage,LPVOID pBuf,DWORD dwLength);
	//写索引
	BOOL WriteIndexInfo(DWORD dwIndexOffset,CodeInfo *pCode,CArray<PageInfo,PageInfo> *codePages);
	//读文件:从文件读dwLength到pBuf把指的空间,pBuf要事件
	BOOL ReadOnePage(const PageInfo* pPage,LPVOID pBuf,DWORD dwLength);
	
	//取缓存文件锁
	CCriticalSection* GetCriticalSection();
	
	//取索引页大小
	int GetOneIndexSize();
	//取页大小
	int GetOnePageSize();


	static unsigned long GetLimitDate(CodeInfo *pCode,short nPeriod,int nDays);
	static CTimeSpan CKLineDataFile:: GetTimeStep(short nPeriod);
	static CTime GetNowTime(short nPeriod);
	static CTime GetTimeFromIntTime(unsigned long lDate,short nPeriod);
	static unsigned long GetIntTimeFromTime(const CTime &t,short nPeriod);
	static int GetDefaultDownloadDay();

private:

	void UnMapFile();
private:
	 CMutex            m_mutex;
	 KLineDataFileHead m_head;
	 CFile* m_pKlineFile;
	 CFile* m_pIndexFile;
	 CCriticalSection		m_critical;      //文件访问锁
	/* CCriticalSection		m_indexcritical;      //索引文件访问锁*/
	 char*			 m_IndexBuffer;  //索引内容
};
