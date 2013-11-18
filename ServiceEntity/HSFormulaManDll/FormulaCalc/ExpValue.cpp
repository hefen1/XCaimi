// ExpValue.cpp: implementation of the CExpValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pageid.h"
#include "ExpValue.h"
#include "express.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CExpValue, CValue)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CCriticalSection CExpValue::m_AccessDataMutex;

static char* m_strCondition[] =
{
	"����",
	"С��",
	"����",
	"����",
	"�ϴ�",
	"����",
//	"ά��",
//	"����"
};

/*
 #define  AnalisysFor0             0x0000  //�ֱʳɽ�
 #define  AnalisysFor1             0x0001  //1����
 #define  AnalisysFor5             0x0002  //�����
 #define  AnalisysFor15            0x0003  //ʮ�����
 #define  AnalisysFor30            0x0004  //��ʮ����
 #define  AnalisysFor60            0x0005  //��ʮ����
 #define  AnalisysForDay           0x0006  //����
 #define  AnalisysForWeek          0x0007  //����
 #define  AnalisysForMonth         0x0008  //����
 #define  AnalisysForMoreDay       0x0009  //������
*/

char* m_strPeriond[] =
{

//	"�ֱʳɽ�",  // 0
	"1������",   // 0
	"5������",   // 1
	"15������",  // 2
	"30������",  // 3
	"60������",  // 4
	//"180����", // 5
	"����",		 // 6
	"����",		 // 7
	"����",      // 8
	"������"     // 9
};

CString GetPeriodStr(WORD dPeriod,int* nPos /*= NULL*/)
{
//	if( dPeriod >= 0 && dPeriod < _countof(m_strPeriond) )
//		return m_strPeriond[dPeriod];

//	return "";

	if( nPos )
	{
		*nPos = -1;
	}

	switch(dPeriod)
	{
	case  AnalisysFor0 :  //�ֱʳɽ�		
		return "";
	case  AnalisysFor1 :  //1����
		dPeriod = 0;
		break;
	case  AnalisysFor5 :  //�����
		dPeriod = 1;
		break;
	case  AnalisysFor15 :  //ʮ�����
		dPeriod = 2;
		break;
	case  AnalisysFor30 :  //��ʮ����
		dPeriod = 3;
		break;
	case  AnalisysFor60 :  //��ʮ����
		dPeriod = 4;
		break;
	//case  AnalisysFor180 :  //180����
	//	dPeriod = 5;
	//	break;
	case  AnalisysForDay :  //����
		dPeriod = 5;
		break;
	case  AnalisysForWeek :  //����
		dPeriod = 6;
		break;
	case  AnalisysForMonth :  //����
		dPeriod = 7;
		break;
	case  AnalisysForMoreDay :  //������	}
		dPeriod = 8;
		break;
	default:
		return "";
	}

	if( nPos )
	{
		*nPos = dPeriod;
	}
	return m_strPeriond[dPeriod];
}

void AddPeriodNameToWnd(CComboBox* pBox)
{
	if( pBox == NULL || pBox->m_hWnd == NULL )
	{
		return;
	}

	for( int i = 0; i < _countof(m_strPeriond); i++ )
	{
		pBox->AddString(m_strPeriond[i]);
	}
}

int GetPeriodNumByName(CString strPeriodName)
{
	for( int i = 0; i < _countof(m_strPeriond); i++ )
	{
		if( !strPeriodName.CompareNoCase(m_strPeriond[i]) )
		{
			return (i + 1);
		}
	}

	return AnalisysForDay;
}

