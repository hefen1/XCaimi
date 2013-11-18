// VarValue.cpp: implementation of the CNumericValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VarValue.h"
#include "FxjFormulaStruct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CNumericValue, CValue)

CNumericValue::CNumericValue()
{
	m_value = NULL;
	m_pMax  = NULL;
	m_pMin  = NULL;
	m_pTestStep = NULL;

	m_pDescribe = NULL;

	AddStyle(HX_EXPRESSNUMBER);
}

CNumericValue::~CNumericValue()
{
	if(m_value != NULL)
	{
		delete (double*)m_value;
		m_value = NULL;
	}

	if(m_pMax != NULL)
	{
		delete (double*)m_pMax;
		m_pMax = NULL;
	}
	if(m_pMin != NULL)
	{
		delete (double*)m_pMin;
		m_pMin = NULL;
	}

	if(m_pTestStep != NULL)
	{
		delete (double*)m_pTestStep;
		m_pTestStep = NULL;
	}
	
	if(m_pDescribe != NULL)
	{
		delete m_pDescribe;
		m_pDescribe = NULL;
	}
}

void CNumericValue::Serialize(FxjParam* pParam)
{
	if(m_value == NULL)
	{
		m_value = new double;
	}
	*(double*)m_value = pParam->m_fDefault;
	
	if(m_pMax == NULL)
	{
		m_pMax = new double;
	}
	*(double*)m_pMax = pParam->m_fMax;
	
	if(m_pMin == NULL)
	{
		m_pMin = new double;
	}
	*(double*)m_pMin = pParam->m_fMin;

	/*if(m_pTestStep == NULL)
	{
		m_pTestStep = new double;
	}
	*(double*)m_pTestStep = pParam->m_pTestStep;*/
}

void CNumericValue::Copy(CNumericValue* pRefValue)
{
	if( pRefValue == NULL || 
		!pRefValue->IsStyle(HX_EXPRESSNUMBER) )
		return;

	if(m_value == NULL)
	{
		m_value = new double;
	}
	*(double*)m_value = pRefValue->GetValue();
	
	if(m_pMax == NULL)
	{
		m_pMax = new double;
	}
	*(double*)m_pMax = pRefValue->GetMaxValue();
	
	if(m_pMin == NULL)
	{
		m_pMin = new double;
	}
	*(double*)m_pMin = pRefValue->GetMinValue();

	if(m_pTestStep == NULL)
	{
		m_pTestStep = new double;
	}
	*(double*)m_pTestStep = pRefValue->GetTestStepValue();
}

void CNumericValue::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{		
		if(m_value != NULL)
		{
			ar << *(double*)m_value;			
		}
		
		if(m_pMax != NULL)
		{
			ar << *(double*)m_pMax;
		}

		if(m_pMin != NULL)
		{
			ar << *(double*)m_pMin;
		}
		
		ar << GetDescribe();
		
		if(m_pTestStep != NULL)
		{
			ar << *(double*)m_pTestStep;
		}
	}
	else
	{
		if(m_value == NULL)
		{
			m_value = new double;
		}
		ar >> *(double*)m_value;

		if(m_pMax == NULL)
		{
			m_pMax = new double;
		}
		ar >> *(double*)m_pMax;

		if(m_pMin == NULL)
		{
			m_pMin = new double;
		}
		ar >> *(double*)m_pMin;

		if(m_pDescribe == NULL)
		{
			m_pDescribe = new CString;
		}
		ar >> *m_pDescribe;
		// 
		if(m_pTestStep == NULL)
		{
			m_pTestStep = new double;
			*(double*)m_pTestStep = 0;
		}
		ar >> *(double*)m_pTestStep;
	}
}

double CNumericValue::GetValue(HSCurDataInfoStatus* pCurDataStatus)
{
	if(m_value == NULL) 
		return 0;

	return *((double*)m_value);
}
void CNumericValue::SetValueOnly(double value)
{
	if (m_value == NULL)
	{
		SetValue(value);
	}
	else
	{
		double *pdb = (double*)m_value;
		*pdb = value;
	}
}
void CNumericValue::SetValue(double value)
{
	if( m_value != NULL )
	{
		delete (double*)m_value;
		m_value = NULL;
	}

	double *val = new double;
	*val = value;
	CValue::SetValue(val);
}

double CNumericValue::GetMaxValue()
{
	if(m_pMax == NULL) 
		return 0;

	return *((double*)m_pMax);
}

void CNumericValue::SetMaxValue(double value)
{
	if(m_pMax != NULL)
	{
		delete (double*)m_pMax;
		m_pMax = NULL;
	}

	double *val = new double;
	*val = value;
	m_pMax = val;
}

void CNumericValue::SetTestStepValue(double value)
{
	if(m_pTestStep != NULL)
	{
		delete (double*)m_pTestStep;
		m_pTestStep = NULL;
	}

	double *val = new double;
	*val = value;
	m_pTestStep = val;
}

double CNumericValue::GetTestStepValue(void)
{
	if(m_pTestStep == NULL) 
		return 0;

	return *((double*)m_pTestStep);
}

double CNumericValue::GetMinValue()
{
	if(m_pMin == NULL) 
		return 0;

	return *((double*)m_pMin);
}

void CNumericValue::SetMinValue(double value)
{
	if(m_pMin != NULL)
	{
		delete (double*)m_pMin;
		m_pMin = NULL;
	}

	double *val = new double;
	*val = value;
	m_pMin = val;
}

void CNumericValue::SetDescribe(CString strDescribe)
{
	if(m_pDescribe != NULL)
	{
		delete m_pDescribe;
		m_pDescribe = NULL;
	}
	m_pDescribe = new CString(strDescribe);	
}

CString CNumericValue::GetDescribe()
{
	if(m_pDescribe != NULL)
	{
		return *m_pDescribe;
	}
	return "";
}

