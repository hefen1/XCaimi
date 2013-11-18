
#include "stdafx.h"
#include "ExpPublic.h"
#include "Tools.h"
// 错误信息
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern int GetStockByMaket(unsigned short nMarket,  CArray<StockUserInfo>& stockArray); 
CTime LongToDate(unsigned long lDate)
{
	try
	{
		lDate %= 100000000;
		if( lDate <= 0 )
		{
			lDate = 0 - lDate;
			return 0;
		}


		int nYear = int(lDate/10000);
		int nMonth, nDay, nHour, nMinute;
		if(nYear < 1970)
		{
			CTime curTime = CTime::GetCurrentTime();

			nMonth  = nYear/100;
			nDay    = nYear%100;
			nHour   = (int)((lDate%10000)/100);
			nMinute = (int)(lDate%100);
			nYear = curTime.GetYear();
		}
		else if(nYear < 2038)
		{
			nMonth  = (int)((lDate%10000)/100);
			nDay    = (int)(lDate%100);
			nHour   = 0;
			nMinute = 0;
			//if(nYear > 2038)  // ??
			//	nYear = 2038;
		}
		else if(nYear >= 2038)
		{
			nYear = 2038 - 1;
			nMonth  = nYear/100;
			nDay    = nYear%100;
			nHour   = (int)((lDate%10000)/100);
			nMinute = (int)(lDate%100);
		}

		if(nDay < 1 || nDay > 31)
			nDay = 1;
		if(nMonth < 1 || nMonth > 12)
			nMonth = 1;
		if(nYear == 1970 && nMonth == 1 && nDay < 8 && nHour == 0)//这个时间CTime会报错
			nHour = 8;
		return CTime(nYear,nMonth,nDay,nHour,nMinute,0);
	}
	catch(...){}
	return 0;
}
nodErrorInfo::nodErrorInfo()
{
	memset(this,0,sizeof(nodErrorInfo));
}

nodErrorInfo::~nodErrorInfo()
{
	if(m_pError != NULL)
	{
		delete m_pError;
		m_pError = NULL;
	}
	if(m_pHelp != NULL)
	{
		delete m_pHelp;
		m_pHelp = NULL;
	}
}

nodErrorInfo::nodErrorInfo(int nError,int nBegin /*= -1*/,int nEnd /*= -1*/,
						   const char* pError, const char* pHelp)
{
	m_nError = nError;
	m_nBegin = nBegin;
	m_nEnd   = nEnd;  
	if( pError && !IsBadReadPtr(pError,1) )
	{
		m_pError = new CString;
		m_pError->Format("%s",pError);
	}
	else
	{ 
		m_pError = NULL;
	}
	if(  pError && !IsBadReadPtr(pError,1) )
	{
		m_pHelp = new CString;
		m_pHelp->Format("%s",pHelp);
	}
	else
	{
		m_pHelp = NULL;
	}
}

CString  nodErrorInfo::GetError()     // 错误信息
{
	if(m_pError != NULL)
	{
		return *m_pError;
	}
	return "";
}
CString  nodErrorInfo::GetHelp()      // 提示、帮助
{
	if(m_pHelp != NULL)
	{
		return *m_pHelp;
	}
	return "";
}

// 通用参数
LPNODELEMENT FormulaGeneralParam::GetParam(int nPos)
{
	if(m_ayVars == NULL) return NULL;
	if(nPos >= 0 && nPos < m_ayVars->GetSize())
	{
		return m_ayVars->GetAt(nPos);
	}
	return NULL;
}

int FormulaGeneralParam::GetSize()
{
	if(m_ayVars == NULL) return 0;
	return m_ayVars->GetSize();
}

void FormulaGeneralParam::Add(LPNODELEMENT pParam)
{
	if(pParam != NULL)
	{
		GetArray()->Add(pParam);
	}
}

void FormulaGeneralParam::InsertAt(int nIndex,LPNODELEMENT pParam)
{
	if(pParam != NULL)
	{
		GetArray()->InsertAt(nIndex,pParam);
	}
}

CArray<LPNODELEMENT,LPNODELEMENT>* FormulaGeneralParam::GetArray()
{
	if(m_ayVars == NULL)
	{
		m_ayVars = new CArray<LPNODELEMENT,LPNODELEMENT>;
	}
	return m_ayVars;
}

FormulaGeneralParam::FormulaGeneralParam()
{
	m_ayVars = NULL;
}

FormulaGeneralParam::~FormulaGeneralParam()
{
	if(m_ayVars != NULL)
	{
		delete m_ayVars;
		m_ayVars = NULL;
	}
}


HSCurDataInfoStatus::HSCurDataInfoStatus(DWORD dwStatus /*= 0*/)
{
	Empty();
	m_dwStatus = dwStatus;

	this->m_pCalcParam = NULL;
}

HSCurDataInfoStatus::HSCurDataInfoStatus(HSCurDataInfoStatus* pCurDataInfoStatus)
{
	Empty();

	if( pCurDataInfoStatus )
	{
		m_pDataSource      = pCurDataInfoStatus->m_pDataSource;
		this->m_pCalcParam = pCurDataInfoStatus->m_pCalcParam;
	}
}

HSCurDataInfoStatus::~HSCurDataInfoStatus()
{
	Free();
}

