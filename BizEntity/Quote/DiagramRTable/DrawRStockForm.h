/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawRStockForm.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  �嵵����ͼԪ
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
#include "FormulaManPublic.h"

class CDrawRStockForm :
	public CDrawTableBase
{
public:
	CDrawRStockForm(IDataSourceEx* pDataSource, BOOL bLevel = FALSE);
	virtual ~CDrawRStockForm();

	BOOL	 CreateSelf();  // �����嵵
	BOOL     CreateBonds(); // ����ծȯ�嵵
	BOOL     CreateStock(); // ������Ʊ�嵵
	BOOL     CreateFund();  // �������� lof�嵵
	BOOL     CreateEFT();   // ����ETF�嵵
	BOOL     CreateIndex(); // ����ָ��
	BOOL     CreateFuture();// �����ڻ�
	/********************************************************************************
	 * �������� : ���ص�Ԫ���ı���ʽ, 
	 * �������� : double newVelue - ��ǰֵ 
	 *			  double preValue - ֮ǰֵ
	 *  ����ֵ  : COLORREF ��ɫ
	 * �������� : 
	 * ��    �� : ����
	 * ������� : 2010-12-7
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	WORD  GetTextColor(double newValue, double preValue);

	/********************************************************************************
	 * �������� : ��ȡ�����µ�����������
	 * �������� : CPoint pos - ��ǰ��Ԫ���±� 
	 *  ����ֵ  : BOOL - 1: ��ʾ�� 0: ��ʾ��
	 * �������� : 
	 * ��    �� : ����
	 * ������� : 2010-12-7
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	BOOL  GetQuickTradeDirection(CPoint pos, double dPrice); 

	void  HandleNowData(DataHead* pHead, CommRealTimeData_Ext* pnowData, int nSize); // ����ʵʱ����
	BOOL  UpdateFundData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);     // ���»��� lof����
	BOOL  UpdateETFData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);      // ����EFT����
	BOOL  UpdateBondsData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);    // ����ծȯ����
	BOOL  UpdateStockData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther);    // ���¹�Ʊ����
	BOOL  UpdateIndexData(HSIndexRealTime* pIndex);    // ���¹�Ʊ����

	//////////////////////////////////////////////////////////////////////////
	// ������leve2������
	BOOL  UpdateFundData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData);  // ���»��� lof����
	BOOL  UpdateETFData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData);   // ����EFT����
	BOOL  UpdateBondsData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData); // ����ծȯ����
	BOOL  UpdateStockData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData); // ���¹�Ʊ����

//interface
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
	virtual BOOL    MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
protected:
	void    InitPriceMap(CMap<long,long,long,long> &lMap, long lPrice[5]);

	WORD    GetSellChange(long lSellPrice, long lNowSellCount, char* strText); // ���������仯
	WORD    GetBuyChange(long lBuyPrice, long lNowBuyCount, char* strText);    // ���������仯
private:
	CMap<long,long,long,long> m_buyMap;  // ��1~5��
	CMap<long,long,long,long> m_sellMap; // ��1~5��
	long   m_nNewPrice; // ��ǰ��Ʊ���¼�

	CodeInfo                   m_preCodeInfo;   // ͼԪ֮ǰ�Ĺ�Ʊ��Ϣ   
	IDataSourceEx*	           m_pDataSource;   // ��������ӿ�ָ��
	BOOL                       m_bLevel;        // �Ƿ�Ϊlevel2
	RealTimeDataLevel          m_sLevelData;    // ����������ͬ��level2���ư�
	HSStockRealTime            m_sRealTime;     // ����������ͬ��level1���ư�
};
