#pragma once

#include "hsds_comudata.h"
#include "MapViewFile.h"

#define KLINEDATAFILE_VERSION_1       1;
#define KLINEDATAFILE_EXPAND          1024*1024*8

struct KLineDataFileHead
{
	unsigned int m_nVersion;//�汾��
	unsigned int m_nIndexCount;//������
	unsigned int m_nIndexCapability; //������������
	unsigned int m_nPageCapability;  //��ҳ����
	unsigned int m_nPageNO;           //��ǰҳ��
};


struct PageInfo
{
	unsigned int m_nPageNO;//���
	CodeInfo	 m_code;   //��ŵĴ���
	int			 m_count;  //ҳ�д�ŵ����߸���
};

struct IndexInfo  //����
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
	
	//ӳ�������ļ�
    BOOL OpenIndexData(CString filename);
	//��ȡ������Ϣ
	 static DWORD GetIndexData(CodeInfo* pCode,int peroid,CString indexfilename,KLineDataFileHead& head, CArray<PageInfo,PageInfo>&);
	
	 static CKLineDataFile* GetIndexFile(CString strKey,short nPeriod);
	//ȡ�ļ�ͷ��Ϣ����
	 static void GetFileHeadOption(KLineDataFileHead *pHead,short nPeriod);
	//ȡ��Ӧ��K���ļ�  qinhn 20101010 ���Ĭ�ϲ���bReleaseFile��Ϊ������������ʱ�ͷ�ӳ���ļ�
	 static CKLineDataFile* GetKLineDataFile(CString strKey,BOOL bReleaseFile = FALSE);
	//дK���ļ�
	 static void WriteKLineData(CodeInfo *pCode,short nPeriod,char* buf,long nBufLen);
	//��K���ļ�
	 static BOOL ReadKLineData(CodeInfo *pCode,short nPeriod,char** buf,int &nDataCount);	
	
	 static void WriteKLineDataEx(CodeInfo *pCode,short nPeriod,char* buf,long nBufLen);  
	 static  BOOL ReadKLineDataEx(CodeInfo *pCode,short nPeriod,char** buf,int &nDataCount);


	 static BOOL IsSameCode(const CodeInfo* pSrc,const CodeInfo* pDes);
	static void RemoveMap();
	//qinhn �ͷ�ӳ���ļ� ������ռ�ù����ڴ�
	 void ReleaseMapFile(CodeInfo *pCode,short nPeriod);
	//ȡ�����ļ� �����ļ�·��
	static BOOL GetFilePath(CodeInfo* pCode,  int nPeroid, CString& indexPath, CString& dataPath);
	//���ļ�
	BOOL Open(CString strFileName);
	//��ʼ���ļ�--дͷ��Ϣ
	BOOL InitFile();
	//ȡ���������ƫ��
	DWORD GetIndexOffset(CodeInfo *pCode);
	//ȡ������Ϣ
	BOOL  GetCodePages(DWORD dwIndexOffset,CArray<PageInfo,PageInfo> *codePages);
	//���ҳ����
	BOOL ClearOnePage(PageInfo* pPage);
	//дҳ����
	BOOL WriteOnePage(PageInfo* pPage,LPVOID pBuf,DWORD dwLength);
	//д����
	BOOL WriteIndexInfo(DWORD dwIndexOffset,CodeInfo *pCode,CArray<PageInfo,PageInfo> *codePages);
	//���ļ�:���ļ���dwLength��pBuf��ָ�Ŀռ�,pBufҪ�¼�
	BOOL ReadOnePage(const PageInfo* pPage,LPVOID pBuf,DWORD dwLength);
	
	//ȡ�����ļ���
	CCriticalSection* GetCriticalSection();
	
	//ȡ����ҳ��С
	int GetOneIndexSize();
	//ȡҳ��С
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
	 CCriticalSection		m_critical;      //�ļ�������
	/* CCriticalSection		m_indexcritical;      //�����ļ�������*/
	 char*			 m_IndexBuffer;  //��������
};
