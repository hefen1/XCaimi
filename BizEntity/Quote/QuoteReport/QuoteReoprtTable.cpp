// QuoteReoprtTable.cpp : 实现文件
//

#include "stdafx.h"
#include "QuoteReport.h"
#include "QuoteReoprtTable.h"
#include "..\common\hscoldefine.h"
#include "..\common\hsothercoldefine.h"
#include "..\common\pageid.h"
#include <sys/stat.h>
#include "quotesort.h"
#include "../HelperLib/GeneralHelper.h"
#include "../Common/Memdc.h"

UINT ROWMASK_DEFAULT_TMP = 0;
UINT* GetTmpMask(UINT mask)
{
	ROWMASK_DEFAULT_TMP = mask;
	return &ROWMASK_DEFAULT_TMP;
}

BOOL IsMarket(HSMarketDataType x,HSMarketDataType y) 
{ 
	return (MakeMarket(x) == MakeMarket(y)); 
}

//for test
StockUserInfo* g_pStock = NULL;
const StockUserInfo* GetStockUserInfo(PageViewInfo* pvInfo, 
									  int& nCount, 
									  CArray<StockUserInfo*,StockUserInfo*>* payStock /*= NULL*/,
									  int* pCurRow /*= NULL*/)
{
	if (payStock == NULL)
	{
		nCount = 7;
		return g_pStock;
	} 

	StockUserInfo* pTemp = NULL;
	if (!g_pStock || IsBadReadPtr(g_pStock,1))
	{
		pTemp = new StockUserInfo;
		g_pStock = pTemp;
		strcpy(pTemp->m_cStockName,_T("浦发银行"));
		strcpy(pTemp->m_cShortPyjc,_T("PFYH"));
		memcpy(pTemp->m_ciStockCode.m_cCode,_T("600000"),6);
		pTemp->m_ciStockCode.m_cCodeType = STOCK_MARKET | SH_BOURSE | KIND_STOCKA;
		pTemp->m_l5DayVol = 10000;
		pTemp->m_lPrevClose = 10000;
		payStock->Add(pTemp);

		pTemp = new StockUserInfo;
		strcpy(pTemp->m_cStockName,_T("白云机场"));
		strcpy(pTemp->m_cShortPyjc,_T("BYJC"));
		memcpy(pTemp->m_ciStockCode.m_cCode,_T("600004"),6);
		pTemp->m_ciStockCode.m_cCodeType = STOCK_MARKET | SH_BOURSE | KIND_STOCKA;
		pTemp->m_l5DayVol = 10000;
		pTemp->m_lPrevClose = 10000;
		payStock->Add(pTemp);

		pTemp = new StockUserInfo();
		strcpy(pTemp->m_cStockName,_T("武钢股份"));
		strcpy(pTemp->m_cShortPyjc,_T("WGGF"));
		memcpy(pTemp->m_ciStockCode.m_cCode,_T("600005"),6);
		pTemp->m_ciStockCode.m_cCodeType = STOCK_MARKET | SH_BOURSE | KIND_STOCKA;
		pTemp->m_l5DayVol = 10000;
		pTemp->m_lPrevClose = 10000;
		payStock->Add(pTemp);

		pTemp = new StockUserInfo();
		strcpy(pTemp->m_cStockName,_T("东风汽车"));
		strcpy(pTemp->m_cShortPyjc,_T("DFQC"));
		memcpy(pTemp->m_ciStockCode.m_cCode,_T("600006"),6);
		pTemp->m_ciStockCode.m_cCodeType = STOCK_MARKET | SH_BOURSE | KIND_STOCKA;
		pTemp->m_l5DayVol = 10000;
		pTemp->m_lPrevClose = 10000;
		payStock->Add(pTemp);

		pTemp = new StockUserInfo();
		strcpy(pTemp->m_cStockName,_T("中国国贸"));
		strcpy(pTemp->m_cShortPyjc,_T("ZGGM"));
		memcpy(pTemp->m_ciStockCode.m_cCode,_T("600007"),6);
		pTemp->m_ciStockCode.m_cCodeType = STOCK_MARKET | SH_BOURSE | KIND_STOCKA;
		pTemp->m_l5DayVol = 10000;
		pTemp->m_lPrevClose = 10000;
		payStock->Add(pTemp);

		pTemp = new StockUserInfo();
		strcpy(pTemp->m_cStockName,_T("首创股份"));
		strcpy(pTemp->m_cShortPyjc,_T("SCGF"));
		memcpy(pTemp->m_ciStockCode.m_cCode,_T("600008"),6);
		pTemp->m_ciStockCode.m_cCodeType = STOCK_MARKET | SH_BOURSE | KIND_STOCKA;
		pTemp->m_l5DayVol = 10000;
		pTemp->m_lPrevClose = 10000;
		payStock->Add(pTemp);

		pTemp = new StockUserInfo();
		strcpy(pTemp->m_cStockName,_T("上海机场"));
		strcpy(pTemp->m_cShortPyjc,_T("SHJC"));
		memcpy(pTemp->m_ciStockCode.m_cCode,_T("600009"),6);
		pTemp->m_ciStockCode.m_cCodeType = STOCK_MARKET | SH_BOURSE | KIND_STOCKA;
		pTemp->m_l5DayVol = 10000;
		pTemp->m_lPrevClose = 10000;
		payStock->Add(pTemp);

	}
	else
	{
		pTemp = g_pStock;
		for (int i = 0; i < 7; i++,pTemp++)
		{
			payStock->Add(pTemp);
		}
	}
	nCount = payStock->GetCount();
	return g_pStock;

}

// CTableColume
CMapStringToPtr CTableColume::m_mapList;
COLORREF* CTableColume::m_colTest = new COLORREF;

CTableColume::CTableColume()
{
	row = -1;
	col = -1;			// Row and Column of item
	mask = NULL;			  // Mask for use in getting/setting cell data
	nState = NULL;		  // cell state (focus/hilighted etc)
	nFormat = NULL;     // Format of cell
	strText = "";		 // Text in cell
	iImage = -1;      // index of the list view item icon
	crBkClr = NULL;     // Background colour (or CLR_DEFAULT)
	crFgClr = NULL;     // Forground colour (or CLR_DEFAULT)
	lParam = NULL;      // 32-bit value to associate with item 

	m_bDelete = 0;
	m_dID     = -1;

	m_pProperty = NULL;

	m_nTempWidth = 0;

	*m_colTest = RGB(200,0,0);
}

CTableColume::CTableColume(WORD dID)
{
	m_bDelete = 0;
	m_dID	  = dID;

	TableColProperty* pCol = GetByKey(m_dID);

	if(pCol != NULL)
	{
		mask = pCol->m_nColMask;
	}
	else
	{
		m_bDelete |= NewColMask;
		mask = -1;
	}
	if(mask == (UINT)-1)
	{
		mask = GVIF_TEXT|GVIF_FORMAT|GVIF_PARAM|GVIF_BKCLR|GVIF_FGCLR|GetStyleByColID(dID)|GetDataTypeByColID(dID);
	}

	if(pCol != NULL)
	{
		nFormat = pCol->m_nColFormat;
	}
	else
	{
		m_bDelete |= NewColFormat;
		nFormat = -1;
	}
	if(nFormat == (UINT)-1)
	{
		nFormat = /*DT_WORDBREAK|DT_END_ELLIPSIS|*/DT_SINGLELINE|DT_VCENTER|DT_CENTER;
	}

	strText  = GetNameByColID(dID);
	crBkClr = GetBkClrByColID(dID);
	if(pCol != NULL)
	{
		crFgClr = GetFgClrByColID(pCol->m_TextColor);
	}
	else
	{
		//for test
		crFgClr = RGB(50,20,10);
	}

	lParam  = (LPARAM)this;

	m_pProperty = pCol;

	m_nTempWidth = 0;
}


CTableColume* CTableColume::operator=(CTableColume* pCol)
{
	if(pCol == NULL)
		return NULL;

	Delete();

	row = pCol->row;
	col = pCol->col;			// Row and Column of item
	mask = pCol->mask;			  // Mask for use in getting/setting cell data
	nState = pCol->nState;		  // cell state (focus/hilighted etc)
	nFormat = pCol->nFormat;     // Format of cell
	strText = pCol->strText;		 // Text in cell
	iImage = pCol->iImage;      // index of the list view item icon
	crBkClr = pCol->crBkClr;     // Background colour (or CLR_DEFAULT)
	crFgClr = pCol->crFgClr;     // Forground colour (or CLR_DEFAULT)
	lParam = pCol->lParam;      // 32-bit value to associate with item 
	memcpy(&lfFont,&pCol->lfFont,sizeof(LOGFONT));      // Cell font

	m_bDelete = pCol->m_bDelete;
	m_dID     = pCol->m_dID;

	m_pProperty = pCol->m_pProperty;

	m_nTempWidth = pCol->m_nTempWidth;

	return this;
}

void CTableColume::Delete()
{
	if(m_bDelete & NewColMask)
	{
		mask = 0;
	}
	if(m_bDelete & NewColFormat)
	{
		nFormat = 0;
	}
}

CTableColume::~CTableColume()
{
	Delete();
}

TableColProperty* CTableColume::GetColProperty()
{
	//return GetByKey(m_dID);

	if(m_pProperty != NULL)
		return m_pProperty;

	m_pProperty = GetByKey(m_dID);
	return m_pProperty;
}

CString CTableColume::GetName()
{
	TableColProperty* pProperty = GetColProperty();
	if(pProperty != NULL)
		return pProperty->m_strName;
	return GetName(m_dID);
}

UINT	CTableColume::GetRowMask()
{
	TableColProperty* pProperty = GetColProperty();
	if(pProperty != NULL)
		return pProperty->m_nRowMask;
	return GetRowMask(m_dID);
}

void CTableColume::SetRowMask(UINT nMask,BOOL bRow /*= TRUE*/)
{
	if(bRow)
	{
		UINT pFormat = GetRowMask(m_dID);
		pFormat = nMask;
	}
	else
	{
		mask = nMask;
	}	
}

UINT	CTableColume::GetRowFormat()
{
	TableColProperty* pProperty = GetColProperty();
	if(pProperty != NULL)
		return pProperty->m_nRowFormat;
	return GetRowFormat(m_dID);
}

void CTableColume::SetRowFormat(UINT nFormatRef,BOOL bRow /*= TRUE*/)
{
	if(bRow)
	{
		SetRowFormat(nFormatRef);
	}
	else
	{
		nFormat = nFormatRef;
	}	
}

CString CTableColume::GetNameByColID(WORD dID)
{
	return GetName(dID);
}

CMapWordToPtrEx* CTableColume::GetMapColName()
{
	return (new CMapWordToPtrEx);
}

CMapWordToPtrEx* CTableColume::GetMap(CString strName)
{
	void* pValue;
	if( m_mapList.Lookup(strName,pValue) )
	{
		return (CMapWordToPtrEx*)pValue;
	}
	return NULL;
}
//根据ID取得所属类别的名称
CString CTableColume::GetMapName(WORD key)
{
	CString strName;

	if(key >= COLUMN_HQ_BASE_BEGIN && key < COLUMN_HQ_BASE_END)     //股票行情
		strName = HQ_BASE_NAME;
	else if(key >= COLUMN_HQ_EX_BEGIN && key < COLUMN_HQ_EX_END)  // 股票扩展行情
		strName = HQ_EX_NAME;
	else if(key >= COLUMN_HQ_DETAIL_BEGIN && key <= COLUMN_HQ_DETAIL_END)  // 明细表
		strName = HQ_DETAIL_NAME;
	else if(key >= COLUMN_CAPITALIZATION_BEGIN && key < COLUMN_CAPITALIZATION_END)  // 股本数据
		strName = GB_GUBEN_NAME;
	else if(key >= COLUMN_FINANCE_BEGIN && key < COLUMN_FINANCE_END)  // 财务数据
		strName = Cj_GUBEN_NAME;
	else if(key >= COLUMN_LINKAGE_BEGIN && key < COLUMN_LINKAGE_END)  // 联动分析
		strName = LOCK_NAME;
	else if(key >= COLUMN_FUTURES_BEGIN && key < COLUMN_FUTURES_END) // 期货行情
		strName = Qihuo_HangQing_Name;
	else if(key >= ZiXun_Browse_Begin && key < ZiXun_Browse_End) // 资讯浏览
		strName = ZiXun_Browse_Name;
	else if (key >= COLUMN_BLOCK_BEGIN && key < COLUMN_BLOCK_END)	//modify by lxqi 20090703	板块信息
		strName = HS_BLOCK_NAME;
	else if(key >= USR_DEF_BEGIN && key < USR_DEF_END)//自定义
		strName = USR_DEF_NAME;
	else if(key >= ORIGINAL_USE_COLUMN_BEGIN && key < ORIGINAL_USE_COLUMN_END)//内部使用
		strName = ORIGINAL_USE_COLUMN_NAME;
	return strName;
}

