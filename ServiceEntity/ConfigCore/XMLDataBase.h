/*******************************************************
  Դ��������:XMLDataBase.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  XML�����ļ����������ඨ��
  ��    ��:  shenglq
  ��������:  20100926
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