CString GetCondition(FormulaCompareSubData* pSubData)
{	
	CString strRet;
	CString str,str1;
	switch(pSubData->m_cType)
	{
	case FormulaCompareSubData::DaYu:			//  ����
		{
			str = hxSubZero(pSubData->m_dData1);

			strRet.Format("%s%s",m_strCondition[pSubData->m_cType],str);
		}
		break;
	case FormulaCompareSubData::XiaoYu:		//	С��
		{
			str = hxSubZero(pSubData->m_dData1);

			strRet.Format("%s%s",m_strCondition[pSubData->m_cType],str);
		}
		break;
	case FormulaCompareSubData::DengYu:		//	����
		{
			str = hxSubZero(pSubData->m_dData1);

			strRet.Format("%s%s",m_strCondition[pSubData->m_cType],str);
		}
		break;
	case FormulaCompareSubData::JieYu:			//	����
		{
			str = hxSubZero(pSubData->m_dData1);
			str1 = hxSubZero(pSubData->m_dData2);

			strRet.Format("%s%s��%s֮��",m_strCondition[pSubData->m_cType],
				str,str1);
		}
		break;
	case FormulaCompareSubData::ShangChuan:	//	�ϴ�
		{
			str = hxSubZero(pSubData->m_dData1);

			strRet.Format("%s%s",m_strCondition[pSubData->m_cType],str);
		}
		break;
	case FormulaCompareSubData::XiaoPo:		//	����
		{
			str = hxSubZero(pSubData->m_dData1);

			strRet.Format("%s%s",m_strCondition[pSubData->m_cType],str);
		}
		break;
	case FormulaCompareSubData::WeiChi:		//	ά��
		{
			str = hxSubZero(pSubData->m_dData2);

			strRet.Format("%s%i���ں��ϴ�%s",m_strCondition[pSubData->m_cType],atol(pSubData->m_dData1),
				str);
		}
		break;
	case FormulaCompareSubData::BiaoChi:		//	���� 
		{
			str = hxSubZero(pSubData->m_dData2);

			strRet.Format("%s%i���ں�����%s",m_strCondition[pSubData->m_cType],atol(pSubData->m_dData1),
				str);
		}
		break;
	}
	return strRet;
}

CString SingleSelCondition::GetDescribe(void* pData /*= NULL*/)
{
	CValue* valoare;
	CString strParam;
	if( CExpression::m_pExternExpression->Lookup( m_strExp,m_nType,valoare ) )
	{
		CExpression* pExpress = valoare->GetExp();
		BOOL bFree = FALSE;
		if( !pExpress->IsCompile() )
		{
			if( !pExpress->Compile() )
			{
				bFree = TRUE;
			}
		}
		CString str = pExpress->GetParamString();
		if( !str.IsEmpty() )
		{
			strParam.Format( "(%s)",str );
		}
		if( bFree )
		{
			((CExpValue*)valoare)->Free();
		}
	}

	CString strRet;
	switch( m_nType )      // ��ʽ����
	{
	case Union: // �������
		{
		}
		break;
	case JiBenMian: // ����ָ��
		{	
			strRet.Format("%s:[%s]%s",m_strExp,m_strCondition,GetCondition(&m_SubData) );
		}
		break;
	case Tech:
		{
			if( pData != NULL )
			{
				strRet.Format("[%s]%s:%s%sָ����%s%s",GetPeriodStr(m_nPeriod),
					*(CString*)pData,m_strExp,strParam,
					m_strCondition,GetCondition(&m_SubData) );
			}
		}
		break;
	case Condition:
		{
			if( pData != NULL )
			{
				strRet.Format("[%s]%s:%s%s",GetPeriodStr(m_nPeriod),
					*(CString*)pData,m_strExp,strParam );
			}
		}
		break;
	case Exchange:
		{
			if( pData != NULL )
			{
				strRet.Format("[%s]%s:%s%s",GetPeriodStr(m_nPeriod),
					*(CString*)pData,m_strExp,strParam );
			}
		}
		break;
	case Strategy: //20090302 YJT ���� Strategy
		{
			if( pData != NULL )
			{
				strRet.Format("[%s]%s:%s%s",GetPeriodStr(m_nPeriod),
					*(CString*)pData,m_strExp,strParam );
			}
		}
		break;
	case MoreKLine:
		{
			if( pData != NULL )
			{
				strRet.Format("[%s]%s:%s%s",GetPeriodStr(m_nPeriod),
					*(CString*)pData,m_strExp,strParam );
			}
		}
		break;
	}
	return strRet;
}

