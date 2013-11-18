/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawTradeStockForm.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  �����嵵����ͼԪ
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2010-12-6
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
#include "DataSourceDefineEx.h"

class CDrawTradeStockForm :
	public CDrawTableBase
{
public:
	CDrawTradeStockForm(IDataSourceEx* pDataSource);
	~CDrawTradeStockForm(void);

	BOOL	 CreateSelf();  // �����嵵
	BOOL     CreateBonds(); // ����ծȯ�嵵
	BOOL     CreateStock(); // ������Ʊ�嵵(��������Ҳ���øù��캯��)

	void     HandleNowData(DataHead* pHead, CommRealTimeData_Ext* pnowData, int nSize); // ����ʵʱ����
	BOOL     UpdateBondsData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);    // ����ծȯ����
	BOOL     UpdateStockData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);    // ���¹�Ʊ����

	void     InitColInfo();    // ��ʼ��������
	long     FindMaxCount();   // ������������������ �ҵ������
	WORD     GetTextColor(double newValue, double preValue);
//interface
	virtual  BOOL    DrawTableBK(CDC* pDC);
	virtual  BOOL    DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem);
	virtual  CString GetTipText(int nCellID, int nItemID);
	virtual  BOOL	 GetRequestData(IRequestCollector* pReqCollector, StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual  int     OnUpdateData(DataHead* pData);
	virtual  BOOL    MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
private:
	double m_nPreTotal;     // ֮ǰ�������������� ������ͬ�����ư�
	long   m_lMaxCount;     // ����� 
	long   m_ayBuyCount[5]; // ������
	long   m_aySellCount[5];// ������

	CodeInfo                   m_preCodeInfo;   // ͼԪ֮ǰ�Ĺ�Ʊ��Ϣ   
	IDataSourceEx*	           m_pDataSource;   // ��������ӿ�ָ��
};
