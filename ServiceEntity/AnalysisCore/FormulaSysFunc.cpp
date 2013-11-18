#include "StdAfx.h"
#include "FormulaSysFunc.h"
#include "AnalysisCore.h"

CFormulaSysFunc::CFormulaSysFunc(void)
{
}

CFormulaSysFunc::~CFormulaSysFunc(void)
{
}


double CFormulaSysFunc::GetNewPrice(const char* code)
{
	return GetCombCacheSet()->GetPrice(code);
}

double CFormulaSysFunc::GetNAV(const char* code)
{
	return GetCombCacheSet()->GetNetValue(code);
}

double CFormulaSysFunc::GetClose(const char* code)
{
	return GetCombCacheSet()->GetClose(code);	
}

double CFormulaSysFunc::GetOpen(const char* code)
{
	return GetCombCacheSet()->GetOpen(code);	
}

double CFormulaSysFunc::GetBPrice(const char* code,double flag)
{
	int nFlag = (int)flag;
	return GetCombCacheSet()->GetBPrice(code,nFlag);	
}

double CFormulaSysFunc::GetSPrice(const char* code,double flag)
{
	int nFlag = (int)flag;
	return GetCombCacheSet()->GetSPrice(code,nFlag);	
}

double CFormulaSysFunc::GetCombPrice(const char* code)
{
	return GetCombCacheSet()->GetCombPrice(code);	
}

double CFormulaSysFunc::GetCombBuyPKSZ(const char* code)
{
	return GetCombCacheSet()->GetCombBPKSZ(code);	
}

double CFormulaSysFunc::GetCombSellPKSZ(const char* code)
{
	return GetCombCacheSet()->GetCombSPKSZ(code);	
}
 