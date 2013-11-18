#pragma once
#include "..\CombComm\AnalysisTable.h"
#include "ExpressionInfoManagerImp.h"

class CAnalysisColumnImp: public IAnalysisColumn
{
public:
	CAnalysisColumnImp();
	~CAnalysisColumnImp();

	virtual CString GetName();
	virtual CString GetDispName();
	virtual CString GetExpression();
	virtual int        GetColumIndex();
	virtual int        GetColWidth();
	virtual CString GetDispFormat();
	virtual IExpressionInfo* GetExpInfo();

	virtual void SetName(CString strName);
	virtual void SetDispName(CString strDispName);
	virtual void SetExpName(CString strExpName);
	virtual void SetColumIndex(int nIndex);
	virtual void SetColWidth(int nWidth);
	virtual void Release();
	virtual void SetDispFormat(CString strDispFormat);
private:
	CString m_strName;
	CString m_strDispName;
	CString m_strExpName;
	int        m_nIndex;
	int        m_nWidth;
	CString m_strDispFormat;
};

class CAnalysisTableImp: public IAnalysisTable
{
public:
	CAnalysisTableImp(void);
	~CAnalysisTableImp(void);

	virtual BOOL AddColumn(CString strColName,IAnalysisColumn* pCol);
	virtual int GetColumnCount();
	virtual IAnalysisColumn* GetColumn(const int nColIndex);
	virtual IAnalysisColumn* GetColumn(const CString strColName);
	virtual void Release();
private:
	CMapStringToPtr    m_columnMap;
};

class CAnalysisTableSet: public IAnalysisTableSet
{
public:
	CAnalysisTableSet();
	~CAnalysisTableSet();
	virtual IAnalysisTable* GetTable(CString strName);
};

