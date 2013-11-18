#pragma once
/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawUnitPositonCtrl.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  λ�ÿ��Ƶ�Ԫ ��λ�ÿ�����
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-5-12
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#include "TechKlineImp.h"
#include "..\..\..\Common\ConfigInterface.h"
class CTechKLineMgr;

#define CALC_POSITION_SPLIT		3
class CDrawUnitPositonCtrl
{
protected:
	CRect						m_rectSelf;
	std::list<CTechKLineImp*>	m_listLinesAbove;			//λ�ڿ��Ƶ�Ԫ֮�ϵ�ͼԪ�б�
	std::list<CTechKLineImp*>	m_listLinesBelow;				//λ�ڿ��Ƶ�Ԫ֮�µ�ͼԪ�б�
	CTechKLineMgr*				m_pMgr;
	int							m_nTotalHeight;
	float						m_fPosRatio;
public:
	CDrawUnitPositonCtrl(CTechKLineMgr* pMgr, int nTotalHeight);
	~CDrawUnitPositonCtrl();
	BOOL								MouseDown(UINT nFlags, CPoint point);
	void								SetRect(CRect rc);
	CRect								GetRect();
	void								SetRatio(float fRatio);
	float								GetRatio();
	void								AddLinks(CTechKLineImp* pImp, BOOL bIsAbove);
	BOOL								UpdatePostion(int nNewPosY);
	int									GetResetPostion(CTechKLineImp* pImp, BOOL bTop, int nTotalHeightNew);
	int									GetLinksName(int nType, CStringArray& ayLinesName, BOOL bTop);	
	/*��ȡָ�������Ŀ��Ƶ�Ԫ�п���ͼԪ�ĸ���*/
	int									IsInMyCtrl(CTechKLineImp* pImp, BOOL bTop);
	/*λ�ƹ���ͼԪλ��(Y����)*/
	int									ResetLinkedImpPos(CRect rcDel, BOOL bFirst);
};

class CDrawUnitPosCtrlMgr
{
protected:
	CTechKLineMgr*						m_pMgr;
	std::list<CDrawUnitPositonCtrl*>	m_listUnitLink;
	short								m_nType;
	BOOL								m_bLastSetCursor;	//��һ�����ù����
	HCURSOR								m_hCursor;
	HCURSOR								m_hCursorOld;
	int									m_nTotalHeight;
	int									m_nTotalWidth;
	IHSKLinePosition*					m_pConfig;
	IRegionNode*						m_pRegionNode;
public:
	CDrawUnitPosCtrlMgr(CTechKLineMgr* pMgr, IRegionNode* pNode);
	~CDrawUnitPosCtrlMgr();
	BOOL								Init();
	BOOL								Save(int nType);
	void								EmptyOldCtrl();
	BOOL								MouseMove(UINT nFlags, CPoint point);
	BOOL								MouseDown(UINT nFlags, CPoint point);
	BOOL								Update(int nTotalHeight, int nTotalWidth, BOOL bUseOldRatio = FALSE);
	void								AddLinks(CTechKLineImp* pImp, int nSetHeight);
	int									OnResetRect(CRect rcNew);
	BOOL								ResetLinksRect(CTechKLineImp* pImp, CRect rcNew, float fCellWidth);		

//method:
	int									CreateLines(int nType, std::list<CTechKLineImp*>* pListTarget, float fCellWidth);
	//��Ӧɾ��ĳ��ͼԪ�Ĳ���
	int									OnDeleteImp(CTechKLineImp* pImp);
protected:
	CTechKLineImp*						CreateFormFile(int nType, CString strName, float fCellWidth, BOOL bAddLine = FALSE, BOOL bBottom = FALSE);
	/*�������ļ�������ʱ��Ĭ�ϴ�����*/
	int									CreateDefault(int nType, std::list<CTechKLineImp*>* pListTarget, float fCellWidth);
	/*�ҵ�ָ����ͼԪ������λ�ÿ��Ƶ�Ԫ(�·���)*/
	CDrawUnitPositonCtrl*				FindBelowLinkedCtrl(CTechKLineImp* pImp);
};