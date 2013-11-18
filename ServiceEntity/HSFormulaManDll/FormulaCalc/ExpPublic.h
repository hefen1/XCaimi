/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	ExpPublic.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	�����˹�ʽ����Ҫ��CMapVariabile�༰һЩID
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
***************************************************************/

#if !defined(_EXPPUBLIC_H_)
#define _EXPPUBLIC_H_
#include "hsds_comudata.h"
#include "hsstructnew.h"
#include "ConfigStruct.h"
#include "QuoteStuctDefine.h"

#include "ExportDll.h"
#include "CompileInterFace.h"
#include "FormulaStruct.h"
#include "windowsx.h"
#include <math.h>
#include <ctype.h>
#include <io.h>
#include <direct.h>
#include <mbstring.h>
#include <afxcmn.h>

//******************************������litao����*****************************************
#include <afxmt.h>
#define COMPILE_NO 1
#define EXECUTE_NO 2
#define NOHAVE_ID 0
#define HAVE_ID 1
#define MORELOOPERROR -999
#define MAXLOOPTIMES  1000
//******************************������litao����*****************************************


#define HS_GETSTOCK_BYCODE				    0x00000100
#define HS_GETSTOCK_BYMAKET					0x00000200 

// ѡ��dlgʹ�� ref DlgCondition.cpp
#define  CDlgCondition_Text_Prompt				  _T("ѡ����: %i,ѡ�б���: %.2f%%")
#define  CDlgCondition_Not_Select_Error			  _T("�����ú��������!")
#define  CDlgCondition_But_Stop					  _T("��ֹѡ��&Z")
#define  CDlgCondition_But_Run					  _T("ִ��ѡ��&Z")
#define  CDlgCondition_But_Request				  _T("�Ƿ���ֹѡ�ɣ�")
#define  CDlgCondition_Default_Name				  _T("����ѡ��")

#define  CDlgCondition_MessageBox_Caption		  _T("ѡ��")
#define  CDlgCondition_Data_Prompt				  _T("ѡ�����̺����ص������н��У���ȷ����������!")

// CDlgSetStockAndTime ref DlgSetStockAndTime.cpp
// ѡ�����ò�������/װ��
#define  CDlgSetStockAndTime_Section			  _T("ѡ��-ʱ��-��Ʊ��Χ-ѡ��")
#define  CDlgSetStockAndTime_Time				  _T("ʱ��")
#define  CDlgSetStockAndTime_Stock				  _T("��Ʊ��Χ")
#define  CDlgSetStockAndTime_Option				  _T("ѡ��")

#define  CDlgSetStockAndTime_Begin_End_Date_Error _T("��ʼʱ�����ô���")

// CDlgUnionConSelStock ref DlgUnionConSelStock.cpp
#define  CDlgUnionConSelStock_Ok_Prompt			  _T("�������Ϊ�գ�ȷ���Ƿ�ɾ����")
#define  CDlgUnionConSelStock_Not_Name			  _T("���Ʋ���Ϊ�գ�")

#define  CDlgUnionConSelStock_Cond_Or			  _T("��������֮һ����")
#define  CDlgUnionConSelStock_Cond_And			  _T("��������ͬʱ����")
#define  CDlgUnionConSelStock_Cond_Or_Pre     	  _T("��ǰ%i-%i��֮��,��������֮һ����")
#define  CDlgUnionConSelStock_Cond_And_Pre	      _T("��ǰ%i-%i��֮��,��������ͬʱ����")

// ��ʽ�Զ���װ����
#define EXTERN_EXPRESS_OVR		    0x0001  // �������й�ʽ
#define EXTERN_EXPRESS_AUTO_NAME	0x0002  // �Զ�����
#define EXTERN_EXPRESS_CHANGE_NAME  0x0004  // ����
#define EXTERN_EXPRESS_SAME_NAME	0x0008  // �Ƿ�����
#define EXTERN_EXPRESS_AUTO_SETUP	0x0010  // �Զ���װ��ʽ

#define  SelectingStock     0x100       //ѡ�ɹ�����
#define  SelectStockOver    0x101       //ѡ�ɽ���

#define BLOCK_NAME_LENGTH		32			// ������Ƴ���
#define _delObject(p)    { if(p) {delete p;   p = NULL; } }
#define _delArray(p)	 { if(p) {delete[] p; p = NULL; } }
#define _delArrayObj(p)  { for(int i = p.GetSize() - 1; i >= 0; i--) { delete p.GetAt(i); } p.RemoveAll(); }

// ���Ų���
#define Operator_Begin  150
#define Operator_End    255
		// ��ѧ����
#define Operator_SIN		 (Operator_Begin+1) // ���Һ��� SIN
#define Operator_COS		 (Operator_Begin+2) // ����
#define Operator_EXP		 (Operator_Begin+3) // ָ��
#define Operator_SQRT		 (Operator_Begin+4) // ƽ����
#define Operator_LOG		 (Operator_Begin+5) // ����
#define Operator_TG			 (Operator_Begin+6) // ����
#define Operator_CTG		 (Operator_Begin+7) // ����
#define Operator_ASIN		 (Operator_Begin+8) // �����Һ���
#define Operator_ACOS		 (Operator_Begin+9) // ������
#define Operator_ATG		 (Operator_Begin+10) // ������
        // ������
#define Operator_LargerEqual (Operator_Begin+11) // >=     ���ڵ���
#define Operator_SmallEqual  (Operator_Begin+12) // <=     С�ڵ���
#define Operator_NotEqual    (Operator_Begin+13) // != <>  ������
#define Operator_OR          (Operator_Begin+14) // || or  ��
#define Operator_AND         (Operator_Begin+15) // && and ����
#define Operator_Semicolon   (Operator_Begin+16) // ;  �ֺ�(�����б���)
#define Operator_Komma		 (Operator_Begin+17) // ,  ����
#define Operator_Evaluate	 (Operator_Begin+18) // := ��ֵ
#define Operator_Colon		 (Operator_Begin+19) // :  ð��(��ͼ)
#define Operator_Function	 (Operator_Begin+20) // ����
#define Operator_Express	 (Operator_Begin+21) // ���ù�ʽ
#define Operator_InportData	 (Operator_Begin+22) // ��������
#define Operator_While		 (Operator_Begin+23) // WHILE

#define Operator_Error       Operator_End // ����������

// �����
#define hxError_Invalid				 (-1000)			    // δ֪����
#define hxError_DIVISION_BY_0		 (hxError_Invalid -  1) // �� 0 ��
#define hxError_ILEGAL_OPERATION	 (hxError_Invalid -  2)
#define hxError_UNDEFINED_VARIABLE   (hxError_Invalid -  3) // ����δ����
#define hxError_INVALID_DOMAIN	     (hxError_Invalid -  4) // log ������
#define hxError_Fun_Exist			 (hxError_Invalid - 5)  // �����Ѿ�����
#define hxError_Var_Exist			 (hxError_Invalid - 6)  // �����Ѿ�����
#define hxError_Nod_CloneFail		 (hxError_Invalid - 7)  // ������Ԫ����ʧ��
#define hxError_InvalidateVars       (hxError_Invalid - 8)  // ��Ч����
#define hxError_NotFunction	         (hxError_Invalid - 9)  // ����������
#define hxError_NotVariable	         (hxError_Invalid - 10) // ����������
#define hxError_Operator	         (hxError_Invalid - 11) // ����������
#define hxError_Fun_param			 (hxError_Invalid - 12) // ������������
#define hxError_Identification		 (hxError_Invalid - 13) // ����ʶ����ַ�

#define hxError_AddRight		     (hxError_Invalid - 14) // +�Ӻ��ұߴ���
#define hxError_SubRight		     (hxError_Invalid - 15) // -�����ұߴ���
#define hxError_RideRight		     (hxError_Invalid - 16) // +�˺��ұߴ���
#define hxError_DivideRight		     (hxError_Invalid - 17) // -�����ұߴ���
#define hxError_PowerRight		     (hxError_Invalid - 18) // ^�˷��ұߴ���
#define hxError_BracketLeft		     (hxError_Invalid - 19) // (�����Ŵ���
#define hxError_BracketRight	     (hxError_Invalid - 21) // )�����Ŵ���
#define hxError_AbsLeft	     	     (hxError_Invalid - 22) // | |����ֵ��ߴ���
#define hxError_AbsRight     	     (hxError_Invalid - 23) // | |����ֵ�ұߴ���
#define hxError_LogicalRight     	 (hxError_Invalid - 24) // �߼��������ұߴ���(< > = == >= <= <> !=)
#define hxError_CommaRight     	     (hxError_Invalid - 25) // �����ұߴ���
#define hxError_OrRight     	     (hxError_Invalid - 26) // or || �߼����ұߴ���
#define hxError_AndRight     	     (hxError_Invalid - 27) // and && �߼������ұߴ���
#define hxError_DefinedRight  	     (hxError_Invalid - 28) // :=��ֵ���ʽ�ұߴ���
#define hxError_DrawRight	  	     (hxError_Invalid - 29) // :��ͼ�������ұߴ���
#define hxError_ReverseRight	  	 (hxError_Invalid - 30) // !ȡ���ұߴ���
#define hxError_Name_Exist			 (hxError_Invalid - 31)  // ��ʶ���Ѿ�����
#define hxError_NotName_Exist		 (hxError_Invalid - 32)  // ��ʶ��δ����
#define hxError_Notes_Invalidate	 (hxError_Invalid - 33)  // ע�ʹ���/**/ //
#define hxError_AtLeastHasExp		 (hxError_Invalid - 34)  // ������Ҫһ�����ʽ
#define hxError_AtLeastHasDrawExp	 (hxError_Invalid - 35)  // ������Ҫһ����ͼ���ʽ
#define hxError_WantInvertedComma	 (hxError_Invalid - 36)  // ȱ�����÷���
#define hxError_Exp_param			 (hxError_Invalid - 37)  // ȱ�����ò���
#define hxError_unknowPeriod		 (hxError_Invalid - 38)  // ����ʶ�������
//******************************������litao����*****************************************
#define hxError_MoreLoop			 (hxError_Invalid - 39)  // ѭ������̫��
//******************************������litao����*****************************************

// ��������
#define FunOper_Begin 100
#define FunOper_End   1000

// �û����庯��
#define FunOper_UserDefFun_Begin (FunOper_Begin+1)
#define FunOper_UserDefFun_End   (FunOper_UserDefFun_Begin+100)

// ���麯��
#define FunOper_QUOTES_Begin    (FunOper_UserDefFun_End + 1)
#define FunOper_QUOTES_End      (FunOper_QUOTES_Begin + 20)

