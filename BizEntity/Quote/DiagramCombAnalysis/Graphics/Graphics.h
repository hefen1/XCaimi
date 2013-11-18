/*******************************************************************************
 * Copyright (c)2003, ���ݺ�����Ϣ�������޹�˾
 * All rights reserved.
 *
 * �ļ����ƣ�Graphics.h
 * �ļ���ʶ�����Ʋ�����
 * ժ    Ҫ�����Ʋ�����
 *
 * ��ǰ�汾��2.0
 * ��    �ߣ�����ʯ
 * ������ڣ�2003��05��16�� ������ 
 *
 * ȡ���汾��
 * ԭ �� �ߣ�����ʯ
 * ������ڣ�2003��05��16�� ������ 
 * ��	 ע��
 *******************************************************************************/
// Graphics.h: interface for the CGraphics class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


#include <math.h>
#include "HSBaseDC.h"
#include "..\..\..\Common\QuoteComm\HSDrawBaseStruct.h"
#include "..\..\..\Common\QuoteComm\DrawBase.h"
#include "..\..\..\Common\ConfigStruct.h"
#include "..\..\..\Common\hsstructnew.h"
#include "..\..\..\Common\hsds_comudata.h"
#include "..\..\..\Common\configDefined.h"

#include "GraphicsData.h"
#define  PiV     3.14159265358979324                       
#define  ConstV  180

#define  MOVECOSTCOUNT 10

#define PKLINE_TYPE_FENSHI   		0x0001  // ��ʱͼ
#define PKLINE_TYPE_K       		0x0002  // k ��ͼ

// ������/��ʾ����
/*
#define DT_LEFT             0x00000000  // �ı���ʾ
#define DT_CENTER           0x00000001
#define DT_RIGHT            0x00000002
#define DT_VCENTER          0x00000004
#define DT_BOTTOM           0x00000008
*/
#define AXIS_LEFT			0x00000010  // �������
#define AXIS_TOP			0x00000020  // �ϱ�����
#define AXIS_RIGHT			0x00000040  // �ұ�����
#define AXIS_BOTTOM			0x00000080  // �±�����

#define AXIS_FIRSTTEXT      0x00000100  // ����������ı�
#define AXIS_LASTTEXT       0x00000200  // �²����ұ��ı�
#define AXIS_FENSHIPRICE    0x00000400  // ��ʱ�۸�����
#define AXIS_DRAWBACK       0x00000800  // ͼ���Ƿ񻭱���
#define AXIS_DRAWBOUNDARY   0x00001000  // ͼ���Ƿ񻭱߽�
#define AXIS_DUOKONGMIDDLE  0x00002000  // ����м����
#define AXIS_SOLIDANDDASH   0x00004000  // ��ʱ�۸�����ʵ�ߺ����߽������
#define AXIS_FENSHIHORI	    0x00008000  // ��ʱ������

#define AXIS_LEFTMARGIN		0x00010000  // ��߽�
#define AXIS_TOPMARGIN		0x00020000  // �ϱ߽�
#define AXIS_RIGHTMARGIN	0x00040000  // �ұ߽�
#define AXIS_BOTTOMMARGIN	0x00080000  // �±߽�

#define AXIS_SELEXPLAINRECT	0x00100000  // ѡ��ͼ��������ʾ����
#define AXIS_FORCEDMARGIN	0x00200000  // ǿ��ʹͼ�����ұ߽��Сһ��
#define AXIS_ACTIVE			0x00400000  // ��ǰ�ǻ״̬
#define AXIS_NOTDRAWFIRST	0x00800000  // ������һ��

#define AXIS_NOTDRAWLAST	0x01000000  // �������һ��
#define AXIS_RIGHTDISPPRICE	0x02000000  // �ұ�Ҫ��ʾ��
#define AXIS_STARHORI		0x04000000  // �ǿ�ͼ�ĺ�����
#define AXIS_HIGHFREQUENCYHORI 0x08000000 //��Ƶ������

// ͼ��ѡ��
#define CHART_SELPKLINE		0xF1  // ѡ�У���
#define CHART_SELVOLUME		0xF2  // ѡ�гɽ���
#define CHART_SELMACD		0xF3  // ѡ��MACD

#define CHART_MOVED			0x08  // ƽ��
#define CHART_LIGHT			0x10  // ����

//////////////////////////////////////////////////////////////////////////////////
////// class CGraphics
class CGraphics
{
public:
	CGraphics();
	CGraphics(CDrawBase* pThis)    { Defaut(); m_pThis = pThis; }
	void SetThis(CDrawBase* pThis)	 { m_pThis = pThis; }

	~CGraphics();

	void Defaut();
	/*�ṹ
	�ɱ��ֲ���
	*/
	struct MoveCostConfig
	{
		char   cValid;		//��ע�ɱ��ֲ������Ƿ���Ч

		int    nMoveCostMul;//Ԥ��
		int    nCostDay;	//�ɱ��������ݸ���
		int    nHand;		//Ԥ��
		double dLuiTongPan;	//Ԥ��
		double nMaxData;	//���ֵ

		void EmptyCost();

		MoveCostConfig() 
		{ 
			nMoveCostMul = 1;
			nCostDay = 500;
            nHand = 0;
			dLuiTongPan = 0;
			nMaxData = 0;
			cValid = 0; 
		}
		//����������Ч��
		BOOL IsValid() { return cValid; }
		//�ɱ��ֲ�,����ɽ,��Ծ������
		static double dCostNowData[MOVECOSTCOUNT + 1][1000];
	};
	//����״̬�ṹ
	struct CurrentActiveStatus
	{
		void* m_pCurObj;	//ָʾ��ǰͼԪ
		CRect m_rcAtive;	//�Ƿ񼤻�
		CurrentActiveStatus()
		{
			m_pCurObj = NULL;
			m_rcAtive.SetRectEmpty();
		}
	};
	//�߽���������ṹ
	struct CMarginParam
	{
		int m_nDef,m_nMin,m_nMax;//ȱʡֵ, ��С, ���ֵ

		enum marginType{left,top,right,bottom};

		CMarginParam() { memset(this,0,sizeof(CMarginParam)); }
		CMarginParam(int nDef, int nMin, int nMax)
		{
			m_nDef = nDef;
			m_nMin = nMin;
			m_nMax = nMax;			
		}
		const CMarginParam& operator=(const CMarginParam& marginSrc)
		{
			m_nDef = marginSrc.m_nDef;
			m_nMin = marginSrc.m_nMin;
			m_nMax = marginSrc.m_nMax;
			return *this;
		}
		void SetSame(int nDef)
		{
			m_nDef = nDef;
			m_nMin = nDef;
			m_nMax = nDef;
		}
	};
	//��Ӧ�� �� �� �±߽�
	struct CMargin { CMarginParam left,top,right,bottom; };	
	//ѡ��״̬���Ͷ���
	enum SelectedReturnStatus { CONTINUE, FINDED = 1, BREAK };
	//����������Ͷ���
	enum CMarkerType { CROSS  = 0x00000001, XCROSS = 0x00000002, STAR = 0x00000004,
					   CIRCLE = 0x00000008, TRIANGLE = 0x00000010, DIAMOND = 0x00000020, 
					   SQUARE = 0x00000040, FCIRCLE = 0x00000080, FTRIANGLE = 0x00000100, 
					   FDIAMOND = 0x00000200, FSQUARE = 0x00000400,
					   SELECTED = 0x00000800 ,DOWNFTRIANGLE = 0x00001000,
					   INFODATA = 0x00002000,DRAWSTRING = 0x00004000};

	enum CLineType { 
					 NONE		  = 0x00000000,
					 STICK        = 0x00000001,	  // ��״�ߣ��ӣ���ָ��ֵ�仭��ֱ��
					 COLORSTICK   = 0x00000002,   // ��ɫ��״�ߣ���ֵΪ0����ʾ��ɫ��������ʾ��ɫ
					 VOLSTICK     = 0x00000004,   // �ɽ�����״�ߣ����ɼ�����ʱ��ʾ��ɫ��������������ʾ��ɫ������
 					 LINESTICK    = 0x00000008,   // ͬʱ������״�ߺ�ָ����
					 CROSSDOT     = 0x00000010,   // С����
					 CIRCLEDOT    = 0x00000020,   // СԲȦ��
					 POINTDOT     = 0x00000040,   // СԲ����

					 PKLINESTICK  = 0x00000080,   // K��
					 USERDRAWLINE = 0x00000100,   // ʹ�û�ͼ����

					 DRAW3D		  = 0x00000200,	  // ��3D��ʽ
					 DRAWZEROLINE = 0x00001000,   // ��������
					 ADDPKLINE	  = 0x00002000,   // ���K��
					 NOTSELMARK	  = 0x00004000,   // ����ѡ�б�־
					 LINEDRAW3D   = 0x00008000,   // ��3D����

					 NOTDRAWLINE  = 0x00010000,    // ��ǰ���߲�����
					 NOTDRAWTEXT  = 0x00020000,    // ��ǰ�ı�������
					 NOTDRAWLINETEXT  = NOTDRAWLINE | NOTDRAWTEXT, // ��ǰ�����ı���������
					 SELECTMARK       = 0x00040000,  // ����ϵͳ��ѡ��ָʾ�����k��
/*
					#define HX_NOTTABLE			   0x00080000  // Ϊ��ͼ���ʽ,���ǲ��ڱ������ʾ
					#define HX_NOTEXPRESSDRAW	   0x00100000  // Ϊ��ͼ���ʽ,���ǲ���ͼ
					#define HX_EXPRESSDRAW		   0x00200000  // Ϊ��ͼ���ʽ
					#define HX_EXPRESSDRAW_NONAME  0x00400000  // Ϊ��ͼ���ʽ(û������)

					#define HX_EXPRESSSENTENCE	   0x00800000  // Ϊ�����ʽ
					#define HX_EXPRESSNUMBER	   0x01000000  // Ϊ��ֵ���ʽ
					#define HX_EXPRESSFUNCTION	   0x02000000  // Ϊ�������ʽ
					#define HX_EXPRESSREFERENCE	   0x04000000  // Ϊ���������ʽ
					#define HX_UNION_EXPRESS	   0x08000000  // Ϊ��ϱ��ʽ
					#define HX_UNKNOWFUNPARAM	   0x10000000  // Ϊ�������ʽ,���������
*/
					CURVELINETOEND       = 0x20000000,    // �����������
					//HX_NOTCALCMAXMIN     0x40000000,    // �����������Сֵ
//ADD BY YUHJ
          NOTDRAWMSGMARK       = 0x80000000
//END
	};

