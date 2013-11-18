
#include "stdafx.h"
#include "systemfun.h"
#include "express.h"
#include "math.h"
#include "RegMsg.h"
#include "FormulaManPublic.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "HSDataSource.h"
#pragma warning( disable : 4244) 

CHSDataSource*							 g_pHxDataSource = NULL;    // �������ݰ�װ
CArray<ColorProperty*,ColorProperty*>*	 g_pColours = NULL;			// ����ָ��ȱʡ��ɫ

CSystemFun* g_pSystemFun = new CSystemFun;
//
// #ifdef _AFXDLL
// HWND  g_hParentWnd = NULL;
// #else
// extern HWND  g_hParentWnd;
// #endif

//static char m_bIntSysFun = 0;
//static CMap<int,int, COMPILEFUNCTION,COMPILEFUNCTION > m_pSysFunList;
//static void intSysFun()
//{
//	if( m_bIntSysFun )
//		return;
//
//	m_bIntSysFun = 1;
//
//	// ���麯��
//	m_pSysFunList.SetAt(FunOper_ADVANCE,g_pSystemFun->ADVANCE);    // ����: ADVANCE(var1) ���Ǽ���
//
//	m_pSysFunList.SetAt(FunOper_AMOUNT,g_pSystemFun->AMOUNT);    // ����: AMOUNT() �ɽ���
//
//	m_pSysFunList.SetAt(FunOper_AMOUNT,g_pSystemFun->AMOUNT);    // ����: AMOUNT() �ɽ���
//
//	m_pSysFunList.SetAt(FunOper_ASKPRICE,g_pSystemFun->ASKPRICE);    // ����: ASKPRICE(var1) ί���� 1-3
//
//	m_pSysFunList.SetAt(FunOper_ASKVOL,g_pSystemFun->ASKVOL);    // ����: ASKVOL(var1) ί���� 1-3
//	
//	m_pSysFunList.SetAt(FunOper_BIDPRICE,g_pSystemFun->BIDPRICE);    // ����: BIDPRICE(var1) ί��� 1-3
//	
//	m_pSysFunList.SetAt(FunOper_BIDVOL,g_pSystemFun->BIDVOL);    // ����: BIDVOL(var1) ί���� 1-3
//	
//	m_pSysFunList.SetAt(FunOper_BUYVOL,g_pSystemFun->BUYVOL);    // ����: BUYVOL() ��������
//	
//	m_pSysFunList.SetAt(FunOper_CLOSE,g_pSystemFun->CLOSE);    // ����: CLOSE() ���̼�
//	
//	m_pSysFunList.SetAt(FunOper_CLOSE,g_pSystemFun->CLOSE);		   // ����: CLOSE() ���̼�
//	
//	m_pSysFunList.SetAt(FunOper_CLOSE,g_pSystemFun->CLOSE);		   // ����: CLOSE() ���̼�
//	
//	m_pSysFunList.SetAt(FunOper_DECLINE,g_pSystemFun->DECLINE);    // ����: DECLINE() �µ�����
//	
//	m_pSysFunList.SetAt(FunOper_EXTDATA,g_pSystemFun->EXTDATA);    // ����: EXTDATA(var1) ��չ����1-11
//	
//	m_pSysFunList.SetAt(FunOper_HIGH,g_pSystemFun->HIGH);    // ����: HIGH() ��߼�
//	
//	m_pSysFunList.SetAt(FunOper_HIGH,g_pSystemFun->HIGH);	    // ����: HIGH() ��߼�
//	
//	m_pSysFunList.SetAt(FunOper_HIGH,g_pSystemFun->HIGH);	    // ����: HIGH() ��߼�
//	
//	m_pSysFunList.SetAt(FunOper_ISBUYORDER,g_pSystemFun->ISBUYORDER);    // ����: ISBUYORDER() �Ƿ���������
//	
//	m_pSysFunList.SetAt(FunOper_LOW,g_pSystemFun->LOW);     // ����: LOW() ��ͼ�
//	
//	m_pSysFunList.SetAt(FunOper_LOW,g_pSystemFun->LOW);	   // ����: LOW() ��ͼ�
//	
//	m_pSysFunList.SetAt(FunOper_LOW,g_pSystemFun->LOW);	   // ����: LOW() ��ͼ�
//	
//	m_pSysFunList.SetAt(FunOper_OPEN,g_pSystemFun->OPEN);   // ����: OPEN() ���̼�
//	 
//	m_pSysFunList.SetAt(FunOper_OPEN,g_pSystemFun->OPEN);	   // ����: OPEN() ���̼�
//	
//	m_pSysFunList.SetAt(FunOper_OPEN,g_pSystemFun->OPEN);	   // ����: OPEN() ���̼�
//	
//	m_pSysFunList.SetAt(FunOper_SELLVOL,g_pSystemFun->SELLVOL);    // ����: SELLVOL() ����������
//	
//	m_pSysFunList.SetAt(FunOper_VOL,g_pSystemFun->VOL);     // ����: VOL() �ɽ���
//	
//	m_pSysFunList.SetAt(FunOper_VOL,g_pSystemFun->VOL);       // ����: VOL() �ɽ���
//	
//	m_pSysFunList.SetAt(FunOper_VOL,g_pSystemFun->VOL);       // ����: VOL() �ɽ���
//	
//	
//	// ʱ�亯��
//	m_pSysFunList.SetAt(FunOper_DATE,g_pSystemFun->hxDATE);    // ����: DATE() ������
//	
//	m_pSysFunList.SetAt(FunOper_PERIOD_TYPE_DAY,g_pSystemFun->hxDAY);    // ����: PERIOD_TYPE_DAY() ����
//	
//	m_pSysFunList.SetAt(FunOper_HOUR,g_pSystemFun->HSOUR);    // ����: HOUR() Сʱ
//	
//	m_pSysFunList.SetAt(FunOper_MINUTE,g_pSystemFun->hxMINUTE);    // ����: MINUTE() ����
//	
//	m_pSysFunList.SetAt(FunOper_MONTH,g_pSystemFun->hxMONTH);    // ����: MONTH() �·�
//	
//	m_pSysFunList.SetAt(FunOper_TIME,g_pSystemFun->hxTIME);    // ����:  TIME() ʱ����
//	
//	m_pSysFunList.SetAt(FunOper_WEEKDAY,g_pSystemFun->hxWEEKDAY);    // ����: WEEKDAY() ����
//	
//	m_pSysFunList.SetAt(FunOper_YEAR,g_pSystemFun->hxYEAR);    // ����: YEAR() ���
//	
//	
//	//���ú���
//	m_pSysFunList.SetAt(FunOper_BACKSET,g_pSystemFun->BACKSET);    // ����: BACKSET(var1,var2) ��ǰ��ֵ
//	
//	m_pSysFunList.SetAt(FunOper_BARSCOUNT,g_pSystemFun->BARSCOUNT);    // ����:  BARSCOUNT(var1) ��Ч������
//	
//	m_pSysFunList.SetAt(FunOper_BARSLAST,g_pSystemFun->BARSLAST);    // ����: BARSLAST(var1) ��һ����������λ��
//	
//	m_pSysFunList.SetAt(FunOper_BARSSINCE,g_pSystemFun->BARSSINCE);    // ����: BARSSINCE(var1) ��һ����������λ��
//	
//	m_pSysFunList.SetAt(FunOper_COUNT,g_pSystemFun->COUNT);    // ����: COUNT(var1,var2) ͳ������
//	
//	m_pSysFunList.SetAt(FunOper_DMA,g_pSystemFun->DMA);    // ����:  DMA(var1,var2) ��̬�ƶ�ƽ��
//	
//	m_pSysFunList.SetAt(FunOper_EMA,g_pSystemFun->EMA);    // ����: EMA(var1,var2) ָ��ƽ���ƶ�ƽ��
//	
//	m_pSysFunList.SetAt(FunOper_HHV,g_pSystemFun->HHV);    // ����: HHV(var1,var2) ���ֵ
//	
//	m_pSysFunList.SetAt(FunOper_HHVBARS,g_pSystemFun->HHVBARS);    // ����: HHVBARS(var1,var2) ��һ�ߵ㵽��ǰ��������
//	
//	m_pSysFunList.SetAt(FunOper_LLV,g_pSystemFun->LLV);    // ����:  LLV(var1,var2) ���ֵ
//	
//	m_pSysFunList.SetAt(FunOper_LLVBARS,g_pSystemFun->LLVBARS);    // ����: LLVBARS(var1,var2) ��һ�͵㵽��ǰ��������
//	
//	m_pSysFunList.SetAt(FunOper_MA,g_pSystemFun->MA);    // ����: MA(var1,var2) ���ƶ�ƽ��
//	
//	m_pSysFunList.SetAt(FunOper_REF,g_pSystemFun->REF);    // ����: REF(var1,var2) ������������ǰ������
//	
//	m_pSysFunList.SetAt(FunOper_SMA,g_pSystemFun->SMA);    // ����:  SMA(var1,var2,var3) �ƶ�ƽ��
//	
//	m_pSysFunList.SetAt(FunOper_SUM,g_pSystemFun->SUM);    // ����: SUM(var1,var2) �ܺ�
//	
//	m_pSysFunList.SetAt(FunOper_SUMBARS,g_pSystemFun->SUMBARS);    // ����: SUMBARS(var1,var2) ��ǰ�ۼӵ�ָ��ֵ�����ڵ�������
//	
//	
//	// �߼�����
//	m_pSysFunList.SetAt(FunOper_IF,g_pSystemFun->IF);    // ����: IF(var1,var2,var3) ����������ͬ��ֵ
//	
//	m_pSysFunList.SetAt(FunOper_ISDOWN,g_pSystemFun->ISDOWN);    // ����: ISDOWN() �������Ƿ�����
//	
//	m_pSysFunList.SetAt(FunOper_ISEQUAL,g_pSystemFun->ISEQUAL);    // ����:  ISEQUAL() �������Ƿ�ƽ��
//	
//	m_pSysFunList.SetAt(FunOper_ISUP,g_pSystemFun->ISUP);    // ����: ISUP() �������Ƿ�����
//	
//	
//	// ��������
//	m_pSysFunList.SetAt(FunOper_ABS,g_pSystemFun->ABS);    // ����: ABS(var1) �����ֵ
//	
//	m_pSysFunList.SetAt(FunOper_CROSS,g_pSystemFun->CROSS);    // ����: CROSS(var1,var2) �����߽���
//	
//	m_pSysFunList.SetAt(FunOper_MAX,g_pSystemFun->MAX);    // ����:  MAX(var1,var2) �����ֵ
//	
//	m_pSysFunList.SetAt(FunOper_MIN,g_pSystemFun->MIN);    // ����: MIN(var1,var2) ����Сֵ
//	
//	m_pSysFunList.SetAt(FunOper_MOD,g_pSystemFun->MOD);    // ����: MOD(var1,var2) ��ģ����
//	
//	m_pSysFunList.SetAt(FunOper_NOT,g_pSystemFun->NOT);    // ����: NOT(var1) ���߼���
//	
//	m_pSysFunList.SetAt(FunOper_REVERSE,g_pSystemFun->REVERSE);    // ����:  REVERSE(var1) ���෴��
//	
//	m_pSysFunList.SetAt(FunOper_SGN,g_pSystemFun->SGN);    // ����: SGN(var1) �����ֵ
//	
//	
//	// ��ѧ����
//	m_pSysFunList.SetAt(FunOper_ACOS,g_pSystemFun->ACOS);    // ����: ACOS(var1) ������ֵ
//	
//	m_pSysFunList.SetAt(FunOper_ASIN,g_pSystemFun->ASIN);    // ����: ASIN(var1) ������ֵ
//	
//	m_pSysFunList.SetAt(FunOper_ATAN,g_pSystemFun->ATAN);    // ����:  ATAN(var1) ������ֵ
//	
//	m_pSysFunList.SetAt(FunOper_COS,g_pSystemFun->COS);    // ����: COS(var1) ����ֵ
//	
//	m_pSysFunList.SetAt(FunOper_EXP,g_pSystemFun->EXP);    // ����: EXP(var1) ָ��
//	
//	m_pSysFunList.SetAt(FunOper_LN,g_pSystemFun->LN);    // ����: LN(var1) ����Ȼ����
//	
//	m_pSysFunList.SetAt(FunOper_LOG,g_pSystemFun->LOG);    // ����:  LOG(var1) ��10Ϊ�׵Ķ���
//	
//	m_pSysFunList.SetAt(FunOper_POW,g_pSystemFun->POW);    // ����: POW(var1,var2) ����
//	
//	m_pSysFunList.SetAt(FunOper_SIN,g_pSystemFun->SIN);    // ����: SIN(var1) ����ֵ
//	
//	m_pSysFunList.SetAt(FunOper_SQRT,g_pSystemFun->SQRT);    // ����: SQRT(var1) ��ƽ��
//	
//	m_pSysFunList.SetAt(FunOper_TAN,g_pSystemFun->TAN);    // ����:  TAN(var1) ����ֵ
//	
//	
//	// ͳ�ƺ���
//	m_pSysFunList.SetAt(FunOper_AVEDEV,g_pSystemFun->AVEDEV);    // ����: AVEDEV(var1,var2) ƽ������ƫ��
//	
//	m_pSysFunList.SetAt(FunOper_DEVSQ,g_pSystemFun->DEVSQ);    // ����: DEVSQ(var1,var2) ����ƫ��ƽ����
//	
//	m_pSysFunList.SetAt(FunOper_FORCAST,g_pSystemFun->FORCAST);    // ����: FORCAST(var1,var2) ���Իع�Ԥ��ֵ
//	
//	m_pSysFunList.SetAt(FunOper_SLOPE,g_pSystemFun->SLOPE);    // ����:  SLOPE(var1,var2) ���Իع�б��
//	
//	m_pSysFunList.SetAt(FunOper_STD,g_pSystemFun->STD);    // ����: STD(var1,var2) �����׼��
//	
//	m_pSysFunList.SetAt(FunOper_STDP,g_pSystemFun->STDP);    // ����: STDP(var1,var2) �����׼��
//	
//	m_pSysFunList.SetAt(FunOper_VAR,g_pSystemFun->VAR);    // ����: VAR(var1,var2) ������������
//	
//	m_pSysFunList.SetAt(FunOper_VARP,g_pSystemFun->VARP);    // ����:  VARP(var1,var2) ������������
//	
//	
//	// ָ������
//	m_pSysFunList.SetAt(FunOper_COST,g_pSystemFun->COST);    // ����: COST(var1) �ɱ��ֲ����
//	
//	m_pSysFunList.SetAt(FunOper_PEAK,g_pSystemFun->PEAK);    // ����: PEAK(var1,var2,var3) ǰM��ZIGת�򲨷�ֵ
//	
//	m_pSysFunList.SetAt(FunOper_PEAKBARS,g_pSystemFun->PEAKBARS);    // ����: PEAKBARS(var1,var2,var3) ǰM��ZIGת�򲨷嵽��ǰ����
//	
//	m_pSysFunList.SetAt(FunOper_SAR,g_pSystemFun->SAR);    // ����:  SAR(var1,var2,var3) ����ת��
//	
//	m_pSysFunList.SetAt(FunOper_SARTURN,g_pSystemFun->SARTURN);    // ����: SARTURN(var1,var2,var3) ����ת���
//	
//	m_pSysFunList.SetAt(FunOper_TROUGH,g_pSystemFun->TROUGH);    // ����: TROUGH(var1,var2,var3) ǰM��ZIGת�򲨹�ֵ
//	
//	m_pSysFunList.SetAt(FunOper_TROUGHBARS,g_pSystemFun->TROUGHBARS); // ����: TROUGHBARS(var1,var2,var3) ǰM��ZIGת�򲨹ȵ���ǰ����
//	
//	m_pSysFunList.SetAt(FunOper_WINNER,g_pSystemFun->WINNER);    // ����:  WINNER(var1) �����̱���
//	
//	m_pSysFunList.SetAt(FunOper_ZIG,g_pSystemFun->ZIG);    // ����:  ZIG(var1,var2) ֮��ת��
//	
//	
//	// ���̺���
//	m_pSysFunList.SetAt(FunOper_INDEXA,g_pSystemFun->INDEXA);    // ����: INDEXA() ��Ӧ���̳ɽ���
//	
//	m_pSysFunList.SetAt(FunOper_INDEXADV,g_pSystemFun->INDEXADV);    // ����: INDEXADV() ��Ӧ�������Ǽ���
//	
//	m_pSysFunList.SetAt(FunOper_INDEXC,g_pSystemFun->INDEXC);    // ����:  INDEXC() ��Ӧ�������̼�
//	
//	m_pSysFunList.SetAt(FunOper_INDEXDEC,g_pSystemFun->INDEXDEC);    // ����:  INDEXDEC() ��Ӧ�����µ�����
//	
//	m_pSysFunList.SetAt(FunOper_INDEXH,g_pSystemFun->INDEXH);    // ����: INDEXH() ��Ӧ������߼�
//	
//	m_pSysFunList.SetAt(FunOper_INDEXL,g_pSystemFun->INDEXL);    // ����: INDEXL() ��Ӧ������ͼ�
//	
//	m_pSysFunList.SetAt(FunOper_INDEXO,g_pSystemFun->INDEXO);    // ����:  INDEXO() ��Ӧ���̿��̼�
//	
//	m_pSysFunList.SetAt(FunOper_INDEXV,g_pSystemFun->INDEXV);    // ����:  INDEXV() ��Ӧ���̳ɽ���
//	
//	
//	// ���ú���
//	m_pSysFunList.SetAt(FunOper_CAPITAL,g_pSystemFun->CAPITAL);    // ����: CAPITAL() ��ͨ�̴�С
//	
//	m_pSysFunList.SetAt(FunOper_VOLUNIT,g_pSystemFun->VOLUNIT);    // ����: VOLUNIT() ÿ�ֹ���
//	
//	m_pSysFunList.SetAt(FunOper_DYNAINFO,g_pSystemFun->DYNAINFO);    // ����:  DYNAINFO(3) ��̬���麯��: ����
//	
//	m_pSysFunList.SetAt(FunOper_FINANCE,g_pSystemFun->FINANCE);    // ����:  FINANCE(1) ����ָ��: �ܹɱ�(���)
//		
//	
//	// ��������
//	      // �������ͳ���
//	m_pSysFunList.SetAt(FunOper_STICK,g_pSystemFun->STICK);  // ��״�ߣ���0��ָ��ֵ�仭��ֱֱ��
//	
//	m_pSysFunList.SetAt(FunOper_COLORSTICK,g_pSystemFun->COLORSTICK);  // ��ɫ��״�ߣ���ֵΪ0����ʾ��ɫ��������ʾ��ɫ
//	
//	m_pSysFunList.SetAt(FunOper_VOLSTICK,g_pSystemFun->VOLSTICK);  // �ɽ�����״�ߣ����ɼ�����ʱ��ʾ��ɫ��������������ʾ��ɫ������
//	
//	m_pSysFunList.SetAt(FunOper_LINESTICK,g_pSystemFun->LINESTICK);  // ͬʱ������״�ߺ�ָ����
//	
//	m_pSysFunList.SetAt(FunOper_CROSSDOT,g_pSystemFun->CROSSDOT);  // С����
//	
//	m_pSysFunList.SetAt(FunOper_CIRCLEDOT,g_pSystemFun->CIRCLEDOT);  // СԲȦ��
//	
//	m_pSysFunList.SetAt(FunOper_POINTDOT,g_pSystemFun->POINTDOT);  // СԲ����
//	
//	m_pSysFunList.SetAt(FunOper_DRAW3D,g_pSystemFun->DRAW3D);    // ����:  FINANCE(1) ����ָ��: �ܹɱ�(���)
//	
//
//	// ���K����ɫ
//	m_pSysFunList.SetAt(FunOper_COLORRED,g_pSystemFun->COLORRED);  // ��ɫ
//	
//	m_pSysFunList.SetAt(FunOper_COLORGREEN,g_pSystemFun->COLORGREEN);  // ��ɫ
//	
//	m_pSysFunList.SetAt(FunOper_COLORBLUE,g_pSystemFun->COLORBLUE);  // ��ɫ
//	
//	m_pSysFunList.SetAt(FunOper_COLORBLACK,g_pSystemFun->COLORBLACK);  // ��ɫ
//	
//	m_pSysFunList.SetAt(FunOper_COLORWHITE,g_pSystemFun->COLORWHITE);  // ��ɫ
//	
//	m_pSysFunList.SetAt(FunOper_COLORGRAY,g_pSystemFun->COLORGRAY);  // ��ɫ
//	
//	m_pSysFunList.SetAt(FunOper_COLORYELLOW,g_pSystemFun->COLORYELLOW);  // ��ɫ
//	
//	m_pSysFunList.SetAt(FunOper_COLORCYAN,g_pSystemFun->COLORCYAN);  // ��ɫ
//	
//	m_pSysFunList.SetAt(FunOper_COLORMAGENTA,g_pSystemFun->COLORMAGENTA);  // Ʒ��ɫ
//	
//	m_pSysFunList.SetAt(FunOper_COLORBROWN,g_pSystemFun->COLORBROWN);  // ��ɫ
//	
//
//	m_pSysFunList.SetAt(FunOper_COLORBRGB,g_pSystemFun->hxRGB);  // RGB����
//	
//
//	// �߷����
//	m_pSysFunList.SetAt(FunOper_SOLID,g_pSystemFun->hxSOLID);
//	
//	m_pSysFunList.SetAt(FunOper_DASH,g_pSystemFun->hxDASH);		     /* -------  */
//	
//	m_pSysFunList.SetAt(FunOper_DOT,g_pSystemFun->hxDOT);			     /* .......  */
//	
//	m_pSysFunList.SetAt(FunOper_DASHDOT,g_pSystemFun->hxDASHDOT);		 /* _._._._  */
//	
//	m_pSysFunList.SetAt(FunOper_DASHDOTDOT,g_pSystemFun->hxDASHDOTDOT); /* _.._.._  */
//	
//
//	// �û���ͼ����
//	m_pSysFunList.SetAt(FunOper_DRAWICON,g_pSystemFun->DRAWICON);
//	
//	m_pSysFunList.SetAt(FunOper_DRAWLINE,g_pSystemFun->DRAWLINE);
//	
//	m_pSysFunList.SetAt(FunOper_DRAWTEXT,g_pSystemFun->DRAWTEXT);
//	
//	m_pSysFunList.SetAt(FunOper_POLYLINE,g_pSystemFun->POLYLINE);
//	
//	m_pSysFunList.SetAt(FunOper_STICKLINE,g_pSystemFun->STICKLINE);
//}