#define FunOper_ADVANCE			(FunOper_QUOTES_Begin+1)    // ����: ADVANCE(var1) ���Ǽ���
#define FunOper_AMOUNT			(FunOper_QUOTES_Begin+2)    // ����: AMOUNT() �ɽ���
#define FunOper_ASKPRICE		(FunOper_QUOTES_Begin+3)    // ����: ASKPRICE(var1) ί���� 1-3
#define FunOper_ASKVOL			(FunOper_QUOTES_Begin+4)    // ����: ASKVOL(var1) ί���� 1-3
#define FunOper_BIDPRICE		(FunOper_QUOTES_Begin+5)    // ����: BIDPRICE(var1) ί��� 1-3
#define FunOper_BIDVOL			(FunOper_QUOTES_Begin+6)    // ����: BIDVOL(var1) ί���� 1-3
#define FunOper_BUYVOL			(FunOper_QUOTES_Begin+7)    // ����: BUYVOL() ��������
#define FunOper_CLOSE			(FunOper_QUOTES_Begin+8)    // ����: CLOSE() ���̼�
#define FunOper_DECLINE			(FunOper_QUOTES_Begin+9)    // ����: DECLINE() �µ�����
#define FunOper_EXTDATA			(FunOper_QUOTES_Begin+10)    // ����: EXTDATA(var1) ��չ����1-11
#define FunOper_HIGH			(FunOper_QUOTES_Begin+11)    // ����: HIGH() ��߼�
#define FunOper_ISBUYORDER		(FunOper_QUOTES_Begin+12)    // ����: ISBUYORDER() �Ƿ���������
#define FunOper_LOW				(FunOper_QUOTES_Begin+13)    // ����: LOW() ��ͼ�
#define FunOper_OPEN			(FunOper_QUOTES_Begin+14)    // ����: OPEN() ���̼�
#define FunOper_SELLVOL			(FunOper_QUOTES_Begin+15)    // ����: SELLVOL() ����������
#define FunOper_VOL				(FunOper_QUOTES_Begin+16)    // ����: VOL() �ɽ���
#ifdef SUPPORT_NETVALUE
#define FunOper_NETVALUE		(FunOper_QUOTES_Begin+17)    // ����: NETVALUE() ��ծ����,����ֵ����λ��Ԫ)
#endif

// ʱ�亯��
#define FunOper_DateTime_Begin    (FunOper_QUOTES_End + 1)
#define FunOper_DateTime_End      (FunOper_DateTime_Begin + 10)

#define FunOper_DATE			(FunOper_DateTime_Begin+1)    // ����: DATE() ������
#define FunOper_PERIOD_TYPE_DAY				(FunOper_DateTime_Begin+2)    // ����: PERIOD_TYPE_DAY() ����
#define FunOper_HOUR			(FunOper_DateTime_Begin+3)    // ����: HOUR() Сʱ
#define FunOper_MINUTE			(FunOper_DateTime_Begin+4)    // ����: MINUTE() ����
#define FunOper_MONTH			(FunOper_DateTime_Begin+5)    // ����: MONTH() �·�
#define FunOper_TIME			(FunOper_DateTime_Begin+6)    // ����:  TIME() ʱ����
#define FunOper_WEEKDAY			(FunOper_DateTime_Begin+7)    // ����: WEEKDAY() ����
#define FunOper_YEAR			(FunOper_DateTime_Begin+8)    // ����: YEAR() ���

//���ú���
#define FunOper_Refer_Begin    (FunOper_DateTime_End + 1)
#define FunOper_Refer_End      (FunOper_Refer_Begin + 20)

#define FunOper_BACKSET			(FunOper_Refer_Begin+1)    // ����: BACKSET(var1,var2) ��ǰ��ֵ
#define FunOper_BARSCOUNT		(FunOper_Refer_Begin+2)    // ����:  BARSCOUNT(var1) ��Ч������
#define FunOper_BARSLAST		(FunOper_Refer_Begin+3)    // ����: BARSLAST(var1) ��һ����������λ��
#define FunOper_BARSSINCE		(FunOper_Refer_Begin+4)    // ����: BARSSINCE(var1) ��һ����������λ��
#define FunOper_COUNT			(FunOper_Refer_Begin+5)    // ����: COUNT(var1,var2) ͳ������
#define FunOper_DMA				(FunOper_Refer_Begin+6)    // ����:  DMA(var1,var2) ��̬�ƶ�ƽ��
#define FunOper_EMA				(FunOper_Refer_Begin+7)    // ����: EMA(var1,var2) ָ��ƽ���ƶ�ƽ��
#define FunOper_HHV				(FunOper_Refer_Begin+8)    // ����: HHV(var1,var2) ���ֵ
#define FunOper_HHVBARS			(FunOper_Refer_Begin+9)    // ����: HHVBARS(var1,var2) ��һ�ߵ㵽��ǰ��������
#define FunOper_LLV				(FunOper_Refer_Begin+10)    // ����:  LLV(var1,var2) ���ֵ
#define FunOper_LLVBARS			(FunOper_Refer_Begin+11)    // ����: LLVBARS(var1,var2) ��һ�͵㵽��ǰ��������
#define FunOper_MA				(FunOper_Refer_Begin+12)    // ����: MA(var1,var2) ���ƶ�ƽ��
#define FunOper_REF				(FunOper_Refer_Begin+13)    // ����: REF(var1,var2) ������������ǰ������
#define FunOper_SMA				(FunOper_Refer_Begin+14)    // ����:  SMA(var1,var2,var3) �ƶ�ƽ��
#define FunOper_SUM				(FunOper_Refer_Begin+15)    // ����: SUM(var1,var2) �ܺ�
#define FunOper_SUMBARS			(FunOper_Refer_Begin+16)    // ����: SUMBARS(var1,var2) ��ǰ�ۼӵ�ָ��ֵ�����ڵ�������

// �߼�����
#define FunOper_Logical_Begin    (FunOper_Refer_End + 1)
#define FunOper_Logical_End      (FunOper_Logical_Begin + 10)

#define FunOper_IF				 (FunOper_Logical_Begin+1)    // ����: IF(var1,var2,var3) ����������ͬ��ֵ
#define FunOper_ISDOWN			 (FunOper_Logical_Begin+2)    // ����: ISDOWN() �������Ƿ�����
#define FunOper_ISEQUAL			 (FunOper_Logical_Begin+3)    // ����:  ISEQUAL() �������Ƿ�ƽ��
#define FunOper_ISUP			 (FunOper_Logical_Begin+4)    // ����: ISUP() �������Ƿ�����

//******************************������litao����*****************************************
#define FunOper_WHILE			 (FunOper_Logical_Begin+5)    // ����: ѭ�����
#define FunOper_FILTER			 (FunOper_Logical_Begin+6)    // ����: �������
//******************************������litao����*****************************************

// ��������
#define FunOper_Arithmetic_Begin    (FunOper_Logical_End + 1)
#define FunOper_Arithmetic_End      (FunOper_Arithmetic_Begin + 15)

#define FunOper_ABS					(FunOper_Arithmetic_Begin+1)    // ����: ABS(var1) �����ֵ
#define FunOper_CROSS				(FunOper_Arithmetic_Begin+2)    // ����: CROSS(var1,var2) �����߽���
#define FunOper_MAX					(FunOper_Arithmetic_Begin+3)    // ����:  MAX(var1,var2) �����ֵ
#define FunOper_MIN					(FunOper_Arithmetic_Begin+4)    // ����: MIN(var1,var2) ����Сֵ
#define FunOper_MOD					(FunOper_Arithmetic_Begin+5)    // ����: MOD(var1,var2) ��ģ����
#define FunOper_NOT					(FunOper_Arithmetic_Begin+6)    // ����: NOT(var1) ���߼���
#define FunOper_REVERSE				(FunOper_Arithmetic_Begin+7)    // ����:  REVERSE(var1) ���෴��
#define FunOper_SGN					(FunOper_Arithmetic_Begin+8)    // ����: SGN(var1) �����ֵ
//*************************************************************************************************
#define FunOper_CEILING				(FunOper_Arithmetic_Begin+9)    // ����: CEILING(var1) ����ȡ��
#define FunOper_FLOOR				(FunOper_Arithmetic_Begin+10)   // ����: FLOOR(var1) ����ȡ��
#define FunOper_ROUND				(FunOper_Arithmetic_Begin+11)   // ����: ROUND(var1) ��������ȡ��
//*************************************************************************************************


// ��ѧ����
#define FunOper_Maths_Begin			(FunOper_Arithmetic_End + 1)
#define FunOper_Maths_End			(FunOper_Maths_Begin + 15)

#define FunOper_ACOS				(FunOper_Maths_Begin+1)    // ����: ACOS(var1) ������ֵ
#define FunOper_ASIN				(FunOper_Maths_Begin+2)    // ����: ASIN(var1) ������ֵ
#define FunOper_ATAN				(FunOper_Maths_Begin+3)    // ����:  ATAN(var1) ������ֵ
#define FunOper_COS					(FunOper_Maths_Begin+4)    // ����: COS(var1) ����ֵ
#define FunOper_EXP					(FunOper_Maths_Begin+5)    // ����: EXP(var1) ָ��
#define FunOper_LN					(FunOper_Maths_Begin+6)    // ����: LN(var1) ����Ȼ����
#define FunOper_LOG					(FunOper_Maths_Begin+7)    // ����:  LOG(var1) ��10Ϊ�׵Ķ���
#define FunOper_POW					(FunOper_Maths_Begin+8)    // ����: POW(var1,var2) ����
#define FunOper_SIN					(FunOper_Maths_Begin+9)    // ����: SIN(var1) ����ֵ
#define FunOper_SQRT				(FunOper_Maths_Begin+10)    // ����: SQRT(var1) ��ƽ��
#define FunOper_TAN					(FunOper_Maths_Begin+11)    // ����:  TAN(var1) ����ֵ

// ͳ�ƺ���
#define FunOper_Statistic_Begin     (FunOper_Maths_End + 1)
#define FunOper_Statistic_End       (FunOper_Statistic_Begin + 10)

