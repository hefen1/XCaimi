/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDrawQuoteTabCtrl.h
*	文件标识：
*	摘	  要：  行情报价Tab页签
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-04-01
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "DiagramTab.h"
#include "DrawQuoteReportStruct.h"
#include "DataSourceDefineEx.h"
#include "ConfigInterface.h"
#include <afxtempl.h>
#include "QuoteLangMngInterFace.h"

#define CDrawQuoteTab_Type_Item		0x1000	// 普通标签
#define CDrawQuoteTab_Type_Guide	0x2000	// 向导标签
#define CDrawQuoteTab_Type_Button	0x4000	// 按钮标签

#define CDRAWTAB_ID_MENU_MARKET_BEGIN		100	// 切换市场（板块）菜单
#define CDRAWTAB_ID_MENU_MARKET_END			CDRAWTAB_ID_MENU_MARKET_BEGIN+1000 
#define CDRAWTAB_ID_MENU_BLOCK_BEGIN		CDRAWTAB_ID_MENU_MARKET_END+1
#define CDRAWTAB_ID_MENU_BLOCK_END			CDRAWTAB_ID_MENU_BLOCK_BEGIN+1000

#define CDRAWTAB_QUOTETAB_SETTING		    CDRAWTAB_ID_MENU_BLOCK_END + 1	// 点击标签页设置选项
#define CDRAWTAB_QUOTETAB_TABGROUPS_BEGIN   CDRAWTAB_QUOTETAB_SETTING + 1	// 切换标签页组
#define CDRAWTAB_QUOTETAB_TABGROUPS_END	    CDRAWTAB_QUOTETAB_TABGROUPS_BEGIN + 50

class CDrawQuoteTabCtrl;
class CDrawQuoteTab
{
	friend CDrawQuoteTabCtrl;
public:	
	CDrawQuoteTab();
	~CDrawQuoteTab();
	enum
	{
		Tab_Pressed = 0x0001,
		Tab_Disabled = 0x0002,
	};

	BOOL DrawGuide(CDC* pDC, CRect& rect);
	BOOL DrawItem(CDC* pDC, CRect& rect, BOOL bPressed);
	BOOL DrawButton(CDC* pDC, CRect& rect, short nMask);

	PageInfo* GetPageInfo() {return &m_PageInfo;}
	CString   GetGroupName() {return m_strColGroupName;}
protected:	
	CString         m_strName;          // 名称
	UINT			m_nID;				// 标签ID
	long            m_lMenuMask;        // 属性或功能说明
	PageInfo    	m_PageInfo;			// 创建页面信息
	CString			m_strColGroupName;	// 字段分组名称
	DWORD			m_lMenuID;          // 菜单ID

	BOOL            m_bVisable;         // 是否可见 
	CRect           m_rcTab;            // 当前分配的区域

private:
	static IHsColor*  m_pColor;         // 颜色配置文件指针
	static IHsFont*   m_pFont;          // 字体配置文件指针
};

//////////////////////////////////////////////////////////////////////////

interface IDrawQuoteTabNotify
{
public:
	virtual void  OnActiveTabChange(CDrawQuoteTab* pActiveItem) = 0;
	virtual void  TabInvalidate(CRect* pRect, BOOL bErase = TRUE) = 0;
	virtual CWnd* TabGetCWnd() = 0;
};

//////////////////////////////////////////////////////////////////////////

typedef CTypedPtrArray<CPtrArray, CDrawQuoteTab*> CDrawQuoteTabItemArray;

class CDrawQuoteTabCtrl
{
public:
	// 功能按钮名
	enum 
	{ 
		TabFenLei  = 0x0001, // 沪深股票
		TabGangGu  = 0x0002, // 港股
		TabWaiPan  = 0x0004, // 期货
		TabQiHuo   = 0x0008, // 外盘
		TabWaiHui  = 0x0010, // 外汇

		TabBlock   = 0x0020, // 板块
		TabUsrDef  = 0x0040, // 自选股
		TabSort    = 0x0080, // 排名

		TabUsrDefine = 0x0100, // 自定义分组弹出菜单,也就是当前弹出内容来自定义
		TabBourse    = 0x0200, // 自定义分组弹出菜单,指向一个市场

