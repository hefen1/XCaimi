// FormulaCalc.cpp: implementation of the CFormulaCalc class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FormulaCalc.h"
#include "SystemFun.h"
#include "FormulaStruct.h"
#include "WinnerApplication.h"
#include "PubStruct.h"
#include "pageid.h"
#include <afxmt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// 由周期得到请求周期类型等
BOOL GetPeriodType(const int nSwitch,
				   short& nPeriodNumber,short& nPeriodType,CString* pPeriodName)
{
	nPeriodNumber = 1;
	switch(nSwitch)
	{
	case AnalisysFor1:
		{
			nPeriodType = PERIOD_TYPE_MINUTE1;
			if( pPeriodName )
			{
				*pPeriodName = "1分钟";
			}
		}
		break;
	case AnalisysFor5:
		{
			nPeriodType = PERIOD_TYPE_MINUTE5;
			if( pPeriodName )
			{
				*pPeriodName = "5分钟";
			}
		}
		break;
	case AnalisysFor15:
		{
			nPeriodType = PERIOD_TYPE_MINUTE15; //20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "15分钟";
			}
			//nPeriodNumber = 3;
		}
		break;
	case AnalisysFor30:
		{
			nPeriodType = PERIOD_TYPE_MINUTE30;	//20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "30分钟";
			}
			//nPeriodNumber = 6;
		}
		break;
	case AnalisysFor60:
		{
			nPeriodType = PERIOD_TYPE_MINUTE60;	//20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "60分钟";
			}
			//nPeriodNumber = 12;
		}
		break;

	case AnalisysFor180:
		{
			nPeriodType = PERIOD_TYPE_MINUTE60;	//20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "180分";
			}
			nPeriodNumber = 3;
		}
		break;

	case AnalisysForDay:
		{
			nPeriodType = PERIOD_TYPE_DAY;
			if( pPeriodName )
			{
				*pPeriodName = "日线";
			}
		}
		break;
	case AnalisysForWeek:
		{
			nPeriodType = PERIOD_TYPE_WEEK;	//20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "周线";
			}
			//nPeriodNumber = 7;
		}
		break;
	case AnalisysForMonth:
		{
			nPeriodType = PERIOD_TYPE_MONTH;	//20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "月线";
			}
			//nPeriodNumber = 30;
		}
		break;
	case AnalisysForMoreDay:
		{
			// gbq add 20060429, 多日线用于多分线
			CWinApp* pApp = AfxGetApp();
			BOOL bPeriodType =FALSE;//= pApp->GetProfileInt(CHSTools_Multi_Day_Number,CHSTools_Multi_Type, 0);
		//	CfgValues::GetDetailCfgInt(m_UserCfgMapOb,CHSTools_Multi_Day_Number,CHSTools_Multi_Type , bPeriodType,0);
			if( bPeriodType )
			{
				nPeriodType = PERIOD_TYPE_MINUTE1;
			//	nPeriodNumber = pApp->GetProfileInt(CHSTools_Multi_Day_Number,
			//		CHSTools_Multi_Time_Number, 3);
				int nTemp=1;
				//CfgValues::GetDetailCfgInt(m_UserCfgMapOb,CHSTools_Multi_Day_Number,CHSTools_Multi_Time_Number , nTemp,3);
				nPeriodNumber = nTemp;
				pPeriodName->Format("%d分钟线",nPeriodNumber);
			}
			else
			{
				nPeriodType = PERIOD_TYPE_DAY;
		//		nPeriodNumber = pApp->GetProfileInt(CHSTools_Multi_Day_Number,
		//			CHSTools_Multi_Day_Number, 10);
				int nTemp=1;
				//CfgValues::GetDetailCfgInt(m_UserCfgMapOb,CHSTools_Multi_Day_Number,CHSTools_Multi_Day_Number , nTemp,10);
				nPeriodNumber = nTemp;
				pPeriodName->Format("%d日线",nPeriodNumber);
			}
			/* old
			nPeriodType = PERIOD_TYPE_DAY;
			CWinApp* pApp = AfxGetApp();
			if( pApp )
			{
				nPeriodNumber = pApp->GetProfileInt(CHSTools_Multi_Day_Number,
					CHSTools_Multi_Day_Number,10);
			}
			if(nPeriodNumber < 1)
			{
				nPeriodNumber = 10;
			}
			if( pPeriodName )
			{
				pPeriodName->Format("%d日线",nPeriodNumber);
			}
			*/
			// gbq end;

			/*nPeriodType = PERIOD_TYPE_DAY;
			HSSettingProperty* pPro = g_hxData.GetSettingProperty(DWORD(HS_IDD_MULTIDAY));
			if(pPro)
			{
				nPeriodNumber = atol(pPro->m_strNewValue);
			}
			if(nPeriodNumber < 1)
			{
				nPeriodNumber = 10;
			}
			if( pPeriodName )
			{
				pPeriodName->Format("%d日线",nPeriodNumber);
			}*/
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}
CArray<BlockInfo*,BlockInfo*> * HSGridBlockData::m_payAllBlock = NULL;

#ifndef _AFXDLL
CArray<StockUserInfo*,StockUserInfo*> StockArea::ayStock;
#endif

//
CMapVariabile*		  CFormulaCalc::m_pExternExpression = NULL;     // 外部表达式表
CMapVariabile*		  CFormulaCalc::m_pExternVariabile = NULL;		 // 外部变量表
CMapVariabile*        CFormulaCalc::m_pExternFunction = NULL;		 // 外部函数表
CSystemFun*			  CFormulaCalc::m_pSystemFun = NULL;			 // 函数指针
//
CWnd*				  CFormulaCalc::m_pMsgWnd = NULL;
SelStockCondition*    CFormulaCalc::m_curSelStockCondition = NULL;
TExchSys*			  CFormulaCalc::m_pExchSys = NULL;

//
int					  CFormulaCalc::m_nStopThread = 0;
HANDLE				  CFormulaCalc::m_hThread = NULL;
DWORD				  g_dwThreadID = -1;

//
// 选股成功信息
CString SuccResult::m_strTip; // 选股成功信息提示

//
CArray<CExpValue*,CExpValue*>		 CFormulaCalc::m_ayFreeList;
CArray<CFormulaCalc::UnionCalc*,CFormulaCalc::UnionCalc*>		 CFormulaCalc::m_UnionArray;

// 数据管理

InportDataParam*  CFormulaCalc::m_pHxDataSource = NULL;

static BOOL		  g_bUserHisDate = FALSE; // 选股时，使用历史时段

//预警系统使用
int															CFormulaCalc::m_nStopAlarmThread = 0;
HANDLE														CFormulaCalc::m_hAlarmThread = NULL;
CArray<AlarmCalcData*, AlarmCalcData*>*				CFormulaCalc::m_pAlarmCondition = NULL;
/*CHSDisk*													CFormulaCalc::m_pAlarmDisk	= NULL;*/
InportDataParam*											CFormulaCalc::m_pAlarmHxDataSource = NULL;
CArray<CFormulaCalc::UnionCalc*,CFormulaCalc::UnionCalc*> CFormulaCalc::m_AlarmUnionArray;
CWnd*														CFormulaCalc::m_pAlarmWnd = NULL;

// 
//CArray<BlockTypeInfo*,BlockTypeInfo*> BlockTabInitData::m_ayBlockCalc;

static CMapStringToPtrEx<CArray<SuccResult*,SuccResult*>*>* m_mapSelectSuccInfo = NULL;


int g_nStartCol = 1;

CFormulaCalc::FormulaVarColData* CFormulaCalc::FormulaIndicatorSortResult::GetColName(int nCol)
{
	if( nCol >= 0 && nCol < m_ayColName.GetSize() )
		return m_ayColName.GetAt(nCol);

	return NULL;
}
IDataSourceEx* CFormulaCalc::m_pDataSource=NULL;
int CFormulaCalc::FormulaIndicatorSortResult::AddColName(int nCol,CString strName)
{
	CFormulaCalc::FormulaVarColData* pColVar = 
		new CFormulaCalc::FormulaVarColData(nCol,strName);
	m_ayColName.Add(pColVar);
	return pColVar->m_nID;
}

void CFormulaCalc::FormulaIndicatorSortResult::Free()
{
	m_pStock = NULL;

	_delArrayObj(m_ayColName);
	////
	//for( int i = m_ayColName.GetSize() - 1; i >= 0 ; i-- )
	//{
	//	delete m_ayColName.GetAt(i);
	//}
	//m_ayColName.RemoveAll();

	//
	POSITION pos = m_ayValue.GetStartPosition();
	int nKey;
	HSDouble* pData;
	while (pos != NULL)
	{
		m_ayValue.GetNextAssoc( pos, nKey, pData );
		if( pData )
		{
			delete pData;
		}
	}
	m_ayValue.RemoveAll();

	pos = m_ayValueEx.GetStartPosition();
	CString* pDataEx;
	while (pos != NULL)
	{
		m_ayValueEx.GetNextAssoc( pos, nKey, pDataEx );
		if( pDataEx )
		{
			delete pDataEx;
		}
	}
	m_ayValueEx.RemoveAll();
}		

void CFormulaCalc::FormulaIndicatorSortResult::EmptyData()
{
	POSITION pos = m_ayValue.GetStartPosition();
	int		  nKey;
	HSDouble* pData;
	while (pos != NULL)
	{
		m_ayValue.GetNextAssoc( pos, nKey, pData );
		if( pData )
		{
			pData->m_dData = hx_DefMinValues;
		}
	}

	pos = m_ayValueEx.GetStartPosition();
	CString* pDataEx;
	while (pos != NULL)
	{
		m_ayValueEx.GetNextAssoc( pos, nKey, pDataEx );
		if( pDataEx )
		{
			pDataEx->Empty();
		}
	}
}

CString*  CFormulaCalc::FormulaIndicatorSortResult::GetDataEx(int nCol)
{
	CString* pData;
	if( m_ayValueEx.Lookup( nCol, pData) )
		return pData;

	return NULL;
}

HSDouble* CFormulaCalc::FormulaIndicatorSortResult::GetData(int nCol)
{
	HSDouble* pData;
	if( m_ayValue.Lookup(nCol,pData) )
		return pData;

	return NULL;
}

void CFormulaCalc::FormulaIndicatorSortResult::SetData(int nCol,HSDouble* pRefData)
{
	if( pRefData == NULL )
		return;

	HSDouble* pData = GetData(nCol);
	if( pData == NULL )
	{
		pData = new HSDouble;		
		m_ayValue.SetAt(nCol,pData);
	}

	pData->m_dData = pRefData->m_dData;
}

void CFormulaCalc::FormulaIndicatorSortResult::SetData(int nCol, CString& strData)
{
	CString* pData = GetDataEx(nCol);
	if( pData == NULL )
	{
		pData = new CString;		
		m_ayValueEx.SetAt(nCol,pData);

		pData->Format("%s", strData);
	}
	else
	{
		pData->Format("%s", strData);
	}
}

class CExpressionBaseEx : public CExpressionBase
{
public:
	virtual CHSDataSource* GetData() 
	{ 
		if( CFormulaCalc::m_pHxDataSource )
		{
			return CFormulaCalc::m_pHxDataSource->m_pDataSrc; 
		}
		return NULL;
	}
};

int CFormulaCalc::FormulaBaseOtherColData::GetData(CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx,int nPos)
{
	if( pBaseDataEx == NULL ||
		m_pFun == NULL )
		return -1;

	static CExpressionBaseEx pExpress;

	//CExpression* pExpress = pCompile->GetExpByCompile();
	//if( pExpress == NULL )
	//	return -1;

	HSCurDataInfoStatus pCurDataStatus;
	pCurDataStatus.SetStatus(hx_GetCurrentPosData);
	pCurDataStatus.m_nDataPos = nPos;
	int nError = 1;

	HSDouble dValue = ( *m_pFun )( &pExpress, m_pParam, &pCurDataStatus,nError );
	if( nError == HS_Return_STRING )
	{
		if( dValue != (double)hx_DefMinValues )
		{
			CString strText;
			strText.Format("%.0f", dValue.m_dData);
			pBaseDataEx->SetData(m_nID, strText);
		}
	}
	else
	{
		if( dValue != (double)hx_DefMinValues )
		{
			pBaseDataEx->SetData( m_nID,&dValue );
		}
	}

	return nError;
}

int CFormulaCalc::FormulaBaseOtherColData::GetData(CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx, void* ayNowData)
{
	if( pBaseDataEx == NULL ||
		m_pFun == NULL  || ayNowData == NULL )
		return -1;

	CArray<CommRealTimeData*, CommRealTimeData*>* pAyData = (CArray<CommRealTimeData*, CommRealTimeData*>*)ayNowData;

	static CExpressionBaseEx pExpress;
	int nError = 1;

	// if nError == 0 then return is double, else is 领涨股票在ayNowData的索引，少于0则为领下股票的索引
	nodElement sTemp;
	if( m_pParam == NULL )
	{
		m_pParam = new FormulaGeneralParam;
		if( m_pParam == NULL )
		{
			return -1;
		}
	}
	m_pParam->Add(&sTemp);
	sTemp.pData = pAyData;
	sTemp.operatie = Operator_InportData;
	HSDouble dValue = ( *m_pFun )( &pExpress, m_pParam, NULL, nError );
	m_pParam->GetArray()->RemoveAll();

	if( nError == HS_Return_Double )
	{
		if( dValue != (double)hx_DefMinValues )
		{
			pBaseDataEx->SetData( m_nID,&dValue );
		}
	}
	else if( nError == HS_Return_Long )
	{
		if( dValue != (double)hx_DefMinValues )
		{
			pBaseDataEx->SetData( m_nID, &dValue);
		}
	}
	else if( nError == HS_Return_ZGB )   //--2010.10.09litao 增加涨股比的处理
	{
		if( dValue != (double)hx_DefMinValues )
		{
			CString strZGB = "";
			char buf[11];
			memset(buf,0,11);
			strZGB = CString(itoa(long(double(dValue) / 100000),buf,10)) + "/";
			memset(buf,0,11);
			strZGB = strZGB + CString(itoa(long(dValue - long(double(dValue) / 100000) * 100000),buf,10));
			pBaseDataEx->SetData( m_nID, strZGB);
		}
	}
	else
	{
		if( dValue != (double)hx_DefMinValues )
		{
			long iIdx = (long)dValue;

			if( iIdx >= 0 && iIdx < pAyData->GetSize() )
			{
				CommRealTimeData* pItem = pAyData->GetAt(iIdx);
				if( pItem != NULL )
				{
					StockUserInfo* pStock =NULL;// (StockUserInfo*)//CStockManager::GetStockUserInfo(&pItem->m_ciStockCode);
					if( pStock != NULL )
					{
						pBaseDataEx->SetData( m_nID, CString(pStock->m_cStockName, sizeof(pStock->m_cStockName)));
					}
					else
					{
						pBaseDataEx->SetData( m_nID, pItem->m_ciStockCode.GetCode());
					}
				}
			}
		}
		return nError;
	}

	return 1;
}

int CFormulaCalc::AddBaseOtherColData( CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx,
									    CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>& ayOther )
{
	if( pBaseDataEx == NULL || ayOther.GetSize() <= 0 )
		return 0;

	CNI_ReceiveData info;
	info.m_lDataTransmited = g_nStartCol;

	FormulaBaseOtherColData* pHSBaseOtherColData;
	for (int i = 0; i < ayOther.GetSize(); i++ )
	{
		pHSBaseOtherColData = (FormulaBaseOtherColData*)ayOther.GetAt(i);
		pHSBaseOtherColData->m_nID = g_nStartCol;
		pBaseDataEx->AddColName(g_nStartCol,pHSBaseOtherColData->m_strName);
		g_nStartCol++;
	}

	if( CFormulaCalc::m_pMsgWnd )
	{
		info.m_lDataTotal = g_nStartCol;
		info.m_pszData	  = (const char*)pBaseDataEx;
		CFormulaCalc::m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,30,(LPARAM)&info);
	}
	return 1;
}

int CFormulaCalc::AddBaseOtherColDataEx( CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx,
									    CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>& ayOther )
{
	if( pBaseDataEx == NULL || ayOther.GetSize() <= 0 )
		return 0;

	CNI_ReceiveData info;
	FormulaBaseOtherColData* pHSBaseOtherColData;
	int iCount = ayOther.GetSize();
	for (int i = 0; i < iCount; i++ )
	{
		pHSBaseOtherColData = (FormulaBaseOtherColData*)ayOther.GetAt(i);
		if( i == 0 )
		{
			info.m_lDataTransmited = pHSBaseOtherColData->m_nID;
		}
		else if( i == iCount - 1 )
		{
			info.m_lDataTotal = pHSBaseOtherColData->m_nID;
		}

		pBaseDataEx->AddColName(pHSBaseOtherColData->m_nID,pHSBaseOtherColData->m_strName);
	}

	if( CFormulaCalc::m_pMsgWnd )
	{
		info.m_pszData	  = (const char*)pBaseDataEx;
		CFormulaCalc::m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,30,(LPARAM)&info);
	}
	return 1;
}

//

HSDouble* GetRefDate()
{
	if( CFormulaCalc::m_pHxDataSource != NULL && 
		CFormulaCalc::m_pHxDataSource->m_pDataSrc != NULL )
	{
		return CFormulaCalc::m_pHxDataSource->m_pDataSrc->GetData(hx_DATE_DATA);
	}
	return NULL;
}

BOOL Include(CFormulaCalc::UnionCalc* pNextUnionCalc,SuccResult* pSucc,SuccResult* pNextSucc)
{
	CTime curDate = CTime(pSucc->m_lDate / 10000,
		(pSucc->m_lDate % 10000)/100,
		(pSucc->m_lDate % 10000)%100,0,0,0);
	CTime compareDate = CTime(pNextSucc->m_lDate / 10000,
		(pNextSucc->m_lDate % 10000)/100,
		(pNextSucc->m_lDate % 10000)%100,0,0,0);

	CTime beginDate = curDate - CTimeSpan(pNextUnionCalc->m_lDateCount,0,0,0);
	CTime endDate   = curDate - CTimeSpan(pNextUnionCalc->m_lEndDate,0,0,0);

	return ((compareDate >= endDate) && (compareDate <= beginDate));
}

int	CFormulaCalc::DelAllSuccData(BOOL bSelf)
{
	if(m_mapSelectSuccInfo != NULL)
	{
		POSITION pos = m_mapSelectSuccInfo->GetStartPosition();
		CString	sName;
		void *pVal;
		CArray<SuccResult*,SuccResult*>* payData;
		int i;
		while (pos != NULL)
		{
			m_mapSelectSuccInfo->GetNextAssoc (pos, sName, pVal);
			if(pVal != NULL)
			{
				payData = (CArray<SuccResult*,SuccResult*>*)pVal;
				if(payData)
				{
					for(i = 0;i < payData->GetSize(); i++)
					{
						if(payData->GetAt(i))
						{
							delete payData->GetAt(i);
						}
					}
					delete payData;
				}
			}
		}

		m_mapSelectSuccInfo->RemoveAll();
		if(bSelf)
		{
			delete m_mapSelectSuccInfo;
			m_mapSelectSuccInfo = NULL;
		}
	}

	return 0;
}

void CFormulaCalc::AddSuccDataToMap(CodeInfo* pCode,CArray<SuccResult*,SuccResult*>* pArray)
{
	if( pCode == NULL ) 
		return;

	if( m_mapSelectSuccInfo == NULL )
	{
		m_mapSelectSuccInfo = new CMapStringToPtrEx<CArray<SuccResult*,SuccResult*>*>;
		m_mapSelectSuccInfo->InitHashTable(1000);
	}

	CString strName = pCode->GetCode();
	m_mapSelectSuccInfo->SetAt(strName,pArray);
}

CArray<SuccResult*,SuccResult*>* AddSuccDataToMap(CodeInfo* pCode)
{
	if( pCode == NULL ) 
		return 0;
	if( m_mapSelectSuccInfo == NULL )
		return 0;

	CString strName = pCode->GetCode();

	void* pData;
	if( m_mapSelectSuccInfo->Lookup(strName,pData) )
	{
		return (CArray<SuccResult*,SuccResult*>*)pData;
	}

	return NULL;
}

// function
static fnHSCompareCondition m_pCompar[] = 
{
	CFormulaCalc::DaYu,       //   大于
		CFormulaCalc::XiaoYu,     //	小于
		CFormulaCalc::DengYu,	   //	等于
		CFormulaCalc::JieYu,      //	介于
		CFormulaCalc::ShangChuan, //	上穿
		CFormulaCalc::XiaoPo,     //	下破
		CFormulaCalc::WeiChi,     //	维持
		CFormulaCalc::BiaoChi,	   //	保持 

		CFormulaCalc::Condition,  // 条件选股
		CFormulaCalc::Exchange,   // 交易系统
		CFormulaCalc::MoreKLine,   // 五彩K线
		CFormulaCalc::Exchange    // 策略（共用交易系统）yulx add 20091117
};

static fnHSCompareCondition GetCompare(int nFun)
{
	if( nFun >= FormulaCompareSubData::DaYu && nFun <= FormulaCompareSubData::Strategy/*MoreKLine*/ )//yulx modify 20091117
	{
		return m_pCompar[nFun];
	}
	return NULL;
}
//

