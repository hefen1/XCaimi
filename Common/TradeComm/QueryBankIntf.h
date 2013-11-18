/*******************************************************
  源程序名称:QueryBankIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询银行接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "QueryBaseIntf.h"

struct IBankItem : virtual public IDataItem
{
	//币种
	virtual EMoneyType GetMoneyType() = 0;
	//银行代码
	virtual CString GetBankNo() = 0;
	//银行名称
	virtual CString GetBankName() = 0;
	//银行账号
	virtual CString GetBankAccount() = 0;
	virtual CString GetShowPassword() = 0;
	virtual CString GetBankPasswordRequire() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IBankList : virtual public IDataList
{
	virtual IBankItem * GetItems(const int i) = 0; 
	virtual IBankItem * NewItem() = 0;
};

//////////////////////////////////////////////////////////////////////////

struct IReqQueryBank : virtual public IReqQueryBase
{
	//币种
	virtual void SetMoneyType(const EMoneyType mtValue) = 0;
	//银行号
	virtual void SetBankNo(const CString sValue) = 0;
	virtual IBankList* GetDataList() = 0;
};