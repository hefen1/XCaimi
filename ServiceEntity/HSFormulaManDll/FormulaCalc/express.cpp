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

// 中文符号
static CString g_strChineseSign = "！＠＃＄％＾＆＊（）｜｛｝：；＇＂“”＜＞《》〈〉，．。／？－＋＝";
// 负号判断
static CString g_strPreNegativeSign = "（，＆｜！－＝＜＞《》〈〉";
// 中文数字
static CString g_strChineseNumber = "０１２３４５６７８９．。";
// 中文字符
static CString g_strChineseChar = "ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ";

static CString g_strSpecialSign = "~!@#$%^&*()_+{}|:\"<>?`-=[]\\;\',./ ";

static CArray<unsigned char,unsigned char> g_ayAheadChar;

/*#define _countof(array) (sizeof(array)/sizeof(array[0]))*/

IMPLEMENT_DYNCREATE(CExpressionBase, CObject)

IMPLEMENT_DYNCREATE(CExpression, CExpressionBase)

//CStockManager*   CExpression::m_pStockManager = NULL;
//CHSDisk*		 CExpression::m_pHSDisk = NULL;

// 函数列表
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
	g_FunctionList[(BYTE)Operator_Semicolon] = CExpression::Semicolon ;   // ; 分号
	g_FunctionList[(BYTE)Operator_Error] = CExpression::Error ;       // 操作符错误
	g_FunctionList[(BYTE)Operator_Komma] = CExpression::Komma ;      // ] = 逗号操作(一行可有多条语句，按逗号分解)
	g_FunctionList[(BYTE)Operator_Colon] = CExpression::Colon ;      // 绘图操作(此项操作完成在窗口上绘图的表达式)
	g_FunctionList[(BYTE)Operator_Evaluate] = CExpression::Evaluate ;   // 赋值操作(完成左边变量赋值] = 可以是表达式，也可以是数值] = 并且将赋值后表达式加入变量或函数列表)			
	g_FunctionList[(BYTE)Operator_Function] = CExpression::Function ;  // 函数操作
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
	g_mapFunList.SetAt((WORD)Operator_Semicolon,CExpression::Semicolon );   // ; 分号
	g_mapFunList.SetAt((WORD)Operator_Error,CExpression::Error );       // 操作符错误
	g_mapFunList.SetAt((WORD)Operator_Komma,CExpression::Komma );      // ,逗号操作(一行可有多条语句，按逗号分解)
	g_mapFunList.SetAt((WORD)Operator_Colon,CExpression::Colon );      // 绘图操作(此项操作完成在窗口上绘图的表达式)
	g_mapFunList.SetAt((WORD)Operator_Evaluate,CExpression::Evaluate );   // 赋值操作(完成左边变量赋值,可以是表达式，也可以是数值,并且将赋值后表达式加入变量或函数列表)			
	g_mapFunList.SetAt((WORD)Operator_Function,CExpression::Function );  // 函数操作
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
	case Strategy: //20090302 YJT 增加 Strategy
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

//消息注册ID
 UINT HX_USER_COMPILEDATA = ::RegisterWindowMessage(HS_COMPILE_DATA);
 UINT HX_USER_UPDATEDATA  = ::RegisterWindowMessage(HS_COMPILE_UPDATEDATA);
 UINT HX_USER_DRAGDATA    = ::RegisterWindowMessage(HS_COMPILE_DRAGDATA);
 UINT EXPRESS_HS_FIND_DATA_MSG = ::RegisterWindowMessage(HS_FIND_DATA_MSG);
 UINT EXPRESS_ZiXun_Data_Request = ::RegisterWindowMessage(HS_ZiXun_Data_Request);
 UINT m_HS_DOWNLOADDATA_MSG = ::RegisterWindowMessage(HS_DOWNLOADDATA_MSG);
 UINT m_HS_DOWNLOADDATA_Other_MSG = ::RegisterWindowMessage(HS_DOWNLOADDATA_Other_MSG);
 UINT HS_USER_STRATEGY = ::RegisterWindowMessage(_T("Strategy Trade Message"));
 UINT HS_USER_ZOOM		= ::RegisterWindowMessage(_T("Zoom In or Out"));	//add by lxqi 20090506	改变范围消息
 UINT g_SendReLoadTabItemMsg = ::RegisterWindowMessage(_T("Tranfer Msg of ReLoadTabItem"));  //qinhn 20090721 Add 转发重新加载K线图Tab指标页签
 UINT g_SendDeleteTechIndexMsg = ::RegisterWindowMessage(_T("Tranfer Msg of Delete K Line TechIndex"));  //qinhn 20090721 Add 转发删除K线图指标信息


CMapVariabile*  CExpression::m_pExternExpression = NULL;         // 外部表达式表
CMapVariabile*  CExpression::m_pExternVariabile  = NULL;		 // 外部变量表
CMapVariabile*  CExpression::m_pExternFunction   = NULL;		 // 外部函数表

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

// 删除map
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

// 得到一个唯一的名称
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

// 由错误号得到错误信息
CString hxGetErrorInfoByErrorNum(UINT nID)
{
	switch(nID)
	{
	case hxError_DIVISION_BY_0: return "被 0 除";
	case hxError_ILEGAL_OPERATION: return "未知错误";
	case hxError_UNDEFINED_VARIABLE: return "变量未定义";
	case hxError_INVALID_DOMAIN: return "log 参数错";
	case hxError_Fun_Exist: return "变量已经存在";
	case hxError_Var_Exist: return "函数已经存在";
	case hxError_Nod_CloneFail: return "操作单元复制失败";
	case hxError_InvalidateVars: return "无效变量";
	case hxError_NotFunction: return "函数不存在";
	case hxError_NotVariable: return "变量不存在";
	case hxError_Operator: return "操作符错误";
	case hxError_Fun_param: return "函数参数错误";
	case hxError_Identification: return "不能识别的字符";
	case hxError_AddRight: return "+加号右边错误";
	case hxError_SubRight: return "-减号右边错误";
	case hxError_RideRight: return "+乘号右边错误";
	case hxError_DivideRight: return "-除号右边错误";
	case hxError_PowerRight: return "^乘方右边错误";
	case hxError_BracketLeft: return "(左括号错误";
	case hxError_BracketRight: return ")右括号错误";
	case hxError_AbsLeft: return "| |绝对值左边错误";
	case hxError_AbsRight: return "| |绝对值右边错误";
	case hxError_LogicalRight: return "逻辑操作符右边错误(< > = == >= <= <> !=)";
	case hxError_CommaRight: return "逗号右边错误";
	case hxError_OrRight: return "or || 逻辑或右边错误";
	case hxError_AndRight: return "and && 逻辑并且右边错误";
	case hxError_DefinedRight: return ":=赋值表达式右边错误";
	case hxError_DrawRight: return ":绘图操作符右边错误";
	case hxError_ReverseRight: return "!取反右边错误";
	case hxError_Name_Exist: return "标识符已经存在";
	case hxError_NotName_Exist: return "标识符未定义";
	case hxError_Notes_Invalidate: return "注释错误/**/ //";
	case hxError_AtLeastHasExp: return "至少需要一个表达式";
	case hxError_AtLeastHasDrawExp: return "至少需要一个绘图表达式";
	case hxError_WantInvertedComma: return "缺少引用符号";
	case hxError_Exp_param: return "缺少引用参数";
	case hxError_unknowPeriod: return "不能识别的周期";
	//******************************以下是litao新增*****************************************
	case hxError_MoreLoop: return "公式循环计算次数过多，停止计算";
	//******************************以上是litao新增*****************************************
	}
	return "";
}

// 根据条件分解字串
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

// 是否添加最后一个分号
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
				if( strLast.Compare("；") )
				{
					strText = strText + ";";
				}
			}
		}
	}
}

// 使文本不是半个汉字
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

// 替换指定字符
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
	if( !strTmp1.CompareNoCase("．") || 
	    !strTmp1.CompareNoCase("。") )
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

// 替换中文数字
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

// 替换中文字符
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

// 是否是全角数字
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

// 是否是汉字
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
			!strKey.CompareNoCase("　") )
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

		//20090302 YJT 增加 Strategy
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

	//******************************以下是litao新增*****************************************
	m_strLineObj = "";				// 头一行位置

	m_nIFWHILEStart = 0;			// 是否是WHILE循环内部的编译
	m_nIFWHILEEnd = 0;				// 是否是WHILE循环内部的编译
	m_nWhileFirst.RemoveAll();		// 循环内头位置
	m_nWhileLast.RemoveAll();		// 循环内尾位置
	m_SaveExpDef.RemoveAll();		// 上次旧的公式

	m_nDefSortId = 0;
	//******************************以上是litao新增*****************************************

