#pragma once
#include "NetEngineInterface.h"
#include "hsstructnew.h"
#pragma pack(1) 
#define		BLOCK_NAME_LENGTH		32 
#define		MARKETTYPE_COUNT		12		// 类型组合个数
#define		MARK_COUNT				10		// 股票标记种类

#define		TAB_ALLMARKET_MENUMASK	0x10000	// 行情向导标签页“全部”的菜单掩码
#define		TAB_BLOCK_MENUMASK		0x10001	// 行情向导标签页 板块的菜单掩码，具体菜单根据分类名称查找

// 配置模块文件使用的列头分组名称，对应HQ_BASE_NAME等
#define CONFIG_HQ_BASE_NAME				_T("StockQuote")			//_T("股票行情字段")
#define CONFIG_HQ_EX_NAME				_T("StockQuoteEx")			//_T("股票扩展行情")
#define CONFIG_HQ_DETAIL_NAME			_T("DetailGrid")			//_T("明细表")
#define CONFIG_EQUITY_DATA_NAME			_T("EquityData")			//_T("股本数据")
#define CONFIG_FINANCIAL_DATA_NAME		_T("FinancialData")			//_T("财务数据")
#define CONFIG_RELATIONAL_ANALYSIS_NAME	_T("RelationalAnalysis")	//_T("联动分析")
#define CONFIG_USER_DEF_NAME			_T("UserDef")				//_T("自定义")
#define CONFIG_ORIGINAL_USE_NAME		_T("OriginalUsed")			//_T("内部使用")
#define CONFIG_FORM_ANALYSIS_NAME		_T("FormAnalysis")			//_T("报表分析")
#define CONFIG_ZIXUN_NAME				_T("ZiXun")					//_T("资讯/EMAIL字段")
#define CONFIG_FUTURE_HQ_NAME			_T("FutureQuote")			//_T("期货行情")
#define CONFIG_FINANCIAL_ANALYSIS_NAME	_T("FiancilaAnalysis")		//_T("财务分析")
#define CONFIG_BLOCK_NAME				_T("Block")					//_T("板块信息")

struct TabItem
{			
	char				m_szName[64];
	long				m_ID;
	long				m_lPageType;
	long				m_lPageTag;
	long				m_lMenuMask;
	long				m_lMenuID;
	HSMarketDataType	m_sMarket[MARKETTYPE_COUNT];	// 类型组合
	char				m_szBlock[BLOCK_NAME_LENGTH]; // 板块名称
	char				m_szColGroupName[BLOCK_NAME_LENGTH]; //列头组名称

	TabItem()
	{
		memset(m_szName,0,64);
		m_ID = 0;
		m_lPageType = 0;
		m_lPageTag = 0;
		m_lMenuMask = 0;
		m_lMenuID = 0;
		for (int i=0; i<MARKETTYPE_COUNT; i++)
		{
			m_sMarket[i] = 0;
		}
		memset(m_szBlock,0,BLOCK_NAME_LENGTH);
		memset(m_szColGroupName,0,BLOCK_NAME_LENGTH);
	}
};
typedef CTypedPtrMap<CMapWordToPtr,UINT,TabItem*> CTabItemIDIndex;
typedef CTypedPtrMap<CMapStringToPtr,CString,TabItem*> CTabItemNameIndex;
typedef CArray<TabItem*,TabItem*> CTabItemArray;
typedef CTypedPtrMap<CMapStringToPtr,CString,CTabItemArray*> CTabGroupMap;
typedef CArray<CommInfo*, CommInfo*> CCommInfoArray;

struct ColorProperty  // 颜色属性
{
	short     m_nID;	   // 标识
	char	  m_szName[20];/*线名称*/
	COLORREF  m_lColor;	   /*线颜色*/
	int		  m_nPenStyle; /*线风格*/
	int		  m_nWidth;	   /*线宽度*/


