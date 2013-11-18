/*******************************************************
  源程序名称:BankStockTransferIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  银证转帐接口定义
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqBankStockTransfer : virtual public ITrading
{
	//币种
	virtual void SetMoneyType(const EMoneyType mtValue) = 0;
	//银行代码
	virtual void SetBankNo(const CString sValue) = 0;
	//银行密码
	virtual void SetBankPassword(const CString sValue) = 0;
	//资金密码
	virtual void SetFundPassword(const CString sValue) = 0;
	//转帐方向
	virtual void SetTransferDirection(const CString sValue) = 0;
	//转帐金额
	virtual void SetOccurBalance(const double Value) = 0;
};