/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDrawInfo.h
*	文件标识：
*	摘	  要：  咨询图元
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-3-17
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "DrawTableBase.h"
#include "QuoteComm\InfoStruct.h"
#include "HSViewInfo.h"

#define INFO_COL_COUNT       10   // 列数
#define INFO_LAST_COL_WIDTH  45   // 最后一列宽度20
#define INFO_ROW_HEIGHT      30   // 行高

class CDrawInfo :
	public CDrawTableBase,
	public IViewInfoNotify
{
public:
	CDrawInfo(IHsColor* pColor, IHsInfo* pInfo);
	~CDrawInfo(void);

	void  CreateSelf();
	void  DelAllInfoIndex();           // 删除所有数据信息
	int   GetArrayIndexFromPoint(CPoint pos);    // 从当前单元的二维坐标获得资讯索引数组的下标
	void  CalcImageRect(CRect* pRcIamge, CRect* pRcItem, BOOL bIsLeft = TRUE); // 从当前区域大小中计算出图标大小 

	void  RequestText(int nAyIndex);   // 获取资讯内容
	// interface
	virtual BOOL    Create(IDrawBaseParam *pParam);
	virtual BOOL	SetRect(CRect rc);
	virtual BOOL    Draw(CDC* pDC);
	virtual BOOL    DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem);	
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
	virtual BOOL	MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual int	    OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	//////////////////////////////////////////////////////////////////////////
	// 实现文本输出图元对象回调函数
	// 刷新操作
	virtual void  ViewInfoInvalidate(CRect* pRect, BOOL bErase = TRUE);
	// 获取句柄操作
	virtual HWND  ViewInfoGetHandle();
private:
	IHsColor*     m_pColor;   // 颜色配置指针
	IHsInfo*      m_pInfo;    // 资讯配置指针

	CodeInfo      m_preCodeInfo;   // 图元之前的股票信息   
	CString       m_strInfoPath;   // 咨询路径
	BOOL          m_bSizeMax;      // F10是否全屏显示

	CArray<InfoIndex*>  m_ayInfoIndex; // 咨询索引对象
	CHSViewInfo*        m_pViewInfo;   // 本文输出框图元对象

	CImageList          m_lstImage;    // 图标资源
	CRect               m_rcMaxScreen; // 全屏目标区域
	CRect               m_rcClose;     // 关闭图标区域
	CRect               m_rcUp;        // 向上图标区域
	CRect               m_rcDown;      // 向下图标区域

	static int           m_nActiveCellID;     // 当前选中的单元格 
};
