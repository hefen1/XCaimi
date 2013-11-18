/*******************************************************
  源程序名称:TradeSysConfig.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  配置中心
  功能说明:  交易系统配置
  作    者:  shenglq
  开发日期:  20101012
*********************************************************/
#pragma once
#include "XMLDataBase.h"
#include "TradeSysConfigIntf.h"
#pragma warning (disable:4250)

//////////////////////////////////////////////////////////////////////////
class CTradeMarketItem : public CNodeData, public ITradeMarketItem
{
public:
	CTradeMarketItem();
	virtual ~CTradeMarketItem();

	CString GetMarketName();
	CString GetDispName();
	CString GetAccountType();
	EMoneyType GetMoneyType();
};
//////////////////////////////////////////////////////////////////////////
class CTradeMarketList : public CNodeTreeData, public ITradeMarketList
{
public:
	CTradeMarketList();
	virtual ~CTradeMarketList();
	ITradeMarketItem* GetItems(const int iIndex);
	ITradeMarketItem* GetItemsByName(const CString sName);
protected:
	virtual ITradeMarketItem* GetItemsFull(CXML xPath, int iIndex = -1);
};
///////////////////////////////////////////////////////////////////////////
//账号类型相关类
class CTradeAccountTypeItem : public CNodeData, public ITradeAccountTypeItem
{
public:
	CTradeAccountTypeItem();
	virtual ~CTradeAccountTypeItem();
	CString GetAccountTypeName();
	CString GetAccountType();
	CString GetBackType();
	CString GetShortName();
	CString GetMarketType();
	CString GetVisibleName();
};

class CTradeAccountTypeList : public CNodeTreeData, public ITradeAccountTypeList
{
public:
	CTradeAccountTypeList();
	virtual ~CTradeAccountTypeList();
	ITradeAccountTypeItem* GetItems(const int iIndex);
	ITradeAccountTypeItem* GetItemsByName(const CString sName);
	CString GetBackTypeByAccountType(const CString sName);
	CString GetMarketTypeByAccountType(const CString sName);
protected:
	virtual ITradeAccountTypeItem* GetItemsFull(CXML xPath, int iIndex = -1);
};

///////////////////////////////////////////////////////////////////////////////////
class CModifyPassowrdTypeItem : public CNodeData, public IModifyPassowrdTypeItem
{
public:
	CModifyPassowrdTypeItem();
	virtual ~CModifyPassowrdTypeItem();
	CString GetOldCharType();
	int GetOldMaxLen();
	int GetOldMinLen();
	CString GetNewCharType();
	int GetNewMaxLen();
	int GetNewMinLen();
};
//////////////////////////////////////////////////////////////////////////
class CModifyPassowrdTypeList : public CNodeTreeData, public IModifyPassowrdTypeList
{
public:
	CModifyPassowrdTypeList();
	virtual ~CModifyPassowrdTypeList();
	IModifyPassowrdTypeItem* GetItems(const int iIndex);
	IModifyPassowrdTypeItem* GetItemsByName(const CString sName);
protected:
	virtual IModifyPassowrdTypeItem* GetItemsFull(CXML xPath, int iIndex = -1);
};

