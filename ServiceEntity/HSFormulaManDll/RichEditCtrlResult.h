/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	RichEditCtrlResult.h
*	文件标识：	
*	摘	  要：	条件设置结果显示控件类(Edit)
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/

#pragma once
#include "RichEditCtrlEx.h"
#include "ExchData.h"
/*#include "XInfoTip.h"*/

class CRichEditCtrlResult :
	public CRichEditCtrlEx
{
public:
	CRichEditCtrlResult(void);
	~CRichEditCtrlResult(void);
	DECLARE_MESSAGE_MAP()

public:
	/******************************************************************
	*	函数功能：	显示条件设置
	*	函数参数：	void* pData		: [in] CExchCalcMain测试数据
	*				BOOL bSignal	: [in] 显示单股票设置还是全市场设置
	*				UINT uID = 0	: [in] 当前参数ID
	*	返 回 值：	LRESULT : 成功(1), 失败(-1)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	LRESULT Display(void* pData, BOOL bSignal, UINT uID = 0, TExchOptimize* pOptimize = NULL);

	/******************************************************************
	*	函数功能：	得到买入条件设置字串
	*	函数参数：	void* pExchSys : [in] CExchCalcMain测试数据
	*	返 回 值：	CString : 得到的设置字串	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	CString GetBuyCon(void* pExchSys);

	/******************************************************************
	*	函数功能：	得到卖出条件设置字串
	*	函数参数：	void* pExchSys : [in] CExchCalcMain测试数据
	*	返 回 值：	CString : 得到的设置字串
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	CString GetSellCom(void* pExchSys);

	/******************************************************************
	*	函数功能：	得到平仓条件设置字串数组
	*	函数参数：	void* pExchSys		: [in] CExchCalcMain测试数据
	*				CStringArray& ayStr : [out] 平仓设置字串
	*	返 回 值：	int : 平仓设置条件个数
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	int GetPCCon(void* pExchSys, CStringArray& ayStr);

public:
	/******************************************************************
	*	函数功能：	显示测试结果
	*	函数参数：	void* pData		: [in] CExchCalcMain测试数据
	*				int nIdx		: [in] 要显示的股票索引(-1时显示总的统计结果)
	*				UINT uID		: [in] 要显示结果的参数ID
	*				BOOL bIsSingal	: [in] "摘要"还是"详细"
	*	返 回 值：	LRESULT : 成功(1), 失败(-1)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	LRESULT Display(void* pData, int nIdx, UINT uID, BOOL bIsSingal);

	/******************************************************************
	*	函数功能：	显示"摘要"测试结果
	*	函数参数：	void* pData		: [in] 一个股票的统计结果数据
	*				void* pExchData : [in] CExchCalcMain测试数据
	*	返 回 值：	LRESULT : 成功(1), 失败(-1)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	LRESULT DisplaySingal(void* pData, void* pExchData);

	/******************************************************************
	*	函数功能：	显示"详细"测试结果
	*	函数参数：	void* pData		: [in] 一个股票的统计结果数据
	*				void* pExchData	: [in] CExchCalcMain测试数据
	*	返 回 值：	LRESULT : 成功(1), 失败(-1)
	*	调用依赖：	 
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	LRESULT DisplayMulti(void* pData, void* pExchData);

	/******************************************************************
	*	函数功能：	在一行中加两个项目(四个列,每两个列一组,分别为标题及内容)
	*	函数参数：	CString str1 : [in] 第一项的项名
	*				CString str2 : [in] 第一项的内容
	*				CString str3 : [in] 第二项的项名
	*				CString str4 : [in] 第二项的内容
	*				COLORREF cl1 : [in] 项名显示色彩
	*				COLORREF cl2 : [in] 项内容的显示色彩
	*				BOOL bLN = TRUE : [in] 是否换行
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	void AddLineDoubleItem(CString str1, CString str2, CString str3, CString str4, COLORREF cl1, COLORREF cl2, BOOL bLN = TRUE);

	/******************************************************************
	*	函数功能：	在一行中加一个项目(分别为项名及内容),加入后换行
	*	函数参数：	CString str1 : [in] 项名
	*				CString str2 : [in] 内容
	*				COLORREF cl1 : [in] 项名显示色彩
	*				COLORREF cl2 : [in] 项内容的显示色彩
	*				BOOL bLN = TRUE : [in] 是否换行
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	void AddLineSingleItem(CString str1, CString str2, COLORREF cl1, COLORREF cl2, BOOL bLN = TRUE);

public:
	/******************************************************************
	*	函数功能：	显示友好提示
	*	函数参数：	LPCSTR lpMsg = NULL : [in] 提示信息
	*	返 回 值：	LRESULT : 成功(1), 失败(-1)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	LRESULT Display(LPCSTR lpMsg = NULL);

	// 提示信息窗口
/*	CXInfoTip		m_pDatatip;*/
	// 初始化TIP
	void InitialTip();
	CString	GetTipText(CString& strTitle,UINT nFlags, CPoint point,
		int* nBegin = NULL,int* nEnd = NULL);
	CString	GetKeyHelp(CString strKey);
	CString	FindWord(CString strText,int& nStart,int nCount);
	CString	CutWord(CString strText,BOOL bLeft);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
