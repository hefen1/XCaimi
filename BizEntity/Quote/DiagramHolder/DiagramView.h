#pragma once

#include "DiagramLayer.h"
#include "QuoteDiagram.h"
#include "DataSourceDefineEx.h"
#include "DataSourceSinkDefine.h"
#include "HSBizDialog.h"
#include "DiagramLayerSelector.h"

class CRequestCollector: public IRequestCollector
{
public:
	CRequestCollector();
	~CRequestCollector();
	virtual void AddRequest(char* pAskData,short lAskLen);
	virtual void Release();
	void DoRequest(IDataSourceEx* pDataSource,long nDataSourceID, BOOL bAutoClear = TRUE);
private:
	CArray<char*,char*> m_arrReqData;
	CArray<short,short> m_arrReqLen;
};

// CDiagramView

class CWndUpdateLocker
{
public:
	CWndUpdateLocker(CWnd* pWnd) 
	{
		m_pWnd = pWnd;
		if(m_pWnd)
			m_pWnd->LockWindowUpdate();
	}
	~CWndUpdateLocker()
	{
		if(m_pWnd)
			m_pWnd->UnlockWindowUpdate();
	}
private:
	CWnd* m_pWnd;
};

class CDiagramView : public CView
{
	DECLARE_DYNAMIC(CDiagramView)

public:
	CDiagramView();
	virtual ~CDiagramView();

	BOOL CreateView(CHSBizDialog* pBizDlg);
	void SetDataSource(IDataSourceEx* pDataSource,long nSourceID);
	BOOL HSDataSourceSink_OnCommNotify(void* pData);
	BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen);
	void GetRequestData(StockUserInfo* pStockInfo);
	void SetNeighbourStockInfo(StockUserInfo* pTargetCode);
	CDiagramLayer* ChangeDiagramLayer(CString strLayoutName,StockUserInfo* pStockInfo);
	BOOL IsActiveDiagramLayer(CString strLayoutName);
	BOOL OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);
	BOOL OnKeyCommand(WPARAM wParam, LPARAM lParam);
	void OpenFixedLayout(CString strFixedLayoutName);
	CDiagramLayer* GetCurrentLayout() { return m_pLayout; }
	void OnEsc();
	void OnF1();
	void OnF2();
	void OnF3();
	void OnF4();
	void OnF5();
	void OnF6();
	void OnF7();
	void OnF8();
	void OnF10();
	void OnPrior();
	void OnNext();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

protected:
	CDiagramLayer*         m_pLayout;
	CHSBizDialog*          m_pHolder;
	IDataSourceEx*		   m_pDataSource;				// 数据引擎接口指针
	long				   m_nDataSourceID;			// 数据请求ID
	CDiagramLayerSelector  m_LayerSelector;

	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void InitKeyboradCmd();
	BOOL IsQuoteReprotKeyboradCmd(int nCmd);
	BOOL IsTechAnalysisKeyboradCmd(int nCmd);
	BOOL IsFunctionKeyboradCmd(int nCmd);
	void OnFunctionKeyboradCmd(int nCmd);

private:
	StockUserInfo   m_currentStockCode;
	CArray<StockUserInfo> m_neighbourStockCodes;
	int            m_currentStockIndex; //在m_neighbourStockCodes的位置
	StockUserInfo* GetNeighbourStockCode(int nIndex);
	CString        m_strFixedLayoutName;
	BOOL           m_bFixedLayoutMode;
	CArray<int, int> m_aReportKeyboardCmd;
	CArray<int, int> m_aTechAnalysisKeyboardCmd;
	CArray<int,int> m_aFunctionKeyboardCmd;
};