void HSCurDataInfoStatus::Empty()
{
	memset(this,0,sizeof(HSCurDataInfoStatus));
}

void HSCurDataInfoStatus::SetStatus(DWORD dwStatus)    { m_dwStatus = dwStatus; }
BOOL HSCurDataInfoStatus::IsStatus(DWORD dwStatus)     { return (m_dwStatus & dwStatus); }
void HSCurDataInfoStatus::AddStatus(DWORD dwStatus)    { m_dwStatus |= dwStatus; }
void HSCurDataInfoStatus::RemoveStatus(DWORD dwStatus) { m_dwStatus &= (~dwStatus); }

void HSCurDataInfoStatus::Alloc( int nCurSize,HSDouble defValues /*= hx_DefMinValues*/ )
{		
	if( nCurSize <= 0 )
	{
		Free();
		return;
	}		
	if(m_lSize != nCurSize)
	{
		Free();

		//m_pData = new HSDouble[nCurSize];

		if( HSDrawData::m_pHSExternAlloc )
		{
			m_pData = (HSDouble*)(*HSDrawData::m_pHSExternAlloc)(nCurSize,HSDoubleType);

			m_lSize = nCurSize;
		}
// 		m_pData = new HSDouble[nCurSize];
// 		m_lSize = nCurSize;
// 		//m_lSize = nCurSize;
	}
	if( m_pData != NULL )
	{
		SetDefaultValues( m_pData,nCurSize,defValues );
	}
}

void HSCurDataInfoStatus::Free()
{
	if ( m_pData != NULL )
	{
		if( !IsStatus(hx_CURRENTDATENOTDELETE) )
		{
			if( HSDrawData::m_pHSExternFree )
			{
				(*HSDrawData::m_pHSExternFree)((void*)m_pData,HSDoubleType);
				//m_pData = NULL;
			}
			//delete[] m_pData;
		}
		m_pData = NULL;
		m_lSize = 0;
	}
}

CString HSCurDataInfoStatus::GetCatStr()
{
	CString strRet;
	for(int i = 0; m_pData && i < m_lSize; i++ )
	{
		if( m_pData[i].m_pOtherData &&
			m_pData[i].m_pOtherData->m_cText )
		{
			CString strText;
			strText.Format("%s",m_pData[i].m_pOtherData->m_cText );
			strRet += strText;
		}
	}
	return strRet;
}


// 绘图返回数据
CFormulaDrawParam::CFormulaDrawParam()
{
	memset(this,0,sizeof(CFormulaDrawParam));
}

BlockTypeInfo::~BlockTypeInfo()
{
	Free();
}

void BlockTypeInfo::Free()
{
	for(int i = 0; i < ayBlock.GetSize(); i++)
	{
		delete ayBlock.GetAt(i);
	}
	ayBlock.RemoveAll();


	ayCalcStock.RemoveAll();

	for( int i = 0; i < ayOtherData.GetSize(); i++)
	{
		delete ayOtherData.GetAt(i);
	}
	ayOtherData.RemoveAll();

	for(int  i = 0; i < ayCalcRet.GetSize(); i++)
	{
		delete ayCalcRet.GetAt(i);
	}
	ayCalcRet.RemoveAll();
}
CDataSourceUseData::CDataSourceUseData()
{ 
	m_pData = NULL;
	m_nSize = 0;
}

CDataSourceUseData::~CDataSourceUseData()
{
	Free();
}

HSDouble* CDataSourceUseData::GetData() { return m_pData; }
BOOL      CDataSourceUseData::IsEmpty() { return m_pData == NULL; }	

CDataSourceUseData* CDataSourceUseData::Alloc(int nCurSize,int nOldSize,
											  long pLastDay /*= 0*/)
{
	if( nCurSize <= 0 )
	{
		Free();
		return NULL;
	}
	if( (nCurSize != nOldSize) || (m_pData == NULL) )
	{
		if( pLastDay != 0 )	 // 
		{
			HSDouble* pData = m_pData;
			m_pData = new HSDouble[nCurSize];

			// 复制旧数据
			if ( pData != NULL && m_nSize < nCurSize )
			{
				memcpy(m_pData,pData,sizeof(HSDouble)*m_nSize);
				SetDefaultValues(&m_pData[m_nSize],1,hx_DefMinValues);

				delete[] pData;
				pData = NULL;
			}
			else
			{
				SetDefaultValues(m_pData,nCurSize,hx_DefMinValues);
			}

			m_nSize = nCurSize;
			return this;
		}
		else
		{
			Free();
			m_pData = new HSDouble[nCurSize];			
			m_nSize = nCurSize;
		}
	}
	if(m_pData != NULL)
	{
		SetDefaultValues(m_pData,nCurSize,hx_DefMinValues);
	}

	return this;
}

void CDataSourceUseData::Free()
{
	if ( m_pData != NULL )
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	m_nSize = 0;
}


// 组合条件选股结构
void FormulaCompareSubData::Copy(FormulaCompareSubData* pData)
{
	if( pData == NULL ) return;

	m_cType  = pData->m_cType;
	m_dData1 = pData->m_dData1;
	m_dData2 = pData->m_dData2;
}

FormulaCompareSubData::FormulaCompareSubData()
{
	m_cType = -1;
}
void FormulaCompareSubData::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_cType;
		ar << m_dData1;
		ar << m_dData2;			// 
	}
	else
	{
		ar >> m_cType;
		ar >> m_dData1;
		ar >> m_dData2;		
	}
}

