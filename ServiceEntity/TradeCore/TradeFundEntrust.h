/*******************************************************
  源程序名称:TradeFundEntrust.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  基金委托接口定义
  作    者:  shenglq
  开发日期:  20110216
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeFundEntrustIntf.h"
#pragma warning (disable:4250)

class CReqFundEntrust : public CTrading, public IReqFundEntrust
{
public:	
	//基金公司
	void SetFundCompany(const CString sValue);
	//基金代码
	void SetFundCode(const CString sValue);
	//委托数量
	void SetAmount(const double Value);
	//委托金额
	void SetBalance(const double Value);
	//收费方式
	void SetChargeType(const CString sValue);
	//巨额赎回
	void SetExceedFlag(const CString sValue);
	//申购日期
	void SetAllotDate(const CString sValue);
	//申购编号
	void SetAllotNo(const CString sValue);
	//委托编号
	void SetEntrustNo(const CString sValue);
	//分红标志
	void SetDividendMethod(const CString sValue);
	//基金帐号
	void SetStockAccount(const CString sValue);
	//转换代码
	void SetTransCode(const CString sValue);
	//转换帐号
	void SetTransAccount(const CString sValue);
	//转换数量
	void SetTransAmount(const double Value);
	//客户风险等级
	void SetClientRiskLevel(const CString sValue);
	//预约日期
	void SetHopeDate(const CString sValue);
	//起始日期
	void SetBeginDate(const CString sValue);
	//终止日期
	void SetEndDate(const CString sValue);
	//扣款日
	void SetFundDate(const CString sValue);
	//基金内码
	void SetFundIntl(const CString sValue);
};