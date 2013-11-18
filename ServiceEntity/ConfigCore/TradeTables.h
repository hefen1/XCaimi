/*******************************************************
  源程序名称:TradeTables.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  配置中心
  功能说明:  交易查询结果集表头类定义
  作    者:  shenglq
  开发日期:  20100926
*********************************************************/
#pragma once
#include "XMLDataBase.h"
#include "TradeTablesIntf.h"
#pragma warning (disable:4250)

class CTradeDict : public CNodeTreeData, public ITradeDict
{
public:
	CTradeDict();
	~CTradeDict();
	CString GetValueByName(CString sName);
protected:
	virtual CXML GetChildNodeName();
};
//////////////////////////////////////////////////////////////////////////


class CTradeColumn : public ITradeColumn
{
public:
	CTradeColumn();
	~CTradeColumn();
	void Release();
	CString GetValue(CString sPropName);
	CString GetColumnName();
	CString GetDispName();	
	EAlign GetAlign();
	ITradeDict* GetTradeDict();
	CString GetDictValue(CString sName);
public:
	void SetRoot(TiXmlNode* pRoot);
	void SetXMLParser(CHSXMLParser* pXMLParser);
private:
	TiXmlNode*		m_pRoot;
	CHSXMLParser*	m_pXMLParser;
	CString			m_sColumnName;
	CString			m_sDispName;
	EAlign			m_eAlign;
	BOOL			m_bHaveTradeDict;
	ITradeDict*		m_pTradeDict;
};
//////////////////////////////////////////////////////////////////////////
class CTradeTable : public CNodeData, public ITradeTable
{
public:
	int GetCount();
	ITradeColumn* GetItems(const int iIndex);
	ITradeColumn* GetItemsByName(const CString sName);
protected:
	virtual CXML GetChildNodeName();
	virtual ITradeColumn* GetItemsFull(CXML xPath, int iIndex  = -1);
};
//////////////////////////////////////////////////////////////////////////

class CTradeTables : public CNodeDataList, public ITradeTables
{
public:	
	ITradeTable* GetItems(const int iIndex);
	ITradeTable* GetItemsByName(const CString sName);
	ITradeDict* GetTradeDictByName(CString sName);
	//根据功能号获取请求参数
	INodeTreeData* GetReqParamByAction(const int nAction);
protected:
	virtual UINT GetIDResource();
	virtual CString GetFileName();
	virtual CXML GetRootPath();
	virtual ITradeTable* GetItemsFull(CXML xPath, int iIndex  = -1);
};
//////////////////////////////////////////////////////////////////////////
