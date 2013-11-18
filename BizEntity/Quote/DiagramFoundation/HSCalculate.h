/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CHSCalculate.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ������
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-03-01
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "..\..\..\Common\hsstructnew.h"
#include "..\..\..\Common\FormulaManPublic.h"
#include "..\..\..\Common\DataSourceDefineEx.h"
#include "..\..\..\Common\QuoteComm\QuoteDefine.h"
#include "..\..\..\Common\QuoteComm\InfoStruct.h"
#include "DiagramFoundationDef.h"

#define STR_LENTH    64

class HS_EXT_CLASS CHSCalculate
{
public:
	CHSCalculate(void);
	~CHSCalculate(void);

	// ���������� lValue�Ƿ��ذ�����������ֵ
	static double GetBuySellCount(const StockUserInfo* pStockInfo, long lValue, char strText[64]); 
	// ����ί��   ί�ȣ�(ί��������ί������)/(ί��������ί������)��100��
	static double GetWeiBi(const HSStockRealTime* pRealTime, char strText[64]);  
	static double GetWeiBi(const LevelRealTime* pLevel, char strText[64]);
	static double GetWeiBi(const HSIndexRealTime* pRealTime, char strText[64]);
	// �����ǵ�
	static double GetZhangdie(BOOL bStop, const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);
    static double GetZhangdie(BOOL bStop, const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);
	static double GetZhangdie(BOOL bStop, const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64]);
	// ������� ����(�Ƿ�=(�ּ�-����)/����)
	static double GetFudu(BOOL bStop, const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);   
	static double GetFudu(BOOL bStop, const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);
	static double GetFudu(BOOL bStop, const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64]);
	// ��������
	static double GetZongshou(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);
	static double GetZongshou(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);
	static double GetZongshou(const HSIndexRealTime* pRealTime, int nHand, short nMask, char strText[64]);
	// ��������
	static double GetXianshou(const StockUserInfo* pStockInfo, const StockOtherData* pOther, char strText[64]);
	static double GetXianshou(const StockUserInfo* pStockInfo, const LevelStockOtherData* pLevelOther, char strText[64]);
	// ������ӯ
	static double GetShiying(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64]);
	static double GetShiying(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64]);
	// ������ͨ
	static double Getliutong(CHSFinanceData* pFinanceData, char strText[64]);
	// ������ֵ
	static double GetShizhi(const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64] );  
	static double GetShizhi(const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64] );
	// ����ί��
	static double GetWeicha(const HSStockRealTime* pRealTime, int nHand, char strText[64]);
	static double GetWeicha(const LevelRealTime* pLevel, int nHand, char strText[64]);
	static double GetWeicha(const HSIndexRealTime* pRealTime, int nHand, char strText[64]);
	// ������� ���� ����E��ʱ�ɽ��������ɽ��ۣ�/�ܳɽ�����
	static double GetJunjia(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);      
	static double GetJunjia(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);      
	// ��������
	static double GetLiangbi(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, const StockOtherData* pOther, IDataSourceEx* pDataSource, char strText[64]);  
	static double GetLiangbi(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, const LevelStockOtherData* pLevelOther, IDataSourceEx* pDataSource, char strText[64]);  
	// ���㻻��
	static double GetHuanshou(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64]); 
	static double GetHuanshou(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64]); 
	// ����ȫ��
	static double GetQuanjia(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);   
	static double GetQuanjia(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);   
	// �����о�
	static double GetShijing(const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64] );  
	static double GetShijing(const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64] );  
	// �����ܹ�
	static double GetZonggu(CHSFinanceData* pFinanceData, char strText[64]);  
	// ������ֵ
	static double GetZongzhi(const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64]);  
	static double GetZongzhi(const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64]);  
	// ��������
	static double GetWaipan(const StockUserInfo* pStockInfo, const StockOtherData* pOther, char strText[64]); 
	static double GetWaipan(const StockUserInfo* pStockInfo, const LevelStockOtherData* pLevelOther, char strText[64]); 
	// ��������
	static double GetNeipan(const StockUserInfo* pStockInfo, const StockOtherData* pOther, char strText[64]);
	static double GetNeipan(const StockUserInfo* pStockInfo, const LevelStockOtherData* pLevelOther, char strText[64]);
	// ����IOPV
	static double GetIOPV(const StockUserInfo* pStockInfo, const HSStockRealTime_Ext* pRealTimeExt, char strText[64]); 
	// ���㾻ֵ
	static double GetJingZhi(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64]);
	static double GetJingZhi(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64]);
	// ���㷴����
	static double GetFanHuiLv(const StockUserInfo* pStockInfo, const HSStockRealTime_Ext* pRealTimeExt, char strText[64]);
	static double GetFanHuiLv(const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64]);
	// ��������
	static double GetZhangSu(const StockUserInfo* pStockInfo, const HSStockRealTimeOther* pRealOther, char strText[64]);
	// �������
	static double GetZhenFu(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);
	static double GetZhenFu(const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64]);
	// ����ÿ��
	static double GetMeiBi(const LevelRealTime* pLevel, char strText[64]);
	// ��������
	static double GetSellAvg(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);
	// �������
	static double GetBuyAvg(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);

	// �����ı������Լ��ı����� ����һ��html�ı� ���ҷ��ر���URL��ַ
	static CString CreateHtml(const CString& strContent, const InfoIndex* pInfoIndex);
	//////////////////////////////////////////////////////////////////////////
	// ����ʱ�� lPreTimeֻ�е�bIsFullTime = TRUEʱ��������
	static CTime  GetTimer(const StockUserInfo* pStockInfo, const StockTick* pTick, StockType* pTypeInfo, BOOL bIsFullTime, long lDate, long lPreTime, char strText[64]);
	// ����һ��ʱ������
	static CTime ConstructTime( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,int nDST = -1);
    //////////////////////////////////////////////////////////////////////////
	// ��������
	static void  GetTextByID(int nID, CurrentFinanceData* pFinanceData, char strText[64]); // ����ID �Ӳ���������ȡ���ݷ����ַ���
	static float GetDataByID(int nID, CurrentFinanceData* pFinanceData); // ����ID �Ӳ���������ȡ����
	static void  AutoAdjustUnit(WORD wKey, double dbData, CString &strUnit, int *pUnit); // �������ݵ�λ 
};