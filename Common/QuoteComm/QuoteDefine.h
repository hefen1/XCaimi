/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	QuoteDefine.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	����һЩͨ�õĳ���
*	
*	��ǰ�汾��
*	��	  �ߣ�	
*	������ڣ�  
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once

#define INFO_PATH_INI_FILE		"infopath.ini"			/* ��Ѷ·�������ļ�*/
#define INFO_TREE_INI_FILE		"InfoTree.ini"			/* ��Ѷ�������ļ� */
#define BASE_INFO_TREE_INI_FILE	"baseinfotree.ini"		/* ���������� */
#define SECRET_TREE_INI_FILE	"secrettree.ini"		/* �ؼ��� */
#define DYN_NEWS_INI_FILE		"dynnews.ini"			/* ���������ļ�	*/
#define WELCOME_HTM_FILE		"welcome.htm"			/* ��ӭ��Ϣ�ļ�	*/
#define CLIENT_VERSION_INI_FILE	"clientversion.ini"		/* �ͻ��˰汾���ļ�		*/
#define SJSXXN_tishi_duizhaobiao	"sjsxxn.ini"		// ��������ʾ��Ϣ���ձ�
#define SRV_condition				"condition.ini"		// ������ѡ�������ļ�

//����ͼ��������
#define Layout_Empty                 _T("");
#define Layout_TechAnalysis          _T("TechAnalysis") //��������&��ʱ
#define Layout_DealDetail            _T("DealDetail")   //F1�ɽ���ϸ
#define Layout_PriceBook             _T("PriceBook")    //F2�ּ۱�
#define Layout_F10Info               _T("F10Info")      //F10��Ѷ
#define Layout_MaxSize_F10Info       _T("F10InfoFullScreen") // F10ȫ����Ѷ
#define Layout_QuoteReport           _T("QuoteReport")  //���鱨��
#define Layout_MiniQuote             _T("MiniQuote")    //��������
#define Layout_CombAnalysis        _T("CombAnalysis")//��Ϸ���
#define Layout_MultiCombAnalysis _T("MultiCombAnalysis")//��Ϸ���

#define DiagramMsgBase        1000
//demo #define DiagramMsg1           DiagramMsgBase + 1
#define DiagramMsg_TickOnClick       DiagramMsgBase + 1  // �ڷֱʵ���ѡ��һ����������ͼԪ���͸���Ϣ
#define DiagramMsg_BlockChange       DiagramMsgBase + 2  // ���鱨���л����
    #define Param_BlockChange_BlockName     "block_name" // �л�������
#define DiagramMsg_OpenLayout        DiagramMsgBase + 3  // ��ĳһ��ͼԪ����
    // OpenLayout ���ò���
	#define Param_Open_MarketType    "mark_type"      // �г�����
	#define Param_Open_StockCode     "stock_code"     // ֤ȯ����
    #define Param_Open_Mark          "mark"           // Ҫ�򿪵�ͼԪ��ʾ
	// 
#define DiagramMsg_FormulaParamChanged  DiagramMsgBase + 7  // ��ʽ�������
	#define Param_TechAnalyas_ExpressName     _T("express_name")   //��ʽ����
	#define Param_TechAnalyas_ExpressType     _T("express_type")   //��ʽ����
	#define Param_TechAnalyse_ExpressStyle    _T("express_style")  //��ʽ�ָ����ͼ���ӣ�
 
#define DiagramMsg_SelStockAns          DiagramMsgBase + 8 //ѡ�ɽ�� 
	#define Param_SelStockAns_Count                _T("sel_stock_ans_count")
    #define Param_SelStockAns_MarketType_Begin     _T("mark_type_begin")
	#define Param_SelStockAns_StockCode_Begin      _T("stock_code_begin")
//���ָ���߱��
#define DiagramMsg_Comb_Indicatrix_Sel        DiagramMsgBase + 9
	#define Param_Comb_IndicatrixObj    _T("indicatrix_obj")
	#define Param_Indicatrix_Name          _T("indicatrix_name")
	#define Param_Indicatrix_DispName    _T("indicatrix_dispname")
#define DiagramMsg_Multi_Comb_Indicatrix_Sel        DiagramMsgBase + 10
	#define Param_WatchSetObj	            _T("watchset_obj")  
	#define Param_Indicatrix_Name          _T("indicatrix_name")
//
#define DiagramMsg_Refresh_Indicatrix_Line             DiagramMsgBase + 11
//��ض�����
#define DiagramMsg_WatcherObj_Change                DiagramMsgBase + 12  
	#define Param_WatcherObj                                 	_T("watcher_obj")    //ָ�����ָ��
    #define Param_Change_Flag                                	_T("change_flag")
         #define  Param_Change_Flag_Del                       _T("1")