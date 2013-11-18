/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	TechKlineImp.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��������ͼԪ - �ײ���Ԫ��
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-12-1
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "..\..\..\Common\QuoteComm\DrawBase.h"
#define DRAWBOTTOM_HEIGHT	20
class CTechKLineMgr;
class CDrawBottom : public CDrawBase
{
	//��Ա
protected:
	CTechKLineMgr*		m_pTeckKLineMgr;
	int					m_nHorPosition;				//ˮƽ�����ϵ�λ��
	WORD				m_wPeriodType;				//��������
	CString				m_strTips;
	CString				m_strPeriodName;			//��������
	CMenu*				m_pMenu;
	HCURSOR				m_hCursor;
	//����
public:
	CDrawBottom(CTechKLineMgr* pMgr, IRegionNode* pRegion);
	virtual ~CDrawBottom();
	//interface:
public:
	virtual BOOL		Create(IDrawBaseParam *pParam);
	virtual int			Draw(CDC* pDC);
	virtual BOOL		MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);

	void				UpdatePositon(int nIndex, CPoint pt);
	void				UpdatePeriodType(WORD wType);
	WORD				GetPeriodType();
	int					TrackPopUpMenu(int nTrackID, CString strMenuString);
	WORD				FormatPeriod(int nTrackID);
	int					HitTest(CPoint pt);
};