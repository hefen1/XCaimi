#pragma once 
/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	DrawBox.h
*	文件标识：
*	摘	  要：  信息提示框
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-3-28
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#include "..\DiagramFoundation\DrawTableBase.h"
#include "GraphicsData.h"
#include "HSDrawBaseStruct.h"

class CTechKLineMgr;
class CDrawBox : public CDrawTableBase
{
protected:
	HsLOGPEN			m_penBorder;
	CString				m_strDate;
	CString				m_strTime;
	CTechKLineMgr*		m_pTeckKLineMgr;
	int					m_nRowCount;
	short				m_nType;

public:
	CDrawBox(CTechKLineMgr* pMgr, IRegionNode* pRegion);
	virtual ~CDrawBox();

//interface:
public:
	virtual BOOL		ReCreate(short nType);
	virtual int			Draw(CDC* pDC);
	virtual BOOL		SetRect(CRect rc);
	virtual BOOL		MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);

//method
	void				SetDateTime(CString strDate, CString strTime);
	void				UpdateData(StockUserInfo* pStock, DrawFenShiData* pFenshiData, long* plAvePrice, long* plHand);
	void				UpdateData(StockDay* pStockDayData, StockUserInfo* pStock, long lPreClose, long lFinaceTotalHand, CString strValue);
	void				SetFenshiFixedData();
	void				SetKLineFixedData();

	BOOL				GetCellItemAttribute(long lDataTarget, long lDataSource, WORD& wClr);
};
