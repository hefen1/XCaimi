#pragma once
#include "muParserTest.h"
#include ".\CombComm\CombManagerInterface.h"

#define VAR_COUNT   10
class CExpressionParser
{
public:
	CExpressionParser();
	~CExpressionParser();
	BOOL Init();
	void SetExpr(CString strExpression);
	double Eval();
private:
	mu::Parser  m_parser;	
	double               m_vars[VAR_COUNT];
};

class CCustomExpression
{
public:
	CCustomExpression(void);
	~CCustomExpression(void);
	BOOL Init();
	void SetExpression(CString strExp) { m_strOrgExpression = strExp; }
	CString GetExpression() { return m_strOrgExpression;}
	BOOL SetParams(IWatcherParams* pWatcherParams);
	double CalculateValue();
private:
	CString     m_strExpression;//±Ì¥Ô Ω
	CString    m_strOrgExpression;
	CExpressionParser m_expParser;
	CStringArray          m_ayExp;
};