// auto delete
static CFormulaCalc::_Auto_Delete_Data _My_Auto_Delete_Data_;
CFormulaCalc::_Auto_Delete_Data::~_Auto_Delete_Data()
 {
//  	if( CFormulaCalc::m_pHSDisk != NULL )
//  	{
//  		delete CFormulaCalc::m_pHSDisk;
//  		CFormulaCalc::m_pHSDisk = NULL;
//  	}
	if( CFormulaCalc::m_pHxDataSource != NULL )
	{
		delete m_pHxDataSource->m_pDataSrc;
		m_pHxDataSource->m_pDataSrc = NULL;
		delete CFormulaCalc::m_pHxDataSource;
		CFormulaCalc::m_pHxDataSource = NULL;
	}
}

//////////////////////////////////////////////////////////
void CFormulaCalc::CalcRetData::SetValueNumber(int nNumber)
{
	CLineProperty* pData;
	for(int nObj = 0; nObj < m_ayData.GetSize(); nObj++)
	{
		pData = GetDataAt(nObj);
		if( pData == NULL )
		{
			pData = new CLineProperty;
			m_ayData.Add(pData);
		}
		if( pData != NULL )
		{
			pData->Alloc(nNumber,0);
		}
	}
}

void CFormulaCalc::CalcRetData::DeleteData()
{
	for(int i = 0;i < m_ayData.GetSize(); i++)
	{
		delete m_ayData.GetAt(i);
	}
	m_ayData.RemoveAll();
}

void CFormulaCalc::CalcRetData::SetDataSize(int nCount)
{
	int nSize = m_ayData.GetSize();
	while(nSize > 0 && nCount < nSize)
	{
		nSize--;
		delete m_ayData.GetAt(nSize);
		m_ayData.RemoveAt(nSize);		
	}	
}

CLineProperty* CFormulaCalc::CalcRetData::GetDataAt(int nPosistion)
{
	if(nPosistion >= 0 && nPosistion < m_ayData.GetSize())
	{
		return m_ayData.GetAt(nPosistion);
	}

	return NULL;
}

void  CFormulaCalc::CalcRetData::SetCurCode(CodeInfo* pCode)
{
	if( pCode == NULL )
	{
		return;
	}

}

//#ifdef User_Share_Fuction // 
// ref hsTools.cpp
extern BOOL GetPeriodType(const int nSwitch,
				   short& nPeriodNumber,short& nPeriodType,CString* pPeriodName);
//#endif

void CFormulaCalc::CalcRetData::SetAsk(int nPeriod /*= -1*/)
{

//#ifdef User_Share_Fuction
	if( !GetPeriodType(nPeriod,ask.m_pCode->m_nPeriodNum,
		ask.m_pCode->m_cPeriod,NULL) )
	{
		return;
	}
//#endif

	nAnaPeriod = ((ask.m_pCode->m_cPeriod >= PERIOD_TYPE_MINUTE1 && 
				   ask.m_pCode->m_cPeriod <= PERIOD_TYPE_MINUTE120)?hx_MINUTE_DATA:hx_PERIOD_TYPE_DAY_DATA);

	ask.m_nIndex = 0;

#ifdef SUPPORT_NETVALUE
	ask.m_nType  = RT_TECHDATA_EX;
#else
	ask.m_nType  = RT_TECHDATA;
#endif

	ask.m_nSize  = sizeof(ReqDayData) / sizeof(CodeInfo);
	if( sizeof(ReqDayData) % sizeof(CodeInfo) )
	{
		ask.m_nSize++;
	}

	/*
	int nPeriodNum = 1;
	switch(nPeriod)
	{
	case 0: 
		nPeriod = PERIOD_TYPE_MINUTE1; 
		break;				//分析周期：1分钟
	case 1: 
		nPeriod = PERIOD_TYPE_MINUTE5; 
		break;				//分析周期：5分钟
	case 2: 
		nPeriodNum = 3;
		nPeriod = PERIOD_TYPE_MINUTE15; 
		break;				//分析周期：15分钟
	case 3: 
		nPeriodNum = 6;
		nPeriod = MINUTE30; 
		break;				//分析周期：30分钟
	case 4: 
		nPeriodNum = 12;
		nPeriod = MINUTE60; 
		break;				//分析周期：60分钟
	//case 5: nPeriod = PERIOD_TYPE_MINUTE120;			//分析周期：120分钟
	case 6:
		nPeriodNum = 7;
		nPeriod = WEEK; 
		break;					//分析周期：周
	case 7: 
		nPeriodNum = 30;
		nPeriod = MONTH;
		break;				//分析周期：月
	case 8: 
		nPeriod = PERIOD_TYPE_DAY_ANY; 
		break;				//日线任意周期
	//case 0: nPeriod = MINUTE_ANY			//分钟任意周期
	default: 
		nPeriod = PERIOD_TYPE_DAY; 
		break;				//分析周期：日
	}

	ask.m_nIndex = 0;
	ask.m_nType  = RT_TECHDATA;
	ask.m_nSize  = 3;

	ask.m_pCode->m_cPeriod    = nPeriod;
	ask.m_pCode->m_nPeriodNum = nPeriodNum;
	*/

	/*
	nAnaPeriod = ((nAnaPeriod >= PERIOD_TYPE_MINUTE5 && nAnaPeriod <= PERIOD_TYPE_MINUTE120)?hx_MINUTE_DATA:hx_PERIOD_TYPE_DAY_DATA);
	*/
}

//////////////////////////////////////////////////////////
void CFormulaCalc::DeleteAll()
{
	CFormulaCalc::CalcRetData* pRetData;
	UnionCalc*					pUnionCalc;

	for(int j = 0; j < m_UnionArray.GetSize(); j++)
	{
		pUnionCalc = m_UnionArray.GetAt(j);
		for(int k = 0; k < pUnionCalc->subArray.GetSize(); k++)
		{
			pRetData = pUnionCalc->subArray.GetAt(k);
			if( pRetData->pUnionData != NULL )
			{
				CFormulaCalc::GetDrawLine( *pRetData,
					pRetData->pUnionData->m_strCondition,
					pRetData->pUnionData->m_strExp,pRetData->pUnionData->m_nType,0x02 );
			}
			delete pRetData;
		}
		delete pUnionCalc;
	}
	m_UnionArray.RemoveAll();

	for(int i = 0; i < m_ayFreeList.GetSize(); i++)
	{
		m_ayFreeList.GetAt(i)->Free();
	}
	m_ayFreeList.RemoveAll();
}

//
int CFormulaCalc::CalcAll( CString& strName,int& nMask,
						   CString& strLine1,CString& strLine2,CString& strLine3,
						   CFormulaCalc::CalcRetData& retData,
						   int& nBegin,int& nEnd,
						   CFormulaCalc::FormulaIndicatorSortResult* pHSTechSortResult /*= NULL*/)
{
	// 得到计算公式
	if( !retData.m_bInitLeftRight )
	{
		retData.m_bInitLeftRight = 1;
		retData.m_pLeft   = CFormulaCalc::GetDrawLine(retData,strLine1,strName,nMask,
														TRUE,pHSTechSortResult);
		retData.m_pRight  = CFormulaCalc::GetDrawLine(retData,strLine2,strName,nMask);
		retData.m_pMiddle = CFormulaCalc::GetDrawLine(retData,strLine3,strName,nMask);
	}
	if( retData.m_pLeft == NULL ) return -1;

	// 计算
	retData.SetValueNumber(nEnd - nBegin);
	CExpValue*	   pCurVal;
	CLineProperty* pData;
	for(int nObj = 0; nObj < retData.m_ayData.GetSize(); nObj++)
	{
		pData   = retData.GetDataAt(nObj);
		pCurVal = (CExpValue*)pData->m_pCurVal;
		if( pCurVal != NULL )
		{
			if( pCurVal == retData.m_pLeft )
			{
				retData.pCalDataLeft = pData;
			}
			else if( pCurVal == retData.m_pRight )
			{
				retData.pCalDataRight = pData;
			}
			else if( pCurVal == retData.m_pMiddle )
			{
				retData.pCalDataMidlle = pData;
			}

			pCurVal->CalcValue(pData,0,nEnd,NULL);

			if( retData.m_pHSTechSortResult && pCurVal->IsStyle(HX_EXPRESSDRAW) )
			{
				if( (nEnd-1) >= 0 && pData && !pData->IsEmpty() )
				{
					if( pData->m_pData[nEnd-1] != (double)hx_DefMinValues )
					{
						retData.m_pHSTechSortResult->SetData(pData->m_dwStyle,
							&pData->m_pData[nEnd-1]);
					}
				}
			}
		}
	}

	// 
	if( (nEnd-1) < 0 )
		return -1;

	return (nEnd-1);
}

int CFormulaCalc::DaYu(SingleSelCondition*  pUnionData,
						CFormulaCalc::CalcRetData& retData,
						int nBegin,int nEnd)		//  大于
{
	try
	{
		if( pUnionData->m_nType == JiBenMian )
		{
			if ( atof( *(CString*)nBegin ) > atof(pUnionData->m_SubData.m_dData1) ) 
			{
				retData.AddSucc(-1,-1);
				return 1;
			}

			return 0;
		}

		if( CalcAll( pUnionData->m_strExp,
			pUnionData->m_nType,
			pUnionData->m_strCondition,
			pUnionData->m_SubData.m_dData1,
			pUnionData->m_SubData.m_dData2,
			retData,
			nBegin,nEnd ) <= 0 )
		{
			return 0;
		}

		//
		int nCur;
		if( retData.m_pRight == NULL )
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 0; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur] != (double)hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur] > atof(pUnionData->m_SubData.m_dData1) )
						{
							nSucc++;
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nCur],nCur);
							}
						}
					}
					return nSucc;
				}
				else
				{
					nEnd -= 1;
					if( nEnd >= 0 &&
						retData.pCalDataLeft->m_pData[nEnd] != (double)hx_DefMinValues )
					{
						if( retData.pCalDataLeft->m_pData[nEnd] > atof(pUnionData->m_SubData.m_dData1) )
						{
							HSDouble* pRefDay = GetRefDate();
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nEnd],nEnd);
								return 1;
							}
						}
						//return ( retData.pCalDataLeft->m_pData[nEnd] > atof(pUnionData->m_SubData.m_dData1) );
					}
				}
			}
		}
		else // 两条线最后数值比较
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 0; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur]  != (double)hx_DefMinValues &&
							retData.pCalDataRight->m_pData[nCur] != (double)hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur] > retData.pCalDataRight->m_pData[nCur] )
						{
							nSucc++;
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nCur],nCur);
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if( nEnd >= 0 &&
						retData.pCalDataLeft->m_pData[nEnd]  != (double)hx_DefMinValues &&
						retData.pCalDataRight->m_pData[nEnd] != (double)hx_DefMinValues )
					{
						if( retData.pCalDataLeft->m_pData[nEnd] > retData.pCalDataRight->m_pData[nEnd] )
						{
							HSDouble* pRefDay = GetRefDate();
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nEnd],nEnd);
								return 1;
							}
						}
					}
				}
			}
		}
	}
	catch(...)
	{
		return 0;
	}

	return 0;
}

int CFormulaCalc::XiaoYu(SingleSelCondition*  pUnionData,
						  CFormulaCalc::CalcRetData& retData,
						  int nBegin,int nEnd)		//	小于
{
	try
	{
		if( pUnionData->m_nType == JiBenMian )
		{
			if ( atof( *(CString*)nBegin ) < atof(pUnionData->m_SubData.m_dData1) )
			{
				retData.AddSucc(-1,-1);
				return 1;
			}
			return 0;
		}

		if( CalcAll( pUnionData->m_strExp,
			pUnionData->m_nType,
			pUnionData->m_strCondition,
			pUnionData->m_SubData.m_dData1,
			pUnionData->m_SubData.m_dData2,
			retData,
			nBegin,nEnd ) <= 0 )
		{
			return 0;
		}

		//
		int nCur;
		if( retData.m_pRight == NULL )
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 0; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur] != (double)hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur] < atof(pUnionData->m_SubData.m_dData1) )
						{
							nSucc++;
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nCur],nCur);
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if( nEnd >= 0 &&
						retData.pCalDataLeft->m_pData[nEnd] != (double)hx_DefMinValues )
					{
						if( retData.pCalDataLeft->m_pData[nEnd] < atof(pUnionData->m_SubData.m_dData1) )
						{
							HSDouble* pRefDay = GetRefDate();
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nEnd],nEnd);
								return 1;
							}
						}
						//return ( retData.pCalDataLeft->m_pData[nEnd] < atof(pUnionData->m_SubData.m_dData1) );
					}
				}
			}
		}
		else // 两条线最后数值比较
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 0; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur]  != (double)hx_DefMinValues &&
							retData.pCalDataRight->m_pData[nCur] != (double)hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur] < retData.pCalDataRight->m_pData[nCur] )
						{
							nSucc++;
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nCur],nCur);
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if( nEnd >= 0 &&
						retData.pCalDataLeft->m_pData[nEnd]  != (double)hx_DefMinValues &&
						retData.pCalDataRight->m_pData[nEnd] != (double)hx_DefMinValues )
					{
						if(retData.pCalDataLeft->m_pData[nEnd] < retData.pCalDataRight->m_pData[nEnd])
						{
							HSDouble* pRefDay = GetRefDate();
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nEnd],nEnd);
								return 1;
							}
						}
						//return (retData.pCalDataLeft->m_pData[nEnd] < retData.pCalDataRight->m_pData[nEnd]);
					}
				}
			}
		}
	}
	catch(...)
	{
		return 0;
	}

	return 0;
}

int CFormulaCalc::DengYu(SingleSelCondition*  pUnionData,
						  CFormulaCalc::CalcRetData& retData,
						  int nBegin,int nEnd)		//	等于
{
	try
	{
		if( pUnionData->m_nType == JiBenMian )
		{
			if ( atof( *(CString*)nBegin ) == atof(pUnionData->m_SubData.m_dData1) )
			{
				retData.AddSucc(-1,-1);
				return 1;
			}
			return 0;
		}

		if( CalcAll( pUnionData->m_strExp,
			pUnionData->m_nType,
			pUnionData->m_strCondition,
			pUnionData->m_SubData.m_dData1,
			pUnionData->m_SubData.m_dData2,
			retData,
			nBegin,nEnd ) <= 0 )
		{
			return 0;
		}

		//
		int nCur;
		if( retData.m_pRight == NULL )
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 0; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur] != (double)hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur] == atof(pUnionData->m_SubData.m_dData1) )
						{
							nSucc++;
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nCur],nCur);
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if(nEnd >= 0 &&
						retData.pCalDataLeft->m_pData[nEnd] != (double)hx_DefMinValues )
					{
						if( retData.pCalDataLeft->m_pData[nEnd] == atof(pUnionData->m_SubData.m_dData1) )
						{
							HSDouble* pRefDay = GetRefDate();
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nEnd],nEnd);
								return 1;
							}
						}
						//return ( retData.pCalDataLeft->m_pData[nEnd] == atof(pUnionData->m_SubData.m_dData1) );
					}
				}
			}
		}
		else // 两条线最后数值比较
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 0; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur]  != (double)hx_DefMinValues &&
							retData.pCalDataRight->m_pData[nCur] != (double)hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur] == retData.pCalDataRight->m_pData[nCur] )
						{
							nSucc++;
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nCur],nCur);
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if( nEnd >= 0 &&
						retData.pCalDataLeft->m_pData[nEnd]  != (double)hx_DefMinValues &&
						retData.pCalDataRight->m_pData[nEnd] != (double)hx_DefMinValues )
					{
						if(retData.pCalDataLeft->m_pData[nEnd] == retData.pCalDataRight->m_pData[nEnd])
						{
							HSDouble* pRefDay = GetRefDate();
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nEnd],nEnd);
								return 1;
							}
						}
						//return (retData.pCalDataLeft->m_pData[nEnd] == retData.pCalDataRight->m_pData[nEnd]);
					}
				}
			}
		}
	}
	catch(...)
	{
		return 0;
	}

	return 0;
}

int CFormulaCalc::JieYu(SingleSelCondition*  pUnionData,
						 CFormulaCalc::CalcRetData& retData,
						 int nBegin,int nEnd)		//	介于
{
	try
	{
		if( pUnionData->m_nType == JiBenMian )
		{
			if ( (atof( *(CString*)nBegin ) > atof(pUnionData->m_SubData.m_dData1) &&
				  atof( *(CString*)nBegin ) < atof(pUnionData->m_SubData.m_dData2)) ||

				 (atof( *(CString*)nBegin ) < atof(pUnionData->m_SubData.m_dData1) &&
				  atof( *(CString*)nBegin ) > atof(pUnionData->m_SubData.m_dData2)) )
			{
				/*if( m_curSelStockCondition )
				{
					retData.AddSucc(m_curSelStockCondition->m_lEndDate,nEnd - 1);
				}
				else*/
				{
					retData.AddSucc(-1,-1);
				}
				return 1;
			}
			return 0;
		}

		if( CalcAll( pUnionData->m_strExp,
			pUnionData->m_nType,
			pUnionData->m_strCondition,
			pUnionData->m_SubData.m_dData1,
			pUnionData->m_SubData.m_dData2,
			retData,
			nBegin,nEnd ) <= 0 )
		{
			return 0;
		}

		//
		int nCur;
		if( retData.m_pRight == NULL && retData.m_pMiddle == NULL )
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 0; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur] != (double)hx_DefMinValues &&
							((retData.pCalDataLeft->m_pData[nCur] > atof(pUnionData->m_SubData.m_dData1) &&
							  retData.pCalDataLeft->m_pData[nCur] < atof(pUnionData->m_SubData.m_dData2)) ||
							 (retData.pCalDataLeft->m_pData[nCur] < atof(pUnionData->m_SubData.m_dData1) &&
							  retData.pCalDataLeft->m_pData[nCur] > atof(pUnionData->m_SubData.m_dData2))) )
						{
							nSucc++;
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nCur],nCur);
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if( nEnd >= 0 && 
						retData.pCalDataLeft->m_pData[nEnd] != (double)hx_DefMinValues )
					{
						if( (retData.pCalDataLeft->m_pData[nEnd] > atof(pUnionData->m_SubData.m_dData1) &&
							 retData.pCalDataLeft->m_pData[nEnd] < atof(pUnionData->m_SubData.m_dData2)) || 
							(retData.pCalDataLeft->m_pData[nEnd] < atof(pUnionData->m_SubData.m_dData1) &&
							 retData.pCalDataLeft->m_pData[nEnd] > atof(pUnionData->m_SubData.m_dData2)) )
						{
							HSDouble* pRefDay = GetRefDate();
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nEnd],nEnd);
								return 1;
							}
						}
						//return ( retData.pCalDataLeft->m_pData[nEnd] >= atof(pUnionData->m_SubData.m_dData1) &&
						//		 retData.pCalDataLeft->m_pData[nEnd] <= atof(pUnionData->m_SubData.m_dData2) );
					}
				}
			}
		}
		else if (retData.m_pRight == NULL && retData.m_pMiddle != NULL )
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataMidlle && !retData.pCalDataMidlle->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 0; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur]   != (double)hx_DefMinValues &&
							retData.pCalDataMidlle->m_pData[nCur] != (double)hx_DefMinValues &&
							((retData.pCalDataLeft->m_pData[nCur] > atof(pUnionData->m_SubData.m_dData1) &&
							  retData.pCalDataLeft->m_pData[nCur] < retData.pCalDataMidlle->m_pData[nCur]) ||
							 (retData.pCalDataLeft->m_pData[nCur] < atof(pUnionData->m_SubData.m_dData1) &&
							  retData.pCalDataLeft->m_pData[nCur] > retData.pCalDataMidlle->m_pData[nCur])) )
						{
							nSucc++;
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nCur],nCur);
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if( nEnd >= 0 && 
						retData.pCalDataLeft->m_pData[nEnd]  != (double)hx_DefMinValues &&
						retData.pCalDataMidlle->m_pData[nEnd] != (double)hx_DefMinValues )
					{
						if( (retData.pCalDataLeft->m_pData[nEnd] > atof(pUnionData->m_SubData.m_dData1) &&
							 retData.pCalDataLeft->m_pData[nEnd] < retData.pCalDataMidlle->m_pData[nEnd]) || 
							(retData.pCalDataLeft->m_pData[nEnd] < atof(pUnionData->m_SubData.m_dData1) &&
							 retData.pCalDataLeft->m_pData[nEnd] > retData.pCalDataMidlle->m_pData[nEnd]) )
						{
							HSDouble* pRefDay = GetRefDate();
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nEnd],nEnd);
								return 1;
							}
						}
						//return ( retData.pCalDataLeft->m_pData[nEnd] >= atof(pUnionData->m_SubData.m_dData1) &&
						//	     retData.pCalDataLeft->m_pData[nEnd] <= retData.pCalDataMidlle->m_pData[nEnd] );
					}
				}
			}
		}
		else if (retData.m_pRight != NULL && retData.m_pMiddle == NULL )
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 0; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur]  != (double)hx_DefMinValues &&
							retData.pCalDataRight->m_pData[nCur] != (double)hx_DefMinValues &&
							((retData.pCalDataLeft->m_pData[nCur] > retData.pCalDataRight->m_pData[nCur] &&
							  retData.pCalDataLeft->m_pData[nCur] < atof(pUnionData->m_SubData.m_dData2)) || 
							 (retData.pCalDataLeft->m_pData[nCur] < retData.pCalDataRight->m_pData[nCur] &&
							  retData.pCalDataLeft->m_pData[nCur] > atof(pUnionData->m_SubData.m_dData2))) )
						{
							nSucc++;
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nCur],nCur);
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if( nEnd >= 0 && 
						retData.pCalDataLeft->m_pData[nEnd]  != (double)hx_DefMinValues &&
						retData.pCalDataRight->m_pData[nEnd] != (double)hx_DefMinValues )
					{
						if( (retData.pCalDataLeft->m_pData[nEnd] > retData.pCalDataRight->m_pData[nEnd] &&
							 retData.pCalDataLeft->m_pData[nEnd] < atof(pUnionData->m_SubData.m_dData2)) ||
							(retData.pCalDataLeft->m_pData[nEnd] < retData.pCalDataRight->m_pData[nEnd] &&
							 retData.pCalDataLeft->m_pData[nEnd] > atof(pUnionData->m_SubData.m_dData2)) )
						{
							HSDouble* pRefDay = GetRefDate();
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nEnd],nEnd);
								return 1;
							}
						}
						//return ( retData.pCalDataLeft->m_pData[nEnd] >= retData.pCalDataRight->m_pData[nEnd] &&
						//	     retData.pCalDataLeft->m_pData[nEnd] <= atof(pUnionData->m_SubData.m_dData2) );
					}
				}
			}
		}
		else // 线最后数值比较
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() &&
				retData.pCalDataMidlle && !retData.pCalDataMidlle->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 0; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur]  != (double)hx_DefMinValues &&
							retData.pCalDataRight->m_pData[nCur] != (double)hx_DefMinValues &&
							retData.pCalDataMidlle->m_pData[nCur] != (double)hx_DefMinValues &&
							((retData.pCalDataLeft->m_pData[nCur] > retData.pCalDataRight->m_pData[nCur] &&
							  retData.pCalDataLeft->m_pData[nCur] < retData.pCalDataMidlle->m_pData[nCur]) || 
							 (retData.pCalDataLeft->m_pData[nCur] < retData.pCalDataRight->m_pData[nCur] &&
							  retData.pCalDataLeft->m_pData[nCur] > retData.pCalDataMidlle->m_pData[nCur])) )
						{
							nSucc++;
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nCur],nCur);
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if( nEnd >= 0 && 
						retData.pCalDataLeft->m_pData[nEnd]   != (double)hx_DefMinValues &&
						retData.pCalDataRight->m_pData[nEnd]  != (double)hx_DefMinValues &&
						retData.pCalDataMidlle->m_pData[nEnd] != (double)hx_DefMinValues )
					{
						if( (retData.pCalDataLeft->m_pData[nEnd] > retData.pCalDataRight->m_pData[nEnd] &&
							 retData.pCalDataLeft->m_pData[nEnd] < retData.pCalDataMidlle->m_pData[nEnd]) || 
							(retData.pCalDataLeft->m_pData[nEnd] < retData.pCalDataRight->m_pData[nEnd] &&
							 retData.pCalDataLeft->m_pData[nEnd] > retData.pCalDataMidlle->m_pData[nEnd]) )
						{
							HSDouble* pRefDay = GetRefDate();
							if( pRefDay )
							{
								retData.AddSucc((long)pRefDay[nEnd],nEnd);
								return 1;
							}
						}
						//return ( retData.pCalDataLeft->m_pData[nEnd] >= retData.pCalDataRight->m_pData[nEnd] &&
						//	     retData.pCalDataLeft->m_pData[nEnd] <= retData.pCalDataMidlle->m_pData[nEnd] );
					}
				}
			}
		}
	}
	catch(...)
	{
		return 0;
	}

	return 0;
}

