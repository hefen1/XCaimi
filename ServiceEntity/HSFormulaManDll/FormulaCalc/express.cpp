// Function.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "math.h"
#include "express.h"
#include <ctype.h>
#include <float.h>

#include "RegMsg.h"
#include "VarValue.h"
#include "ExpValue.h"
#include "FunExpresion.h"
#include "HSDataSource.h"
#include "GeneralHelper.h"
#include "windowsx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static double g_dwRetValoare;

extern CHSDataSource*							 g_pHxDataSource;
extern CArray<ColorProperty*,ColorProperty*>*    g_pColours;
extern CSystemFun*								 g_pSystemFun;

int g_nExpressVersion = 1;

// ���ķ���
static CString g_strChineseSign = "�������磥�ޣ�����������������������������������������������������";
// �����ж�
static CString g_strPreNegativeSign = "��������������������������";
// ��������
static CString g_strChineseNumber = "������������������������";
// �����ַ�
static CString g_strChineseChar = "���£ãģţƣǣȣɣʣˣ̣ͣΣϣУѣңӣԣգ֣ףأ٣�";

static CString g_strSpecialSign = "~!@#$%^&*()_+{}|:\"<>?`-=[]\\;\',./ ";

static CArray<unsigned char,unsigned char> g_ayAheadChar;

/*#define _countof(array) (sizeof(array)/sizeof(array[0]))*/

IMPLEMENT_DYNCREATE(CExpressionBase, CObject)

IMPLEMENT_DYNCREATE(CExpression, CExpressionBase)

//CStockManager*   CExpression::m_pStockManager = NULL;
//CHSDisk*		 CExpression::m_pHSDisk = NULL;

// �����б�
typedef double ( *fnHSExpress)( LPNODELEMENT a,
							  HSCurDataInfoStatus* pCurDataStatus,
							  CExpression* pExpression,
							  double& v1,double& v2,
							  HSCurDataInfoStatus& calc1,
							  HSCurDataInfoStatus& calc2 );

static char	g_cInitMapFunList = 0;
//static CMapWordToPtr g_mapFunList;
static fnHSExpress g_FunctionList[255];
static void init()
{
	if( g_cInitMapFunList )
		return;

	g_cInitMapFunList = 1;

	memset(g_FunctionList,0,_countof(g_FunctionList));

	g_FunctionList[(BYTE)'+'] = CExpression::Add;
	g_FunctionList[(BYTE)'-'] = CExpression::Sub;
	g_FunctionList[(BYTE)'*'] = CExpression::Ride;
	g_FunctionList[(BYTE)'/'] = CExpression::Divide;

	g_FunctionList[(BYTE)'|'] = CExpression::Or;
	g_FunctionList[(BYTE)'^'] = CExpression::Pow;
	g_FunctionList[(BYTE)'<'] = CExpression::LessThan;
	g_FunctionList[(BYTE)'>'] = CExpression::GreatLess;

	g_FunctionList[(BYTE)'='] = CExpression::Equal;
	g_FunctionList[(BYTE)'!'] = CExpression::Not;
	g_FunctionList[(BYTE)'@'] = CExpression::ReturnValue;

	//g_FunctionList[(BYTE)'`'] = CExpression::ReturnString;

	g_FunctionList[(BYTE)Operator_SIN] = CExpression::Sin;
	g_FunctionList[(BYTE)Operator_COS] = CExpression::Cos;
	g_FunctionList[(BYTE)Operator_EXP ] = CExpression::Exp ; 
	g_FunctionList[(BYTE)Operator_SQRT ] = CExpression::Sqrt ; 
	g_FunctionList[(BYTE)Operator_LOG ] = CExpression::Log ; 
	g_FunctionList[(BYTE)Operator_TG ] = CExpression::Tg ;
	g_FunctionList[(BYTE)Operator_CTG ] = CExpression::Ctg ;
	g_FunctionList[(BYTE)Operator_ASIN ] = CExpression::Asin ;
	g_FunctionList[(BYTE)Operator_ACOS ] = CExpression::Acos ;
	g_FunctionList[(BYTE)Operator_ATG ] = CExpression::ATG ;
	g_FunctionList[(BYTE)Operator_LargerEqual] = CExpression::LargerEqual ; // >=
	g_FunctionList[(BYTE)Operator_SmallEqual] = CExpression::SmallEqual ;  // <=
	g_FunctionList[(BYTE)Operator_NotEqual] = CExpression::NotEqual ;    // != <>
	g_FunctionList[(BYTE)Operator_OR] = CExpression::DoubleOR ;          // || or
	g_FunctionList[(BYTE)Operator_AND] = CExpression::AND ;		   // && and
	g_FunctionList[(BYTE)Operator_Semicolon] = CExpression::Semicolon ;   // ; �ֺ�
	g_FunctionList[(BYTE)Operator_Error] = CExpression::Error ;       // ����������
	g_FunctionList[(BYTE)Operator_Komma] = CExpression::Komma ;      // ] = ���Ų���(һ�п��ж�����䣬�����ŷֽ�)
	g_FunctionList[(BYTE)Operator_Colon] = CExpression::Colon ;      // ��ͼ����(�����������ڴ����ϻ�ͼ�ı��ʽ)
	g_FunctionList[(BYTE)Operator_Evaluate] = CExpression::Evaluate ;   // ��ֵ����(�����߱�����ֵ] = �����Ǳ��ʽ��Ҳ��������ֵ] = ���ҽ���ֵ����ʽ������������б�)			
	g_FunctionList[(BYTE)Operator_Function] = CExpression::Function ;  // ��������
	g_FunctionList[(BYTE)Operator_Express] = CExpression::Express ;
	g_FunctionList[(BYTE)Operator_InportData] = CExpression::InportData ;

	/*
	g_mapFunList.InitHashTable(34);

	g_mapFunList.SetAt((WORD)'+',CExpression::Add);
	g_mapFunList.SetAt((WORD)'-',CExpression::Sub);
	g_mapFunList.SetAt((WORD)'*',CExpression::Ride);
	g_mapFunList.SetAt((WORD)'/',CExpression::Divide);

	g_mapFunList.SetAt((WORD)'|',CExpression::Or);
	g_mapFunList.SetAt((WORD)'^',CExpression::Pow);
	g_mapFunList.SetAt((WORD)'<',CExpression::LessThan);
	g_mapFunList.SetAt((WORD)'>',CExpression::GreatLess);

	g_mapFunList.SetAt((WORD)'=',CExpression::Equal);
	g_mapFunList.SetAt((WORD)'!',CExpression::Not);
	g_mapFunList.SetAt((WORD)'@',CExpression::ReturnValue);

	g_mapFunList.SetAt((WORD)Operator_SIN,CExpression::Sin);
	g_mapFunList.SetAt((WORD)Operator_COS,CExpression::Cos);
	g_mapFunList.SetAt((WORD)Operator_EXP ,CExpression::Exp ); 
	g_mapFunList.SetAt((WORD)Operator_SQRT ,CExpression::Sqrt ); 
	g_mapFunList.SetAt((WORD)Operator_LOG ,CExpression::Log ); 
	g_mapFunList.SetAt((WORD)Operator_TG ,CExpression::Tg );
	g_mapFunList.SetAt((WORD)Operator_CTG ,CExpression::Ctg );
	g_mapFunList.SetAt((WORD)Operator_ASIN ,CExpression::Asin );
	g_mapFunList.SetAt((WORD)Operator_ACOS ,CExpression::Acos );
	g_mapFunList.SetAt((WORD)Operator_ATG ,CExpression::ATG );
	g_mapFunList.SetAt((WORD)Operator_LargerEqual,CExpression::LargerEqual ); // >=
	g_mapFunList.SetAt((WORD)Operator_SmallEqual,CExpression::SmallEqual );  // <=
	g_mapFunList.SetAt((WORD)Operator_NotEqual,CExpression::NotEqual );    // != <>
	g_mapFunList.SetAt((WORD)Operator_OR,CExpression::DoubleOR );          // || or
	g_mapFunList.SetAt((WORD)Operator_AND,CExpression::AND );		   // && and
	g_mapFunList.SetAt((WORD)Operator_Semicolon,CExpression::Semicolon );   // ; �ֺ�
	g_mapFunList.SetAt((WORD)Operator_Error,CExpression::Error );       // ����������
	g_mapFunList.SetAt((WORD)Operator_Komma,CExpression::Komma );      // ,���Ų���(һ�п��ж�����䣬�����ŷֽ�)
	g_mapFunList.SetAt((WORD)Operator_Colon,CExpression::Colon );      // ��ͼ����(�����������ڴ����ϻ�ͼ�ı��ʽ)
	g_mapFunList.SetAt((WORD)Operator_Evaluate,CExpression::Evaluate );   // ��ֵ����(�����߱�����ֵ,�����Ǳ��ʽ��Ҳ��������ֵ,���ҽ���ֵ����ʽ������������б�)			
	g_mapFunList.SetAt((WORD)Operator_Function,CExpression::Function );  // ��������
	g_mapFunList.SetAt((WORD)Operator_Express,CExpression::Express );
	g_mapFunList.SetAt((WORD)Operator_InportData,CExpression::InportData );
	*/
}

static fnHSExpress GetExpressFun(WORD nFun)
{
	init();

	if( nFun >= 0 && nFun < 255 )
	{
		return g_FunctionList[nFun];
	}

	return NULL;
	/*
	void* value;
	if( g_mapFunList.Lookup(nFun,value) )
	{
		return (fnHSExpress)value;
	}
	return CExpression::Default;
	*/
}
// 

char GetCharByExpressType( char ch )
{
	switch( ch )
	{
	case Tech:
		ch = '1';
		break;
	case Exchange:
		ch = '2';
		break;
	case Condition:
		ch = '3';
		break;
	case MoreKLine:
		ch = '4';
		break;
	case Union:
		ch = '5';
		break;
	case ExpressBin:
		ch = '6';
		break;
	case Strategy: //20090302 YJT ���� Strategy
		ch = '7';
		break;
	default:
		break;
	}
	return ch;
}
//////////////////////////////////////////////////////////////////////
// CExpression Class
//////////////////////////////////////////////////////////////////////

//��Ϣע��ID
 UINT HX_USER_COMPILEDATA = ::RegisterWindowMessage(HS_COMPILE_DATA);
 UINT HX_USER_UPDATEDATA  = ::RegisterWindowMessage(HS_COMPILE_UPDATEDATA);
 UINT HX_USER_DRAGDATA    = ::RegisterWindowMessage(HS_COMPILE_DRAGDATA);
 UINT EXPRESS_HS_FIND_DATA_MSG = ::RegisterWindowMessage(HS_FIND_DATA_MSG);
 UINT EXPRESS_ZiXun_Data_Request = ::RegisterWindowMessage(HS_ZiXun_Data_Request);
 UINT m_HS_DOWNLOADDATA_MSG = ::RegisterWindowMessage(HS_DOWNLOADDATA_MSG);
 UINT m_HS_DOWNLOADDATA_Other_MSG = ::RegisterWindowMessage(HS_DOWNLOADDATA_Other_MSG);
 UINT HS_USER_STRATEGY = ::RegisterWindowMessage(_T("Strategy Trade Message"));
 UINT HS_USER_ZOOM		= ::RegisterWindowMessage(_T("Zoom In or Out"));	//add by lxqi 20090506	�ı䷶Χ��Ϣ
 UINT g_SendReLoadTabItemMsg = ::RegisterWindowMessage(_T("Tranfer Msg of ReLoadTabItem"));  //qinhn 20090721 Add ת�����¼���K��ͼTabָ��ҳǩ
 UINT g_SendDeleteTechIndexMsg = ::RegisterWindowMessage(_T("Tranfer Msg of Delete K Line TechIndex"));  //qinhn 20090721 Add ת��ɾ��K��ͼָ����Ϣ


CMapVariabile*  CExpression::m_pExternExpression = NULL;         // �ⲿ���ʽ��
CMapVariabile*  CExpression::m_pExternVariabile  = NULL;		 // �ⲿ������
CMapVariabile*  CExpression::m_pExternFunction   = NULL;		 // �ⲿ������

CImageList*		CExpression::m_expressImages = NULL;

// Error code var
CArray<nodErrorInfo*,nodErrorInfo*>* m_pAyError = NULL;
CWnd* m_pMsgWnd = NULL;

struct _Auto_Delete_ErrorInfo_
{
	~_Auto_Delete_ErrorInfo_()
	{
		DeleteError();
	}
	_Auto_Delete_ErrorInfo_()
	{
		g_ayAheadChar.Add((unsigned char)Operator_Evaluate);
		g_ayAheadChar.Add((unsigned char)Operator_Colon);
	}
};
static _Auto_Delete_ErrorInfo_ _auto_delete_errorinfo_;

void AddError( nodErrorInfo* pError )
{
	if(m_pAyError == NULL)
		m_pAyError = new CArray<nodErrorInfo*,nodErrorInfo*>;

	if( m_pMsgWnd != NULL && 
		::IsWindow(m_pMsgWnd->m_hWnd) )
	{
		m_pMsgWnd->SendMessage(HX_USER_COMPILE_INFO,0,(LPARAM)pError);
	}

	m_pAyError->Add(pError);
}

void DeleteError()
{
	if(m_pAyError != NULL)
	{
		for(int i = m_pAyError->GetSize() - 1; i >= 0; i--)
		{
			delete m_pAyError->GetAt(i);
		}
		m_pAyError->RemoveAll();
		delete m_pAyError;
		m_pAyError = NULL;
	}
}

int GetErrorCount()
{
	if(m_pAyError != NULL)
	{
		return m_pAyError->GetSize();
	}
	return 0;
}

BOOL IsStop()
{
	return (GetErrorCount() > 0);
}

int HSIsalpha(int c)
{
	if( (unsigned)(c + 1) <= 256 )
		return isalpha(c);
	return 0;
}

int HSisdigit(int c)
{
	if( (unsigned)(c + 1) <= 256 )
		return isdigit(c);
	return 0;
}

// fine map
int FindInMap(const CString& strKey,
			  CMapVariabile* pVarsMap,
			  DWORD dwFindMask /*= 0*/,
			  CArray<CValue*,CValue*>* pArray /*= NULL*/,
			  CListCtrl* pCtrl/*= NULL*/)
{
	if( dwFindMask == 0 ) 
		return 0;
	if(pVarsMap != NULL)
	{
		POSITION pos = pVarsMap->GetStartPosition();
		CString sName;
		CValue *pVal;
		while (pos != NULL)
		{			
			pVarsMap->GetNextAssoc (pos, sName, pVal);
			if(pVal != NULL)
			{
				if( pVal->IsStyle(HX_EXPRESSSENTENCE) )
				{
					/*				if( ((CExpValue*)pVal)->Find(strKey,dwFindMask) )
					{

					}
	*/			}		
			}
		}
	}

	return 0;
}

// ɾ��map
CMapVariabile* DeleteVarsMap(CMapVariabile* pVarsMap,BOOL bSelf)
{
	if(pVarsMap != NULL)
	{
		POSITION pos = pVarsMap->GetStartPosition();
			CString sName;
		CValue *pVal;
		while (pos != NULL)
		{			
			pVarsMap->GetNextAssoc (pos, sName, pVal);
			if(pVal != NULL)
			{
				delete pVal;
			}
		}
		pVarsMap->RemoveAll();
		if(bSelf)
		{
			delete pVarsMap;
			pVarsMap = NULL;
		}
	}
	return pVarsMap;
}

// �õ�һ��Ψһ������
CString	GetIdentifierExpressName(CString strWholeName,CString strOldName,CMapVariabile* pVarsMap)
{
	if(pVarsMap != NULL)
	{
		CString strName;
		if( strWholeName.GetLength() > 0 )
		{
			strName = strWholeName;
		}
		else
		{
			strName = strOldName;
		}

		CString strRetName;
		CValue* valoare;
		int i = 1;
		while(1)
		{
			if ( !pVarsMap->Lookup(strName,valoare) )
			{
				break;
			}
			else
			{
				if( strWholeName.GetLength() > 0 )
				{
					strName.Format("%c%s(%i)",strWholeName.GetAt(0),strOldName,i);
				}
				else
				{
					strName.Format("%s(%i)",strOldName,i);					
				}

				strRetName.Format("%s(%i)",strOldName,i);
			}
			i++;
		}

		return strRetName;
	}

	return strOldName;
}

// �ɴ���ŵõ�������Ϣ
CString hxGetErrorInfoByErrorNum(UINT nID)
{
	switch(nID)
	{
	case hxError_DIVISION_BY_0: return "�� 0 ��";
	case hxError_ILEGAL_OPERATION: return "δ֪����";
	case hxError_UNDEFINED_VARIABLE: return "����δ����";
	case hxError_INVALID_DOMAIN: return "log ������";
	case hxError_Fun_Exist: return "�����Ѿ�����";
	case hxError_Var_Exist: return "�����Ѿ�����";
	case hxError_Nod_CloneFail: return "������Ԫ����ʧ��";
	case hxError_InvalidateVars: return "��Ч����";
	case hxError_NotFunction: return "����������";
	case hxError_NotVariable: return "����������";
	case hxError_Operator: return "����������";
	case hxError_Fun_param: return "������������";
	case hxError_Identification: return "����ʶ����ַ�";
	case hxError_AddRight: return "+�Ӻ��ұߴ���";
	case hxError_SubRight: return "-�����ұߴ���";
	case hxError_RideRight: return "+�˺��ұߴ���";
	case hxError_DivideRight: return "-�����ұߴ���";
	case hxError_PowerRight: return "^�˷��ұߴ���";
	case hxError_BracketLeft: return "(�����Ŵ���";
	case hxError_BracketRight: return ")�����Ŵ���";
	case hxError_AbsLeft: return "| |����ֵ��ߴ���";
	case hxError_AbsRight: return "| |����ֵ�ұߴ���";
	case hxError_LogicalRight: return "�߼��������ұߴ���(< > = == >= <= <> !=)";
	case hxError_CommaRight: return "�����ұߴ���";
	case hxError_OrRight: return "or || �߼����ұߴ���";
	case hxError_AndRight: return "and && �߼������ұߴ���";
	case hxError_DefinedRight: return ":=��ֵ���ʽ�ұߴ���";
	case hxError_DrawRight: return ":��ͼ�������ұߴ���";
	case hxError_ReverseRight: return "!ȡ���ұߴ���";
	case hxError_Name_Exist: return "��ʶ���Ѿ�����";
	case hxError_NotName_Exist: return "��ʶ��δ����";
	case hxError_Notes_Invalidate: return "ע�ʹ���/**/ //";
	case hxError_AtLeastHasExp: return "������Ҫһ�����ʽ";
	case hxError_AtLeastHasDrawExp: return "������Ҫһ����ͼ���ʽ";
	case hxError_WantInvertedComma: return "ȱ�����÷���";
	case hxError_Exp_param: return "ȱ�����ò���";
	case hxError_unknowPeriod: return "����ʶ�������";
	//******************************������litao����*****************************************
	case hxError_MoreLoop: return "��ʽѭ������������ֹ࣬ͣ����";
	//******************************������litao����*****************************************
	}
	return "";
}

// ���������ֽ��ִ�
int AnalyzeStr(CString strResource,const char* strDelimit,CStringArray& ayRet)
{
	ayRet.RemoveAll();

	CString strCur;
	strCur.Format("%s",strResource);
	CString tmp;
	char* token = strtok( (char*)LPCSTR(strCur), strDelimit);
	while(token != NULL)
	{
		tmp.Format("%s",token);
		ayRet.Add(tmp);
		token = strtok( NULL, strDelimit);
	}
	return ayRet.GetSize();
}

// �Ƿ�������һ���ֺ�
void RelaceLastChar(CString& strText)
{
	int nLength = strText.GetLength();
	CString strLast;
	if( nLength > 0 )
	{
		if( strText[nLength-1] != ';' )
		{
			if( nLength > 1 )
			{
				strLast = strText.Mid(nLength-2);
				if( strLast.Compare("��") )
				{
					strText = strText + ";";
				}
			}
		}
	}
}

// ʹ�ı����ǰ������
CString AdjustStr(CString pOrg,int& nReqLen,CString* pstrLeft /*= NULL*/)
{	
	try
	{
		CString strTmp,strTmp1;
		strTmp.Format("%s",pOrg);
		strTmp = strTmp + '\0' + '\0';
		int nLen = strTmp.GetLength();
		char cChar = 0;
		int nPos = 0;
		CStringArray ay;
		while( strTmp[nPos] != '\0' )
		{
			if( ((BYTE)strTmp[nPos]) > 0x7f )
			{	
				cChar ++;
			}
			else if( HSisdigit( strTmp[nPos] ) || 
				     HSIsalpha( strTmp[nPos] ) ||
					 isalnum( strTmp[nPos] )    ||
					 (g_strSpecialSign.Find(strTmp[nPos]) != -1) )
			{
				strTmp1.Format("%c",strTmp[nPos] );
				ay.Add(strTmp1);
			}
			
			if(cChar == 2)
			{
				strTmp1.Format("%s",strTmp.Mid(nPos-1,2) );
				ay.Add(strTmp1);
				cChar = 0;
			}

			nPos++;
		}

		strTmp1.Empty();
		int nCharPos = 0;
		for(nPos = 0; nPos < ay.GetSize() && nCharPos < nReqLen;nPos++)
		{
			strTmp = ay.GetAt(nPos);
			nCharPos += strTmp.GetLength();
			strTmp1 += strTmp;
		}
		nReqLen = nCharPos;

		if( pstrLeft != NULL )
		{
			pstrLeft->Empty();
			for(; nPos < ay.GetSize();nPos++)
			{
				*pstrLeft += ay.GetAt(nPos);
			}
		}

		return strTmp1;

		/*
		CString strOrg = pOrg;

		CString strTemp;
		CString strFormat;
		strFormat.Format("%%%is",nReqLen);

		int nTrueLenth = strOrg.GetLength();
		if( nReqLen >= nTrueLenth )
		{
			strTemp.Format(strFormat,strOrg);
			return strTemp;
		}
		
		int nLenth = nReqLen;
		char ctemp;
		int  i = 0;
		int  nLittle = 0;
		while( i < nLenth )
		{
			ctemp = strOrg.GetAt(i);
			if( (ctemp & 0x80) == 0 )
			{
				nLittle++;
			}
			i++;
		}
		if( (nLittle % 2) && (ctemp & 0x80) != 0 )
		{
			nLenth--;
		}
		
		CString strTemp1;
		strTemp1.Format("%s",strOrg);
		strTemp1 = strTemp1.Left(nLenth);

		strFormat.Format("%%%is",nLenth);
		strTemp.Format(strFormat,strTemp1);

		return strTemp;
		*/
	}
	catch (...) 
	{
	}
	return "";
}

// �滻ָ���ַ�
CString hxRelaceReturnKey(CString strExp,CString seps,CString* pstrCur /*= NULL*/)
{	
	CString strRet,strTmp;
	strRet.Empty();
	char* token = strtok( (char *)LPCTSTR(strExp), seps );
	while( token != NULL )
	{
		strTmp =  token;		
		strRet += strTmp;
		if( pstrCur )
		{
			strRet += *pstrCur;
		}
		token = strtok( NULL, seps );
	}	
	return strRet;
}

BOOL IsChineseChar(CString& strTmp1)
{
	strTmp1.MakeUpper();
	int nPos = g_strChineseChar.Find(strTmp1);
	if(nPos != -1)
	{
		strTmp1.Format("%c",nPos/2+'A');
		return TRUE;
	}
	return FALSE;
}

BOOL IsChineseNum(CString& strTmp1)
{
	if( !strTmp1.CompareNoCase("��") || 
	    !strTmp1.CompareNoCase("��") )
	{
		strTmp1 = ".";
	}
	else
	{
		int nPos = g_strChineseNumber.Find(strTmp1);
		if( nPos != -1 )
		{
			strTmp1.Format("%c",(nPos/2)+'0');
			return TRUE;
		}
	}

	return FALSE;
}

// �滻��������
CString ReplaceNumber(CString& strSource)
{
	CString strTmp1,strTmp2;
	strTmp2.Empty();
	for(int i = 0; i < strSource.GetLength(); i ++)
	{
		if( (BYTE)strSource[i] > 0x7f )
		{
			strTmp1 = strSource.Mid(i,2);
			i += 1;
		}
		else
		{
			strTmp1 = strSource.GetAt(i);
		}
		IsChineseNum(strTmp1);
		strTmp2 += strTmp1;
	}
	return strTmp2;
}

// �滻�����ַ�
CString ReplaceChar(CString& strSource)
{
	CString strTmp1,strTmp2;
	strTmp2.Empty();
	for(int i = 0; i < strSource.GetLength(); i ++)
	{
		if( (BYTE)strSource[i] > 0x7f )
		{
			strTmp1 = strSource.Mid(i,2);
			i += 1;
		}
		else
		{
			strTmp1 = strSource.GetAt(i);
		}
		IsChineseChar(strTmp1);
		IsChineseNum(strTmp1);
		strTmp2 += strTmp1;
	}
	return strTmp2;
}

// �Ƿ���ȫ������
int IsChineseNumber(BYTE cChar,const CString& strSource,int& nPos)
{
	if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);		
		if( g_strChineseNumber.Find(strKey) != -1 )
		{
			return 2;
		}
		return 0;
	}
	if(cChar == '.')
		return 1;
	return 0;
}

// �Ƿ��Ǻ���
int IsChinese(BYTE cChar,const CString& strSource,int& nPos)
{
	if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( g_strChineseSign.Find(strKey) != -1 )
		{
			return 0;
		}
		return 1;
	}
	return 0;
}

int IsChinese(BYTE cChar,const CString& strSource,int& nPos,BOOL bChange)
{
	if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid( ((nPos - 1) < 0)?0:(nPos - 1) ,2);
		if( g_strChineseSign.Find(strKey) != -1 ||
			!strKey.CompareNoCase("��") )
		{
			if(bChange)
			{
				nPos --;				
			}
			return 2;
		}
		return 1;
	}
	return 0;
}

