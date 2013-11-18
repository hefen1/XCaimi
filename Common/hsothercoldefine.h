/*******************************************************************************
 * Copyright (c)2010, �������ӹɷ����޹�˾
 * All rights reserved.
 *
 * �ļ����ƣ�HSOtherColDefine.h
 * �ļ���ʶ���ֶκ궨��
 * ժ    Ҫ�����ڿͻ��˵��ֶκ궨�壬��hscoldefine.h���ʹ��
 *
 * �޸ļ�¼��
 * ��    �ߣ�
 * �޸����ڣ�
 * �޸����ݣ�
 *******************************************************************************/
#pragma once
#include "hscoldefine.h"

/* �ֶη�������*���� BEGIN*/
#define	HQ_BASE_NAME				_T("��Ʊ�����ֶ�")
#define	HQ_EX_NAME					_T("��Ʊ��չ����")
#define	HQ_DETAIL_NAME				_T("��ϸ��")
#define	GB_GUBEN_NAME				_T("�ɱ�����")
#define	Cj_GUBEN_NAME				_T("��������")
#define	LOCK_NAME					_T("��������")
#define	USR_DEF_NAME				_T("�Զ���")
#define ORIGINAL_USE_COLUMN_NAME	_T("�ڲ�ʹ��")
#define Block_Anysis_Name			_T("�������")
#define ZiXun_Browse_Name			_T("��Ѷ/EMAIL�ֶ�")
#define Qihuo_HangQing_Name			_T("�ڻ�����")
#define FINANCIAL_STATEMENT_NAME	_T("�������")
#define HS_BLOCK_NAME				_T("�����Ϣ")
/* �ֶη�������*���� END*/

/* ���䶨��hscoldefine.h BEGIN*/
#define COLUMN_HQ_BASE_NUMBER				(COLUMN_HQ_BASE_BEGIN + 73) // ���
#define COLUMN_HQ_BASE_TECH_SORT			(COLUMN_HQ_BASE_BEGIN + 74) // ����ָ����������
#define COLUMN_HQ_BASE_INFO_MARK			(COLUMN_HQ_BASE_BEGIN + 75) // ��Ϣ���ױ��
#define COLUMN_HQ_BASE_ARROW				(COLUMN_HQ_BASE_BEGIN + 76) // ���¼�ͷ
#define COLUMN_HQ_BASE_INVERSE_PRICE		(COLUMN_HQ_BASE_BEGIN + 77) // ������
/* ���䶨��hscoldefine.h END*/


#ifdef COLUMN_END
	#define COLUMN_OTHER_BEGIN				(COLUMN_END + 1)	
#else
	#define COLUMN_OTHER_BEGIN				11000
#endif
#define COLUMN_OTHER_END					(COLUMN_OTHER_BEGIN + 2999)

/* ��ϸ�� COLUMN_HQ_DETAIL_ DEFINE BEGIN*/
#define	COLUMN_HQ_DETAIL_BEGIN				(COLUMN_OTHER_BEGIN)
#define	COLUMN_HQ_DETAIL_END				(COLUMN_HQ_DETAIL_BEGIN + 150)

// ��ʱ��ϸ��
#define COLUMN_HQ_DETAIL_TREND_Time        	(COLUMN_HQ_DETAIL_BEGIN + 0)	//ʱ��
#define COLUMN_HQ_DETAIL_TREND_NEW_PRICE   	(COLUMN_HQ_DETAIL_BEGIN + 1)	//�ɽ���
#define COLUMN_HQ_DETAIL_TREND_RISE_VALUE  	(COLUMN_HQ_DETAIL_BEGIN + 2)	//�ǵ�ֵ
#define COLUMN_HQ_DETAIL_TREND_BUY_PRICE    (COLUMN_HQ_DETAIL_BEGIN + 3)	//�����
#define COLUMN_HQ_DETAIL_TREND_SELL_PRICE   (COLUMN_HQ_DETAIL_BEGIN + 4)	//������
#define COLUMN_HQ_DETAIL_TREND_HAND     	(COLUMN_HQ_DETAIL_BEGIN + 5)	//����
//	�����ӣ�20040315
#define	COLUMN_HQ_DETAIL_TREND_REALHANDS	(COLUMN_HQ_DETAIL_BEGIN + 6)	//	ʵ�ʷֱ���
//	�����ӽ���
#define COLUMN_HQ_DETAIL_TREND_POSITION		(COLUMN_HQ_DETAIL_BEGIN+7)		// �ܳ�
#define COLUMN_HQ_DETAIL_TREND_KPC			(COLUMN_HQ_DETAIL_BEGIN+8)		// ��ƽ��

