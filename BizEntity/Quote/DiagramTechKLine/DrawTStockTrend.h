#pragma once
/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	DrawTVolume.h
*	文件标识：
*	摘	  要：  成交量图元接口实现
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2011-3-1
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/

#include "TechKLineImp.h"
class CDrawTStockTrend : public CTechKLineImp
{
protected:
	int					m_nVertCellHeight;
	int					m_nSplitNums;
public:
	CDrawTStockTrend(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth);
	virtual ~CDrawTStockTrend();

//interface:
public:
	virtual BOOL		SetRect(CRect rc);
	virtual int			Draw(CDC* pDC);
	virtual void		GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual BOOL		RecalcRect(CRect rcArea);
	virtual BOOL		IsMainLine();
	virtual int			GetDataYPos(int nDataIndex);
	virtual void		SetHorizCellNum(int nHorizCellNum);
	virtual int			GetMaxHorizCellNum();
	virtual	short		GetLineType();
};

class CDrawTStockTrendVolume : public CDrawTStockTrend
{
public:
	CDrawTStockTrendVolume(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth);
	virtual ~CDrawTStockTrendVolume();
public:
	virtual BOOL		SetRect(CRect rc);
	virtual int			Draw(CDC* pDC);
	virtual void		GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual BOOL		IsMainLine();
	virtual	short		GetLineType();
	virtual CString		GetTeckKLineName();
};

class CDrawTStockTrendAdd : public CDrawTStockTrend
{
protected:
	ColorProperty*					m_pColor;				//颜色指针
	//分时数据
	CFenshiData						m_dataFenshi;		

public:
	CDrawTStockTrendAdd(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth);
	virtual ~CDrawTStockTrendAdd();

//method:
	BOOL				GetRequestData(IRequestCollector* pReqCollector, StockUserInfo* pStockInfo, int nPeriod, int nReqDays);
	int					SetColor(ColorProperty* pColor);
	virtual int			OnUpdateData(DataHead* pData);
	virtual BOOL		IsMainLine();
	virtual	short		GetLineType();
	virtual void		GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual int			Draw(CDC* pDC);
	virtual BOOL		GetVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& ayTitle, int nDataIndex);
	//
	BOOL				UpdateDataPos(int nReqDay, int nDataLen);
	BOOL				CheckRecvComplete();
};