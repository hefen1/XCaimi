/*******************************************************
  源程序名称:TradeStockEntrustIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  证券委托接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqStockEntrust : virtual public ITrading
{
	//市场
	virtual void SetExchangeType(const CString sValue) = 0;
	//股东账号
	virtual void SetStockAccount(const CString sValue) = 0;
	//证券代码
	virtual void SetStockCode(const CString sValue) = 0;
	//买卖方向
	virtual void SetEntrustBs(const EBSDirection Value) =0;
	//委托数量
	virtual void SetEntrustAmount(const int Value) = 0;
	//委托价格
	//virtual void SetEntrustPrice(const COleCurrency Value) = 0;
	virtual void SetEntrustPrice(const double Value) = 0;
	//委托属性
	virtual void SetEntrustProp(const CString sValue) = 0;
	//对方席位
	virtual void SetOppoSeatNo(const CString sValue) = 0;
	//协议编号
	virtual void SetConferNo(const CString sValue) = 0;
	//转换代码
	virtual void SetTransCode(const CString sValue) = 0;
	//委托类型
	virtual void SetEntrustType(const CString sValue) = 0;
	//成分股代码
	virtual void SetComponentCode(const CString sValue) = 0;
};