BOOL SingleSelCondition::Open(void* pData /*= NULL*/)
{
	return 0;
}

char** CExpValue::GetConditionStr(int& nCount)
{
	nCount = (sizeof(m_strCondition)/sizeof(m_strCondition[0]));
	return m_strCondition;
}

CExpValue::CExpValue()
{
	m_value = NULL;
	m_pRefCalcValues.Empty();
	m_pParam = NULL;

	//******************************������litao����*****************************************
	m_LineName = "";
	m_InWhile = false;
	m_CurExecInWhile = false;
	m_WhileLevel=0;
	m_CurExecWhileLevel = 0;
	//******************************������litao����*****************************************

	//m_nIndex = 0;

	AddStyle(HX_EXPRESSSENTENCE);
}

CExpValue::~CExpValue()
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
		if(m_value != NULL)
		{
			delete (UnionSelCondition*)m_value;
			m_value = NULL;
		}
	}
	else
	{
		if(m_value != NULL)
		{
			delete (CExpression*)m_value;
			m_value = NULL;
		}
		m_pRefCalcValues.Empty();
		m_pParam = DeleteVarsMap(m_pParam,TRUE);
	}
	//add by xudan
	RemoveAllNo(EXECUTE_NO);
}

void CExpValue::Clean()
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
		if(m_value != NULL)
		{
			delete (UnionSelCondition*)m_value;
			m_value = NULL;
		}
		SetStyle(HX_UNION_EXPRESS);
	}
	else
	{
		if(m_value != NULL)
		{
			delete (CExpression*)m_value;
			m_value = NULL;
		}
		m_pRefCalcValues.Empty();
		m_pParam = DeleteVarsMap(m_pParam,TRUE);
		
		//m_nIndex = 0;
		
		SetStyle(HX_EXPRESSSENTENCE);
	}
}

void CExpValue::Free()
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
	}
	else
	{
		if(m_value != NULL)
		{
			((CExpression*)m_value)->Free();
		}
		m_pRefCalcValues.Empty();
		m_pParam = DeleteVarsMap(m_pParam,TRUE);
		
		//m_nIndex = 0;
	}
}

void CExpValue::Serialize(CArchive &ar)
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
		if (ar.IsStoring())
		{		
			char cStatus = 0;
			if( m_value != NULL )
			{
				cStatus = 2;
				ar << cStatus;
				((UnionSelCondition*)m_value)->Serialize(ar);
			}
			else
			{
				ar << cStatus;
			}
		}
		else
		{
			if(m_value == NULL)
			{
				m_value = new UnionSelCondition;
			}
			((UnionSelCondition*)m_value)->Serialize(ar);
		}
	}
	else
	{
		if (ar.IsStoring())
		{		
			char cStatus = 0;
			if( m_value != NULL && 
				!((CExpression*)m_value)->IsStyle(HS_EXP_TEMPIN) )
			{
				cStatus = 1;
				ar << cStatus;
				((CExpression*)m_value)->Serialize(ar);
			}
			else
			{
				ar << cStatus;
			}
		}
		else
		{
			if(m_value == NULL)
			{
				m_value = new CExpression(NULL,NULL,NULL);
			}
			((CExpression*)m_value)->Serialize(ar);
		}
	}
}

void CExpValue::Serialize(FxjExpress*  pFxjExpress)
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
	}
	else
	{
		if(pFxjExpress == NULL)
			return;
		if(m_value == NULL)
		{
			m_value = new CExpression;
		}
		((CExpression*)m_value)->Serialize(pFxjExpress);
	}
}

