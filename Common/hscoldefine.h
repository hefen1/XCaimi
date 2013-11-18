/*******************************************************************************
* Copyright (c)2003, 恒生信息技术有限公司
* All rights reserved.
*
* 文件名称：HSColDefine.h
* 文件标识：表格相关列ID的定义文件
* 摘    要：本文件在C/S端通用
*
* 当前版本：大福星2003
* 作    者：俞礼石
* 完成日期：2003-07-30
*
* 取代版本：
* 原 作 者：俞礼石
* 完成日期：
* 备	 注：
*******************************************************************************/
#ifndef _HSCOLDEFINE_H_
#define _HSCOLDEFINE_H_

#define COLUMN_BEGIN						10000
#define COLUMN_END							(COLUMN_BEGIN + 999)
/* 基本行情 COLUMN_HQ_BASE_ DEFINE BEGIN */
#define	COLUMN_HQ_BASE_BEGIN				COLUMN_BEGIN
#define	COLUMN_HQ_BASE_END					(COLUMN_HQ_BASE_BEGIN + 100)

#define COLUMN_HQ_BASE_NAME					(COLUMN_HQ_BASE_BEGIN + 47)	 	// 股票名称 10047
#define COLUMN_HQ_BASE_OPEN					(COLUMN_HQ_BASE_BEGIN + 48)	 	// 开盘价格
#define COLUMN_HQ_BASE_NEW_PRICE			(COLUMN_HQ_BASE_BEGIN + 49)	 	// 成交价格
#define COLUMN_HQ_BASE_RISE_VALUE			(COLUMN_HQ_BASE_BEGIN + 50)	 	// 涨跌值
#define COLUMN_HQ_BASE_TOTAL_HAND			(COLUMN_HQ_BASE_BEGIN + 51)	 	// 总手
#define COLUMN_HQ_BASE_HAND					(COLUMN_HQ_BASE_BEGIN + 52)	 	// 现手
#define COLUMN_HQ_BASE_MAX_PRICE			(COLUMN_HQ_BASE_BEGIN + 53)	 	// 最高价格
#define COLUMN_HQ_BASE_MIN_PRICE 			(COLUMN_HQ_BASE_BEGIN + 54)	 	// 最低价格
#define COLUMN_HQ_BASE_BUY_PRICE			(COLUMN_HQ_BASE_BEGIN + 55)	 	// 买入价格
#define COLUMN_HQ_BASE_SELL_PRICE			(COLUMN_HQ_BASE_BEGIN + 56)	 	// 卖出价格
#define COLUMN_HQ_BASE_RISE_RATIO			(COLUMN_HQ_BASE_BEGIN + 57)	 	// 涨跌幅
#define COLUMN_HQ_BASE_CODE					(COLUMN_HQ_BASE_BEGIN + 58)	 	// 股票代码

#define COLUMN_HQ_BASE_PRECLOSE				(COLUMN_HQ_BASE_BEGIN + 59)    	// 昨收
#define COLUMN_HQ_BASE_VOLUME_RATIO	 		(COLUMN_HQ_BASE_BEGIN + 60)	 	// 量比
#define COLUMN_HQ_BASE_ORDER_BUY_PRICE 		(COLUMN_HQ_BASE_BEGIN + 61)	 	// 委买价
#define COLUMN_HQ_BASE_ORDER_BUY_VOLUME		(COLUMN_HQ_BASE_BEGIN + 62)	 	// 委买量 
#define COLUMN_HQ_BASE_ORDER_SELL_PRICE		(COLUMN_HQ_BASE_BEGIN + 63)    	// 委卖价
#define COLUMN_HQ_BASE_ORDER_SELL_VOLUME	(COLUMN_HQ_BASE_BEGIN + 64)    	// 委卖量 
#define COLUMN_HQ_BASE_IN_HANDS		 		(COLUMN_HQ_BASE_BEGIN + 65)    	// 内盘 
#define COLUMN_HQ_BASE_OUT_HANDS			(COLUMN_HQ_BASE_BEGIN + 66)    	// 外盘 
#define COLUMN_HQ_BASE_MONEY		 		(COLUMN_HQ_BASE_BEGIN + 67)    	// 成交金额 
#define COLUMN_HQ_BASE_RISE_SPEED	 		(COLUMN_HQ_BASE_BEGIN + 68)    	// 涨速（不用）
#define COLUMN_HQ_BASE_AVERAGE_PRICE   		(COLUMN_HQ_BASE_BEGIN + 69)    	// 均价 
#define COLUMN_HQ_BASE_RANGE	     		(COLUMN_HQ_BASE_BEGIN + 70)    	// 振幅
#define COLUMN_HQ_BASE_ORDER_RATIO     		(COLUMN_HQ_BASE_BEGIN + 71)    	// 委比
#define COLUMN_HQ_BASE_ORDER_DIFF     		(COLUMN_HQ_BASE_BEGIN + 72)    	// 委差
#define COLUMN_HQ_BASE_SPEEDUP				(COLUMN_HQ_BASE_BEGIN + 78) 	// 新的涨速
/* 基本行情 COLUMN_HQ_BASE_ DEFINE END */

