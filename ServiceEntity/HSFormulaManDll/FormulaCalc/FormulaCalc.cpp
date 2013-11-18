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
// �����ڵõ������������͵�
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
				*pPeriodName = "1����";
			}
		}
		break;
	case AnalisysFor5:
		{
			nPeriodType = PERIOD_TYPE_MINUTE5;
			if( pPeriodName )
			{
				*pPeriodName = "5����";
			}
		}
		break;
	case AnalisysFor15:
		{
			nPeriodType = PERIOD_TYPE_MINUTE15; //20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "15����";
			}
			//nPeriodNumber = 3;
		}
		break;
	case AnalisysFor30:
		{
			nPeriodType = PERIOD_TYPE_MINUTE30;	//20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "30����";
			}
			//nPeriodNumber = 6;
		}
		break;
	case AnalisysFor60:
		{
			nPeriodType = PERIOD_TYPE_MINUTE60;	//20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "60����";
			}
			//nPeriodNumber = 12;
		}
		break;

	case AnalisysFor180:
		{
			nPeriodType = PERIOD_TYPE_MINUTE60;	//20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "180��";
			}
			nPeriodNumber = 3;
		}
		break;

	case AnalisysForDay:
		{
			nPeriodType = PERIOD_TYPE_DAY;
			if( pPeriodName )
			{
				*pPeriodName = "����";
			}
		}
		break;
	case AnalisysForWeek:
		{
			nPeriodType = PERIOD_TYPE_WEEK;	//20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "����";
			}
			//nPeriodNumber = 7;
		}
		break;
	case AnalisysForMonth:
		{
			nPeriodType = PERIOD_TYPE_MONTH;	//20081008 YJT 
			if( pPeriodName )
			{
				*pPeriodName = "����";
			}
			//nPeriodNumber = 30;
		}
		break;
	case AnalisysForMoreDay:
		{
			// gbq add 20060429, ���������ڶ����
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
				pPeriodName->Format("%d������",nPeriodNumber);
			}
			else
			{
				nPeriodType = PERIOD_TYPE_DAY;
		//		nPeriodNumber = pApp->GetProfileInt(CHSTools_Multi_Day_Number,
		//			CHSTools_Multi_Day_Number, 10);
				int nTemp=1;
				//CfgValues::GetDetailCfgInt(m_UserCfgMapOb,CHSTools_Multi_Day_Number,CHSTools_Multi_Day_Number , nTemp,10);
				nPeriodNumber = nTemp;
				pPeriodName->Format("%d����",nPeriodNumber);
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
				pPeriodName->Format("%d����",nPeriodNumber);
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
				pPeriodName->Format("%d����",nPeriodNumber);
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
CMapVariabile*		  CFormulaCalc::m_pExternExpression = NULL;     // �ⲿ���ʽ��
CMapVariabile*		  CFormulaCalc::m_pExternVariabile = NULL;		 // �ⲿ������
CMapVariabile*        CFormulaCalc::m_pExternFunction = NULL;		 // �ⲿ������
CSystemFun*			  CFormulaCalc::m_pSystemFun = NULL;			 // ����ָ��
//
CWnd*				  CFormulaCalc::m_pMsgWnd = NULL;
SelStockCondition*    CFormulaCalc::m_curSelStockCondition = NULL;
TExchSys*			  CFormulaCalc::m_pExchSys = NULL;

//
int					  CFormulaCalc::m_nStopThread = 0;
HANDLE				  CFormulaCalc::m_hThread = NULL;
DWORD				  g_dwThreadID = -1;

//
// ѡ�ɳɹ���Ϣ
CString SuccResult::m_strTip; // ѡ�ɳɹ���Ϣ��ʾ

//
CArray<CExpValue*,CExpValue*>		 CFormulaCalc::m_ayFreeList;
CArray<CFormulaCalc::UnionCalc*,CFormulaCalc::UnionCalc*>		 CFormulaCalc::m_UnionArray;

// ���ݹ���

InportDataParam*  CFormulaCalc::m_pHxDataSource = NULL;

static BOOL		  g_bUserHisDate = FALSE; // ѡ��ʱ��ʹ����ʷʱ��

//Ԥ��ϵͳʹ��
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

	// if nError == 0 then return is double, else is ���ǹ�Ʊ��ayNowData������������0��Ϊ���¹�Ʊ������
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
	else if( nError == HS_Return_ZGB )   //--2010.10.09litao �����ǹɱȵĴ���
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
	CFormulaCalc::DaYu,       //   ����
		CFormulaCalc::XiaoYu,     //	С��
		CFormulaCalc::DengYu,	   //	����
		CFormulaCalc::JieYu,      //	����
		CFormulaCalc::ShangChuan, //	�ϴ�
		CFormulaCalc::XiaoPo,     //	����
		CFormulaCalc::WeiChi,     //	ά��
		CFormulaCalc::BiaoChi,	   //	���� 

		CFormulaCalc::Condition,  // ����ѡ��
		CFormulaCalc::Exchange,   // ����ϵͳ
		CFormulaCalc::MoreKLine,   // ���K��
		CFormulaCalc::Exchange    // ���ԣ����ý���ϵͳ��yulx add 20091117
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
		break;				//�������ڣ�1����
	case 1: 
		nPeriod = PERIOD_TYPE_MINUTE5; 
		break;				//�������ڣ�5����
	case 2: 
		nPeriodNum = 3;
		nPeriod = PERIOD_TYPE_MINUTE15; 
		break;				//�������ڣ�15����
	case 3: 
		nPeriodNum = 6;
		nPeriod = MINUTE30; 
		break;				//�������ڣ�30����
	case 4: 
		nPeriodNum = 12;
		nPeriod = MINUTE60; 
		break;				//�������ڣ�60����
	//case 5: nPeriod = PERIOD_TYPE_MINUTE120;			//�������ڣ�120����
	case 6:
		nPeriodNum = 7;
		nPeriod = WEEK; 
		break;					//�������ڣ���
	case 7: 
		nPeriodNum = 30;
		nPeriod = MONTH;
		break;				//�������ڣ���
	case 8: 
		nPeriod = PERIOD_TYPE_DAY_ANY; 
		break;				//������������
	//case 0: nPeriod = MINUTE_ANY			//������������
	default: 
		nPeriod = PERIOD_TYPE_DAY; 
		break;				//�������ڣ���
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
	// �õ����㹫ʽ
	if( !retData.m_bInitLeftRight )
	{
		retData.m_bInitLeftRight = 1;
		retData.m_pLeft   = CFormulaCalc::GetDrawLine(retData,strLine1,strName,nMask,
														TRUE,pHSTechSortResult);
		retData.m_pRight  = CFormulaCalc::GetDrawLine(retData,strLine2,strName,nMask);
		retData.m_pMiddle = CFormulaCalc::GetDrawLine(retData,strLine3,strName,nMask);
	}
	if( retData.m_pLeft == NULL ) return -1;

	// ����
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
						int nBegin,int nEnd)		//  ����
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
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
		else // �����������ֵ�Ƚ�
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() )
			{
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
						  int nBegin,int nEnd)		//	С��
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
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
		else // �����������ֵ�Ƚ�
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() )
			{
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
						  int nBegin,int nEnd)		//	����
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
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
		else // �����������ֵ�Ƚ�
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() )
			{
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
						 int nBegin,int nEnd)		//	����
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
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
		else // �������ֵ�Ƚ�
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() &&
				retData.pCalDataMidlle && !retData.pCalDataMidlle->IsEmpty() )
			{
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
							  int nBegin,int nEnd)	//	�ϴ�
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
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
		else // �����߱Ƚ�
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() )
			{
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
						  int nBegin,int nEnd)		//	����
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
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
		else // �����߱Ƚ�
		{
			if( retData.pCalDataLeft && !retData.pCalDataLeft->IsEmpty() && 
				retData.pCalDataRight && !retData.pCalDataRight->IsEmpty() )
			{
				if( g_bUserHisDate ) // ��ʷʱ��������ѡ�еġ�
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
						  int nBegin,int nEnd)		//	ά��
{
	if( pUnionData->m_nType == JiBenMian )
		return 0;

	return 0;
}

int CFormulaCalc::BiaoChi(SingleSelCondition*  pUnionData,
						   CFormulaCalc::CalcRetData& retData,
						   int nBegin,int nEnd)	//	���� 
{
	if( pUnionData->m_nType == JiBenMian )
		return 0;

	return 0;
}

int CFormulaCalc::Condition(SingleSelCondition*  pUnionData,
							 CFormulaCalc::CalcRetData& retData,
							 int nBegin,int nEnd)	//	����ѡ�� 
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
			if( g_bUserHisDate )  // ��ʷʱ��������ѡ�еġ�
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
							int nBegin,int nEnd)	//	����ϵͳ 
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
		if( g_bUserHisDate )  // ��ʷʱ��������ѡ�еġ�
		{
			for( ; nBegin < nEnd; nBegin++ )
			{
				// ENTERLONG -- ����
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

				// EXITLONG -- ����
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

			// ENTERLONG -- ����
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

			// EXITLONG -- ����
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
							 int nBegin,int nEnd)	//	���K�� 
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
			if( g_bUserHisDate )  // ��ʷʱ��������ѡ�еġ�
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
				
				// ��ͼ����
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

		// ָ������
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

// �����Ƶõ���ͼ���ʽ
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

// �����Ƶõ����ʽ(��ʽ)
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
	//WRITE_LOG(__FILE__, __LINE__, "��ʼ����");
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
			// �Զ����� �ռ����
			ayOther = new CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>;
			if( ayOther == NULL )
			{
			/*	//WRITE_LOG(__FILE__, __LINE__, "�����ڴ����");*/
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
			//WRITE_LOG(__FILE__, __LINE__, "�Ƿ���ֹ");
#endif
			goto Calculate__;
		}
		HSGridBlockData* pGridBlock = NULL;//(HSGridBlockData*)m_curSelStockCondition->m_lDateCount;
		CMapStringToPtr* pMapNowData = (CMapStringToPtr*)m_curSelStockCondition->m_lEndDate;
		HSGridBlockData::m_payAllBlock = (CArray<BlockInfo*,BlockInfo*>*)m_curSelStockCondition->m_pHSUnion;
		if( pMapNowData == NULL || HSGridBlockData::m_payAllBlock == NULL )
			goto Calculate__;
		//  

		// ���
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
				//WRITE_LOG(__FILE__, __LINE__, "������ֹ");
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
				//WRITE_LOG(__FILE__, __LINE__, "���Ϸ�����");
				continue;
			}

			for( int k = ayRetData.GetSize() - 1; k >= 0; k-- )
			{
				ayRetData.GetAt(k)->EmptyData();
			}
