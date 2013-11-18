#pragma once
/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawTVolume.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  �ɽ���ͼԪ�ӿ�ʵ��
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2011-3-1
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
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
	ColorProperty*					m_pColor;				//��ɫָ��
	//��ʱ����
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