//




struct _Auto_Delete_SystemFun_
{
	~_Auto_Delete_SystemFun_()
	{
		if(g_pSystemFun != NULL)
		{
			delete g_pSystemFun;
			g_pSystemFun = NULL;
		}
	}
};
static _Auto_Delete_SystemFun_ _auto_delete_systemfun_;
static CHSDataSource* GetData(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( pCurDataStatus->m_pDataSource )
	{
		return pCurDataStatus->m_pDataSource;
	}
	else if( pCompile )
	{
		if( pCompile->GetData() != NULL )
			return pCompile->GetData();
	}
	return g_pHxDataSource;
}


CSystemFun* CSystemFun::GetSystemFunPtr(CHSDataSource* pHxDataSource)
{
	g_pHxDataSource = pHxDataSource;
	return g_pSystemFun;
}

CHSDataSource* CSystemFun::SetDataSource(CHSDataSource* pHxDataSource)
{
	CHSDataSource* pOldHxDataSource = g_pHxDataSource;
	g_pHxDataSource = pHxDataSource;
	return pOldHxDataSource;
}

int FxjUserDefCoordinate::GetOffset()
{
	int i;
	for( i = 0; i < 7; i++ )
	{
		if( m_fPoint[i] && m_fPoint[i] < 0.0000001)
			return 32;
	}
	int nValid = 0;
	for(i = 0; i < 7; i++)
	{
		if( m_fPoint[i] )
		{
			nValid++;
		}
	}
	if(nValid != m_nCount)
		return 1;

	return 0;
}

BOOL FxjEidolon::IsValid()
{
	long* p = (long*)m_strUnknown;
	if(p && *p == 9)
	{
		return TRUE;
		/*
		p++;
		if(*p == 0x06060606)
		{
			if(m_strUnknown[8] == 0x04)
			{
				return TRUE;
			}
		}
		*/
	}
	return FALSE;
}
#if 0
static int SymbolValue(CHSDataSource* pData,__int64 handle)
{
	if (!pData )
	{
		return 0;
	}
	int count = pData->GetDaySize();
	HSDouble* pCloseData = pData->GetData(hx_CLOSE_DATA);
	HSDouble* pOpenData = pData->GetData(hx_OPEN_DATA);
	HSDouble* pHighData = pData->GetData(hx_HIGH_DATA);
	HSDouble* pLowData = pData->GetData(hx_LOW_DATA);
	//HSDouble* pVolData = pData->GetData(hx_VOL_DATA);
	//HSDouble* pDateData = pData->GetData(hx_DATE_DATA);

	VARIANT_BOOL bRet;
	g_pSystemFun->m_iATIndicator->SetSymbolSize(handle, count);
	for (int i = 0; i < count; i++)
	{
		g_pSystemFun->m_iATIndicator->SetSymbolValue(handle, i, float(pOpenData[i].m_dData), float(pHighData[i].m_dData), float(pLowData[i].m_dData), float(pCloseData[i].m_dData), &bRet);
	}
	return count;
}
#endif
CSystemFun::CSystemFun()
{
	/*m_bLoadCom = FALSE;*/
}

CSystemFun::CSystemFun(CHSDataSource* pHxDataSource)
{
	g_pHxDataSource = pHxDataSource;
	/*m_bLoadCom = FALSE;*/
}

CSystemFun::~CSystemFun()
{

}
#if 0
HSDouble CSystemFun::TLOW (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					   HSCurDataInfoStatus* pCurDataStatus, int& nError)   // ����: 
{
	if(g_pSystemFun && g_pSystemFun->m_bLoadCom)
	{
		CHSDataSource* pData = NULL;
		pData = GetData(pCompile,ayVars,pCurDataStatus,nError);
		if (!pData)
		{
			return hx_DefMinValues;
		}	
		BSTR tecName;
		g_pSystemFun->m_iATIndicator->GetObjName(0, &tecName);//��ù�ʽ����
		__int64 hHandle;
		g_pSystemFun->m_iATIndicator->CreateObj(tecName, &hHandle);
		int count = SymbolValue(pData,hHandle);//�洢����
		if(count <= 0)
		{
			g_pSystemFun->m_iATIndicator->DeleteObj(hHandle);
			return hx_DefMinValues;
		}
		g_pSystemFun->m_iATIndicator->Calculation(hHandle, 0, count - 1);
		long lCount = 0;
		g_pSystemFun->m_iATIndicator->PlotCount(hHandle, &lCount);
		pCurDataStatus->Alloc(count);
		float dValue;
		VARIANT_BOOL bIsPlotOn;
		for (int i = 0; i < count; i++)
		{
			//for (int j = 0; j < lCount; j++)
			{
				g_pSystemFun->m_iATIndicator->IsPlotOn(hHandle, 1, i, &bIsPlotOn);
				if (bIsPlotOn)
				{
					g_pSystemFun->m_iATIndicator->GetPlotData(hHandle, 1, i, &dValue);
					pCurDataStatus->m_pData[i].m_dData = dValue;
				}
			}
		}
		g_pSystemFun->m_iATIndicator->DeleteObj(hHandle);
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}
HSDouble CSystemFun::THIGH (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						   HSCurDataInfoStatus* pCurDataStatus, int& nError)   // ����: 
{
	if(g_pSystemFun && g_pSystemFun->m_bLoadCom)
	{
		CHSDataSource* pData = NULL;
		pData = GetData(pCompile,ayVars,pCurDataStatus,nError);
		if (!pData)
		{
			return hx_DefMinValues;
		}	
		BSTR tecName;
		g_pSystemFun->m_iATIndicator->GetObjName(0, &tecName);//��ù�ʽ����
		__int64 hHandle;
		g_pSystemFun->m_iATIndicator->CreateObj(tecName, &hHandle);
		int count = SymbolValue(pData,hHandle);//�洢����
		if(count <= 0)
		{
			g_pSystemFun->m_iATIndicator->DeleteObj(hHandle);
			return hx_DefMinValues;
		}
		g_pSystemFun->m_iATIndicator->Calculation(hHandle, 0, count - 1);
		long lCount = 0;
		g_pSystemFun->m_iATIndicator->PlotCount(hHandle, &lCount);
		pCurDataStatus->Alloc(count);
		float dValue;
		VARIANT_BOOL bIsPlotOn;
		for (int i = 0; i < count; i++)
		{
			//for (int j = 0; j < lCount; j++)
			{
				g_pSystemFun->m_iATIndicator->IsPlotOn(hHandle, 0/*j*/, i, &bIsPlotOn);
				if (bIsPlotOn)
				{
					g_pSystemFun->m_iATIndicator->GetPlotData(hHandle, 0/*j*/, i, &dValue);
					pCurDataStatus->m_pData[i].m_dData = dValue;
				}
			}
		}
		g_pSystemFun->m_iATIndicator->DeleteObj(hHandle);
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}
#endif
// ����: OPEN(); ���̼�	
HSDouble CSystemFun::OPEN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: HIGH(); ��߼�
HSDouble CSystemFun::HIGH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	if(pRefDay == NULL)
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: LOW(); ��ͼ�
HSDouble CSystemFun::LOW(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					     HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: CLOSE(); ���̼�
HSDouble CSystemFun::CLOSE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						   HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}
	
	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

BOOL CSystemFun::CompareCloseOpen(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{	
	return OPEN(pCompile,ayVars,pCurDataStatus,nError) <= CLOSE(pCompile,ayVars,pCurDataStatus,nError);
}

// ����: VOL(); �ɽ���
HSDouble CSystemFun::VOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_VOL_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: AMOUNT(); �ɽ���
HSDouble CSystemFun::AMOUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)    
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_AMOUNT_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

#ifdef SUPPORT_NETVALUE
// ����: NETVALUE(); ��ծ���ʣ�����ֵ
HSDouble CSystemFun::NETVALUE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_NetValue_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}
#endif

// ����: DATEDATA(); ����
HSDouble CSystemFun::DATEDATA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
	{
		return hx_DefMinValues;
	}

	HSDouble* pRefDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if(pRefDay == NULL)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			return pRefDay[pCurDataStatus->m_nDataPos];
		}
	}
	else
	{
		pCurDataStatus->AddStatus(hx_CURRENTDATENOTDELETE);
		pCurDataStatus->m_pData = pRefDay;
		pCurDataStatus->m_lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ʱ�亯��
// ����: DATE(); ������
HSDouble CSystemFun::hxDATE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);	
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) )
					return pRefDateDay[pCurDataStatus->m_nDataPos];
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) )
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = pRefDateDay[nCur];
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: PERIOD_TYPE_DAY(); ����
HSDouble CSystemFun::hxDAY(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) )
					return ((long)pRefDateDay[pCurDataStatus->m_nDataPos]%100);
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) )
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = ((long)pRefDateDay[nCur]%100);
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: HOUR(); Сʱ
HSDouble CSystemFun::HSOUR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) ) // ����ʱ��
					return ((long)pRefDateDay[pCurDataStatus->m_nDataPos]/100)%100;
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) )
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = ((long)pRefDateDay[nCur]/100)%100;
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: MINUTE(); ����
HSDouble CSystemFun::hxMINUTE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) ) // ����ʱ��
					return (long)pRefDateDay[pCurDataStatus->m_nDataPos]%100;
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) )
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = (long)pRefDateDay[nCur]%100;
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: MONTH(); �·�
HSDouble CSystemFun::hxMONTH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) ) // ����ʱ��
					return ((long)pRefDateDay[pCurDataStatus->m_nDataPos]/1000000)%100;
				else if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // ������
					return ((long)pRefDateDay[pCurDataStatus->m_nDataPos]/100)%100;
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) )
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = ((long)pRefDateDay[nCur]/1000000)%100;
				}
			}
		}
		else if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // ������
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = ((long)pRefDateDay[nCur]/100)%100;
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: TIME(); ʱ����
HSDouble CSystemFun::hxTIME(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) ) // ����ʱ��
					return (long)pRefDateDay[pCurDataStatus->m_nDataPos]%1000000;
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_MINUTE_DATA) )  // ����ʱ��
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = (long)pRefDateDay[nCur]%1000000;
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: WEEKDAY(); ����
HSDouble CSystemFun::hxWEEKDAY(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}

	CTime curTime;
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // ������
				{
					curTime = hxLongToTime((long)pRefDateDay[pCurDataStatus->m_nDataPos]);
					if(curTime != -1)
					{
						return (curTime.GetDayOfWeek() - 1);
					}
				}
				else
				{
					return 0;
				}
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // ������
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					curTime = hxLongToTime((long)pRefDateDay[nCur]);
					if(curTime != -1)
					{
						pCurDataStatus->m_pData[nCur] = curTime.GetDayOfWeek() - 1;
					}
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: YEAR(); ���
HSDouble CSystemFun::hxYEAR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	HSDouble* pRefDateDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_DATE_DATA);
	if( pRefDateDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 )
	{
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefDateDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues )
			{
				if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // ������
					return ((long)pRefDateDay[pCurDataStatus->m_nDataPos]/10000);
				else
					return 0;
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() );
		if( GetData(pCompile,ayVars,pCurDataStatus, nError)->IsStyle(hx_PERIOD_TYPE_DAY_DATA) ) // ������
		{
			for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
			{
				if( pRefDateDay[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = ((long)pRefDateDay[nCur]/10000);
				}
			}
		}
		else
		{
			SetDefaultValues(pCurDataStatus->m_pData,pCurDataStatus->m_lSize,0);			
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

//���ú���
// ����: BACKSET(var1,var2) ��ǰ��ֵ
HSDouble CSystemFun::BACKSET(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			for( int nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues && 
					calc2.m_pData[nCur].m_dData != hx_DefMinValues)
				{
					if( calc1.m_pData[nCur] )
					{
						for(int nPos = nCur - (long)calc2.m_pData[nCur] + 1;nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
							{
								pCurDataStatus->m_pData[nPos] = 1;
							}
						}
					}
					else
					{
						pCurDataStatus->m_pData[nCur] = 0;
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues && v2 != hx_DefMinValues)
						{
							if( pData[nCur] )
							{
								for(int nPos = nCur - (long)v2 + 1;nPos >= 0 && nPos <= nCur; nPos++)
								{
									if( pData[nPos].m_dData != hx_DefMinValues )
									{
										pCurDataStatus->m_pData[nPos] = 1;
									}
								}
							}
							else
							{
								pCurDataStatus->m_pData[nCur] = 0;
							}
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues && v1 != hx_DefMinValues)
						{
							if( pData[nCur] )
							{
								for(int nPos = nCur - (long)v1 + 1;nPos >= 0 && nPos <= nCur; nPos++)
								{
									if( pData[nPos].m_dData != hx_DefMinValues )
									{
										pCurDataStatus->m_pData[nPos] = 1;
									}
								}
							}
							else
							{
								pCurDataStatus->m_pData[nCur] = 0;
							}
						}
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����:  BARSCOUNT(var1) ��Ч������
HSDouble CSystemFun::BARSCOUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		int nCount = 0;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				nCount++;
				pCurDataStatus->m_pData[nCur] = nCount;
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: BARSLAST(var1) ��һ����������λ��
HSDouble CSystemFun::BARSLAST(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData    == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL )
	{
		int  nCount = 0;
		BOOL nFirst = FALSE;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				if( curStatus.m_pData[nCur] )
				{
					nCount = 0;
					nFirst = TRUE;
				}

				if( nFirst )
				{
					nCount++;
				}

				pCurDataStatus->m_pData[nCur] = nCount;
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: BARSSINCE(var1) ��һ����������λ��
HSDouble CSystemFun::BARSSINCE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData    == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL )
	{
		int  nCount = 0;
		BOOL bSucc  = FALSE;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				if( !bSucc && (double)curStatus.m_pData[nCur] )
				{					
					bSucc = TRUE;
				}
				if( bSucc )
				{
					pCurDataStatus->m_pData[nCur] = nCount;
					nCount++;
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: COUNT(var1,var2) ͳ������
HSDouble CSystemFun::COUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int  nCount = 0;
		int  nCur;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			int lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,calc2.m_lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if(calc2.m_pData[nCur].m_dData != hx_DefMinValues)
				{
					lRefPeriod = (long)calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}			
				for(int nPos = ((lRefPeriod <= 0)?0:(nCur - lRefPeriod)); nPos >= 0 && nPos <= nCur;nPos++)
				{
					if( calc1.m_pData[nPos].m_dData != hx_DefMinValues && 
						(double)calc1.m_pData[nPos] )
					{
						nCount++;
					}
				}
				//---2009.11.09 litaoע�͵�����������nCountΪ0ʱ����������
				//if(nCount > 0)
				//{
					pCurDataStatus->m_pData[nCur] = nCount;
				//}
				//--------------------------------------------
				nCount = 0;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{									
						for(int nPos = ((v2 <= 0)?0:(nCur-v2)); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								(double)pData[nPos] )
							{
								nCount++;
							}
						}
						//---2009.11.09 litaoע�͵�����������nCountΪ0ʱ����������
						//if( nCount > 0 )
						//{
							pCurDataStatus->m_pData[nCur] = nCount;
						//}
						//---------------------------------------------		
						nCount = 0;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{
						for(int nPos = ((v1 <= 0)?0:(v1-nCur)); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								(double)pData[nPos] )
							{
								nCount++;
							}
						}
						//---2009.11.09 litaoע�͵�����������nCountΪ0ʱ����������
						//if( nCount > 0 )
						//{
							pCurDataStatus->m_pData[nCur] = nCount;
						//}
						//---------------------------------------------
						nCount = 0;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����:  DMA(var1,var2) ��̬�ƶ�ƽ��
HSDouble CSystemFun::DMA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int  nCur;
		BOOL bOneTime = TRUE;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{		
			for(nCur = 0; nCur < lSize; nCur++)
			{
				if( calc1.m_pData[nCur] == hx_DefMinValues ||
					calc2.m_pData[nCur] == hx_DefMinValues )
					continue;

				if(bOneTime)
				{
					pCurDataStatus->m_lPreData = calc1.m_pData[nCur];
					bOneTime = FALSE;
				}
				
				pCurDataStatus->m_lPreData = 
							(pCurDataStatus->m_lPreData * ((double)1 - calc2.m_pData[nCur]) + 
							calc1.m_pData[nCur] * calc2.m_pData[nCur]);
				pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(nCur = 0; nCur < lSize; nCur++)
					{
						if( pData[nCur] == hx_DefMinValues )
							continue;
						
						if(bOneTime)
						{
							pCurDataStatus->m_lPreData = pData[nCur];
							bOneTime = FALSE;
						}
						
						pCurDataStatus->m_lPreData = 
							pCurDataStatus->m_lPreData * (1 - v2) + (double)pData[nCur] * v2;
						pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(nCur = 0; nCur < lSize; nCur++)
					{
						if( pData[nCur] == hx_DefMinValues )
							continue;
						
						if(bOneTime)
						{
							pCurDataStatus->m_lPreData = pData[nCur];
							bOneTime = FALSE;
						}
						
						pCurDataStatus->m_lPreData = 
							pCurDataStatus->m_lPreData * (1 - v1) + (double)pData[nCur] * v1;
						pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
					}
				}
			}
		}		
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: EMA(var1,var2) ָ��ƽ���ƶ�ƽ��
HSDouble CSystemFun::EMA( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);
	
	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long N = (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||	N <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		BOOL bOneTime = TRUE;
		pCurDataStatus->Alloc(curStatus.m_lSize);		
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				pCurDataStatus->m_lPreData = 0;
				pCurDataStatus->m_pData[nCur] = 0.0;
				continue;
			}
			if(bOneTime)
			{
				pCurDataStatus->m_lPreData = ( (double)curStatus.m_pData[nCur] * (N - 1) +
											   (double)curStatus.m_pData[nCur] * 2 ) / (N + 1);
				pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
				bOneTime = FALSE;
				continue;
			}
			
			pCurDataStatus->m_lPreData = 
						( (double)pCurDataStatus->m_lPreData * (N - 1) + (double)curStatus.m_pData[nCur] * 2 ) / (N + 1);
			pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: HHV(var1,var2) ���ֵ
HSDouble CSystemFun::HHV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble curMax = hx_DefMaxValues;
		int  nCur;
		int  nPos;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for( nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					nPos = ((calc2.m_pData[nCur] <= 0)?0:(nCur - (double)calc2.m_pData[nCur] + 1));
					if(calc2.m_pData[nCur] > nCur)
						nPos = 0;
					for(; nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData !=hx_DefMaxValues &&
							curMax < calc1.m_pData[nPos] )
						{
							curMax = calc1.m_pData[nPos];
						}
						if( curMax.m_dData != hx_DefMaxValues )
						{
							pCurDataStatus->m_pData[nCur] = curMax;
						}
					}
				}
				curMax = hx_DefMaxValues;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						nPos = ((v2 <= 0)?0:(nCur - v2 + 1));
						if(v2 > nCur)
							nPos = 0;
						for(; nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData !=hx_DefMaxValues && 
								curMax < pData[nPos].m_dData )
							{				
								curMax = pData[nPos];
							}
							if( curMax.m_dData != hx_DefMaxValues )
							{
								pCurDataStatus->m_pData[nCur] = curMax;
							}
						}
						curMax = hx_DefMaxValues;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						nPos = ((v1 <= 0)?0:(nCur - v1 + 1));
						if(v1 > nCur)
							nPos = 0;
						for(; nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData !=hx_DefMaxValues && 
								curMax < pData[nPos] )
							{							
								curMax = pData[nPos];
							}
							if( curMax.m_dData != hx_DefMaxValues )
							{
								pCurDataStatus->m_pData[nCur] = curMax;
							}
						}
						curMax = hx_DefMaxValues;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: HHVBARS(var1,var2) ��һ�ߵ㵽��ǰ��������
HSDouble CSystemFun::HHVBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int nCount = 0;
		int nCur;
		int nPos;
		HSDouble curMax = hx_DefMaxValues;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for( nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					for( nPos = ((calc2.m_pData[nCur] <= 0)?0:(nCur - (double)calc2.m_pData[nCur] + 1));nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues && 
							curMax < calc1.m_pData[nPos] )
						{
							curMax = calc1.m_pData[nPos];
							nCount = 0;
						}
						else if( curMax.m_dData != hx_DefMaxValues )
						{
							nCount++;
						}
					}
				}
				if( curMax.m_dData != hx_DefMaxValues )
				{
					pCurDataStatus->m_pData[nCur] = nCount;
				}
				curMax = hx_DefMaxValues;
				nCount = 0;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for( nPos = ((v2 <= 0)?0:(nCur - v2 + 1));nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								curMax < pData[nPos] )
							{
								curMax = pData[nPos];
								nCount = 0;
							}
							else if( curMax.m_dData != hx_DefMaxValues )
							{
								nCount++;
							}
						}
						if( curMax.m_dData != hx_DefMaxValues )
						{
							pCurDataStatus->m_pData[nCur] = nCount;
						}
						curMax = hx_DefMaxValues;
						nCount = 0;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for( nPos = ((v1 <= 0)?0:(nCur - v1 + 1));nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								curMax < pData[nPos] )
							{
								curMax = pData[nPos];
								nCount = 0;
							}
							else if( curMax.m_dData != hx_DefMaxValues )
							{
								nCount++;
							}
						}
						if( curMax.m_dData != hx_DefMaxValues )
						{
							pCurDataStatus->m_pData[nCur] = nCount;
						}
						curMax = hx_DefMaxValues;
						nCount = 0;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����:  LLV(var1,var2) ���ֵ