	/********************************************************************************
	 * �������� :������������,�㷨�ο�CRatio�ṹ���� 
	 * �������� : double xmin ,ymin ,xmax ,ymax ��Ӧ CRatio �ṹ��Ա
	 *  ����ֵ  : void
	 * �������� : ��Default()����
	 * ��    �� :����ʯ
	 * ������� :2003-4-22
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void SetRatio(double xmin = 0, double ymin = 0, double xmax = 1, double ymax = 1);
	/********************************************************************************
	 * �������� : ����X,Y������(m_XRefParam, m_YRefParam)��ز���
	 * �������� : double min ,max,  unit ,ref,int nDiv , nDecimal , lineWidth -
	 *			   ��ӦCRefParam�ṹ����
	 *			  CHSBaseDC* pDC - δʹ��
	 *  ����ֵ  : void
	 * �������� : ����Reset Size������
	 * ��    �� :����ʯ
	 * ������� :2003-4-22
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void SetYRefParam(double min = 0, double max = 0,
		double unit = 1, double ref = 0,
		int nDiv = 1, int nDecimal = 0,
		int lineWidth = 0,
		CHSBaseDC* pDC = NULL);

	// �Զ�������
	CArray<double,double>* GetUsrDefAxis() { return &m_ayUsrDefAxis; }
	void AddUsrDefAxis(	double dwAxis,BOOL bDelAll = FALSE );

	void SetXRefParam(double min = 0, double max = 0, double unit = 1, 
		double ref = 0,int nDiv = 1, int nDecimal = 0,
		int lineWidth = 0,CHSBaseDC* pDC = NULL);
	/********************************************************************************
	 * �������� : ���ÿ̶���ز��� (m_TickLineLen)
	 * �������� : unsigned char lm ,tm ,rm , bm  - ��Ӧ�ṹCTickLineLen����
	 *  ����ֵ  : void
	 * �������� : ��ʼ��
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void SetTickLineLen(unsigned char lm = 3,unsigned char tm = 3,unsigned char rm = 3,unsigned char bm = 4);
	//���ر߽�����ָ��
	CRect* GetMargin() { return &m_rcMargin; }
	/********************************************************************************
	 * �������� : ���ر߽�����ָ��;��������߽�����ƫ��(PKLINE_TYPE_FENSHIƫ��1,����3)
	 *			  ���ָ���˻�ͼ�豸,�����ȸ��������������;
	 * �������� : CRect& rect - ���ر߽�����topƫ��
	 *			  DWORD dwStyle - ��ʽPKLINE_TYPE_FENSHI��
	 *			  CDC* pDC - ��ͼ�豸ָ��
	 *  ����ֵ  : CRect*	�߽�����ָ�� &m_rcMargin
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	CRect* GetMargin(CRect& rect,DWORD dwStyle = 0,CDC* pDC = NULL);   // margin
	/********************************************************************************
	 * �������� : ���ñ߽����� ���ݷ��AXIS_LEFTMARGIN,AXIS_TOPMARGIN,AXIS_RIGHTMARGIN,AXIS_BOTTOMMARGIN
	 *			  �����Ƿ�����ֵ������0	
	 * �������� : CRect rect - ����
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void		  SetMargin(CRect rect);
	/********************************************************************************
	 * �������� : ���ñ߽�����	
	 * �������� : CMarginParam margin - ֵ
	 *			  char cType - ���õ�����, ȡֵΪCMarginParam::top��ö��
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void		  SetMargin(CMarginParam margin,char cType);
	/********************************************************************************
	 * �������� : ȡ�ñ߽�����ṹָ��
	 * �������� : char cType - ���õ�����, ȡֵΪCMarginParam::top��ö��
	 *  ����ֵ  : CGraphics::CMarginParam* �߽�����ṹָ��,ʧ�ܷ���NULL
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	CGraphics::CMarginParam* GetMargin(char cType);
	/********************************************************************************
	 * �������� : ���ô�ӡ�̶�
	 * �������� : HDC& hDC - ��ӡ��ͼ���þ��
	 *			  RECT& rect - ��ӡ����
	 *			  ����δ�ı�ֵ
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void SetPrintScale(HDC& hDC, RECT& rect);
	/********************************************************************************
	 * �������� : ���¼�������,�����ͻ�����,�߽�����,ʵ�ʻ�ͼ�����
	 * �������� : RECT& rt - ָ���ͻ�����
	 *  ����ֵ  : void
	 * �������� : �˺����Ƚ���Ҫ,��ӡ,��ͼ����Size��������
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void RecalcRects(RECT& rt);
	/********************************************************************************
	 * �������� : ��ͼ��ʼ,����䱳����ֽ���
	 * �������� : CHSBaseDC* pDC - ��ͼ�豸ָ��
	 *  ����ֵ  : void
	 * �������� : DrawBkGround(), DrawBoundary()
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void BeginDraw(CHSBaseDC* pDC);
	/********************************************************************************
	 * �������� : ��ͼ��ֹ, ����Ǵ�ӡ,���ô�ӡ��ʶΪ�ս�
	 * �������� : CHSBaseDC* pDC - ��ͼ�豸ָ��
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void EndDraw(CHSBaseDC* pDC);
	/********************************************************************************
	 * �������� : �����ɫ���� ���û���ȡ����ɫ���ýṹ
	 *			  ����;��������ɫ;�߿���ɫ;�̶�����ɫ;�̶��ı���ɫ;������ɫ
	 * �������� : ColorProperty* cr - ��ɫ�ṹָ��;
	 *			  ColorProperty* crVer,ColorProperty* crHor - �ݺ���ɫ�ṹָ��
	 *  ����ֵ  : void
	 *			  ColorProperty ��ɫ���ýṹ
	 * �������� : �ı���ɫʱ�� , ������, ���������ػ�
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	
	void SetBackColor(ColorProperty* cr)						  { *m_pBackColor = cr; }
	ColorProperty GetBackColor()								  { return **m_pBackColor; }
	
	void SetGridColor(ColorProperty* crVer,ColorProperty* crHor)  { *m_pVerLineColor = crVer; *m_pHorLineColor = crHor; }
	ColorProperty GetGridVerColor()							{ return **m_pVerLineColor; }
	ColorProperty GetGridHorColor()							{ return **m_pHorLineColor; }
	
	void SetBorderColor(ColorProperty* cr)					{ *m_pGridLineColor = cr; }
	ColorProperty GetBorderColor()							{ return **m_pGridLineColor; }
	
	void SetTickLineColor(ColorProperty* cr)				{ *m_pChartTickColor = cr; }
	ColorProperty GetTickLineColor()						{ return **m_pChartTickColor; }
	
	void SetTickTXTColor(ColorProperty* cr)					{ *m_pChartTickTXTColor = cr; }
	ColorProperty GetTickTXTColor()							{ return **m_pChartTickTXTColor; }

	void SetTitleColor(ColorProperty* cr)					{ *m_pChartTitleColor = cr; }
	ColorProperty GetTitleColor()							{ return **m_pChartTitleColor; }
	//ȡ�õ�ǰ��ͼ�豸ָ�� ֱ�ӷ���m_pDC
	CHSBaseDC* GetBaseDC()						  { return m_pDC; }
	/********************************************************************************
	 * �������� : ����X��Y��̶Ȼ��ָ���, �������
	 * �������� : int x, y - X��Y��̶Ȼ��ָ���, �������
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void SetXNumOfTicks(int x = 50);
	void SetYNumOfTicks(int y = 50);
  int  GetYNumOfTicks();
	void SetXNumOfGridTicks(int x = 10);
	void SetYNumOfGridTicks(int y = 10);
	/********************************************************************************
	 * �������� : ȡ��,�����ݺ�������λ
	 * �������� : int decimal - ������λ
	 *  ����ֵ  : void, int
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 * ����֮�� : ����δʹ��, ��Ӧ����δʹ��,��չʹ��
	 *******************************************************************************/
	void SetXDecimal(int decimal)				{ m_nXDecimal = decimal; }
	int  GetXDecimal()							{ return m_nXDecimal; }
	void SetYDecimal(int decimal)				{ m_nYDecimal = decimal; }
	int  GetYDecimal()							{ return m_nYDecimal; }
	/********************************************************************************
	 * �������� : �����ı����뷽ʽ
	 * �������� : DWORD x - ���� DWORD y - ����
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void SetStringAlign(DWORD x, DWORD y)		{ m_StrAlign.HAlign = x; m_StrAlign.VAlign = y; }
	/********************************************************************************
	 * �������� :�������� 
	 * �������� : const char* FontName - ��������, �ǿ��ָ���
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void SetFontName(const char* FontName)		{ strcpy(m_LogFont.lfFaceName, FontName); }
	/********************************************************************************
	 * �������� : ʹ�ܱ߿����Լ��߿���Ӱ
	 * �������� : bool bFlag - ʹ�ܱ�ʶ
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 * ����֮�� : Ŀǰδʹ��, ����չ��
	 *******************************************************************************/
	void EnableLegend(bool bFlag = true)		{ m_bEnableLegend = bFlag; }
	void EnableLegendShadow(bool bFlag = true)	{ m_bLegendShadow = bFlag; }
	//ʹ��˫����
	void EnableMemoryDraw(bool bFlag = false)	{ m_bMemoryDraw = bFlag; }
	/********************************************************************************
	 * �������� : ���߽� 
	 * �������� : ColorProperty& cr - �߽����͵�����
	 *			  int nMargin - ��ӡʱ��ʹ��, ������С����
	 *  ����ֵ  : void
	 * �������� : ������AXIS_DRAWBOUNDARY���
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
    void DrawBoundary(ColorProperty& cr, int nMargin);
	/********************************************************************************
	 * �������� : ����Ӱ
	 * �������� : int n - ƫ����
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void DrawShadow(int n);
	/********************************************************************************
	 * �������� :������� �������ߵײ� X�����Y���
	 * �������� :const char* Title - �ı�, int Pos -  λ�� DT_TOP��
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 * ��ע		:��������, δʹ��
	 *******************************************************************************/
	void Title(const char* Title, int Pos = DT_TOP);
	void XAxisTitle(const char* Title, int Pos = DT_BOTTOM);
	void YAxisTitle(const char* Title, int Pos = DT_LEFT);
	/********************************************************************************
	 * �������� : ����ͼ�α߿��Լ��̶��� �̶��ı�
	 * �������� : void
	 *  ����ֵ  : void
	 * �������� : DrawBorder
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void Axes();
	/********************************************************************************
	 * �������� : ����ͼ�α߿�
	 * �������� : void
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void DrawBorder();
	/********************************************************************************
	 * �������� :	���ڻ���ͼ���ڱ���,�����չA..., ����ָʾ�ı���
	 * �������� :	CToptextType<double>* Value,... - �����ı��ṹ, �����ı�,��ɫ�� ���
	 *				CArray< CToptextType<double>*,CToptextType<double>* >& Value
	 *					- �����ı��ṹ����, �����ı�,��ɫ�� ���
	 *				class CDrawBase* pCurObj- ��ǰ����ͼԪ, ����ȡ��ǰһͼԪ
	 *				CNameProperty& strExpName-��ʽ�ṹ,�����ı�,��ɫ���Ե�,Ҫ���ı��ǿ��ָ���
	 *				CArray<CLineProperty*,CLineProperty*>* ayData -
	 *				int nCurDataPos-��ǰ���ָʾ������λ��
	 *				int nDiv- δʹ��
	 *				short nDecimal- �������ű���(����)
	 *				BYTE bArrow- �Ƿ񻭼�ͷ
	 *				DWORD dStyle ��״̬, PKLINE_TYPE_ACTIVE_CHART
	 *				DWORD dObjectStyle  - ��ʽ����CExpression::Condition��
	 *  ����ֵ  : int
	 * �������� : ������
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	int DrawVarTitle(CToptextType<double>* Value,...);
	int DrawVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& Value, int nStartPositon = 0);
	int DrawVarTitle(class CDrawBase* pCurObj,CNameProperty& strExpName,CArray<CLineProperty*,CLineProperty*>* ayData,int nCurDataPos,
					  int nDiv,short nDecimal,BYTE bArrow,DWORD dStyle = 0,DWORD dObjectStyle = 0);
	//���÷�Χ ���麯�� 
	virtual bool SetRange(double xmin, double ymin, double xmax, double ymax) = 0;
	bool SetRange_LogY(double xmin, double ymin, double xmax, double ymax);

	/********************************************************************************
	 * �������� : ��X,Y������,�̶��������ı�
	 * �������� : void
	 *  ����ֵ  : void
	 * �������� : ���Ƶ���
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual void XAxis();
	virtual void YAxis();
	void YAxis_LogY();

	/********************************************************************************
	 * �������� : ��������ͼ�ұ�������̶����Լ��ı�
	 * �������� : void
	 *  ����ֵ  : void
	 * �������� : ���Ƶ���
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual void RiXiaRightYAxis();
	/********************************************************************************
	 * �������� : ��ʱ����ͼ,�������̶����ı�
	 * �������� :  void    
	 *  ����ֵ  :  void    
	 * �������� :  ���Ƶ���
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual void FenShiYAxis();
	/********************************************************************************
	 * �������� : ��ʱ����ͼ,�ұ�����̶����ı�
	 * �������� :  void    
	 *  ����ֵ  :  void    
	 * �������� :  ���Ƶ���
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual void FenShiRightYTick();
	/********************************************************************************
	 * �������� : ����������(������ʱ������)
	 * �������� : void
	 *  ����ֵ  : void
	 * �������� : DrawMiddleLine
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual void Grid();
	void		 Grid_LogY();
	/********************************************************************************
	 * �������� : ����ͼ���ڱ���ĸ���ѡ��״̬
	 * �������� : void
	 *  ����ֵ  : void
	 * �������� : Ŀǰ�Ѿ�ȡ��, ����ֱ�ӷ���
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual void DrawExplainRect();
	//���ƽ���߿�, Ŀǰ�Ѿ�ȡ��, ���뱻ע��
	virtual void DrawActive(BOOL bActive = FALSE);
	/********************************************************************************
	 * �������� : �����м�ķָ���
	 * �������� : int x,int y,int x1,int y1- ֱ�ߵ�������
	 *			  int LineWidth , int LineStyle  - ֱ�ߵ���ʽ����
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual void DrawMiddleLine(int x,int y,int x1,int y1,int LineWidth = 2, int LineStyle = PS_SOLID);
	/********************************************************************************
	* �������� : �����м�ķָ����ߣ���������ָ��ߵĻ滭������
	 * �������� : int x,int y,int x1,int y1- ֱ�ߵ�������
	 *			  int LineWidth , int LineStyle  - ֱ�ߵ���ʽ����
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :������
	 * ������� :2008-11-4
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual void DrawMiddleVerLine(int x,int y,int x1,int y1,int LineWidth = 2, int LineStyle = PS_SOLID);
	/*******************************************************************************
     * �������� : ���ƹ����ʱͼ�е�Сʱ��
	 * �������� : int x,int y,int x1,int y1- ֱ�ߵ�������
	 *			  int LineWidth , int LineStyle  - ֱ�ߵ���ʽ����
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :������
	 * ������� :2008-11-4
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual void DrawHourLine(int x,int y,int x1,int y1,int LineWidth = 1, int LineStyle = PS_SOLID);
	/********************************************************************************
	* �������� : ���ƹ����ʱͼ�еİ�Сʱ����
	 * �������� : int x,int y,int x1,int y1- ֱ�ߵ�������
	 *			  int LineWidth , int LineStyle  - ֱ�ߵ���ʽ����
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :������
	 * ������� :2008-11-4
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual void DrawHalfHourLine(int x,int y,int x1,int y1,int LineWidth = 1, int LineStyle = PS_DOT);
	/********************************************************************************
	* �������� : ���ƹ�������ͼ�е���ߴ�ֱ������������Ķ������ߣ���MA��VOL��MACD����������ĺ���
	 * �������� : int x,int y,int x1,int y1- ֱ�ߵ�������
	 *			  int LineWidth , int LineStyle  - ֱ�ߵ���ʽ����
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :������
	 * ������� :2008-11-7
	 * �޸ļ�¼ : 
	 *******************************************************************************/ 
	virtual void DrawKLineTopLine(int x,int y,int x1,int y1,int LineWidth = 1, int LineStyle = PS_DOT);  
	/********************************************************************************  qinhn
	/********************************************************************************
	 * �������� : �жϵ��Ƿ���������
	 * �������� : DWORD linetype - �߷������PKLINESTICK
	 *			  CPoint& point- ��
	 *			  CRect& rect-����
	 *			  short nCurve,void* pMask - δʹ��
	 *  ����ֵ  : �жϳɹ�����FINDED, ����CONTINUE
	 * �������� : ������IsOnLine
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual long IsSelected(DWORD linetype,CPoint& point,CRect& rect,short nCurve,void* pMask,
						    CLineProperty* pData = NULL);
	/********************************************************************************
	 * �������� : �������ز���, ����, ���, ɾ��, �ж�
	 * �������� : DWORD dwAxesStyle - ����
	 *  ����ֵ  : void, BOOL:�ɹ�����TRUE, ʧ�ܷ���FALSE
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void SetAxesStyle( DWORD dwAxesStyle)	 { m_dwAxesStyle = dwAxesStyle; }
	void AddAxesStyle( DWORD dwAxesStyle)	 { m_dwAxesStyle |= dwAxesStyle; }
	void RemoveAxesStyle( DWORD dwAxesStyle) { m_dwAxesStyle &= ~dwAxesStyle; }
	BOOL IsAxesStyle ( DWORD dwAxesStyle)	 { return (m_dwAxesStyle & dwAxesStyle); }
	/********************************************************************************
	 * �������� : ѡ��״̬����ز���, ����, ���, ɾ��, �ж�
	 * �������� : short cStyle - ѡ��״̬
	 *  ����ֵ  : void, BOOL:�ɹ�����TRUE, ʧ�ܷ���FALSE
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void SetSelectedStyle(short cStyle)	     { m_cSelectedStyle = cStyle; }
	void AddSelectedStyle(short cStyle)	     { m_cSelectedStyle = cStyle; }
	void RemoveSelectedStyle();
	BOOL IsSelectedStyle(short cStyle)		 { return (m_cSelectedStyle == cStyle); }
	BOOL IsSelectedStyle()					 { return (m_cSelectedStyle != -1); }
	/********************************************************************************
	 * �������� : ���ù����б��Լ��϶�����ָ��, ֱ��ָ�븳ֵm_ayDrawList,m_ayDrag
	 * �������� : CArray<class CHSDrawObj*, class CHSDrawObj*>* payDrawList �����б�ָ��
	 *			  CArray<class CDragObjManager*,class CDragObjManager*>* ayDrag �϶�����ָ��
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void SetRelating(CArray<class CHSDrawObj*, class CHSDrawObj*>* payDrawList,
					 CArray<class CDragObjManager*,class CDragObjManager*>* ayDrag);
	//���� ��ͼ�����б�ָ��
	CArray<class CHSDrawObj*, class CHSDrawObj*>*			   GetDrawList() { return m_ayDrawList; }
	//���� �Ϸ�Ŀ�����ָ��
	CArray<class CDragObjManager*,class CDragObjManager*>* GetDragObj()  { return m_ayDrag; }	 

	/********************************************************************************
	 * �������� : ȡ��ָ��ͼԪ���Ϸ������е�ǰһ�����ߺ�һ��ͼԪ
	 * �������� : CDrawBase* pCurObj- ͼԪָ�� , ��NULL
	 *			  BOOL bNext - ��ǰһ�����һ��
	 *  ����ֵ  : �ɹ�����ͼԪָ��, ʧ�ܷ���NULL
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	CDrawBase* GetPrevObj(CDrawBase* pCurObj,BOOL bNext);
	CDrawBase* GetPrevObjHaveShow(CDrawBase* pCurObj,BOOL bNext);
	//�����ı�ע������
	CRect  GetExplainTextRect();
	
	/********************************************************************************
	 * �������� : ����ҳ������
	 * �������� : char cType - ҳ�����������ʱ��, 1��ʾ��ͼ��, 2��ʾ������, ���ز�ͬ������ 
	 *  ����ֵ  : CRect ҳ������, ʧ�ܷ��ؿ�����
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	CRect  GetOperatorRect(char cType = 0);
	/********************************************************************************
	 * �������� : �жϵ��Ƿ���ҳ��������
	 * �������� : CPoint pointm - �� DWORD dwStyle - ҳ������ PKLINE_TYPE_TECH_LIST��
	 *  ����ֵ  : �ɹ����ض�Ӧ��ҳ������, ʧ�ܷ���-1
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	int	   IsOperatorRect(CPoint pointm,DWORD dwStyle = 0);
	/********************************************************************************
	 * �������� : ������ͼ��, Ŀǰ�Լ�ȡ��
	 * �������� : DWORD dwStyle - ҳ����ʽ
	 *  ����ֵ  : void
	 * �������� : ����ҳ��ʱ������
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void   DrawOperatorRect(DWORD dwStyle);

	//���������Ļ�������(������������K�ߵȵ�����)
	CRect  GetChartRect();
	/********************************************************************************
	 * �������� : ȡ��ҳ������, ����ҳ������
	 * �������� : nMask - ��ʾ���������ĸ�λ�õ�ҳ�߾� AXIS_LEFTMARGIN��
	 *  ����ֵ  : CRect - ����, ʧ�ܷ��ؿ�����; void
	 * �������� : �����Ȩ�и��������ҳ������
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	CRect  GetMarginRect(int nMask);
	void   DrawMarginRect(int nMask);
	//����ѡ��ע��(����)��������
	CRect  GetExplainRect() { return m_rcExplain; }
	//����ѡ��ע��(����)��չ�˵�ͼ��Ļ�������
	CRect  GetExplainMenuRect();
	//����-1, �޴���, ����:����ȥ��
	int    GetAxisRect(CPoint& pt);
	/********************************************************************************
	 * �������� : ����ע��λ��
	 * �������� : CRect rect-����
	 *			  CDrawBase* pThis ͼԪָ��
	 *			  BOOL bAuto - ������ֵ 3:ֱ�Ӹ�ֵ 2:ֱ�ӷ���, 0: ��ֵ����ӷ��AXIS_SELEXPLAINRECT
	 *						��2,3,0:ԭ������Ч��ǰ���¸�ֵ
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void   SetExplainRect( CRect rect,CDrawBase* pThis,BOOL bAuto = FALSE);
	//���ע������, ɾ�����ͼԪ���
	void   SetExplainEmpty();
	/********************************************************************************
	 * �������� : ����һ������(�����ߵ�, ��Ҫָ����������)
	 * �������� : CKLineChartParam* sChartParam - �����ṹָ�� ��NULL
	 *			  StockDay&	pValues, - ��������, ���ڻ����̼��ߵ�
	 *			  CDragObjManager* pDragObjManager, - �Ϸ�����ָ��
	 *			  CDispMaxMinValue* pCurMin- ָʾ��Сֵ��������λ�õ���Ϣ�Ľṹָ��
	 *			  CDispMaxMinValue* pCurMax- ָʾ���ֵ��������λ�õ���Ϣ�Ľṹָ��
	 *			  BOOL bUp - ���߻�������
	 *			  int nDataPos- ����λ��(���ȫ��)
	 *			  int x  - �м��ߵ�λ��
	 *			  int top - �߼�λ��
	 *			  int bottom- �ͼ�λ��
	 *			  CRect rect-������ɵ��ķ�����
	 *			  CPen* pUpPen,CBrush* pUpBrush - ���ߵ����Լ���ˢ
	 *			  CPen* pDownPen,CBrush* pDownBrush - ���ߵ����Լ���ˢ
	 *			  BOOL bSpecial - �Ƿ�Ϊ�����־ add by lxqi 20090507
	 *  ����ֵ  : �ɹ�����TRUE, ʧ�ܷ���FLASE
	 * �������� : �˺�����Ϊ���߻��Ƶ���ں���, �ɲ���ָ������, �ٷ�֧����
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	BOOL DrawPKByMark(CKLineChartParam* sChartParam,StockDay&	pValues,
					  CDragObjManager* pDragObjManager,
					  CDispMaxMinValue* pCurMin,CDispMaxMinValue* pCurMax,
					  BOOL bUp,int nDataPos,int x,int top,int bottom,CRect rect,
					  CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush,
					  BOOL bSpecial = FALSE);
	/********************************************************************************
	 * �������� : ����K��
	 * �������� : BOOL bUp - ���߻�������
	 *			  int nDataPos- ����λ��(���ȫ��)
	 *			  int x  - �м��ߵ�λ��
	 *			  int top - �߼�λ��
	 *			  int bottom- �ͼ�λ��
	 *			  CRect rect-������ɵ��ķ�����
	 *			  CPen* pUpPen,CBrush* pUpBrush - ���ߵ����Լ���ˢ
	 *			  CPen* pDownPen,CBrush* pDownBrush - ���ߵ����Լ���ˢ
	 *  ����ֵ  : void
	 * �������� : ��DrawPKByMark������
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void DrawPKline( BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					 CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	/********************************************************************************
	 * �������� : ����������
	 * �������� : BOOL bUp - ���߻�������
	 *			  int nDataPos- ����λ��(���ȫ��)
	 *			  int x  - �м��ߵ�λ��
	 *			  int top - �߼�λ��
	 *			  int bottom- �ͼ�λ��
	 *			  CRect rect-������ɵ��ķ�����
	 *			  CPen* pUpPen,CBrush* pUpBrush - ���ߵ����Լ���ˢ
	 *			  CPen* pDownPen,CBrush* pDownBrush - ���ߵ����Լ���ˢ
	 *  ����ֵ  : void
	 * �������� : ��DrawAmericaByMark������
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void DrawAmerica( BOOL& bUp,int& nX,int& nTop,int& nBottom,CRect& rect,
		CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	/********************************************************************************
	 * �������� : �������̼���DrawClose, ��ͨѹ����DrawCNDEQV ����ѹ����DrawKEQV ������DrawTowerline
	 *				OX��DrawNPline
	 * �������� : CKLineChartParam* sChartParam - �����ṹָ�� ��NULL
	 *			  StockDay&	pValues, - ��������, ���ڻ����̼��ߵ�
	 *			  CDragObjManager* pDragObjManager, - �Ϸ�����ָ��
	 *			  CDispMaxMinValue* pCurMin- ָʾ��Сֵ��������λ�õ���Ϣ�Ľṹָ��
	 *			  CDispMaxMinValue* pCurMax- ָʾ���ֵ��������λ�õ���Ϣ�Ľṹָ��
	 *			  BOOL bUp - ���߻�������
	 *			  int nDataPos- ����λ��(���ȫ��)
	 *			  int x  - �м��ߵ�λ��
	 *			  int top - �߼�λ��
	 *			  int bottom- �ͼ�λ��
	 *			  CRect rect-������ɵ��ķ�����
	 *			  CPen* pUpPen,CBrush* pUpBrush - ���ߵ����Լ���ˢ
	 *			  CPen* pDownPen,CBrush* pDownBrush - ���ߵ����Լ���ˢ
	 *  ����ֵ  : void                 
	 * �������� : ��DrawPKByMark������ 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void DrawClose(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	void DrawCNDEQV(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	void DrawEQV(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	void DrawKEQV(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	void DrawTowerline(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	void DrawNPline(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
  //ADD BY YUHJ 2008.5.14
	void DrawOXline(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
					        CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
  //END

	//����ֻ��������ʵ�� ����,����ȥ��
	BOOL DrawAmericaByMark(StockDay&	pValues,
					  CDragObjManager* pDragObjManager,
					  CDispMaxMinValue* pCurMin,CDispMaxMinValue* pCurMax,
					  BOOL bUp,int nDataPos,int x,int top,int bottom,CRect rect,
					  CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);

	/********************************************************************************
	 * �������� : �������̼�
	 * �������� : StockDay&	pValues, - ��������, ���ڻ����̼��ߵ�
	 *			  CDragObjManager* pDragObjManager, - �Ϸ�����ָ��
	 *			  CDispMaxMinValue* pCurMin- ָʾ��Сֵ��������λ�õ���Ϣ�Ľṹָ��
	 *			  CDispMaxMinValue* pCurMax- ָʾ���ֵ��������λ�õ���Ϣ�Ľṹָ��
	 *			  BOOL bUp - ���߻�������
	 *			  int nDataPos- ����λ��(���ȫ��)
	 *			  int x  - �м��ߵ�λ��
	 *			  int top - �߼�λ��
	 *			  int bottom- �ͼ�λ��
	 *			  CRect rect-������ɵ��ķ�����
	 *			  CPen* pUpPen,CBrush* pUpBrush - ���ߵ����Լ���ˢ
	 *			  CPen* pDownPen,CBrush* pDownBrush - ���ߵ����Լ���ˢ
	 *  ����ֵ  : void                 
	 * �������� : �ޱ�����
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	BOOL DrawCloseByMark(StockDay&	pValues,
					  CDragObjManager* pDragObjManager,
					  CDispMaxMinValue* pCurMin,CDispMaxMinValue* pCurMax,
					  BOOL bUp,int nDataPos,int x,int top,int bottom,CRect rect,
					  CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush);
	/********************************************************************************
	 * �������� : ������Ӧ����Ϣ, �����Ȩ��ʶ,��Ϣ���׵�
	 * �������� : int x  - Xλ��
	 *			  int y - Yλ��
	 *			  CRect rect-������ɵ��ķ�����
	 *			  StockDay&	pValues, - �������� δʹ��
	 *  ����ֵ  : void
	 * �������� : ����K�ߺ����
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void DrawPKMessageMask(int x,int y,CRect& rect,StockDay& pValues);
	/********************************************************************************
	 * �������� : ���������ͱ�ʶ
	 * �������� :  int x  - �м��ߵ�λ��
	 *			  int top - �߼�λ��
	 *			  int bottom- �ͼ�λ��
	 *			  CRect rect-������ɵ��ķ�����
	 *			  CDispMaxMinValue* pCurMin- ָʾ��Сֵ��������λ�õ���Ϣ�Ľṹָ��
	 *			  CDispMaxMinValue* pCurMax- ָʾ���ֵ��������λ�õ���Ϣ�Ľṹָ��
	 *  ����ֵ  : void
	 * �������� : ����������K�ߺ����
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void DrawMaxMin(int nXPos,int nTop,int nBottom,CRect rect,CDispMaxMinValue* pCurMin,CDispMaxMinValue* pCurMax);
	//���ش�ͼԪָ��, �൱��this
	class CDrawBase* GetPKLine();
	/********************************************************************************
	 * �������� : �ж���������������
	 * �������� : int nPosition - ����λ�� 
	 *  ����ֵ  : ��:TRUE, ��:FALSE
	 * �������� : ����K��ʹ��, ʵ���ϵ���AnalisysObjData:IsUp()���
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	BOOL  IsUp(int nPosition);
	/********************************************************************************
	 * �������� : ȡ����ĻK����������Сλ��������Ϣ�ṹָ��
	 * �������� : BOOL bMax - ȡ�������С, TRUE���, ��֮ȡ��С
	 *  ����ֵ  : CDispMaxMinValue* , ʧ�ܷ���NULL
	 * �������� : �ڻ�������ǰ����
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	CDispMaxMinValue* GetMaxMin(BOOL bMax);
	/********************************************************************************
	 * �������� :���ػ����ı�
	 * �������� : MouseTipTextInfo* pMouseTipTextInfo - ���TIP�ı��ṹָ��, ȡ���ı������浽�ṹ
	 *			  int nPosition  - δʹ�� CPoint& point - ��ǰ����
	 *  ����ֵ  : �ɹ�����TRUE, ʧ�ܷ���FALSE
	 * �������� : ����ƶ�ʱ�򴥷�
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	BOOL GetTipText(MouseTipTextInfo* pMouseTipTextInfo,int nPosition,CPoint& point);
	int DrawAdvance(DWORD& linetype,int& nCurve,ColorProperty& crSingleLineColor,
					POINT& pt1,POINT& pt2,
					HSDouble& pCurData,HSDrawOtherData* pOtherData,int& nPos,
					int nDistance);
	/********************************************************************************
	 * �������� : �ж��Ƿ�����һ��
	 * �������� : CPoint& ptFrom  CPoint& ptTo, CPoint& point  - ��ʼ��ֹ��, �м��
	 *  ����ֵ  : �ɹ�����TRUE, ʧ�ܷ���FALSE
	 * �������� : �ж�ѡ��ʱ�����
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	static BOOL IsOnLine(CPoint& ptFrom, CPoint& ptTo, CPoint& point);
	static BOOL IsOnDot(CPoint& ptFrom, CPoint& ptTo, CPoint& point);


public:
	/********************************************************************************
	 * �������� : �ж�ֵ����Ч�� �Є�����[hx_DefMinValues hx_DefMaxValues]
	 * �������� : long* lValue,...; float* lValue,... - ���ж����� ���
	 *  ����ֵ  : int ������Ч����TRUE ���򷵻�FALSE
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	int IsValue(long* lValue,...);
	int IsValue(float* lValue,...);
	int IsValue(double* lValue,...);
	/********************************************************************************
	 * �������� : ������ֵ,���ض�Ӧ����Ļ��λ��, ��������ȡ��K�߸ߵ���͵��λ��
	 * �������� : double lValue  - ��ֵ
	 *  ����ֵ  : int , ��Ļ��λ��
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	int ValueToVertPos(double lValue);

	void SetAxesType(int nAxesType);
	int	 IsAxesType(int nAxesType);

public:
	//ͼԪ������Ϣ
	static CurrentActiveStatus m_curAtiveStatus;
public:
	//�Є���������
	RECT		m_PlotRect;
	//ʹ���ڴ�˫�������
	bool		m_bMemoryDraw;
	//�Ƿ����ڴ�ӡ
	bool		m_bPrinting;
	//��ӡģʽ
	int			m_nPreMode;
	//��ӡ����
	int			m_nPrintScale;
public:
	//������ʽ �ֱ��ʾ��ͨXY, ����X, ����Y, ����XY
  //CHANGE BY YUHJ 2008.3.24
	enum   CAxesType	{ XY, XLOG, XYLOG, YDEF, YLOG, YLN, YPER, YGLD, YEQU };
  //END
protected:
	//�̶Ƚṹ �ֱ��ʾ X��̶���С��λ, ��̶���С��λ, X����Сλ��, X�����λ��,Y����Сλ��, Y�����λ��
	struct CScale		{ double dx, dy, xmin, ymin, xmax, ymax; };
	/*�ṹ	��������
	�����㷨(Rect):
	left  += xmin * WIDTH;
	top	  += ymin * HEIGHT;
	right  -= (1 - xmax) * WIDTH;
	bottom -= (1 - ymax) * HEIGHT;
	*/
	struct CRatio		{ double xmin, ymin, xmax, ymax; };
	//�ı�����ṹ �ֱ��ʾ���������ʽ, ���������ʽ
	struct StringAlign	{ DWORD HAlign, VAlign; };
	//�������ݲ����ṹ �ֱ��ʾ��Сֵ, ���ֵ, һ��λ����, ������, ��������, С��λ��; �߿�
	struct CRefParam	{ double min, max, unit, ref;int nDiv, nDecimal;int lineWidth; 
						  //�����Ƿ���Ч	
						  BOOL IsValid() {return (min != 0 || max != 0);} 
						};
	//�̶���ë�̳��Ƚṹ, �ֱ��ʾ��,��, ��,�ײ�ë�̳���
	struct CTickLineLen	{ unsigned char lm,tm,rm,bm; };	
	//�����ἰ�ı���ʾ���
	DWORD      m_dwAxesStyle;	
	//ͼ��ѡ�з��
	short	   m_cSelectedStyle;

