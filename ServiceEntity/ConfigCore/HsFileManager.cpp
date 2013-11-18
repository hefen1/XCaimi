#include "StdAfx.h"
#include "HsFileManager.h"
#include "..\..\Resource\QuoteLang\Resource.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CHsFileManager::CHsFileManager()
{
	m_pColorCfg = NULL;  // 字体文件
	m_pFontCfg = NULL;   // 颜色文件
	m_pSystemCfg = NULL; // 系统文件
	m_pDispColumnCfg = NULL;    // DispColumn.xml文件
	m_pReportColumnCfg = NULL;  // ReportColumn.xml文件
	m_pReportTabCfg = NULL;     // ReportTab.xml文件
	m_pServerCfg = NULL;        // 通讯配置文件
	m_pUserDataCfg = NULL;      // 获取运行中用户数据文件指针 
	m_pKeyboardCfg = NULL;      // 键盘精灵配置文件
	m_pKLineTab = NULL;         // 获取K线tab项配置
	m_pKLinePostionCtrol = NULL;// 获取K线tab项配置
}

CHsFileManager::~CHsFileManager()
{
	if (m_pColorCfg)
	{
		delete m_pColorCfg;
		m_pColorCfg = NULL;
	}

	if (m_pFontCfg)
	{
		delete m_pFontCfg;
		m_pFontCfg = NULL;
	}

	if (m_pSystemCfg)
	{
		delete m_pSystemCfg;
		m_pSystemCfg = NULL;
	}

	if (m_pDispColumnCfg)
	{
		delete m_pDispColumnCfg;
		m_pDispColumnCfg = NULL;
	}

	if (m_pReportColumnCfg)
	{
		delete m_pReportColumnCfg;
		m_pReportColumnCfg = NULL;
	}

	if (m_pReportTabCfg)
	{
		delete m_pReportTabCfg;
		m_pReportTabCfg = NULL;
	}

	if (m_pServerCfg)
	{
		delete m_pServerCfg;
		m_pServerCfg = NULL;
	}

	if (m_pUserDataCfg)
	{
		delete m_pUserDataCfg;
		m_pUserDataCfg = NULL;
	}
	if ( m_pKeyboardCfg )
	{
		delete m_pKeyboardCfg;
		m_pKeyboardCfg = NULL;
	}
	if (m_pKLineTab)
	{
		delete m_pKLineTab;
		m_pKLineTab = NULL;
	}
	if (m_pKLinePostionCtrol != NULL)
	{
		delete m_pKLinePostionCtrol;
		m_pKLinePostionCtrol = NULL;
	}
}

CConfigBase* CHsFileManager::GetColorCfg()
{
	if (m_pColorCfg == NULL)
	{
		m_pColorCfg = new CConfigBase;
	}
	if (m_pColorCfg->IsInit() == FALSE)
	{
		CString strFile = CGeneralHelper::GetAppPath() + _T("config\\ColorFont\\HsColor.xml");
		CString strUserFile = CGeneralHelper::GetAppPath() + _T("config\\ColorFont\\HsColorUser.xml");
		int nRcID = IDR_COLOR;
		m_pColorCfg->Init(nRcID, strFile, strUserFile);
	}
	return m_pColorCfg;
}

CConfigBase* CHsFileManager::GetFontCfg()
{
	if (m_pFontCfg == NULL)
	{
		m_pFontCfg = new CConfigBase;
	}
	if (m_pFontCfg->IsInit() == FALSE)
	{
		CString strFile = CGeneralHelper::GetAppPath() + _T("config\\ColorFont\\HsFont.xml");
		CString strUserFile = CGeneralHelper::GetAppPath() + _T("config\\ColorFont\\HsFontUser.xml");
		int nRcID = IDR_FONT;
		m_pFontCfg->Init(nRcID, strFile, strUserFile);
	}
	return m_pFontCfg;
}

CConfigBase* CHsFileManager::GetSystemCfg()
{
	if (m_pSystemCfg == NULL)
	{
		m_pSystemCfg = new CConfigBase;
	}
	if (m_pSystemCfg->IsInit() == FALSE)
	{
		CString strFile = CGeneralHelper::GetAppPath() + _T("config\\HsSystem.xml");
		CString strUserFile = CGeneralHelper::GetAppPath() + _T("config\\HsSystemUser.xml");
		int nRcID = IDR_SYSTEM;
		m_pSystemCfg->Init(nRcID, strFile, strUserFile);
	}
	return m_pSystemCfg;
}