CMapWordToPtrEx* CTableColume::GetMap(WORD key)
{
	return GetMap( GetMapName(key) );
}

void CTableColume::AddToList(CString strName,CMapWordToPtrEx* pValue)
{
	if( pValue == NULL || strName.IsEmpty() )
		return;
	m_mapList.SetAt(strName,pValue);
}

void CTableColume::DelList()
{
	POSITION pos = m_mapList.GetStartPosition();
	CString strName;
	void* pValue = NULL;
	while(pos)
	{
		m_mapList.GetNextAssoc(pos, strName, pValue);
		DelMapColName(strName);
	}
	m_mapList.RemoveAll();
}

void CTableColume::DelMapColName(CString strName)
{
	CMapWordToPtrEx* pMap = GetMap(strName);
	if( !pMap->IsEmpty() )
	{
		TableColProperty* pCol;
		CString  szEntry;
		CString  szValue;

		POSITION pos = pMap->GetStartPosition();
		WORD wKey;
		void* pValue = NULL;
		if(pMap->IsStyle(MPEXS_RWCONFIG))
		{
			while(pos)
			{
				pMap->GetNextAssoc(pos, wKey, pValue);
				if(pValue != NULL)
				{

					pCol = (TableColProperty*)pValue;
					if(pCol)
					{
						delete pCol;
					}
				}
			}
		}
		else
		{
			while(pos)
			{
				pMap->GetNextAssoc(pos, wKey, pValue);
				if(pValue != NULL)
				{
					delete (TableColProperty*)pValue;					
				}
			}
		}
		pMap->RemoveAll();
	}
	delete pMap;
	pMap = NULL;
}

BOOL CTableColume::GetColName(CWinApp* pApp,WORD dNameID,CString& strName,CString strSection)
{
	CString szEntry;
	szEntry.Format("%i",dNameID);

	//for test
	char szName[128];
	ZeroMemory(szName,128);
	BOOL bRes = ::GetPrivateProfileString(strSection, szEntry, _T("Testing"),szName,127, CGeneralHelper::GetSpecifyPath(CGeneralHelper::SYSCONFIG_PATH) + _T("ColProperty.cfg"));
	strName.Format("%s",szName);
	return bRes;
}

void	CTableColume::WriteColName(WORD wKey, TableColProperty* pCol /*= NULL*/, LPCTSTR lpszSection /*= NULL*/,LPCTSTR lpszFileName /*= NULL*/)
{
	CString strFile, strSection, strEntry, strValume;
	//NULL,则自动取得
	if(pCol == NULL)
	{
		CTableColume::GetByKey(wKey);
	}
	//
	if( pCol == NULL )
	{
		return ;
	}
	//
	// 为了自动列宽列好的处理起做用。
	pCol->m_cConfigWidth = pCol->m_cWidth;

	// for testing
	//strFile.Format("%s", lpszFileName ? lpszFileName : (CHSFilePath::GetPath(CHSFilePath::Setting) + FILENAME_COLPROPERTY_CFG));
	//strSection.Format("%s", lpszSection ? lpszSection : CTableColume::GetMapName(wKey));
	//strEntry.Format("%d", wKey);
	//strValume.Format("%s;%i;%i;%i;%i;%i;%i",pCol->m_strName,pCol->m_nRowMask,pCol->m_nRowFormat,
	//	pCol->m_nColMask,pCol->m_nColFormat,pCol->m_lColorID,pCol->m_cWidth);
	//WritePrivateProfileString(strSection, strEntry, strValume, strFile);
}

