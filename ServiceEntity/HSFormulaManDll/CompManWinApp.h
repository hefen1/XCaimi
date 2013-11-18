/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	CompManWinApp.h
*	文件标识：	公式管理模块应用类定义
*	摘	  要：	MFC程序框架的需要,功能上,并没有做什么
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/
// CompManWinApp.h: interface for the CCompManWinApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPMANWINAPP_H__8FCDECAB_9281_45EB_A335_549C30FE1579__INCLUDED_)
#define AFX_COMPMANWINAPP_H__8FCDECAB_9281_45EB_A335_549C30FE1579__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCompManWinApp : public CWinApp  
{
public:
	CCompManWinApp();
	virtual ~CCompManWinApp();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// 初始化应用
	virtual BOOL InitInstance();

	// 模块退出时要做的处理
	virtual int ExitInstance();
};

#endif // !defined(AFX_COMPMANWINAPP_H__8FCDECAB_9281_45EB_A335_549C30FE1579__INCLUDED_)