#define FunOper_AVEDEV				(FunOper_Statistic_Begin+1)    // ����: AVEDEV(var1,var2) ƽ������ƫ��
#define FunOper_DEVSQ				(FunOper_Statistic_Begin+2)    // ����: DEVSQ(var1,var2) ����ƫ��ƽ����
#define FunOper_FORCAST				(FunOper_Statistic_Begin+3)    // ����: FORCAST(var1,var2) ���Իع�Ԥ��ֵ
#define FunOper_SLOPE				(FunOper_Statistic_Begin+4)    // ����:  SLOPE(var1,var2) ���Իع�б��
#define FunOper_STD					(FunOper_Statistic_Begin+5)    // ����: STD(var1,var2) �����׼��
#define FunOper_STDP				(FunOper_Statistic_Begin+6)    // ����: STDP(var1,var2) �����׼��
#define FunOper_VAR					(FunOper_Statistic_Begin+7)    // ����: VAR(var1,var2) ������������
#define FunOper_VARP				(FunOper_Statistic_Begin+8)    // ����:  VARP(var1,var2) ������������

// ָ������
#define FunOper_Index_Begin			(FunOper_Statistic_End + 1)
#define FunOper_Index_End			(FunOper_Index_Begin + 12)

#define FunOper_COST		(FunOper_Index_Begin+1)    // ����: COST(var1) �ɱ��ֲ����
#define FunOper_PEAK		(FunOper_Index_Begin+2)    // ����: PEAK(var1,var2,var3) ǰM��ZIGת�򲨷�ֵ
#define FunOper_PEAKBARS	(FunOper_Index_Begin+3)    // ����: PEAKBARS(var1,var2,var3) ǰM��ZIGת�򲨷嵽��ǰ����
#define FunOper_SAR			(FunOper_Index_Begin+4)    // ����:  SAR(var1,var2,var3) ����ת��
#define FunOper_SARTURN		(FunOper_Index_Begin+5)    // ����: SARTURN(var1,var2,var3) ����ת���
#define FunOper_TROUGH		(FunOper_Index_Begin+6)    // ����: TROUGH(var1,var2,var3) ǰM��ZIGת�򲨹�ֵ
#define FunOper_TROUGHBARS	(FunOper_Index_Begin+7)    // ����: TROUGHBARS(var1,var2,var3) ǰM��ZIGת�򲨹ȵ���ǰ����
#define FunOper_WINNER		(FunOper_Index_Begin+8)    // ����:  WINNER(var1) �����̱���
#define FunOper_ZIG			(FunOper_Index_Begin+9)    // ����:  ZIG(var1,var2) ֮��ת��
#define FunOper_LWINNER		(FunOper_Index_Begin+10)    // ����:  LWINNER(var1,var2) ���ڻ����̱���
#define FunOper_PWINNER		(FunOper_Index_Begin+11)    // ����:  PWINNER(var1,var2) Զ�ڻ����̱���

// ���̺���
#define FunOper_BigIndex_Begin    (FunOper_Index_End + 1)
#define FunOper_BigIndex_End      (FunOper_BigIndex_Begin + 10)

#define FunOper_INDEXA			(FunOper_BigIndex_Begin+1)    // ����: INDEXA() ��Ӧ���̳ɽ���
#define FunOper_INDEXADV		(FunOper_BigIndex_Begin+2)    // ����: INDEXADV() ��Ӧ�������Ǽ���
#define FunOper_INDEXC			(FunOper_BigIndex_Begin+3)    // ����:  INDEXC() ��Ӧ�������̼�
#define FunOper_INDEXDEC		(FunOper_BigIndex_Begin+4)    // ����:  INDEXDEC() ��Ӧ�����µ�����
#define FunOper_INDEXH			(FunOper_BigIndex_Begin+5)    // ����: INDEXH() ��Ӧ������߼�
#define FunOper_INDEXL			(FunOper_BigIndex_Begin+6)    // ����: INDEXL() ��Ӧ������ͼ�
#define FunOper_INDEXO			(FunOper_BigIndex_Begin+7)    // ����:  INDEXO() ��Ӧ���̿��̼�
#define FunOper_INDEXV			(FunOper_BigIndex_Begin+8)    // ����:  INDEXV() ��Ӧ���̳ɽ���

// ���ú���
#define FunOper_InCommonUse_Begin    (FunOper_BigIndex_End + 1)
#define FunOper_InCommonUse_End      (FunOper_InCommonUse_Begin + 30)

#define FunOper_CAPITAL		(FunOper_InCommonUse_Begin+1)    // ����: CAPITAL() ��ͨ�̴�С
#define FunOper_VOLUNIT		(FunOper_InCommonUse_Begin+2)    // ����: VOLUNIT() ÿ�ֹ���

#define FunOper_DYNAINFO	(FunOper_InCommonUse_Begin+3) // ��̬���麯��

#define FunOper_FINANCE		(FunOper_InCommonUse_Begin+4)  // ����ָ��

// רҵ��������
#define FunOper_PROFFIN		(FunOper_InCommonUse_Begin+5)  // רҵ��������
#define FunOper_PROFSTR		(FunOper_InCommonUse_Begin+6)  // רҵ�����ַ�������
#define FunOper_PROFFINLAST	(FunOper_InCommonUse_Begin+7)  // רҵ�������ݽ�ֹ��λ��
#define FunOper_PROFFINON	(FunOper_InCommonUse_Begin+8)  // ָ�����ڵ�רҵ��������
#define FunOper_PROFSTRON	(FunOper_InCommonUse_Begin+9)  // ָ�����ڵ�רҵ�ַ�����������
#define FunOper_PROFFINTERM	(FunOper_InCommonUse_Begin+10) // רҵ�������ݱ�����
#define FunOper_SETPROFFIN	(FunOper_InCommonUse_Begin+11) // ����רҵ��������

#define FunOper_FOR			(FunOper_InCommonUse_Begin+12) // for ѭ��
#define FunOper_CONTINUE	(FunOper_InCommonUse_Begin+13) // continue
#define FunOper_BREAK		(FunOper_InCommonUse_Begin+14) // break
#define FunOper_OPERATOR	(FunOper_InCommonUse_Begin+15) // operator
#define FUNOPER_STROUT		(FunOper_InCommonUse_Begin+16) // strout
#define FUNOPER_DATETOSTR   (FunOper_InCommonUse_Begin+17) // DATETOSTR // ����ת�����ַ���
#define FUNOPER_BLOCKCALC   (FunOper_InCommonUse_Begin+18) // ������

// ��������
#define FunOper_Constant_Begin    (FunOper_InCommonUse_End + 1)
#define FunOper_Constant_End      (FunOper_Constant_Begin + 35)

		// �������ͳ���
#define FunOper_STICK			(FunOper_Constant_Begin+1)  // ��״�ߣ���0��ָ��ֵ�仭��ֱֱ��
#define FunOper_COLORSTICK		(FunOper_Constant_Begin+2)  // ��ɫ��״�ߣ���ֵΪ0����ʾ��ɫ��������ʾ��ɫ
#define FunOper_VOLSTICK		(FunOper_Constant_Begin+3)  // �ɽ�����״�ߣ����ɼ�����ʱ��ʾ��ɫ��������������ʾ��ɫ������
#define FunOper_LINESTICK		(FunOper_Constant_Begin+4)  // ͬʱ������״�ߺ�ָ����
#define FunOper_CROSSDOT		(FunOper_Constant_Begin+5)  // С����
#define FunOper_CIRCLEDOT		(FunOper_Constant_Begin+6)  // СԲȦ��
#define FunOper_POINTDOT		(FunOper_Constant_Begin+7)  // СԲ����
#define FunOper_DRAW3D			(FunOper_Constant_Begin+8)  // 3d
#define FunOper_LINEDRAW3D		(FunOper_Constant_Begin+9)  // 3d����
#define FunOper_NOTDRAWLINE		(FunOper_Constant_Begin+10) // ��ǰ���߲�����
#define FunOper_NOTDRAWTEXT		(FunOper_Constant_Begin+11) // ��ǰ�ı�������
#define FunOper_NOTDRAWLINETEXT	(FunOper_Constant_Begin+12) // ��ǰ�����ı���������
#define FunOper_NOTDRAW			(FunOper_Constant_Begin+13) // ������ͼ��
#define FunOper_NOTTABLE		(FunOper_Constant_Begin+14) // ���ڱ������ʾ
#define FunOper_NOTMAXMIN		(FunOper_Constant_Begin+15) // �����������Сֵ,�ڻ�ͼʱʹ��

	// ���K����ɫ
#define FunOper_COLORRED	    (FunOper_Constant_Begin+16)  // ��ɫ
#define FunOper_COLORGREEN		(FunOper_Constant_Begin+17)  // ��ɫ
#define FunOper_COLORBLUE		(FunOper_Constant_Begin+18)  // ��ɫ
#define FunOper_COLORBLACK		(FunOper_Constant_Begin+19)  // ��ɫ
#define FunOper_COLORWHITE		(FunOper_Constant_Begin+20)  // ��ɫ
#define FunOper_COLORGRAY		(FunOper_Constant_Begin+21)  // ��ɫ
#define FunOper_COLORYELLOW		(FunOper_Constant_Begin+22)  // ��ɫ
#define FunOper_COLORCYAN		(FunOper_Constant_Begin+23)  // ��ɫ
#define FunOper_COLORMAGENTA	(FunOper_Constant_Begin+24)  // Ʒ��ɫ
#define FunOper_COLORBROWN		(FunOper_Constant_Begin+25)  // ��ɫ
#define FunOper_COLORBRGB		(FunOper_Constant_Begin+26)  // RGB

	// �߷����
#define FunOper_SOLID            (FunOper_Constant_Begin+27)
#define FunOper_DASH             (FunOper_Constant_Begin+28)       /* -------  */
#define FunOper_DOT              (FunOper_Constant_Begin+29)       /* .......  */
#define FunOper_DASHDOT          (FunOper_Constant_Begin+30)       /* _._._._  */
#define FunOper_DASHDOTDOT       (FunOper_Constant_Begin+31)       /* _.._.._  */

#define FunOper_DRAWNULL		 (FunOper_Constant_Begin+32) // ���ز���������
//--2010.10.8 litao #define FunOper_RBTREND		     (FunOper_Constant_Begin+33) // ���ز���������



#define FunOper_Draw_Begin       (FunOper_Constant_End + 1)
#define FunOper_Draw_End	     (FunOper_Draw_Begin + 30)

// �û���ͼ����
#define FunOper_DRAWICON         (FunOper_Draw_Begin+1)
#define FunOper_DRAWLINE         (FunOper_Draw_Begin+2)
#define FunOper_DRAWTEXT         (FunOper_Draw_Begin+3)
#define FunOper_POLYLINE         (FunOper_Draw_Begin+4)
#define FunOper_STICKLINE        (FunOper_Draw_Begin+5)