// �̺���ϸ
#define COLUMN_HQ_DETAIL_TECH_TIME        	(COLUMN_HQ_DETAIL_BEGIN + 20)	//ʱ��
#define COLUMN_HQ_DETAIL_TECH__RISE_VALUE   (COLUMN_HQ_DETAIL_BEGIN + 21)	//�ǵ�
#define COLUMN_HQ_DETAIL_TECH_OPEN_PRICE   	(COLUMN_HQ_DETAIL_BEGIN + 22)	//����
#define COLUMN_HQ_DETAIL_TECH_MAX_PRICE    	(COLUMN_HQ_DETAIL_BEGIN + 23)	//���
#define COLUMN_HQ_DETAIL_TECH_MIN_PRICE    	(COLUMN_HQ_DETAIL_BEGIN + 24)	 //���
#define COLUMN_HQ_DETAIL_TECH_CLOSE_PRICE  	(COLUMN_HQ_DETAIL_BEGIN + 25)	 //����
#define COLUMN_HQ_DETAIL_TECH_VOLUME      	(COLUMN_HQ_DETAIL_BEGIN + 26)	 //�ɽ���
#define COLUMN_HQ_DETAIL_TECH_5DAY_AVGPRICE (COLUMN_HQ_DETAIL_BEGIN + 27)	 //5�վ���
#define COLUMN_HQ_DETAIL_TECH_5DAY_TOTALVOL (COLUMN_HQ_DETAIL_BEGIN + 28)	 //5������
// ������ϸ
#define COLUMN_HQ_DETAIL_INDEX_TIME		  	(COLUMN_HQ_DETAIL_BEGIN + 40)	 //ʱ��
#define COLUMN_HQ_DETAIL_INDEX_INDEX	  	(COLUMN_HQ_DETAIL_BEGIN + 41)	 //ָ��
#define COLUMN_HQ_DETAIL_INDEX_RISE_VALUE 	(COLUMN_HQ_DETAIL_BEGIN + 42)	 //�ǵ�ֵ
#define COLUMN_HQ_DETAIL_INDEX_RISE_RATIO 	(COLUMN_HQ_DETAIL_BEGIN + 43)	 //�ǵ���
#define COLUMN_HQ_DETAIL_INDEX_RISE_COUNT 	(COLUMN_HQ_DETAIL_BEGIN + 44)	 //���Ǽ���
#define COLUMN_HQ_DETAIL_INDEX_FALL_COUNT 	(COLUMN_HQ_DETAIL_BEGIN + 45)	 //�µ�����
#define COLUMN_HQ_DETAIL_INDEX_EQUAL_COUNT	(COLUMN_HQ_DETAIL_BEGIN + 46)    //ƽ�̼���	
#define COLUMN_HQ_DETAIL_INDEX_SUM_PRICE	(COLUMN_HQ_DETAIL_BEGIN + 47)	 //�ۼƳɽ�ֵ
#define COLUMN_HQ_DETAIL_INDEX_PRICE		(COLUMN_HQ_DETAIL_BEGIN + 48)	 //�ɽ�ֵ
#define COLUMN_HQ_DETAIL_INDEX_SUM_VOLUME	(COLUMN_HQ_DETAIL_BEGIN + 49)	 //�ۼƳɽ���
#define COLUMN_HQ_DETAIL_INDEX_VOLUME		(COLUMN_HQ_DETAIL_BEGIN + 50)	 //�ɽ���
// ������
#define COLUMN_HQ_DETAIL_BSORDER_DIRECTION	(COLUMN_HQ_DETAIL_BEGIN + 60)	 //��������
#define COLUMN_HQ_DETAIL_BSORDER_PRICE		(COLUMN_HQ_DETAIL_BEGIN + 61)	 //�ɽ���
#define COLUMN_HQ_DETAIL_BSORDER_VOLUME     (COLUMN_HQ_DETAIL_BEGIN + 62)	 //�ɽ���
// �ּ۱�
#define COLUMN_HQ_DETAIL_SPREAD_PRICE		(COLUMN_HQ_DETAIL_BEGIN + 66)	 //�ɽ���
#define COLUMN_HQ_DETAIL_SPREAD_VOLUME		(COLUMN_HQ_DETAIL_BEGIN + 67)	 //����
#define COLUMN_HQ_DETAIL_SPREAD_GRAPHIC		(COLUMN_HQ_DETAIL_BEGIN + 68)	 //�ּ۱��ͼ
#define	COLUMN_HQ_DETAIL_SPREAD_VOLUMERATIO	(COLUMN_HQ_DETAIL_BEGIN + 69)	 //�ּ�����
#define	COLUMN_HQ_DETAIL_SPREAD_SELLVOLUME	(COLUMN_HQ_DETAIL_BEGIN + 70)	 //���̻�ͼ

