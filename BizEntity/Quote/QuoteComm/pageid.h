/*************************************************************
*	Copyright (c)2002, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	pageid.h
*	作	  者：	吕连新
*  当前版本：	4.0
*  创建日期：	2003年5月19日
*  完成日期：	2003年5月19日
*  描    述：	页面ID定义文件
*
*	修改历史：	
*          日期：  
*          作者：  
*          改动：  
*
***************************************************************/

#ifndef _PAGEID_H_
#define _PAGEID_H_
  
#define  TextMenuWindow           0x0001    //文字菜单 
/////////////////////////////////////////////////////////////////
//大盘分析的几项
  #define  Sh30Exp            0x0010    //上证１８０指数
  #define  ShTrend            0x0011    //上证走势
  #define  ShLeadExp          0x0012    //上证领先指标
  #define  ShATrend           0x0013    //上证A股走势 
  #define  ShBTrend           0x0014    //上证B股走势
  #define  ShADL              0x0015    //上证ADL指标
  #define  ShDuoKong          0x0016    //上证多空指标
  #define  ShLiDao            0x0017    //上证买卖力道
  #define  ShPerExpTrend      0x0018    //上证分类指数走势
  #define  SzTotalExpTrend    0x0019    //深证综合指数走势
  #define  SzTranExpTrend     0x001A    //深证成交指数走势
  #define  SzLeadExp          0x001B    //深证领先指标
  #define  SzATrend           0x001C    //深证A股走势
  #define  SzBTrend           0x001D    //深证B股走势
  #define  SzADL              0x001E    //深证ADL指标
  #define  SzDuoKong          0x001F    //深证多空指标
  #define  SzLiDao            0x0020    //深证买卖力道
  #define  SzPerExpTrend      0x0021    //深证分类指数走势
  #define  ShSzTrend          0x0022    //上证深证走势
  #define  ShSzTotal          0x0023    //上证深证综合
  #define  BigTchAnalisys     0x0024    //大盘技术分析
  #define  ExpReportForm      0x0025    //指数行情报表（5，15，30，60）

  #define  StockBigAnalisys   0x0028

  #define  BigBegin			  Sh30Exp
  #define  BigEnd			  ShSzTrend
/////////////////////////////////////////////////////////////////
//文本浏览窗口
  #define  TextView			  0x0050
/////////////////////////////////////////////////////////////////
//个股分析的几项
  #define  StockAnalisys      0x0060    //个股及时分析（量比，分时，力道）

  #define  StockTrDetail      0x0061    //个股成交明细
  #define  StockFenPrice      0x0062    //个股分价表
  #define  Stock1TrDetail     0x0063    //个股一分钟成交明细
  #define  StockDayDetail     0x0065    //日线成交明细
  #define  Big1TrDetail       0x0066    //大盘1分钟成交明细
  #define  Big5TrDetail       0x0067    //大盘五分钟成交明细
  #define  StockMaiMaiChart   0x0068    //个股买卖盘图

  #define  StockTchAnalisys   0x0069    //个股技术分析 
  #define  StockReportForm    0x0070    //个股行情播报
  #define  StockTickChart     0x0071    //个股tick图
  #define  StockStarField     0x0072    //星空图
  #define  StockMulTrend	  0x0073	//多日分时图 	
  #define  StockGannAnalisys  0x0074	//江恩图分析
  #define  HS_PAGEID_GRAPHICS_FINANCE	0x0075	//	财务图示页面
  #define  TechTickChart	  0x0076	//add by lxqi 20090622  技术Tick图
  


///////////////////////////////////////////////////////////////////////  
//报价分析的几项
  #define  PriceAnalisys1     0x0082    //报价分析1
  #define  PriceAnalisys2     0x0083    //报价分析2
  #define  PriceAnalisys3     0x0084    //报价分析3
  #define  PriceAnalisys4     0x0085    //报价分析4

  #define  PriceTechSort      0x0086    //指标排序报价分析
  #define  PriceQiangRuoSort  0x0087    //强弱排序报价分析
  #define  PriceJieDuanSort   0x0088    //阶段排序报价分析
  #define  PriceSelStock      0x0089    //条件选股报价分析
  #define  PriceBlockStock    0x0090    //热门板块报价分析

  #define  PriceAnalisys5     0x0099    //报价分析5

  #define  PA_BEGIN			  PriceAnalisys1
  #define  PA_END			  PriceAnalisys5
///////////////////////////////////////////////////////////////////////  
  
  #define  MultiStock         0x0100    	//多股同列
  #define  SpecialReport      0x0110    	//特别报道

  #define  StockChoose        0x0120    	//股票选择                  
