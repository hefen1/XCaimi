#pragma once 
/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawBox.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��Ϣ��ʾ��
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-3-28
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#include "..\DiagramFoundation\DrawTableBase.h"
#include "GraphicsData.h"
#include "HSDrawBaseStruct.h"

class CTechKLineMgr;
class CDrawBox : public CDrawTableBase
{
protected:
	HsLOGPEN			m_penBorder;
	CString				m_strDate;
	CString				m_strTime;
	CTechKLineMgr*		m_pTeckKLineMgr;
	int					m_nRowCount;
	short				m_nType;

public:
	CDrawBox(CTechKLineMgr* pMgr, IRegionNode* pRegion);
	virtual ~CDrawBox();

//interface:
public:
	virtual BOOL		ReCreate(short nType);
	virtual int			Draw(CDC* pDC);
	virtual BOOL		SetRect(CRect rc);
	virtual BOOL		MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);

//method
	void				SetDateTime(CString strDate, CString strTime);
	void				UpdateData(StockUserInfo* pStock, DrawFenShiData* pFenshiData, long* plAvePrice, long* plHand);
	void				UpdateData(StockDay* pStockDayData, StockUserInfo* pStock, long lPreClose, long lFinaceTotalHand, CString strValue);
	void				SetFenshiFixedData();
	void				SetKLineFixedData();

	BOOL				GetCellItemAttribute(long lDataTarget, long lDataSource, WORD& wClr);
};