//	m_pExternExpression = NULL;
//	m_pExternVariabile  = NULL;
//	m_pExternFunction   = NULL;

	m_pExpDraw	 = NULL;	
	m_pParamVar  = NULL;
	m_pDataSource = NULL;

	m_pRefExpDraw   = NULL;			 // 内部绘图表
	m_pRefInsideVar = NULL;		     // 内部变量表
	m_pRefParamVar  = NULL;		     // 内部参数表

	m_pRefExpress	= NULL;			 // 相关联公式

	m_wEstopPeriod = 0;			      // 禁用周期
	m_wDefaultPeriod = HX_PERIOD_PERIOD_TYPE_DAY; // 缺省周期

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

	m_pRefExpDraw   = NULL;			 // 内部绘图表
	m_pRefInsideVar = NULL;		     // 内部变量表
	m_pRefParamVar  = NULL;		     // 内部参数表	

	m_pRefExpress	= NULL;			 // 相关联公式

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
	m_pRefInsideVar = NULL;		 // 内部变量表
	m_pRefParamVar  = NULL;		 // 内部参数表	

	m_pRefExpress	= NULL;		 // 相关联公式
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
	m_pRefInsideVar = NULL;		 // 内部变量表
	m_pRefParamVar  = NULL;		 // 内部参数表	

	m_pRefExpress	= NULL;		 // 相关联公式
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
			// 检查参数			
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

	operatie  = pClone->operatie;		   // 操作符
	cDataType = pClone->cDataType;
	if( (pClone->cDataType & DataTypeOperStr) )
	{
		valoarestr = new CString;
		valoarestr->Format("%s",*pClone->valoarestr);
	}
	else if( operatie == '@' )				// 数值
	{
		valoare = pClone->valoare;
	}

	if( cDataType & DataFunParam ) // 函数引用指针，不删除
	{
		cDataType &= (~DataFunParam);
	}

	if( cDataType & DataExpParam ) // 公式引用指针，不删除
	{
		cDataType &= (~DataExpParam);
	}

	if( cDataType & DataInport )   // 数据引用指针，不删除
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
		ar << operatie;		   // 操作符
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
		else if( operatie == '@' )				// 数值
		{
			ar << valoare;
		}
		
		cNext = 0;
		if((cDataType & DataFunParam) && pData != NULL && operatie == Operator_Function)
		{
			if ( left != NULL ) // 左
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
			if ( left  != NULL ) // 左
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

		if ( right != NULL ) // 右
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
		ar >> operatie;		   // 操作符
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
		else if( operatie == '@' )				// 数值
		{
			ar >> valoare;
		}

		cNext = 0;
		if((cDataType & DataFunParam) && operatie == Operator_Function)
		{
			ar >> cNext;  // 左
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
			ar >> cNext; // 左
			if ( cNext ) 
			{
				left = new nodElement;
				left->Serialize(pExpress,ar);
			}
		}

		ar >> cNext;    // 右
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
	DeleteError();  // 删除上一次错误
	UpdateArbore();
	return GetErrorCount();
}

void CExpression::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{
		ar << g_nExpressVersion; // 版本

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

		//-----2009.12.16 litao修改常用公式的排序未保存问题----------------------------------------------
		ar << m_nDefSortId;
		//-----------------------------------------------------------------------------------------------

		// 内部参数表		
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
		ar >> nExpressVersion; // 版本

		ar >> m_nExpressType;  // 类型
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
		//-----2009.12.16 litao修改常用公式的排序未保存问题----------------------------------------------
		ar >> m_nDefSortId;
		//-----------------------------------------------------------------------------------------------

		m_pExpDraw	 = NULL;
		m_pParamVar  = NULL;
		m_Arbore   = NULL;
		m_nPozitie = 0;

		m_pDataSource = NULL;

		// 内部参数表
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

		//DeleteError();  // 删除上一次错误
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
			m_nExpressType = Tech;  // 类型
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
			m_nExpressType = none;  // 类型
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
		
		// 内部参数表
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
		m_nExpressType = pExpress->m_nExpressType;  // 类型
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
		
		// 内部参数表
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

		m_nExpressType = pRefExpress->GetExpressType();  // 类型
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

		// 添加参数
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
			if( ayVars != NULL )  // 当前参数
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
			else // 缺省参数
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
	//******************************以下是litao新增*****************************************
	//变更公式字符串中WHILE函数赋给W
	///CString OldExp="";
	///OldExp = m_definitie;
	///m_definitie = AddWHILE(m_definitie);

	//编译前后将序列对象清空
	m_pCompNo.RemoveAll();
	//m_pExecNo.RemoveAll();
	//******************************以上是litao新增*****************************************

	m_Arbore = LineOp();//OrlogicalOp();//logicalOp();//expresie();

	//******************************以下是litao新增*****************************************
	//恢复公式字符串
	///m_nPozitie = OldExp.GetLength()-1;
	///m_definitie = OldExp;

	//编译结束后将序列对象清空
	m_pCompNo.RemoveAll();
	//m_pExecNo.RemoveAll();
	//******************************以上是litao新增*****************************************

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
	if ( (nPos = IsNegativeSign()) ) // 负号
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
	if ( nPos = IsBracketLeft(m_definitie[nPos],m_definitie,nPos) /*m_definitie[m_nPozitie] == '('*/ ) // 括号
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
			  m_definitie[m_nPozitie+1] != '|'*/ // 绝对值
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
	// 是数字(中文或英文数字)
	if (nPos || (HSisdigit(m_definitie[m_nPozitie]) && !((BYTE)m_definitie[m_nPozitie] > 0x7f)) )	// 数值
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
			strPrompt.Format("\" %s \"为非法字符! (\" %s \"前面必须有字母)",strID,strID);
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
	// 字母或汉字符号或引号(表求引用)或不是引号是字串
	else if ( HSIsalpha(m_definitie[m_nPozitie]) ||
		      IsChinese(m_definitie[m_nPozitie],m_definitie,m_nPozitie) ||
		      ( nInvertedComma = IsInvertedComma(m_definitie[m_nPozitie],m_definitie,m_nPozitie/*nInvertedComma*/)) ||
			  ( !nInvertedComma && (nText = IsText(m_definitie[m_nPozitie],m_definitie,m_nPozitie/*nText*/)) ) )
	{
		if( !nInvertedComma && !nText ) // 引用
		{
			nInvertedComma = IsInvertedComma(m_definitie[m_nPozitie],m_definitie,m_nPozitie);
		}
		if( !nInvertedComma && !nText ) // 文本?
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

		//******************************以下是litao新增*****************************************
		int nHaveID;   //当前行输出是否本ID  1：是；0：否
		//******************************以上是litao新增*****************************************

		int   nEnd;
		int   nGetType = -1;
		int   nExeType = none;
		if( nInvertedComma ) // " 引用操作开始
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
					if( !strTemp.Compare("TECH") )          // 指标
					{
						nExeType = Tech;
					}
					else if( !strTemp.Compare("SYSTEM") )	// 交易
					{
						nExeType = Exchange;
					}
					else if( !strTemp.Compare("EXPLORER") ) // 选股
					{
						nExeType = Condition;
					}
					else if( !strTemp.Compare("KCOLOR") )   // 五彩K线
					{
						nExeType = MoreKLine;
					}
					else if( !strTemp.Compare("FUN") )      // 函数
					{
						nGetType = -2;
						nExeType = -1;
					}
					else									// 指标
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
				else if( pInExternal->IsKey('$') )     // 数据,如: 0001$Close ...
				{
					nGetType  = -2;
					strPeriod = pInExternal->GetName();
				}
				else if( pInExternal->IsKey('#') )	   // 跨周期引用,如: macd#week ...
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
		else if( nText ) // 文本
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
		else  // 其他
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
			
			if( !id.CompareNoCase("and") || !id.CompareNoCase("并且") ||
				!id.CompareNoCase("or")  || !id.CompareNoCase("或") )
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
				if ( !(nPos1 = IsEqual(m_definitie[nPos1],m_definitie,nPos1)) /*m_definitie[m_nPozitie] != '='*/ )  // :冒号绘图操作
				{
					nod = new NODELEMENT;
					nod -> left  = NULL;
					nod -> right = NULL;
					
					//******************************以下是litao新增*****************************************
					CString tmpID = AddObjNo(id,COMPILE_NO);

					nod -> operatie    = Operator_Colon;
					nod -> cDataType   = DataTypeOperStr;
					//nod -> valoarestr  = new CString(strRaw);
					nod -> valoarestr  = new CString(tmpID);
					m_strLineObj = id;
					//******************************以上是litao新增*****************************************

					SkipSpaces();
					
					return nod;
				}
				else   // :=赋值操作
				{
					nod = new NODELEMENT;
					nod -> left = NULL;
					nod -> right = NULL;
					
					//******************************以下是litao新增*****************************************
					CString tmpID = AddObjNo(id,COMPILE_NO);

					nod -> operatie   = Operator_Evaluate;
					nod -> cDataType  = DataTypeOperStr;
					//nod -> valoarestr = new CString(strRaw);
					nod -> valoarestr  = new CString(tmpID);
					m_strLineObj = id;
					//******************************以上是litao新增*****************************************

					m_nPozitie += nPos1;
					SkipSpaces();
					
					return nod;
				}
			}
			
			//
			nod2 = IsNumFun(id); // 数学
			if(nod2 != NULL)
				return nod2;
		}

		short nRet = 0;
		CValue* valoare = NULL;

		//******************************以下是litao新增*****************************************
		//同一行当重复出现相同ID，且该ID是赋值时，需要取前一个序号的标示
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
		//******************************以上是litao修改*****************************************

		if( valoare != NULL && nRet )
		{
			switch(nRet)
			{
			case ExpExternFun:  // 函数
				{
					//******************************以下是litao新增*****************************************
					//当函数为循环或者分支逻辑语句
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
							//******************************以上是litao新增*****************************************

							nod2 = factor();

							// 检查参数
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
								strPrompt.Format("\" %s \"函数参数错误! %s",id,pFun->GetNotes() );						
								AddError(new nodErrorInfo(hxError_Fun_param,poz,nEnd,strPrompt));

								if(pFunctionParam)
								{
									delete pFunctionParam;
								}
								return NULL;
							}

							nod = new NODELEMENT;
							nod->left  = nod2;

							if( nGetType == -2 ) // 引用外部股票的数据
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
								nodData->pData = pFunctionParam; // 函数指针

								nodData->operatie   = Operator_Function;
								nodData->cDataType  = DataTypeOperStr | DataFunParam;
								nodData->valoarestr = new CString(id);

								nod->right = nodData;

								nod->pData      = pInportData; // 数据对象指针
								nod->operatie   = Operator_InportData;
								nod->cDataType  = DataTypeOperStr | DataInport;
								nod->valoarestr = new CString(strPeriod);
							}
							else
							{
								nod->right = NULL;
								nod->pData = pFunctionParam; // 函数指针
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
			case ExpExternExp: // 引用公式
				{
					CExpValue* pValue		 = (CExpValue*)valoare;
					CExpression* pExpression = pValue->GetExpByCompile();

					if( pExpression == NULL )
					{
						CString strPrompt;
						strPrompt.Format("公式\" %s \"里有错误,具体到\" %s \"查看! ",id,id );						
						AddError(new nodErrorInfo(hxError_Exp_param,poz,nEnd,strPrompt));
						return NULL;
					}

					if( !strSub.IsEmpty() )
					{
						CValue*	pDraw = pExpression->GetDrawExpress(strSub);
						if(pDraw == NULL)
						{
							CString strPrompt;
							strPrompt.Format("公式\" %s \"里无\" %s \"变量! %s",id,strSub,pExpression->GetNotes() );						
							AddError(new nodErrorInfo(hxError_Exp_param,poz,nEnd,strPrompt));
							return NULL;
						}
					}

					nod2 = factor();

					FormulaGeneralParam* ayVars = NULL;
					ExpressParam* pFunctionParam = new ExpressParam();

					if( nod2 != NULL )
					{
						// 得到参数
						ayVars = new FormulaGeneralParam;
						if( nod2 != NULL && pExpression->GetParamVarCount() > 0 )
						{						
							GetDetailVariable(nod2,ayVars);
						}
#if 0
						if( !IsValidateParam(pExpression,nod2,ayVars) )
						{
							CString strPrompt;
							strPrompt.Format("\" %s \"引用参数错误! %s",strRaw,pExpression->GetNotes() );						
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
							strPrompt.Format( " 无可用内存编译该公式! " );
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

					if( pNewValue->GetExpByCompile() == NULL ) // 编译
					{
						elibmem(nod2);
						delete pFunctionParam;

						CString strPrompt;
						strPrompt.Format( " 公式引用失败! " );
						AddError(new nodErrorInfo(hxError_Exp_param,poz,nEnd,strPrompt));
						return NULL;
					}

					nod = new NODELEMENT;
					nod->left  = nod2;

					if(nGetType == -2 || nGetType == -3)  // 数据
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
						nodData->pData = pInportData; // 数据对象指针
						
						nodData->operatie   = Operator_InportData;
						nodData->cDataType  = DataTypeOperStr | DataInport;
						nodData->valoarestr = new CString(strPeriod);

						nod->right = nodData;
					}
					else
					{
						nod->right = NULL;
					}
					nod->pData = pFunctionParam; // 引用公式指针
					
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
			if( nGetType == -4 ) // 文本
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
	if (id == "SIN")		// 正弦函数
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_SIN;
		SkipSpaces();
		return nod;
	}
	if (id == "COS")		// 余弦
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_COS;
		SkipSpaces();
		return nod;
	}
	if (id == "EXP")		// 指数
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_EXP;
		SkipSpaces();
		return nod;
	}
	if (id == "SQRT")		// 平方根
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_SQRT;
		SkipSpaces();
		return  nod;
	}
	if (id == "LOGX")		// 对数
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_LOG;
		SkipSpaces();
		return nod;
	}
	if (id == "TG")		// 正切
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_TG;
		SkipSpaces();
		return nod;
	}
	if (id == "CTG")		// 余切
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_CTG;
		SkipSpaces();
		return nod;
	}
	if (id == "ASIN")		// 反正弦函数
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_ASIN;
		SkipSpaces();
		return nod;
	}
	if (id == "ACOS")		// 反余弦
	{
		nod2 = factor();
		nod = new NODELEMENT;
		nod->left  = nod2;
		nod->right = NULL;
		
		nod->operatie = Operator_ACOS;
		SkipSpaces();
		return nod;
	}
	if (id == "ATG" || id == "ATAN")		// 反正切
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
	DeleteError();  // 删除上一次错误

	// 编译串
