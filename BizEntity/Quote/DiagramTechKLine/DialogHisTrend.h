#pragma once


// CDialogHisTrend dialog
#include "Resource.h"
#include "TechKLineImp.h"
#include "ManagerBase.h"
#include "DrawHistrendRTable.h"

class CTechKLineMgr;
class CDialogHisTrend : public CDialog, public CManagerBase
{
	DECLARE_DYNAMIC(CDialogHisTrend)
protected:
	std::list<CTechKLineImp*>		m_listFenshi;			//��ʱͼԪ����
	IRegionNode*					m_pRegion;				//ͼԪ���ڵ�����
	StockUserInfo					m_stkInfo;				//����
	CTechKLineMgr*					m_pTeckKLineMgr;
	CDrawHisTrendRTable*			m_pHisTrendRTable;
	HsLOGPEN						m_penBorder;

protected:
	void							SetChildRect();
//
public:
//CManagerBase interface
	virtual StockUserInfo*						GetStockUserInfo();
	virtual void								GetTopMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual int									DrawTitle(void* pRefDraw);
	virtual short								GetKLineType();
	virtual CWnd*								GetParentWnd();
	/*��ȡ��ͼ�ϵ������Сֵ*/
	void										GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);

//����
	void										SetHisTrendDate(long lDate);
	int											OnUpdateData(DataHead* pData, StockDay* pStockDayData, long lPreClose, long lFinaceTotalHand);
	void										SetMainMgr(CTechKLineMgr* pMgr);
	void										UpdateStock(StockUserInfo* pStockUserInfo);
	void										SetStockType(StockType* pStockType);
public:
	CDialogHisTrend(StockUserInfo* pStockUserInfo, IRegionNode* pRegion, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogHisTrend();

// Dialog Data
	enum { IDD = IDD_HISTREND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnPrevTrend();
	afx_msg void OnNextTrend();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
