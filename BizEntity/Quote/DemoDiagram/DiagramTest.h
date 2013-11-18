#pragma once
#include "DrawBase.h"
#include "InfomationForm.h"

class CDiagramTest: public CDrawBase
{
private:
	CInfomationForm   m_pInfoForm;
public:
	CDiagramTest(void);
	~CDiagramTest(void);

	virtual BOOL SetRect(CRect rc);
	virtual void SetRegionNode(IRegionNode* pNode);
	virtual int Draw(CDC* pDC);
	virtual BOOL MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL); 
	virtual BOOL MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL); 
	virtual BOOL GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo);

	virtual int	OnUpdateData(DataHead* pData);
	void OnActiveChange(BOOL bShow);
	virtual int Delete() { delete this; return 0;}
private:
	AnsRealTime m_realTime;
};
