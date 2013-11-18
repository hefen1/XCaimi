/*******************************************************
  源程序名称:TradeTablesIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  配置中心
  功能说明:  交易查询结果集表头接口定义
  作    者:  shenglq
  开发日期:  20100926
*********************************************************/
#pragma once
#include "XMLDataBaseIntf.h"
#include "TradeConst.h"

struct ITradeDict: virtual public INodeTreeData
{
	virtual CString GetValueByName(CString sName) = 0;
};
//////////////////////////////////////////////////////////////////////////

struct ITradeColumn
{
	virtual void Release() = 0;
	virtual CString GetValue(CString sPropName) = 0;
	//获取列名
	virtual CString GetColumnName() = 0;
	//获取显示名称
	virtual CString GetDispName() = 0;
	//对齐
	virtual EAlign GetAlign() = 0;
	//获取数据字典
	virtual ITradeDict* GetTradeDict() = 0;
	virtual CString GetDictValue(CString sName) = 0;

};
//////////////////////////////////////////////////////////////////////////

struct ITradeTable: virtual public INodeData
{
	virtual int GetCount() = 0;
	virtual ITradeColumn* GetItems(const int iIndex) = 0;
	virtual ITradeColumn* GetItemsByName(const CString sName) = 0;
};
//////////////////////////////////////////////////////////////////////////
struct ITradeTables : virtual public INodeDataList
{
	virtual ITradeTable* GetItems(const int iIndex) = 0;
	virtual ITradeTable* GetItemsByName(const CString sName) = 0;
	//根据名称获取数据字典
	virtual ITradeDict* GetTradeDictByName(CString sName) = 0;
	//根据功能号获取请求参数
	virtual INodeTreeData* GetReqParamByAction(const int nAction) = 0;
};