	~ColorProperty()
	{

	}
	ColorProperty()
	{
		m_nID       = -1;   // 标识
		m_lColor    = RGB(255,255,255);	/*线颜色*/
		m_nPenStyle = 0;	/*线风格*/
		m_nWidth    = 0;	/*线宽度*/	
		memset(m_szName, 0, sizeof(m_szName));
	}
	ColorProperty(COLORREF lColor,int nPenStyle = -1,int nWidth = 0)
	{
		m_lColor    = lColor;
		m_nPenStyle = nPenStyle;
		m_nWidth    = nWidth;
	}
	ColorProperty(short nID,COLORREF lColor,CString strName,
		int nPenStyle = -1, /*线风格*/
		int nWidth = 0	   /*线宽度*/)
	{
		m_nID       = nID;
		m_lColor    = lColor;
		m_nPenStyle = nPenStyle;
		m_nWidth    = nWidth;
		sprintf_s(m_szName, "%s", strName);
	}
	void Set(short nID,COLORREF lColor,CString strName,			 
		CArray<ColorProperty*,ColorProperty*>* payColorProperty = NULL,
		int nPenStyle = -1, /*线风格*/
		int nWidth = 0	   /*线宽度*/)
	{
		m_nID       = nID;
		m_lColor    = lColor;
		m_nPenStyle = nPenStyle;
		m_nWidth    = nWidth;
		strncpy(m_szName, strName, min(sizeof(m_szName), strName.GetLength()));
		if(payColorProperty != NULL)
		{
			payColorProperty->Add(this);
		}
	}
	operator COLORREF() { return m_lColor; }
	COLORREF GetColor() { return m_lColor; }

	const ColorProperty& operator=(const ColorProperty& colorSrc)
	{
		m_nID       = colorSrc.m_nID;      
		m_lColor    = colorSrc.m_lColor;   
		m_nPenStyle = colorSrc.m_nPenStyle;
		m_nWidth    = colorSrc.m_nWidth;   
		sprintf(m_szName, "%s", colorSrc.m_szName);
		return *this; 
	}
	const ColorProperty& operator=(const ColorProperty* pColorSrc)
	{
		if( pColorSrc == NULL ) 
		{
			m_nID       = -1;      
			m_lColor    = RGB(0,255,255);
			m_nPenStyle = 1;
			m_nWidth    = 1;
		}
		else
		{
			m_nID       = pColorSrc->m_nID;      
			m_lColor    = pColorSrc->m_lColor;   
			m_nPenStyle = pColorSrc->m_nPenStyle;
			m_nWidth    = pColorSrc->m_nWidth;   
			sprintf(m_szName, "%s", pColorSrc->m_szName);
		}

		return *this; 
	}
	COLORREF GetInvertColor()
	{
		BYTE r,g,b;
		r = GetRValue(m_lColor);
		r = ~r;
		g = GetGValue(m_lColor);
		g = ~g;
		b = GetBValue(m_lColor);
		b = ~b;
		return RGB(r,g,b);
	}
};

struct FontProperty // 字体属性
{
	short     m_nID;		// 标识
	CFont*    m_pFont;		// 字体
	CString   m_strName;    // 对应的名字

	FontProperty()
	{
		m_nID = -1;
		m_pFont = NULL;
	}
	FontProperty(short nID,CFont* pFont,CString strName)
	{
		m_nID     = nID;
		m_pFont   = pFont;
		m_strName.Format(strName);
	}
	~FontProperty()
	{
		Free();
	}
	//释放字体内存空间
	void Free()
	{
		if(m_pFont != NULL)
		{
			delete m_pFont;
		//	m_pFont->DeleteObject();
			m_pFont = NULL;
		}
	}
	void Set(short nID,CFont* pFont,CString strName,
		CArray<FontProperty*,FontProperty*>* payFontProperty = NULL)
	{
		m_nID     = nID;
		m_pFont   = pFont;
		m_strName.Format(strName);
		if(payFontProperty != NULL)
		{
			payFontProperty->Add(this);
		}
	}
	CFont*   GetFont()
	{
		return m_pFont;
	}

};

struct SettingProperty 
{
	/*
	UrsDword : 类型1,用于系统设置中颜色设置中,底部checkbox的各项目
	UrsString: 类型2,用于系统设置中参数设置1,参数设置2等各个项目
	*/
	enum {UrsDword = 1,UrsString};

	CString   m_strName;	//描述的文本 , 将显示在对应的项目
	short     m_nID;		//ID,标志,起到序号的作用
	char      m_cType;		//类型,取值为UrsDword或者UrsString
	DWORD     m_dwKey;		//关键键值,例如UrsString类型,将对于窗口的控件ID
	CString   m_strNewValue;//值
	//构造1
	SettingProperty() 
	{
		m_nID   = -1;
		m_cType = -1;		
	}
	//构造2
	SettingProperty(short nID,CString strName,DWORD dwKey,
		CArray<SettingProperty*,SettingProperty*>* paySettingProperty = NULL,
		char cType = UrsDword,
		CString strDefault = "",
		CMap<int,int,SettingProperty*,SettingProperty*>* pmapSettingProperty = NULL);
};

