#include "StdAfx.h"
#include "CustomExpression.h"
#include "FormulaSysFunc.h"
#include "..\..\..\HelperLib\GeneralHelper.h"

//////////////////////////////////////////////////////////////////////////
//CExpressionParser
//////////////////////////////////////////////////////////////////////////
CExpressionParser::CExpressionParser()
{
	Init();
}

CExpressionParser::~CExpressionParser()
{
	
}

BOOL CExpressionParser::Init()
{
	CString str;
	for(int i =0 ;i < VAR_COUNT; i++)
	{
		str.Format("Temp%d",i+1);
		m_vars[i] = 0;
		m_parser.DefineVar(str.GetBuffer(),&m_vars[i]);
	}
	//P:取最新价函数
	m_parser.DefineFun(_T("P"),CFormulaSysFunc::GetNewPrice,false);
	//NAV:最基金净值
	m_parser.DefineFun(_T("NAV"),CFormulaSysFunc::GetNAV,false);
	//C:昨收
	m_parser.DefineFun(_T("C"),CFormulaSysFunc::GetClose,false);
	//O:开盘价
	m_parser.DefineFun(_T("O"),CFormulaSysFunc::GetClose,false);
	//PB:取买盘价
	m_parser.DefineFun(_T("PB"),CFormulaSysFunc::GetBPrice,false);
	//PS:取卖盘价
	m_parser.DefineFun(_T("PS"),CFormulaSysFunc::GetSPrice,false);

	//COMBP:取卖盘价
	m_parser.DefineFun(_T("COMBP"),CFormulaSysFunc::GetCombPrice,false);
	//COMBBUYPKSZ:买盘口市值
	m_parser.DefineFun(_T("COMBBUYPKSZ"),CFormulaSysFunc::GetCombBuyPKSZ,false);
	//COMBSELLPKSZ:买盘口市值
	m_parser.DefineFun(_T("COMBSELLPKSZ"),CFormulaSysFunc::GetCombSellPKSZ,false);

	return TRUE;
}

void CExpressionParser::SetExpr(CString strExpression)
{
	m_parser.SetExpr(strExpression.GetBuffer());
}

double CExpressionParser::Eval()
{
	return m_parser.Eval();
}

//////////////////////////////////////////////////////////////////////////
//CCustomExpression
//////////////////////////////////////////////////////////////////////////

CCustomExpression::CCustomExpression(void)
{
	
}

CCustomExpression::~CCustomExpression(void)
{
}

BOOL CCustomExpression::SetParams(IWatcherParams* pWatcherParams)
{
	if(pWatcherParams == NULL)
		return FALSE;
	//替换参数
	CString strParamName,strParamValue;
	m_strExpression = m_strOrgExpression;
	for(int i=0 ; i < pWatcherParams->GetParamCount(); i++)
	{
		IParamInfo* pParamInfo = pWatcherParams->GetParamInfo(i);
		if(pParamInfo)
		{
			strParamName = pParamInfo->GetParamName();
			strParamValue = pParamInfo->GetParamValue();
			char cType = pParamInfo->GetParamType();
			if(cType == 'N')
			{
				m_strExpression.Replace(strParamName,strParamValue);	
			}
			else
			{
				m_strExpression.Replace(strParamName,_T("\"") + strParamValue + _T("\""));	
			}
		}
	}
	//分割表达式
	m_ayExp.RemoveAll();
	CGeneralHelper::AnalyseStr(m_strExpression,_T(";"),m_ayExp);
	return TRUE;
}

double CCustomExpression::CalculateValue()
{
	try
	{
		double dResult = 0;
		for(int i=0; i<m_ayExp.GetCount();i++)
		{
			CString strExp = m_ayExp.GetAt(i);
			m_expParser.SetExpr(strExp);
			dResult = m_expParser.Eval();
		}
		return dResult;
	}
	catch(mu::Parser::exception_type &e)
	{
		CString str = _T("\nError:\n");
		CString strMsg;   strMsg.Format("%s,%s\n",    _T("Message:     "),e.GetMsg());
		CString strExp;     strExp.Format("%s,%s\n",     _T("Expression:  "), e.GetExpr());
		CString strToken; strToken.Format("%s,%s\n",  _T("Token:       "), e.GetToken());
		CString strPos;     strPos.Format("%s,%d\n",     _T("Position:    ") , (int)e.GetPos());
		CString strErrc;     strErrc.Format("%s,%d\n",    _T("Errc:        ") ,e.GetCode());
		OutputDebugString(str + strMsg + strExp + strToken + strPos + strErrc);
	}
	return 0;
}