#ifdef _DEBUG
			//if( pItem->strText.CompareNoCase("H�ɰ��") == 0 )
			//{
			//	CString str;
			//}
#endif

			// ����Ʊ
			int nStockCount = pItem->ayStockList.GetSize();
			CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> ayNowData; // ��������
			for( int iStock = 0; iStock < nStockCount; iStock++ )
			{
				if( CFormulaCalc::m_nStopThread != -2 )
				{
				/*	//WRITE_LOG(__FILE__, __LINE__, "������ֹ");*/
					break;
				}

				CodeInfo* pStockItem = pItem->ayStockList.GetAt(iStock);
				if( pStockItem == NULL )
				{
#ifndef _DEBUG
					//WRITE_LOG(__FILE__, __LINE__, "���Ϸ�����");
#endif
					continue;
				}

				void * pTempItem;
				CommRealTimeDataEmpty* pNowDataItem;
				// �õ���������
				if( pMapNowData->Lookup(pStockItem->m_cCode, pTempItem) )
				{
					pNowDataItem = (CommRealTimeDataEmpty*)pTempItem;
					ayNowData.Add(pNowDataItem);
				}
			}

			// �������Դ����
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
			// ���������
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

			// ��ǰ��� 
			info.m_lDataTransmited = iType;
			SelectStockResult.pStock =NULL;// (void*)//CStockManager::GetBlockUserInfo((char*)(LPCSTR)pItem->strText, SI_NAME);
			SelectStockResult.dData.m_pOtherData = (HSDrawOtherData*)&ayRetData;
			info.m_pszData = (const char*)&SelectStockResult;

			if( m_pMsgWnd != NULL && ::IsWindow(m_pMsgWnd->m_hWnd) )
			{
				m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,4,(LPARAM)&info); // ѡ����Ʊ���			
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

	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("BlockReport MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}
	catch(CNotSupportedException e) // �ļ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("BlockReport NotSupported: %s", szCause);
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}
#endif

	catch(...) // �����쳣
	{
		// ������־
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
		//WRITE_LOG(__FILE__, __LINE__, "�������ڴ��쳣");
	}

	if( m_pMsgWnd != NULL && ::IsWindow(m_pMsgWnd->m_hWnd) ) // end
	{
		m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,5,
			(LPARAM)m_curSelStockCondition->m_dStyle); // ѡ����Ʊ���
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

		CArray<StockUserInfo*,StockUserInfo*>* pAyStock = &StockArea::ayStock; // ��Ʊ����
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

			} // ��Ʊ��ѭ��

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
		* ����
		* ʱ�䣺  2003��9��24��
		* ������  Ϊ����Ӧ��Э��
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

		// �Ƿ�������
		if( pDiskData == NULL || pDiskData->IsEmpty() )
		{
			return TCaclReturn_NoData;
		}
		// С��
		int nDecimal   = 0;
		// ��λ
		//long nPriceUnit = //CStockManager::GetStockPriceUnit(pCaclTemp->pCode->m_cCodeType);
		long nPriceUnit = 0;//CStockManager::GetStockPriceUnit(pCaclTemp->pCode);

		// �趨��������
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

		// �Ƿ�������
		if( nFindDateRet <= 0 )
		{
			return TCaclReturn_NoData;
		}

		// �������ݶ�λ
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
		// ���񣬳�Ȩ...���õ��Ĵ�����Ϣ
		m_pHxDataSource->m_pDataSrc->SetAsk( (AskData*)&ask,0 );
		// �趨��������
		m_pHxDataSource->m_pDataSrc->SetFinaceData(pCaclTemp->pCode);

		// �趨����Դ����
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

		// ������ָ������
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

	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("HSExchCalc MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));

		DeleteAll();
		return TCaclReturn_MoneryFail;
	}
	catch(CFileException e) // �ļ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("HSExchCalc FileException: %s", szCause);
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}
#endif
	catch(...) // �����쳣
	{
		// ������־
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

		CArray<StockUserInfo*,StockUserInfo*>* pAyStock = &StockArea::ayStock; // ��Ʊ����
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


					if( pRetData->pUnionData->m_nType ==JiBenMian ) // ��������
					{
						// �趨��������
						m_pHxDataSource->m_pDataSrc->SetFinaceData(&pStock->m_ciStockCode);

						// calc
						strCondition.Format("%.3f",
							m_pHxDataSource->m_pDataSrc->GetFinaceDataByID( atol(pRetData->pUnionData->m_strCondition) ));

						pRetData->nResult = CFormulaCalc::SingleCal( pRetData->pUnionData,
							*pRetData,(int)&strCondition,0 );
					}
					else // ָ��...
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
							// �趨��������
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
								if( (m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Sort ) // ����
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
								else // ѡ��
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
					else // ѡ��
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
				pUnionCalc = m_UnionArray.GetAt(0);  // ��������л�������
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

			// ��ǰ��� 
			info.m_lDataTransmited = i;
			SelectStockResult.pStock = (void*)pStock;
			if( (m_curSelStockCondition->m_dStyle & 0x0F00) & SelectStock_Sort ) // sort
			{
				info.m_pszData = (const char*)&SelectStockResult;
			}
			else // ѡ��
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
			m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,4,(LPARAM)&info); // ѡ����Ʊ���

			nRet = 0;
		} // ��Ʊ��ѭ��
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
			(LPARAM)m_curSelStockCondition->m_dStyle); // ѡ����Ʊ���
	}

	DeleteAll();

	CFormulaCalc::m_nStopThread = 0;

	return 0;
}

