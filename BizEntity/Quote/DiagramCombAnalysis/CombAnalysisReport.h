#pragma once
#include "DrawTableBase.h"

#define COMBANALYSISREPOR_ROW_HEIGHT       22  // �и�
class CCombAnalysisReport: public CDrawTableBase
{
public:
	CCombAnalysisReport(void);
	~CCombAnalysisReport(void);

	virtual BOOL Create(IDrawBaseParam *pParam);
protected:
	void InitReport();
};