//////////////////////////////////////////////////////////////////////////
//基金风险问卷
class CFundRiskQuestionItem : public CNodeData, public IFundRiskQuestionItem
{
public:
	CFundRiskQuestionItem();
	~CFundRiskQuestionItem();
	CString GetCode();
	CString GetTitle();
	CString GetChoice();
	CString GetChoiceText();
	int GetChoiceScore();
	CString GetRemark();
protected:
	virtual CString GetIDPropName();
};
class CFundRiskQuestionList : public CNodeTreeData, public IFundRiskQuestionList
{
public:
	CFundRiskQuestionList();
	~CFundRiskQuestionList();
	IFundRiskQuestionItem* GetItems(const int iIndex);
	IFundRiskQuestionItem* GetItemsByName(const CString sName);
protected:
	virtual IFundRiskQuestionItem* GetItemsFull(CXML xPath, int iIndex = -1);
};
//////////////////////////////////////////////////////////////////////////
//公共参数类
class CCommParamList : public CNodeTreeData, virtual public ICCommParamList
{
public:	
	CCommParamList();
	virtual ~CCommParamList();
	virtual CXML GetListPath();
	void SetNextParamList(ICCommParamList* pParamlist);
	void SetCellAndAccount(const CString sCellID, const CString sAccount);
	//获取默认请求列表
	INodeTreeData* GetDefaultReqDataList(CString sID = "");
	//获取账号类别
	ITradeAccountTypeList* GetAccountTypeList(const CString sID = "");
	//根据节点名称获取NV列表
	INodeTreeData* GetNVListByName(const CString sNodeName, const CString sID = "");
	//获取修改密码类型
	IModifyPassowrdTypeList* GetModifyPassowrdTypeList();
	//获取市场类别
	ITradeMarketList* GetMarketList();	
	//获取上海市价委托类型
	INodeTreeData* GetSHSJEntrustTypeList();
	//获取深圳市价委托类型
	INodeTreeData* GetSZSJEntrustTypeList();
	//获取巨额赎回
	INodeTreeData* GetExceedFlagList();
	//获取收费方式
	INodeTreeData* GetChargeTypeList();
	//获取分红方式
	INodeTreeData* GetDividendMethodList();
	//获取币种类型
	INodeTreeData* GetMoneyTypeList();
	//获取股票操作类别
	INodeTreeData* GetStockEntrustType();
	//获取证件类型
	INodeTreeData* GetIDKindList();
	//获取委托状态
	INodeTreeData* GetEntrustStatusList();
	//获取基金风险测试问卷
	IFundRiskQuestionList* GetFundRiskQuestionList();
	//获取用户风险等级
	INodeTreeData* GetFundAccRiskTypes();
	
protected:
	CString m_sCellID;
	CString m_sAccount;
	ICCommParamList* m_pNextParamList;
	CString m_sID;
private:
	TiXmlNode* GetChildNodeByIDName(const CString sID, const CString sName);
};

//////////////////////////////////////////////////////////////////////////
class CCellItem : public CNodeData, public ICellItem
{
public:
	CCellItem();
	virtual ~CCellItem();
	CString GetCellID();
	CString GetCaption();
	CString GetType();
	CString GetTypeName();
	int GetServerID();
	CString GetBranchNo();
	TiXmlElement* GetFunctionTree();
protected:
	virtual CString GetIDPropName();
};
//券商参数类
class CCellList : public CCommParamList, public ICellList
{
public:
	CCellList();
	virtual ~CCellList();
	virtual CXML GetListPath();
	ICellItem* GetItems(const int iIndex);
	ICellItem* GetItemsByName(const CString sName);
protected:
	virtual CXML GetChildNodeName();
	virtual CString GetIDPropName();
	virtual ICellItem* GetItemsFull(CXML xPath, int iIndex = -1);
};

//////////////////////////////////////////////////////////////////////////

class CAccountConfigItem : public CNodeData, public IAccountConfigItem
{
public:
	CAccountConfigItem();
	virtual ~CAccountConfigItem();
	CString GetID();
protected:
	virtual CString GetIDPropName();
};
//帐号参数类
class CAccountConfigList : public CCommParamList, public IAccountConfigList
{
public:
	CAccountConfigList();
	virtual ~CAccountConfigList();
	virtual CXML GetListPath();
	IAccountConfigItem* GetItems(const int iIndex);
	IAccountConfigItem* GetItemsByName(const CString sName);
protected:
	virtual CXML GetChildNodeName();
	virtual CString GetIDPropName();
	virtual IAccountConfigItem* GetItemsFull(CXML xPath, int iIndex = -1);
};

class CTradeConfigDef : public CNodeDataList
{
public:
	CTradeConfigDef();
	virtual~CTradeConfigDef();
	ICCommParamList* GetDefaultConfigList();
protected:
	virtual UINT GetIDResource();
};

class CTradeConfigCell : public CNodeDataList
{
public:
	CTradeConfigCell();
	virtual~CTradeConfigCell();
	ICellList* GetCellList();
protected:
	virtual CString GetFileName();
};

class CTradeConfigAccount : public CNodeDataList
{
public:
	CTradeConfigAccount();
	virtual~CTradeConfigAccount();
	IAccountConfigList* GetAccountConfigList();
protected:
	virtual CString GetFileName();
};

