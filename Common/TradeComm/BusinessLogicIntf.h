/*******************************************************
  源程序名称:BusinesslogicIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易业务逻辑
  			1、委托接口定义
  			2、查询接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "QueryStockAccountIntf.h"
#include "QueryFundInfoIntf.h"
#include "QueryBankIntf.h"
#include "QueryUserInfoIntf.h"
#include "QueryStockFundInfoIntf.h"
#include "QueryFundStockAccountIntf.h"
#include "QueryFundCompanyIntf.h"

#define PASSWORD_TYPE_FUND		"1"//资金密码
#define PASSWORD_TYPE_TRADE		"2"//交易密码
#define PASSWORD_TYPE_COMM		"3"//通讯密码

struct IBusinessLogic
{
	//释放
	virtual void Release() = 0;

	virtual void SetAccount(IAccount* lpAccount) = 0;
  
	/*获取股东账号
	*@param   sMsg: [out]查询信息
	*@return : 股东账号列表
	*/
	virtual IStockAccountList * GetStockAccount(CString& sMsg) = 0;

	/*证券委托
	*@param   sExchangeType:市场
	*@param   sStockAccount:股东账号
	*@param   sStockCode:证券代码
	*@param   eEntrustBs:买卖方向
	*@param   dEntrustPrice:委托价格
	*@param   iEntrustAmount:委托数量
	*@param   sEntrustProp:委托属性
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL StockEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, CString& sMsg) = 0;

	/*证券撤单
	*@param   sExchangeType:市场
	*@param   sStockAccount:股东账号
	*@param   sEntrustNo:委托编号
	*@param   sMsg: [out]撤单信息
	*@return : 是否成功
	*/
	virtual BOOL StockWithDraw(const CString sExchangeType, const CString sStockAccount, const CString sEntrustNo, 
		CString& sMsg) = 0;	

	/*数据同步
	*@param   dDataType:需要同步的数据
	*@param   sMsg: [out]撤单信息
	*@return : 是否成功
	*/
	//virtual BOOL SyncCacheData(const DWORD dDataType, CString& sMsg) = 0;

