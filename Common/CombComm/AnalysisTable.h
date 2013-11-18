#pragma once

#define OJB_AnalysisTableSet        _T("AnalysisCore.CAnalysisTableSet")

#define PT_STRING         'S'
#define PT_NUM            'N'
struct IExpParamInfo
{
	virtual CString GetName() = 0;
	virtual char     GetType() = 0;
	virtual CString GetNote() = 0;
	virtual CString GetFlag() = 0;
};

struct IExpressionInfo
{
	virtual CString GetName() = 0;
	virtual CString GetExpression() = 0;
	virtual void GetParams(CMapStringToString &mapParams ) = 0;
	virtual int  GetParamCount() = 0;
	virtual IExpParamInfo* GetParamInfo(int nIndex) = 0;
};

struct IAnalysisColumn
{
	virtual CString GetName() = 0;//获取列名（英文名,一般作为内部索引用）
	virtual CString GetDispName() = 0;//获取显示列名
	virtual CString GetExpression() = 0;//获取对应的表达式串
	virtual int        GetColumIndex() = 0;//获取列位置
	virtual int        GetColWidth() = 0;//获取列宽
	virtual CString GetDispFormat() = 0;//获取格式化串
	virtual IExpressionInfo* GetExpInfo() = 0;//获取公式信息指针

	virtual void SetName(CString strName) = 0;
	virtual void SetDispName(CString strDispName) = 0;
	virtual void SetExpName(CString strExpName) = 0;
	virtual void SetColumIndex(int nIndex) = 0;
	virtual void SetColWidth(int nWidth) = 0;
	virtual void SetDispFormat(CString strDispFormat) = 0;

	virtual void Release() = 0;
};

struct IAnalysisTable
{
	virtual BOOL AddColumn(CString strColName,IAnalysisColumn* pCol) = 0;
	virtual int GetColumnCount() = 0;
	virtual IAnalysisColumn* GetColumn(const int nColIndex) = 0;
	virtual IAnalysisColumn* GetColumn(const CString strColName) = 0;
	virtual void Release() = 0;
};

struct IAnalysisTableSet
{
	virtual IAnalysisTable* GetTable(CString strName) = 0;
};

