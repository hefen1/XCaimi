#pragma once
#include <afxmt.h>
#include "CustomExpression.h"
#include "CombComm/IndicatrixObj.h"

#define  IndicatrixFileVersion1  1

struct IndicatrixFileHead
{
	long m_lVersion;
	IndicatrixFileHead()
	{
		m_lVersion = IndicatrixFileVersion1;
	}
};

class CIndicatrixObj: public IIndicatrixObj
{
public:
	CIndicatrixObj(void);
	~CIndicatrixObj(void);
	virtual void SetIndicatrixName(CString strName) { m_strName = strName; }
	virtual CString GetIndicatrixName() { return m_strName; }
	virtual CString GetExpression() { return m_pExp->GetExpression(); }
	virtual BOOL BuildExpressionObj(CString strExp,IWatcherParams* pWatcherParams);
	virtual void DoCalculate(const CTime& now );
	virtual double GetLastValue() { return m_dLastValue;}
	virtual BOOL Save(CString strFileName);
	virtual BOOL Load(CString strFileName);
	virtual	BOOL GetDrawIndicatrixData(DrawIndicatrixData* pDrawIndicatrixData,int nDataCount);
	virtual BOOL GetOtherData(double &dMax,double &dMin,int &nValidCount);
	virtual void Release();
protected:
	int GetPos(const CTime &tm);
private:
	CString                                m_strName;         //指标名称
	CCustomExpression*             m_pExp;              //指标表达试
	CCriticalSection                    m_critSection;
	IndicatrixFileHead                 m_head;              //指标缓存文件头
	double                                m_dLastValue;      //指标最新值
	double                                m_dMaxValue;     //指标最大值
	double                                m_dMiniValue;		//指标最小值
	int                                       m_nValidDataCount; //当前序列中的有效个数，随时间推移变更
	DrawIndicatrixData*              m_pIndicatrixValue;//指标数据序列
};
