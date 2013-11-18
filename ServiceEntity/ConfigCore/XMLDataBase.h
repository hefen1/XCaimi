/*******************************************************
  源程序名称:XMLDataBase.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  配置中心
  功能说明:  XML配置文件基础数据类定义
  作    者:  shenglq
  开发日期:  20100926
*********************************************************/
#pragma once
#include "XMLDataBaseIntf.h"
#include "HSXMLParser.h"
#pragma warning (disable:4250)

class CNodeData : virtual public INodeData
{
public:
	CNodeData();
	virtual ~CNodeData();
	void Release();
	CString GetValue(const CString sPropName);
	CString Name();
	CString Value();
	CString ValueEx();
public:
	void SetXMLParser(CHSXMLParser* pXMLParser);
	void SetRoot(TiXmlNode* pRoot);
protected:
	virtual CString GetIDPropName();
	TiXmlNode* m_pRoot;
	TiXmlNode* m_pParent;
	CHSXMLParser* m_pXMLParser;
};
//////////////////////////////////////////////////////////////////////////
class CNodeTreeData : public CNodeData, virtual public INodeTreeData
{
public:
	CNodeTreeData();
	virtual ~CNodeTreeData();
	int GetCount();
	virtual INodeData* GetItems(const int iIndex);
	virtual INodeData* GetItemsByName(const CString sName);
protected:
	virtual CXML GetChildNodeName();
	virtual INodeData* GetItemsFull(CXML xPath, int iIndex = -1);
};

//////////////////////////////////////////////////////////////////////////

class CNodeDataList : virtual public INodeDataList
{
public:
	CNodeDataList();
	virtual ~CNodeDataList();
	void Release();
	
	int GetCount();
	virtual INodeData* GetItems(const int iIndex);
	virtual INodeData* GetItemsByName(const CString sName);
	virtual INodeTreeData* GetNodeTreeData(const CString sName);
public:
	BOOL InitData();
protected:
	virtual CString GetFileName();
	virtual UINT GetIDResource();
	virtual CXML GetRootPath();
	virtual CXML GetChildNodeName();
	virtual CString GetIDPropName();
	virtual INodeData* GetItemsFull(CXML xPath, int iIndex = -1);
	TiXmlNode* m_pRoot;
	CHSXMLParser* m_pXMLParser;
};