CString hxSubZero(CString str)
{
	int nLen = str.GetLength() - 1;
	if( str.Find(".") != -1 )
	{
		for( ; nLen > 0; nLen-- )
		{
			if( (TCHAR)str[nLen] == (TCHAR)'0' )
			{
				str.Delete(nLen,1);
			}
			else if( (TCHAR)str[nLen] == (TCHAR)'.' )
			{
				str.Delete(nLen,1);
				break;
			}
			else
			{
				break;
			}
		}
	}
	if( nLen == 1 && (TCHAR)str[0] == (TCHAR)'0' )
	{
		/*str.Delete(0,1);*/
	}

	return str;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Lookup
CMapVariabile::CMapVariabile()
{
}

CMapVariabile::~CMapVariabile()
{
}


BOOL CMapVariabile::Lookup(CString key, CValue*& rValue) const
{
	return CHSMapVariabile::Lookup(key, rValue); 
}

BOOL CMapVariabile::Lookup(CString key,char cType, CValue*& rValue) const
{
	cType = GetCharByExpressType(cType);
	if( cType != 0 )
	{
		key.Insert(0,cType);
	}
	return CHSMapVariabile::Lookup(key, rValue);
}

BOOL CMapVariabile::IsExist(CString key, CValue*& rValue) const
{
	if( !key.IsEmpty() )
	{
		CString strKey = key;
		strKey.Insert(0,GetCharByExpressType(Tech));
		if( CHSMapVariabile::Lookup(key, rValue) )
			return TRUE;

		strKey = key;
		strKey.Insert(0,GetCharByExpressType(Exchange));
		if( CHSMapVariabile::Lookup(key, rValue) )
			return TRUE;

		strKey = key;
		strKey.Insert(0,GetCharByExpressType(Condition));
		if( CHSMapVariabile::Lookup(key, rValue) )
			return TRUE;

		strKey = key;
		strKey.Insert(0,GetCharByExpressType(MoreKLine));
		if( CHSMapVariabile::Lookup(key, rValue) )
			return TRUE;

		strKey = key;
		strKey.Insert(0,GetCharByExpressType(Union));
		if( CHSMapVariabile::Lookup(key, rValue) )
			return TRUE;

		strKey = key;
		strKey.Insert(0,GetCharByExpressType(ExpressBin));
		if( CHSMapVariabile::Lookup(key, rValue) )
			return TRUE;

		//20090302 YJT ���� Strategy
		strKey = key;
		strKey.Insert(0,GetCharByExpressType(Strategy));
		if( CHSMapVariabile::Lookup(key, rValue) )
			return TRUE;
	}
	
	return CHSMapVariabile::Lookup(key, rValue); 
}

BOOL CMapVariabile::RemoveKey(CString key,char cType)
{
	cType = GetCharByExpressType(cType);
	if( cType != 0 )
	{
		key.Insert(0,cType);
	}
	return CHSMapVariabile::RemoveKey(key);
}

BOOL CMapVariabile::RemoveKey(CString key,CExpression* pExp)
{
	if( pExp != NULL )
	{
		return RemoveKey(key,pExp->GetExpressType() );
	}
	return CHSMapVariabile::RemoveKey(key);
}

// Lookup and add if not there
CValue* CMapVariabile::AddExpress(CString key,CValue* rValue)
{
	CValue* pGet;
	if( !key.IsEmpty() && rValue != NULL )
	{
		key = rValue->GetWholeName();
/*		CExpression* pExpress = rValue->GetExp();
		if( pExpress != NULL )
		{
			key = pExpress->GetWholeName();
		}
*/
		if( Lookup(key, pGet) ) // exist
		{
			return NULL;
		}
		else
		{
			CValue*& pValue = CHSMapVariabile::operator[](key); 
			pValue = rValue;
			return pValue;
		}
	}

	if( Lookup(key, pGet) )    // exist
	{
		return NULL;
	}
	
	CValue*& pValue = CHSMapVariabile::operator[](key); 
	pValue = rValue;
	return pValue;	
}

CValue*& CMapVariabile::operator[](CString key)
{
	return (CValue*&)CHSMapVariabile::operator[](key);
}

CString CExpression::InExternal::GetName()
{
	CString strRet = ReplaceChar(m_strName);
	strRet.TrimLeft();
	strRet.TrimRight();
	strRet.MakeUpper();
	return strRet;
}

CExpression::HSFormulaFileHead::HSFormulaFileHead()
{
	m_lCount   = 0;
	m_lVersion = 100001001; 
	m_strHead  = _T("HowAreYou!");
}

void CExpression::HSFormulaFileHead::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_lCount;
		ar << m_lVersion;
		ar << m_strHead;
	}
	else
	{
		ar >> m_lCount;
		ar >> m_lVersion;
		ar >> m_strHead;
	}
}

BOOL CExpression::HSFormulaFileHead::IsValid()
{
	CExpression::HSFormulaFileHead Head;
	return ( m_lVersion == Head.m_lVersion );
}

CWnd* CExpression::SetCurWnd(CWnd* pMsgWnd)
{
	m_pMsgWnd = pMsgWnd;
	return m_pMsgWnd;
}

CString CExpression::GetWholeName()		    
{
	char ch = GetCharByExpressType( GetExpressType() );
	if( ch != 0 )
	{
		CString strWholeName = m_strName;
		strWholeName.Insert(0,ch);
		return strWholeName;
	}
	return m_strName;
}	 

void CExpression::elibmem ( LPNODELEMENT a) // delete
{
	if ( a == NULL ) return;
	if ( a->left  != NULL ) elibmem(a->left);
	if ( a->right != NULL ) elibmem(a->right);
	delete a;
}

CExpression::CExpression()
{
	m_Arbore    = NULL;
	m_definitie = "";
	m_nPozitie  = 0;

	//******************************������litao����*****************************************
	m_strLineObj = "";				// ͷһ��λ��

	m_nIFWHILEStart = 0;			// �Ƿ���WHILEѭ���ڲ��ı���
	m_nIFWHILEEnd = 0;				// �Ƿ���WHILEѭ���ڲ��ı���
	m_nWhileFirst.RemoveAll();		// ѭ����ͷλ��
	m_nWhileLast.RemoveAll();		// ѭ����βλ��
	m_SaveExpDef.RemoveAll();		// �ϴξɵĹ�ʽ

	m_nDefSortId = 0;
	//******************************������litao����*****************************************

//	m_pExternExpression = NULL;
//	m_pExternVariabile  = NULL;
//	m_pExternFunction   = NULL;

	m_pExpDraw	 = NULL;	
	m_pParamVar  = NULL;
	m_pDataSource = NULL;

	m_pRefExpDraw   = NULL;			 // �ڲ���ͼ��
	m_pRefInsideVar = NULL;		     // �ڲ�������
	m_pRefParamVar  = NULL;		     // �ڲ�������

	m_pRefExpress	= NULL;			 // �������ʽ

	m_wEstopPeriod = 0;			      // ��������
	m_wDefaultPeriod = HX_PERIOD_PERIOD_TYPE_DAY; // ȱʡ����

	m_nExpressType = 0;

	m_dStyle = HS_EXP_SUBCHART;
}

CExpression::CExpression(CMapVariabile* pExternExpression,
						 CMapVariabile* pExternVariabile,
						 CMapVariabile* pExternFunction)
{
	m_Arbore    = NULL;
	m_definitie = "";
	m_nPozitie  = 0;
	
//	m_pExternExpression = pExternExpression;
//	m_pExternVariabile  = pExternVariabile;
//	m_pExternFunction   = pExternFunction;

	m_pExpDraw	 = NULL;
	m_pParamVar  = NULL;
	m_pDataSource = NULL;

	m_pRefExpDraw   = NULL;			 // �ڲ���ͼ��
	m_pRefInsideVar = NULL;		     // �ڲ�������
	m_pRefParamVar  = NULL;		     // �ڲ�������	

	m_pRefExpress	= NULL;			 // �������ʽ

	m_nExpressType = 0;

	m_dStyle = HS_EXP_SUBCHART;

}

void CExpression::Free()
{
	if( m_Arbore != NULL )
	{
		elibmem( m_Arbore );
		m_Arbore = NULL;
	}	

	m_pExpDraw   = DeleteVarsMap(m_pExpDraw,TRUE);	
	//m_pParamVar  = DeleteVarsMap(m_pParamVar,TRUE);

	m_nPozitie = 0;

/*
	m_pRefExpDraw   = NULL;
	m_pRefInsideVar = NULL;		 // �ڲ�������
	m_pRefParamVar  = NULL;		 // �ڲ�������	

	m_pRefExpress	= NULL;		 // �������ʽ
*/
}

CExpression::~CExpression()
{
	if(m_Arbore != NULL)
	{
		elibmem( m_Arbore );
		m_Arbore = NULL;
	}	

	m_pExpDraw   = DeleteVarsMap(m_pExpDraw,TRUE);	
	m_pParamVar  = DeleteVarsMap(m_pParamVar,TRUE);

	m_pRefExpDraw   = NULL;
	m_pRefInsideVar = NULL;		 // �ڲ�������
	m_pRefParamVar  = NULL;		 // �ڲ�������	

	m_pRefExpress	= NULL;		 // �������ʽ
}

ExpressParam::ExpressParam()
{
	pValue = NULL;
	pOther = NULL;
}

ExpressParam::~ExpressParam()
{
	if(pValue != NULL)
	{
		delete (CExpValue*)pValue;
		pValue = NULL;
	}
	pOther = NULL;
}

InportDataParam::InportDataParam()
{
	m_pDataSrc = NULL;
}

InportDataParam::~InportDataParam()
{
	if(m_pDataSrc != NULL)
	{
		delete m_pDataSrc;
		m_pDataSrc = NULL;
	}
}
//
////////
FunctionParam::FunctionParam()
{
	pFun   = NULL;
	ayVars = new FormulaGeneralParam;
}

FunctionParam::~FunctionParam()
{
	if(ayVars != NULL)
	{
		delete ayVars;
	}
	pFun   = NULL;
	ayVars = NULL;
}

void FunctionParam::Serialize(CExpression* pExpress,LPNODELEMENT nod,CArchive& ar)
{
	/*
	CString strName = _T("");
	if (ar.IsStoring())
	{
		CFunExpresion* pCurFun = (CFunExpresion*)pFun;
		if(pCurFun != NULL)
		{
			strName = pCurFun->GetFunName();
		}
		ar << strName;
	}
	else
	{
		ar >> strName;
		
		short nRet;
		CValue* valoare = pExpress->GetVariable(strName,nRet);
		if( valoare != NULL && nRet == CExpression::ExpExternFun )
		{
			// ������			
			FunctionParam* pFunctionParam = new FunctionParam();
			if(pFunctionParam == NULL) return;
			
			CFunExpresion* pFun = (CFunExpresion*)valoare;
			pFunctionParam->pFun = pFun;
			
			LPNODELEMENT pLTree = NULL;
			if( nod != NULL && pFun->GetParams() > 0 )
			{						
				pLTree = nod;
				pExpress->GetVariable(pLTree,pFunctionParam->ayVars);
			}
			if(pFun->GetParams() > 0 && pLTree)
			{
				pFunctionParam->ayVars->Add(pLTree);
			}
		}
	}
	*/
}

//////////////////
nodElement::nodElement()
{
	cDataType    = 0;

	operatie     = 0;
	left = right = NULL;
	valoare      = 0;
	valoarestr   = NULL;

	pData	     = NULL;
}

nodElement::~nodElement()
{
	if( (cDataType & DataTypeOperStr) &&  // cDataType == DataTypeOperStr
		valoarestr != NULL )
	{
		delete valoarestr;
		valoarestr = NULL;
	}
	if((cDataType & DataFunParam) && pData != NULL && operatie == Operator_Function)
	{
		delete (FunctionParam*)pData;
		pData = NULL;
	}
	if((cDataType & DataExpParam) && pData != NULL && operatie == Operator_Express)
	{
		delete (ExpressParam*)pData;
		pData = NULL;
	}
	if((cDataType & DataInport) && pData != NULL && operatie == Operator_InportData)
	{
		delete (InportDataParam*)pData;
		pData = NULL;
	}
}

void nodElement::Copy(const nodElement* pClone)
{
	if(pClone == NULL)
		return;

	operatie  = pClone->operatie;		   // ������
	cDataType = pClone->cDataType;
	if( (pClone->cDataType & DataTypeOperStr) )
	{
		valoarestr = new CString;
		valoarestr->Format("%s",*pClone->valoarestr);
	}
	else if( operatie == '@' )				// ��ֵ
	{
		valoare = pClone->valoare;
	}

	if( cDataType & DataFunParam ) // ��������ָ�룬��ɾ��
	{
		cDataType &= (~DataFunParam);
	}

	if( cDataType & DataExpParam ) // ��ʽ����ָ�룬��ɾ��
	{
		cDataType &= (~DataExpParam);
	}

	if( cDataType & DataInport )   // ��������ָ�룬��ɾ��
	{
		cDataType &= (~DataInport);
	}

	pData = pClone->pData;
}

void nodElement::Serialize(CExpression* pExpress,CArchive& ar)
{
	/*
	char cNext = 0;
	if (ar.IsStoring())
	{
		ar << operatie;		   // ������
		ar << cDataType;

		if( (cDataType & DataTypeOperStr) )
		{
			if(valoarestr != NULL)
			{
				cNext = 1;
				ar << cNext;
				ar << *valoarestr;
			}
			else
			{
				cNext = 0;
				ar << cNext;
			}
		}
		else if( operatie == '@' )				// ��ֵ
		{
			ar << valoare;
		}
		
		cNext = 0;
		if((cDataType & DataFunParam) && pData != NULL && operatie == Operator_Function)
		{
			if ( left != NULL ) // ��
			{
				cNext = 2;
				ar << cNext;
				left->Serialize(pExpress,ar);
			}
			else
			{
				ar << cNext;
			}
			
			if(pData != NULL)
			{
				cNext = 3;
				ar << cNext;
				((FunctionParam*)pData)->Serialize(pExpress,this,ar);
			}
			else
			{
				cNext = 0;
				ar << cNext;
			}

			cNext = 99;
		}

		if(cNext == 0)
		{
			if ( left  != NULL ) // ��
			{
				cNext = 4;
				ar << cNext;
				left->Serialize(pExpress,ar);
			}
			else
			{
				ar << cNext;
			}
		}

		if ( right != NULL ) // ��
		{
			cNext = 5;
			ar << cNext;
			right->Serialize(pExpress,ar);
		}
		else
		{
			cNext = 0;
			ar << cNext;
		}
	}
	else
	{
		ar >> operatie;		   // ������
		ar >> cDataType;
		if( (cDataType & DataTypeOperStr) )
		{
			ar >> cNext;
			if( cNext )
			{
				valoarestr = new CString;
				ar >> *valoarestr;
			}
		}
		else if( operatie == '@' )				// ��ֵ
		{
			ar >> valoare;
		}

		cNext = 0;
		if((cDataType & DataFunParam) && operatie == Operator_Function)
		{
			ar >> cNext;  // ��
			if ( cNext ) 
			{
				left = new nodElement;
				left->Serialize(pExpress,ar);
			}

			ar >> cNext;
			if( cNext )
			{
				pData = new FunctionParam;
				((FunctionParam*)pData)->Serialize(pExpress,left,ar);
			}

			cNext = 99;
		}

		if(cNext == 0)
		{
			ar >> cNext; // ��
			if ( cNext ) 
			{
				left = new nodElement;
				left->Serialize(pExpress,ar);
			}
		}

		ar >> cNext;    // ��
		if ( cNext ) 
		{
			right = new nodElement;
			right->Serialize(pExpress,ar);
		}
	}
	*/
}

BOOL CExpression::IsCompile()
{
	return (m_Arbore != NULL) || (m_nPozitie == -1);
}

int CExpression::Compile()
{
	DeleteError();  // ɾ����һ�δ���
	UpdateArbore();
	return GetErrorCount();
}

void CExpression::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{
		ar << g_nExpressVersion; // �汾

		ar << m_nExpressType;
		ar << m_strName;

		ar << m_strPWD;

		RelaceLastChar(m_definitie);
		ar << m_definitie;

		ar << m_strDescribe;
		ar << m_strNotes;
		ar << m_strEidolon;

		ar << m_strDefAxis;
		ar << m_wEstopPeriod;
		ar << m_wDefaultPeriod;
		ar << m_dStyle;

		//-----2009.12.16 litao�޸ĳ��ù�ʽ������δ��������----------------------------------------------
		ar << m_nDefSortId;
		//-----------------------------------------------------------------------------------------------

		// �ڲ�������		
		if(m_pParamVar != NULL)
		{
			CArray<class CValue*,class CValue*>* pArray = m_pParamVar->GetObjArray();
			if( pArray != NULL )
			{
				ar << pArray->GetSize();

				CValue* pCurVal;	
				CString strName;
				for(int nObj = 0; nObj < pArray->GetSize(); nObj++)
				{
					pCurVal = pArray->GetAt(nObj);
					strName = m_pParamVar->GetObjectName(pCurVal);
					ar << strName;
					pCurVal->Serialize(ar);
				}
			}
			else
			{
				ar << 0;
			}

			/*
			POSITION pos = m_pParamVar->GetStartPosition();
			while (pos != NULL)
			{
				CString sName;
				CValue *pVal;
				m_pParamVar->GetNextAssoc (pos, sName, pVal);
				if(pVal != NULL)
				{
					ar << sName;
					pVal->Serialize(ar);
				}
			}
			*/
		}
		else
		{
			ar << 0;
		}

#if 0
		char nNext = 0;
		if( m_Arbore != NULL )
		{
			nNext = 1;
			ar << nNext;
			m_Arbore->Serialize(this,ar);
		}
		else
		{
			ar << nNext;
		}
#endif

	}
	else
	{
		int nExpressVersion;
		ar >> nExpressVersion; // �汾

		ar >> m_nExpressType;  // ����
		ar >> m_strName;
		
		ar >> m_strPWD;

		ar >> m_definitie;
		ar >> m_strDescribe;
		ar >> m_strNotes;
		ar >> m_strEidolon;
		ar >> m_strDefAxis;
		ar >> m_wEstopPeriod;
		ar >> m_wDefaultPeriod;
		ar >> m_dStyle;
		//-----2009.12.16 litao�޸ĳ��ù�ʽ������δ��������----------------------------------------------
		ar >> m_nDefSortId;
		//-----------------------------------------------------------------------------------------------

		m_pExpDraw	 = NULL;
		m_pParamVar  = NULL;
		m_Arbore   = NULL;
		m_nPozitie = 0;

		m_pDataSource = NULL;

		// �ڲ�������
		int nCountVar;
		ar >> nCountVar;
		for(short i = 0; i < nCountVar;i++)
		{
			CString sName;
			CNumericValue *pVal = new CNumericValue();
			ar >> sName;
			pVal->Serialize(ar);
			AddParamer(sName,pVal);
		}

		m_definitie = m_definitie + '\0' + '\0';

		//DeleteError();  // ɾ����һ�δ���
		//UpdateArbore();
#if 0		
		{		
			char nNext = 0;
			ar >> nNext;
			if( nNext )
			{
				m_Arbore = new nodElement;
				m_Arbore->Serialize(this,ar);
			}
		}
#endif

	}
}

void CExpression::Serialize(FxjExpress*  pFxjExpress)
{
	if( pFxjExpress == NULL || 
		pFxjExpress->m_pName == NULL )
		return;

	try
	{
		m_dStyle = 0;

		if(pFxjExpress->m_pName->m_nChart == 4 || pFxjExpress->m_pName->m_nChart == 3)
		{
			m_nExpressType = Tech;  // ����
			m_dStyle = ((pFxjExpress->m_pName->m_nChart == 4)?HS_EXP_SUBCHART:HS_EXP_MAINCHART);
		}
		else if( pFxjExpress->m_pName->m_nChart == 5 )
		{
			m_nExpressType = MoreKLine;
		}
		else if( pFxjExpress->m_pName->m_nChart == 1 )
		{
			m_nExpressType = Exchange;
		}
		else if( pFxjExpress->m_pName->m_nChart == 0 )
		{
			m_nExpressType = Condition;
		}
		else
		{
			m_nExpressType = none;  // ����
		}
		m_strPWD = "";
		
		if(pFxjExpress->m_pBody != NULL)
		{
			m_definitie = pFxjExpress->m_pBody->GetText();
		}
		
		if(pFxjExpress->m_pDepiction != NULL)
		{
			m_strDescribe = pFxjExpress->m_pDepiction->GetText();
		}
		if(pFxjExpress->m_pNotes != NULL)
		{
			m_strNotes    = pFxjExpress->m_pNotes->GetText();
		}
		if(pFxjExpress->m_pEidolon != NULL)
		{
			m_strEidolon = pFxjExpress->m_pEidolon->m_sEidolon.GetText();
		}
		
		m_strDefAxis     = "";
		m_wEstopPeriod   = 0;
		m_wDefaultPeriod = 0;
		
		//m_dStyle = 0;
		
		m_pExpDraw	 = NULL;
		m_pParamVar  = NULL;
		m_pDataSource = NULL;
		
		m_Arbore   = NULL;
		m_nPozitie = 0;
		
		// �ڲ�������
		if(pFxjExpress->m_ayParam != NULL)
		{
			int nCountVar = pFxjExpress->m_ayParam->GetSize();
			FxjParam* pParam;
			for(short i = 0; i < nCountVar;i++)
			{
				pParam = pFxjExpress->m_ayParam->GetAt(i);
				CString sName = pParam->GetText();
				if( !sName.IsEmpty() )
				{
					CNumericValue *pVal = new CNumericValue();
					pVal->Serialize(pParam);
					AddParamer(sName,pVal);
				}
			}
		}
		
		m_definitie = m_definitie + '\0' + '\0';
	}
	catch(...)
	{
	}
}

void CExpression::Serialize(CExpression* pExpress)
{
	if( pExpress == NULL || 
		pExpress->m_strName.IsEmpty() )
		return;

	try
	{
		m_nExpressType = pExpress->m_nExpressType;  // ����
		m_dStyle	   = pExpress->m_dStyle;
		m_strPWD	   = pExpress->m_strPWD;
		
		m_definitie    = pExpress->m_definitie;
		
		if( m_strName.IsEmpty() )
		{
			m_strName = pExpress->m_strName;
		}
		m_strDescribe  = pExpress->m_strDescribe;
		m_strNotes     = pExpress->m_strNotes;
		m_strEidolon   = pExpress->m_strEidolon;
		
		m_strDefAxis     = pExpress->m_strDefAxis;
		m_wEstopPeriod   = pExpress->m_wEstopPeriod;
		m_wDefaultPeriod = pExpress->m_wDefaultPeriod;
		
		m_pExpDraw	     = NULL;
		m_pParamVar      = NULL;
		m_pDataSource = NULL;
		
		m_Arbore		 = NULL;
		m_nPozitie		 = 0;
		
		// �ڲ�������
		CMapVariabile* pRefParamVar = pExpress->GetParamVar();
		if( pRefParamVar != NULL )
		{
			CArray<class CValue*,class CValue*>* pAyrray = pRefParamVar->GetObjArray();
			CValue* pCurVal;	
			CString strName;
			if(pAyrray != NULL)
			{
				for(int nObj = 0; nObj < pAyrray->GetSize(); nObj++)
				{
					pCurVal = pAyrray->GetAt(nObj);
					strName = pRefParamVar->GetObjectName(pCurVal);
					if( !strName.IsEmpty() && pCurVal != NULL )
					{
						CNumericValue *pVal = new CNumericValue();
						pVal->Copy((CNumericValue*)pCurVal);
						AddParamer(strName,pVal);
					}
				}
			}
		}
		
		m_definitie = m_definitie + '\0' + '\0';
	}
	catch(...)
	{
	}
}

void CExpression::NewExpress(CExpression* pExpress,ExpressParam* pParam)
{
	if( pExpress == NULL ||
		pParam   == NULL )
		return;

	try
	{
		CExpression* pRefExpress = (CExpression*)pParam->pValue;
		if(pRefExpress == NULL)
			return;

		m_nExpressType = pRefExpress->GetExpressType();  // ����
		m_dStyle	   = pRefExpress->GetStyle();
		m_definitie	   = pRefExpress->GetSource();

		m_strPWD = "";

		m_strDescribe = "";
		m_strNotes    = "";
		m_strEidolon = "";
		
		m_strDefAxis     = "";
		m_wEstopPeriod   = 0;
		m_wDefaultPeriod = 0;
		
		m_pExpDraw	 = NULL;
		m_pParamVar  = NULL;
		m_pDataSource = NULL;
		
		m_Arbore   = NULL;
		m_nPozitie = 0;

		// ��Ӳ���
		CMapVariabile* pOldVar = pRefExpress->GetParamVar();
		CArray<class CValue*,class CValue*>* ayObj = NULL;
		if(pOldVar != NULL)
		{
			ayObj = pOldVar->GetObjArray();
		}
		if(ayObj != NULL)
		{
			CString strName;
			FormulaGeneralParam* ayVars = (FormulaGeneralParam*)pParam->pOther;
			if( ayVars != NULL )  // ��ǰ����
			{
				LPNODELEMENT pParam;
				for(int i = 0; i < ayObj->GetSize(); i++)
				{
					strName = pOldVar->GetObjectName(ayObj->GetAt(i));
					pParam = ayVars->GetParam(i);
					if(pParam != NULL)
					{
						LPNODELEMENT ppClone = clone( ayVars->GetParam(i) );
						if( ppClone != NULL )
						{
							CExpression* pExpresie = new CExpression(NULL,NULL,NULL);
							pExpresie->AddStyle(HX_EXPRESSREFERENCE);
							pExpresie->SetArbore(pExpress, ppClone );	
							CExpValue* pValue = new CExpValue;
							pValue->AddStyle(HX_EXPRESSREFERENCE);
							pValue->SetValue( pExpresie );
							AddParamer(strName,pValue);
						}
					}
					else
					{
						CNumericValue *pVal = new CNumericValue();
						pVal->Copy((CNumericValue*)ayObj->GetAt(i));
						AddParamer(strName,pVal);
					}

				}
			}
			else // ȱʡ����
			{
				CValue* pParam;
				for(int i = 0; i < ayObj->GetSize() ; i++)
				{
					pParam  = ayObj->GetAt(i);
					strName = pOldVar->GetObjectName(pParam);

					CNumericValue *pVal = new CNumericValue();
					pVal->Copy((CNumericValue*)pParam);
					AddParamer(strName,pVal);
				}
			}
		}
		//
		
		m_definitie = m_definitie + '\0' + '\0';

	}
	catch(...)
	{
	}
}

int	CExpression::SetSource(CString definitie,void* pData /*= NULL*/)
{
	m_definitie.Format("%s",definitie);

	return Compile();
}

int CExpression::UpdateArbore()
{
	if (m_definitie.IsEmpty())
		return 0;
	
	elibmem(m_Arbore); // Eliberarea memoriei ocupate de LPNODELEMENT
	m_Arbore = NULL;
	m_nPozitie  = 0;

	m_nIFWHILEStart = 0;
	//******************************������litao����*****************************************
	//�����ʽ�ַ�����WHILE��������W
	///CString OldExp="";
	///OldExp = m_definitie;
	///m_definitie = AddWHILE(m_definitie);

	//����ǰ�����ж������
	m_pCompNo.RemoveAll();
	//m_pExecNo.RemoveAll();
	//******************************������litao����*****************************************

	m_Arbore = LineOp();//OrlogicalOp();//logicalOp();//expresie();

	//******************************������litao����*****************************************
	//�ָ���ʽ�ַ���
	///m_nPozitie = OldExp.GetLength()-1;
	///m_definitie = OldExp;

	//������������ж������
	m_pCompNo.RemoveAll();
	//m_pExecNo.RemoveAll();
	//******************************������litao����*****************************************

	SkipSpaces();
	if ( m_definitie[m_nPozitie] != '\0' ) 
	{
		AddError( new nodErrorInfo(hxError_Identification,
				  m_nPozitie,m_definitie.GetLength(),
				  m_definitie.Mid(m_nPozitie,m_definitie.GetLength())) );

		elibmem(m_Arbore);
		m_Arbore = NULL;
	}
	
	if (m_Arbore == NULL)
	{
		AddError( new nodErrorInfo(hxError_AtLeastHasExp,
				  m_nPozitie,m_definitie.GetLength(),
				  m_definitie.Mid(m_nPozitie,m_definitie.GetLength())) );
		return m_nPozitie;
	}
	
	if(m_pExpDraw == NULL)
	{
		if( m_Arbore && m_Arbore->operatie != Operator_Semicolon )
		{
			AddExternDraw(m_Arbore);
		}
	}
	
	m_nPozitie = -1;
	return -1;	
}

LPNODELEMENT CExpression::expresie()
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod;
	LPNODELEMENT arb1 = termen();
	LPNODELEMENT arb2;
	if (arb1 == NULL) 
	{
		return NULL;  // In caz de eroare terminate
	}

	SkipSpaces();
	int nPos  = m_nPozitie;
	int nPos1 = m_nPozitie;
	int bAdd = 0,bSub = 0;
	while ( (bAdd = nPos  = IsAddSign(m_definitie[nPos],m_definitie,nPos)) ||
			(bSub = nPos1 = IsSubSign(m_definitie[nPos1],m_definitie,nPos1)) )
	//while ( ( m_definitie[m_nPozitie] == '-' ) || 
	//		( m_definitie[m_nPozitie] == '+' ) )
	{
		nod = new NODELEMENT;
		nod->left = arb1;
		
		nod->operatie = bAdd?'+':'-';
		m_nPozitie += (bAdd+bSub);

		arb2 = termen();
		nod->right = arb2;
		if( arb2 == NULL )
		{
			AddError( new nodErrorInfo(nod->operatie == '+'?hxError_AddRight:hxError_SubRight,
					  m_nPozitie,m_definitie.GetLength(),
					  nod->operatie == '+'?"+":"-") );
			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
		arb1 = nod;

		nPos  = m_nPozitie;
		nPos1 = m_nPozitie;
		bAdd = 0;
		bSub = 0;
	}
	return arb1;
}

