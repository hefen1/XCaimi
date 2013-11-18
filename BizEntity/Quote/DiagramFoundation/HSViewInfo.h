/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CHSViewInfo.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��Ѷ��ʾͼԪ
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-03-18
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "HSScrollBar.h"
#include "DiagramFoundationDef.h"

interface IViewInfoNotify 
{
	// ˢ�²���
	virtual void  ViewInfoInvalidate(CRect* pRect, BOOL bErase = TRUE) = 0;
	// ��ȡ�������
	virtual HWND  ViewInfoGetHandle() = 0;
};

#define  VIEW_SCROLL_SIZE    20   // ��������С
class HS_EXT_CLASS CHSViewInfo : IScrollNotify
{
public:
	CHSViewInfo(void);
	~CHSViewInfo(void);

	BOOL   Draw(CDC* pDC);       // ���ƺ���      
	void   SetRect(CRect rc);	 // ���������С
	void   SetText(CString str); // �����ı�����
	void   SetViewNotify(IViewInfoNotify* pNotify);   // ���ûص�����ָ��

	void   GetTextOutString(CString& strText);   // �����ƶ���λ�õõ���ǰ��Ҫ���Ƶ��ı�����
	int    CalculateRow();       // �����ı����ݵ�����
	int    CalculateShowRow();   // ��������߶ȼ��㵱ǰ����ʾ������

    BOOL   MouseDown(UINT nFlags, CPoint point);
	BOOL   OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	int	   OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	//////////////////////////////////////////////////////////////////////////
	// �������ص�����ʵ��
	// ����λ�ò���
	virtual BOOL  SetPosition(int nPosition, BOOL bIsVert, BOOL bIsRedraw = TRUE);
	// ˢ�²���
	virtual void  ScrollInvalidate(CRect* pRect, BOOL bErase = TRUE);
	// ��ȡ�������
	virtual HWND  GetHandle();
private:
	CHSScrollBar*    m_pScrollBar; // ������ָ��
 	CRect            m_rcInfo;     // �����С
	CString          m_strText;    // �ı�����
	int              m_nRowCount;  // �ı�����

	int              m_nMove;      // ��ֱ�ƶ�����
	IViewInfoNotify* m_iNotify;    // �ص�ָ��  
};
