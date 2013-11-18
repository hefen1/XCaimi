/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DSWriteFile.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	���������е�д�ļ���
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-9-1
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once

#include "..\..\..\Common\QuoteComm\QuoteDefine.h"

#define EX_RIGHT_FILE			"exright.dat"			/* ��Ȩ�����ļ� */
#define CURRENT_FINANCE_FILE	"curfinance.fin"		/* ���²������� */
#define SYS_BLOCK_FILE			"sysblock"				/* ϵͳ����������ƣ������ļ���չ����INI�������ļ���չ����DAT*/
#define USER_BLOCK_FILE			"userblock"				/* add by lxqi 20090907 ��̨�Զ����� */
#define PYJC_INI_FILE			"pyjc.ini"				/*ƴ������ļ�*/

typedef int (WINAPI* PFN_UNZIPFILE)(const char *,const char *);


#define DSVERSION_CODELIST			1000
#include "CommBourseManager.h"
#include "stockManager.h"
class CDSWriteFile
{
public:
enum FileType
{
	BlockUserStock,		// ��顢��ѡ��
	Save_DrawObj,		// ����
	Save_GridObj,		// ���
	Save_PageObj,		// ҳ��
	Setting,			// ����...
	SelfDir,			// ҳ��·��
	SelfMenu,			// �˵�
	SelfTemp,			// ��ʱĿ¼
	DownloadDataTemp,	// ����������ʱĿ¼
	InfoData,			// ��Ѷ���ݸ�·��
	ZiXunData,	        // ����Ѷ���ݸ�·��
	DefaultZiXunData,	// ȱʡ��Ѷ���ݸ�·��
	QQBasePath,			// QQ�������ݸ�·��
	MemoBasePath,		// ����¼·�� 
	FinancialData,		// ���񱨱��������·�� 
	HTML,				// HTML
	Keyboard,			// �Զ������·��
	UserDefineIndex,	// �Զ���ָ������·��

	FenShiDat,			//��ʱ����
	LevelDat,			//Level2����·��
	RealDat,			// ��ʱ����·��
	InitDat,			// ��ʼ������·��
	XiDan,				// �µ�·��
	ReplaceDataDir,     // �޸����ݱ���·��
	BlockIndexDir,			// ���ָ������·��
	ExternDll,			// �ⲿdll·��
	UserBlock,			// add by lxqi 20090907 ��̨�Զ�����
};

protected:
	CString						m_strSettingPath;
	CString						m_strBlockPath;
	CString						m_strInitPath;
	HMODULE						m_hZipFileDll;
	PFN_UNZIPFILE				m_pfnUnZipFile;

	CFile						m_fileStock;
public:
	//	���캯������������
	CDSWriteFile();
	~CDSWriteFile();
protected:
	BOOL						WriteTempFile(CString strFileName, char* pszData, int nSize);
public:
	//��ʼ��
	BOOL						InitWriteFileEnv();
	//��ȡ·��
	CString						GetSettingFilePath();
	// ��¼�󷵻ذ�д�̣� ֻ�д��ڰ����������²ŷ���FALSE(m_nError!=0)
	BOOL						WriteAnsLoginData(AnsLogin* pAnsLogin, CString& pHtmlPage, CStringArray& pAy);
	//д�������
	BOOL						WriteAnsSysBlockData(char* pszData, int nSize);
	//д��������
	BOOL						WriteAnsFinanceData(char* pszData, int nSize);
	//д��Ȩ����
	BOOL						WriteAnsExrightData(char* pszData, int nSize);
	//д�г���Ϣ
	BOOL						WriteComBouseData(BourseInfo* pInfo);
	//���г���Ϣ
	BOOL						ReadComBouseData(CHsBouseManager* pManager);
	//д��Ʊ��Ϣ
	BOOL						StartWriteStockData();
	BOOL						WriteStocksData(StockUserInfo* pStock);
	BOOL						EndWriteStockData();
	//����Ʊ��Ϣ
	BOOL						ReadStocksData(CStockManager* pManager);
	//д���������ļ�
	BOOL						WriteRequestFile(CString strFileName, void* pData, int nSize);
	//����������
	BOOL						SetStocksFinanceData(CMapStringToPtr& mapFinanceData);
	BOOL						ReadStocksFinanceData(CString strCodeComp, CArray<CurrentFinanceData>& ayFinanceData, int nTotal);
	//����Ȩ����
	BOOL						ReadStocksExrightData(CString strCodeComp, CArray<CurrentExRightData>& ayExrightData, int nTotal);
};