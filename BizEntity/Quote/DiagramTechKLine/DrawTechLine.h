
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
*	完成日期：  2010-12-1
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#include "TechKLineImp.h"

class CDrawTechLine : public CTechKLineImp
{
protected:
	CString									m_strName;
	CArray<CLineProperty*,CLineProperty*>	m_arLine;
	int										m_nStockDayDataLen;
	BOOL									m_bInitFormFenshi;		//分时中的技术线
	CRect									m_rcClose;
protected:
	//构造
public:
	CDrawTechLine(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth = 0, int nCellNum = 0);
	virtual ~CDrawTechLine();

//interface:
public:
	virtual BOOL		SetRect(CRect rc);
	virtual BOOL		MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual int			OnUpdateData(DataHead* pData);
	virtual int			Draw(CDC* pDC);
	virtual void		SetHorizCellNum(int nHorizCellNum);
	virtual void		GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual BOOL		IsMainLine();
	virtual BOOL		GetVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& ayTitle, int nDataIndex);
	virtual	short		GetLineType();
	virtual void		SetTeckKLineName(CString strName);
	virtual CString		GetTeckKLineName();
};