		//HsTCButton
		TabPrev = 0x0400, 
		TabNext = 0x0800,

		TabTitle   = 0x1000, // 标题
		TabClose   = 0x2000, // 关闭
		TabOpertor = 0x4000, // 操作(设置)
		TabCode    = 0x8000, // 代码

		TabNoIndexNoQH = 0x100000,		// 分类不显示指数 不显示期货
		TabMaxWin      = 0x200000,		// 最大化窗口
		TabRestoreWin  = 0x400000,		// 还原窗口
		//
		TabF10HideRight = 0x1000000,

		TabHide        = 0x80000000,    // 当前tab隐藏
	};

	CDrawQuoteTabCtrl();
	~CDrawQuoteTabCtrl(void);

	void Create();           // 初始化函数
	void SetRect(CRect rc);  // 设置大小

	BOOL ChangeTabGroup(CString strGroup = _T(""), BOOL bForce = FALSE);	// 改变当前标签组
	CString GetDefaultTabGroupName();	            // 获取默认标签组名称
	// 根据配置提取的信息添加标签页
	LONG AddTabItem(CString strName, UINT nID, long lMenuMask = 0, PageInfo* pageInfo = NULL, CString strGroupName = _T(""));
	int  FindTabItem(CString strName);  // 根据标签名称查询标签序号
	// 改变当前标签页
	BOOL ChangeCurItem(CString strName, BOOL bNotify = TRUE, short nMarkType = 0);
	BOOL ChangeCurItem(int nIndex, BOOL bNotify = TRUE); 	
	// 使指定标签页可见
	int EnSureVisible(CString strName);
	int EnSureVisible(int nIndex);

	void OnScrollTCItem(int nCount); // 移动标签页偏移   nCount>0向右移动nCount个  ==0不移动  <0向左移动nCount个
	BOOL RemoveTabItem(int nItem);   // 删除tab页签
	
	void DrawTab(CDC *pDC); // 绘制所有标签页
	BOOL MouseMove(UINT nFlags, CPoint point);
	BOOL MouseDown(UINT nFlags, CPoint point);

	// 是否选中
	int  HitTest(CPoint point, int& nIndex);

	void SetNotify(IDrawQuoteTabNotify* pDrawTabNotify){m_pDrawTabNotify = pDrawTabNotify;}
protected:
	int  OpenBlockMenu(CPoint point, BOOL bSysBlock);              // 打开板块菜单
	int  OpenSecondBlockMenu(CString strMarketName, CPoint point); // 打开二次板块市场菜单
	int  OpenAllMarketMenu(CPoint point);                      // 打开全市场菜单
	int  OpenSettingMenu(CPoint point);                        // 打开设置菜单

	IDataSourceEx*          m_pDataSource;      // 数据引擎指针
	IDrawQuoteTabNotify*    m_pDrawTabNotify;   // 回调函数指针 

	CString                 m_strCurGroupName;  // 当前标签组
	CDrawQuoteTabItemArray  m_ayGuideItem;      // 导航页
	CDrawQuoteTabItemArray  m_ayItem;           // 标签页
	CDrawQuoteTabItemArray  m_ayButtonItem;     // 按钮页
	
	CRect                   m_rcClient;         // 区域大小

	short		            m_nMouseActiveItem;	// 鼠标移动到项的激活
	short		            m_nCurItem;	        // 当前激活项
	short                   m_nCurButton;       // 当前激活按钮
	short		            m_nLeftOffset;	    // 偏移的第一项
	short		            m_nRightOffset;	    // 显示的最后一项

	CStringArray            m_ayTempGroups;     // 存放显示组

	static IHsColor*        m_pColor;           // 颜色配置接口
	static IHsTabCfg*		m_iTab;				// 配置模块接口
	static IBlockManager*	m_iBlockManager;	// 板块管理接口
	static IQuoteLang*	    m_iQuoteLang;		// 行情资源接口
	static CMap<UINT,UINT,CString,CString> m_mapTempMenuIndex; // 存放菜单ID对应的市场或者板块名称
};