void CExpValue::Serialize(CExpression* pExpress)
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
	}
	else
	{
		if(pExpress == NULL)
			return;
		if(m_value == NULL)
		{
			m_value = new CExpression;
		}
		((CExpression*)m_value)->Serialize(pExpress);
	}
}

CExpression* CExpValue::GetExp()
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
		return (CExpression*)GetUnion();
	}
	else
	{
		if( m_value != NULL )
		{
			CExpression* pExpress = (CExpression*)m_value;
			if( pExpress->IsKindOf(RUNTIME_CLASS(CExpression)) )
			{
				return pExpress;
			}
		}
	}
	return NULL;
}

CExpression* CExpValue::GetExpByCompile()
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
	}
	else
	{
		if( m_value != NULL )
		{
			CExpression* pExpress = (CExpression*)m_value;
			if( pExpress->IsKindOf(RUNTIME_CLASS(CExpression)) )
			{
				if( !pExpress->IsCompile() )
				{
					if( pExpress->Compile() )
						return NULL;
				}
				return pExpress;
			}
		}
	}
	return NULL;
}

double CExpValue::GetValue(HSCurDataInfoStatus* pCurDataStatus)
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
	}
	else
	{
		double value = hx_DefMinValues;
		CExpression* pExpress = GetExpByCompile();
		if( pExpress != NULL )
		{
			pExpress->Value(value,pCurDataStatus);
		}
		return value;
	}
	return -1;
}

BOOL CExpValue::CalcValue(CLineProperty*& pRefData,int nStart,int nEnd,HSCurDataInfoStatus* pCurDataStatus,bool bEnd,CArray<CLineProperty*,CLineProperty*>* pAyData /*= NULL*/)
{

	BOOL bRet = FALSE;
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
	}
	else
	{	
		if( (m_value != NULL)&&(m_CurExecWhileLevel==m_WhileLevel)&&((!m_InWhile)||((m_InWhile)&&(m_CurExecInWhile))))
		{
			m_pRefCalcValues.m_lStart = nStart;
			m_pRefCalcValues.m_lSize  = nEnd;

			bRet = ((CExpression*)m_value)->CalcValue(pRefData,nStart,nEnd,
				pCurDataStatus,&m_pRefCalcValues,pAyData);

			CString strName;
			strName = m_LineName;
			//((CExpression*)m_value)->SetMaxObjNo(strName,EXECUTE_NO);
			if (bEnd)
				RemoveAllNo(EXECUTE_NO);
			else
				SetMaxObjNo(strName,EXECUTE_NO);
			m_pRefCalcValues.m_pValue = pRefData->m_pData;
		}
	}

	return bRet;
}

void CExpValue::UpdateValue()
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
	}
	else
	{
		if( m_value != NULL )
		{
			((CExpression*)m_value)->UpdateArbore();
		}
	}
}

void CExpValue::AddParam(CValue* pExpress)
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
	}
	else
	{
		if( pExpress == NULL)
			return;
		
		if(m_pParam == NULL)
		{
			m_pParam = new CMapVariabile;
		}
		CString strName;
		strName.Format("Param%i",m_pParam->GetCount());
		strName.MakeUpper();
		m_pParam->InsertAt((char*)LPCSTR(strName),pExpress,0);
	}
}

BOOL CExpValue::Find(CString strKey,CString& strRet,DWORD dMask /*= 0*/) 
{ 
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
	}
	else
	{
		if( m_value != NULL )
		{
			return ((CExpression*)m_value)->IsFind(strKey,strRet,dMask);
		}
	}
	return 0;
}