// ���񱨱�
#define COLHEAD_FINANCE_NAME				(COLUMN_HQ_DETAIL_BEGIN + 80)	 //����
#define COLHEAD_FINANCE_VALUE				(COLUMN_HQ_DETAIL_BEGIN + 81)	 //����

// ��������ϸ
#define COLUMN_BSORDER_ORDER				(COLUMN_HQ_DETAIL_BEGIN	+ 82)	 //�ɽ�
#define COLUMN_BSORDER_BS1					(COLUMN_HQ_DETAIL_BEGIN + 83)	 //����һ
#define COLUMN_BSORDER_BS2					(COLUMN_HQ_DETAIL_BEGIN + 84)	 //�����
#define COLUMN_BSORDER_BS3					(COLUMN_HQ_DETAIL_BEGIN + 85)	 //������
#define COLUMN_BSORDER_BS4					(COLUMN_HQ_DETAIL_BEGIN + 86)	 //������
#define COLUMN_BSORDER_BS5					(COLUMN_HQ_DETAIL_BEGIN + 87)	 //������

// ��ɳ�Ȩ
#define COLUMN_HQ_DETAIL_EXRIGHT_DATE		(COLUMN_HQ_DETAIL_BEGIN	+ 90)	 //��Ȩ����
#define COLUMN_HQ_DETAIL_EXRIGHT_GIVE		(COLUMN_HQ_DETAIL_BEGIN + 91)	 //�͹�
#define COLUMN_HQ_DETAIL_EXRIGHT_PLACING	(COLUMN_HQ_DETAIL_BEGIN + 92)	 //���
#define COLUMN_HQ_DETAIL_EXRIGHT_GIVE_PRICE	(COLUMN_HQ_DETAIL_BEGIN + 93)	 //�͹ɼ�
#define COLUMN_HQ_DETAIL_EXRIGHT_BONUS		(COLUMN_HQ_DETAIL_BEGIN + 94)	 //�ֺ�
/* ��ϸ�� COLUMN_HQ_DETAIL_ DEFINE END*/

/* �ɱ����� COLUMN_CAPITALIZATION_ DEFINE BEGIN*/
#define	COLUMN_CAPITALIZATION_BEGIN			(COLUMN_HQ_DETAIL_END + 1)
#define	COLUMN_CAPITALIZATION_END			(COLUMN_CAPITALIZATION_BEGIN + 20)

#define	COLUMN_CAPITALIZATION_TOTAL			COLUMN_CAPITALIZATION_BEGIN		  //1 �ܹɱ�����ɣ�
#define	COLUMN_CAPITALIZATION_NATIONAL		(COLUMN_CAPITALIZATION_BEGIN + 1) //2 ���ҹ�
#define	COLUMN_CAPITALIZATION_INITIATOR		(COLUMN_CAPITALIZATION_BEGIN + 2) //3 �����˷��˹�
#define	COLUMN_CAPITALIZATION_CORPORATION	(COLUMN_CAPITALIZATION_BEGIN + 3) //4 ���˹� 
#define	COLUMN_CAPITALIZATION_B				(COLUMN_CAPITALIZATION_BEGIN + 4) //5 B��
#define	COLUMN_CAPITALIZATION_H				(COLUMN_CAPITALIZATION_BEGIN + 5) //6 H��
#define	COLUMN_CAPITALIZATION_PASS_A		(COLUMN_CAPITALIZATION_BEGIN + 6) //7 ��ͨA�� 
#define	COLUMN_CAPITALIZATION_EMPLOYEE		(COLUMN_CAPITALIZATION_BEGIN + 7) //8 ְ����
#define	COLUMN_CAPITALIZATION_A2_GIVE		(COLUMN_CAPITALIZATION_BEGIN + 8) //9 A2ת���
/* �ɱ����� COLUMN_CAPITALIZATION_ DEFINE END*/

