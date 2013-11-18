/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawTableStruct.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ������Ҫ�ṹ�Լ�ȫ�ֱ��� �����ļ�
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2010-12-1
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/

#pragma once
#include "DiagramFoundation.h"
#include "DiagramFoundationDef.h"
extern CDiagramFoundationApp theFoundationApp;

#define  g_lDefautColor RGB(0,0,0)
//#define  g_lHighLightColor RGB(1,37,218)

//////////////////////////////////////////////////////////////////////////
// �������
#define  MOUSE_SIZEWE   0x0001 // ���������
#define  MOUSE_ARROW    0x0002 // �����ԭ��
#define  MOUSE_HAND     0x0004 // ���������
// ��ǰ��Ҫ�����ı�־
#define  NEED_NOTHING       0x0001 // ����Ҫ���κβ���
#define  NEED_RESIZE        0x0002 // ��Ҫ������С
#define  NEED_MOVE          0x0004 // ��Ҫ�����ƶ�
#define  NEED_TEXTHIGHLIGHT 0x0008 // �ı�������ʾ
#define  NEED_TIP           0x0010 // ��Ҫ��ʾTip
// Tip
#define  DELAY_SHOWTIP_TIMESPAN 500    // Tip�ӳ���ʾʱ��
#define  OFFSET_CX               10    // Tip��ʾ����ʼλ��ƫ�Ƶ�ǰ������� X��
#define  OFFSET_CY               10    // Tip��ʾ����ʼλ��ƫ�Ƶ�ǰ������� Y��

// ��ɫ
#define  TEXT_COLOR_UP				0x0001 // ������ɫ
#define  TEXT_COLOR_DOWN			0x0002 // �µ���ɫ
#define  TEXT_COLOR_FLAT			0x0003 // ƽ����ɫ
#define  TEXT_COLOR_TIME			0x0004 // ʱ����ɫ
#define  TEXT_COLOR_FIXEDNAME		0x0005 // �ֶ�������ɫ
#define  TEXT_COLOR_ACTIVEFIXEDNAME 0x0006 // �ֶ����Ƽ�����ɫ
#define  TEXT_COLOR_XIANHAND        0x0007 // ������ɫ
#define  TEXT_COLOR_ZONGBI          0x0008 // �ܱ���ɫ �� ������ɫһ��
#define  TEXT_COLOR_JINGZHI         0x0008 // ��ֵ��ɫ �� ������ɫһ��
#define  TEXT_COLOR_CHENGJIAO       0x0009 // �ɽ�����ɫ
#define  TEXT_COLOR_ZONGE           0x0009 // �ܶ���ɫ �� �ɽ���һ��
#define  TEXT_COLOR_HUANSHOU        0x000A // ������ɫ
#define  TEXT_COLOR_LIANGBI         0x000B // ������ɫ
#define  TEXT_COLOR_STOCKCODE       0x000C // ��Ʊ������ɫ
#define  TEXT_COLOR_STOCKNAME       0x000D // ��Ʊ������ɫ
#define  TEXT_COLOR_STOCKNAME_SELF  0x000E // ��ѡ�ɹ�Ʊ������ɫ
#define  TEXT_COLOR_TOTALHAND		0x000F // ������ɫ
#define  TEXT_COLOR_MEIBI           0x000F // ÿ����ɫ �� ������ɫһ��
#define  TEXT_COLOR_WEIBUY          0x0010 // ί������ɫ
#define  TEXT_COLOR_BUYCOUNT        0x0010 // ������ɫ �� ί������ɫһ��
#define  TEXT_COLOR_JUNLIANG        0x0010 // 5�վ�����ɫ �� ί������ɫһ��
#define  TEXT_COLOR_WEISELL         0x0020 // ί������ɫ
#define  TEXT_COLOR_SELLCOUNT       0x0020 // ������ɫ �� ί������ɫһ��
#define  TEXT_COLOR_TIPFG           0x0030 // Tip�ı���ɫ
#define  TEXT_COLOR_TIPBK           0x0040 // Tip������ɫ
#define  TEXT_COLOR_TRADEFIXEDNAME  0x0050 // ����ͼԪ�̶��ı���ɫ
#define  TEXT_COLOR_TRADEUP         0x0060 // ����������ɫ
#define  TEXT_COLOR_TRADEDOWN       0x0070 // �����µ���ɫ
#define  TEXT_COLOR_TRADEXIANSHOU   0x0080 // ����������ɫ