/* 扩展行情 COLUMN_HQ_EX_ DEFINE BEGIN */
#define	COLUMN_HQ_EX_BEGIN					(COLUMN_HQ_BASE_END + 1)
#define	COLUMN_HQ_EX_END					(COLUMN_HQ_EX_BEGIN + 50)

#define COLUMN_HQ_EX_BUY_PRICE1				(COLUMN_HQ_EX_BEGIN + 1)		//买入价格一
#define COLUMN_HQ_EX_BUY_VOLUME1			(COLUMN_HQ_EX_BEGIN + 2)		//买入数量一
#define COLUMN_HQ_EX_BUY_PRICE2				(COLUMN_HQ_EX_BEGIN + 3)		//买入价格二
#define COLUMN_HQ_EX_BUY_VOLUME2			(COLUMN_HQ_EX_BEGIN + 4)		//买入数量二
#define COLUMN_HQ_EX_BUY_PRICE3				(COLUMN_HQ_EX_BEGIN + 5)		//买入价格三
#define COLUMN_HQ_EX_BUY_VOLUME3			(COLUMN_HQ_EX_BEGIN + 6)		//买入数量三
#define COLUMN_HQ_EX_BUY_PRICE4				(COLUMN_HQ_EX_BEGIN + 7)		//买入价格四
#define COLUMN_HQ_EX_BUY_VOLUME4			(COLUMN_HQ_EX_BEGIN + 8)		//买入数量四
#define COLUMN_HQ_EX_BUY_PRICE5				(COLUMN_HQ_EX_BEGIN + 9)		//买入价格五
#define COLUMN_HQ_EX_BUY_VOLUME5			(COLUMN_HQ_EX_BEGIN + 10)		//买入数量五

#define COLUMN_HQ_EX_SELL_PRICE1			(COLUMN_HQ_EX_BEGIN + 11)		//卖出价格一
#define COLUMN_HQ_EX_SELL_VOLUME1			(COLUMN_HQ_EX_BEGIN + 12)		//卖出数量一
#define COLUMN_HQ_EX_SELL_PRICE2			(COLUMN_HQ_EX_BEGIN + 13)		//卖出价格二
#define COLUMN_HQ_EX_SELL_VOLUME2			(COLUMN_HQ_EX_BEGIN + 14)		//卖出数量二
#define COLUMN_HQ_EX_SELL_PRICE3			(COLUMN_HQ_EX_BEGIN + 15)		//卖出价格三
#define COLUMN_HQ_EX_SELL_VOLUME3			(COLUMN_HQ_EX_BEGIN + 16)		//卖出数量三
#define COLUMN_HQ_EX_SELL_PRICE4			(COLUMN_HQ_EX_BEGIN + 17)		//卖出价格四
#define COLUMN_HQ_EX_SELL_VOLUME4			(COLUMN_HQ_EX_BEGIN + 18)		//卖出数量四
#define COLUMN_HQ_EX_SELL_PRICE5			(COLUMN_HQ_EX_BEGIN + 19)		//卖出价格五
#define COLUMN_HQ_EX_SELL_VOLUME5			(COLUMN_HQ_EX_BEGIN + 20)		//卖出数量五

