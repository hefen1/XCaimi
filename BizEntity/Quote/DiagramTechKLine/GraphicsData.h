#pragma once

/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	GraphicsData.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ���߹������ݴ洢��
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2011-3-1
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#include "..\..\..\Common\ConfigStruct.h"

#define GL_HighLightText					0x00000001	// K�������Сֵ�Ƿ������͸������ʾ
#define GL_DayHorAxes						0x00000002	// ���ߺ�����
#define GL_DayVerAxes						0x00000004	// ����������
#define GL_FenShiHorAxes					0x00000008	// ��ʱ������
#define GL_FenShiVerAxes					0x00000010	// ��ʱ������
#define GL_RIGHTSTARTZOOM					0x00000020	// ���ߴ��ұ߿�ʼ����
#define GL_BOTTOMSTARTZOOM					0x00000040	// ��������ӵײ���ʼ����
#define GL_STOCK_INFO_SHOW					0x00000080	// �ұ߹�Ʊ��Ϣ
#define GL_WHOLE_SCREEN_SHOW				0x00000100	// ȫ����ʾ
#define GL_CROSS_CURSOR						0x00000200	// ʮ�ֹ��
#define GL_ONE_CURSOR						0x00000400	// һ�ֹ��
#define GL_TREND_RIGHT_AXES					0x00000800	// ��ʱͼ�ұ�����

// ����
#define GL_PERIOD_TYPE_DAY_CHART_DRAG		0x00001000	// ���������Ϸ�
#define GL_KLine_TopLine					0x00002000	// ���߶�������
#define GL_PERIOD_TYPE_DAY_CHART_DELETE		0x00004000	// ��ɾ����ͼ
#define GL_PERIOD_TYPE_DAY_CHART_ASIX_TIP	0x00008000	// ����������������ʾ
#define GL_PERIOD_TYPE_DAY_TIP_DATA			0x00010000	// ��ǰ����Ҫ������ʾ
#define GL_PERIOD_TYPE_DAY_CHUQUAN_TEXT		0x00020000	// ��ʾ��Ȩ��ϸ�ı�
#define GL_PERIOD_TYPE_DAY_AUTO_CHUQUAN		0x00040000	// �Զ���Ȩ
#define GL_PERIOD_TYPE_DAY_RIGHT_CHUQUAN	0x00080000	// ����Ȩ
#define GL_PERIOD_TYPE_DAY_RIGHT_AXES		0x00100000	// �ұ�������

#define GL_PERIOD_TYPE_DAY_INDRAWK_COST		0x00200000	// �ɱ��ֲ���K���ڻ���
#define GL_PERIOD_TYPE_DAY_SHOWHIDE_COST	0x00400000	// �Ƿ���ʾ�ɱ��ֲ�

#define GL_PERIOD_TYPE_DAY_DEFCOST_COST		0X00800000	// �ɱ��ֲ�
#define GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST	0X01000000	// ����
#define GL_PERIOD_TYPE_DAY_HUOYAODU_COST	0X02000000	// ��Ծ��

#define GL_TITLE_ADVISE_ICON_SHOW			0X04000000	// �������Ľ���ͼ��
#define GL_TITLE_JIAOYI_ICON_SHOW			0X08000000	// �������Ľ���ͼ��

#define GL_ACTIVE_HSHELP					0X10000000	// �Ƿ�ѡ���˰���
#define GL_STOCK_INFO_SHOW_EX				0X20000000  // �Ƿ��ֹ��л���Сͼ��ʾ

#define FONT_NORMAL				0x01  // ����
#define FONT_BOLD				0x02  // ����
#define FONT_ITALIC				0x04  // б��
#define FONT_UNDERLINE			0x08  // �»���

class CImageListEx : public CImageList
{
public:
	int Draw(CDC* pDC, int nImage, POINT pt, UINT nStyle,CRect rcFocus = CRect(0,0,0,0));
	BOOL DrawEx(CDC* pDC, int nImage, POINT pt, SIZE sz, COLORREF clrBk, COLORREF clrFg, UINT nStyle);
};

struct CGraphicsDataSettting
{
	DWORD m_dwStyle;	//���

	CGraphicsDataSettting()
	{
		memset(this,0,sizeof(CGraphicsDataSettting));
	}
	//���,ɾ��,�ж�,����,ȡ�÷��ֵ
	void    AddStyle(DWORD dwStyle)	   { m_dwStyle |= dwStyle; }
	void    RemoveStyle(DWORD dwStyle) { m_dwStyle &= ~dwStyle; }
	BOOL    IsStyle(DWORD dwStyle)     { return BOOL(m_dwStyle & dwStyle); }
	void    SetStyle(DWORD dwStyle)	   { m_dwStyle = dwStyle; }
	double  GetStyle()								   { return m_dwStyle; }
	//������=
	void operator=(const CGraphicsDataSettting& dwStyle)
	{
		m_dwStyle = dwStyle.m_dwStyle;
	}
	//������=
	void operator=(const long dwStyle)
	{
		m_dwStyle = dwStyle;
	}
};

class CGraphicsData
{
	//��ɫ����
public:
	// ͼ��ҳ��
	ColorProperty clPageBK;			// ����
	ColorProperty clMouseMoveMarkBK;// ����ƶ���Ǳ���
	ColorProperty clUp;				// ����
	ColorProperty clDown;			// �µ�
	ColorProperty clEqual;			// ƽ��
	ColorProperty clPreClose;	    // ����
	ColorProperty clHorLine;		// �����
	ColorProperty clVerLine;		// ������
	ColorProperty clGridLine;		// ͼ�α߿���
	ColorProperty clChartBK;		// ͼ�α���
	ColorProperty clChartTick;		// ͼ��������
	ColorProperty clChartTickTXT;   // ͼ�������ı�
	ColorProperty clChartTitle;		// ͼ�α���	
	ColorProperty clHorTXT;			// �������ı�
	ColorProperty clFixedTXT;		// �̶��ı�
	ColorProperty clFixedNameTXT;	// ���Ʊ����ı�
	ColorProperty clFixedCodeTXT;	// ��������ı�
	ColorProperty clInChartTitleTXT; // ͼ���ڱ����ı�
	ColorProperty clMoveLine;
	ColorProperty clVerDayTXT;		// �����������ı�
	ColorProperty clVerAmountTXT;   // �ɽ����������ı�
	ColorProperty clVerLiangBiTXT;  // �����������ı�
	ColorProperty clVerBuySellTXT;	// ���������������ı�
	ColorProperty clVerADLTXT;		// ADL�������ı�
	ColorProperty clVerDouKongTXT;	// ����������ı�
	ColorProperty	clHorAmountTXT;		// �ɽ����������ı�
	ColorProperty 	clHorLiangBiTXT;	// ���Ⱥ������ı�
	ColorProperty 	clHorBuySellTXT;	// ���������������ı�
	ColorProperty 	clHorADLTXT;		// ADL�������ı�
	ColorProperty 	clHorDouKongTXT;	// ��պ������ı�
	// ��ʱ����/���̷���
	ColorProperty clJsPriceLine;	// �۸���
	ColorProperty clJsAvgLine;		// ����
	ColorProperty clJsPriceMiddleLine;	// �۸��ߵ��м����
	///////////////////////////////////////////////////////////////////////////////////
	ColorProperty clJsPriceMiddleVerLine;	// �۸��ߵ��м�����
	ColorProperty clJsPriceHourLine;	// �۸��ߵ�Сʱ��
	ColorProperty clJsPriceHalfHourLine;	// �۸��ߵİ�Сʱ��
	//����ļ۸��ߵ��м����ߣ�������ָ��ߣ���Сʱ�ߣ���Сʱ�ߵ���ӻ滭����������2008-11-4���
	ColorProperty clJsLiangBiLine;  // ����ָ��
	ColorProperty clJsDownVolLine;	// �ɽ�����
	ColorProperty clJsUpVolLine;	// �ɽ�����
	ColorProperty clJsADLLine;		// ����ADL
	ColorProperty clJsDouKongLine;	// ���̶��
	ColorProperty clJsBuyLine;		// ����
	ColorProperty clJsSellLine;		// ����
	ColorProperty crDiffValueColor; // ������ֵ
	// ����
	ColorProperty clDayUpLine;		 // ������(����)
	ColorProperty clDayDownLine;	 // �µ���(����)
	ColorProperty clDayUpFillLine;   // �������(�������)
	ColorProperty clDayDownFillLine; // �µ����(�������)
	ColorProperty clDayZeroHorLine;  // ������
	ColorProperty clDayAmericaLine;	 // ������
	ColorProperty clDayTrendLine;	 // ������
	ColorProperty clDayGridVerLine;	 // ������������
	ColorProperty clDotHorLine;      // ��ʱͼ���������
	ColorProperty clDotVerLine;      // ��ʱͼ����������
	ColorProperty clDayHorLine;      // ����ͼ���������
	ColorProperty clDayVerLine;      // ����ͼ����������
	ColorProperty clDayTopLine;      // qinhn2008-11-7��ӣ�����ͼ����ߴ�ֱ�������������Ķ������ߣ�����MA��VOL������ĺ���
	ColorProperty clMaxMinValueBk;   //����ͼ�����Сֵ��������ɫ
	ColorProperty clMaxMinValueFK;   //����ͼ�����Сֵ����ǰ��ɫ
	ColorProperty clJsIOPVLine;   // IOPV��

