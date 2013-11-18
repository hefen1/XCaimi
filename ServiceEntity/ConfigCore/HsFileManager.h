#pragma once
/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsFileManager.h
* 摘    要：文件操作实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-10
* 备	 注：
*******************************************************************************/
#include "ConfigBase.h"

class CHsFileManager
{
public:
	CHsFileManager();
	~CHsFileManager();

	CConfigBase* GetColorCfg();  // 获取颜色配置文件操作类指针
	CConfigBase* GetFontCfg();   // 获取字体配置文件操作类指针	
	CConfigBase* GetSystemCfg(); // 获取系统配置文件操作类指针
	CConfigBase* GetDispColumnCfg();   // 获取DispColumn.xml文件指针
	CConfigBase* GetReportColumnCfg(); // 获取ReportColumn.xml文件指针
	CConfigBase* GetReportTabCfg();    // 获取ReportTab.xml文件指针
	CConfigBase* GetServerCfg();       // 获取通讯配置文件指针
	CConfigBase* GetUserDataCfg();     // 获取运行中用户数据文件指针
	CConfigBase* GetKeyboardCfg();     // 获取运行中键盘精灵热键文件指针
	CConfigBase* GetKLineTab();         //获取K线tab项配置
	CConfigBase* GetKLinePostionCtrl();	//获取K线位置控制
private:
	CConfigBase* m_pColorCfg;  // 字体配置文件
	CConfigBase* m_pFontCfg;   // 颜色配置文件
	CConfigBase* m_pSystemCfg; // 系统配置文件
	CConfigBase* m_pDispColumnCfg;    // DispColumn.xml文件
	CConfigBase* m_pReportColumnCfg;  // ReportColumn.xml文件
	CConfigBase* m_pReportTabCfg;     // ReportTab.xml文件
	CConfigBase* m_pServerCfg;        // 通讯配置文件
	CConfigBase* m_pUserDataCfg;      // 运行中用户数据文件
	CConfigBase* m_pKeyboardCfg;      // 键盘精灵配置文件
	CConfigBase* m_pKLineTab;         // 获取K线tab项配置
	CConfigBase* m_pKLinePostionCtrol;// 获取K线tab项配置

};
