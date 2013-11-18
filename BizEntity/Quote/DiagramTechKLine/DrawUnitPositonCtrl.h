#pragma once
/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	DrawUnitPositonCtrl.h
*	文件标识：
*	摘	  要：  位置控制单元 及位置控制类
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-5-12
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#include "TechKlineImp.h"
#include "..\..\..\Common\ConfigInterface.h"
class CTechKLineMgr;

#define CALC_POSITION_SPLIT		3
class CDrawUnitPositonCtrl
{
protected:
	CRect						m_rectSelf;
	std::list<CTechKLineImp*>	m_listLinesAbove;			//位于控制单元之上的图元列表
	std::list<CTechKLineImp*>	m_listLinesBelow;				//位于控制单元之下的图元列表
	CTechKLineMgr*				m_pMgr;
	int							m_nTotalHeight;
	float						m_fPosRatio;
public:
	CDrawUnitPositonCtrl(CTechKLineMgr* pMgr, int nTotalHeight);
	~CDrawUnitPositonCtrl();
	BOOL								MouseDown(UINT nFlags, CPoint point);
	void								SetRect(CRect rc);
	CRect								GetRect();
	void								SetRatio(float fRatio);
	float								GetRatio();
	void								AddLinks(CTechKLineImp* pImp, BOOL bIsAbove);
	BOOL								UpdatePostion(int nNewPosY);
	int									GetResetPostion(CTechKLineImp* pImp, BOOL bTop, int nTotalHeightNew);
	int									GetLinksName(int nType, CStringArray& ayLinesName, BOOL bTop);	
	/*获取指定关联的控制单元中控制图元的个数*/
	int									IsInMyCtrl(CTechKLineImp* pImp, BOOL bTop);
	/*位移关联图元位置(Y方向)*/
	int									ResetLinkedImpPos(CRect rcDel, BOOL bFirst);
};

class CDrawUnitPosCtrlMgr
{
protected:
	CTechKLineMgr*						m_pMgr;
	std::list<CDrawUnitPositonCtrl*>	m_listUnitLink;
	short								m_nType;
	BOOL								m_bLastSetCursor;	//上一次设置过光标
	HCURSOR								m_hCursor;
	HCURSOR								m_hCursorOld;
	int									m_nTotalHeight;
	int									m_nTotalWidth;
	IHSKLinePosition*					m_pConfig;
	IRegionNode*						m_pRegionNode;
public:
	CDrawUnitPosCtrlMgr(CTechKLineMgr* pMgr, IRegionNode* pNode);
	~CDrawUnitPosCtrlMgr();
	BOOL								Init();
	BOOL								Save(int nType);
	void								EmptyOldCtrl();
	BOOL								MouseMove(UINT nFlags, CPoint point);
	BOOL								MouseDown(UINT nFlags, CPoint point);
	BOOL								Update(int nTotalHeight, int nTotalWidth, BOOL bUseOldRatio = FALSE);
	void								AddLinks(CTechKLineImp* pImp, int nSetHeight);
	int									OnResetRect(CRect rcNew);
	BOOL								ResetLinksRect(CTechKLineImp* pImp, CRect rcNew, float fCellWidth);		

//method:
	int									CreateLines(int nType, std::list<CTechKLineImp*>* pListTarget, float fCellWidth);
	//响应删除某个图元的操作
	int									OnDeleteImp(CTechKLineImp* pImp);
protected:
	CTechKLineImp*						CreateFormFile(int nType, CString strName, float fCellWidth, BOOL bAddLine = FALSE, BOOL bBottom = FALSE);
	/*当配置文件不存在时，默认创建的*/
	int									CreateDefault(int nType, std::list<CTechKLineImp*>* pListTarget, float fCellWidth);
	/*找到指定子图元关联的位置控制单元(下方向)*/
	CDrawUnitPositonCtrl*				FindBelowLinkedCtrl(CTechKLineImp* pImp);
};