//各个窗口类中标记的定义 
//给m_nInPageTag的值
// 31          23                15         7              0
// ---------------------------------------------------------
// |    保留    |   技术指标索引值  | 证券类型  |  窗口内部标志 |
// ---------------------------------------------------------
////////////////////////////////////////////////////////////////////
//证券类型的定义
////////////////////////////////////////////////////////////////////
//m_nInPageTag的值
 #define PT_LIST		  		0x8000		//股票列表
 #define PT_MULITSEL		  	0x4000		//多选
 #define PT_TYPE		  		0x0100		//分类股，列表时HSMarketDataType指明类型
 #define PT_BLOCK	  			0x0200		//板块股，列表时CODE指明板块代码
 #define PT_USERDEFINE 			0x0300		//自选股
 #define PT_INDEX				0x0400		//指数类
 #define PT_GOOD				0x0500		//商品顺序
 #define PT_TCHTYPE				0x0600		//技术指标
 #define PT_CONDITION			0x0700		//条件选股
 #define PT_USRBLOCK			0x0800		//自选股/板块股
 #define PT_ALLUSRTYPE			0x0900		//所有分类
 #define PT_CW_GBQX				0x0A00		//财务-股本权息
 #define PT_CW_CWSJ				0x0B00		//财务-财务数据
 #define PT_CW_CWZB				0x0C00		//财务-财务指标
 #define PT_CW_JJZB				0x0D00		//财务-基金周报v
 #define PT_BLOCK_INDEX			0x0E00		//板块指数	add by lxqi 20090702
 #define PT_USER_BLOCK_INDEX	0x0F00		//扩展板块指数	add by lxqi 20090702
/////////////////////////////////////////////////////////////// //
////////////////////////////////////////////////////////////////////
//个股技术分析大盘指数分析窗口分析周期标记的定义
////////////////////////////////////////////////////////////////////
 #define  AnalisysFor0             0x0000  //分笔成交
 #define  AnalisysFor1             0x0001  //1分钟
// gbq add 20060217
//#ifdef Support_DHJR
// #define  AnalisysFor3			   0x000A  //3分钟
//#endif 
// gbq add end;
 #define  AnalisysFor5             0x0002  //五分钟
 #define  AnalisysFor15            0x0003  //十五分钟
 #define  AnalisysFor30            0x0004  //三十分钟
 #define  AnalisysFor60            0x0005  //六十分钟
 #define  AnalisysForDay           0x0006  //日线
 #define  AnalisysForWeek          0x0007  //周线
 #define  AnalisysForMonth         0x0008  //月线
 #define  AnalisysForMoreDay       0x0009  //多日线
// gbq add 20060217, 撤销修改 20060220
 #define  AnalisysFor180           0x000A  //180分钟
/*
#ifdef Support_DHJR
 #define  AnalisysFor180           0x000B  //180分钟
#else
 #define  AnalisysFor180           0x000A  //180分钟
#endif
 */
// gbq end;

 #define  AnalisysBegin			   AnalisysFor1
// gbq add
//#ifdef Support_DHJR
// #define  AnalisysEnd         	   AnalisysFor3
//#else
// #define  AnalisysEnd         	   AnalisysForMoreDay
//#endif 
 #define  AnalisysEnd         	   AnalisysForMoreDay
// gbq end;

/////////////////////////////////////////////////////////////// //
//个股及时分析窗口的几种类型
/////////////////////////////////////////////////////////////// //
 #define  StockFenTime             0x0030  //分时走势
 #define  StockLiDao               0x0031  //买卖力道
 #define  StockLiangBi             0x0032  //量比指?
 #define  StockTotalHolding		   0x0033  //总持
 #define  StockETFTech			   0x0034  //etf指标

//特别报道的几种类别
/////////////////////////////////////////////////////////////// //
 
 #define  SpecialReport1          0x0040   //涨跌幅排名
 #define  SpecialReport2          0x0041   //成交加震幅排名
 #define  SpecialReport3          0x0042   //成交量排名
 #define  SpecialReport4          0x0043   //成交量变化排名
 #define  SpecialReport5          0x0044   //今日强势股
 #define  SpecialReport6          0x0045   //今日弱势股
 #define  SpecialReport7          0x0046   //今日适合做多股
 #define  SpecialReport8          0x0047   //今日适合做空股
 #define  SpecialReport9          0x0048   //今日资金流向排名
 #define  SpecialReport10         0x0049   //买卖量差排名
 #define  SpecialReport11         0x004A   //综合指标排名

 #define  SR_BEGIN				  SpecialReport1
 #define  SR_END				  SpecialReport11
/////////////////////////////////////////////////////////////// // 
//文本浏览窗口
/////////////////////////////////////////////////////////////// // 
 #define  StockMessage            0x0060  //个股基本资料
 #define  ShStockInfo             0x0061  //上海证交所信息
 #define  SzStockInfo             0x0062  //深证证交所信息
 #define  StockerInfo1            0x0063  //券商信息1
 #define  StockerInfo2            0x0064  //券商信息2
 #define  StockNews               0x0065  //财经信息
 #define  Notice	              0x0066  //紧急公告
 #define  GraphicFinance		  0x0067  //财务图示，杨朝惠添加
 #define  HtmlView				  0x0068  //浏览HTML页面		
///////////////////////////////////////////////////////////////////
// 江恩分析图
///////////////////////////////////////////////////////////////////
 #define GannSquare				  0x0070	//江恩正方
 #define GannHexagon			  0x0071	//江恩六角
// 高级资讯相关的几个页面
#define  TreeView				  0x00A0	//树视图
#define  GraphAnalyseView		  0x00A1	//财务图形分析视图
#define  FinancialTableiew		  0x00A2	//财务分析报表视图
#define  InfoTextView			  0x00A3	//文本相关视图

// 股票类型
#ifndef SC_StockZ
#define SC_StockZ			0x04 // 债券
#endif
#ifndef SC_StockJ
#define SC_StockJ			0x05 // 基金　
#endif

// by spec 2003.04.16
//#define Yls_Split_Status    0x1000 // 拆分状态

/////////////////////////////////////////////////////////////// // 
#endif // _PAGEID_H_
