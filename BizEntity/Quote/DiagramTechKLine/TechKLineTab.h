#pragma once

/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	TechKLineTab.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  tab
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-4-28
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/

#include "..\DiagramFoundation\DiagramTab.h"
#include "..\..\..\Common\ConfigInterface.h"
#include "..\..\..\Common\FormulaManPublic.h"

class CTechKLineMgr;

enum eTabItemIndex
{
	eTabItemIndex_RootTech = 0,
	eTabItemIndex_Roottemplet = 1,

	eTabItemIndex_ratio = 6,

	eTabItemIndex_branchStart = 3,
	eTabItemIndex_branchEnd = 99,
};
#define TAB_ITEM_HEIGHT		14
#define TAB_ITEM_UNITWIDTH	12

class CTechKLineTab : public CDiagramTab
{
protected:
	int					m_nTabKey;
	CTechIndexArray		m_ayTechIndex;
	CTechTempleteArray	m_ayTemplete;
	CTechIndexArray		m_ayTechMore;
	CTechKLineMgr*		m_pMgr;
public:
	CTechKLineTab();
	~CTechKLineTab();
	virtual void		ReCalTabItemSize();
	virtual void		Draw(CDC *pDC);

	void				SetTechKLineMgr(CTechKLineMgr* pMgr);
	void				ReSetItemHideShow(int nRootIndex);
	int					GetTabKey();
	TechIndex*			GetTabTechIndex(CString strCaption);
	TechTemplete*		GetTabTechTemplete(CString strCaption);
	void				OpenSpecifyTemplete(ExpPropery* pExp);
};

class CTechKLineTabNotify : public IDiagramTabNotify
{
protected:
	CTechKLineMgr*		m_pMgr;
	CTechKLineTab*		m_pTab;
public:
	CTechKLineTabNotify();
	void			SetNotifyMgr(CTechKLineMgr*	pMgr, CTechKLineTab* pTab);
	virtual void	OnActiveTabChange(CDiagramTabItem* pActiveItem,CDiagramTabItem* pOldActiveItem);
};