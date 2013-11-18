#pragma once

#include "ConfigInterface.h"
#include "quotestuctdefine.h"
#include "datasourcedefineex.h"

#define QUOTETAB_NOTIFY					WM_USER + 200	// 标签页控件通知消息
#define QUOTETAB_SETTING				WM_USER + 201	// 点击标签页设置选项
#define QUOTETAB_ADDTEMP_MENUINDEX		WM_USER + 261	// 添加一个临时菜单对应项

#define ID_MENU_MARKET_BEGIN			100	// 切换市场（板块）菜单
#define ID_MENU_MARKET_END				ID_MENU_MARKET_BEGIN+1000
#define ID_MENU_BLOCK_BEGIN				ID_MENU_MARKET_END+1
#define ID_MENU_BLOCK_END				ID_MENU_BLOCK_BEGIN+1000
#define QUOTETAB_TABGROUPS_BEGIN		ID_MENU_BLOCK_END + 100	// 切换标签页组
#define QUOTETAB_TABGROUPS_END			QUOTETAB_TABGROUPS_BEGIN + 50

#define CQuoteTab_Type_Item				0x00000000	// 普通标签
#define CQuoteTab_Type_Guide			0x10000000	// 向导标签(左侧)
#define CQuoteTab_type_Extre			0x20000000	// 扩展标签(右侧)

#define CQuoteTab_ALIGN_LEFT			0x00000010	// 左
#define CQuoteTab_ALIGN_TOP				0x00000020	// 顶
#define CQuoteTab_ALIGN_RIGHT			0x00000030	// 右
#define CQuoteTab_ALIGN_BOTTOM			0x00000040	// 底

#define CQuoteTab_Show_Auto_TILE		0x00000100	// 标签项平铺

#define TCIS_DISABLE					0x0004		// 标签页按钮不可用

class CQuoteTabCtrl;
struct HsTCItem
{
	UINT	mask;			// 当前标签的属性或功能说明

	CRect   rc;				// 当前标签范围，每次Draw的时候划定范围
	CString pszText;		// 标签内容
	int		iImage;			// 标签图标(置于内容文字之前)
	LPARAM  lParam;			// 标签参数

	int		m_nSubImage;	// 标签子图标(置于内容文字之后)
	int		m_nOtherMask;	// 关闭窗口时，自动删除tab项

	HsTCItem()
	{
		rc.SetRectEmpty();

		mask    = 0;
		iImage  = -1;
		lParam  = NULL;
		m_nSubImage = -1;
		m_nOtherMask = 0;
	}
	BOOL Draw(CDC* pDC,CRect& rect,int nMask,int nWidth = 0,int nTabType = 1);
	virtual BOOL Operator(CWnd* pParentWnd,int nMouse);

	void  Serialize(CArchive& ar,CWnd* pParent);
};

struct HsTCButton:public HsTCItem
{
	enum
	{
		TabPrev			= 0x0400, 
		TabNext			= 0x0800,

		TabTitle		= 0x1000, // 标题
		TabClose		= 0x2000, // 关闭
		TabOpertor		= 0x4000, // 操作(设置)
		TabCode			= 0x8000, // 代码

		TabNoIndexNoQH	= 0x100000,		// 分类不显示指数 不显示期货
		TabMaxWin		= 0x200000,		// 最大化窗口
		TabRestoreWin	= 0x400000,		// 还原窗口
	};
	BOOL DrawButton(CDC* pDC, CRect& rect, int nMask, int nTabType = 1);
	BOOL Operator(CWnd* pParentWnd,int nMouse);
};

struct TabItemData
{
	UINT			m_nID;				// 标签ID
	DWORD			m_dStyle;			// 标签样式
	PageViewInfo	m_PageInfo;			// 创建页面信息
	CString			m_strColGroupName;	// 字段分组名称
	DWORD			m_lMenuID;

	TabItemData()
	{
		m_dStyle = 0;
		m_lMenuID = 0;
	}
	~TabItemData()
	{
	}
};

struct TempMenuIndex		// 临时菜单对应项
{
	UINT m_nID;
	CString m_strMenuText;
};

// CQuoteTabCtrl