//	m_definitie = expresie;		
//	CString str(VK_TAB);
//	m_definitie = hxRelaceReturnKey(m_definitie,str);

	m_definitie = expresie + '\0' + '\0';

	UpdateArbore();  // 执行编译
	
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // 注意：当 calc1.m_lSize > 0 && calc2.m_lSize > 0 时,则 calc1.m_lSize 恒等于 calc2.m_lSize
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
	//	return (double)(long)a->valoarestr; // 返回指针，外面注意使用。
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
		case '@' :  // 返回数值
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
		case Operator_Semicolon:   // ; 分号
		case Operator_Error:       // 操作符错误
		case Operator_Komma:      // ,逗号操作(一行可有多条语句，按逗号分解)
		case Operator_Colon:      // 绘图操作(此项操作完成在窗口上绘图的表达式)
		case Operator_Evaluate:   // 赋值操作(完成左边变量赋值,可以是表达式，也可以是数值,并且将赋值后表达式加入变量或函数列表)			
		case Operator_Function:  // 函数操作
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
	while ( (nPos = IsChineseSpace(cChar,m_definitie,nPos)) || // 是全角空格符
			 (cChar  == VK_RETURN)  ||		// 是回车
			 (cChar  == '\n')	    ||		// 换行符
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

LPNODELEMENT CExpression::logicalOp() // "< > = == >= <= <> !="操作
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

LPNODELEMENT CExpression::KommaOp() // "," 操作
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod;
	LPNODELEMENT arb1 = EvaluateOp(); //赋值
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
		if( !id.CompareNoCase("or") || !id.CompareNoCase("或") )
		{
			return TRUE;
		}
	}
	m_nPozitie = nOldPos;
	return FALSE;
}

LPNODELEMENT CExpression::OrlogicalOp() // "||" 操作
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
		if( !id.CompareNoCase("and") || !id.CompareNoCase("并且") )
		{
			return TRUE;
		}
	}
	m_nPozitie = nOldPos;
	return FALSE;
}

LPNODELEMENT CExpression::AndlogicalOp() // "&&" 操作
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

LPNODELEMENT CExpression::LineOp() // 分号(按行编译)
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

		// 一行语句。加入到表
		if( arb1 && arb1->operatie != Operator_Semicolon )
		{
			AddExternDraw(arb1);
		}

		// 一行语句。
		HSCurDataInfoStatus status;
		status.m_dwStatus = hx_EXCUTE_PROMPT_ERROR | hx_GETLINE_DRAWEXPRESS;
		vexp(arb1, &status);
		
		nod->operatie = Operator_Semicolon;
		m_nPozitie++;
		
		arb2 = KommaOp();//EvaluateOp();//OrlogicalOp();
		nod->right = arb2;
		arb1 = nod;

		// 一行语句。加入到表
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
			// 一行语句。
			vexp(arb2, &status);
		}
	}

	return arb1;
}

