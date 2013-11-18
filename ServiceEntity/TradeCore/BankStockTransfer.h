/*******************************************************
  源程序名称:BankStockTransfer.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  银证转帐类定义
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#pragma once
#include "Trading.h"
#include "BankStockTransferIntf.h"
#pragma warning (disable:4250)

class CReqBankStockTransfer : public CTrading, public IReqBankStockTransfer
{
	
public:
	//币种
	void SetMoneyType(const EMoneyType mtValue);
	//银行代码
	void SetBankNo(const CString sValue);
	//银行密码
	void SetBankPassword(const CString sValue);
	//资金密码
	void SetFundPassword(const CString sValue);
	//转帐方向
	void SetTransferDirection(const CString sValue);
	//转帐金额
	void SetOccurBalance(const double Value);
};