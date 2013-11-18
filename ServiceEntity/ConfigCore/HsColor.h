/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsColor.h
* ժ    Ҫ����ɫ����ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-10
* ��	 ע��
*******************************************************************************/

#pragma  once 
#include "ConfigBase.h"
#include "ConfigDefined.h"
#include "ConfigStruct.h"
#include "ConfigInterface.h"

enum ColorType{ClassicalColor };
enum TYPE {COLWIDTH = 0x00, COLOR = 0x01,FONT = 0x02,COLGROUP = 0x04,SETTING = 0x08,DYNNEWS = 0x10, CHUQUAN = 0x20, MOVECOST = 0x40, PROGRAMSET = 0x80};

class CHsColor :public IHsColor
{
public:
	CHsColor();
	~CHsColor();

	void Init();
	BOOL IsInit() {return m_bInited;}

public:
	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();
	
private:
	COLORREF StringToColor(const char *pColor);
	CString  ColorToString(COLORREF lColor);
	
	BOOL           SetColorNode(short nID, ColorProperty* pColor); //������ɫID ������ɫ
	TiXmlNode*     GetColorNode(short nID, DocType nType = Default); //������ɫID���ĵ����� ������ɫ�ڵ�
	ColorProperty* GetColorProperty(short nID); 	//������ɫID ������ɫ����ָ��
	TiXmlElement*  GetGroupElementFromId( WORD nColorId ); //����ID ��ȡGroup�ڵ�

public:
	virtual const COLORREF GetColumnColor(UINT nColorId); 
	virtual const COLORREF GetBlackgroundColor( );  
	virtual const COLORREF GetFixedCodeColor( );   // ��ȡ��������ı�
	virtual const COLORREF GetFixedNameColor( );   // ����֤ȯ����
	virtual const COLORREF GetFixedNameSelfColor();// ����֤ȯ��ѡ������
	virtual const COLORREF GetUpColor( );          // ��ȡ������ɫ
	virtual const COLORREF GetDownColor( );        // ��ȡ�µ���ɫ
	virtual const COLORREF GetEqualColor( );       // ��ȡƽ����ɫ
	virtual COLORREF GetTotalColor();              // ��ȡ������ɫ
	virtual COLORREF GetNowColor();                // ��ȡ������ɫ
	virtual COLORREF GetDoneColor();               // ��ȡ�ɽ���ɫ
	virtual COLORREF GetChangeColor();             // ��ȡ������ɫ
	virtual COLORREF GetQuantitativeRateColor();   // ��ȡ������ɫ 
	virtual COLORREF GetOrderBuyVolColor();        // ��ȡί������ɫ
	virtual COLORREF GetOrderSellVolColor();       // ��ȡί������ɫ

	virtual const COLORREF GetStatusTextColor( );  // ��ȡ״̬���ı���ɫ
	virtual const COLORREF GetScrollTestColor( );  // ��ȡ������ı���ɫ
	virtual const COLORREF GetScrollBKColor( );    // ��ȡ����Ʊ�����ɫ
	virtual void  SetScrollTextColor(const COLORREF clrText); // ����������ı���ɫ
	virtual void  SetScrollBKColor(const COLORREF clrBk);     // ��������Ʊ�����ɫ

	virtual const COLORREF GetTipTitleBk( );	// tip��ʾ���ⱳ��
	virtual const COLORREF GetTipTitleFg( );	// tip��ʾ����ǰ��
	virtual const COLORREF GetTipTextFg( );		// tip��ʾ�ı�����
	virtual const COLORREF GetTipTextBk( );		// tip��ʾ�ı�ǰ��

	virtual COLORREF GetQuoteFixedRowColBKColor( );	  // ��ȡ���鱨�۹̶��й̶��б���ɫ
	virtual COLORREF GetQuoteFixedRowBKColor( );	  // ��ȡ���鱨�۹̶��б���ɫ
	virtual COLORREF GetQuoteFixedColBKColor( );	  // ��ȡ���鱨�۹̶��б���ɫ
	virtual COLORREF GetQuoteHighLightFgColor( );	  // ��ȡ���鱨�۸���ǰ��ɫ
	virtual COLORREF GetQuoteHighLightBKColor( );	  // ��ȡ���鱨�۸�������ɫ
	virtual COLORREF GetQuotePriceChangeBKColor( );	  // ��ȡ���鱨�۸���.�����ı�����

	virtual COLORREF GetQuoteTabBKColor( );				// ��ȡ���鱨�۱�ǩҳ������ɫ
	virtual COLORREF GetQuoteTabTextColor( );			// ��ȡ���鱨�۱�ǩҳ������ɫ
	virtual COLORREF GetQuoteGuideTabBKColor( );		// ��ȡ���鱨���򵼱�ǩҳ������ɫ
	virtual COLORREF GetQuoteGuideTabTextColor( );		// ��ȡ���鱨���򵼱�ǩҳ������ɫ
	virtual COLORREF GetQuoteActiveTabBKColor( );		// ��ȡ���鱨�ۼ����ǩҳ������ɫ
	virtual COLORREF GetQuoteActiveTabTextColor( );		// ��ȡ���鱨�ۼ����ǩҳ������ɫ
	virtual COLORREF GetQuoteTabButtonBKColor( );		// ��ȡ���鱨�۱�ǩҳ��ť������ɫ

	virtual const ColorProperty* GetQuoteTabFrameColorProp( );	    // ��ȡ���鱨�۱�ǩҳ�߿���������ɫ
	virtual const ColorProperty* GetQuoteGuideTabFrameColorProp( ); // ��ȡ���鱨���򵼱�ǩҳ�߿���������ɫ

	virtual COLORREF GetRTitleCode();          // ��Сͼ�����Ʊ������ɫ
	virtual COLORREF GetRTitleCodeName();      // ��Сͼ�����Ʊ������ɫ
	virtual COLORREF GetRTableFixedName();     // ��Сͼ�̶��ı���ɫ
	virtual COLORREF GetRTableTime();          // ��Сͼʱ����ɫ
	virtual COLORREF GetRTableVer();           // ��Сͼ������ɫ
	virtual COLORREF GetRTableHor();           // ��Сͼ������ɫ
	virtual COLORREF GetTableTitle();          // ������ͼԪ������ɫ
	virtual COLORREF GetTableTitleBK();        // ������ͼԪ������ɫ
	virtual COLORREF GetTradeTableFixedName(); // ����ͼԪ������

	virtual COLORREF GetZiXunBK();             // ��Ѷ����ɫ
	virtual COLORREF GetZiXunFG();             // ��Ѷ�ı�ǰ��ɫ

	virtual COLORREF GetDiagramLayoutBKColor();              // ͼ���ǩҳ����
	virtual COLORREF GetDiagramLayoutActiveFillColor();      // ͼ���ǩҳ����
	virtual COLORREF GetDiagramLayoutFillColor();            // ͼ���ǩҳ����
	virtual COLORREF GetDiagramLayoutForeColor();            // ͼ���ǩҳǰ��
	virtual COLORREF GetDiagramLayoutBorderColor();          // ͼ���ǩҳ�߿�

	virtual COLORREF GetTechLineColor(int nID);         //��ȡ��������ɫ

private:
    CConfigBase*   m_pConfigBase;    // �ļ�������ָ��
	CMapWordToPtr  m_CurrColorMap;   // ��ǰʹ�õ���ɫӳ���	

	BOOL     m_bInited;          //�Ƿ��ʼ��
};