LPNODELEMENT CExpression::EvaluateOp()   // := 赋值操作
{
	if( IsStop() ) return NULL;

	LPNODELEMENT nod;
	LPNODELEMENT arb1 = OrlogicalOp();
	LPNODELEMENT arb2;
	if(arb1 == NULL) 
		return NULL;  // In caz de eroare terminate
	
	SkipSpaces();
	if( (arb1->operatie == Operator_Evaluate) || // 赋值表达式
	    (arb1->operatie == Operator_Colon) )     // 绘图表达式
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

		// 给语句块内变量赋值。(存储变量)
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

BOOL CExpression::SkipNote() // 是 /* // 注释操作
{
	while ( (((BYTE)m_definitie[m_nPozitie] == VK_SPACE) && (m_definitie[m_nPozitie] != '\0')) || // 是空格符
			((BYTE)m_definitie[m_nPozitie]  == VK_RETURN)  ||		// 是回车
			((BYTE)m_definitie[m_nPozitie]  == '\n')	   ||		// 换行符
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
		// /**/注释
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

		// // 注释
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

		// { } 注释
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
		
		while ( (((BYTE)m_definitie[m_nPozitie] == VK_SPACE) && (m_definitie[m_nPozitie] != '\0')) || // 是空格符
				(bNext != 2 && (((BYTE)m_definitie[m_nPozitie]  == VK_RETURN)  ||		// 是回车
				((BYTE)m_definitie[m_nPozitie]  == '\n')))	   ||		// 换行符
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

		//while ( ((m_definitie[m_nPozitie] == ' ') &&    // 是空格符
		//		(m_definitie[m_nPozitie] != '\0')) ||
		//		(bNext != 2 && (((m_definitie[m_nPozitie] == '\r')  ||  // 是回车、换行符
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

		if( pTree->operatie != Operator_Function && // 函数
			pTree->operatie != Operator_Express )   // 引用公式
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

		if( pTree->operatie != Operator_Function && // 函数
			pTree->operatie != Operator_Express )   // 引用公式
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
	if(pTree != NULL)   // 将tree右边表达式save到AddParam(...)
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
	//******************************以下是litao新增*****************************************
	//CValue* pValoare = GetVariable(*a->valoarestr,nRet);
	CString strName = *a->valoarestr;
	CString tmpName = strName.Left(strName.GetLength()-3);
	CString oldName = ""; CString oldNo = "";
	CString tmpNo = strName.Right(3);
	CValue* pValoare;  CValue* pValoareNow;
	int iObjNo = _ttoi(tmpNo);


	//******************************以上是litao新增*****************************************
	pValoareNow = GetVariableExec(strName,nRetNow);

	if( pValoareNow != NULL && nRetNow )
	{				
		switch(nRetNow)
		{
		case ExpDrawObj: // 已计算,绘图表达式，在窗口上绘图的表达式
			{
				if( pValoareNow->IsStyle(HX_EXPRESSSENTENCE))
				{
					CTValues<void>* pDataNow = ((CExpValue*)pValoareNow)->GetRefCalcValues();
					//CTValues<void>* pData = pCurDataStatus->GetRefCalcValues();

					if(pDataNow != NULL && pDataNow->IsEmpty())
					{
						//将所有的该ID的变量的中间计算值全部更新
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
								case ExpDrawObj: // 已计算,绘图表达式，在窗口上绘图的表达式
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
		case ExpInsideVar: // 已计算,内部变量，一般指在表达式内部定义的变量
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

		//******************************以下是litao新增*****************************************
		//一行编译结束则将 本行赋值的标识存放变量重新初始化
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
		//******************************以上是litao新增*****************************************

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
			if( pValoare->IsStyle(HX_EXPRESSDRAW_NONAME) ) // 绘图表达式没有名称
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

		if( calc1.m_pData != NULL ) // 绘图函数
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
		//******************************以下是litao新增*****************************************
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
		//******************************以上是litao新增*****************************************

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

		// 数据源设定，不设定，则取当前的。
		InportDataParam* pInportData = (InportDataParam*)pTree->pData;
		if( pInportData && pInportData->m_pDataSrc )
		{
			pInportData->m_pDataSrc->SetRefDataSource(g_pHxDataSource);
			pInportData->m_pDataSrc->ReadData();
		}

		// 引用的函数，取数据
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

		// 一个一个计算
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

		//******************************以下是litao新增*****************************************
		CString strFunName = pFun->GetFunName();

		if (0 == strFunName.CompareNoCase("WHILE"))
		{
			dValue = WHILE(this, pFunctionParam->ayVars,pCurDataStatus,nError);
			if(nError == MORELOOPERROR)
			{
				AddError(new nodErrorInfo(hxError_MoreLoop));
			//	MessageBox(AfxGetMainWnd()->m_hWnd,hxGetErrorInfoByErrorNum(hxError_MoreLoop),"提示",MB_OK);
				//::AfxMessageBox(hxGetErrorInfoByErrorNum(hxError_MoreLoop));
				return hx_DefMinValues; // return 0;
			}
		}
		else
			nError =  pFun->Evaluate(this,pFunctionParam->ayVars,pCurDataStatus,dValue);
		//******************************以下是litao新增*****************************************

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
	//******************************以下是litao新增*****************************************
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

	//判断当为循环内的第一条语句时，赋头尾节点的位置
	if ((m_nIFWHILEStart >  0) && (m_nWhileFirst.GetCount() < m_nIFWHILEStart))
	{
		m_nWhileFirst.SetAt(m_nIFWHILEStart, m_pExpDraw->GetObjArray()->GetSize() - 1);
	}

	//每次按头层次，保存尾的位置
	if (m_nIFWHILEStart > 0)
		m_nWhileLast.SetAt(m_nIFWHILEStart, m_pExpDraw->GetObjArray()->GetSize() - 1);

	//******************************以上是litao新增*****************************************
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

		//******************************以下是litao新增*****************************************
		CString tmpName;
		tmpName = strName;
		if (0==strName.CompareNoCase(""))
			tmpName = AddObjNo(strName,COMPILE_NO);

		pValue->m_LineName = tmpName;
		pValue->m_InWhile = ((m_nIFWHILEStart>0)?true:false);
		pValue->m_WhileLevel = m_nIFWHILEStart;
		pValue->m_CurExecWhileLevel = 0;
		m_pExpDraw->SetAt((char*)LPCSTR(tmpName),pValue);
		//******************************以上是litao新增*****************************************
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

	if(nPos == -1) // 最后一根线
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

	// 引用公式
	if( nGetType != -1 ) 
	{
		if ( m_pExternExpression != NULL && 
			 m_pExternExpression->Lookup(strName,nGetType,valoare) )	// 外部表达式表
		{
			nRet = ExpExternExp;
			return valoare;
		}
		return NULL;
	}
	//
	if ( m_pExternVariabile != NULL && 
		 m_pExternVariabile->Lookup(strName,valoare) ) // 外部变量表
	{
		nRet = ExpExternVar;
		return valoare;
	}
	if ( m_pExternFunction != NULL && 
		 m_pExternFunction->Lookup(strName,valoare) ) // 外部函数表
	{
		nRet = ExpExternFun;
		return valoare;
	}	

	//******************************以下是litao修改*****************************************

	if ( m_pParamVar != NULL && 
		 m_pParamVar->Lookup(strName,valoare) )     // 内部参数表
	{
		nRet = ExpParamVar;
		return valoare;
	}
	
	CString tmpName;
	tmpName = GetObjNo(strName,COMPILE_NO,nHaveID);
	if ( m_pExpDraw != NULL && 
		m_pExpDraw->Lookup(tmpName,valoare ) )      // 内部绘图表
	{
		nRet = ExpDrawObj;
		strName = tmpName;
		return valoare;
	}
	//tmpName = SubObjNo(strName,COMPILE_NO);
	//******************************以上是litao修改*****************************************

/*
	if ( m_pRefExpDraw != NULL && 
		 m_pRefExpDraw->Lookup(strName,valoare) )   // 内部绘图表
	{
		nRet = ExpDrawObj;
		return valoare;
	}
	if ( m_pRefInsideVar != NULL && 
		 m_pRefInsideVar->Lookup(strName,valoare) ) // 内部参数表
	{
		nRet = ExpParamVar;
		return valoare;
	}
	if ( m_pRefParamVar != NULL && 
		 m_pRefParamVar->Lookup(strName,valoare) )	// 内部变量表
	{
		nRet = ExpParamVar;
		return valoare;
	}
*/
	
	if(m_pRefExpress != NULL)						 // 相关联公式
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
		 m_pExternFunction->Lookup(strName,valoare) ) // 外部函数表
	{
		return ((CFunExpresion*)valoare)->GetCurFun();
	}

	return NULL;
}

void InitSysFunction(CMapVariabile*& pExternFunction) // 初始化已定义的函数
{
	CFunExpresion* pFun;
	
	//*************************************************************************************************
	pFun = new CFunExpresion(FunOper_CEILING,"CEILING",1,g_pSystemFun->CEILING);    // 函数: CEILING(var1) 求符号值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_FLOOR,"FLOOR",1,g_pSystemFun->FLOOR);    // 函数: FLOOR(var1) 求符号值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ROUND,"ROUND",1,g_pSystemFun->ROUND);    // 函数: ROUND(var1) 求符号值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LWINNER,"LWINNER",2,g_pSystemFun->LWINNER);    // 函数: 近期获利盘比例
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PWINNER,"PWINNER",2,g_pSystemFun->PWINNER);    // 函数: 远期获利盘比例
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DRAWNULL,"DRAWNULL",0,g_pSystemFun->DRAWNULL);    // 函数: 返回不绘制类型
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	//2010.10.8 litao pFun = new CFunExpresion(FunOper_RBTREND,"RBTREND",1,g_pSystemFun->RBTREND);    // 函数: AbleTrend test
	//pExternFunction->SetAt(pFun->GetFunName(),pFun);
	//*************************************************************************************************

	// 行情函数
#ifdef SUPPORT_NETVALUE
	pFun = new CFunExpresion(FunOper_NETVALUE,"NETVALUE",0,g_pSystemFun->NETVALUE);    // 函数: NETVALUE() 国债利率,基金净值（单位：元)
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
#endif
	pFun = new CFunExpresion(FunOper_ADVANCE,"ADVANCE",0,g_pSystemFun->ADVANCE);    // 函数: ADVANCE(var1) 上涨家数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_AMOUNT,"AMOUNT",0,g_pSystemFun->AMOUNT);    // 函数: AMOUNT() 成交额
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_AMOUNT,"成交额",0,g_pSystemFun->AMOUNT);    // 函数: AMOUNT() 成交额
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ASKPRICE,"ASKPRICE",1,g_pSystemFun->ASKPRICE);    // 函数: ASKPRICE(var1) 委卖价 1-3
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ASKVOL,"ASKVOL",1,g_pSystemFun->ASKVOL);    // 函数: ASKVOL(var1) 委卖量 1-3
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BIDPRICE,"BIDPRICE",1,g_pSystemFun->BIDPRICE);    // 函数: BIDPRICE(var1) 委买价 1-3
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BIDVOL,"BIDVOL",1,g_pSystemFun->BIDVOL);    // 函数: BIDVOL(var1) 委买量 1-3
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BUYVOL,"BUYVOL",0,g_pSystemFun->BUYVOL);    // 函数: BUYVOL() 主动性买单
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CLOSE,"CLOSE",0,g_pSystemFun->CLOSE);    // 函数: CLOSE() 收盘价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CLOSE,"C",0,g_pSystemFun->CLOSE);		   // 函数: CLOSE() 收盘价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CLOSE,"收盘价",0,g_pSystemFun->CLOSE);		   // 函数: CLOSE() 收盘价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DECLINE,"DECLINE",0,g_pSystemFun->DECLINE);    // 函数: DECLINE() 下跌家数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_EXTDATA,"EXTDATA",1,g_pSystemFun->EXTDATA);    // 函数: EXTDATA(var1) 扩展数据1-11
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HIGH,"HIGH",0,g_pSystemFun->HIGH);    // 函数: HIGH() 最高价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HIGH,"H",0,g_pSystemFun->HIGH);	    // 函数: HIGH() 最高价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HIGH,"最高价",0,g_pSystemFun->HIGH);	    // 函数: HIGH() 最高价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ISBUYORDER,"ISBUYORDER",0,g_pSystemFun->ISBUYORDER);    // 函数: ISBUYORDER() 是否主动性买单
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LOW,"LOW",0,g_pSystemFun->LOW);     // 函数: LOW() 最低价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LOW,"L",0,g_pSystemFun->LOW);	   // 函数: LOW() 最低价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LOW,"最低价",0,g_pSystemFun->LOW);	   // 函数: LOW() 最低价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_OPEN,"OPEN",0,g_pSystemFun->OPEN);   // 函数: OPEN() 开盘价
	pExternFunction->SetAt(pFun->GetFunName(),pFun); 
	pFun = new CFunExpresion(FunOper_OPEN,"O",0,g_pSystemFun->OPEN);	   // 函数: OPEN() 开盘价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_OPEN,"开盘价",0,g_pSystemFun->OPEN);	   // 函数: OPEN() 开盘价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SELLVOL,"SELLVOL",0,g_pSystemFun->SELLVOL);    // 函数: SELLVOL() 主动性卖单
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VOL,"VOL",0,g_pSystemFun->VOL);     // 函数: VOL() 成交量
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VOL,"V",0,g_pSystemFun->VOL);       // 函数: VOL() 成交量
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VOL,"成交量",0,g_pSystemFun->VOL);       // 函数: VOL() 成交量
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// 时间函数
	pFun = new CFunExpresion(FunOper_DATE,"DATE",0,g_pSystemFun->hxDATE);    // 函数: DATE() 年月日
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PERIOD_TYPE_DAY,"DAY",0,g_pSystemFun->hxDAY);    // 函数: PERIOD_TYPE_DAY() 日期
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HOUR,"HOUR",0,g_pSystemFun->HSOUR);    // 函数: HOUR() 小时
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MINUTE,"MINUTE",0,g_pSystemFun->hxMINUTE);    // 函数: MINUTE() 分钟
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MONTH,"MONTH",0,g_pSystemFun->hxMONTH);    // 函数: MONTH() 月份
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_TIME,"TIME",0,g_pSystemFun->hxTIME);    // 函数:  TIME() 时分秒
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_WEEKDAY,"WEEKDAY",0,g_pSystemFun->hxWEEKDAY);    // 函数: WEEKDAY() 星期
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_YEAR,"YEAR",0,g_pSystemFun->hxYEAR);    // 函数: YEAR() 年份
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	//引用函数
	pFun = new CFunExpresion(FunOper_BACKSET,"BACKSET",2,g_pSystemFun->BACKSET);    // 函数: BACKSET(var1,var2) 向前赋值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BARSCOUNT,"BARSCOUNT",1,g_pSystemFun->BARSCOUNT);    // 函数:  BARSCOUNT(var1) 有效周期数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BARSLAST,"BARSLAST",1,g_pSystemFun->BARSLAST);    // 函数: BARSLAST(var1) 上一次条件成立位置
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_BARSSINCE,"BARSSINCE",1,g_pSystemFun->BARSSINCE);    // 函数: BARSSINCE(var1) 第一个条件成立位置
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COUNT,"COUNT",2,g_pSystemFun->COUNT);    // 函数: COUNT(var1,var2) 统计总数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DMA,"DMA",2,g_pSystemFun->DMA);    // 函数:  DMA(var1,var2) 动态移动平均
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_EMA,"EMA",2,g_pSystemFun->EMA);    // 函数: EMA(var1,var2) 指数平滑移动平均
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HHV,"HHV",2,g_pSystemFun->HHV);    // 函数: HHV(var1,var2) 最高值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_HHVBARS,"HHVBARS",2,g_pSystemFun->HHVBARS);    // 函数: HHVBARS(var1,var2) 上一高点到当前的周期数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LLV,"LLV",2,g_pSystemFun->LLV);    // 函数:  LLV(var1,var2) 最低值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LLVBARS,"LLVBARS",2,g_pSystemFun->LLVBARS);    // 函数: LLVBARS(var1,var2) 上一低点到当前的周期数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MA,"MA",2,g_pSystemFun->MA);    // 函数: MA(var1,var2) 简单移动平均
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_REF,"REF",2,g_pSystemFun->REF);    // 函数: REF(var1,var2) 引用若干周期前的数据
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SMA,"SMA",3,g_pSystemFun->SMA);    // 函数:  SMA(var1,var2,var3) 移动平均
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SUM,"SUM",2,g_pSystemFun->SUM);    // 函数: SUM(var1,var2) 总和
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SUMBARS,"SUMBARS",2,g_pSystemFun->SUMBARS);    // 函数: SUMBARS(var1,var2) 向前累加到指定值到现在的周期数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// 逻辑函数
	pFun = new CFunExpresion(FunOper_IF,"IF",3,g_pSystemFun->IF);    // 函数: IF(var1,var2,var3) 根据条件求不同的值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ISDOWN,"ISDOWN",0,g_pSystemFun->ISDOWN);    // 函数: ISDOWN() 该周期是否收阴
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ISEQUAL,"ISEQUAL",0,g_pSystemFun->ISEQUAL);    // 函数:  ISEQUAL() 该周期是否平盘
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ISUP,"ISUP",0,g_pSystemFun->ISUP);    // 函数: ISUP() 该周期是否收阳
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	//******************************以下是litao新增*****************************************
	pFun = new CFunExpresion(FunOper_WHILE,"WHILE",3,g_pSystemFun->WHILE);    // 函数: 执行WHILE循环
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_FILTER,"FILTER",2,g_pSystemFun->FILTER);  // 函数: 过滤函数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	//******************************以上是litao新增*****************************************

	// 算术函数
	pFun = new CFunExpresion(FunOper_ABS,"ABS",1,g_pSystemFun->ABS);    // 函数: ABS(var1) 求绝对值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CROSS,"CROSS",2,g_pSystemFun->CROSS);    // 函数: CROSS(var1,var2) 两条线交叉
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MAX,"MAX",2,g_pSystemFun->MAX);    // 函数:  MAX(var1,var2) 求最大值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MIN,"MIN",2,g_pSystemFun->MIN);    // 函数: MIN(var1,var2) 求最小值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_MOD,"MOD",2,g_pSystemFun->MOD);    // 函数: MOD(var1,var2) 求模运算
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_NOT,"NOT",1,g_pSystemFun->NOT);    // 函数: NOT(var1) 求逻辑非
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_REVERSE,"REVERSE",1,g_pSystemFun->REVERSE);    // 函数:  REVERSE(var1) 求相反数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SGN,"SGN",1,g_pSystemFun->SGN);    // 函数: SGN(var1) 求符号值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// 数学函数
	pFun = new CFunExpresion(FunOper_ACOS,"ACOS",1,g_pSystemFun->ACOS);    // 函数: ACOS(var1) 反余弦值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ASIN,"ASIN",1,g_pSystemFun->ASIN);    // 函数: ASIN(var1) 反正弦值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ATAN,"ATAN",1,g_pSystemFun->ATAN);    // 函数:  ATAN(var1) 反正切值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COS,"COS",1,g_pSystemFun->COS);    // 函数: COS(var1) 余弦值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_EXP,"EXP",1,g_pSystemFun->EXP);    // 函数: EXP(var1) 指数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LN,"LN",1,g_pSystemFun->LN);    // 函数: LN(var1) 求自然对数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LOG,"LOG",1,g_pSystemFun->LOG);    // 函数:  LOG(var1) 求10为底的对数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_POW,"POW",2,g_pSystemFun->POW);    // 函数: POW(var1,var2) 乘幂
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SIN,"SIN",1,g_pSystemFun->SIN);    // 函数: SIN(var1) 正弦值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SQRT,"SQRT",1,g_pSystemFun->SQRT);    // 函数: SQRT(var1) 开平方
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_TAN,"TAN",1,g_pSystemFun->TAN);    // 函数:  TAN(var1) 正切值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// 统计函数
	pFun = new CFunExpresion(FunOper_AVEDEV,"AVEDEV",2,g_pSystemFun->AVEDEV);    // 函数: AVEDEV(var1,var2) 平均绝对偏差
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DEVSQ,"DEVSQ",2,g_pSystemFun->DEVSQ);    // 函数: DEVSQ(var1,var2) 数据偏差平方和
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_FORCAST,"FORCAST",2,g_pSystemFun->FORCAST);    // 函数: FORCAST(var1,var2) 线性回归预测值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SLOPE,"SLOPE",2,g_pSystemFun->SLOPE);    // 函数:  SLOPE(var1,var2) 线性回归斜率
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_STD,"STD",2,g_pSystemFun->STD);    // 函数: STD(var1,var2) 估算标准差
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_STDP,"STDP",2,g_pSystemFun->STDP);    // 函数: STDP(var1,var2) 总体标准差
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VAR,"VAR",2,g_pSystemFun->VAR);    // 函数: VAR(var1,var2) 估算样本方差
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VARP,"VARP",2,g_pSystemFun->VARP);    // 函数:  VARP(var1,var2) 总体样本方差
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// 指数函数
	pFun = new CFunExpresion(FunOper_COST,"COST",1,g_pSystemFun->COST);    // 函数: COST(var1) 成本分布情况
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PEAK,"PEAK",3,g_pSystemFun->PEAK);    // 函数: PEAK(var1,var2,var3) 前M个ZIG转向波峰值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PEAKBARS,"PEAKBARS",3,g_pSystemFun->PEAKBARS);    // 函数: PEAKBARS(var1,var2,var3) 前M个ZIG转向波峰到当前距离
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SAR,"SAR",3,g_pSystemFun->SAR);    // 函数:  SAR(var1,var2,var3) 抛物转向
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SARTURN,"SARTURN",3,g_pSystemFun->SARTURN);    // 函数: SARTURN(var1,var2,var3) 抛物转向点
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_TROUGH,"TROUGH",3,g_pSystemFun->TROUGH);    // 函数: TROUGH(var1,var2,var3) 前M个ZIG转向波谷值
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_TROUGHBARS,"TROUGHBARS",3,g_pSystemFun->TROUGHBARS); // 函数: TROUGHBARS(var1,var2,var3) 前M个ZIG转向波谷到当前距离
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_WINNER,"WINNER",1,g_pSystemFun->WINNER);    // 函数:  WINNER(var1) 获利盘比例
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_ZIG,"ZIG",2,g_pSystemFun->ZIG);    // 函数:  ZIG(var1,var2) 之字转向
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// 大盘函数
	pFun = new CFunExpresion(FunOper_INDEXA,"INDEXA",0,g_pSystemFun->INDEXA);    // 函数: INDEXA() 对应大盘成交额
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXADV,"INDEXADV",0,g_pSystemFun->INDEXADV);    // 函数: INDEXADV() 对应大盘上涨家数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXC,"INDEXC",0,g_pSystemFun->INDEXC);    // 函数:  INDEXC() 对应大盘收盘价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXDEC,"INDEXDEC",0,g_pSystemFun->INDEXDEC);    // 函数:  INDEXDEC() 对应大盘下跌家数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXH,"INDEXH",0,g_pSystemFun->INDEXH);    // 函数: INDEXH() 对应大盘最高价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXL,"INDEXL",0,g_pSystemFun->INDEXL);    // 函数: INDEXL() 对应大盘最低价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXO,"INDEXO",0,g_pSystemFun->INDEXO);    // 函数:  INDEXO() 对应大盘开盘价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_INDEXV,"INDEXV",0,g_pSystemFun->INDEXV);    // 函数:  INDEXV() 对应大盘成交量
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	// 常用函数
	pFun = new CFunExpresion(FunOper_CAPITAL,"CAPITAL",0,g_pSystemFun->CAPITAL);    // 函数: CAPITAL() 流通盘大小
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VOLUNIT,"VOLUNIT",0,g_pSystemFun->VOLUNIT);    // 函数: VOLUNIT() 每手股数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DYNAINFO,"DYNAINFO",1,g_pSystemFun->DYNAINFO);    // 函数:  DYNAINFO(3) 动态行情函数: 昨收
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_FINANCE,"FINANCE",1,g_pSystemFun->FINANCE);    // 函数:  FINANCE(1) 财务指标: 总股本(万股)
	pExternFunction->SetAt(pFun->GetFunName(),pFun);	

	// 专业财务数据
	pFun = new CFunExpresion(FunOper_PROFFIN,"PROFFIN",2,g_pSystemFun->PROFFIN);    // 专业财务数据
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun->AddStyle(HX_UNKNOWFUNPARAM);

	pFun = new CFunExpresion(FunOper_PROFFINON,"PROFFINON",1,g_pSystemFun->PROFFINON);    // 指定日期的专业财务数据
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun->AddStyle(HX_UNKNOWFUNPARAM);
/*
	pFun = new CFunExpresion(FunOper_PROFSTR,"PROFSTR",2,g_pSystemFun->PROFSTR);    // 专业财务字符串数据
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PROFFINLAST,"PROFFINLAST",1,g_pSystemFun->PROFFINLAST);    // 专业财务数据截止期位置
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	pFun = new CFunExpresion(FunOper_PROFSTRON,"PROFSTRON",3,g_pSystemFun->PROFSTRON);    // 指定日期的专业字符串财务数据
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_PROFFINTERM,"PROFFINTERM",1,g_pSystemFun->PROFFINTERM);    // 专业财务数据报告期
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_SETPROFFIN,"SETPROFFIN",1,g_pSystemFun->SETPROFFIN);    // 设置专业财务数据
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
*/

	// 循环
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
	// 字符串
	pFun = new CFunExpresion(FUNOPER_STROUT,"STROUT",1,g_pSystemFun->STROUT);    // STROUT
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun->AddStyle(HX_UNKNOWFUNPARAM);

	// 日期转换到字符串
	pFun = new CFunExpresion(FUNOPER_DATETOSTR,"DATETOSTR",2,g_pSystemFun->DATETOSTR);    // STROUT
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun->AddStyle(HX_UNKNOWFUNPARAM);
	
	// 日期转换到字符串
	pFun = new CFunExpresion(FUNOPER_BLOCKCALC,"BLOCKCALC",2,g_pSystemFun->BLOCKCALC);    // STROUT
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun->AddStyle(HX_UNKNOWFUNPARAM);
	
	

	// 函数常量
	      // 画线类型常量
	pFun = new CFunExpresion(FunOper_STICK,"STICK",0,g_pSystemFun->STICK);  // 柱状线，从0到指标值间画垂直直线
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORSTICK,"COLORSTICK",0,g_pSystemFun->COLORSTICK);  // 彩色柱状线，当值为0是显示红色，否则显示绿色
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_VOLSTICK,"VOLSTICK",0,g_pSystemFun->VOLSTICK);  // 成交量柱状线，当股价上涨时显示红色空心柱，否则显示绿色空心柱
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LINESTICK,"LINESTICK",0,g_pSystemFun->LINESTICK);  // 同时画出柱状线和指标线
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CROSSDOT,"CROSSDOT",0,g_pSystemFun->CROSSDOT);  // 小叉线
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_CIRCLEDOT,"CIRCLEDOT",0,g_pSystemFun->CIRCLEDOT);  // 小圆圈线
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_POINTDOT,"POINTDOT",0,g_pSystemFun->POINTDOT);  // 小圆点线
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_DRAW3D,"DRAW3D",0,g_pSystemFun->DRAW3D);    // 3d
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_LINEDRAW3D,"LINEDRAW3D",0,g_pSystemFun->LINEDRAW3D);    // 3d曲线
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(FunOper_NOTDRAWLINE,"NOTDRAWLINE",0,g_pSystemFun->NOTDRAWLINE);    // 当前曲线不绘制
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_NOTDRAWTEXT,"NOTDRAWTEXT",0,g_pSystemFun->NOTDRAWTEXT);    // 当前文本不绘制
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_NOTDRAWLINETEXT,"NOTDRAWLINETEXT",0,g_pSystemFun->NOTDRAWLINETEXT); // 当前曲线文本都不绘制
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_NOTDRAW,"NOTDRAW",0,g_pSystemFun->NOTDRAW); // 不绘制图形
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(FunOper_NOTTABLE,"NOTTABLE",0,g_pSystemFun->NOTTABLE); // 不绘制图形
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	
	//
	pFun = new CFunExpresion(FunOper_NOTMAXMIN,"NOTMAXMIN",0,g_pSystemFun->NOTMAXMIN); // 不计算最大最小值,在绘图时使用
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	
	// 五彩K线颜色
	pFun = new CFunExpresion(FunOper_COLORRED,"COLORRED",0,g_pSystemFun->COLORRED);  // 红色
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORGREEN,"COLORGREEN",0,g_pSystemFun->COLORGREEN);  // 绿色
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORBLUE,"COLORBLUE",0,g_pSystemFun->COLORBLUE);  // 蓝色
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORBLACK,"COLORBLACK",0,g_pSystemFun->COLORBLACK);  // 黑色
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORWHITE,"COLORWHITE",0,g_pSystemFun->COLORWHITE);  // 白色
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORGRAY,"COLORGRAY",0,g_pSystemFun->COLORGRAY);  // 灰色
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORYELLOW,"COLORYELLOW",0,g_pSystemFun->COLORYELLOW);  // 黄色
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORCYAN,"COLORCYAN",0,g_pSystemFun->COLORCYAN);  // 青色
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORMAGENTA,"COLORMAGENTA",0,g_pSystemFun->COLORMAGENTA);  // 品红色
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(FunOper_COLORBROWN,"COLORBROWN",0,g_pSystemFun->COLORBROWN);  // 棕色
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(FunOper_COLORBRGB,"RGB",3,g_pSystemFun->hxRGB);  // RGB函数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	// 线风格常量
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

	// 用户绘图函数
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

	// 其他内部计算函数
	pFun = new CFunExpresion(-1,"QIANGDU",3,g_pSystemFun->QIANGDU); // n日强度
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"ZHANGDIEFUDU",2,g_pSystemFun->ZHANGDIEFUDU); // 涨跌幅度
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"HUANSHOULV",2,g_pSystemFun->HUANSHOULV); // 换手率
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"LIANGBIFUDU",2,g_pSystemFun->LIANGBIFUDU); // 量比幅度
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"ZHENGDANGFUDU",2,g_pSystemFun->ZHENGDANGFUDU); // 震荡幅度
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(-1,"BETWEENSHICHANGBILI",0,g_pSystemFun->BetweenShiChangBiLi); // 区间市场比例
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENOPEN",0,g_pSystemFun->BetweenOpen); // 区间开盘
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENCLOSE",0,g_pSystemFun->BetweenClose); // 区间收盘
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENHIGH",0,g_pSystemFun->BetweenHigh); // 区间最高
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENLOW",0,g_pSystemFun->BetweenLow); // 区间最低
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENVOL",0,g_pSystemFun->BetweenVol); // 区间成交量
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENAMOUNT",0,g_pSystemFun->BetweenAmount); // 区间成交量
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"CENTERPRICE",0,g_pSystemFun->CenterPrice); // 重心价
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENBEGINDATE",0,g_pSystemFun->BetweenBeginDate); // 开始时间
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BETWEENENDDATE",0,g_pSystemFun->BetweenEndDate); // 结速时间
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(-1,"BK_ZHANGDIEFUDU",0,g_pSystemFun->BK_ZHANGDIEFUDU); // 涨跌幅
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_Q_ZHANGDIEFUDU",0,g_pSystemFun->BK_Q_ZHANGDIEFUDU); // 权涨跌幅
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_VOL",0,g_pSystemFun->BK_Vol); // 总成交量
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_AMOUNT",0,g_pSystemFun->BK_Amount); // 成交额
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_DEAL",0,g_pSystemFun->BK_Deal); // 成交比例
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_HUANSHOULV",0,g_pSystemFun->BK_HuanShouLv); // 换手率
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_SHIYING",0,g_pSystemFun->BK_ShiYing); // 市盈率
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_LINGZHANG",0,g_pSystemFun->BK_LingZhang); // 令涨股
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_LINGDIE",0,g_pSystemFun->BK_LingDie); // 领跌股
	pExternFunction->SetAt(pFun->GetFunName(),pFun);
	pFun = new CFunExpresion(-1,"BK_NUM",0,g_pSystemFun->BK_Num); // 股票数
	pExternFunction->SetAt(pFun->GetFunName(),pFun);	
	pFun = new CFunExpresion(-1,"BK_UPRATIO",0,g_pSystemFun->BK_UpRatio); // 涨跌比
	pExternFunction->SetAt(pFun->GetFunName(),pFun);	

	pFun = new CFunExpresion(-1,"HLTCH",4,g_pSystemFun->HLTCH);   // 弘历通-彩虹
	pExternFunction->SetAt(pFun->GetFunName(),pFun);

	pFun = new CFunExpresion(-1,"HLTHBQ",4,g_pSystemFun->HLTHBQ); // 弘历通-红白圈
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
	if(bSave) // 存储
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

					if(pFunAyTree != NULL) // 从tree中保存到文件
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
	else // 装入
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
				if(pFunAyTree != NULL) // 加入到tree
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

	m_pRefExpDraw   = pThisExp->m_pExpDraw;			 // 内部绘图表
	m_pRefParamVar  = pThisExp->m_pParamVar;		 // 内部参数表

