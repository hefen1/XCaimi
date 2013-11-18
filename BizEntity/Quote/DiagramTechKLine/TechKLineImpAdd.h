/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	TechKlineImp.h
*	文件标识：
*	摘	  要：  技术分析图元接口（叠加） --- 实现
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-4-28
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "TechKLineImp.h"
#include "ManagerBase.h"

class CTechKLineImpAdd	: public CTechKLineImp
{
protected:
	//K线数据
	CTechKLineData					m_dataKLine;
	//数据长度
	long							m_lDataVauleLen;
	ColorProperty*					m_pColor;				//颜色指针
public:
	CTechKLineImpAdd(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth);
	virtual ~CTechKLineImpAdd();

//interface
public:
	virtual int			OnUpdateData(DataHead* pData);
	virtual int			Draw(CDC* pDC);
	virtual void		GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual	short		GetLineType();
	virtual BOOL		GetVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& ayTitle, int nDataIndex);
	virtual BOOL		IsMainLine();

//method:
	BOOL				GetRequestData(IRequestCollector* pReqCollector, StockUserInfo* pStockInfo, int nPeriod, WORD wPeriodType);
	int					SetColor(ColorProperty* pColor);
};