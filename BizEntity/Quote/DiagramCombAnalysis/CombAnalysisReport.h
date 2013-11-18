#pragma once
#include "DrawTableBase.h"

#define COMBANALYSISREPOR_ROW_HEIGHT       22  // ÐÐ¸ß
class CCombAnalysisReport: public CDrawTableBase
{
public:
	CCombAnalysisReport(void);
	~CCombAnalysisReport(void);

	virtual BOOL Create(IDrawBaseParam *pParam);
protected:
	void InitReport();
};