//	if( IsStyle(HX_EXPRESSREFERENCE) )
//	{
	m_pRefExpress	= pThisExp;						 // 相关联公式
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
    			status->m_pDataSource = (CHSDataSource*)(pRefData->m_pDataSource); // 设定数据源
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

		pRefData->m_dwStyle |= (status->m_dwStatus & HX_USERDRAWFUN_EXPRESS);  // 绘图函数

		if( status->m_pData == NULL )
		{
			if( (long)g_dwRetValoare != hx_DefMinValues )
			{
				if( pRefData->m_pData == NULL && nStart == 0 && nEnd == 0 ) // 表达式引用
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

		if( pRefData->m_pData == NULL && nStart == 0 && nEnd == 0) // 表达式引用
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
				// 只能对财务数据才能这样做。
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
		g_strCurDefEidolon.Add(" 请设置计算参数：");
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
	if( (dMask & FindInName) ) // 名称
	{
		strText = GetName();
		strText.MakeUpper();
		bCountinue = (strText.Find(strKey) != -1);
		if(!nUnique)
		{
			nUnique = !strText.CompareNoCase(strKey);
		}
		strText = this->GetWholeName();//取全名
	//	strRet = strText;
		if( bCountinue )
		{
	//		strRet += "-";
	//		strRet += "公式全名";
			bRet = bCountinue;
		}
	}
	if( (dMask & FindInDescribe) ) // 表达式描述
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
	//		strRet += "公式描述";
			bRet = bCountinue;
		}
	}
	if( (dMask & FindInNotes) ) // 表达式注释
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
	//		strRet += "公式注释";
			bRet = bCountinue;
		}
	}
	if( (dMask & FindInEidolon) ) // 参数精灵
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
			strRet += "参数精灵";
			bRet = bCountinue;
		}
	}
	if( (dMask & FindInSource) ) // 要编译的字符串
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
	//		strRet += "公式表达式";
			bRet = bCountinue;
		}
	}
	strRet = GetWholeName();
	strRet += "公式全名";
	strRet += GetDescribe();
	strRet += "公式描述";
	strRet += GetNotes();
	strRet += "公式注释";
	strRet += GetEidolon();
	strRet +=  "参数精灵";
	strRet += GetSource();
	strRet += "公式表达式";
	
	return bRet;
}