HSDouble CSystemFun::LLV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble curMin = hx_DefMinValues;
		int  nCur;
		int  nPos;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for( nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					nPos = ((calc2.m_pData[nCur] <= 0)?0:(nCur - (double)calc2.m_pData[nCur] + 1));
					if(calc2.m_pData[nCur] > nCur)
						nPos = 0;
					for( ; nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues && 
							curMin > calc1.m_pData[nPos] )
						{							
							curMin = calc1.m_pData[nCur];
						}
						if(curMin.m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[nCur] = curMin;
						}
					}
				}
				curMin = hx_DefMinValues;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						nPos = ((v2 <= 0)?0:(nCur - v2 + 1));
						if(v2 > nCur)
							nPos = 0;
						for(;nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && curMin > pData[nPos] )
							{							
								curMin = pData[nPos];
							}
							if(curMin.m_dData != hx_DefMinValues)
							{
								pCurDataStatus->m_pData[nCur] = curMin;
							}
						}
						curMin = hx_DefMinValues;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						nPos = ((v1 <= 0)?0:(nCur - v1 + 1));
						if(v1 > nCur)
							nPos = 0;
						for(; nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && curMin > pData[nPos] )
							{
								curMin = pData[nPos];
							}
							if(curMin.m_dData != hx_DefMinValues)
							{
								pCurDataStatus->m_pData[nCur] = curMin;
							}
						}
						curMin = hx_DefMinValues;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: LLVBARS(var1,var2) ��һ�͵㵽��ǰ��������
HSDouble CSystemFun::LLVBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{ 
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int nCount = 0;
		int nCur;
		HSDouble curMin = hx_DefMinValues;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for( nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					for(int nPos = ((calc2.m_pData[nCur] <= 0)?0:(nCur - (double)calc2.m_pData[nCur] + 1));nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues && 
							curMin > calc1.m_pData[nPos] )
						{
							curMin = calc1.m_pData[nPos];
							nCount = 0;
						}
						else if( curMin.m_dData != hx_DefMinValues )
						{
							nCount++;
						}
					}
				}
				if( curMin.m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = nCount;
				}
				curMin = hx_DefMinValues;
				nCount = 0;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for(int nPos = ((v2 <= 0)?0:(nCur - v2 + 1));nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								curMin > pData[nPos] )
							{
								curMin = pData[nPos];
								nCount = 0;
							}
							else if( curMin.m_dData != hx_DefMinValues )
							{
								nCount++;
							}
						}
						if( curMin.m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[nCur] = nCount;
						}
						curMin = hx_DefMinValues;
						nCount = 0;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for(int nPos = ((v1 <= 0)?0:(nCur - v1 + 1));nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues && 
								curMin > pData[nPos] )
							{
								curMin = pData[nPos];
								nCount = 0;
							}
							else if( curMin.m_dData != hx_DefMinValues )
							{
								nCount++;
							}
						}
						if( curMin.m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[nCur] = nCount;
						}
						curMin = hx_DefMinValues;
						nCount = 0;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: MA(var1,var2) ���ƶ�ƽ��
HSDouble CSystemFun::MA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{

	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble curSum = 0;
		int  nCur,nPos;
		BOOL bValid = FALSE;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			long lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			nPos = GetDataValidPos(calc1.m_pData,lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					lRefPeriod = calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}
				lRefPeriod = (((lRefPeriod <= 1)?1:lRefPeriod) - 1);
				if( lRefPeriod <= 0 )
				{
					pCurDataStatus->m_pData[nCur] = calc1.m_pData[nCur];
				}
				else
				{
					for(nPos = (lRefPeriod <= 0)?0:(nCur - lRefPeriod); nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
						{
							curSum += calc1.m_pData[nPos];
							bValid = TRUE;
						}
					}
					if( bValid && lRefPeriod != hx_DefMinValues )
					{
						pCurDataStatus->m_pData[nCur] = curSum/lRefPeriod;
					}
					curSum = 0;
					bValid = FALSE;
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{
						if( v2 <= 0 )
						{
							if( pData[nCur].m_dData != hx_DefMinValues )
							{
								pCurDataStatus->m_pData[nCur] = pData[nCur];
							}
						}
						else
						{
							for( nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos].m_dData != hx_DefMinValues )
								{
									curSum += pData[nPos];
									bValid = TRUE;
								}
							}
							if( bValid )
							{
								pCurDataStatus->m_pData[nCur] = curSum/(v2+1);
							}
							curSum = 0;
							bValid = FALSE;
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{	
						if( v1 <= 0 )
						{
							if( pData[nCur].m_dData != hx_DefMinValues )
							{
								pCurDataStatus->m_pData[nCur] = pData[nCur];
							}
						}
						else
						{
							for( nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos].m_dData != hx_DefMinValues )
								{
									curSum += pData[nPos];
									bValid = TRUE;
								}
							}
							if( bValid )
							{
								pCurDataStatus->m_pData[nCur] = curSum/(v1+1);
							}
							curSum = 0;
							bValid = FALSE;
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;

	/*
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pVar0 = ayVars->GetAt(0);
	LPNODELEMENT pRefData = pVar0->left;
	long lRefPeriod = (long)pCompile->vexp(pVar0->right,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pRefData    == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus;
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		HSDouble curAverage = hx_DefMinValues;
		int  nNext = 0,nCur,nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);

		nCur = GetDataValidPos(curStatus.m_pData,curStatus.m_lSize);
		if(nCur > 0)
		{
			nNext += nCur;
		}
		for( nCur = nCur + lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for( nPos = nNext; nPos >= 0 && nPos <= nCur; nPos++ )
			{
				if( curStatus.m_pData[nPos] != hx_DefMinValues )
				{
					if( curAverage == hx_DefMinValues )
					{
						curAverage = 0;
					}
					curAverage += curStatus.m_pData[nPos];
				}
			}
			if(curAverage != hx_DefMinValues)
			{
				pCurDataStatus->m_pData[nCur] = curAverage/lRefPeriod;
			}

			curAverage = hx_DefMinValues;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
	*/
}

// ����: REF(var1,var2) ������������ǰ������
HSDouble CSystemFun::REF(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			for( int nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					if( nCur - (long)calc2.m_pData[nCur] >= 0 )
					{
						pCurDataStatus->m_pData[nCur] = calc1.m_pData[nCur - (long)calc2.m_pData[nCur]];
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( nCur - (long)v2 >= 0 )
						{
							pCurDataStatus->m_pData[nCur] = calc1.m_pData[nCur - (long)v2];
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( nCur - (long)v1 >= 0 )
						{
							pCurDataStatus->m_pData[nCur] = calc1.m_pData[nCur - (long)v1];
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����:  SMA(var1,var2,var3) �ƶ�ƽ��
HSDouble CSystemFun::SMA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	LPNODELEMENT pParam2  = ayVars->GetParam(2);

	if(pParam0 == NULL || pParam1 == NULL || pParam2 == NULL)
	{
		return hx_DefMinValues;
	}

	long N = (long)pCompile->vexp(pParam1,pCurDataStatus);
	long M = (long)pCompile->vexp(pParam2,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		N <= 0				  ||
		N < M				  ||
		M <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL)
	{
		BOOL bOneTime = TRUE;
		pCurDataStatus->Alloc(curStatus.m_lSize);		
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
				continue;

			if(bOneTime)
			{
				pCurDataStatus->m_lPreData = ( curStatus.m_pData[nCur] * (N - M) +
											   curStatus.m_pData[nCur] * M ) / N;

				bOneTime = FALSE;
				continue;
			}
			pCurDataStatus->m_lPreData = ( pCurDataStatus->m_lPreData * (N - M) + curStatus.m_pData[nCur] * M ) / N;
			pCurDataStatus->m_pData[nCur] = pCurDataStatus->m_lPreData;
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: SUM(var1,var2) �ܺ�
HSDouble CSystemFun::SUM(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble curSum = 0;
		int  nCur;
		BOOL bValid = FALSE;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			long lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					lRefPeriod = calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}
				for(int nPos = ((lRefPeriod <= 0)?0:(nCur - lRefPeriod)); nPos >= 0 && nPos <= nCur; nPos++)
				{
					if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
					{
						curSum += calc1.m_pData[nPos];
						bValid = TRUE;
					}
				}
				if( bValid )
				{
					pCurDataStatus->m_pData[nCur] = curSum;
				}
				curSum = 0;
				bValid = FALSE;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{
						for(int nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								curSum += pData[nPos];
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							pCurDataStatus->m_pData[nCur] = curSum;
						}
						curSum = 0;
						bValid = FALSE;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{		
						for(int nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								curSum += pData[nPos];
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							pCurDataStatus->m_pData[nCur] = curSum;
						}
						curSum = 0;
						bValid = FALSE;
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: SUMBARS(var1,var2) ��ǰ�ۼӵ�ָ��ֵ�����ڵ�������
HSDouble CSystemFun::SUMBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int nCur;
		int nCount = 0;
		HSDouble curSum = 0;
		int nPos;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for( nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					for( nPos = nCur; nPos >= 0; nPos-- )
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
						{
							curSum += calc1.m_pData[nPos];
							nCount++;
						}
						if( curSum >= calc2.m_pData[nCur] )
						{
							pCurDataStatus->m_pData[nCur] = nCount;//-1;
							break;
						}
					}
				}
				nCount = 0;
				curSum = 0;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for( nPos = nCur; nPos >= 0; nPos-- )
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								curSum += pData[nPos];
								nCount++;
							}
							if( curSum >= v2 )
							{
								pCurDataStatus->m_pData[nCur] = nCount;//-1;
								break;
							}
						}
						nCount = 0;
						curSum = 0;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( ; nCur < lSize; nCur++ )
					{
						for( nPos = nCur; nPos >= 0; nPos-- )
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								curSum += pData[nPos];
								nCount++;
							}
							if( curSum >= v1 )
							{
								pCurDataStatus->m_pData[nCur] = nCount;//-1;
								break;
							}
						}
						nCount = 0;
						curSum = 0;
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: IF(var1,var2,var3) ����������ͬ��ֵ
HSDouble CSystemFun::IF(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	LPNODELEMENT pParam2  = ayVars->GetParam(2);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL ||
		pParam0  == NULL ||
		pParam1 == NULL ||
		pParam2 == NULL)
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v0 = pCompile->vexp(pParam0,&calc0);
	double v1 = pCompile->vexp(pParam1,&calc1);
	double v2 = pCompile->vexp(pParam2,&calc2);

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		v0 = pCompile->vexp(pParam0,pCurDataStatus);
		v1 = pCompile->vexp(pParam1,pCurDataStatus);
		v2 = pCompile->vexp(pParam2,pCurDataStatus);

		if( v0 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return ( v0?v1:v2 );
	}

	if( (calc0.m_pData != NULL) || (calc1.m_pData != NULL) || (calc2.m_pData != NULL) ) // һ�Զ࣬���һ����Զ�,
	{
		int i;
		long lSize = max(max(calc0.m_lSize,calc1.m_lSize),calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc0.m_pData != NULL && calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc0.m_pData[i].m_dData != hx_DefMinValues && 
					calc1.m_pData[i].m_dData != hx_DefMinValues &&
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?calc1.m_pData[i]:calc2.m_pData[i]);
				}
			}
		}
		else if( (calc0.m_pData != NULL) && (calc1.m_pData != NULL || calc2.m_pData != NULL) )
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{	
					for(i = 0; i < lSize; i++)
					{
						if( pData[i].m_dData != hx_DefMinValues && 
							calc0.m_pData[i].m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?pData[i]:v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if( pData[i].m_dData != hx_DefMinValues &&
							calc0.m_pData[i].m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?v1:pData[i]);
						}
					}
				}
			}
		}
		else if( (calc0.m_pData == NULL) && (calc1.m_pData != NULL || calc2.m_pData != NULL) )
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v0 != hx_ReturnPointValues && v2 != hx_ReturnPointValues )
				{	
					for(i = 0; i < lSize; i++)
					{
						if( pData[i].m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[i] = (v0?pData[i]:v2);
						}
					}
				}
				else if( v0 != hx_ReturnPointValues && v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if( pData[i].m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[i] = (v0?v1:pData[i]);
						}
					}
				}
			}
		}
		else if( (calc0.m_pData != NULL) && (calc1.m_pData == NULL) && (calc2.m_pData == NULL) )
		{
			if( v1 != hx_ReturnPointValues && v2 != hx_ReturnPointValues )
			{	
				for(i = 0; i < lSize; i++)
				{
					if( calc0.m_pData[i].m_dData != hx_DefMinValues )
					{
						pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?v1:v2);
					}
				}
			}
		}
		
		return hx_ReturnPointValues;
	}
	else
	{
		if( v0 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return ( v0?v1:v2 );
	}

	return hx_DefMinValues;
}

// ����: ISDOWN();  �������Ƿ�����
HSDouble CSystemFun::ISDOWN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSDouble* pRefOpenDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);	
	if(pRefOpenDay == NULL || pRefCloseDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefCloseDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues && 
				pRefOpenDay[pCurDataStatus->m_nDataPos].m_dData  != hx_DefMinValues )
			{
				return pRefCloseDay[pCurDataStatus->m_nDataPos] < pRefOpenDay[pCurDataStatus->m_nDataPos];
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc(GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize());
		for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
		{
			if( pRefCloseDay[nCur].m_dData != hx_DefMinValues && 
				pRefOpenDay[nCur].m_dData  != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = (pRefCloseDay[nCur] < pRefOpenDay[nCur]);
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: ISEQUAL(); �������Ƿ�ƽ��
HSDouble CSystemFun::ISEQUAL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSDouble* pRefOpenDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);	
	if(pRefOpenDay == NULL || pRefCloseDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefCloseDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues && 
				pRefOpenDay[pCurDataStatus->m_nDataPos].m_dData  != hx_DefMinValues )
			{
				return (pRefCloseDay[pCurDataStatus->m_nDataPos] == pRefOpenDay[pCurDataStatus->m_nDataPos]);
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc(GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize());
		for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
		{
			if( pRefCloseDay[nCur].m_dData != hx_DefMinValues && 
				pRefOpenDay[nCur].m_dData  != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = (pRefCloseDay[nCur] == pRefOpenDay[nCur]);
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: ISUP();    �������Ƿ�����
HSDouble CSystemFun::ISUP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSDouble* pRefOpenDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);	
	if(pRefOpenDay == NULL || pRefCloseDay == NULL || GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0)
	{	
		return hx_DefMinValues;
	}
	if( pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		if( pCurDataStatus->m_nDataPos >= 0 &&  
			pCurDataStatus->m_nDataPos < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() )
		{
			if( pRefCloseDay[pCurDataStatus->m_nDataPos].m_dData != hx_DefMinValues && 
				pRefOpenDay[pCurDataStatus->m_nDataPos].m_dData  != hx_DefMinValues )
			{
				return pRefCloseDay[pCurDataStatus->m_nDataPos] > pRefOpenDay[pCurDataStatus->m_nDataPos];
			}
		}
	}
	else
	{
		pCurDataStatus->Alloc(GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize());
		for(int nCur = 0; nCur < GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize(); nCur++)
		{
			if( pRefCloseDay[nCur].m_dData != hx_DefMinValues && 
				pRefOpenDay[nCur].m_dData  != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = (pRefCloseDay[nCur] > pRefOpenDay[nCur]);
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}


// ����: ABS(var1) �����ֵ
HSDouble CSystemFun::ABS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return fabs( var1 );
	}

	// ����һ������
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = fabs(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����:  MAX(var1,var2) �����ֵ
HSDouble CSystemFun::MAX(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL || pParam1 == NULL )
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	
	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		v1 = pCompile->vexp(pParam0,pCurDataStatus);
		v2 = pCompile->vexp(pParam1,pCurDataStatus);
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return __max( v1,v2 );
	}

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = __max(calc1.m_pData[i],calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = __max(pData[i],v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = __max(v1,pData[i]);
						}
					}
				}
			}
		}
		
		return hx_ReturnPointValues;
	}
	else
	{
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return __max( v1,v2 );
	}

	return hx_DefMinValues;
}

// ����: MIN(var1,var2) ����Сֵ
HSDouble CSystemFun::MIN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		v1 = pCompile->vexp(pParam0,pCurDataStatus);
		v2 = pCompile->vexp(pParam1,pCurDataStatus);
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return __min( v1,v2 );
	}

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[i] = __min(calc1.m_pData[i],calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = __min(pData[i],v2);
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues)
						{
							pCurDataStatus->m_pData[i] = __min(v1,pData[i]);
						}
					}
				}
			}
		}
		
		return hx_ReturnPointValues;
	}
	else
	{
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		return __min( v1,v2 );
	}
	
	return hx_DefMinValues;
}

// ͳ�ƺ���

// ����: AVEDEV(var1,var2) ƽ������ƫ��
HSDouble CSystemFun::AVEDEV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curAvg = 0;
		HSDouble sum = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = lRefPeriod - 1; nCur > 0 && nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				for(nPos = nNext; nPos <= nCur; nPos++)
				{
					if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
					{
						curAvg += curStatus.m_pData[nPos];
					}
				}

				if( curAvg.m_dData != 0 )
				{
					curAvg /= lRefPeriod;
					for(nPos = nNext; nPos <= nCur; nPos++)
					{
						if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
						{
							sum += fabs(curStatus.m_pData[nPos] - curAvg);
						}
					}

					if( sum.m_dData != 0 )
					{ 
						sum /= lRefPeriod;
						pCurDataStatus->m_pData[nCur] = sum;
					}
				}
				
				curAvg = 0;
				sum = 0;
			}
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}

// ����: DEVSQ(var1,var2) ����ƫ��ƽ����
HSDouble CSystemFun::DEVSQ(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curAvg = 0;
		HSDouble curTotal = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for(nPos = nNext; nPos <= nCur; nPos++)
			{
				if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
				{
					curAvg += curStatus.m_pData[nPos];
				}
			}
			curAvg /= lRefPeriod;
			if( curAvg.m_dData != 0 )
			{
				for(nPos = nNext; nPos <= nCur; nPos++)
				{
					if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
					{
						curTotal += (curStatus.m_pData[nPos] - curAvg)*(curStatus.m_pData[nPos] - curAvg);
					}
				}
				pCurDataStatus->m_pData[nCur] = curTotal;
			}

			curTotal = 0;//curAvg;
			curAvg = 0;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}

//	FORCAST		();    // ����: FORCAST(var1,var2) ���Իع�Ԥ��ֵ
//	SLOPE		();    // ����:  SLOPE(var1,var2) ���Իع�б��

// ����: STD(var1,var2) �����׼��
HSDouble CSystemFun::STD(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curTotalofSqr = 0;
		HSDouble curTotal = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		
		int nCur = GetDataValidPos(curStatus.m_pData,curStatus.m_lSize);
		if( nCur > 0)
		{
			nNext += nCur;
		}
		for( nCur = nCur + lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for(nPos = nNext; nPos <= nCur; nPos++)
			{
				if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
				{
					curTotalofSqr += curStatus.m_pData[nPos]*curStatus.m_pData[nPos];
					curTotal	  += curStatus.m_pData[nPos];
				}
			}
			if( curTotalofSqr.m_dData != 0 )
			{
				curTotalofSqr *= lRefPeriod; // ƽ����*����
			}
			if( curTotal.m_dData != 0 )
			{
				curTotal *= curTotal;		 // �͵�ƽ��
			}

			if( curTotal.m_dData != 0 && curTotalofSqr.m_dData != 0 )
			{
				pCurDataStatus->m_pData[nCur] = sqrt(abs((double)(curTotalofSqr - curTotal)) / (lRefPeriod*(lRefPeriod - 1)) );
			}

			curTotalofSqr = 0;
			curTotal	  = 0;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}

// ����: STDP(var1,var2) �����׼��
HSDouble CSystemFun::STDP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curTotalofSqr = 0;
		HSDouble curTotal = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for(nPos = nNext; nPos <= nCur; nPos++)
			{
				if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
				{
					curTotalofSqr += curStatus.m_pData[nPos]*curStatus.m_pData[nPos];
					curTotal	  += curStatus.m_pData[nPos];
				}
			}
			if( curTotalofSqr.m_dData != 0 )
			{
				curTotalofSqr *= lRefPeriod; // ƽ����*����
			}
			if( curTotal.m_dData != 0 )
			{
				curTotal *= curTotal;		 // �͵�ƽ��
			}

			if( curTotal.m_dData != 0 && curTotalofSqr.m_dData != 0 )
			{
				pCurDataStatus->m_pData[nCur] = sqrt(abs((double)(curTotalofSqr - curTotal)) / (lRefPeriod*lRefPeriod) );
			}

			curTotalofSqr = 0;
			curTotal	  = 0;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}

// ����: VAR(var1,var2) ������������
HSDouble CSystemFun::VAR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod = (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curTotalofSqr = 0;
		HSDouble curTotal = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for(nPos = nNext; nPos <= nCur; nPos++)
			{
				if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
				{
					curTotalofSqr += curStatus.m_pData[nPos]*curStatus.m_pData[nPos];
					curTotal	  += curStatus.m_pData[nPos];
				}
			}
			if( curTotalofSqr.m_dData != 0 )
			{
				curTotalofSqr *= lRefPeriod; // ƽ����*����
			}
			if( curTotal.m_dData != 0 )
			{
				curTotal *= curTotal;		 // �͵�ƽ��
			}

			if( curTotal.m_dData != 0 && curTotalofSqr.m_dData != 0 )
			{
				pCurDataStatus->m_pData[nCur] = (double)(curTotalofSqr - curTotal) / (lRefPeriod*(lRefPeriod - 1));
			}

			curTotalofSqr = 0;
			curTotal	  = 0;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}

// ����:  VARP(var1,var2) ������������
HSDouble CSystemFun::VARP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	if(pParam0 == NULL || pParam0 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		lRefPeriod  <= 0 )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL )
	{
		HSDouble curTotalofSqr = 0;
		HSDouble curTotal = 0;
		int nNext = 0;
		int nPos;
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = lRefPeriod - 1; nCur < curStatus.m_lSize; nCur++ )
		{
			for(nPos = nNext; nPos <= nCur; nPos++)
			{
				if( curStatus.m_pData[nPos].m_dData != hx_DefMinValues )
				{
					curTotalofSqr += curStatus.m_pData[nPos]*curStatus.m_pData[nPos];
					curTotal	  += curStatus.m_pData[nPos];
				}
			}
			if( curTotalofSqr.m_dData != 0 )
			{
				curTotalofSqr *= lRefPeriod; // ƽ����*����
			}
			if( curTotal.m_dData != 0 )
			{
				curTotal *= curTotal;		 // �͵�ƽ��
			}

			if( curTotal.m_dData != 0 && curTotalofSqr.m_dData != 0 )
			{
				pCurDataStatus->m_pData[nCur] = (curTotalofSqr - curTotal) / (lRefPeriod*lRefPeriod);
			}

			curTotalofSqr = 0;
			curTotal	  = 0;
			nNext ++;
		}
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}


