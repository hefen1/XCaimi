/*******************************************************
  源程序名称:QueryBank.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询银行类
  			1、请求类定义
  			2、返回类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "QueryBankIntf.h"
#include "QueryBase.h"
#pragma warning (disable:4250)

class CBankItem : public CDataItem, public IBankItem
{
public:	
	//币种
	EMoneyType GetMoneyType();
	//银行代码
	CString GetBankNo();
	//银行名称
	CString GetBankName();
	//银行账号
	CString GetBankAccount();
	CString GetShowPassword();
	CString GetBankPasswordRequire();
};
//////////////////////////////////////////////////////////////////////////

class CBankList : public CDataList, public IBankList
{
public:	
	IBankItem * GetItems(const int i); 
	IBankItem * NewItem();
};

//////////////////////////////////////////////////////////////////////////

class CReqQueryBank : public CReqQueryBase, virtual public IReqQueryBank
{
	
public:
	//币种
	void SetMoneyType(const EMoneyType mtValue);
	//银行号
	void SetBankNo(const CString sValue);
	IBankList* GetDataList();
protected:
	IBankList* NewDataList();
};