BOOL CExpression::IsFind(CString strKey,int& nUnique,
						 DWORD dMask/* = CExpression::FindInName | CExpression::FindInDescribe*/)
{
	if( (dMask & 0xFF) != m_nExpressType )
		return 0;

	BOOL bRet = FALSE;
	CString strText;
	if( (dMask & FindInName) ) // 名称
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
	if( (dMask & FindInDescribe) ) // 表达式描述
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
	if( (dMask & FindInNotes) ) // 表达式注释
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
	if( (dMask & FindInEidolon) ) // 参数精灵
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
	if( (dMask & FindInSource) ) // 要编译的字符串
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
		if( !strKey.CompareNoCase("＋") )
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
		if( !strKey.CompareNoCase("－") )
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
		if( !strKey.CompareNoCase("＊") )
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
		if( !strKey.CompareNoCase("／") )
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
		if( !strKey.CompareNoCase("！") )
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
		if( !strKey.CompareNoCase("＾") )
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
		if( !strKey.CompareNoCase("＆") )
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
		if( !strKey.CompareNoCase("（") )
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
		if( !strKey.CompareNoCase("）") )
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
		if( !strKey.CompareNoCase("｜") )
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
		if( !strKey.CompareNoCase("；") )
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
		if( !strKey.CompareNoCase("，") )
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
		if( !strKey.CompareNoCase("：") )
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
		if( !strKey.CompareNoCase("＞") ||
			!strKey.CompareNoCase("》") ||
			!strKey.CompareNoCase("〉") )
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
		if( !strKey.CompareNoCase("＜") ||
			!strKey.CompareNoCase("《") ||
			!strKey.CompareNoCase("〈") )
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
		if( !strKey.CompareNoCase("＝") )
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
		if( !strKey.CompareNoCase("．") ||
			!strKey.CompareNoCase("。") )
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
		if( !strKey.CompareNoCase("＄") )
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
		if( !strKey.CompareNoCase("＃") )
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsChineseSpace(BYTE cChar,const CString& strSource,int& nPos)	// 全角空格
{
	if( cChar == ' ' )
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("　") )
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
		if( !strKey.CompareNoCase("＂") ||
			!strKey.CompareNoCase("“") ||
			!strKey.CompareNoCase("”"))
		{
			nPos++;
			return 2;
		}
	}
	return 0;
}

