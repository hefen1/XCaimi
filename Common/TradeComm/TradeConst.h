/*******************************************************
  源程序名称:TradeConst.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  常量定义
  作    者:  shenglq
  开发日期:  20100910
  *********************************************************/
#pragma once
//                人民币   美圆      港币
enum EMoneyType {mtCNY, mtUSD, mtHKD};
//			 左对齐    居中    右对齐
enum EAlign {alLeft, alCenter, alRight, alNone};

#define STOCKCODE_LENGTH				6 //证券代码长度
#define ENTRUST_MINPRICE				0.00001 //委托最小价格
#define FUNDCODE_LENGTH					6 //基金代码长度