#define COLUMN_HQ_EX_EXHAND_RATIO			(COLUMN_HQ_EX_BEGIN + 21)		//换手率 
#define COLUMN_HQ_EX_5DAY_AVGVOLUME			(COLUMN_HQ_EX_BEGIN + 22)		//5日平均量 
#define COLUMN_HQ_EX_PE_RATIO  				(COLUMN_HQ_EX_BEGIN + 23)		//市盈率 
#define COLUMN_HQ_EX_DIRECTION   			(COLUMN_HQ_EX_BEGIN + 24)       //成交方向
#define COLUMN_HQ_EX_FUND_NETVALUE 			(COLUMN_HQ_EX_BEGIN + 25)       //基金净值
#define COLUMN_HQ_EX_BOND_ACCRUAL 			(COLUMN_HQ_EX_BEGIN + 26)       //国债利息
/* 扩展行情 COLUMN_HQ_EX_ DEFINE END */

/* 期货行情 COLUMN_FUTURES_ DEFINE BEGIN*/
#define COLUMN_FUTURES_BEGIN				(COLUMN_HQ_EX_END + 1)
#define COLUMN_FUTURES_END					(COLUMN_FUTURES_BEGIN + 50)

#define COLUMN_FUTURES_CODE			   		(COLUMN_FUTURES_BEGIN + 0)	  // 商品代码
#define COLUMN_FUTURES_NAME			   		(COLUMN_FUTURES_BEGIN + 1)    // 商品名称
#define COLUMN_FUTURES_OPEN			   		(COLUMN_FUTURES_BEGIN + 2)    // 开盘
#define COLUMN_FUTURES_PRECLOSE		   		(COLUMN_FUTURES_BEGIN + 3)    // 昨收盘
#define COLUMN_FUTURES_HIGH			   		(COLUMN_FUTURES_BEGIN + 4)    // 最高
#define COLUMN_FUTURES_LOW			   		(COLUMN_FUTURES_BEGIN + 5)    // 最低
#define COLUMN_FUTURES_BID1_PRICE	   		(COLUMN_FUTURES_BEGIN + 6)    // 买1价
#define COLUMN_FUTURES_ASK1_PRICE	   		(COLUMN_FUTURES_BEGIN + 7)    // 卖1价
#define COLUMN_FUTURES_BID1_VOL		   		(COLUMN_FUTURES_BEGIN + 8)    // 买1量
#define COLUMN_FUTURES_ASK1_VOL		   		(COLUMN_FUTURES_BEGIN + 9 )   // 卖1量

#define COLUMN_FUTURES_NEW_PRICE	   		(COLUMN_FUTURES_BEGIN + 18)   // 最新价
#define COLUMN_FUTURES_VOLUME		   		(COLUMN_FUTURES_BEGIN + 19)   // 成交量
#define COLUMN_FUTURES_AMOUNT		   		(COLUMN_FUTURES_BEGIN + 20)   // 持仓量
#define COLUMN_FUTURES_SETTLE		   		(COLUMN_FUTURES_BEGIN + 21)   // 结算价
#define COLUMN_FUTURES_MARKET		   		(COLUMN_FUTURES_BEGIN + 22)   // 市场
#define COLUMN_FUTURES_NEW_VOL				(COLUMN_FUTURES_BEGIN + 23)   // 现量
#define COLUMN_FUTURES_TOTAL_AMOUNT	    	(COLUMN_FUTURES_BEGIN + 24)   // 总持仓
#define COLUMN_FUTURES_RISE_VALUE			(COLUMN_FUTURES_BEGIN + 25)   // 涨跌值
#define COLUMN_FUTURES_ORDER_RATIO			(COLUMN_FUTURES_BEGIN + 26)   // 涨跌幅
#define COLUMN_FUTURES_PRESETTLE			(COLUMN_FUTURES_BEGIN + 27)   // 昨结算
#define COLUMN_FUTURES_Current_SETTLE		(COLUMN_FUTURES_BEGIN + 28)   // 现结算
#define COLUMN_FUTURES_IO_SUB				(COLUMN_FUTURES_BEGIN + 29)   // IO差
#define COLUMN_FUTURES_AMOUNT_SUB		    (COLUMN_FUTURES_BEGIN + 30)   //持仓差
#define COLUMN_FUTURES_OPEN_POSITION	    (COLUMN_FUTURES_BEGIN + 31)   //现开仓
#define COLUMN_FUTURES_CLEAR_POSITION	    (COLUMN_FUTURES_BEGIN + 32)   //现平仓
#define COLUMN_FUTURES_PRESETTLE_PRICE		(COLUMN_FUTURES_BEGIN + 33)   //前结价

