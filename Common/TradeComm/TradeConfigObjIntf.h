/*******************************************************
  源程序名称:TradeConfigObjIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易交易配置
  功能说明:  
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradeTablesIntf.h"
#include "TradeSysConfigIntf.h"

#define OBJ_TRADECONFIG			_T("ConfigCore.TradeConfigObj")

struct ITradeConfigObj
{
	//释放
	virtual void Release() = 0;
	//获取营业部列表
	virtual ICellList* GetCellList() = 0;
	//virtual TiXmlNode* GetMemTablesRoot() = 0; 
	//根据功能号获取表头信息
	virtual ITradeTable* GetTradeTableData(const int iAction) = 0;
	//获取数据字典
	virtual ITradeDict* GetTradeDict(CString sName) = 0;
	////根据节点名称获取NV列表
	//virtual INodeTreeData* GetNodeTreeData(CString sName, CString sCellID = "", CString sAccount = "") = 0;
	////获取上海市价委托类型
	//virtual INodeTreeData* GetSHSJEntrustTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////获取深圳市价委托类型
	//virtual INodeTreeData* GetSZSJEntrustTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////获取市场类别
	//virtual ITradeMarketList* GetTradeMarketList(CString sCellID = "", CString sAccount = "") = 0;
	////获取账号类别
	//virtual ITradeAccountTypeList* GetTradeAccountTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////获取巨额赎回
	//virtual INodeTreeData* GetExceedFlagList(CString sCellID = "", CString sAccount = "") = 0;
	////获取收费方式
	//virtual INodeTreeData* GetChargeTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////获取分红方式
	//virtual INodeTreeData* GetDividendMethodList(CString sCellID = "", CString sAccount = "") = 0;
	////获取币种类型
	//virtual INodeTreeData* GetMoneyTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////获取修改密码类型
	//virtual IModifyPassowrdTypeList* GetModifyPassowrdTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////获取股票操作类别
	//virtual INodeTreeData* GetStockEntrustType(CString sCellID = "", CString sAccount = "") = 0;
	////获取证件类型
	//virtual INodeTreeData* GetIDKindList(CString sCellID = "", CString sAccount = "") = 0;
	////获取委托状态
	//virtual INodeTreeData* GetEntrustStatusList(CString sCellID = "", CString sAccount = "") = 0;
	////获取默认请求列表
	//virtual INodeTreeData* GetDefaultReqDataList(CString sCellID = "", CString sAccount = "") = 0;
	//根据功能号获取请求参数
	virtual INodeTreeData* GetReqParamList(const int iAction) = 0;
	//获取功能树节点
	virtual TiXmlElement* GetFunctionTree(CString sCellID, CString sAccount = "") = 0;
	//获取帐号参数列表
	virtual ICCommParamList* GetAccountParamList(CString sCellID, CString sAccount) = 0;
};