SelStockCondition::SelStockCondition()
{
	memset(this,0,sizeof(SelStockCondition));
}
void SingleSelCondition::Copy(SingleSelCondition* pData)
{
	if( pData == NULL ) return;

	m_strExp  = pData->m_strExp;
	m_nType   = pData->m_nType;
	m_nPeriod = pData->m_nPeriod;    // 周期
	m_strCondition = pData->m_strCondition;
	m_SubData.Copy(&pData->m_SubData);
}

SingleSelCondition::SingleSelCondition()
{
	m_nType   = hx_DefMaxValues; 
	m_nPeriod = HX_PERIOD_PERIOD_TYPE_DAY; // 日线
}
void SingleSelCondition::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_strExp;
		ar << m_nType;
		ar << m_nPeriod;			// 
		ar << m_strCondition;			// 
	}
	else
	{
		ar >> m_strExp;
		ar >> m_nType;
		ar >> m_nPeriod;			// 
		ar >> m_strCondition;			// 		
	}
	m_SubData.Serialize(ar);
}

void UnionSelConditionSub::Free()
{
	for(int i = 0; i < m_aySub->GetSize(); i++)
	{
		delete m_aySub->GetAt(i);
	}
	m_aySub->RemoveAll();
}

void UnionSelConditionSub::Copy(UnionSelConditionSub* pData)
{
	if( pData == NULL )
		return;

	Free();

	m_strName     = pData->m_strName;
	m_nBeginDate  = pData->m_nBeginDate;
	m_nEndDate    = pData->m_nEndDate;    // 周期
	m_cAndOr      = pData->m_cAndOr;

	if( pData->m_aySub )
	{
		for(int i = 0; i < pData->m_aySub->GetSize(); i++)
		{
			SingleSelCondition* pCurData = new SingleSelCondition;
			pCurData->Copy(pData->m_aySub->GetAt(i));
			m_aySub->Add(pCurData);
		}
	}
}

UnionSelConditionSub::UnionSelConditionSub()
{
	m_cAndOr = 0;
	m_nBeginDate = 0;
	m_nEndDate = 0;

	m_lDateCount = 0;
	m_nCalcEndDate = 0;

	m_aySub = new CArray<SingleSelCondition*,SingleSelCondition*>;
}
UnionSelConditionSub::~UnionSelConditionSub()
{
	Free();
	delete m_aySub;
	m_aySub = NULL;
}
void UnionSelConditionSub::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_strName;
		ar << m_nBeginDate;
		ar << m_nEndDate;

		ar << m_lDateCount;	// 周期数
		ar << m_nCalcEndDate; // 计算结果时间

		ar << m_cAndOr;		// 条件项是与还是或的关系

		int nCount = m_aySub->GetSize();
		ar << nCount;
		for(int i = 0; i < nCount; i++)
		{
			m_aySub->GetAt(i)->Serialize(ar);
		}
	}
	else
	{
		ar >> m_strName;
		ar >> m_nBeginDate;
		ar >> m_nEndDate;

		ar >> m_lDateCount;	  // 周期数
		ar >> m_nCalcEndDate; // 计算结果时间

		ar >> m_cAndOr;		  // 条件项是与还是或的关系

		int nCount;
		ar >> nCount;
		SingleSelCondition* pHSUnionData;
		for(int i = 0; i < nCount; i++)
		{
			pHSUnionData = new SingleSelCondition;
			pHSUnionData->Serialize(ar);
			m_aySub->Add(pHSUnionData);
		}
	}
}

void UnionSelCondition::Free()
{
	for(int i = 0; i < m_aySub->GetSize(); i++)
	{
		delete m_aySub->GetAt(i);
	}
	m_aySub->RemoveAll();
}
void UnionSelCondition::Copy(UnionSelCondition* pData)
{
	if( pData == NULL )
		return;

	Free();

	m_strName = pData->m_strName;
	if( pData->m_aySub )
	{
		for(int i = 0; i < pData->m_aySub->GetSize(); i++)
		{
			UnionSelConditionSub* pCurData = new UnionSelConditionSub;
			pCurData->Copy(pData->m_aySub->GetAt(i));
			m_aySub->Add(pCurData);
		}
	}
}

UnionSelCondition::UnionSelCondition()
{
	m_aySub = new CArray<UnionSelConditionSub*,UnionSelConditionSub*>;
}
UnionSelCondition::~UnionSelCondition()
{
	Free();
	delete m_aySub;
	m_aySub = NULL;
}

void UnionSelCondition::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_strName;

		int nCount = m_aySub->GetSize();
		ar << nCount;
		for(int i = 0; i < nCount; i++)
		{
			m_aySub->GetAt(i)->Serialize(ar);
		}
	}
	else
	{
		ar >> m_strName;

		int nCount;
		ar >> nCount;
		UnionSelConditionSub* pHSUnionSelSub;
		for(int i = 0; i < nCount; i++)
		{
			pHSUnionSelSub = new UnionSelConditionSub;
			pHSUnionSelSub->Serialize(ar);
			m_aySub->Add(pHSUnionSelSub);
		}
	}
}