// ��������
// �������ͳ���
HSDouble CSystemFun::STICK(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ��״�ߣ���0��ָ��ֵ�仭��ֱֱ��
{
	return (HSDouble)0x00000001;
}

HSDouble CSystemFun::COLORSTICK(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ��ɫ��״�ߣ���ֵΪ0����ʾ��ɫ��������ʾ��ɫ
{
	return (HSDouble)0x00000002;
}

HSDouble CSystemFun::VOLSTICK(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // �ɽ�����״�ߣ����ɼ�����ʱ��ʾ��ɫ��������������ʾ��ɫ������
{
	return (HSDouble)0x00000004;
}

HSDouble CSystemFun::LINESTICK(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ͬʱ������״�ߺ�ָ����
{
	return (HSDouble)0x00000008;
}

HSDouble CSystemFun::CROSSDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // С����
{
	return (HSDouble)0x00000010;
}

HSDouble CSystemFun::CIRCLEDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // СԲȦ��
{
	return (HSDouble)0x00000020;
}

HSDouble CSystemFun::POINTDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // СԲ����
{
	return (HSDouble)0x00000040;
}

HSDouble CSystemFun::DRAW3D(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 3d
{
	return (HSDouble)0x00000200;
}

HSDouble CSystemFun::LINEDRAW3D(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 3d
{
	return (HSDouble)0x00008000;
}

HSDouble CSystemFun::NOTDRAWLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // 3d
{
	return (HSDouble)0x00010000;
}

// ��ǰ�ı�������
HSDouble CSystemFun::NOTDRAWTEXT( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return (HSDouble)0x00020000;
}

// ��ǰ�����ı���������
HSDouble CSystemFun::NOTDRAWLINETEXT( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return (HSDouble)(0x00010000 | 0x00020000);
}  

HSDouble CSystemFun::NOTDRAW( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return (HSDouble)(HX_NOTEXPRESSDRAW);
}  

HSDouble CSystemFun::NOTTABLE( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return (HSDouble)(HX_NOTTABLE);
}  

HSDouble CSystemFun::NOTMAXMIN( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return (HSDouble)(HX_NOTCALCMAXMIN);
}  

// ���K����ɫ
HSDouble CSystemFun::COLORRED(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ��ɫ
{
	return RGB(255,0,0);
}

HSDouble CSystemFun::COLORGREEN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ��ɫ
{
	return RGB(0,255,0);
}

HSDouble CSystemFun::COLORBLUE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ��ɫ
{
	return RGB(0,0,255);
}

HSDouble CSystemFun::COLORBLACK(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ��ɫ
{
	return 0;
}

HSDouble CSystemFun::COLORWHITE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ��ɫ
{
	return RGB(255,255,255);
}

HSDouble CSystemFun::COLORGRAY(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ��ɫ
{
	return RGB(192,192,192);
}

HSDouble CSystemFun::COLORYELLOW(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ��ɫ
{
	return RGB(255,255,0);
}

HSDouble CSystemFun::COLORCYAN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ��ɫ
{
	return RGB(0,128,192);
}

HSDouble CSystemFun::COLORMAGENTA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // Ʒ��ɫ
{
	return RGB(255,128,255);
}

HSDouble CSystemFun::COLORBROWN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)  // ��ɫ
{
	return RGB(128,0,0);
}

HSDouble CSystemFun::hxRGB		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)

{
	if(ayVars == NULL || ayVars->GetSize() <= 1)
	{
		nError = 0;
		return 0;
	}

	LPNODELEMENT pVar0 = ayVars->GetParam(0);
	LPNODELEMENT pVar1 = ayVars->GetParam(1);
	LPNODELEMENT pVar2 = ayVars->GetParam(2);

	if( pVar0 == NULL ||
		pVar1 == NULL ||
		pVar2 == NULL )
		return 0;

	return RGB( (BYTE)pCompile->vexp(pVar0,pCurDataStatus),
				(BYTE)pCompile->vexp(pVar1,pCurDataStatus),
				(BYTE)pCompile->vexp(pVar2,pCurDataStatus) );
}

	// �߷����
HSDouble CSystemFun::hxSOLID(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	return PS_SOLID;
}

HSDouble CSystemFun::hxDASH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)       /* -------  */
{
	return PS_DASH;
}

HSDouble CSystemFun::hxDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)        /* .......  */
{
	return PS_DOT;
}

HSDouble CSystemFun::hxDASHDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)    /* _._._._  */
{
	return PS_DASHDOT;
}

HSDouble CSystemFun::hxDASHDOTDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError) /* _.._.._  */
{
	return PS_DASHDOTDOT;
}

int CSystemFun::GetDataValidPos(HSDouble* pData,int nSize)
{
	if(pData == NULL || nSize == 0) 
		return 0;
	int nCurPos;
	for( nCurPos = 0; nCurPos < nSize; nCurPos++ )
	{
		if( pData[nCurPos].m_dData != hx_DefMinValues )
		{
			break;
		}
	}
	return nCurPos;
}

time_t CSystemFun::hxLongToTime(long lDate)
{
	struct tm atm;
	memset(&atm, 0, sizeof(tm));
	TCHAR szTime[32];
	_stprintf(szTime, _T("%ld"), lDate);
	int nLen = _tcslen(szTime);
	if(nLen <= 8)
	{
		atm.tm_year = int(lDate/10000);
 		if(atm.tm_year < 1900)
 		{
			atm.tm_mon    = atm.tm_year/100 - 1;
			atm.tm_mday   = atm.tm_year%100;
			atm.tm_hour   = (int)((lDate%10000)/100);
			atm.tm_min    = (int)(lDate%100);
			time_t ct     = time(NULL);
			atm.tm_year   = localtime(&ct)->tm_year + 1900;
		}
		else
		{
			atm.tm_mon    = (int)((lDate%10000)/100) - 1;
			atm.tm_mday   = (int)(lDate%100);
			atm.tm_hour   = 0;
			atm.tm_min    = 0;
		}
	}
	else if(nLen > 8)
	{
		atm.tm_year = (int)(lDate/100000000 + 1990);
		int nTemp   = (int)(lDate%100000000)/10000;
		atm.tm_mon  = nTemp/100 - 1;
		atm.tm_mday = nTemp%100;
		atm.tm_hour = (int)((lDate%10000)/100);
		atm.tm_min  = (int)(lDate%100);
	}
	atm.tm_year -= 1900;
	if(atm.tm_mday < 1 || atm.tm_mday > 31 || 
	   atm.tm_mon  < 0 || atm.tm_mon  > 11 ||
	   atm.tm_hour < 0 || atm.tm_hour > 23 ||
	   atm.tm_min  < 0 || atm.tm_min  > 59 ||
	   atm.tm_sec  < 0 || atm.tm_sec  > 59)
	{
		return -1;
	}
 	return mktime(&atm);
}

HSDouble CSystemFun::ADVANCE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::ASKPRICE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ASKPRICE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	switch( (int)v1 )
	{
	case 1: v1 = 34; break;
	case 2: v1 = 35; break;
	case 3: v1 = 36; break;
	//----2009.11.06 litao���Ӷ�����4��5���̿ڵ��ж�
	case 4: v1 = 45; break;
	case 5: v1 = 46; break;
	//---------------------------------------
	default: return hx_DefMinValues;
	}

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)v1);
	
	//nError = -1;
	//return 0;
}

HSDouble CSystemFun::ASKVOL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	switch( (int)v1 )
	{
	case 1: v1 = 31; break;
	case 2: v1 = 32; break;
	case 3: v1 = 33; break;
	//----2009.11.06 litao���Ӷ�����4��5���̿ڵ��ж�
	case 4: v1 = 47; break;
	case 5: v1 = 48; break;
	//---------------------------------------
	default: return hx_DefMinValues;
	}

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)v1);
	
	//nError = -1;
	//return 0;
}
HSDouble CSystemFun::BIDPRICE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	switch( (int)v1 )
	{
	case 1: v1 = 28; break;
	case 2: v1 = 29; break;
	case 3: v1 = 30; break;
	//----2009.11.06 litao���Ӷ�����4��5���̿ڵ��ж�
	case 4: v1 = 41; break;
	case 5: v1 = 42; break;
	//---------------------------------------
	default: return hx_DefMinValues;
	}

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)v1);
	
	//nError = -1;
	//return 0;
}
HSDouble CSystemFun::BIDVOL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	switch( (int)v1 )
	{
	case 1: v1 = 25; break;
	case 2: v1 = 26; break;
	case 3: v1 = 27; break;
	//----2009.11.06 litao���Ӷ�����4��5���̿ڵ��ж�
	case 4: v1 = 43; break;
	case 5: v1 = 44; break;
	//---------------------------------------
	default: return hx_DefMinValues;
	}

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)v1);
	
	//nError = -1;
	//return 0;
}

HSDouble CSystemFun::BUYVOL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;
	
	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)23);
	
	//nError = -1;
	//return 0;
}

HSDouble CSystemFun::DECLINE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::EXTDATA (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::ISBUYORDER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::SELLVOL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;
	
	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)22);
	
	//nError = -1;
	//return 0;
}

HSDouble CSystemFun::CROSS (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int  nCur,nPos;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			nPos = GetDataValidPos(calc1.m_pData,lSize);
			nCur = __max(nCur,nPos);
			nCur++; // ��һ��
			//if( nCur <= 0 )
			//	nCur = 1;
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur-1].m_dData != hx_DefMinValues &&
					calc1.m_pData[nCur-1].m_dData != hx_DefMinValues &&
					calc2.m_pData[nCur].m_dData   != hx_DefMinValues &&
					calc1.m_pData[nCur].m_dData   != hx_DefMinValues )
				{
					if( (calc1.m_pData[nCur-1] < calc2.m_pData[nCur-1]) &&
						(calc1.m_pData[nCur]   > calc2.m_pData[nCur]) )
					{
						pCurDataStatus->m_pData[nCur] = 1;
					}
					else
					{
						pCurDataStatus->m_pData[nCur] = 0;
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					nCur++;
					for( ; nCur < lSize; nCur++ )
					{
						if( pData[nCur-1].m_dData != hx_DefMinValues &&
							pData[nCur].m_dData   != hx_DefMinValues )
						{
							if( (pData[nCur-1] < v2) &&
								(pData[nCur]   > v2) )
							{
								pCurDataStatus->m_pData[nCur] = 1;
							}
							else
							{
								pCurDataStatus->m_pData[nCur] = 0;
							}
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					nCur++;
					for( ; nCur < lSize; nCur++ )
					{
						if( pData[nCur-1].m_dData != hx_DefMinValues &&
							pData[nCur].m_dData   != hx_DefMinValues )
						{
							if( (pData[nCur-1] > v1) &&
								(pData[nCur]   < v1) )
							{
								pCurDataStatus->m_pData[nCur] = 1;
							}
							else
							{
								pCurDataStatus->m_pData[nCur] = 0;
							}
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;

//	nError = -1;
//	return 0;
}

HSDouble GetCurrentMod(HSDouble v1,HSDouble v2)
{
	return ((long)((double)v1) % (long)((double)v2)) ;
}

HSDouble CSystemFun::MOD (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL ||
		pParam0 == NULL || 
		pParam1 == NULL )
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	
	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		v1 = pCompile->vexp(pParam0,pCurDataStatus);
		v2 = pCompile->vexp(pParam1,pCurDataStatus);
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}		
		if( v2 == 0 )
		{
			return hx_DefMinValues;
		}

		return GetCurrentMod( v1 ,v2 );
	}

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		int i;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			for(i = 0; i < lSize; i++)
			{
				if( calc1.m_pData[i].m_dData != hx_DefMinValues && 
					calc2.m_pData[i].m_dData != hx_DefMinValues &&
					calc2.m_pData[i].m_dData != 0 )
				{
					pCurDataStatus->m_pData[i] = GetCurrentMod(calc1.m_pData[i] ,calc2.m_pData[i]);
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{		
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues && v2 != 0)
						{
							pCurDataStatus->m_pData[i] = GetCurrentMod( pData[i] ,v2 );
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for(i = 0; i < lSize; i++)
					{
						if(pData[i].m_dData != hx_DefMinValues && pData[i].m_dData != 0 )
						{
							pCurDataStatus->m_pData[i] = GetCurrentMod( v1 , pData[i] );
						}
					}
				}
			}
		}
		
		return hx_ReturnPointValues;
	}
	else
	{
		if( v1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		if( v2 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}

		if( v2 == 0 )
		{
			return hx_DefMinValues;
		}

		return GetCurrentMod( v1,v2 );
	}

	return hx_DefMinValues;

	//nError = -1;
	//return 0;
}

HSDouble CSystemFun::NOT (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pRefData,&curStatus);

	if( curStatus.m_pData != NULL )
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = !curStatus.m_pData[nCur];
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::REVERSE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pRefData,&curStatus);

	if( curStatus.m_pData != NULL )
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = (0 - (double)curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::SGN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL  )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pRefData,&curStatus);

	if( curStatus.m_pData != NULL )
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = curStatus.m_pData[nCur] > 0?1:
												curStatus.m_pData[nCur] < 0?-1:0;
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::ACOS (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return acos(var1);
	}

	// ����һ������
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = acos(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::ASIN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return asin(var1);
	}

	// ����һ������
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = asin(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::ATAN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return atan(var1);
	}

	// ����һ������
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = atan(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::COS (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return cos(var1);
	}

	// ����һ������
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = cos(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::EXP (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return exp(var1);
	}

	// ����һ������
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = exp(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::LN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	// e = 2.71828
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pRefData = ayVars->GetParam(0);
	
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pRefData,&curStatus);
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			pCurDataStatus->m_pData[nCur] = log(curStatus.m_pData[nCur])/log(2.71828);
		}
		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if (v1 <= 0)
	{
		return hx_DefMinValues;
	}
	
	return ( log( v1 )/log(2.71828) );
}

HSDouble CSystemFun::LOG (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pRefData = ayVars->GetParam(0);
	
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL				   ||
		pRefData == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pRefData,&curStatus);
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			pCurDataStatus->m_pData[nCur] = log((double)curStatus.m_pData[nCur].m_dData)/log((float)10);
		}
		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if (v1 <= 0)
	{
		return hx_DefMinValues;
	}
	
	return ( log( v1 )/log((float)10) );
	//return 0;
}

HSDouble CSystemFun::POW (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	if(pParam0 == NULL || pParam0 == NULL)
		return hx_DefMinValues;

	long lRefPeriod = (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL)
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&curStatus);
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				pCurDataStatus->m_pData[nCur] = pow(curStatus.m_pData[nCur],(double)lRefPeriod);
			}
		}
		return hx_ReturnPointValues;
	}

	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	
	return pow(v1,lRefPeriod);
}

