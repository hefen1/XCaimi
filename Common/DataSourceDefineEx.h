/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DataSourceDefineEx.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	��������ӿ���չ����ͷ�ļ�
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-9-7
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/

#pragma once

#include "DataSourceDefine.h"
#include "StockInfo.h"

class IDataSourceEx : public IDataSource
{
public:
	/********************************************************************************
	* �������� :�����������ȡ������Ʊ��Ϣ
	* �������� :CodeInfo* pInfo - ���й�Ʊ���롢�г�����Ϣ
	*			 StockUserInfo& stockInfo; ��Ʊ��Ϣ������
	*  ����ֵ  :TRUE �ɹ�		
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual BOOL	HSDataSourceEx_GetStockUserInfo(IN CodeInfo* pInfo, IN OUT StockUserInfo& stockInfo) = 0;
	/********************************************************************************
	* �������� :�����������ȡ������ʱ��
	* �������� :
	*			 long &lDate, long &lTime; lData����������ǰ���ڣ���ʽΪYYYYMMDD��lTime����������ǰʱ�䣬��ʽΪhhmmss
	*  ����ֵ  :TRUE �ɹ�		
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ : 20110308 �����������治����¼���������Ըýӿڹ��ܱ����lDate����Ϊ��������һ�γ�ʼ�����ڣ���20110308��lTimeΪ0
	*******************************************************************************/
	virtual BOOL	HSDataSourceEx_GetServerDataTime(IN OUT long &lDate, IN OUT long &lTime) = 0;

	/********************************************************************************
	* �������� :�����������ȡĳ���г���Ʊ��Ϣ��
	* �������� :
	*			 unsigned short nMarket; ��Ʊ��Ϣ������
	*  ����ֵ  : �б��С	
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual int	HSDataSourceEx_GetMarketStocksInfo(IN unsigned short nMarket, IN OUT CArray<StockUserInfo>& stockArray) = 0;

	/********************************************************************************
	* �������� :�����������ȡϵͳ�����г���Ϣ
	* �������� : unsigned short* pMarket - �г��������� ���Ϊ�գ���������
	*			  int nMarketCount	�г����������С
	*			CArray<StockType>& stockArray ���ϵͳ�����Ϣ������
	*  ����ֵ  : �б��С	
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual int	HSDataSourceEx_GetSystemMarketInfo(IN unsigned short* pMarket, IN int nMarketCount, IN OUT CArray<StockType>& blockArray) = 0;

	/********************************************************************************
	* �������� :�����������ȡ������Ʊ��Ϣ
	* �������� :char* pszCode - ��Ʊ����
	*			unsigned short nMarket �г����࣬���Ʊ���ڻ���
	*			 StockUserInfo& stockInfo; ��Ʊ��Ϣ������
	*			unsigned short nMarketException ��Ҫ�ų���С��,���ų�ָ����
	*  ����ֵ  :TRUE �ɹ�		
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual BOOL	HSDataSourceEx_GetStockUserInfo(IN char* pszCode, IN unsigned short nMarket, IN OUT StockUserInfo& stockInfo, IN unsigned short nMarketException = 0) = 0;

	/********************************************************************************
	* �������� :�����������ȡ�����Ʊ��Ϣ-�����Ʊ��Ϣ
	* �������� :CArray<CodeInfo> arInfo ��Ʊ������Ϣ������	; �������򷵻�ȫ����Ʊ	
	*			 CArray<StockUserInfo>& stockInfo; ��Ʊ��Ϣ���������
	*			BOOL bCompareOnlyMainMarket; ֻ�Ƚ����г���ʾ
	*  ����ֵ  :�����С		
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual int	HSDataSourceEx_GetStocksUserInfo(IN CArray<CodeInfo>& arInfo, IN OUT CArray<StockUserInfo>& stockInfo, BOOL bCompareOnlyMainMarket = FALSE) = 0;

	/********************************************************************************
	* �������� :�����������ȡ�����Ʊ����-
	* �������� :CArray<CodeInfo> arInfo ��Ʊ������Ϣ������	
	*			 CArray<CurrentFinanceData>& stockInfo; ��Ʊ�����������������
	*			
	*  ����ֵ  :�����С		
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual int	HSDataSourceEx_GetStocksFinanceData(IN CArray<CodeInfo>& arInfo, IN OUT CArray<CurrentFinanceData>& ayFinanceData) = 0;

	/********************************************************************************
	* �������� :�����������ȡ�����Ʊ��Ȩ����
	* �������� :CArray<CodeInfo> arInfo ��Ʊ������Ϣ������	
	*			 CArray<CurrentExRightData*>& ayExrightData; ��Ʊ��Ȩ��������ָ�������
	*			
	*  ����ֵ  :�����С		
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	* ��ע*	   : CurrentExRightData �����ܵ����ݣ���Ҫ����Release() ȥ�ͷ�
	*******************************************************************************/
	virtual int	HSDataSourceEx_GetStocksExrightData(IN CArray<CodeInfo>& arInfo, IN OUT CArray<CurrentExRightData>& ayExrightData) = 0;
	/********************************************************************************
	* ��������:��ʽ�����������棬��ȡ����k������
	* ��������:
	*			:void* pAsk  - AskData�ṹ��ַ
	*			int nMask - δʹ��	
	*			int nExRight - ��Ȩ״̬ 0 ����Ȩ  HS_HxDataSource_Left_ChuQuan ǰ��Ȩ HS_HxDataSource_Back_ChuQuan ���Ȩ
	*			
	*			
	*			
	*  ����ֵ : void* ���ض�ȡ���ļ�����
	* ��������: 	
	* ��   ��:
	* �������:
	* �޸ļ�¼:
	*******************************************************************************/
	virtual void* HSDataSource_GetDayData(IN void* pAsk, IN int nMask = 0,IN int nExRight = 0) = 0;
	/********************************************************************************
	* ��������:д�����ļ�
	* ��������:	codeinfo  - ��Ʊ����
	*			int period - ���� ����
	*			StockCompDayDataEx* pNewData - ������������
	*			int nSize                -  StockCompDayDataEx�ĸ���
	*			
	*			
	*  ����ֵ : �ɹ�����true ʧ��false
	* ��������: 	
	* ��   ��:
	* �������:
	* �޸ļ�¼:
	*******************************************************************************/
	virtual BOOL HSDataSource_WriteDayData(CodeInfo* info,int period,StockCompDayDataEx* pNewData,int nSize ) = 0;
	/********************************************************************************
	* ��������:���̾�����ã������ַ�ƥ�����
	* ��������:
	*			
	*			
	*			
	*			
	*  ����ֵ : ��ǰkeyֵ��λ��
	* ��������: HSDataSource_FindStockFromKey
	* ��   ��:
	* �������:
	* �޸ļ�¼:
	*******************************************************************************/
	virtual int HSDataSource_FindStockFromKey(StockUserInfo* pStock, const CString& strKey,int& nUnique,
									int& nStart,int& nEnd, CString &szMarch,int& nBetter,const int nCurIndex) = 0;
	/********************************************************************************
	* ��������:���̾�����ã���ȡƴ�����
	* ��������:
	*			
	*			
	*			
	*			
	*  ����ֵ : ��ǰkeyֵ��λ��
	* ��������: HSDataSource_GetStockPyjc
	* ��   ��:
	* �������:
	* �޸ļ�¼:
	*******************************************************************************/
	virtual CString HSDataSource_GetStockPyjc(StockUserInfo* pStock) = 0;
};