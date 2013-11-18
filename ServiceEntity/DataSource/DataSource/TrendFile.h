/*******************************************************************************
* Copyright (c)2011, 杭州恒生电子
* All rights reserved.
*
* 文件名称：CTrendFile.h
* 文件标识：历史分时 分笔
* 摘    要：DISK文件操作主类
*
* 当前版本：
* 作    者：
* 完成日期：
*
* 取代版本：
* 原 作 者：
* 完成日期：
* 参考资料: DISK数据请求包定义.DOC
*******************************************************************************/


#pragma once


#define EVERYTRENDSIZE   241
#define DEFAULTTRENINDEXCOUNT 1000
struct CTrendFileHead 
{
public:
	long 				m_lVersion; // 文件结构版本标识
	unsigned short		m_nIndexSize;    //索引个数
	unsigned short		m_nDataSize;     //存放的数据结构体大小
	long 			    m_lCount;         // 当前索引数据个数	 
};

//分时文件索引
struct CTrendDataIndex
{
public:
	long			m_lDate;		// 日期 格式:20030915
	CodeInfo        m_code;         //代码
	long	        m_lDataPosition;// 数据在文件中的起始位置
	long			m_lDataSize;	// 该索引块数据大小

};

class CTrendFile
{

public:
	enum TrendDataType { HisTrace, NewTrace, Tick }; //历史分时，当天分时，分笔
	CTrendFile(void);
	virtual ~CTrendFile(void);

	//打开文件
	BOOL Open(CodeInfo* pCode,TrendDataType type);
	//初始化索引
	BOOL InitIndex();
	//分时走势，按市场分类存放
	BOOL InitIndex(HSMarketDataType nMarket);
	//读取数据
	char* Read(long lDate, long& nSize,CodeInfo* pCode=NULL);
	//写数据
	BOOL Write(char* pData, long lSize,long lDate, CodeInfo* pCode=NULL);

	BOOL CreateMultipleDirectory(char * sDirectory);
	BOOL MakeFile(CString strFileName);
	static BOOL DelFile(CString);
protected:
	CFile*			 m_pFile;      //文件指针
	char*			 m_strBuffer;  //索引内容
	CTrendFileHead   m_Head;       //文件头
	TrendDataType    m_nTrendType;  //请求数据类型
	CMutex			 m_mutex;	 //文件读写互斥量
};