class CQuoteTabCtrl : public CWnd
{
	DECLARE_DYNAMIC(CQuoteTabCtrl)

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

public:
	CQuoteTabCtrl(IDataSourceEx* pDataSource, long nDataSourceID);
	virtual ~CQuoteTabCtrl();

	// 根据配置提取的信息添加标签页
	LONG AddTabItem(CString strName, UINT nID, DWORD dStyle = 0, PageViewInfo* pageInfo = NULL, CString strGroupName = _T(""));
	
	// 插入标签项
	LONG InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, int nImage, LPARAM lParam,int nOtherMask = 0);
	// 删除标签项
	BOOL DeleteItem(int nItem);
	// 获取标签项
	HsTCItem* GetItem(int nItem);
	// 获取标签项参数
	LPARAM GetItemParam(int nItem);

	// 插入导航项
	LONG InsertGuideItem(UINT nMask, int nItem, LPCTSTR lpszItem, int nImage, LPARAM lParam,int nOtherMask = 0);

	// 插入右侧按钮
	LONG InsertButtonItem(UINT nMask, int nItem, LPCTSTR lpszItem, int nImage, LPARAM lParam,int nOtherMask = 0);

	BOOL ChangeCurItem(int nIndex, BOOL bNotify = TRUE);
	BOOL ChangeCurItem(CString strName, BOOL bNotify = TRUE, short nMarkType = 0);

	// 根据标签名称查询标签序号
	int FindTabItem(CString strName);

	// 使指定标签页可见
	int EnSureVisible(CString strName);
	int EnSureVisible(int nIndex);
	// 改变当前标签组
	BOOL ChangeTabGroup(CString strGroup = _T(""));
	// 获取默认标签组名称
	CString GetDefaultTabGroupName();

	// 绘制所有标签页
	void DrawTab(CDC *pDC);

	// 是否选中
	int  HitTest(CArray<HsTCItem*,HsTCItem*>& ay,CPoint point);
	int  HitTest(CArray<HsTCButton*,HsTCButton*>& ay,CPoint point);

	// 窗口样式操作
	void  SetStyle(DWORD dStyle)    { m_nStyle = dStyle; } 
	void  AddStyle(DWORD dStyle)    { m_nStyle |= dStyle; }
	void  RemoveStyle(DWORD dStyle) { m_nStyle &= ~dStyle; }
	BOOL  IsStyle(DWORD dStyle)     { return ((m_nStyle & dStyle) == dStyle); }

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CArray<HsTCItem*,HsTCItem*> m_ayGuideItem;   // 向导项
	CArray<HsTCItem*,HsTCItem*> m_ayItem;        // tab 项
	CArray<HsTCButton*,HsTCButton*> m_ayButtonItem;  // 右边按钮项

	HsTCItem	m_LeftArrow;
	HsTCItem	m_RightArrow;

	short		m_nMouseActiveItem;		// 鼠标移动到项的激活
	short		m_nCurButton;			// 当前激活按钮
	short		m_nLeftOffset;			// 偏移的第一项
	short		m_nRightOffset;			// 显示的最后一项

	CString		m_strCurGroupName;			// 当前标签组名称
	long		m_nStyle;
	UINT		m_nTimerID;

	CStringArray m_ayTempGroups;


public:
	short		m_nCurItem;				// 当前激活项

	static IHsTabCfg*		m_iTab;				// 配置模块接口
	static IHsColor*        m_iColor;           // 颜色模块接口
	static IHsFont*			m_iFont;            // 字体模块接口
	static IBlockManager*	m_iBlockManager;	// 板块管理接口

	static IDataSourceEx*		m_pDataSource;				// 数据引擎接口指针
	static long				m_nDataSourceID;			// 数据请求ID

	static CMap<UINT,UINT,CString,CString> m_mapTempMenuIndex;
	// 消息处理函数
public:
	void OnScrollTCItem(int nCount);			// 移动标签页偏移   nCount>0向右移动nCount个  ==0不移动  <0向左移动nCount个
	void OnButtonClickMsg(UINT nID);
	void OnTabSetting();
	void OnGuideItemMarketMenuMsg(UINT nID);
	void OnGuideItemBlockMenuMsg(UINT nID);
	void OnTabGroupChangeMenuMsg(UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