int CExpValue::ReName( CString strNewName,CMapVariabile* pExternExpression )
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
	}
	else
	{
		if( m_value == NULL || 
			strNewName.IsEmpty() || 
			pExternExpression == NULL) 
			return Fail;
		
		CString strOldName = ((CExpression*)m_value)->GetName();
		strNewName.MakeUpper();
		if( !strOldName.CompareNoCase(strNewName) )
			return Equal;

		CValue* valoare;
		if( pExternExpression->Lookup(strNewName,
			((CExpression*)m_value)->GetExpressType(),valoare) )
			return Exist;

		int nRet = Succ;
		if( CExpression::m_pExternExpression->Lookup(strNewName,
					((CExpression*)m_value)->GetExpressType(),valoare) )
		{
			nRet = SysExist;
		}
	
		if( !pExternExpression->Lookup(strOldName,
					((CExpression*)m_value)->GetExpressType(),valoare) )
			return NoFind;

		((CExpression*)m_value)->SetName(strNewName);
		pExternExpression->RemoveKey( strOldName,((CExpression*)m_value)->GetExpressType() );
		pExternExpression->AddExpress(strNewName,valoare);

		return nRet;
	}
	return 0;
}

CString CExpValue::GetWholeName()
{
	if( IsStyle(HX_UNION_EXPRESS) ) 
	{
		UnionSelCondition* pUnion = GetUnion();
		if( pUnion != NULL )
		{
			CString strWholeName = pUnion->m_strName;
			strWholeName.Insert(0,'5'); // Union
			return strWholeName;
		}
	}
	else
	{
		if(m_value != NULL)
		{
			return ((CExpression*)m_value)->GetWholeName();
		}
	}
	return "";
}

UnionSelCondition* CExpValue::GetUnion()
{
	return (UnionSelCondition*)m_value;
}

CTValues<void>*	CExpValue::GetRefCalcValues()
{
	return &m_pRefCalcValues;
}

// ��ز�����
CMapVariabile* CExpValue::GetParam()
{ 
	return m_pParam; 
}

void CExpValue::RemoveAllNo(int iCompExec)
{
//	((CExpression*)(m_value))->RemoveAllNo(EXECUTE_NO);
	CString strName = "";
	ExecObjNoList* lpObjNo = NULL;
	POSITION atpos = m_pExecNo.GetStartPosition();  
	while (atpos != NULL)
	{
		m_pExecNo.GetNextAssoc( atpos, strName, lpObjNo );
		delete(lpObjNo);
	}
	m_pExecNo.RemoveAll();

	//******************************������litao����*****************************************
	/*ExecObjNoList* lpObjNo = NULL;
	CString strName = "";

	POSITION atpos = m_pExecNo.GetStartPosition();  
	while (atpos != NULL)
	{
	m_pExecNo.GetNextAssoc( atpos, strName, lpObjNo );
	delete(lpObjNo);
	}*/

	//******************************������litao����*****************************************
}


//******************************������litao����*****************************************
void CExpValue::SetMaxObjNo(CString key,int iCompExec)
{
	CString strName = "";
	CString tmpName = "";

	LPEXECOBJNO lpObjNo;

	int iObjNo = 0;

	tmpName = key.Left(key.GetLength()-3);
	CString tmpNo = key.Right(3);
	int itmpNo = _ttoi(tmpNo);

	if (EXECUTE_NO ==  iCompExec)
	{
		if (m_pExecNo.Lookup(tmpName,lpObjNo))
		{		
			//if (lpObjNo->iObjNo < itmpNo) //�����ѭ���Ժ󣬱�ʾ��˳�����ã�����Ӧ�ò������ı���ʶ
			{
				lpObjNo->pcsObjNo->Lock();
				//m_pExecNo.SetAt(tmpName,itmpNo);
				lpObjNo->iObjNo = itmpNo;
				lpObjNo->pcsObjNo->Unlock();
			}
		}
		else
		{
			lpObjNo = new ExecObjNoList();
			lpObjNo->pcsObjNo->Lock();
			lpObjNo->iObjNo = itmpNo;
			lpObjNo->strName = tmpName;
			m_pExecNo.SetAt(tmpName,lpObjNo);
			lpObjNo->pcsObjNo->Unlock();
		}
	}
}
//******************************������litao����*****************************************
