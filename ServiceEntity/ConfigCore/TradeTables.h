/*******************************************************
  Դ��������:TradeTables.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ���ײ�ѯ�������ͷ�ඨ��
  ��    ��:  shenglq
  ��������:  20100926
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
	//���ݹ��ܺŻ�ȡ�������
	INodeTreeData* GetReqParamByAction(const int nAction);
protected:
	virtual UINT GetIDResource();
	virtual CString GetFileName();
	virtual CXML GetRootPath();
	virtual ITradeTable* GetItemsFull(CXML xPath, int iIndex  = -1);
};
//////////////////////////////////////////////////////////////////////////
