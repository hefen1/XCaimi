
#pragma once
/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawTVolume.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  �ɽ���ͼԪ�ӿ�ʵ��
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
#include "TechKLineImp.h"

class CDrawTechLine : public CTechKLineImp
{
protected:
	CString									m_strName;
	CArray<CLineProperty*,CLineProperty*>	m_arLine;
	int										m_nStockDayDataLen;
	BOOL									m_bInitFormFenshi;		//��ʱ�еļ�����
	CRect									m_rcClose;
protected:
	//����
public:
	CDrawTechLine(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth = 0, int nCellNum = 0);
	virtual ~CDrawTechLine();

//interface:
public:
	virtual BOOL		SetRect(CRect rc);
	virtual BOOL		MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual int			OnUpdateData(DataHead* pData);
	virtual int			Draw(CDC* pDC);
	virtual void		SetHorizCellNum(int nHorizCellNum);
	virtual void		GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual BOOL		IsMainLine();
	virtual BOOL		GetVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& ayTitle, int nDataIndex);
	virtual	short		GetLineType();
	virtual void		SetTeckKLineName(CString strName);
	virtual CString		GetTeckKLineName();
};