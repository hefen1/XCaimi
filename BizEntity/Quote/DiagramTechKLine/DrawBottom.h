/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	TechKlineImp.h
*	文件标识：
*	摘	  要：  技术分析图元 - 底部单元格
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
#pragma once
#include "..\..\..\Common\QuoteComm\DrawBase.h"
#define DRAWBOTTOM_HEIGHT	20
class CTechKLineMgr;
class CDrawBottom : public CDrawBase
{
	//成员
protected:
	CTechKLineMgr*		m_pTeckKLineMgr;
	int					m_nHorPosition;				//水平方向上的位置
	WORD				m_wPeriodType;				//周期类型
	CString				m_strTips;
	CString				m_strPeriodName;			//周期名称
	CMenu*				m_pMenu;
	HCURSOR				m_hCursor;
	//构造
public:
	CDrawBottom(CTechKLineMgr* pMgr, IRegionNode* pRegion);
	virtual ~CDrawBottom();
	//interface:
public:
	virtual BOOL		Create(IDrawBaseParam *pParam);
	virtual int			Draw(CDC* pDC);
	virtual BOOL		MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);

	void				UpdatePositon(int nIndex, CPoint pt);
	void				UpdatePeriodType(WORD wType);
	WORD				GetPeriodType();
	int					TrackPopUpMenu(int nTrackID, CString strMenuString);
	WORD				FormatPeriod(int nTrackID);
	int					HitTest(CPoint pt);
};