int CFormulaCalc::ShangChuan(SingleSelCondition*  pUnionData,
							  CFormulaCalc::CalcRetData& retData,
							  int nBegin,int nEnd)	//	上穿
{
	try
	{
		if( pUnionData->m_nType == JiBenMian )
			return 0;

		if( CalcAll( pUnionData->m_strExp,
			pUnionData->m_nType,
			pUnionData->m_strCondition,
			pUnionData->m_SubData.m_dData1,
			pUnionData->m_SubData.m_dData2,
			retData,
			nBegin,nEnd ) <= 0 )
		{
			return 0;
		}

		int nCur;
		if( retData.m_pRight == NULL )
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 1; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur-1].m_dData != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur].m_dData   != hx_DefMinValues )
						{
							if( (retData.pCalDataLeft->m_pData[nCur-1] < atof(pUnionData->m_SubData.m_dData1)) &&
								(retData.pCalDataLeft->m_pData[nCur]   > atof(pUnionData->m_SubData.m_dData1)) )
							{
								nSucc++;
								if( pRefDay )
								{
									retData.AddSucc((long)pRefDay[nCur],nCur);
								}
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if ( nEnd - 1 >= 0 ) // current
					{
						if( retData.pCalDataLeft->m_pData[nEnd-1].m_dData != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nEnd].m_dData   != hx_DefMinValues )
						{
							if( (retData.pCalDataLeft->m_pData[nEnd-1] < atof(pUnionData->m_SubData.m_dData1)) &&
								(retData.pCalDataLeft->m_pData[nEnd]   > atof(pUnionData->m_SubData.m_dData1)) )
							{
								HSDouble* pRefDay = GetRefDate();
								if( pRefDay )
								{
									retData.AddSucc((long)pRefDay[nEnd],nEnd);
									return 1;
								}
								//return 1;
							}
						}
					}
				}
			}
		}
		else // 两条线比较
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 1; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataRight->m_pData[nCur-1].m_dData != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur-1].m_dData  != hx_DefMinValues &&
							retData.pCalDataRight->m_pData[nCur].m_dData   != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur].m_dData    != hx_DefMinValues )
						{
							if( (retData.pCalDataLeft->m_pData[nCur-1] < retData.pCalDataRight->m_pData[nCur-1]) &&
								(retData.pCalDataLeft->m_pData[nCur]   > retData.pCalDataRight->m_pData[nCur]) )
							{
								nSucc++;
								if( pRefDay )
								{
									retData.AddSucc((long)pRefDay[nCur],nCur);
								}
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if( nEnd - 1 >= 0 )
					{
						if( retData.pCalDataRight->m_pData[nEnd-1].m_dData != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nEnd-1].m_dData  != hx_DefMinValues &&
							retData.pCalDataRight->m_pData[nEnd].m_dData   != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nEnd].m_dData    != hx_DefMinValues )
						{
							if( (retData.pCalDataLeft->m_pData[nEnd-1] < retData.pCalDataRight->m_pData[nEnd-1]) &&
								(retData.pCalDataLeft->m_pData[nEnd]   > retData.pCalDataRight->m_pData[nEnd]) )
							{
								HSDouble* pRefDay = GetRefDate();
								if( pRefDay )
								{
									retData.AddSucc((long)pRefDay[nEnd],nEnd);
									return 1;
								}
								//return 1;
							}
						}
					}
				}
			}
		}
	}
	catch(...)
	{
		return 0;
	}

	return 0;
}

int CFormulaCalc::XiaoPo(SingleSelCondition*  pUnionData,
						  CFormulaCalc::CalcRetData& retData,
						  int nBegin,int nEnd)		//	下破
{
	try
	{
		if( pUnionData->m_nType == JiBenMian )
			return 0;

		if( CalcAll( pUnionData->m_strExp,
			pUnionData->m_nType,
			pUnionData->m_strCondition,
			pUnionData->m_SubData.m_dData1,
			pUnionData->m_SubData.m_dData2,
			retData,
			nBegin,nEnd ) <= 0 )
		{
			return 0;
		}

		int nCur;
		if( retData.m_pRight == NULL )
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 1; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataLeft->m_pData[nCur-1].m_dData != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur].m_dData   != hx_DefMinValues )
						{
							if( (retData.pCalDataLeft->m_pData[nCur-1] > atof(pUnionData->m_SubData.m_dData1)) &&
								(retData.pCalDataLeft->m_pData[nCur]   < atof(pUnionData->m_SubData.m_dData1)) )
							{
								nSucc++;
								if( pRefDay )
								{
									retData.AddSucc((long)pRefDay[nCur],nCur);
								}
							}
						}
					}
					return nSucc;
				}
				else 
				{
					nEnd -= 1;
					if( nEnd - 1 >= 0 )
					{
						if( retData.pCalDataLeft->m_pData[nEnd-1].m_dData != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nEnd].m_dData   != hx_DefMinValues )
						{
							if( (retData.pCalDataLeft->m_pData[nEnd-1] > atof(pUnionData->m_SubData.m_dData1)) &&
								(retData.pCalDataLeft->m_pData[nEnd]   < atof(pUnionData->m_SubData.m_dData1)) )
							{
								HSDouble* pRefDay = GetRefDate();
								if( pRefDay )
								{
									retData.AddSucc((long)pRefDay[nEnd],nEnd);
									return 1;
								}
								//return 1;
							}
						}
					}
				}
			}
		}
		else // 两条线比较
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() )
			{
				if( g_bUserHisDate ) // 历史时段中所有选中的。
				{
					HSDouble* pRefDay = GetRefDate();
					int nSucc = 0;
					for( nCur = 1; nCur < nEnd; nCur++ )
					{
						if( retData.pCalDataRight->m_pData[nCur-1].m_dData != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur-1].m_dData  != hx_DefMinValues &&
							retData.pCalDataRight->m_pData[nCur].m_dData   != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nCur].m_dData    != hx_DefMinValues )
						{
							if( (retData.pCalDataLeft->m_pData[nCur-1] > retData.pCalDataRight->m_pData[nCur-1]) &&
								(retData.pCalDataLeft->m_pData[nCur]   < retData.pCalDataRight->m_pData[nCur]) )
							{
								nSucc++;
								if( pRefDay )
								{
									retData.AddSucc((long)pRefDay[nCur],nCur);
								}
							}
						}
					}
					return nSucc;
				}
				else
				{
					nEnd -= 1;
					if( nEnd - 1 >= 0 )
					{
						if( retData.pCalDataRight->m_pData[nEnd-1].m_dData != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nEnd-1].m_dData  != hx_DefMinValues &&
							retData.pCalDataRight->m_pData[nEnd].m_dData   != hx_DefMinValues &&
							retData.pCalDataLeft->m_pData[nEnd].m_dData    != hx_DefMinValues )
						{
							if( (retData.pCalDataLeft->m_pData[nEnd-1] > retData.pCalDataRight->m_pData[nEnd-1]) &&
								(retData.pCalDataLeft->m_pData[nEnd]   < retData.pCalDataRight->m_pData[nEnd]) )
							{
								HSDouble* pRefDay = GetRefDate();
								if( pRefDay )
								{
									retData.AddSucc((long)pRefDay[nEnd],nEnd);
									return 1;
								}
								//return 1;
							}
						}
					}
				}
			}
		}
	}
	catch(...)
	{
		return 0;
	}

	return 0;
}

int CFormulaCalc::WeiChi(SingleSelCondition*  pUnionData,
						  CFormulaCalc::CalcRetData& retData,
						  int nBegin,int nEnd)		//	维持
{
	if( pUnionData->m_nType == JiBenMian )
		return 0;

	return 0;
}

int CFormulaCalc::BiaoChi(SingleSelCondition*  pUnionData,
						   CFormulaCalc::CalcRetData& retData,
						   int nBegin,int nEnd)	//	保持 
{
	if( pUnionData->m_nType == JiBenMian )
		return 0;

	return 0;
}

int CFormulaCalc::Condition(SingleSelCondition*  pUnionData,
							 CFormulaCalc::CalcRetData& retData,
							 int nBegin,int nEnd)	//	条件选股 
{
	try
	{
		if( pUnionData->m_nType == JiBenMian )
			return 0;

		if( CalcAll( pUnionData->m_strExp,
			pUnionData->m_nType,
			pUnionData->m_strCondition,
			pUnionData->m_SubData.m_dData1,
			pUnionData->m_SubData.m_dData2,
			retData,
			nBegin,nEnd ) <= 0 )
		{
			return 0;
		}

		//
		if( retData.pCalDataLeft && 
			!retData.pCalDataLeft->IsEmpty() )
		{
			if( g_bUserHisDate )  // 历史时段中所有选中的。
			{
				HSDouble* pRefDay = GetRefDate();
				int nSucc = 0;
				for( ; nBegin < nEnd; nBegin++ )
				{
					if( retData.pCalDataLeft->m_pData[nBegin] != (double)hx_DefMinValues &&
						(double)retData.pCalDataLeft->m_pData[nBegin] )
					{
						nSucc++;
						if( pRefDay )
						{
							retData.AddSucc((long)pRefDay[nBegin],nBegin);
						}
					}
				}
				return nSucc;
			}
			else
			{
				nEnd -= 1;
				if( nEnd >= 0 && 
					retData.pCalDataLeft->m_pData[nEnd] != (double)hx_DefMinValues &&
					(double)retData.pCalDataLeft->m_pData[nEnd] )
				{
					HSDouble* pRefDay = GetRefDate();
					if( pRefDay )
					{
						retData.AddSucc((long)pRefDay[nEnd],nEnd);
					}
					return 1;
				}
			}
		}
	}
	catch(...)
	{
		return 0;
	}
	return 0;
}

int CFormulaCalc::Exchange(SingleSelCondition*  pUnionData,
							CFormulaCalc::CalcRetData& retData,
							int nBegin,int nEnd)	//	交易系统 
{
	if( pUnionData->m_nType == JiBenMian )
		return 0;

	try
	{

		if( pUnionData->m_strCondition.IsEmpty() )
		{
			pUnionData->m_strCondition = "ENTERLONG";
		}
		if( pUnionData->m_SubData.m_dData1.IsEmpty() )
		{
			pUnionData->m_SubData.m_dData1 = "EXITLONG";
		}

		if( CalcAll( pUnionData->m_strExp,
			pUnionData->m_nType,
			pUnionData->m_strCondition,
			pUnionData->m_SubData.m_dData1,
			pUnionData->m_SubData.m_dData2,
			retData,
			nBegin,nEnd ) <= 0 )
		{
			return 0;
		}

		HSDouble* pRefDay = GetRefDate();
		int nSucc = 0;
		if( g_bUserHisDate )  // 历史时段中所有选中的。
		{
			for( ; nBegin < nEnd; nBegin++ )
			{
				// ENTERLONG -- 买入
				if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
					retData.pCalDataLeft->m_pData[nBegin] != (double)hx_DefMinValues &&
					(double)retData.pCalDataLeft->m_pData[nBegin] )
				{
					nSucc++;
					if( pRefDay )
					{
						retData.AddSucc((long)pRefDay[nBegin],nBegin);
					}
				}

				// EXITLONG -- 卖出
				if( retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() && 
					retData.pCalDataRight->m_pData[nBegin] != (double)hx_DefMinValues &&
					(double)retData.pCalDataRight->m_pData[nBegin] )
				{
					nSucc++;
					if( pRefDay )
					{
						retData.AddSucc((long)pRefDay[nBegin],nBegin);
					}
				}
			}
			return nSucc;
		}
		else 
		{
			nEnd -= 1;
			if( nEnd < 0 )
				return 0;

			// ENTERLONG -- 买入
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataLeft->m_pData[nEnd] != (double)hx_DefMinValues &&
				(double)retData.pCalDataLeft->m_pData[nEnd] )
			{
				if( pRefDay )
				{
					nSucc++;
					retData.AddSucc((long)pRefDay[nEnd],nEnd);
				}
			}

			// EXITLONG -- 卖出
			if( retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() && 
				retData.pCalDataRight->m_pData[nEnd] != (double)hx_DefMinValues &&
				(double)retData.pCalDataRight->m_pData[nEnd] )
			{
				if( pRefDay )
				{
					nSucc++;
					retData.AddSucc((long)pRefDay[nEnd],nEnd);
				}
			}

			return nSucc;
		}
	}
	catch(...)
	{
		return 0;
	}

	return 0;
}

int CFormulaCalc::MoreKLine(SingleSelCondition*  pUnionData,
							 CFormulaCalc::CalcRetData& retData,
							 int nBegin,int nEnd)	//	五彩K线 
{
	try
	{
		if( pUnionData->m_nType == JiBenMian )
			return 0;

		if( CalcAll( pUnionData->m_strExp,
			pUnionData->m_nType,
			pUnionData->m_strCondition,
			pUnionData->m_SubData.m_dData1,
			pUnionData->m_SubData.m_dData2,
			retData,
			nBegin,nEnd ) <= 0 )
		{
			return 0;
		}

		//
		if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() )
		{
			if( g_bUserHisDate )  // 历史时段中所有选中的。
			{
				HSDouble* pRefDay = GetRefDate();
				int nSucc = 0;

				for( ; nBegin < nEnd; nBegin++ )
				{
					if( retData.pCalDataLeft->m_pData[nBegin] != (double)hx_DefMinValues &&
						(double)retData.pCalDataLeft->m_pData[nBegin] )
					{
						nSucc++;
						if( pRefDay )
						{
							retData.AddSucc((long)pRefDay[nBegin],nBegin);
						}
					}
				}
				return nSucc;
			}
			else 
			{
				nEnd -= 1;
				if( nEnd >= 0 &&
					retData.pCalDataLeft->m_pData[nEnd] != (double)hx_DefMinValues &&
					(double)retData.pCalDataLeft->m_pData[nEnd] )
				{
					HSDouble* pRefDay = GetRefDate();
					if( pRefDay )
					{
						retData.AddSucc((long)pRefDay[nEnd],nEnd);
						return 1;
					}
					//return 1;
				}
			}
		}
	}
	catch(...)
	{
		return 0;
	}
	return 0;
}

int CFormulaCalc::SingleCal( SingleSelCondition*  pUnionData,
							 CFormulaCalc::CalcRetData& retData,
							 int nBegin,int nEnd)
{
	fnHSCompareCondition pFun = GetCompare(pUnionData->m_SubData.m_cType);
	if( pFun )
	{
		return (*pFun)( pUnionData,retData,nBegin,nEnd );
	}
	return 0;
}

int CFormulaCalc::UnionCal(SelStockCondition* pUnionSubData,
							CFormulaCalc::CalcRetData& retData,
							int nBegin,int nEnd)
{

	return 0; // succ
}

CExpValue* CFormulaCalc::GetDrawLine(CFormulaCalc::CalcRetData& retData,
									  CString strDrawName,CString strName,
									  int nMask /*= Tech*/,
									  BOOL bTechLine /*= FALSE*/,
									  CFormulaCalc::FormulaIndicatorSortResult* pHSTechSortResult /*= NULL*/)
{
	strName.MakeUpper();
	CValue* valoare;
	if ( m_pExternExpression == NULL || 
		!m_pExternExpression->Lookup(strName,nMask,valoare) )
	{
		return NULL;
	}

	CExpression* pExp = ((CExpValue*)valoare)->GetExpByCompile();//GetExp();
	if( pExp == NULL ) return NULL;

	/*if( bTechLine & 0x01 )
	{
		pExp->SetData(m_pHxDataSource);
	}
	else if( bTechLine & 0x02 )
	{
		pExp->SetData(NULL);
	}*/

	m_ayFreeList.Add( ((CExpValue*)valoare) );

	/*
	if( !pExp->IsCompile() )
	{
	if( !pExp->Compile() )
	{
	m_ayFreeList.Add( ((CExpValue*)valoare) );
	}
	else
	{
	return NULL;
	}
	}
	*/
	short nDrawCount;

	if( bTechLine )
	{
		CMapVariabile* pVar = pExp->GetExpDraw();
		if( pVar == NULL ||
			pVar->GetObjArray() == NULL )
		{
			return NULL;
		}
		pVar->GetDrawCount(nDrawCount);
		if( nDrawCount == 0 )
		{
			retData.SetDataSize(0);
			return NULL;
		}

		retData.SetDataSize( pVar->GetObjArray()->GetSize() );

		CString			strCurName;

		CNI_ReceiveData info;
		info.m_lDataTransmited = g_nStartCol;

		CExpValue* pCurVal;
		CLineProperty* pData;
		for(int nObj = 0; nObj < pVar->GetObjArray()->GetSize(); nObj++)
		{
			pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
			if( pCurVal != NULL )
			{
				pData = retData.GetDataAt(nObj);
				if( pData == NULL )
				{
					pData = new CLineProperty;
					if( m_pHxDataSource )
					{
						pData->m_pDataSource = m_pHxDataSource->m_pDataSrc;
					}
					retData.m_ayData.Add(pData);
				}
				
				// 绘图名称
				if( (bTechLine & 0x01) && pCurVal->IsStyle(HX_EXPRESSDRAW) )
				{
					CString strObjName = pVar->GetObjectName(pCurVal);
					if( strObjName.CompareNoCase("NONE") == 0 )
					{
						strCurName = strName;
					}
					else
					{
						strCurName.Format( "%s.%s",strName,strObjName );							
					}
					if( retData.m_pHSTechSortResult )
					{
						pData->m_dwStyle = g_nStartCol;
						retData.m_pHSTechSortResult->AddColName(g_nStartCol,strCurName);
						g_nStartCol++;
					}
				}

				//
				pData->m_pCurVal = pCurVal;
				pExp = pCurVal->GetExp();
				if( pExp )
				{
					/*if( bTechLine & 0x01 )
					{
						pExp->SetData(m_pHxDataSource);
					}
					else if( bTechLine & 0x02 )
					{
						pExp->SetData(NULL);
					}*/
				}
			}
		}

		// 指标排序
		if( (bTechLine & 0x01) && retData.m_pHSTechSortResult && m_pMsgWnd )
		{
			info.m_lDataTotal = g_nStartCol;
			info.m_pszData	  = (const char*)retData.m_pHSTechSortResult;
			m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,30,(LPARAM)&info);
		}

		//
		if( strDrawName.IsEmpty() ) 
		{
			CArray<class CValue*,class CValue*>* pAay = pVar->GetObjArray();
			if( pAay && pAay->GetSize() > 0)
			{
				return (CExpValue*)pAay->GetAt(pAay->GetSize() - 1);
			}

			return NULL;

			//valoare = pVar->GetObjArray()->GetAt(0);
			//return (CExpValue*)valoare;
		}
	}

	strDrawName.MakeUpper();
	valoare = pExp->GetVariable(strDrawName,nDrawCount);
	if( nDrawCount != CExpression::ExpDrawObj ) return NULL;

	return (CExpValue*)valoare;

}