/* 
�������� COLUMN_FINANCE_ DEFINE BEGIN 
ACCFUND = accumulation fund
*/
#define	COLUMN_FINANCE_BEGIN				(COLUMN_CAPITALIZATION_END + 1)
#define	COLUMN_FINANCE_END					(COLUMN_FINANCE_BEGIN + 50)

#define	COLUMN_FINANCE_TOTAL_ASSETS			COLUMN_FINANCE_BEGIN        //10 ���ʲ� 
#define	COLUMN_FINANCE_CURRENT_ASSETS		(COLUMN_FINANCE_BEGIN+1 )   //11 �����ʲ�  
#define	COLUMN_FINANCE_CAPITAL_ASSETS		(COLUMN_FINANCE_BEGIN+2 )   //12 �̶��ʲ�  
#define	COLUMN_FINANCE_UNBODIED_ASSETS		(COLUMN_FINANCE_BEGIN+3 )   //13 �����ʲ�  
#define	COLUMN_FINANCE_LONG_INVESTMENT		(COLUMN_FINANCE_BEGIN+4 )   //14 ����Ͷ��  
#define	COLUMN_FINANCE_CURRENT_LIABILITIES  (COLUMN_FINANCE_BEGIN+5 )   //15 ������ծ  
#define	COLUMN_FINANCE_LONG_LIABILITIES		(COLUMN_FINANCE_BEGIN+6 )   //16 ���ڸ�ծ  
#define	COLUMN_FINANCE_CAPITAL_ACCFUND		(COLUMN_FINANCE_BEGIN+7 )   //17 �ʱ�������
#define	COLUMN_FINANCE_PERSTOCK_ACCFUND	    (COLUMN_FINANCE_BEGIN+8 )   //18 ÿ�ɹ�����
#define	COLUMN_FINANCE_PARTNER_RIGHT	    (COLUMN_FINANCE_BEGIN+9 )   //19 �ɶ�Ȩ��  
#define	COLUMN_FINANCE_MAIN_INCOME			(COLUMN_FINANCE_BEGIN+10 )  //20 ��Ӫ����
#define	COLUMN_FINANCE_MAIN_PROFIT			(COLUMN_FINANCE_BEGIN+11 )  //21 ��Ӫ���� 
#define	COLUMN_FINANCE_OTHER_PROFIT		    (COLUMN_FINANCE_BEGIN+12 )  //22 �������� 
#define	COLUMN_FINANCE_TAKING_PROFIT		(COLUMN_FINANCE_BEGIN+13 )  //23 Ӫҵ����  
#define	COLUMN_FINANCE_YIELD				(COLUMN_FINANCE_BEGIN+14 )  //24 Ͷ������  
#define	COLUMN_FINANCE_SUBSIDY				(COLUMN_FINANCE_BEGIN+15 )  //25 ��������
#define	COLUMN_FINANCE_OTHER_INCOME		    (COLUMN_FINANCE_BEGIN+16 )  //26 Ӫҵ����֧
#define	COLUMN_FINANCE_LAST_PROFIT_LOSS		(COLUMN_FINANCE_BEGIN+17 )  //27 �����������
#define	COLUMN_FINANCE_TOTAL_PROFIT			(COLUMN_FINANCE_BEGIN+18 )  //28 �����ܶ�
#define	COLUMN_FINANCE_SCOT_PROFIT			(COLUMN_FINANCE_BEGIN+19 )	//29 ˰������
#define	COLUMN_FINANCE_RETAINED_PROFITS		(COLUMN_FINANCE_BEGIN+20 )  //30 ������
#define	COLUMN_FINANCE_UNPAID_PROFIT		(COLUMN_FINANCE_BEGIN+21 )  //31 δ��������
#define	COLUMN_FINANCE_PER_UNPAID			(COLUMN_FINANCE_BEGIN+22 )  //32 ÿ��δ����
#define	COLUMN_FINANCE_PER_INCOME			(COLUMN_FINANCE_BEGIN+23 )  //33 ÿ������
#define	COLUMN_FINANCE_PER_ASSETS		    (COLUMN_FINANCE_BEGIN+24 )	//34 ÿ�ɾ��ʲ�
#define	COLUMN_FINANCE_ADJUST_PER_ASSETS	(COLUMN_FINANCE_BEGIN+25 )  //35 ����ÿ�ɾ��ʲ�
#define	COLUMN_FINANCE_PARTNER_RIGHT_RATIO  (COLUMN_FINANCE_BEGIN+26 )  //36 �ɶ�Ȩ���
#define	COLUMN_FINANCE_ASSETS_YIELD			(COLUMN_FINANCE_BEGIN+27 )  //37 ����������
#define	COLUMN_FINANCE_REPORT_DATE			(COLUMN_FINANCE_BEGIN+28 )  //   ������
/* �������� COLUMN_FINANCE_ DEFINE END */