// ��ǰ����״̬
#define hx_EXCUTE_PROMPT_ERROR   0x00000001 // ִ��ʱ������д�������ӵ������б�(ȱʡ�����)
#define hx_GETLINE_DRAWEXPRESS   0x00000002 // �õ�һ�л�ͼ���ʽ
#define hx_GetCurrentPosData     0x00000004 // �õ���ǰλ������
#define hx_StrOutData			 0x00000008 // ��ǰ������ַ�������

#define hx_CURRENTDATENOTDELETE  0x00010000 // ��ǰ���ݲ���ɾ��

#define HX_USERDRAWFUN_EXPRESS	 0x00000100  // ʹ�û�ͼ����,�μ�CGraphics::USERDRAWLINE (Graphics.h)

#define HX_USER_COMPILE_INFO	WM_USER + 101 // ����ʱ���͵���Ϣ
#define HX_USER_INITFUN			WM_USER + 102 // ��ʼ��������Ϣ
#define HX_USER_SENDADDSTR		WM_USER + 103 // ��༭��ʽview��������ַ���
#define HX_USER_LOADCUR 		WM_USER + 104 // װ�뵱ǰ��ʽ��ȱʡ��
#define HX_USER_DELETECUR 		WM_USER + 105 // ɾ����ǰ��ʽ��ȱʡ��
// ��������
#define HX_PERIOD_ONEDEAL         0x0001 // �ֱʳɽ�
#define HX_PERIOD_ONEMINUTE       0x0002 // 1������
#define HX_PERIOD_FIVEMINUTE      0x0004 // 5������
#define HX_PERIOD_FIFTEENMINUTE   0x0008 // 15������
#define HX_PERIOD_THIRTYMINUTE    0x0010 // 30������
#define HX_PERIOD_SIXTYMINUTE     0x0020 // 60������
#define HX_PERIOD_PERIOD_TYPE_DAY             0x0040 // ����
#define HX_PERIOD_WEEK            0x0080 // ����
#define HX_PERIOD_MONTH           0x0100 // ����
#define HX_PERIOD_MOREDAY         0x0200 // ������

// Ĭ�������ַ���
#define STRING_PERIOD_ONEDEAL          _T("�ֱʳɽ�")
#define STRING_PERIOD_ONEMINUTE        _T("1������" )
#define STRING_PERIOD_FIVEMINUTE       _T("5������" )
#define STRING_PERIOD_FIFTEENMINUTE    _T("15������")
#define STRING_PERIOD_THIRTYMINUTE     _T("30������")
#define STRING_PERIOD_SIXTYMINUTE      _T("60������")
#define STRING_PERIOD_PERIOD_TYPE_DAY  _T("����"    )
#define STRING_PERIOD_WEEK             _T("����"    )
#define STRING_PERIOD_MONTH            _T("����"    )
#define STRING_PERIOD_MOREDAY          _T("������"  )

//end 

#ifdef VC_7_0

template<class BASE_CLASS, class KEY, class VALUE>

#else

#define BASE_CLASS CMapStringToOb
template<class _CLASS,class KEY, class VALUE>

#endif

class CHSTypedPtrMap : public BASE_CLASS
{
protected:
	CArray<class CValue*,class CValue*>* m_ayObj;
public:
// Construction
	CHSTypedPtrMap(int nBlockSize = 10)
		: BASE_CLASS(nBlockSize) 
	{
		m_ayObj = NULL;
	}
	
	~CHSTypedPtrMap()
	{
		if(m_ayObj != NULL)
		{
			delete m_ayObj;
			m_ayObj = NULL;
		}
	}

	void RemoveAll()
	{
		BASE_CLASS::RemoveAll();
		
		if(m_ayObj != NULL)
		{
			m_ayObj->RemoveAll();
			delete m_ayObj;
			m_ayObj = NULL;
		}
	}

	// Lookup
	BOOL Lookup(BASE_CLASS::BASE_ARG_KEY key, VALUE& rValue) const
		{ return BASE_CLASS::Lookup(key, (BASE_CLASS::BASE_VALUE&)rValue); }

	// Lookup and add if not there
	VALUE& operator[](BASE_CLASS::BASE_ARG_KEY key)
		{ return (VALUE&)BASE_CLASS::operator[](key); }

	void DeleteAt(CValue* pValoare)
	{
		if(m_ayObj == NULL)
			return;
		for(int i = 0; i < m_ayObj->GetSize();i++)
		{
			if(m_ayObj->GetAt(i) == pValoare)
			{
				m_ayObj->RemoveAt(i);
				i--; //20090122 YJT
			}
		}
	}

	// add a new key (key, value) pair
	void SetAt(KEY key, VALUE newValue)
	{
		CValue* valoare;
		if ( Lookup(key,valoare) )
		{
			DeleteAt(valoare);

			delete valoare;
			valoare = NULL;
		}

		BASE_CLASS::SetAt(key, newValue);

		if(m_ayObj == NULL)
		{
			m_ayObj = new CArray<class CValue*,class CValue*>;
		}
		m_ayObj->Add((CValue*)newValue);
	}

	void InsertAt(KEY key, VALUE newValue,int nIndex)
	{
		CValue* valoare;
		if ( Lookup(key,valoare) )
		{
			DeleteAt(valoare);

			delete valoare;
			valoare = NULL;
		}

		BASE_CLASS::SetAt(key, newValue);

		if(m_ayObj == NULL)
		{
			m_ayObj = new CArray<class CValue*,class CValue*>;
		}
		m_ayObj->InsertAt(nIndex,(CValue*)newValue);
	}

	void Add(KEY key, VALUE newValue)
	{
		CValue* valoare;
		if ( Lookup(key,valoare) )
		{
			DeleteAt(valoare);
			RemoveKey(key);
		}
		BASE_CLASS::SetAt(key, newValue);

		if(m_ayObj == NULL)
		{
			m_ayObj = new CArray<class CValue*,class CValue*>;
		}
		m_ayObj->Add((CValue*)newValue);
	}

	// removing existing (key, ?) pair
	BOOL RemoveKey(KEY key)
	{
		return BASE_CLASS::RemoveKey(key); 
	}

	// iteration
	void GetNextAssoc(POSITION& rPosition, KEY& rKey, VALUE& rValue) const
	{ 
		BASE_CLASS::GetNextAssoc(rPosition, (BASE_CLASS::BASE_KEY&)rKey,
			(BASE_CLASS::BASE_VALUE&)rValue); 
	}

public:
	CString GetObjectName(CObject* pObject)
	{
		POSITION pos = GetStartPosition();
		CString sName;
		CValue *pVal;
		while (pos != NULL)
		{	
			GetNextAssoc(pos, sName, pVal);
			if(pVal != NULL && pVal == pObject)
			{
				return sName;
			}
		}
		return "";
	}

	CArray<class CValue*,class CValue*>* GetObjArray()
	{
		return m_ayObj; 
	}

	CValue* GetAt(int nPos)
	{
		if(m_ayObj == NULL)
			return NULL;
		if( nPos >= 0 && nPos < m_ayObj->GetSize() )
		{
			return m_ayObj->GetAt(nPos);
		}
		return NULL;
	}

	short GetDrawCount(short& nDrawCount)
	{		
		nDrawCount = 0;
		if(m_ayObj == NULL)
			return 0;

		short nNotDraw = m_ayObj->GetSize();
		for(short i = 0; i < nNotDraw;i++)
		{
			if( m_ayObj->GetAt(i)->IsStyle(HX_EXPRESSDRAW) )
			{
				nDrawCount++;
			}
		}
		return (nNotDraw - nDrawCount);
	}
};
typedef CHSTypedPtrMap<CMapStringToOb,CString,class CValue*> CHSMapVariabile;

 // ִ��ʱ��ʶ�����к�

struct ExecObjNoList
{
	CString strName;
	int iObjNo;
	CCriticalSection *pcsObjNo;				//�ٽ�������
	ExecObjNoList(){pcsObjNo = new CCriticalSection(); }
	~ExecObjNoList(){ delete pcsObjNo;}
};
typedef struct ExecObjNoList  EXECOBJNO;
typedef struct ExecObjNoList* LPEXECOBJNO;

struct HSExpressAutoSetup
{
	CString  m_strFileName; // �ļ�����
	DWORD	 m_dwStye;      // �������ͣ����� EXTERN_EXPRESS_*
	void*	 m_pData;       // ����
	HSExpressAutoSetup()
	{
		m_dwStye = 0;
		m_pData  = NULL;
	}
};

/*************************************************************
*	�� �� �ƣ�	CMapVariabile
*	�� �� ����	��(StringToOb)��ͬʱ����������
***************************************************************/
class CExpression;
class BCGCONTROLBARDLLEXPORT CMapVariabile : public CHSMapVariabile
{
public:
	CMapVariabile();
	~CMapVariabile();
public:
	// Lookup

	/******************************************************************
	*	�������ܣ�	����Key�õ���Ӧ��rValueֵ
	*	����������	CString key    : ��Ա��еļ�ֵ
	*				CValue*& rValue: ���ص�����
	*	�� �� ֵ��	BOOL : Ϊ�����ʾ�ҵ�Key����Ӧֵ,����û�ҵ�
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��10��
	*	�޸�˵����	
	*****************************************************************/
	BOOL Lookup(CString key, CValue*& rValue) const;

	/******************************************************************
	*	�������ܣ�	����Key��cType�õ���Ӧ��rValueֵ
	*	����������	CString key : ��Ա��еļ�ֵ
	*				char cType  : ��ʽ����, Tech, Exchange...
	*				CValue*& rValue : ���ص�����
	*	�� �� ֵ��	BOOL  : Ϊ�����ʾ�ҵ�Key����Ӧֵ,����û�ҵ�
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��10��
	*	�޸�˵����	
	*****************************************************************/
	BOOL Lookup(CString key,char cType, CValue*& rValue) const;

	/******************************************************************
	*	�������ܣ�	��ǰ�ļ�ֵ�Ƿ����
	*	����������	CString key : ��Ա��еļ�ֵ
	*				CValue*& rValue : ���ص�����
	*	�� �� ֵ��	BOOL  : Ϊ�����ʾ�ҵ�Key����Ӧֵ,���򲻴���
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��10��
	*	�޸�˵����	
	*****************************************************************/
	BOOL IsExist(CString key, CValue*& rValue) const;

	/******************************************************************
	*	�������ܣ�	����м�һ����ʽ
	*	����������	CString key     : ��ʽ��
	*				CValue* rValue  : ��ʽ��
	*	�� �� ֵ��	CValue*			: ���ɹ��򷵻�NULL, ���򷵻�rValue
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��10��
	*	�޸�˵����	
	*****************************************************************/
	CValue* AddExpress(CString key,CValue* rValue);

