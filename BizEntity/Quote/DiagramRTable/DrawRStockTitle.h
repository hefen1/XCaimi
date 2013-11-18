/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDrawRTableBase.h
*	文件标识：
*	摘	  要：  绘制标题
*	
*	当前版本：
*	作	  者：	王超
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
#include "DrawTableBase.h"
#include "ConfigInterface.h"


class CDrawRStockTitle : public CDrawTableBase
{
public:
	CDrawRStockTitle(IHsUserDataCfg* pUserDefCfg, IHsColor* pColor, IHsFont* pFont);
	virtual ~CDrawRStockTitle();
	
	void ReadFile(); // 读配置文件

	virtual int	 DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect &rcItem);
	virtual CString GetTipText(int nCellID, int nItemID);
	virtual	BOOL GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);

private:
	COLORREF m_clrBK;        // 背景色
	COLORREF m_clrCode;      // 股票代码颜色
	COLORREF m_clrCodeName;  // 股票名称颜色
	CFont* m_pFontCode;      // 代码字体
	CFont* m_pFontCodeName;  // 名称字体

	IHsUserDataCfg*   m_iUserDefCfg;    // 用户数据接口（股票标记）
	IHsColor*         m_pColor;         // 颜色配置
	IHsFont*          m_pFont;          // 字体配置
	CImageList*       m_pImageList;     // 股票标记图标
};

