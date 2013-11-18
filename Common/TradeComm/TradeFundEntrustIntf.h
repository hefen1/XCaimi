/*******************************************************
  源程序名称:TradeFundEntrustIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  基金委托接口定义
  作    者:  shenglq
  开发日期:  20110216
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqFundEntrust : virtual public ITrading
{
	//基金公司
	virtual void SetFundCompany(const CString sValue) = 0;
	//基金代码
	virtual void SetFundCode(const CString sValue) = 0;
	//委托数量
	virtual void SetAmount(const double Value) = 0;
	//委托金额
	virtual void SetBalance(const double Value) = 0;
	//收费方式
	virtual void SetChargeType(const CString sValue) = 0;
	//巨额赎回
	virtual void SetExceedFlag(const CString sValue) = 0;
	//申购日期
	virtual void SetAllotDate(const CString sValue) = 0;
	//申购编号
	virtual void SetAllotNo(const CString sValue) = 0;
	//委托编号
	virtual void SetEntrustNo(const CString sValue) = 0;
	//分红标志
	virtual void SetDividendMethod(const CString sValue) = 0;
	//基金帐号
	virtual void SetStockAccount(const CString sValue) = 0;
	//转换代码
	virtual void SetTransCode(const CString sValue) = 0;
	//转换帐号
	virtual void SetTransAccount(const CString sValue) = 0;
	//转换数量
	virtual void SetTransAmount(const double Value) = 0;
	//客户风险等级
	virtual void SetClientRiskLevel(const CString sValue) = 0;
	//预约日期
	virtual void SetHopeDate(const CString sValue) = 0;
	//起始日期
	virtual void SetBeginDate(const CString sValue) = 0;
	//终止日期
	virtual void SetEndDate(const CString sValue) = 0;
	//扣款日
	virtual void SetFundDate(const CString sValue) = 0;
	//基金内码
	virtual void SetFundIntl(const CString sValue) = 0;
};