/* ���������� COLUMN_LINKAGE_ DEFINE BEGIN*/
#define	COLUMN_LINKAGE_BEGIN				(COLUMN_FINANCE_END + 1)
#define	COLUMN_LINKAGE_END					(COLUMN_LINKAGE_BEGIN + 100)
#define	AU_CODE_ID							(COLUMN_LINKAGE_BEGIN +0)		// "A�ɴ���"
#define	AU_NAME_ID							(COLUMN_LINKAGE_BEGIN +1)		 // A������	
#define	AU_LATEST_ID						(COLUMN_LINKAGE_BEGIN +2)		//"A�����¼�"
#define	AU_CJL_ID							(COLUMN_LINKAGE_BEGIN +3)		//"A�ɳɽ���"
#define	AU_LTP_ID							(COLUMN_LINKAGE_BEGIN +4)		//"A����ͨ��
#define	AU_LTPBL_ID							(COLUMN_LINKAGE_BEGIN +5)		//"A����ͨ�̱���"
#define	AU_ZF_ID							(COLUMN_LINKAGE_BEGIN +6)		//"A���Ƿ�"
#define	AU_5ZF_ID							(COLUMN_LINKAGE_BEGIN +7)		//A��5���Ƿ�"
#define	AU_10ZF_ID							(COLUMN_LINKAGE_BEGIN +8)		//"A��10�Ƿ�"
#define	AU_20ZF_ID							(COLUMN_LINKAGE_BEGIN +9)		//"A��20�Ƿ�"
#define	AU_60ZF_ID							(COLUMN_LINKAGE_BEGIN +10 )		//"A��60�Ƿ�"
#define	AU_YL_ID							(COLUMN_LINKAGE_BEGIN +11 )		//"A����ӯ��"
#define	AU_HSL_ID							(COLUMN_LINKAGE_BEGIN +12 )		//"A�ɻ�����"
#define	AU_LTPSZ_ID							(COLUMN_LINKAGE_BEGIN +13 )		//"A����ͨ����ֵ"

#define	BU_CODE_ID							(COLUMN_LINKAGE_BEGIN +14 ) 	//"B�ɴ���"
#define	BU_NAME_ID							(COLUMN_LINKAGE_BEGIN +15 ) 	//"B������"
#define	BU_LATEST_ID						(COLUMN_LINKAGE_BEGIN +16 )		//"B�����¼�"
#define	BU_CJL_ID							(COLUMN_LINKAGE_BEGIN +17 )		//"B�ɳɽ���"
#define	BU_LTP_ID							(COLUMN_LINKAGE_BEGIN +18 )		//"B����ͨ��"
#define	BU_LTPBL_ID							(COLUMN_LINKAGE_BEGIN +19 )		//"B����ͨ�̱���"
#define	BU_ZF_ID							(COLUMN_LINKAGE_BEGIN +20 )		//"B���Ƿ�"
#define	BU_5ZF_ID							(COLUMN_LINKAGE_BEGIN +21 )		//"B��5���Ƿ�"
#define	BU_10ZF_ID							(COLUMN_LINKAGE_BEGIN +22 )		//"B��10�Ƿ�"
#define	BU_20ZF_ID							(COLUMN_LINKAGE_BEGIN +23 )		//"B��20�Ƿ�"
#define	BU_60ZF_ID							(COLUMN_LINKAGE_BEGIN +24 )		//"B��60�Ƿ�"
#define	BU_YL_ID							(COLUMN_LINKAGE_BEGIN +25 )		//"B����ӯ��"
#define	BU_HSL_ID							(COLUMN_LINKAGE_BEGIN +26 )		//"B�ɻ�����"
#define	BU_LTPSZ_ID							(COLUMN_LINKAGE_BEGIN +27 )		//"B����ͨ����ֵ"