//////////////////////////////////////////////////////////////////////////
	
	/*获取基金信息
	*@param   sMsg: [out]查询信息
	*@return : 基金信息列表
	*/
	virtual IFundInfoList * GetFundInfo(CString& sMsg) = 0;	

	/*获取基金账户
	*@param   sMsg: [out]查询信息
	*@return : 基金账户列表
	*/
	virtual IFundStockAccountList * GetFundStockAccount(CString& sMsg, BOOL bQueryFlag = FALSE) = 0;	

	/*获取基金公司
	*@param   sMsg: [out]查询信息
	*@return : 基金公司列表
	*/
	virtual IFundCompanyList * GetFundCompany(CString& sMsg) = 0;	
	
	/*基金认购
	*@param   sFundCompany:基金公司
	*@param   sFundCode:基金代码
	*@param   dBalance:认购金额
	*@param   sChargeType:收费方式
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL FundSubcribe(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, CString& sMsg) = 0;

	/*基金申购
	*@param   sFundCompany:基金公司
	*@param   sFundCode:基金代码
	*@param   dBalance:认购金额
	*@param   sChargeType:收费方式
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL FundAllot(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, CString& sMsg) = 0;

	/*基金赎回
	*@param   sFundCompany:基金公司
	*@param   sFundCode:基金代码
	*@param   dAmount:赎回数量
	*@param   sAllotDate:申购日期
	*@param   sAllotNo:申购编号
	*@param   sExceedFlag:巨额赎回标志
	*@param   sChargeType:收费方式
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL FundRedeem(const CString sFundCompany, const CString sFundCode, const double dAmount, 
		const CString sAllotDate, const CString sAllotNo, const CString sExceedFlag, const CString sChargeType, CString& sMsg) = 0;

	/*基金撤单
	*@param   sfundcompany:基金公司
	*@param   sfundcode:基金代码
	*@param   sentrustno:委托编号
	*@param   sallotdate:申购日期
	*@param   sallotno:申购编号
	*@param   smsg: [out]撤单信息
	*@return : 是否成功
	*/
	virtual BOOL FundWithDraw(const CString sFundCompany, const CString sFundCode, const CString sEntrustNo, 
		const CString sAllotDate, const CString sAllotNo,CString& sMsg) = 0;	

	/*分红设置
	*@param   sFundCompany:基金公司
	*@param   sFundCode:基金代码
	*@param   sDividendMethod:分红标志
	*@param   sChargeType:收费方式
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL FundSetDividend(const CString sFundCompany, const CString sFundCode, const CString sDividendMethod, const CString sChargeType, CString& sMsg) = 0;

	/*基金转换
	*@param   sFundCompany:基金公司
	*@param   sFundCode:基金代码
	*@param   sTransCode:转换代码
	*@param   dTransAmount:转换数量
	*@param   sAllotDate:申购日期
	*@param   sAllotNo:申购编号
	*@param   sChargeType:收费方式
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL FundTrans(const CString sFundCompany, const CString sFundCode,const CString sTransCode, const double dTransAmount, 
		const CString sAllotDate, const CString sAllotNo, const CString sChargeType, CString& sMsg) = 0;

	/*设置风险等级
	*@param   sRiskLevel:风险等级
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL FundSetRiskLevel(const CString sRiskLevel, CString& sMsg) = 0;

	/*基金预约申购
	*@param   sFundCompany:基金公司
	*@param   sFundCode:基金代码
	*@param   dBalance:认购金额
	*@param   sChargeType:收费方式
	*@param   sHopeDate:预约日期
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL FundHopeAllot(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, const CString sHopeDate, CString& sMsg) = 0;

	/*基金预约赎回
	*@param   sFundCompany:基金公司
	*@param   sFundCode:基金代码
	*@param   dAmount:赎回数量
	*@param   sAllotDate:申购日期
	*@param   sAllotNo:申购编号
	*@param   sExceedFlag:巨额赎回标志
	*@param   sChargeType:收费方式
	*@param   sHopeDate:预约日期
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL FundHopeRedeem(const CString sFundCompany, const CString sFundCode, const double dAmount, 
		const CString sAllotDate, const CString sAllotNo, const CString sExceedFlag, const CString sChargeType, const CString sHopeDate, CString& sMsg) = 0;

	/*基金定投
	*@param   sFundCompany:基金公司
	*@param   sFundCode:基金代码
	*@param   dBalance:定投金额
	*@param   sChargeType:收费方式
	*@param   sBeginDate:起始日期
	*@param   sEndDate:终止日期
	*@param   sFundDate:月扣款日
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL Fund7432(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, const CString sBeginDate, const CString sEndDate, const CString sFundDate, CString& sMsg) = 0;


	/*获取银行信息
	*@param   sMsg: [out]查询信息
	*@return : 银行信息号列表
	*/
	virtual IBankList * GetBankInfo(CString& sMsg) = 0;	

	/*银证转帐
	*@param   mtValue:币种
	*@param   sBankNo:银行代码
	*@param   sBankPsw:银行密码
	*@param   sFundPsw:资金密码
	*@param   sDirection:转帐方向
	*@param   dBalance:发生金额
	*@return : 是否成功
	*/
	virtual BOOL BankStockTransfer(const EMoneyType mtValue, const CString sBankNo, const CString sBankPsw, 
		const CString sFundPsw, const CString sDirection, const double dBalance, CString& sMsg) = 0;
	
	/*修改密码
	*@param   sPasswordType:密码类型
	*@param   sOldPassword:原密码
	*@param   sNewPassword:新密码
	*@return : 是否成功
	*/
	virtual BOOL ChangePassword(const CString sPasswordType, const CString sOldPassword, const CString sNewPassword, CString& sMsg) = 0;

	/*重置成本价
	*@param   sExchangeType:市场
	*@param   sStockAccount:股东账号
	*@param   sStockCode:证券代码
	*@param   dCostPrice:成本价
	*@param   sSeat:席位
	*@param   dEntrustBalance:金额
	*@param   sRemark:备注
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL SetStockCostPrice(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const double dCostPrice, const CString sSeat, const double dEntrustBalance, const CString sRemark, CString& sMsg) = 0;

	/*获取用户信息
	*@param   sMsg: [out]查询信息
	*@return : 用户信息
	*/
	virtual IUserInfoItem * GetUserInfo(CString& sMsg, BOOL bQueryFlag = FALSE) = 0;	

	/*三板委托
	*@param   sExchangeType:市场
	*@param   sStockAccount:股东账号
	*@param   sStockCode:证券代码
	*@param   eEntrustBs:买卖方向
	*@param   dEntrustPrice:委托价格
	*@param   iEntrustAmount:委托数量
	*@param   sEntrustProp:委托属性
	*@param   sSeatNo:对方席位
	*@param   sConferNo:协议编号
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL StockSBEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, 
		const CString sSeatNo, const CString sConferNo, CString& sMsg) = 0;
	/*证券三板撤单
	*@param   sExchangeType:市场
	*@param   sStockAccount:股东账号
	*@param   sEntrustNo:委托编号
	*@param   sMsg: [out]撤单信息
	*@return : 是否成功
	*/
	virtual BOOL StockSBWithDraw(const CString sExchangeType, const CString sStockAccount, const CString sEntrustNo, 
		CString& sMsg) = 0;	

	/*基金盘后业务委托
	*@param   iAction:功能号
	*@param   sExchangeType:市场
	*@param   sStockAccount:股东账号
	*@param   sStockCode:证券代码
	*@param   iEntrustAmount:委托数量
	*@param   sTransCode:转换代码
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL StockFundEntrust(const int iAction, const CString sExchangeType, const CString sStockAccount, 
		const CString sStockCode, const int iEntrustAmount, const CString sTransCode, CString& sMsg) = 0;

	/*基金盘后业务撤单
	*@param   sExchangeType:市场
	*@param   sStockCode:证券代码
	*@param   sEntrustNo:委托编号
	*@param   sMsg: [out]撤单信息
	*@return : 是否成功
	*/
	virtual BOOL StockFundWithDraw(const CString sExchangeType, const CString sStockCode, const CString sEntrustNo, 
		CString& sMsg) = 0;	


	/*基金盘后业务基金信息
	*@param   sMsg: [out]查询信息
	*@return : 基金信息列表
	*/
	virtual IStockFundInfoList * GetStockFundInfo(CString& sMsg) = 0;

	/*融资融券委托
	*@param   sExchangeType:市场
	*@param   sStockAccount:股东账号
	*@param   sStockCode:证券代码
	*@param   eEntrustBs:买卖方向
	*@param   dEntrustPrice:委托价格
	*@param   iEntrustAmount:委托数量
	*@param   sEntrustProp:委托属性
	*@param   sEntrustType:委托类别
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL RZRQEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, 
		const CString sEntrustType, CString& sMsg) = 0;

	/*融资融券直接还款
	*@param   sMoneyType:币种
	*@param   dEntrustBalance:金额
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL RZRQZJHK(const CString sMoneyType, const double dEntrustBalance, CString& sMsg) = 0;

	/*融资融券直接还券
	*@param   sExchangeType:市场
	*@param   sStockAccount:股东账号
	*@param   sStockCode:证券代码
	*@param   iEntrustAmount:委托数量
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL RZRQZJHQ(const CString sExchangeType, const CString sStockAccount, const CString sStockCode,
		const int iEntrustAmount, CString& sMsg) = 0;

	/*ETF网下现金认购
	*@param   sExchangeType:市场
	*@param   sStockAccount:股东账号
	*@param   sStockCode:证券代码
	*@param   iEntrustAmount:委托数量
	*@param   sEntrustProp:委托属性
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL ETF9835(const CString sExchangeType, const CString sStockAccount, 
		const CString sStockCode, const int iEntrustAmount,const CString sEntrustProp, CString& sMsg) = 0;
	
	/*ETF网下股份换购
	*@param   sExchangeType:市场
	*@param   sStockAccount:股东账号
	*@param   sComponentCode:成份股代码
	*@param   eEntrustBs:买卖方向
	*@param   iEntrustAmount:委托数量
	*@param   sETFCode:ETF代码
	*@param   sMsg: [out]委托信息
	*@return : 是否成功
	*/
	virtual BOOL ETF9837(const CString sExchangeType, const CString sStockAccount, 
		const CString sComponentCode, const int iEntrustAmount, const CString sETFCode, const CString sEntrustProp, CString& sMsg) = 0;


};