#define COLUMN_FUTURES_HIS_HIGH				(COLUMN_FUTURES_BEGIN + 34)	  //	"史最高"
#define COLUMN_FUTURES_HIS_LOW				(COLUMN_FUTURES_BEGIN + 35)	  //	"史最低"
#define COLUMN_FUTURES_UPPER_LIM			(COLUMN_FUTURES_BEGIN + 36)	  //	"涨停板"
#define COLUMN_FUTURES_LOWER_LIM			(COLUMN_FUTURES_BEGIN + 37)	  //	"跌停板"
#define COLUMN_FUTURES_RISE_VALUE2			(COLUMN_FUTURES_BEGIN + 38)	  //	"涨跌值2"
#define COLUMN_FUTURES_ORDER_RATIO2			(COLUMN_FUTURES_BEGIN + 39)	  //	"涨跌幅2"

/* 期货行情 COLUMN_FUTURES_ DEFINE END*/

/* 财务报表分析(基本面分析)相关的数据项定义 BEGIN */
#define COLUMN_FINANCIAL_STATEMENT_BEGIN		50000//30000
#define COLUMN_FINANCIAL_STATEMENT_END			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 999)

/*	股本类	*/
#define COLUMN_FINANCIAL_STATEMENT_ZGB			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 0)	//总股本		
#define COLUMN_FINANCIAL_STATEMENT_GJG			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 1)	//国家股		
#define COLUMN_FINANCIAL_STATEMENT_FQRG			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 2)	//发起人股		
#define COLUMN_FINANCIAL_STATEMENT_FRG			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 3)	//法人股		
#define COLUMN_FINANCIAL_STATEMENT_NBZGG		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 4)	//内部职工股		
#define COLUMN_FINANCIAL_STATEMENT_LTAG			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 5)	//流通A股		
#define COLUMN_FINANCIAL_STATEMENT_ZPG			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 6)	//转配股		
#define COLUMN_FINANCIAL_STATEMENT_HG			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 7)	//H股		
#define COLUMN_FINANCIAL_STATEMENT_BG			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 8)	//B股		
#define COLUMN_FINANCIAL_STATEMENT_YXG			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 9)	//优先股		

/*	财务数据大分类 - 资产负债表部分 */
#define COLUMN_FINANCIAL_STATEMENT_HBZJ			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 10)	//货币资金			现金比率(指所涉及到的计算项,下同)
#define COLUMN_FINANCIAL_STATEMENT_DQTZJE		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 11)	//短期投资净额		现金比率
#define COLUMN_FINANCIAL_STATEMENT_YSZKJE		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 12)	//应收帐款净额		应收帐款周转率
#define COLUMN_FINANCIAL_STATEMENT_YSZK			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 13)	//应收帐款		
#define COLUMN_FINANCIAL_STATEMENT_YSPJ			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 14)	//应收票据		
#define COLUMN_FINANCIAL_STATEMENT_YSGL			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 15)	//应收股利		
#define COLUMN_FINANCIAL_STATEMENT_YSLX			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 16)	//应收利息		
#define COLUMN_FINANCIAL_STATEMENT_CHJE			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 17)	//存货净额		速动比率
#define COLUMN_FINANCIAL_STATEMENT_LDZC			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 18)	//流动资产		流动比率，速动比率，营运资本，流动资产周转率
#define COLUMN_FINANCIAL_STATEMENT_CQTZJE		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 19)	//长期投资净额	长期资产适合率

