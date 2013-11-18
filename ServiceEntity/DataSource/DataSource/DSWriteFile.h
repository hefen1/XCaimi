/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	DSWriteFile.h
*	文件标识：
*	摘	  要：	数据引擎中的写文件类
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-9-1
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once

#include "..\..\..\Common\QuoteComm\QuoteDefine.h"

#define EX_RIGHT_FILE			"exright.dat"			/* 除权数据文件 */
#define CURRENT_FINANCE_FILE	"curfinance.fin"		/* 最新财务数据 */
#define SYS_BLOCK_FILE			"sysblock"				/* 系统板块数据名称，配置文件扩展名：INI，数据文件扩展名：DAT*/
#define USER_BLOCK_FILE			"userblock"				/* add by lxqi 20090907 后台自定义板块 */
#define PYJC_INI_FILE			"pyjc.ini"				/*拼音简称文件*/

typedef int (WINAPI* PFN_UNZIPFILE)(const char *,const char *);


#define DSVERSION_CODELIST			1000
#include "CommBourseManager.h"
#include "stockManager.h"
class CDSWriteFile
{
public:
enum FileType
{
	BlockUserStock,		// 板块、自选股
	Save_DrawObj,		// 绘制
	Save_GridObj,		// 表格
	Save_PageObj,		// 页面
	Setting,			// 设置...
	SelfDir,			// 页面路径
	SelfMenu,			// 菜单
	SelfTemp,			// 临时目录
	DownloadDataTemp,	// 下载数据临时目录
	InfoData,			// 资讯数据根路径
	ZiXunData,	        // 多资讯数据根路径
	DefaultZiXunData,	// 缺省资讯数据根路径
	QQBasePath,			// QQ聊天数据根路径
	MemoBasePath,		// 备忘录路径 
	FinancialData,		// 财务报表分析数据路径 
	HTML,				// HTML
	Keyboard,			// 自定义键盘路径
	UserDefineIndex,	// 自定义指数保存路径

	FenShiDat,			//分时数据
	LevelDat,			//Level2数据路径
	RealDat,			// 分时数据路径
	InitDat,			// 初始化数据路径
	XiDan,				// 下单路径
	ReplaceDataDir,     // 修改数据保存路径
	BlockIndexDir,			// 板块指数保存路径
	ExternDll,			// 外部dll路径
	UserBlock,			// add by lxqi 20090907 后台自定义板块
};

protected:
	CString						m_strSettingPath;
	CString						m_strBlockPath;
	CString						m_strInitPath;
	HMODULE						m_hZipFileDll;
	PFN_UNZIPFILE				m_pfnUnZipFile;

	CFile						m_fileStock;
public:
	//	构造函数与析构函数
	CDSWriteFile();
	~CDSWriteFile();
protected:
	BOOL						WriteTempFile(CString strFileName, char* pszData, int nSize);
public:
	//初始化
	BOOL						InitWriteFileEnv();
	//获取路径
	CString						GetSettingFilePath();
	// 登录后返回包写盘； 只有存在包错误的情况下才返回FALSE(m_nError!=0)
	BOOL						WriteAnsLoginData(AnsLogin* pAnsLogin, CString& pHtmlPage, CStringArray& pAy);
	//写板块数据
	BOOL						WriteAnsSysBlockData(char* pszData, int nSize);
	//写财务数据
	BOOL						WriteAnsFinanceData(char* pszData, int nSize);
	//写除权数据
	BOOL						WriteAnsExrightData(char* pszData, int nSize);
	//写市场信息
	BOOL						WriteComBouseData(BourseInfo* pInfo);
	//读市场信息
	BOOL						ReadComBouseData(CHsBouseManager* pManager);
	//写股票信息
	BOOL						StartWriteStockData();
	BOOL						WriteStocksData(StockUserInfo* pStock);
	BOOL						EndWriteStockData();
	//读股票信息
	BOOL						ReadStocksData(CStockManager* pManager);
	//写下载配置文件
	BOOL						WriteRequestFile(CString strFileName, void* pData, int nSize);
	//读财务数据
	BOOL						SetStocksFinanceData(CMapStringToPtr& mapFinanceData);
	BOOL						ReadStocksFinanceData(CString strCodeComp, CArray<CurrentFinanceData>& ayFinanceData, int nTotal);
	//读除权数据
	BOOL						ReadStocksExrightData(CString strCodeComp, CArray<CurrentExRightData>& ayExrightData, int nTotal);
};