// ����
#define  TEXT_FONT_FIXED      0x0001 // ��̬�ֶ�����       
#define  TEXT_FONT_DYNAMICAL  0x0002 // ��̬�ֶ�����
#define  TEXT_FONT_FINANCE    0x0003 // ���ֵҳ�ı�����
#define  TEXT_FONT_TIME       0x0004 // ʱ�����ı�����
#define  TEXT_FONT_TITLE      0x0005 // ����������
#define  TEXT_FONT_TIP        0x0006 // TIP����
#define  TEXT_FONT_TRADE      0x0007 // ����ͼԪ����
#define  TEXT_FONT_QUOTE_CODE 0x0008 // ���鱨�۴�������
#define  TEXT_FONT_QUOTE_NAME 0x0009 // ���鱨����������
//////////////////////////////////////////////////////////////////////////
//���ʽṹ
struct HsLOGPEN// :public LOGPEN
{
public:
	enum HorVer {hor,ver};

	UINT        lopnStyle;	//�߷��
	POINT       lopnWidth;	//�߿��
	COLORREF    lHorColor;	//��������ɫָ��
	COLORREF    lVerColor;  //��������ɫָ��

	HsLOGPEN()
	{
		Empty();
	}
	//����ȱʡ
	void Empty()
	{
		lHorColor = lVerColor = g_lDefautColor;
	}
	//��ֵ������
	void operator=(HsLOGPEN logpen)
	{
		lopnStyle = logpen.lopnStyle;
		lopnWidth = logpen.lopnWidth;
		lHorColor = logpen.lHorColor;
		lVerColor = logpen.lVerColor;		
	}
	LOGPEN GetPen(HorVer horver)
	{
		LOGPEN pen;
		pen.lopnStyle = lopnStyle;
		pen.lopnWidth = lopnWidth;
		pen.lopnColor = (horver == hor)?lHorColor:lVerColor;
		return pen;
	}
};

//////////////////////////////////////////////////////////////////////////
// 
class HS_EXT_CLASS DrawTool
{
public:
	static COLORREF GetColor(WORD wClr);
	static CFont* GetFont(WORD wFont);

private:
	static CMap<WORD,WORD&, DWORD, DWORD&>  m_mapColor;
	static CMapWordToPtr                    m_mapFont;
};

//////////////////////////////////////////////////////////////////////////
struct ItemConstructionStruct
{// ���쵥ԪС��ʱ ���Խṹ��
	char   str[64];     // �ı�����
	WORD   wClr;        // ��ɫID
	WORD   wFont;       // ����ID
	DWORD  dwItemStyle; // ��Ԫ������
	double dWidth;      // ��ռ��Ȱٷֱ�
	DWORD  dwExtent;    // ��չ
};
//////////////////////////////////////////////////////////////////////////

struct ColInfoStruct
{// ��������Ϣ
	int   nWidth;       // �п�
	int   nID;          // ��ID

	ColInfoStruct(int width = 0, int ID = -1)
	{
		nWidth = width;
		nID    = ID;
	}

	void Copy(ColInfoStruct* pColInfo)
	{
		if (pColInfo)
		{
			nWidth = pColInfo->nWidth;
			nID    = pColInfo->nID;
		}
	}

	void operator=(ColInfoStruct* pColInfo)
	{
		Copy(pColInfo);
	}

	void operator=(ColInfoStruct pColInfo)
	{
		Copy(&pColInfo);
	}

};