LPNODELEMENT CExpression::termen()
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod;
	LPNODELEMENT arb1 = putere();
	LPNODELEMENT arb2;
	if ( arb1 == NULL )
		return NULL;  // In caz de eroare terminate

	SkipSpaces();
	int nPos  = m_nPozitie;
	int nPos1 = m_nPozitie;
	int bRide = 0,bDivide = 0;
	while ( (bRide   = nPos  = IsRideSign(m_definitie[nPos],m_definitie,nPos)) ||
			(bDivide = nPos1 = IsDivideSign(m_definitie[nPos1],m_definitie,nPos1)) )
	//while ( ( m_definitie[m_nPozitie] == '*' ) || 
	//		( m_definitie[m_nPozitie] == '/' ) ) 
	{
		nod = new NODELEMENT;
		nod->left = arb1;

		nod->operatie = bRide?'*':'/';
		m_nPozitie += (bRide+bDivide);

		arb2 = putere();
		nod->right = arb2;
		if  (arb2 == NULL) 
		{
			AddError( new nodErrorInfo(nod->operatie == '*'?hxError_RideRight:hxError_DivideRight,
					  m_nPozitie,m_definitie.GetLength(),
					  nod->operatie == '*'?"*":"/") );
			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
		arb1 = nod;

		nPos  = m_nPozitie;
		nPos1 = m_nPozitie;
		bRide = 0;
		bDivide = 0;
	}
	return arb1;
}

LPNODELEMENT CExpression::putere()
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod = NULL;
	LPNODELEMENT arb1 = sgOp();//logicalOp();
	LPNODELEMENT arb2;
	if  (arb1 == NULL) return NULL;  // In caz de eroare terminate

	SkipSpaces();
	while ( m_definitie[m_nPozitie] == '^') 
	{
		nod = new NODELEMENT;
		nod->left = arb1;
		
		nod->operatie = m_definitie[m_nPozitie];
		m_nPozitie ++;

		arb2 = sgOp();//logicalOp();
		nod->right = arb2;
		if  (arb2 == NULL) 
		{
			AddError( new nodErrorInfo(hxError_PowerRight,
					  m_nPozitie,m_definitie.GetLength(),
					  "^") );
			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
		arb1 = nod;
	}
	return arb1;
}

int CExpression::IsNegativeSign()
{
	int nPos1 = m_nPozitie;
	if ( (nPos1 = IsSubSign(m_definitie[nPos1],m_definitie,nPos1)) )
	{		
		if(m_nPozitie == 0)
		{
			return 1;
		}
		else
		{
			CString strKey;
			int nPos = m_nPozitie - 1;
			while( nPos >= 0 )
			{
				if(nPos >= 1)
				{
					strKey = m_definitie.Mid(nPos-1,2);
				}
				if( m_definitie[nPos] == '(' || m_definitie[nPos] == ','  || 
					m_definitie[nPos] == '&' || m_definitie[nPos] == '|'  || 
					m_definitie[nPos] == '!' || m_definitie[nPos] == '-'  ||
					m_definitie[nPos] == '=' || m_definitie[nPos] == '>'  ||
					m_definitie[nPos] == '<' ||
					(g_strPreNegativeSign.Find(strKey) != -1) )
				{
					return nPos1;
				}
				
				if( m_definitie[nPos] != ' ' )
					return 0;
				nPos --;
			}
		}
	}
	return 0;
}

LPNODELEMENT CExpression::factor()
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod = NULL,nod2 = NULL,left = NULL;

	int nPos = m_nPozitie;
	SkipSpaces();
	if ( (nPos = IsNegativeSign()) ) // ����
	{
		//m_definitie.Insert(m_nPozitie,"(0-1)*");
		//return expresie();

		nod  = new NODELEMENT;
		left = new NODELEMENT;
		left->right = NULL;
		left->left = NULL;
		left->operatie = '@';
		left->valoare = -1;
		nod->left = left;
		
		nod->operatie = '*';
		m_nPozitie += nPos;

		nod->right = expresie();//KommaOp();//OrlogicalOp();//expresie();
		if (nod->right == NULL) 
		{
			return NULL;
		}
		return nod;
		
	}
	nPos = m_nPozitie;
	if ( nPos = IsBracketLeft(m_definitie[nPos],m_definitie,nPos) /*m_definitie[m_nPozitie] == '('*/ ) // ����
    {
		m_nPozitie += nPos;
		nod = KommaOp();//OrlogicalOp();//expresie();
		if (nod == NULL) 
			return NULL;

		nPos = m_nPozitie;
		if ( !(nPos = IsBracketRight(m_definitie[nPos],m_definitie,nPos)) /*m_definitie[m_nPozitie] != ')'*/ )
		{
			AddError( new nodErrorInfo(hxError_BracketRight,
					  m_nPozitie,m_definitie.GetLength(),
					  "(") );
			elibmem(nod);
			return NULL;
		}
		m_nPozitie += nPos;
		return nod;
    }
	nPos = m_nPozitie;
	if ( nPos = IsOr(m_definitie[nPos],m_definitie,nPos) )
			 /*m_definitie[m_nPozitie] == '|' && 
			  m_definitie[m_nPozitie+1] != '|'*/ // ����ֵ
	{
		int nPos1 = m_nPozitie + nPos;
		if( !(nPos1 = IsOr(m_definitie[nPos1],m_definitie,nPos1)) )
		{	
			m_nPozitie += nPos;
			nod2 = KommaOp();
			if (nod2 == NULL) 
				return NULL;

			nPos = m_nPozitie;
			if ( !(nPos = IsOr(m_definitie[nPos],m_definitie,nPos)) /*m_definitie[m_nPozitie] != '|'*/ )
			{
				AddError( new nodErrorInfo(hxError_AbsRight,
					m_nPozitie,m_definitie.GetLength(),
					"|") );
				elibmem(nod);
				return NULL;
			}
			nod = new NODELEMENT;
			nod->left = nod2;
			nod->right = NULL;

			nod->operatie = '|';
			m_nPozitie += nPos;
			return nod;
		}
	}
	
	return identificator();	
}

LPNODELEMENT CExpression::identificator()
{
	if( IsStop() ) return NULL;

	int poz;
	LPNODELEMENT nod = NULL,nod2 = NULL;
	LPNODELEMENT result = NULL;
	poz = m_nPozitie;

	SkipSpaces();
	if ( m_definitie[m_nPozitie] == '\0' )
		result  = NULL;

	int nInvertedComma = 0;
	int nText = 0;
	int nPos = m_nPozitie;
	nPos = IsChineseNumber(m_definitie[nPos],m_definitie,nPos);
	// ������(���Ļ�Ӣ������)
	if (nPos || (HSisdigit(m_definitie[m_nPozitie]) && !((BYTE)m_definitie[m_nPozitie] > 0x7f)) )	// ��ֵ
	{
		while ( ((nPos == 1 || HSisdigit(m_definitie[m_nPozitie])) && !((BYTE)m_definitie[m_nPozitie] > 0x7f)) || 
				  nPos == 2 )
		{
			if( nPos )
				m_nPozitie += nPos;
			else
				m_nPozitie ++;

			nPos = m_nPozitie;
			nPos = IsChineseNumber(m_definitie[nPos],m_definitie,nPos);
		}

		/*
		while ( (HSisdigit(m_definitie[m_nPozitie]) || 
				 (m_definitie[m_nPozitie] == '.' )) && 
				!((BYTE)m_definitie[m_nPozitie] > 0x7f) )
		{
			m_nPozitie++;
		}
		*/

		CString strID = m_definitie.Mid(poz,m_nPozitie - poz);
		strID = ReplaceNumber(strID);

		SkipSpaces();
		if( IsColon(m_definitie[m_nPozitie],m_definitie,m_nPozitie) /*(m_definitie[m_nPozitie] == ':')*/ )
		{
			CString strPrompt;
			strPrompt.Format("\" %s \"Ϊ�Ƿ��ַ�! (\" %s \"ǰ���������ĸ)",strID,strID);
			AddError(new nodErrorInfo(hxError_Fun_param,poz,m_nPozitie,strPrompt));
			return NULL;
		}

		nod = new NODELEMENT;
		nod->left  = NULL; 
		nod->right = NULL;

		nod->operatie = '@';
		nod->valoare = atof(strID);
		result = nod;
	}
	// ��ĸ���ַ��Ż�����(��������)�����������ִ�
	else if ( HSIsalpha(m_definitie[m_nPozitie]) ||
		      IsChinese(m_definitie[m_nPozitie],m_definitie,m_nPozitie) ||
		      ( nInvertedComma = IsInvertedComma(m_definitie[m_nPozitie],m_definitie,m_nPozitie/*nInvertedComma*/)) ||
			  ( !nInvertedComma && (nText = IsText(m_definitie[m_nPozitie],m_definitie,m_nPozitie/*nText*/)) ) )
	{
		if( !nInvertedComma && !nText ) // ����
		{
			nInvertedComma = IsInvertedComma(m_definitie[m_nPozitie],m_definitie,m_nPozitie);
		}
		if( !nInvertedComma && !nText ) // �ı�?
		{
			nText = IsText(m_definitie[m_nPozitie],m_definitie,m_nPozitie);
		}
		if(nInvertedComma == 2)
		{
			m_nPozitie--;
		}
		if(nText == 2)
		{
			m_nPozitie--;
		}

		CString id,strRaw,strSub,strPeriod;

		//******************************������litao����*****************************************
		int nHaveID;   //��ǰ������Ƿ�ID  1���ǣ�0����
		//******************************������litao����*****************************************

		int   nEnd;
		int   nGetType = -1;
		int   nExeType = none;
		if( nInvertedComma ) // " ���ò�����ʼ
		{
			m_nPozitie += nInvertedComma;
			poz += nInvertedComma;
			
			int nPos1,nPos2;
			CArray<InExternal*,InExternal*>* ayStr = new CArray<InExternal*,InExternal*>;
			while ( (m_definitie[m_nPozitie] != '\0') &&
				    !(nInvertedComma = IsInvertedComma(m_definitie[m_nPozitie],m_definitie,m_nPozitie)) )
			{
				nPos = IsChinese(m_definitie[m_nPozitie],m_definitie,m_nPozitie,TRUE);
				if( (nPos = IsDot(m_definitie[m_nPozitie], m_definitie, m_nPozitie))  || // .
					(nPos1 = Is$(m_definitie[m_nPozitie], m_definitie, m_nPozitie))   || // $
					(nPos2 = IsWell(m_definitie[m_nPozitie], m_definitie, m_nPozitie)) ) // #
				{
					if(nPos)
					{
						ayStr->Add( new InExternal( m_definitie.Mid(poz,m_nPozitie - poz),'.') );
					}
					else if(nPos1)
					{
						ayStr->Add( new InExternal( m_definitie.Mid(poz,m_nPozitie - poz),'$') );
					}
					else if(nPos2)
					{
						ayStr->Add( new InExternal( m_definitie.Mid(poz,m_nPozitie - poz),'#') );
					}
					poz = m_nPozitie+1;
				}
				else
				{
					if( nPos == 0 )
					{
						nPos = 1;
					}
				}
				m_nPozitie ++;
			}
			if(nInvertedComma == 2)
			{
				m_nPozitie--;
			}
			if( poz != m_nPozitie )
			{
				ayStr->Add( new InExternal( m_definitie.Mid(poz,m_nPozitie - poz),-1) );
			}
			nEnd = m_nPozitie;

			   // end
			nInvertedComma = IsInvertedComma(m_definitie[m_nPozitie],m_definitie,m_nPozitie);
			if( nInvertedComma == 2 )
			{
				m_nPozitie--;
			}
			nEnd	   += nInvertedComma;
			m_nPozitie += nInvertedComma;

			int i;
			if( !nInvertedComma )
			{
				AddError( new nodErrorInfo(hxError_WantInvertedComma,poz,nEnd,strRaw) );
				for( i = 0; i < ayStr->GetSize(); i++)
				{
					delete ayStr->GetAt(i);
				}
				delete ayStr;
				return NULL;
			}

			InExternal* pInExternal;
			int nCount = ayStr->GetSize();
			id.Empty();
			nGetType = ExpExternExp;
			CString strTemp;
			//
			for( i = 0; i < nCount; i++ )
			{
				pInExternal = ayStr->GetAt(i);
				if( pInExternal->IsKey('.') )
				{
					strTemp = pInExternal->GetName();
					if( !strTemp.Compare("TECH") )          // ָ��
					{
						nExeType = Tech;
					}
					else if( !strTemp.Compare("SYSTEM") )	// ����
					{
						nExeType = Exchange;
					}
					else if( !strTemp.Compare("EXPLORER") ) // ѡ��
					{
						nExeType = Condition;
					}
					else if( !strTemp.Compare("KCOLOR") )   // ���K��
					{
						nExeType = MoreKLine;
					}
					else if( !strTemp.Compare("FUN") )      // ����
					{
						nGetType = -2;
						nExeType = -1;
					}
					else									// ָ��
					{
						if( id.IsEmpty() )
						{
							id = strTemp;
						}
						else
						{
							strSub = strTemp;
						}
					}
				}
				else if( pInExternal->IsKey('$') )     // ����,��: 0001$Close ...
				{
					nGetType  = -2;
					strPeriod = pInExternal->GetName();
				}
				else if( pInExternal->IsKey('#') )	   // ����������,��: macd#week ...
				{
					nGetType = -3;
					if( id.IsEmpty() )
					{
						id = pInExternal->GetName();
					}
					else
					{
						strSub = pInExternal->GetName();
					}
				}
				else
				{
					if( nGetType == -3 )
					{
						strPeriod = pInExternal->GetName();
					}
					else
					{
						if( id.IsEmpty() )
						{
							id = pInExternal->GetName();
						}
						else if( strSub.IsEmpty() )
						{
							strSub = pInExternal->GetName();
						}
						else
						{
							strPeriod = pInExternal->GetName();
						}
					}
				}
			}

			for( i = 0; i < ayStr->GetSize(); i++)
			{
				delete ayStr->GetAt(i);
			}
			delete ayStr;

			if( nGetType == -3 )
			{
				if( !IsPeriod(strPeriod) )
				{
					AddError( new nodErrorInfo(hxError_unknowPeriod,poz,nEnd,strPeriod) );
					return NULL;
				}
			}
		}
		else if( nText ) // �ı�
		{
			m_nPozitie += nText;
			poz += nText;

			while ( (m_definitie[m_nPozitie] != '\0') &&
				    !(nText = IsText(m_definitie[m_nPozitie],m_definitie,m_nPozitie)) )
			{
				m_nPozitie ++;
			}
			if(nText == 2)
			{
				m_nPozitie--;
			}
			nEnd = m_nPozitie;

			id = m_definitie.Mid(poz,m_nPozitie - poz);

			   // end
			nText = IsText(m_definitie[m_nPozitie],m_definitie,m_nPozitie);
			if(nText == 2)
			{
				m_nPozitie--;
			}
			nEnd	   += nText;
			m_nPozitie += nText;

			if( !nText )
			{
				return NULL;
			}
			nGetType = -4;
		}
		else  // ����
		{
			while ( (m_definitie[m_nPozitie] != '\0') && 
				    ((nPos = IsChinese(m_definitie[m_nPozitie],m_definitie,m_nPozitie,TRUE)) ||
					 isalnum(m_definitie[m_nPozitie])) )
			{
				if( nPos == 2 )
					break;
				m_nPozitie++;			
			}
			
			nEnd   = m_nPozitie;
			id     = m_definitie.Mid(poz,m_nPozitie - poz);
			strRaw = id;
			id.MakeUpper();
			
			id = ReplaceChar(id);	
			
			if( !id.CompareNoCase("and") || !id.CompareNoCase("����") ||
				!id.CompareNoCase("or")  || !id.CompareNoCase("��") )
			{
				m_nPozitie = poz;
				return NULL;
			}
			
			SkipSpaces();
			nPos = m_nPozitie;
			if( (nPos = IsColon(m_definitie[nPos],m_definitie,nPos)) ) // :
			{
				m_nPozitie += nPos;
				
				SkipSpaces();
				int nPos1 = m_nPozitie;
				if ( !(nPos1 = IsEqual(m_definitie[nPos1],m_definitie,nPos1)) /*m_definitie[m_nPozitie] != '='*/ )  // :ð�Ż�ͼ����
				{
					nod = new NODELEMENT;
					nod -> left  = NULL;
					nod -> right = NULL;
					
					//******************************������litao����*****************************************
					CString tmpID = AddObjNo(id,COMPILE_NO);

					nod -> operatie    = Operator_Colon;
					nod -> cDataType   = DataTypeOperStr;
					//nod -> valoarestr  = new CString(strRaw);
					nod -> valoarestr  = new CString(tmpID);
					m_strLineObj = id;
					//******************************������litao����*****************************************

					SkipSpaces();
					
					return nod;
				}
				else   // :=��ֵ����
				{
					nod = new NODELEMENT;
					nod -> left = NULL;
					nod -> right = NULL;
					
					//******************************������litao����*****************************************
					CString tmpID = AddObjNo(id,COMPILE_NO);

					nod -> operatie   = Operator_Evaluate;
					nod -> cDataType  = DataTypeOperStr;
					//nod -> valoarestr = new CString(strRaw);
					nod -> valoarestr  = new CString(tmpID);
					m_strLineObj = id;
					//******************************������litao����*****************************************

					m_nPozitie += nPos1;
					SkipSpaces();
					
					return nod;
				}
			}
			
			//
			nod2 = IsNumFun(id); // ��ѧ
			if(nod2 != NULL)
				return nod2;
		}

		short nRet = 0;
		CValue* valoare = NULL;

		//******************************������litao����*****************************************
		//ͬһ�е��ظ�������ͬID���Ҹ�ID�Ǹ�ֵʱ����Ҫȡǰһ����ŵı�ʾ
		if (id == m_strLineObj)
			nHaveID = 1;
		else
			nHaveID = 0;

		if( nGetType == -2 || nExeType == -1 )
		{
			if( nExeType == -1 || nExeType == none )
			{
				valoare = GetVariable(id,nRet,-1);
			}
			else
			{
				valoare = GetVariable(id,nRet,ExpExternExp,nHaveID);
			}
		}
		else if( nGetType == -3 )
		{
			valoare = GetVariable(id,nRet,ExpExternExp,nHaveID);
		}
		else if( nGetType == -4 )
		{
		}
		else
		{
			if( nGetType == ExpExternExp )
			{
				if( nExeType == none )
				{
					nGetType = Tech;
				}
				else
				{
					nGetType = nExeType;
				}
			}
			valoare = GetVariable(id,nRet,nGetType,nHaveID);
		}
		//******************************������litao�޸�*****************************************

		if( valoare != NULL && nRet )
		{
			switch(nRet)
			{
			case ExpExternFun:  // ����
				{
					//******************************������litao����*****************************************
					//������Ϊѭ�����߷�֧�߼����
					if (0 == id.CompareNoCase("WHILE")) \
					{			
						nod = whiledo(valoare,nEnd);

						return nod;
					}
					else 
					{
						//if (0 == id.CompareNoCase("IF")) 
						//{
						//	return NULL;
						//}
						//else
						{
							//******************************������litao����*****************************************

							nod2 = factor();

							// ������
							FunctionParam* pFunctionParam = new FunctionParam();
							if(pFunctionParam == NULL) return NULL;

							CFunExpresion* pFun = (CFunExpresion*)valoare;
							pFunctionParam->pFun = pFun;

							LPNODELEMENT pLTree = NULL;
							if( nod2 != NULL )
							{			
								if( pFun->IsStyle(HX_UNKNOWFUNPARAM) )
								{
									GetDetailVariable(nod2,pFunctionParam->ayVars);
								}
								else if( pFun->GetParams() > 1 )
								{
									GetDetailVariable(nod2,pFunctionParam->ayVars);
								}
								else
								{
									pFunctionParam->ayVars->Add(nod2);
								}
							}
							if( !IsValidateParam(pFun,nod2,pFunctionParam->ayVars) )
							{
								CString strPrompt;
								strPrompt.Format("\" %s \"������������! %s",id,pFun->GetNotes() );						
								AddError(new nodErrorInfo(hxError_Fun_param,poz,nEnd,strPrompt));

								if(pFunctionParam)
								{
									delete pFunctionParam;
								}
								return NULL;
							}

							nod = new NODELEMENT;
							nod->left  = nod2;

							if( nGetType == -2 ) // �����ⲿ��Ʊ������
							{
								InportDataParam* pInportData = new InportDataParam();

								AskData ask;
								memset(&ask,0,sizeof(ask));

								ask.m_nIndex = 0;
								//ask.m_nType  = RT_TECHDATA | PERIOD_TYPE_DAY;
#ifdef SUPPORT_NETVALUE
								ask.m_nType  = RT_TECHDATA_EX | PERIOD_TYPE_DAY;
#else
								ask.m_nType  = RT_TECHDATA | PERIOD_TYPE_DAY;
#endif
								ask.m_nSize  = 0;

								ask.m_pCode[0].m_cCodeType = 0;
								strncpy(ask.m_pCode[0].m_cCode,strPeriod,sizeof(ask.m_pCode[0].m_cCode));
								pInportData->m_pDataSrc	= new CHSDataSource( &ask );

								LPNODELEMENT nodData = new NODELEMENT;
								nodData->left  = NULL;
								nodData->right = NULL;
								nodData->pData = pFunctionParam; // ����ָ��

								nodData->operatie   = Operator_Function;
								nodData->cDataType  = DataTypeOperStr | DataFunParam;
								nodData->valoarestr = new CString(id);

								nod->right = nodData;

								nod->pData      = pInportData; // ���ݶ���ָ��
								nod->operatie   = Operator_InportData;
								nod->cDataType  = DataTypeOperStr | DataInport;
								nod->valoarestr = new CString(strPeriod);
							}
							else
							{
								nod->right = NULL;
								nod->pData = pFunctionParam; // ����ָ��
								nod->operatie   = Operator_Function;
								nod->cDataType  = DataTypeOperStr | DataFunParam;
								nod->valoarestr = new CString(id);
							}

							SkipSpaces();
							return nod;			
						}
					}
				}
				break;
			case ExpExternExp: // ���ù�ʽ
				{
					CExpValue* pValue		 = (CExpValue*)valoare;
					CExpression* pExpression = pValue->GetExpByCompile();

					if( pExpression == NULL )
					{
						CString strPrompt;
						strPrompt.Format("��ʽ\" %s \"���д���,���嵽\" %s \"�鿴! ",id,id );						
						AddError(new nodErrorInfo(hxError_Exp_param,poz,nEnd,strPrompt));
						return NULL;
					}

					if( !strSub.IsEmpty() )
					{
						CValue*	pDraw = pExpression->GetDrawExpress(strSub);
						if(pDraw == NULL)
						{
							CString strPrompt;
							strPrompt.Format("��ʽ\" %s \"����\" %s \"����! %s",id,strSub,pExpression->GetNotes() );						
							AddError(new nodErrorInfo(hxError_Exp_param,poz,nEnd,strPrompt));
							return NULL;
						}
					}

					nod2 = factor();

					FormulaGeneralParam* ayVars = NULL;
					ExpressParam* pFunctionParam = new ExpressParam();

					if( nod2 != NULL )
					{
						// �õ�����
						ayVars = new FormulaGeneralParam;
						if( nod2 != NULL && pExpression->GetParamVarCount() > 0 )
						{						
							GetDetailVariable(nod2,ayVars);
						}
#if 0
						if( !IsValidateParam(pExpression,nod2,ayVars) )
						{
							CString strPrompt;
							strPrompt.Format("\" %s \"���ò�������! %s",strRaw,pExpression->GetNotes() );						
							AddError(new nodErrorInfo(hxError_Exp_param,poz,nEnd,strPrompt));
							
							if(ayVars == NULL)
							{
								delete ayVars;
							}
							if(pFunctionParam == NULL)
							{
								delete pFunctionParam;
							}
							return NULL;
						}
						
						if(pFunctionParam == NULL)
						{
							elibmem(nod2);
							
							CString strPrompt;
							strPrompt.Format( " �޿����ڴ����ù�ʽ! " );
							AddError(new nodErrorInfo(hxError_Exp_param,poz,nEnd,strPrompt));
							return NULL;
						}
#endif
					}

					pFunctionParam->pValue = pExpression;
					pFunctionParam->strSub = strSub;
					pFunctionParam->pOther = (void*)ayVars;

					//
					CExpValue* pNewValue = new CExpValue;
					CExpression* pNewExpression = new CExpression(NULL,NULL,NULL);
					pNewValue->SetValue(pNewExpression);
					pNewExpression->SetName(id);
					pNewExpression->NewExpress(this,pFunctionParam);
					
					pFunctionParam->pValue = pNewValue;
					pFunctionParam->strSub = strSub;
					pFunctionParam->pOther = NULL;
					delete ayVars;

					if( pNewValue->GetExpByCompile() == NULL ) // ����
					{
						elibmem(nod2);
						delete pFunctionParam;

						CString strPrompt;
						strPrompt.Format( " ��ʽ����ʧ��! " );
						AddError(new nodErrorInfo(hxError_Exp_param,poz,nEnd,strPrompt));
						return NULL;
					}

					nod = new NODELEMENT;
					nod->left  = nod2;

					if(nGetType == -2 || nGetType == -3)  // ����
					{
						InportDataParam* pInportData = new InportDataParam();
						AskData ask;
						memset(&ask,0,sizeof(ask));
						ask.m_pCode[0].m_cCodeType = 0;
						strncpy(ask.m_pCode[0].m_cCode,strPeriod,sizeof(ask.m_pCode[0].m_cCode));
						pInportData->m_pDataSrc		 = new CHSDataSource( &ask );
						
						LPNODELEMENT nodData = new NODELEMENT;
						nodData->left  = NULL;
						nodData->right = NULL;
						nodData->pData = pInportData; // ���ݶ���ָ��
						
						nodData->operatie   = Operator_InportData;
						nodData->cDataType  = DataTypeOperStr | DataInport;
						nodData->valoarestr = new CString(strPeriod);

						nod->right = nodData;
					}
					else
					{
						nod->right = NULL;
					}
					nod->pData = pFunctionParam; // ���ù�ʽָ��
					
					nod->operatie   = Operator_Express;
					nod->cDataType  = DataTypeOperStr | DataExpParam;
					nod->valoarestr = new CString(id);

					SkipSpaces();

					return nod;
				}
				break;
			default:
				{
					nod = new NODELEMENT;
					nod -> left  = NULL;
					nod -> right = NULL;

					nod -> operatie   = '`';
					nod -> cDataType  = DataTypeOperStr;
					nod -> valoarestr = new CString(id);
					result = nod;
				}
				break;
			}
		}
		else
		{
			if( nGetType == -4 ) // �ı�
			{
				nod = new NODELEMENT;
				nod -> left  = NULL;
				nod -> right = NULL;
				
				nod -> operatie   = '`';
				nod -> cDataType  = DataTypeOperStr|DataText;
				nod -> valoarestr = new CString(id);
				result = nod;
			}
			else
			{
				AddError( new nodErrorInfo(hxError_NotVariable,poz,nEnd,id) );
				result = NULL;
			}
		}
	}

	SkipSpaces();

	return result;
	
}

LPNODELEMENT CExpression::IsNumFun(CString id)
{
	LPNODELEMENT nod,nod2 = NULL;
	if (id == "SIN")		// ���Һ���
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_SIN;
		SkipSpaces();
		return nod;
	}
	if (id == "COS")		// ����
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_COS;
		SkipSpaces();
		return nod;
	}
	if (id == "EXP")		// ָ��
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_EXP;
		SkipSpaces();
		return nod;
	}
	if (id == "SQRT")		// ƽ����
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_SQRT;
		SkipSpaces();
		return  nod;
	}
	if (id == "LOGX")		// ����
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_LOG;
		SkipSpaces();
		return nod;
	}
	if (id == "TG")		// ����
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_TG;
		SkipSpaces();
		return nod;
	}
	if (id == "CTG")		// ����
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_CTG;
		SkipSpaces();
		return nod;
	}
	if (id == "ASIN")		// �����Һ���
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_ASIN;
		SkipSpaces();
		return nod;
	}
	if (id == "ACOS")		// ������
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_ACOS;
		SkipSpaces();
		return nod;
	}
	if (id == "ATG" || id == "ATAN")		// ������
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_ATG;
		SkipSpaces();
		return nod;
	}
	return NULL;
}

