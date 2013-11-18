/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsScrollNews.h
* ժ    Ҫ�����������ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-20
* ��	 ע��
*******************************************************************************/

#pragma once

#include "ConfigInterface.h"
#include "ConfigBase.h"

class CHsScrollNews : public IHsScrollNews
{
public:
	CHsScrollNews(void);
	~CHsScrollNews(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();
private:
	int  GetStockInfo(TiXmlNode *pNode ,CStringArray &ayStockList);
	BOOL SetStockList(const CString &strPath, const CString &strParentPath, const CStringArray &ayStockList);
//�ӿ�ʵ�ֲ���
public:
	virtual int		IsDispThirdIndex();		    // �������Ƿ���ʾ
    virtual int		IsDispFourthIndex();		// �������Ƿ���ʾ 
	virtual int		IsScrollFixed();			// ������Ƿ�̶���״̬����
	virtual int		IsScrollMultiLine();		// ������Ƿ������ʾ  
 	virtual int		GetShowScrollAlone();	    // �������ʾ��ʽ 0������ƺ�״̬����ͬһ�У�1������Ƶ���һ����״̬�����棻2��״̬������һ�У��������������һ��

	virtual CString GetStatusName();          // ��ȡ����Ʊ�������
	virtual int		GetStatusScrollSpeed();	  // ������� ��Ϊ��λ
	virtual int     GetStockNewName(CStringArray &ayStockList); // ��ȡ��Ʊ������            
	virtual int     GetUpDownRate();          // �Ƿ���ʾ�ǵ���
	virtual int		GetTotal();               // �Ƿ���ʾ�ܶ�
	virtual int		GetShowNewPirce();        // �Ƿ���ʾ���³ɽ���
	virtual int		GetUpDown();              // �Ƿ���ʾ�ǵ�

	virtual int		GetScrollSpeed();		  // ��������ٶ� ����Ϊ��λ
	virtual int		GetHorizonScrollWay();	  // ����ˮƽ�������� 3: ����; 4������
	virtual int		GetVerticalScrollWay();   // ���鴹ֱ�������� 1: ����; 2������
	virtual int		GetClickWay();			  // ��ʾ���鷽ʽ 0������; 1��˫�� 
	virtual void	GetScrollText(CStringArray &ayScrollText);  // ��ȡ����ƹ�����ѯ

	//��ȡÿ��ָ������Ӧ�Ĵ����б�һ����¼��Ӧһ��CStringArray��ÿ��CStringArray�����Ⱥ����֤ȯ������г����
	//���ô˺�����ÿ��CStringArray������ڴ��ͷ�
	virtual int		GetFirstListStockInfo(CStringArray &ayStockList);
	virtual int		GetSecondListStockInfo(CStringArray &ayStockList);
	virtual int		GetThirdListStockInfo(CStringArray &ayStockList);
	virtual int		GetFourthListStockInfo(CStringArray &ayStockList);

	virtual BOOL	SetDispThirdIndex(int nValue);    // �����������ʾ���� 
	virtual BOOL	SetDispFourthIndex(int nValue);   // �����������ʾ����
	virtual BOOL	SetScrollFixed(int nValue);       // ��������ƹ̶�����
	virtual BOOL	SetScrollMultiLine(int nValue);   // ��������ƶ��п���

    virtual BOOL	SetStatusScrollSpeed(int nValue); // ����ָ����ʾ�ٶ� ��Ϊ��λ
	virtual BOOL	SetScrollSpeed(int nValue);       // ������������ٶ�
	virtual BOOL	SetHorizonScrollWay(int nValue);  // ��������ˮƽ��������
	virtual BOOL	SetVerticalScrollWay(int nValue); // �������鴹ֱ��������
	virtual BOOL	SetClickWay(int nValue);          // ����������ʾ��ʽ

	virtual BOOL	SetFirstListStockInfo(const CStringArray &ayStockList);
	virtual BOOL	SetSecondListStockInfo(const CStringArray &ayStockList);
	virtual BOOL	SetThirdListStockInfo(const CStringArray &ayStockList);
	virtual BOOL	SetFourthListStockInfo(const CStringArray &ayStockList);
private:
	CConfigBase*   m_pConfigBase;    // �ļ�������ָ��
	CMapWordToPtr  m_CurrColorMap;   // ��ǰʹ�õ���ɫӳ���	

	BOOL           m_bInited;        // �Ƿ��ʼ��
	CString        m_strRootPath;    // �߼����ڵ�·�� 
};
