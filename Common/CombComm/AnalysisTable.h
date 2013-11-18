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
	virtual CString GetName() = 0;//��ȡ������Ӣ����,һ����Ϊ�ڲ������ã�
	virtual CString GetDispName() = 0;//��ȡ��ʾ����
	virtual CString GetExpression() = 0;//��ȡ��Ӧ�ı��ʽ��
	virtual int        GetColumIndex() = 0;//��ȡ��λ��
	virtual int        GetColWidth() = 0;//��ȡ�п�
	virtual CString GetDispFormat() = 0;//��ȡ��ʽ����
	virtual IExpressionInfo* GetExpInfo() = 0;//��ȡ��ʽ��Ϣָ��

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