HSDouble CSystemFun::SIN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::SQRT (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::TAN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::FORCAST (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble sumx = 0,sumn = 0,sumxn = 0,sum2n = 0,averagex,averagen;
		int  nCur,nPos;
		BOOL bValid = FALSE;
		HSDouble slope,b;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);

		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			long lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			nPos = GetDataValidPos(calc1.m_pData,lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					lRefPeriod = calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}

				lRefPeriod = (((lRefPeriod <= 1)?1:lRefPeriod) - 1);
				for(nPos = (lRefPeriod <= 0)?0:(nCur - lRefPeriod); nPos >= 0 && nPos <= nCur; nPos++)
				{
					if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
					{
						sumx  += calc1.m_pData[nPos];
						sumn  += nPos;
						sumxn += nPos*(double)calc1.m_pData[nPos];
						sum2n += nPos*nPos;
						bValid = TRUE;
					}
				}

				if( bValid && lRefPeriod != hx_DefMinValues && lRefPeriod != 0 )
				{
					averagex = sumx / lRefPeriod;
					averagen = sumn / lRefPeriod;
					slope = (lRefPeriod * sumxn - sumx * sumn) / \
						    (lRefPeriod * sum2n - sumn * sumn);

					/*
					sumx  = 0;
					sumn  = 0;
					bValid = FALSE;
					for(nPos = (lRefPeriod <= 0)?0:(nCur - lRefPeriod); nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos] != hx_DefMinValues )
						{
							sumx += calc1.m_pData[nPos];
							sumn += nPos;
							bValid = TRUE;
						}
					}
					*/
					if( bValid )
					{
						//b = (sumx / lRefPeriod) - slope * ( sumn / lRefPeriod );
						b = averagex - slope * averagen;
						pCurDataStatus->m_pData[nCur] = slope.m_dData * nCur + b;//(double)slope * ( lRefPeriod - 1 ) + b;
					}
				}

				sumx  = 0;
				sumn  = 0;
				sumxn = 0;
				sum2n = 0;
				bValid = FALSE;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{
						for( nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								sumx  += pData[nPos];
								sumn  += nPos;
								sumxn += nPos*(double)pData[nPos];
								sum2n += nPos*nPos;
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							averagex = sumx / (v2+1);
							averagen = sumn / (v2+1);
							slope = ((v2+1) * sumxn - sumx * sumn) / \
									((v2+1) * sum2n - sumn * sumn);

							/*
							sumx  = 0;
							sumn  = 0;
							bValid = FALSE;
							for( nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos] != hx_DefMinValues )
								{
									sumx += pData[nPos];
									sumn += nPos;
									bValid = TRUE;
								}
							}
							*/
							if( bValid )
							{
								//b = (sumx / (v2+1)) - slope * ( sumn / (v2+1) );
								b = averagex - slope * averagen;
								pCurDataStatus->m_pData[nCur] = slope.m_dData * nCur + b;
							}
						}
						
						sumx  = 0;
						sumn  = 0;
						sumxn = 0;
						sum2n = 0;
						bValid = FALSE;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{		
						for( nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								sumx  += pData[nPos];
								sumn  += nPos;
								sumxn += nPos*(double)pData[nPos];
								sum2n += nPos*nPos;
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							averagex = sumx / (v1+1);
							averagen = sumn / (v1+1);
							slope = ((v1+1) * sumxn - sumx * sumn) / \
									((v1+1) * sum2n - sumn * sumn);

							/*
							sumx  = 0;
							sumn  = 0;
							bValid = FALSE;
							for( nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos] != hx_DefMinValues )
								{
									sumx += pData[nPos];
									sumn += nPos;
									bValid = TRUE;
								}
							}
							*/

							if( bValid )
							{
								//b = (sumx / (v1+1)) - slope * ( sumn / (v1+1) );
								b = averagex - slope * averagen;
								pCurDataStatus->m_pData[nCur] = slope.m_dData * nCur + b;//slope * ( (v1+1) - 1 ) + b;
							}
						}

						sumx  = 0;
						sumn  = 0;
						sumxn = 0;
						sum2n = 0;
						bValid = FALSE;
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::SLOPE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble sumx = 0,sumn = 0,sumxn = 0,sum2n = 0,averagex,averagen;
		int  nCur,nPos;
		BOOL bValid = FALSE;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);

		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			long lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			nPos = GetDataValidPos(calc1.m_pData,lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					lRefPeriod = calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}

				lRefPeriod = (((lRefPeriod <= 1)?1:lRefPeriod) - 1);
				for(nPos = (lRefPeriod <= 0)?0:(nCur - lRefPeriod); nPos >= 0 && nPos <= nCur; nPos++)
				{
					if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
					{
						sumx  += calc1.m_pData[nPos];
						sumn  += nPos;
						sumxn += nPos*(double)calc1.m_pData[nPos];
						sum2n += nPos*nPos;
						bValid = TRUE;
					}
				}

				if( bValid && lRefPeriod != hx_DefMinValues && lRefPeriod != 0 )
				{
					//averagex = sumx / lRefPeriod;
					//averagen = sumn / lRefPeriod;
					pCurDataStatus->m_pData[nCur] = (lRefPeriod * sumxn - sumx * sumn) / \
												    (lRefPeriod * sum2n - sumn * sumn);
				}

				sumx  = 0;
				sumn  = 0;
				sumxn = 0;
				sum2n = 0;
				bValid = FALSE;
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{
						for( nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								sumx  += pData[nPos];
								sumn  += nPos;
								sumxn += nPos*(double)pData[nPos];
								sum2n += nPos*nPos;
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							/*averagex = sumx / (v2+1);
							averagen = sumn / (v2+1);*/
							pCurDataStatus->m_pData[nCur] = ((v2+1) * sumxn - sumx * sumn) / \
															((v2+1) * sum2n - sumn * sumn);
						}
						
						sumx  = 0;
						sumn  = 0;
						sumxn = 0;
						sum2n = 0;
						bValid = FALSE;
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{		
						for( nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
						{
							if( pData[nPos].m_dData != hx_DefMinValues )
							{
								sumx  += pData[nPos];
								sumn  += nPos;
								sumxn += nPos*(double)pData[nPos];
								sum2n += nPos*nPos;
								bValid = TRUE;
							}
						}
						if( bValid )
						{
							//averagex = sumx / (v1+1);
							//averagen = sumn / (v1+1);
							pCurDataStatus->m_pData[nCur] = ((v1+1) * sumxn - sumx * sumn) / \
															((v1+1) * sum2n - sumn * sumn);
						}

						sumx  = 0;
						sumn  = 0;
						sumxn = 0;
						sum2n = 0;
						bValid = FALSE;
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::COST (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::PEAK (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::PEAKBARS (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::SARTURN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::TROUGH (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::TROUGHBARS (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

//-------2009.12.02 litao������ƽ���ֲ�����ĳɱ���---------------------------------------------------
HSDouble CSystemFun::WINNER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	
	if(pParam0 == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;

	HSDouble* pHighDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);
	HSDouble* pVolDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_VOL_DATA);

	//ȡ���ݵĳ���
	int lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
	if (lSize < 1 ) return hx_DefMinValues;

	lSize = max(lSize,curStatus.m_lSize) ;

	//ȡ�ܹɱ�
    double lCapital = GetData(pCompile,ayVars,pCurDataStatus, nError)->CAPITAL()/10000.0;
	
	pCurDataStatus->Alloc(lSize);

	//�����Ż�����������
	//CArray<long*, long*> *payCostData;
	double fSumVol=0.0; double fWinVol=0.0;

	if(curStatus.m_pData != NULL)
	{
		//�����һ��Ļ�����
		if(pHighDay[0].m_dData < curStatus.m_pData[0])
			pCurDataStatus->m_pData[0] = 1.0;
		else if(pLowDay[0].m_dData > curStatus.m_pData[0])
			pCurDataStatus->m_pData[0] = 0.0;
		else
		{ 
			if((pHighDay[0].m_dData - pLowDay[0].m_dData)>0.00001)
				pCurDataStatus->m_pData[0] = (curStatus.m_pData[0]-pLowDay[0].m_dData)/(pHighDay[0].m_dData - pLowDay[0].m_dData) ;
			else
				pCurDataStatus->m_pData[0] = 1.0;
		}

		for( int nCur = 1; nCur < lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
				continue;

			fSumVol = 0.0; fWinVol = 0.0;

			//�ӵڶ��쿪ʼ��������,����Ŀ�ʼ�Ǵ��ۼƻ����ʴﵽ100%��������
			for(int i=nCur; i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < curStatus.m_pData[nCur])
					fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;
				else if(pLowDay[i].m_dData > curStatus.m_pData[nCur])
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(curStatus.m_pData[nCur] - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if (fSumVol > lCapital)
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}
	else  //������Ϊ����ʱ
	{
		double nPrice = double(pCompile->vexp(pParam0,&curStatus));

		if (nPrice < 0.0001) return hx_DefMinValues;

		//�����һ��Ļ�����
		if(pHighDay[0].m_dData < nPrice)
			pCurDataStatus->m_pData[0] = 1.0;
		else if(pLowDay[0].m_dData > nPrice)
			pCurDataStatus->m_pData[0] = 0.0;
		else
		{ 
			if((pHighDay[0].m_dData - pLowDay[0].m_dData)>0.00001)
				pCurDataStatus->m_pData[0] = (nPrice-pLowDay[0].m_dData)/(pHighDay[0].m_dData - pLowDay[0].m_dData) ;
			else
				pCurDataStatus->m_pData[0] = 1.0;
		}

		for( int nCur = 1; nCur < lSize; nCur++ )
		{
			fSumVol = 0.0; fWinVol = 0.0;

			//�ӵ���������ǰ��ʼ��������
			for(int i=nCur; i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < nPrice)
					fWinVol = fWinVol + pVolDay[i]/10000.0;
				else if(pLowDay[i].m_dData > nPrice)
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(nPrice - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if (fSumVol > lCapital)
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

//���ڻ����̱���
HSDouble CSystemFun::LWINNER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);

	if((pParam0 == NULL ) || (pParam1 == NULL ))
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pParam1,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;
	
	int nDays = int(pCompile->vexp(pParam0,&curStatus));
	if(nDays <= 0) 
		return hx_DefMinValues;

	HSDouble* pHighDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);
	HSDouble* pVolDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_VOL_DATA);

	//ȡ���ݵĳ���
	int lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
	if (lSize < 1 ) return hx_DefMinValues;

	lSize = max(lSize,curStatus.m_lSize) ;

	//ȡ�ܹɱ�
    double lCapital = GetData(pCompile,ayVars,pCurDataStatus, nError)->CAPITAL()/10000.0;
	
	pCurDataStatus->Alloc(lSize);

	//�����Ż�����������
	//CArray<long*, long*> *payCostData;
	double fSumVol=0.0; double fWinVol=0.0;

	if(curStatus.m_pData != NULL)
	{
		//�����һ��Ļ�����
		if(pHighDay[0].m_dData < curStatus.m_pData[0])
			pCurDataStatus->m_pData[0] = 1.0;
		else if(pLowDay[0].m_dData > curStatus.m_pData[0])
			pCurDataStatus->m_pData[0] = 0.0;
		else
		{ 
			if((pHighDay[0].m_dData - pLowDay[0].m_dData)>0.00001)
				pCurDataStatus->m_pData[0] = (curStatus.m_pData[0]-pLowDay[0].m_dData)/(pHighDay[0].m_dData - pLowDay[0].m_dData) ;
			else
				pCurDataStatus->m_pData[0] = 1.0;
		}

		for( int nCur = 1; nCur < lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
				continue;

			fSumVol = 0.0; fWinVol = 0.0;

			//�ӵڶ��쿪ʼ��������,����Ŀ�ʼ�Ǵ��ۼ�nDays
			for(int i=nCur; i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < curStatus.m_pData[nCur])
					fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;
				else if(pLowDay[i].m_dData > curStatus.m_pData[nCur])
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(curStatus.m_pData[nCur] - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if ((nCur-i+1) >= (nDays))
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}
	else  //������Ϊ����ʱ
	{
		double nPrice = double(pCompile->vexp(pParam0,&curStatus));

		if (nPrice < 0.0001) return hx_DefMinValues;

		//�����һ��Ļ�����
		if(pHighDay[0].m_dData < nPrice)
			pCurDataStatus->m_pData[0] = 1.0;
		else if(pLowDay[0].m_dData > nPrice)
			pCurDataStatus->m_pData[0] = 0.0;
		else
		{ 
			if((pHighDay[0].m_dData - pLowDay[0].m_dData)>0.00001)
				pCurDataStatus->m_pData[0] = (nPrice-pLowDay[0].m_dData)/(pHighDay[0].m_dData - pLowDay[0].m_dData) ;
			else
				pCurDataStatus->m_pData[0] = 1.0;
		}

		for( int nCur = 1; nCur < lSize; nCur++ )
		{
			fSumVol = 0.0; fWinVol = 0.0;

			//�ӵ���������ǰ��ʼ��������
			for(int i=nCur; i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < nPrice)
					fWinVol = fWinVol + pVolDay[i]/10000.0;
				else if(pLowDay[i].m_dData > nPrice)
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(nPrice - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if ((nCur-i+1) >= (nDays))
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

//Զ�ڻ����̱���
HSDouble CSystemFun::PWINNER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);

	if((pParam0 == NULL ) || (pParam1 == NULL ))
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if(pCompile->vexp(pParam1,&curStatus) == hx_DefMinValues)
		return hx_DefMinValues;
	
	int nDays = int(pCompile->vexp(pParam0,&curStatus));
	if(nDays < 0) 
		return hx_DefMinValues;

	HSDouble* pHighDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);
	HSDouble* pVolDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_VOL_DATA);

	//ȡ���ݵĳ���
	int lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize();
	if (lSize < 1 ) return hx_DefMinValues;

	lSize = max(lSize,curStatus.m_lSize) ;

	//ȡ�ܹɱ�
    double lCapital = GetData(pCompile,ayVars,pCurDataStatus, nError)->CAPITAL()/10000.0;
	
	pCurDataStatus->Alloc(lSize);

	//�����Ż�����������
	//CArray<long*, long*> *payCostData;
	double fSumVol=0.0; double fWinVol=0.0;

	if(curStatus.m_pData != NULL)
	{
		//ǰNǰ�ڳ�ʼ������ʾ����
		for ( int i = 0; i < nDays; i++ )
		{
			if (i >= lSize) break;
			pCurDataStatus->m_pData[i] = hx_DefMinValues ;
		}

		for( int nCur = nDays; nCur < lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
				continue;

			fSumVol = 0.0; fWinVol = 0.0;

			//�ӵڶ��쿪ʼ��������,����Ŀ�ʼ�Ǵ��ۼƻ����ʴﵽ100%��������
			for(int i=(nCur-nDays); i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < curStatus.m_pData[nCur])
					fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;
				else if(pLowDay[i].m_dData > curStatus.m_pData[nCur])
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(curStatus.m_pData[nCur] - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if (fSumVol > lCapital)
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}
	else  //������Ϊ����ʱ
	{
		double nPrice = double(pCompile->vexp(pParam0,&curStatus));

		if (nPrice < 0.0001) return hx_DefMinValues;

		//ǰN���ڳ�ʼ������ʾ����
		for ( int i = 0; i < nDays; i++ )
		{
			if (i >= lSize) break;
			pCurDataStatus->m_pData[i] = hx_DefMinValues ;
		}

		for( int nCur = nDays; nCur < lSize; nCur++ )
		{
			fSumVol = 0.0; fWinVol = 0.0;

			//�ӵ���������ǰ��ʼ��������
			for(int i=(nCur-nDays); i >=0 ; i--)
			{
				fSumVol = fSumVol + pVolDay[i].m_dData/10000.0;

				if(pHighDay[i].m_dData < nPrice)
					fWinVol = fWinVol + pVolDay[i]/10000.0;
				else if(pLowDay[i].m_dData > nPrice)
					fWinVol = fWinVol + 0.0;
				else
				{
					if((pHighDay[i].m_dData - pLowDay[i].m_dData)>0.00001)
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0 *(nPrice - pLowDay[i].m_dData)/(pHighDay[i].m_dData - pLowDay[i].m_dData) ;
					else
						fWinVol = fWinVol + pVolDay[i].m_dData/10000.0;	
				}
				if (fSumVol > lCapital)
					break;
			}
			if(fSumVol>0.00001)
				pCurDataStatus->m_pData[nCur] = fWinVol/fSumVol ;
			else
				pCurDataStatus->m_pData[nCur] = 1.0;		
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}
//---------------------------------------------------------------------------------------------------------------------------------

HSDouble CSystemFun::ZIG (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXA (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXADV (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXC (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXDEC (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXH (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXO (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::INDEXV (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::CAPITAL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	// CAPITAL��������ͨ�̴�С����λΪ�֡�����A�ɵõ���ͨA�ɣ�B�ɵõ�B���ܹɱ���ָ��Ϊ0

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->CAPITAL();
}

HSDouble CSystemFun::VOLUNIT (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::DYNAINFO (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetNowDataByCode((int)v1);
	//return 0;
}

HSDouble CSystemFun::FINANCE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ADVANCE(var1) ���Ǽ���
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pParam0 == NULL )
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc1(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);

	return GetData(pCompile,ayVars,pCurDataStatus, nError)->GetFinaceDataByPos((int)v1);
}

// �û���ͼ����
HSDouble CSystemFun::DRAWICON(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 1)
	{
		nError = 0;
		return 0;
	}

	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);
	LPNODELEMENT pParam2 = ayVars->GetParam(2);

	if( pParam0 == NULL ||
		pParam1 == NULL ||
		pParam2 == NULL )
		return 0;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	double v3 = pCompile->vexp(pParam2,&calc3);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDrawOtherData* pHSDrawOtherData;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;

		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			memcpy(pCurDataStatus->m_pData,calc2.m_pData,sizeof(*pCurDataStatus->m_pData)*lSize);

			for( int nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					if( calc1.m_pData[nCur] )
					{
						pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
						//pHSDrawOtherData = new HSDrawOtherData();
						pHSDrawOtherData->m_bType1 = HS_DRAWICON;
						pHSDrawOtherData->m_bType2 = v3;
						pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues )
						{
							if( pData[nCur] )
							{
								pCurDataStatus->m_pData[nCur] = v2;
								
								pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
								//pHSDrawOtherData = new HSDrawOtherData();
								pHSDrawOtherData->m_bType1 = HS_DRAWICON;
								pHSDrawOtherData->m_bType2 = v3;
								pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
							}
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					memcpy(pCurDataStatus->m_pData,pData,sizeof(*pCurDataStatus->m_pData)*lSize);

					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues )
						{
							pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
							//pHSDrawOtherData = new HSDrawOtherData();
							pHSDrawOtherData->m_bType1 = HS_DRAWICON;
							pHSDrawOtherData->m_bType2 = v3;
							pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
						}
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::DRAWLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 1)
	{
		nError = 0;
		return 0;
	}

	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);
	LPNODELEMENT pParam2 = ayVars->GetParam(2);
	LPNODELEMENT pParam3 = ayVars->GetParam(3);
	LPNODELEMENT pParam4 = ayVars->GetParam(4);

	if( pParam0 == NULL ||
		pParam1 == NULL ||
		pParam2 == NULL ||
		pParam3 == NULL ||
		pParam4 == NULL)
		return 0;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	HSCurDataInfoStatus calc4(pCurDataStatus);
	HSCurDataInfoStatus calc5(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	double v3 = pCompile->vexp(pParam2,&calc3);
	double v4 = pCompile->vexp(pParam3,&calc4);
	double v5 = pCompile->vexp(pParam4,&calc5);

	if( calc1.m_pData != NULL && calc2.m_pData != NULL &&
		calc3.m_pData != NULL && calc4.m_pData != NULL )
	{
		HSDrawOtherData* pHSDrawOtherData;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;

		for( int nCur = 0; nCur < lSize; nCur++ ) 
		{
			if( calc1.m_pData[nCur].m_dData != hx_DefMinValues )
			{
				if( (double)calc1.m_pData[nCur] && 
					calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					pCurDataStatus->m_pData[nCur] = calc2.m_pData[nCur];
					
					pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
					pHSDrawOtherData->m_bType1 = HS_DRAWLINE;
					pHSDrawOtherData->m_bType4 = (BYTE)v5;    // ����:0.�����ӳ���1.�ӳ���ʼ�㣻2.�ӳ���ֹ�㣻3.��ʼ����ֹ��ͬʱ�ӳ�
					pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
					
					nCur++;
					for( ; nCur < lSize; nCur++ ) 
					{
						if( calc3.m_pData[nCur].m_dData != hx_DefMinValues )
						{
							if( (double)calc3.m_pData[nCur] && 
								calc3.m_pData[nCur].m_dData != hx_DefMinValues )
							{
								pHSDrawOtherData->m_bType2 = LOBYTE((short)nCur);            // �������
								pHSDrawOtherData->m_bType3 = HIBYTE((short)nCur);
								pHSDrawOtherData->m_dValue = calc4.m_pData[nCur];
								break;
							}
						}
					}

				}
			}
		}
		
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::DRAWTEXT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 1)
	{
		nError = 0;
		return 0;
	}

	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);
	LPNODELEMENT pParam2 = ayVars->GetParam(2);

	if( pParam0 == NULL ||
		pParam1 == NULL ||
		pParam2 == NULL )
		return 0;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	double v3 = pCompile->vexp(pParam2,&calc3);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDrawOtherData* pHSDrawOtherData;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;

		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{
			memcpy(pCurDataStatus->m_pData,calc2.m_pData,sizeof(*pCurDataStatus->m_pData)*lSize);

			for( int nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					if( calc1.m_pData[nCur] )
					{
						pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
						//pHSDrawOtherData = new HSDrawOtherData();
						pHSDrawOtherData->m_bType1 = HS_DRAWTEXT;
						if( (pParam2->cDataType & DataTypeOperStr) && 
							(pParam2->cDataType & DataText) &&
							pParam2->valoarestr )
						{
							//if( HSDrawData::m_pHSExternAlloc )
                               // pHSDrawOtherData->m_strText = (CString*)(*HSDrawData::m_pHSExternAlloc)((long)pParam2->valoarestr,HSCStringCopy);//new CString( *pParam2->valoarestr );
							//pHSDrawOtherData->m_strText.Format("%s", *(pParam2->valoarestr));	
							memcpy(pHSDrawOtherData->m_cText, pParam2->valoarestr->GetBuffer(),min(sizeof(pHSDrawOtherData->m_cText)-1,pParam2->valoarestr->GetLength()));
						}

						pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues )
						{
							if( pData[nCur] )
							{
								pCurDataStatus->m_pData[nCur] = v2;
								
								pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
								//pHSDrawOtherData = new HSDrawOtherData();
								pHSDrawOtherData->m_bType1 = HS_DRAWTEXT;
								if( (pParam2->cDataType & DataTypeOperStr) && 
									(pParam2->cDataType & DataText) &&
									pParam2->valoarestr )
								{
									//if( HSDrawData::m_pHSExternAlloc )
									//	pHSDrawOtherData->m_strText = (CString*)(*HSDrawData::m_pHSExternAlloc)((long)pParam2->valoarestr,HSCStringCopy);
									//pHSDrawOtherData->m_strText.Format("%s", *(pParam2->valoarestr));
									memcpy(pHSDrawOtherData->m_cText, pParam2->valoarestr->GetBuffer(),min(sizeof(pHSDrawOtherData->m_cText)-1,pParam2->valoarestr->GetLength()));
								}
								
								pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
							}
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					memcpy(pCurDataStatus->m_pData,pData,sizeof(*pCurDataStatus->m_pData)*lSize);

					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues )
						{
							pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
							//pHSDrawOtherData = new HSDrawOtherData();
							pHSDrawOtherData->m_bType1 = HS_DRAWTEXT;
							if( (pParam2->cDataType & DataTypeOperStr) && 
								(pParam2->cDataType & DataText) &&
								pParam2->valoarestr )
							{
								//if( HSDrawData::m_pHSExternAlloc )
								//	pHSDrawOtherData->m_strText = (CString*)(*HSDrawData::m_pHSExternAlloc)((long)pParam2->valoarestr,HSCStringCopy);
								//pHSDrawOtherData->m_strText.Format("%s", *(pParam2->valoarestr));	
								memcpy(pHSDrawOtherData->m_cText, pParam2->valoarestr->GetBuffer(),min(sizeof(pHSDrawOtherData->m_cText)-1,pParam2->valoarestr->GetLength()));
							}
							
							pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
						}
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::POLYLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;

		HSDrawOtherData* pHSDrawOtherData;
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			for( int nCur = 0; nCur < lSize; nCur++ )
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues && 
					calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					if( calc1.m_pData[nCur] )
					{
						pCurDataStatus->m_pData[nCur] = calc2.m_pData[nCur];
						pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
						//pHSDrawOtherData = new HSDrawOtherData();
						pHSDrawOtherData->m_bType1 = HS_POLYLINE;
						pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
					}
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				if( v2 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues && 
							v2 != hx_DefMinValues )
						{
							if( pData[nCur] )
							{
								pCurDataStatus->m_pData[nCur] = v2;
								pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
								//pHSDrawOtherData = new HSDrawOtherData();
								pHSDrawOtherData->m_bType1 = HS_POLYLINE;
								pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
							}
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					for( int nCur = 0; nCur < lSize; nCur++ )
					{
						if( pData[nCur].m_dData != hx_DefMinValues && v1 != hx_DefMinValues)
						{
							if( pData[nCur] )
							{
								pCurDataStatus->m_pData[nCur] = v1;
								pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
								//pHSDrawOtherData = new HSDrawOtherData();
								pHSDrawOtherData->m_bType1 = HS_POLYLINE;
								pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
							}
						}
					}
				}
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::STICKLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				   HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 1)
	{
		nError = 0;
		return 0;
	}

	LPNODELEMENT pParam0 = ayVars->GetParam(0);
	LPNODELEMENT pParam1 = ayVars->GetParam(1);
	LPNODELEMENT pParam2 = ayVars->GetParam(2);
	LPNODELEMENT pParam3 = ayVars->GetParam(3);
	LPNODELEMENT pParam4 = ayVars->GetParam(4);

	if( pParam0 == NULL ||
		pParam1 == NULL ||
		pParam2 == NULL ||
		pParam3 == NULL ||
		pParam4 == NULL)
		return 0;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	HSCurDataInfoStatus calc4(pCurDataStatus);
	HSCurDataInfoStatus calc5(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);
	double v3 = pCompile->vexp(pParam2,&calc3);
	double v4 = pCompile->vexp(pParam3,&calc4);
	double v5 = pCompile->vexp(pParam4,&calc5);

	if( calc1.m_pData != NULL )
	{
		HSDrawOtherData* pHSDrawOtherData;

		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;

		if( calc2.m_pData != NULL || calc3.m_pData != NULL )
		{
			HSDouble* pData = ((calc2.m_pData != NULL)?calc2.m_pData:calc3.m_pData);
			for( int nCur = 0; nCur < lSize; nCur++ ) 
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues && (double)calc1.m_pData[nCur] )
				{
					if( calc2.m_pData != NULL && calc3.m_pData != NULL )
					{
						if( calc2.m_pData[nCur].m_dData != hx_DefMinValues &&
							calc3.m_pData[nCur].m_dData != hx_DefMinValues )
						{
							pCurDataStatus->m_pData[nCur] = calc2.m_pData[nCur];
							
							pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
							//pHSDrawOtherData = new HSDrawOtherData();
							pHSDrawOtherData->m_bType1 = HS_STICKLINE;
							pHSDrawOtherData->m_bType2 = (BYTE)v4;
							pHSDrawOtherData->m_bType3 = (BYTE)v5;
							pHSDrawOtherData->m_dValue = calc3.m_pData[nCur];
							pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
						}
					}
					else
					{
						pCurDataStatus->m_pData[nCur] = pData[nCur];
						
						pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
						//pHSDrawOtherData = new HSDrawOtherData();
						pHSDrawOtherData->m_bType1 = HS_STICKLINE;
						pHSDrawOtherData->m_bType2 = (BYTE)v4;
						pHSDrawOtherData->m_bType3 = (BYTE)v5;
						pHSDrawOtherData->m_dValue = (calc2.m_pData != NULL)?v3:v2;
						pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
					}
				}
			}
		}
		else
		{
			for( int nCur = 0; nCur < lSize; nCur++ ) 
			{
				if( calc1.m_pData[nCur].m_dData != hx_DefMinValues && (double)calc1.m_pData[nCur] )
				{
					pCurDataStatus->m_pData[nCur] = (nCur%2)?v2:v3;
					
					pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
					//pHSDrawOtherData = new HSDrawOtherData();
					pHSDrawOtherData->m_bType1 = HS_STICKLINE;
					pHSDrawOtherData->m_bType2 = (BYTE)v4;
					pHSDrawOtherData->m_bType3 = (BYTE)v5;
					pHSDrawOtherData->m_dValue = (nCur%2)?v3:v2;
					pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
				}
			}
		}
		
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

CHSDataSource* CSystemFun::GetDataSource(CExpressionBase* pCompile)
{
	if( pCompile == NULL )
		return NULL;
	return pCompile->GetData();
}

#define Day_Len_SectionCacl		5
#define Vol_OneDay_SectionCacl	0
HSDouble CSystemFun::Get5DayVol(CHSDataSource* pHxData, int nDayPos)
{
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	if( nDayPos < Day_Len_SectionCacl )
		return Vol_OneDay_SectionCacl;

	HSDouble* pVol = pHxData->GetData(hx_VOL_DATA);
	int nCount = pHxData->GetDaySize();
	if( pVol == NULL || nCount <= Day_Len_SectionCacl 
		|| nDayPos >= nCount )
		return (double)hx_DefMinValues;

	HSDouble* pCurPos = pVol + nDayPos;
	int nEnd = Day_Len_SectionCacl;

	HSDouble dReturn = 0;
	for( int i = 0; i < nEnd && pCurPos != NULL; i++, pCurPos--)
	{
		dReturn += pCurPos->m_dData;
	}

	return dReturn.m_dData / nEnd;
}

HSDouble CSystemFun::GetHisLIANGBI(CHSDataSource* pHxData, int nDayPos)
{
	HSDouble d5DayVol = Get5DayVol(pHxData, nDayPos-1);
	if( d5DayVol == (double)hx_DefMinValues )
		return 0;

	HSDouble* pVol = pHxData->GetData(hx_VOL_DATA);

	if(d5DayVol > 0 && fabs(d5DayVol.m_dData) > 0.0001 )
	{
		return (pVol[nDayPos].m_dData/d5DayVol.m_dData);
	}
	else
	{
		return 0;
	}
}

//  n��ǿ��
HSDouble CSystemFun::QIANGDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	int nDays = 5;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;
		}
	}

	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nDayCount = pHxData->GetDaySize();
	if( nDayCount <= nDays )
	{
		nDays = 0;
	}
	else
	{
		nDays = nDayCount - nDays;
	}
	HSDouble* pClose   = pHxData->GetData(hx_CLOSE_DATA);
	HSDouble dPreClose = -1;
	double dUpAmount = 0;
	double dDownAmount = 0;

	for( ; nDays < nDayCount && pClose != NULL; nDays++ )
	{
		if( pClose[nDays].m_dData != (double)hx_DefMinValues )
		{
			if( dPreClose == -1 )
			{
				dPreClose = pClose[nDays];
			}
			else
			{
				double dTemp = pClose[nDays].m_dData - dPreClose.m_dData;
				if( dTemp > 0 )
				{
					dUpAmount += dTemp;
				}
				else
				{
					dDownAmount += dTemp;
				}
				dPreClose = pClose[nDays];
			}
		}
	}

	if( fabs(dUpAmount-dDownAmount) < 0.0001 ) 
	{
		return (double)hx_DefMinValues;
	}

	return (dUpAmount+dDownAmount) / (dUpAmount-dDownAmount) * 100;
	return dUpAmount / (dUpAmount-dDownAmount) * 100;
}

//   �ǵ�����"
HSDouble CSystemFun::ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	HSDouble dOpen = BetweenOpen(pCompile, ayVars, pCurDataStatus, nError);
	HSDouble dClose = BetweenClose(pCompile, ayVars, pCurDataStatus, nError);
	if( dOpen == (double)hx_DefMinValues 
		|| dClose == (double)hx_DefMinValues 
		|| fabs(dOpen.m_dData) <= 0.0001 )
	{
		return (double)hx_DefMinValues;
	}

	return (dClose.m_dData - dOpen.m_dData)*100/dOpen.m_dData;
}

//   ������
HSDouble CSystemFun::HUANSHOULV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	HSDouble dRetCapital = pHxData->CAPITAL();
	if ( dRetCapital == (double)hx_DefMaxValues || fabs(dRetCapital.m_dData) <= 0.0001 )
	{
		return (double)hx_DefMinValues;
	}

	HSDouble dBetweenVol = BetweenVol(pCompile, ayVars, pCurDataStatus, nError);
	if( dBetweenVol == (double)hx_DefMaxValues )
	{
		return (double)hx_DefMinValues;
	}

	return (100.0*dBetweenVol.m_dData/dRetCapital.m_dData);
}

//   ���ȷ���"
HSDouble CSystemFun::LIANGBIFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nDays = pHxData->GetDaySize();
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;
		}
	}
	HSDouble dLiangBi;
	HSDouble dAmount = 0;
	int nDayCount = pHxData->GetDaySize();
	int nCount = min(nDayCount, nDays);
	int nValidDays = 0;
	nDayCount--;

	for( int i = 0; i < nCount; i++ )
	{
		dLiangBi = GetHisLIANGBI(pHxData, nDayCount-i);
		if( dLiangBi.m_dData != 0 )
		{
			dAmount += dLiangBi;
			nValidDays++;
		}
	}

	if( nValidDays == 0 )
	{
		return (double)hx_DefMinValues;
	}

	dLiangBi = dAmount.m_dData / (nValidDays);

	return dLiangBi;
}

//   �𵴷���"
HSDouble CSystemFun::ZHENGDANGFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

#ifndef Super_V1_50_ZZF
	HSDouble dOpenPrice = BetweenOpen(pCompile, ayVars, pCurDataStatus, nError);
#endif
	HSDouble dMaxPrice = BetweenHigh(pCompile, ayVars, pCurDataStatus, nError);
	HSDouble dMinPrice = BetweenLow(pCompile, ayVars, pCurDataStatus, nError);

	if( dMaxPrice == (double)hx_DefMinValues || (double)hx_DefMinValues == dMinPrice )
		return (double)hx_DefMinValues;

#ifndef Super_V1_50_ZZF
	if( dOpenPrice == (double)hx_DefMinValues 
		|| fabs(dOpenPrice.m_dData) <= 0.0001 )
		return (double)hx_DefMinValues;
#else
	if( fabs(dMinPrice.m_dData) <= 0.0001 )
		return (double)hx_DefMinValues;
#endif

#ifndef Super_V1_50_ZZF
	return (dMaxPrice.m_dData - dMinPrice.m_dData)/(dOpenPrice.m_dData)*100;
#else
	return (dMaxPrice.m_dData - dMinPrice.m_dData)/(dMinPrice.m_dData)*100;
#endif
}

//   �����г�����
HSDouble CSystemFun::BetweenShiChangBiLi(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	HSDouble dFinace = pHxData->CAPITAL();
	HSDouble dBetweenVol = BetweenVol(pCompile, ayVars, pCurDataStatus, nError);
	if( (double)hx_DefMaxValues == dFinace 
		|| dFinace == (double)hx_DefMinValues 
		|| dBetweenVol == (double)hx_DefMinValues 
		|| fabs(dFinace.m_dData) <= 0.0001 )
	{
		return (double)hx_DefMinValues;
	}

#ifdef _DEBUG
	if( dFinace.m_dData < 0 )
	{
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T("�����г����� ��ͨ�̲���ȷ"));
	}
	if( dBetweenVol.m_dData < 0 )
	{
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T("�����г����� ����ɽ�������ȷ"));
	}
#endif

	return (dBetweenVol.m_dData)*100/dFinace.m_dData;
}

//   ���俪��
HSDouble CSystemFun::BetweenOpen(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nDayCount = pHxData->GetDaySize();
	int nDays = nDayCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;
		}
	}

	int nReturnDays = nDayCount - nDays;

	if( nReturnDays < 0 )
		return (double)hx_DefMinValues;

	if( nDayCount > nReturnDays )
	{
		HSDouble* pOpen   = pHxData->GetData(hx_OPEN_DATA);
		if( pOpen != NULL )
			return pOpen[nReturnDays];
	}

	return (double)hx_DefMinValues;
}

//   ��������
HSDouble CSystemFun::BetweenClose(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nDayCount = pHxData->GetDaySize(); //10
	//int nDays = nDayCount;//10
	//if( ayVars != NULL && ayVars->GetSize() >= 1 )
	//{
	//	LPNODELEMENT pItem = ayVars->GetParam(0);
	//	if( pItem != NULL )
	//	{
	//		nDays = (int)pItem->valoare;//1
	//	}
	//}
	//else
	//{
	//	nDays = 1;
	//}

	int nReturnDays = nDayCount - 1;//9

	if( nReturnDays < 0 )
		return (double)hx_DefMinValues;

	if( nDayCount > nReturnDays )
	{
		HSDouble* pOpen   = pHxData->GetData(hx_CLOSE_DATA);
		if( pOpen != NULL )
			return pOpen[nReturnDays];
	}

	return (double)hx_DefMinValues;
}

//   �������
HSDouble CSystemFun::BetweenHigh(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();//10
	int nDays = nCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;//1
		}
	}

	nDays = min(nDays, nCount);
	HSDouble dReturn = (double)hx_DefMinValues;
	HSDouble* pHigh   = pHxData->GetData(hx_HIGH_DATA);
	pHigh += (nCount-1);

	for( int i = 0; i < nDays && pHigh != NULL; i++ )
	{
		if( (dReturn < *pHigh || dReturn == (double)hx_DefMinValues) && *pHigh != (double)hx_DefMinValues )
		{
			dReturn = *pHigh;
		}
		pHigh--;
	}

	return dReturn;
}

//   �������
HSDouble CSystemFun::BetweenLow(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();//10
	int nDays = nCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;//1
		}
	}
	nDays = min(nDays, nCount);
	HSDouble dReturn = (double)hx_DefMinValues;
	HSDouble* pHigh   = pHxData->GetData(hx_LOW_DATA);
	pHigh += (nCount-1);

	for( int i = 0; i < nDays && pHigh != NULL; i++ )
	{
		if( (dReturn > *pHigh || dReturn == (double)hx_DefMinValues) && *pHigh != (double)hx_DefMinValues )
		{
			dReturn = *pHigh;
		}
		pHigh--;
	}

	return dReturn;
}

//   ����ɽ���
HSDouble CSystemFun::BetweenVol(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();//10
	int nDays = nCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;//1
		}
	}

	nDays = nCount - nDays;
	if( nDays < 0 )
		nDays = 0;
	HSDouble dReturn = 0;
	HSDouble* pVol = pHxData->GetData(hx_VOL_DATA);
	pVol += nDays;
	for( int i = nDays; i < nCount && pVol != NULL; i++, pVol++ )
	{
		if( *pVol != (double)hx_DefMinValues )
			dReturn += *pVol;
	}

#ifdef _DEBUG
	if( dReturn.m_dData < 0 )
	{
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T("����ɽ��� ����0����ȷ"));
	}
#endif

	return dReturn;
}

