/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	stockManager.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	��Ʊ������
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-9-1
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/

#pragma once
#define _delArrayObj(p)  { for(int i = p.GetSize() - 1; i >= 0; i--) { delete p.GetAt(i); } p.RemoveAll(); }

#include "..\..\..\Common\StockInfo.h"
#include "PyjcManager.h"
class CDataSourceImpl;
class CStockManager
{
protected:
	//���ػ����Ʊ���ݶ���
	CArray<CHSDSStockInfoIntact*,CHSDSStockInfoIntact*> m_ayStocks;
	CMapStringToOb                      m_mapIndex;
	//�����Ʊ���ݻ�����
	CMutex								m_mutexStock;	
	CMapStringToPtr                     m_mapFinanceData;
	CDataSourceImpl*					m_pDataSource;
	CPyjcManager						m_pyjcManager;
	CArray<StockInfoEx *, StockInfoEx *> *  m_psiExtend; 
public:
	//	���캯������������
	CStockManager();
	virtual ~CStockManager();
	//
	void								SetDataSourceImpl(CDataSourceImpl* pImpl);
	CStringArray*						GetPyjcByHZ(char*& pHZ,int& nNext);
public:
	/**********************smal********************************************
	*	�������ܣ�	����ָ���ĵ��ʷ�����Ӧ��ƴ�����
	*	����������	const char* pcWord	������
	*				char* pcVoice		��������Ӧƴ����ƵĻ���
	*				int nSize			��ƴ��������ֽ���Ŀ
	*	�� �� ֵ��	�� 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003-4-29
	*	�޸�˵����	
	*********************************************************************/	
	void WordToPYJC(const char* pcWord,char* pcVoice, int nSize);
	void								MakeShortName(StockUserInfo *pStock);
	void								MakeAllShortName();
	StockInfoEx*						GetStockInfoEx(const CodeInfo *pCode);

	BOOL								CreateIndex();
	//��Ʊ��Ϣ���뵽����
	BOOL								PushStock(CHSDSStockInfoIntact* pStock);
	//��ȡ��Ʊ��Ϣ 
	CHSDSStockInfoIntact*				GetStock(CodeInfo* pInfo);
	CHSDSStockInfoIntact*				GetStock(char* pszCode, unsigned short nMarket, unsigned short nMarketException);

	//ɾ��ĳ����Ʊ��Ϣ
	BOOL								RemoveStock(CHSDSStockInfoIntact* pStock);
	BOOL								RemoveStockInfoEx();
	//��ձ��ع�Ʊ����
	BOOL								RemoveAllStocks();
	//��ձ���ĳ�г���Ʊ����
	BOOL								RemoveStocksByMarket(HSMarketDataType nMarket);
	//��ȡָ���г��Ĺ�Ʊ�б�
	int									GetMarketStockList(HSMarketDataType* pMarket, int nMarketCount,CArray<StockUserInfo>& ayRef);
	//��ȡָ��������Ϣ�Ĺ�Ʊ�б�
	int									GetStocksList(CArray<CodeInfo>* pArInfo, CArray<StockUserInfo>& stockInfo, BOOL bCompareOnlyMainMarket);
	//��ȡ���й�Ʊ�Ĵ��뼰�г���Ϣ
	int									GetStockCodeInfoList(CArray<CodeInfo*>& arCodeInfo);
	//�յ������������ǵ�ͣ����
	int									RecvServerCalcData(AnsSeverCalculate* pData);
	//д�����ļ�
	BOOL								WriteLocalFile();
	BOOL								SetFinanceDataToMap();
	//����������
	int									GetStocksFinanceData(CArray<CodeInfo>* pArInfo, CArray<CurrentFinanceData>& ayFinanceData);
	//����Ȩ����
	int									GetStocksExrighData(CArray<CodeInfo>* pArInfo, CArray<CurrentExRightData>& ayExrightData);
	short								GetStockHand(HSMarketDataType pMarket);
	int									FindStockFromKey(StockUserInfo* pStock, const CString& strKey,int& nUnique, int& nStart,int& nEnd, CString &szMarch,int& nBetter,const int nCurIndex );
	CString								GetStockPyjc( StockUserInfo* pStock );
};