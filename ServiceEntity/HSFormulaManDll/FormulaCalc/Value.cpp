// Valoare.cpp: implementation of the CValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Express.h"
#include "Value.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CValue, CObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CValue::CValue()
{
	m_dStyle = 0;
	m_value  = NULL;	
}

CValue::~CValue()
{
	if(m_value != NULL)
	{
		delete m_value;
		m_value = NULL;
	}
}

void CValue::Serialize(CArchive & ar)
{	
}


void CValue::UpdateValue()
{
}

double CValue::GetValue(HSCurDataInfoStatus* pCurDataStatus)
{
	return 0;
}

void CValue::SetValue(void* value)
{
	m_value = value;
}
