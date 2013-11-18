/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	TechKlineImp.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��������ͼԪ�ӿڣ����ӣ� --- ʵ��
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
#pragma once
#include "TechKLineImp.h"
#include "ManagerBase.h"

class CTechKLineImpAdd	: public CTechKLineImp
{
protected:
	//K������
	CTechKLineData					m_dataKLine;
	//���ݳ���
	long							m_lDataVauleLen;
	ColorProperty*					m_pColor;				//��ɫָ��
public:
	CTechKLineImpAdd(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth);
	virtual ~CTechKLineImpAdd();

//interface
public:
	virtual int			OnUpdateData(DataHead* pData);
	virtual int			Draw(CDC* pDC);
	virtual void		GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual	short		GetLineType();
	virtual BOOL		GetVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& ayTitle, int nDataIndex);
	virtual BOOL		IsMainLine();

//method:
	BOOL				GetRequestData(IRequestCollector* pReqCollector, StockUserInfo* pStockInfo, int nPeriod, WORD wPeriodType);
	int					SetColor(ColorProperty* pColor);
};