/********************************************************************************
* �������� : ���ҳ�������
* �������� : SuccResult* pSucc,CArray<SuccResult*,SuccResult*>& aySucc
* �� �� ֵ : SuccResult*
* �������� : void CheckUnionCondition(UnionCalc* pUnionCalc) �Ⱥ�����ʹ��
* ��    �� : 
* ������� : 2003-08-01
* �޸ļ�¼ : 
*******************************************************************************/
SuccResult* FindBySuccPoint(SuccResult* pSucc,CArray<SuccResult*,SuccResult*>& aySucc)
{
	if( pSucc == NULL )
		return NULL;

	SuccResult* pCurSucc;
	for( int i = aySucc.GetSize() - 1; i >= 0; i-- )
	{
		pCurSucc = aySucc.GetAt(i);
		if( (pSucc->m_lDate == pCurSucc->m_lDate) ||			// ���ڶԱ�
			(pCurSucc->m_lDate == -1 && pCurSucc->m_nPos == -1) // ��������
		  )
		{
			return pSucc;
		}
	}

	return NULL;
}

/********************************************************************************
* �������� : ��������Ƿ����
* �������� : UnionCalc* pUnionCalc
* �� �� ֵ : void
* �������� : DWORD WINAPI CFormulaCalc::Calculate(LPVOID pParameter) �Ⱥ�����ʹ��
* ��    �� : 
* ������� : 2003-08-01
* �޸ļ�¼ : 
*******************************************************************************/
void CheckUnionCondition(CFormulaCalc::UnionCalc* pUnionCalc)
{
	if( pUnionCalc == NULL )
		return;

	SuccResult* pSucc;
	CFormulaCalc::CalcRetData* pRetData;
	
	int i;
	int j;
	
	if( pUnionCalc->bAndOr ) // ͬ����
	{
		SuccResult* pFindSucc;
		CFormulaCalc::CalcRetData* pBaseRetData = NULL;

		int nType = 2;

		// ���Ҳ��ǲ������ݵĽ��
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

		if( nSize != -1 && pBaseRetData ) // ��� pBaseRetData == NULL����ȫ�ǲ���Ƚϣ�������k���Ͻ��޷���ʾ
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
					if( pFindSucc == NULL ) // ��һ������������ֹͣ
						break;
				}
				if( pFindSucc )
				{
					if( (nType == 1)												  || // ȫ�ǲ�������
						(CFormulaCalc::m_curSelStockCondition->m_lDateCount == 0)	  || // ��ǰ����
						(pFindSucc->m_lDate  >= CFormulaCalc::m_curSelStockCondition->m_lDateCount && 
						 pFindSucc->m_lDate  <= CFormulaCalc::m_curSelStockCondition->m_lEndDate) )
					{
						pUnionCalc->AddSucc(new SuccResult(pFindSucc->m_lDate,pFindSucc->m_nPos));
					}
				}
			}	
		}
	
		// ɾ��
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

				if( (pRetData->pUnionData->m_nType == JiBenMian) || // ��������
					(CFormulaCalc::m_curSelStockCondition->m_lDateCount == 0) || // ��ǰ����
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
* �������� : �õ�������ϵ��ı�����
* �������� : CArray<CFormulaCalc::UnionCalc*,CFormulaCalc::UnionCalc*>& ayUnionCalc
* �� �� ֵ : CString
* �������� : DWORD WINAPI CFormulaCalc::Calculate(LPVOID pParameter) �Ⱥ�����ʹ��
* ��    �� : 
* ������� : 2003-08-05
* �޸ļ�¼ : 
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
			switch( pUnionData->m_nType )      // ��ʽ����
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
			case Strategy: //20090302 YJT ���� Strategy
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

		// ����Ԥ����
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

		// ��ʾ
		SuccResult::m_strTip = GetDescribe(m_UnionArray);

		//  
		int nBegin = 0;
		int nEnd;

		// Ŀ���ʼ��
		InitObj();

		 // ��Ʊ����
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

		CArray<StockUserInfo*,StockUserInfo*> aySelectRetStock; // ѡ�ɽ��

		int nMoreSucc,nUnionCount,nSubCount;

		g_nStartCol = HS_USR_DEF_BEGIN + 40;
		SelStockResult SelectStockResult;

		/*
		
		����ѡ��ִ��ͼ��:

		��Ʊ loop----------------------------��Ʊ��Χ
		{
		   ������ loop---------------------�����������,û�����Զ�����һ��
		   {
		       ����� loop-------------------�����������
			   {
			        ��������-----------------��ĳ��ָ���������
					{
					    �ȽϺ���-------------ָ�뺯��
					    ...
					}
			   }
		   }
		}

		*/

		CString strCondition;
		for( i = 0; i < nCount; i++ ) // ��Ʊ
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
			for( j = 0; j < nUnionCount; j++) // ��������е�n�������
			{
				if( CFormulaCalc::m_nStopThread != -2 )
					break;

				pUnionCalc = m_UnionArray.GetAt(j);
				nSubCount  = pUnionCalc->subArray.GetSize();
				for(k = 0; k < nSubCount; k++) // ÿ��������
				{
					if( CFormulaCalc::m_nStopThread != -2 ) //
						break;

					pRetData = pUnionCalc->subArray.GetAt(k);

					if( pRetData->pUnionData->m_nType == JiBenMian ) // ��������
					{
						AskData ask;
						memcpy(&ask,&pRetData->ask,sizeof(ask));
						memcpy(ask.m_pCode,&pStock->m_ciStockCode,sizeof(ask.m_pCode));
						m_pHxDataSource->m_pDataSrc->SetAsk( &ask,0 );						

						double d = hx_DefMaxValues;

						// calc
						UINT nID = atol(pRetData->pUnionData->m_strCondition);

						// ����
						if( (nID >= COLUMN_HQ_BASE_BEGIN && nID <= COLUMN_HQ_BASE_END) ||
							(nID >= COLUMN_HQ_EX_BEGIN && nID <= COLUMN_HQ_EX_END) )
						{
							d = (double)m_pHxDataSource->m_pDataSrc->GetNowDataByCode( nID );
						}
						else
						{
							// �趨��������
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
					else // ָ��...
					{
						memcpy(&pRetData->ask.m_pCode->m_ciCode,&pStock->m_ciStockCode,sizeof(CodeInfo));
	
						//��ȡk������ֱ�Ӵ����������ȡ
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
							
							// ���ݲ���ʱ������,���ڽ���н��
							StockDay* pDay = (StockDay*)pDiskData->m_pData;
							nEnd = pDiskData->m_lSize;

							// ���񣬳�Ȩ...���õ��Ĵ�����Ϣ
							AskData ask;
							memcpy(&ask,&pRetData->ask,sizeof(ask));
							memcpy(ask.m_pCode,&pRetData->ask.m_pCode->m_ciCode,sizeof(ask.m_pCode));
							m_pHxDataSource->m_pDataSrc->SetAsk( &ask,0 );

							// ��ȡ��������
							m_pHxDataSource->m_pDataSrc->SetFinaceData( &pRetData->ask.m_pCode->m_ciCode );

							
							// �趨����Դ����
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


				// ѡ��
				{
					if( nSubCount == 1 )
					{
						SuccResult* pSuccResult;
						pRetData = pUnionCalc->subArray.GetAt(0);
						for( nMoreSucc = 0; nMoreSucc < pRetData->m_aySucc.GetSize(); nMoreSucc++)
						{
							pSuccResult = pRetData->m_aySucc.GetAt(nMoreSucc);
							
							if( (pRetData->pUnionData->m_nType == JiBenMian) || // ��������
								(m_curSelStockCondition->m_lDateCount == 0)				  || // ��ǰ����
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

				// �Ƿ���k������ʾ
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
			else if( nUnionCount > 1 ) // ��������ĸ߼�,���ڽ���
			{
				pUnionCalc = m_UnionArray.GetAt(0);  // ��������л�������
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

			// ��ǰ��� 
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
				m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,SelectingStock,(LPARAM)&info); // ѡ����Ʊ���
			}

			nRet = 0;

		} // ��Ʊ��ѭ��

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
			(LPARAM)m_curSelStockCondition->m_dStyle); // ѡ����Ʊ���
	}

	CFormulaCalc::m_nStopThread = 0;
	DelAllSuccData(TRUE);
	return 0;
}


DWORD WINAPI CFormulaCalc::TechSortCalculate(LPVOID pParameter)
{
	// ��ʼ����
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

		// ָ���ʼ������
		if( m_curSelStockCondition->m_pData != NULL )
		{
			pUnionCalc = new UnionCalc;
			if( pUnionCalc == NULL )
			{
				//WRITE_LOG(__FILE__, __LINE__, "�����ڴ����");
				goto Calculate__;
			}

			m_UnionArray.Add(pUnionCalc);
			pUnionCalc->bAndOr = 0;
			// ��ֹʱ��
			pUnionCalc->m_lDateCount = m_curSelStockCondition->m_lDateCount;
			pUnionCalc->m_lEndDate   = m_curSelStockCondition->m_lEndDate;

			// ����Ϣ
			FormulaAddColParam* pHSAddColParam = (FormulaAddColParam*)m_curSelStockCondition->m_pData;
			if( pHSAddColParam->payOther == NULL || pHSAddColParam->payOther->GetSize() < 1 )
			{
				//WRITE_LOG(__FILE__, __LINE__, "�����������");
				goto Calculate__;
			}
			pBaseDataEx = new FormulaIndicatorSortResult;
			if( pBaseDataEx == NULL )
			{
				//WRITE_LOG(__FILE__, __LINE__, "�����ڴ����");
				goto Calculate__;
			}
			// ��ʼ���Զ�����
			AddBaseOtherColData(pBaseDataEx,*pHSAddColParam->payOther);
			// �Զ����� �ռ����
			ayOther = new CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>;
			if( ayOther == NULL )
			{
				//WRITE_LOG(__FILE__, __LINE__, "�����ڴ����");
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
			
			// ��������
			ayRetData.Add(pBaseDataEx);

			// �Զ���ָ����
			CArray<SingleSelCondition*,SingleSelCondition*>* pExArray = pHSAddColParam->pExArray;
			SingleSelCondition* pHSUnionData = NULL;
			/*************************************************************
			* ����
			* ʱ�䣺  2003��9��10��
			* ������  ������pExArray->GetSize() > 0 �жϣ���Ϊ���Զ������
			*		  �У����п���Ϊ0
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
						//WRITE_LOG(__FILE__, __LINE__, "�����ڴ����");
						goto Calculate__;
					}
					pUnionCalc->subArray.Add(pRetData);
					pRetData->pUnionData = new SingleSelCondition;
					if( pRetData->pUnionData == NULL )
					{
						//WRITE_LOG(__FILE__, __LINE__, "�����ڴ����");
						goto Calculate__;
					}
					pRetData->pUnionData->Copy(pHSUnionData);
					ayTempFree.Add(pRetData->pUnionData);
					pRetData->m_pHSTechSortResult = new FormulaIndicatorSortResult;
					/*************************************************************
					* ���� Ϊ����Ӧ���� 2003��9��10��
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
			/**********���Ľ���*************/
		}

		InitObj();

		int nBegin = 0;
		int nEnd;
		CArray<StockUserInfo*,StockUserInfo*> sAyStockTemp;
		sAyStockTemp.Copy(StockArea::ayStock);
		CArray<StockUserInfo*,StockUserInfo*>* pAyStock = &sAyStockTemp; // ��Ʊ����
		int nCount = pAyStock->GetSize(); // ��Ʊ��
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

		// ���� Ϊ��ֹ��������ʱ��������������BUG
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

		// ����(��Ʊ��
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
			for( j = 0; j < nUnionCount; j++) // ������
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
 
 						// �趨��������
 						nBeginFind = 0;
 						if( m_curSelStockCondition->m_lDateCount > 0 ) // FIND BEGIN DATE 
 						{
 							nBeginFind = FindDateExact(m_curSelStockCondition->m_lDateCount, 0,
 							(StockDay*)pDiskData->m_pData,pDiskData->m_lSize);
 						}
 
 						// �Ƿ�Ϊ�Ǿ�ȷ��λ
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
 
 							// ���񣬳�Ȩ...���õ��Ĵ�����Ϣ
 							AskData ask;
 							memcpy(&ask,&pRetData->ask,sizeof(ask));
 							memcpy(ask.m_pCode,&pRetData->ask.m_pCode->m_ciCode,sizeof(ask.m_pCode));
 							m_pHxDataSource->m_pDataSrc->SetAsk( &ask,0 );
 							// ��ȡ��������
 							m_pHxDataSource->m_pDataSrc->SetFinaceData( &pRetData->ask.m_pCode->m_ciCode );
 							// �趨����Դ����
 							m_pHxDataSource->m_pDataSrc->SetDay( pDay,pDiskData->m_lSize,
 								pRetData->nChengJiaoDiv,0,0,nPriceUnit,nDecimal);
 							m_pHxDataSource->m_pDataSrc->RemoveStyle( pRetData->nAnaPeriod );
 							m_pHxDataSource->m_pDataSrc->AddStyle( pRetData->nAnaPeriod );
 
 							// ֱ�ӷ��ص�����
 							for( t = 0; ayOther && t < ayOther->GetSize(); t++ )
 							{
 								pHSBaseOtherColData = ayOther->GetAt(t);
 								// ���ò���
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
 								// ��ʽ���� begin
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
 								} // ��ʽ���� end
 							}
 
 						}
 					}
 				}
			}

			// ��ǰ��� 
			info.m_lDataTransmited = i;
			SelectStockResult.pStock = pStock;
			SelectStockResult.dData.m_pOtherData = (HSDrawOtherData*)&ayRetData;
			info.m_pszData = (const char*)&SelectStockResult;

			if( m_pMsgWnd && ::IsWindow(m_pMsgWnd->m_hWnd) )
			{
				m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,4,(LPARAM)&info); // ѡ����Ʊ���			
			}
			else
			{
				//WRITE_LOG(__FILE__, __LINE__, "����������⣬�����Ѳ�����!");
				goto Calculate__;
			}

			nRet = 0;

		} // ��Ʊ��ѭ��

		SelectStockResult.dData.m_pOtherData = NULL;

		// ɾ���ռ�
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

		// ɾ������
		DeleteAll();
		for( int i = 0; i < ayTempFree.GetSize(); i++ )
		{
			delete ayTempFree.GetAt(i);
		}
		ayTempFree.RemoveAll();

		if( m_pMsgWnd != NULL && ::IsWindow(m_pMsgWnd->m_hWnd) ) // end
		{
			m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,5,
				(LPARAM)m_curSelStockCondition->m_dStyle); // ѡ����Ʊ���
		}

		CFormulaCalc::m_nStopThread = 0;
		return 0;
	}