int CExpression::IsText(BYTE cChar,const CString& strSource,int& nPos)	// ' 字符串
{
	if( cChar == '\'')
	{
		return 1;
	}
	else if( cChar > 0x7f )
	{
		CString strKey = strSource.Mid(nPos,2);
		if( !strKey.CompareNoCase("＇") ||
			!strKey.CompareNoCase("‘") ||
			!strKey.CompareNoCase("’"))
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
		CExpValue* pCurVal;      // 当前绘图表达式
		
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
			CExpValue* pCurVal;      // 当前绘图表达式
			CExpValue* pCurParamVal; // 绘图参数
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
						
						// 参数名称
						if( pCurVal->IsStyle(HX_EXPRESSDRAW_NONAME) )
						{
							strTmp = "";
						}
						else
						{
							strTmp.Format( "%s",pVar->GetObjectName(pCurVal) );
						}
						strncpy(pData->m_strName,strTmp,sizeof(pData->m_strExpParam));
						
						// 参数数值
						CMapVariabile* pParam = ((CExpValue*)pCurVal)->GetParam();
						if( pParam != NULL )
						{
							// 参数：绘图类型;绘图颜色;线风格;线宽度;...PS_SOLID
							nParamSize = pParam->GetObjArray()->GetSize();							
							for( nPar  = 0; nPar < nParamSize; nPar++ )
							{
								pCurParamVal = (CExpValue*)pParam->GetObjArray()->GetAt(nPar);
								lValue		 = pCurParamVal->GetValue(NULL);
								if( nPar == 3 )	  // 线宽度
								{
									pData->m_nWidth = lValue;
								}
								else if( nPar == 2 ) // 线风格
								{
									pData->m_nPenStyle = lValue;
								}
								else if( nPar == 1 ) // 绘图颜色
								{
									pData->m_lColor = lValue;
								}
								else if( nPar == 0 ) // 绘图类型
								{
									pData->m_lStyle |= (long)lValue;
								}
								else  // 无效
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

//******************************以下是litao新增*****************************************
//修改标识序列号,递增1
CString CExpression::AddObjNo(CString key,int iCompExec)
{
	CString strName = "";
	LPEXECOBJNO lpObjNo;
	int iObjNo = 0;
	//----2009.11.13 litao修改，在无变量表达式时也可以递增默认变量000、001。。。
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
			//没有找到，则新加入
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
			//没有找到，则新加入
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

//修改标识序列号,递减1
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
	return strName;  //没有找到，则返回空串
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

	// 引用公式
	if( nGetType != -1 ) 
	{
		if ( m_pExternExpression != NULL && 
			m_pExternExpression->Lookup(strName,nGetType,valoare ) )	// 外部表达式表
		{
			nRet = ExpExternExp;
			return valoare;
		}
		return NULL;
	}
	//
	if ( m_pExternVariabile != NULL && 

		m_pExternVariabile->Lookup(strName,valoare ) ) // 外部变量表
	{
		nRet = ExpExternVar;
		return valoare;
	}
	if ( m_pExternFunction != NULL && 
		m_pExternFunction->Lookup(strName,valoare ) ) // 外部函数表
	{
		nRet = ExpExternFun;
		return valoare;
	}	


	if ( m_pParamVar != NULL && 
		m_pParamVar->Lookup(strName,valoare ) )     // 内部参数表
	{
		nRet = ExpParamVar;
		return valoare;
	}

	CString tmpName;
	tmpName = GetMaxObjNo(strName,EXECUTE_NO);

	if ( m_pExpDraw != NULL && 
		m_pExpDraw->Lookup(tmpName,valoare ) )      // 内部绘图表
	{
		nRet = ExpDrawObj;
		return valoare;
	}


	if(m_pRefExpress != NULL)						 // 相关联公式
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

	// 引用公式
	if( nGetType != -1 ) 
	{
		if ( m_pExternExpression != NULL && 
			m_pExternExpression->Lookup(strName,nGetType,valoare ) )	// 外部表达式表
		{
			nRet = ExpExternExp;
			return valoare;
		}
		return NULL;
	}
	//
	if ( m_pExternVariabile != NULL && 

		m_pExternVariabile->Lookup(strName,valoare ) ) // 外部变量表
	{
		nRet = ExpExternVar;
		return valoare;
	}
	if ( m_pExternFunction != NULL && 
		m_pExternFunction->Lookup(strName,valoare ) ) // 外部函数表
	{
		nRet = ExpExternFun;
		return valoare;
	}	


	if ( m_pParamVar != NULL && 
		m_pParamVar->Lookup(strName,valoare ) )     // 内部参数表
	{
		nRet = ExpParamVar;
		return valoare;
	}

	if ( m_pExpDraw != NULL && 
		m_pExpDraw->Lookup(strName,valoare ) )      // 内部绘图表
	{
		nRet = ExpDrawObj;
		return valoare;
	}


	if(m_pRefExpress != NULL)						 // 相关联公式
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

	nodleft = factor();  //得到条件表达式的跟节点

	//直接找到结尾换行符
	SkipSpaces();

	//保存原来的公式字符串，并将循环内的语句重新付给公式字符串	
	CString OldExpDef = m_definitie;
	m_SaveExpDef.SetAt(m_SaveExpDef.GetCount(),OldExpDef);

	//将循环体内的语句赋给m_definitie。
	m_definitie = GetWhileDefinitie(m_definitie);

	int nOldpos = m_nPozitie;  //保存当前字符位置
	m_nPozitie = 0;

	if (m_definitie.GetLength() < 1)
	{
		AddError( new nodErrorInfo(hxError_Fun_param,
			m_nPozitie,m_definitie.GetLength(),
			"ENDLOOP") );
		return NULL;
	}

	//继续编译本循环内部语句
	nodright = LineOp(); 

	//结束后恢复原来的字符串
	m_nPozitie = nOldpos;
	m_definitie = m_SaveExpDef[m_SaveExpDef.GetCount()-1];
	m_SaveExpDef.RemoveKey(m_SaveExpDef.GetCount()-1);

	int nFirst,nLast;
	m_nWhileFirst.Lookup(m_nIFWHILEStart,nFirst);
	m_nWhileLast.Lookup(m_nIFWHILEStart,nLast);

	m_nWhileFirst.RemoveKey(m_nIFWHILEStart);
	m_nWhileLast.RemoveKey(m_nIFWHILEStart);


	//记录循环内的第一语句的位置(在内部绘图表中的下标)
	firstNod = new NODELEMENT;
	firstNod->left  = NULL; 
	firstNod->right = NULL;
	firstNod->operatie = '@';
	firstNod->valoare = nFirst+m_nIFWHILEStart;  //由于WHILE函数还要加入循环内头节点前，所以加1;当多层循环时还要考虑循环层次-1的WHILE节点

	//记录循环内的最后语句的位置(在内部绘图表中的下标)	
	lastNod = new NODELEMENT;
	lastNod->left  = NULL; 
	lastNod->right = NULL;
	lastNod->operatie = '@';
	lastNod->valoare = nLast+m_nIFWHILEStart;    //由于WHILE函数还要加入循环内头节点前，所以加1;当多层循环时还要考虑循环层次-1的WHILE节点

	// 检查参数
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
	nod->pData = pFunctionParam; // 函数指针
	nod->operatie   = Operator_Function;
	nod->cDataType  = DataTypeOperStr | DataFunParam;
	nod->valoarestr = new CString("WHILE");

	//将函数加入循环内部起始位置处
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
	pValue->m_InWhile = ((m_nIFWHILEStart>1)?true:false);  //第一个循环函数应属于循环外调用
	pValue->m_WhileLevel = m_nIFWHILEStart-1;			   //第一个循环函数层次应为0
	pValue->m_CurExecWhileLevel = 0;

	m_pExpDraw->InsertAt(strName,pValue,nFirst);

	m_nIFWHILEStart--;

	//保存头尾位置
	if ((m_nIFWHILEStart >  0) && (m_nWhileFirst.GetCount() < m_nIFWHILEStart))
	{
		m_nWhileFirst.SetAt(m_nIFWHILEStart, m_pExpDraw->GetObjArray()->GetSize() - 1);
	}

	//每次按头层次，保存尾的位置
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
		if( !strKey.CompareNoCase("｛") )
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
		if( !strKey.CompareNoCase("｝") )
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
	int nWhileNum=0; //记录是否出现循环嵌套的情况。


	while (m_nPozitie < m_definitie.GetLength()-7)
	{
		//每次取一个标示
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
		//每次取一个标示
		id = m_definitie.Mid(i,5);
		id.MakeUpper();
		if (0 == id.CompareNoCase("WHILE"))
			//前面加上"WHILE:="
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

	//计算条件表达式
	double v0 = vexp(condNod,&calc0);

	while (v0)
	{
		//从跟节点开始遍历全部的公式树节点，当左节点操作符为赋值时，则根据右节点公式进行计算
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

		//循环体内执行完毕，重新计算条件表达式
		double v0 = vexp(condNod,&calc0);
	}

	CValue* pValoare;
	pValoare = GetVariableExec(strName,nRet);

	return hx_DefMinValues; // return 0;

}

HSDouble CExpression::WHILE (CExpression* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	int		 nLoopNum=0;			//当前循环的次数

	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pParam0  = ayVars->GetParam(0);  //条件表达式计算根节点
	LPNODELEMENT pParam1  = ayVars->GetParam(1);  //循环内语句第一个输出的位置
	LPNODELEMENT pParam2  = ayVars->GetParam(2);  //循环内语句最后一个输出的位置

	//无数据/条件判断语句退出
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
			//将本次循环的各个语句循环层次+1；
			for(int nObj = N1; nObj <= N2; nObj++)
			{
				pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
				if( pCurVal != NULL )
				{
					pCurVal->m_CurExecWhileLevel++;  
				}
			}
		}

		while (v0)   //条件判断应该为一组值
		{
			nLoopNum++;
			if (nLoopNum > MAXLOOPTIMES) //判断循环次数是否太高
			{
				//提示错误
				nError = MORELOOPERROR;
				return hx_DefMinValues;
			}

			// 一个一个中间标识进行计算

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

					//执行完设置开关变量，防止循环内语句在WHILE循环外被多执行一次。
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


			//重新计算条件值
			v0 = vexp(pParam0,&calc0);
			if (v0 == hx_ReturnPointValues)
			{
				v0 = calc0.m_pData[0];
			}
		}

		//循环结束将本次循环的各个语句循环层次-1；
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


//******************************以上是litao新增*****************************************
