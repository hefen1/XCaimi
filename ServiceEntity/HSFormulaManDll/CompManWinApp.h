/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CompManWinApp.h
*	�ļ���ʶ��	��ʽ����ģ��Ӧ���ඨ��
*	ժ	  Ҫ��	MFC�����ܵ���Ҫ,������,��û����ʲô
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
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

	// ��ʼ��Ӧ��
	virtual BOOL InitInstance();

	// ģ���˳�ʱҪ���Ĵ���
	virtual int ExitInstance();
};

#endif // !defined(AFX_COMPMANWINAPP_H__8FCDECAB_9281_45EB_A335_549C30FE1579__INCLUDED_)
