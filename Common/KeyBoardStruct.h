/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	keyboardstruct.h
*	�ļ���ʶ��	���̾��鹫��ͷ�ļ�
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/

#ifndef KEYBOARDH
#define KEYBOARDH

#include "QuoteStuctDefine.h"

//ҳ������

#define PageInfo_Quote      0x001     //����
#define PageInfo_Tech       0x002	  //k�� ��ʱ
#define PageInfo_Other      0x003     //����

//�ȼ�id
#define KeyBegin			99
#define KeyMainMenu			(KeyBegin+0)               //���˵�
#define KeyInfoData			(KeyBegin+1)            //��Ϣ����
#define KeyQuickBuy			(KeyBegin+2)            //������
#define KeyQuickSell		(KeyBegin+3)           //��������
#define KeyQuoteAdd			(KeyBegin+4)           //���Ӵ���ָ��
#define KeyShangAQuote		(KeyBegin+14)			//��A����
#define KeyShangBQuote		(KeyBegin+15)			//��B����
#define KeyShengAQuote		(KeyBegin+16)		    //��A����
#define KeyShengBQuote		(KeyBegin+17)			//��B����
#define KeyShangBondQuote   (KeyBegin+18)           //��ծ����
#define KeyShengBondQuote   (KeyBegin+19)           //��ծ����
#define KeyShangFundQuote	(KeyBegin+20)			//�Ͻ𱨼�
#define KeyShengFundQuote	(KeyBegin+21)			//��𱨼�
#define KeySmallStocksQuote  (KeyBegin+22)			//��С�̱���
#define KeyGEM              (KeyBegin+32)           //��ҵ��
#define KeyGEMRange			(KeyBegin+33)			//��ҵ���Ƿ�
#define KeyARange			(KeyBegin+34)			//A���Ƿ�
#define KeyAGeneral			(KeyBegin+35)			//A���ۺ�			
#define KeyShangARange		(KeyBegin+36)			//��A�Ƿ�
#define KeyShangBRange		(KeyBegin+37)			//��B�Ƿ�
#define KeyShengARange		(KeyBegin+38)			//��A�Ƿ�
#define KeyShengBRange		(KeyBegin+39)			//��B�Ƿ�
#define KeyShangBondRange	(KeyBegin+40)			//��ծ�Ƿ�
#define KeyShengBondRange	(KeyBegin+41)			//��ծ�Ƿ�	
#define KeyShangFundRange	(KeyBegin+42)			//�Ͻ��Ƿ�
#define KeyShengFundRange	(KeyBegin+43)			//����Ƿ�
#define KeySmallStocksRange	(KeyBegin+44)			//��С���Ƿ�
#define KeyShangAGeneral	(KeyBegin+45)			//��A�ۺ�
#define KeyShangBGeneral	(KeyBegin+46)			//��B�ۺ�
#define KeyShengAGeneral	(KeyBegin+47)			//��A�ۺ�
#define KeyShengBGeneral	(KeyBegin+48)			//��B�ۺ�
#define KeyShangBondGeneral	(KeyBegin+49)			//��ծ�ۺ�
#define KeyShengBondGeneral	(KeyBegin+50)			//��ծ�ۺ�
#define KeySmallStocksGeneral	(KeyBegin+51)		//��С���ۺ�
#define KeyAStocks1Minute	(KeyBegin+52)			//A���ۺ�1����
#define KeyAStocks2Minute	(KeyBegin+53)			//A���ۺ�2����
#define KeyAStocks3Minute	(KeyBegin+54)			//A���ۺ�3����
#define KeyAStocks4Minute	(KeyBegin+55)			//A���ۺ�4����
#define KeyAStocks5Minute	(KeyBegin+56)			//A���ۺ�5����
#define KeyAStocks10Minute	(KeyBegin+57)			//A���ۺ�10����
#define KeyAStocks15Minute	(KeyBegin+58)			//A���ۺ�15����

