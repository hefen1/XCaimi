/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	HsTitle.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ����ͼԪ
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-03-08
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "DiagramFoundationDef.h"

#define TITLE_HEIGHT 16             // ����߶�16����

interface ITitleNotify
{
	// ˢ�²���
	virtual void  TitleInvalidate(CRect* pRect, BOOL bErase = TRUE) = 0;
};


class HS_EXT_CLASS CHSTitle
{
public:
	CHSTitle();
	~CHSTitle();

	CRect   SetRect(CRect rc);   // ���������Ĵ�С(rc)��������Լ���λ�÷�������ʣ�µ�����
	void    SetText(CString str);// ���ñ����ı�
	
	void    Draw(CDC* pDC);      // ���ƺ���
	
	void    SetScrollNotify(ITitleNotify* pNotify) {m_iNotify = pNotify;}
private:
	CString m_str;     // ������ַ�
	CRect   m_rcTitle; // ����������

	CFont*   m_pFont;  // �����ı�����
	COLORREF m_clrBK;  // ���ⱳ��ɫ
	COLORREF m_clrFG;  // ����ǰ��ɫ

	ITitleNotify* m_iNotify;  // �ص�ָ��
};