void CTableColume::InitColName()
{
	if( m_mapList.GetCount() > 0 )
		return;

	CWinApp* pApp = AfxGetApp();
	CString strName;

	// 基本行情 ===================
	CMapWordToPtrEx* pMap = GetMapColName();
	pMap->InitHashTable(COLUMN_HQ_BASE_END - COLUMN_HQ_BASE_BEGIN);
	pMap->SetType(MPEXT_HQ);
	CString strSection = HQ_BASE_NAME;
	AddToList(strSection,pMap);
	pMap->SetAt(COLUMN_HQ_BASE_NAME,				new TableColProperty( COLUMN_HQ_BASE_NAME ,-1,ROWFORMAT_DEFAULT|DT_CENTER ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_CODE,				new TableColProperty( COLUMN_HQ_BASE_CODE ,-1,ROWFORMAT_DEFAULT|DT_CENTER ),pApp,strName,strSection );		
	pMap->SetAt(COLUMN_HQ_BASE_OPEN,				new TableColProperty( COLUMN_HQ_BASE_OPEN ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_NEW_PRICE,			new TableColProperty( COLUMN_HQ_BASE_NEW_PRICE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_RISE_VALUE,			new TableColProperty( COLUMN_HQ_BASE_RISE_VALUE  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_TOTAL_HAND,			new TableColProperty( COLUMN_HQ_BASE_TOTAL_HAND  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_HAND,				new TableColProperty( COLUMN_HQ_BASE_HAND  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_MAX_PRICE,			new TableColProperty( COLUMN_HQ_BASE_MAX_PRICE  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_MIN_PRICE,			new TableColProperty( COLUMN_HQ_BASE_MIN_PRICE  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_BUY_PRICE,			new TableColProperty( COLUMN_HQ_BASE_BUY_PRICE  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_SELL_PRICE,			new TableColProperty( COLUMN_HQ_BASE_SELL_PRICE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_RISE_RATIO,			new TableColProperty( COLUMN_HQ_BASE_RISE_RATIO   ),pApp,strName,strSection );

	pMap->SetAt(COLUMN_HQ_BASE_PRECLOSE,			new TableColProperty( COLUMN_HQ_BASE_PRECLOSE	  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_VOLUME_RATIO,		new TableColProperty( COLUMN_HQ_BASE_VOLUME_RATIO	  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_ORDER_BUY_PRICE,		new TableColProperty( COLUMN_HQ_BASE_ORDER_BUY_PRICE	  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_ORDER_BUY_VOLUME,    new TableColProperty( COLUMN_HQ_BASE_ORDER_BUY_VOLUME  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_ORDER_SELL_PRICE,    new TableColProperty( COLUMN_HQ_BASE_ORDER_SELL_PRICE  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_ORDER_SELL_VOLUME,	new TableColProperty( COLUMN_HQ_BASE_ORDER_SELL_VOLUME ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_IN_HANDS,			new TableColProperty( COLUMN_HQ_BASE_IN_HANDS	  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_OUT_HANDS,			new TableColProperty( COLUMN_HQ_BASE_OUT_HANDS	  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_MONEY,				new TableColProperty( COLUMN_HQ_BASE_MONEY		 ),pApp,strName,strSection );

	pMap->SetAt(COLUMN_HQ_BASE_SPEEDUP,				new TableColProperty( COLUMN_HQ_BASE_SPEEDUP  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_AVERAGE_PRICE,		new TableColProperty( COLUMN_HQ_BASE_AVERAGE_PRICE	  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_RANGE,				new TableColProperty( COLUMN_HQ_BASE_RANGE		 ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_ORDER_RATIO,			new TableColProperty( COLUMN_HQ_BASE_ORDER_RATIO		),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_ORDER_DIFF,			new TableColProperty( COLUMN_HQ_BASE_ORDER_DIFF	  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_NUMBER,				new TableColProperty( COLUMN_HQ_BASE_NUMBER,-1,ROWFORMAT_DEFAULT),pApp,strName,strSection );
	//	pMap->SetAt(COLUMN_HQ_BASE_TECH_SORT	,new TableColProperty( COLUMN_HQ_BASE_TECH_SORT),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_BASE_INFO_MARK,			new TableColProperty( COLUMN_HQ_BASE_INFO_MARK,-1,-1,-1,-1,2),pApp,strName,strSection );	

	pMap->SetAt(COLUMN_HQ_BASE_ARROW,				new TableColProperty( COLUMN_HQ_BASE_ARROW),pApp,strName,strSection );
	//pMap->SetAt(COLUMN_HQ_BASE_INVERSE_PRICE	,new TableColProperty( COLUMN_HQ_BASE_INVERSE_PRICE,-1,-1,-1,-1,2),pApp,strName,strSection );	


	// 明细表 ===================
	pMap = GetMapColName();
	pMap->SetType(MPEXT_HQ);
	pMap->InitHashTable(COLUMN_HQ_DETAIL_END - COLUMN_HQ_DETAIL_BEGIN);

	strSection = HQ_DETAIL_NAME;
	AddToList(strSection,pMap);

	// 分时明细表
	pMap->SetAt(COLUMN_HQ_DETAIL_TREND_Time,		new TableColProperty( COLUMN_HQ_DETAIL_TREND_Time	    ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TREND_NEW_PRICE,	new TableColProperty( COLUMN_HQ_DETAIL_TREND_NEW_PRICE  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TREND_RISE_VALUE,	new TableColProperty( COLUMN_HQ_DETAIL_TREND_RISE_VALUE  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TREND_BUY_PRICE,	new TableColProperty( COLUMN_HQ_DETAIL_TREND_BUY_PRICE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TREND_SELL_PRICE,	new TableColProperty( COLUMN_HQ_DETAIL_TREND_SELL_PRICE),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TREND_HAND,		new TableColProperty( COLUMN_HQ_DETAIL_TREND_HAND  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TREND_REALHANDS,	new TableColProperty(COLUMN_HQ_DETAIL_TREND_REALHANDS), pApp, strName, strSection);
	pMap->SetAt(COLUMN_HQ_DETAIL_TREND_POSITION,	new TableColProperty(COLUMN_HQ_DETAIL_TREND_POSITION), pApp, strName, strSection);
	pMap->SetAt(COLUMN_HQ_DETAIL_TREND_KPC,			new TableColProperty(COLUMN_HQ_DETAIL_TREND_KPC), pApp, strName, strSection);

	// 盘后明细
	pMap->SetAt(COLUMN_HQ_DETAIL_TECH_TIME,			new TableColProperty( COLUMN_HQ_DETAIL_TECH_TIME	      ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TECH__RISE_VALUE,	new TableColProperty( COLUMN_HQ_DETAIL_TECH__RISE_VALUE    ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TECH_OPEN_PRICE,   new TableColProperty( COLUMN_HQ_DETAIL_TECH_OPEN_PRICE  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TECH_MAX_PRICE,    new TableColProperty( COLUMN_HQ_DETAIL_TECH_MAX_PRICE   ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TECH_MIN_PRICE,		new TableColProperty( COLUMN_HQ_DETAIL_TECH_MIN_PRICE   ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TECH_CLOSE_PRICE,  new TableColProperty( COLUMN_HQ_DETAIL_TECH_CLOSE_PRICE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TECH_VOLUME,		new TableColProperty( COLUMN_HQ_DETAIL_TECH_VOLUME     ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TECH_5DAY_AVGPRICE,new TableColProperty( COLUMN_HQ_DETAIL_TECH_5DAY_AVGPRICE  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_TECH_5DAY_TOTALVOL,new TableColProperty( COLUMN_HQ_DETAIL_TECH_5DAY_TOTALVOL ),pApp,strName,strSection );
	// 大盘明细
	pMap->SetAt(COLUMN_HQ_DETAIL_INDEX_TIME,		new TableColProperty( COLUMN_HQ_DETAIL_INDEX_TIME		),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_INDEX_INDEX,		new TableColProperty( COLUMN_HQ_DETAIL_INDEX_INDEX		),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_INDEX_RISE_VALUE,	new TableColProperty( COLUMN_HQ_DETAIL_INDEX_RISE_VALUE		),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_INDEX_RISE_RATIO,	new TableColProperty( COLUMN_HQ_DETAIL_INDEX_RISE_RATIO	),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_INDEX_RISE_COUNT,	new TableColProperty( COLUMN_HQ_DETAIL_INDEX_RISE_COUNT		),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_INDEX_FALL_COUNT,	new TableColProperty( COLUMN_HQ_DETAIL_INDEX_FALL_COUNT		),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_INDEX_EQUAL_COUNT,	new TableColProperty( COLUMN_HQ_DETAIL_INDEX_EQUAL_COUNT		),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_INDEX_SUM_PRICE,	new TableColProperty( COLUMN_HQ_DETAIL_INDEX_SUM_PRICE	),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_INDEX_PRICE,		new TableColProperty( COLUMN_HQ_DETAIL_INDEX_PRICE		),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_INDEX_SUM_VOLUME,	new TableColProperty( COLUMN_HQ_DETAIL_INDEX_SUM_VOLUME		),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_INDEX_VOLUME,		new TableColProperty( COLUMN_HQ_DETAIL_INDEX_VOLUME	),pApp,strName,strSection );

	// 买卖盘
	pMap->SetAt(COLUMN_BSORDER_ORDER,				new TableColProperty( COLUMN_BSORDER_ORDER, -1, -1, -1, -1, -1, 9	),pApp,strName,strSection );
	pMap->SetAt(COLUMN_BSORDER_BS1,					new TableColProperty( COLUMN_BSORDER_BS1, -1, -1, -1, -1, -1, 9	),pApp,strName,strSection );
	pMap->SetAt(COLUMN_BSORDER_BS2,					new TableColProperty( COLUMN_BSORDER_BS2, -1, -1, -1, -1, -1, 9	),pApp,strName,strSection );
	pMap->SetAt(COLUMN_BSORDER_BS3,					new TableColProperty( COLUMN_BSORDER_BS3, -1, -1, -1, -1, -1, 9	),pApp,strName,strSection );
	pMap->SetAt(COLUMN_BSORDER_BS4,					new TableColProperty( COLUMN_BSORDER_BS4, -1, -1, -1, -1, -1, 9	),pApp,strName,strSection );
	pMap->SetAt(COLUMN_BSORDER_BS5,					new TableColProperty( COLUMN_BSORDER_BS5, -1, -1, -1, -1, -1, 9	),pApp,strName,strSection );

	// 分价表
	pMap->SetAt(COLUMN_HQ_DETAIL_SPREAD_PRICE,		new TableColProperty( COLUMN_HQ_DETAIL_SPREAD_PRICE, -1, ROWFORMAT_DEFAULT|DT_RIGHT, -1,  DT_RIGHT, -1, 9 ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_SPREAD_VOLUME,		new TableColProperty( COLUMN_HQ_DETAIL_SPREAD_VOLUME, -1, ROWFORMAT_DEFAULT|DT_RIGHT, -1,  DT_RIGHT, -1, 9 ),pApp,strName,strSection );
	//	增加内外盘、分价量比等列，杨朝惠，2003-08-11
	pMap->SetAt(COLUMN_HQ_DETAIL_SPREAD_VOLUMERATIO,new TableColProperty( COLUMN_HQ_DETAIL_SPREAD_VOLUMERATIO, -1, ROWFORMAT_DEFAULT|DT_RIGHT,	 -1, DT_RIGHT, -1, 11), pApp, strName, strSection);
	pMap->SetAt(COLUMN_HQ_DETAIL_SPREAD_GRAPHIC,	new TableColProperty( COLUMN_HQ_DETAIL_SPREAD_GRAPHIC, -1, ROWFORMAT_DEFAULT|DT_CENTER,		 -1,DT_LEFT,-1, 25 ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_SPREAD_SELLVOLUME,	new TableColProperty( COLUMN_HQ_DETAIL_SPREAD_SELLVOLUME, -1, ROWFORMAT_DEFAULT|DT_CENTER,	 -1,DT_RIGHT,-1, 25), pApp, strName, strSection);

	//右边小图值
	pMap->SetAt(COLHEAD_FINANCE_VALUE,				new TableColProperty( COLHEAD_FINANCE_VALUE -1, DT_RIGHT|DT_VCENTER|DT_SINGLELINE));
	pMap->SetAt(COLHEAD_FINANCE_NAME,				new TableColProperty( COLHEAD_FINANCE_NAME, -1, DT_LEFT|DT_VCENTER|DT_SINGLELINE));

	/*************************************************************
	* 吕连新更改
	* 时间：  2003年9月3日
	* 描述：  为适应新协议
	*************************************************************/
	//配
	pMap->SetAt(COLUMN_HQ_DETAIL_EXRIGHT_DATE,		new TableColProperty( COLUMN_HQ_DETAIL_EXRIGHT_GIVE),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_EXRIGHT_GIVE,		new TableColProperty( COLUMN_HQ_DETAIL_EXRIGHT_GIVE),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_EXRIGHT_PLACING,	new TableColProperty( COLUMN_HQ_DETAIL_EXRIGHT_PLACING),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_EXRIGHT_GIVE_PRICE,new TableColProperty( COLUMN_HQ_DETAIL_EXRIGHT_GIVE_PRICE),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_DETAIL_EXRIGHT_BONUS,		new TableColProperty( COLUMN_HQ_DETAIL_EXRIGHT_BONUS),pApp,strName,strSection );
	// 扩展行情 ==============
	pMap = GetMapColName();
	pMap->InitHashTable(COLUMN_HQ_EX_END - COLUMN_HQ_EX_BEGIN);
	pMap->SetType(MPEXT_HQ);

	strSection = HQ_EX_NAME;
	AddToList(strSection,pMap);

	pMap->SetAt(COLUMN_HQ_EX_BUY_PRICE1,			new TableColProperty( COLUMN_HQ_EX_BUY_PRICE1),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_BUY_VOLUME1,			new TableColProperty( COLUMN_HQ_EX_BUY_VOLUME1),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_BUY_PRICE2,			new TableColProperty( COLUMN_HQ_EX_BUY_PRICE2),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_BUY_VOLUME2,			new TableColProperty( COLUMN_HQ_EX_BUY_VOLUME2),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_BUY_PRICE3,			new TableColProperty( COLUMN_HQ_EX_BUY_PRICE3),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_BUY_VOLUME3,			new TableColProperty( COLUMN_HQ_EX_BUY_VOLUME3),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_BUY_PRICE4,			new TableColProperty( COLUMN_HQ_EX_BUY_PRICE4),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_BUY_VOLUME4,			new TableColProperty( COLUMN_HQ_EX_BUY_VOLUME4),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_BUY_PRICE5,			new TableColProperty( COLUMN_HQ_EX_BUY_PRICE5),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_BUY_VOLUME5,			new TableColProperty( COLUMN_HQ_EX_BUY_VOLUME5),pApp,strName,strSection );


	pMap->SetAt(COLUMN_HQ_EX_SELL_PRICE1,			new TableColProperty(COLUMN_HQ_EX_SELL_PRICE1),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_SELL_VOLUME1,			new TableColProperty(COLUMN_HQ_EX_SELL_VOLUME1),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_SELL_PRICE2,			new TableColProperty(COLUMN_HQ_EX_SELL_PRICE2),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_SELL_VOLUME2,			new TableColProperty(COLUMN_HQ_EX_SELL_VOLUME2),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_SELL_PRICE3,			new TableColProperty(COLUMN_HQ_EX_SELL_PRICE3),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_SELL_VOLUME3,			new TableColProperty(COLUMN_HQ_EX_SELL_VOLUME3),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_SELL_PRICE4,			new TableColProperty(COLUMN_HQ_EX_SELL_PRICE4),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_SELL_VOLUME4,			new TableColProperty(COLUMN_HQ_EX_SELL_VOLUME4),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_SELL_PRICE5,			new TableColProperty(COLUMN_HQ_EX_SELL_PRICE5),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_SELL_VOLUME5,			new TableColProperty(COLUMN_HQ_EX_SELL_VOLUME5),pApp,strName,strSection );

	pMap->SetAt(COLUMN_HQ_EX_EXHAND_RATIO,			new TableColProperty( COLUMN_HQ_EX_EXHAND_RATIO),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_5DAY_AVGVOLUME,		new TableColProperty( COLUMN_HQ_EX_5DAY_AVGVOLUME),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_PE_RATIO,				new TableColProperty( COLUMN_HQ_EX_PE_RATIO),pApp,strName,strSection );

	pMap->SetAt(COLUMN_HQ_EX_FUND_NETVALUE,			new TableColProperty( COLUMN_HQ_EX_FUND_NETVALUE),pApp,strName,strSection );
	pMap->SetAt(COLUMN_HQ_EX_BOND_ACCRUAL,			new TableColProperty( COLUMN_HQ_EX_BOND_ACCRUAL),pApp,strName,strSection );

	// 股本数据 ==============
	pMap = GetMapColName();
	pMap->InitHashTable(COLUMN_CAPITALIZATION_END - COLUMN_CAPITALIZATION_BEGIN);
	pMap->SetType(MPEXT_HQ);

	strSection = GB_GUBEN_NAME;
	AddToList(strSection,pMap);

	pMap->SetAt(COLUMN_CAPITALIZATION_TOTAL,
		new TableColProperty( COLUMN_CAPITALIZATION_TOTAL ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_CAPITALIZATION_NATIONAL,
		new TableColProperty( COLUMN_CAPITALIZATION_NATIONAL ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_CAPITALIZATION_INITIATOR,
		new TableColProperty( COLUMN_CAPITALIZATION_INITIATOR ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_CAPITALIZATION_CORPORATION,
		new TableColProperty( COLUMN_CAPITALIZATION_CORPORATION ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_CAPITALIZATION_B,
		new TableColProperty( COLUMN_CAPITALIZATION_B ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_CAPITALIZATION_H,
		new TableColProperty( COLUMN_CAPITALIZATION_H ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_CAPITALIZATION_PASS_A,
		new TableColProperty( COLUMN_CAPITALIZATION_PASS_A ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_CAPITALIZATION_EMPLOYEE,
		new TableColProperty( COLUMN_CAPITALIZATION_EMPLOYEE ),pApp,strName,strSection );
	// 财务数据 ==============
	pMap = GetMapColName();
	pMap->InitHashTable(COLUMN_FINANCE_END - COLUMN_FINANCE_BEGIN);
	pMap->SetType(MPEXT_HQ);

	strSection = Cj_GUBEN_NAME;
	AddToList(strSection,pMap);

	pMap->SetAt(COLUMN_FINANCE_TOTAL_ASSETS			,new TableColProperty( COLUMN_FINANCE_TOTAL_ASSETS ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_CURRENT_ASSETS		,new TableColProperty( COLUMN_FINANCE_CURRENT_ASSETS ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_CAPITAL_ASSETS		,new TableColProperty( COLUMN_FINANCE_CAPITAL_ASSETS ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_UNBODIED_ASSETS		,new TableColProperty( COLUMN_FINANCE_UNBODIED_ASSETS ),pApp,strName,strSection );
	//	pMap->SetAt(COLUMN_FINANCE_LONG_INVESTMENT				 ,new TableColProperty( COLUMN_FINANCE_LONG_INVESTMENT				  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_CURRENT_LIABILITIES	,new TableColProperty( COLUMN_FINANCE_CURRENT_LIABILITIES ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_LONG_LIABILITIES		,new TableColProperty( COLUMN_FINANCE_LONG_LIABILITIES ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_CAPITAL_ACCFUND		,new TableColProperty( COLUMN_FINANCE_CAPITAL_ACCFUND ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_PERSTOCK_ACCFUND		,new TableColProperty( COLUMN_FINANCE_PERSTOCK_ACCFUND ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_PARTNER_RIGHT		,new TableColProperty( COLUMN_FINANCE_PARTNER_RIGHT ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_MAIN_INCOME			,new TableColProperty( COLUMN_FINANCE_MAIN_INCOME ),pApp,strName,strSection );
	//	pMap->SetAt(COLUMN_FINANCE_MAIN_PROFIT			 ,new TableColProperty( COLUMN_FINANCE_MAIN_PROFIT			  ),pApp,strName,strSection );
	//	pMap->SetAt(COLUMN_FINANCE_OTHER_PROFIT			 ,new TableColProperty( COLUMN_FINANCE_OTHER_PROFIT			  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_TAKING_PROFIT		,new TableColProperty( COLUMN_FINANCE_TAKING_PROFIT ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_YIELD				,new TableColProperty( COLUMN_FINANCE_YIELD ),pApp,strName,strSection );
	//	pMap->SetAt(COLUMN_FINANCE_SUBSIDY				 ,new TableColProperty( COLUMN_FINANCE_SUBSIDY				  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_OTHER_INCOME			,new TableColProperty( COLUMN_FINANCE_OTHER_INCOME ),pApp,strName,strSection );
	//	pMap->SetAt(COLUMN_FINANCE_LAST_PROFIT_LOSS				 ,new TableColProperty( COLUMN_FINANCE_LAST_PROFIT_LOSS				  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_TOTAL_PROFIT			,new TableColProperty( COLUMN_FINANCE_TOTAL_PROFIT ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_SCOT_PROFIT			,new TableColProperty( COLUMN_FINANCE_SCOT_PROFIT ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_RETAINED_PROFITS		,new TableColProperty( COLUMN_FINANCE_RETAINED_PROFITS ),pApp,strName,strSection );
	//	pMap->SetAt(COLUMN_FINANCE_UNPAID_PROFIT			 ,new TableColProperty( COLUMN_FINANCE_UNPAID_PROFIT			  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_PER_UNPAID			,new TableColProperty( COLUMN_FINANCE_PER_UNPAID ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_PER_INCOME			,new TableColProperty( COLUMN_FINANCE_PER_INCOME ),pApp,strName,strSection );

	pMap->SetAt(COLUMN_FINANCE_PER_ASSETS			,new TableColProperty( COLUMN_FINANCE_PER_ASSETS ),pApp,strName,strSection );
	//	pMap->SetAt(COLUMN_FINANCE_ADJUST_PER_ASSETS	 ,new TableColProperty( COLUMN_FINANCE_ADJUST_PER_ASSETS	  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FINANCE_PARTNER_RIGHT_RATIO	,new TableColProperty( COLUMN_FINANCE_PARTNER_RIGHT_RATIO ),pApp,strName,strSection );

	pMap->SetAt(COLUMN_FINANCE_ASSETS_YIELD			,new TableColProperty( COLUMN_FINANCE_ASSETS_YIELD ),pApp,strName,strSection );


	pMap = GetMapColName();
	pMap->InitHashTable(COLUMN_FUTURES_END - COLUMN_FUTURES_BEGIN);
	pMap->SetType(MPEXT_HQ);

	strSection = Qihuo_HangQing_Name;
	AddToList(strSection,pMap);
	pMap->SetAt(COLUMN_FUTURES_CODE					,new TableColProperty( COLUMN_FUTURES_CODE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_NAME					,new TableColProperty( COLUMN_FUTURES_NAME ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_OPEN					,new TableColProperty( COLUMN_FUTURES_OPEN ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_PRECLOSE			    ,new TableColProperty( COLUMN_FUTURES_PRECLOSE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_HIGH					,new TableColProperty( COLUMN_FUTURES_HIGH ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_LOW					,new TableColProperty( COLUMN_FUTURES_LOW ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_BID1_PRICE			,new TableColProperty( COLUMN_FUTURES_BID1_PRICE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_ASK1_PRICE			,new TableColProperty( COLUMN_FUTURES_ASK1_PRICE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_BID1_VOL				,new TableColProperty( COLUMN_FUTURES_BID1_VOL ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_ASK1_VOL				,new TableColProperty( COLUMN_FUTURES_ASK1_VOL ),pApp,strName,strSection );
	//pMap->SetAt(Yls_QiHuoBID2			    ,new TableColProperty( Yls_QiHuoBID2			  ),pApp,strName,strSection );
	//pMap->SetAt(Yls_QiHuoASK2			    ,new TableColProperty( Yls_QiHuoASK2			  ),pApp,strName,strSection );
	//pMap->SetAt(Yls_QiHuoBID2_VOL		    ,new TableColProperty( Yls_QiHuoBID2_VOL		  ),pApp,strName,strSection );
	//pMap->SetAt(Yls_QiHuoASK2_VOL		    ,new TableColProperty( Yls_QiHuoASK2_VOL		  ),pApp,strName,strSection );
	//pMap->SetAt(Yls_QiHuoBID3			    ,new TableColProperty( Yls_QiHuoBID3			  ),pApp,strName,strSection );
	//pMap->SetAt(Yls_QiHuoASK3			    ,new TableColProperty( Yls_QiHuoASK3			  ),pApp,strName,strSection );
	//pMap->SetAt(Yls_QiHuoBID3_VOL		    ,new TableColProperty( Yls_QiHuoBID3_VOL		  ),pApp,strName,strSection );
	//pMap->SetAt(Yls_QiHuoASK3_VOL		    ,new TableColProperty( Yls_QiHuoASK3_VOL		  ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_NEW_PRICE			,new TableColProperty( COLUMN_FUTURES_NEW_PRICE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_VOLUME			    ,new TableColProperty( COLUMN_FUTURES_VOLUME ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_AMOUNT			    ,new TableColProperty( COLUMN_FUTURES_AMOUNT ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_SETTLE			    ,new TableColProperty( COLUMN_FUTURES_SETTLE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_MARKET			    ,new TableColProperty( COLUMN_FUTURES_MARKET ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_NEW_VOL				,new TableColProperty( COLUMN_FUTURES_NEW_VOL ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_TOTAL_AMOUNT			,new TableColProperty( COLUMN_FUTURES_TOTAL_AMOUNT ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_RISE_VALUE		    ,new TableColProperty( COLUMN_FUTURES_RISE_VALUE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_ORDER_RATIO		    ,new TableColProperty( COLUMN_FUTURES_ORDER_RATIO ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_PRESETTLE		    ,new TableColProperty( COLUMN_FUTURES_PRESETTLE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_Current_SETTLE	    ,new TableColProperty( COLUMN_FUTURES_Current_SETTLE ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_IO_SUB			    ,new TableColProperty( COLUMN_FUTURES_IO_SUB ),pApp,strName,strSection );	
	pMap->SetAt(COLUMN_FUTURES_AMOUNT_SUB		    ,new TableColProperty( COLUMN_FUTURES_AMOUNT_SUB ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_OPEN_POSITION	    ,new TableColProperty( COLUMN_FUTURES_OPEN_POSITION ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_CLEAR_POSITION	    ,new TableColProperty( COLUMN_FUTURES_CLEAR_POSITION ),pApp,strName,strSection );
	pMap->SetAt(COLUMN_FUTURES_PRESETTLE_PRICE		,new TableColProperty( COLUMN_FUTURES_PRESETTLE_PRICE ),pApp,strName,strSection );	

	pMap->SetAt(COLUMN_FUTURES_HIS_HIGH			    ,new TableColProperty( COLUMN_FUTURES_HIS_HIGH ),pApp,strName,strSection );	
	pMap->SetAt(COLUMN_FUTURES_HIS_LOW			    ,new TableColProperty( COLUMN_FUTURES_HIS_LOW ),pApp,strName,strSection );	
	pMap->SetAt(COLUMN_FUTURES_UPPER_LIM		    ,new TableColProperty( COLUMN_FUTURES_UPPER_LIM ),pApp,strName,strSection );	
	pMap->SetAt(COLUMN_FUTURES_LOWER_LIM		    ,new TableColProperty( COLUMN_FUTURES_LOWER_LIM ),pApp,strName,strSection );	
	pMap->SetAt(COLUMN_FUTURES_RISE_VALUE2			,new TableColProperty( COLUMN_FUTURES_RISE_VALUE2 ),pApp,strName,strSection );	
	pMap->SetAt(COLUMN_FUTURES_ORDER_RATIO2			,new TableColProperty( COLUMN_FUTURES_ORDER_RATIO2),pApp,strName,strSection );	

	//面向内部使用的字段
	pMap = GetMapColName();
	pMap->InitHashTable(ORIGINAL_USE_COLUMN_END - ORIGINAL_USE_COLUMN_BEGIN, 0);//初始化长度,但是不分配内存
	pMap->SetType(MPEXT_ORIGINAL_USE);
	pMap->ModifyStyle(MPEXS_COLNAME_NORWCFG, 0);

	strSection = ORIGINAL_USE_COLUMN_NAME;
	AddToList(strSection,pMap);

	//板块信息
	pMap = GetMapColName();
	pMap->SetType(MPEXT_HQ);
	pMap->InitHashTable(COLUMN_BLOCK_END - COLUMN_BLOCK_BEGIN);
	strSection = HS_BLOCK_NAME;
	AddToList(strSection,pMap);
	// 分时明细表
	pMap->SetAt(COLUMN_BLOCK_RECOMMEND_NAME,	     new TableColProperty(COLUMN_BLOCK_RECOMMEND_NAME),pApp,strName,strSection );
	pMap->SetAt(COLUMN_BLOCK_RECOMMEND_DATE,	     new TableColProperty(COLUMN_BLOCK_RECOMMEND_DATE),pApp,strName,strSection );
	pMap->SetAt(COLUMN_BLOCK_RECOMMEND_BUYPRICE,	     new TableColProperty(COLUMN_BLOCK_RECOMMEND_BUYPRICE),pApp,strName,strSection );
	pMap->SetAt(COLUMN_BLOCK_RECOMMEND_SELLPRICE,	     new TableColProperty(COLUMN_BLOCK_RECOMMEND_SELLPRICE),pApp,strName,strSection );
}

BOOL CTableColume::Find(WORD key)
{
	CMapWordToPtrEx* pMap = GetMap(key);
	if(pMap != NULL)
	{
		void* rValue;
		return pMap->Lookup(key, rValue);
	}
	return FALSE;
}

CString CTableColume::GetName(WORD key)
{
	TableColProperty* pCol = GetByKey(key);
	if(pCol != NULL)
	{
		return pCol->m_strName;
	}
	return _T("无");
}

UINT CTableColume::GetColMask(WORD key)
{
	TableColProperty* pCol = GetByKey(key);
	if(pCol != NULL)
	{
		return pCol->m_nColMask;
	}
	return NULL;
}

UINT CTableColume::GetColFormat(WORD key)
{
	TableColProperty* pCol = GetByKey(key);
	if(pCol != NULL)
	{
		return pCol->m_nColFormat;
	}
	return NULL;
}

UINT CTableColume::GetRowMask(WORD key)
{
	TableColProperty* pCol = GetByKey(key);
	if(pCol != NULL)
	{
		return pCol->m_nRowMask;
	}
	return ROWMASK_DEFAULT;
}

UINT CTableColume::GetRowFormat(WORD key)
{
	TableColProperty* pCol = GetByKey(key);
	if(pCol != NULL)
	{
		return pCol->m_nRowFormat;
	}
	return ROWFORMAT_DEFAULT;
}

TableColProperty* CTableColume::GetByKey(WORD key)
{
	CMapWordToPtrEx* pMap = GetMap(key);
	if(pMap != NULL)
	{
		void* rValue;
		if( pMap->Lookup(key, rValue) )
		{
			if(rValue != NULL)
			{
				return (TableColProperty*)rValue;
			}
		}
		else if(pMap->GetType() == MPEXT_ORIGINAL_USE)
		{
			CWinApp* pApp = AfxGetApp();
			CString strName;
			CString strSection = ORIGINAL_USE_COLUMN_NAME;

			TableColProperty *pCol = new TableColProperty( key, -1, -1, -1,-1, -1, 12);
			pMap->SetAt(key, (void*)pCol, pApp,strName,strSection );
			return pCol;
		}
	}
	return NULL;
}

int CTableColume::GetColWidth()
{
	TableColProperty* pCol = GetByKey(m_dID);
	if(pCol != NULL)
	{
		return pCol->m_cWidth;
	}
	return 7;
}

void CTableColume::SetColWidth(int nWidth)
{
	TableColProperty* pCol = GetByKey(m_dID);
	if(pCol != NULL)
	{
		pCol->m_cWidth = (char)nWidth;
	}
}

COLORREF CTableColume::GetBkClrByColID(WORD dID)
{
	//switch(dID)
	//{
		// 一般报价
		//for testing
		//case COLUMN_HQ_BASE_NAME:	    return &g_hxData.clPageBK.m_lColor;
		//case COLUMN_HQ_BASE_CODE:	    return &g_hxData.clPageBK.m_lColor;		
		//case COLUMN_HQ_BASE_OPEN: return &g_hxData.clPageBK.m_lColor;
		//case COLUMN_HQ_BASE_NEW_PRICE: return &g_hxData.clPageBK.m_lColor;
		//case COLUMN_HQ_BASE_RISE_VALUE:  return &g_hxData.clPageBK.m_lColor;
		//case COLUMN_HQ_BASE_TOTAL_HAND:  return &g_hxData.clPageBK.m_lColor;
		//case COLUMN_HQ_BASE_HAND:  return &g_hxData.clPageBK.m_lColor;
		//case COLUMN_HQ_BASE_MAX_PRICE:  return &g_hxData.clPageBK.m_lColor;
		//case COLUMN_HQ_BASE_MIN_PRICE:  return &g_hxData.clPageBK.m_lColor;
		//case COLUMN_HQ_BASE_BUY_PRICE:  return &g_hxData.clPageBK.m_lColor;
		//case COLUMN_HQ_BASE_SELL_PRICE: return &g_hxData.clPageBK.m_lColor;
		//case COLUMN_HQ_BASE_RISE_RATIO:   return &g_hxData.clPageBK.m_lColor;
		//
	//default:
	//	break;
	//}
	return *m_colTest/*&g_hxData.clPageBK.m_lColor*/;
}

COLORREF CTableColume::GetFgClrByColID(WORD dID)
{
	// for testing
	ColorProperty* pColor = new ColorProperty(RGB(255,0,0))/*g_hxData.GetColorProperty(dID)*/;
	//if(pColor != NULL)
	return pColor->m_lColor;
	//return &g_hxData.clGridDynamic.m_lColor/*clFixedNameTXT*/;
}

UINT CTableColume::GetStyleByColID(WORD dID)
{
	switch(dID)
	{
		// 一般报价
	case COLUMN_HQ_BASE_NAME:	    return GVIF_NONO;
	case COLUMN_HQ_BASE_CODE:	    return GVIF_NONO;
	case COLUMN_HQ_BASE_OPEN: return GVIF_UNDELINE;
	case COLUMN_HQ_BASE_NEW_PRICE: return GVIF_UNDELINE;
	case COLUMN_HQ_BASE_RISE_VALUE:  return GVIF_UNDELINE|GVIF_ARROW;
	case COLUMN_HQ_BASE_TOTAL_HAND:  return GVIF_NONO/*|GVIF_FgClrSAMEHEAD*/;
	case COLUMN_HQ_BASE_HAND:  return GVIF_NONO/*|GVIF_FgClrSAMEHEAD*/;
	case COLUMN_HQ_BASE_MAX_PRICE:  return GVIF_UNDELINE;
	case COLUMN_HQ_BASE_MIN_PRICE:  return GVIF_UNDELINE;
	case COLUMN_HQ_BASE_BUY_PRICE:  return GVIF_UNDELINE;
	case COLUMN_HQ_BASE_SELL_PRICE: return GVIF_UNDELINE;
	case COLUMN_HQ_BASE_RISE_RATIO:   return GVIF_SYMBOL|GVIF_UNDELINE;
		// 分时明细表
	case COLUMN_HQ_DETAIL_TREND_Time:	  return GVIF_NONO;
	case COLUMN_HQ_DETAIL_TREND_NEW_PRICE:   return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_TREND_RISE_VALUE:   return GVIF_SYMBOL|GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_TREND_BUY_PRICE:  return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_TREND_SELL_PRICE: return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_TREND_HAND:   return GVIF_NONO;

	case COLUMN_HQ_DETAIL_TREND_KPC:	return GVIF_NONO;
	case COLUMN_HQ_DETAIL_TREND_POSITION: return GVIF_SYMBOL|GVIF_UNDELINE;

		// 盘后明细
	case COLUMN_HQ_DETAIL_TECH_TIME:	    return GVIF_NONO;
	case COLUMN_HQ_DETAIL_TECH__RISE_VALUE:     return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_TECH_OPEN_PRICE:   return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_TECH_MAX_PRICE:    return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_TECH_MIN_PRICE:    return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_TECH_CLOSE_PRICE:  return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_TECH_VOLUME:      return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_TECH_5DAY_AVGPRICE:   return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_TECH_5DAY_TOTALVOL:  return GVIF_UNDELINE;
		// 大盘明细
	case COLUMN_HQ_DETAIL_INDEX_TIME:		return GVIF_NONO;
	case COLUMN_HQ_DETAIL_INDEX_INDEX:		return GVIF_NONO;
	case COLUMN_HQ_DETAIL_INDEX_RISE_VALUE:		return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_INDEX_RISE_RATIO:	return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_INDEX_RISE_COUNT:		return GVIF_NONO;
	case COLUMN_HQ_DETAIL_INDEX_FALL_COUNT:		return GVIF_NONO;
	case COLUMN_HQ_DETAIL_INDEX_VOLUME:	return GVIF_NONO;
	case COLUMN_HQ_DETAIL_INDEX_EQUAL_COUNT:		return GVIF_NONO;
	case COLUMN_HQ_DETAIL_INDEX_SUM_PRICE:	return GVIF_NONO;
	case COLUMN_HQ_DETAIL_INDEX_PRICE:		return GVIF_NONO;
	case COLUMN_HQ_DETAIL_INDEX_SUM_VOLUME:		return GVIF_NONO;
		// 买卖盘
	case COLUMN_HQ_DETAIL_BSORDER_DIRECTION:		return GVIF_NONO;
	case COLUMN_HQ_DETAIL_BSORDER_PRICE:		return GVIF_UNDELINE;
	case COLUMN_HQ_DETAIL_BSORDER_VOLUME:		return GVIF_NONO;
		// 分价表
	case COLUMN_HQ_DETAIL_SPREAD_PRICE:		return GVIF_UNDELINE/*|GVIF_FgClrSAMEHEAD*/;
	case COLUMN_HQ_DETAIL_SPREAD_VOLUME:		return GVIF_NONO/*|GVIF_FgClrSAMEHEAD*/;
	case COLUMN_HQ_DETAIL_SPREAD_GRAPHIC:	return GVIF_NONO/*|GVIF_FgClrSAMEHEAD*/;
		//财务报表
	case COLHEAD_FINANCE_VALUE:	return GVIF_NONO;
	default:
		break;
	}

	return GVIF_NONO;
}

//删除一列
void CTableColume::DelColumn(WORD key)
{
	CMapWordToPtrEx* pMap = GetMap(key);
	if( !pMap->IsEmpty() && pMap->IsStyle(MPEXS_RWCONFIG) )
	{
		TableColProperty* pCol = GetByKey(key);
		if(pCol)
		{
			CString szEntry;
			szEntry.Format("%i",key);

			AfxGetApp()->WriteProfileString(GetMapName(key), szEntry, NULL);
			delete pCol;
		}
		pMap->RemoveKey(key);
	}
}
UINT CTableColume::GetDataTypeByColID(WORD dID)
{
	switch(dID)
	{
		// 一般报价
	case COLUMN_HQ_BASE_NAME:
		//	case COLUMN_HQ_BASE_CODE:
		// 分时明细表
	case COLUMN_HQ_DETAIL_TREND_Time:
		// 盘后明细
	case COLUMN_HQ_DETAIL_TECH_TIME:
		// 大盘明细
	case COLUMN_HQ_DETAIL_INDEX_TIME:
		// 买卖盘
	case COLUMN_HQ_DETAIL_BSORDER_DIRECTION:
		return GVIF_STRING;
	case COLUMN_HQ_DETAIL_SPREAD_GRAPHIC:
		return GVIF_DRAWGRAPHIC;
		// 分价表
	default:
		break;
	}
	return GVIF_NUMBER;
}


// CQuoteReportTable

IMPLEMENT_DYNAMIC(CQuoteReportTable, CHSGridTable)

CQuoteReportTable::CQuoteReportTable(int nRows /* = 0 */, int nCols /* = 0 */, int nFixedRows /* = 0 */, int nFixedCols /* = 0 */)
:CHSGridTable(nRows,nCols,nFixedRows,nFixedCols)
{
	m_nCurSortColumnID = -1;
	m_bAscending = TRUE;
	m_nCurActiveRow = -1;
	m_bAllowDraw = TRUE;
	
	m_pHorScrollBar = NULL;
	m_pVerScrollBar = NULL;

	SetEditable(FALSE);
	SetListMode();
	EnableSelection();
	SetFixedRowSelection(TRUE);
	SetHeaderSort();
	SetGridLines(GVL_NONE);
	SetColumnResize();
	SetRowResize(FALSE);
	SetVirtualMode(FALSE);
	SetDoubleBuffering();
	SetAlwaysShowSelection();
	SetFrameFocusCell(FALSE);

	AllowSelectRowInFixedCol(true);
	EnableDragRowMode();

	SetDefCellHeight(25);
	SetDefCellMargin(2);

	CTableColume::InitColName();
}

CQuoteReportTable::~CQuoteReportTable()
{
}


BEGIN_MESSAGE_MAP(CQuoteReportTable, CHSGridTable)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CString CQuoteReportTable::GetColHeadTemplFileName(int nType /* = 0 */)
{
	CString strPath = CGeneralHelper::GetSpecifyPath(CGeneralHelper::SYSCONFIG_PATH);
	CString strCfgFile;
	strCfgFile.Format("%sTableColGroup.cfg",strPath);
	return strCfgFile;
}

int CQuoteReportTable::LoadColHeadTemplate(CString strTemplName, int nType /* = 0 */, CArray<CStringArray*,CStringArray*>* ayColTemplList /* = NULL */)
{
	CString strFile = GetColHeadTemplFileName(nType);

	struct _stat buf;
	memset(&buf, 0, sizeof(buf));
	_stat(strFile, &buf);
	int nSize = (int)buf.st_size;
	if(nSize <= 0)
	{
		return 0;
	}
	char* strBuf = new char[nSize+1];
	memset(strBuf,0,nSize+1);

	::GetPrivateProfileString(strTemplName,NULL,"",strBuf,nSize,strFile);
	char* p = strBuf;

	int nLen;
	int nTotal = 0;
	int nCount = 0;

	char strItem[1024];
	CStringArray* pArray;
	CString strValue;

	while( *p != '\0' )
	{
		::GetPrivateProfileString(strTemplName,p,"",strItem,1023,strFile);		

		pArray = new CStringArray;

		AnalyzeStr(strItem,";",*pArray);

		if (ayColTemplList)
		{
			ayColTemplList->Add(pArray);
		}

		nLen = strlen(p) + 1;

		nTotal += nLen;
		if( nTotal >= nSize )
			break;

		p += nLen;
		nCount++;

	}

	delete[] strBuf;

	return nCount;

}

CString CQuoteReportTable::GetDefultColGroup(CString strFileName /* = _T */)
{
	CString strRet;
	CWinApp* pApp = AfxGetApp();
	//strRet = pApp->GetProfileString(Table_YlsGridTable_Report_Tab_Head,m_strColumnEntry,"");
	//CfgValues::GetDetailCfgString(m_UserCfgMapOb,Table_YlsGridTable_Report_Tab_Head,m_strColumnEntry,strRet,"");

	if( !strRet.CompareNoCase(Table_GridTable_Default_ColGroup) )
	{
		strRet.Empty();
	}
	if( strRet.IsEmpty() )
	{
		switch( (m_pviInfo.m_nInPageTag & 0x0F00) )
		{
		case PT_BLOCK:                      
		case PT_USERDEFINE:
			{
				strRet = Table_GridTable_Default_ColGroup_Stock;
			}
			return strRet;
		}

		if( MakeMarket(m_pviInfo.m_ciCodeInfo.m_cCodeType) == QH_Bourse 
			|| IsMarket(WP_MARKET, m_pviInfo.m_ciCodeInfo.m_cCodeType) )
		{ 
			// 国际指数
			if( MakeMainMarket(m_pviInfo.m_ciCodeInfo.m_cCodeType) == (WP_MARKET | WP_INDEX) )
			{
				strRet = Table_GridTable_Default_ColGroup_Stock;
			}
			else // 期货
			{
				strRet = Table_GridTable_Default_ColGroup_QH;
			}
		}
		else if( IsMarket(HK_MARKET, m_pviInfo.m_ciCodeInfo.m_cCodeType) )
		{ 
			// 港股
			strRet = Table_GridTable_Default_ColGroup_HK;
		}
		else if( IsMarket(FOREIGN_MARKET, m_pviInfo.m_ciCodeInfo.m_cCodeType) )
		{ 
			// 外汇
			strRet = Table_GridTable_Default_ColGroup_FOREIGN;
		}
		else if(IsFund(m_pviInfo.m_ciCodeInfo.m_cCodeType))
		{ 
			// 基金
			strRet = Table_GridTable_Default_ColGroup_FUND;
		}
		else if( MakeMarket(m_pviInfo.m_ciCodeInfo.m_cCodeType) == STOCK_MARKET 
			&& (KIND_BOND&MakeSubMarket(m_pviInfo.m_ciCodeInfo.m_cCodeType)) == KIND_BOND )
		{ 
			// 债券
			strRet = Table_GridTable_Default_ColGroup_BOND;
		}
		else
		{ 
			// 股票
			strRet = Table_GridTable_Default_ColGroup_Stock;
		}
		return strRet;
	}


	return strRet;

}

int CQuoteReportTable::GetColIndexByID(UINT uiID)
{
	int nPos = -1;
	if (!m_mapColumn.Lookup(uiID,nPos))
	{
		return -1;
	}
	return nPos;
}

CTableColume* CQuoteReportTable::GetColByID(UINT uiID)
{
	int nPos = GetColIndexByID(uiID);
	if (nPos >= 0 && nPos < m_ayColInfo.GetCount())
	{
		CTableColume* pTabCol = m_ayColInfo.GetAt(nPos);
		if (pTabCol && !IsBadReadPtr(pTabCol,1))
		{
			return pTabCol;
		}
	}
	return NULL;
}

CTableColume* CQuoteReportTable::GetHead(int nPos)
{
	if( nPos >= 0 && nPos < m_ayColInfo.GetSize() )
	{
		return m_ayColInfo.GetAt(nPos);
	}

	return NULL;
}


CTableColume* CQuoteReportTable::GetColumn(UINT nID,int& nPos)
{
	nPos = GetColIndexByID(nID);
	if (nPos >= 0 && nPos < m_ayColInfo.GetCount())
	{
		CTableColume* pTabCol = m_ayColInfo.GetAt(nPos);
		if (pTabCol && !IsBadReadPtr(pTabCol,1))
		{
			return pTabCol;
		}
	}
	return NULL;
}

int CQuoteReportTable::SetColumnWidth(int nColID,int nWidth /* = -1 */)
{
	int nOldPos = 0;
	CTableColume* pCol = GetColumn(nColID, nOldPos);
	if(pCol)
		pCol->SetColWidth(nWidth);

	CClientDC dc(this);
	short tmLen = dc.GetTextExtent("0").cx;

	return CHSGridTable::SetColumnWidth(nOldPos,nWidth*tmLen);
}

int CQuoteReportTable::DelColHead(int nBegin /* = -1 */,int nEnd /* = -1 */)
{
	if(nBegin == -1)
		nBegin = 0;
	if(nEnd == -1)
		nEnd = m_ayColInfo.GetCount();
	int i;
	for( i = nBegin; i < nEnd; i++)
	{
		delete m_ayColInfo.GetAt(i);
	}
	for( i = nBegin; m_ayColInfo.GetCount() > nBegin; i++)
	{
		m_ayColInfo.RemoveAt(nBegin);
	}

	m_nFixedCols = 0;

	ResetCorrespondMap();

	return m_ayColInfo.GetCount();
}

void CQuoteReportTable::ResetCorrespondMap()
{
	int nSize = m_ayColInfo.GetSize();
	if( nSize <= 0 )
		return;

	m_mapColumn.RemoveAll();
	m_mapColumn.InitHashTable(nSize + nSize*0.2);
	CTableColume* pCol;
	for(int i = 0; i < nSize; i++)
	{
		pCol = m_ayColInfo.GetAt(i);
		m_mapColumn.SetAt(pCol->m_dID,i);
	}

}

void CQuoteReportTable::InitColGroup(CString strGroupName)
{
	if (m_strCurColGroup == strGroupName)
	{
		return;
	}

	DelColHead();


	CArray<CStringArray*,CStringArray*> ayColTemplList;
	CStringArray* pArray;

	if( strGroupName.IsEmpty() )
	{
		strGroupName = GetDefultColGroup();
	}

	int nCount = LoadColHeadTemplate(strGroupName, 0,&ayColTemplList);
	if( nCount <= 0 || strGroupName == _T("缺省"))
	{
		AddColumn(COLUMN_HQ_BASE_NUMBER  );
		AddColumn(COLUMN_HQ_BASE_CODE		);
		AddColumn(COLUMN_HQ_BASE_NAME		);
		AddColumn(COLUMN_HQ_BASE_NEW_PRICE		);
		AddColumn(COLUMN_HQ_BASE_RISE_RATIO);
		AddColumn(COLUMN_HQ_BASE_TOTAL_HAND);
		AddColumn(COLUMN_HQ_BASE_HAND);
		AddColumn(COLUMN_HQ_BASE_PRECLOSE);
		AddColumn(COLUMN_HQ_BASE_OPEN);
		AddColumn(COLUMN_HQ_BASE_MAX_PRICE);
		AddColumn(COLUMN_HQ_BASE_MIN_PRICE);
		AddColumn(COLUMN_HQ_BASE_BUY_PRICE);
		AddColumn(COLUMN_HQ_BASE_SELL_PRICE);
		AddColumn(COLUMN_HQ_BASE_RISE_VALUE);	
		AddColumn(COLUMN_HQ_BASE_MONEY);
		AddColumn(COLUMN_HQ_BASE_RANGE);
		AddColumn(COLUMN_HQ_EX_EXHAND_RATIO);
		AddColumn(COLUMN_HQ_BASE_ORDER_RATIO);

	}
	else
	{
		WORD wKey = 0;
		int nOldPos = 0;
		int nWidth = 0;
		CTableColume* pCol = NULL;
		for(int i = 0; i < nCount; i++)
		{
			pArray = ayColTemplList.GetAt(i);
			if(pArray->GetSize() > 0)
			{
				wKey = atoi(pArray->GetAt(0));
				AddColumn( wKey );
				if(pArray->GetSize() > 1)
				{
					nWidth = atoi(pArray->GetAt(1));
					SetColumnWidth(wKey,nWidth);
				}
			}
			delete pArray;
		}
	}
}

void CQuoteReportTable::AddColumn(UINT nID)
{
	if( CTableColume::Find(nID) )
	{
		int bFixedCol = FALSE;
		if( nID == COLUMN_HQ_BASE_NAME		|| 
			nID == COLUMN_HQ_BASE_CODE		||
			nID == COLUMN_HQ_BASE_NUMBER	||
			nID == COLUMN_HQ_BASE_INFO_MARK ||
			nID == COLUMN_BSORDER_ORDER		||
			nID == COLUMN_HQ_DETAIL_TREND_Time	 || // 分时明细表
			nID == COLUMN_HQ_DETAIL_TECH_TIME    || // 盘后明细
			nID == COLUMN_HQ_DETAIL_INDEX_TIME   || // 大盘明细
			nID == COLUMN_HQ_DETAIL_SPREAD_PRICE ||	// 成交价
			nID == COLUMN_HQ_BASE_ARROW
			)
		{
			bFixedCol = TRUE;
		}

		int nOldPos = 0;
		CTableColume* pOldCol = GetColumn(nID,nOldPos);
		if( pOldCol != NULL && nOldPos != -1 )
		{
			return;
		}

		CTableColume* pCol = new CTableColume(nID);
		if (!pCol || IsBadReadPtr(pCol,1))
		{
			return;
		}

		//ADD BY YUHJ 2008.5.30
		if(nID == COLUMN_HQ_BASE_OPEN || nID == COLUMN_HQ_BASE_NEW_PRICE)
			pCol->mask |= GVIF_VALUEABS;
		//END
		//20081111 add by fangly begin: 最高，最低排序时对数字取
		if(nID == COLUMN_HQ_BASE_MAX_PRICE || nID == COLUMN_HQ_BASE_MIN_PRICE)
			pCol->mask |= GVIF_VALUEABS;
		if(nID == COLUMN_HQ_BASE_RISE_RATIO)
			pCol->mask &= ~GVIF_VALUEABS;
		//20081111 add by fangly end

		if(nID == COLUMN_HQ_DETAIL_TREND_RISE_VALUE) //20090326 add by fangly end 分时成交涨跌去掉前面的箭头
		{
			pCol->mask = GVIF_SYMBOL|GVIF_VALUEABS;
		}

		if( bFixedCol )
		{		
			pCol->mask |= GVIF_FIXED;
			switch(nID)
			{
			case COLUMN_HQ_BASE_NAME:
				{
					if( GetColumn(COLUMN_HQ_BASE_CODE,nOldPos) != NULL )
					{
						nOldPos++;
						break;
					}
					else if( GetColumn(COLUMN_HQ_BASE_INFO_MARK,nOldPos) != NULL )
					{
						//				nOldPos++;
						break;
					}
					else if( GetColumn(COLUMN_HQ_BASE_ARROW,nOldPos) != NULL )
					{
						break;
					}
					else if( GetColumn(COLUMN_HQ_BASE_NUMBER,nOldPos) != NULL )
					{
						nOldPos++;
						break;
					}
					else
					{
						nOldPos = 0;
					}
				}
				break;
			case COLUMN_HQ_BASE_CODE:
				{
					if( GetColumn(COLUMN_HQ_BASE_NAME,nOldPos) != NULL )
					{
						break;
					}
					else if( GetColumn(COLUMN_HQ_BASE_INFO_MARK,nOldPos) != NULL )
					{
						break;
					}				
					else if( GetColumn(COLUMN_HQ_BASE_ARROW,nOldPos) != NULL )
					{
						break;
					}
					else if( GetColumn(COLUMN_HQ_BASE_NUMBER,nOldPos) != NULL )
					{
						nOldPos++;
						break;
					}
					else
					{
						nOldPos = 0;
					}
				}
				break;
			case COLUMN_HQ_BASE_INFO_MARK:
				{
					if( GetColumn(COLUMN_HQ_BASE_NAME,nOldPos) != NULL )
					{
						nOldPos++;
						break;
					}				
					else if( GetColumn(COLUMN_HQ_BASE_ARROW,nOldPos) != NULL )
					{
						break;
					}
					else if( GetColumn(COLUMN_HQ_BASE_CODE,nOldPos) != NULL )
					{
						nOldPos++;
						break;
					}
					else if( GetColumn(COLUMN_HQ_BASE_NUMBER,nOldPos) != NULL )
					{
						nOldPos++;
						break;
					}
					else
					{
						nOldPos = 0;
					}
				}
				break;

			case COLUMN_HQ_BASE_ARROW:
				{
					if( GetColumn(COLUMN_HQ_BASE_INFO_MARK,nOldPos) != NULL ) 
					{
						nOldPos++;
						break;
					}
					else if( GetColumn(COLUMN_HQ_BASE_NAME,nOldPos) != NULL )
					{
						nOldPos++;
						break;
					}
					else if( GetColumn(COLUMN_HQ_BASE_CODE,nOldPos) != NULL )
					{
						nOldPos++;
						break;
					}
					else if( GetColumn(COLUMN_HQ_BASE_NUMBER,nOldPos) != NULL )
					{
						nOldPos++;
						break;
					}
					else
					{
						nOldPos = 0;
					}
				}
				break;

			case COLUMN_HQ_BASE_NUMBER:
			default: nOldPos = 0; break;
			}

			m_ayColInfo.InsertAt(nOldPos,pCol);			
			ResetMap();
		}
		else
		{
			int nIndex = m_ayColInfo.Add(pCol);
			AddColToMap(pCol->m_dID,nIndex);
		}

		CString strHead;
		for (int i = 0; i < pCol->GetColWidth(); i++)
		{
			strHead.AppendChar('A');
		}

		int nCol = InsertColumn(strHead,pCol->nFormat,nOldPos);
		SetItemText(0,nCol,pCol->GetName());

		if (bFixedCol)
		{
			SetFixedColumnCount(GetFixedColumnCount() + 1);
		}
	}
}

void CQuoteReportTable::ResetMap()
{
	int nSize = m_ayColInfo.GetSize();
	if( nSize <= 0 )
		return;

	m_mapColumn.RemoveAll();
	m_mapColumn.InitHashTable(nSize + nSize*0.2);
	CTableColume* pCol;
	for(int i = 0; i < nSize; i++)
	{
		pCol = m_ayColInfo.GetAt(i);

		AddColToMap(pCol->m_dID,i);
	}

}

void CQuoteReportTable::AddColToMap(int nID,int nColPos)
{
	int nPos;
	if( m_mapColumn.Lookup(nID,nPos) )
	{
		return;
	}
	m_mapColumn.SetAt(nID,nColPos);
}

void CQuoteReportTable::GetClientRect(LPRECT lpRect)
{
	CHSGridTable::GetClientRect(lpRect);
}

void CQuoteReportTable::SetPageViewInfo(PageViewInfo* pvInfo)
{
	memcpy(&m_pviInfo, pvInfo, sizeof(PageViewInfo));
}

StockUserInfo* CQuoteReportTable::GetStock( int nPos )
{
	StockUserInfo* pStock;
	if( nPos >= 0 && nPos < m_pStock.GetSize() )
	{
		pStock = m_pStock.GetAt(nPos);
		//		if( !AfxIsValidAddress(pStock,sizeof(*pStock)) )
		//			return NULL;

		return pStock;
	}
	return NULL;
}

void CQuoteReportTable::SetStock(StockUserInfo* pStock,int nTotal)
{
	if( !pStock || IsBadReadPtr(pStock,1) || nTotal <= 0 )
		return;

	EmptyStock();

	for(int i = 0; i < nTotal; i++)
	{
		if( m_nCurActiveRow == -1 &&
			!memcmp(&pStock[i].m_ciStockCode,&m_pviInfo.m_ciCodeInfo,sizeof(m_pviInfo.m_ciCodeInfo)) )
			m_nCurActiveRow = i + 1;

		m_pStock.Add(&pStock[i]);
	}
}

void CQuoteReportTable::SetStock(CArray<StockUserInfo*,StockUserInfo*>& payStock,int nTotal)
{

}

void CQuoteReportTable::EmptyStock(int nType /* = 0 */)
{
	m_pStock.RemoveAll();
}

void CQuoteReportTable::Sort(int nColumnID, BOOL bAscending)
{
	m_bAscending = bAscending;	

	//HSSortDataStr* pStockKey = NULL;

	//int i;

	//if( (m_nColumn != nColumn) || 
	//	(pStock->GetSize() != m_nDataCount + m_nFixedRow) )
	//{
	//	bChangeSort = TRUE;

	//	m_pStock  = pStock;

	//	m_nColumn = nColumn;

	//	if( ( m_nColumn >= COLUMN_CAPITALIZATION_TOTAL  && m_nColumn <= COLUMN_CAPITALIZATION_A2_GIVE ) ||
	//		( m_nColumn >= COLUMN_FINANCE_TOTAL_ASSETS && m_nColumn <= COLUMN_FINANCE_ASSETS_YIELD ) )
	//	{
	//		if( !SetCWSortData() )
	//			return NULL;
	//	}		
	//	else if( (m_nColumn == COLUMN_HQ_BASE_NAME) ||
	//		(m_nColumn == COLUMN_HQ_BASE_CODE) )
	//	{
	//		//m_nColumn = -1;

	//		if( m_pYlsGridCtrl )  // 删除空的
	//		{
	//			m_nFixedRow = m_pYlsGridCtrl->GetFixedRowCount();
	//			for( i = 0; i < m_nFixedRow && m_pStock->GetSize() > 0; i ++ )
	//			{
	//				m_pStock->RemoveAt(0);
	//			}
	//		}

	//		m_nDataCount = m_pStock->GetSize();
	//		if( m_nDataCount <= 0 )
	//			return NULL;

	//		pStockKey = new HSSortDataStr[m_nDataCount];
	//		for( i = 0; i < m_nDataCount; i++)
	//		{
	//			pStockKey[i].m_pKey = m_pStock->GetAt(i);
	//			if( pStockKey[i].m_pKey == NULL )
	//			{
	//				pStockKey[i].m_szValue[0] = '\0';
	//				continue;
	//			}

	//			if( m_nColumn == COLUMN_HQ_BASE_NAME )
	//			{
	//				pStockKey[i].Copy(pStockKey[i].m_pKey->GetName());
	//			}
	//			else
	//			{
	//				pStockKey[i].Copy(pStockKey[i].m_pKey->m_ciStockCode.GetCode());
	//			}
	//		}
	//	}
	//}

	//m_bAscending = bAscending;

	//if( bChangeSort )
	//{
	//	KQuickSort qsData;
	//	if( pStockKey )
	//	{
	//		qsData.m_pData      = pStockKey;
	//		qsData.m_nDataLen   = sizeof(HSSortDataStr);
	//		qsData.m_nKeyOffset = sizeof(void*);
	//		qsData.m_nKeyLen    = 64;
	//		qsData.m_nLow       = 0;
	//		qsData.m_nHigh      = (short)(m_nDataCount - 1);
	//	}
	//	else
	//	{
	//		qsData.m_pData      = m_pData;
	//		qsData.m_nDataLen   = sizeof(HSSortData);
	//		qsData.m_nKeyOffset = sizeof(void*);
	//		qsData.m_nKeyLen    = QS_DOUBLE;
	//		qsData.m_nLow       = 0;
	//		qsData.m_nHigh      = (short)(m_nDataCount - 1);
	//	}

	//	//
	//	if( m_bAscending )
	//	{
	//		QuickSortAscend2(&qsData);
	//	}
	//	else
	//	{
	//		QuickSortDescend2(&qsData);
	//	}

	//	m_pStock->RemoveAll();
	//	if( m_pYlsGridCtrl )
	//	{
	//		m_pStock->SetSize(m_nFixedRow);
	//	}
	//	//

	//	if( pStockKey )
	//	{
	//		HSSortDataStr* pData = pStockKey;
	//		for(i = 0; i < m_nDataCount; i++,pData++)
	//		{
	//			m_pStock->Add(pData->m_pKey);
	//		}
	//		delete[] pStockKey;
	//		m_nDataCount = 0;
	//	}
	//	else
	//	{
	//		HSSortData* pData = m_pData;
	//		for(i = 0; i < m_nDataCount; i++,pData++ )
	//		{
	//			if( pData->m_pKey )
	//			{
	//				m_pStock->Add(pData->m_pKey);
	//			}
	//		}
	//	}
	//}

	//return m_pData;

}

CFont* CQuoteReportTable::GetColHeadFontPtr(int nType)
{
	//CFont* pFont = GetStockObject();
	//if( pFont != NULL )
	//{
	//	return pFont;
	//}

	//if(IsLargeFont())
	//{
	//	return g_hxData.ftLargeFont.m_pFont;
	//}

	//if( nType == 0 )  // 列头字体
	//{
	//	pFont =  g_hxData.ftGridHead.m_pFont;		
	//}
	//else if( nType == 1 )  // 列头字体
	//{
	//	hxTableColume* pCol = (hxTableColume*)GetItemData(0, 0);

	//	if((pCol->m_dID == COLUMN_HQ_DETAIL_TREND_Time)) //if语句，qinhn 2008-11-11加入实现对成交明细表时间列字体的设置管理
	//		pFont =  g_hxData.ftTradeTime.m_pFont;   
	//	else
	//		pFont =  g_hxData.ftGridFixed.m_pFont;

	//	//qinhn 20100127 注释 避免"箭"列的小箭头在有主推数据过来时会右移的现象(有主推时其字体用代码名称列字体，而最初其用的是报价固定列
	//	//字体,因两种字体大小不一样，导致在计算小箭头位置的时候出现偏差，故会出现小箭头右移的现象)
	//	//		if (pCol && COLUMN_HQ_BASE_NUMBER == pCol->m_dID)
	//	//		{
	//	//			pFont =  g_hxData.ftGridSeqNum.m_pFont;
	//	//		}
	//	//qinhn 20100127 注释 End
	//}
	//else
	//{
	//	pFont =  g_hxData.ftGridDynamic.m_pFont;						
	//}
	return NULL/*pFont*/;

}

int CQuoteReportTable::ChangeContent(PageViewInfo* pvInfo)
{
	if (pvInfo == NULL)
	{
		return -1;
	}

	// 判断是否跟当前页面信息相同，若为显示板块则不判断（板块的成员股票可能变更）
	if(pvInfo != NULL)
	{
		BOOL bComp = 1;
		switch(pvInfo->m_nInPageTag & 0x0F00)
		{
		case PT_INDEX:
		case PT_TYPE:
			break;
		case PT_BLOCK:                                                     
		case PT_USERDEFINE:
			bComp = 0;
			break;
		}

		if ( bComp )
		{
			if( !memcmp(pvInfo, &m_pviInfo,sizeof(PageViewInfo)) &&
				(GetRowCount() - GetFixedRowCount()) != 0 )
			{
				return 1;
			}
		}
	}

	SetPageViewInfo(pvInfo);


	int nTotal = 0;

	EmptyStock();

	//mapReplaceColText(pvInfo);

	// 获取当前板块的股票列表信息
	const StockUserInfo* pStock = NULL;
	pStock = GetStockUserInfo(pvInfo,nTotal,&m_pStock,NULL);


	// 若是深圳A股板块，则附加中小板块代码
	if((pvInfo->m_ciCodeInfo.m_cCodeType == (SZ_Bourse | KIND_STOCKA)) && (pvInfo->GetBlockName().Trim().IsEmpty())) //防止系统板块加载多次
	{
		// 获取中小板股票
		pStock = GetStockUserInfo(pvInfo,nTotal,&m_pStock,NULL);
	}


	// 
	if( (pStock != NULL) && (nTotal > 0) )
	{
		//SetStock((StockUserInfo*)pStock,nTotal);
	}
	else if( nTotal > 0 )
	{
		// 是否为股指期货,是则添加沪深300指数
		if( MakeGZIndex(pvInfo->m_ciCodeInfo.m_cCodeType) )
		{
			//for test
			StockUserInfo* pInfo = g_pStock;
			if( pInfo != NULL )
			{
				m_pStock.Add(pInfo);
			}
		}

		// 去除错误代码
		CArray<int,int> ay;
		for( int i = 0; i < m_pStock.GetSize(); i++ )
		{
			pStock = m_pStock.GetAt(i);
			if( pStock && 
				(pStock->m_ciStockCode.m_cCodeType == 0 || pStock->m_ciStockCode.m_cCode[0] == '\0'))
			{
				ay.Add(i);
			}
		}

		for( int i = ay.GetSize() - 1; i >= 0; i-- )
		{
			m_pStock.RemoveAt(ay.GetAt(i));
		}

	}
	else
	{
		EmptyStock();
	}

	// 默认按照代码排序
	if(pvInfo != NULL && pvInfo->m_lPageType == PriceAnalisys1)//SpecialReport)
	{
		if( ((pvInfo->m_nInPageTag & 0x0F00) == PT_BLOCK) ||   // 板块
			((pvInfo->m_nInPageTag & 0x0F00) == PT_USERDEFINE) // 自选
			)
		{
		}
		else
		{
			Sort(COLUMN_HQ_BASE_CODE,TRUE);
		}
		FindStockPos(m_pStock,pvInfo->m_ciCodeInfo,m_nCurActiveRow);

	}

	CString strColGroupName = GetDefultColGroup();
	InitColGroup(strColGroupName);

	InitRowData();
	

	return 0;

}

void CQuoteReportTable::InitRowData()
{
	CArray<StockUserInfo*,StockUserInfo*>* pArray = &m_pStock;
	if(pArray == NULL)
		return;

	InitFixedDataItem(0,m_pStock.GetCount());
}

void CQuoteReportTable::InitFixedDataItem(int nFirst,int nCount)
{
	try
	{
		SetRowCount(nCount);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}

	// 行
	StockUserInfo* pStock = NULL;
	CGridCellBase* pCell;

	int row,col,nDataStart;

	if( m_idCurrentCell.row == -1 &&
		!(m_idCurrentCell.row >= nFirst && m_idCurrentCell.row <= nFirst + nCount) )
	{
		m_idCurrentCell.row = m_nFixedRows;
		m_idCurrentCell.col = m_nFixedCols;
	}

	CCellRange cellRange = GetVisibleNonFixedCellRange();
	for ( row = cellRange.GetMinRow(),nDataStart = nFirst + 1; 
		row <= cellRange.GetMaxRow(); row++,nDataStart++)
	{
		pStock = GetStock( nDataStart - 1 );
		if( pStock )
		{
			for ( col = 0; col < GetColumnCount()/* && col < maxVisibleCol*/; col++)
			{
				pCell = GetCell(row, col);
				if( pCell == NULL )
					continue;

				SetFixedData( row,col,pCell,pStock, nDataStart,FALSE,NULL );

				if( m_idCurrentCell.row == row )
				{
					pCell->SetState(pCell->GetState() | GVIS_SELECTED | GVIS_LIGHT);
					m_SelectedCellMap.SetAt(MAKELONG(row,col),CCellID(row,col));
				}
			}
		}
	}

	this->Invalidate();
}

void CQuoteReportTable::SetFixedData(int nRow,int nCol,CGridCellBase* pCell, StockUserInfo* pStock /* = NULL */,
									 int nNumber /* = -1 */, BOOL bRefresh /* = TRUE */, CDC* pDC /* = NULL */)
{
	CTableColume* pCol = this->GetHead(nCol);
	if( pCol == NULL )
		return;

	CString strText; 
	//财务数据 股本数据
	if( ( pCol->m_dID >= COLUMN_CAPITALIZATION_TOTAL && pCol->m_dID <= COLUMN_CAPITALIZATION_A2_GIVE ) 
		||	( pCol->m_dID >= COLUMN_FINANCE_TOTAL_ASSETS && pCol->m_dID <= COLUMN_FINANCE_ASSETS_YIELD ) )
	{
//		pCell->Empty();

		//if( m_pYlsSort.IsValid() && pCol->m_dID == m_pYlsSort.m_nColumn )
		//{
		//	strText.Format("%.3f",m_pYlsSort.GetData(nNumber - m_pYlsSort.m_nFixedRow,pStock) );
		//	if(strText[0] == '-')
		//	{
		//		strText.Insert(0, ' ');
		//	}
		//	pCell->szText = strText;
		//}
		//else if( pStock )
		//{
		//	FinanceData* pFince = CHSDisk::GetFinaceData(&pStock->m_ciStockCode);
		//	if (pFince != NULL)
		//	{
		//		CHSFinanceData YlsFinaceData;
		//		memcpy(&YlsFinaceData,pFince,sizeof(FinanceData));

		//		HSDouble hxData = YlsFinaceData.GetDataByID(pCol->m_dID);
		//		if( (long)hxData == hx_DefMaxValues )
		//		{
		//			hxData = 0;
		//		}
		//		strText.Format("%.3f",hxData);

		//		if(strText[0] == '-')
		//		{
		//			strText.Insert(0, ' ');
		//		}
		//		pCell->szText = strText;//hxSubZero(strText);
		//	}
		//}
		//else
		//{
		//	bRefresh = FALSE;
		//}
	}
	else if (pCol->m_dID >= COLUMN_BLOCK_BEGIN && pCol->m_dID <= COLUMN_BLOCK_END)		//add by lxqi 20090706	板块信息
	{
//		pCell->Empty();

		//if( pStock )
		//{
		//	BlockInfoEx* pBlockInfo = NULL;
		//	if (g_hxData.m_mapBlockInfoEx.Lookup(m_strColumnEntry,pBlockInfo))
		//	{
		//	}
		//	else
		//	{
		//		pBlockInfo = new BlockInfoEx;
		//		pBlockInfo->m_pStock = pStock;
		//		g_hxData.GetBlockMember(m_strColumnEntry,NULL,NULL,&pBlockInfo->m_pMapBlockMember);
		//		g_hxData.m_mapBlockInfoEx.SetAt(m_strColumnEntry,pBlockInfo);
		//	}

		//	BlockMemberInfo* pMemberInfo = NULL;
		//	if (pBlockInfo->m_pMapBlockMember.Lookup(pStock->GetCode(),(void*&)pMemberInfo))
		//	{
		//		if (pMemberInfo != NULL && !pMemberInfo->m_mapUserItem.Lookup(pCol->m_dID,strText))
		//		{
		//			strText = "";
		//		}
		//	}
		//	pCell->szText = strText;
		//}
		//else
		//{
		//	bRefresh = FALSE;
		//}
	}
	else
	{
//		pCell->Empty();

		switch(pCol->m_dID)
		{
		case COLUMN_HQ_BASE_INFO_MARK:
			if( pStock )
			{
				pCell->SetText(pStock->m_ciStockCode.GetCode());

				//// 加入地雷标记
				//void* pData;
				//if( m_mapCodeMark.Lookup(pCell->szText,pData) )
				//{
				//	pCell->szText = "*";
				//}
				//else
				//{
				//	pCell->szText = "";
				//}
			}
			else
			{
				pCell->SetText("");
			}
			break;
		case COLUMN_HQ_BASE_CODE: // 股票代码
			if( pStock )
			{				
				pCell->SetText(pStock->GetCode());

				pCell->SetData((LPARAM)pStock);
			}
			break;
		case COLUMN_HQ_BASE_NAME: 		// 股票名称
			if( pStock )
			{
				pCell->SetText(pStock->GetName());
				pCell->SetData((LPARAM)pStock);
			}
			break;
		case COLUMN_HQ_BASE_NUMBER:   	// 序号
			{
				if( nNumber == -1 )
				{
					CString strTemp;
					strTemp.Format("%i",nRow - m_nFixedRows + 1);
					pCell->SetText(strTemp);
				}
				else
				{
					CString strTemp;
					strTemp.Format("%i",nNumber);
					pCell->SetText(strTemp);
				}				
			}
			break;
		case COLUMN_HQ_BASE_PRECLOSE:   // 昨收
			{
				if( pStock == NULL )
					break;

				// for test
				//CHSChildFrame* pFrame = (CHSChildFrame*)GetYlsChildFrame();
				//long  lClose   = pStock->m_lPrevClose;
				////long nUnit    = pFrame->GetStockPriceUnit(pStock->m_ciStockCode.m_cCodeType);
				//long nUnit    = pFrame->GetStockPriceUnit(&pStock->m_ciStockCode);
				//short nDecimal = pFrame->GetStockPriceDecimal(&pStock->m_ciStockCode);
				//strText.GetBuffer(64);
				//LongToString(pStock->m_lPrevClose, pStock->m_lPrevClose, nUnit, nDecimal, (char*)LPCSTR(strText));
				//strText.ReleaseBuffer();
				pCell->SetText(_T("-昨收-"));
			}
			break;
			// gbq test 20060620
		case COLUMN_HQ_BASE_ARROW:
			{
				pCell->SetText("!");
			}
			break;
			// gbq end
		default:
			{
				bRefresh = FALSE;
			}
			break;
		}
	}

	pCell->SetFormat(pCol->GetColProperty()->m_nColFormat);

	//pCell->SetFormat(pCol->GetColFormat())
	//if( pCell )
	//	FillColWidth(nCol,pCell->szText.GetLength());

	if( bRefresh )
	{
		if (GetDoubleBuffering())
		{
			CMemDC memDC(pDC);
			RedrawCell( nRow, nCol, &memDC );
		} 
		else
		{
			RedrawCell( nRow, nCol, pDC );
		}
	}
}

BOOL CQuoteReportTable::SetHorScrollBarRef(CSkinScrollBar* pNewHorScrollBar /* = NULL */)
{
	if (pNewHorScrollBar && !IsBadReadPtr(pNewHorScrollBar,1))
	{
		EnableScrollBars(SB_HORZ,FALSE);
		m_pHorScrollBar = pNewHorScrollBar;
		ResetScrollBars();
	} 
	else
	{
		EnableScrollBars(SB_HORZ);
		m_pHorScrollBar = NULL;
	}
	return TRUE;
}

BOOL CQuoteReportTable::SetVerScrollBarRef(CSkinScrollBar* pNewVerScrollBar /* = NULL */)
{
	if (pNewVerScrollBar && !IsBadReadPtr(pNewVerScrollBar,1))
	{
		EnableScrollBars(SB_VERT,FALSE);
		m_pVerScrollBar = pNewVerScrollBar;
		ResetScrollBars();
	} 
	else
	{
		EnableScrollBars(SB_VERT);
		m_pVerScrollBar = NULL;
	}
	return TRUE;
}

void CQuoteReportTable::ResetScrollBars()
{
	// Force a refresh. 
	m_idTopLeftCell.row = -1;

	if (!m_bAllowDraw || !::IsWindow(GetSafeHwnd())) 
		return;

	CRect rect;

	// This would have caused OnSize event - Brian 
	//EnableScrollBars(SB_BOTH, FALSE); 

	GetClientRect(rect);

	if (rect.left == rect.right || rect.top == rect.bottom)
		return;

	if (IsVisibleVScroll())
		rect.right += GetSystemMetrics(SM_CXVSCROLL) + GetSystemMetrics(SM_CXBORDER);

	if (IsVisibleHScroll())
		rect.bottom += GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CYBORDER);

	rect.left += GetFixedColumnWidth();
	rect.top += GetFixedRowHeight();


	if (rect.left >= rect.right || rect.top >= rect.bottom)
	{
		if (m_pVerScrollBar && IsBadReadPtr(m_pVerScrollBar,1))
		{
 			//m_pVerScrollBar->EnableScrollBarCtrl(SB_VERT, FALSE);
		} 
		else if (m_pHorScrollBar && !IsBadReadPtr(m_pHorScrollBar,1))
		{
// 			m_pHorScrollBar->EnableScrollBarCtrl(SB_HORZ, FALSE);
		}
		EnableScrollBarCtrl(SB_BOTH, FALSE);
		return;
	}

	CRect VisibleRect(GetFixedColumnWidth(), GetFixedRowHeight(), 
		rect.right, rect.bottom);
	CRect VirtualRect(GetFixedColumnWidth(), GetFixedRowHeight(),
		GetVirtualWidth(), GetVirtualHeight());

	// Removed to fix single row scrollbar problem (Pontus Goffe)
	// CCellRange visibleCells = GetUnobstructedNonFixedCellRange();
	// if (!IsValid(visibleCells)) return;

	//TRACE(_T("Visible: %d x %d, Virtual %d x %d.  H %d, V %d\n"), 
	//      VisibleRect.Width(), VisibleRect.Height(),
	//      VirtualRect.Width(), VirtualRect.Height(),
	//      IsVisibleHScroll(), IsVisibleVScroll());

	// If vertical scroll bar, horizontal space is reduced
	if (VisibleRect.Height() < VirtualRect.Height())
		VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);
	// If horz scroll bar, vert space is reduced
	if (VisibleRect.Width() < VirtualRect.Width())
		VisibleRect.bottom -= ::GetSystemMetrics(SM_CYHSCROLL);

	// Recheck vertical scroll bar
	//if (VisibleRect.Height() < VirtualRect.Height())
	// VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);

	if (VisibleRect.Height() < VirtualRect.Height())
	{
		if (m_pVerScrollBar)
		{
// 			m_pVerScrollBar->EnableScrollBarCtrl(SB_VERT, TRUE);
		} 
		else
		{
			EnableScrollBars(SB_VERT, TRUE); 
		}
		m_nVScrollMax = VirtualRect.Height() - 1;
	}
	else
	{
		if (m_pVerScrollBar)
		{
// 			m_pVerScrollBar->EnableScrollBarCtrl(SB_VERT, FALSE);
		} 
		else
		{
			EnableScrollBars(SB_VERT, FALSE); 
		}
		m_nVScrollMax = 0;
	}

	if (VisibleRect.Width() < VirtualRect.Width())
	{
		if (m_pHorScrollBar)
		{
// 			m_pHorScrollBar->EnableScrollBarCtrl(SB_HORZ, TRUE);
		} 
		else
		{
			EnableScrollBars(SB_HORZ, TRUE); 
		}
		m_nHScrollMax = VirtualRect.Width() - 1;
	}
	else
	{
		if (m_pHorScrollBar)
		{
// 			m_pHorScrollBar->EnableScrollBarCtrl(SB_HORZ, FALSE);
		} 
		else
		{
			EnableScrollBars(SB_HORZ, FALSE); 
		}
		m_nHScrollMax = 0;
	}

	ASSERT(m_nVScrollMax < INT_MAX && m_nHScrollMax < INT_MAX); // This should be fine

	// New code - Paul Runstedler 
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nPage = (m_nHScrollMax>0)? VisibleRect.Width() : 0;
	si.nMin = 0;
	si.nMax = m_nHScrollMax;
	if (m_pHorScrollBar)
	{
		m_pHorScrollBar->SetScrollInfo(&si, TRUE);
	} 
	else
	{
		SetScrollInfo(SB_HORZ, &si, TRUE);
	}

	si.fMask |= SIF_DISABLENOSCROLL;
	si.nPage = (m_nVScrollMax>0)? VisibleRect.Height() : 0;
	si.nMin = 0;
	si.nMax = m_nVScrollMax;
	if (m_pVerScrollBar)
	{
		m_pVerScrollBar->SetScrollInfo(&si, TRUE);
	} 
	else
	{
		SetScrollInfo(SB_VERT, &si, TRUE);
	}

}

int CQuoteReportTable::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
	if (nBar == SB_HORZ)
	{
		if (m_pHorScrollBar && !IsBadReadPtr(m_pHorScrollBar,1))
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);

			if (bGetTrackPos)
			{
				if (m_pHorScrollBar->GetScrollInfo(&si, SIF_TRACKPOS))
					return si.nTrackPos;
			}
			else
			{
				if (m_pHorScrollBar->GetScrollInfo(&si, SIF_POS))
					return si.nPos;
			}

			return 0;
		} 
		else
		{
			return CHSGridTable::GetScrollPos32(nBar,bGetTrackPos);
		}
	} 
	else
	{
		if (m_pVerScrollBar && !IsBadReadPtr(m_pVerScrollBar,1))
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);

			if (bGetTrackPos)
			{
				if (m_pVerScrollBar->GetScrollInfo(&si, SIF_TRACKPOS))
					return si.nTrackPos;
			}
			else
			{
				if (m_pVerScrollBar->GetScrollInfo(&si, SIF_POS))
					return si.nPos;
			}

			return 0;
		} 
		else
		{
			return CHSGridTable::GetScrollPos32(nBar,bGetTrackPos);
		}
	}
}

BOOL CQuoteReportTable::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
	m_idTopLeftCell.row = -1;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_POS;
	si.nPos   = nPos;

	if (nBar == SB_HORZ
		&& m_pHorScrollBar && !IsBadReadPtr(m_pHorScrollBar,1))
	{
		return m_pHorScrollBar->SetScrollInfo(&si, bRedraw);
	} 
	else if (nBar == SB_VERT
		&& m_pVerScrollBar && !IsBadReadPtr(m_pVerScrollBar,1))
	{
		return m_pVerScrollBar->SetScrollInfo(&si, bRedraw);
	}
	return SetScrollInfo(nBar, &si, bRedraw);
}

CString CQuoteReportTable::GetMenuName(int nType /* = 0 */, int nSubType /* = 0 */)
{
	return CHSGridTable::GetMenuName(nType,nSubType);
}

BOOL CQuoteReportTable::ShowRow(int row, BOOL bRefresh /* = FALSE */)
{
	BOOL bRes = SetRowHeight(row,m_cellFixedRowDef.GetHeight());
	if (bRefresh)
	{
		Refresh();
	}
	return bRes;
}
BOOL CQuoteReportTable::ShowCol(int col, BOOL bRefresh /* = FALSE */)
{
	CTableColume* pCol = m_ayColInfo.GetAt(col);
	int nWidth = 1;
	if (pCol)
	{
		nWidth = pCol->GetColWidth();
	}
	BOOL bRes = SetColumnWidth(col,nWidth);
	if (nWidth == 1)
	{
		AutoSizeColumn(col);
	}
	if (bRefresh)
	{
		Refresh();
	}
	return bRes;
}

BOOL CQuoteReportTable::MoveRowTo(int nSrc,int nDes)
{
	if (!CHSGridTable::MoveRowTo(nSrc, nDes))
	{
		return FALSE;
	}

	StockUserInfo* pStock = m_pStock.GetAt(nSrc);
	if (pStock)
	{
		if (nDes == -1)
		{
			m_pStock.Add(pStock);
		} 
		else
		{
			m_pStock.InsertAt(nDes,pStock);
			if (nDes <= nSrc)
			{
				m_nCurActiveRow++;
				nSrc++;
			}
		}
		m_pStock.RemoveAt(nSrc);
		return TRUE;
	}
	return FALSE;

}

BOOL CQuoteReportTable::MoveColTo(int nSrc,int nDes)
{
	if (CHSGridTable::MoveColTo(nSrc,nDes))
	{
		if (nDes < 0)
		{

		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// CQuoteReportTable 消息处理程序

void CQuoteReportTable::OnDraw(CDC* pDC)
{
	CHSGridTable::OnDraw(pDC);
}
void CQuoteReportTable::OnLButtonDown(UINT nFlags, CPoint point)
{
	CHSGridTable::OnLButtonDown(nFlags, point);
}
