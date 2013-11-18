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
	CString                                m_strName;         //ָ������
	CCustomExpression*             m_pExp;              //ָ������
	CCriticalSection                    m_critSection;
	IndicatrixFileHead                 m_head;              //ָ�껺���ļ�ͷ
	double                                m_dLastValue;      //ָ������ֵ
	double                                m_dMaxValue;     //ָ�����ֵ
	double                                m_dMiniValue;		//ָ����Сֵ
	int                                       m_nValidDataCount; //��ǰ�����е���Ч��������ʱ�����Ʊ��
	DrawIndicatrixData*              m_pIndicatrixValue;//ָ����������
};