CConfigBase* CHsFileManager::GetDispColumnCfg()
{
	if (m_pDispColumnCfg == NULL)
	{
		m_pDispColumnCfg = new CConfigBase;
	}
	if (m_pDispColumnCfg->IsInit() == FALSE)
	{
		CString strFile = CGeneralHelper::GetAppPath() + _T("config\\DispColumn.xml");
		CString strUserFile = CGeneralHelper::GetAppPath() + _T("config\\DispColumnUser.xml");
		int nRcID = IDR_DispColumn;
		m_pDispColumnCfg->Init(nRcID, strFile, strUserFile);
	}
	return m_pDispColumnCfg;
}

CConfigBase* CHsFileManager::GetReportColumnCfg()
{
	if (m_pReportColumnCfg == NULL)
	{
		m_pReportColumnCfg = new CConfigBase;
	}
	if (m_pReportColumnCfg->IsInit() == FALSE)
	{
		CString strFile = CGeneralHelper::GetAppPath() + _T("config\\ReportColumn.xml");
		CString strUserFile = CGeneralHelper::GetAppPath() + _T("config\\ReportColumnUser.xml");
		int nRcID = IDR_ReportColunm;
		m_pReportColumnCfg->Init(nRcID, strFile, strUserFile);
	}
	return m_pReportColumnCfg;
}

CConfigBase* CHsFileManager::GetReportTabCfg()
{
	if (m_pReportTabCfg == NULL)
	{
		m_pReportTabCfg = new CConfigBase;
	}
	if (m_pReportTabCfg->IsInit() == FALSE)
	{
		CString strFile = CGeneralHelper::GetAppPath() + _T("config\\ReportTab.xml");
		CString strUserFile = CGeneralHelper::GetAppPath() + _T("config\\ReportTabUser.xml");
		int nRcID = IDR_ReportTab;
		m_pReportTabCfg->Init(nRcID, strFile, strUserFile);
	}
	return m_pReportTabCfg;
}

CConfigBase* CHsFileManager::GetServerCfg()
{
	if (m_pServerCfg == NULL)
	{
		m_pServerCfg = new CConfigBase;
	}
	if (m_pServerCfg->IsInit() == FALSE)
	{
		CString strFile = CGeneralHelper::GetAppPath() + _T("config\\Server.xml");
		CString strUserFile = CGeneralHelper::GetAppPath() + _T("config\\ServerUser.xml");
		int nRcID = IDR_SERVER;
		m_pServerCfg->Init(nRcID, strFile, strUserFile);
	}
	return m_pServerCfg;
}

CConfigBase* CHsFileManager::GetUserDataCfg()
{
	if (m_pUserDataCfg == NULL)
	{
		m_pUserDataCfg = new CConfigBase;
	}
	if (m_pUserDataCfg->IsInit() == FALSE)
	{
		CString strUserFile = CGeneralHelper::GetAppPath() + _T("config\\HsCodeMarkUser.xml");
		m_pUserDataCfg->Init(0, "", strUserFile);
	}
	return m_pUserDataCfg;
}

CConfigBase* CHsFileManager::GetKeyboardCfg()
{
	if (m_pKeyboardCfg == NULL)
	{
		m_pKeyboardCfg = new CConfigBase;
	}
	if (m_pKeyboardCfg->IsInit() == FALSE)
	{
		CString strFile = CGeneralHelper::GetAppPath() + _T("config\\Keyboard.xml");
		m_pKeyboardCfg->Init(IDR_Keyboard, strFile, "");
	}
	return m_pKeyboardCfg;
}

CConfigBase* CHsFileManager::GetKLineTab()
{
	if (m_pKLineTab == NULL)
	{
		m_pKLineTab = new CConfigBase;
	}
	if (m_pKLineTab->IsInit() == FALSE)
	{
		CString strFile = CGeneralHelper::GetAppPath() + _T("config\\KLineTabLayout.xml");
		m_pKLineTab->Init(IDR_KLineTab, strFile, "");
	}
	return m_pKLineTab;
}

CConfigBase* CHsFileManager::GetKLinePostionCtrl()
{
	if (m_pKLinePostionCtrol == NULL)
	{
		m_pKLinePostionCtrol = new CConfigBase;
	}
	if (!m_pKLinePostionCtrol->IsInit())
	{
		CString strUserFile = CGeneralHelper::GetAppPath() + _T("config\\KLinePositionLayout.xml");
// 		if (_access(strUserFile, 0) != 0)
// 		{
// 			delete m_pKLinePostionCtrol;
// 			m_pKLinePostionCtrol = NULL;
// 			return NULL;
// 		}
		m_pKLinePostionCtrol->Init(0, "", strUserFile);
	}
	return m_pKLinePostionCtrol;
}