HSOpenExpressUseData::HSOpenExpressUseData()
{
	memset(this,0,sizeof(HSOpenExpressUseData));
	m_ShowShortKey = 0;
}
HSOpenExpressUseData::HSOpenExpressUseData(WORD wType)
{
	m_wType   = wType;
	m_strName = NULL;
	m_pData   = NULL;
	m_dStyle  = 0;
	m_hParent = NULL;

	m_ShowShortKey = 0;
}

BOOL HSOpenExpressUseData::IsType(WORD wType) { return (m_wType&wType); }
//


StockArea::StockArea()
{
	strText = "";
	lData = 0;
}
void StockArea::AddStock(DWORD market)
{
	CArray<StockUserInfo> ayTempStock;
	int nCount = GetStockByMaket(market, ayTempStock);
	for (int i=0; i<nCount; i++)
	{
		StockUserInfo* pStock = new StockUserInfo;
		if (pStock)
			pStock->Copy(&ayTempStock.GetAt(i));
		ayStock.Add(pStock);
	}
}

////////////////////////////////////////////////////////////////
CMapStringToPtr* AllFinancialData::m_pMapStringToID = NULL;

AllFinancialData::AllFinancialData() 
{
	m_pCalcExternParam = NULL;
}

AllFinancialData::~AllFinancialData() 
{
	if(m_pCalcExternParam)
	{
		if( HSDrawData::m_pHSExternFree )
		{
			(*HSDrawData::m_pHSExternFree)((char*)m_pCalcExternParam,
				charType);
		}
		else
		{
			delete m_pCalcExternParam;		
		}

		m_pCalcExternParam = NULL;
	}
}

void AllFinancialData::SetMyBlockData(int nBegin, int nEnd, CString strBlock)
{
	if(m_pCalcExternParam ==NULL)
	{
		if(HSDrawData::m_pHSExternAlloc)
		{
			m_pCalcExternParam = (CCalcExternParam*)(*HSDrawData::m_pHSExternAlloc)
				( (long)sizeof(CCalcExternParam),charType );
		}
		else
		{
			m_pCalcExternParam = new CCalcExternParam;
		}
	}

	m_pCalcExternParam->m_nBeginDate = nBegin;
	m_pCalcExternParam->m_nEndDate = nEnd ;
	memset(m_pCalcExternParam->m_szBlock, 0, sizeof(m_pCalcExternParam->m_szBlock));
	strcpy(m_pCalcExternParam->m_szBlock, strBlock);	
}

BOOL AllFinancialData::IsMyBlockData(int nBegin, int nEnd, CString strBlock)
{
	return (m_pCalcExternParam 
		&& nEnd == m_pCalcExternParam->m_nEndDate
		//&&Find(nBegin,  0, 0, FALSE) != -1
		&& strBlock.Compare(m_pCalcExternParam->m_szBlock) == 0);
}

void AllFinancialData::DeleteMap()
{
	/* 删除财务报表分析字段映射表 */
	if(m_pMapStringToID)
	{
		CString strKey;
		FinancialColProperty* pValue = NULL;
		POSITION pos = m_pMapStringToID->GetStartPosition( );
		while(pos)
		{
			m_pMapStringToID->GetNextAssoc(pos, strKey, (void*&)pValue);
			_delObject(pValue);
		}
		m_pMapStringToID->RemoveAll();
		_delObject(m_pMapStringToID);
	}
}

#define SET_MAPSTRINGTOID(ID, STR/*,  CH, STYLE*/) \
	m_pMapStringToID->SetAt((STR),	new FinancialColProperty((ID), (STR)/*, (CH), (STYLE)*/));