#define COLUMN_FINANCIAL_STATEMENT_GDZCHJ		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 20)	//固定资产合计		
#define COLUMN_FINANCIAL_STATEMENT_GDZCJZ		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 21)	//固定资产净值	长期资产适合率
#define COLUMN_FINANCIAL_STATEMENT_WXZC			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 22)	//无形资产		债务与有形净值比率
#define COLUMN_FINANCIAL_STATEMENT_WXJQTZC		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 23)	//无形及其他资产		
#define COLUMN_FINANCIAL_STATEMENT_ZZC			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 24)	//总资产		总资产周转率
#define COLUMN_FINANCIAL_STATEMENT_DQJK			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 25)	//短期借款		
#define COLUMN_FINANCIAL_STATEMENT_YFZK			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 26)	//应付帐款		
#define COLUMN_FINANCIAL_STATEMENT_LDFZ			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 27)	//流动负债		负债结构比率
#define COLUMN_FINANCIAL_STATEMENT_CQFZ			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 28)	//长期负债		负债结构比率，长期负债权益比率，长期资产适合率
#define COLUMN_FINANCIAL_STATEMENT_FZHJ			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 29)	//负债合计		

#define COLUMN_FINANCIAL_STATEMENT_SSGDQY		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 30)	//少数股东权益		
#define COLUMN_FINANCIAL_STATEMENT_GDQYHJ		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 31)	//股东权益合计		长期资产适合率，债务与有形净值比率
#define COLUMN_FINANCIAL_STATEMENT_GB			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 32)	//股本		
#define COLUMN_FINANCIAL_STATEMENT_ZBGJ			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 33)	//资本公积		
#define COLUMN_FINANCIAL_STATEMENT_YYGJ			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 34)	//盈余公积		
#define COLUMN_FINANCIAL_STATEMENT_WFPLR		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 35)	//未分配利润		
#define COLUMN_FINANCIAL_STATEMENT_FZHGDQYZJ	(COLUMN_FINANCIAL_STATEMENT_BEGIN + 36)	//负债和股东权益总计		

/*	财务数据大分类 - 利润及利润分配部分 */
#define COLUMN_FINANCIAL_STATEMENT_ZYYWSR		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 37)	//主营业务收入		
#define COLUMN_FINANCIAL_STATEMENT_ZYYWSRJE		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 38)	//主营业务收入净额		
#define COLUMN_FINANCIAL_STATEMENT_ZYYWCB		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 39)	//主营业务成本		

#define COLUMN_FINANCIAL_STATEMENT_ZYYWLR		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 40)	//主营业务利润
#define COLUMN_FINANCIAL_STATEMENT_QTYWLR		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 41)	//其它业务利润
#define COLUMN_FINANCIAL_STATEMENT_BTSR			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 42)	//补贴收入
#define COLUMN_FINANCIAL_STATEMENT_YYWSR		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 43)	//营业外收入
#define COLUMN_FINANCIAL_STATEMENT_YYWZC		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 44)	//营业外支出
#define COLUMN_FINANCIAL_STATEMENT_YYFY			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 45)//营业费用
#define COLUMN_FINANCIAL_STATEMENT_GLFY			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 46)//管理费用
#define COLUMN_FINANCIAL_STATEMENT_CWFY			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 47)//财务费用
#define COLUMN_FINANCIAL_STATEMENT_YYLR			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 48)//营业利润
#define COLUMN_FINANCIAL_STATEMENT_TZSY			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 49)//投资收益

#define COLUMN_FINANCIAL_STATEMENT_LRZE			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 50)//利润总额
#define COLUMN_FINANCIAL_STATEMENT_SDS			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 51)	//所得税
#define COLUMN_FINANCIAL_STATEMENT_JLR			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 52)	//净利润
#define COLUMN_FINANCIAL_STATEMENT_KGFPDLR		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 53)//可分配利润
#define COLUMN_FINANCIAL_STATEMENT_KCFJCXSYHDJLR	(COLUMN_FINANCIAL_STATEMENT_BEGIN + 54)//扣除经常性损益后的净利润

/*	财务数据大分类 - 现金流量部分	 */
#define COLUMN_FINANCIAL_STATEMENT_JYXJLRXJ		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 55)//经营活动现金收入
#define COLUMN_FINANCIAL_STATEMENT_JYXJLCXJ		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 56)//经营活动现金流出
#define COLUMN_FINANCIAL_STATEMENT_JYHDXJJE		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 57)//经营活动现金净额
#define COLUMN_FINANCIAL_STATEMENT_TZXJLRXJ		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 58)//投资现金流入
#define COLUMN_FINANCIAL_STATEMENT_TZXJLCXJ		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 59)//投资现金流出

