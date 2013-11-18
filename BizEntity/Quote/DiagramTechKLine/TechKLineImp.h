/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	TechKlineImp.h
*	文件标识：
*	摘	  要：  技术分析图元接口实现
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

#define SPLIT_COLUMN  2		//每个日期上的间隔
#include "ManagerBase.h"

enum etechLineType
{
	etechLineType_main		= 0x01,		//主图
	etechLineType_tech		= 0x02,		//指标
	etechLineType_stock		= 0x04,		//附加代码（股票或指数）

	etechLineType_all		= 0x0f,		//所有		
};

class CTechKLineImp : public CDrawBase
{
//构造
public:
	CTechKLineImp(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth);
	virtual ~CTechKLineImp();
protected:
//成员
	CManagerBase*		m_pTeckKLineMgr;

	CRect				m_rcGird;			//表格线大约位置
	int					m_nGridNum;			//间隔条目数
	int					m_nHoriPoints;			//横向, 数据个数, 一般分时图中为241个
	int					m_nHoriCellNum;			//横向, 小单元个数, 一般分时图中为241个
	int					m_nMaxHoriCellNum;
	float				m_fHoriCellWidth;		//横向单元格宽度
	float				m_fKLineWidth;			//K线宽度
	CRect				m_rcVarTitle;			//标题区域

	CHSLinear<float>*						m_pLinear;			// 绘图类
	DrawPointInfo*							m_pPointPos;		//指向绘制数据结构
	CKLineChartParam						m_sChartParam;		//K线参数
	BOOL									m_bFenshiDrawType;	//主图是否为分时图？
	BOOL									m_bAddLine;			//附加图
	BOOL									m_bBottomLine;		//底部图
	DWORD									m_dwLinearStyle;

protected:	
	CDispMaxMinValue					m_lCurMinValue;	//最小值数据结构
	CDispMaxMinValue					m_lCurMaxValue; //最大值数据结构

//interface:
public:
	virtual BOOL		Create(IDrawBaseParam *pParam);
	virtual void		SetRegionNode(IRegionNode* pNode);
	virtual BOOL		SetRect(CRect rc);
	virtual CRect		GetRect();
	virtual void		SetVisible(BOOL bVisible);
	virtual BOOL		GetVisible();
	virtual void		UpdateCodeInfo(StockUserInfo* pCodeInfoNew);
	virtual StockUserInfo*	GetCodeInfo();
	virtual short		GetDrawType();
	virtual BOOL		MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		MouseUp(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	virtual BOOL		RButtonUp(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	virtual BOOL		OnCursor(UINT nFlags, CPoint point);
	virtual int			OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual long		OnMenuMsg(WPARAM wParam, LPARAM lParam);
	virtual int			Draw(CDC* pDC);
	virtual BOOL		GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce);
	virtual int			OnUpdateData(DataHead* pData);
	virtual int			Delete();
	virtual	short		GetLineType();
	virtual void		SetTeckKLineName(CString strName);
	virtual CString		GetTeckKLineName();

//属性:
	CHSLinear<float>*	GetLinear() {return m_pLinear;}
	virtual void		GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual BOOL		IsUp(int nPosition);
	virtual void		SetFenshiDrawType(BOOL bType);
	virtual BOOL		IsFenshiDrawType();
	virtual CManagerBase*		GetTechLineMgr();
	/*是否为主图*/
	virtual BOOL		IsMainLine();
	/*获取区域图*/
	virtual CRect		GetMarginRect();
	virtual void		SetHorizCellWidth(float fNewCellWidth);
	virtual float		GetHorizCellWidth();
	virtual void		SetHorizCellNum(int nHorizCellNum);
	virtual int			GetHorizCellNum();
	virtual int			GetMaxHorizCellNum();
	virtual	void		SetAddLine(BOOL bAddLine);
	virtual	BOOL		IsAddLine();
	virtual BOOL		GetVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& ayTitle, int nDataIndex);
	virtual int			GetDataXPos(int nDataIndex);
	virtual int			GetDataYPos(int nDataIndex);
	virtual int			GetInMarginYPos(int nSourceYPos);
	virtual int			HitTest(CPoint pt);
	virtual BOOL		GetTipText(MouseTipTextInfo* pMouseTipTextInfo,int nPosition,CPoint& point);
	virtual void		SetIsBottom(BOOL bIsBottom);
	virtual BOOL		GetIsBottom();
	virtual DWORD		ModifyLinearStyle(DWORD dwAddStyle, DWORD dwRemoveStyle);
	virtual void		SetVarTitleRect(CRect rc, BOOL bAdd = FALSE);
	virtual CRect		GetVarTitleRect();
	void                SetChartType(int nType) { m_sChartParam.m_dwType = nType;}
	CDispMaxMinValue*	GetCurMaxValue();
	CDispMaxMinValue*	GetCurMinValue();
};