#define	HU_CODE_ID							(COLUMN_LINKAGE_BEGIN +28 )		//"H�ɴ���"
#define	HU_NAME_ID							(COLUMN_LINKAGE_BEGIN +29 )		//"H������"
#define	HU_LATEST_ID						(COLUMN_LINKAGE_BEGIN +30 )		//"H�����¼�"
#define	HU_CJL_ID							(COLUMN_LINKAGE_BEGIN +31 )		//"H�ɳɽ���"
#define	HU_LTP_ID							(COLUMN_LINKAGE_BEGIN +32 )		//"H����ͨ��"
#define	HU_LTPBL_ID							(COLUMN_LINKAGE_BEGIN +33 )		//"H����ͨ�̱���"
#define	HU_ZF_ID							(COLUMN_LINKAGE_BEGIN +34 )		//"H���Ƿ�"
#define	HU_5ZF_ID							(COLUMN_LINKAGE_BEGIN +35 )		//"H��5���Ƿ�"
#define	HU_10ZF_ID							(COLUMN_LINKAGE_BEGIN +36 )		//"H��10�Ƿ�"
#define	HU_20ZF_ID							(COLUMN_LINKAGE_BEGIN +37 )		//"H��20�Ƿ�"
#define	HU_60ZF_ID							(COLUMN_LINKAGE_BEGIN +38 )		//"H��60�Ƿ�"
#define	HU_YL_ID							(COLUMN_LINKAGE_BEGIN +39 )		//"H����ӯ��"
#define	HU_HSL_ID							(COLUMN_LINKAGE_BEGIN +40 )		//"H�ɻ�����"
#define	HU_LTPSZ_ID							(COLUMN_LINKAGE_BEGIN +41 )		//"H����ͨ����ֵ"

#define	ABU_PRICE_ID						(COLUMN_LINKAGE_BEGIN +42 )		//"A�ɼ۸�/B�ɼ۸�"
#define	AHU_PRICE_ID						(COLUMN_LINKAGE_BEGIN +43 )		//"A�ɼ۸�/H�ɼ۸�"
#define	ALL_GB_ID							(COLUMN_LINKAGE_BEGIN +44 )		//"�ܹɱ�"
#define	EVERY_BEHALF_ID						(COLUMN_LINKAGE_BEGIN +45 )		//"ÿ������"
#define	EVERY_CAPITAL_ID					 ( COLUMN_LINKAGE_BEGIN +46 )	//"ÿ�ɾ��ʲ�"
#define	EVERY_CB_ID							(COLUMN_LINKAGE_BEGIN +47 )		//"���ʲ�������"	
#define	DELETE_ID							(COLUMN_LINKAGE_BEGIN +48 )		//"���ʲ�������"

/* ���������� COLUMN_LINKAGE_ DEFINE END*/

/* ������ COLUMN_BLOCK_ DEFINE BEGIN*/
#define COLUMN_BLOCK_BEGIN					(COLUMN_LINKAGE_END + 1)
#define COLUMN_BLOCK_END					(COLUMN_BLOCK_BEGIN + 40)

#define Block_To_Stock_Name					(COLUMN_BLOCK_BEGIN + 0)
#define Block_To_Stock_1 					(COLUMN_BLOCK_BEGIN + 1)
#define Block_To_Stock_2 					(COLUMN_BLOCK_BEGIN + 2)
#define Block_To_Stock_3 					(COLUMN_BLOCK_BEGIN + 3)
#define Block_To_Stock_4 					(COLUMN_BLOCK_BEGIN + 4)
#define Block_To_Stock_5 					(COLUMN_BLOCK_BEGIN + 5)
#define Block_To_Stock_6 					(COLUMN_BLOCK_BEGIN + 6)
#define Block_To_Stock_7 					(COLUMN_BLOCK_BEGIN + 7)
#define Block_To_Stock_8 					(COLUMN_BLOCK_BEGIN + 8)

