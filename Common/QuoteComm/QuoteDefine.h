/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	QuoteDefine.h
*	文件标识：
*	摘	  要：	定义一些通用的常量
*	
*	当前版本：
*	作	  者：	
*	完成日期：  
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once

#define INFO_PATH_INI_FILE		"infopath.ini"			/* 资讯路径配置文件*/
#define INFO_TREE_INI_FILE		"InfoTree.ini"			/* 资讯树配置文件 */
#define BASE_INFO_TREE_INI_FILE	"baseinfotree.ini"		/* 基本资料树 */
#define SECRET_TREE_INI_FILE	"secrettree.ini"		/* 秘籍树 */
#define DYN_NEWS_INI_FILE		"dynnews.ini"			/* 滚动新闻文件	*/
#define WELCOME_HTM_FILE		"welcome.htm"			/* 欢迎信息文件	*/
#define CLIENT_VERSION_INI_FILE	"clientversion.ini"		/* 客户端版本号文件		*/
#define SJSXXN_tishi_duizhaobiao	"sjsxxn.ini"		// 交易所提示信息对照表
#define SRV_condition				"condition.ini"		// 服务器选股配置文件

//内置图层名定义
#define Layout_Empty                 _T("");
#define Layout_TechAnalysis          _T("TechAnalysis") //技术分析&分时
#define Layout_DealDetail            _T("DealDetail")   //F1成交明细
#define Layout_PriceBook             _T("PriceBook")    //F2分价表
#define Layout_F10Info               _T("F10Info")      //F10资讯
#define Layout_MaxSize_F10Info       _T("F10InfoFullScreen") // F10全屏资讯
#define Layout_QuoteReport           _T("QuoteReport")  //行情报价
#define Layout_MiniQuote             _T("MiniQuote")    //交易行情
#define Layout_CombAnalysis        _T("CombAnalysis")//组合分析
#define Layout_MultiCombAnalysis _T("MultiCombAnalysis")//组合分析

#define DiagramMsgBase        1000
//demo #define DiagramMsg1           DiagramMsgBase + 1
#define DiagramMsg_TickOnClick       DiagramMsgBase + 1  // 在分笔单击选中一数据项，向逐笔图元发送该消息
#define DiagramMsg_BlockChange       DiagramMsgBase + 2  // 行情报价切换板块
    #define Param_BlockChange_BlockName     "block_name" // 切换板块参数
#define DiagramMsg_OpenLayout        DiagramMsgBase + 3  // 打开某一个图元布局
    // OpenLayout 公用参数
	#define Param_Open_MarketType    "mark_type"      // 市场类型
	#define Param_Open_StockCode     "stock_code"     // 证券代码
    #define Param_Open_Mark          "mark"           // 要打开的图元表示
	// 
#define DiagramMsg_FormulaParamChanged  DiagramMsgBase + 7  // 公式参数变更
	#define Param_TechAnalyas_ExpressName     _T("express_name")   //公式名称
	#define Param_TechAnalyas_ExpressType     _T("express_type")   //公式类型
	#define Param_TechAnalyse_ExpressStyle    _T("express_style")  //公式分割（主副图叠加）
 
#define DiagramMsg_SelStockAns          DiagramMsgBase + 8 //选股结果 
	#define Param_SelStockAns_Count                _T("sel_stock_ans_count")
    #define Param_SelStockAns_MarketType_Begin     _T("mark_type_begin")
	#define Param_SelStockAns_StockCode_Begin      _T("stock_code_begin")
//组合指标线变更
#define DiagramMsg_Comb_Indicatrix_Sel        DiagramMsgBase + 9
	#define Param_Comb_IndicatrixObj    _T("indicatrix_obj")
	#define Param_Indicatrix_Name          _T("indicatrix_name")
	#define Param_Indicatrix_DispName    _T("indicatrix_dispname")
#define DiagramMsg_Multi_Comb_Indicatrix_Sel        DiagramMsgBase + 10
	#define Param_WatchSetObj	            _T("watchset_obj")  
	#define Param_Indicatrix_Name          _T("indicatrix_name")
//
#define DiagramMsg_Refresh_Indicatrix_Line             DiagramMsgBase + 11
//监控对象变更
#define DiagramMsg_WatcherObj_Change                DiagramMsgBase + 12  
	#define Param_WatcherObj                                 	_T("watcher_obj")    //指标对象指针
    #define Param_Change_Flag                                	_T("change_flag")
         #define  Param_Change_Flag_Del                       _T("1")