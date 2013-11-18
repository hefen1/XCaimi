/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	HsTitle.h
*	文件标识：
*	摘	  要：  标题图元
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-03-08
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "DiagramFoundationDef.h"

#define TITLE_HEIGHT 16             // 标题高度16像素

interface ITitleNotify
{
	// 刷新操作
	virtual void  TitleInvalidate(CRect* pRect, BOOL bErase = TRUE) = 0;
};


class HS_EXT_CLASS CHSTitle
{
public:
	CHSTitle();
	~CHSTitle();

	CRect   SetRect(CRect rc);   // 根据容器的大小(rc)，计算出自己的位置返回容器剩下的区域
	void    SetText(CString str);// 设置标题文本
	
	void    Draw(CDC* pDC);      // 绘制函数
	
	void    SetScrollNotify(ITitleNotify* pNotify) {m_iNotify = pNotify;}
private:
	CString m_str;     // 输出的字符
	CRect   m_rcTitle; // 标题栏区域

	CFont*   m_pFont;  // 标题文本字体
	COLORREF m_clrBK;  // 标题背景色
	COLORREF m_clrFG;  // 标题前景色

	ITitleNotify* m_iNotify;  // 回调指针
};