CMapStringToPtr* AllFinancialData::GetMap()
{
	if(m_pMapStringToID == NULL)
	{
		m_pMapStringToID = new CMapStringToPtr; 
		m_pMapStringToID->InitHashTable(120);

		m_pMapStringToID->SetAt("ZGB"	, new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZGB	,"ZGB"));
		m_pMapStringToID->SetAt("GJG"	, new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_GJG	,"GJG"));
		m_pMapStringToID->SetAt("FQRG"	, new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_FQRG	,"FQRG"));
		m_pMapStringToID->SetAt("FRG"	, new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_FRG	,"FRG"));
		m_pMapStringToID->SetAt("NBZGG" , new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_NBZGG	,"NBZGG"));
		m_pMapStringToID->SetAt("LTAG"	, new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_LTAG	,"LTAG"));
		m_pMapStringToID->SetAt("ZPG"	, new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZPG	,"ZPG"));
		m_pMapStringToID->SetAt("HG"	, new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_HG	,"HG"));
		m_pMapStringToID->SetAt("BG"	, new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_BG	,"BG"));
		m_pMapStringToID->SetAt("YXG"	, new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YXG	,"YXG"));

		m_pMapStringToID->SetAt("HBZJ",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_HBZJ	,"HBZJ"));
		m_pMapStringToID->SetAt("DQTZJE",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_DQTZJE	,"DQTZJE"));
		m_pMapStringToID->SetAt("YSZKJE",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSZKJE	,"YSZKJE"));
		m_pMapStringToID->SetAt("YSZK", 	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSZK	,"YSZK"));
		m_pMapStringToID->SetAt("YSPJ", 	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSPJ	,"YSPJ"));
		m_pMapStringToID->SetAt("YSGL", 	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSGL	,"YSGL"));
		m_pMapStringToID->SetAt("YSLX", 	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSLX	,"YSLX"));
		m_pMapStringToID->SetAt("CHJE", 	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_CHJE	,"CHJE"));
		m_pMapStringToID->SetAt("LDZC",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_LDZC	,"LDZC"));
		m_pMapStringToID->SetAt("CQTZJE",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_CQTZJE	,"CQTZJE"));

		m_pMapStringToID->SetAt("GDZCHJ",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_GDZCHJ	,"GDZCHJ",	NULL));
		m_pMapStringToID->SetAt("GDZCJZ",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_GDZCJZ	,"GDZCJZ",	NULL));
		m_pMapStringToID->SetAt("WXZC",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_WXZC	,"WXZC",	NULL));
		m_pMapStringToID->SetAt("WXJQTZC",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_WXJQTZC	,"WXJQTZC",	NULL));
		m_pMapStringToID->SetAt("ZZC",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZZC		,"ZZC",		NULL));
		m_pMapStringToID->SetAt("DQJK",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_DQJK	,"DQJK",	NULL));
		m_pMapStringToID->SetAt("YFZK",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YFZK	,"YFZK",	NULL));
		m_pMapStringToID->SetAt("LDFZ",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_LDFZ	,"LDFZ",	NULL));
		m_pMapStringToID->SetAt("CQFZ",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_CQFZ	,"CQFZ",	NULL));
		m_pMapStringToID->SetAt("FZHJ",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_FZHJ	,"FZHJ",	NULL));

		m_pMapStringToID->SetAt("SSGDQY",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_SSGDQY		,"SSGDQY",	NULL));
		m_pMapStringToID->SetAt("GDQYHJ",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_GDQYHJ		,"GDQYHJ",	NULL));
		m_pMapStringToID->SetAt("GB",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_GB			,"GB",		NULL));
		m_pMapStringToID->SetAt("ZBGJ",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZBGJ		,"ZBGJ",	NULL));
		m_pMapStringToID->SetAt("YYGJ",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YYGJ		,"YYGJ",	NULL));
		m_pMapStringToID->SetAt("WFPLR",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_WFPLR		,"WFPLR",	NULL));
		m_pMapStringToID->SetAt("FZHGDQYZJ",new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_FZHGDQYZJ  ,"FZHGDQYZJ",NULL));
		m_pMapStringToID->SetAt("ZYYWSR",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZYYWSR		,"ZYYWSR",	NULL));
		m_pMapStringToID->SetAt("ZYYWSRJE",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZYYWSRJE	,"ZYYWSRJE",NULL));
		m_pMapStringToID->SetAt("ZYYWCB",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZYYWCB		,"ZYYWCB",	NULL));

		m_pMapStringToID->SetAt("ZYYWLR",	new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_ZYYWLR	,"ZYYWLR",	NULL));
		m_pMapStringToID->SetAt("QTYWLR",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_QTYWLR	,"QTYWLR",	NULL));
		m_pMapStringToID->SetAt("BTSR",		new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_BTSR	,"BTSR",	NULL));
		m_pMapStringToID->SetAt("YYWSR",	new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_YYWSR	,"YYWSR",	NULL));
		m_pMapStringToID->SetAt("YYWZC",	new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_YYWZC	,"YYWZC",	NULL));
		m_pMapStringToID->SetAt("YYFY",		new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_YYFY	,"YYFY",	NULL));
		m_pMapStringToID->SetAt("GLFY",		new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_GLFY	,"GLFY",	NULL));
		m_pMapStringToID->SetAt("CWFY",		new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_CWFY	,"CWFY",	NULL));
		m_pMapStringToID->SetAt("YYLR",		new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_YYLR	,"YYLR",	NULL));
		m_pMapStringToID->SetAt("TZSY",		new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_TZSY	,"TZSY",	NULL));

		m_pMapStringToID->SetAt("LRZE",			new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_LRZE			,"LRZE",		NULL));
		m_pMapStringToID->SetAt("SDS",			new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_SDS			,"SDS",			NULL));
		m_pMapStringToID->SetAt("JLR",			new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_JLR			,"JLR",			NULL));
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_KGFPDLR, "KGFPLR");
		m_pMapStringToID->SetAt("KCFJCXSYHDJLR",new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_KCFJCXSYHDJLR	,"KCFJCXSYHDJLR",NULL));
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_JYXJLRXJ, "JYHDXJLR");
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_JYXJLCXJ, "JYHDXJLC");
		m_pMapStringToID->SetAt("JYHDXJJE",		new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_JYHDXJJE		,"JYHDXJJE",		NULL));
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_TZXJLRXJ, "TZXJLR");
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_TZXJLCXJ, "TZXJLC");

		m_pMapStringToID->SetAt("TZXJJE",		new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_TZXJJE			,"TZXJJE",		NULL));

		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_CZXJLRXJ, "CCXJLR");
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_CZXJLCXJ, "CCXJLC");
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_CZXJJE,	"CCXJJE");

		m_pMapStringToID->SetAt("XJJDJWJZJE",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_XJJDJWJZJE		,"XJJDJWJZJE",	NULL));
		m_pMapStringToID->SetAt("XSSPSDDXJ",	new FinancialColProperty( COLUMN_FINANCIAL_STATEMENT_XSSPSDDXJ		,"XSSPSDDXJ",	NULL));
		m_pMapStringToID->SetAt("YSZKZE",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSZKZE			,"YSZKZE",		NULL));
		m_pMapStringToID->SetAt("HZZB",			new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_HZZB			,"HZZB",			NULL));
		m_pMapStringToID->SetAt("YSZKJEZZCZEBL",new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSZKJEZZCZEBL	,"YSZKJEZZCZEBL",NULL));
		m_pMapStringToID->SetAt("YSZK1",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSZK1			,"YSZK1",		NULL));

		m_pMapStringToID->SetAt("YSZK2",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSZK2		,"YSZK2",	NULL));
		m_pMapStringToID->SetAt("YSZK3",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSZK3		,"YSZK3",	NULL));
		m_pMapStringToID->SetAt("YSZK4",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSZK4		,"YSZK4",	NULL));
		m_pMapStringToID->SetAt("QTYSZKZE", new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_QTYSZKZE	,"QTYSZKZE",NULL));
		m_pMapStringToID->SetAt("QTYSK1",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_QTYSK1		,"QTYSK1",	NULL));
		m_pMapStringToID->SetAt("QTYSK2",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_QTYSK2		,"QTYSK2",	NULL));
		m_pMapStringToID->SetAt("QTYSK3",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_QTYSK3		,"QTYSK3",	NULL));
		m_pMapStringToID->SetAt("QTYSK4",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_QTYSK4		,"QTYSK4",	NULL));
		m_pMapStringToID->SetAt("MGSYKC",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_MGSYKC		,"MGSYKC",	NULL));
		m_pMapStringToID->SetAt("MGTBSY",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_MGTBSY		,"MGTBSY",	NULL));

		m_pMapStringToID->SetAt("MGJQSY",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_MGJQSY		,"MGJQSY",		NULL));
		m_pMapStringToID->SetAt("MGSY",			new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_MGSY		,"MGSY",			NULL));
		m_pMapStringToID->SetAt("MGJZC",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_MGJZC		,"MGJZC",		NULL));
		m_pMapStringToID->SetAt("TZHMGJZC", 	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_TZHMGJZC	,"TZHMGJZC", 	NULL));
		m_pMapStringToID->SetAt("MGZBGJJ",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_MGZBGJJ		,"MGZBGJJ",		NULL));
		m_pMapStringToID->SetAt("MGWFPLR",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_MGWFPLR		,"MGWFPLR",		NULL));
		m_pMapStringToID->SetAt("MGXJJLL",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_MGXJJLL		,"MGXJJLL",		NULL));
		m_pMapStringToID->SetAt("MGJYHDXJLL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_MGJYHDXJLL	,"MGJYHDXJLL",	NULL));
		m_pMapStringToID->SetAt("XSMLL",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_XSMLL		,"XSMLL",		NULL));
		m_pMapStringToID->SetAt("ZYYWLRL",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZYYWLRL		,"ZYYWLRL",		NULL));

		m_pMapStringToID->SetAt("XSJLL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_XSJLL	,"XSJLL",	NULL));
		m_pMapStringToID->SetAt("ZZCSYL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZZCSYL	,"ZZCSYL",	NULL));
		m_pMapStringToID->SetAt("JZCSYL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_JZCSYL	,"JZCSYL",	NULL));
		m_pMapStringToID->SetAt("JQJZCSYL", new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_JQJZCSYL,"JQJZCSYL", NULL));
		m_pMapStringToID->SetAt("YSZKZZL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YSZKZZL	,"YSZKZZL",	NULL));
		m_pMapStringToID->SetAt("CHZZL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_CHZZL	,"CHZZL",	NULL));
		m_pMapStringToID->SetAt("GDZCZZL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_GDZCZZL	,"GDZCZZL",	NULL));
		m_pMapStringToID->SetAt("GDQYZZL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_GDQYZZL	,"GDQYZZL",	NULL));
		m_pMapStringToID->SetAt("ZZCZZL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZZCZZL	,"ZZCZZL",	NULL));
		m_pMapStringToID->SetAt("LDBL",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_LDBL	,"LDBL",		NULL));

		m_pMapStringToID->SetAt("SDBL",			new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_SDBL		 ,"SDBL",			NULL));
		m_pMapStringToID->SetAt("LXBZBS",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_LXBZBS		 ,"LXBZBS",		NULL));
		m_pMapStringToID->SetAt("GDQYYGDZCBL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_GDQYYGDZCBL	 ,"GDQYYGDZCBL",	NULL));
		m_pMapStringToID->SetAt("CQFZYYYZJBL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_CQFZYYYZJBL	 ,"CQFZYYYZJBL",	NULL));
		m_pMapStringToID->SetAt("ZCFZBL",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZCFZBL		 ,"ZCFZBL",		NULL));
		m_pMapStringToID->SetAt("GDQYBL",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_GDQYBL		 ,"GDQYBL",		NULL));
		m_pMapStringToID->SetAt("GDZCBL",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_GDZCBL		 ,"GDZCBL",		NULL));
		m_pMapStringToID->SetAt("ZYYWSRZZL",	new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_ZYYWSRZZL	 ,"ZYYWSRZZL",	NULL));
		m_pMapStringToID->SetAt("YYLRZZL",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_YYLRZZL		 ,"YYLRZZL",		NULL));
		m_pMapStringToID->SetAt("SHLRZZL",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_SHLRZZL		 ,"SHLRZZL",		NULL));

		m_pMapStringToID->SetAt("JZCZZL",		new FinancialColProperty(COLUMN_FINANCIAL_STATEMENT_JZCZZL		,"JZCZZL",	NULL));

		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_LLZEZZL, "LRZEZZL");
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_ZZCZZHANGL, "ZZCZZHANGL");

		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_ZGDS , "ZGDS");
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_YLZD1, "YLZD1");
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_YLZD2, "YLZD2");
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_YLZD3, "YLZD3");
		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_LJZJ, "LJZJ");

		SET_MAPSTRINGTOID(COLUMN_FINANCIAL_STATEMENT_FSRQ, "RQ");		
	}
	return m_pMapStringToID;
}
/*static */
/* 
通过英文简称, 返回该ID(如果pProperty不为空, 则复制数据到pProperty)
*/
int AllFinancialData::GetFinColProperty(CString strSimpleEn,
										FinancialColProperty* pProperty /*= NULL*/)
{ 
	FinancialColProperty* pLookup = NULL;

	strSimpleEn.TrimLeft(' ');
	strSimpleEn.TrimRight(' ');

	ASSERT(m_pMapStringToID);
	if(m_pMapStringToID )
	{
		strSimpleEn.MakeUpper();
		m_pMapStringToID->Lookup(strSimpleEn, (void*&)pLookup);
	} 
	int nID = -1;
	if(pLookup)
	{
		nID = pLookup->m_dwID;
	}
	else if(!strSimpleEn.IsEmpty() && isdigit(strSimpleEn[0]) )/* 可能是输入了索引字符串 */
	{
		nID = atoi(strSimpleEn);
		nID += COLUMN_FINANCIAL_STATEMENT_BEGIN;

		if(nID < COLUMN_FINANCIAL_STATEMENT_BEGIN 
			|| nID > COLUMN_FINANCIAL_STATEMENT_END)
		{
			nID = -1;
		}
	}

	//ASSERT(nID != -1);

	if(pProperty)
	{
		memset(pProperty, 0, sizeof(FinancialColProperty));
		if(pLookup)
		{
			memcpy(pProperty, pLookup, sizeof(FinancialColProperty));			
		}
		else
		{
			pProperty->m_dwID = nID;
		}
	}

	return nID;
}

