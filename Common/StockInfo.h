/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	StockInfo.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	��������ӿڹ�Ʊ���ݶ���ͷ�ļ�
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-7-19
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
//HSDS--hundsun datasource
#include "hsds_comudata.h"

/*�����������ݶ��д�����ͷ��m_lUpdateTime��ָ�����������Ƿ���Ҫ���µ�����˸�������*/
struct CHSDSDataHead
{
	short					m_nSize;			//���ݸ���
	unsigned long			m_lUpdateTime;		//���ݸ���ʱ��
};

/*ʵʱ��������*/
struct CHSDSStockOtherData : public CHSDSDataHead
{
	StockOtherData m_othData;
};

//ʱʱ����
struct CHSDSStockRealTime : public CHSDSDataHead
{
	HSStockRealTime	m_stockRealTime;
};

//��չ����
struct CHSDSShareRealTimeData : public CHSDSDataHead
{
	ShareRealTimeData_Ext	m_stockRealTime;	
};

//��ʷ���䡢���շ�ʱ����Сͼ�·�ʱ����
struct CHSDSHisTrendData : public CHSDSDataHead
{
//	StockCompHistoryData*		m_pHisData;
};

// �ֱʼ�¼
struct CHSDSStockTick : public CHSDSDataHead
{
//	StockTick*				m_pStockTick;
};

//��ʱ����
struct CHSDSPriceVolItem : public CHSDSDataHead
{
//	PriceVolItem*			m_pPriceVolItem;
};

//����������
struct CHSDSBuySellOrder : public CHSDSDataHead
{
//	BuySellOrderData		m_buysellorderData;
};


//��������
struct CHSDSStockCompDayData : public CHSDSDataHead
{
//	StockCompDayDataEx*		m_pStockCompDayData;
};

/* �������������� */
struct CHSDSBuySellPowerData : public CHSDSDataHead
{
//	BuySellPowerData*		m_pBuySellPowerData;
};

//��ʷ��������
struct CHSDSHisFinanceData : public CHSDSDataHead
{
//	HisFinanceData*			m_pHisfinanceData;
};

//��Сͼ��ֵ��������(��Ʊ)
struct CHSDSCalcData_Share : public CHSDSDataHead
{
	long			m_nTime;			// ʱ�䣬���뿪�̷�����
	unsigned long	m_lTotal;			// ����
	float			m_fAvgPrice;		// �ܽ��
	long			m_lNewPrice;		// ���¼�
	long			m_lTickCount;		// �ɽ�����	
//	CalcData_Share	m_Share;			// ��Ʊ����

};

//��ʳɽ�
struct CHSDSHSLevelTransaction : public CHSDSDataHead
{
	HSLevelTransaction*			m_pLeveltransaction;
};

//��ʷ����ֱ����ݽṹ
struct CHSDSHistroyInfo_Deal : public CHSDSDataHead
{
//	HistroyInfo_Deal*		m_pHistroyInfoDeal;
};

/*��Ʊ��ָ�����ڻ����۹ɵ�*/
struct CHSDSStockInfoIntact
{
	StockUserInfo				m_stockInfo;

	CHSDSStockOtherData*		m_pOthData;			// ʵʱ��������
	CHSDSStockRealTime*			m_pStockRealTime;	//ʱʱ����
	CHSDSShareRealTimeData*		m_pStockOther;		//��չ����
	CHSDSHisTrendData*			m_pHisTrend;		//��ʷ���䡢���շ�ʱ����Сͼ�·�ʱ����
	CHSDSStockTick*				m_pStockTick;		//�ֱʼ�¼
	CHSDSPriceVolItem*			m_pPriceVolItem;	//�ֱʼ�¼
	CHSDSBuySellOrder*			m_pBuySellOrder;	//����������
	CHSDSStockCompDayData*		m_pStockCompDayData;//��������
	CHSDSBuySellPowerData*		m_pBuySellPowerData;//��������������
	CHSDSHisFinanceData*		m_pHisFinanceData;	//��ʷ��������
	CHSDSCalcData_Share*		m_pCalcDataShare;	//��Сͼ��ֵ��������(��Ʊ)
	CHSDSHSLevelTransaction*	m_pLevelTransaction;//��ʳɽ�
	CHSDSHistroyInfo_Deal*		m_pHistroyInfo;		//��ʷ����ֱ����ݽṹ

	
	void*                       m_pData;              //���ػ�������
	long                        m_lSize;              //�������ݳ���
};