int CExpression::ChangeExpression(CString & expresie)
{
	DeleteError();  // ɾ����һ�δ���

	// ���봮
//	m_definitie = expresie;		
//	CString str(VK_TAB);
//	m_definitie = hxRelaceReturnKey(m_definitie,str);

	m_definitie = expresie + '\0' + '\0';

	UpdateArbore();  // ִ�б���
	
	return GetErrorCount();
}

int CExpression::Value(double& valoare,HSCurDataInfoStatus* pCurDataStatus)
{
	if (m_Arbore == NULL) 
		return -1;

	valoare = vexp(m_Arbore,pCurDataStatus);

	return 0;
}

double CExpression::Add(LPNODELEMENT a,
						HSCurDataInfoStatus* pCurDataStatus,
						CExpression* pExpression,
						double& v1,double& v2,
						HSCurDataInfoStatus& calc1,
						HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] + calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] + v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (v1 + pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}		
	if( v2 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( v1 + v2 );
}

// -
double CExpression::Sub(  LPNODELEMENT a,
				  HSCurDataInfoStatus* pCurDataStatus,
				  CExpression* pExpression,
				  double& v1,double& v2,
				  HSCurDataInfoStatus& calc1,
				  HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues &&
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] - calc2.m_pData[i]);						
				}
				/*else if( calc1.m_pData[i] != hx_DefMinValues )
				{
				pCurDataStatus->m_pData[i] = calc1.m_pData[i];
				}
				else if( calc2.m_pData[i] != hx_DefMinValues )
				{
				pCurDataStatus->m_pData[i] = calc2.m_pData[i];
				}*/
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] - v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (v1 - pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( v1 - v2 );
}

// *
double CExpression::Ride(  LPNODELEMENT a,
				   HSCurDataInfoStatus* pCurDataStatus,
				   CExpression* pExpression,
				   double& v1,double& v2,
				   HSCurDataInfoStatus& calc1,
				   HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] * calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] * v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (v1 * pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( v1 * v2 );
}

// /
double CExpression::Divide(  LPNODELEMENT a,
					 HSCurDataInfoStatus* pCurDataStatus,
					 CExpression* pExpression,
					 double& v1,double& v2,
					 HSCurDataInfoStatus& calc1,
					 HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != 0 )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] / calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues && v2 != 0)
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] / v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if( pData[i].m_dData != hx_DefMinValues && pData[i].m_dData != 0 )
						{
							pCurDataStatus->m_pData[i] = (v1 / pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}		
	if( v2 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if (v2 == 0)
	{
		if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
		{
			AddError(new nodErrorInfo(hxError_DIVISION_BY_0));
		}
		return hx_DefMinValues; // -v1/0.001;
	}
		
	return ( v1 / v2 );
}

// |
double CExpression::Or(  LPNODELEMENT a,
					 HSCurDataInfoStatus* pCurDataStatus,
					 CExpression* pExpression,
					 double& v1,double& v2,
					 HSCurDataInfoStatus& calc1,
					 HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = fabs(calc1.m_pData[i]);
				}
			}
		}
		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( fabs(v1) );
}

// ^
double CExpression::Pow(  LPNODELEMENT a,
					 HSCurDataInfoStatus* pCurDataStatus,
					 CExpression* pExpression,
					 double& v1,double& v2,
					 HSCurDataInfoStatus& calc1,
					 HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = pow(calc1.m_pData[i].m_dData,calc2.m_pData[i].m_dData);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = pow(pData[i],v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = pow(v1,pData[i].m_dData);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( pow( v1,v2 ) );
}

// <
double CExpression::LessThan(  LPNODELEMENT a,
					 HSCurDataInfoStatus* pCurDataStatus,
					 CExpression* pExpression,
					 double& v1,double& v2,
					 HSCurDataInfoStatus& calc1,
					 HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] < calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] < v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (v1 < pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return 0;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return 0;
	}
	return ( v1 < v2 );
}

// >
double CExpression::GreatLess(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] > calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] > v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (v1 > pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return 0;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return 0;
	}
	return ( v1 > v2 );
}

double CExpression::Equal(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] == calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] == v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (v1 == pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return 0;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return 0;
	}
	return ( v1 == v2 );
}

// !
double CExpression::Not(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->right,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = !calc1.m_pData[i];
				}
			}
		}
		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return 0;
	}
	return ( !v1 );
}

// sin
double CExpression::Sin(  LPNODELEMENT a,
				  HSCurDataInfoStatus* pCurDataStatus,
				  CExpression* pExpression,
				  double& v1,double& v2,
				  HSCurDataInfoStatus& calc1,
				  HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = sin(calc1.m_pData[i]);
				}
			}
		}
		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( sin( v1 ) );
}

// cos
double CExpression::Cos(  LPNODELEMENT a,
				  HSCurDataInfoStatus* pCurDataStatus,
				  CExpression* pExpression,
				  double& v1,double& v2,
				  HSCurDataInfoStatus& calc1,
				  HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = cos(calc1.m_pData[i]);
				}
			}
		}
		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( cos( v1 ) );
}

// exp
double CExpression::Exp(  LPNODELEMENT a,
				  HSCurDataInfoStatus* pCurDataStatus,
				  CExpression* pExpression,
				  double& v1,double& v2,
				  HSCurDataInfoStatus& calc1,
				  HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = exp(calc1.m_pData[i]);
				}
			}
		}
		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( exp( v1 ) );
}

// SQRT
double CExpression::Sqrt(  LPNODELEMENT a,
						 HSCurDataInfoStatus* pCurDataStatus,
						 CExpression* pExpression,
						 double& v1,double& v2,
						 HSCurDataInfoStatus& calc1,
						 HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && calc1.m_pData[i] >= 0)
				{
					pCurDataStatus->m_pData[i] = sqrt(calc1.m_pData[i]);
				}
			}
		}
		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}						
	if (v1 < 0)
	{
		if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
		{
			AddError(new nodErrorInfo(hxError_INVALID_DOMAIN));
		}
		return hx_DefMinValues;
	}
	
	return ( sqrt( v1 ) );
}

// log
double CExpression::Log(  LPNODELEMENT a,
						HSCurDataInfoStatus* pCurDataStatus,
						CExpression* pExpression,
						double& v1,double& v2,
						HSCurDataInfoStatus& calc1,
						HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && calc1.m_pData[i] > 0)
				{
					pCurDataStatus->m_pData[i] = log(calc1.m_pData[i]);
				}
			}
		}
		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if (v1 <= 0)
	{
		if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
		{
			AddError(new nodErrorInfo(hxError_INVALID_DOMAIN));
		}
		return hx_DefMinValues;
	}
	else 
	{
		return ( log( v1 ) );
	}
}

// tg
double CExpression::Tg(  LPNODELEMENT a,
					   HSCurDataInfoStatus* pCurDataStatus,
					   CExpression* pExpression,
					   double& v1,double& v2,
					   HSCurDataInfoStatus& calc1,
					   HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && calc1.m_pData[i] > 0)
				{
					pCurDataStatus->m_pData[i] = tan(calc1.m_pData[i]);
				}
			}
		}
		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( tan(v1) );
}

// ctg
double CExpression::Ctg(  LPNODELEMENT a,
						HSCurDataInfoStatus* pCurDataStatus,
						CExpression* pExpression,
						double& v1,double& v2,
						HSCurDataInfoStatus& calc1,
						HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && calc1.m_pData[i] > 0)
				{
					pCurDataStatus->m_pData[i] = 1 / tan(calc1.m_pData[i]);
				}
			}
		}
		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( 1 / tan(v1) );
}

//ASIN
double CExpression::Asin(  LPNODELEMENT a,
						 HSCurDataInfoStatus* pCurDataStatus,
						 CExpression* pExpression,
						 double& v1,double& v2,
						 HSCurDataInfoStatus& calc1,
						 HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && calc1.m_pData[i] > 0)
				{
					pCurDataStatus->m_pData[i] = asin(calc1.m_pData[i]);
				}
			}
		}
		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( asin(v1) );
}
// ACOS
double CExpression::Acos(  LPNODELEMENT a,
						 HSCurDataInfoStatus* pCurDataStatus,
						 CExpression* pExpression,
						 double& v1,double& v2,
						 HSCurDataInfoStatus& calc1,
						 HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && calc1.m_pData[i] > 0)
				{
					pCurDataStatus->m_pData[i] = acos(calc1.m_pData[i]);
				}
			}
		}
		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( acos(v1) );
}

// ATG
double CExpression::ATG(  LPNODELEMENT a,
						HSCurDataInfoStatus* pCurDataStatus,
						CExpression* pExpression,
						double& v1,double& v2,
						HSCurDataInfoStatus& calc1,
						HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	if( calc1.m_pData != NULL )
	{
		int i;
		pCurDataStatus->Alloc(calc1.m_lSize);
		if( calc1.m_pData != NULL )
		{
			for(i = 0; i < calc1.m_lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && calc1.m_pData[i] > 0)
				{
					pCurDataStatus->m_pData[i] = atan(calc1.m_pData[i]);
				}
			}
		}
		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	return ( atan(v1) );
}

// LargerEqual
double CExpression::LargerEqual(  LPNODELEMENT a,
								HSCurDataInfoStatus* pCurDataStatus,
								CExpression* pExpression,
								double& v1,double& v2,
								HSCurDataInfoStatus& calc1,
								HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] >= calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] >= v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (v1 >= pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return 0;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return 0;
	}
	return ( v1 >= v2 );
}

// SmallEqual
double CExpression::SmallEqual(  LPNODELEMENT a,
								HSCurDataInfoStatus* pCurDataStatus,
								CExpression* pExpression,
								double& v1,double& v2,
								HSCurDataInfoStatus& calc1,
								HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] <= calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] <= v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (v1 <= pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return 0;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return 0;
	}
	return ( v1 <= v2 );
}
// NotEqual
double CExpression::NotEqual(  LPNODELEMENT a,
							 HSCurDataInfoStatus* pCurDataStatus,
							 CExpression* pExpression,
							 double& v1,double& v2,
							 HSCurDataInfoStatus& calc1,
							 HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] != calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] != v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (v1 != pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return 0;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return 0;
	}
	return ( v1 != v2 );
}
// ||
double CExpression::DoubleOR(  LPNODELEMENT a,
				  HSCurDataInfoStatus* pCurDataStatus,
				  CExpression* pExpression,
				  double& v1,double& v2,
				  HSCurDataInfoStatus& calc1,
				  HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] || calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] || v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (v1 || pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return 0;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return 0;
	}
	return ( v1 || v2 );
}

// AND &&
double CExpression::AND(  LPNODELEMENT a,
						HSCurDataInfoStatus* pCurDataStatus,
						CExpression* pExpression,
						double& v1,double& v2,
						HSCurDataInfoStatus& calc1,
						HSCurDataInfoStatus& calc2)
{
	v1 = pExpression->vexp(a->left,&calc1);
	v2 = pExpression->vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc1.m_pData[i] && calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (pData[i] && v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = (v1 && pData[i]);
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return 0;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return 0;
	}
	return ( v1 && v2 );
}

// Semicolon ;
double CExpression::Semicolon(  LPNODELEMENT a,
							  HSCurDataInfoStatus* pCurDataStatus,
							  CExpression* pExpression,
							  double& v1,double& v2,
							  HSCurDataInfoStatus& calc1,
							  HSCurDataInfoStatus& calc2)
{
	//return (vexp(a->left,pCurDataStatus) && vexp(a->right,pCurDataStatus));
	return 0;
}

// Error
double CExpression::Error(  LPNODELEMENT a,
						  HSCurDataInfoStatus* pCurDataStatus,
						  CExpression* pExpression,
						  double& v1,double& v2,
						  HSCurDataInfoStatus& calc1,
						  HSCurDataInfoStatus& calc2)
{
	if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
	{
		AddError(new nodErrorInfo(hxError_Operator));
	}
	return 0;
}

// Komma ,
double CExpression::Komma(  LPNODELEMENT a,
						  HSCurDataInfoStatus* pCurDataStatus,
						  CExpression* pExpression,
						  double& v1,double& v2,
						  HSCurDataInfoStatus& calc1,
						  HSCurDataInfoStatus& calc2)
{
	return pExpression->ExeKomma(a,pCurDataStatus,v1,v2,calc1,calc2);
}

// Colon
double CExpression::Colon(  LPNODELEMENT a,
						  HSCurDataInfoStatus* pCurDataStatus,
						  CExpression* pExpression,
						  double& v1,double& v2,
						  HSCurDataInfoStatus& calc1,
						  HSCurDataInfoStatus& calc2)
{
	return pExpression->ExeColon(a,pCurDataStatus);
}

// Evaluate
double CExpression::Evaluate(  LPNODELEMENT a,
							 HSCurDataInfoStatus* pCurDataStatus,
							 CExpression* pExpression,
							 double& v1,double& v2,
							 HSCurDataInfoStatus& calc1,
							 HSCurDataInfoStatus& calc2)
{
	return pExpression->ExeEvaluate( a,pCurDataStatus );
}

// Function
double CExpression::Function(  LPNODELEMENT a,
							 HSCurDataInfoStatus* pCurDataStatus,
							 CExpression* pExpression,
							 double& v1,double& v2,
							 HSCurDataInfoStatus& calc1,
							 HSCurDataInfoStatus& calc2)
{
	return pExpression->ExeFunction( a ,pCurDataStatus);
}

//Express
double CExpression::Express(  LPNODELEMENT a,
							HSCurDataInfoStatus* pCurDataStatus,
							CExpression* pExpression,
							double& v1,double& v2,
							HSCurDataInfoStatus& calc1,
							HSCurDataInfoStatus& calc2)
{
	return pExpression->ExeExpress( a ,pCurDataStatus);
}

// InportData
double CExpression::InportData(  LPNODELEMENT a,
							   HSCurDataInfoStatus* pCurDataStatus,
							   CExpression* pExpression,
							   double& v1,double& v2,
							   HSCurDataInfoStatus& calc1,
							   HSCurDataInfoStatus& calc2)
{
	return pExpression->ExeInport( a ,pCurDataStatus);
}

// Default
double CExpression::Default(  LPNODELEMENT a,
							HSCurDataInfoStatus* pCurDataStatus,
							CExpression* pExpression,
							double& v1,double& v2,
							HSCurDataInfoStatus& calc1,
							HSCurDataInfoStatus& calc2)
{
	return pExpression->ExeDefault( a ,pCurDataStatus );
}

// ReturnValue '@'
double CExpression::ReturnValue(  LPNODELEMENT a,
								HSCurDataInfoStatus* pCurDataStatus,
								CExpression* pExpression,
								double& v1,double& v2,
								HSCurDataInfoStatus& calc1,
								HSCurDataInfoStatus& calc2)
{
	return (a->valoare);
}

// ReturnString '`'
double CExpression::ReturnString(  LPNODELEMENT a,
								HSCurDataInfoStatus* pCurDataStatus,
								CExpression* pExpression,
								double& v1,double& v2,
								HSCurDataInfoStatus& calc1,
								HSCurDataInfoStatus& calc2)
{
	//if( (a -> cDataType == (DataTypeOperStr|DataText)) && a->valoarestr )
	//{
	//	return (double)(long)a->valoarestr; // ����ָ�룬����ע��ʹ�á�
	//}

	return hx_DefMinValues;
}

double CExpression::vexp( LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus )
{
	try
	{
		//if( !pCurDataStatus )
		//{
		//	AddError(new nodErrorInfo(hxError_Invalid));
		//	return 0;
		//}

		if( a == NULL )
		{
			if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
			{
				AddError(new nodErrorInfo(hxError_Invalid));
			}
			return hx_DefMinValues; // return 0;
		}
		if ( a->operatie == NULL) 
		{
			if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
			{
				AddError(new nodErrorInfo(hxError_Invalid));
			}
			return hx_DefMinValues; // return 0;
		}

		double v1 = hx_DefMinValues;
		double v2 = hx_DefMinValues;

		HSCurDataInfoStatus calc1(pCurDataStatus);
		HSCurDataInfoStatus calc2(pCurDataStatus);
/*
		if( pCurDataStatus != NULL )
		{
			calc1.m_pRefCalcValues.m_pValue = pCurDataStatus->m_pRefCalcValues.m_pValue;
			calc1.m_pRefCalcValues.m_lStart = pCurDataStatus->m_pRefCalcValues.m_lStart;
			calc1.m_pRefCalcValues.m_lSize  = pCurDataStatus->m_pRefCalcValues.m_lSize;

			calc2.m_pRefCalcValues.m_pValue = pCurDataStatus->m_pRefCalcValues.m_pValue;
			calc2.m_pRefCalcValues.m_lStart = pCurDataStatus->m_pRefCalcValues.m_lStart;
			calc2.m_pRefCalcValues.m_lSize  = pCurDataStatus->m_pRefCalcValues.m_lSize;
		}
*/
		// find by function list 
		fnHSExpress pFun = GetExpressFun(a->operatie);
		if( pFun != NULL )
		{
			return (*pFun)(a,pCurDataStatus,this,v1,v2,calc1,calc2);
		}
		else
		{
			return CExpression::Default(a,pCurDataStatus,this,v1,v2,calc1,calc2);
		}
/*
		switch(a->operatie) 
		{
		case '@' :  // ������ֵ
		case Operator_SIN :
		case Operator_COS : 
		case Operator_EXP : 
		case Operator_SQRT : 
		case Operator_LOG : 
		case Operator_TG :
		case Operator_CTG :
		case Operator_ASIN :
		case Operator_ACOS :
		case Operator_ATG :
		case Operator_LargerEqual: // >=
		case Operator_SmallEqual:  // <=
		case Operator_NotEqual:    // != <>
		case Operator_OR:          // || or
		case Operator_AND:		   // && and
		case Operator_Semicolon:   // ; �ֺ�
		case Operator_Error:       // ����������
		case Operator_Komma:      // ,���Ų���(һ�п��ж�����䣬�����ŷֽ�)
		case Operator_Colon:      // ��ͼ����(�����������ڴ����ϻ�ͼ�ı��ʽ)
		case Operator_Evaluate:   // ��ֵ����(�����߱�����ֵ,�����Ǳ��ʽ��Ҳ��������ֵ,���ҽ���ֵ����ʽ������������б�)			
		case Operator_Function:  // ��������
		case Operator_Express:
		case Operator_InportData:
		default: 
			break;
		}
		*/
	}
	catch(...)
	{
	}

	return hx_DefMinValues; // return 0;
}

LPNODELEMENT CExpression::GetArbore()
{
	return m_Arbore;
}

CExpression::CExpression(CExpression & expresie)
{
	*this = expresie;
}

LPNODELEMENT CExpression::CloneTree()
{
	return clone(m_Arbore);
}

void CExpression::AtachVariables(CArray<CMapVariabile*,CMapVariabile*>& ayExternVar)
{
	for(short i = 0; i < ayExternVar.GetSize(); i++)
	{
		if(i == 0)
		{
			m_pExternExpression = ayExternVar.GetAt(i);
		}
		else if(i == 1)
		{
			m_pExternVariabile = ayExternVar.GetAt(i);
		}
		else if(i == 2)
		{
			m_pExternFunction = ayExternVar.GetAt(i);
		}
	}
}

LPNODELEMENT CExpression::clone(LPNODELEMENT arb)
{
	if (arb == NULL)
		return NULL;	

	LPNODELEMENT clonArb = new NODELEMENT;
	clonArb->Copy(arb);
	clonArb->left  = clone(arb->left);
	clonArb->right = clone(arb->right);

	return clonArb;
}

CExpression& CExpression::operator=(CExpression &expr)
{
	m_definitie  = expr.m_definitie;
	
	m_pExpDraw	  = expr.m_pExpDraw;
	m_pParamVar   = expr.m_pParamVar;
	m_pDataSource = expr.m_pDataSource;
	
	m_nExpressType = expr.m_nExpressType;
	
	m_nPozitie = 0;
	m_Arbore   = expr.CloneTree();

	return *this;
}

void CExpression::SkipSpaces()
{
	SkipNote();

	BYTE cChar = m_definitie[m_nPozitie];
	int nPos = m_nPozitie;
	while ( (nPos = IsChineseSpace(cChar,m_definitie,nPos)) || // ��ȫ�ǿո��
			 (cChar  == VK_RETURN)  ||		// �ǻس�
			 (cChar  == '\n')	    ||		// ���з�
			 (cChar  == VK_TAB) )			// tab
	{
		if(nPos)
			m_nPozitie += nPos;
		else
			m_nPozitie ++;
		cChar = m_definitie[m_nPozitie];
		nPos = m_nPozitie;
	}
}

LPNODELEMENT CExpression::logicalOp() // "< > = == >= <= <> !="����
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod;
	LPNODELEMENT arb1 = expresie();
	LPNODELEMENT arb2;
	if(arb1 == NULL) 
		return NULL;  // In caz de eroare terminate
	
	SkipSpaces();
	int nPos = m_nPozitie;
	
	int   nLen = 0;
	short bNext = 0;
	nPos = IsLessThan(m_definitie[nPos],m_definitie,nPos);
	if(nPos)
	{
		nLen  = nPos;
		bNext = 1;
	}
	else
	{
		nPos = m_nPozitie;
		nPos = IsGreatLess(m_definitie[nPos],m_definitie,nPos);
		if(nPos)
		{	
			nLen = nPos;
			bNext = 2;
		}
		else
		{
			nPos = m_nPozitie;
			nPos = IsEqual(m_definitie[nPos],m_definitie,nPos);			
			if(nPos)
			{
				nLen = nPos;
				bNext = 3;
			}
			else
			{
				nPos = m_nPozitie;
				nPos = IsNot(m_definitie[nPos],m_definitie,nPos);				
				if(nPos)
				{
					nLen = nPos;
					bNext = 4;
				}
			}
		}
	}
	
	//short bNext = ( (m_definitie[m_nPozitie] == '<') ?1:
	//			    (m_definitie[m_nPozitie] == '>') ?2:
	//			    (m_definitie[m_nPozitie] == '=') ?3:
	//			    (m_definitie[m_nPozitie] == '!') ?4:0);
	short bUnion;
	while ( bNext )
	{
		m_nPozitie += nLen;

		bUnion = 0;
		nPos   = m_nPozitie;
		nPos   = IsEqual(m_definitie[nPos],m_definitie,nPos);
		if(nPos)
		{
			nLen = nPos;
			bUnion = 1;
		}
		else
		{
			nPos = m_nPozitie;
			nPos = IsGreatLess(m_definitie[nPos],m_definitie,nPos);
			if(nPos)
			{
				nLen = nPos;
				bUnion = 2;
			}
		}
		//bUnion = ( ( m_definitie[m_nPozitie] == '=' )?1:
		//		   ( m_definitie[m_nPozitie] == '>' )?2:0);
		if( !bUnion )
		{
			m_nPozitie -= nLen;
		}
		
		nod = new NODELEMENT;
		nod->left = arb1;

		nod->operatie = ( bNext == 1? (bUnion == 1?Operator_SmallEqual:bUnion == 2?Operator_NotEqual:'<') :
						  bNext == 2? (bUnion == 1?Operator_LargerEqual:'>') :
						  bNext == 3? '=':
						              (bUnion == 1?Operator_NotEqual:Operator_Error));	
		m_nPozitie += nLen;

		arb2 = expresie();
		nod->right = arb2;
		if  (arb2 == NULL) 
		{
			CString str;
			str.Format("%c",nod->operatie);
			AddError( new nodErrorInfo(hxError_LogicalRight,
					  m_nPozitie,m_definitie.GetLength(),
					  str) );

			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
		arb1 = nod;
		
		nPos  = m_nPozitie;		
		nLen  = 0;
		bNext = 0;
		nPos  = IsLessThan(m_definitie[nPos],m_definitie,nPos);
		if(nPos)
		{
			nLen  = nPos;
			bNext = 1;
		}
		else
		{
			nPos = m_nPozitie;
			nPos = IsGreatLess(m_definitie[nPos],m_definitie,nPos);
			if(nPos)
			{	
				nLen = nPos;
				bNext = 2;
			}
			else
			{
				nPos = m_nPozitie;
				nPos = IsEqual(m_definitie[nPos],m_definitie,nPos);			
				if(nPos)
				{
					nLen = nPos;
					bNext = 3;
				}
				else
				{
					nPos = m_nPozitie;
					nPos = IsNot(m_definitie[nPos],m_definitie,nPos);				
					if(nPos)
					{
						nLen = nPos;
						bNext = 4;
					}
				}
			}
		}

		//bNext = ((m_definitie[m_nPozitie] == '<')?1:
		//		   (m_definitie[m_nPozitie] == '>')?2:
		//		   (m_definitie[m_nPozitie] == '=')?3:
		//		   (m_definitie[m_nPozitie] == '!')?4:0);
	}
	return arb1;
}

LPNODELEMENT CExpression::KommaOp() // "," ����
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod;
	LPNODELEMENT arb1 = EvaluateOp(); //��ֵ
	LPNODELEMENT arb2;
	if(arb1 == NULL) 
		return NULL;  // In caz de eroare terminate
	
	SkipSpaces();
	while ( IsKomma(m_definitie[m_nPozitie],m_definitie,m_nPozitie) /*m_definitie[m_nPozitie] == ','*/ )
	{
		nod = new NODELEMENT;
		nod->left = arb1;
		
		nod->operatie = Operator_Komma;
		m_nPozitie++;
		
		arb2 = EvaluateOp();
		nod->right = arb2;
		if  (arb2 == NULL) 
		{
			CString str;
			str.Format("%c",nod->operatie);
			AddError( new nodErrorInfo(hxError_CommaRight,
					  m_nPozitie,m_definitie.GetLength(),
					  str) );

			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
		arb1 = nod;		
	}

	return arb1;
}

BOOL CExpression::IsOrlogic()
{
	/*
	if( m_definitie[m_nPozitie] == '|' )
	{
		m_nPozitie++;
		if ( m_definitie[m_nPozitie] != '|' )
		{
			m_nPozitie--;
			return FALSE;
		}
		return TRUE;
	}
	*/

	int nOldPos = m_nPozitie;
	if( nOldPos = IsOr(m_definitie[nOldPos],m_definitie,nOldPos) )
	{
		m_nPozitie += nOldPos;
		int nPos = m_nPozitie;
		if ( !(nPos = IsOr(m_definitie[nPos],m_definitie,nPos)) )
		{
			m_nPozitie -= nOldPos;
			return FALSE;
		}
		return TRUE;
	}

	nOldPos = m_nPozitie;
	if ( HSIsalpha(m_definitie[m_nPozitie]) ||
		 IsChinese(m_definitie[m_nPozitie],m_definitie,m_nPozitie) )	// Am i an identifier ?
	{
		char cChar;
		while ( (cChar = IsChinese(m_definitie[m_nPozitie],m_definitie,m_nPozitie,TRUE)) ||
				isalnum(m_definitie[m_nPozitie]) )
		{
			if(cChar == 2)
				break;
			m_nPozitie++;
		}

		CString id = m_definitie.Mid(nOldPos,m_nPozitie - nOldPos);
		id = ReplaceChar(id);
		if( !id.CompareNoCase("or") || !id.CompareNoCase("��") )
		{
			return TRUE;
		}
	}
	m_nPozitie = nOldPos;
	return FALSE;
}

