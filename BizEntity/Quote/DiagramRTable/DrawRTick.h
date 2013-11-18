/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawRTick.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ���Ʒֱ�(LeveL1)
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2010-12-31
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

#define RTICK_ROW         9
#define RTICK_ZHUBI_ROW   20
#define RTICK_ROW_HEIGHT  18

class CDrawRTick 
	: public CDrawTableBase
{
public:
	CDrawRTick(IDataSourceEx* pDataSource);
	virtual ~CDrawRTick(void);

	BOOL	CreateSelf(); 
	void    InitFlag();      // ��ʼ�����ֵ
// interface
	virtual BOOL    Create(IDrawBaseParam *pParam);
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
	virtual BOOL	MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual void    OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam);
protected:
	void    EmptyCellHighLight();  // ������и�������
	void    InitData(StockTick* pTick);           // ��ʼ������
private:
	// ����ʵʱ
	void    HandleStockTrace(StockTick* pData);
	// ��������
	void    HandleNowData(DataHead* pHead, CommRealTimeData_Ext* pnowData, int nSize);  // LEVEL1
	void    HandleNowData(DataHead *pHead, HSLevelTransaction *pData, int nSize);       // LEVEL2
	// һЩ���ܺ���
	unsigned int GetLineNum(); // ��ȡ��ʾ���к�
	WORD    GetTextColorID(double newValue, double preValue); // �Ƚ�ǰ������ֵ����ȡ��ɫID
// ����
private:
	BOOL m_bDispFour;           // �Ƿ���ʾ������ �ֽ׶��Ϻ�����ʾ ������ʾ
	BOOL m_bFullTime;           // ���ÿ���: �Ƿ���ʾ����ʱ��

	long m_lStockTickCount;     // tick��������
	unsigned long m_lTotalVol;	// ����
	long	m_lPreNewPrice;     // ǰһ�ɽ���

	unsigned long m_nTotalHand;	// �ֲܳ���
	long  m_lBuyPrice;		    // ���
	long  m_lSellPrice;		    // ����
	long  m_lPreTime;		    // ǰһʱ��
	long  m_lPreSecond;         // ǰһʱ�����

	unsigned short   m_nDataLineNum;  // ��ǰ�����ݵ�����
	unsigned short*  m_ayRowFlag;     // ��¼ÿһ�еı��ֵ
	unsigned short   m_nFlag;         // ���ֵ

	BOOL m_bLevel;              // �Ƿ�ΪLevel2
	BOOL m_bZhuBi;              // �Ƿ�Ϊ���
	
	HSLevelTransaction   m_sLevelDataAuto; // level2���еĵ�һ�� ����������ͬ�����ư�      

	StockType       m_StockType;     // ��ȡ��ǰ��Ʊ��һЩ��Ϣ ���ڼ���ʱ��
	IDataSourceEx*	m_pDataSource;   // ��������ӿ�ָ��
};
