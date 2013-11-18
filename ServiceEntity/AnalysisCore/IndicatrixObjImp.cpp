#include "StdAfx.h"
#include "IndicatrixObjImp.h"
#include "..\HelperLib\GeneralHelper.h"

#define  UNASSIGNED_VALUE      0xFFFF

CIndicatrixObj::CIndicatrixObj(void)
{
	m_pExp = NULL;
	m_dLastValue = 0;
	m_dMaxValue = UNASSIGNED_VALUE;
	m_dMiniValue = UNASSIGNED_VALUE;
	m_nValidDataCount = 0;
	m_pIndicatrixValue = new DrawIndicatrixData[IndicatrixValuesLen];
	memset(m_pIndicatrixValue,0,sizeof(DrawIndicatrixData) *  IndicatrixValuesLen);
}

CIndicatrixObj::~CIndicatrixObj(void)
{
	if(m_pExp)
		delete m_pExp;
	if(m_pIndicatrixValue)
		delete[] m_pIndicatrixValue; 
}


BOOL CIndicatrixObj::Save(CString strFileName)
{
	CString strSaveFile,strSaveDir;
	CTime tm = CTime::GetCurrentTime();
	CString strDay = tm.Format(_T("%Y%m%d"));
	strSaveDir.Format(_T("%scaches\\indicatrix\\%s"),CGeneralHelper::GetAppPath(),
		strDay);
	strSaveFile.Format(_T("%s\\%s.dat"),strSaveDir,strFileName);

	CreateDirectory(strSaveDir,NULL);

	CFile file;
	if ( file.Open(strSaveFile, CFile::modeCreate | CFile::modeReadWrite) )
	{	
		file.Write(&m_head,sizeof(m_head));
		file.Write(&m_dMaxValue,sizeof(m_dMaxValue));
		file.Write(&m_dMiniValue,sizeof(m_dMiniValue));
		file.Write(m_pIndicatrixValue,sizeof(DrawFenShiData) * IndicatrixValuesLen);
		file.Flush();
	}
	return TRUE;
}

BOOL CIndicatrixObj::Load(CString strFileName)
{
	CString strSaveFile,strSaveDir;
	CTime tm = CTime::GetCurrentTime();
	CString strDay = tm.Format(_T("%Y%m%d"));
	strSaveDir.Format(_T("%scaches\\indicatrix\\%s"),CGeneralHelper::GetAppPath(),
		strDay);
	strSaveFile.Format(_T("%s\\%s.dat"),strSaveDir,strFileName);

	CreateDirectory(strSaveDir,NULL);

	CFile file;
	if ( file.Open(strSaveFile, CFile::modeReadWrite) )
	{
		file.SeekToBegin();
		file.Read(&m_head,sizeof(m_head));
		file.Read(&m_dMaxValue,sizeof(m_dMaxValue));
		file.Read(&m_dMiniValue,sizeof(m_dMiniValue));
		file.Read(m_pIndicatrixValue,sizeof(DrawFenShiData) * IndicatrixValuesLen);
	}
	return FALSE;
}

BOOL CIndicatrixObj::BuildExpressionObj(CString strExp,IWatcherParams* pWatcherParams)
{
	//加锁保护
	CSingleLock singleLock(&m_critSection,TRUE);

	if (m_pExp == NULL)
		m_pExp = new CCustomExpression();
	m_pExp->SetExpression(strExp);
	m_pExp->SetParams(pWatcherParams);
	return TRUE;
}

void CIndicatrixObj::DoCalculate(const CTime& now )
{
	//加锁保护
	CSingleLock singleLock(&m_critSection,TRUE);

	int nPos =GetPos(now);

	double dValue = m_pExp->CalculateValue();

	//更新最新，最大，最小
	m_dLastValue = dValue;
	if( nPos >= 0 )
	{	
		if(UNASSIGNED_VALUE == m_dMaxValue )
			m_dMaxValue = m_dLastValue;
		if(UNASSIGNED_VALUE == m_dMiniValue )
			m_dMaxValue = m_dLastValue;

		if( m_dLastValue > m_dMaxValue)
			m_dMaxValue = m_dLastValue;
		if( m_dLastValue < m_dMiniValue )
			m_dMiniValue = m_dLastValue;

		m_nValidDataCount = nPos;
		m_pIndicatrixValue[nPos].m_dValue = dValue;
	}	
}

int CIndicatrixObj::GetPos(const CTime &tm)
{
	int nPos = -1;
	int nH,nM,nS,lTime;
	char szTime[32];
	sprintf(szTime, "%d%02d%02d", tm.GetHour(),tm.GetMinute(),tm.GetSecond());
	lTime = atol(szTime);

	nH = lTime / 10000 ;
	nM = lTime%10000 / 100;
	nS = lTime % 100; 

	if (lTime >= 93000 && lTime  <= 113000 )
	{
		CTime t1(tm.GetYear(),tm.GetMonth(),tm.GetDay(),nH,nM,nS);
		CTime t2(tm.GetYear(),tm.GetMonth(),tm.GetDay(),9,30,0);
		CTimeSpan ts = t1 - t2;
		nPos = ts.GetTotalSeconds() + 1;
	}
	else if (lTime >= 130000 /*&& lTime <= 150000 */)
	{
		CTime t1(tm.GetYear(),tm.GetMonth(),tm.GetDay(),nH,nM,nS);
		CTime t2(tm.GetYear(),tm.GetMonth(),tm.GetDay(),13,0,0);
		CTimeSpan ts = t1 - t2;
		nPos = ts.GetTotalSeconds() + 2 * 60* 60;	
	}

	if ( nPos >= IndicatrixValuesLen)
		nPos = IndicatrixValuesLen -1;
	return nPos;
}

BOOL CIndicatrixObj::GetDrawIndicatrixData(DrawIndicatrixData* pDrawFenShiData,int nDataCount)
{
	//加锁保护
	CSingleLock singleLock(&m_critSection,TRUE);

	if(nDataCount <= 0 || nDataCount > IndicatrixValuesLen)
		return FALSE;
	memcpy(pDrawFenShiData,m_pIndicatrixValue,sizeof(DrawIndicatrixData) * nDataCount);
	return TRUE;
}

BOOL CIndicatrixObj::GetOtherData(double &dMax,double &dMin,int &nValidCount)
{
	//加锁保护
	CSingleLock singleLock(&m_critSection,TRUE);

	if(m_dMaxValue != UNASSIGNED_VALUE)
		dMax = m_dMaxValue;
	else
		dMax = m_dLastValue;

	if(m_dMaxValue != UNASSIGNED_VALUE)
		dMin = m_dMiniValue;
	else
		dMin = m_dLastValue;

	if(m_nValidDataCount == 0)
		m_nValidDataCount = GetPos(CTime::GetCurrentTime());
	nValidCount = m_nValidDataCount;
	return TRUE;
}

void CIndicatrixObj::Release() 
{
	delete this;
}