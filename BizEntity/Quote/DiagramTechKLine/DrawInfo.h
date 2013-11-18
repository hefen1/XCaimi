#pragma once
/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	DrawInfo.h
*	文件标识：
*	摘	  要：  信息地雷显示
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-5-6
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#include "..\..\..\Common\QuoteComm\DrawBase.h"
#include "..\..\..\Common\ConfigInterface.h"
#include "..\DiagramFoundation\DiagramComm.h"
#include "..\DiagramFoundation\AnalysisTextData.h"
#include "..\DiagramFoundation\DHtmlShowDlg.h"

class CTechKLineMgr;

struct CDrawInfoUnitData
{
	long	m_lDateTime;				//日期时间
	CPoint	m_ptPosition;				//信息地雷对应的坐标点
	CPoint	m_ptChuquan;				//除权点对应的坐标
};

struct InfoIndexEx : public InfoIndex
{
	BOOL	m_bIsShow;
	CRect	m_rect;
	BOOL	m_bSelected;

	InfoIndexEx();
	BOOL	Copy(InfoIndex* pInfo);
	void	SetRect(CRect rc = CRect(0, 0, 0, 0));
};

class CDrawInfo : public CDrawBase
{
protected:
	CTechKLineMgr*				m_pMgr;
	CString						m_strInfoPath;
	IHsInfo*					m_pInfo;
	CArray<InfoIndexEx*>		m_ayInfoIndex;	// 咨询索引对象
//	CArray<InfoIndexEx*>		m_ayInfoChuquan;// 除权索引对象
	CArray<CDrawInfoUnitData>	m_ayUnitData;	//画图控制
	int							m_nSelInfoIndex;	//选中的对象
	CRect						m_rcInvalidateOld;	//原有的刷新区
	int							m_nTipTimeID;		//定时刷新ID
	CDHtmlShowDlg*				m_pDlgShowHtml;		// html对话框
	CArray<InfoIndexEx*>		m_ayInfoIndexSelect;// 咨询索引对象:选中的部分
	int							m_nItemHeight;
public:
	CDrawInfo(CTechKLineMgr* pMgr, IHsInfo* pInfo, IRegionNode* pNode);
	virtual ~CDrawInfo();

//method:
	virtual BOOL		GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce);
	virtual int			OnUpdateData(DataHead* pData);
	virtual BOOL		MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);

	virtual int			Draw(CDC* pDC);
	int					EmptyData();
	void				KillTipTimer();
	void				RequestText(int nSelIndex);
	int					ResetSelectInfoIndex();
	int					DrawInfo(CDC* pDC);
	int					EmptyChuquanData();
	
	static VOID CALLBACK    ShowTipProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);  // 显示Tip     
};