CFormulaCalc::CFormulaCalc()
{
	
}

CFormulaCalc::~CFormulaCalc()
{

}

// 由名称得到绘图表达式
CMapVariabile* CFormulaCalc::GetExpDraw(CString strName,int nMask /*= Tech*/)
{
	strName.MakeUpper();
	CValue* valoare;
	if ( m_pExternExpression != NULL && 
		m_pExternExpression->Lookup(strName,nMask,valoare) )
	{
		CExpression* pExp = ((CExpValue*)valoare)->GetExpByCompile();//GetExp();
		if(pExp != NULL)
		{			
			return pExp->GetExpDraw();
		}
	}
	return NULL;
}

// 由名称得到表达式(公式)
CExpression* CFormulaCalc::GetExpress(CString strName,int nMask /*= Tech*/)
{
	strName.MakeUpper();
	CValue* valoare;
	if ( m_pExternExpression != NULL && 
		m_pExternExpression->Lookup(strName,nMask,valoare) )
	{
		CExpression* pExp = ((CExpValue*)valoare)->GetExpByCompile();//GetExp();		
		if(pExp != NULL)
		{
			return pExp;
		}
	}
	return NULL;
}

DWORD WINAPI CFormulaCalc::HSBlockReportCalulate(LPVOID pParameter)
{
	CFormulaCalc::m_nStopThread = -2;

#ifndef _DEBUG
	//WRITE_LOG(__FILE__, __LINE__, "开始计算");
#endif
	CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>* ayOther = NULL;
	try
	{
		DeleteAll();
		InitObj();
		g_nStartCol = HS_USR_DEF_BEGIN + 40;
		BOOL bAndSucc  = FALSE;
		BOOL bIsOnlyOther = FALSE;
		CArray<CFormulaCalc::FormulaIndicatorSortResult*,CFormulaCalc::FormulaIndicatorSortResult*> ayRetData;
		//
		FormulaIndicatorSortResult* pBaseDataEx = NULL;
		FormulaBaseOtherColData* pHSBaseOtherColData;
		SelStockResult SelectStockResult;
		CNI_ReceiveData info;
		memset(&info,0,sizeof(CNI_ReceiveData));
		//
		if( m_curSelStockCondition->m_pData != NULL )
		{
			FormulaAddColParam* pHSAddColParam = (FormulaAddColParam*)m_curSelStockCondition->m_pData;
			ayOther = pHSAddColParam->payOther;
			pBaseDataEx = new FormulaIndicatorSortResult;
			AddBaseOtherColDataEx(pBaseDataEx,*ayOther);
			// 自定义列 空间独立
			ayOther = new CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>;
			if( ayOther == NULL )
			{
			/*	//WRITE_LOG(__FILE__, __LINE__, "分配内存错误");*/
				goto Calculate__;
			}
			for( int i = 0; i < pHSAddColParam->payOther->GetSize(); i++ )
			{
				FormulaBaseOtherColData* pItem = pHSAddColParam->payOther->GetAt(i);
				if( pItem == NULL )
				{
					continue;
				}
				FormulaBaseOtherColData* pAddItem = new FormulaBaseOtherColData;
				*pAddItem = *pItem;
				ayOther->Add(pAddItem);
			}
			ayRetData.Add(pBaseDataEx);
			//
		}
		else
		{
#ifndef _DEBUG
			//WRITE_LOG(__FILE__, __LINE__, "非法终止");
#endif
			goto Calculate__;
		}
		HSGridBlockData* pGridBlock = NULL;//(HSGridBlockData*)m_curSelStockCondition->m_lDateCount;
		CMapStringToPtr* pMapNowData = (CMapStringToPtr*)m_curSelStockCondition->m_lEndDate;
		HSGridBlockData::m_payAllBlock = (CArray<BlockInfo*,BlockInfo*>*)m_curSelStockCondition->m_pHSUnion;
		if( pMapNowData == NULL || HSGridBlockData::m_payAllBlock == NULL )
			goto Calculate__;
		//  

		// 板块
		CArray<BlockInfo*,BlockInfo*> *payAllBlock = HSGridBlockData::m_payAllBlock;

		int nBlockCount = payAllBlock->GetSize();

		int nEnd = nBlockCount;
		BOOL bAll = TRUE;
		if( pGridBlock != NULL && pGridBlock->m_payIdx != NULL )
		{
			nEnd = pGridBlock->m_payIdx->GetSize();
			bAll = FALSE;
		}

		info.m_lDataTotal = nEnd;

#ifdef _DEBUG
		CFile fpLog;
		fpLog.Open("C:\\CalData.txt", CFile::modeCreate|CFile::modeReadWrite);
#endif

		for( int iType = 0; iType < nEnd; iType++ )
		{
			if( CFormulaCalc::m_nStopThread != -2 )
			{
				//WRITE_LOG(__FILE__, __LINE__, "计算终止");
				break;
			}

			BlockInfo* pItem;
			if( !bAll )
			{
				int iIdx = (int)pGridBlock->m_payIdx->GetAt(iType);
				if( iIdx < 0 || iIdx > nBlockCount )
					continue;

				pItem = payAllBlock->GetAt(iIdx);
			}
			else
			{
				pItem = payAllBlock->GetAt(iType);
			}

			 
			if( pItem == NULL || pItem->ayStockList.GetSize() <= 0 )
			{
				//WRITE_LOG(__FILE__, __LINE__, "不合法数据");
				continue;
			}

			for( int k = ayRetData.GetSize() - 1; k >= 0; k-- )
			{
				ayRetData.GetAt(k)->EmptyData();
			}
#ifdef _DEBUG
			//if( pItem->strText.CompareNoCase("H股板块") == 0 )
			//{
			//	CString str;
			//}
#endif

			// 板块股票
			int nStockCount = pItem->ayStockList.GetSize();
			CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> ayNowData; // 行情数据
			for( int iStock = 0; iStock < nStockCount; iStock++ )
			{
				if( CFormulaCalc::m_nStopThread != -2 )
				{
				/*	//WRITE_LOG(__FILE__, __LINE__, "计算终止");*/
					break;
				}

				CodeInfo* pStockItem = pItem->ayStockList.GetAt(iStock);
				if( pStockItem == NULL )
				{
#ifndef _DEBUG
					//WRITE_LOG(__FILE__, __LINE__, "不合法数据");
#endif
					continue;
				}

				void * pTempItem;
				CommRealTimeDataEmpty* pNowDataItem;
				// 得到行情数据
				if( pMapNowData->Lookup(pStockItem->m_cCode, pTempItem) )
				{
					pNowDataItem = (CommRealTimeDataEmpty*)pTempItem;
					ayNowData.Add(pNowDataItem);
				}
			}

			// 输出计算源数据
#ifdef _DEBUG
			CString strCalData;
			CString strItem;
			CommRealTimeDataEmpty* pNowDataItem;
			strCalData.Format("[%s] %d\n", pItem->strBlockName, nStockCount);
			for( int iLog = 0; iLog < ayNowData.GetSize(); iLog++ )
			{
				pNowDataItem = ayNowData.GetAt(iLog);
				if( pNowDataItem == NULL )
				{
					continue;
				}
				ShareRealTimeData* pShareData = NULL;
				if ( pItem )
					pShareData = pNowDataItem->GetShareData();
				strItem.Format("%s : %d\t%d\t%d\t%f\n", 
					pNowDataItem->m_ciStockCode.GetCode(),
					pShareData->m_nowData.m_lOpen, 
					pShareData->m_nowData.m_lNewPrice, 
					pShareData->m_nowData.m_lTotal, 
					pShareData->m_nowData.m_fAvgPrice);
				strCalData += strItem;
			}
			fpLog.Write(strCalData, strCalData.GetLength());
#endif
			// 数据项计算
			int nCaclCount = ayOther->GetSize();
			if( ayOther )
			{
				for( int t = 0; t < nCaclCount; t++ )
				{
					pHSBaseOtherColData = ayOther->GetAt(t);
					if( pHSBaseOtherColData && ::IsWindow(m_pMsgWnd->m_hWnd) )
					{
						pHSBaseOtherColData->GetData(pBaseDataEx, &ayNowData);
					}
				}
			}

			// 当前结果 
			info.m_lDataTransmited = iType;
			SelectStockResult.pStock =NULL;// (void*)//CStockManager::GetBlockUserInfo((char*)(LPCSTR)pItem->strText, SI_NAME);
			SelectStockResult.dData.m_pOtherData = (HSDrawOtherData*)&ayRetData;
			info.m_pszData = (const char*)&SelectStockResult;

			if( m_pMsgWnd != NULL && ::IsWindow(m_pMsgWnd->m_hWnd) )
			{
				m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,4,(LPARAM)&info); // 选出股票结果			
			}
		}

#ifdef _DEBUG
		fpLog.Close();
#endif

		SelectStockResult.dData.m_pOtherData = NULL;

		if( pBaseDataEx )
		{
			delete pBaseDataEx;
		}
	}

#ifdef VC_7_0

	// 异常处理
	catch(CMemoryException e) //　内存异常
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("BlockReport MemoryException: %s", szCause);
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}
	catch(CNotSupportedException e) // 文件异常
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("BlockReport NotSupported: %s", szCause);
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}
#endif

	catch(...) // 其它异常
	{
		// 调试日志
		CString strError;
		strError.Format("BlockReport other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}

Calculate__:

	try
	{
		DeleteAll();
		if( ayOther != NULL )
		{
			_delArrayObj((*ayOther));
			delete ayOther;
			ayOther = NULL;
		}
	}
	catch(...)
	{
		//WRITE_LOG(__FILE__, __LINE__, "板块计算内存异常");
	}

	if( m_pMsgWnd != NULL && ::IsWindow(m_pMsgWnd->m_hWnd) ) // end
	{
		m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,5,
			(LPARAM)m_curSelStockCondition->m_dStyle); // 选出股票结果
	}

	CFormulaCalc::m_nStopThread = 0;
	return 0;
}

DWORD WINAPI CFormulaCalc::HSBlockCalculate(LPVOID pParameter)
{	

	CFormulaCalc::m_nStopThread = -2;
	try
	{
		DeleteAll();

		CFormulaCalc::CalcRetData* pRetData;
		UnionCalc*					pUnionCalc;

		CArray<StockUserInfo*,StockUserInfo*>* pAyStock = &StockArea::ayStock; // 股票总数
		int nCount = pAyStock->GetSize();

		BOOL bAndSucc  = FALSE;
		g_bUserHisDate = (((m_curSelStockCondition->m_dStyle & 0xFF00) & SelectStock_HisDate) == SelectStock_HisDate);

		UnionSelCondition* pHSUnion = m_curSelStockCondition->m_pHSUnion;
		if( pHSUnion )
		{
			CArray<UnionSelConditionSub*,UnionSelConditionSub*>* pArray = pHSUnion->m_aySub;
			if( pArray )
			{
				UnionSelConditionSub* pHSUnionSelSub;
				SingleSelCondition*   pUnionData;
				for(int i = 0; i < pArray->GetSize(); i++)
				{
					pHSUnionSelSub = pArray->GetAt(i);

					pUnionCalc = new UnionCalc;
					m_UnionArray.Add(pUnionCalc);
					pUnionCalc->bAndOr = pHSUnionSelSub->m_cAndOr;
					if( pHSUnionSelSub->m_nBeginDate == 0 && pHSUnionSelSub->m_nEndDate == 0 ) // default date
					{
						pUnionCalc->m_lDateCount = (short)m_curSelStockCondition->m_lDateCount;
						pUnionCalc->m_lEndDate   = m_curSelStockCondition->m_lEndDate;
					}
					else
					{
						pUnionCalc->m_lDateCount = (short)pHSUnionSelSub->m_nBeginDate;
						pUnionCalc->m_lEndDate   = pHSUnionSelSub->m_nEndDate;
					}

					for(int j = 0; j < pHSUnionSelSub->m_aySub->GetSize(); j++)
					{
						pUnionData = pHSUnionSelSub->m_aySub->GetAt(j);

						pRetData = new CFormulaCalc::CalcRetData;
						pUnionCalc->subArray.Add(pRetData);
						pRetData->pUnionData = pUnionData;
						pRetData->SetAsk(-1);
					}
				}
			}

			InitObj();

			CNI_ReceiveData info;
			memset(&info,0,sizeof(CNI_ReceiveData));

			long nPriceUnit;
			int nDecimal;
			StockUserInfo* pStock;
			CString strCondition;
			int i,j,k;
			//int nFindDateRet;
			int nBegin = 0;
			int nEnd;
			//int nMoreSucc;
			int nUnionCount,nSubCount;
			SelStockResult SelectStockResult;
			BlockRetRowData	sutRetData;

			for( i = 0; i < nCount; i++ )
			{
				if( CFormulaCalc::m_nStopThread != -2 )
					break;

				sutRetData.nIndex = i;
				sutRetData.ayValus.RemoveAll();

				// 
				pStock = pAyStock->GetAt(i);
				m_pHxDataSource->m_pDataSrc->SetFinaceData(&pStock->m_ciStockCode);

				nUnionCount = m_UnionArray.GetSize();
				for( j = 0; j < nUnionCount; j++)
				{
					if( CFormulaCalc::m_nStopThread != -2 )
						break;

					pUnionCalc = m_UnionArray.GetAt(j);
					nSubCount = pUnionCalc->subArray.GetSize();
					for(k = 0; k < nSubCount; k++)
					{
						if( CFormulaCalc::m_nStopThread != -2 ) //
							break;

						pRetData = pUnionCalc->subArray.GetAt(k);


						// get disk data
						memcpy(&pRetData->ask.m_pCode->m_ciCode,&pStock->m_ciStockCode,sizeof(CodeInfo));
						RetDiskData* pDiskData = NULL;
						GetDataSourcePtr()->HSDataSource_GetDayData(&pRetData->ask,1);

						if( pDiskData == NULL || pDiskData->IsEmpty() )
						{
							sutRetData.ayValus.Add(0);
						}
						else
						{
							nDecimal   = pStock->m_nDecimal;						
							nPriceUnit = pStock->m_nPriceUnit;

							StockDay* pDay = (StockDay*)pDiskData->m_pData;
							nEnd		   = pDiskData->m_lSize;
							m_pHxDataSource->m_pDataSrc->SetDay( pDay,pDiskData->m_lSize,
								pRetData->nChengJiaoDiv,0,0,nPriceUnit,nDecimal);

							m_pHxDataSource->m_pDataSrc->RemoveStyle( pRetData->nAnaPeriod );
							m_pHxDataSource->m_pDataSrc->AddStyle( pRetData->nAnaPeriod );
							// calc
							if( CalcAll( pRetData->pUnionData->m_strExp,
								pRetData->pUnionData->m_nType,
								pRetData->pUnionData->m_strCondition,
								pRetData->pUnionData->m_SubData.m_dData1,
								pRetData->pUnionData->m_SubData.m_dData2,
								*pRetData,
								nBegin,nEnd ) > 0 )
							{
								if( pRetData->pCalDataLeft && !pRetData->pCalDataLeft->IsEmpty() )
								{
									if( pRetData->pCalDataLeft->m_pData[nEnd-1] != (double)hx_DefMinValues )
									{
										sutRetData.ayValus.Add( pRetData->pCalDataLeft->m_pData[nEnd-1].m_dData );
									}
									else
									{
										sutRetData.ayValus.Add(0);
									}
								}
								else
								{
									sutRetData.ayValus.Add(0);
								}
							}
							else
							{
								sutRetData.ayValus.Add(0);
							}
						}
					}
				}

				sutRetData.ayValus.Add( (double)m_pHxDataSource->m_pDataSrc->CAPITAL() );//GetFinaceDataByID( COLUMN_CAPITALIZATION_PASS_A ) );
				m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA, 15, (LPARAM)&sutRetData); 

			} // 股票大循环

			if( CFormulaCalc::m_nStopThread == -2 )
			{
				sutRetData.nIndex = nCount + 2;
				m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA, 15, (LPARAM)&sutRetData); 
			}
		}

		DeleteAll();

	}
	catch(...)
	{}

	DeleteAll();

	CFormulaCalc::m_nStopThread = 0;

	return 0;
}


