// FunExpresion.cpp : implementation file
//

#include "stdafx.h"
#include "FunExpresion.h"
#include "express.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFunExpresion

#define Function_Notes_Size 1024

IMPLEMENT_DYNCREATE(CFunExpresion, CValue)

CFunExpresion::CFunExpresion()
{
	m_nOperator = 0;
	m_pFunName  = NULL;
	m_nParams   = 0;
	m_pNotes    = NULL; 

	AddStyle(HX_EXPRESSFUNCTION);
}

CFunExpresion::CFunExpresion(unsigned short nOperator,
							 CString pFunName,short nParams,COMPILEFUNCTION curFn)
{				  
	AddStyle(HX_EXPRESSFUNCTION);

	m_nOperator = nOperator;
	m_nParams   = nParams;
	
	if(!pFunName.IsEmpty())
	{
		m_pFunName = new CString;
		*m_pFunName = pFunName;
	}
	else
	{
		m_pFunName = NULL;
	}

	// ��������
	m_nFunType = 
	(nOperator > FunOper_QUOTES_Begin && nOperator < FunOper_QUOTES_End)?FunctionQuotes:	     // ����
	(nOperator > FunOper_DateTime_Begin && nOperator < FunOper_DateTime_End)?FunctionDateTime:	 // ʱ��
	(nOperator > FunOper_Refer_Begin && nOperator < FunOper_Refer_End)?FunctionReference:        // ����
	(nOperator > FunOper_Logical_Begin && nOperator < FunOper_Logical_End)?FunctionLogical:	     // �߼�
	(nOperator > FunOper_Arithmetic_Begin && nOperator < FunOper_Arithmetic_End)?FunctionArithmetic: // ����
	(nOperator > FunOper_Maths_Begin && nOperator < FunOper_Maths_End)?FunctionMath:		         // ��ѧ
	(nOperator > FunOper_Statistic_Begin && nOperator < FunOper_Statistic_End)?FunctionStat:		 // ͳ��
	(nOperator > FunOper_Index_Begin && nOperator < FunOper_Index_End)?FunctionTarget:		         // ָ��
	(nOperator > FunOper_BigIndex_Begin && nOperator < FunOper_BigIndex_End)?FunctionBig:		     // ����
	(nOperator > FunOper_InCommonUse_Begin && nOperator < FunOper_InCommonUse_End)?FunctionInCommonUse: // ����
	(nOperator > FunOper_UserDefFun_Begin && nOperator < FunOper_UserDefFun_End)?FunctionUserDefine:    // �Զ���
	(nOperator > FunOper_Constant_Begin && nOperator < FunOper_Constant_End)?FunctionConstant:  // �˺����ǳ���
	(nOperator > FunOper_Draw_Begin && nOperator < FunOper_Draw_End)?FunctionDraw:FunctionNone;  // �û���ͼ����

	m_pNotes	  = NULL;
	m_strDescribe = NULL;

	//SetNotes();
	//SetDescribe();

	m_curFn = curFn; // ��ǰ���õĺ���

}

CFunExpresion::~CFunExpresion()
{	
	if(m_pFunName != NULL)
	{
		delete m_pFunName;
		m_pFunName = NULL;
	}
	if(m_pNotes != NULL)
	{
		delete m_pNotes;
		m_pNotes = NULL;
	}
	if(m_strDescribe != NULL)
	{
		delete m_strDescribe;
		m_strDescribe = NULL;
	}
}

void CFunExpresion::SetValue(void* value)
{	
}

double CFunExpresion::GetValue()
{
	return 0;
}

void CFunExpresion::UpdateValue()
{
}

BOOL CFunExpresion::SetParams(short nParams)
{
	m_nParams = nParams;
	return 0;
}

BOOL CFunExpresion::SetFunName(CString* pFunName)
{
	if(pFunName == NULL) return 0;
	
	if(m_pFunName != NULL)
	{
		delete m_pFunName;
		m_pFunName = NULL;
	}
	m_pFunName = new CString;
	m_pFunName->Format(*pFunName);

	return 0;
}

char* CFunExpresion::GetFunName()
{ 
	if(m_pFunName == NULL) 
		return "";
	return (char*)LPCSTR(*m_pFunName);
}

CString CFunExpresion::GetNotes()
{
	if(m_pNotes == NULL || m_pNotes->IsEmpty() ) 
	{
		SetNotes();
	}
	if(m_pNotes == NULL) 
		return "";
	CString strRet;
	strRet.Format("%s",*m_pNotes);
	return strRet;	
}