//   ����ɽ���
HSDouble CSystemFun::BetweenAmount(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();//10
	int nDays = nCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;//1
		}
	}

	nDays = nCount - nDays;
	if( nDays < 0 ) nDays = 0;
	HSDouble dReturn = 0;
	HSDouble* pVol = pHxData->GetData(hx_AMOUNT_DATA);
	pVol += nDays;
	for( int i = nDays; i < nCount && pVol != NULL; i++, pVol++ )
	{
		if( *pVol != (double)hx_DefMinValues )
			dReturn += *pVol;
	}

	//dReturn.m_dData *= 1000;

#ifdef _DEBUG
	if( dReturn.m_dData < 0 )
	{
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T("����ɽ��� ����0����ȷ"));
	}
#endif

	return dReturn;
}

// ���ļ�
HSDouble CSystemFun::CenterPrice(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	return (double)hx_DefMinValues;
}

// ��ʼʱ��
HSDouble CSystemFun::BetweenBeginDate(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_STRING;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();
	int nDays = nCount;
	if( ayVars != NULL && ayVars->GetSize() >= 1 )
	{
		LPNODELEMENT pItem = ayVars->GetParam(0);
		if( pItem != NULL )
		{
			nDays = (int)pItem->valoare;//1
		}
	}

	nDays = nCount - nDays;
	if( nDays < 0 ) nDays = 0;
	HSDouble dReturn = (double)hx_DefMinValues;
	HSDouble* pVol = pHxData->GetData(hx_DATE_DATA);
	if( pVol != NULL )
	{
		dReturn.m_dData = pVol[nDays].m_dData;
	}

	return dReturn;
}

// ����ʱ��
HSDouble CSystemFun::BetweenEndDate(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_STRING;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( pHxData == NULL )
		return (double)hx_DefMinValues;

	int nCount = pHxData->GetDaySize();//10
	if( nCount <= 0 )
	{
		return (double)hx_DefMinValues;
	}

	HSDouble dReturn = (double)hx_DefMinValues;
	HSDouble* pVol = pHxData->GetData(hx_DATE_DATA);
	if( pVol != NULL )
	{
		dReturn.m_dData = pVol[nCount-1].m_dData;
	}

	return dReturn;
}

// �ǵ���
HSDouble CSystemFun::BK_ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	long lCountHQ;
	if( payNowData == NULL || (lCountHQ = payNowData->GetSize()) <= 0 || pHxData == NULL )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;

	double dTemp = 0;
	long lValid = 0;

	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		StockUserInfo* pInfo = (StockUserInfo*)pHxData->SendDataToWnd((void*)&pItem->m_ciStockCode.GetCode(), 
			HS_COMPILE_DATA_WPARAM_GetStockUserInfo);

		if( pInfo != NULL )
		{
			dTemp += (pShareData->m_nowData.m_lNewPrice - pInfo->m_lPrevClose) * 100.0 / ( pInfo->m_lPrevClose == 0 ? 1 : pInfo->m_lPrevClose);
		}
		else
		{
			dTemp += (pShareData->m_nowData.m_lNewPrice - pShareData->m_nowData.m_lOpen) * 100.0 / ( pShareData->m_nowData.m_lOpen == 0 ? 1 : pShareData->m_nowData.m_lOpen);
		}

		lValid++;
	}
	
	if( lValid == 0 )
	{
		return (double)hx_DefMinValues;
	}

	return dTemp / (lValid);
}

// Ȩ�ǵ���
HSDouble CSystemFun::BK_Q_ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	long lCountHQ;
	if( payNowData == NULL || (lCountHQ = payNowData->GetSize()) <= 0 || pHxData == NULL )
	{
		return (double)hx_DefMinValues;
	}

	CommRealTimeDataEmpty* pItem;
	double dTemp = 0;
	double dCapital;
	double dCapitalAll = 0;
	int lValid = 0;

	AskData sAsk;
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		StockUserInfo* pInfo = (StockUserInfo*)pHxData->SendDataToWnd((void*)&pItem->m_ciStockCode.GetCode(), 
			HS_COMPILE_DATA_WPARAM_GetStockUserInfo);

		memcpy(sAsk.m_pCode, &pItem->m_ciStockCode, sizeof(pItem->m_ciStockCode));
		pHxData->SetAsk(&sAsk);
		pHxData->SetFinaceData(&pItem->m_ciStockCode);
		dCapital = pHxData->CAPITAL();
		if( dCapital == 0 || (double)hx_DefMaxValues == dCapital )
		{
			// ������־
			//WRITE_LOG(__FILE__, __LINE__, _T("Ȩ�ǵ��� ��ͨ�����ݲ���ȷ"));
			dCapital = 1;
		}
		if( pInfo != NULL )
		{//2010.10.28 �޸�Ϊ��ͨ��ֵ
			dTemp += dCapital* pShareData->m_nowData.m_lNewPrice * ( pShareData->m_nowData.m_lNewPrice - pInfo->m_lPrevClose ) * 100.0 / (pInfo->m_lPrevClose == 0? 1:pInfo->m_lPrevClose);
		}
		else
		{//2010.10.28 �޸�Ϊ��ͨ��ֵ
			dTemp += dCapital* pShareData->m_nowData.m_lNewPrice * (pShareData->m_nowData.m_lNewPrice - pShareData->m_nowData.m_lOpen) * 100.0 / (pShareData->m_nowData.m_lOpen == 0 ? 1:pShareData->m_nowData.m_lOpen);
		}
		dCapitalAll += dCapital* pShareData->m_nowData.m_lNewPrice;  //2010.10.28 �޸�Ϊ��ͨ��ֵ
		lValid++;
	}

	if( fabs(dCapitalAll) <= 0.0001 )
	{
		return (double)hx_DefMinValues;
	}

	if( lValid <= 0 )
	{
		return (double)hx_DefMinValues;
	}

	return dTemp / dCapitalAll;
}

// �ܳɽ���
HSDouble CSystemFun::BK_Vol(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	long lCountHQ;
	if( payNowData == NULL || (lCountHQ = payNowData->GetSize()) <= 0 )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;

	double dTemp = 0;
	int lValid = 0;

	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}

		if( pShareData->m_nowData.m_nHand <= 0 )
		{
			dTemp += pShareData->m_nowData.m_lTotal/100;
		}
		else
		{
			dTemp += pShareData->m_nowData.m_lTotal;
		}
		lValid++;
	}

#ifdef _DEBUG
	if( dTemp < 0 )
	{
		// ������־
		/*WRITE_LOG(__FILE__, __LINE__, _T("�ܳɽ��� dTemp < 0"));*/
	}
#endif

	if( lValid <= 0 )
	{
		return (double)hx_DefMinValues;
	}

	return dTemp;
}

// �ɽ���
HSDouble CSystemFun::BK_Amount(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	if( payNowData == NULL || payNowData->GetSize() <= 0 )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;

	double dTemp = 0;
	int lValid = 0;

	long lCountHQ = payNowData->GetSize();
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		dTemp += pShareData->m_nowData.m_fAvgPrice;
		lValid++;
	}

#ifdef _DEBUG
	if( dTemp < 0 )
	{
		// ������־
	/*	WRITE_LOG(__FILE__, __LINE__, _T("�ɽ��� dTemp < 0"));*/
	}
#endif
	if( lValid <= 0 )
	{
		return (double)hx_DefMinValues;
	}
	return dTemp;
}

// �ɽ�����
HSDouble CSystemFun::BK_Deal(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;
	double dTemp = 0;
	double dCapital;
	double dCapitalAll = 0;
	int lValid = 0;

	long lCountHQ = payNowData->GetSize();
	AskData sAsk;
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}

		memcpy(sAsk.m_pCode, &pItem->m_ciStockCode, sizeof(pItem->m_ciStockCode));
		pHxData->SetAsk(&sAsk);
		pHxData->SetFinaceData(&pItem->m_ciStockCode);
		dCapital = pHxData->CAPITAL();

		if( fabs(dCapital) <= 0.0001 || (double)hx_DefMaxValues == dCapital )
		{
			//return (double)hx_DefMinValues;  //2010.10.28 litao�޸ĵ�ȡ�����ܹɱ�ʱ�����أ�Ҳ�������ۼ�
		}
		else
		{
			dCapitalAll += dCapital;
			dTemp += pShareData->m_nowData.m_lTotal;
			lValid++;
		}
	}

	if( fabs(dCapitalAll) <= 0.0001 )
	{
		return (double)hx_DefMinValues;
	}
	if( lValid <= 0 )
	{
		return (double)hx_DefMinValues;
	}

#ifdef _DEBUG
	if( dTemp < 0 )
	{
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T("�ɽ����� dTemp < 0"));
	}
	if( dCapitalAll < 0 )
	{
		// ������־
		//WRITE_LOG(__FILE__, __LINE__, _T("�ɽ����� dCapitalAll < 0"));
	}
#endif

	return dTemp / dCapitalAll;
}

// ������
HSDouble CSystemFun::BK_HuanShouLv(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;
	double dTemp = 0;
	HSDouble dCapital;
	int iCount = 0;

	long lCountHQ = payNowData->GetSize();
	AskData sAsk;
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		memcpy(sAsk.m_pCode, &pItem->m_ciStockCode, sizeof(pItem->m_ciStockCode));
		pHxData->SetAsk(&sAsk);
		pHxData->SetFinaceData(&pItem->m_ciStockCode);
		dCapital = pHxData->CAPITAL();

		if( fabs(dCapital.m_dData) <= 0.0001 || (double)hx_DefMaxValues == dCapital.m_dData )
		{
			//return (double)hx_DefMinValues;//2010.10.28 litao�޸ĵ�ȡ�����ܹɱ�ʱ�����أ�Ҳ�������ۼ�
		}
		else
		{
			dTemp += pShareData->m_nowData.m_lTotal/ dCapital.m_dData;
			iCount++;
		}
	}

	if( iCount <= 0 )
	{
		return (double)hx_DefMinValues;
	}

	return dTemp / iCount;
}

// ��ӯ��
HSDouble CSystemFun::BK_ShiYing(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Double;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL )
		return (double)hx_DefMinValues;

	double dReturn = 0;
	int iValid = 0;

	long lCountHQ = payNowData->GetSize();
	AskData sAsk;
	for( int i = 0; i < lCountHQ; i++ )
	{
		CommRealTimeDataEmpty* pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem != NULL )
			pShareData = pItem->GetShareData();
		if( pItem != NULL && pShareData->m_nowData.m_lNewPrice > 0)
		{
			memcpy(sAsk.m_pCode, &pItem->m_ciStockCode, sizeof(pItem->m_ciStockCode));
			pHxData->SetAsk(&sAsk);
			pHxData->SetFinaceData(&pItem->m_ciStockCode);
			HSDouble dRet = pHxData->GetFinaceDataByID(COLUMN_FINANCE_PER_INCOME);

			double nQuarter = pHxData->GetFinaceDataByID(0); //20090605 YJT �޸���ӯ���㷨
			//20090605 YJT �޸���ӯ���㷨
			if (abs(nQuarter - 1) < 0.0001)
				nQuarter = 0.25;
			else if (abs(nQuarter - 2) < 0.0001)
				nQuarter = 0.5;
			else if (abs(nQuarter - 3) < 0.0001)
				nQuarter = 0.75;
			else
				nQuarter = 1;

			short lUnit = (short)pHxData->SendDataToWnd(&pItem->m_ciStockCode.m_cCodeType, HS_COMPILE_DATA_WPARAM_GetStockPriceUnit);
			if ( dRet.m_dData == 0 || dRet.m_dData == (double)hx_DefMaxValues )
			{
				continue;
			}
			dRet.m_dData *= (lUnit == 0 ? 1 : lUnit);

			dReturn += pShareData->m_nowData.m_lNewPrice/dRet.m_dData * nQuarter; //20090605 YJT �޸���ӯ���㷨
			iValid++;
		}
	}

	if( iValid <= 0 )
	{
		return (double)hx_DefMinValues;
	}

	//2010.11.05 litao ������������򲻼�����ӯ��
	if (dReturn < 0.0)
		return (double)hx_DefMinValues;

	return (double)dReturn/iValid;
}