	/******************************************************************
	*	�������ܣ�	����key+cType��ɼ�ֵ,����ӱ���ɾ��
	*	����������	CString key : ���ĺ󲿷ּ�ֵ 
	*				char cType  : ���ݹ�ʽ���;������ĵ�һλ����
	*	�� �� ֵ��	BOOL        : TRUE�Ǳ�ʾɾ���ɹ�,����ʧ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��10��
	*	�޸�˵����	
	*****************************************************************/
	BOOL    RemoveKey(CString key,char cType);

	/******************************************************************
	*	�������ܣ�	ͬ��,ֻ�ǹ�ʽ���ʹ�pExp����õ��������
	*	����������	CString key
	*				CExpression* pExp : ��ʽ��,����pExp->GetExpressType()�õ���ʽ����
	*	�� �� ֵ��	BOOL  
	*	����������	RemoveKey(CString key,char cType);
	*	��    �ߣ�	
	*	������ڣ�	2003��4��10��
	*	�޸�˵����	
	*****************************************************************/
	BOOL    RemoveKey(CString key,CExpression* pExp);

	/******************************************************************
	*	�������ܣ�	���ؼ�ֵΪKey������,û�����һ��
	*	����������	CString key : ��ֵ
	*	�� �� ֵ��	CValue*&    : ���ص�����
	*	����������	����CHSMapVariabile::operator[]
	*	��    �ߣ�	
	*	������ڣ�	2003��4��10��
	*	�޸�˵����	
	*****************************************************************/
	CValue*& operator[](CString key);
};
/******************************************************************
*	�������ܣ�	���һ���������
*	����������	struct nodErrorInfo* pError : ������Ϣ
*	�� �� ֵ��	void 
*	����������	
*	��    �ߣ�	
*	������ڣ�	2003��4��10��
*	�޸�˵����	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern void AddError(struct nodErrorInfo* pError);  // 

/******************************************************************
*	�������ܣ�	ɾ�����б������
*	����������	���ޣ�
*	�� �� ֵ��	void 
*	����������	
*	��    �ߣ�	
*	������ڣ�	2003��4��10��
*	�޸�˵����	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern void DeleteError();                          // 

/******************************************************************
*	�������ܣ�	�õ������������
*	����������	���ޣ�
*	�� �� ֵ��	int  
*	����������	
*	��    �ߣ�	
*	������ڣ�	2003��4��10��
*	�޸�˵����	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern int  GetErrorCount();                        // 

/******************************************************************
*	�������ܣ�	�ɴ���ŵõ�������Ϣ
*	����������	UINT nID
*	�� �� ֵ��	CString  
*	����������	
*	��    �ߣ�	
*	������ڣ�	2003��4��10��
*	�޸�˵����	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern CString  hxGetErrorInfoByErrorNum(UINT nID); // 

/******************************************************************
*	�������ܣ�	�滻�س����з�
*	����������	CString strExp : Դ�ִ�
*				CString seps   : Ҫ�滻�������ַ���
*				CString* pstrCur = NULL : Ҫ�����seps[i]�滻�����ִ�
*	�� �� ֵ��	CString  : �滻��Ľ��, ��strExp = "a,b.c" seps=",." pstrCur = "A", ����Ϊ"aAbAc"
*	����������	
*	��    �ߣ�	
*	������ڣ�	2003��4��10��
*	�޸�˵����	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern CString  hxRelaceReturnKey(CString strExp,CString seps,CString* pstrCur = NULL);  // 

/******************************************************************
*	�������ܣ�	ɾ���б�
*	����������	CMapVariabile* pVarsMap : Ҫɾ�����б�
*				BOOL bSelf              : �Ƿ�ɾ���Լ�,���Ϊ����Ҫ����delete  pVarsMap;
*	�� �� ֵ��	CMapVariabile*          : pVarsMap
*	����������	
*	��    �ߣ�	
*	������ڣ�	2003��4��10��
*	�޸�˵����	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern  CMapVariabile* DeleteVarsMap(CMapVariabile* pVarsMap,BOOL bSelf); // 

/******************************************************************
*	�������ܣ�	�ڱ�pVarsMap�еõ�һ��Ψһ������
*	����������	CString strOldName : ���Ƶĳ�ʼֵ,�õ��Ľ��Ϊ����Ϊǰ׺,�����һ������ strOldName = "AVC",��������Ϊ "AVC120"
*				CMapVariabile* pVarsMap : Ŀ���
*	�� �� ֵ��	CString	: �õ���Ψһ����
*	����������	
*	��    �ߣ�	
*	������ڣ�	
*	�޸�˵����	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern  CString		  GetIdentifierExpressName(CString strWholeName,CString strOldName,CMapVariabile* pVarsMap); // 

// Ŀǰ����
BCGCONTROLBARDLLEXPORT extern int FindInMap(const CString& strKey,
											CMapVariabile* pVarsMap,
											DWORD dwFindMask = 0,
											CArray<CValue*,CValue*>* pArray = NULL,
											class CListCtrl* pCtrl = NULL);

/******************************************************************
*	�������ܣ�	��ʼ���Ѷ���ĺ����������pExternFunction��
*	����������	CMapVariabile*& pExternFunction : ����ʼ���ú��ϵͳ��������˱���
*	�� �� ֵ��	void  
*	����������	
*	��    �ߣ�	
*	������ڣ�	
*	�޸�˵����	
*****************************************************************/
BCGCONTROLBARDLLEXPORT extern  void   InitSysFunction(CMapVariabile*& pExternFunction);
// �洢�ѱ���Ĺ�ʽ��Ϣ
typedef BOOL ( *HSFunAddDefaultTechAnysis)(void* pExpress,BOOL bFind,void* pData,char cOperater);
typedef BOOL ( *HSFunAddToTree)(void* pExpress,void* pData,char cOperater);												 
BCGCONTROLBARDLLEXPORT extern  BOOL   SaveLoadMap(BOOL bSave,CMapVariabile*& pExternExpression,
												  CMapVariabile* pExternVariabile,CMapVariabile*  pExternFunction,
												  CComboBox* pBox = NULL,char* strFileName = NULL,
												  HSFunAddDefaultTechAnysis pFun = NULL,
												  HSFunAddToTree pFunAyTree = NULL);

BCGCONTROLBARDLLEXPORT extern void    SetDefaultValues(HSDouble* pData,long lSize,HSDouble values);
BCGCONTROLBARDLLEXPORT extern void    SetDefaultValues(long* pData,long lSize);
BCGCONTROLBARDLLEXPORT extern CString ReplaceNumber(CString& strSource);

// �ֽ��ִ�
BCGCONTROLBARDLLEXPORT extern int	  AnalyzeStr(CString strResource,const char* strDelimit,CStringArray& ayRet);
// �Ƿ�������һ���ֺ�
BCGCONTROLBARDLLEXPORT extern void    RelaceLastChar(CString& strText);
// ʹ�ı����ǰ������
BCGCONTROLBARDLLEXPORT extern CString AdjustStr(CString pOrg,int& nReqLen,CString* pstrLeft = NULL);
// �Ƿ��Ǻ���
BCGCONTROLBARDLLEXPORT extern int     IsChinese(BYTE cChar,const CString& strSource,int& nPos);
BCGCONTROLBARDLLEXPORT extern int     IsChinese(BYTE cChar,const CString& strSource,int& nPos,BOOL bChange);

// ���ݼ�����ָ��ȱʡ��ɫ
class CHSDataSource;
BCGCONTROLBARDLLEXPORT extern void    SetHxDataSource( CHSDataSource* pData,CArray<struct ColorProperty*,struct ColorProperty*>* pDefColours);
BCGCONTROLBARDLLEXPORT extern CString hxSubZero(CString str);

//
// ��������
enum  FunctionType
{	
	FunctionNone,
	FunctionQuotes,		 // ����
	FunctionDateTime,	 // ʱ��
	FunctionReference,   // ����
	FunctionLogical,	 // �߼�
	FunctionArithmetic,	 // ����
	FunctionMath,		 // ��ѧ
	FunctionStat,		 // ͳ��
	FunctionTarget,		 // ָ��
	FunctionBig,		 // ����
	FunctionInCommonUse, // ����
	FunctionDraw,	     // ��ͼ

	FunctionUserDefine,  // �Զ���
	FunctionConstant     // �˺����ǳ���
};

#define DataTypeOperStr		  0x01  // �� nodElement �� valoarestr��Ա ��Ч��
#define DataTypeOperCompile   0x02  // ���뷽ʽ
#define DataFunParam		  0x04  // ��������
#define DataExpParam		  0x08  // ���ò���
#define DataInport  		  0x10  // ��������
#define DataText  		      0x20  // �ı�����


// ������Ϣ
struct nodErrorInfo
{
	int		  m_nError;     // �����
	int		  m_nBegin;     // ����ʼλ��
	int       m_nEnd;       // �������λ��
	CString*  m_pError;     // ������Ϣ
	CString*  m_pHelp;      // ��ʾ������

	nodErrorInfo();
	/*nodErrorInfo(int nError,int nBegin = -1,int nEnd = -1, CString pError = "",CString pHelp = "");*/
	nodErrorInfo(int nError,int nBegin = -1,int nEnd = -1, 
		const char* pError = NULL, const char* pHelp=NULL);

	CString  GetError();     // ������Ϣ
	CString  GetHelp();      // ��ʾ������
	
	~nodErrorInfo();
};

// ��������
struct FormulaGeneralParam;
struct FunctionParam
{
	void*			 pFun;	// ����ָ��
	FormulaGeneralParam* ayVars; // ���������б�ָ��

	FunctionParam();
	~FunctionParam();
	
	virtual void  Serialize(class CExpression* pExpress,struct nodElement* nod,CArchive& ar);
};

// ���ù�ʽ����
struct ExpressParam
{
	void*		   pValue; // ���õĹ�ʽָ�� 
	CString		   strSub; // ���õĹ�ʽ�е������
	void*		   pOther; // һ��Ϊ��,����

	ExpressParam();
	~ExpressParam();
};

// ���ù�ʽ����
struct InportDataParam
{
	CHSDataSource*	m_pDataSrc;	// ���õ�����Դ

	InportDataParam();
	~InportDataParam();
};

typedef struct StockDay* LPSTOCKDAY; // ��������

// ��ͼ��������
struct CFormulaDrawParam
{
	char	  m_strName[256];      // ����
	char      m_strExpParam[256];  // ��������

	long      m_lStyle;		// ���

