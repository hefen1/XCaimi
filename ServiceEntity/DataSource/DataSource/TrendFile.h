/*******************************************************************************
* Copyright (c)2011, ���ݺ�������
* All rights reserved.
*
* �ļ����ƣ�CTrendFile.h
* �ļ���ʶ����ʷ��ʱ �ֱ�
* ժ    Ҫ��DISK�ļ���������
*
* ��ǰ�汾��
* ��    �ߣ�
* ������ڣ�
*
* ȡ���汾��
* ԭ �� �ߣ�
* ������ڣ�
* �ο�����: DISK�������������.DOC
*******************************************************************************/


#pragma once


#define EVERYTRENDSIZE   241
#define DEFAULTTRENINDEXCOUNT 1000
struct CTrendFileHead 
{
public:
	long 				m_lVersion; // �ļ��ṹ�汾��ʶ
	unsigned short		m_nIndexSize;    //��������
	unsigned short		m_nDataSize;     //��ŵ����ݽṹ���С
	long 			    m_lCount;         // ��ǰ�������ݸ���	 
};

//��ʱ�ļ�����
struct CTrendDataIndex
{
public:
	long			m_lDate;		// ���� ��ʽ:20030915
	CodeInfo        m_code;         //����
	long	        m_lDataPosition;// �������ļ��е���ʼλ��
	long			m_lDataSize;	// �����������ݴ�С

};

class CTrendFile
{

public:
	enum TrendDataType { HisTrace, NewTrace, Tick }; //��ʷ��ʱ�������ʱ���ֱ�
	CTrendFile(void);
	virtual ~CTrendFile(void);

	//���ļ�
	BOOL Open(CodeInfo* pCode,TrendDataType type);
	//��ʼ������
	BOOL InitIndex();
	//��ʱ���ƣ����г�������
	BOOL InitIndex(HSMarketDataType nMarket);
	//��ȡ����
	char* Read(long lDate, long& nSize,CodeInfo* pCode=NULL);
	//д����
	BOOL Write(char* pData, long lSize,long lDate, CodeInfo* pCode=NULL);

	BOOL CreateMultipleDirectory(char * sDirectory);
	BOOL MakeFile(CString strFileName);
	static BOOL DelFile(CString);
protected:
	CFile*			 m_pFile;      //�ļ�ָ��
	char*			 m_strBuffer;  //��������
	CTrendFileHead   m_Head;       //�ļ�ͷ
	TrendDataType    m_nTrendType;  //������������
	CMutex			 m_mutex;	 //�ļ���д������
};