// ���ǹ�
HSDouble CSystemFun::BK_LingZhang(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = 0;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL )
		return (double)hx_DefMinValues;

	CommRealTimeDataEmpty* pItem;
	double dTemp = 0;
	double dTempPre = 0;
	double dRet = -1;
	
	long lCountHQ = payNowData->GetSize();
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem != NULL)
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		StockUserInfo* pInfo = (StockUserInfo*)pHxData->SendDataToWnd((void*)&pItem->m_ciStockCode.GetCode(), 
			HS_COMPILE_DATA_WPARAM_GetStockUserInfo);

		if( pInfo != NULL )
		{
			dTemp = (pShareData->m_nowData.m_lNewPrice - pInfo->m_lPrevClose) * 100.0 / pInfo->m_lPrevClose;
		}
		else
		{
			dTemp = (pShareData->m_nowData.m_lNewPrice - pShareData->m_nowData.m_lOpen) * 100.0 / pShareData->m_nowData.m_lOpen;
		}

		if( dTempPre < dTemp && pShareData->m_nowData.m_lNewPrice > 0 )
		{
			dTempPre = dTemp;
			dRet = i;
		}
	}

	return dRet;
}

// �����
HSDouble CSystemFun::BK_LingDie(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = 0;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL)
		return (double)hx_DefMinValues;

	long lCountHQ = payNowData->GetSize();
	CommRealTimeDataEmpty* pItem;
	double dTemp = 0;
	double dTempPre = 0;
	double dRet = -1;
	
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		StockUserInfo* pInfo = (StockUserInfo*)pHxData->SendDataToWnd((void*)&pItem->m_ciStockCode.GetCode(), 
			HS_COMPILE_DATA_WPARAM_GetStockUserInfo);

		if( pInfo != NULL )
		{
			dTemp = (pShareData->m_nowData.m_lNewPrice - pInfo->m_lPrevClose) * 100.0 / pInfo->m_lPrevClose;
		}
		else
		{
			dTemp = (pShareData->m_nowData.m_lNewPrice - pShareData->m_nowData.m_lOpen) * 100.0 / pShareData->m_nowData.m_lOpen;
		}

		if( dTempPre > dTemp && pShareData->m_nowData.m_lNewPrice > 0 )
		{
			dTempPre = dTemp;
			dRet = i;
		}
	}

	return dRet;
}

// ��Ʊ��
HSDouble CSystemFun::BK_Num(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_Long;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	if( payNowData == NULL || payNowData->GetSize() <= 0 )
		return (double)hx_DefMinValues;

	return payNowData->GetSize();
}

// �ǹɱȣ� ��double�ͱ�ʾ����λ��5λ��ʾ�������������5λ�ĸ�λ��ʾ���ǵļ���
HSDouble CSystemFun::BK_UpRatio(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = HS_Return_ZGB;
	CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*> *payNowData = (CArray<CommRealTimeDataEmpty*, CommRealTimeDataEmpty*>*)ayVars->GetParam(0)->pData;
	CHSDataSource* pHxData = GetDataSource(pCompile);
	if( payNowData == NULL || payNowData->GetSize() <= 0 || pHxData == NULL)
		return (double)hx_DefMinValues;

	long lCountHQ = payNowData->GetSize();
	long lUpNum = 0; //���ǹ�Ʊ����
	CommRealTimeDataEmpty* pItem;

	double dRet = -1;
	
	for( int i = 0; i < lCountHQ; i++ )
	{
		pItem = payNowData->GetAt(i);
		ShareRealTimeData* pShareData = NULL;
		if ( pItem )
			pShareData = pItem->GetShareData();
		if( pItem == NULL || pShareData->m_nowData.m_lNewPrice <= 0 )
		{
			continue;
		}
		StockUserInfo* pInfo = (StockUserInfo*)pHxData->SendDataToWnd((void*)&pItem->m_ciStockCode.GetCode(), 
			HS_COMPILE_DATA_WPARAM_GetStockUserInfo);

		if( pInfo != NULL )
		{
			if(pShareData->m_nowData.m_lNewPrice - pInfo->m_lPrevClose > 0.0)
				lUpNum++;
		}
	}
	dRet = lUpNum * 100000.0 + lCountHQ;
	return dRet;
}


// ָ�����͵�רҵ��������
// PROFFIN(ID,Begin,End,type,code)
// (ID,����,��ʼ���,��ֹ���,��Ʊ����)
// �������������������������
// ���ͣ�1����һ���ȼ�����2���б���3���������ȼ�����4���걨
HSDouble CSystemFun::PROFFIN (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = 0;
	return hx_DefMinValues;

	/*
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	if( GetData(pCompile,ayVars,pCurDataStatus,nError) == NULL )
		return hx_DefMinValues;

	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	LPNODELEMENT pParam2  = ayVars->GetParam(2);
	LPNODELEMENT pParam3  = ayVars->GetParam(3);
	LPNODELEMENT pParam4  = ayVars->GetParam(4);

	if( pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	double v0 = 0,v1 = 0,v2 = 0,v3 = 0;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);

	// ����id����
	CString strID;
	if( pParam0 )
	{
		if(pParam0->cDataType & DataTypeOperStr && pParam0->valoarestr)
		{
			strID = *pParam0->valoarestr;
		}
	}

	// ����
	if( pParam1 )
		v1 = pCompile->vexp(pParam1,&calc1);

	// ��ʼ����
	if( pCurDataStatus->m_pCalcParam )
		v2 = pCurDataStatus->m_pCalcParam->m_nBeginDate;
	else if( pParam2 )
		v2 = pCompile->vexp(pParam2,&calc2);

	// ��ֹ����
	if( pCurDataStatus->m_pCalcParam )
		v3 = pCurDataStatus->m_pCalcParam->m_nEndDate;
	else if( pParam3 )
		v3 = pCompile->vexp(pParam2,&calc3);

	// ��Ʊ����
	CString strCode;
	if( pCurDataStatus->m_pCalcParam )
	{
		strCode = pCurDataStatus->m_pCalcParam->m_sCode.GetCode();
	}
	else if( pParam4 )
	{
		if(pParam4->cDataType & DataTypeOperStr && pParam4->valoarestr)
		{
			strCode = *pParam4->valoarestr;
		}
	}

	return GetData(pCompile,ayVars,pCurDataStatus,nError)->PROFFIN(strID,v1,v2,v3,strCode,
		pCurDataStatus,nError);
		*/
}

// ָ�����ڵ�רҵ��������
// PROFFINON(ID,ǰһ����,Begin,End,code)
// (ID,ǰһ����,��ʼʱ��,��ֹʱ��,��Ʊ����)
HSDouble CSystemFun::PROFFINON (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	if( GetData(pCompile,ayVars,pCurDataStatus,nError) == NULL )
		return hx_DefMinValues;

	LPNODELEMENT pParam0  = ayVars->GetParam(0); // �ֶ�
	LPNODELEMENT pParam1  = ayVars->GetParam(1); // ��ʼ����
	LPNODELEMENT pParam2  = ayVars->GetParam(2); // ��ֹ����
	LPNODELEMENT pParam3  = ayVars->GetParam(3); // ��������
	LPNODELEMENT pParam4  = ayVars->GetParam(4); // ƫ��������
	LPNODELEMENT pParam5  = ayVars->GetParam(5); // ����

	if( pCompile == NULL ||
		pParam0  == NULL )
		return hx_DefMinValues;

	double v0 = 0,v1 = 0,v2 = 0,v3 = 0,v4 = 0;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	HSCurDataInfoStatus calc4(pCurDataStatus);
	HSCurDataInfoStatus calc5(pCurDataStatus);

	CString strID;	// ����id����
	if( pParam0 )
	{
		if(pParam0->cDataType & DataTypeOperStr && pParam0->valoarestr)
		{
			strID = *pParam0->valoarestr;
		}
	}
	
	// ��ʼ����
	if( pCurDataStatus->m_pCalcParam )
		v1 = pCurDataStatus->m_pCalcParam->m_nBeginDate;
	else if( pParam1 )
		v1 = pCompile->vexp(pParam1,&calc1);

	// ��ֹ����
	if( pCurDataStatus->m_pCalcParam )
		v2 = pCurDataStatus->m_pCalcParam->m_nEndDate;
	else if( pParam2 )
		v2 = pCompile->vexp(pParam2,&calc2);

	if( pParam3 )  // ��������
	{
		v3 = pCompile->vexp(pParam3,&calc3);
	}

	if( pParam4 )  // ƫ��������
	{
		v4 = pCompile->vexp(pParam4,&calc4);
	}

	// ��Ʊ����
	CString strCode;
	CString strBlockName; // ���
	if( pCurDataStatus->m_pCalcParam )
	{
		strCode      = pCurDataStatus->m_pCalcParam->m_sCode.GetCode();
		strBlockName = pCurDataStatus->m_pCalcParam->GetBlockName();
	}
	else if( pParam5 )
	{
		if(pParam5->cDataType & DataTypeOperStr && pParam5->valoarestr)
		{
			strCode = *pParam5->valoarestr;
		}
	}

	if( strBlockName.IsEmpty() ) // ���ǰ�����
	{
		return GetData(pCompile,ayVars,pCurDataStatus,nError)->PROFFINON(strID,
			v1,v2,&calc3,v4,strCode,
			pCurDataStatus,nError);
	}

	return GetData(pCompile,ayVars,pCurDataStatus,nError)->BlockCalc(strID,
		strBlockName,
		v1,v2,
		&calc3,
		v4,
		pCurDataStatus,nError); 
}


//2.	PROFSTR				רҵ�����ַ�������
//PROFSTR(N,M)
//��ʾȡ��M��������֮ǰ�ĵ�N��רҵ�����ַ�������,
//����PROFSTR(4001,0)��ʾ���һ�ڵ�һ��ɶ����ƣ�N��ȡֵ������±�
HSDouble CSystemFun::PROFSTR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 0;
}


//3.	PROFFINLAST			רҵ�������ݽ�ֹ��λ��
//PROFFINLAST(N)
//��ʾ��������ڵĵ�N��רҵ�������ݾ������ڵ���������N��ȡֵ������±�
HSDouble CSystemFun::PROFFINLAST	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 0;
}


//5.	PROFSTRON			ָ�����ڵ�רҵ�ַ�����������
//PROFFINON(N,Y,MD)
//��ʾȡ��Y��M��D�յĵ�N��רҵ�������ݣ�
//��PROFSTRON(5001,2000,0101)ȡ��2000��1��1�յĵ�һ��ɶ����ƣ�N��ȡֵ������±�
HSDouble CSystemFun::PROFSTRON	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 0;
}

//6.	PROFFINTERM		רҵ�������ݱ�����
//PROFFINTERM(M)
//��ʾȡ��M��������֮ǰ�Ĳ��񱨱����걨���б����Ǽ���
//����1����һ���ȼ�����2���б���3���������ȼ�����4���걨
//���磺��PROFFINTERM(0)����4�����ʾ���һ�ڲ��񱨱����걨��
HSDouble CSystemFun::PROFFINTERM	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 0;
}

//7.	SETPROFFIN			����רҵ��������
//SETPROFFIN(X)
//�ڱ������������רҵ���������ܵ������������õ�Ӱ�죬��ʾ��רҵ�������������趨ΪX��X�ӵ͵���ÿһλ��ʾһ������
//��1λ�������걨��
//��2λ�������б���
//��3λ������������
//��4λ���������²���ָ�꣬
//��5λ�������б�����������ָ�꣬
//����SETPROFFIN(01011)��ʾȡ���걨���б�������һ�����ݣ��������Ǹ������ڶ�Ҫ�������ڣ��б���������
//��������ñ�������ϵͳĬ��ֵΪ01111
HSDouble CSystemFun::SETPROFFIN	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 0;
}

HSDouble CSystemFun::FOR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
					 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	/*
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	LPNODELEMENT pParam2  = ayVars->GetParam(2);
	if(pParam0 == NULL || pParam1 == NULL || pParam2 == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);

	double v0 = pCompile->vexp(pParam0,&calc0);
	double v1 = pCompile->vexp(pParam1,&calc1);
	//double v2 = pCompile->vexp(pParam2,&calc2);

	if( v1 == hx_DefMinValues )
		return hx_DefMinValues;

	int nCount = ayVars->GetSize();

	for( int i = 3; v1 && i < nCount; i++ )
	{
		HSCurDataInfoStatus calc(pCurDataStatus);
		LPNODELEMENT pParam  = ayVars->GetParam(i);

		pCompile->vexp(pParam,&calc);
	}

	*/
	nError = -1;
	return 0;
}

HSDouble CSystemFun::CONTINUE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	//nError = -1;
	return 0;
}

HSDouble CSystemFun::BREAK	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	//nError = -1;
	return 1;
}

HSDouble CSystemFun::OPERATOR   (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	nError = -1;
	return 1;
	/*
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);
	LPNODELEMENT pParam2  = ayVars->GetParam(2);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( //GetData(pCompile,ayVars,pCurDataStatus,nError)->GetDaySize() <= 0 || 
		pCompile == NULL   ||
		pParam0  == NULL   ||
		pParam1  == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v0 = pCompile->vexp(pParam0,&calc0);
	double v1 = pCompile->vexp(pParam1,&calc1);
	double v2 = pCompile->vexp(pParam2,&calc2);

	// һ�Զ࣬���һ����Զ�,
	if( (calc0.m_pData != NULL) || (calc1.m_pData != NULL) || (calc2.m_pData != NULL) ) 
	{
	}
	else if( v0 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if( v1 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}
	if( v2 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}

	switch( (int)v2 )
	{
	case 0: // +
		return (v0 + v2);
	case 1: // -
		return (v0 - v2);
	case 2: // *
		return (v0 * v2);
	case 3: // /
		if( v2 )
			return (v0 / v2);
		else
			break;
	case 4: // =
		v0 = v2;
		return v0;
	}

	return hx_DefMinValues;

	//return hx_DefMinValues;
	*/
}

// �ַ�����������������������ת�����ַ���,ֻ�����ַ���������
HSDouble CSystemFun::STROUT	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(pCurDataStatus == NULL || pCompile == NULL || ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	pCurDataStatus->Alloc(ayVars->GetSize());

	HSDrawOtherData* pHSDrawOtherData;
	LPNODELEMENT pParam;

	for( int i = 0; i < ayVars->GetSize(); i++ )
	{
		pParam = ayVars->GetParam(i);
		if( pParam == NULL ||
			pParam->valoarestr == NULL ||
			!((pParam->cDataType & DataTypeOperStr) && (pParam->cDataType & DataText)) )
			continue;

		pHSDrawOtherData = pCurDataStatus->m_pData[i].AllocOther(NULL,TRUE);
		if( pHSDrawOtherData )
			//pHSDrawOtherData->m_strText.Format("%s", *(pParam->valoarestr));
			memcpy(pHSDrawOtherData->m_cText,pParam->valoarestr->GetBuffer(),min(sizeof(pHSDrawOtherData->m_cText)-1,pParam->valoarestr->GetLength()));
	}

	return hx_ReturnPointValues;
}

// ����ת������:(date,format)
HSDouble CSystemFun::DATETOSTR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
								 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(pCompile == NULL || pCurDataStatus == NULL || ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( pParam0 == NULL || pParam1 == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
//	double v2 = pCompile->vexp(pParam1,&calc2);

	if( pParam1->valoarestr == NULL ||
		!((pParam1->cDataType & DataTypeOperStr) && (pParam1->cDataType & DataText)) )
		return hx_DefMinValues;

	HSDrawOtherData* pHSDrawOtherData;
	CString strFormat = *pParam1->valoarestr;
	CTime tm;
	long lDate;
	if( calc1.m_pData != NULL )
	{
		long lSize = calc1.m_lSize;
		pCurDataStatus->Alloc(lSize);
		int nCur = GetDataValidPos(calc1.m_pData,lSize);
		for( ; nCur < lSize; nCur++ )
		{
			pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
			if( pHSDrawOtherData )
			{
				pCurDataStatus->m_pData[nCur].m_dData = calc1.m_pData[nCur].m_dData;
				lDate = calc1.m_pData[nCur].m_dData;
				tm    = LongToDate(lDate);
				if( tm > 0 )
				{
					CString strText;
					strText.Format("%s", tm.Format(strFormat));
					memcpy(pHSDrawOtherData->m_cText, strText.GetBuffer(),min(sizeof(pHSDrawOtherData->m_cText)-1,strText.GetLength()));
					//pHSDrawOtherData->Alloc(&tm.Format(strFormat));
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

HSDouble CSystemFun::BLOCKCALC(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError)
{

	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	if( GetData(pCompile,ayVars,pCurDataStatus,nError) == NULL )
		return hx_DefMinValues;

	LPNODELEMENT pParam0  = ayVars->GetParam(0); // �ֶ�
	LPNODELEMENT pParam1  = ayVars->GetParam(1); // �������
	LPNODELEMENT pParam2  = ayVars->GetParam(2); // ��ʼ����
	LPNODELEMENT pParam3  = ayVars->GetParam(3); // ��ֹ����
	LPNODELEMENT pParam4  = ayVars->GetParam(4); // ��������
	LPNODELEMENT pParam5  = ayVars->GetParam(5); // ƫ��������
	LPNODELEMENT pParam6  = ayVars->GetParam(6); // ����
	LPNODELEMENT pParam7  = ayVars->GetParam(7); // ����

	if( pCompile == NULL ||
		pParam0  == NULL )
		return hx_DefMinValues;

	double v0 = 0,v1 = 0,v2 = 0,v3 = 0,v4 = 0,v5 = 0,v6 = 0;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	HSCurDataInfoStatus calc3(pCurDataStatus);
	HSCurDataInfoStatus calc4(pCurDataStatus);
	HSCurDataInfoStatus calc5(pCurDataStatus);
	HSCurDataInfoStatus calc6(pCurDataStatus);
	HSCurDataInfoStatus calc7(pCurDataStatus);

	// ����id����
	CString strID;	
	if( pParam0 )
	{
		if(pParam0->cDataType & DataTypeOperStr && pParam0->valoarestr)
		{
			strID = *pParam0->valoarestr;
		}
	}

	// �������
	CString strBlockName;
	if( pParam1 )
	{
		if(pParam1->cDataType & DataTypeOperStr && pParam1->valoarestr)
		{
			strBlockName = *pParam1->valoarestr;
		}
	}
	if( strBlockName.IsEmpty() )
	{
		if( pCurDataStatus->m_pCalcParam )
			strBlockName = pCurDataStatus->m_pCalcParam->GetBlockName();
	}
	if( strBlockName.IsEmpty() )
	{
		return hx_DefMinValues;
	}
	
	// ��ʼ����
	if( pCurDataStatus->m_pCalcParam )
		v2 = pCurDataStatus->m_pCalcParam->m_nBeginDate;
	else if( pParam1 )
		v2 = pCompile->vexp(pParam2,&calc2);

	// ��ֹ����
	if( pCurDataStatus->m_pCalcParam )
		v3 = pCurDataStatus->m_pCalcParam->m_nEndDate;
	else if( pParam3 )
		v3 = pCompile->vexp(pParam3,&calc3);

	if( pParam4 )  // ��������
	{
		v4 = pCompile->vexp(pParam4,&calc4);
	}

	if( pParam5 )  // ƫ��������
	{
		v5 = pCompile->vexp(pParam5,&calc5);
	}

	// ����
	if( pParam6 )  
	{
		v6 = pCompile->vexp(pParam6,&calc6);
	}


	return GetData(pCompile,ayVars,pCurDataStatus,nError)->BlockCalc(strID,
		strBlockName,
		v2,v3,
		&calc4,
		v5,
		pCurDataStatus,nError,
		v6);	
}


// ����ͨ-�ʺ� 
HSDouble CSystemFun::HLTCH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL	  ||
		pParam0    == NULL   ||
		pParam1   == NULL)
		return hx_DefMinValues;

	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);
	double v1 = pCompile->vexp(pParam0,&calc1);
	double v2 = pCompile->vexp(pParam1,&calc2);

	if( calc1.m_pData != NULL || calc2.m_pData != NULL )
	{
		HSDouble curSum = 0;
		int  nCur,nPos;
		int  i,j;
		i = 0;
		j = 0;
		BOOL bValid = FALSE;
		long lSize = max(calc1.m_lSize,calc2.m_lSize); // ע�⣺�� calc1.m_lSize > 0 && calc2.m_lSize > 0 ʱ,�� calc1.m_lSize ����� calc2.m_lSize
		pCurDataStatus->Alloc(lSize);
		if( calc1.m_pData != NULL && calc2.m_pData != NULL )
		{			
			long lRefPeriod;
			nCur = GetDataValidPos(calc2.m_pData,lSize);
			nPos = GetDataValidPos(calc1.m_pData,lSize);
			for( ; nCur < lSize; nCur++ )
			{
				if( calc2.m_pData[nCur].m_dData != hx_DefMinValues )
				{
					lRefPeriod = calc2.m_pData[nCur];
				}
				else
				{
					lRefPeriod = 0;
				}
				lRefPeriod = (((lRefPeriod <= 1)?1:lRefPeriod) - 1);
				if( lRefPeriod <= 0 )
				{
					pCurDataStatus->m_pData[nCur] = calc1.m_pData[nCur];
				}
				else
				{
					i = 0;
					j = 0;
					for(nPos = (lRefPeriod <= 0)?0:(nCur - lRefPeriod); nPos >= 0 && nPos <= nCur; nPos++)
					{
						if( calc1.m_pData[nPos].m_dData != hx_DefMinValues )
						{
							i = i + 1;
							curSum += (double)i * calc1.m_pData[nPos];
							bValid = TRUE;
							j = j + i;
						}
					}
					if( bValid && lRefPeriod != hx_DefMinValues )
					{
						pCurDataStatus->m_pData[nCur] = curSum / (double)j;
					}
					curSum = 0;
					bValid = FALSE;
				}
			}
		}
		else
		{
			HSDouble* pData = ((calc1.m_pData != NULL)?calc1.m_pData:calc2.m_pData);
			if( pData != NULL )
			{
				nCur = GetDataValidPos(pData,lSize);
				if( v2 != hx_ReturnPointValues )
				{
					v2 = (((v2 <= 1)?1:v2) - 1);
					for( nCur = nCur + v2; nCur < lSize; nCur++ )
					{
						if( v2 <= 0 )
						{
							if( pData[nCur].m_dData != hx_DefMinValues )
							{
								pCurDataStatus->m_pData[nCur] = pData[nCur];
							}
						}
						else
						{
							i = 0;
					        j = 0;
							for( nPos = (v2 <= 0)?0:(nCur - v2); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos].m_dData != hx_DefMinValues )
								{
									i = i + 1;
									curSum += ((double)i * pData[nPos]);
									bValid = TRUE;
									
						            j = j + i;
								}
							}
							if( bValid )
							{
								pCurDataStatus->m_pData[nCur] =  curSum / (double)j;
							}
							curSum = 0;
							bValid = FALSE;
						}
					}
				}
				else if( v1 != hx_ReturnPointValues )
				{
					v1 = (((v1 <= 1)?1:v1) - 1);
					for( nCur = nCur + v1; nCur < lSize; nCur++ )
					{	
						if( v1 <= 0 )
						{
							if( pData[nCur].m_dData != hx_DefMinValues )
							{
								pCurDataStatus->m_pData[nCur] = pData[nCur];
							}
						}
						else
						{
                            i = 0;
					        j = 0;
							for( nPos = (v1 <= 0)?0:(nCur - v1); nPos >= 0 && nPos <= nCur; nPos++)
							{
								if( pData[nPos].m_dData != hx_DefMinValues )
								{
									i = i + 1;
									curSum += ((double)i * pData[nPos]);
									bValid = TRUE;
									j = j + i;
								}
							}
							if( bValid )
							{
								pCurDataStatus->m_pData[nCur] =  curSum/(double)j;
							}
							curSum = 0;
							bValid = FALSE;
						}
					}
				}
			}
		}

		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����ͨ-���Ȧ
HSDouble CSystemFun::HLTHBQ(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
				 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}
	
	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	long lRefPeriod =  (long)pCompile->vexp(pParam1,pCurDataStatus);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;
	
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pParam0,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;

    HSDouble* pRefHighDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pRefLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);	
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);

	if( curStatus.m_pData != NULL )
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		pCurDataStatus->m_dwStatus |= HX_USERDRAWFUN_EXPRESS;
		return HLTHBQ1(lRefPeriod,curStatus.m_lSize,pCurDataStatus,pRefHighDay,pRefLowDay,pRefCloseDay,nError);
	}
	return hx_DefMinValues;
}

