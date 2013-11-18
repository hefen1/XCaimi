#pragma once
#include "muParserDef.h"

class CFormulaSysFunc
{
public:
	CFormulaSysFunc(void);
	~CFormulaSysFunc(void);

	//�������
	static double GetNewPrice(const char* code);
	static double GetNAV(const char* code);
	static double GetClose(const char* code);
	static double GetOpen(const char* code);
	static double GetBPrice(const char* code,double flag);
	static double GetSPrice(const char* code,double flag);
	//������
	static double GetCombPrice(const char* code); 
	static double GetCombBuyPKSZ(const char* code);
	static double GetCombSellPKSZ(const char* code);
};