LPNODELEMENT CExpression::OrlogicalOp() // "||" ����
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod;
	LPNODELEMENT arb1 = AndlogicalOp();
	LPNODELEMENT arb2;
	if(arb1 == NULL) 
		return NULL;  // In caz de eroare terminate
	
	SkipSpaces();
	while ( IsOrlogic() )
	{
		nod = new NODELEMENT;
		nod->left = arb1;		
		
		nod->operatie = Operator_OR;
		m_nPozitie++;
		
		arb2 = AndlogicalOp();
		nod->right = arb2;
		if  (arb2 == NULL) 
		{
			AddError( new nodErrorInfo(hxError_OrRight,
				m_nPozitie,m_definitie.GetLength(),
				"||") );
			
			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}		
		arb1 = nod;
	}
	/*
	SkipSpaces();
	while ( m_definitie[m_nPozitie] == '|' )
	{
		m_nPozitie++;
		if ( m_definitie[m_nPozitie] != '|' )
		{
			m_nPozitie--;
			break;
		}
		else
		{						
			nod = new NODELEMENT;
			nod->left = arb1;		
			
			nod->operatie = Operator_OR;
			m_nPozitie++;

			arb2 = AndlogicalOp();
			nod->right = arb2;
			if  (arb2 == NULL) 
			{
				AddError( new nodErrorInfo(hxError_OrRight,
					  m_nPozitie,m_definitie.GetLength(),
					  "||") );

				elibmem(nod);
				return NULL;  // In caz de eroare terminate
			}		
			arb1 = nod;
		}
	}
	*/

	return arb1;
}

BOOL CExpression::IsAndlogic()
{
	if( m_definitie[m_nPozitie] == '&' )
	{
		m_nPozitie++;
		if ( m_definitie[m_nPozitie] != '&' )
		{
			m_nPozitie--;
			return FALSE;
		}
		return TRUE;
	}

	int nOldPos = m_nPozitie;
	if ( HSIsalpha(m_definitie[m_nPozitie]) ||
		 IsChinese(m_definitie[m_nPozitie],m_definitie,m_nPozitie) )	// Am i an identifier ?
	{
		char cChar;
		while ( (cChar = IsChinese(m_definitie[m_nPozitie],m_definitie,m_nPozitie,TRUE)) ||
			isalnum(m_definitie[m_nPozitie]) )
		{
			if(cChar == 2)
				break;
			m_nPozitie++;
		}

		CString id = m_definitie.Mid(nOldPos,m_nPozitie - nOldPos);
		id = ReplaceChar(id);
		if( !id.CompareNoCase("and") || !id.CompareNoCase("����") )
		{
			return TRUE;
		}
	}
	m_nPozitie = nOldPos;
	return FALSE;
}

LPNODELEMENT CExpression::AndlogicalOp() // "&&" ����
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod;
	LPNODELEMENT arb1 = logicalOp();
	LPNODELEMENT arb2;
	if(arb1 == NULL) 
		return NULL;  // In caz de eroare terminate
		
	SkipSpaces();
	while ( IsAndlogic() )
	{
		nod = new NODELEMENT;
		nod->left = arb1;		
		
		nod->operatie = Operator_AND;
		m_nPozitie++;
		
		arb2 = logicalOp();
		nod->right = arb2;
		if  (arb2 == NULL) 
		{
			AddError( new nodErrorInfo(hxError_AndRight,
				m_nPozitie,m_definitie.GetLength(),
				"&&") );
			
			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}		
		arb1 = nod;
	}

	
	return arb1;
}

LPNODELEMENT CExpression::LineOp() // �ֺ�(���б���)
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod;
	LPNODELEMENT arb1 = KommaOp();//EvaluateOp();//OrlogicalOp();
	LPNODELEMENT arb2;
	if(arb1 == NULL) 
		return NULL;  // In caz de eroare terminate
	
	SkipSpaces();
	while ( IsSemicolon(m_definitie[m_nPozitie],m_definitie,m_nPozitie)/*(m_definitie[m_nPozitie] == ';')*/ )
	{
		nod = new NODELEMENT;
		nod->left = arb1;

		// һ����䡣���뵽��
		if( arb1 && arb1->operatie != Operator_Semicolon )
		{
			AddExternDraw(arb1);
		}

		// һ����䡣
		HSCurDataInfoStatus status;
		status.m_dwStatus = hx_EXCUTE_PROMPT_ERROR | hx_GETLINE_DRAWEXPRESS;
		vexp(arb1, &status);
		
		nod->operatie = Operator_Semicolon;
		m_nPozitie++;
		
		arb2 = KommaOp();//EvaluateOp();//OrlogicalOp();
		nod->right = arb2;
		arb1 = nod;

		// һ����䡣���뵽��
		if(arb2 != NULL)
		{
			AddExternDraw(arb2);
		}

		//
		if(arb2 == NULL)
		{
			break;
		}
		else
		{
			// һ����䡣
			vexp(arb2, &status);
		}
	}

	return arb1;
}

LPNODELEMENT CExpression::EvaluateOp()   // := ��ֵ����
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod;
	LPNODELEMENT arb1 = OrlogicalOp();
	LPNODELEMENT arb2;
	if(arb1 == NULL) 
		return NULL;  // In caz de eroare terminate
	
	SkipSpaces();
	if( (arb1->operatie == Operator_Evaluate) || // ��ֵ���ʽ
	    (arb1->operatie == Operator_Colon) )     // ��ͼ���ʽ
	{		
		nod = new NODELEMENT;
		nod->left = arb1;
		
		nod->operatie = arb1->operatie;
		arb2 = OrlogicalOp();
		nod->right = arb2;
		if  (arb2 == NULL) 
		{
			AddError( new nodErrorInfo(nod->operatie == Operator_Evaluate?hxError_DefinedRight:hxError_DrawRight,
					  m_nPozitie,m_definitie.GetLength(),
					  nod->operatie == Operator_Evaluate?":=":":") );

			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
		arb1 = nod;

		// �������ڱ�����ֵ��(�洢����)
		arb1->cDataType |= DataTypeOperCompile;
		double d = vexp( arb1 ,0);
		arb1->cDataType &= ~DataTypeOperCompile;
	}

	return arb1;
}

LPNODELEMENT CExpression::sgOp()
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod = NULL;
	LPNODELEMENT arb2;

	SkipSpaces(); // 
	if (( m_definitie[m_nPozitie] == '!' ) /* || another same priority operations*/) 
	{
		nod = new NODELEMENT;
		nod->left = NULL;
		
		nod->operatie = m_definitie[m_nPozitie];
		m_nPozitie ++;

		arb2 = sgOp();
		nod->right = arb2;
		if  (arb2 == NULL) 
		{
			CString str;
			str.Format("%c",nod->operatie);
			AddError( new nodErrorInfo(hxError_ReverseRight,
					  m_nPozitie,m_definitie.GetLength(),
					  str) );

			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
	}
	else
	{
		nod = factor();
	}
	return nod;
}

BOOL CExpression::SkipNote() // �� /* // ע�Ͳ���
{
	while ( (((BYTE)m_definitie[m_nPozitie] == VK_SPACE) && (m_definitie[m_nPozitie] != '\0')) || // �ǿո��
			((BYTE)m_definitie[m_nPozitie]  == VK_RETURN)  ||		// �ǻس�
			((BYTE)m_definitie[m_nPozitie]  == '\n')	   ||		// ���з�
			((BYTE)m_definitie[m_nPozitie]  == VK_TAB) ) // tab
	{
		if( IsEnd() )
		{
			break;
		}
		else
		{
			m_nPozitie ++;
		}
	}
	
	int   nBegin = -1,nEnd = -1;
	int   nPos = m_nPozitie;
	short bNext = -1;
	BOOL  bLoop = TRUE;
	while ( bLoop && 
		    !IsEnd() )
	{
		// /**/ע��
		if( nBegin == -1 &&
			(m_definitie[m_nPozitie] == '/') && (m_definitie[m_nPozitie+1] == '*') )
		{
			nPos = m_nPozitie;

			m_nPozitie++;
			nBegin = m_nPozitie;
			bNext = 1;
		}
		if( bNext == 1 &&
			(m_definitie[m_nPozitie] == '*') && (m_definitie[m_nPozitie+1] == '/') )
		{
			m_nPozitie++;
			nEnd = m_nPozitie;
		}

		// // ע��
		if( nBegin == -1 &&
			(m_definitie[m_nPozitie] == '/') && (m_definitie[m_nPozitie+1] == '/') )
		{	
			nPos = m_nPozitie;

			m_nPozitie++;
			nBegin = m_nPozitie;
			bNext = 2;
		}
		if( bNext == 2 &&
			((m_definitie[m_nPozitie] == '\n') || (m_definitie[m_nPozitie+1] == '\r')) )
		{	
			m_nPozitie++;
			nEnd = m_nPozitie;
		}

		// { } ע��
		if( nBegin == -1 &&
			(m_definitie[m_nPozitie] == '{') )
		{
			nPos = m_nPozitie;

			m_nPozitie++;
			nBegin = m_nPozitie;
			bNext = 1;
		}
		if( bNext == 1 &&
			(m_definitie[m_nPozitie] == '}') )
		{
			m_nPozitie++;
			nEnd = m_nPozitie;
		}

		if(nBegin != -1 && nEnd != -1)
		{
			m_nPozitie++;
			SkipNote();

			bNext  = -1;
			nBegin = -1;
			nEnd   = -1;
			return FALSE;
		}
		if(bNext != -1)
			m_nPozitie++;
		else
			break;
		
		while ( (((BYTE)m_definitie[m_nPozitie] == VK_SPACE) && (m_definitie[m_nPozitie] != '\0')) || // �ǿո��
				(bNext != 2 && (((BYTE)m_definitie[m_nPozitie]  == VK_RETURN)  ||		// �ǻس�
				((BYTE)m_definitie[m_nPozitie]  == '\n')))	   ||		// ���з�
				((BYTE)m_definitie[m_nPozitie]  == VK_TAB) ) // tab
		{
			if( IsEnd() )
			{
				break;
			}
			else
			{
				m_nPozitie ++;
			}
		}

		//while ( ((m_definitie[m_nPozitie] == ' ') &&    // �ǿո��
		//		(m_definitie[m_nPozitie] != '\0')) ||
		//		(bNext != 2 && (((m_definitie[m_nPozitie] == '\r')  ||  // �ǻس������з�
		//		 (m_definitie[m_nPozitie] == '\n')))) )
		//{
		//	m_nPozitie ++;
		//}

	}
	
	if(bNext != -1 && nBegin != -1 && nEnd == -1)
	{
		AddError( new nodErrorInfo(hxError_Notes_Invalidate,
					  nPos,m_nPozitie) );
	}

	return FALSE;
}

BOOL CExpression::IsOperInTree(LPNODELEMENT pTree,CArray<unsigned char,unsigned char>& ayOperatie,int nSize)
{	
	BOOL bRet = FALSE;
	if( pTree != NULL )
	{
		for( char i = 0; i < nSize; i++ )
		{
			if( pTree->operatie == ayOperatie.GetAt(i) )
			{
				return TRUE;
			}
		}

		if(pTree->left != NULL)
		{
			bRet = IsOperInTree(pTree->left,ayOperatie,nSize);
			if(bRet)
				return TRUE;
		}
		if(pTree->right != NULL)
		{
			bRet = IsOperInTree(pTree->right,ayOperatie,nSize);
			if(bRet)
				return TRUE;
		}
	}
	return bRet;
}

void CExpression::GetDetailVariable( LPNODELEMENT pTree,FormulaGeneralParam* ayVars )
{
	if(pTree != NULL)
	{
		if( pTree->operatie == Operator_Komma )
		{
			if(pTree->right != NULL)
			{
				ayVars->InsertAt(0,pTree->right);
			}
			if(pTree->left != NULL)
			{
				if(pTree->left == NULL || pTree->left->operatie != Operator_Komma)
				{
					ayVars->InsertAt(0,pTree->left);
				}
			}
		}

		if( pTree->operatie != Operator_Function && // ����
			pTree->operatie != Operator_Express )   // ���ù�ʽ
		{
			if(pTree->left != NULL)
			{
				GetDetailVariable(pTree->left,ayVars);
			}
			if(pTree->right != NULL)
			{
				GetDetailVariable(pTree->right,ayVars);
			}
		}

		if( ayVars->GetSize() <= 0 )
			ayVars->Add(pTree);
	}
}

/*
void CExpression::GetVariable(LPNODELEMENT pTree,FormulaGeneralParam* ayVars)
{	
	if(pTree != NULL)
	{
		if( pTree->operatie == Operator_Komma )
		{
			ayVars->Add(pTree);
		}

		if( pTree->operatie != Operator_Function && // ����
			pTree->operatie != Operator_Express )   // ���ù�ʽ
		{
			if(pTree->left != NULL)
			{
				GetVariable(pTree->left,ayVars);
			}
			if(pTree->right != NULL)
			{
				GetVariable(pTree->right,ayVars);
			}
		}		
	}
}
*/

void CExpression::GetVariable(CExpValue* pValoare,LPNODELEMENT pTree,HSCurDataInfoStatus* pCurDataStatus)
{	
	if(pTree != NULL)   // ��tree�ұ߱��ʽsave��AddParam(...)
	{
		if( pTree->operatie == Operator_Komma )
		{
			LPNODELEMENT ppClone = clone(pTree->right);
			if( ppClone == NULL )
			{
				if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
				{
					AddError(new nodErrorInfo(hxError_Nod_CloneFail));
				}
				return;
			}
			
			CExpression* pExpresie = new CExpression(m_pExternExpression,
				m_pExternVariabile,m_pExternFunction);
			pExpresie->SetArbore(pValoare->GetExp(), ppClone);			
			CExpValue* pValue = new CExpValue;
			pValue->SetValue( pExpresie );
			pValoare->AddParam(pValue);
		}

		if( pTree->left != NULL && pTree->left->operatie == Operator_Komma )
		{
			GetVariable(pValoare,pTree->left,pCurDataStatus);
		}
		//if( pTree->operatie != Operator_Function )
		//{
		//	GetVariable(pValoare,pTree->right,pCurDataStatus);
		//}
	}
}

double CExpression::ExeDefault(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus)
{
	if( a->valoarestr == NULL )
	{
		if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
		{
			AddError(new nodErrorInfo(hxError_NotName_Exist));
		}
		return hx_DefMinValues; // return 0;
	}
	short nRet,nRetNow;
	//******************************������litao����*****************************************
	//CValue* pValoare = GetVariable(*a->valoarestr,nRet);
	CString strName = *a->valoarestr;
	CString tmpName = strName.Left(strName.GetLength()-3);
	CString oldName = ""; CString oldNo = "";
	CString tmpNo = strName.Right(3);
	CValue* pValoare;  CValue* pValoareNow;
	int iObjNo = _ttoi(tmpNo);


	//******************************������litao����*****************************************
	pValoareNow = GetVariableExec(strName,nRetNow);

	if( pValoareNow != NULL && nRetNow )
	{				
		switch(nRetNow)
		{
		case ExpDrawObj: // �Ѽ���,��ͼ���ʽ���ڴ����ϻ�ͼ�ı��ʽ
			{
				if( pValoareNow->IsStyle(HX_EXPRESSSENTENCE))
				{
					CTValues<void>* pDataNow = ((CExpValue*)pValoareNow)->GetRefCalcValues();
					//CTValues<void>* pData = pCurDataStatus->GetRefCalcValues();

					if(pDataNow != NULL && pDataNow->IsEmpty())
					{
						//�����еĸ�ID�ı������м����ֵȫ������
						for (int i=0; i<= iObjNo - 1;i++)
						{
							oldNo.Format("%d",i);
							if (1 == oldNo.GetLength()) 
								oldName = tmpName + "00" + oldNo;
							else if (2 == strName.GetLength())
								oldName = tmpName + "0" + oldNo;
							else
								oldName = tmpName + oldNo;

							//pValoare = GetVariableExec(oldName,nRet);
							pValoare = GetVariable2(oldName,nRet);

							if( pValoare != NULL && nRet )
							{				
								switch(nRet)
								{
								case ExpDrawObj: // �Ѽ���,��ͼ���ʽ���ڴ����ϻ�ͼ�ı��ʽ
									{
										if( pValoare->IsStyle(HX_EXPRESSSENTENCE))
										{
											CTValues<void>* pData = ((CExpValue*)pValoare)->GetRefCalcValues();
											if(pData != NULL && pData->IsEmpty())
											{
												//pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
												pData->m_pValue = pDataNow->m_pValue ;
												pData->m_lSize = pDataNow->m_lSize;
											}
										}
									}
									break;
								}		
							}
						}

						if( pCurDataStatus && pCurDataStatus->m_nDataPos >= 0 && 
							pCurDataStatus->m_nDataPos < pDataNow->m_lSize &&
							pCurDataStatus->IsStatus(hx_GetCurrentPosData))
						{
							return ((HSDouble*)pDataNow->m_pValue)[pCurDataStatus->m_nDataPos];
						}
						else 
						{
							if ( pCurDataStatus )
							{
								pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
								pCurDataStatus->m_pData = (HSDouble*)pDataNow->m_pValue;
								pCurDataStatus->m_lSize = pDataNow->m_lSize;
								return hx_ReturnPointValues;
							}
						}
					}
				}
			}
			break;
		case ExpInsideVar: // �Ѽ���,�ڲ�������һ��ָ�ڱ��ʽ�ڲ�����ı���
			{
				if( pValoareNow->IsStyle(HX_EXPRESSSENTENCE) )
				{
					CTValues<void>* pDataNow = ((CExpValue*)pValoareNow)->GetRefCalcValues();
					//CTValues<void>* pData = pCurDataStatus->GetRefCalcValues();
					if(pDataNow != NULL && pDataNow->IsEmpty() )
					{
						if( pCurDataStatus->m_nDataPos >= 0 && 
							pCurDataStatus->m_nDataPos < pDataNow->m_lSize &&
							pCurDataStatus->IsStatus(hx_GetCurrentPosData))
						{
							return ((HSDouble*)pDataNow->m_pValue)[pCurDataStatus->m_nDataPos];
						}
						else
						{
							pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
							pCurDataStatus->m_lSize = pDataNow->m_lSize;
							pCurDataStatus->m_pData = (HSDouble*)pDataNow->m_pValue;
							return hx_ReturnPointValues;
						}
					}
				}
			}
			break;
		default:
			return pValoareNow->GetValue(pCurDataStatus);
		}	
	}
	else
	{
		if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
		{
			AddError(new nodErrorInfo(hxError_UNDEFINED_VARIABLE));
		}
	}

	return hx_DefMinValues; // return 0;
}

double CExpression::ExeColon(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus)
{
	if( (a->cDataType & DataTypeOperCompile) &&
		a->left != NULL && 
		a->left->valoarestr != NULL )
	{
		CString strName = *a->left->valoarestr;

		//******************************������litao����*****************************************
		//һ�б�������� ���и�ֵ�ı�ʶ��ű������³�ʼ��
		m_strLineObj = "";

		//CString tmpName = strName.Left(strName.GetLength() - 3);

		/*
		tmpName.MakeUpper();
		CValue* pValoare = GetVariable(tmpName,nRet);
		if( pValoare != NULL && nRet )
		{
		if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
		{
		AddError(new nodErrorInfo(hxError_Name_Exist,
		GetDefinedVarPos(),m_nPozitie,*a->left->valoarestr));
		}

		return hx_DefMinValues; // return 0; 
		}
		*/
		//******************************������litao����*****************************************

		LPNODELEMENT ppClone = clone(a->right);
		if( ppClone == NULL )
		{
			if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
			{
				AddError(new nodErrorInfo(hxError_Nod_CloneFail));
			}
			
			return hx_DefMinValues; // return 0;
		}

		CExpression* pExpresie = new CExpression(m_pExternExpression,m_pExternVariabile,m_pExternFunction);				
		pExpresie->SetArbore(this, ppClone );				
		CExpValue* pValue = new CExpValue;
		pValue->AddStyle(HX_EXPRESSDRAW);
		pValue->SetValue( pExpresie );
		pValue->m_LineName = strName;
		pValue->m_InWhile = ((m_nIFWHILEStart>0)?true:false);
		pValue->m_WhileLevel = m_nIFWHILEStart;
		pValue->m_CurExecWhileLevel = 0;
		AddExternDraw( strName,pValue );
		
		return hx_DefMinValues; // return 0;
	}

	return hx_DefMinValues; // return 0;
}

double CExpression::ExeKomma(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus,
							double& v1,double& v2,
							HSCurDataInfoStatus& calc1,
							HSCurDataInfoStatus& calc2)
{
	if( m_pExpDraw != NULL && m_pExpDraw->GetObjArray() != NULL &&
		m_pExpDraw->GetObjArray()->GetSize() > 0)
	{
		CValue* pValoare = m_pExpDraw->GetObjArray()->GetAt(m_pExpDraw->GetObjArray()->GetSize()-1);

		if( pValoare->IsStyle(HX_EXPRESSSENTENCE) )
		{
			if( pValoare->IsStyle(HX_EXPRESSDRAW_NONAME) ) // ��ͼ���ʽû������
			{
				CExpValue* pExp = (CExpValue*)pValoare;
				if( pExp != NULL )
				{
					CExpression* pExpression = pExp->GetExp();
					if(pExpression != NULL)
					{
						GetVariable(pExp,pExpression->GetArbore(),pCurDataStatus);
					}
				}
			}
			else
			{
				CString strName    = m_pExpDraw->GetObjectName(pValoare);
				LPNODELEMENT pTree = GetVarFromTree(a,strName);
				if( pTree != NULL )
				{
					CExpValue* pExp = (CExpValue*)pValoare;
					if( pExp != NULL )
					{
						GetVariable(pExp,a,pCurDataStatus);
					}
				}
			}
		}

		return hx_DefMinValues; // return 0;
	}

	v1 = vexp(a->left,&calc1);
	v2 = vexp(a->right,&calc2);
	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int nEnd = max(calc1.m_lSize,calc2.m_lSize);
		pCurDataStatus->Alloc(nEnd);
		HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);

		if( calc1.m_pData != NULL ) // ��ͼ����
		{
			pCurDataStatus->m_dwStatus |= (calc1.m_dwStatus & HX_USERDRAWFUN_EXPRESS);
		}
		else
		{
			pCurDataStatus->m_dwStatus |= (calc2.m_dwStatus & HX_USERDRAWFUN_EXPRESS);
		}
		for(int nStart = 0; nStart < nEnd; nStart++)
		{
			pCurDataStatus->m_pData[nStart].Copy( &pData[nStart] );
		}
		//memcpy(pCurDataStatus->m_pData,pData,sizeof(HSDouble)*max(calc1.m_lSize,calc2.m_lSize));
		return hx_ReturnPointValues;
	}

	if( v1 != (double)hx_DefMinValues )
	{
		return v1;
	}
	if( v2 != (double)hx_DefMinValues )
	{
		return v2;
	}

	return hx_DefMinValues;
}

double CExpression::ExeEvaluate( LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus )
{
	if( (a->cDataType & DataTypeOperCompile) &&
		a->left != NULL && 
		a->left->valoarestr != NULL )
	{
		CString strName = *a->left->valoarestr;
		strName.MakeUpper();
		//******************************������litao����*****************************************
		//strName = strName.Left(strName.GetLength() - 3);

		/*CValue* pValoare = GetVariable(strName,nRet);
		if( pValoare != NULL && nRet )
		{
		/*	if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
		{
		AddError(new nodErrorInfo(hxError_Name_Exist,
		GetDefinedVarPos(),m_nPozitie,*a->left->valoarestr));
		}

		return hx_DefMinValues; // return 0; 
		}
		*/
		//******************************������litao����*****************************************

		LPNODELEMENT ppClone = clone(a->right);
		if( ppClone == NULL )
		{
			if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
			{
				AddError(new nodErrorInfo(hxError_Nod_CloneFail));
			}
			return hx_DefMinValues; // return 0;
		}

		CExpression* pExpresie = new CExpression(m_pExternExpression,m_pExternVariabile,m_pExternFunction);
		pExpresie->SetArbore(this, ppClone );				
		CExpValue* pValue = new CExpValue;
		pValue->SetValue( pExpresie );
		pValue->m_LineName = strName;
		pValue->m_InWhile = ((m_nIFWHILEStart>0)?true:false);
		pValue->m_WhileLevel = m_nIFWHILEStart;
		pValue->m_CurExecWhileLevel = 0;
		AddExternDraw( strName,pValue );

		return hx_DefMinValues; // return 0;
	}

	return hx_DefMinValues; // return 0;
}

double CExpression::ExeInport( LPNODELEMENT pTree ,HSCurDataInfoStatus* pCurDataStatus)
{
	try
	{
		if ( pTree->right == NULL )
			return hx_DefMinValues;

		// ����Դ�趨�����趨����ȡ��ǰ�ġ�
		InportDataParam* pInportData = (InportDataParam*)pTree->pData;
		if( pInportData && pInportData->m_pDataSrc )
		{
			pInportData->m_pDataSrc->SetRefDataSource(g_pHxDataSource);
			pInportData->m_pDataSrc->ReadData();
		}

		// ���õĺ�����ȡ����
		FunctionParam* pFunctionParam = (FunctionParam*)pTree->right->pData;
		CFunExpresion* pFun = (CFunExpresion*)pFunctionParam->pFun;
		if( pFunctionParam == NULL || pFun == NULL )
		{
			AddError(new nodErrorInfo(hxError_NotFunction));
			return hx_DefMinValues; // return 0;
		}
		double dValue = hx_DefMinValues;

		this->SetData(pInportData);

		int nError = pFun->Evaluate(this,pFunctionParam->ayVars,pCurDataStatus,dValue);

		this->SetData(NULL);

		if(nError < 0)
			return dValue;

	}
	catch(...)
	{
	}

	return hx_DefMinValues; // return 0;
}

double CExpression::ExeExpress( LPNODELEMENT pTree ,HSCurDataInfoStatus* pCurDataStatus)
{
	try
	{
		ExpressParam* pFunctionParam = (ExpressParam*)pTree->pData;
		CExpValue*			pValue   = (CExpValue*)pFunctionParam->pValue;
		if(pValue == NULL)
		{
			AddError(new nodErrorInfo(hxError_Exp_param));
			return hx_DefMinValues; // return 0;
		}
		CExpression* pExpression = pValue->GetExpByCompile();
		if( pFunctionParam == NULL || pExpression == NULL )
		{
			AddError(new nodErrorInfo(hxError_Exp_param));
			return hx_DefMinValues; // return 0;
		}

		//
		CMapVariabile* pVar = pExpression->GetExpDraw();
		if( pVar == NULL ||
			pVar->GetObjArray() == NULL )
		{
			AddError(new nodErrorInfo(hxError_Exp_param));
			return hx_DefMinValues;
		}

		short nDrawCount;
		pVar->GetDrawCount(nDrawCount);
		if( nDrawCount == 0 )
		{
			AddError(new nodErrorInfo(hxError_Exp_param));
			return hx_DefMinValues;
		}

		//
		CString	strSub = pFunctionParam->strSub;
		CExpValue* pDraw = NULL;
		if( !strSub.IsEmpty() )
		{
			pDraw = (CExpValue *)pExpression->GetDrawExpress(strSub);
		}
		else
		{
			pDraw = (CExpValue *)pExpression->GetDrawExpress(-1);
		}

		// һ��һ������
		CArray<CLineProperty*,CLineProperty*> ayData;
		CExpValue* pCurVal;
		CLineProperty* pData;
		for(int nObj = 0; nObj < pVar->GetObjArray()->GetSize(); nObj++)
		{
			pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
			if( pCurVal != NULL )
			{
				pData = new CLineProperty;
				if( pCurDataStatus )
				{
					pData->m_pDataSource = pCurDataStatus->m_pDataSource;
				}
				ayData.Add(pData);
				
				if( pDraw == pCurVal )
				{
					pCurVal->CalcValue(pData,0,0,pCurDataStatus);
					break;
				}
				else
				{
					pCurVal->CalcValue(pData,0,0,NULL);
				}
			}
		}

		_delArrayObj(ayData);

		return 0;

		/*
		// begin
		CString	strSub = pFunctionParam->strSub;
		CExpValue* pDraw = NULL;
		if( !strSub.IsEmpty() )
		{
			pDraw = (CExpValue *)pExpression->GetDrawExpress(strSub);
		}
		else
		{
			pDraw = (CExpValue *)pExpression->GetDrawExpress(-1);
		}

		double dValue = hx_DefMinValues;
		int nError	  = -1;
		if(pDraw != NULL)
		{
			pExpression = pDraw->GetExp();
			if(pExpression != NULL)
			{
				nError = pExpression->Value(g_dwRetValoare,pCurDataStatus);
			}
		}
		// end
		*/

		//if(nError < 0)
		//	return dValue;
	}
	catch(...)
	{
	}

	return hx_DefMinValues; // return 0;
}

