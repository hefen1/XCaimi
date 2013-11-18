/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	RichEditCtrlEx.h
*	文件标识：	
*	摘	  要：	多色彩EDIT控件显示类
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#pragma once

#define HS_Rich_Text_Color_Black	RGB(0,0,0)
#define HS_Rich_Text_Color_White	RGB(255,255,255)
#define HS_Rich_Text_Color_Blue	RGB(0,0,255)
#define HS_Rich_Text_Color_Red		RGB(255,0,0)

// CRichEditCtrlEx

class CRichEditCtrlEx : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CRichEditCtrlEx)

public:
	CRichEditCtrlEx();
	virtual ~CRichEditCtrlEx();

protected:
	DECLARE_MESSAGE_MAP()

protected:

	/******************************************************************
	*	函数功能：	得到控件中当前选中文本的格式数据
	*	函数参数：	DWORD dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE : [in] 得到数据时的MASK
	*	返 回 值：	CHARFORMAT : 格式数据
	*	调用依赖：	GetSelectionCharFormat()
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	CHARFORMAT GetCharFormat(DWORD dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE);

public:

	/******************************************************************
	*	函数功能：	在当前行上加一个字串str,以clTextColor色彩显示
	*	函数参数：	CString str			 : [in] 要显示的字串
	*				COLORREF clTextColor : [in] 显示的颜色RGB值
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	void AddItem(CString str, COLORREF clTextColor);

	/******************************************************************
	*	函数功能：	在当前行上加一个字串str,以clTextColor色彩显示,并换新行
	*	函数参数：	CString str			 : [in] 要显示的字串
	*				COLORREF clTextColor : [in] 显示的颜色RGB值
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	void AddItemLN(CString str, COLORREF clTextColor);

	/******************************************************************
	*	函数功能：	在当前行上加一个TAB
	*	函数参数：	（无）
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	void AddTab() { AddItem("\t", HS_Rich_Text_Color_White); }

	/******************************************************************
	*	函数功能：	加一空行
	*	函数参数：	（无）
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	void AddSpaceLine() { AddItem("\r\n\r\n", HS_Rich_Text_Color_White); }

	/******************************************************************
	*	函数功能：	新的一行开始
	*	函数参数：	（无）
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	void AddNewLine() { AddItem("\r\n", HS_Rich_Text_Color_White); }

	/******************************************************************
	*	函数功能：	清空所有
	*	函数参数：	（无）
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	void ClearScreen();
	// 创建控件
	virtual BOOL Create(CWnd* pParentWnd, UINT nID);
};


