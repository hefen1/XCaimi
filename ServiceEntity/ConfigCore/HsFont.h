/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsFont.h
* ժ    Ҫ�������������ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-17
* ��	 ע��
*******************************************************************************/
#pragma once

#include "ConfigInterface.h"
#include "ConfigBase.h"
#include "ConfigStruct.h"

class CHsFont :
 	public IHsFont
{
public:
	CHsFont(void);
	~CHsFont(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

private:

	BOOL           SetFontNode(short nID, FontProperty* pColor);   //��������ID ��������
	TiXmlNode*     GetFontNode(short nID, DocType nType = Default); //��������ID���ĵ����� ��������ڵ�
	FontProperty*  GetFontProperty(short nID); 	//��������ID ������ɫ����ָ��

public:
	//////////////////////////////////////////////////////////////////////////
	// ��ȡ
	virtual CFont* GetTipTitleFont();       //��ȡTIP��ʾ��������
	virtual CFont* GetTipTxtFont();         //��ȡTIP��ʾ�ı�����

	virtual CFont* GetQuoteTabFont();		//��ȡ���鱨�۱�ǩҳ����
	virtual CFont* GetQuoteGuideTabFont();  //��ȡ���鱨���򵼱�ǩҳ����
	virtual CFont* GetStatusFontProt();	    //��ȡ״̬��ָ������
	virtual CFont* GetScrollFontProt();     //��ȡ�������������

	virtual CFont* GetQuoteRefreshFont();	// ��ȡ���鱨�۸�������
	virtual CFont* GetQuoteColHeadFont();	// ��ȡ���鱨�۱�ͷ����
	virtual CFont* GetQuoteStringFont();	// ��ȡ���鱨����������(������)
	virtual CFont* GetQuoteCodeFont();	    // ��ȡ���鱨�۹�Ʊ��������
	virtual CFont* GetQuoteDynamicFont();	// ��ȡ���鱨�۶�̬�ı�����
	virtual CFont* GetQuoteFixedFont();	    // ��ȡ���鱨�۹̶��ı����壨��
	
	virtual CFont* GetRTableFixedFont();        // ��ȡ��Сͼ�ֶ���������
	virtual CFont* GetRTableDynamicalFont();    // ��ȡ��Сͼ��̬�ֶ�����
	virtual CFont* GetRTableTitleCodeFont();    // ��ȡ��Сͼ�����Ʊ��������
	virtual CFont* GetRTableTitleNameFont();    // ��ȡ��Сͼ�����Ʊ��������
	virtual CFont* GetRTableFinanceFont();      // ��ȡ��Сͼ���ֵҳ�ı�����
	virtual CFont* GetRTableTimeFont();         // ��ȡ��Сͼʱ��������
	virtual CFont* GetDiagramLayoutTabFont();   // ��ȡͼ���ǩҳ����
	virtual CFont* GetDiagramTableTitleFont();  // ��ȡ������ͼԪ�ı�����

	virtual CFont* GetZiXunFont();              // ��ȡ��Ѷ����
	virtual CFont* GetDiagramTradeFont();       // ��ȡ����ͼԪ����
	//////////////////////////////////////////////////////////////////////////
	// д��
	virtual BOOL SetScrollFontProt(CFont* pFontScroll); // �����������������


private:
    CConfigBase*    m_pConfigBase;     // �ļ�������ָ��
	CMapWordToPtr   m_CurrFontMap;     // ��ǰʹ�õ�����ӳ���	

	BOOL     m_bInited;          //�Ƿ��ʼ��

	CFont m_QuoteDefaultFont;
	CFont m_TradeDefaultFont;
};
