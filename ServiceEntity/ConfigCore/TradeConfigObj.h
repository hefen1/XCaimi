/*******************************************************
  源程序名称:TradeConfigObj.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易交易配置
  功能说明:  
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradeConfigObjIntf.h"
#include "TradeTables.h"
#include "TradeSysConfig.h"
class CTradeConfigObj : public ITradeConfigObj
{
public:
	CTradeConfigObj();
	~CTradeConfigObj();
	void Release();
	//获取营业部列表
	ICellList* GetCellList();
	//根据功能号获取表头信息
	ITradeTable* GetTradeTableData(const int iAction);
	//获取数据字典
	ITradeDict* GetTradeDict(CString sName);
	//根据功能号获取请求参数
	INodeTreeData* GetReqParamList(const int iAction);
	//获取功能树节点
	TiXmlElement* GetFunctionTree(CString sCellID, CString sAccount = "");
	//获取帐号参数列表
	ICCommParamList* GetAccountParamList(CString sCellID, CString sAccount);
	//TiXmlNode* GetMemTablesRoot();
private:
	CTradeTables* m_pTradeTables;
	CTradeConfigDef* m_pConfigDef;
	CTradeConfigCell* m_pConfigCell;
	CTradeConfigAccount* m_pConfigAccount;
	BOOL LoadTradeSysConfigs();
	BOOL LoadTradeTables();
};