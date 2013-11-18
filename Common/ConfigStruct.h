#pragma once
#include "NetEngineInterface.h"
#include "hsstructnew.h"
#pragma pack(1) 
#define		BLOCK_NAME_LENGTH		32 
#define		MARKETTYPE_COUNT		12		// ������ϸ���
#define		MARK_COUNT				10		// ��Ʊ�������

#define		TAB_ALLMARKET_MENUMASK	0x10000	// �����򵼱�ǩҳ��ȫ�����Ĳ˵�����
#define		TAB_BLOCK_MENUMASK		0x10001	// �����򵼱�ǩҳ ���Ĳ˵����룬����˵����ݷ������Ʋ���

// ����ģ���ļ�ʹ�õ���ͷ�������ƣ���ӦHQ_BASE_NAME��
#define CONFIG_HQ_BASE_NAME				_T("StockQuote")			//_T("��Ʊ�����ֶ�")
#define CONFIG_HQ_EX_NAME				_T("StockQuoteEx")			//_T("��Ʊ��չ����")
#define CONFIG_HQ_DETAIL_NAME			_T("DetailGrid")			//_T("��ϸ��")
#define CONFIG_EQUITY_DATA_NAME			_T("EquityData")			//_T("�ɱ�����")
#define CONFIG_FINANCIAL_DATA_NAME		_T("FinancialData")			//_T("��������")
#define CONFIG_RELATIONAL_ANALYSIS_NAME	_T("RelationalAnalysis")	//_T("��������")
#define CONFIG_USER_DEF_NAME			_T("UserDef")				//_T("�Զ���")
#define CONFIG_ORIGINAL_USE_NAME		_T("OriginalUsed")			//_T("�ڲ�ʹ��")
#define CONFIG_FORM_ANALYSIS_NAME		_T("FormAnalysis")			//_T("�������")
#define CONFIG_ZIXUN_NAME				_T("ZiXun")					//_T("��Ѷ/EMAIL�ֶ�")
#define CONFIG_FUTURE_HQ_NAME			_T("FutureQuote")			//_T("�ڻ�����")
#define CONFIG_FINANCIAL_ANALYSIS_NAME	_T("FiancilaAnalysis")		//_T("�������")
#define CONFIG_BLOCK_NAME				_T("Block")					//_T("�����Ϣ")

struct TabItem
{			
	char				m_szName[64];
	long				m_ID;
	long				m_lPageType;
	long				m_lPageTag;
	long				m_lMenuMask;
	long				m_lMenuID;
	HSMarketDataType	m_sMarket[MARKETTYPE_COUNT];	// �������
	char				m_szBlock[BLOCK_NAME_LENGTH]; // �������
	char				m_szColGroupName[BLOCK_NAME_LENGTH]; //��ͷ������

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

struct ColorProperty  // ��ɫ����
{
	short     m_nID;	   // ��ʶ
	char	  m_szName[20];/*������*/
	COLORREF  m_lColor;	   /*����ɫ*/
	int		  m_nPenStyle; /*�߷��*/
	int		  m_nWidth;	   /*�߿��*/