	//Y���������ݲ����ṹ 
	CRefParam			  m_YRefParam;
	CArray<double,double> m_ayUsrDefAxis;

	//X���������ݲ����ṹ 
	CRefParam   m_XRefParam;
	//��ɫ����ָ���ָ��
	ColorProperty**	m_pBackColor;		//����
	ColorProperty**	m_pVerLineColor;	//����
	ColorProperty**	m_pHorLineColor;	//����
	ColorProperty**	m_pGridLineColor;	//������
	ColorProperty**	m_pChartTickColor;	//�̶���
	ColorProperty**	m_pChartTickTXTColor;//�̶��ı�
	ColorProperty**	m_pChartTitleColor;	//ͼ���ڱ����ı�
	//�̶���ë�̳��Ƚṹ����
	CScale		m_Scale;
	//��������
	CRatio		m_Ratio;	
	//����
	static LOGFONT	m_LogFont;
	//���ڱ������������߶�, ��ʵ��ʹ��
	SIZE		m_Size;
	//�ı�������ʽ
	StringAlign m_StrAlign;
	//��չʹ��, Ŀǰδʹ��
	int			m_nXDecimal;
	//��չʹ��, Ŀǰδʹ��
	int			m_nYDecimal;
	//ʹ�ܱ߿�
	bool		m_bEnableLegend;
	//ʹ����Ӱ
	bool		m_bLegendShadow;
	//������ ��XY��
	int			m_nAxesType;
	//��չʹ��, Ŀǰδʹ��
	int			m_nXStep;
	//��չʹ��, Ŀǰδʹ��
	int			m_nYStep;
	//�߽�����
	CRect		m_rcMargin;
	//�� �� ���� �߽���ز���
	CMargin		m_rcDefMargin;
	//��ͼ�����б�ָ�� ֱ��ָ��
	CArray<class CHSDrawObj*, class CHSDrawObj*>*			     m_ayDrawList; 
	//�Ϸ�Ŀ�����ָ�� ֱ��ָ��
	CArray<class CDragObjManager*,class CDragObjManager*>* m_ayDrag;
	//����������ָ��
	CDrawBase*	 m_pThis;
	//�Є�����
	RECT		m_Rect;
	//��������
	RECT		m_ClipBox;
	//�ͻ�����
	RECT		m_ClientRect;
	//��ͼ�豸ָ��
	CHSBaseDC*	m_pDC;
	//��չʹ��, �̳���ʹ��
	POINT		m_Pt;
	//ע������
	CRect				m_rcExplain;
	//Сͼ���б�
	static CImageList	m_MenuBitmap;
	//Сͼ���С
	static SIZE			m_MenuSize;
	//��Ӧ��,��, ��, ��,��, ��
	int m_GL, m_GR, m_GT, m_GB, m_PX, m_PY;
	//X, Y�̶�����Ŀ, X,Y�̶ȴ�λ
	int m_XTicks, m_YTicks, m_XGridTicks, m_YGridTicks;
	//�̶���ë�̳��Ƚṹ
	CTickLineLen m_TickLineLen;
	//X,Y�̶ȴ�λ����С��λ����
	int	   m_XTicksUnit,m_YTicksUnit; 

protected:	
	/********************************************************************************
	 * �������� : ���ƺ�߿��Լ��ı� 
	 * �������� : COLORREF cr- �ı���ɫint Index ƫ������ const char* Name - �ı�����
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	virtual void Legend(COLORREF cr, int Index, const char* Name);
	//����m_Rect
	void GetPixelRect(RECT& rt);
	//����Pixel, ���������ű��ʸ�ֵ
	void SetPixelRect(RECT rt);
	//���Ʊ���
	void DrawBkGround();
	/********************************************************************************
	 * �������� : ����Բ, ���ԲDrawFilledCircle
	 * �������� : int x, int y, int radius - ��, �뾶
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void DrawCircle(int x, int y, int radius);
	void DrawFilledCircle(int x, int y, int radius);
	//�����ķ����� (left,top), (right, bottom)
	void DrawRectangle(int x1, int y1, int x2, int y2);
	/********************************************************************************
	 * �������� : �����������, �����¼�ͷ��
	 * �������� : int x, int y - ԭ�� int mode  - �������� CROSS��
	 *			  int size - ƫ�Ƴ���
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void DrawMarker(int x, int y, int mode, int size = 6,CString strText = "");
	void DrawMarker( CPoint ptStart, CPoint ptEnd, ColorProperty& crColor );
	/********************************************************************************
	 * �������� : ��������ʽ��������ָ���
	 * �������� : int decimal - С��λ�� 
	 *		      CString str, �����ָ��� 
	 *			  float value ����
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void Format(int decimal, CString& str, double value);
	//�ı����������ʽ ������ , ���߿�
	enum { PrintString_Float = 0x01,PrintString_Rect = 0x02 };
	/********************************************************************************
	 * �������� : ����Ļ����ı�
	 * �������� : int x, int y - ���λ�� 
	 *			  int theta, �뾶
	 *			  const char* fmt, �ı�
     *			  UINT nFormat ����
	 *			  short sType ������ʽ
	 *			  CFont* pCurFont ����
	 *  ����ֵ  : void
	 * �������� : ����
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void PrintString(int x, int y, int theta, const char* fmt,
					 UINT nFormat = DT_SINGLELINE|DT_END_ELLIPSIS|DT_WORDBREAK,
					 short sType = PrintString_Float,
					 CFont* pCurFont = NULL);
	//ͨ��λ��ȡ��ʱ����ʾ�ָ��� ��ʽ%02d:%02d
	void GetFenShiTimeStr(int nPos,CString& strBuf,BOOL bLast = FALSE);
	void GetHighFrequencyTimeStr(int nPos,CString& strBuf,BOOL bLast = FALSE);
	//��ҳ��,���ƿ̶�
	void Ticks();
	//��������̶�
	void RightYTick();
	
public:
	/********************************************************************************
	 * �������� : ����
	 * �������� : int x1, int y1, int x2 ,int y2,- ������
	 *			  char bUser �Ƿ�ʹ���Զ���ֱ�߻���,�˻���Ϊ:ʹ�õ�,���ӳ���,
	 *			  ColorProperty* pColor ������ָ�� ���Զ��廭���Є�
	 *  ����ֵ  : void
	 * �������� : ����
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void DrawLine(int x1, int y1, int x2, int y2,char bUser = FALSE,ColorProperty* pColor = NULL);
protected:
	//����
	void DrawLine(POINT& pt1, POINT& pt2,char bUser = FALSE,ColorProperty* pColor = NULL)	{ DrawLine(pt1.x, pt1.y, pt2.x, pt2.y,bUser,pColor); }
	//��Բ
	void DrawCircle(POINT& pt, int radius)					{ DrawCircle(pt.x, pt.y, radius); }
	//���Բ
	void DrawFilledCircle(POINT& pt, int radius)			{ DrawFilledCircle(pt.x, pt.y, radius); }
	//������
	void DrawRectangle(POINT& pt1, POINT& pt2)				{ DrawRectangle(pt1.x, pt1.y, pt2.x, pt2.y); }
	//���������
	void DrawMarker(POINT& pt, int mode, int size = 6)		{ DrawMarker(pt.x, pt.y, mode, size); }
	//���ָ���
	void PrintString(POINT& pt, int theta, const char* fmt)	{ PrintString(pt.x, pt.y, theta, fmt); }
  //��������
  //ADD BY YUHJ 2008.5.14
  void DrawCross(POINT &pt, int radius);
  //END
protected:
	//�����ֶ�Ӧ�����С,�˴��������ַ���
	enum    FontSizeType { ftTitle,ftXAxisTitle,ftYAxisTitle,ftLegend,
						   ftXAxis,ftYAxis,ftFenShiYAxis,ftAxes,
						   ftRightYTick,ftFenShiRightYTick,ftMulti,
						   ftDrawVarTitle,ftDrawMaxMin };
	//�������ַ������������С
	int		GetFontSize(char cType);
	//�������ַ���ȡ������
	CFont*	GetFont(char cType);
	//���ڱ���ǰһ�������߽�������
	int		m_nPreLeftMaxMargin;
	int		m_nPreRightMaxMargin;
public:
	/********************************************************************************
	 * �������� : ������������ı������, �ұ������ı������GetYRightMaxWidth
	 * �������� : CHSBaseDC* pDC -  ��ͼ�豸ָ�� ��NULL
	 *			  BOOL bOneCharWidth - �Ƿ�����ַ����
	 *  ����ֵ  : �����
	 * �������� : ��CalcLeftRightWidth����
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	int		GetYLeftMaxWidth(CHSBaseDC* pDC,BOOL bOneCharWidth = FALSE);
	int		GetYRightMaxWidth(CHSBaseDC* pDC);
	/********************************************************************************
	 * �������� : ���µ������ұ߽�����
	 * �������� : CHSBaseDC* pDC -  ��ͼ�豸ָ�� ��NULL
	 *  ����ֵ  : void
	 * �������� : ������GetYLeftMaxWidth,GetYRightMaxWidth
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void    CalcLeftRightWidth(CHSBaseDC* pDC);
	/********************************************************************************
	 * �������� : �������ݷ�����Ӧ���������ָ������� ����"��10"
	 * �������� : int nValue - ������
	 *  ����ֵ  : CString , �������ָ�������,ʧ�ܷ���"Over"
	 * �������� : ������������ͨ�����д����ı�����
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	CString ConvertIntToStr(int nValue);
	/********************************************************************************
	 * �������� : ˢ��, �ڴ��ຯ�������µ�����Size, δ�ػ�
	 * �������� : CDC* pDC  - ��ͼ�豸ָ�� WPARAM wp = 0,LPARAM lp = 0 - ����
	 *  ����ֵ  : void
	 * �������� : ������ɫ���øı�ʱ��ˢ��
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void	Refresh(CDC* pDC,WPARAM wp = 0,LPARAM lp = 0);
	//��Ӧ����ı���Ӧ������ı�
	void	SetSizeByFont(CDC* pDC);
public:
	// ��������Ⱥ
	virtual void DrawStars(DWORD linetype,
		CTValues<DrawPointInfo> pRefX,
		CTValues<StarFieldItem> pRefValues,
		ColorProperty& clLine,
		COLORREF cr = 0, int Index = 0, const char* Name = NULL);
public:
	//���л�
	virtual void	Serialize(CArchive& ar);
public:
	//	�ɱ��ֲ�����ؽṹ
	struct MoveCostParam
	{
		char		  cPos;		//��������
		COLORREF	  m_lColor; //��ɫ
		//���ݷ�������, ������ɫ
		void SetColor(BYTE r,BYTE g,BYTE b);
	};
	//�ɱ��ֲ���ɫ��ز���
	struct MoveCostParamArray
	{
		ColorProperty lColor;							//��ɫ
		CArray<MoveCostParam*,MoveCostParam*> ayParam;	//������ɫ
		~MoveCostParamArray()
		{
			for(int i = 0; i < ayParam.GetSize(); i++)
			{
				delete ayParam.GetAt(i);
			}
		}
	};
	//�ɱ��ֱ���Ҫʹ�õ������ֵ
	struct MoveCostDetail
	{
		int		nData;				//���ڣ��ɱ��ֲ�����
		float	fInterestRatio;		//������ �ٷֱ�С��ֵ, ������ӦΪ:1-fInterestRatio
		float	fInterestPan;		//ָʾ������λλ��(�±�)
		float	fEvenCost;			//Ԥ��
		float	fPercent90Up;		//Ԥ��
		float	fPercent90Down;		//Ԥ��
		float	fPercent90Focus;	//Ԥ��	
		float	fPercent70Up;		//Ԥ��
		float	fPercent70Down;		//Ԥ��
		float	fPercent70Focus;	//Ԥ��	
		float   fHYS[MOVECOSTCOUNT];	//����ɽ�ɱ�ռ�ܳɱ��ı�ֵ
		float	fHYD[MOVECOSTCOUNT];    //��Ծ�ȳɱ�ռ�ܳɱ��ı�ֵ

		MoveCostDetail()
		{
			nData = 0;
			fInterestRatio = 0;
			fInterestPan = 0;
			fEvenCost = 0;
			fPercent90Up = 0;
			fPercent90Down = 0;
			fPercent90Focus = 0;
			fPercent70Up = 0;
			fPercent70Down = 0;
			fPercent70Focus = 0;

			for( int i = 0; i < MOVECOSTCOUNT; i++ )
			{
				fHYS[i] = 0;
				fHYD[i] = 0;
			}
		}
		/*������ȫ������ 
		why not use : memset(this, 0, sizeof(this));? by xiongzb 
		*/
		void DeleteAll()
		{
			nData = 0;
			fInterestRatio = 0;
			fInterestPan = 0;
			fEvenCost = 0;
			fPercent90Up = 0;
			fPercent90Down = 0;
			fPercent90Focus = 0;
			fPercent70Up = 0;
			fPercent70Down = 0;
			fPercent70Focus = 0;

			for( int i = 0; i < MOVECOSTCOUNT; i++ )
			{
				fHYS[i] = 0;
				fHYD[i] = 0;
			}
		}
	};
	//
	struct HxhMoveCostData
	{
		HxhMoveCostData();
		int	  m_nCalcType;					//���ڼ�¼�����ǻ���ƽ��:1�����ǣ�2��ƽ��
		float m_fReduce;					//���ڼ�¼��ʷ˥��ϵ��
		float m_fFocus;						//���ж�
		BOOL  m_bRefine;					//��ϸ����
		float m_fDefaultHand;				//û����ͨ���ǵĵĻ�����
		int   m_ayMoveCost[MOVECOSTCOUNT];  //�ƶ��ɱ�����ʾ���
		int   m_ayHouYanShan[MOVECOSTCOUNT]; //����ɽ����ʾ���
		int   m_ayHuoYueDu[MOVECOSTCOUNT];	 //��Ծ�ȵ���ʾ���
		int	  m_nHYSCount;					 //����ɽ����
		int   m_nHYDCount;					 //��Ծ�ȸ���

		static int Load();	//�������ļ�����
		static int Save();	//���浽�����ļ�
	};
	/********************************************************************************
	 * �������� : ���Ƴɱ����Ƶ�
	 * �������� : CGraphics::MoveCostConfig& moveCost  - ����
	 *		      MoveCostParamArray& ayParam, - ��������
	 *			  CRect& clipBox, - ��������
	 *			  AnalisysObjData* pAnalisysData, ��������ָ��
	 *			  int nCurPos - ��ǰ����λ��
	 *  ����ֵ  : �ɹ�����1, ʧ�ܷ���0
	 * �������� : ��ʼ����
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	int  PaintMoveCost(CGraphics::MoveCostConfig& moveCost,MoveCostParamArray& ayParam, 
		CRect& clipBox,AnalisysObjData* pAnalisysData,int nCurPos);
	/********************************************************************************
	 * �������� : ˢ�³ɱ��ֲ���
	 * �������� : AnalisysObjData* pAnalisysData - ��������ָ��
	 *			 ,CGraphics::MoveCostConfig& moveCost, ����
	 *			  int nStart, ������ʼλ�� 
	 *			  int nEnd, ������ֹλ��
	 *			  int nCurPos - ��ǰ����λ��
	 *  ����ֵ  : �ɹ�����1, ʧ�ܷ���0
	 * �������� : ˢ�»���
	 * ��    �� :����ʯ
	 * ������� :2003-4-23
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	int  CalcMoveCost( AnalisysObjData* pAnalisysData,CGraphics::MoveCostConfig& moveCost,
					  int nStart, int nEnd, int nCurPos );

	// �Ƿ�Ϊ��ͼ�����������Ƿ����K�ߣ������ټӣ�
	BOOL IsMainChart();

public:
	virtual POINT GetPoint(double x, double y);
	POINT GetPoint_LogY(double x, double y);
};
/********************************************************************************
 * �������� : ���óɱ��ֲ��Ʋ���
 * �������� : void
 *  ����ֵ  : void
 * �������� : ��Ӧ:���öԻ���OK, ϵͳ���øı䵼�µ�ˢ��
 * ��    �� :����ʯ
 * ������� :2003-4-23
 * �޸ļ�¼ : 
 *******************************************************************************/
extern void SetMoveCostParam();
/********************************************************************************
 * �������� : ���ع������ݽṹָ��
 * �������� : void
 *  ����ֵ  : CGraphics::HxhMoveCostData* 
 * �������� : 
 * ��    �� :����ʯ
 * ������� :2003-4-23
 * �޸ļ�¼ : 
 *******************************************************************************/
extern CGraphics::HxhMoveCostData* GetHxhMoveData();

