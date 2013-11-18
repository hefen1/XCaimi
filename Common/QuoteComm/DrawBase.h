#pragma once
#include "QuoteDiagram.h"
#include "QuoteDefine.h"

#define DrawType_UnKnow                     0x0000
#define DrawType_FenShi                        0x0001  // 分时图
#define DrawType_KLine                          0x0002  //K线

class CDrawBaseParam: public IDrawBaseParam
{
private:
	CMapStringToString m_valueMap;
public:
	void AddParam(CString strParamName,CString strValue);

	virtual CString WINAPI GetStringParam(CString strParamName,CString strDefault = _T(""));
	virtual int WINAPI GetIntegerParam(CString strParamName,int nDefault = 0);
	virtual double WINAPI GetDoubleParam(CString strParamName,double nDefault = 0);
	virtual POSITION GetStartPosition();
	virtual void GetNextAssoc(POSITION& rNextPosition,CString& strParamName,CString& strValue);
};

class CDrawBase: public IDrawBase
{
public:
	CDrawBase::CDrawBase();
	void  SetDrawType(int nDrawType) { m_nDrawType = nDrawType; } 
	BOOL  IsDrawType (int nDrawType) { return m_nDrawType == nDrawType; }
	virtual ~CDrawBase() {};
	virtual BOOL Create(IDrawBaseParam *pParam) {return TRUE;}
	virtual CString GetName() { return m_strName;}
	virtual void SetName(CString strName) { m_strName = strName;}
	virtual void SetRegionNode(IRegionNode* pNode);
	virtual BOOL SetRect(CRect rc);
	virtual CRect	GetRect();
	virtual void SetVisible(BOOL bVisible) {m_bVisable = bVisible;};
	virtual BOOL GetVisible() {return m_bVisable; };
	virtual void UpdateCodeInfo(StockUserInfo* pCodeInfoNew);
	virtual StockUserInfo* GetCodeInfo();
	virtual short	GetDrawType();
	//鼠标消息
	virtual BOOL MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL MouseUp(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	virtual BOOL RButtonUp(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	virtual BOOL OnCursor(UINT nFlags, CPoint point);
	virtual int	 OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual int  OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);
	// 菜单消息
	virtual long OnMenuMsg(WPARAM wParam, LPARAM lParam);
	virtual int Draw(CDC* pDC);
	virtual BOOL GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce);
	virtual int	OnUpdateData(DataHead* pData);
	virtual void OnActiveChange(BOOL bShow);
	virtual void OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam);
	virtual int Delete();
protected:
	BOOL              m_bVisable;   //是否可见
	CRect              m_rect;       //图元所在的rect
	int                   m_nDrawType;//图元类型标识
	StockUserInfo     m_stkInfo;    //当前图元关联的代码
	IRegionNode*      m_pRegion;    //图元所在的区域
	CString           m_strName;    //图元名称
};