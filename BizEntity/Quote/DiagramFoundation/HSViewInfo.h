/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CHSViewInfo.h
*	文件标识：
*	摘	  要：  资讯显示图元
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-03-18
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "HSScrollBar.h"
#include "DiagramFoundationDef.h"

interface IViewInfoNotify 
{
	// 刷新操作
	virtual void  ViewInfoInvalidate(CRect* pRect, BOOL bErase = TRUE) = 0;
	// 获取句柄操作
	virtual HWND  ViewInfoGetHandle() = 0;
};

#define  VIEW_SCROLL_SIZE    20   // 滚动条大小
class HS_EXT_CLASS CHSViewInfo : IScrollNotify
{
public:
	CHSViewInfo(void);
	~CHSViewInfo(void);

	BOOL   Draw(CDC* pDC);       // 绘制函数      
	void   SetRect(CRect rc);	 // 输入区域大小
	void   SetText(CString str); // 输入文本内容
	void   SetViewNotify(IViewInfoNotify* pNotify);   // 设置回调函数指针

	void   GetTextOutString(CString& strText);   // 根据移动的位置得到当前需要绘制的文本内容
	int    CalculateRow();       // 根据文本内容的行数
	int    CalculateShowRow();   // 根据字体高度计算当前可显示的行数

    BOOL   MouseDown(UINT nFlags, CPoint point);
	BOOL   OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	int	   OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	//////////////////////////////////////////////////////////////////////////
	// 滚动条回调函数实现
	// 设置位置操作
	virtual BOOL  SetPosition(int nPosition, BOOL bIsVert, BOOL bIsRedraw = TRUE);
	// 刷新操作
	virtual void  ScrollInvalidate(CRect* pRect, BOOL bErase = TRUE);
	// 获取句柄操作
	virtual HWND  GetHandle();
private:
	CHSScrollBar*    m_pScrollBar; // 滚动条指针
 	CRect            m_rcInfo;     // 区域大小
	CString          m_strText;    // 文本内容
	int              m_nRowCount;  // 文本行数

	int              m_nMove;      // 垂直移动距离
	IViewInfoNotify* m_iNotify;    // 回调指针  
};
