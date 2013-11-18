/*************************************************************
*	Copyright (c)2003, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	WriteReadStandardConfig.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	��д��׼˽�������ļ���������
*
*	��ǰ�汾��	2.0
*	��	  �ߣ�	
*	�������ڣ�	2003-4-27
*
***************************************************************/
#ifndef	__WRITEREADSTANDARDCONFIG_H__
#define	__WRITEREADSTANDARDCONFIG_H__

#pragma once

//#ifndef Have_defineMapOb

//#define  Have_defineMapOb   0x01
//#endif


//	added by hlw 2002.0815
//	��д˽�������ļ���
class CWriteReadStandardConfig
{
public:
	//	���캯������������
	CWriteReadStandardConfig();
	~CWriteReadStandardConfig(void);
	
	//	��д˽�������ļ�
	
	/******************************************************************
	*	�������ܣ�	��ȡָ�����Ƶ������ļ���ָ���ε������ַ�������
	*				���Ѹ��ַ�����ֵ�����ָ�����ַ���������
	*	����������	const char* strSection	��ָ���Ķ�����
	*				CStringArray& strArray	������ṹ���ַ�������
	*				const char* strFileName	������д���ļ�����
	*	�� �� ֵ��	BOOL					�������ɹ�����TRUE��ʧ��ʱ����FALSE
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003-4-27
	*	�޸�˵����	
	*****************************************************************/
	static BOOL ReadConfigSectionEntry(const char* strSection, CStringArray& strArray,const char* strFileName = NULL);
	static BOOL ReadConfigSectionString(const char* strSection, CStringArray& strArray,const char* strFileName  = NULL);
	
	static BOOL ReadConfigSectionEntry(const char* pstrSection, 
		const char* pstrEntry, 
		CString* pstrItem, 
		CStringArray* pstrArray, 
		const char* pData, 
		const int nLength);
	/******************************************************************
	*	�������ܣ�	��ȡָ������ָ�����Ƶ����ͼ�ֵ
	*	����������	const char *strSection	��ָ���Ķ���
	*				const char* strEntry	������ȡ��������
	*				CUIntArray &UIntArray	����Ÿ����ͼ�ֵ������
	*				const char* strFileName ������д���ļ�����
	*	�� �� ֵ��	BOOL					�������ɹ�����TRUE��ʧ��ʱ����FALSE				
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003-4-27
	*	�޸�˵����	
	*****************************************************************/
	
	static BOOL ReadSectionInt(const char *strSection,const char* strEntry,CUIntArray &UIntArray,const char* strFileName = NULL);
	
	/******************************************************************
	*	�������ܣ�	��ȡָ������ָ�����Ƶ��ַ�����ֵ
	*	����������	const char* strSection	��ָ���Ķ���
	*				const char* strEntry	������ȡ�ļ�������
	*				CString& strItem		����������ȡ����ֵ�Ļ���
	*				const char* strFileName ������д���ļ�����
	*	�� �� ֵ��	BOOL 					�������ɹ�����TRUE��ʧ��ʱ����FALSE��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003-4-27
	*	�޸�˵����	
	*****************************************************************/
	
	static BOOL ReadConfigString(const char* strSection,const char* strEntry,CString& strItem,const char* strFileName = NULL);
	
	/******************************************************************
	*	�������ܣ�	��ȡָ������ָ�����Ƶ����ͼ�ֵ
	*	����������	const char* strSection	��ָ���Ķ���
	*				const char* strEntry	������ȡ�ļ�������
	*				int& nValue				����������ȡ����ֵ
	*				const char* strFileName ������д���ļ�����
	*	�� �� ֵ��	BOOL 					�������ɹ�����TRUE��ʧ��ʱ����FALSE��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003-4-27
	*	�޸�˵����	
	*****************************************************************/
	
	static BOOL ReadConfigInt(const char* strSection,const char* strEntry,int& nValue,const char* strFileName = NULL);
	
	/******************************************************************
	*	�������ܣ�	����ָ������ָ�����Ƶ��ַ�������ֵ��
	*	����������	const char* strSection	��ָ���Ķ���
	*				const char* strEntry	�������µļ�������
	*				const char* strItem		�������¼�����ֵ
	*				const char* strFileName ������д���ļ�����
	*	�� �� ֵ��	BOOL 					�������ɹ�����TRUE��ʧ��ʱ����FALSE��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003-4-27
	*	�޸�˵����	
	*****************************************************************/
	
	static BOOL WriteConfigString(const char* strSection,const char* strEntry, const char* strItem,const char* strFileName = NULL);
	
	/******************************************************************
	*	�������ܣ�	����ָ������ָ�����Ƶ����ͼ���ֵ��
	*	����������	const char* strSection	��ָ���Ķ���
	*				const char* strEntry	�������µļ�������
	*				CUIntArray &UIntArray	�������¼�����ֵ��
	*				const char* strFileName ������д���ļ�����
	*	�� �� ֵ��	BOOL 					�������ɹ�����TRUE��ʧ��ʱ����FALSE��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003-4-27
	*	�޸�˵����	
	*****************************************************************/

	
	static BOOL WriteSectionInt(const char *strSection,const char* strEntry,CUIntArray &UIntArray,const char* strFileName = NULL);
	static BOOL WriteSectionInt(const char *strSection,const char* strEntry,long lValue,const char* strFileName = NULL);
	
	
	/******************************************************************
	*	�������ܣ�	���µĶ���������ָ���Ķ���
	*	����������	const char *strSectionNew	��ָ�����¶���
	*				const char* strSectionOld	�������¶εĶ���
	*				const char* strFileName		�������µ��ļ�����
	*	�� �� ֵ��	BOOL						�������ɹ�����TRUE��ʧ��ʱ����FALSE��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003-4-27
	*	�޸�˵����	
	*****************************************************************/
	
	static BOOL ChangeSection(const char *strSectionNew,const char* strSectionOld,const char* strFileName = NULL);
	
	static CString GetContentsByHelpID(CString strFile,CString strHelpID);

};


#endif	//__WRITEREADSTANDARDCONFIG_H__