//add by lxqi 20090703
/* �����Ϣ BEGIN*/
#define COLUMN_BLOCK_RECOMMEND_NAME			(COLUMN_BLOCK_BEGIN + 10)		// �Ƽ���   
#define COLUMN_BLOCK_RECOMMEND_DATE			(COLUMN_BLOCK_BEGIN + 11)		// ����
#define COLUMN_BLOCK_RECOMMEND_BUYPRICE		(COLUMN_BLOCK_BEGIN + 12)		// ���
#define COLUMN_BLOCK_RECOMMEND_SELLPRICE	(COLUMN_BLOCK_BEGIN + 13)		// ����
#define COLUMN_BLOCK_EX_INFO				(COLUMN_BLOCK_END)				// ��̬��ȡ��չ��Ϣ
/* �����Ϣ END */

//end by lxqi 20090703

/* ������ COLUMN_BLOCK_ DEFINE END*/



// �Զ���������									 
#define	USR_DEF_BEGIN        (COLUMN_BLOCK_END + 1)
#define	USR_DEF_END		     (USR_DEF_BEGIN + 200)									
// �ڲ�ʹ���ֶ�
#define ORIGINAL_USE_COLUMN_BEGIN	(USR_DEF_END + 1)
#define ORIGINAL_USE_COLUMN_END		(ORIGINAL_USE_COLUMN_BEGIN + 200)

// ��Ѷ���
#define ZiXun_Browse_Begin			(ORIGINAL_USE_COLUMN_END + 1)
#define ZiXun_Browse_End			(ZiXun_Browse_Begin + 30)

#define ZiXun_ReadMark			   (ZiXun_Browse_Begin + 0)    // ��д���
#define ZiXun_NewsProtected		   (ZiXun_Browse_Begin + 1)    // ���ű���
#define ZiXun_Block				   (ZiXun_Browse_Begin + 2)    // ���Ű��/��Ŀ
#define ZiXun_Title				   (ZiXun_Browse_Begin + 3)    // ���ű���
#define ZiXun_AcceptTime		   (ZiXun_Browse_Begin + 4)    // ����ʱ��
#define ZiXun_ZuoZhe			   (ZiXun_Browse_Begin + 5)    // ����
#define ZiXun_FuJian			   (ZiXun_Browse_Begin + 6)    // ����
#define ZiXun_InfoSource           (ZiXun_Browse_Begin + 7)    // ��Ϣ�ṩ��

#define ZiXun_CreateTime           (ZiXun_Browse_Begin + 8)     // ����ʱ��
#define ZiXun_ZhongYaoXing         (ZiXun_Browse_Begin + 9)     // ��Ҫ��
#define ZiXun_SendTime             (ZiXun_Browse_Begin + 10)    // ����ʱ��
#define ZiXun_DownloadStatus       (ZiXun_Browse_Begin + 11)    // ����״̬
#define ZiXun_ReadOver		       (ZiXun_Browse_Begin + 12)    // �Ѷ�
#define ZiXun_AcceptMan		       (ZiXun_Browse_Begin + 13)    // �ռ���
#define ZiXun_MaxMin		       (ZiXun_Browse_Begin + 14)    // ��С
#define ZiXun_OverTime		       (ZiXun_Browse_Begin + 15)    // ����ʱ��
#define ZiXun_MinGanDu		       (ZiXun_Browse_Begin + 16)    // ���ж�
#define ZiXun_ChaoSong		       (ZiXun_Browse_Begin + 17)    // ����
#define ZiXun_ConnectMan		   (ZiXun_Browse_Begin + 18)    // ��ϵ���ֶ�
#define ZiXun_Type				   (ZiXun_Browse_Begin + 19)    // ���
#define ZiXun_XuYaoHuiZhi		   (ZiXun_Browse_Begin + 20)    // ��Ҫ��ִ
#define ZiXun_MailInterVer		   (ZiXun_Browse_Begin + 21)    // Mail�ڲ��汾
#define ZiXun_SendMan			   (ZiXun_Browse_Begin + 22)    // ������