double CExpression::ExeFunction( LPNODELEMENT pTree ,HSCurDataInfoStatus* pCurDataStatus)
{
	try
	{
		FunctionParam* pFunctionParam = (FunctionParam*)pTree->pData;
		CFunExpresion* pFun = (CFunExpresion*)pFunctionParam->pFun;
		if( pFunctionParam == NULL || pFun == NULL )
		{
			AddError(new nodErrorInfo(hxError_NotFunction));
			return hx_DefMinValues; // return 0;
		}
		double dValue = hx_DefMinValues;

		int nError;

		//******************************������litao����*****************************************
		CString strFunName = pFun->GetFunName();

		if (0 == strFunName.CompareNoCase("WHILE"))
		{
			dValue = WHILE(this, pFunctionParam->ayVars,pCurDataStatus,nError);
			if(nError == MORELOOPERROR)
			{
				AddError(new nodErrorInfo(hxError_MoreLoop));
			//	MessageBox(AfxGetMainWnd()->m_hWnd,hxGetErrorInfoByErrorNum(hxError_MoreLoop),"��ʾ",MB_OK);
				//::AfxMessageBox(hxGetErrorInfoByErrorNum(hxError_MoreLoop));
				return hx_DefMinValues; // return 0;
			}
		}
		else
			nError =  pFun->Evaluate(this,pFunctionParam->ayVars,pCurDataStatus,dValue);
		//******************************������litao����*****************************************

		if(nError < 0)
			return dValue;
	}
	catch(...)
	{
	}

	return hx_DefMinValues; // return 0;
}

void CExpression::AddParamer(CString strName,CValue* pExpress)
{
	if( strName.IsEmpty() || pExpress == NULL )
		return;

	if(m_pParamVar == NULL)
	{
		m_pParamVar = new CMapVariabile;
	}
	strName.MakeUpper();
	strName = ReplaceChar(strName);	
	m_pParamVar->SetAt((char*)LPCSTR(strName),pExpress);
}

void CExpression::AddExternDraw(CString strName,CValue* pExpress)
{
	//******************************������litao����*****************************************
	/*if( strName.IsEmpty() || pExpress == NULL)
	return;

	if(m_pExpDraw == NULL)
	{
	m_pExpDraw = new CMapVariabile;
	}
	strName.MakeUpper();
	strName = ReplaceChar(strName);
	m_pExpDraw->SetAt((char*)LPCSTR(strName),pExpress);*/

	if( strName.IsEmpty() || pExpress == NULL)
		return;

	if(m_pExpDraw == NULL)
	{
		m_pExpDraw = new CMapVariabile;
	}
	strName.MakeUpper();
	strName = ReplaceChar(strName);

	//CString tmpName;

	//tmpName = AddObjNo(strName,COMPILE_NO);

	m_pExpDraw->SetAt((char*)LPCSTR(strName),pExpress);

	//�жϵ�Ϊѭ���ڵĵ�һ�����ʱ����ͷβ�ڵ��λ��
	if ((m_nIFWHILEStart >  0) && (m_nWhileFirst.GetCount() < m_nIFWHILEStart))
	{
		m_nWhileFirst.SetAt(m_nIFWHILEStart, m_pExpDraw->GetObjArray()->GetSize() - 1);
	}

	//ÿ�ΰ�ͷ��Σ�����β��λ��
	if (m_nIFWHILEStart > 0)
		m_nWhileLast.SetAt(m_nIFWHILEStart, m_pExpDraw->GetObjArray()->GetSize() - 1);

	//******************************������litao����*****************************************
}

void CExpression::AddExternDraw(LPNODELEMENT pTree)
{
	if( pTree == NULL )
		return;

	if( !IsOperInTree(pTree,g_ayAheadChar,g_ayAheadChar.GetSize()) )
	{
		LPNODELEMENT ppClone = clone(pTree);
		
		CExpression* pExpress = new CExpression(NULL,NULL,NULL);
		pExpress->SetArbore(this, ppClone );
		CExpValue* pValue = new CExpValue;
		pValue->AddStyle(HX_EXPRESSDRAW|HX_EXPRESSDRAW_NONAME);
		pValue->SetValue( pExpress );
		
		if(m_pExpDraw == NULL)
		{
			m_pExpDraw = new CMapVariabile;
		}
		CString strName = _T("NONE");

		strName = GetIdentifierExpressName("",strName,m_pExpDraw);
		
		strName.MakeUpper();
		strName = ReplaceChar(strName);

		//******************************������litao����*****************************************
		CString tmpName;
		tmpName = strName;
		if (0==strName.CompareNoCase(""))
			tmpName = AddObjNo(strName,COMPILE_NO);

		pValue->m_LineName = tmpName;
		pValue->m_InWhile = ((m_nIFWHILEStart>0)?true:false);
		pValue->m_WhileLevel = m_nIFWHILEStart;
		pValue->m_CurExecWhileLevel = 0;
		m_pExpDraw->SetAt((char*)LPCSTR(tmpName),pValue);
		//******************************������litao����*****************************************
	}
}

CValue* CExpression::GetDrawExpress(CString strName)
{
	strName.MakeUpper();
	CValue* valoare;
	if ( m_pExpDraw != NULL && 
		 m_pExpDraw->Lookup(strName,valoare) )
	{
		return valoare;
	}
	return NULL;
}

CValue* CExpression::GetDrawExpress(int nPos)
{
	CMapVariabile* pVar = GetExpDraw();
	if(pVar == NULL)
		return NULL;

	CArray<class CValue*,class CValue*>* pAy = pVar->GetObjArray();
	if(pAy == NULL)
		return NULL;

	if(nPos == -1) // ���һ����
		nPos = pAy->GetSize() - 1;
	if( nPos >= 0 && nPos < pAy->GetSize() )
	{
		return pAy->GetAt(nPos);
	}

	return NULL;
}

CValue* CExpression::GetVariable(CString& strName,short& nRet,int nGetType,int nHaveID /*= -1*/)
{
	strName.MakeUpper();

	CValue* valoare;

	// ���ù�ʽ
	if( nGetType != -1 ) 
	{
		if ( m_pExternExpression != NULL && 
			 m_pExternExpression->Lookup(strName,nGetType,valoare) )	// �ⲿ���ʽ��
		{
			nRet = ExpExternExp;
			return valoare;
		}
		return NULL;
	}
	//
	if ( m_pExternVariabile != NULL && 
		 m_pExternVariabile->Lookup(strName,valoare) ) // �ⲿ������
	{
		nRet = ExpExternVar;
		return valoare;
	}
	if ( m_pExternFunction != NULL && 
		 m_pExternFunction->Lookup(strName,valoare) ) // �ⲿ������
	{
		nRet = ExpExternFun;
		return valoare;
	}	

	//******************************������litao�޸�*****************************************

	if ( m_pParamVar != NULL && 
		 m_pParamVar->Lookup(strName,valoare) )     // �ڲ�������
	{
		nRet = ExpParamVar;
		return valoare;
	}
	
	CString tmpName;
	tmpName = GetObjNo(strName,COMPILE_NO,nHaveID);
	if ( m_pExpDraw != NULL && 
		m_pExpDraw->Lookup(tmpName,valoare ) )      // �ڲ���ͼ��
	{
		nRet = ExpDrawObj;
		strName = tmpName;
		return valoare;
	}
	//tmpName = SubObjNo(strName,COMPILE_NO);
	//******************************������litao�޸�*****************************************

/*
	if ( m_pRefExpDraw != NULL && 
		 m_pRefExpDraw->Lookup(strName,valoare) )   // �ڲ���ͼ��
	{
		nRet = ExpDrawObj;
		return valoare;
	}
	if ( m_pRefInsideVar != NULL && 
		 m_pRefInsideVar->Lookup(strName,valoare) ) // �ڲ�������
	{
		nRet = ExpParamVar;
		return valoare;
	}
	if ( m_pRefParamVar != NULL && 
		 m_pRefParamVar->Lookup(strName,valoare) )	// �ڲ�������
	{
		nRet = ExpParamVar;
		return valoare;
	}
*/
	
	if(m_pRefExpress != NULL)						 // �������ʽ
	{
		return m_pRefExpress->GetVariable(strName,nRet,nGetType);
	}

	nRet = 0;
	return NULL;
}

int	CExpression::GetDefinedVarPos()
{
	int nPos = m_nPozitie-1;
	int nPos1 = nPos;
	int nPos2 = nPos;
	while( nPos >= 0 && 
		  !( (nPos1 = IsSemicolon(m_definitie[nPos],m_definitie,nPos)) || // ???
		     (nPos2 = IsKomma(m_definitie[nPos],m_definitie,nPos))	 ||
		     (m_definitie[nPos] == '\r') || 
		     (m_definitie[nPos] == '\n') ||
		     (m_definitie[nPos] == '/')) )
	{
		if(nPos1)
			nPos -= nPos1;
		else if(nPos2)
			nPos -= nPos2;
		else
			nPos--;
	}
	return (nPos+1);

}

BOOL CExpression::IsValidPWD(CString strPwd)
{
	if( !strPwd.Compare(m_strPWD) )
		return TRUE;

	return FALSE;
}

COMPILEFUNCTION CExpression::GetSysFun(CString strName)
{
	strName.MakeUpper();
	CValue* valoare;

	if ( m_pExternFunction != NULL && 
		 m_pExternFunction->Lookup(strName,valoare) ) // �ⲿ������
	{
		return ((CFunExpresion*)valoare)->GetCurFun();
	}

	return NULL;
}

void InitSysFunction(CMapVariabile*& pExternFunction) // ��ʼ���Ѷ���ĺ���
{
	CFunExpresion* pFun;
	
	//*************************************************************************************************
	pFun = new CFunExpresion(FunOper_CEILING,"CEILING",1,g_pSystemFun->CEILING);    // ����: CEILING(var1) �����ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_FLOOR,"FLOOR",1,g_pSystemFun->FLOOR);    // ����: FLOOR(var1) �����ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ROUND,"ROUND",1,g_pSystemFun->ROUND);    // ����: ROUND(var1) �����ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LWINNER,"LWINNER",2,g_pSystemFun->LWINNER);    // ����: ���ڻ����̱���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PWINNER,"PWINNER",2,g_pSystemFun->PWINNER);    // ����: Զ�ڻ����̱���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DRAWNULL,"DRAWNULL",0,g_pSystemFun->DRAWNULL);    // ����: ���ز���������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	//2010.10.8 litao pFun = new CFunExpresion(FunOper_RBTREND,"RBTREND",1,g_pSystemFun->RBTREND);    // ����: AbleTrend test
	//pExternFunction->SetAt(pFun->GetFunName(),pFun);
	//*************************************************************************************************

	// ���麯��
#ifdef SUPPORT_NETVALUE
	pFun = new CFunExpresion(FunOper_NETVALUE,"NETVALUE",0,g_pSystemFun->NETVALUE);    // ����: NETVALUE() ��ծ����,����ֵ����λ��Ԫ)
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
#endif
	pFun = new CFunExpresion(FunOper_ADVANCE,"ADVANCE",0,g_pSystemFun->ADVANCE);    // ����: ADVANCE(var1) ���Ǽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_AMOUNT,"AMOUNT",0,g_pSystemFun->AMOUNT);    // ����: AMOUNT() �ɽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_AMOUNT,"�ɽ���",0,g_pSystemFun->AMOUNT);    // ����: AMOUNT() �ɽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ASKPRICE,"ASKPRICE",1,g_pSystemFun->ASKPRICE);    // ����: ASKPRICE(var1) ί���� 1-3
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ASKVOL,"ASKVOL",1,g_pSystemFun->ASKVOL);    // ����: ASKVOL(var1) ί���� 1-3
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BIDPRICE,"BIDPRICE",1,g_pSystemFun->BIDPRICE);    // ����: BIDPRICE(var1) ί��� 1-3
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BIDVOL,"BIDVOL",1,g_pSystemFun->BIDVOL);    // ����: BIDVOL(var1) ί���� 1-3
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BUYVOL,"BUYVOL",0,g_pSystemFun->BUYVOL);    // ����: BUYVOL() ��������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CLOSE,"CLOSE",0,g_pSystemFun->CLOSE);    // ����: CLOSE() ���̼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CLOSE,"C",0,g_pSystemFun->CLOSE);		   // ����: CLOSE() ���̼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CLOSE,"���̼�",0,g_pSystemFun->CLOSE);		   // ����: CLOSE() ���̼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DECLINE,"DECLINE",0,g_pSystemFun->DECLINE);    // ����: DECLINE() �µ�����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_EXTDATA,"EXTDATA",1,g_pSystemFun->EXTDATA);    // ����: EXTDATA(var1) ��չ����1-11
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HIGH,"HIGH",0,g_pSystemFun->HIGH);    // ����: HIGH() ��߼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HIGH,"H",0,g_pSystemFun->HIGH);	    // ����: HIGH() ��߼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HIGH,"��߼�",0,g_pSystemFun->HIGH);	    // ����: HIGH() ��߼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ISBUYORDER,"ISBUYORDER",0,g_pSystemFun->ISBUYORDER);    // ����: ISBUYORDER() �Ƿ���������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LOW,"LOW",0,g_pSystemFun->LOW);     // ����: LOW() ��ͼ�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LOW,"L",0,g_pSystemFun->LOW);	   // ����: LOW() ��ͼ�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LOW,"��ͼ�",0,g_pSystemFun->LOW);	   // ����: LOW() ��ͼ�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_OPEN,"OPEN",0,g_pSystemFun->OPEN);   // ����: OPEN() ���̼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun); 
	pFun = new CFunExpresion(FunOper_OPEN,"O",0,g_pSystemFun->OPEN);	   // ����: OPEN() ���̼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_OPEN,"���̼�",0,g_pSystemFun->OPEN);	   // ����: OPEN() ���̼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SELLVOL,"SELLVOL",0,g_pSystemFun->SELLVOL);    // ����: SELLVOL() ����������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VOL,"VOL",0,g_pSystemFun->VOL);     // ����: VOL() �ɽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VOL,"V",0,g_pSystemFun->VOL);       // ����: VOL() �ɽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VOL,"�ɽ���",0,g_pSystemFun->VOL);       // ����: VOL() �ɽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// ʱ�亯��
	pFun = new CFunExpresion(FunOper_DATE,"DATE",0,g_pSystemFun->hxDATE);    // ����: DATE() ������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PERIOD_TYPE_DAY,"DAY",0,g_pSystemFun->hxDAY);    // ����: PERIOD_TYPE_DAY() ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HOUR,"HOUR",0,g_pSystemFun->HSOUR);    // ����: HOUR() Сʱ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MINUTE,"MINUTE",0,g_pSystemFun->hxMINUTE);    // ����: MINUTE() ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MONTH,"MONTH",0,g_pSystemFun->hxMONTH);    // ����: MONTH() �·�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_TIME,"TIME",0,g_pSystemFun->hxTIME);    // ����:  TIME() ʱ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_WEEKDAY,"WEEKDAY",0,g_pSystemFun->hxWEEKDAY);    // ����: WEEKDAY() ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_YEAR,"YEAR",0,g_pSystemFun->hxYEAR);    // ����: YEAR() ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	//���ú���
	pFun = new CFunExpresion(FunOper_BACKSET,"BACKSET",2,g_pSystemFun->BACKSET);    // ����: BACKSET(var1,var2) ��ǰ��ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BARSCOUNT,"BARSCOUNT",1,g_pSystemFun->BARSCOUNT);    // ����:  BARSCOUNT(var1) ��Ч������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BARSLAST,"BARSLAST",1,g_pSystemFun->BARSLAST);    // ����: BARSLAST(var1) ��һ����������λ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BARSSINCE,"BARSSINCE",1,g_pSystemFun->BARSSINCE);    // ����: BARSSINCE(var1) ��һ����������λ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COUNT,"COUNT",2,g_pSystemFun->COUNT);    // ����: COUNT(var1,var2) ͳ������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DMA,"DMA",2,g_pSystemFun->DMA);    // ����:  DMA(var1,var2) ��̬�ƶ�ƽ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_EMA,"EMA",2,g_pSystemFun->EMA);    // ����: EMA(var1,var2) ָ��ƽ���ƶ�ƽ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HHV,"HHV",2,g_pSystemFun->HHV);    // ����: HHV(var1,var2) ���ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HHVBARS,"HHVBARS",2,g_pSystemFun->HHVBARS);    // ����: HHVBARS(var1,var2) ��һ�ߵ㵽��ǰ��������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LLV,"LLV",2,g_pSystemFun->LLV);    // ����:  LLV(var1,var2) ���ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LLVBARS,"LLVBARS",2,g_pSystemFun->LLVBARS);    // ����: LLVBARS(var1,var2) ��һ�͵㵽��ǰ��������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MA,"MA",2,g_pSystemFun->MA);    // ����: MA(var1,var2) ���ƶ�ƽ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_REF,"REF",2,g_pSystemFun->REF);    // ����: REF(var1,var2) ������������ǰ������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SMA,"SMA",3,g_pSystemFun->SMA);    // ����:  SMA(var1,var2,var3) �ƶ�ƽ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SUM,"SUM",2,g_pSystemFun->SUM);    // ����: SUM(var1,var2) �ܺ�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SUMBARS,"SUMBARS",2,g_pSystemFun->SUMBARS);    // ����: SUMBARS(var1,var2) ��ǰ�ۼӵ�ָ��ֵ�����ڵ�������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// �߼�����
	pFun = new CFunExpresion(FunOper_IF,"IF",3,g_pSystemFun->IF);    // ����: IF(var1,var2,var3) ����������ͬ��ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ISDOWN,"ISDOWN",0,g_pSystemFun->ISDOWN);    // ����: ISDOWN() �������Ƿ�����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ISEQUAL,"ISEQUAL",0,g_pSystemFun->ISEQUAL);    // ����:  ISEQUAL() �������Ƿ�ƽ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ISUP,"ISUP",0,g_pSystemFun->ISUP);    // ����: ISUP() �������Ƿ�����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	//******************************������litao����*****************************************
	pFun = new CFunExpresion(FunOper_WHILE,"WHILE",3,g_pSystemFun->WHILE);    // ����: ִ��WHILEѭ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_FILTER,"FILTER",2,g_pSystemFun->FILTER);  // ����: ���˺���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	//******************************������litao����*****************************************

	// ��������
	pFun = new CFunExpresion(FunOper_ABS,"ABS",1,g_pSystemFun->ABS);    // ����: ABS(var1) �����ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CROSS,"CROSS",2,g_pSystemFun->CROSS);    // ����: CROSS(var1,var2) �����߽���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MAX,"MAX",2,g_pSystemFun->MAX);    // ����:  MAX(var1,var2) �����ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MIN,"MIN",2,g_pSystemFun->MIN);    // ����: MIN(var1,var2) ����Сֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MOD,"MOD",2,g_pSystemFun->MOD);    // ����: MOD(var1,var2) ��ģ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_NOT,"NOT",1,g_pSystemFun->NOT);    // ����: NOT(var1) ���߼���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_REVERSE,"REVERSE",1,g_pSystemFun->REVERSE);    // ����:  REVERSE(var1) ���෴��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SGN,"SGN",1,g_pSystemFun->SGN);    // ����: SGN(var1) �����ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// ��ѧ����
	pFun = new CFunExpresion(FunOper_ACOS,"ACOS",1,g_pSystemFun->ACOS);    // ����: ACOS(var1) ������ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ASIN,"ASIN",1,g_pSystemFun->ASIN);    // ����: ASIN(var1) ������ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ATAN,"ATAN",1,g_pSystemFun->ATAN);    // ����:  ATAN(var1) ������ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COS,"COS",1,g_pSystemFun->COS);    // ����: COS(var1) ����ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_EXP,"EXP",1,g_pSystemFun->EXP);    // ����: EXP(var1) ָ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LN,"LN",1,g_pSystemFun->LN);    // ����: LN(var1) ����Ȼ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LOG,"LOG",1,g_pSystemFun->LOG);    // ����:  LOG(var1) ��10Ϊ�׵Ķ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_POW,"POW",2,g_pSystemFun->POW);    // ����: POW(var1,var2) ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SIN,"SIN",1,g_pSystemFun->SIN);    // ����: SIN(var1) ����ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SQRT,"SQRT",1,g_pSystemFun->SQRT);    // ����: SQRT(var1) ��ƽ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_TAN,"TAN",1,g_pSystemFun->TAN);    // ����:  TAN(var1) ����ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// ͳ�ƺ���
	pFun = new CFunExpresion(FunOper_AVEDEV,"AVEDEV",2,g_pSystemFun->AVEDEV);    // ����: AVEDEV(var1,var2) ƽ������ƫ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DEVSQ,"DEVSQ",2,g_pSystemFun->DEVSQ);    // ����: DEVSQ(var1,var2) ����ƫ��ƽ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_FORCAST,"FORCAST",2,g_pSystemFun->FORCAST);    // ����: FORCAST(var1,var2) ���Իع�Ԥ��ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SLOPE,"SLOPE",2,g_pSystemFun->SLOPE);    // ����:  SLOPE(var1,var2) ���Իع�б��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_STD,"STD",2,g_pSystemFun->STD);    // ����: STD(var1,var2) �����׼��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_STDP,"STDP",2,g_pSystemFun->STDP);    // ����: STDP(var1,var2) �����׼��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VAR,"VAR",2,g_pSystemFun->VAR);    // ����: VAR(var1,var2) ������������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VARP,"VARP",2,g_pSystemFun->VARP);    // ����:  VARP(var1,var2) ������������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// ָ������
	pFun = new CFunExpresion(FunOper_COST,"COST",1,g_pSystemFun->COST);    // ����: COST(var1) �ɱ��ֲ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PEAK,"PEAK",3,g_pSystemFun->PEAK);    // ����: PEAK(var1,var2,var3) ǰM��ZIGת�򲨷�ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PEAKBARS,"PEAKBARS",3,g_pSystemFun->PEAKBARS);    // ����: PEAKBARS(var1,var2,var3) ǰM��ZIGת�򲨷嵽��ǰ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SAR,"SAR",3,g_pSystemFun->SAR);    // ����:  SAR(var1,var2,var3) ����ת��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SARTURN,"SARTURN",3,g_pSystemFun->SARTURN);    // ����: SARTURN(var1,var2,var3) ����ת���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_TROUGH,"TROUGH",3,g_pSystemFun->TROUGH);    // ����: TROUGH(var1,var2,var3) ǰM��ZIGת�򲨹�ֵ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_TROUGHBARS,"TROUGHBARS",3,g_pSystemFun->TROUGHBARS); // ����: TROUGHBARS(var1,var2,var3) ǰM��ZIGת�򲨹ȵ���ǰ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_WINNER,"WINNER",1,g_pSystemFun->WINNER);    // ����:  WINNER(var1) �����̱���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ZIG,"ZIG",2,g_pSystemFun->ZIG);    // ����:  ZIG(var1,var2) ֮��ת��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// ���̺���
	pFun = new CFunExpresion(FunOper_INDEXA,"INDEXA",0,g_pSystemFun->INDEXA);    // ����: INDEXA() ��Ӧ���̳ɽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXADV,"INDEXADV",0,g_pSystemFun->INDEXADV);    // ����: INDEXADV() ��Ӧ�������Ǽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXC,"INDEXC",0,g_pSystemFun->INDEXC);    // ����:  INDEXC() ��Ӧ�������̼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXDEC,"INDEXDEC",0,g_pSystemFun->INDEXDEC);    // ����:  INDEXDEC() ��Ӧ�����µ�����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXH,"INDEXH",0,g_pSystemFun->INDEXH);    // ����: INDEXH() ��Ӧ������߼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXL,"INDEXL",0,g_pSystemFun->INDEXL);    // ����: INDEXL() ��Ӧ������ͼ�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXO,"INDEXO",0,g_pSystemFun->INDEXO);    // ����:  INDEXO() ��Ӧ���̿��̼�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXV,"INDEXV",0,g_pSystemFun->INDEXV);    // ����:  INDEXV() ��Ӧ���̳ɽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// ���ú���
	pFun = new CFunExpresion(FunOper_CAPITAL,"CAPITAL",0,g_pSystemFun->CAPITAL);    // ����: CAPITAL() ��ͨ�̴�С
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VOLUNIT,"VOLUNIT",0,g_pSystemFun->VOLUNIT);    // ����: VOLUNIT() ÿ�ֹ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DYNAINFO,"DYNAINFO",1,g_pSystemFun->DYNAINFO);    // ����:  DYNAINFO(3) ��̬���麯��: ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_FINANCE,"FINANCE",1,g_pSystemFun->FINANCE);    // ����:  FINANCE(1) ����ָ��: �ܹɱ�(���)
	pExternFunction->SetAt(pFun->GetFunName(),pFun);	

	// רҵ��������
	pFun = new CFunExpresion(FunOper_PROFFIN,"PROFFIN",2,g_pSystemFun->PROFFIN);    // רҵ��������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun->AddStyle(HX_UNKNOWFUNPARAM);

	pFun = new CFunExpresion(FunOper_PROFFINON,"PROFFINON",1,g_pSystemFun->PROFFINON);    // ָ�����ڵ�רҵ��������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun->AddStyle(HX_UNKNOWFUNPARAM);
/*
	pFun = new CFunExpresion(FunOper_PROFSTR,"PROFSTR",2,g_pSystemFun->PROFSTR);    // רҵ�����ַ�������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PROFFINLAST,"PROFFINLAST",1,g_pSystemFun->PROFFINLAST);    // רҵ�������ݽ�ֹ��λ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	pFun = new CFunExpresion(FunOper_PROFSTRON,"PROFSTRON",3,g_pSystemFun->PROFSTRON);    // ָ�����ڵ�רҵ�ַ�����������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PROFFINTERM,"PROFFINTERM",1,g_pSystemFun->PROFFINTERM);    // רҵ�������ݱ�����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SETPROFFIN,"SETPROFFIN",1,g_pSystemFun->SETPROFFIN);    // ����רҵ��������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
*/

	// ѭ��
