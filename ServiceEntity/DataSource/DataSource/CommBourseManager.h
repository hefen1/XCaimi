/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CommBourseManager.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	�г�������
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
class CDataSourceImpl;
class CHsBouseManager 
{
protected:
	//���ػ����г����ݶ���
	std::list<BourseInfo*>				m_listBourseInfo;
	//�����г����ݻ�����
	CMutex								m_mutexBourse;	
	CDataSourceImpl*					m_pDataSource;
public:
	//	���캯������������
	CHsBouseManager();
	~CHsBouseManager();
protected:
	BOOL								LookupMainMarket(unsigned short* pMarket, int nMarketCount, unsigned short nMarketSource);
	BOOL								LookupSameMarket(unsigned short* pMarket, int nMarketCount, unsigned short nMarketSource);
	short								RetainGetPriceUnit(unsigned short nMarketCode);
	short								RetainGetPriceDecimal(unsigned short nMarketCode);
public:
	//
	void								SetDataSourceImpl(CDataSourceImpl* pImpl);
	//��ȡ�г�����
	int									GetBouserSize();
	//�г���Ϣ���뵽����
	BOOL								PushBourse(BourseInfo* pBourse);
	//��ȡ�г���Ϣ 
	BourseInfo*							GetBourse(unsigned short nMarket);
	//ɾ��ĳ���г���Ϣ
	BOOL								RemoveBourse(BourseInfo* pBourse);
	//ɾ��ĳ���г�
	BOOL								RemoveBourse(HSMarketDataType nMarket );
	//��ձ����г�����
	BOOL								RemoveAllBourse();
	
	//��ȡ�����г���Ϣ
	int									GetSystemMarketInfo(unsigned short* pMarket, int nMarketCount, CArray<StockType>& blockArray);
	//��ȡ�����г�
	StockType*							GetStockType(unsigned short nMarketCode);
	//��ȡ�г�����飩�۸�λ	
	short								GetMarketBlockPriceUnit(unsigned short nMarketCode);
	//��ȡ�г�����飩�۸񾫶�
	short								GetStockPriceDecimal(unsigned short nMarketCode);
	//ͬʱ��ȡ�۸�λ������
	BOOL								GetStockPriceUnitDecimal(unsigned short nMarketCode, short& nPriceUnit, short& nDecimal);
	//д�����ļ�
	BOOL								WriteLocalFile();
};