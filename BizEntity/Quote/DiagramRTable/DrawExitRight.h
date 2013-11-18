#pragma once
#include "DrawTableBase.h"


#define ExitRight_Table_ShowColCount            1
#define ExitRight_Table_RowHeight              22

class CDrawExitRight: public CDrawTableBase
{
public:
	CDrawExitRight(IDataSourceEx* pDataSource);
	~CDrawExitRight(void);

	virtual	BOOL  GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	void  OnUpdateExitRightData(CurrentExRightData* pData);	
private:
	IDataSourceEx*	               m_pDataSource;   // 数据引擎接口指针
	CodeInfo                       m_preCodeInfo;   // 图元之前的代码信息
};