/*
	pFun = new CFunExpresion(FunOper_FOR,"FOR",3,g_pSystemFun->FOR);    // for
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun->AddStyle(HX_UNKNOWFUNPARAM);

	pFun = new CFunExpresion(FunOper_CONTINUE,"CONTINUE",0,g_pSystemFun->CONTINUE);    // continue
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BREAK,"BREAK",0,g_pSystemFun->BREAK);    // break
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_OPERATOR,"OPERATOR",3,g_pSystemFun->OPERATOR);    // break
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
*/	 
	// �ַ���
	pFun = new CFunExpresion(FUNOPER_STROUT,"STROUT",1,g_pSystemFun->STROUT);    // STROUT
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun->AddStyle(HX_UNKNOWFUNPARAM);

	// ����ת�����ַ���
	pFun = new CFunExpresion(FUNOPER_DATETOSTR,"DATETOSTR",2,g_pSystemFun->DATETOSTR);    // STROUT
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun->AddStyle(HX_UNKNOWFUNPARAM);
	
	// ����ת�����ַ���
	pFun = new CFunExpresion(FUNOPER_BLOCKCALC,"BLOCKCALC",2,g_pSystemFun->BLOCKCALC);    // STROUT
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun->AddStyle(HX_UNKNOWFUNPARAM);
	
	

	// ��������
	      // �������ͳ���
	pFun = new CFunExpresion(FunOper_STICK,"STICK",0,g_pSystemFun->STICK);  // ��״�ߣ���0��ָ��ֵ�仭��ֱֱ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORSTICK,"COLORSTICK",0,g_pSystemFun->COLORSTICK);  // ��ɫ��״�ߣ���ֵΪ0����ʾ��ɫ��������ʾ��ɫ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VOLSTICK,"VOLSTICK",0,g_pSystemFun->VOLSTICK);  // �ɽ�����״�ߣ����ɼ�����ʱ��ʾ��ɫ��������������ʾ��ɫ������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LINESTICK,"LINESTICK",0,g_pSystemFun->LINESTICK);  // ͬʱ������״�ߺ�ָ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CROSSDOT,"CROSSDOT",0,g_pSystemFun->CROSSDOT);  // С����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CIRCLEDOT,"CIRCLEDOT",0,g_pSystemFun->CIRCLEDOT);  // СԲȦ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_POINTDOT,"POINTDOT",0,g_pSystemFun->POINTDOT);  // СԲ����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DRAW3D,"DRAW3D",0,g_pSystemFun->DRAW3D);    // 3d
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LINEDRAW3D,"LINEDRAW3D",0,g_pSystemFun->LINEDRAW3D);    // 3d����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(FunOper_NOTDRAWLINE,"NOTDRAWLINE",0,g_pSystemFun->NOTDRAWLINE);    // ��ǰ���߲�����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_NOTDRAWTEXT,"NOTDRAWTEXT",0,g_pSystemFun->NOTDRAWTEXT);    // ��ǰ�ı�������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_NOTDRAWLINETEXT,"NOTDRAWLINETEXT",0,g_pSystemFun->NOTDRAWLINETEXT); // ��ǰ�����ı���������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_NOTDRAW,"NOTDRAW",0,g_pSystemFun->NOTDRAW); // ������ͼ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(FunOper_NOTTABLE,"NOTTABLE",0,g_pSystemFun->NOTTABLE); // ������ͼ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	//
	pFun = new CFunExpresion(FunOper_NOTMAXMIN,"NOTMAXMIN",0,g_pSystemFun->NOTMAXMIN); // �����������Сֵ,�ڻ�ͼʱʹ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	
	// ���K����ɫ
	pFun = new CFunExpresion(FunOper_COLORRED,"COLORRED",0,g_pSystemFun->COLORRED);  // ��ɫ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORGREEN,"COLORGREEN",0,g_pSystemFun->COLORGREEN);  // ��ɫ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORBLUE,"COLORBLUE",0,g_pSystemFun->COLORBLUE);  // ��ɫ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORBLACK,"COLORBLACK",0,g_pSystemFun->COLORBLACK);  // ��ɫ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORWHITE,"COLORWHITE",0,g_pSystemFun->COLORWHITE);  // ��ɫ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORGRAY,"COLORGRAY",0,g_pSystemFun->COLORGRAY);  // ��ɫ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORYELLOW,"COLORYELLOW",0,g_pSystemFun->COLORYELLOW);  // ��ɫ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORCYAN,"COLORCYAN",0,g_pSystemFun->COLORCYAN);  // ��ɫ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORMAGENTA,"COLORMAGENTA",0,g_pSystemFun->COLORMAGENTA);  // Ʒ��ɫ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORBROWN,"COLORBROWN",0,g_pSystemFun->COLORBROWN);  // ��ɫ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(FunOper_COLORBRGB,"RGB",3,g_pSystemFun->hxRGB);  // RGB����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	// �߷����
	pFun = new CFunExpresion(FunOper_SOLID,"SOLID",0,g_pSystemFun->hxSOLID);
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DASH,"DASH",0,g_pSystemFun->hxDASH);		     /* -------  */
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DOT,"DOT",0,g_pSystemFun->hxDOT);			     /* .......  */
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DASHDOT,"DASHDOT",0,g_pSystemFun->hxDASHDOT);		 /* _._._._  */
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DASHDOTDOT,"DASHDOTDOT",0,g_pSystemFun->hxDASHDOTDOT); /* _.._.._  */
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	// �û���ͼ����
	pFun = new CFunExpresion(FunOper_DRAWICON,"DRAWICON",3,g_pSystemFun->DRAWICON);
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DRAWLINE,"DRAWLINE",5,g_pSystemFun->DRAWLINE);
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DRAWTEXT,"DRAWTEXT",3,g_pSystemFun->DRAWTEXT);
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_POLYLINE,"POLYLINE",2,g_pSystemFun->POLYLINE);
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_STICKLINE,"STICKLINE",5,g_pSystemFun->STICKLINE);
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	// �����ڲ����㺯��
	pFun = new CFunExpresion(-1,"QIANGDU",3,g_pSystemFun->QIANGDU); // n��ǿ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"ZHANGDIEFUDU",2,g_pSystemFun->ZHANGDIEFUDU); // �ǵ�����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"HUANSHOULV",2,g_pSystemFun->HUANSHOULV); // ������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"LIANGBIFUDU",2,g_pSystemFun->LIANGBIFUDU); // ���ȷ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"ZHENGDANGFUDU",2,g_pSystemFun->ZHENGDANGFUDU); // �𵴷���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(-1,"BETWEENSHICHANGBILI",0,g_pSystemFun->BetweenShiChangBiLi); // �����г�����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENOPEN",0,g_pSystemFun->BetweenOpen); // ���俪��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENCLOSE",0,g_pSystemFun->BetweenClose); // ��������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENHIGH",0,g_pSystemFun->BetweenHigh); // �������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENLOW",0,g_pSystemFun->BetweenLow); // �������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENVOL",0,g_pSystemFun->BetweenVol); // ����ɽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENAMOUNT",0,g_pSystemFun->BetweenAmount); // ����ɽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"CENTERPRICE",0,g_pSystemFun->CenterPrice); // ���ļ�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENBEGINDATE",0,g_pSystemFun->BetweenBeginDate); // ��ʼʱ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENENDDATE",0,g_pSystemFun->BetweenEndDate); // ����ʱ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(-1,"BK_ZHANGDIEFUDU",0,g_pSystemFun->BK_ZHANGDIEFUDU); // �ǵ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_Q_ZHANGDIEFUDU",0,g_pSystemFun->BK_Q_ZHANGDIEFUDU); // Ȩ�ǵ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_VOL",0,g_pSystemFun->BK_Vol); // �ܳɽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_AMOUNT",0,g_pSystemFun->BK_Amount); // �ɽ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_DEAL",0,g_pSystemFun->BK_Deal); // �ɽ�����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_HUANSHOULV",0,g_pSystemFun->BK_HuanShouLv); // ������
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_SHIYING",0,g_pSystemFun->BK_ShiYing); // ��ӯ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_LINGZHANG",0,g_pSystemFun->BK_LingZhang); // ���ǹ�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_LINGDIE",0,g_pSystemFun->BK_LingDie); // �����
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_NUM",0,g_pSystemFun->BK_Num); // ��Ʊ��
	pExternFunction->SetAt(pFun->GetFunName(),pFun);	
	pFun = new CFunExpresion(-1,"BK_UPRATIO",0,g_pSystemFun->BK_UpRatio); // �ǵ���
	pExternFunction->SetAt(pFun->GetFunName(),pFun);	

	pFun = new CFunExpresion(-1,"HLTCH",4,g_pSystemFun->HLTCH);   // ����ͨ-�ʺ�
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(-1,"HLTHBQ",4,g_pSystemFun->HLTHBQ); // ����ͨ-���Ȧ
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	

	//
	//litao test----------------------------------------------------------------------------------------
	//g_pSystemFun->m_hrATIndicator = g_pSystemFun->m_iATIndicator.CoCreateInstance(__uuidof(ATIndicator));
	//--------------------------------------------------------------------------------------------------
}

BOOL SaveLoadMap(BOOL bSave,
							  CMapVariabile*& pExternExpression,
							  CMapVariabile* pExternVariabile,
							  CMapVariabile* pExternFunction,
							  CComboBox* pBox /*= NULL*/,
							  char* strFileName/*= ""*/,
							  HSFunAddDefaultTechAnysis pFun /*= NULL*/,
							  HSFunAddToTree pFunAyTree /*= NULL*/)
{
	CFile fp;
	CString strCurFile;   
	if(strFileName == NULL)
		strCurFile.Format("%sCompileFile.exp",CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH));//setting
	else
		strCurFile.Format("%s",strFileName);
	BOOL bOpen = FALSE;
	if(bSave) // �洢
	{
		if(pExternExpression->GetCount() <= 0) 
			return FALSE;

		try
		{
			CString strTempPath;
			GetTempPath (MAX_PATH, strTempPath.GetBuffer (MAX_PATH));
			strTempPath.ReleaseBuffer();
			
			CString strTempName;
			GetTempFileName (strTempPath, _T("tmp"), 0, strTempName.GetBuffer (MAX_PATH));
			strTempName.ReleaseBuffer ();
			
			bOpen = fp.Open( strTempName, CFile::typeBinary | CFile::shareDenyNone | 
										  CFile::modeCreate | CFile::modeReadWrite );
			if(!bOpen) 
			{
				return FALSE;
			}
			
			CArchive ar( &fp, CArchive::store );
			
			int nCountVar = pExternExpression->GetCount();
			ar << nCountVar;
			POSITION pos = pExternExpression->GetStartPosition();
			while (pos != NULL)
			{
				CString sName;
				CValue *pVal;
				pExternExpression->GetNextAssoc (pos, sName, pVal);
				if(pVal != NULL)
				{
					ar << sName;
					pVal->Serialize(ar);

					if(pFunAyTree != NULL) // ��tree�б��浽�ļ�
					{
						(*pFunAyTree)(pVal,(void*)&ar,'-');
					}
				}
			}
			
			ar.Close();

			bOpen = fp.Open( strTempName, CFile::typeBinary | CFile::shareDenyNone | CFile::modeReadWrite );
			if(!bOpen) 
			{
				return FALSE;
			}

			ULONGLONG lLen = fp.GetLength();
			if(lLen > 0)
			{
				char* pData = new char[lLen];
				fp.Read(pData,lLen);
				CFile curFile;
				if( curFile.Open( strCurFile, CFile::typeBinary | CFile::shareDenyNone | 
											  CFile::modeCreate | CFile::modeReadWrite ))
				{
					curFile.Write(pData,lLen);
					curFile.Close();
				}
				delete[] pData;
			}

		}
		catch(...)
		{
		}
	}
	else // װ��
	{
		try
		{
			CFileException fileEx;
			bOpen = fp.Open( strCurFile, CFile::typeBinary | CFile::shareDenyNone | 
				CFile::modeReadWrite, &fileEx); 		
			if(!bOpen) 
			{
				return FALSE;
			}
			
			CArchive ar( &fp, CArchive::load );		
			ULONGLONG lLen = fp.GetLength();
			
			int nCountVar;
			ar >> nCountVar;
			for(short i = 0; i < nCountVar;i++)
			{
				CString strName;
				CExpValue* pValue = new CExpValue;			
				CExpression* pExpresie = new CExpression(pExternExpression,
					pExternVariabile,pExternFunction);
				pValue->SetValue(pExpresie);
				
				ar >> strName;
				pExpresie->SetName(strName);
//				if( !pExpresie->IsStyle(HS_EXP_USES) && !pExpresie->IsStyle(HS_EXP_DELETE) )
//					 pExpresie->AddStyle(HS_EXP_USES);
				pValue->Serialize(ar);
				
				if(pBox != NULL)
				{
					pBox->AddString(strName);
				}
				
				CValue* valoare;
				if ( pExternExpression->Lookup(strName,valoare) )
				{
					CExpression* pExp = valoare->GetExp();
					pExternExpression->RemoveKey( (char*)LPCSTR(strName),pExp );
					delete valoare;
				}
				(*pExternExpression)[strName] = pValue;
				if( pExpresie->IsStyle(HS_EXP_DEFAULT_TECH) && pFun != NULL)
				{
					(*pFun)(pValue,FALSE,NULL,'+');
				}
				if(pFunAyTree != NULL) // ���뵽tree
				{
					(*pFunAyTree)(pValue,(void*)&ar,'+');
				}
			}
		}
		catch(...)
		{
		}
	}

	return TRUE;
}

void SetHxDataSource(class CHSDataSource* pData,CArray<ColorProperty*,ColorProperty*>* pDefColours)
{
	g_pHxDataSource = pData;
	g_pColours      = pDefColours;
}

void CExpression::SetArbore(CExpression* pThisExp,LPNODELEMENT Arbore) 
{
	if(m_Arbore != NULL)
	{
		elibmem( m_Arbore );
		m_Arbore = NULL;
	}

	m_pExpDraw   = DeleteVarsMap(m_pExpDraw,TRUE);
	m_pParamVar  = DeleteVarsMap(m_pParamVar,TRUE);

	m_pDataSource   = pThisExp->m_pDataSource;

	m_pRefExpDraw   = pThisExp->m_pExpDraw;			 // �ڲ���ͼ��
	m_pRefParamVar  = pThisExp->m_pParamVar;		 // �ڲ�������

//	if( IsStyle(HX_EXPRESSREFERENCE) )
//	{
	m_pRefExpress	= pThisExp;						 // �������ʽ
//	}

	m_nExpressType  = pThisExp->m_nExpressType;

	m_Arbore = Arbore;
}

int CExpression::CalcValue(CLineProperty*& pRefData,int nStart,int nEnd,
							HSCurDataInfoStatus* pCurDataStatus /*= NULL*/,
							CTValues<void>* pRefCalcValues,CArray<CLineProperty*,CLineProperty*>* pAyData /*= NULL*/)
{

	if( pRefData == NULL || 
		nStart < 0 )
		return 0;
	HSCurDataInfoStatus* status;
	try
	{
		status = new HSCurDataInfoStatus();
  		if ( pRefData->m_pDataSource )
    			status->m_pDataSource = (CHSDataSource*)(pRefData->m_pDataSource); // �趨����Դ
 		if ( pRefData->m_pCalcParam )
  			status->m_pCalcParam  = pRefData->m_pCalcParam;

		m_nEnd = nEnd;
		m_pAyData = pAyData;

		g_dwRetValoare = hx_DefMinValues;
		if( Value(g_dwRetValoare,status) < 0  )
		{
			status->Free();
			delete status;
			return 0;
		}

		pRefData->m_dwStyle |= (status->m_dwStatus & HX_USERDRAWFUN_EXPRESS);  // ��ͼ����

		if( status->m_pData == NULL )
		{
			if( (long)g_dwRetValoare != hx_DefMinValues )
			{
				if( pRefData->m_pData == NULL && nStart == 0 && nEnd == 0 ) // ���ʽ����
				{
				}
				else
				{
					HSDouble dd;
					dd.m_dData = g_dwRetValoare;

					for(; nStart < nEnd; nStart++)
					{
						pRefData->m_pData[nStart].Copy(&dd);// = g_dwRetValoare;					
					}
				}
			}
			status->Free();
			delete status;
			return 1;
		}

		if( pRefData->m_pData == NULL && nStart == 0 && nEnd == 0) // ���ʽ����
		{
			if( pCurDataStatus )
			{
				pCurDataStatus->Alloc( status->m_lSize );
			}

			if( pRefCalcValues )
			{
				pRefCalcValues->m_lStart = 0;
				pRefCalcValues->m_lSize  = status->m_lSize;
			}

			pRefData->Alloc(status->m_lSize,0);
			for(int i = 0;i < status->m_lSize; i++)
			{
				if( status->m_pData[i].m_dData != hx_DefMinValues )
				{
					if( pCurDataStatus )
					{
						pCurDataStatus->m_pData[i].Copy(&status->m_pData[i],0 );
					}
					pRefData->m_pData[i].Copy( &status->m_pData[i],0 );				
				}
			}
		}
		else
		{
			if( status->m_lSize != nEnd )
			{
				// ֻ�ܶԲ������ݲ�����������
				for(; nStart < status->m_lSize && nStart < nEnd; nStart++)
				{
					if(status->m_pData[nStart].m_dData != hx_DefMinValues)
					{
						pRefData->m_pData[nStart].Copy( &status->m_pData[nStart],TRUE );
						
					}
				}
			}
			else
			{
				for(; nStart < nEnd; nStart++)
				{
 					if(status->m_pData[nStart].m_dData != hx_DefMinValues)
 					{
						pRefData->m_pData[nStart].Copy(&(status->m_pData[nStart]),TRUE );
					}
				}
			}
		}
 		status->Free();
 		delete status;
	}
	catch(...)
	{
		;
	}
	return 1;
}

LPNODELEMENT CExpression::GetVarFromTree(LPNODELEMENT pTree,CString strName) 
{
	LPNODELEMENT pRet = pTree;
	if( pTree != NULL && !strName.IsEmpty())
	{		
		if( pTree->valoarestr != NULL )
		{
			if( !strName.CompareNoCase(*pTree->valoarestr) )
			{
				return pRet;
			}
		}

		return GetVarFromTree(pTree->left,strName);
		return GetVarFromTree(pTree->right,strName);		
	}

	return NULL;
}

CMapVariabile* CExpression::GetParamVar()
{
	return ((m_pParamVar != NULL)?m_pParamVar:m_pRefParamVar);
}

int CExpression::GetParamVarCount()
{
	CMapVariabile* pParamVar = GetParamVar();
	if(pParamVar != NULL)
	{
		return pParamVar->GetCount();
	}
	return 0;
}

CSystemFun* CExpression::GetSystemFunAddr()
{
	return g_pSystemFun;
}

static CStringArray g_strCurDefEidolon;
CStringArray* CExpression::GetDefEidolon()
{
	g_strCurDefEidolon.RemoveAll();

	CMapVariabile* pParamVar = ((m_pParamVar != NULL)?m_pParamVar:m_pRefParamVar);
	if(pParamVar != NULL)
	{
		g_strCurDefEidolon.Add(" �����ü��������");
		CNumericValue* pValue;
		CString strName,strMin,strMax,strDef,strTmp1,strTmp2;
		for(int nPos = 0; nPos < pParamVar->GetCount(); nPos++)
		{
			pValue = (CNumericValue*)pParamVar->GetAt(nPos);
			if( pValue != NULL && pValue->IsStyle(HX_EXPRESSNUMBER) )
			{
				strName.Format("%s:",pParamVar->GetObjectName(pValue));
				strMin.Format("%f",pValue->GetMinValue());
				strMin = hxSubZero(strMin);
				strMax.Format("%f",pValue->GetMaxValue());
				strMax = hxSubZero(strMax);

				strTmp1.Format("%sParam#%i(%s-%s)",strName,nPos+1,strMin,strMax);
				g_strCurDefEidolon.Add(strTmp1);
			}
		}
	}
	return &g_strCurDefEidolon;
}

BOOL CExpression::IsFind(CString strKey,CString& strRet,
						 DWORD dMask /*= CExpression::FindInName | CExpression::FindInDescribe*/)
{
	int nUnique = 1;
	BOOL bRet = 0,bCountinue = 0;
	CString strText;
	strRet = "";
	if( (dMask & FindInName) ) // ����
	{
		strText = GetName();
		strText.MakeUpper();
		bCountinue = (strText.Find(strKey) != -1);
		if(!nUnique)
		{
			nUnique = !strText.CompareNoCase(strKey);
		}
		strText = this->GetWholeName();//ȡȫ��
	//	strRet = strText;
		if( bCountinue )
		{
	//		strRet += "-";
	//		strRet += "��ʽȫ��";
			bRet = bCountinue;
		}
	}
	if( (dMask & FindInDescribe) ) // ���ʽ����
	{
		strText = GetDescribe();
		if( !bCountinue )
		{
			strText.MakeUpper();
			bCountinue = (strText.Find(strKey) != -1);
			if(!nUnique)
			{
				nUnique = !strText.CompareNoCase(strKey);
			}
		}
	//	strRet += strText;
		if(bCountinue)
		{
	//		strRet += "-";
	//		strRet += "��ʽ����";
			bRet = bCountinue;
		}
	}
	if( (dMask & FindInNotes) ) // ���ʽע��
	{
		strText = GetNotes();
		if( !bCountinue )
		{
			strText.MakeUpper();
			bCountinue = (strText.Find(strKey) != -1);
			if(!nUnique)
			{
				nUnique = !strText.CompareNoCase(strKey);
			}
		}
	//	strRet += strText;
		if(bCountinue)
		{
	//		strRet += "-";
	//		strRet += "��ʽע��";
			bRet = bCountinue;
		}
	}
	if( (dMask & FindInEidolon) ) // ��������
	{
		strText = GetEidolon();
		if( !bCountinue )
		{
			strText.MakeUpper();
			bCountinue = (strText.Find(strKey) != -1);
			if(!nUnique)
			{
				nUnique = !strText.CompareNoCase(strKey);
			}
		}
	//	strRet += strText;
	//	if(bCountinue)
		{
			strRet += "-";
			strRet += "��������";
			bRet = bCountinue;
		}
	}
	if( (dMask & FindInSource) ) // Ҫ������ַ���
	{
		strText = GetSource();
		if( !bCountinue )
		{
			strText.MakeUpper();
			bCountinue = (strText.Find(strKey) != -1);
			if(!nUnique)
			{
				nUnique = !strText.CompareNoCase(strKey);
			}
		}
	//	strRet += strText;
		if(bCountinue)
		{
	//		strRet += "-";
	//		strRet += "��ʽ���ʽ";
			bRet = bCountinue;
		}
	}
	strRet = GetWholeName();
	strRet += "��ʽȫ��";
	strRet += GetDescribe();
	strRet += "��ʽ����";
	strRet += GetNotes();
	strRet += "��ʽע��";
	strRet += GetEidolon();
	strRet +=  "��������";
	strRet += GetSource();
	strRet += "��ʽ���ʽ";
	
	return bRet;
}

BOOL CExpression::IsFind(CString strKey,int& nUnique,
						 DWORD dMask/* = CExpression::FindInName | CExpression::FindInDescribe*/)
{
	if( (dMask & 0xFF) != m_nExpressType )
		return 0;

	BOOL bRet = FALSE;
	CString strText;
	if( (dMask & FindInName) ) // ����
	{
		strText = GetName();
		strText.MakeUpper();
		bRet = (strText.Find(strKey) != -1);
		if(!nUnique)
		{
			nUnique = !strText.CompareNoCase(strKey);
		}
		if(bRet)
			return bRet;
	}
	if( (dMask & FindInDescribe) ) // ���ʽ����
	{
		strText = GetDescribe();
		strText.MakeUpper();
		bRet = (strText.Find(strKey) != -1);
		if(!nUnique)
		{
			nUnique = !strText.CompareNoCase(strKey);
		}
		if(bRet)
			return bRet;
	}
	if( (dMask & FindInNotes) ) // ���ʽע��
	{
		strText = GetNotes();
		strText.MakeUpper();
		bRet = (strText.Find(strKey) != -1);
		if(!nUnique)
		{
			nUnique = !strText.CompareNoCase(strKey);
		}
		if(bRet)
			return bRet;
	}
	if( (dMask & FindInEidolon) ) // ��������
	{
		strText = GetEidolon();
		strText.MakeUpper();
		bRet = (strText.Find(strKey) != -1);
		if(!nUnique)
		{
			nUnique = !strText.CompareNoCase(strKey);
		}
		if(bRet)
			return bRet;
	}
	if( (dMask & FindInSource) ) // Ҫ������ַ���
	{
		strText = GetSource();
		strText.MakeUpper();
		bRet = (strText.Find(strKey) != -1);
		if(!nUnique)
		{
			nUnique = !strText.CompareNoCase(strKey);
		}
		if(bRet)
			return bRet;
	}
	return bRet;
}

BOOL CExpression::IsValidateParam(CFunExpresion* pFun,LPNODELEMENT pTree,FormulaGeneralParam* ayVars)
{
	if( pFun   == NULL || 
		ayVars == NULL )
		return FALSE;

	if( pFun->IsStyle(HX_UNKNOWFUNPARAM) )
		return (pFun->GetParams() <= ayVars->GetSize());

	return (pFun->GetParams() == ayVars->GetSize());
}

BOOL CExpression::IsValidateParam(CExpression* pExpression,LPNODELEMENT pTree,FormulaGeneralParam* ayVars)
{
	if( pExpression == NULL || 
		ayVars == NULL )
		return FALSE;

	return ( pExpression->GetParamVarCount() == ayVars->GetSize() );
}

