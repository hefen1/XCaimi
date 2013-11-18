/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	SystemFun.h
*	�ļ���ʶ��	�ײ�ϵͳ��������
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
***************************************************************/

#if !defined(_SYSTEMFUN_H_)
#define _SYSTEMFUN_H_

/*#include "..\comm\HSexportbase.h"*/
#include "exppublic.h"
#include "HSDataSource.h"
#include "ExpValue.h"
#include "hsds_comudata.h"

#if 0
#import "C:\Documents and Settings\yulaixin\����\AbleTrend.SH\_bin\AblesysIndicators.exe"
using namespace AblesysIndicatorsLib;
#endif
#define HS_Return_Double -10000
#define HS_Return_Long	  -20000
#define HS_Return_STRING -30000
#define HS_Return_ZGB -40000

//--2010.10.8litao #import "AblesysIndicators.exe"
//--using namespace AblesysIndicatorsLib;

#define  CommRealTimeDataEmpty CommRealTimeData
/*************************************************************
*	�� �� �ƣ�	CSystemFun
*	�� �� ����	�����е�ϵͳ������֯����,ȫΪ��̬����,������
***************************************************************/
class CExpressionBase;
class CExpValue;

class BCGCONTROLBARDLLEXPORT CSystemFun : public CObject
{
//--2010.10.8litao public:
//	CComPtr<IATIndicator> m_iATIndicator;
//	HRESULT m_hrATIndicator;

public:
#if 0
	CComPtr<IATIndicator> m_iATIndicator;
	BOOL m_bLoadCom;
#endif
	CSystemFun();
	// ��ʼ��ȫ�ֵ�����Դ����g_pHxDataSource
	CSystemFun(CHSDataSource* pHxDataSource);
	~CSystemFun();
	// 
public:
	static HSDouble TLOW (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	static HSDouble THIGH (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ���麯��
	/******************************************************************
	*	�������ܣ�	ADVANCE(var1) ���Ǽ���
	*	����������	CExpressionBase* pCompile				: [in] ��ʽ����,��������Ҫ��Ϊ��ȡ������Դ
	*				FormulaGeneralParam* ayVars					: [in] ͨ�ò���
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] ����״̬������������
	*				int& nError								: [out] ���������Ϣ
	*	�� �� ֵ��	HSDouble								: ������
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	static HSDouble ADVANCE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    // ����: 

	// ���º���,���в�����������ͬ,������Ҳ��һ��.��ο�����!!!
	// ���̼�
	static HSDouble OPEN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    // ����: 	

	// ��߼�
	static HSDouble HIGH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ����: LOW(); ��ͼ�
	static HSDouble LOW(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ����: CLOSE(); ���̼�
	static HSDouble CLOSE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ����: VOL(); �ɽ���
	static HSDouble VOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ����: AMOUNT(); �ɽ���
	static HSDouble AMOUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
#ifdef SUPPORT_NETVALUE
	// ����: NETVALUE(); ��ծ���ʣ�����ֵ
	static HSDouble NETVALUE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
#endif

	// ����: DATEDATA(); ����
	static HSDouble DATEDATA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    


	/******************************************************************
	*	�������ܣ�
	*	����������	CExpressionBase* pCompile
	*				FormulaGeneralParam* ayVars
	*				HSCurDataInfoStatus* pCurDataStatus
	*				int& nError
	*	�� �� ֵ��	HSDouble 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	static BOOL CompareCloseOpen(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);


	// ����: ASKPRICE(var1) ί���� 1-3
	static HSDouble ASKPRICE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ����: ASKVOL(var1) ί���� 1-3
	static HSDouble ASKVOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// BIDPRICE(var1) ί��� 1-3
	static HSDouble BIDPRICE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    // ����: 

	// ����: BIDVOL(var1) ί���� 1-3
	static HSDouble BIDVOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  

	// ����: BUYVOL(); ��������
	static HSDouble BUYVOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ����: DECLINE(); �µ�����
	static HSDouble DECLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ����: EXTDATA(var1) ��չ����1-11
	static HSDouble EXTDATA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ����: ISBUYORDER(); �Ƿ���������
	static HSDouble ISBUYORDER(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ����: SELLVOL(); ����������
	static HSDouble SELLVOL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    


	// ʱ�亯��
	// ����: DATE(); ������
	static HSDouble hxDATE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// ����: PERIOD_TYPE_DAY(); ����
	static HSDouble hxDAY(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);   
	// ����: HOUR(); Сʱ
	static HSDouble HSOUR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: MINUTE(); ����
	static HSDouble hxMINUTE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: MONTH(); �·�
	static HSDouble hxMONTH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: TIME(); ʱ����
	static HSDouble hxTIME(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: WEEKDAY(); ����
	static HSDouble hxWEEKDAY(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: YEAR(); ���
	static HSDouble hxYEAR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	//���ú���
	// ����: BACKSET(var1,var2) ��ǰ��ֵ
	static HSDouble BACKSET(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  BARSCOUNT(var1) ��Ч������
	static HSDouble BARSCOUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: BARSLAST(var1) ��һ����������λ��
	static HSDouble BARSLAST(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: BARSSINCE(var1) ��һ����������λ��
	static HSDouble BARSSINCE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);   
	// ����: COUNT(var1,var2) ͳ������
	static HSDouble COUNT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  DMA(var1,var2) ��̬�ƶ�ƽ��
	static HSDouble DMA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ����: EMA(var1,var2) ָ��ƽ���ƶ�ƽ��
	static HSDouble EMA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError );    

	// ����: HHV(var1,var2) ���ֵ
	static HSDouble HHV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: HHVBARS(var1,var2) ��һ�ߵ㵽��ǰ��������
	static HSDouble HHVBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  LLV(var1,var2) ���ֵ
	static HSDouble LLV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: LLVBARS(var1,var2) ��һ�͵㵽��ǰ��������
	static HSDouble LLVBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: MA(var1,var2) ���ƶ�ƽ��
	static HSDouble MA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: REF(var1,var2) ������������ǰ������
	static HSDouble REF(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  SMA(var1,var2,var3) �ƶ�ƽ��
	static HSDouble SMA(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: SUM(var1,var2) �ܺ�
	static HSDouble SUM(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: SUMBARS(var1,var2) ��ǰ�ۼӵ�ָ��ֵ�����ڵ�������
	static HSDouble SUMBARS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// �߼�����
	// ����: IF(var1,var2,var3) ����������ͬ��ֵ
	static HSDouble IF(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: ISDOWN();  �������Ƿ�����
	static HSDouble ISDOWN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: ISEQUAL(); �������Ƿ�ƽ��
	static HSDouble ISEQUAL(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: ISUP();    �������Ƿ�����
	static HSDouble ISUP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ��������
	// ����: ABS(var1) �����ֵ
	static HSDouble ABS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: CROSS(var1,var2) �����߽���
	static HSDouble CROSS(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  MAX(var1,var2) �����ֵ
	static HSDouble MAX(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);   
	// ����: MIN(var1,var2) ����Сֵ
	static HSDouble MIN(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);   
	// ����: MOD(var1,var2) ��ģ����
	static HSDouble MOD		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: NOT(var1) ���߼���
	static HSDouble NOT		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  REVERSE(var1) ���෴��
	static HSDouble REVERSE	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// ����: SGN(var1) �����ֵ
	static HSDouble SGN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ��ѧ����
	// ����: ACOS(var1) ������ֵ
	static HSDouble ACOS	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: ASIN(var1) ������ֵ
	static HSDouble ASIN	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// ����:  ATAN(var1) ������ֵ
	static HSDouble ATAN	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: COS(var1) ����ֵ
	static HSDouble COS		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: EXP(var1) ָ��
	static HSDouble EXP		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// ����: LN(var1) ����Ȼ����
	static HSDouble LN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  LOG(var1) ��10Ϊ�׵Ķ���
	static HSDouble LOG		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: POW(var1,var2) ����
	static HSDouble POW		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: SIN(var1) ����ֵ
	static HSDouble SIN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: SQRT(var1) ��ƽ��
	static HSDouble SQRT	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  TAN(var1) ����ֵ
	static HSDouble TAN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ͳ�ƺ���
	// ����: AVEDEV(var1,var2) ƽ������ƫ��
	static HSDouble AVEDEV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: DEVSQ(var1,var2) ����ƫ��ƽ����
	static HSDouble DEVSQ(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: FORCAST(var1,var2) ���Իع�Ԥ��ֵ
	static HSDouble FORCAST		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  SLOPE(var1,var2) ���Իع�б��
	static HSDouble SLOPE		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: STD(var1,var2) �����׼��
	static HSDouble STD(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: STDP(var1,var2) �����׼��	
	static HSDouble STDP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);   
	// ����: VAR(var1,var2) ������������
	static HSDouble VAR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  VARP(var1,var2) ������������
	static HSDouble VARP(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);     

	// ָ������
	// ����: COST(var1) �ɱ��ֲ����
	static HSDouble COST		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: PEAK(var1,var2,var3) ǰM��ZIGת�򲨷�ֵ
	static HSDouble PEAK		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: PEAKBARS(var1,var2,var3) ǰM��ZIGת�򲨷嵽��ǰ����
	static HSDouble PEAKBARS	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  SAR(var1,var2,var3) ����ת��
	static HSDouble SAR			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: SARTURN(var1,var2,var3) ����ת���
	static HSDouble SARTURN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// ����: TROUGH(var1,var2,var3) ǰM��ZIGת�򲨹�ֵ
	static HSDouble TROUGH		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: TROUGHBARS(var1,var2,var3) ǰM��ZIGת�򲨹ȵ���ǰ����
	static HSDouble TROUGHBARS	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  WINNER(var1) �����̱���
	static HSDouble WINNER		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  ZIG(var1,var2) ֮��ת��
	static HSDouble ZIG			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  LWINNER(var1) ���ڻ����̱���
	static HSDouble LWINNER		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  PWINNER(var1) Զ�ڻ����̱���
	static HSDouble PWINNER		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ���̺���
	// ����: INDEXA(); ��Ӧ���̳ɽ���
	static HSDouble INDEXA			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: INDEXADV(); ��Ӧ�������Ǽ���
	static HSDouble INDEXADV		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  INDEXC(); ��Ӧ�������̼�
	static HSDouble INDEXC			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  INDEXDEC(); ��Ӧ�����µ�����
	static HSDouble INDEXDEC		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: INDEXH(); ��Ӧ������߼�
	static HSDouble INDEXH			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: INDEXL(); ��Ӧ������ͼ�
	static HSDouble INDEXL			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  INDEXO(); ��Ӧ���̿��̼�
	static HSDouble INDEXO			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����:  INDEXV(); ��Ӧ���̳ɽ���
	static HSDouble INDEXV			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ���ú���
	// ����: CAPITAL(); ��ͨ�̴�С
	static HSDouble CAPITAL		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	// ����: VOLUNIT(); ÿ�ֹ���
	static HSDouble VOLUNIT		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);    

	// ��̬���麯��
	static HSDouble DYNAINFO	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 
	// ����:  DYNAINFO(3) ��̬���麯��: ����
	// ����:  DYNAINFO(4) ��̬���麯��: ��
	// ����:  DYNAINFO(5) ��̬���麯��: ���
	// ����:  DYNAINFO(6) ��̬���麯��: ���
	// ����:  DYNAINFO(7) ��̬���麯��: ����
	// ����:  DYNAINFO(8) ��̬���麯��: ����
	// ����:  DYNAINFO(9) ��̬���麯��: ����
	// ����:  DYNAINFO(10) ��̬���麯��: �ܶ�
	// ����:  DYNAINFO(11) ��̬���麯��: ����
	// ����:  DYNAINFO(12) ��̬���麯��: �ǵ�
	// ����:  DYNAINFO(13) ��̬���麯��: ���
	// ����:  DYNAINFO(14) ��̬���麯��: �Ƿ�
	// ����:  DYNAINFO(15) ��̬���麯��: ί��
	// ����:  DYNAINFO(16) ��̬���麯��: ί��
	// ����:  DYNAINFO(17) ��̬���麯��: ����
	// ����:  DYNAINFO(18) ��̬���麯��: ί��
	// ����:  DYNAINFO(19) ��̬���麯��: ί��
	// ����:  DYNAINFO(20) ��̬���麯��: ί���
	// ����:  DYNAINFO(21) ��̬���麯��: ί����
	// ����:  DYNAINFO(22) ��̬���麯��: ����
	// ����:  DYNAINFO(23) ��̬���麯��: ����
	// ����:  DYNAINFO(24) ��̬���麯��: ����
	// ����:  DYNAINFO(25) ��̬���麯��: ��һ��
	// ����:  DYNAINFO(26) ��̬���麯��: �����
	// ����:  DYNAINFO(27) ��̬���麯��: ������
	// ����:  DYNAINFO(28) ��̬���麯��: ��һ��
	// ����:  DYNAINFO(29) ��̬���麯��: �����
	// ����:  DYNAINFO(30) ��̬���麯��: ������
	// ����:  DYNAINFO(31) ��̬���麯��: ��һ��
	// ����:  DYNAINFO(32) ��̬���麯��: ������
	// ����:  DYNAINFO(33) ��̬���麯��: ������
	// ����:  DYNAINFO(34) ��̬���麯��: ��һ��
	// ����:  DYNAINFO(35) ��̬���麯��: ������
	// ����:  DYNAINFO(36) ��̬���麯��: ������
	// ����:  DYNAINFO(37) ��̬���麯��: ������
	// ����:  DYNAINFO(38) ��̬���麯��: 5�վ���
	// ����:  DYNAINFO(39) ��̬���麯��: ��ӯ��
	// ����:  DYNAINFO(40) ��̬���麯��: �ɽ�����

	// ����ָ��
	static HSDouble FINANCE	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ����:  FINANCE(1) ����ָ��: �ܹɱ�(���)
	// ����:  FINANCE(2) ����ָ��: ���ҹ�
	// ����:  FINANCE(3) ����ָ��: �����˷��˹�
	// ����:  FINANCE(4) ����ָ��: ���˹�
	// ����:  FINANCE(5) ����ָ��: B��
	// ����:  FINANCE(6) ����ָ��: H��
	// ����:  FINANCE(7) ����ָ��: ��ͨA��
	// ����:  FINANCE(8) ����ָ��: ְ����
	// ����:  FINANCE(9) ����ָ��: A2ת���
	// ����:  FINANCE(10) ����ָ��: ���ʲ�(ǧԪ)
	// ����:  FINANCE(11) ����ָ��: �����ʲ�
	// ����:  FINANCE(12) ����ָ��: �̶��ʲ�
	// ����:  FINANCE(13) ����ָ��: �����ʲ�
	// ����:  FINANCE(14) ����ָ��: ����Ͷ��
	// ����:  FINANCE(15) ����ָ��: ������ծ
	// ����:  FINANCE(16) ����ָ��: ���ڸ�ծ
	// ����:  FINANCE(17) ����ָ��: �ʱ�������
	// ����:  FINANCE(18) ����ָ��: ÿ�ɹ�����
	// ����:  FINANCE(19) ����ָ��: �ɶ�Ȩ��
	// ����:  FINANCE(20) ����ָ��: ��Ӫ����
	// ����:  FINANCE(21) ����ָ��: ��Ӫ����
	// ����:  FINANCE(22) ����ָ��: ��������
	// ����:  FINANCE(23) ����ָ��: Ӫҵ����
	// ����:  FINANCE(24) ����ָ��: Ͷ������
	// ����:  FINANCE(25) ����ָ��: ��������
	// ����:  FINANCE(26) ����ָ��: Ӫҵ����֧
	// ����:  FINANCE(27) ����ָ��: �����������
	// ����:  FINANCE(28) ����ָ��: �����ܶ�
	// ����:  FINANCE(29) ����ָ��: ˰������
	// ����:  FINANCE(30) ����ָ��: ������
	// ����:  FINANCE(31) ����ָ��: δ��������
	// ����:  FINANCE(32) ����ָ��: ÿ��δ����
	// ����:  FINANCE(33) ����ָ��: ÿ������
	// ����:  FINANCE(34) ����ָ��: ÿ�ɾ��ʲ�
	// ����:  FINANCE(35) ����ָ��: ����ÿ�ɾ���
	// ����:  FINANCE(36) ����ָ��: �ɶ�Ȩ���
	// ����:  FINANCE(37) ����ָ��: ����������

	// ��������
	// �������ͳ���
	// ��״�ߣ���0��ָ��ֵ�仭��ֱֱ��
	static HSDouble STICK				(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ��ɫ��״�ߣ���ֵΪ0����ʾ��ɫ��������ʾ��ɫ
	static HSDouble COLORSTICK	   	    (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// �ɽ�����״�ߣ����ɼ�����ʱ��ʾ��ɫ��������������ʾ��ɫ������
	static HSDouble VOLSTICK			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ͬʱ������״�ߺ�ָ����
	static HSDouble LINESTICK			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// С����
	static HSDouble CROSSDOT			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// СԲȦ��
	static HSDouble CIRCLEDOT			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// СԲ����
	static HSDouble POINTDOT			(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// 3d
	static HSDouble DRAW3D( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  

	// ��3D����
	static HSDouble LINEDRAW3D( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError); 

	// ��ǰ���߲�����
	static HSDouble NOTDRAWLINE( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  

	// ��ǰ�ı�������
	static HSDouble NOTDRAWTEXT( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  

	// ��ǰ�����ı���������
	static HSDouble NOTDRAWLINETEXT( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	
	static HSDouble NOTDRAW( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);	

	static HSDouble NOTTABLE( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);	

	// �����������Сֵ,�ڻ�ͼʱʹ��
	static HSDouble NOTMAXMIN( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	

	// ���K����ɫ
	// ��ɫ
	static HSDouble COLORRED	    (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ��ɫ
	static HSDouble COLORGREEN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ��ɫ
	static HSDouble COLORBLUE		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ��ɫ
	static HSDouble COLORBLACK		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ��ɫ
	static HSDouble COLORWHITE		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ��ɫ
	static HSDouble COLORGRAY		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ��ɫ
	static HSDouble COLORYELLOW	    (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ��ɫ
	static HSDouble COLORCYAN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// Ʒ��ɫ
	static HSDouble COLORMAGENTA	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ��ɫ	
	static HSDouble COLORBROWN		(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);  
	// ָ��ɫ
	static HSDouble hxRGB	    	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// �߷����
	// ʵ��
	static HSDouble hxSOLID(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	/* -------  */
	static HSDouble hxDASH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);       
	/* .......  */
	static HSDouble hxDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);        
	/* _._._._  */
	static HSDouble hxDASHDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);    
	/* _.._.._  */
	static HSDouble hxDASHDOTDOT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError); 

	// �û���ͼ����
	// ��ͼ��
	static HSDouble DRAWICON(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ����
	static HSDouble DRAWLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);       /*   */
	// ���ı�
	static HSDouble DRAWTEXT(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);        /*   */
	// ����
	static HSDouble POLYLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);    /*   */
	// 
	static HSDouble STICKLINE(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);    /*  */

	// �����ڲ����㺯��
	// �õ�����Դ
	static CHSDataSource* GetDataSource(CExpressionBase* pCompile);
	// �õ�������
	static HSDouble Get5DayVol(CHSDataSource* pHxData, int nDayPos);

	// ǿ�ȷ�����
	// �õ���ʷ����
	static HSDouble GetHisLIANGBI(CHSDataSource* pHxData, int nDayPos);
	// n��ǿ��
	static HSDouble QIANGDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// �ǵ�����=
	static HSDouble ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ������
	static HSDouble HUANSHOULV(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ���ȷ���"
	static HSDouble LIANGBIFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// �𵴷���"
	static HSDouble ZHENGDANGFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// ���������
	// �����г�����
	static HSDouble BetweenShiChangBiLi(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ���俪��
	static HSDouble BetweenOpen(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ��������
	static HSDouble BetweenClose(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// �������
	static HSDouble BetweenHigh(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// �������
	static HSDouble BetweenLow(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ����ɽ���
	static HSDouble BetweenVol(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ����ɽ���
	static HSDouble BetweenAmount(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ���ļ�
	static HSDouble CenterPrice(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ��ʼʱ��
	static HSDouble BetweenBeginDate(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ����ʱ��
	static HSDouble BetweenEndDate(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// ��������
	// �ǵ���
	static HSDouble BK_ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// Ȩ�ǵ���
	static HSDouble BK_Q_ZHANGDIEFUDU(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// �ܳɽ���
	static HSDouble BK_Vol(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// �ɽ���
	static HSDouble BK_Amount(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// �ɽ�����
	static HSDouble BK_Deal(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ������
	static HSDouble BK_HuanShouLv(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ��ӯ��
	static HSDouble BK_ShiYing(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ���ǹ�
	static HSDouble BK_LingZhang(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// �����
	static HSDouble BK_LingDie(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ��Ʊ��
	static HSDouble BK_Num(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ��Ʊ��
	static HSDouble BK_UpRatio(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,HSCurDataInfoStatus* pCurDataStatus, int& nError);
public:
	/******************************************************************
	*	�������ܣ�	��YYYMMDD��ʱ���ʽת���ɻ���1970Ϊ����ĺ�����
	*	����������	long lDate	: [in] ��YYYMMDDΪ��ʾ��ʱ��
	*	�� �� ֵ��	time_t	    : ����1970Ϊ����ĺ�����
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	static	time_t	   hxLongToTime(long lDate);

	/******************************************************************
	*	�������ܣ�	��������Դ��ͬʱ����g_pSystemFunȫ�ֱ���
	*	����������	CHSDataSource* pHxDataSource	: [in] ����Դ
	*	�� �� ֵ��	CSystemFun*						: ȫ�ֵ�ϵͳ�������������
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	static CSystemFun*    GetSystemFunPtr(CHSDataSource* pHxDataSource);

	/******************************************************************
	*	�������ܣ�	��������Դ
	*	����������	CHSDataSource* pHxDataSource : [in] ����Դ
	*	�� �� ֵ��	CHSDataSource*				 : �ɵ�����Դ
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	static CHSDataSource* SetDataSource(CHSDataSource* pHxDataSource);

	/******************************************************************
	*	�������ܣ�	��Ч���ݵ�ƫ����
	*	����������	HSDouble* pData : [in] Ҫ���ı�������
	*				int nSize		: [in] ���鳤��
	*	�� �� ֵ��	int				: ��һ����Ч���ݵ�������
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	static int		      GetDataValidPos(HSDouble* pData,int nSize);


	// ָ�����͵�רҵ��������
	static HSDouble PROFFIN	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ָ�����ڵ�רҵ��������
	static HSDouble PROFFINON	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ����ַ���
	static HSDouble STROUT	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ����ת������
	static HSDouble DATETOSTR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	
	static HSDouble BLOCKCALC	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	
	

	//2.	PROFSTR				רҵ�����ַ�������
	static HSDouble PROFSTR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//PROFSTR(N,M)
	//��ʾȡ��M��������֮ǰ�ĵ�N��רҵ�����ַ�������,
	//����PROFSTR(4001,0)��ʾ���һ�ڵ�һ��ɶ����ƣ�N��ȡֵ������±�

	//3.	PROFFINLAST			רҵ�������ݽ�ֹ��λ��
	static HSDouble PROFFINLAST	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//PROFFINLAST(N)
	//��ʾ��������ڵĵ�N��רҵ�������ݾ������ڵ���������N��ȡֵ������±�

	
	//5.	PROFSTRON			ָ�����ڵ�רҵ�ַ�����������
	static HSDouble PROFSTRON	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//PROFFINON(N,Y,MD)
	//��ʾȡ��Y��M��D�յĵ�N��רҵ�������ݣ�
	//��PROFSTRON(5001,2000,0101)ȡ��2000��1��1�յĵ�һ��ɶ����ƣ�N��ȡֵ������±�

	//6.	PROFFINTERM		רҵ�������ݱ�����
	static HSDouble PROFFINTERM	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//PROFFINTERM(M)
	//��ʾȡ��M��������֮ǰ�Ĳ��񱨱����걨���б����Ǽ���
	//����1����һ���ȼ�����2���б���3���������ȼ�����4���걨
	//���磺��PROFFINTERM(0)����4�����ʾ���һ�ڲ��񱨱����걨��

	//7.	SETPROFFIN			����רҵ��������
	static HSDouble SETPROFFIN	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//SETPROFFIN(X)
	//�ڱ������������רҵ���������ܵ������������õ�Ӱ�죬��ʾ��רҵ�������������趨ΪX��X�ӵ͵���ÿһλ��ʾһ������
	//��1λ�������걨��
	//��2λ�������б���
	//��3λ������������
	//��4λ���������²���ָ�꣬
	//��5λ�������б�����������ָ�꣬
	//����SETPROFFIN(01011)��ʾȡ���걨���б�������һ�����ݣ��������Ǹ������ڶ�Ҫ�������ڣ��б���������
	//��������ñ�������ϵͳĬ��ֵΪ01111


	// ����ͨ-�ʺ�
	static HSDouble HLTCH(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// ����ͨ-���Ȧ
	static HSDouble HLTHBQ(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
    // ����ͨ-���Ȧ
	static HSDouble HLTHBQ1(long lRefPeriod, long lRefCount,HSCurDataInfoStatus* pCurDataStatus,
				 HSDouble* pRefHighDay,HSDouble* pRefLowDay,HSDouble* pRefCloseDay, int& nError);
    // ����ͨ-���Ȧ
    static HSDouble HLTHBQ2(long lRefPeriod, long lRefCount,HSCurDataInfoStatus* pCurDataStatus,
				 HSDouble* pRefHighDay,HSDouble* pRefLowDay,HSDouble* pRefCloseDay, int& nError);

	// ѭ�����
public:
	static HSDouble FOR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	static HSDouble CONTINUE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	static HSDouble BREAK	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	static HSDouble OPERATOR	(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	//******************************������litao����*****************************************
	//���������ز����Ƶ���������
	static HSDouble DRAWNULL (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError)
	{
		return hx_DefMinValues;
	}
	// ���˺���������������N�����ں����������Ϊ0
	static HSDouble FILTER (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// WHILE (�������ʽ) DO  [ѭ�������] END LOOP
	static HSDouble WHILE (CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	// ����: CEILING(var1) ��������ȡ��
	static HSDouble CSystemFun::CEILING(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ����: FLOOR(var1) ��������ȡ��
	static HSDouble CSystemFun::FLOOR(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ����: ROUND(var1) ��������ȡ��
	static HSDouble CSystemFun::ROUND(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	// ����: RBTREND() AbleTrend�ĺ����߱�־
	//--2010.10.8static HSDouble CSystemFun::RBTREND(CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
	//	HSCurDataInfoStatus* pCurDataStatus, int& nError);
	//******************************������litao����*****************************************
};
#endif
