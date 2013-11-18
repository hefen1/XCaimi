/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDlgInfoView.h
*	文件标识：
*	摘	  要：  显示信息地雷内容对话框
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-3-21
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "Resource.h"
#include "HSHtmlView.h"

// CDlgInfoView 对话框

class CDlgInfoView : public CDialog
{
	DECLARE_DYNAMIC(CDlgInfoView)

public:
	CDlgInfoView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInfoView();

// 对话框数据
	enum { IDD = IDD_SHOWINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	void  SetUrlString(CString str);   // 设置要打开的Url地址

private:
	CHSHtmlView*   m_pInfoView;
	CString        m_strUrl;
};