#define COLUMN_FINANCIAL_STATEMENT_TZXJJE		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 60)	//投资现金净额
#define COLUMN_FINANCIAL_STATEMENT_CZXJLRXJ		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 61)//筹措现金流入
#define COLUMN_FINANCIAL_STATEMENT_CZXJLCXJ		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 62)//筹措现金流出
#define COLUMN_FINANCIAL_STATEMENT_CZXJJE	 	(COLUMN_FINANCIAL_STATEMENT_BEGIN + 63)//筹措现金净额
#define COLUMN_FINANCIAL_STATEMENT_XJJDJWJZJE	(COLUMN_FINANCIAL_STATEMENT_BEGIN + 64)//现金及现金等价物净增额
#define COLUMN_FINANCIAL_STATEMENT_XSSPSDDXJ	(COLUMN_FINANCIAL_STATEMENT_BEGIN + 65)//销售商品收到的现金

/*	财务数据大分类 - 应收帐款部分 */
#define COLUMN_FINANCIAL_STATEMENT_YSZKZE			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 66)//应收帐款总额
#define COLUMN_FINANCIAL_STATEMENT_HZZB				(COLUMN_FINANCIAL_STATEMENT_BEGIN + 67)//坏帐准备
#define COLUMN_FINANCIAL_STATEMENT_YSZKJEZZCZEBL	(COLUMN_FINANCIAL_STATEMENT_BEGIN + 68)//应收帐款净额占资产总额比例
#define COLUMN_FINANCIAL_STATEMENT_YSZK1			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 69)//1年内应收帐款

#define COLUMN_FINANCIAL_STATEMENT_YSZK2		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 70)//1到2年应收帐款
#define COLUMN_FINANCIAL_STATEMENT_YSZK3		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 71)//2到3年应收帐款
#define COLUMN_FINANCIAL_STATEMENT_YSZK4		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 72)//3年以上应收帐款
#define COLUMN_FINANCIAL_STATEMENT_QTYSZKZE		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 73)//其他应收帐款总额
#define COLUMN_FINANCIAL_STATEMENT_QTYSK1		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 74)//1年内其他应收款
#define COLUMN_FINANCIAL_STATEMENT_QTYSK2		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 75)//1到2年其他应收款
#define COLUMN_FINANCIAL_STATEMENT_QTYSK3		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 76)//2到3年其他应收款
#define COLUMN_FINANCIAL_STATEMENT_QTYSK4		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 77)//3年以上其他应收款

/* 财务分析大类 - 每股指标部分*/
#define COLUMN_FINANCIAL_STATEMENT_MGSYKC		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 78)//每股收益扣除
#define COLUMN_FINANCIAL_STATEMENT_MGTBSY		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 79)//每股摊薄收益

#define COLUMN_FINANCIAL_STATEMENT_MGJQSY		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 80)//每股加权收益
#define COLUMN_FINANCIAL_STATEMENT_MGSY			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 81)//每股收益
#define COLUMN_FINANCIAL_STATEMENT_MGJZC		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 82)//每股净资产
#define COLUMN_FINANCIAL_STATEMENT_TZHMGJZC		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 83)//调整后每股净资产
#define COLUMN_FINANCIAL_STATEMENT_MGZBGJJ		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 84)//每股资本公积金
#define COLUMN_FINANCIAL_STATEMENT_MGWFPLR		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 85)//每股未分配利润
#define COLUMN_FINANCIAL_STATEMENT_MGXJJLL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 86)//每股现金净流量
#define COLUMN_FINANCIAL_STATEMENT_MGJYHDXJLL	(COLUMN_FINANCIAL_STATEMENT_BEGIN + 87)//每股经营活动产生的现金流量净额

/* 财务分析大类 - 计算部分 */
#define COLUMN_FINANCIAL_STATEMENT_XSMLL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 88)//毛利率--------------------获利能力
#define COLUMN_FINANCIAL_STATEMENT_ZYYWLRL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 89)//主营业务利润率

