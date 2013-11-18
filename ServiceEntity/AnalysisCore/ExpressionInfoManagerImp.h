#pragma once
#include "CombComm\AnalysisTable.h"

class CExpressionInfoManagerImp;

class  CExpParamInfoImp: public IExpParamInfo
{
public:
	friend class CExpressionInfoManagerImp;
	CString GetName();
	char     GetType();
	CString GetNote();
	CString GetFlag();
private:
	CString  m_strName;
	char      m_cType;
	CString  m_strNote;
	CString  m_strFlag;
};

typedef CTypedPtrArray<CPtrArray,CExpParamInfoImp*> CParamInfoArray;
class CExpressionInfoImp: public IExpressionInfo
{
public:
	friend class CExpressionInfoManagerImp;
	~CExpressionInfoImp();
	virtual CString  GetName() { return m_strName; }
	virtual CString GetExpression() { return m_strExpression;}
	virtual void GetParams(CMapStringToString &mapParams);
	virtual int  GetParamCount();
	virtual IExpParamInfo* GetParamInfo(int nIndex);
private:
	CString                            m_strName;       //指标名称
	CString                            m_strExpression;//表达式串
	CParamInfoArray              m_ayParams;
};

class CExpressionInfoManagerImp
{
public:
	CExpressionInfoManagerImp();
	~CExpressionInfoManagerImp();
	virtual void LoadExpression();
	virtual CExpressionInfoImp* GetExpressionInfo(CString strExpName);
private:
	CMapStringToOb            m_mapExpressionInfo;
};

CExpressionInfoManagerImp* GetExpressionInfoManager();