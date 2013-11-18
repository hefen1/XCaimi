/*******************************************************
  Դ��������:HSXMLParser.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  
  ����˵��:  XMLParser�ඨ��
  ��    ��:  shenglq
  ��������:  20100927
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
	//   ��ȡָ��·���Ľڵ����
	// </summary>
	// <param name="ARoot">���ڵ�·��</param>  
	// <param name="APath">����ڸ��ڵ��·��</param>
	// <returns>ָ��·���Ľڵ����</returns>
	int GetNodeCount(CXML xRoot, CXML xPath);
	// <summary>
	//   ��ȡָ��·���Ľڵ����
	// </summary>    
	// <param name="APath">����ڸ��ڵ��·��</param> 
	// <param name="AParent">ָ����XML�ڵ㣻Ĭ��ֵΪ��ָ���ڵ㣬��ʱAPathΪ��������ǰ���ڵ�����·��·��</param> 
	// <returns>ָ��·���Ľڵ����</returns>
	int GetNodeCount(CXML xPath, TiXmlNode* pParent = NULL);
	// <summary>
	//   ��ȡָ��·���Ľڵ�����ֵ
	// </summary>
	// <param name="ARoot">���ڵ�·��</param>
	// <param name="APath">����ڸ��ڵ��·��</param>   
	// <param name="Default">Ĭ��ֵ</param>  
	// <returns>ָ��·���Ľڵ�����ֵ</returns>
	CString GetNodeValue(CXML xRoot, CXML xPath, CString Default = "");
	// <summary>
	//   ��ȡָ��·���Ľڵ�����ֵ
	// </summary>
	// <param name="APath">����ڸ��ڵ��·��</param>   
	// <param name="AParent">ָ����XML�ڵ㣻Ĭ��ֵΪ��ָ���ڵ㣬��ʱAPathΪ��������ǰ���ڵ�����·��·��</param>
	// <param name="Default">Ĭ��ֵ</param>
	// <returns>ָ��·���Ľڵ�����ֵ</returns>
	CString GetNodeValue(CXML xPath, TiXmlNode* pParent = NULL, CString Default = "");
	// <summary>
	//   ��ȡָ��·���Ľڵ�
	// </summary>
	// <param name="ARoot">���ڵ�·��</param>
	// <param name="APath">����ڸ��ڵ��·��</param>  
	// <param name="Index">�б�ڵ������ֵ��Ĭ��Ϊ-1��ȡ�б��еĵ�һ���ڵ�</param>
	// <returns>ָ��·���Ľڵ�</returns>
	TiXmlNode* SelectSingleNode(CXML xRoot, CXML xPath, int iIndex = -1);
	// <summary>
	//   ��ȡָ��·���Ľڵ�
	// </summary>
	// <param name="APath">����ڸ��ڵ��·��</param>  
	// <param name="AParent">ָ����XML�ڵ㣻Ĭ��ֵΪ��ָ���ڵ㣬��ʱAPathΪ��������ǰ���ڵ�����·��·��</param>
	// <param name="Index">�б�ڵ������ֵ��Ĭ��Ϊ-1��ȡ�б��еĵ�һ���ڵ�</param>
	// <returns>ָ��·���Ľڵ�</returns>
	TiXmlNode* SelectSingleNode(CXML xPath, TiXmlNode* pParent = NULL, int iIndex = -1);

	BOOL GetModified();
	void SetModified(BOOL bValue);

private:
	TiXmlDocument* m_pTiXmlDocument;
	TiXmlNode* m_pRoot;
	BOOL m_bModifed;
};