#ifdef VC_7_0

	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("TechSort MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}
	catch(CNotSupportedException e) // �ļ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("TechSort NotSupported: %s", szCause);
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}
#endif
	
	catch(...) // �����쳣
	{
		// ������־
		CString strError;
		strError.Format("TechSort other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG(__FILE__, __LINE__, _T(strError));
	}

Calculate__:

	try
	{
		// ɾ���ռ�
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
				(LPARAM)m_curSelStockCondition->m_dStyle); // ѡ����Ʊ���
		}
	}
	catch(...)
	{
		//WRITE_LOG(__FILE__, __LINE__, "��������ڴ��쳣");
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
		m_pMsgWnd->SendMessage(HX_USER_COMPILEDATA,5,0); // ѡ����Ʊ���
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
	//	//CArray<StockArea*,StockArea*>* payCheck = m_curSelStockCondition->m_ayCheck; // ��Ʊ��Χ
	//	//CArray<StockArea*,StockArea*>* payCheck = m_pExchSys->m_ayCheck; // ��Ʊ��Χ

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
		//WRITE_LOG(__FILE__, __LINE__, "���������߳�");
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
// 		CArray<StockArea*,StockArea*>* payCheck = m_curSelStockCondition->m_ayCheck; // ��Ʊ��Χ
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

	// �ҵ���Ӧ����
	if( pDay[lMid].m_lDate == lDate )
	{
		return lMid;
	}

	// �Ľ�
	// ���ҵ����ڲ����ڴ���
	if( bIsBegin )
	{
		// �����ҵ���������С�� ����12 13 15 16����14���λ��15
		// ������ҵ��Ľ��Ϊ15
		if( pDay[lMid].m_lDate > lDate
			&& lMid > 0 && lMid < lSize 
			&& pDay[lMid-1].m_lDate < lDate )
		{
			return lMid;
		}
		else if( lMid >= 0 && lMid+1 < lSize     // �������λ��13�����
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
		// �����ҵ���������С�� ����12 13 15 16����14���λ��15
		// ������ҵ��Ľ��Ϊ13
		if( pDay[lMid].m_lDate > lDate
			&& lMid > 0 && lMid < lSize 
			&& pDay[lMid-1].m_lDate < lDate )
		{
			return lMid-1;
		}
		else if( lMid >= 0 && lMid+1 < lSize     // �������λ��13�����
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
		return 1;	//�߳��Կ���
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
		//ɾ��ԭ����������
		AlarmDeleteAll();

		CFormulaCalc::CalcRetData* pRetData;
		UnionCalc*					pUnionCalc;
		BOOL bAndSucc  = FALSE;

		if(AlarmCreateAll() == FALSE )
		{
			//������Ϣ�������ڣ�˵��Ԥ��û������
			goto AlarmCalculate__;
		}
		// ��ʼ������Դ
		InitAlarmObj();

		int nBegin = 0;
		int nEnd;
		CArray<StockUserInfo*,StockUserInfo*>* pAyStock;// = &StockArea::ayStock; // ��Ʊ����
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

		// �߳���ѭ����ʼ
		while( 1 )
		{
			//Ԥ��������ѭ����ʼ
			time_t lBeginTime, lEndTime;
			time(&lBeginTime);
			for( nAlarmFor = 0; nAlarmFor < m_pAlarmCondition->GetSize(); nAlarmFor++ )
			{
				if( CFormulaCalc::m_nStopAlarmThread != -2 )
					goto AlarmCalculate__;
				// �е���ǰ������Ʊ�����㹫ʽ�Ŀ�ʼ�����к�
				pHSCalData = m_pAlarmCondition->GetAt(nAlarmFor);

				if( pHSCalData == NULL || pHSCalData->m_bFlag == FALSE )
					continue;
				pAyStock = &pHSCalData->m_ayStkList;
				nCount = pAyStock->GetSize();
				nBeginCount = pHSCalData->m_nUnionCalBegin;
				nEndCount = pHSCalData->m_nUnionCalEnd;

				// ��ʼ��Ʊ��ѭ��
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

					// ��ʼ��������ѭ��--��������õ��Ŀ�ʼ��������к�
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

							if( pRetData->pUnionData->m_nType == JiBenMian ) // ��������
							{
								// �趨��������
								m_pAlarmHxDataSource->m_pDataSrc->SetFinaceData(&pStock->m_ciStockCode);

								// calc
								strCondition.Format("%.3f",
									m_pAlarmHxDataSource->m_pDataSrc->GetFinaceDataByID( atol(pRetData->pUnionData->m_strCondition) ));

								pRetData->nResult = CFormulaCalc::SingleCal( pRetData->pUnionData,
									*pRetData,(int)&strCondition,0 );
							}
							else // ָ��...
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
 
 									// �趨��������
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
 										// ���񣬳�Ȩ...���õ��Ĵ�����Ϣ
 										AskData ask;
 										memcpy(&ask,&pRetData->ask,sizeof(ask));
 										memcpy(ask.m_pCode,&pRetData->ask.m_pCode->m_ciCode,sizeof(ask.m_pCode));
 										m_pAlarmHxDataSource->m_pDataSrc->SetAsk( &ask,0 );
 										// ��ȡ��������
 										m_pAlarmHxDataSource->m_pDataSrc->SetFinaceData( &pRetData->ask.m_pCode->m_ciCode );
 
 										// �趨����Դ����
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
					}// ������������ѭ��

					if( nUnionCount == 1 )
					{
						nRet = pUnionCalc->m_aySucc.GetSize();
					}
					else if( nUnionCount > 1 )
					{
						pUnionCalc = m_AlarmUnionArray.GetAt(nBeginCount);  // ��������л�������
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

					// ͳ�Ƶ�ǰ�������: nRet > 0 ��ʾ�й�Ʊѡ���� ������
					pUnionCalc->RemoveAllSucc();

					// �������Ƿ��з��������Ĺ�Ʊ������У��򷢾�����Ϣ�� û��������
					if( m_pAlarmWnd != NULL && nRet ) // middle
					{
						//���;�����Ϣ
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
						BOOL bBreak = (BOOL)::SendMessage(m_pAlarmWnd->m_hWnd, HX_USER_COMPILEDATA,27,(LPARAM)&AlarmData); // ѡ����Ʊ���
						if( bBreak == FALSE )
						{
							pHSCalData->m_bFlag = FALSE;
						}
						AlarmData.m_pData = NULL;
					}
					nRet = 0;
					Sleep((nCount > 500) ? 500 : ( nCount < 50 ) ? 50 : nCount);
				} // ��Ʊ��ѭ������
			}// Ԥ��������ѭ������

			pHSCalData = m_pAlarmCondition->GetAt(0);
			if( pHSCalData != NULL )
				pHSCalData->m_nCalculateCount++;

			time(&lEndTime);
			if( lEndTime - lBeginTime < pHSCalData->m_nCycle )
				Sleep( (pHSCalData->m_nCycle - lEndTime + lBeginTime) * 1000);
		}// ��ѭ������
	}

	// ��������
	catch(...)
	{
	}


	// ʧ�ܴ���
AlarmCalculate__:

	if( m_pAlarmWnd != NULL ) // end
	{
		m_pAlarmWnd->SendMessage(HX_USER_COMPILEDATA,27,
			(LPARAM)0); // ѡ����Ʊ���
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
	// ע�� by xudan 
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

		pCalData->m_nUnionCalBegin = m_AlarmUnionArray.GetSize();	//�趨��ǰ�����ڼ��㹫ʽ���п�ʼ��

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

		pCalData->m_nUnionCalEnd = m_AlarmUnionArray.GetSize();	//�趨��ǰ�����ڼ��㹫ʽ���н�����
	}
	return TRUE;
}

//add by lxqi 20090310	���Խ��׼��㺯��
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
				//�ж�������������������Ҳ��������ֱ�ӷ���0
				CString strName;
				strName.Format("%s", pLine->m_strName);
				CString str = strName.Left(strName.GetLength() - 3);//yulx add ȥ������λ000
				if (/*pLine->m_strName*/str == "ENTERLONG")		//�ж��Ƿ�����
				{
					if (pLine->m_pData[nTempSize-1].m_dData > 0.5 && pLine->m_pData[nTempSize-1].m_dData != hx_DefMinValues)			//����double������>0
					{
						if (nResult == 0)	
						{
							nResult = 1;
						}
						else if (nResult == -1)		//�ж��Ƿ��Ѿ��������ź�
						{
							nResult = 2;
						}
					} 
				} 
				else if (/*pLine->m_strName*/str == "EXITLONG")	//�ж��Ƿ�����
				{
					if (pLine->m_pData[nTempSize-1].m_dData > 0.5 && pLine->m_pData[nTempSize-1].m_dData != hx_DefMinValues)
					{
						if (nResult == 0)
						{
							nResult = -1;
						} 
						else if (nResult == 1)		//�ж��Ƿ��Ѿ��������ź�
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
		//WRITE_LOG(__FILE__,__LINE__,"CFormulaCalc::CalcStrategyTrade()=====>���Խ��׼����쳣��");
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