HSDouble AllFinancialData::GetDataByName(UpdatedFinancialData* pData, CString nID)
{
	return GetDataByID(pData, GetFinColProperty(nID));
}
HSDouble AllFinancialData::GetDataByID(UpdatedFinancialData* pData, UINT nID)
{
	nID = nID - COLUMN_FINANCIAL_STATEMENT_BEGIN;
	if(pData)
	{
		if(nID >= 0 && nID < FINANCIAL_DATA_LENGTH )
		{
			return (double)pData->m_fData[nID];
		}
		else if(nID == (COLUMN_FINANCIAL_STATEMENT_FSRQ -COLUMN_FINANCIAL_STATEMENT_BEGIN))
		{
			return (double)pData->m_lBiuldDate;
		}
	}
	return hx_DefMaxValues;
}

HSDouble AllFinancialData::GetDataByPos(UpdatedFinancialData* pData, int nPos)
{
	if(pData )
	{
		if ( nPos >= 0 && nPos < FINANCIAL_DATA_LENGTH )
			return (double)pData->m_fData[nPos];
		else if(nPos == (COLUMN_FINANCIAL_STATEMENT_FSRQ - COLUMN_FINANCIAL_STATEMENT_BEGIN))
		{
			return (double)pData->m_lBiuldDate;
		}
	}
	return hx_DefMaxValues;
}