#define COLUMN_FINANCIAL_STATEMENT_XSJLL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 90)//净利率
#define COLUMN_FINANCIAL_STATEMENT_ZZCSYL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 91)//总资产收益率
#define COLUMN_FINANCIAL_STATEMENT_JZCSYL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 92)//净资产收益率
#define COLUMN_FINANCIAL_STATEMENT_JQJZCSYL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 93)//加权净资产收益率
#define COLUMN_FINANCIAL_STATEMENT_YSZKZZL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 94)//应收帐周转率--------------经营能力
#define COLUMN_FINANCIAL_STATEMENT_CHZZL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 95)//存货周转率
#define COLUMN_FINANCIAL_STATEMENT_GDZCZZL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 96)//固定资产周转率
#define COLUMN_FINANCIAL_STATEMENT_GDQYZZL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 97)//股东权益周转率
#define COLUMN_FINANCIAL_STATEMENT_ZZCZZL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 98)//总资产周转率
#define COLUMN_FINANCIAL_STATEMENT_LDBL			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 99)//流动比率------------------偿债能力

#define COLUMN_FINANCIAL_STATEMENT_SDBL			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 100)//速动比率
#define COLUMN_FINANCIAL_STATEMENT_LXBZBS		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 101)//利息保障倍数
#define COLUMN_FINANCIAL_STATEMENT_GDQYYGDZCBL	(COLUMN_FINANCIAL_STATEMENT_BEGIN + 102)//股东权益与固定资产比率
#define COLUMN_FINANCIAL_STATEMENT_CQFZYYYZJBL	(COLUMN_FINANCIAL_STATEMENT_BEGIN + 103)//长期负债与运营资金比率
#define COLUMN_FINANCIAL_STATEMENT_ZCFZBL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 104)//资产负债比率
#define COLUMN_FINANCIAL_STATEMENT_GDQYBL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 105)//股东权益比率
#define COLUMN_FINANCIAL_STATEMENT_GDZCBL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 106)//固定资产比率
#define COLUMN_FINANCIAL_STATEMENT_ZYYWSRZZL	(COLUMN_FINANCIAL_STATEMENT_BEGIN + 107)//主营业务收入增长率--------发展能力
#define COLUMN_FINANCIAL_STATEMENT_YYLRZZL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 108)//营业利润增长率
#define COLUMN_FINANCIAL_STATEMENT_SHLRZZL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 109)//税后利润增长率

#define COLUMN_FINANCIAL_STATEMENT_JZCZZL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 110)//净资产增长率
#define COLUMN_FINANCIAL_STATEMENT_LLZEZZL		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 111)//利润总额增长率
#define COLUMN_FINANCIAL_STATEMENT_ZZCZZHANGL	(COLUMN_FINANCIAL_STATEMENT_BEGIN + 112)//总资产增长率

#define COLUMN_FINANCIAL_STATEMENT_ZGDS			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 113)//总股东数
#define COLUMN_FINANCIAL_STATEMENT_LJZJ			(COLUMN_FINANCIAL_STATEMENT_BEGIN + 114)//累计折旧
#define COLUMN_FINANCIAL_STATEMENT_YLZD1		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 115)//预留字段2
#define COLUMN_FINANCIAL_STATEMENT_YLZD2		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 116)//预留字段3
#define COLUMN_FINANCIAL_STATEMENT_YLZD3		(COLUMN_FINANCIAL_STATEMENT_BEGIN + 117)//预留字段4
/* 财务报表分析(基本面分析)相关的数据项定义 END */

/* 数据结束标识 如果数据有增加或改动, 请保持此定义同步修改*/
#define COLUMN_FINANCIAL_STATEMENT_DATA_END		COLUMN_FINANCIAL_STATEMENT_YLZD3

/* 用于标示其它信息的定义 BEGIN */
#define COLUMN_FINANCIAL_STATEMENT_FSRQ		(COLUMN_FINANCIAL_STATEMENT_DATA_END + 1) // 发生日期
/* 用于标示其它信息的定义 END */
#endif // _TABLEHEADSTRUCT_H_