	ColorProperty crHightBkColor;		// ����������ɫ
	ColorProperty clBoxLine;     /////DrawBox�߿���ɫ
	ColorProperty clBoxLineWidth;     /////DrawBox�߿�������ɫ�ʹ�С
	ColorProperty clDrawBoxValue;     ///DrawBox�ı���ɫ

	ColorProperty clBottomBK;		///�ײ�ʱ����ʾ������ɫ
	ColorProperty clBottomChoose;	///�ײ����ߣ������ߵȣ��л��ı���ɫ
	ColorProperty clInfoLine;		///��Ϣ���ױ߿���ɫ
	ColorProperty clInfoBk;			/////��Ϣ���ױ���
	ColorProperty clInfoHilight;     ///��Ϣ���׸���
	ColorProperty clSplitLine;     ///�ָ���
	ColorProperty clInChartText;    //  ͼԪ���ı�
	
	// -----------------------------------
	// ���岿��
	// -----------------------------------	 
	FontProperty ftXAxis;			// ��������
	FontProperty ftYAxis;			// ��������
	FontProperty ftMulti;		    // �Ŵ���
	FontProperty ftDrawVarTitle;	// ����˵��
	FontProperty ftDrawInText;		// ͼ���ڱ����ı�
	FontProperty ftPopupText;		// ͼ�ε����ı�
	FontProperty ftPeriod;			// K�������������� modify by lxqi 20100305
	FontProperty ftTab;				// tab

	CImageListEx*	m_expressImages;		 //ȫ�ֵ�ͼ���б�

//////////////////////////////////////////////////////////////////////////
private:
	CArray<ColorProperty*,ColorProperty*>	m_ayColorProperty;		// ��ǰʹ�õ���ɫ
	CArray<FontProperty*,FontProperty*>		m_ayFontProperty;		// ��ǰʹ�õ�����
	
	CArray<ColorProperty*,ColorProperty*> m_ayDefTechColor;			// ȱʡָ����ɫ

	CGraphicsDataSettting					m_sStyle;	
public:
	CGraphicsData();
	~CGraphicsData();

	BOOL			 Init();
	/********************************************************************************
	* �������� : ȫ�ַ�����,����:���,ɾ��,�ж�,����,ȡ��
	* �������� : YlsStyleSettting_Type dwStyle - ȫ�ַ��
	*  ����ֵ  : void
	*			  �ɹ�����TRUE, ʧ�ܷ���FALSE
	* �������� : ȫ�ַ�����
	* ��    �� :����ʯ
	* ������� :2003-5-7
	* �޸ļ�¼ : 
	*******************************************************************************/
	void    AddStyle(DWORD dwStyle)    { m_sStyle.AddStyle(dwStyle); }
	void    RemoveStyle(DWORD dwStyle) { m_sStyle.RemoveStyle(dwStyle); }
	BOOL    IsStyle(DWORD dwStyle)	   { return m_sStyle.IsStyle(dwStyle); }
	void    SetStyle(DWORD dwStyle)	   { m_sStyle.SetStyle(dwStyle); }
	void    SetStyle(CGraphicsDataSettting oStyle)	       { m_sStyle = oStyle; }
	double  GetStyle()								   { return m_sStyle.GetStyle(); }

	CFont*			CreateFont(char * cFontName,int nFontSize, int nFontStyle);
	/********************************************************************************
	* �������� : �ж�ֵ����Ч�� �Є�����[hx_DefMinValues hx_DefMaxValues]
	* �������� : long* lValue,...; float* lValue,... - ���ж����� ���
	*  ����ֵ  : int ������Ч����TRUE ���򷵻�FALSE
	* �������� : 
	* ��    �� :
	* ������� :
	* �޸ļ�¼ : 
	*******************************************************************************/
	BOOL			IsValue(double* lValue,...);
	long			HsGetYearMothDay(long lDate);
	ColorProperty*	GetDefTechColor(short nIndex);
};

extern CGraphicsData g_hsGraphicsData;