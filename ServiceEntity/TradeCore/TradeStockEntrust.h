/*******************************************************
  源程序名称:TradeStockEntrust.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  证券委托类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeStockEntrustIntf.h"
#pragma warning (disable:4250)

class CReqStockEntrust : public CTrading, public IReqStockEntrust
{
public:	
	//市场
	void SetExchangeType(const CString sValue);
	//股东账号
	void SetStockAccount(const CString sValue);
	//证券代码
	void SetStockCode(const CString sValue);
	//买卖方向
	void SetEntrustBs(const EBSDirection Value);
	//委托数量
	void SetEntrustAmount(const int Value);
	//委托价格
	//void SetEntrustPrice(const COleCurrency Value);
	void SetEntrustPrice(const double Value);
	//委托属性
	void SetEntrustProp(const CString sValue);
	//对方席位
	void SetOppoSeatNo(const CString sValue);
	//协议编号
	void SetConferNo(const CString sValue);
	//转换代码
	void SetTransCode(const CString sValue);
	//委托类型
	void SetEntrustType(const CString sValue);
	//成分股代码
	void SetComponentCode(const CString sValue);
};