	~ColorProperty()
	{

	}
	ColorProperty()
	{
		m_nID       = -1;   // ��ʶ
		m_lColor    = RGB(255,255,255);	/*����ɫ*/
		m_nPenStyle = 0;	/*�߷��*/
		m_nWidth    = 0;	/*�߿��*/	
		memset(m_szName, 0, sizeof(m_szName));
	}
	ColorProperty(COLORREF lColor,int nPenStyle = -1,int nWidth = 0)
	{
		m_lColor    = lColor;
		m_nPenStyle = nPenStyle;
		m_nWidth    = nWidth;
	}
	ColorProperty(short nID,COLORREF lColor,CString strName,
		int nPenStyle = -1, /*�߷��*/
		int nWidth = 0	   /*�߿��*/)
	{
		m_nID       = nID;
		m_lColor    = lColor;
		m_nPenStyle = nPenStyle;
		m_nWidth    = nWidth;
		sprintf_s(m_szName, "%s", strName);
	}
	void Set(short nID,COLORREF lColor,CString strName,			 
		CArray<ColorProperty*,ColorProperty*>* payColorProperty = NULL,
		int nPenStyle = -1, /*�߷��*/
		int nWidth = 0	   /*�߿��*/)
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

struct FontProperty // ��������
{
	short     m_nID;		// ��ʶ
	CFont*    m_pFont;		// ����
	CString   m_strName;    // ��Ӧ������

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
	//�ͷ������ڴ�ռ�
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
	UrsDword : ����1,����ϵͳ��������ɫ������,�ײ�checkbox�ĸ���Ŀ
	UrsString: ����2,����ϵͳ�����в�������1,��������2�ȸ�����Ŀ
	*/
	enum {UrsDword = 1,UrsString};

	CString   m_strName;	//�������ı� , ����ʾ�ڶ�Ӧ����Ŀ
	short     m_nID;		//ID,��־,����ŵ�����
	char      m_cType;		//����,ȡֵΪUrsDword����UrsString
	DWORD     m_dwKey;		//�ؼ���ֵ,����UrsString����,�����ڴ��ڵĿؼ�ID
	CString   m_strNewValue;//ֵ
	//����1
	SettingProperty() 
	{
		m_nID   = -1;
		m_cType = -1;		
	}
	//����2
	SettingProperty(short nID,CString strName,DWORD dwKey,
		CArray<SettingProperty*,SettingProperty*>* paySettingProperty = NULL,
		char cType = UrsDword,
		CString strDefault = "",
		CMap<int,int,SettingProperty*,SettingProperty*>* pmapSettingProperty = NULL);
};

struct TableColProperty
{
	char		strName[32];	// �ֶ�����,��ʾ����ͷ
	UINT		m_nCellMask;    // �е�Ԫ������
//	UINT		m_nItemMask;	// �е�ԪС������
//	UINT        m_nColorID;     // ��ɫID
	UINT		m_ColumnID;		// ��ID
	UINT		m_nWidth;		// Ĭ���п��

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


//�����ı����Խṹ,Ŀǰ������ʾ���۷���������ʵʱ�仯����
struct RefreshDispTextProp
{
	COLORREF m_dwBkColor;	//������ɫָ��
	COLORREF m_dwFgColor;	//������ɫָ��
	CFont*  m_pFont;		//����ָ��

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

//Xml�ڵ�ṹ
struct  HsXmlItem
{
	CString strItemText;
	CString strItemName;
	CMapStringToString mAttributelist;
	CList <HsXmlItem *> m_ChildList;

	CString GetAttribute(const char *pName);
};

// �����ǽṹ
struct HsCodeMark
{
	int			m_nType;		// �������(-1Ϊ���ֱ������)
	char		m_szIndex[BLOCK_NAME_LENGTH];		// ��������ַ���(��Ʊ�г�+���� �� ������� ��)
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
// ������ṹ
struct BlockGroup
{
	CString strGroupName;
	CArray<BlockInfo*,BlockInfo*> ayBlockList;
};
typedef CArray<BlockGroup*, BlockGroup*> CBlockGroupArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, BlockInfo*> CBlockInfoMap;

//////////////////////////////////////////////////////////////////////////
// ��������tab�ṹ

// ָ��
struct TechIndex
{
	char    m_szName[32];
	char    m_szIndexName[32];
	int     m_nIndexType;
	int     m_nLineType;
};
typedef CArray<TechIndex*, TechIndex*> CTechIndexArray;

// ģ��
struct TechTemplete
{
	char    m_szName[32];
	char    m_szDefaultIndex[64];
	double  m_dRaito;
};
typedef CArray<TechTemplete*, TechTemplete*> CTechTempleteArray;
//////////////////////////////////////////////////////////////////////////
#pragma pack()