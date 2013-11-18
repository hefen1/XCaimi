/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawTableBase.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  �ּ۱�
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-03-11
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "DrawTableBase.h"

#define FENPRICE_ROW_HEIGHT      22  // �и�
#define FENPRICE_COL_COUNT        4  // ����
#define FENPRICE_COL_WIDTH_NUM1  45  // ��һ���п�
#define FENPRICE_COL_WIDTH_NUM2  50  // �ڶ����п�
#define FENPRICE_COL_WIDTH_NUM3  55  // �������п�
#define FENPRICE_COL_WIDTH_NUM4   0  // �������п� �������Ƿǹ̶��� ��С�ڻ���������Զ������

struct CFenPriceData
{
	long m_lPrice;      //  �۸�
	long m_lBuyVolume;	//	����
	long m_lSellVolume;	//	����
	long m_lTurnover;   //  �ɽ�����
};


class CDrawFenPrice :
	public CDrawTableBase
{
public:
	CDrawFenPrice(IDataSourceEx* pDataSource, IHsColor* pColor, IHsFont* pFont);
	~CDrawFenPrice(void);

	void CreatSelf();  // �����̶���
	void InitSize(int nRowCount);   // ������������
	void InsertArray(StockTick* pStockTick); // ��� ���Ұ��۸�Ӵ�С���в��뵽������
	void RemoveAllData(); // ��������ɾ����������
	void UpdateCell();    // ͨ���õ������� ���µ�Ԫ��
	long FindMaxVolume(); // ���������ҵ�ĳһ���۸����ĳɽ���
//interface
	virtual int     Draw( CDC* pDC );
	virtual BOOL    DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem);	
	virtual CString GetTipText(int nCellID, int nItemID);
	virtual	BOOL    GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int     OnUpdateData( DataHead* pData );
private:
	IDataSourceEx*	          m_pDataSource;   // ��������ӿ�ָ��
	IHsColor*                 m_pColor;        // ��ɫ�ӿ�ָ�� 
	IHsFont*                  m_pFont;         // ����ӿ�ָ��

	StockType                 m_StockType;     // ��ǰ��Ʊ�Ķ�ӦһЩ��Ϣ
	CArray<CFenPriceData*, CFenPriceData*&>    m_ayFenPriceData;// �ּ���������
	
	COLORREF m_clrTitleBK;    // ���ⱳ��ɫ

	long    m_lTotalVol;      // �ܳɽ���
	long    m_lPreNewPrice;   // ǰһ�����ĳɽ���
	long    m_lPreSellPrice;  // ǰһ����
	long    m_lPreBuyPrice;   // ǰһ���
	long    m_lStockTickCount;// ��¼STOCKTICK����size
};
