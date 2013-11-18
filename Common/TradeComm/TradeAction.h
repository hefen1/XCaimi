/*******************************************************
  源程序名称:TradeAction.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易功能号定义
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#pragma once
//公共功能
#define TRADE_HEART						60003//心跳
#define TRDADE_LOGIN					200 //登陆
#define TRADE_QUERYFUND					405 //查询资金
#define TRADE_QUERYBANK					452 //查询银行信息
#define TRADE_BANKTRANSFER				500 //银证转帐
#define TRADE_QUERYBANKBALANCE			502 //查询银行余额
#define TRADE_CHANGEPASSWORD			201 //修改密码
#define TRADE_QUERYUSERINFO				415 //查询用户信息
#define TRADE_MODIFYUSERINFO			206 //修改用户信息

//证券功能
#define TRADE_STOCK_QUERYMAXSELL		300 //最大可卖
#define TRADE_STOCK_QUERYMAXBUY			301 //最大可买
#define TRADE_STOCK_ENTRUST				302 //委托
#define TRADE_STOCK_WITHDRAW			304 //撤单
#define TRADE_STOCK_QUERYACCOUNT		407 //查询股东账号
#define TRADE_STOCK_QUERYENTRUST		401 //当日委托
#define TRADE_STOCK_303					303 //可撤委托
#define TRADE_STOCK_QUERYDEAL			402 //当日成交
#define TRADE_STOCK_QUERYHOLD			403 //持仓
#define TRADE_STOCK_SETCOSTPRICE		309 //重置成本价
#define TRADE_STOCK_SBENTRUST			332 //三板委托
#define TRADE_STOCK_QUERYSBENTRUST		333 //查询三板委托
#define TRADE_STOCK_SBWITHDRAW			334 //三板撤单
#define TRADE_STOCK_QUERYSBQUOTE		6177//查询协议行情
#define TRADE_STOCK_FUNDMERGE			7603//基金盘后业务合并
#define TRADE_STOCK_FUNDSPLIT			7604//基金盘后业务拆分
#define TRADE_STOCK_FUNDTRANS			7605//基金盘后业务转换
#define TRADE_STOCK_FUNDWITHDRAW		7607//基金盘后业务撤单
#define TRADE_STOCK_QUERYFUNDINFO		7609//查询基金盘后业务信息
#define TRADE_STOCK_QUERYFUNDENTRUST	7630//查询基金盘后业务当日委托
#define TRADE_STOCK_QUERYMERGESPLIT		7610//查询可合并拆分数量
#define	TRADE_STOCK_9835				9835//ETF网下现金认购
#define	TRADE_STOCK_9837				9837//ETF网下股份换购





//基金功能
#define TRADE_FUND_SUBSCRIBE			7401//基金认购
#define TRADE_FUND_ALLOT				7402//基金申购
#define TRADE_FUND_REDEEM				7403//基金赎回
#define TRADE_FUND_WITHDRAW				7404//基金撤单
#define TRADE_FUND_QUERYENTRUST			7410//当日委托
#define TRADE_FUND_QUERYHOLD			7411//查询份额
#define TRADE_FUND_QUERYFUNDINFO		7413//查询基金信息
#define TRADE_FUND_QUERYFUNDACCOUNT		7414//查询基金帐户
#define TRADE_FUND_TRANS				7405//基金转换
#define TRADE_FUND_SETDIVIDEND			7406//设置分红方式
#define TRADE_FUND_SETRISKLEVEL			7425//设置风险等级
#define TRADE_FUND_QUERYSTOCKACCOUNT	7414//基金账户查询
#define TRADE_FUND_QUERYFUNDCOMPANY		7417//基金公司查询
#define TRADE_FUND_OPEN					7418//基金开户
#define TRADE_FUND_HOPEALLOT			7408//基金预约申购
#define TRADE_FUND_HOPEREDEEM			7409//基金预约赎回
#define TRADE_FUND_7431					7431//基金定投查询
#define TRADE_FUND_7432					7432//基金定投
#define TRADE_FUND_7434					7434//基金定投撤单



//融资融券功能
#define  TRADE_RZRQ_QUERYMAXBUY			702//最大可买
#define  TRADE_RZRQ_QUERYMAXSELL		703//最大可卖
#define  TRADE_RZRQ_ENTRUST				704//委托
#define  TRADE_RZRQ_ZJHK				707//直接还款
#define  TRADE_RZRQ_ZJHQ				708//直接还券
#define  TRADE_RZRQ_QUERYDEBITTOTAL		705//负债汇总
#define  TRADE_RZRQ_QUERYRQAMOUNT		701//查询融券委托数据