/********************************************************************************
* 函数功能 : 检测数据是否有效
* 函数参数 : CString code - 判别的股票代码, 代码可以为空字串,则忽略判断
*  返回值  : 成功返回TRUE, 失败返回FALSE
* 调用依赖 : 
* 作    者 : 熊钟彬
* 完成日期 : 2004-04-06
* 修改记录 : 
* 作	者 : 
* 日	期 : 
* 修改内容 : 
*******************************************************************************/
BOOL AllFinancialData::IsEmpty(CString code)
{
	if(!IsValid())
	{
		return TRUE;
	}

	UpdatedFinancialData* pData = (UpdatedFinancialData*)m_pData;
	if(!code.IsEmpty() && strncmp(code, pData->m_strCode, 
		sizeof(pData->m_strCode)) != 0)
	{
		return TRUE;
	}

	return pData->m_lBiuldDate < 19710101;
}
/********************************************************************************
* 函数功能 : 根据日期,返回该数据属于哪一期期 1,2,3,4
* 函数参数 : long lDate - 日期, 格式为20040406
*  返回值  : int, 失败返回0, 成功返回1,2,3,4,分别表示第1,2,3,4季度
* 调用依赖 : 
* 作    者 : 熊钟彬
* 完成日期 : 2004-04-06
* 修改记录 : 
* 作	者 : 
* 日	期 : 
* 修改内容 : 
*******************************************************************************/
int AllFinancialData::GetType(long lDate)
{
	int nType = 0;
	lDate = lDate % 1000;

	if(lDate == 331)
	{
		nType = 1;
	}
	else if(lDate == 630)
	{
		nType = 2;
	}
	else if(lDate == 930)
	{
		nType = 3;
	}
	else if(lDate == 1231)
	{
		nType = 4;
	}
	else
	{
	}
	return nType;
}
/********************************************************************************
* 函数功能 : 根据数据索引位置(相对于数据个数的索引),返回在数据指针
* 函数参数 : int nPos- 索引位置
*  返回值  : UpdatedFinancialData*数据指针, 失败返回NULL
* 调用依赖 : 
* 作    者 : 熊钟彬
* 完成日期 : 2004-04-06
* 修改记录 : 
* 作	者 : 
* 日	期 : 
* 修改内容 : 
*******************************************************************************/
UpdatedFinancialData* AllFinancialData::GetData(int nPos)
{
	UpdatedFinancialData* pData = NULL;
	long lLen = nPos * sizeof(UpdatedFinancialData);
	if(lLen < this->m_lSize && lLen >= 0)
	{
		pData = (UpdatedFinancialData*)(m_pData + lLen);
	}
	return pData;
}
/********************************************************************************
* 函数功能 : 模糊查找一个日期区间的位置
* 函数参数 : 
*  返回值  : 
* 调用依赖 : 
* 作    者 : 熊钟彬
* 完成日期 : 2004-04-17
* 修改记录 : 
* 作	者 : 
* 日	期 : 
* 修改内容 : 
*******************************************************************************/
void AllFinancialData::Find(int Begin,int End, int &nBeginPos, int &nEndPos)
{
	nBeginPos	= 0;
	nEndPos		= GetSize();

	if(Begin == End)
	{
		if(Begin == -1)
		{
			nBeginPos = nEndPos - 1;
		}
		else if(Begin > 0)
		{
			nBeginPos	= Find(Begin, -1, -1, FALSE);	
			nEndPos = nBeginPos + 1;
		}
	}
	else
	{
		if(Begin > 0 )
		{
			nBeginPos	= Find(Begin);	
		}

		if(End > Begin)
		{
			nEndPos	= Find(End, nBeginPos);
		}
	}	
}

