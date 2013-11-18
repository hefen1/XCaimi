/*******************************************************
  源程序名称:HSXMLParser.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  
  功能说明:  XMLParser类定义
  作    者:  shenglq
  开发日期:  20100927
*********************************************************/
#pragma once
#include "xpath_static.h"

#define Root_Text _T("root")

typedef CString CXML;

class CHSXMLParser
{
public:
	CHSXMLParser();
	~CHSXMLParser();
	void Release();
	BOOL LoadFile(CString sFileName);
	BOOL LoadXML(const char* szXML);
	BOOL SaveFile(CString sFileName);
	TiXmlNode* GetRoot();
	// <summary>
	//   获取指定路径的节点个数
	// </summary>
	// <param name="ARoot">根节点路径</param>  
	// <param name="APath">相对于根节点的路径</param>
	// <returns>指定路径的节点个数</returns>
	int GetNodeCount(CXML xRoot, CXML xPath);
	// <summary>
	//   获取指定路径的节点个数
	// </summary>    
	// <param name="APath">相对于根节点的路径</param> 
	// <param name="AParent">指定的XML节点；默认值为不指定节点，此时APath为解析器当前根节点的相对路径路径</param> 
	// <returns>指定路径的节点个数</returns>
	int GetNodeCount(CXML xPath, TiXmlNode* pParent = NULL);
	// <summary>
	//   获取指定路径的节点属性值
	// </summary>
	// <param name="ARoot">根节点路径</param>
	// <param name="APath">相对于根节点的路径</param>   
	// <param name="Default">默认值</param>  
	// <returns>指定路径的节点属性值</returns>
	CString GetNodeValue(CXML xRoot, CXML xPath, CString Default = "");
	// <summary>
	//   获取指定路径的节点属性值
	// </summary>
	// <param name="APath">相对于根节点的路径</param>   
	// <param name="AParent">指定的XML节点；默认值为不指定节点，此时APath为解析器当前根节点的相对路径路径</param>
	// <param name="Default">默认值</param>
	// <returns>指定路径的节点属性值</returns>
	CString GetNodeValue(CXML xPath, TiXmlNode* pParent = NULL, CString Default = "");
	// <summary>
	//   获取指定路径的节点
	// </summary>
	// <param name="ARoot">根节点路径</param>
	// <param name="APath">相对于根节点的路径</param>  
	// <param name="Index">列表节点的索引值；默认为-1，取列表中的第一个节点</param>
	// <returns>指定路径的节点</returns>
	TiXmlNode* SelectSingleNode(CXML xRoot, CXML xPath, int iIndex = -1);
	// <summary>
	//   获取指定路径的节点
	// </summary>
	// <param name="APath">相对于根节点的路径</param>  
	// <param name="AParent">指定的XML节点；默认值为不指定节点，此时APath为解析器当前根节点的相对路径路径</param>
	// <param name="Index">列表节点的索引值；默认为-1，取列表中的第一个节点</param>
	// <returns>指定路径的节点</returns>
	TiXmlNode* SelectSingleNode(CXML xPath, TiXmlNode* pParent = NULL, int iIndex = -1);

	BOOL GetModified();
	void SetModified(BOOL bValue);

private:
	TiXmlDocument* m_pTiXmlDocument;
	TiXmlNode* m_pRoot;
	BOOL m_bModifed;
};