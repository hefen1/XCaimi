/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	RichEditCtrlEx.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	��ɫ��EDIT�ؼ���ʾ��
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
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
	*	�������ܣ�	�õ��ؼ��е�ǰѡ���ı��ĸ�ʽ����
	*	����������	DWORD dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE : [in] �õ�����ʱ��MASK
	*	�� �� ֵ��	CHARFORMAT : ��ʽ����
	*	����������	GetSelectionCharFormat()
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	CHARFORMAT GetCharFormat(DWORD dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE);

public:

	/******************************************************************
	*	�������ܣ�	�ڵ�ǰ���ϼ�һ���ִ�str,��clTextColorɫ����ʾ
	*	����������	CString str			 : [in] Ҫ��ʾ���ִ�
	*				COLORREF clTextColor : [in] ��ʾ����ɫRGBֵ
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	void AddItem(CString str, COLORREF clTextColor);

	/******************************************************************
	*	�������ܣ�	�ڵ�ǰ���ϼ�һ���ִ�str,��clTextColorɫ����ʾ,��������
	*	����������	CString str			 : [in] Ҫ��ʾ���ִ�
	*				COLORREF clTextColor : [in] ��ʾ����ɫRGBֵ
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	void AddItemLN(CString str, COLORREF clTextColor);

	/******************************************************************
	*	�������ܣ�	�ڵ�ǰ���ϼ�һ��TAB
	*	����������	���ޣ�
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	void AddTab() { AddItem("\t", HS_Rich_Text_Color_White); }

	/******************************************************************
	*	�������ܣ�	��һ����
	*	����������	���ޣ�
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	void AddSpaceLine() { AddItem("\r\n\r\n", HS_Rich_Text_Color_White); }

	/******************************************************************
	*	�������ܣ�	�µ�һ�п�ʼ
	*	����������	���ޣ�
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	void AddNewLine() { AddItem("\r\n", HS_Rich_Text_Color_White); }

	/******************************************************************
	*	�������ܣ�	�������
	*	����������	���ޣ�
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	void ClearScreen();
	// �����ؼ�
	virtual BOOL Create(CWnd* pParentWnd, UINT nID);
};