DWORD WINAPI CFormulaCalc::HSExchCalc(LPVOID pParameter)
{
	try
	{
		DeleteAll();
		InitObj();

		TCaclTemp* pCaclTemp = NULL;
		pCaclTemp = (TCaclTemp*)pParameter;
		if( pCaclTemp == NULL )
		{
			return TCaclReturn_VainParam;
		}

		/*************************************************************
		* 更改
		* 时间：  2003年9月24日
		* 描述：  为了适应新协议
		*************************************************************/
		AskDataEx ask;

		if( !GetPeriodType(pCaclTemp->nAnaPeriod, ask.m_pCode->m_nPeriodNum,
			ask.m_pCode->m_cPeriod,NULL) )
		{
			return TCaclReturn_VainParam;
		}

#ifdef SUPPORT_NETVALUE
		ask.m_nType  = RT_TECHDATA_EX;
#else
		ask.m_nType  = RT_TECHDATA;
#endif
		ask.m_nSize  = sizeof(ReqDayData) / sizeof(CodeInfo);
		if( sizeof(ReqDayData) % sizeof(CodeInfo) )
		{
			ask.m_nSize++;
		}
		memcpy(&ask.m_pCode->m_ciCode, pCaclTemp->pCode, sizeof(CodeInfo));
		RetDiskData* pDiskData =NULL;
		GetDataSourcePtr()->HSDataSource_GetDayData(&ask,1);

		// 是否无数据
		if( pDiskData == NULL || pDiskData->IsEmpty() )
		{
			return TCaclReturn_NoData;
		}
		// 小数
		int nDecimal   = 0;
		// 单位
		//long nPriceUnit = //CStockManager::GetStockPriceUnit(pCaclTemp->pCode->m_cCodeType);
		long nPriceUnit = 0;//CStockManager::GetStockPriceUnit(pCaclTemp->pCode);

		// 设定日线数据
		int nFindDateRet = FindDate(pCaclTemp->lEndDate,
			pCaclTemp->lCountDate,
			(StockDay*)pDiskData->m_pData,pDiskData->m_lSize);	

		int nFindDataBegin = FindDateExact(pCaclTemp->lBeginDate,
			pCaclTemp->lCountDate,
			(StockDay*)pDiskData->m_pData,pDiskData->m_lSize);	

		if( nFindDataBegin < 0 || nFindDataBegin > nFindDateRet )
		{
			nFindDataBegin = 0;
		}

		// 是否无数据
		if( nFindDateRet <= 0 )
		{
			return TCaclReturn_NoData;
		}

		// 日线数据定位
		StockDay* pDay = (StockDay*)pDiskData->m_pData;
/*		if( pCaclTemp->lCountDate != 0 )
		{
			if( pDiskData->m_lSize > pCaclTemp->lCountDate )
			{
				pDiskData->m_lSize = pCaclTemp->lCountDate;
			}
			nFindDateRet -= pCaclTemp->lCountDate;
			pDay = (StockDay*)&pDay[__max(nFindDateRet,0)];
		}
		else */
		if( nFindDateRet < pDiskData->m_lSize )
		{
			pDiskData->m_lSize = nFindDateRet+1;
		}

		pCaclTemp->pRetDiskData = pDay+nFindDataBegin;
		// 财务，除权...，用到的代码信息
		m_pHxDataSource->m_pDataSrc->SetAsk( (AskData*)&ask,0 );
		// 设定财务数据
		m_pHxDataSource->m_pDataSrc->SetFinaceData(pCaclTemp->pCode);

		// 设定数据源数据
		m_pHxDataSource->m_pDataSrc->SetDay( pDay,pDiskData->m_lSize,
			pCaclTemp->nChengJiaoDiv, 0, 0, nPriceUnit,nDecimal);

		m_pHxDataSource->m_pDataSrc->RemoveStyle( pCaclTemp->nAnaPeriod );
		m_pHxDataSource->m_pDataSrc->AddStyle( pCaclTemp->nAnaPeriod );

		CalcRetData retData;
		int nMask = (int)pCaclTemp->pPress->GetExpressType();
		CString strExpName = pCaclTemp->pPress->GetName();
		int nBegin = 0;
		int nEnd = (int)pDiskData->m_lSize;

		if( CalcAll(strExpName, nMask, 
			pCaclTemp->szTechMain, pCaclTemp->szTechSideLeft, 
			pCaclTemp->szTechSideRitht, retData, nBegin, nEnd) <= 0 )
		{
			return TCaclReturn_CalFail;
		}

		if( retData.pCalDataLeft == NULL || retData.pCalDataLeft->IsEmpty() )
			return TCaclReturn_CalFail;

		// 复制三指标数据
		if( pCaclTemp->pRetCacl->Copy(retData.pCalDataLeft->m_pData+nFindDataBegin, 
			pDiskData->m_lSize-nFindDataBegin) == FALSE ) 
		{
			return TCaclReturn_Fail;
		}
		
		if( retData.pCalDataRight != NULL && !retData.pCalDataRight->IsEmpty() )
		{
			if( pCaclTemp->pRetCacl->Copy(retData.pCalDataRight->m_pData+nFindDataBegin, 
				pDiskData->m_lSize-nFindDataBegin, TCaclResultTemp::ToLeft) == FALSE )
			{
				return TCaclReturn_Fail;
			}
		}

		if( retData.pCalDataMidlle != NULL && !retData.pCalDataMidlle->IsEmpty() )
		{
			if( pCaclTemp->pRetCacl->Copy(retData.pCalDataMidlle->m_pData+nFindDataBegin, 
				pDiskData->m_lSize-nFindDataBegin, TCaclResultTemp::ToRight) == FALSE ) 
			{
				return TCaclReturn_Fail;
			}
		}
		return TCaclReturn_Succ;
	}

#ifdef VC_7_0

	// 异常处理
	catch(CMemoryException e) //　内存异常
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("HSExchCalc MemoryException: %s", szCause);
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));

		DeleteAll();
		return TCaclReturn_MoneryFail;
	}
	catch(CFileException e) // 文件异常
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("HSExchCalc FileException: %s", szCause);
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}
#endif
	catch(...) // 其它异常
	{
		// 调试日志
		CString strError;
		strError.Format("HSExchCalc other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}

	DeleteAll();

	return TCaclReturn_Fail;
}


DWORD WINAPI CFormulaCalc::HSCalculate(LPVOID pParameter)
{
	CFormulaCalc::m_nStopThread = -2;
	try
	{
		DeleteAll();

		CFormulaCalc::CalcRetData* pRetData;
		UnionCalc*					pUnionCalc;

		BOOL bAndSucc  = FALSE;
		g_bUserHisDate = (((m_curSelStockCondition->m_dStyle & 0xFF00) & SelectStock_HisDate) == SelectStock_HisDate);

		//
		if( ((m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Single)  ||
			((m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Sort) )
		{
			UnionCalc* pUnionCalc = new UnionCalc;
			m_UnionArray.Add(pUnionCalc);
			pUnionCalc->bAndOr = 0;
			pUnionCalc->m_lDateCount = (short)m_curSelStockCondition->m_lDateCount; // date
			pUnionCalc->m_lEndDate   = m_curSelStockCondition->m_lEndDate;

			pRetData = new CFormulaCalc::CalcRetData;
			pRetData->pUnionData = m_curSelStockCondition->m_sSingleSel;
			pRetData->SetAsk(-1);
			pUnionCalc->subArray.Add(pRetData);
		}
		else if( ((m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Union)  ||
			((m_curSelStockCondition->m_dStyle & 0xF000) & SelectStock_SelStockAllCalc) )
		{
			bAndSucc = (((m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_AndSucc) == SelectStock_AndSucc);

			UnionSelCondition* pHSUnion = m_curSelStockCondition->m_pHSUnion;
			if( pHSUnion )
			{
				CArray<UnionSelConditionSub*,UnionSelConditionSub*>* pArray = pHSUnion->m_aySub;
				if( pArray )
				{
					UnionSelConditionSub* pHSUnionSelSub;
					SingleSelCondition*   pUnionData;
					for(int i = 0; i < pArray->GetSize(); i++)
					{
						pHSUnionSelSub = pArray->GetAt(i);

						pUnionCalc = new UnionCalc;
						m_UnionArray.Add(pUnionCalc);
						pUnionCalc->bAndOr = pHSUnionSelSub->m_cAndOr;
						if( pHSUnionSelSub->m_nBeginDate == 0 && pHSUnionSelSub->m_nEndDate == 0 ) // default date
						{
							pUnionCalc->m_lDateCount = (short)m_curSelStockCondition->m_lDateCount;
							pUnionCalc->m_lEndDate   = m_curSelStockCondition->m_lEndDate;
						}
						else
						{
							pUnionCalc->m_lDateCount = pHSUnionSelSub->m_nBeginDate;
							pUnionCalc->m_lEndDate   = pHSUnionSelSub->m_nEndDate;
						}

						for(int j = 0; j < pHSUnionSelSub->m_aySub->GetSize(); j++)
						{
							pUnionData = pHSUnionSelSub->m_aySub->GetAt(j);

							pRetData = new CFormulaCalc::CalcRetData;
							pUnionCalc->subArray.Add(pRetData);
							pRetData->pUnionData = pUnionData;
							pRetData->SetAsk(-1);
						}
					}
				}
			}
		}
		else
		{
			goto Calculate__; // 
		}

		//  
		int nBegin = 0;
		int nEnd;

		InitObj();

		CArray<StockUserInfo*,StockUserInfo*>* pAyStock = &StockArea::ayStock; // 股票总数
		int nCount = pAyStock->GetSize();

		//
		long nPriceUnit;
		int nDecimal;

		CNI_ReceiveData info;
		memset(&info,0,sizeof(CNI_ReceiveData));
		info.m_lDataTotal = nCount;

		int nRet = 0;
		int nPreRet = 0;
		StockUserInfo* pStock;
		int i,j,k;
		int nFindDateRet;

		int nMoreSucc,nUnionCount,nSubCount;
		SelStockResult SelectStockResult;

		CString strCondition;
		for( i = 0; i < nCount; i++ )
		{
			if( CFormulaCalc::m_nStopThread != -2 )
				break;

			pStock = pAyStock->GetAt(i);

			nUnionCount = m_UnionArray.GetSize();
			for( j = 0; j < nUnionCount; j++)
			{
				if( CFormulaCalc::m_nStopThread != -2 )
					break;

				pUnionCalc = m_UnionArray.GetAt(j);
				nSubCount = pUnionCalc->subArray.GetSize();
				for(k = 0; k < nSubCount; k++)
				{
					if( CFormulaCalc::m_nStopThread != -2 ) //
						break;

					pRetData = pUnionCalc->subArray.GetAt(k);


					if( pRetData->pUnionData->m_nType ==JiBenMian ) // 财务数据
					{
						// 设定财务数据
						m_pHxDataSource->m_pDataSrc->SetFinaceData(&pStock->m_ciStockCode);

						// calc
						strCondition.Format("%.3f",
							m_pHxDataSource->m_pDataSrc->GetFinaceDataByID( atol(pRetData->pUnionData->m_strCondition) ));

						pRetData->nResult = CFormulaCalc::SingleCal( pRetData->pUnionData,
							*pRetData,(int)&strCondition,0 );
					}
					else // 指标...
					{
						memcpy(&pRetData->ask.m_pCode->m_ciCode,&pStock->m_ciStockCode,sizeof(CodeInfo));
						RetDiskData* pDiskData = NULL;
							GetDataSourcePtr()->HSDataSource_GetDayData(&pRetData->ask,1);

						// SORT
						if( (m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Sort )
						{
							SelectStockResult.dData = hx_DefMinValues;
						}
						//
						if( pDiskData == NULL || pDiskData->IsEmpty() )
						{
							pRetData->nResult = 0;
						}
						else
						{
							nDecimal   = pStock->m_nDecimal;
							nPriceUnit = pStock->m_nPriceUnit;

							//
							// 设定日线数据
							nFindDateRet = FindDate(m_curSelStockCondition->m_lEndDate,
								m_curSelStockCondition->m_lDateCount,
								(StockDay*)pDiskData->m_pData,pDiskData->m_lSize);							

							if( nFindDateRet <= 0 /*|| nFindDateRet == 0*/ )
							{
								pRetData->nResult = 0;
							}
							else
							{
								StockDay* pDay = (StockDay*)pDiskData->m_pData;
								if( m_curSelStockCondition->m_lDateCount != 0 )
								{
									if( pDiskData->m_lSize > m_curSelStockCondition->m_lDateCount )
									{
										pDiskData->m_lSize = m_curSelStockCondition->m_lDateCount;
									}
									else
									{
										//AfxMessageBox("Over!");
									}
									nFindDateRet -= m_curSelStockCondition->m_lDateCount;
									pDay = (StockDay*)&pDay[__max(nFindDateRet,0)];
								}
								else
								{
									if( nFindDateRet < pDiskData->m_lSize )
									{
										pDiskData->m_lSize = nFindDateRet;
									}
								}

								//StockDay* pDay = (StockDay*)pDiskData->m_pData;
								nEnd = pDiskData->m_lSize;
								m_pHxDataSource->m_pDataSrc->SetDay( pDay,pDiskData->m_lSize,
									pRetData->nChengJiaoDiv,0,0,nPriceUnit,nDecimal);

								m_pHxDataSource->m_pDataSrc->RemoveStyle( pRetData->nAnaPeriod );
								m_pHxDataSource->m_pDataSrc->AddStyle( pRetData->nAnaPeriod );

								// calc
								if( (m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Sort ) // 排序
								{
									if( CalcAll( pRetData->pUnionData->m_strExp,
										pRetData->pUnionData->m_nType,
										pRetData->pUnionData->m_strCondition,
										pRetData->pUnionData->m_SubData.m_dData1,
										pRetData->pUnionData->m_SubData.m_dData2,
										*pRetData,
										nBegin,nEnd ) > 0 )
									{
										if( pRetData->pCalDataLeft && !pRetData->pCalDataLeft->IsEmpty() )
										{
											if( pRetData->pCalDataLeft->m_pData[nEnd] != (double)hx_DefMinValues )
											{
												SelectStockResult.dData = pRetData->pCalDataLeft->m_pData[nEnd];
											}
										}
									}									
								}
								else // 选股
								{
									pRetData->nResult = CFormulaCalc::SingleCal( pRetData->pUnionData,
										*pRetData,nBegin,nEnd );
								}
							}
						}
					}

					if( (m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Sort ) // sort
					{
					}
					else // 选股
					{
						for(int tt = 0;tt < pUnionCalc->subArray.GetSize();tt++)
						{
							pRetData = pUnionCalc->subArray.GetAt(tt);
							if(pRetData != NULL)
							{
								if(pRetData->m_aySucc.GetSize() >= 1)
								{
									pRetData->m_aySuccStock.Add(pStock);

									for( nMoreSucc = 0; nMoreSucc < pRetData->m_aySucc.GetSize(); nMoreSucc++)
									{
										if(pRetData->m_aySucc.GetAt(nMoreSucc))
										{
											delete pRetData->m_aySucc.GetAt(nMoreSucc);
										}
									}
									pRetData->m_aySucc.RemoveAll();
								}
							}

						}


						/*if( nSubCount == 1 )
						{
						pRetData = pUnionCalc->subArray.GetAt(0);
						for( nMoreSucc = 0; nMoreSucc < pRetData->m_aySucc.GetSize(); nMoreSucc++)
						{
						pUnionCalc->m_aySucc.Add(pRetData->m_aySucc.GetAt(nMoreSucc));
						}
						pRetData->m_aySucc.RemoveAll();
						}*/

						/*
						else if( nSubCount > 1 )
						{
						pRetData = pUnionCalc->subArray.GetAt(0);
						SuccResult* pSucc;
						SuccResult* pNextSucc;
						CFormulaCalc::CalcRetData* pNextRetData;
						for( int t = 0; t < pRetData->m_aySucc.GetSize(); t++ )
						{
						if( CFormulaCalc::m_nStopThread != -2 ) //
						break;

						pSucc = pRetData->m_aySucc.GetAt(t);
						for( nMoreSucc = 1; nMoreSucc < nSubCount; nMoreSucc++ )
						{
						if( CFormulaCalc::m_nStopThread != -2 ) //
						break;

						pNextRetData = pUnionCalc->subArray.GetAt(nMoreSucc);
						for( int tt = 0; tt < pNextRetData->m_aySucc.GetSize(); tt++ )
						{
						if( CFormulaCalc::m_nStopThread != -2 ) //
						break;

						pNextSucc = pNextRetData->m_aySucc.GetAt(tt);
						if( pUnionCalc->bAndOr )
						{
						if( pNextSucc->m_lDate == pSucc->m_lDate )
						{
						pUnionCalc->AddSucc(new SuccResult(pSucc->m_lDate,pSucc->m_nPos));
						}
						}
						else
						{
						pUnionCalc->AddSucc(new SuccResult(pSucc->m_lDate,pSucc->m_nPos));
						}
						}
						pNextRetData->RemoveAllSucc();
						}
						pRetData->RemoveAllSucc();
						}
						}
						*/
					}
				}
			}

			// 
			if( nUnionCount == 1 )
			{
				nRet = pUnionCalc->m_aySucc.GetSize();

				CArray<SuccResult*,SuccResult*>* paySucc = NULL;
				for( int h = 0; h < pUnionCalc->m_aySucc.GetSize(); h++)
				{
					if( paySucc == NULL )
					{
						paySucc = new CArray<SuccResult*,SuccResult*>;
						AddSuccDataToMap(&pStock->m_ciStockCode,paySucc);
					}
					paySucc->Add(pUnionCalc->m_aySucc.GetAt(h));
				}
				pUnionCalc->m_aySucc.RemoveAll();
			}
			else if( nUnionCount > 1 )
			{
				pUnionCalc = m_UnionArray.GetAt(0);  // 组合条件中基本条件
				UnionCalc*	pNextUnionCalc;
				SuccResult* pSucc;
				SuccResult* pNextSucc;

				CArray<SuccResult*,SuccResult*>* paySucc = NULL;
				for( int h = 0; h < pUnionCalc->m_aySucc.GetSize(); h++)
				{
					pSucc = pUnionCalc->m_aySucc.GetAt(h);
					for( int hh = 1; hh < nUnionCount; hh++ )
					{
						pNextUnionCalc = m_UnionArray.GetAt(hh);
						for( int hhh = 0; hhh < pNextUnionCalc->m_aySucc.GetSize(); hhh++)
						{
							pNextSucc = pNextUnionCalc->m_aySucc.GetAt(hhh);
							if( Include(pNextUnionCalc,pSucc,pNextSucc) )  // pNextSucc->m_lDate == pSucc->m_lDate
							{
								nRet++;
								if( paySucc == NULL )
								{
									paySucc = new CArray<SuccResult*,SuccResult*>;
									AddSuccDataToMap(&pStock->m_ciStockCode,paySucc);
								}
								paySucc->Add(new SuccResult(pSucc->m_lDate,pSucc->m_nPos));
							}
						}
						pNextUnionCalc->RemoveAllSucc();
					}
				}
				pUnionCalc->RemoveAllSucc();
			}

			// 当前结果 
			info.m_lDataTransmited = i;
			SelectStockResult.pStock = (void*)pStock;
			if( (m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Sort ) // sort
			{
				info.m_pszData = (const char*)&SelectStockResult;
			}
			else // 选股
			{
				if( m_pMsgWnd != NULL && nRet ) // middle
				{
					info.m_nStatus ++;
					info.m_pszData = (const char*)&SelectStockResult;
				}
				else
				{
					info.m_pszData = NULL;
				}
			}
			m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,4,(LPARAM)&info); // 选出股票结果

			nRet = 0;
		} // 股票大循环
		for(int ttt = 0;ttt < pUnionCalc->subArray.GetSize();ttt++)
		{
			pRetData = pUnionCalc->subArray.GetAt(ttt);
			if( pRetData != NULL)
			{
				CString strSection;
				SingleSelCondition* pHSUnion = pRetData->pUnionData;
				if(pHSUnion != NULL)
				{
					/*
					strSection = *(pHSUnion->m_strCondition;
					strSection += "+";*/
					strSection = "";
					strSection += pHSUnion->m_strExp;
				}
				SaveToIni(strSection,"");

				StockUserInfo* pInfo;
				for(int kkk = 0;kkk < pRetData->m_aySuccStock.GetSize(); kkk++)
				{
					pInfo = pRetData->m_aySuccStock.GetAt(kkk);
					if( pInfo != NULL)
					{
						CString strKey;
						strKey = pInfo->m_ciStockCode.GetKey();
						SaveToIni(strSection,strKey);
					}

				}
			}

		}

		DeleteAll();

	}
	catch(...)
	{
	}

Calculate__:

	if( m_pMsgWnd != NULL ) // end
	{
		m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,5,
			(LPARAM)m_curSelStockCondition->m_dStyle); // 选出股票结果
	}

	DeleteAll();

	CFormulaCalc::m_nStopThread = 0;

	return 0;
}

/********************************************************************************
* 函数功能 : 查找成立条件
* 函数参数 : SuccResult* pSucc,CArray<SuccResult*,SuccResult*>& aySucc
* 返 回 值 : SuccResult*
* 调用依赖 : void CheckUnionCondition(UnionCalc* pUnionCalc) 等函数中使用
* 作    者 : 
* 完成日期 : 2003-08-01
* 修改记录 : 
*******************************************************************************/
SuccResult* FindBySuccPoint(SuccResult* pSucc,CArray<SuccResult*,SuccResult*>& aySucc)
{
	if( pSucc == NULL )
		return NULL;

	SuccResult* pCurSucc;
	for( int i = aySucc.GetSize() - 1; i >= 0; i-- )
	{
		pCurSucc = aySucc.GetAt(i);
		if( (pSucc->m_lDate == pCurSucc->m_lDate) ||			// 日期对比
			(pCurSucc->m_lDate == -1 && pCurSucc->m_nPos == -1) // 财务数据
		  )
		{
			return pSucc;
		}
	}

	return NULL;
}

/********************************************************************************
* 函数功能 : 组合条件是否成立
* 函数参数 : UnionCalc* pUnionCalc
* 返 回 值 : void
* 调用依赖 : DWORD WINAPI CFormulaCalc::Calculate(LPVOID pParameter) 等函数中使用
* 作    者 : 
* 完成日期 : 2003-08-01
* 修改记录 : 
*******************************************************************************/
void CheckUnionCondition(CFormulaCalc::UnionCalc* pUnionCalc)
{
	if( pUnionCalc == NULL )
		return;

	SuccResult* pSucc;
	CFormulaCalc::CalcRetData* pRetData;
	
	int i;
	int j;
	
	if( pUnionCalc->bAndOr ) // 同成立
	{
		SuccResult* pFindSucc;
		CFormulaCalc::CalcRetData* pBaseRetData = NULL;

		int nType = 2;

		// 查找不是财务数据的结果
		int nSize = hx_DefMinValues;
		int nCount = pUnionCalc->subArray.GetSize() - 1;
		for( i = nCount; i >= 0; i-- )
		{
			pRetData = pUnionCalc->subArray.GetAt(i);
			if( pRetData->m_aySucc.GetSize() > 0 )
			{
				if( (i == nCount) &&
					nType		  &&
					(pRetData->pUnionData->m_nType == JiBenMian) )
				{
					nType = 1;
				}
				else
				{
					nType = 0;
				}

				pSucc = pRetData->m_aySucc.GetAt(0);
				if( pSucc->m_lDate != -1 && pSucc->m_nPos != -1 )
				{
					if( nSize > pRetData->m_aySucc.GetSize() )
					{
						pBaseRetData = pRetData;
					}
				}
				nSize = pRetData->m_aySucc.GetSize();
			}
			else
			{
				nSize = -1;
				break;
			}
		}

		if( pBaseRetData == NULL )
		{
			pBaseRetData = pUnionCalc->subArray.GetAt(0);			
		}

		if( nSize != -1 && pBaseRetData ) // 如果 pBaseRetData == NULL，则全是财务比较，这样在k线上将无法显示
		{
			for( j = pBaseRetData->m_aySucc.GetSize() - 1; j >= 0; j-- )
			{
				pSucc = pBaseRetData->m_aySucc.GetAt(j);
				for( i = pUnionCalc->subArray.GetSize() - 1; i >= 0; i-- )
				{
					pRetData = pUnionCalc->subArray.GetAt(i);
					if( pRetData == pBaseRetData )
						continue;

					pFindSucc = FindBySuccPoint(pSucc,pRetData->m_aySucc);
					if( pFindSucc == NULL ) // 有一个不成立，则停止
						break;
				}
				if( pFindSucc )
				{
					if( (nType == 1)												  || // 全是财务数据
						(CFormulaCalc::m_curSelStockCondition->m_lDateCount == 0)	  || // 当前日期
						(pFindSucc->m_lDate  >= CFormulaCalc::m_curSelStockCondition->m_lDateCount && 
						 pFindSucc->m_lDate  <= CFormulaCalc::m_curSelStockCondition->m_lEndDate) )
					{
						pUnionCalc->AddSucc(new SuccResult(pFindSucc->m_lDate,pFindSucc->m_nPos));
					}
				}
			}	
		}
	
		// 删除
		for( i = nCount; i >= 0; i-- )
		{
			pRetData = pUnionCalc->subArray.GetAt(i);
			if( pRetData == NULL )
				continue;
			pRetData->RemoveAllSucc();
		}

	}
	else
	{
		for( i = pUnionCalc->subArray.GetSize() - 1; i >= 0; i-- )
		{
			pRetData = pUnionCalc->subArray.GetAt(i);
			if( pRetData == NULL )
				continue;

			SuccResult* pSuccResult;
			for( j = pRetData->m_aySucc.GetSize() - 1; j >= 0; j-- )
			{
				pSuccResult = pRetData->m_aySucc.GetAt(j);

				if( (pRetData->pUnionData->m_nType == JiBenMian) || // 财务数据
					(CFormulaCalc::m_curSelStockCondition->m_lDateCount == 0) || // 当前日期
					(pSuccResult->m_lDate >= CFormulaCalc::m_curSelStockCondition->m_lDateCount && 
					 pSuccResult->m_lDate <= CFormulaCalc::m_curSelStockCondition->m_lEndDate) )
				{
					pUnionCalc->AddSucc(pSuccResult);
				}
				else
				{
					delete pSuccResult;
				}
			}
			pRetData->m_aySucc.RemoveAll();
		}
	}

}

/********************************************************************************
* 函数功能 : 得到条件组合的文本描述
* 函数参数 : CArray<CFormulaCalc::UnionCalc*,CFormulaCalc::UnionCalc*>& ayUnionCalc
* 返 回 值 : CString
* 调用依赖 : DWORD WINAPI CFormulaCalc::Calculate(LPVOID pParameter) 等函数中使用
* 作    者 : 
* 完成日期 : 2003-08-05
* 修改记录 : 
*******************************************************************************/
CString GetDescribe(CArray<CFormulaCalc::UnionCalc*,CFormulaCalc::UnionCalc*>& ayUnionCalc)
{
	int i,j;

	CFormulaCalc::CalcRetData* pRet;
	SingleSelCondition*        pUnionData;
	CFormulaCalc::UnionCalc*   pUnionCalc;

	CString strRet,strTmp;
	for( i = 0; i < ayUnionCalc.GetSize(); i++ )
	{
		pUnionCalc = ayUnionCalc.GetAt(i);

		strTmp.Format("%i.",i+1);
		strRet += strTmp;
		strRet += pUnionCalc->bAndOr?CDlgUnionConSelStock_Cond_And:CDlgUnionConSelStock_Cond_Or;
		strRet += "\r\n";

		for( j = 0; j < pUnionCalc->subArray.GetSize(); j++ )
		{
			pRet = pUnionCalc->subArray.GetAt(j);
			if( pRet == NULL || pRet->pUnionData == NULL ) 
				continue;

			pUnionData = pRet->pUnionData;

			CString* strCurRet = new CString;
			switch( pUnionData->m_nType )      // 公式类型
			{
			case Tech:
				*strCurRet = G_STRTECH;
				break;
			case Condition:
				*strCurRet = G_STRCONDITION;
				break;
			case Exchange:
				*strCurRet = G_STREXCHANGE;
				break;
			case Strategy: //20090302 YJT 增加 Strategy
				*strCurRet = G_STRSTRATEGY;
				break;
			case MoreKLine:
				*strCurRet = G_STRCOLORKLINE;
				break;
			}

			strTmp.Format("%i.%i",i+1,j+1);
			strRet += strTmp;
			strRet += pUnionData->GetDescribe(strCurRet);
			strRet += "\r\n";

			delete strCurRet;
		}
	}

	return strRet;
}

DWORD WINAPI CFormulaCalc::Calculate(LPVOID pParameter)
{
	//int* nStopFlag = (int*)pParameter;
	CFormulaCalc::m_nStopThread = -2;

	try
	{
		DeleteAll();

		CFormulaCalc::CalcRetData* pRetData;
		UnionCalc*				   pUnionCalc;

		CArray<CFormulaCalc::FormulaIndicatorSortResult*,CFormulaCalc::FormulaIndicatorSortResult*> ayRetData;

		FormulaIndicatorSortResult* pBaseDataEx = NULL;
		CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>* ayOther = NULL;

		if( m_curSelStockCondition->m_pData != NULL )
		{
			FormulaAddColParam* pHSAddColParam = (FormulaAddColParam*)m_curSelStockCondition->m_pData;
			ayOther		= pHSAddColParam->payOther;
			pBaseDataEx = new FormulaIndicatorSortResult;
			AddBaseOtherColData(pBaseDataEx,*ayOther);
			ayRetData.Add(pBaseDataEx);
		}

		//
		BOOL bAndSucc  = FALSE;
		g_bUserHisDate = (((m_curSelStockCondition->m_dStyle & 0xFF00) & SelectStock_HisDate) == SelectStock_HisDate);

		// 条件预处理
		if( (m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Single )
		{
			UnionCalc* pUnionCalc = new UnionCalc;
			m_UnionArray.Add(pUnionCalc);
			pUnionCalc->bAndOr = 0;
			pUnionCalc->m_lDateCount = m_curSelStockCondition->m_lDateCount; // date
			pUnionCalc->m_lEndDate   = m_curSelStockCondition->m_lEndDate;

			pRetData = new CFormulaCalc::CalcRetData;
			
			pRetData->pUnionData = m_curSelStockCondition->m_sSingleSel;
			pRetData->SetAsk(m_curSelStockCondition->m_sSingleSel->m_nPeriod);
			pUnionCalc->subArray.Add(pRetData);

		}
		else if( (m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Union )
		{
			bAndSucc = (((m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_AndSucc) == SelectStock_AndSucc);

			UnionSelCondition* pHSUnion = m_curSelStockCondition->m_pHSUnion;
			if( pHSUnion )
			{
				CArray<UnionSelConditionSub*,UnionSelConditionSub*>* pArray = pHSUnion->m_aySub;
				if( pArray )
				{
					UnionSelConditionSub* pHSUnionSelSub;
					SingleSelCondition*   pUnionData;
					for(int i = 0; i < pArray->GetSize(); i++)
					{
						pHSUnionSelSub = pArray->GetAt(i);

						pUnionCalc = new UnionCalc;
						m_UnionArray.Add(pUnionCalc);
						pUnionCalc->bAndOr = pHSUnionSelSub->m_cAndOr;
						if( pHSUnionSelSub->m_nBeginDate == 0 && 
							pHSUnionSelSub->m_nEndDate   == 0 ) // default date
						{
							pUnionCalc->m_lDateCount = m_curSelStockCondition->m_lDateCount;
							pUnionCalc->m_lEndDate   = m_curSelStockCondition->m_lEndDate;
						}
						else
						{
							pUnionCalc->m_lDateCount = pHSUnionSelSub->m_nBeginDate;
							pUnionCalc->m_lEndDate   = pHSUnionSelSub->m_nEndDate;
						}

						for(int j = 0; j < pHSUnionSelSub->m_aySub->GetSize(); j++)
						{
							pUnionData = pHSUnionSelSub->m_aySub->GetAt(j);

							pRetData = new CFormulaCalc::CalcRetData;
							pUnionCalc->subArray.Add(pRetData);
							pRetData->pUnionData = pUnionData;
							pRetData->SetAsk(pUnionData->m_nPeriod);
						}
					}
				}
			}
		}
		else
		{
			goto Calculate__; // 
		}

		// 提示
		SuccResult::m_strTip = GetDescribe(m_UnionArray);

		//  
		int nBegin = 0;
		int nEnd;

		// 目标初始化
		InitObj();

		 // 股票总数
		CArray<StockUserInfo*,StockUserInfo*>* pAyStock = &StockArea::ayStock;
		int nCount = pAyStock->GetSize();

		//
		long nPriceUnit;
		int nDecimal;

		CNI_ReceiveData info;
		memset(&info,0,sizeof(CNI_ReceiveData));
		info.m_lDataTotal = nCount;

		int nRet = 0;
		int nPreRet = 0;
		StockUserInfo* pStock;
		register int i,j,k;

		CArray<StockUserInfo*,StockUserInfo*> aySelectRetStock; // 选股结果

		int nMoreSucc,nUnionCount,nSubCount;

		g_nStartCol = HS_USR_DEF_BEGIN + 40;
		SelStockResult SelectStockResult;

		/*
		
		以下选股执行图解:

		股票 loop----------------------------股票范围
		{
		   多个组合 loop---------------------组合条件设置,没有则自动生成一个
		   {
		       子组合 loop-------------------组合条件设置
			   {
			        单个条件-----------------到某个指标或条件等
					{
					    比较函数-------------指针函数
					    ...
					}
			   }
		   }
		}

		*/

		CString strCondition;
		for( i = 0; i < nCount; i++ ) // 股票
		{
			if( CFormulaCalc::m_nStopThread != -2 )
				break;

			pStock = pAyStock->GetAt(i);
			if( pStock == NULL )
				continue;
#ifdef _DEBUG
			if( pStock->m_ciStockCode.GetCode().Compare("600215") == 0 )
			{
				int nDebug = 1;
			}
#endif
			// empty
			for( k = ayRetData.GetSize() - 1; k >= 0; k-- )
			{
				ayRetData.GetAt(k)->EmptyData();
			}

			nUnionCount = m_UnionArray.GetSize();
			for( j = 0; j < nUnionCount; j++) // 组合条件中第n个子组合
			{
				if( CFormulaCalc::m_nStopThread != -2 )
					break;

				pUnionCalc = m_UnionArray.GetAt(j);
				nSubCount  = pUnionCalc->subArray.GetSize();
				for(k = 0; k < nSubCount; k++) // 每个单条件
				{
					if( CFormulaCalc::m_nStopThread != -2 ) //
						break;

					pRetData = pUnionCalc->subArray.GetAt(k);

					if( pRetData->pUnionData->m_nType == JiBenMian ) // 财务数据
					{
						AskData ask;
						memcpy(&ask,&pRetData->ask,sizeof(ask));
						memcpy(ask.m_pCode,&pStock->m_ciStockCode,sizeof(ask.m_pCode));
						m_pHxDataSource->m_pDataSrc->SetAsk( &ask,0 );						

						double d = hx_DefMaxValues;

						// calc
						UINT nID = atol(pRetData->pUnionData->m_strCondition);

						// 行情
						if( (nID >= COLUMN_HQ_BASE_BEGIN && nID <= COLUMN_HQ_BASE_END) ||
							(nID >= COLUMN_HQ_EX_BEGIN && nID <= COLUMN_HQ_EX_END) )
						{
							d = (double)m_pHxDataSource->m_pDataSrc->GetNowDataByCode( nID );
						}
						else
						{
							// 设定财务数据
							m_pHxDataSource->m_pDataSrc->SetFinaceData(&pStock->m_ciStockCode);
							d = (double)m_pHxDataSource->m_pDataSrc->GetFinaceDataByID( nID );
						}

						if( d == hx_DefMaxValues )
						{
							pRetData->nResult = 0;
						}
						else
						{
							strCondition.Format("%.3f",d);
							pRetData->nResult = CFormulaCalc::SingleCal( pRetData->pUnionData,
								*pRetData,(int)&strCondition,0 );
						}
					}
					else // 指标...
					{
						memcpy(&pRetData->ask.m_pCode->m_ciCode,&pStock->m_ciStockCode,sizeof(CodeInfo));
	
						//获取k线数据直接从数据引擎获取
						RetDiskData* pDiskData = NULL;
						GetDataSourcePtr()->HSDataSource_GetDayData(&pRetData->ask,1,((m_curSelStockCondition->m_dStyle & SelectStock_ChuQuan)?1:0));
					
						if( pDiskData == NULL || pDiskData->IsEmpty() )
						{
							pRetData->nResult = 0;
						}
						else
						{
							nDecimal   = pStock->m_nDecimal; 
							nPriceUnit = pStock->m_nPriceUnit;
							
							// 数据不做时段限制,将在结果中解决
							StockDay* pDay = (StockDay*)pDiskData->m_pData;
							nEnd = pDiskData->m_lSize;

							// 财务，除权...，用到的代码信息
							AskData ask;
							memcpy(&ask,&pRetData->ask,sizeof(ask));
							memcpy(ask.m_pCode,&pRetData->ask.m_pCode->m_ciCode,sizeof(ask.m_pCode));
							m_pHxDataSource->m_pDataSrc->SetAsk( &ask,0 );

							// 读取财务数据
							m_pHxDataSource->m_pDataSrc->SetFinaceData( &pRetData->ask.m_pCode->m_ciCode );

							
							// 设定数据源数据
							m_pHxDataSource->m_pDataSrc->SetDay( pDay,pDiskData->m_lSize,
								pRetData->nChengJiaoDiv,0,0,nPriceUnit,nDecimal);
							// 
							m_pHxDataSource->m_pDataSrc->RemoveStyle( pRetData->nAnaPeriod );
							m_pHxDataSource->m_pDataSrc->AddStyle( pRetData->nAnaPeriod );
							pRetData->nResult = CFormulaCalc::SingleCal( pRetData->pUnionData,
									*pRetData,nBegin,nEnd );
						}
					}
				}


				// 选股
				{
					if( nSubCount == 1 )
					{
						SuccResult* pSuccResult;
						pRetData = pUnionCalc->subArray.GetAt(0);
						for( nMoreSucc = 0; nMoreSucc < pRetData->m_aySucc.GetSize(); nMoreSucc++)
						{
							pSuccResult = pRetData->m_aySucc.GetAt(nMoreSucc);
							
							if( (pRetData->pUnionData->m_nType == JiBenMian) || // 财务数据
								(m_curSelStockCondition->m_lDateCount == 0)				  || // 当前日期
								(pSuccResult->m_lDate  >= m_curSelStockCondition->m_lDateCount && 
								 pSuccResult->m_lDate  <= m_curSelStockCondition->m_lEndDate) )
							{
								pUnionCalc->m_aySucc.Add(pSuccResult);
							}
							else
							{
								delete pSuccResult;
							}
						}
						pRetData->m_aySucc.RemoveAll();
					}
					else if( nSubCount > 1 )
					{
						CheckUnionCondition(pUnionCalc);
					}
				}

			}

			// 
			if( nUnionCount == 1 )
			{
				nRet = pUnionCalc->m_aySucc.GetSize();

				// 是否在k线上显示
				if( m_curSelStockCondition->m_dStyle & SelectStock_InK_Disp_Mark )
				{
					CArray<SuccResult*,SuccResult*>* paySucc = NULL;
					for( int h = pUnionCalc->m_aySucc.GetSize() - 1; h >= 0; h-- )
					{
						if( paySucc == NULL )
						{
							paySucc = new CArray<SuccResult*,SuccResult*>;
							AddSuccDataToMap(&pStock->m_ciStockCode,paySucc);
						}
						paySucc->Add(pUnionCalc->m_aySucc.GetAt(h));
					}
					pUnionCalc->m_aySucc.RemoveAll();
				}
				else
				{
					pUnionCalc->RemoveAllSucc();
				}
			}
			else if( nUnionCount > 1 ) // 组合条件的高级,现在禁用
			{
				pUnionCalc = m_UnionArray.GetAt(0);  // 组合条件中基本条件
				UnionCalc*	pNextUnionCalc;
				SuccResult* pSucc;
				SuccResult* pNextSucc;

				CArray<SuccResult*,SuccResult*>* paySucc = NULL;
				for( int h = 0; h < pUnionCalc->m_aySucc.GetSize(); h++)
				{
					pSucc = pUnionCalc->m_aySucc.GetAt(h);
					for( int hh = 1; hh < nUnionCount; hh++ )
					{
						pNextUnionCalc = m_UnionArray.GetAt(hh);
						for( int hhh = 0; hhh < pNextUnionCalc->m_aySucc.GetSize(); hhh++)
						{
							pNextSucc = pNextUnionCalc->m_aySucc.GetAt(hhh);
							if( Include(pNextUnionCalc,pSucc,pNextSucc) )  // pNextSucc->m_lDate == pSucc->m_lDate
							{
								nRet++;
								if( paySucc == NULL )
								{
									paySucc = new CArray<SuccResult*,SuccResult*>;
									AddSuccDataToMap(&pStock->m_ciStockCode,paySucc);
								}
								paySucc->Add(new SuccResult(pSucc->m_lDate,pSucc->m_nPos));
							}
						}
						pNextUnionCalc->RemoveAllSucc();
					}
				}
				pUnionCalc->RemoveAllSucc();
			}

			// 当前结果 
			info.m_lDataTransmited   = i;
			info.m_bFile = (long)pStock;
			SelectStockResult.pStock = pStock;
			SelectStockResult.dData.m_pOtherData = (HSDrawOtherData*)&ayRetData;
			SelectStockResult.pAyStock = &aySelectRetStock;
			if( m_pMsgWnd != NULL && nRet ) // middle
			{
				info.m_nStatus ++;
				info.m_pszData = (const char*)&SelectStockResult;

				aySelectRetStock.Add(pStock);
			}
			else
			{
				info.m_pszData = NULL;
			}
			if( m_pMsgWnd )
			{
				m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,SelectingStock,(LPARAM)&info); // 选出股票结果
			}

			nRet = 0;

		} // 股票大循环

		SelectStockResult.dData.m_pOtherData = NULL;

		DeleteAll();

	}
	catch(...)
	{
	}

Calculate__:

	DeleteAll();

	if( m_pMsgWnd != NULL ) // end
	{
		m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,SelectStockOver,
			(LPARAM)m_curSelStockCondition->m_dStyle); // 选出股票结果
	}

	CFormulaCalc::m_nStopThread = 0;
	DelAllSuccData(TRUE);
	return 0;
}


DWORD WINAPI CFormulaCalc::TechSortCalculate(LPVOID pParameter)
{
	// 开始计算
	CFormulaCalc::m_nStopThread = -2;

	CArray<SingleSelCondition*,SingleSelCondition*>  ayTempFree;
	FormulaIndicatorSortResult* pBaseDataEx = NULL;
	CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>* ayOther = NULL;
	try
	{
		DeleteAll();
		g_nStartCol = HS_USR_DEF_BEGIN + 40;

		CFormulaCalc::CalcRetData* pRetData;
		UnionCalc*					pUnionCalc;
		BOOL bAndSucc  = FALSE;
		BOOL bIsOnlyOther = FALSE;
		CArray<CFormulaCalc::FormulaIndicatorSortResult*,CFormulaCalc::FormulaIndicatorSortResult*> ayRetData;
		FormulaBaseOtherColData* pHSBaseOtherColData;

		// 指标初始化处理
		if( m_curSelStockCondition->m_pData != NULL )
		{
			pUnionCalc = new UnionCalc;
			if( pUnionCalc == NULL )
			{
				//WRITE_LOG(__FILE__, __LINE__, "分配内存错误");
				goto Calculate__;
			}

			m_UnionArray.Add(pUnionCalc);
			pUnionCalc->bAndOr = 0;
			// 起止时间
			pUnionCalc->m_lDateCount = m_curSelStockCondition->m_lDateCount;
			pUnionCalc->m_lEndDate   = m_curSelStockCondition->m_lEndDate;

			// 列信息
			FormulaAddColParam* pHSAddColParam = (FormulaAddColParam*)m_curSelStockCondition->m_pData;
			if( pHSAddColParam->payOther == NULL || pHSAddColParam->payOther->GetSize() < 1 )
			{
				//WRITE_LOG(__FILE__, __LINE__, "传入参数错误");
				goto Calculate__;
			}
			pBaseDataEx = new FormulaIndicatorSortResult;
			if( pBaseDataEx == NULL )
			{
				//WRITE_LOG(__FILE__, __LINE__, "分配内存错误");
				goto Calculate__;
			}
			// 初始化自定义列
			AddBaseOtherColData(pBaseDataEx,*pHSAddColParam->payOther);
			// 自定义列 空间独立
			ayOther = new CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>;
			if( ayOther == NULL )
			{
				//WRITE_LOG(__FILE__, __LINE__, "分配内存错误");
				goto Calculate__;
			}
			for( int i = 0; i < pHSAddColParam->payOther->GetSize(); i++ )
			{
				FormulaBaseOtherColData* pItem = pHSAddColParam->payOther->GetAt(i);
				if( pItem == NULL )
				{
					continue;
				}
				FormulaBaseOtherColData* pAddItem = new FormulaBaseOtherColData;
				*pAddItem = *pItem;
				ayOther->Add(pAddItem);
			}
			
			// 返回数据
			ayRetData.Add(pBaseDataEx);

			// 自定义指标列
			CArray<SingleSelCondition*,SingleSelCondition*>* pExArray = pHSAddColParam->pExArray;
			SingleSelCondition* pHSUnionData = NULL;
			/*************************************************************
			* 更改
			* 时间：  2003年9月10日
			* 描述：  加入了pExArray->GetSize() > 0 判断，因为在自定义分析
			*		  中，这有可能为0
			*************************************************************/
			if( pExArray != NULL && pExArray->GetSize() > 0 )
			{
				for( int i = 0; i < pExArray->GetSize(); i++)
				{
					pHSUnionData = pExArray->GetAt(i);
					if( pHSUnionData == NULL )
					{
						continue;
					}

					pRetData = new CFormulaCalc::CalcRetData;
					if( pRetData == NULL )
					{
						//WRITE_LOG(__FILE__, __LINE__, "分配内存错误");
						goto Calculate__;
					}
					pUnionCalc->subArray.Add(pRetData);
					pRetData->pUnionData = new SingleSelCondition;
					if( pRetData->pUnionData == NULL )
					{
						//WRITE_LOG(__FILE__, __LINE__, "分配内存错误");
						goto Calculate__;
					}
					pRetData->pUnionData->Copy(pHSUnionData);
					ayTempFree.Add(pRetData->pUnionData);
					pRetData->m_pHSTechSortResult = new FormulaIndicatorSortResult;
					/*************************************************************
					* 更改 为了适应周期 2003年9月10日
					*************************************************************/
					pRetData->SetAsk(AnalisysForDay);

					ayRetData.Add(pRetData->m_pHSTechSortResult);
				}
			}
			else
			{
				bIsOnlyOther = TRUE;
				pRetData = new CFormulaCalc::CalcRetData;
				pRetData->pUnionData = NULL;
				pRetData->m_pHSTechSortResult = NULL;
				pRetData->SetAsk(AnalisysForDay);
				pRetData->ask.m_pCode->m_cPeriod = PERIOD_TYPE_DAY;
				pRetData->ask.m_pCode->m_nPeriodNum = 1;
				pUnionCalc->subArray.Add(pRetData);
			}
			/**********更改结束*************/
		}

		InitObj();

		int nBegin = 0;
		int nEnd;
		CArray<StockUserInfo*,StockUserInfo*> sAyStockTemp;
		sAyStockTemp.Copy(StockArea::ayStock);
		CArray<StockUserInfo*,StockUserInfo*>* pAyStock = &sAyStockTemp; // 股票数组
		int nCount = pAyStock->GetSize(); // 股票数
		long nPriceUnit;
		int nDecimal;
		CNI_ReceiveData info;
		memset(&info,0,sizeof(CNI_ReceiveData));
		info.m_lDataTotal = nCount;

		int nRet = 0;
		int nPreRet = 0;
		StockUserInfo* pStock;
		register int i,j,k,t;
		int nFindDateRet;
		int nBeginFind;
		int nUnionCount,nSubCount;		
		SelStockResult SelectStockResult;
		CString strCondition;
		nUnionCount = m_UnionArray.GetSize();

		// 更改 为防止列无数据时，不出现列名的BUG
		if( bIsOnlyOther == FALSE )
		{
			for( i = 0; i < nUnionCount; i++ )
			{
				int nEndTemp = 0;
				int nBeginTemp = 0;
				pUnionCalc = m_UnionArray.GetAt(i);
				if( pUnionCalc == NULL )
				{
					continue;
				}

				for( k = 0; k < pUnionCalc->subArray.GetSize(); k++ )
				{
					if( CFormulaCalc::m_nStopThread != -2 ) //
						goto Calculate__;

					pRetData = pUnionCalc->subArray.GetAt(k);
					if( pRetData == NULL )
					{
						continue;
					}
					pRetData->m_bInitLeftRight = 1;
					pRetData->m_pLeft   = CFormulaCalc::GetDrawLine(*pRetData,pRetData->pUnionData->m_strCondition,pRetData->pUnionData->m_strExp,pRetData->pUnionData->m_nType,TRUE);
					pRetData->m_pRight  = CFormulaCalc::GetDrawLine(*pRetData,pRetData->pUnionData->m_SubData.m_dData1,pRetData->pUnionData->m_strExp,pRetData->pUnionData->m_nType);
					pRetData->m_pMiddle = CFormulaCalc::GetDrawLine(*pRetData,pRetData->pUnionData->m_SubData.m_dData2,pRetData->pUnionData->m_strExp,pRetData->pUnionData->m_nType);
				}
			}
		}

		// 计算(股票）
		for( i = 0; i < nCount; i++ )
		{
			if( CFormulaCalc::m_nStopThread != -2 )
				break;

			pStock = pAyStock->GetAt(i);

#ifdef _DEBUG
			if( pStock->GetKey().CompareNoCase("600611") == 0 )
			{
				int i = 0;
			}
#endif
			for( j = 0; j < nUnionCount; j++) // 条件数
			{
				if( CFormulaCalc::m_nStopThread != -2 )
					break;

				pUnionCalc = m_UnionArray.GetAt(j);
				nSubCount  = pUnionCalc->subArray.GetSize();

				// empty
				for( k = ayRetData.GetSize() - 1; k >= 0; k-- )
				{
					ayRetData.GetAt(k)->EmptyData();
				}
				
				pRetData = pUnionCalc->subArray.GetAt(0);

				if( pRetData != NULL )
				{
					memcpy(&pRetData->ask.m_pCode->m_ciCode,&pStock->m_ciStockCode,sizeof(CodeInfo));
					RetDiskData* pDiskData = NULL;
					if( m_curSelStockCondition->m_dStyle&SelectStock_ChuQuan )
					{
 						pDiskData = NULL;
						GetDataSourcePtr()->HSDataSource_GetDayData(&pRetData->ask,1);
					}
					else
					{
 						pDiskData =NULL;
						GetDataSourcePtr()->HSDataSource_GetDayData(&pRetData->ask,1);
					}

 					if( pDiskData == NULL || pDiskData->IsEmpty() )
 					{
 						pRetData->nResult = 0;
 					}
 					else
 					{
 						nDecimal   = pStock->m_nDecimal;
 						nPriceUnit = pStock->m_nPriceUnit;
 
 						// 设定日线数据
 						nBeginFind = 0;
 						if( m_curSelStockCondition->m_lDateCount > 0 ) // FIND BEGIN DATE 
 						{
 							nBeginFind = FindDateExact(m_curSelStockCondition->m_lDateCount, 0,
 							(StockDay*)pDiskData->m_pData,pDiskData->m_lSize);
 						}
 
 						// 是否为非精确定位
 						if( m_curSelStockCondition->m_dStyle&SelectStock_EndDate_UNEXACT )
 						{
 							nFindDateRet = FindDate(m_curSelStockCondition->m_lEndDate, 0,
 									(StockDay*)pDiskData->m_pData,pDiskData->m_lSize);
 							if( nFindDateRet >=  pDiskData->m_lSize)
 							{
 								nFindDateRet = pDiskData->m_lSize - 1;
 							}
 						}
 						else
 						{
 							nFindDateRet = FindDateExact(m_curSelStockCondition->m_lEndDate, 0,
 									(StockDay*)pDiskData->m_pData,pDiskData->m_lSize, FALSE);
 						}
 
 
 						if( nFindDateRet < 0 || nBeginFind < 0 )
 						{
 							pRetData->nResult = 0;
 						}
 						else
 						{
 							StockDay* pDay = (StockDay*)pDiskData->m_pData;
 							nBeginFind = (nBeginFind <= 1 ? 0 : nBeginFind);
 							//pDay += nBeginFind;
 							pDiskData->m_lSize = nFindDateRet/* - nBeginFind*/ + 1;
 							nEnd = pDiskData->m_lSize;
 
 							// 财务，除权...，用到的代码信息
 							AskData ask;
 							memcpy(&ask,&pRetData->ask,sizeof(ask));
 							memcpy(ask.m_pCode,&pRetData->ask.m_pCode->m_ciCode,sizeof(ask.m_pCode));
 							m_pHxDataSource->m_pDataSrc->SetAsk( &ask,0 );
 							// 读取财务数据
 							m_pHxDataSource->m_pDataSrc->SetFinaceData( &pRetData->ask.m_pCode->m_ciCode );
 							// 设定数据源数据
 							m_pHxDataSource->m_pDataSrc->SetDay( pDay,pDiskData->m_lSize,
 								pRetData->nChengJiaoDiv,0,0,nPriceUnit,nDecimal);
 							m_pHxDataSource->m_pDataSrc->RemoveStyle( pRetData->nAnaPeriod );
 							m_pHxDataSource->m_pDataSrc->AddStyle( pRetData->nAnaPeriod );
 
 							// 直接返回的数据
 							for( t = 0; ayOther && t < ayOther->GetSize(); t++ )
 							{
 								pHSBaseOtherColData = ayOther->GetAt(t);
 								// 设置参数
 								if( m_curSelStockCondition->m_lDateCount > 0 
 									&& pHSBaseOtherColData->m_pParam 
 									&& nBeginFind >= 0 )
 								{
 									LPNODELEMENT pParamItem = pHSBaseOtherColData->m_pParam->GetParam(0);	
 									if( pParamItem )
 									{
 										pParamItem->valoare = nFindDateRet - nBeginFind + 1;
 									}
 								}
 								if( pHSBaseOtherColData && ::IsWindow(m_pMsgWnd->m_hWnd) )
 								{
 									pHSBaseOtherColData->GetData(pBaseDataEx,nEnd-1);
 								}
 							}
 
 							if( bIsOnlyOther == FALSE )
 							{
 								// 公式数据 begin
 								for( k = 0; k < nSubCount; k++ )
 								{
 									if( CFormulaCalc::m_nStopThread != -2 )
 									{
 										break;
 									}
 
 									pRetData = pUnionCalc->subArray.GetAt(k);
 									CalcAll( 
 										pRetData->pUnionData->m_strExp,
 										pRetData->pUnionData->m_nType,
 										pRetData->pUnionData->m_strCondition,
 										pRetData->pUnionData->m_SubData.m_dData1,
 										pRetData->pUnionData->m_SubData.m_dData2,
 										*pRetData,
 										nBegin,nEnd ) ;
 								} // 公式数据 end
 							}
 
 						}
 					}
 				}
			}

			// 当前结果 
			info.m_lDataTransmited = i;
			SelectStockResult.pStock = pStock;
			SelectStockResult.dData.m_pOtherData = (HSDrawOtherData*)&ayRetData;
			info.m_pszData = (const char*)&SelectStockResult;

			if( m_pMsgWnd && ::IsWindow(m_pMsgWnd->m_hWnd) )
			{
				m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,4,(LPARAM)&info); // 选出股票结果			
			}
			else
			{
				//WRITE_LOG(__FILE__, __LINE__, "报表计算问题，窗口已不存在!");
				goto Calculate__;
			}

			nRet = 0;

		} // 股票大循环

		SelectStockResult.dData.m_pOtherData = NULL;

		// 删除空间
		if( pBaseDataEx )
		{
			delete pBaseDataEx;
		}
		if( ayOther )
		{
			_delArrayObj((*ayOther));
			delete ayOther;
			ayOther = NULL;
		}

		// 删除数据
		DeleteAll();
		for( int i = 0; i < ayTempFree.GetSize(); i++ )
		{
			delete ayTempFree.GetAt(i);
		}
		ayTempFree.RemoveAll();

		if( m_pMsgWnd != NULL && ::IsWindow(m_pMsgWnd->m_hWnd) ) // end
		{
			m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,5,
				(LPARAM)m_curSelStockCondition->m_dStyle); // 选出股票结果
		}

		CFormulaCalc::m_nStopThread = 0;
		return 0;
	}

#ifdef VC_7_0

	// 异常处理
	catch(CMemoryException e) //　内存异常
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("TechSort MemoryException: %s", szCause);
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}
	catch(CNotSupportedException e) // 文件异常
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("TechSort NotSupported: %s", szCause);
		// 调试日志
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}
#endif
	
	catch(...) // 其它异常
	{
		// 调试日志
		CString strError;
		strError.Format("TechSort other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}

Calculate__:

	try
	{
		// 删除空间
		if( pBaseDataEx )
		{
			delete pBaseDataEx;
		}
		if( ayOther )
		{
			_delArrayObj((*ayOther));
			delete ayOther;
			ayOther = NULL;
		}

		DeleteAll();
		for( int i = 0; i < ayTempFree.GetSize(); i++ )
		{
			delete ayTempFree.GetAt(i);
		}
		ayTempFree.RemoveAll();
		if( m_pMsgWnd != NULL && ::IsWindow(m_pMsgWnd->m_hWnd) ) // end
		{
			m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,5,
				(LPARAM)m_curSelStockCondition->m_dStyle); // 选出股票结果
		}
	}
	catch(...)
	{
		//WRITE_LOG(__FILE__, __LINE__, "排序计算内存异常");
	}

	CFormulaCalc::m_nStopThread = 0;
	return 0;
}


void CFormulaCalc::Stop()
{
	if( m_nStopThread == -2 )
	{
		m_nStopThread = 2;
		Sleep(100);
		/*
		if( m_hThread )
		{
		SuspendThread(m_hThread);
		CloseHandle(m_hThread);
		m_hThread = NULL;

		DeleteAll();

		if( m_pMsgWnd != NULL ) // end
		{
		m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,5,0); // 选出股票结果
		}
		}
		*/
	}
	//m_nStopThread = 0;
}

int CFormulaCalc::StartExchSys()
{
	//Stop();

	//if( m_nStopThread > 0 )
	//	return -1;

	//if( m_pExchSys == NULL )
	//	return -1;

	//if( (m_pExchSys->m_sTExchBuy.m_curSelStockCondition.m_dStyle & 0x000F) & ExchSys_Calc )
	//{
	//	HSCreateThread(CFormulaCalc::HSExchCalc);
	//	return 1;
	//}
	///*else if( (m_curSelStockCondition->m_dStyle & 0xFF00) & SelectStock_PhaseSort )
	//{
	//	HSCreateThread(CFormulaCalc::HSPhaseSortCalculate);
	//	return 1;
	//}
	//else if( (m_curSelStockCondition->m_dStyle & 0xFF00) & SelectStock_BlockCalc )
	//{
	//	HSCreateThread(CFormulaCalc::HSBlockCalculate);
	//	return 1;
	//}*/
	//else
	//{
	//	//CArray<StockArea*,StockArea*>* payCheck = m_curSelStockCondition->m_ayCheck; // 股票范围
	//	//CArray<StockArea*,StockArea*>* payCheck = m_pExchSys->m_ayCheck; // 股票范围

	//	//if( payCheck == NULL )
	//	//	return -1;

	//	//CArray<StockUserInfo*,StockUserInfo*>* pStock = &StockArea::ayStock;
	//	//if( pStock->GetSize() <= 0 )
	//	//	return -2;

	//	//DelAllSuccData(FALSE);
	//	//return HSCreateThread(CFormulaCalc::Calculate);
	//}

	return 0;
}

int CFormulaCalc::Start()
{
	Stop();

	if( m_nStopThread > 0 )
		return -1;

	if( m_curSelStockCondition == NULL )
		return -1;

	if( (m_curSelStockCondition->m_dStyle & 0xFF00) & SelectStock_SelStockAllCalc )
	{
		HSCreateThread(CFormulaCalc::HSCalculate);
		return 1;
	}
	else if( (m_curSelStockCondition->m_dStyle & 0xFF00) & SelectStock_PhaseSort )
	{
#ifndef _DEBUG
		//WRITE_LOG(__FILE__, __LINE__, "创建计算线程");
#endif
		HSCreateThread(CFormulaCalc::HSBlockReportCalulate);
		//HSCreateThread(CFormulaCalc::HSPhaseSortCalculate);
		return 1;
	}
	else if( (m_curSelStockCondition->m_dStyle & 0xFF00) & SelectStock_BlockCalc )
	{
		HSCreateThread(CFormulaCalc::HSBlockCalculate);
		return 1;
	}
	else if( (m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Sort )
	{
		HSCreateThread(CFormulaCalc::TechSortCalculate);
		return 1;
	}
	else
	{
// 		CArray<StockArea*,StockArea*>* payCheck = m_curSelStockCondition->m_ayCheck; // 股票范围
// 		if( payCheck == NULL )
// 			return -1;

		CArray<StockUserInfo*,StockUserInfo*>* pStock = &StockArea::ayStock;
		if( pStock->GetSize() <= 0 )
			return -2;

		DelAllSuccData(FALSE);
		return HSCreateThread(CFormulaCalc::Calculate);
	}

	return 0;
}

BOOL CFormulaCalc::HSCreateThread(LPTHREAD_START_ROUTINE fnThreadFun)
{
	g_dwThreadID = -1;
	m_nStopThread = 1;
	m_hThread = CreateThread(NULL, 0, fnThreadFun, 
		(void *)&m_nStopThread, 0, &g_dwThreadID);
	if (m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
		while (m_nStopThread == 1)
		{
			Sleep(20);
		}
		if (m_nStopThread == -1) // fail
		{
			return FALSE;
		}
	}
	else // fail
	{
		return FALSE;
	}

	return TRUE;
}

int CFormulaCalc::FindDateExact(unsigned long lDate,long lLen,StockDay* pDay, int lSize, BOOL bIsBegin)
{
	if( lSize <= 0 || pDay == NULL ) 
		return -1;

	int  nBegin = -1,nEnd = -1;
	long lHigh = lSize - 1;
	long lLow = 0;
	long lMid = -1;
	do
	{
		lMid = (lLow + lHigh) / 2;
		if( pDay[lMid].m_lDate > lDate )
		{			
			nEnd   = lMid - 1;
			lHigh  = lMid - 1;
		}
		else if( pDay[lMid].m_lDate < lDate )
		{
			nBegin = lMid;
			lLow   = lMid + 1;
		}
		else
		{
			nBegin = -1;
			nEnd   = -1;
			break;
		}
	}
	while (lLow <= lHigh);

	if( nEnd != -1 && nBegin != -1 )
	{
		lLow  = __min(nEnd,nBegin);
		lHigh = __max(nEnd,nBegin);

		nBegin = 100;
		for(; lLow < lHigh; lLow++)
		{
			if( pDay[lLow].m_lDate < lDate )
			{
				if( nBegin == 100 )
					nBegin = -1;
				if( nBegin == 1 )
				{				
					lMid = lLow;
					break;
				}
			}
			else if( pDay[lLow].m_lDate > lDate )
			{
				if( nBegin == 100 )
					nBegin = 1;
				if( nBegin == -1 )
				{
					lMid = lLow;
					break;
				}
			}
		}
	}

	if( lMid+1 > lSize || lMid < 0)
		return -1;

	// 找到相应日期
	if( pDay[lMid].m_lDate == lDate )
	{
		return lMid;
	}

	// 改进
	// 查找的日期不存在处理
	if( bIsBegin )
	{
		// 并比找到最后的日期小， 如在12 13 15 16在找14最后定位到15
		// 则最后找到的结果为15
		if( pDay[lMid].m_lDate > lDate
			&& lMid > 0 && lMid < lSize 
			&& pDay[lMid-1].m_lDate < lDate )
		{
			return lMid;
		}
		else if( lMid >= 0 && lMid+1 < lSize     // 这是最后定位到13的情况
			&& pDay[lMid+1].m_lDate > lDate )
		{
			return lMid+1;
		}
		if( pDay[0].m_lDate > lDate )
		{
			return 0;
		}
	}
	else
	{
		// 并比找到最后的日期小， 如在12 13 15 16在找14最后定位到15
		// 则最后找到的结果为13
		if( pDay[lMid].m_lDate > lDate
			&& lMid > 0 && lMid < lSize 
			&& pDay[lMid-1].m_lDate < lDate )
		{
			return lMid-1;
		}
		else if( lMid >= 0 && lMid+1 < lSize     // 这是最后定位到13的情况
			&& pDay[lMid+1].m_lDate > lDate )
		{
			return lMid;
		}
	}

	return -1;
}

int CFormulaCalc::FindDate(unsigned long lDate,long lLen,StockDay* pDay,int lSize)
{
	if( lSize <= 0 || pDay == NULL ) 
		return -1;

	int  nBegin = -1,nEnd = -1;
	long lHigh = lSize - 1;
	long lLow = 0;
	long lMid = -1;
	do
	{
		lMid = (lLow + lHigh) / 2;
		if( pDay[lMid].m_lDate > lDate )
		{			
			nEnd   = lMid - 1;
			lHigh  = lMid - 1;
		}
		else if( pDay[lMid].m_lDate < lDate )
		{
			nBegin = lMid;
			lLow   = lMid + 1;
		}
		else
		{
			nBegin = -1;
			nEnd   = -1;
			break;
		}
	}
	while (lLow <= lHigh);

	if( nEnd != -1 && nBegin != -1 )
	{
		lLow  = __min(nEnd,nBegin);
		lHigh = __max(nEnd,nBegin);

		nBegin = 100;
		for(; lLow < lHigh; lLow++)
		{
			if( pDay[lLow].m_lDate < lDate )
			{
				if( nBegin == 100 )
					nBegin = -1;
				if( nBegin == 1 )
				{				
					lMid = lLow;
					break;
				}
			}
			else if( pDay[lLow].m_lDate > lDate )
			{
				if( nBegin == 100 )
					nBegin = 1;
				if( nBegin == -1 )
				{
					lMid = lLow;
					break;
				}
			}
		}
	}

	if( lMid > lSize )
		lMid = lSize;

	if( pDay[lMid].m_lDate < lDate )
		lMid = __min(lMid+1,lSize);

	return lMid;
}

BOOL CFormulaCalc::InitObj()
{
//  	if( m_pHSDisk == NULL )
//  	{
//  		m_pHSDisk = new CHSDisk();
//  	}
	//
	if( m_pHxDataSource == NULL )
	{
		m_pHxDataSource = new InportDataParam();
		m_pHxDataSource->m_pDataSrc = new CHSDataSource;
		m_pHxDataSource->m_pDataSrc->SetStyle(0);
	}
	return TRUE;
}
BOOL CFormulaCalc::SaveToIni(CString strSection ,CString strKeyName)
{
	CString strFile =CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH);// 
	strFile += "IntelligentSelect.ini";
	if( _access(strFile, 0) != 0 )
	{
		FILE* fp = fopen(strFile, "w");
		fclose(fp);
		return false;
	}
	if(strKeyName == "")
	{
		::WritePrivateProfileString(strSection,
			NULL,"",strFile);
	}
	else
	{
		::WritePrivateProfileString(strSection,
			strKeyName,"",strFile);
	}
	return TRUE;
}

int	CFormulaCalc::StartAlarm()
{
	if( m_nStopAlarmThread == -2 )
		return 1;	//线程以开启
	g_dwThreadID = -1;
	m_nStopAlarmThread = 1;
	m_hThread = CreateThread(NULL, 0, AlarmCalculate, 
		(void *)&m_nStopAlarmThread, 0, &g_dwThreadID);
	if (m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
		while (m_nStopAlarmThread == 1)
		{
			Sleep(20);
		}
		if (m_nStopAlarmThread == -1) // fail
		{
			return -1;
		}
	}
	else // fail
	{
		return -1;
	}
	return 0;
}

void CFormulaCalc::StopAlarm()
{
	if( m_nStopAlarmThread == -2 )
	{
		m_nStopAlarmThread = 2;
		Sleep(100);
	}
}

DWORD WINAPI CFormulaCalc::AlarmCalculate(LPVOID pParameter)
{
	CFormulaCalc::m_nStopAlarmThread = -2;

	try
	{
		//删除原来计算数据
		AlarmDeleteAll();

		CFormulaCalc::CalcRetData* pRetData;
		UnionCalc*					pUnionCalc;
		BOOL bAndSucc  = FALSE;

		if(AlarmCreateAll() == FALSE )
		{
			//发送消息到主窗口，说明预警没有启动
			goto AlarmCalculate__;
		}
		// 初始化数据源
		InitAlarmObj();

		int nBegin = 0;
		int nEnd;
		CArray<StockUserInfo*,StockUserInfo*>* pAyStock;// = &StockArea::ayStock; // 股票总数
		AlarmCalcData* pHSCalData;
		int nCount;
		int nBeginCount, nEndCount;
		long nPriceUnit;
		int nDecimal;
		int nRet = 0;
		int nPreRet = 0;
		StockUserInfo* pStock;
		int i,j,k;
		int nFindDateRet;
		int nMoreSucc,nUnionCount,nSubCount;
		SelStockResult SelectStockResult;
		CString strCondition;
		int	nAlarmFor;

		// 线程死循环开始
		while( 1 )
		{
			//预警条件大循环开始
			time_t lBeginTime, lEndTime;
			time(&lBeginTime);
			for( nAlarmFor = 0; nAlarmFor < m_pAlarmCondition->GetSize(); nAlarmFor++ )
			{
				if( CFormulaCalc::m_nStopAlarmThread != -2 )
					goto AlarmCalculate__;
				// 行到当前条件股票及计算公式的开始结束行号
				pHSCalData = m_pAlarmCondition->GetAt(nAlarmFor);

				if( pHSCalData == NULL || pHSCalData->m_bFlag == FALSE )
					continue;
				pAyStock = &pHSCalData->m_ayStkList;
				nCount = pAyStock->GetSize();
				nBeginCount = pHSCalData->m_nUnionCalBegin;
				nEndCount = pHSCalData->m_nUnionCalEnd;

				// 开始股票大循环
				for( i = 0; i < nCount; i++ )
				{
					if( CFormulaCalc::m_nStopAlarmThread != -2 )
						goto AlarmCalculate__;

					pStock = pAyStock->GetAt(i);
					if( pStock->m_ciStockCode.GetCode().CompareNoCase("500056") == 0 )
					{
						int nI;
						nI = 1;
					}

					if( nEndCount < nBeginCount || nEndCount > m_AlarmUnionArray.GetSize() )
						goto AlarmCalculate__;
					nUnionCount = nEndCount - nBeginCount;

					// 开始计算条件循环--根据上面得到的开始行与结束行号
					for( j = nBeginCount; j < nEndCount; j++)
					{
						if( CFormulaCalc::m_nStopAlarmThread != -2 )
							goto AlarmCalculate__;

						pUnionCalc = m_AlarmUnionArray.GetAt(j);
						nSubCount = pUnionCalc->subArray.GetSize();
						for(k = 0; k < nSubCount; k++)
						{
							if( CFormulaCalc::m_nStopAlarmThread != -2 ) //
								goto AlarmCalculate__;

							pRetData = pUnionCalc->subArray.GetAt(k);

							if( pRetData->pUnionData->m_nType == JiBenMian ) // 财务数据
							{
								// 设定财务数据
								m_pAlarmHxDataSource->m_pDataSrc->SetFinaceData(&pStock->m_ciStockCode);

								// calc
								strCondition.Format("%.3f",
									m_pAlarmHxDataSource->m_pDataSrc->GetFinaceDataByID( atol(pRetData->pUnionData->m_strCondition) ));

								pRetData->nResult = CFormulaCalc::SingleCal( pRetData->pUnionData,
									*pRetData,(int)&strCondition,0 );
							}
							else // 指标...
							{
 								memcpy(&pRetData->ask.m_pCode->m_ciCode,&pStock->m_ciStockCode,sizeof(CodeInfo));
   								RetDiskData* pDiskData = NULL;
								GetDataSourcePtr()->HSDataSource_GetDayData(&pRetData->ask,1);
 								//
 								if( pDiskData == NULL || pDiskData->IsEmpty() )
 								{
 									pRetData->nResult = 0;
 								}
 								else
 								{
 									nDecimal   =  pStock->m_nDecimal;
 									nPriceUnit =  pStock->m_nPriceUnit;
 
 									// 设定日线数据
 									nFindDateRet = FindDate(pHSCalData->m_pCondition->m_lEndDate,
 										pHSCalData->m_pCondition->m_lDateCount,
 										(StockDay*)pDiskData->m_pData,pDiskData->m_lSize);							
 
 									if( nFindDateRet <= 0 /*|| nFindDateRet == 0*/ )
 									{
 										pRetData->nResult = 0;
 									}
 									else
 									{
 										StockDay* pDay = (StockDay*)pDiskData->m_pData;
 										if( pHSCalData->m_pCondition->m_lDateCount != 0 )
 										{
 											if( pDiskData->m_lSize > pHSCalData->m_pCondition->m_lDateCount )
 											{
 												pDiskData->m_lSize = pHSCalData->m_pCondition->m_lDateCount;
 											}
 											else
 											{
 												//AfxMessageBox("Over!");
 											}
 											nFindDateRet -= pHSCalData->m_pCondition->m_lDateCount;
 											pDay = (StockDay*)&pDay[__max(nFindDateRet,0)];
 										}
 										else
 										{
 											if( nFindDateRet < pDiskData->m_lSize )
 											{
 												pDiskData->m_lSize = nFindDateRet;
 											}
 										}
 
 										//StockDay* pDay = (StockDay*)pDiskData->m_pData;
 										nEnd = pDiskData->m_lSize;
 										// 财务，除权...，用到的代码信息
 										AskData ask;
 										memcpy(&ask,&pRetData->ask,sizeof(ask));
 										memcpy(ask.m_pCode,&pRetData->ask.m_pCode->m_ciCode,sizeof(ask.m_pCode));
 										m_pAlarmHxDataSource->m_pDataSrc->SetAsk( &ask,0 );
 										// 读取财务数据
 										m_pAlarmHxDataSource->m_pDataSrc->SetFinaceData( &pRetData->ask.m_pCode->m_ciCode );
 
 										// 设定数据源数据
 										m_pAlarmHxDataSource->m_pDataSrc->SetDay( pDay,pDiskData->m_lSize,
 											pRetData->nChengJiaoDiv,0,0,nPriceUnit,nDecimal);
 
 										m_pAlarmHxDataSource->m_pDataSrc->RemoveStyle( pRetData->nAnaPeriod );
 										m_pAlarmHxDataSource->m_pDataSrc->AddStyle( pRetData->nAnaPeriod );
 
 										pRetData->nResult = CFormulaCalc::SingleCal( pRetData->pUnionData,
 											*pRetData,nBegin,nEnd );
 									}
 								}
							}

							/*	pRetData = pUnionCalc->subArray.GetAt(0);
							for( nMoreSucc = 0; nMoreSucc < pRetData->m_aySucc.GetSize(); nMoreSucc++)
							{
							pUnionCalc->m_aySucc.Add(pRetData->m_aySucc.GetAt(nMoreSucc));
							}
							pRetData->m_aySucc.RemoveAll();*/
						}

						if( nSubCount == 1 )
						{
							pRetData = pUnionCalc->subArray.GetAt(0);
							for( nMoreSucc = 0; nMoreSucc < pRetData->m_aySucc.GetSize(); nMoreSucc++)
							{
								pUnionCalc->m_aySucc.Add(pRetData->m_aySucc.GetAt(nMoreSucc));
							}
							pRetData->m_aySucc.RemoveAll();
						}

						else if( nSubCount > 1 )
						{

							bool bFindFlag = false;

							for(int Sub = 0;Sub < nSubCount;Sub ++)
							{
								if(!bFindFlag)
								{
									pRetData = pUnionCalc->subArray.GetAt(Sub);
									if(pRetData->m_aySucc.GetSize() >= 1)
									{
										if(!(pUnionCalc->bAndOr) )
										{
											bFindFlag = true;
											pUnionCalc->m_aySucc.Add(pRetData->m_aySucc.GetAt(0));
										}

										if(Sub == nSubCount-1)
										{
											bFindFlag = true;
											pUnionCalc->m_aySucc.Add(pRetData->m_aySucc.GetAt(0));
										}
									}
									else
									{
										if((pUnionCalc->bAndOr) )
										{
											bFindFlag = true;
										}
									}
								}
								pRetData->m_aySucc.RemoveAll();
							}
						}
					}// 结束计算条件循环

					if( nUnionCount == 1 )
					{
						nRet = pUnionCalc->m_aySucc.GetSize();
					}
					else if( nUnionCount > 1 )
					{
						pUnionCalc = m_AlarmUnionArray.GetAt(nBeginCount);  // 组合条件中基本条件
						UnionCalc*	pNextUnionCalc;
						SuccResult* pSucc;
						SuccResult* pNextSucc;

						CArray<SuccResult*,SuccResult*>* paySucc = NULL;
						for( int h = 0; h < pUnionCalc->m_aySucc.GetSize(); h++)
						{
							pSucc = pUnionCalc->m_aySucc.GetAt(h);
							for( int hh = nBeginCount + 1; hh < nEndCount; hh++ )
							{
								pNextUnionCalc = m_AlarmUnionArray.GetAt(hh);
								for( int hhh = 0; hhh < pNextUnionCalc->m_aySucc.GetSize(); hhh++)
								{
									pNextSucc = pNextUnionCalc->m_aySucc.GetAt(hhh);
									if( Include(pNextUnionCalc,pSucc,pNextSucc) )  // pNextSucc->m_lDate == pSucc->m_lDate
									{
										nRet++;
									}
								}
								pNextUnionCalc->RemoveAllSucc();
							}
						}
					}

					// 统计当前结果备查: nRet > 0 表示有股票选出， 否则无
					pUnionCalc->RemoveAllSucc();

					// 查结果，是否有符合条件的股票，如果有，则发警告消息， 没有则跳过
					if( m_pAlarmWnd != NULL && nRet ) // middle
					{
						//发送警报信息
						AlarmDisplayData AlarmData;

						AlarmData.m_Stock = pStock->m_ciStockCode;
						AlarmData.m_nIndex = nAlarmFor;
						AlarmData.m_pData = (void *)pHSCalData;
						if( pHSCalData->m_pCondition->m_sSingleSel != NULL )
						{
							AlarmData.m_strDesc = pHSCalData->m_pCondition->m_sSingleSel->GetDescribe();
						}
						else
						{
							AlarmData.m_strDesc.Empty();
						}
						BOOL bBreak = (BOOL)::SendMessage(m_pAlarmWnd->m_hWnd, HX_USER_COMPILEDATA,27,(LPARAM)&AlarmData); // 选出股票结果
						if( bBreak == FALSE )
						{
							pHSCalData->m_bFlag = FALSE;
						}
						AlarmData.m_pData = NULL;
					}
					nRet = 0;
					Sleep((nCount > 500) ? 500 : ( nCount < 50 ) ? 50 : nCount);
				} // 股票大循环结束
			}// 预警条件大循环结束

			pHSCalData = m_pAlarmCondition->GetAt(0);
			if( pHSCalData != NULL )
				pHSCalData->m_nCalculateCount++;

			time(&lEndTime);
			if( lEndTime - lBeginTime < pHSCalData->m_nCycle )
				Sleep( (pHSCalData->m_nCycle - lEndTime + lBeginTime) * 1000);
		}// 死循环结束
	}

	// 导常处理
	catch(...)
	{
	}


	// 失败处理
AlarmCalculate__:

	if( m_pAlarmWnd != NULL ) // end
	{
		m_pAlarmWnd->SendMessage(HX_USER_COMPILEDATA,27,
			(LPARAM)0); // 选出股票结果
	}

	AlarmDeleteAll();

	CFormulaCalc::m_nStopAlarmThread = 0;
	return 0;
}

BOOL CFormulaCalc::InitAlarmObj()
{
// 	if( m_pAlarmDisk == NULL )
// 	{
// 		m_pAlarmDisk = new CHSDisk();
// 	}
	// 注释 by xudan 
	if( m_pAlarmHxDataSource == NULL )
	{
		m_pAlarmHxDataSource = new InportDataParam();
		m_pAlarmHxDataSource->m_pDataSrc = new CHSDataSource;
		m_pAlarmHxDataSource->m_pDataSrc->SetStyle(0);
	}
	return TRUE;
}

void CFormulaCalc::AlarmDeleteAll()
{
	CFormulaCalc::CalcRetData* pRetData;
	UnionCalc*					pUnionCalc;

	for(int j = 0; j < m_AlarmUnionArray.GetSize(); j++)
	{
		pUnionCalc = m_AlarmUnionArray.GetAt(j);
		for(int k = 0; k < pUnionCalc->subArray.GetSize(); k++)
		{
			pRetData = pUnionCalc->subArray.GetAt(k);
			//CFormulaCalc::GetDrawLine( *pRetData,
			//	pRetData->pUnionData->m_strCondition,
			//	pRetData->pUnionData->m_strExp,pRetData->pUnionData->m_nType,0x02 );
			delete pRetData;
		}
		delete pUnionCalc;
	}
	m_AlarmUnionArray.RemoveAll();
}


BOOL CFormulaCalc::AlarmCreateAll()
{
	CFormulaCalc::CalcRetData* pRetData;
	UnionCalc*					pUnionCalc;
	AlarmCalcData*			pCalData;

	AlarmDeleteAll();

	for(int nFor = 0; nFor < m_pAlarmCondition->GetSize(); nFor++ )
	{
		pCalData = m_pAlarmCondition->GetAt(nFor);
		if( pCalData == NULL )
		{
			return FALSE;
		}

		pCalData->m_nUnionCalBegin = m_AlarmUnionArray.GetSize();	//设定当前条件在计算公式表中开始行

		if( pCalData->m_pCondition->m_sSingleSel != NULL )
		{
			UnionCalc* pUnionCalc = new UnionCalc;
			m_AlarmUnionArray.Add(pUnionCalc);
			pUnionCalc->bAndOr = 0;
			pUnionCalc->m_lDateCount = (short)pCalData->m_pCondition->m_lDateCount; // date
			pUnionCalc->m_lEndDate   = pCalData->m_pCondition->m_lEndDate;

			pRetData = new CFormulaCalc::CalcRetData;
			pRetData->pUnionData = pCalData->m_pCondition->m_sSingleSel;
			//pRetData->SetAsk(-1);
			pRetData->SetAsk(pRetData->pUnionData->m_nPeriod);//yulx modify
			pUnionCalc->subArray.Add(pRetData);
		}
		else if( pCalData->m_pCondition->m_pHSUnion != NULL )
		{
			UnionSelCondition* pHSUnion = pCalData->m_pCondition->m_pHSUnion;
			if( pHSUnion )
			{
				CArray<UnionSelConditionSub*,UnionSelConditionSub*>* pArray = pHSUnion->m_aySub;
				if( pArray )
				{
					UnionSelConditionSub* pHSUnionSelSub;
					SingleSelCondition*   pUnionData;
					for(int i = 0; i < pArray->GetSize(); i++)
					{
						pHSUnionSelSub = pArray->GetAt(i);

						pUnionCalc = new UnionCalc;
						m_AlarmUnionArray.Add(pUnionCalc);
						pUnionCalc->bAndOr = pHSUnionSelSub->m_cAndOr;
						if( pHSUnionSelSub->m_nBeginDate == 0 && pHSUnionSelSub->m_nEndDate == 0 ) // default date
						{
							pUnionCalc->m_lDateCount = (short)pCalData->m_pCondition->m_lDateCount;
							pUnionCalc->m_lEndDate   = pCalData->m_pCondition->m_lEndDate;
						}
						else
						{
							pUnionCalc->m_lDateCount = pHSUnionSelSub->m_nBeginDate;
							pUnionCalc->m_lEndDate   = pHSUnionSelSub->m_nEndDate;
						}

						for(int j = 0; j < pHSUnionSelSub->m_aySub->GetSize(); j++)
						{
							pUnionData = pHSUnionSelSub->m_aySub->GetAt(j);

							pRetData = new CFormulaCalc::CalcRetData;
							pUnionCalc->subArray.Add(pRetData);
							pRetData->pUnionData = pUnionData;
							pRetData->SetAsk(-1);
						}
					}
				}
			}
		}
		else
		{
			return FALSE;
		}

		pCalData->m_nUnionCalEnd = m_AlarmUnionArray.GetSize();	//设定当前条件在计算公式表中结束行
	}
	return TRUE;
}

//add by lxqi 20090310	策略交易计算函数
int CFormulaCalc::CalcStrategyTrade(CLineProperty*& pRefData,HSCurDataInfoStatus* pCurDataStatus /* = NULL */,int nSize)
{
	if (pRefData == NULL || nSize <= 0/*pRefData->m_strName != "ENTERLONG" && pRefData->m_strName != "EXITLONG"*/)
	{
		return 0;
	}
	//CExpression* pExp = pRefData->m_pCurVal->GetExpByCompile();
	int	nTempSize = ((CHSDataSource*)(pRefData->m_pDataSource))->GetDaySize();
	int nResult = 0;
	CLineProperty *pLine = pRefData;
	bool bEnd = false;
	//bool bIsLocal = FALSE;
	//if (pCurDataStatus == NULL)
	//{
	//	pCurDataStatus = new HSCurDataInfoStatus;
	//	bIsLocal = TRUE;
	//}
	try
	{
		for (int i = 0; i < nSize; i++)
		{
			if (i == nSize - 1)
			{
				bEnd = true;
			}
			pLine->Alloc(nTempSize,nTempSize);
			if (((CExpValue*)(pLine->m_pCurVal))->CalcValue(pLine,0,nTempSize,pCurDataStatus,bEnd))
			{
				//判断是买还是卖，即不是买也不是卖则直接返回0
				CString strName;
				strName.Format("%s", pLine->m_strName);
				CString str = strName.Left(strName.GetLength() - 3);//yulx add 去掉后三位000
				if (/*pLine->m_strName*/str == "ENTERLONG")		//判断是否买入
				{
					if (pLine->m_pData[nTempSize-1].m_dData > 0.5 && pLine->m_pData[nTempSize-1].m_dData != hx_DefMinValues)			//类型double不能用>0
					{
						if (nResult == 0)	
						{
							nResult = 1;
						}
						else if (nResult == -1)		//判断是否已经有卖出信号
						{
							nResult = 2;
						}
					} 
				} 
				else if (/*pLine->m_strName*/str == "EXITLONG")	//判断是否卖出
				{
					if (pLine->m_pData[nTempSize-1].m_dData > 0.5 && pLine->m_pData[nTempSize-1].m_dData != hx_DefMinValues)
					{
						if (nResult == 0)
						{
							nResult = -1;
						} 
						else if (nResult == 1)		//判断是否已经有买入信号
						{
							nResult = 2;
						}
					}
				}
			} 
			pLine++;
		}
	}
	catch (...)
	{
		//WRITE_LOG(__FILE__,__LINE__,"CFormulaCalc::CalcStrategyTrade()=====>策略交易计算异常！");
	}
	//if (bIsLocal)
	//{
	//	delete pCurDataStatus;
	//}
	return nResult;
}

IDataSourceEx* CFormulaCalc::GetDataSourcePtr()
{
	if ( m_pDataSource == NULL)
		m_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	return m_pDataSource;
}
//end by lxqi 20090310