struct TableColProperty
{
	char		strName[32];	// 字段名称,显示在列头
	UINT		m_nCellMask;    // 列单元格属性
//	UINT		m_nItemMask;	// 列单元小格属性
//	UINT        m_nColorID;     // 颜色ID
	UINT		m_ColumnID;		// 列ID
	UINT		m_nWidth;		// 默认列宽度

	TableColProperty()
	{
		memset(this,0,sizeof(TableColProperty));
	}

	TableColProperty(const char *pName, UINT nCellMask = -1, UINT nColumnID = -1, UINT nWidth = 0)
	{
		// name
		memcpy(strName, pName, min(strlen(pName)+1, sizeof(strName)-1));
		m_nCellMask = nCellMask;
		m_ColumnID = nColumnID;
		m_nWidth = nWidth;
	}
};
typedef CArray<TableColProperty*, TableColProperty*> CColPropArray;
typedef CTypedPtrMap<CMapStringToPtr,CString,CColPropArray*> CColPropGroupMap;
typedef CTypedPtrMap<CMapStringToPtr,CString,CArray<CStringArray*,CStringArray*>*> CColTemplGroupMap;

typedef CTypedPtrMap<CMapWordToPtr,UINT, TableColProperty*> CColPropIDIndex;
typedef CTypedPtrMap<CMapStringToPtr,CString,TableColProperty*> CColPropNameIndex;


//高亮文本属性结构,目前用于显示报价分析中主推实时变化数据
struct RefreshDispTextProp
{
	COLORREF m_dwBkColor;	//背景颜色指针
	COLORREF m_dwFgColor;	//字体颜色指针
	CFont*  m_pFont;		//字体指针

	RefreshDispTextProp()
	{
		m_dwBkColor = 0;
		m_dwFgColor = 0;
		m_pFont = NULL;
	}

	RefreshDispTextProp(COLORREF dwBkColor,COLORREF dwFgColor,CFont* pFont)
	{
		m_dwBkColor = dwBkColor;
		m_dwBkColor = dwFgColor;
		m_pFont    = pFont;
	}
};

//Xml节点结构
struct  HsXmlItem
{
	CString strItemText;
	CString strItemName;
	CMapStringToString mAttributelist;
	CList <HsXmlItem *> m_ChildList;

	CString GetAttribute(const char *pName);
};

// 代码标记结构
struct HsCodeMark
{
	int			m_nType;		// 标记类型(-1为文字标记内容)
	char		m_szIndex[BLOCK_NAME_LENGTH];		// 标记索引字符串(股票市场+代码 或 板块名称 等)
	CString		m_strContent;
	
	HsCodeMark()
	{
		m_nType = 0;
		memset(m_szIndex,0,sizeof(BLOCK_NAME_LENGTH));
	}

	~HsCodeMark()
	{
	}

	const HsCodeMark& operator=(const HsCodeMark* pMark)
	{
		if( pMark == NULL ) 
		{
			m_nType       = 0;      
			memset(m_szIndex,0,sizeof(BLOCK_NAME_LENGTH));
			m_strContent.Empty();
		}
		else
		{
			m_nType = pMark->m_nType;
			strncpy(m_szIndex,pMark->m_szIndex,BLOCK_NAME_LENGTH);
			m_strContent = pMark->m_strContent;
		}

		return *this; 
	}

};

typedef CArray<HsCodeMark*,HsCodeMark*> CCodeMarkArray;
typedef CTypedPtrMap<CMapStringToPtr,CString,HsCodeMark*> CCodeMarkMap;


struct BlockInfo
{
	CString strBlockName;
	CArray<CodeInfo*,CodeInfo*> ayStockList;
};
// 板块分类结构
struct BlockGroup
{
	CString strGroupName;
	CArray<BlockInfo*,BlockInfo*> ayBlockList;
};
typedef CArray<BlockGroup*, BlockGroup*> CBlockGroupArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, BlockInfo*> CBlockInfoMap;

//////////////////////////////////////////////////////////////////////////
// 技术分析tab结构

// 指标
struct TechIndex
{
	char    m_szName[32];
	char    m_szIndexName[32];
	int     m_nIndexType;
	int     m_nLineType;
};
typedef CArray<TechIndex*, TechIndex*> CTechIndexArray;

// 模板
struct TechTemplete
{
	char    m_szName[32];
	char    m_szDefaultIndex[64];
	double  m_dRaito;
};
typedef CArray<TechTemplete*, TechTemplete*> CTechTempleteArray;
//////////////////////////////////////////////////////////////////////////
#pragma pack()