int CExpression::IsAddSign(BYTE cChar,const CString& strSource,int& nPos)		// +
{
	if( cChar == '+')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsSubSign(BYTE cChar,const CString& strSource,int& nPos)		// -
{
	if( cChar == '-')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsRideSign(BYTE cChar,const CString& strSource,int& nPos)		// *
{
	if( cChar == '*')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsDivideSign(BYTE cChar,const CString& strSource,int& nPos)	// /
{
	if( cChar == '/')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsNot(BYTE cChar,const CString& strSource,int& nPos)			// !
{
	if( cChar == '!')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsPower(BYTE cChar,const CString& strSource,int& nPos)			// ^
{
	if( cChar == '^')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsAnd(BYTE cChar,const CString& strSource,int& nPos)			// &
{
	if( cChar == '&')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsBracketLeft(BYTE cChar,const CString& strSource,int& nPos)   // (
{
	if( cChar == '(')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsBracketRight(BYTE cChar,const CString& strSource,int& nPos)  // )	
{
	if( cChar == ')')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsOr(BYTE cChar,const CString& strSource,int& nPos)			// |	
{
	if( cChar == '|')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsSemicolon(BYTE cChar,const CString& strSource,int& nPos)		// ;
{
	if( cChar == ';')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}

	return 0;
}

int CExpression::IsKomma(BYTE cChar,const CString& strSource,int& nPos)			// ,
{
	if( cChar == ',')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}

	return 0;
}

int CExpression::IsColon(BYTE cChar,const CString& strSource,int& nPos)			// :
{
	if( cChar == ':')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsGreatLess(BYTE cChar,const CString& strSource,int& nPos)		// >
{
	if( cChar == '>')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") ||
			!strKey.CompareNoCase("��") ||
			!strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsLessThan(BYTE cChar,const CString& strSource,int& nPos)		// <
{
	if( cChar == '<')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") ||
			!strKey.CompareNoCase("��") ||
			!strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsEqual(BYTE cChar,const CString& strSource,int& nPos)			// =
{
	if( cChar == '=')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsDot(BYTE cChar,const CString& strSource,int& nPos)	// .
{
	if( cChar == '.')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") ||
			!strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::Is$(BYTE cChar,const CString& strSource,int& nPos)			// $
{
	if( cChar == '$')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsWell(BYTE cChar,const CString& strSource,int& nPos)		// #
{
	if( cChar == '#')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsChineseSpace(BYTE cChar,const CString& strSource,int& nPos)	// ȫ�ǿո�
{
	if( cChar == ' ' )
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			return 2;
		}
	}
	return 0;
}

int CExpression::IsInvertedComma(BYTE cChar,const CString& strSource,int& nPos)	// "
{
	if( cChar == '\"')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") ||
			!strKey.CompareNoCase("��") ||
			!strKey.CompareNoCase("��"))
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsText(BYTE cChar,const CString& strSource,int& nPos)	// ' �ַ���
{
	if( cChar == '\'')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") ||
			!strKey.CompareNoCase("��") ||
			!strKey.CompareNoCase("��"))
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

BOOL CExpression::IsEnd()
{
	return ( (m_definitie.GetLength() <= m_nPozitie+1) ||
		    ( m_definitie[m_nPozitie]   == '\0' && 
		      m_definitie[m_nPozitie+1] == '\0' ) );
}

int CExpression::GetImageIndex()
{
	if( IsStyle(HS_EXP_DEFAULT_TECH) )
		return 3;
	return 4;
}

void CExpression::SetData(InportDataParam* pDataSource)
{
	m_pDataSource = pDataSource;
}

CHSDataSource* CExpression::GetData()
{
	if( m_pDataSource != NULL )
	{
		return m_pDataSource->m_pDataSrc;
	}
	return NULL;
}

BOOL CExpression::IsPeriod(CString strPeriod)
{
	return ( !strPeriod.CompareNoCase("MIN1")  ||
		     !strPeriod.CompareNoCase("MIN5")  ||
		     !strPeriod.CompareNoCase("MIN15") ||
			 !strPeriod.CompareNoCase("MIN30") ||
			 !strPeriod.CompareNoCase("MIN60") ||
			 !strPeriod.CompareNoCase("DAY")   ||
			 !strPeriod.CompareNoCase("WEEK")  ||
			 !strPeriod.CompareNoCase("MONTH") );
}

CString CExpression::GetParamString(CExpression* pExpress)
{
	if(pExpress == NULL) return "";
	CMapVariabile* pRefParamVar = pExpress->GetParamVar();
	if( pRefParamVar == NULL )
		return "";

	CArray<class CValue*,class CValue*>* pAyrray = pRefParamVar->GetObjArray();
	CValue* pCurVal;	
	CString strParamString,strTmp;
	if(pAyrray != NULL)
	{
		for(int nObj = 0; nObj < pAyrray->GetSize(); nObj++)
		{
			pCurVal = pAyrray->GetAt(nObj);
			if( pCurVal != NULL )
			{
				strTmp.Format( "%d",(long)pCurVal->GetValue(NULL) );
				strParamString += strTmp;
				if( nObj != pAyrray->GetSize() - 1 )
					strParamString += ",";
			}
		}
	}
	return strParamString;
}

static CString m_strParamString;
CString CExpression::GetParamString()
{
	m_strParamString.Empty();

	CMapVariabile* pVar = GetExpDraw();
	if(pVar == NULL)
		return m_strParamString;
	
	if( pVar != NULL &&
		pVar->GetObjArray() != NULL )
	{
		CExpValue* pCurVal;      // ��ǰ��ͼ���ʽ
		
		short nDrawCount;
		pVar->GetDrawCount(nDrawCount);
		if(nDrawCount == 0)
		{
			return m_strParamString;
		}
		
		int nDrawParamCount = pVar->GetObjArray()->GetSize();
		for(int nObj = 0; nObj < nDrawParamCount; nObj++)
		{
			pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
			if( pCurVal != NULL )
			{
				if(nObj == 0)
				{
					CExpression* pExpress = pCurVal->GetExp();
					if( pExpress != NULL )
					{
						m_strParamString = GetParamString(pExpress);
						return m_strParamString;
					}
				}
			}
		}
	}
	return m_strParamString;
}

int CExpression::GetDraw(CString& strParam,CArray<CFormulaDrawParam*,CFormulaDrawParam*>& ayDraw)
{
	CMapVariabile* pVar = GetExpDraw();
	if(pVar == NULL)
		return 0;

	try
	{
		if( pVar != NULL &&
			pVar->GetObjArray() != NULL )
		{
			CExpValue* pCurVal;      // ��ǰ��ͼ���ʽ
			CExpValue* pCurParamVal; // ��ͼ����
			int		   nPar;
			HSDouble   lValue;
			short	   nParamSize = 0;
			CFormulaDrawParam* pData;
			CString    strTmp;
			
			short nDrawCount;
			pVar->GetDrawCount(nDrawCount);
			if(nDrawCount == 0)
			{
				return 0;
			}
			
			int nDrawParamCount = pVar->GetObjArray()->GetSize();
			for(int nObj = 0; nObj < nDrawParamCount; nObj++)
			{
				pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
				if( pCurVal != NULL )
				{
					if(nObj == 0)
					{
						CExpression* pExpress = pCurVal->GetExp();
						if( pExpress != NULL )
						{
							strParam = GetParamString(pExpress);
						}
					}
					
					if( pCurVal->IsStyle(HX_EXPRESSDRAW) )
					{
						pData = new CFormulaDrawParam;
						ayDraw.Add(pData);
						
						pData->m_lStyle = HX_EXPRESSDRAW;
						
						// ��������
						if( pCurVal->IsStyle(HX_EXPRESSDRAW_NONAME) )
						{
							strTmp = "";
						}
						else
						{
							strTmp.Format( "%s",pVar->GetObjectName(pCurVal) );
						}
						strncpy(pData->m_strName,strTmp,sizeof(pData->m_strExpParam));
						
						// ������ֵ
						CMapVariabile* pParam = ((CExpValue*)pCurVal)->GetParam();
						if( pParam != NULL )
						{
							// ��������ͼ����;��ͼ��ɫ;�߷��;�߿��;...PS_SOLID
							nParamSize = pParam->GetObjArray()->GetSize();							
							for( nPar  = 0; nPar < nParamSize; nPar++ )
							{
								pCurParamVal = (CExpValue*)pParam->GetObjArray()->GetAt(nPar);
								lValue		 = pCurParamVal->GetValue(NULL);
								if( nPar == 3 )	  // �߿��
								{
									pData->m_nWidth = lValue;
								}
								else if( nPar == 2 ) // �߷��
								{
									pData->m_nPenStyle = lValue;
								}
								else if( nPar == 1 ) // ��ͼ��ɫ
								{
									pData->m_lColor = lValue;
								}
								else if( nPar == 0 ) // ��ͼ����
								{
									pData->m_lStyle |= (long)lValue;
								}
								else  // ��Ч
								{
									break;
								}
							}
						}
					}
				}
			}
		}
		return ayDraw.GetSize();
	}
	catch(...) {}
	return 0;
}

int CExpression::GetImgIndex(int nType)
{
	switch( nType )
	{
	case Tech: return  HS_Exp_Tech;
	case Condition: return  HS_Exp_Condition;
	case Exchange: return   HS_Exp_Exchange;
	case MoreKLine: return  HS_Exp_MoreKLine;
	case ExpressBin: return  HS_Exp_Bin;
	case Union: return HS_Exp_Union;
	}
	return HS_Exp_Tech;
}

void   CExpression::SetParamVar(CMapVariabile*  pParamVar)
{ 
	m_pParamVar = pParamVar;
}

//******************************������litao����*****************************************
//�޸ı�ʶ���к�,����1
CString CExpression::AddObjNo(CString key,int iCompExec)
{
	CString strName = "";
	LPEXECOBJNO lpObjNo;
	int iObjNo = 0;
	//----2009.11.13 litao�޸ģ����ޱ������ʽʱҲ���Ե���Ĭ�ϱ���000��001������
	//if (key.IsEmpty() )
		//return strName;
	//------------------------------------------------------------
	if (COMPILE_NO ==  iCompExec)
	{
		if (m_pCompNo.Lookup(key,iObjNo))
		{
			iObjNo++;
			m_pCompNo.SetAt(key,iObjNo);
			strName.Format("%d",iObjNo);
			if (1 == strName.GetLength()) 
				strName = key + "00" + strName;
			else if (2 == strName.GetLength()) 
				strName = key + "0" + strName;
			else
				strName = key + strName;
		}
		else
		{
			//û���ҵ������¼���
			m_pCompNo.SetAt(key,iObjNo);
			strName = key + "000";
		}
	}
	else
	{
		if (m_pExecNo.Lookup(key,lpObjNo))
		{
			lpObjNo->pcsObjNo->Lock();
			lpObjNo->iObjNo++;
			//m_pExecNo.SetAt(key,iObjNo);

			strName.Format("%d",lpObjNo->iObjNo);
			lpObjNo->pcsObjNo->Unlock();

			if (1 == strName.GetLength()) 
				strName = key + "00" + strName;
			else if (2 == strName.GetLength()) 
				strName = key + "0" + strName;
			else
				strName = key + strName;
		}
		else
		{
			//û���ҵ������¼���
			lpObjNo = new ExecObjNoList();
			lpObjNo->pcsObjNo->Lock();
			lpObjNo->iObjNo = iObjNo;
			lpObjNo->strName = key;
			m_pExecNo.SetAt(key,lpObjNo);
			lpObjNo->pcsObjNo->Unlock();
			strName = key + "000";
		}
	}
	return strName;
}

//�޸ı�ʶ���к�,�ݼ�1
CString CExpression::SubObjNo(CString key,int iCompExec)
{
	CString strName = "";
	LPEXECOBJNO lpObjNo;
	int iObjNo = 0;

	if (COMPILE_NO ==  iCompExec)
	{
		if (m_pCompNo.Lookup(key,iObjNo))
		{
			iObjNo--;
			if (iObjNo >= 0)
			{
				m_pCompNo.SetAt(key,iObjNo);

				strName.Format("%d",iObjNo);
				if (1 == strName.GetLength()) 
					strName = key + "00" + strName;
				else if (2 == strName.GetLength()) 
					strName = key + "0" + strName;
				else
					strName = key + strName;
			}
			else
			{
				m_pCompNo.RemoveKey(key);
			}
		}
	}
	else
	{
		if (m_pExecNo.Lookup(key,lpObjNo))
		{
			lpObjNo->pcsObjNo->Lock();
			lpObjNo->iObjNo--;
			if (iObjNo >= 0)
			{
				//m_pExecNo.SetAt(key,iObjNo);
				strName.Format("%d",lpObjNo->iObjNo);
				lpObjNo->pcsObjNo->Unlock();
				if (1 == strName.GetLength()) 
					strName = key + "00" + strName;
				else if (2 == strName.GetLength()) 
					strName = key + "0" + strName;
				else
					strName = key + strName;
			}
			else
			{
				lpObjNo = new ExecObjNoList();
				lpObjNo->pcsObjNo->Lock();
				lpObjNo->iObjNo = iObjNo;
				lpObjNo->strName = key;
				m_pExecNo.RemoveKey(key);
				lpObjNo->pcsObjNo->Unlock();
			}
		}
	}
	return strName;  //û���ҵ����򷵻ؿմ�
}

CString CExpression::GetObjNo(CString key,int iCompExec,int nHaveID)
{
	CString strName = "";
	LPEXECOBJNO lpObjNo;
	int iObjNo = 0;

	if (COMPILE_NO ==  iCompExec)
	{
		if (m_pCompNo.Lookup(key,iObjNo))
		{
			if (HAVE_ID == nHaveID)
				iObjNo --;
			if (iObjNo < 0)   
				iObjNo = 0;

			strName.Format("%d",iObjNo);
			if (1 == strName.GetLength()) 
				strName = key + "00" + strName;
			else if (2 == strName.GetLength()) 
				strName = key + "0" + strName;
			else
				strName = key + strName;
		}
		else
		{
			strName = key + "000";
		}
	}
	else
	{
		if (m_pExecNo.Lookup(key,lpObjNo))
		{
			if (HAVE_ID == nHaveID)
				iObjNo = lpObjNo->iObjNo--;
			if (iObjNo < 0)   
				iObjNo = 0;

			strName.Format("%d",iObjNo);

			if (1 == strName.GetLength()) 
				strName = key + "00" + strName;
			else if (2 == strName.GetLength()) 
				strName = key + "0" + strName;
			else
				strName = key + strName;
		}
		else
		{
			strName = key + "000";
		}
	}
	return strName; 
}

CValue* CExpression::GetVariableExec(CString strName,short& nRet,int nGetType /*= -1*/ )
{
	strName.MakeUpper();

	CValue* valoare;

	// ���ù�ʽ
	if( nGetType != -1 ) 
	{
		if ( m_pExternExpression != NULL && 
			m_pExternExpression->Lookup(strName,nGetType,valoare ) )	// �ⲿ���ʽ��
		{
			nRet = ExpExternExp;
			return valoare;
		}
		return NULL;
	}
	//
	if ( m_pExternVariabile != NULL && 

		m_pExternVariabile->Lookup(strName,valoare ) ) // �ⲿ������
	{
		nRet = ExpExternVar;
		return valoare;
	}
	if ( m_pExternFunction != NULL && 
		m_pExternFunction->Lookup(strName,valoare ) ) // �ⲿ������
	{
		nRet = ExpExternFun;
		return valoare;
	}	


	if ( m_pParamVar != NULL && 
		m_pParamVar->Lookup(strName,valoare ) )     // �ڲ�������
	{
		nRet = ExpParamVar;
		return valoare;
	}

	CString tmpName;
	tmpName = GetMaxObjNo(strName,EXECUTE_NO);

	if ( m_pExpDraw != NULL && 
		m_pExpDraw->Lookup(tmpName,valoare ) )      // �ڲ���ͼ��
	{
		nRet = ExpDrawObj;
		return valoare;
	}


	if(m_pRefExpress != NULL)						 // �������ʽ
	{
		return m_pRefExpress->GetVariableExec(strName,nRet,nGetType );
	}

	nRet = 0;
	return NULL;
}

CValue* CExpression::GetVariable2(CString& strName,short& nRet,int nGetType)
{
	strName.MakeUpper();

	CValue* valoare;

	// ���ù�ʽ
	if( nGetType != -1 ) 
	{
		if ( m_pExternExpression != NULL && 
			m_pExternExpression->Lookup(strName,nGetType,valoare ) )	// �ⲿ���ʽ��
		{
			nRet = ExpExternExp;
			return valoare;
		}
		return NULL;
	}
	//
	if ( m_pExternVariabile != NULL && 

		m_pExternVariabile->Lookup(strName,valoare ) ) // �ⲿ������
	{
		nRet = ExpExternVar;
		return valoare;
	}
	if ( m_pExternFunction != NULL && 
		m_pExternFunction->Lookup(strName,valoare ) ) // �ⲿ������
	{
		nRet = ExpExternFun;
		return valoare;
	}	


	if ( m_pParamVar != NULL && 
		m_pParamVar->Lookup(strName,valoare ) )     // �ڲ�������
	{
		nRet = ExpParamVar;
		return valoare;
	}

	if ( m_pExpDraw != NULL && 
		m_pExpDraw->Lookup(strName,valoare ) )      // �ڲ���ͼ��
	{
		nRet = ExpDrawObj;
		return valoare;
	}


	if(m_pRefExpress != NULL)						 // �������ʽ
	{
		return m_pRefExpress->GetVariableExec(strName,nRet,nGetType );
	}

	nRet = 0;
	return NULL;
}

void CExpression::RemoveAllNo(int iCompExec)
{
	if (COMPILE_NO == iCompExec)
		m_pCompNo.RemoveAll();
	else
	{
		CString strName = "";
		ExecObjNoList* lpObjNo = NULL;
		POSITION atpos = m_pExecNo.GetStartPosition();  
		while (atpos != NULL)
		{
			m_pExecNo.GetNextAssoc( atpos, strName, lpObjNo );
			delete(lpObjNo);
		}
		m_pExecNo.RemoveAll();
		
	}
}

LPNODELEMENT CExpression::whiledo(CValue* valoare,int nEnd)
{
	if( IsStop() ) return NULL;

	m_nIFWHILEStart++;

	LPNODELEMENT nod = NULL,nodleft = NULL,nodright = NULL,firstNod = NULL,lastNod = NULL;

	nodleft = factor();  //�õ��������ʽ�ĸ��ڵ�

	//ֱ���ҵ���β���з�
	SkipSpaces();

	//����ԭ���Ĺ�ʽ�ַ���������ѭ���ڵ�������¸�����ʽ�ַ���	
	CString OldExpDef = m_definitie;
	m_SaveExpDef.SetAt(m_SaveExpDef.GetCount(),OldExpDef);

	//��ѭ�����ڵ���丳��m_definitie��
	m_definitie = GetWhileDefinitie(m_definitie);

	int nOldpos = m_nPozitie;  //���浱ǰ�ַ�λ��
	m_nPozitie = 0;

	if (m_definitie.GetLength() < 1)
	{
		AddError( new nodErrorInfo(hxError_Fun_param,
			m_nPozitie,m_definitie.GetLength(),
			"ENDLOOP") );
		return NULL;
	}

	//�������뱾ѭ���ڲ����
	nodright = LineOp(); 

	//������ָ�ԭ�����ַ���
	m_nPozitie = nOldpos;
	m_definitie = m_SaveExpDef[m_SaveExpDef.GetCount()-1];
	m_SaveExpDef.RemoveKey(m_SaveExpDef.GetCount()-1);

	int nFirst,nLast;
	m_nWhileFirst.Lookup(m_nIFWHILEStart,nFirst);
	m_nWhileLast.Lookup(m_nIFWHILEStart,nLast);

	m_nWhileFirst.RemoveKey(m_nIFWHILEStart);
	m_nWhileLast.RemoveKey(m_nIFWHILEStart);


	//��¼ѭ���ڵĵ�һ����λ��(���ڲ���ͼ���е��±�)
	firstNod = new NODELEMENT;
	firstNod->left  = NULL; 
	firstNod->right = NULL;
	firstNod->operatie = '@';
	firstNod->valoare = nFirst+m_nIFWHILEStart;  //����WHILE������Ҫ����ѭ����ͷ�ڵ�ǰ�����Լ�1;�����ѭ��ʱ��Ҫ����ѭ�����-1��WHILE�ڵ�

	//��¼ѭ���ڵ��������λ��(���ڲ���ͼ���е��±�)	
	lastNod = new NODELEMENT;
	lastNod->left  = NULL; 
	lastNod->right = NULL;
	lastNod->operatie = '@';
	lastNod->valoare = nLast+m_nIFWHILEStart;    //����WHILE������Ҫ����ѭ����ͷ�ڵ�ǰ�����Լ�1;�����ѭ��ʱ��Ҫ����ѭ�����-1��WHILE�ڵ�

	// ������
	FunctionParam* pFunctionParam = new FunctionParam();
	if(pFunctionParam == NULL) return NULL;

	CFunExpresion* pFun = (CFunExpresion*)valoare;
	pFunctionParam->pFun = pFun;

	pFunctionParam->ayVars->InsertAt(0,lastNod);
	pFunctionParam->ayVars->InsertAt(0,firstNod);
	pFunctionParam->ayVars->InsertAt(0,nodleft);

	nod = new NODELEMENT;
	nod->left  = nodleft;
	nod->right = nodright;
	nod->pData = pFunctionParam; // ����ָ��
	nod->operatie   = Operator_Function;
	nod->cDataType  = DataTypeOperStr | DataFunParam;
	nod->valoarestr = new CString("WHILE");

	//����������ѭ���ڲ���ʼλ�ô�
	LPNODELEMENT ppClone = clone(nod);
	CExpression* pExpresie = new CExpression(m_pExternExpression,m_pExternVariabile,m_pExternFunction);
	pExpresie->SetArbore(this, ppClone );				
	CExpValue* pValue = new CExpValue;
	pValue->SetValue( pExpresie);	

	CString strName="";
	strName.Format("%d",m_nIFWHILEStart-1);
	if (1 == strName.GetLength()) 
		strName = "WHILE00" + strName;
	else if (2 == strName.GetLength()) 
		strName = "WHILE0" + strName;
	else
		strName = "WHILE" + strName;

	pValue->m_LineName = strName;
	pValue->m_InWhile = ((m_nIFWHILEStart>1)?true:false);  //��һ��ѭ������Ӧ����ѭ�������
	pValue->m_WhileLevel = m_nIFWHILEStart-1;			   //��һ��ѭ���������ӦΪ0
	pValue->m_CurExecWhileLevel = 0;

	m_pExpDraw->InsertAt(strName,pValue,nFirst);

	m_nIFWHILEStart--;

	//����ͷβλ��
	if ((m_nIFWHILEStart >  0) && (m_nWhileFirst.GetCount() < m_nIFWHILEStart))
	{
		m_nWhileFirst.SetAt(m_nIFWHILEStart, m_pExpDraw->GetObjArray()->GetSize() - 1);
	}

	//ÿ�ΰ�ͷ��Σ�����β��λ��
	if (m_nIFWHILEStart > 0)
		m_nWhileLast.SetAt(m_nIFWHILEStart, m_pExpDraw->GetObjArray()->GetSize() - 1);


	SkipSpaces();

	return nod;
}

int CExpression::IsBigBracketLeft(BYTE cChar,const CString& strSource,int& nPos)   // {
{
	if( cChar == '{')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsBigBracketRight(BYTE cChar,const CString& strSource,int& nPos)   // }
{
	if( cChar == '}')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("��") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

CString CExpression::GetWhileDefinitie(CString strSource)
{
	CString tmpWhileDef="";
	CString id="";CString id2="";
	int nBgnPos=m_nPozitie ; int nEndPos=0;
	int nWhileNum=0; //��¼�Ƿ����ѭ��Ƕ�׵������


	while (m_nPozitie < m_definitie.GetLength()-7)
	{
		//ÿ��ȡһ����ʾ
		id = m_definitie.Mid(m_nPozitie,7);
		id2 = m_definitie.Mid(m_nPozitie,5);
		if (0 == id2.CompareNoCase("WHILE"))
		{
			nWhileNum++;
		}

		if (0 == id.CompareNoCase("ENDLOOP"))
		{
			if (0==nWhileNum)
			{
				nEndPos = m_nPozitie;
				m_nPozitie = m_nPozitie + 7;
				break;
			}
			else
			{
				nWhileNum--;
			}
		}
		m_nPozitie++;
	}

	tmpWhileDef = m_definitie.Mid(nBgnPos,(nEndPos - nBgnPos));

	return tmpWhileDef;
}

CString CExpression::AddWHILE(CString strSource)
{
	CString tmpWhileDef="";
	CString id="";
	int nBgnPos=m_nPozitie ; int nEndPos=0;
	int i=0;

	while (i < m_definitie.GetLength()-5)
	{
		//ÿ��ȡһ����ʾ
		id = m_definitie.Mid(i,5);
		id.MakeUpper();
		if (0 == id.CompareNoCase("WHILE"))
			//ǰ�����"WHILE:="
			tmpWhileDef = tmpWhileDef + "WHILE:=" + m_definitie.Mid(i,1);
		else
			tmpWhileDef = tmpWhileDef + m_definitie.Mid(i,1);

		i++;
	}

	return tmpWhileDef;
}



double CExpression::ExeWhile(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus)
{
	CString strName;

	if( a->valoarestr == NULL )
	{
		if( !pCurDataStatus || pCurDataStatus->IsStatus(hx_EXCUTE_PROMPT_ERROR) )
		{
			AddError(new nodErrorInfo(hxError_NotName_Exist));
		}
		return hx_DefMinValues; // return 0;
	}
	short nRet;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	LPNODELEMENT condNod;
	condNod = clone(a->left);

	//�����������ʽ
	double v0 = vexp(condNod,&calc0);

	while (v0)
	{
		//�Ӹ��ڵ㿪ʼ����ȫ���Ĺ�ʽ���ڵ㣬����ڵ������Ϊ��ֵʱ��������ҽڵ㹫ʽ���м���
		LPNODELEMENT Nod = clone(a->right); LPNODELEMENT oldNod = clone(a->right);
		LPNODELEMENT lNod;

		while(Nod->left)
		{
			lNod = clone(Nod->left);
			if ((Nod->operatie == Operator_Semicolon) && (lNod->operatie == Operator_Semicolon))
			{
				strName = *lNod->valoarestr;



			}
			else
			{
				Nod = clone(Nod->left);
			}


			a = clone(a->left);
		}

		//ѭ������ִ����ϣ����¼����������ʽ
		double v0 = vexp(condNod,&calc0);
	}

	CValue* pValoare;
	pValoare = GetVariableExec(strName,nRet);

	return hx_DefMinValues; // return 0;

}

HSDouble CExpression::WHILE (CExpression* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	int		 nLoopNum=0;			//��ǰѭ���Ĵ���

	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pParam0  = ayVars->GetParam(0);  //�������ʽ������ڵ�
	LPNODELEMENT pParam1  = ayVars->GetParam(1);  //ѭ��������һ�������λ��
	LPNODELEMENT pParam2  = ayVars->GetParam(2);  //ѭ����������һ�������λ��

	//������/�����ж�����˳�
	//if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL ||
	//	pParam0  == NULL || pParam1  == NULL )
	//	return hx_DefMinValues;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);

	double v0 = vexp(pParam0,&calc0);
	if( v0 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}

	int N1 = -1; int N2 = -1; 
	if (NULL  != pParam1) 
		N1 = (int)vexp(pParam1,&calc1);

	if (NULL  != pParam2) 	
		N2 = (int)vexp(pParam2,&calc2);


	try
	{	
		CMapVariabile* pVar = GetExpDraw();
		//CMapVariabile* pVar = g_hxData.GetExpDraw("test1",m_strExpName.m_dStyle & 0x00FF);

		if (NULL == pVar)
			pVar = m_pRefExpress->GetExpDraw();

		if( pVar == NULL ||
			pVar->GetObjArray() == NULL )
		{
			AddError(new nodErrorInfo(hxError_Fun_param));
			return hx_DefMinValues;
		}

		if (v0 == hx_ReturnPointValues)
		{
			v0 = calc0.m_pData[0];
		}


		CArray<CLineProperty*,CLineProperty*> ayData;
		CExpValue* pCurVal;
		CLineProperty* pData;
		int nObjNum=0;
		nObjNum  = pVar->GetObjArray()->GetCount();
		if (nObjNum > N2)
		{
			//������ѭ���ĸ������ѭ�����+1��
			for(int nObj = N1; nObj <= N2; nObj++)
			{
				pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
				if( pCurVal != NULL )
				{
					pCurVal->m_CurExecWhileLevel++;  
				}
			}
		}

		while (v0)   //�����ж�Ӧ��Ϊһ��ֵ
		{
			nLoopNum++;
			if (nLoopNum > MAXLOOPTIMES) //�ж�ѭ�������Ƿ�̫��
			{
				//��ʾ����
				nError = MORELOOPERROR;
				return hx_DefMinValues;
			}

			// һ��һ���м��ʶ���м���

			for(int nObj = N1; nObj <= N2; nObj++)
			{
				pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
				if( pCurVal != NULL )
				{
					pCurVal->m_CurExecInWhile = true;
					if(nObj >= 0 && nObj < m_pAyData->GetSize())
						pData = m_pAyData->GetAt(nObj);

					if( !pData->IsEmpty() )
						pCurVal->CalcValue(pData,0,m_nEnd,NULL,false,m_pAyData);

					//ִ�������ÿ��ر�������ֹѭ���������WHILEѭ���ⱻ��ִ��һ�Ρ�
					pCurVal->m_CurExecInWhile = false;  
				}
			}

			/*
			CExpValue* pCurVal;
			CValue* pValoare ;
			LPNODELEMENT  nod;
			short nRet;

			CString strName;
			for(int nObj = N1; nObj <= N2; nObj++)
			{
			pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
			if( pCurVal != NULL )
			{
			strName = pCurVal->m_LineName;
			pValoare = GetVariableExec(strName,nRet);

			nod = ((CExpression*)(pValoare->m_value))->GetExpArbore();

			}
			}
			*/


			//���¼�������ֵ
			v0 = vexp(pParam0,&calc0);
			if (v0 == hx_ReturnPointValues)
			{
				v0 = calc0.m_pData[0];
			}
		}

		//ѭ������������ѭ���ĸ������ѭ�����-1��
		if (nObjNum > N2)
		{
			for(int nObj = N1; nObj <= N2; nObj++)
			{
				pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
				if( pCurVal != NULL )
				{
					pCurVal->m_CurExecWhileLevel--;  
				}
			}
		}
		return 0;
	}
	catch(...)
	{
	}

	return hx_DefMinValues; // return 0;

}

CString CExpression::GetMaxObjNo(CString key,int iCompExec)
{
	CString strName = "";
	CString tmpName = "";
	LPEXECOBJNO lpObjNo;
	int iObjNo = 0;

	tmpName = key.Left(key.GetLength()-3);

	if (EXECUTE_NO ==  iCompExec)
	{
		if (m_pExecNo.Lookup(tmpName,lpObjNo))
		{		
			strName.Format("%d",lpObjNo->iObjNo);
			if (1 == strName.GetLength()) 
				strName = tmpName + "00" + strName;
			else if (2 == strName.GetLength()) 
				strName = tmpName + "0" + strName;
			else
				strName = tmpName + strName;
		}
		else
		{
			strName = tmpName + "000";
		}
	}
	return strName; 
}


//******************************������litao����*****************************************
