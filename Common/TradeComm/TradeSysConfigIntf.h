/*******************************************************
  源程序名称:TradeSysConfigIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  配置中心
  功能说明:  交易系统配置
  作    者:  shenglq
  开发日期:  20101012
*********************************************************/
#pragma once
#include "TradeConst.h"
#include "XMLDataBaseIntf.h"
#include "tinyxml.h"

//市场类型
struct ITradeMarketItem : virtual public INodeData
{
	//获取显示名称
	virtual CString GetMarketName() = 0;
	virtual CString GetDispName() = 0;
	virtual CString GetAccountType() = 0;
	virtual EMoneyType GetMoneyType() = 0;
};

struct ITradeMarketList : virtual public INodeTreeData
{
	virtual ITradeMarketItem* GetItems(const int iIndex) = 0;
	virtual ITradeMarketItem* GetItemsByName(const CString sName) = 0;
};
//////////////////////////////////////////////////////////////////////////
//账号类型
struct ITradeAccountTypeItem : virtual public INodeData
{
	virtual CString GetAccountTypeName() = 0;
	virtual CString GetAccountType() = 0;
	virtual CString GetBackType() = 0;
	virtual CString GetShortName() = 0;
	virtual CString GetMarketType() = 0;
	virtual CString GetVisibleName() = 0;
};

struct ITradeAccountTypeList : virtual public INodeTreeData
{
	virtual ITradeAccountTypeItem* GetItems(const int iIndex) = 0;
	virtual ITradeAccountTypeItem* GetItemsByName(const CString sName) = 0;

	virtual CString GetBackTypeByAccountType(const CString sName) = 0;
	virtual CString GetMarketTypeByAccountType(const CString sName) = 0;
};

//////////////////////////////////////////////////////////////////////////
//可修改密码类型
struct IModifyPassowrdTypeItem : virtual public INodeData
{
	//获取显示名称
	virtual CString GetOldCharType() = 0;
	virtual int GetOldMaxLen() = 0;
	virtual int GetOldMinLen() = 0;
	virtual CString GetNewCharType() = 0;
	virtual int GetNewMaxLen() = 0;
	virtual int GetNewMinLen() = 0;
};

struct IModifyPassowrdTypeList : virtual public INodeTreeData
{
	virtual IModifyPassowrdTypeItem* GetItems(const int iIndex) = 0;
	virtual IModifyPassowrdTypeItem* GetItemsByName(const CString sName) = 0;
};

//////////////////////////////////////////////////////////////////////////
//基金风险问卷

struct IFundRiskQuestionItem : virtual public INodeData
{
	virtual CString GetCode() = 0;
	virtual CString GetTitle() = 0;
	virtual CString GetChoice() = 0;
	virtual CString GetChoiceText() = 0;
	virtual int GetChoiceScore() = 0;
	virtual CString GetRemark() = 0;
};
struct IFundRiskQuestionList : virtual public INodeTreeData
{
	virtual IFundRiskQuestionItem* GetItems(const int iIndex) = 0;
	virtual IFundRiskQuestionItem* GetItemsByName(const CString sName) = 0;
};


//公共参数
struct ICCommParamList: virtual public INodeTreeData
{
	virtual void SetNextParamList(ICCommParamList* pParamlist) = 0;
	virtual void SetCellAndAccount(const CString sCellID, const CString sAccount) = 0;
	//获取默认请求列表
	virtual INodeTreeData* GetDefaultReqDataList(CString sID = "") = 0;
	//获取账号类别
	virtual ITradeAccountTypeList* GetAccountTypeList(const CString sID = "") = 0;
	//根据节点名称获取NV列表
	virtual INodeTreeData* GetNVListByName(const CString sNodeName, const CString sID = "") = 0;
	//获取修改密码类型
	virtual IModifyPassowrdTypeList* GetModifyPassowrdTypeList() = 0;
	//获取市场类别
	virtual ITradeMarketList* GetMarketList() = 0;	
	//获取上海市价委托类型
	virtual INodeTreeData* GetSHSJEntrustTypeList() = 0;
	//获取深圳市价委托类型
	virtual INodeTreeData* GetSZSJEntrustTypeList() = 0;
	//获取巨额赎回
	virtual INodeTreeData* GetExceedFlagList() = 0;
	//获取收费方式
	virtual INodeTreeData* GetChargeTypeList() = 0;
	//获取分红方式
	virtual INodeTreeData* GetDividendMethodList() = 0;
	//获取币种类型
	virtual INodeTreeData* GetMoneyTypeList() = 0;
	//获取股票操作类别
	virtual INodeTreeData* GetStockEntrustType() = 0;
	//获取证件类型
	virtual INodeTreeData* GetIDKindList() = 0;
	//获取委托状态
	virtual INodeTreeData* GetEntrustStatusList() = 0;
	//获取基金风险测试问卷
	virtual IFundRiskQuestionList* GetFundRiskQuestionList() = 0;
	//获取用户风险等级
	virtual INodeTreeData* GetFundAccRiskTypes() = 0;

};

//券商参数配置
struct ICellItem : virtual public INodeData
{
	virtual CString GetCellID() = 0;
	virtual CString GetCaption() = 0;
	virtual CString GetType() = 0;
	virtual CString GetTypeName() = 0;
	virtual int GetServerID() = 0;
	virtual	CString GetBranchNo() = 0;
	virtual TiXmlElement* GetFunctionTree() = 0;
};

struct ICellList : virtual public ICCommParamList
{
	virtual ICellItem* GetItems(const int iIndex) = 0;
	virtual ICellItem* GetItemsByName(const CString sName) = 0;
};

//////////////////////////////////////////////////////////////////////////
//帐号参数配置
struct IAccountConfigItem : virtual public INodeData
{
	virtual CString GetID() = 0;
};

struct IAccountConfigList : virtual public ICCommParamList
{
	virtual IAccountConfigItem* GetItems(const int iIndex) = 0;
	virtual IAccountConfigItem* GetItemsByName(const CString sName) = 0;
};

//////////////////////////////////////////////////////////////////////////

