#include "stdafx.h"
#include "CTableColume.h"
#include "hsothercoldefine.h"
#include "hscoldefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
		crFgClr = GetFgClrByColID(dID);
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
		return 0/*pProperty->m_nRowMask*/;
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
		return 0/*pProperty->m_nRowForma*/;
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
		return 0/*pCol->m_nRowMask*/;
	}
	return ROWMASK_DEFAULT;
}

UINT CTableColume::GetRowFormat(WORD key)
{
	TableColProperty* pCol = GetByKey(key);
	if(pCol != NULL)
	{
		return 0/*pCol->m_nRowFormat*/;
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