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
*	������ڣ�  2010-4-28
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#include "..\DiagramFoundation\DrawTableBase.h"

class CDrawHisTrendRTable : public CDrawTableBase
{
protected:
	int					m_nRowCount;
	int					m_nColCount;
public:
	CDrawHisTrendRTable();
	virtual ~CDrawHisTrendRTable();

//interface
	virtual BOOL DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem);	

//����
protected:
	BOOL		CreateTable();
public:
	BOOL		UpdateData(StockUserInfo* pStock, StockDay* pStockDayData, long lPreClose, long lFinaceTotalHand);
};