	long      m_lColor;	   /*����ɫ*/
	int		  m_nPenStyle; /*�߷��*/
	int		  m_nWidth;	   /*�߿��*/
	CFormulaDrawParam();
};

struct BlockCurDispCalc
{
	double m_dValue;		//ͳ�ƽ��
};

struct BlockOtherData
{
	double m_dCapital; // ��ͨ�̴�С
};

// ����ʱ�����ⲿ����
//struct CCalcExternParam
//{
//	long		m_nBeginDate; // ��ʼ����,��19901020
//	long	    m_nEndDate;   
//	CodeInfo    m_sCode;	  // ����
//	char 		m_szBlock[BLOCK_NAME_LENGTH]; // �������
//
//	CCalcExternParam()
//	{
//		EmptyBlock();
//	}
//
//	void    EmptyBlock() { memset(m_szBlock,0,sizeof(m_szBlock)); }
//	CString GetBlockName() { return CString(m_szBlock).Left(BLOCK_NAME_LENGTH); }
//	void    SetBlockName(CString pData)
//	{
//		EmptyBlock();
//		if( pData.IsEmpty() )
//			return;
//
//		strncpy(m_szBlock,pData,sizeof(m_szBlock));
//	}
//};

/*************************************************************
*	�� �� �ƣ�	CLineProperty
*	�� �� ����	��ͼ����(���̺���������õ�)
***************************************************************/
class  CExpValue;
/*************************************************************
*	�� �� �ƣ�	BlockCalcRet
*	�� �� ����	��������
***************************************************************/
struct BlockCalcRet
{
	CArray<double,double> ayValues;		//�ӵ�3�п�ʼ�ļ�����,��ayColName��Ӧ
};

/*************************************************************
*	�� �� �ƣ�	SelStockResult
*	�� �� ����	ѡͶ��������м����
***************************************************************/
struct SelStockResult
{
	HSDouble dData;
	void*	 pStock;
	void*    pAyStock; // 
 };
/*************************************************************
*	�� �� �ƣ�	BlockTypeInfo
*	�� �� ����	���������Ϣ
***************************************************************/
struct BlockTypeInfo
{
	CString		strTypeName;	//������
	// �������еİ����Ϣ
	CArray<BlockInfo*,BlockInfo*>	ayBlock; 
	// ����Ҫ����Ĺ�Ʊ��Ϣ
	CArray<StockUserInfo*,StockUserInfo*> ayCalcStock;		
	// Ҫ���ص���������,��ayCalcStock��Ӧ
	CArray<BlockOtherData*,BlockOtherData*> ayOtherData;		
	//������,��ayCalcStock��Ӧ
	CArray<BlockCalcRet*,BlockCalcRet*> ayCalcRet;			
	// ��ʽ����
	void*	m_pColName;        

	// �����Դ
	~BlockTypeInfo();
	void Free();
};
typedef struct BlockTypeInfo TYPEANDBLOCK;
/*************************************************************
*	�� �� �ƣ�	CDataSourceUseData
*	�� �� ����	������Դ��(CHxDataSoures)��ʹ��,һ�������ʾһ�����͵����ݴ�
***************************************************************/
class CDataSourceUseData
{
public:
	HSDouble*    m_pData;    // ����
	int			 m_nSize;
public:
	// ��ʼ������
	CDataSourceUseData();
	// ��մ���
	~CDataSourceUseData();
	// ����m_pData
	HSDouble* GetData();
	// �ռ��Ƿ�Ϊ��
	BOOL      IsEmpty();
	// �����ÿռ�
	CDataSourceUseData* Alloc(int nCurSize,int nOldSize,long pLastDay = 0);
	// ��տռ�
	void Free();
};
// ����Դ�����
typedef CTypedPtrMap<CMapWordToPtr,WORD,class CDataSourceUseData*> CMapDataSource;

// ����������ID
#define hx_OPEN_DATA   0x01 // ���̼����� 
#define hx_HIGH_DATA   0x02 // ��߼����� 
#define hx_LOW_DATA    0x03 // ��ͼ����� 
#define hx_CLOSE_DATA  0x04 // ���̼����� 
#define hx_VOL_DATA    0x05 // �ɽ������� 
#define hx_AMOUNT_DATA 0x06 // �ɽ������� 
#define hx_DATE_DATA   0x07 // ��������  ��ʽΪ19910101

#ifdef SUPPORT_NETVALUE
#define hx_NetValue_DATA 0x08 // ��ծ���ʣ�����ֵ
#endif

// ��������
#define hx_PERIOD_TYPE_DAY_DATA		  0x00000001 // �������ڣ�>=��
#define hx_MINUTE_DATA	              0x00000002 // �������ڣ�>=����

/*************************************************************
*	�� �� �ƣ�	FormulaCompareSubData
*	�� �� ����	Ҫ�Ƚϵ����ݽṹ
***************************************************************/
struct FormulaCompareSubData
{
	enum 
	{
		DaYu,		//  ����
		XiaoYu,		//	С��
		DengYu,		//	����
		JieYu,		//	����
		ShangChuan, //	�ϴ�
		XiaoPo,		//	����
		WeiChi,		//	ά��
		BiaoChi,	//	����

		Condition, // ����ѡ��
		Exchange,  // ����ϵͳ
		MoreKLine,  // ���K��
		Strategy   // ���Խ���  //20090302 YJT ���� Strategy
	};

	char     m_cType; // ��������
	CString m_dData1; // �������ұ�1
	CString m_dData2; // �������ұ�2
	
	void Copy(FormulaCompareSubData* pData);
	FormulaCompareSubData();
	void Serialize(CArchive& ar);
};


struct StockArea
{
	CString strText; // ����
	DWORD	lData;   // �����

	static CArray<StockUserInfo*,StockUserInfo*> ayStock;
	static void AddStock(DWORD market);
	StockArea();
};

/*************************************************************
*	�� �� �ƣ�	SingleSelCondition
*	�� �� ����	������ѡ��
***************************************************************/
class SingleSelCondition
{
public:
	CString			m_strExp;		// ����/��ʽ����
	int				m_nType;		// ��ʽ����

	short			m_nPeriod;		// ����
    CString			m_strCondition; // ����ָ��ָ:��һ��ָ����;����ϵͳָ:�����ź�;������ָ:����ָ��ID
	FormulaCompareSubData m_SubData;		// Ҫ��m_strConditionָ��������Ƚϵ�ָ�������

	SingleSelCondition();

	void Copy(SingleSelCondition* pData);
	void Serialize(CArchive& ar);

#ifndef _HSDiskDll_
	virtual CString GetDescribe(void* pData = NULL);
	virtual BOOL	Open(void* pData = NULL);
#endif

};
/*************************************************************
*	�� �� �ƣ�	UnionSelConditionSub
*	�� �� ����	�������ѡ�����һ��
***************************************************************/
struct UnionSelConditionSub
{
	CString m_strName;		// ����
	short	m_nBeginDate;	// ��ʼʱ��
	short	m_nEndDate;		// ����ʱ��

	short	m_lDateCount;	// ������
	long	m_nCalcEndDate; // ������ʱ��

	char    m_cAndOr;		// ���������뻹�ǻ�Ĺ�ϵ
    CArray<SingleSelCondition*,SingleSelCondition*>* m_aySub; // ����ѡ������

	void Free();
	void Copy(UnionSelConditionSub* pData);
	UnionSelConditionSub();
	~UnionSelConditionSub();
	void Serialize(CArchive& ar);
};

struct UnionSelCondition;
struct SelStockCondition
{
	WORD				  m_dStyle;					// ���
	CArray<StockArea*,StockArea*>* m_ayCheck; // ��Ʊ��Χ
	CString*			  m_strBlock;               // �����

	SingleSelCondition*		  m_sSingleSel;			    // ����ѡ������
	UnionSelCondition*			  m_pHSUnion;			    // ���ѡ������

	unsigned long				  m_lEndDate;				// ������ֹ����  //--2010.11.12 litao����2012ʱ������
	long				  m_lDateCount;				// �������ڳ���

	void* m_pData;									// ������������(����)

	SelStockCondition();
};

/*************************************************************
*	�� �� �ƣ�	UnionSelCondition
*	�� �� ����	�������ѡ��m_aySub.GetSize()Ŀǰ��Ϊ1
***************************************************************/

struct UnionSelCondition
{
	CString m_strName; // �����������
    CArray<UnionSelConditionSub*,UnionSelConditionSub*>* m_aySub; // ���������

	void Free();
	void Copy(UnionSelCondition* pData);
	UnionSelCondition();
	~UnionSelCondition();
	void Serialize(CArchive& ar);
};


struct SuccResult  // ѡ�ɳɹ���Ϣ
{
	unsigned long	m_lDate;  // ʱ��
	long	m_nPos;   // ��ǰk��λ��

#ifndef HS_SUPPORT_UNIX
	static CString m_strTip; // ѡ�ɳɹ���Ϣ��ʾ

	SuccResult(unsigned long lDate,long nPos)
	{
		m_lDate = lDate; // ʱ��
		m_nPos  = nPos;
	}
#endif

};
		

struct HSGridBlockData
{
	CString		m_strTypeName;						// ������
	CUIntArray  *m_payIdx;							// �����ͺ�����Щ��飨Ϊm_payAllBlock�е������������飩

	static CArray<BlockInfo*,BlockInfo*> *m_payAllBlock; // �ⲿ�ռ䣬���а���б��ڴ˲���ɾ��

	HSGridBlockData()
	{
		m_payIdx = NULL;
		m_payAllBlock = NULL;
	}

	~HSGridBlockData()
	{
		if( m_payIdx != NULL )
		{
			m_payIdx->RemoveAll();
			delete m_payIdx;
			m_payIdx = NULL;
		}
	}
};
struct HSOpenExpressUseData
{
	CString* m_strName; // ���ʽ����
	void*	 m_pData;	// ��ʽָ��
	DWORD	 m_wType;   // ����

	DWORD	m_dStyle;	// ��ʽ���ָ��CExpression:m_dStyle,���գ�HS_EXP_FINANCE �ȶ���
	// ���˹�ʽ�ã���������ݹ�ʽ��ֻҪ=HS_EXP_FINANCE����.

	HWND	m_hParent;

	short    m_ShowShortKey;   //���ʹ�ã�Ϊ���ڱ��۽���ſ�21 23��ݼ�

	HSOpenExpressUseData();
	HSOpenExpressUseData(WORD wType);
	BOOL IsType(WORD wType);
};

//add by lxqi 20090311
//���Խ��׼�����Ϣ
struct StrategyInfo
{
	//����////////////////////////////////////////////////////////////////////////////
	short			m_nID;				// ���ID
	char			m_cUserNo[20];		// �û��ʽ��˺�		