void CFunExpresion::SetNotes()
{
	if(m_pNotes != NULL)
	{
		delete m_pNotes;
		m_pNotes = NULL;
	}
	m_pNotes = new CString;
	CString str;
	str.Format("%s0",GetFunName());
	GetPrivateProfileString(GetFunName(),str,"",m_pNotes->GetBuffer(Function_Notes_Size),Function_Notes_Size-1,
		CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH)+FILENAME_HS_FUNCTION_HELP_CFG);
	m_pNotes->ReleaseBuffer();
}

CString CFunExpresion::GetDescribe()
{
	if(m_strDescribe == NULL) 
	{
		SetDescribe();
	}
	if(m_strDescribe == NULL) 
		return "";
	CString strRet;
	strRet.Format("%s",*m_strDescribe);
	return strRet;
}

void CFunExpresion::SetDescribe()
{
	if(m_strDescribe != NULL)
	{
		delete m_strDescribe;
		m_strDescribe = NULL;
	}
	m_strDescribe = new CString;
	GetPrivateProfileString(GetFunName(),GetFunName(),"",
		m_strDescribe->GetBuffer(Function_Notes_Size),Function_Notes_Size-1,
		CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH)+FILENAME_HS_FUNCTION_HELP_CFG);
	m_strDescribe->ReleaseBuffer();
}

FunctionType CFunExpresion::SetFunType(FunctionType nType)
{
	FunctionType nOldType = m_nFunType;
	m_nFunType = nType;
	return nType;
}

int	CFunExpresion::Evaluate(CExpression* pCompile,
							FormulaGeneralParam* ayVars,
							HSCurDataInfoStatus* pCurDataStatus,
							double& dValue)
{
	if(m_curFn != NULL)
	{
		int nError = -1;
		dValue = (*m_curFn)(pCompile,ayVars,pCurDataStatus,nError);
		return nError;
	}

	return 0;
}

CString CFunExpresion::GetFunDefMode()
{
	CString str,strRet;
	str.Format("%s1",GetFunName());
	GetPrivateProfileString(GetFunName(),str,"",strRet.GetBuffer(Function_Notes_Size),Function_Notes_Size-1,
		CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH)+FILENAME_HS_FUNCTION_HELP_CFG);
	strRet.ReleaseBuffer();
	return strRet;
}

int CFunExpresion::GetFunDefMode(CString& strFun)
{
	int nPos;
	strFun.Format("%s",GetFunName());
	nPos = strFun.GetLength();
	if( strFun.IsEmpty() )
		return nPos;

	CString strRet = GetFunDefMode();
	if( !strRet.IsEmpty() )
	{
		strFun = strRet;
		return nPos+1;
	}

	if( m_nParams > 0 )
	{
		strFun += "( ";
		nPos += 2;
		BOOL bEnter = FALSE;
		for(int i = 0; i < m_nParams-1;i++)
		{
			strFun += ", ";
			bEnter = TRUE;
		}
		if(bEnter)
			strFun += ")";
		else
			strFun += " )";
	}
	else
	{
		nPos += 1;
	}

	return nPos;
}

BOOL CFunExpresion::EvaluateFunction(FormulaGeneralParam* ayVars)
{
	return 0;
}

void CFunExpresion::Serialize(CArchive& ar)
{
}

BOOL CFunExpresion::Find(CString strKey,CString& strRet,DWORD dMask /*= 0*/) 
{ 
	int nUnique = 1;

	BOOL bRet = 0,bCountinue = 0;
	CString strText;
	strRet = "";
	if( (dMask & FindInName) ) // ����
	{
		strText = GetFunName();
		strText.MakeUpper();
		bCountinue = strText.Find(strKey);
		if(!nUnique)
		{
			nUnique = !strText.CompareNoCase(strKey);
		}
		if( bCountinue != -1 )
		{
			strRet  = strText;
			strText = GetDescribe();
			if(!strText.IsEmpty())
			{
				strRet += "-";
				strRet += strText;
			}
			strText = GetNotes();
			if(!strText.IsEmpty())
			{
				strRet += "-";
				strRet += strText;
			}
			bRet = 1;
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
		strRet += strText;
		if(bCountinue)
		{
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
		strRet += strText;
		if(bCountinue)
		{
			bRet = bCountinue;
		}
	}
	
	return bRet;
}