#define KeyF1				(KeyBegin+61)	
#define KeyF2				(KeyBegin+62)	
#define KeyF3				(KeyBegin+63)	
#define KeyF4				(KeyBegin+64)	
#define KeyF5				(KeyBegin+65)	
#define KeyF6				(KeyBegin+66)	
#define KeyF7				(KeyBegin+67)	
#define KeyF8				(KeyBegin+68)	
#define KeyCtrlX			(KeyBegin+69)	
#define KeyF10				(KeyBegin+70)	

#define KeyOneMinute		(KeyBegin+101)			//1������
#define KeyFiveMinute		(KeyBegin+102)			//5������
#define Key15Minute			(KeyBegin+103)            //15������
#define Key30Minute			(KeyBegin+104)			//30����
#define Key60Minute			(KeyBegin+105)			//60����
#define KeyDayData			(KeyBegin+106)			//����
#define KeyWeekData			(KeyBegin+107)			//����
#define KeyYearData			(KeyBegin+108)			//����
#define KeyMultiDayData		(KeyBegin+109)			//������

// ���̾���
struct HSShowKeyboardInfo 
{
	enum Type{left = 0x1,top = 0x2,right = 0x4,bottom = 0x8,
		all = 0x10,stock = 0x20,key = 0x40,express = 0x80};

	CRect   m_oRect;
	CWnd*   m_pWnd;
	WORD	m_cStyle;
	WORD    m_nPageType;

	// �����б����
	HSMarketDataType	m_GP; // ��Ʊ�г�
	HSMarketDataType	m_GG; // �۹�
	HSMarketDataType	m_WH; // ���
	HSMarketDataType	m_WP; // ����
	HSMarketDataType	m_QH; // �ڻ�

	char m_cShortKey;  // ���
	char m_cExpress;   // ָ��/��ʽѡ��
	char m_cEmptyAll;  // �������
	char m_cDispAll;   // ��ʾ����

	char m_cResever[7];   // ����

	HSShowKeyboardInfo(){memset(this,0,sizeof(HSShowKeyboardInfo));}
	BOOL IsStyle(WORD cStyle) { return (m_cStyle & cStyle); }
};

struct HSCurKeyInfo
{
	// wParam ��ֵ
	enum
	{
		KeyStock = 0x1000,	    // ��Ʊ
		KeyDataQuick,				// 1~6��61~66��81~86...
		UpdateExpressData,	    // ���¹�ʽ
		DragUpdateExpressData2,	// �зŸ��¹�ʽ
		DragUpdateExpressData3,	// �зŸ��¹�ʽ
		OpenBlock,			    // �򿪰��
		OpenUnionTechChart,     // ��ָ��ͼ���
		UpDataUnionTechChart,   // ����ָ��ͼ���
		OpenUsrDefPage,		    // �򿪶����ҳ��
		OpenIE,				    // ��ie
		UpdateDefExpress,		// ���³���ָ��״̬
		PreSynchronization,		// ˢ�¹�ʽǰ�Ĵ���
		OpenHKBrokerTip,		// �򿪸۹ɾ�����ϢTIP ���� 2004��7��6��
		OpenHKBrokerDeal,		// �򿪸۹ɾ���׷��ҳ��
		OpenHKStock,			// �򿪸۹ɹ�Ʊ
		GetSplSize,				// �õ���ִ��ڴ�С,�ڲ����ʹ��
		KeyDataEx,				// �ر�Ĵ򿪴���
		KeyUsedViewInfo,		// ���̾���򿪣����Ͱ���PageViewInfo���ݣ���Ҫ���Զ��������ʹ��
		KeyCommand,
		KeyStockAdd             //��Ʊ����
	};

	int			m_nOpen;	 // ������,������պ�����void CKeyboardDataList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
	char		m_szText[256];   // �ı�(ָ������Key��)
	CodeInfo    m_cCode;     // ��Ʊ��Ϣ

	PageViewInfo m_pInfo;
	HSCurKeyInfo();
	CodeInfo* GetCode();
	void Empty();
	BOOL IsEmpty();

	void Copy(const HSCurKeyInfo* pCode);
	void Copy(const char* strText);
	void Copy(const CodeInfo* pCode);
	void Copy(const PageViewInfo* pCode);

	CString GetText();
	BOOL    IsText();
	BOOL	IsBlock();
};
#endif