	short			m_cCodeType;		// ֤ȯ���ͣ�����ָ���г����ࣩ
	char			m_cCode[6];			// ֤ȯ����
	char			m_sCodeName[10];	// ֤ȯ����
	char			m_sExpName[32];		// ��ʽ����
	//----------------------------------------------------------
	char			m_cTrigName[32];			//������������
	int				m_lStartTime;		//��Чʱ����ʼ
	int				m_lEndTime;			//��Чʱ��ֹ
	char			m_cTradeName[32];	// ���ײ�������
	//==========================================================
	short			m_nPeriodType;		// ָ������ �ο� BaseDefine.h�е�PERIOD_TYPE_DAY��
	short			m_nPeriodNumber;	// ��������ʱ��ָ������
	short			m_nStatus;			// ���״̬��0ֹͣ��1������

	short			m_nBuyTimes;		// ����������ﵽ���ƴ������ٷ�����������
	short			m_nMaxBuyTimes;		// ����������ƣ���������ﵽ�����ƴ����������롣-1��ʾ������
	short			m_nSellTimes;		// �����������ﵽ�������ٷ�����������-1��ʾ������
	short			m_nMaxSellTimes;	// �����������ƣ����������ﵽ�����ƴ�������������-1��ʾ������
	short			m_nTrigTimes;		// ����������������������m_nMaxTrigTimes�κ󣬽��н���
	short			m_nMaxTrigTimes;	// �����������ޣ��ﵽ�������򴥷�����
	short			m_nDuration;		// ��������ʱ�䣨�룩��������m_nDuration������Ȼ����������н���
	short			m_nGap;				//����ί�м����һ�δ�������֮�󣬼�ֹͣ�Ըù�Ʊ�ļ�أ�ֱ��������ί�м��(����30��)��֮���ٿ�ʼ���¼�ظù�Ʊ��
										//������ί�м����Ĭ��30�롣�������������Ϊ-1����ʾ����ֻ����һ�ν��ֲ���������һ���Զ����ֽ��׺�ֹͣ�Զ����֡�
	long			m_nLastTrigTime;	//�ϴδ�����ʱ��
	short			m_bFuQuan;			//�Ƿ�Ȩ
	short			m_bXD;			//�µ�ʱ�Ƿ���ʾ
	//////////////////////////////////////////////////////////////////////////
	//�������ݡ���ʽ
	short			m_nSize;			// ��ʽ�����ݸ���
	CLineProperty*	m_pData;			// ���������ݣ�����ԭʼ���ݡ���ؽ���ͼ��㹫ʽ��
	StrategyInfo()
	{
		m_nID = 0;			
		m_cCodeType = 0;	

		ZeroMemory(m_cCode,6);
		ZeroMemory(m_sCodeName,10);
		ZeroMemory(m_sExpName,32);	
		ZeroMemory(m_cTradeName,32);
		ZeroMemory(m_cTrigName,32);
		m_nSize = 0;		
		m_nPeriodType = 0;	
		m_nStatus = 0;		
		m_nPeriodNumber = 0;
		
		m_lStartTime = 930;		//��Чʱ����ʼ
		m_lEndTime = 1500;			//��Чʱ��ֹ
		m_nMaxBuyTimes = -1;
		m_nMaxSellTimes = -1;
		m_nBuyTimes = 0;	
		m_nSellTimes = 0;	
		m_nTrigTimes = 0;	
		m_nDuration = 0;	
		m_nGap = 0;
		m_nLastTrigTime = 0;
		m_bFuQuan = 1;
		m_nGap = 30;
		m_pData = NULL;
		m_bXD = 0;
	}
};

//add by lxqi 20090425	��������ģ��ṹ
struct StrategyTrig
{
	char			m_cName[32];			//������������
	//char			m_cExplain[64];		// ������������
	short			m_nReserve;			// ����

	int				m_lStartTime;		//��Чʱ����ʼ
	int				m_lEndTime;			//��Чʱ��ֹ
	short			m_nPeriodType;		// ָ������ �ο� BaseDefine.h�е�PERIOD_TYPE_DAY��
	short			m_nPeriodNumber;	// ��������ʱ��ָ������

	short			m_nMaxBuyTimes;		// ����������ƣ���������ﵽ�����ƴ����������롣-1��ʾ������
	short			m_nMaxSellTimes;	// �����������ƣ����������ﵽ�����ƴ�������������-1��ʾ������
	//short			m_nMaxTrigTimes;	// �����������ޣ��ﵽ�������򴥷�����
	//short			m_nDuration;		// ��������ʱ�䣨�룩��������m_nDuration������Ȼ����������н���
	short			m_nGap;				// ����ί�м����һ�δ�������֮�󣬼�ֹͣ�Ըù�Ʊ�ļ�أ�ֱ��������ί�м��(����30��)��֮���ٿ�ʼ���¼�ظù�Ʊ��
										// ������ί�м����Ĭ��30�롣�������������Ϊ-1����ʾ����ֻ����һ�ν��ֲ���������һ���Զ����ֽ��׺�ֹͣ�Զ����֡�
	short			m_bFuQuan;			// �Ƿ�Ȩ
	CExpValue*		m_pExpValue;		// ��Ź�ʽ��ص�ָ��
	StrategyTrig()
	{
		ZeroMemory(m_cName,32);
//		ZeroMemory(m_cExplain,64);
		m_lStartTime = 930;
		m_lEndTime = 1500;
		m_nPeriodType = 0;
		m_nPeriodNumber = 0;
		m_nMaxBuyTimes = -1;
		m_nMaxSellTimes = -1;
		//m_nMaxTrigTimes = 0;
		//m_nDuration = 0;
		m_nGap = 30;
		m_bFuQuan = 1;
		m_pExpValue = NULL;
	}
};
//end by lxqi 20090425

//add by lxqi 20090311	���Խ��׼����б�����
typedef CTypedPtrMap<CMapWordToPtr,WORD,StrategyInfo*> CMapStrategyInfo;
//end by lxqi
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------

struct StrategyHis//���Խ�����ʷ����
{
	CodeInfo		*m_pCode;			// ��ǰ���ԵĹ�Ʊ��Ϣ
	char			m_sCodeName[16];	// ֤ȯ����
	char			m_Tradecode[6];		// ���״���
	short			m_TradeCodeType;	//���״�������
	//--------------------------------------------------
	char			m_cTrigName[32];		// ������������
	//=====================================================
	char			m_cTradeName[32];	// ���ײ�������
	float			m_nMaxGain;			// ӯ�����ޣ��ٷֱȣ������۸���ڵ�ǰ�ɱ��۴ﵽӯ������ʱ����������ȫ���ֲ֣�-1��ֹӯ
	float			m_nMaxDeficit;		// �������ޣ��ٷֱȣ������۸������ǰ�ɱ��۴ﵽ��������ʱ����������ȫ���ֲ֣�-1��ֹ��
	short			m_nMaxBuyTimes;		// ����������ƣ��ﵽ�����ƺ�ǿ���������У�-1������
	short			m_nMaxSellTimes;	// �����������ƣ����������ﵽ�����ƴ�������������-1��ʾ������
	short			m_nBuyType;			// �������ͣ�0��ȫ���ʽ����룬1�������ʽ�����(������)��2���̶��ʽ����룬3���̶��������루�֣�
	double			m_nBuyNum;			// ��������������m_nBuyType�жϾ������ݺ���
	short			m_nSellType;		// �������ͣ�0��ȫ���ʽ�������1�������ʽ�����(������)��2���̶��ʽ�������3���̶������������֣�
	double			m_nSellNum;			// ��������������m_nSellType�жϾ������ݺ���
	
	short			m_nGap;				// ����ί�м����һ�δ�������֮�󣬼�ֹͣ�Ըù�Ʊ�ļ�أ�ֱ��������ί�м��(����30��)��֮���ٿ�ʼ���¼�ظù�Ʊ��
										// ������ί�м����Ĭ��30�롣�������������Ϊ-1����ʾ����ֻ����һ�ν��ֲ���������һ���Զ����ֽ��׺�ֹͣ�Զ����֡�
	short			m_bFuQuan;			// �Ƿ�Ȩ
	short			m_bOpt;             // �Ƿ��ǵ���
	short			m_bByDay;			// �Ƿ���ÿ�շ���
	double			m_nFees;			// �����ѣ��ٷֱȣ�������ÿ�ν��׵Ľ�����

	double			m_nMoney;			// �ܽ��
	float			m_fCost;			// ���ý��
	long			m_lHolding;			// �ֹ������ɣ�
	
	short   m_nStop;//�Ƿ�ֹ��ֹӯ
	short   m_nDynStop;//�Ƿ�ֹ̬��ֹӯ
	double  m_lSellStopLossRange;//����ֹ�����
	double  m_lSellStopProfitRange;//����ֹӯ����

	/*year-month-day ,example: 19960616
	�������ݵı�ʾ�������£�yymmddhhnn(������ʱ��)
	yyָ����year - 1990������ݱ�ﷶΧ��1990 - 2011
	��0905131045��ָ���ǣ�1999��5��13��10��45�֡�*/
	long  			m_nStartDate;		// ��ʼʱ��
	long	 		m_nEndDate;			// ����ʱ��
	long			m_lEffectStartTime;	// ��Чʱ����ʼ
	long			m_lEffectEndTime;	// ��Чʱ��ֹ
	short			m_nPeriodType;		// ָ������ �ο� BaseDefine.h�е�PERIOD_TYPE_DAY��
	short			m_nPeriodNumber;	// ��������ʱ��ָ������
	char			m_sExpName[32];		// ��ʽ����
	short			m_nSize;			// ��ʽ�����ݸ���
	CLineProperty*	m_pData;			// ���������ݣ�����ԭʼ���ݡ���ؽ���ͼ��㹫ʽ��
	CHSDataSource*  m_pTradeDataSrc;    // ���ڽ��׵�����
	CMap<CString,LPCTSTR,double,double> m_mapVar;//����
	StrategyHis()
	{
		m_pCode = NULL;
		ZeroMemory(m_sCodeName,16);
		ZeroMemory(m_sExpName,32);	
		ZeroMemory(m_cTradeName,32);
		ZeroMemory(m_cTrigName,32);
		ZeroMemory(m_Tradecode,6);
		m_bFuQuan = 1;
		m_fCost = 0;
		m_lHolding = 0;
		m_nMoney = 0;		
		m_nPeriodType = 0;	
		m_nPeriodNumber = 0;
		m_nStartDate = 0;	
		m_nEndDate = 0;	
		m_nSize = 0;	
		m_nMaxBuyTimes = -1;
		m_nMaxSellTimes = -1;
		m_nMaxGain=-1;
		m_nMaxDeficit=-1;
		m_nBuyType=0;
		m_nBuyNum=0;
		m_nFees=0;
		m_nGap =30;
		m_lEffectStartTime = 930;		//��Чʱ����ʼ
		m_lEffectEndTime = 1500;			//��Чʱ��ֹ
		m_pData = NULL;
		m_pTradeDataSrc = NULL;
		m_nStop = 0;
		m_nDynStop = 0;
		m_lSellStopLossRange = 0;
		m_lSellStopProfitRange = 0;
		m_bOpt = 0;
		m_bByDay = 0;
	}
	void Free()
	{
		if (m_pCode)
		{
			ZeroMemory(m_pCode->m_cCode,6);
			m_pCode->m_cCodeType = 0;
		}
		ZeroMemory(m_sCodeName,16);
		ZeroMemory(m_sExpName,32);	
		ZeroMemory(m_cTradeName,32);
		ZeroMemory(m_cTrigName,32);
		ZeroMemory(m_Tradecode,6);
		m_bFuQuan = 1;
		m_fCost = 0;
		m_lHolding = 0;
		m_nMoney = 0;		
		m_nPeriodType = 0;	
		m_nPeriodNumber = 0;
		m_nStartDate = 0;	
		m_nEndDate = 0;	
		m_nMaxBuyTimes = -1;
		m_nMaxSellTimes = -1;
		m_nMaxGain=-1;
		m_nMaxDeficit=-1;
		m_nBuyType=0;
		m_nBuyNum=0;
		m_nFees=0;
		m_nGap =30;
		m_lEffectStartTime = 930;		//��Чʱ����ʼ
		m_lEffectEndTime = 1500;			//��Чʱ��ֹ
		m_nStop = 0;
		m_nDynStop = 0;
		m_lSellStopLossRange = 0;
		m_lSellStopProfitRange = 0;
		m_bOpt = 0;
		m_bByDay = 0;
		if(m_nSize > 0)
		{
			delete[] m_pData;
			m_pData = NULL;
			m_nSize = 0;
		}
	}
};
//�ͻ������ݰ���ͷ
struct CNI_General
{
	int	 m_nSize;	// ���ݰ�����
	long m_lCommID;	// ͨѶ����
	int	 m_nEvent;	// �¼�, CEV_...

	long m_nLoginSrvType;	// �μ�RT_LOGIN����
};
struct CNI_ReceiveData
{
	struct CNI_General m_hd;	//���ݱ�ͷ
	int m_nStatus;				// ״̬CLX_TS_...
	BOOL m_bFile;				//�Ƿ�Ϊ�ļ�
	long m_lDataTotal;			//���ݳ���
	long m_lDataTransmited;		//�Ѿ����͵����ݳ���
	const char* m_pszData;		// if m_bFile is TRUE, m_pszData points to file name
};

#define FINANCIAL_DATA_LENGTH	118 /* ��Ӧ�汾 20040401-2 */
/* �������� */
struct UpdatedFinancialData
{
	char		m_strCode[6];	 	/* ��Ʊ����	 */
	long		m_lBiuldDate;		/* ��������  */
	long		m_lModifyDate;		/* �޸�����	 */
	float		m_fData[FINANCIAL_DATA_LENGTH]; /* ���������� */
};
/* ���񱨱������ID���Խṹ */
struct FinancialColProperty
{
	//DWORD	m_dwStyle;				/* ���, ����δʹ�� */
	DWORD	m_dwID;					/* �ֶ�ID */

	CString	m_strSimpleEnglish;		/* Ӣ�ļ�� */
	//CString	m_strSimpleChinese;	/* ���ļ�� */

	FinancialColProperty()
	{
		m_dwID = 0;		
	}

	FinancialColProperty(DWORD	dwID,	LPCTSTR	lszSimpleEnglish,
		LPCTSTR	lpszSimpleChinese = NULL, DWORD	dwStyle = 0)
	{
		ASSERT(lszSimpleEnglish );

		//m_dwStyle	= dwStyle;
		m_dwID		= dwID;

		m_strSimpleEnglish.Format("%s", lszSimpleEnglish);	

		/*if(lpszSimpleChinese)
		{
			m_strSimpleChinese.Format("%s", lpszSimpleChinese);		
		}*/
	};

	CString GetSimpleChinese()
	{
		CString strChinese;
		ASSERT(strChinese.LoadString(m_dwID));
		return strChinese;
	}
};
class AllFinancialData : public HSRetFileData
{
public:
	AllFinancialData() ;
	~AllFinancialData() ;

	HSDouble GetDataByName(UpdatedFinancialData* pData, CString nID);
	//	���ݲ������ݵ�ID�ŷ������Ӧ�Ĳ�������
	HSDouble GetDataByID(UpdatedFinancialData* pData, UINT nID);
	//	���ݲ������ݵ����λ�÷������Ӧ�Ĳ�������
	HSDouble GetDataByPos(UpdatedFinancialData* pData, int nPos);

	//	��������Ƿ���Ч
	BOOL IsEmpty(CString code); 	

protected:
	CCalcExternParam* m_pCalcExternParam;
public:
	BOOL IsMyBlockData(int nBegin, int nEnd, CString strBlock);
	void SetMyBlockData(int nBegin, int nEnd, CString strBlock);
public:
	/* ȡ�õ�ǰ���ݳ��� */

	long GetSize(){return m_lSize / sizeof(UpdatedFinancialData);}

	/* ͨ����������λ��, ��������ָ�� */

	UpdatedFinancialData* GetData(int nPos);

	/* ��������,���ظ�����������һ���� 1,2,3,4*/
	int GetType(long lDate);	

public:
	void Find(int Begin,int End, int &nBeginPos, int &nEndPos);

	/* ���������۰����, ������������λ�� ģ�����һ�׼ȷ����*/
	int Find(long lDate, long nLow = -1, long nHigh = -1, BOOL bBlur = TRUE,
	BOOL bBinarySearch = TRUE);	
public:
	void ReSort();
public:
	/* ���񱨱������������ID�Ĺ�ϣ�� */
	static CMapStringToPtr* m_pMapStringToID;
public:
	static CMapStringToPtr* GetMap();
	static void DeleteMap();
public:	
	/* 
	ͨ��Ӣ�ļ��, ���ظ�ID(���pProperty��Ϊ��, �������ݵ�pProperty)
	*/
	int GetFinColProperty(CString strSimpleEn, FinancialColProperty* pProperty = NULL);
};


class CFinanceStatmentFile
{
public:
	CFinanceStatmentFile(void);
	~CFinanceStatmentFile(void);
public:
	/* ��װ�����ļ� �ļ��ṹ��ͨѶЭ��*/
	CString	Setup(void* pParamData , int* pThreadStatus = NULL); 
	/* ��װ���µ����� */
	BOOL	Setup(UpdatedFinancialData* pData, const short nSize, const char cAppend = TRUE); 
public:
	/* ���ļ���ȡ������ */
	static BOOL	Load(LPCTSTR lpszCode, AllFinancialData* pAllFinancialData);

public:
	/* ȡ�ô��������ļ�·��(�ͻ���) */
	static CString GetFilePath(LPCTSTR lpszCode);
public:
	/* ��ӳ�����ļ�����д��,�ر��ļ�,�ͷ�ӳ��� */
	BOOL	Flush();
protected:
	/* ��Ʊ�������ļ�ָ���ӳ���*/
	CMapStringToPtr* m_mapPosition;
	BOOL Lookup( LPCTSTR key, CFile*& rValue);
};


/*
��������: RT_TECHDATA / RT_TECHDATA_EX
����˵��: �̺����
*/

///*����ṹ*/
//struct ReqDayData 	
//{
//	short				m_nPeriodNum;		// ���ڳ���,����������
//	unsigned short		m_nSize;			// �������ݵ�ǰ�Ѿ���ȡ������ʼ����,����������
//	long				m_lBeginPosition;	// ��ʼ������-1 ��ʾ��ǰλ�á� �����������Ѿ����صĸ�����
//	unsigned short		m_nDay;				// ����ĸ���
//	short				m_cPeriod;			// ��������
//	CodeInfo			m_ciCode;			// ����Ĺ�Ʊ����
//
//	void To(char cIntelToComputer )
//	
//	}
//};



template <class THSMapBaseType>
class CMapStringToPtrEx : public CMapStringToPtr
{
public:
	// add a new (key, value) pair
	void SetAt(LPCTSTR key, void* newValue)
	{
		void* pData;
		if( this->Lookup(key,pData) ) // �Ƿ����
		{
			return;
		}
		CMapStringToPtr::SetAt(key, newValue);	
	}

	BOOL SetAt(LPCTSTR key, THSMapBaseType newValue)
	{
		void* pData;
		if( this->Lookup(key,pData) ) // �Ƿ����
		{
			return FALSE;
		}
		CMapStringToPtr::SetAt(key, (void*)newValue);
		return TRUE;
	}

	BOOL SetAt(CodeInfo* pCode, THSMapBaseType newValue)
	{
		if( pCode == NULL )
			return FALSE;
		CString key;
		key.Format("%s",pCode->GetKey());
		void* pData;
		if( this->Lookup(key,pData) ) // �Ƿ����
		{
			return FALSE;
		}
		CMapStringToPtr::SetAt(key, (void*)newValue);
		return TRUE;
	}

	// Lookup
	BOOL Lookup(LPCTSTR key, void*& rValue) const
	{ return CMapStringToPtr::Lookup(key, rValue); }

	BOOL Lookup(CodeInfo* pCode, char* strKey,THSMapBaseType& rValue) const
	{ 		
		return CMapStringToPtr::Lookup( pCode->GetKey(strKey), (void*&)rValue ); 
	}

	void Free()
	{
		POSITION pos = this->GetStartPosition();
		CString	sName;
		void *pVal;
		while (pos != NULL)
		{
			this->GetNextAssoc (pos, sName, pVal);
			if(pVal != NULL)
			{
				delete (THSMapBaseType)pVal;
			}
		}
		this->RemoveAll();
	}
};

// ��pCode����ȫ�ֵ�ѡ�ɽ���б���,�����ض�Ӧ�ķ��ɳɹ���������
extern CArray<SuccResult*,SuccResult*>* AddSuccDataToMap(CodeInfo* pCode);
extern CTime LongToDate(unsigned long lDate);


//========================================================================
#endif