void AllFinancialData::ReSort()
{
	if(!IsValid())
	{
		return ;
	}

	/* 绝对区间的查找 */
	KQuickSort qsData;
	qsData.m_pData      = m_pData;
	qsData.m_nDataLen   = sizeof(UpdatedFinancialData);
	qsData.m_nKeyOffset = sizeof(((UpdatedFinancialData*)m_pData)->m_strCode);
	qsData.m_nKeyLen    = QS_UNLONG;
	qsData.m_nLow       = 0;
	qsData.m_nHigh      = GetSize() - 1;

	QuickSortAscend(&qsData);
}
/********************************************************************************
* 函数功能 : 使用折半查找法, 查找日期所在的模糊位置
* 函数参数 : long lDate - 查找目标, long nLow - 低位置, long nHigh - 高位置 
*  返回值  : 失败返回-1, 成功返回索引位置
* 调用依赖 : 
* 作    者 : 熊钟彬
* 完成日期 : 2004-04-06
* 修改记录 : 
* 作	者 : 
* 日	期 : 
* 修改内容 : 
*******************************************************************************/
int AllFinancialData::Find(long lDate, long nLow /*= 0*/, 
						   long nHigh /*= 0*/, BOOL bBlur /*= TRUE*/,BOOL bBinarySearch /*= TRUE*/)
{
	if(!IsValid())
	{
		return -1;
	}

	if(nLow < 0)
	{
		nLow = 0;
	}

	if(nHigh < 0)
	{
		nHigh = GetSize();
	}	

	long mid = -1;

	if(bBlur)
	{
		/* 查找模糊区间 */
		UpdatedFinancialData* pBeginData = (UpdatedFinancialData*)m_pData;
		UpdatedFinancialData* pCurData = NULL;
		long  iLow = nLow, iHigh = nHigh;
		do
		{
			mid = (iLow + iHigh) / 2;

			pCurData = pBeginData + mid;
			if(pCurData->m_lBiuldDate == lDate)
			{
				break;
			}
			else if(pCurData->m_lBiuldDate > lDate)
			{
				if(mid > (nLow + 1) && lDate > (pCurData -1)->m_lBiuldDate)
				{
					break;			  
				}
				iHigh = mid - 1;
			}
			else
			{
				iLow  = mid + 1;
			}      
		}while (iLow <= iHigh);
	}
	else
	{
		if(bBinarySearch)/* 折半查找 */
		{
			/* 绝对区间的查找 */
			KQuickSort qsData;
			qsData.m_pData      = m_pData;
			qsData.m_nDataLen   = sizeof(UpdatedFinancialData);
			qsData.m_nKeyOffset = sizeof(((UpdatedFinancialData*)m_pData)->m_strCode);
			qsData.m_nKeyLen    = QS_UNLONG;
			qsData.m_nLow       = nLow;
			qsData.m_nHigh      = nHigh - 1;

			mid =  BinarySearch(&qsData, &lDate);		
		}
		else/* 逐个查找 */
		{
			UpdatedFinancialData* pCurData = NULL;
			for(int i = nLow; i < nHigh; i++)
			{
				pCurData = this->GetData(i);
				if(pCurData == NULL)
				{
					continue;
				}
				if(pCurData->m_lBiuldDate == lDate)
				{
					mid = i;
					break;
				}
			}
		}
	}
	return mid;
}