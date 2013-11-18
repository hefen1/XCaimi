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
	IDataSourceEx*	               m_pDataSource;   // ��������ӿ�ָ��
	CodeInfo                       m_preCodeInfo;   // ͼԪ֮ǰ�Ĵ�����Ϣ
};