// ����ͨ-���Ȧ
HSDouble CSystemFun::HLTHBQ1(long lRefPeriod, long lRefCount,HSCurDataInfoStatus* pCurDataStatus,
				 HSDouble* pRefHighDay,HSDouble* pRefLowDay,HSDouble* pRefCloseDay, int& nError)
{
	HSDrawOtherData* pHSDrawOtherData;

	HSDouble curSAR = 0;
	HSDouble curXK = 0;
	for( int nCur = lRefPeriod ; nCur < lRefCount; nCur++ )
	{
		if( nCur < 3 )
		{
			curSAR = pRefHighDay[nCur];
            pCurDataStatus->m_pData[nCur] = pRefHighDay[nCur];

			if( pCurDataStatus->m_pData[nCur].m_pOtherData == NULL )
			{
				pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
				//pHSDrawOtherData = new HSDrawOtherData();
				pHSDrawOtherData->m_bType1 = HS_HLTHBQ;
				pHSDrawOtherData->m_bType2 = 2;
				pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
			}

			continue;
		}
        
		if( nCur == lRefPeriod)
		{
            curSAR = pRefHighDay[nCur];
			for( int k = 1; k <= 3; k ++)
			{
				if( pRefHighDay[nCur - k] > curSAR )
					curSAR = pRefHighDay[nCur - k];
			}
            continue;
			//pCurDataStatus->m_pData[nCur] = curSAR;
		}
        
		curXK = curXK + 0.02;
		if (curXK > 0.2 )
			curXK = 0.2;
        curSAR = curSAR + (curXK * (pRefLowDay[nCur - 1] - curSAR));
        pCurDataStatus->m_pData[nCur] = curSAR;

		if( pCurDataStatus->m_pData[nCur].m_pOtherData == NULL )
		{
			pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
			//pHSDrawOtherData = new HSDrawOtherData();
			pHSDrawOtherData->m_bType1 = HS_HLTHBQ;
			if( lRefPeriod == (nCur - 1) )
				pHSDrawOtherData->m_bType2 = 2;
			else
				pHSDrawOtherData->m_bType2 = 2;
			pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
		}

        if(curSAR < pRefCloseDay[nCur])
		{
			return HLTHBQ2(nCur,lRefCount,pCurDataStatus,pRefHighDay,pRefLowDay,pRefCloseDay,nError);
		}
	}
	return hx_ReturnPointValues;
}


// ����ͨ-���Ȧ2
HSDouble CSystemFun::HLTHBQ2(long lRefPeriod, long lRefCount,HSCurDataInfoStatus* pCurDataStatus,
				 HSDouble* pRefHighDay,HSDouble* pRefLowDay,HSDouble* pRefCloseDay, int& nError)
{
	HSDrawOtherData* pHSDrawOtherData;
	HSDouble curSAR = 0;
	HSDouble curXK = 0;
	for( int nCur = lRefPeriod ; nCur < lRefCount; nCur++ )
	{
		if( nCur < 3 )
		{
			curSAR = pRefHighDay[nCur];
            pCurDataStatus->m_pData[nCur] = pRefHighDay[nCur];

			if( pCurDataStatus->m_pData[nCur].m_pOtherData == NULL )
			{
				pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
				//pHSDrawOtherData = new HSDrawOtherData();
				pHSDrawOtherData->m_bType1 = HS_HLTHBQ;
				pHSDrawOtherData->m_bType2 = 1;
				pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
			}

			continue;
		}
        
		if( nCur == lRefPeriod)
		{
            curSAR = pRefLowDay[nCur];
			for( int k = 1; k <= 3; k ++)
			{
				if( pRefLowDay[nCur - k] < curSAR )
					curSAR = pRefLowDay[nCur - k];
			}

			if( pCurDataStatus->m_pData[nCur].m_pOtherData == NULL )
			{
				pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
				//pHSDrawOtherData = new HSDrawOtherData();
				pHSDrawOtherData->m_bType1 = HS_HLTHBQ;
				pHSDrawOtherData->m_bType2 = 1;
				pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
			}
			pCurDataStatus->m_pData[nCur] = curSAR;

            continue;
		}
        
		curXK = curXK + 0.02;
		if (curXK > 0.2 )
			curXK = 0.2;
        curSAR = curSAR + (curXK * (pRefHighDay[nCur - 1] - curSAR));
        pCurDataStatus->m_pData[nCur] = curSAR;

		//
		if( pCurDataStatus->m_pData[nCur].m_pOtherData == NULL )
		{
			pHSDrawOtherData = pCurDataStatus->m_pData[nCur].AllocOther(NULL,TRUE);
			//pHSDrawOtherData = new HSDrawOtherData();
			pHSDrawOtherData->m_bType1 = HS_HLTHBQ;
			pHSDrawOtherData->m_bType2 = 1;
			pCurDataStatus->m_pData[nCur].m_pOtherData = pHSDrawOtherData;
		}

        if(curSAR > pRefCloseDay[nCur])
		{
			return HLTHBQ1(nCur,lRefCount,pCurDataStatus,pRefHighDay,pRefLowDay,pRefCloseDay,nError);
		}
	}
	return hx_ReturnPointValues;
}

//******************************������litao����*****************************************
HSDouble CSystemFun::SAR (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						  HSCurDataInfoStatus* pCurDataStatus, int& nError)    // ����: ������ת��
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pParam0  = ayVars->GetParam(0);	//����
	LPNODELEMENT pParam1  = ayVars->GetParam(1);	//����ϵ��
	LPNODELEMENT pParam2  = ayVars->GetParam(2);	//����ϵ������

	if(pParam0 == NULL || pParam1 == NULL)
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);

	long lRefPeriod =  (long)pCompile->vexp(pParam0,&calc0);
	if (lRefPeriod < 1)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	double AF =  (double)pCompile->vexp(pParam1,&calc1)/100.0;
	double MaxAF =  (double)pCompile->vexp(pParam2,&calc2)/100.0;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;

	HSDouble* pRefHighDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pRefLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);	
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);
	HSDouble* pRefOpenDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);


	long lSize;
	HSDouble curSAR = 0.0;
	double curEP = 0.0;double curAF = AF;
	int i,j; bool bUp;  //��ǰ�������������ڻ����µ����� trueΪ��������,��falseΪ�µ�����

	//ȡ���ݵĳ���
	lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() ;

	if(lSize>lRefPeriod)
	{
		//lSize = calc0.m_lSize;
		pCurDataStatus->Alloc(lSize+1);

		//��ʼ��
		for (i = 0;i<lSize;i++)
		{
			pCurDataStatus->m_pData[i] = hx_DefMinValues;
		}

		//�ж��������µ����ڣ�����SAR(0)
		if (pRefCloseDay[lRefPeriod-1] < pRefCloseDay[0])  //����
		{
			bUp = true;
			pCurDataStatus->m_pData[lRefPeriod] = 9999999.0;
			//ǰn�յ���ͼ�
			for (i = 0;i<lRefPeriod;i++)
			{
				if (pRefLowDay[i] < pCurDataStatus->m_pData[lRefPeriod])
					pCurDataStatus->m_pData[lRefPeriod] = pRefLowDay[i];
			}

		}
		else	//����
		{
			bUp = false;
			pCurDataStatus->m_pData[lRefPeriod] = 0.0;
			//ǰn�յ���߼�
			for (i = 0;i<lRefPeriod;i++)
			{
				if (pRefHighDay[i] > pCurDataStatus->m_pData[lRefPeriod])
					pCurDataStatus->m_pData[lRefPeriod] = pRefHighDay[i];
			}

		}

		for (i = lRefPeriod+1;i<lSize;i++)  //���μ������SARֵ
		{

			//���(n-1)���ǵͿ����յģ���EP(n-1)��ȡ����߼ۣ�����ȡ��ͼ�
			if (pRefCloseDay[i-1] >= pRefOpenDay[i-1])
				curEP = pRefHighDay[i-1];
			else
				curEP = pRefLowDay[i-1];

			//������������������߼۱�ǰ�����߼ۻ���,��AF����0.02,��֮Ϊ�µ�����
			if ((i-2) >= 0)
			{	
				if (((pRefHighDay[i] > pRefHighDay[i-2]) && (bUp)) || ((pRefLowDay[i] < pRefLowDay[i-2]) && (!bUp)))
					curAF = min(MaxAF,curAF + 0.02);
			}

			pCurDataStatus->m_pData[i] = pCurDataStatus->m_pData[i-1] + curAF * (curEP - pCurDataStatus->m_pData[i-1]);


			//����ڿ��������У������ĳ�յ�SARֵ�ȵ��յ���ͼ۸ߣ���Ӧ�Ե��յ���ͼ�Ϊ���յ�SARֵ��
			//����ڿ��������У��������ĳ�յ�SARֵ�ȵ��յ���߼۵ͣ���Ӧ�Ե��յ���߼�Ϊĳ�յ�SARֵ��
			if ((bUp) && (pRefLowDay[i] <= pCurDataStatus->m_pData[i])) //��������ͻ��
			{
				////pCurDataStatus->m_pData[i] = min(pCurDataStatus->m_pData[i],pRefLowDay[i]);
				curAF = AF; 
				bUp = !bUp;

				//���¼�����һ��SAR(0)
				pCurDataStatus->m_pData[i+1] = 0.0;
				//ǰn�յ���߼�
				for (j = i - lRefPeriod +1;j< i ;j++)
				{
					if (pRefHighDay[j] > pCurDataStatus->m_pData[i+1])
						pCurDataStatus->m_pData[i+1] = pRefHighDay[j];
				}
				i++;
			}
			else if	((!bUp) && (pRefHighDay[i] >= pCurDataStatus->m_pData[i])) //��������ͻ��
			{
				////pCurDataStatus->m_pData[i] = max(pCurDataStatus->m_pData[i],pRefHighDay[i]);
				curAF = AF; 
				bUp = !bUp;

				//���¼�����һ��SAR(0) 
				pCurDataStatus->m_pData[i+1] = 9999999.0;
				for (j = i - lRefPeriod +1;j< i ;j++)
				{
					if (pRefLowDay[j] < pCurDataStatus->m_pData[i+1])
						pCurDataStatus->m_pData[i+1] = pRefLowDay[j];
				}
				i++;
			}			

		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}


//���˺���������������N�����ں����������Ϊ0
HSDouble CSystemFun::FILTER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pParam0  = ayVars->GetParam(0);
	LPNODELEMENT pParam1  = ayVars->GetParam(1);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL ||
		pParam0  == NULL ||
		pParam1 == NULL )
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);

	double v0 = pCompile->vexp(pParam0,&calc0);
	int N1 = 0;
	N1 = (int)pCompile->vexp(pParam1,&calc1);

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		v0 = pCompile->vexp(pParam0,pCurDataStatus);

		if( v0 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}	
		return ( v0?1:0);
	}

	if( (calc0.m_pData != NULL)) 
	{
		int i,j;
		long lSize =calc0.m_lSize; 
		pCurDataStatus->Alloc(lSize);
		if(( calc0.m_pData != NULL) && (N1 > 0))
		{
			i = 0;
			while( i < lSize)
			{
				if( calc0.m_pData[i].m_dData != hx_DefMinValues)
				{
					pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?1:0);
					if (calc0.m_pData[i])  //�������������N���ڵ���0
					{
						for(j = 1; j <= N1; j++)
						{
							if ((i+j) < lSize)
								pCurDataStatus->m_pData[i+j] = (calc0.m_pData[i]?0:0);
						}
						i = i + N1+1;
					}
					else
						i++;
				}
				else
					i++;
			}
		}
		else if( (calc0.m_pData == NULL) && (N1 <= 0) )
		{
			i = 0;
			while( i < lSize)
			{
				if( calc0.m_pData[i].m_dData != hx_DefMinValues)
				{
					pCurDataStatus->m_pData[i] = (calc0.m_pData[i]?1:0);
					if (calc0.m_pData[i])
					{
						for(j = i+1; j < lSize; j++)
						{
							pCurDataStatus->m_pData[j] = 0;
						}
						i = lSize;
						break;
					}	
				}
				i++;
			}
		}		
		return hx_ReturnPointValues;
	}
	else
	{
		if( v0 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return ( v0?1:0);
	}

	return hx_DefMinValues;
}


//������˵����..\truck\bin\HQClient\Setting\FunctionHelp.cfg
//���ڲ����в��ܷ����ڲ���ͼ���Ժ�����ʽʵ��WHILE������
HSDouble CSystemFun::WHILE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	CExpression* pExpression = ((CExpression*)pCompile);

	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	LPNODELEMENT pParam0  = ayVars->GetParam(0);  //�������ʽ������ڵ�
	LPNODELEMENT pParam1  = ayVars->GetParam(1);  //ѭ��������һ�������λ��
	LPNODELEMENT pParam2  = ayVars->GetParam(2);  //ѭ����������һ�������λ��

	//������/�����ж�����˳�
	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL ||
		pParam0  == NULL || pParam1  == NULL )
		return hx_DefMinValues;

	HSCurDataInfoStatus calc0(pCurDataStatus);
	HSCurDataInfoStatus calc1(pCurDataStatus);
	HSCurDataInfoStatus calc2(pCurDataStatus);

	double v0 = pExpression->vexp(pParam0,&calc0);
	if( v0 == (double)hx_DefMinValues )
	{
		return hx_DefMinValues;
	}

	int N1 = -1; int N2 = -1; 
	if (NULL  != pParam1) 
		N1 = (int)pCompile->vexp(pParam1,&calc1);

	if (NULL  != pParam2) 	
		N2 = (int)pCompile->vexp(pParam2,&calc2);


	try
	{	

		CMapVariabile* pVar = pExpression->GetExpDraw();
		//CMapVariabile* pVar = g_hxData.GetExpDraw("test1",m_strExpName.m_dStyle & 0x00FF);

		if (NULL == pVar)
			pVar = (pExpression->m_pRefExpress)->GetExpDraw();

		if( pVar == NULL ||
			pVar->GetObjArray() == NULL )
		{
			AddError(new nodErrorInfo(hxError_Fun_param));
			return hx_DefMinValues;
		}

		while (v0) 
		{
			// һ��һ���м��ʶ���м���
			CArray<CLineProperty*,CLineProperty*> ayData;
			CExpValue* pCurVal;
			CLineProperty* pData;
			for(int nObj = N1; nObj <= N2; nObj++)
			{
				pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
				if( pCurVal != NULL )
				{
					pData = new CLineProperty;
					if( pCurDataStatus )
					{
						pData->m_pDataSource = pCurDataStatus->m_pDataSource;
					}
					ayData.Add(pData);

					pCurVal->CalcValue(pData,0,0,NULL);

				}
			}
			_delArrayObj(ayData);

			//���¼�������ֵ
			v0 = pCompile->vexp(pParam0,&calc0);
		}

		return 0;
	}
	catch(...)
	{
	}

	return hx_DefMinValues; // return 0;

}


// ����: CEILING(var1) ��������ȡ��
HSDouble CSystemFun::CEILING(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return ceil( var1 );
	}

	// ����һ������
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = ceil(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: FLOOR(var1) ��������ȡ��
HSDouble CSystemFun::FLOOR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						   HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return floor( var1 );
	}

	// ����һ������
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = floor(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

// ����: ROUND(var1) ��������ȡ��
HSDouble CSystemFun::ROUND(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						   HSCurDataInfoStatus* pCurDataStatus, int& nError)
{
	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;

	}

	LPNODELEMENT pRefData = ayVars->GetParam(0);

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL || pRefData == NULL )
		return hx_DefMinValues;

	// ���ص�ֵ
	if(pCurDataStatus != NULL && pCurDataStatus->IsStatus(hx_GetCurrentPosData) )
	{
		double var1 = pCompile->vexp(pRefData,pCurDataStatus);
		if( var1 == (double)hx_DefMinValues )
		{
			return hx_DefMinValues;
		}
		return (int)( var1 );
	}

	// ����һ������
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	if( pCompile->vexp(pRefData,&curStatus) == hx_DefMinValues )
		return hx_DefMinValues;
	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);
		for( int nCur = 0; nCur < curStatus.m_lSize; nCur++ )
		{
			if( curStatus.m_pData[nCur] == hx_DefMinValues )
			{
				curStatus.m_pData[nCur] = hx_DefMinValues;
			}
			else
			{
				pCurDataStatus->m_pData[nCur] = (int)(curStatus.m_pData[nCur]);
			}
		}
		return hx_ReturnPointValues;
	}

	return hx_DefMinValues;
}

/*HSDouble CSystemFun::RBTREND(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
						   HSCurDataInfoStatus* pCurDataStatus, int& nError)
{

	if (FAILED(g_pSystemFun->m_hrATIndicator))
    {
		nError = 0;
		return hx_DefMinValues;
	}

	__int64 hHandle;
    g_pSystemFun->m_iATIndicator->CreateObj("T1", &hHandle);

	if(ayVars == NULL || ayVars->GetSize() <= 0)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL)
		return hx_DefMinValues;

	// ����һ������
	HSCurDataInfoStatus curStatus(pCurDataStatus);
	
	LPNODELEMENT pParam0 = ayVars->GetParam(0);

	long lRefPeriod =  (long)pCompile->vexp(pParam0,&curStatus);
	if (lRefPeriod < 1)
	{
		nError = 0;
		return hx_DefMinValues;
	}

	if( GetData(pCompile,ayVars,pCurDataStatus, nError) == NULL )
		return hx_DefMinValues;

	if( GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() <= 0 || 
		pCompile == NULL )
		return hx_DefMinValues;

	if( curStatus.m_pData != NULL)
	{
		pCurDataStatus->Alloc(curStatus.m_lSize);

	HSDouble* pRefHighDay  = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_HIGH_DATA);
	HSDouble* pRefLowDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_LOW_DATA);	
	HSDouble* pRefCloseDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_CLOSE_DATA);
	HSDouble* pRefOpenDay = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetData(hx_OPEN_DATA);

	long lSize = GetData(pCompile,ayVars,pCurDataStatus, nError)->GetDaySize() ;


        VARIANT_BOOL bRet;
        g_pSystemFun->m_iATIndicator->SetSymbolSize(hHandle, lSize + 1);
        for (int i = 0; i < lSize; i++)
        {
			g_pSystemFun->m_iATIndicator->SetSymbolValue(hHandle, i, pRefOpenDay[i].m_dData, pRefHighDay[i].m_dData, pRefLowDay[i].m_dData, pRefCloseDay[i].m_dData, &bRet);
        }


    g_pSystemFun->m_iATIndicator->Calculation(hHandle, 0, lSize-1);

    long lCount = 0;
    g_pSystemFun->m_iATIndicator->PlotCount(hHandle, &lCount);


        float dValue;
        VARIANT_BOOL bIsPlotOn;
        for (int i = 0; i < lSize; i++)
        {
			g_pSystemFun->m_iATIndicator->IsPlotOn(hHandle, 0, i, &bIsPlotOn);
            if (bIsPlotOn)
            {
                g_pSystemFun->m_iATIndicator->GetPlotData(hHandle, 0, i, &dValue);
				pCurDataStatus->m_pData[i] = dValue;
            }
            else
            {
				pCurDataStatus->m_pData[i] = hx_DefMinValues;
            }
		}

		g_pSystemFun->m_iATIndicator->DeleteObj(hHandle);
		return hx_ReturnPointValues;
	}
	return hx_DefMinValues;
}*/

//******************************������litao����*****************************************

