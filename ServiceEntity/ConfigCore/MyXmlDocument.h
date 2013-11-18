#pragma once

#include "QuoteLangMngInterFace.h"
#include "TradeComm\TradeLangMngIntf.h"
#include "tinyxml.h"
#include "WinnerApplication.h"



#define Root_Text _T("root")

class CMyXmlDocument : public TiXmlDocument
{
public:
	CMyXmlDocument();
	~CMyXmlDocument();
protected:
	/********************************************************************************
	* �������� : ��ָ����Դ��ȡ�ļ�����
	* �������� : UINT nFileID : ��ԴID
	*  ����ֵ  : BOOL : �ɹ�ΪTRUE,ʧ��ΪFALSE
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-11-18
	* �޸ļ�¼ : 
	*******************************************************************************/
	char * GetDataFromResource(UINT nFileID);
public:
	/********************************************************************************
	* �������� : ����Դ�����xml�ļ�
	* �������� : UINT nFileID : ��ԴID
	*			 
	*  ����ֵ  : BOOL : ���سɹ�����TRUE,ʧ�ܷ���FALSE
	* �������� : 
	* ��    ע : �ֱ��������Դ��(QuoteLang)�ͽ�����Դ��(TradeLang)���Ҷ�Ӧ��Դ����ID����
	*			 �����ȴ���������
	* ��    �� : lxqi
	* ������� : 2010-11-18
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL	LoadFromResource(UINT nFileID);

	/********************************************************************************
	* �������� : ����ָ��XML�ļ�
	* �������� : const char *pFilePath : �ļ�·��
	*			 
	*  ����ֵ  : BOOL : ���سɹ�����TRUE,ʧ�ܷ���FALSE
	* �������� : 
	* ��    ע : ��Ҫ����·��
	* ��    �� : lxqi
	* ������� : 2010-11-18
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL	LoadFromFile(const char* pFilePath);

	/********************************************************************************
	* �������� : �ͷű��ĵ�������Դ
	* �������� : 
	*  ����ֵ  : 
	* �������� : 
	* ��    ע : 
	* ��    �� : lxqi
	* ������� : 2010-11-18
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void	Release();

	/********************************************************************************
	* �������� : ���浱ǰ�ĵ�����
	* �������� : 
	*  ����ֵ  : 
	* �������� : 
	* ��    ע : 
	* ��    �� : ����
	* ������� : 2010-12-23
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL	SaveFile(const CString& sFileName);

	/********************************************************************************
	* �������� : ��ȡ��ǰ�ĵ����ڵ� 
	* �������� : 
	*  ����ֵ  : TiXmlNode* : ���ڵ�ָ�� ��������߼��������߼����ڵ㣬û�з����ĵ����ڵ�
	* �������� : 
	* ��    ע : 
	* ��    �� : ����
	* ������� : 2010-12-14
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual TiXmlNode* GetRootNode();

	/********************************************************************************
	* �������� : ��ȡ�ӽڵ����
	* �������� : CString strFindPath : ͳ�ƽڵ�·��(��Ϊ�գ���Ĭ��Ϊ���ڵ�)
	*			 TiXmlNode* pParentNode : ͳ�ƽڵ�ָ��(��Ϊ�գ���Ĭ��Ϊ���ڵ�)
	*			 CString strChildElement : �ӽڵ��ǩ��(��Ϊ���򲻿����ӱ�ǩ�����أ�ֻ������һ�������ӽڵ����)
	*  ����ֵ  : int : ��һ���ӽڵ������Ϊ�ҵ�ͳ�ƽڵ㷵��-1
	* �������� : 
	* ��    ע : 
	* ��    �� : lxqi
	* ������� : 2010-11-18
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual int GetChildNodeCount(const CString& strFindPath,
		const CString& strAttrName = "", 
		const CString& strAttrValue = "",
		const CString& strChildElement = "");
	virtual int GetChildNodeCount(TiXmlNode* pParentNode = NULL, const CString& strChildElement = "");

	/********************************************************************************
	* �������� : ����ָ���ڵ�·�����ҽڵ�
	* �������� : CString sFileName pFindPath : ·����Ϣ���á�/���ָ�
	*  ����ֵ  : TiXmlNode* : �ҵ��Ľڵ�ָ�룬δ�ҵķ���NULL
	* �������� : 
	* ��    ע : ����Ľڵ�·������ڵ�֮�����ɡ�/���ָ�������System/HsQuote
	*			 ����·����Ϊ�����GetRootNode()���õĸ��ڵ�
	* ��    �� : lxqi
	* ������� : 2010-11-18
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual TiXmlNode* GetNode(const CString& strFindPath); 

	/********************************************************************************
	* �������� : ����ָ��·������ָ��������������ֵ�Ľڵ�
	* �������� : CString strFindPath : ·����Ϣ���á�/���ָ�
	*			 CString strName : ��������
	*			 CString strNameValue : ����ֵ
	*			 int nNameValue : ����ֵ
	*			 double dNameValue : ����ֵ
	*  ����ֵ  : TiXmlNode* : �ҵ��Ľڵ�ָ�룬δ�ҵķ���NULL
	* �������� : 
	* ��    ע : ����Ľڵ�·������ڵ�֮�����ɡ�/���ָ�������System/HsQuote
	*			 ����·����Ϊ�����GetRootNode()���õĸ��ڵ�
	* ��    �� : lxqi
	* ������� : 2010-11-18
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strName, const CString& strNameValue);
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strName, int nNameValue);
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strName, double dNameValue);

	/********************************************************************************
	* �������� : ���ø��ڵ�
	* �������� : CString strFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 CString strAttrName : ��������
	*			 CString strAttrValue : ����ֵ
	*			 TiXmlNode* pRootNode : ָ�����ڵ�ָ��
	*  ����ֵ  : TiXmlNode* : ��ǰ���ڵ�
	* �������� : 
	* ��    ע : ��������ڵ㲻ͬ��Ҫ���ش˺���
	* ��    �� : ����
	* ������� : 2010-12-1
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL SetRootNode(const CString& strFindPath,
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");
	virtual BOOL SetRootNode(TiXmlNode* pRootNode);

	//////////////////////////////////////////////////////////////////////////���¸����Һ���֧��xPath��׼���
	/********************************************************************************
	* �������� : ��ȡ���趨ָ���ڵ��
	* �������� : CString strFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 CString strNewValue : ���趨ֵ
	*			 CString strAttrName : ��������
	*			 CString strAttrValue : ����ֵ
	*  ����ֵ  : CString : �ڵ�������ı�����
	*			 BOOL : �趨�ɹ���
	* �������� : 
	* ��    ע : 
	* ��    �� : lxqi
	* ������� : 2010-11-16
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual CString GetNodeData(TiXmlNode* pNode);
	virtual CString	GetNodeData(const CString& strFindPath,
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");
	virtual BOOL SetNodeData(TiXmlNode* pNode, const CString& strNewValue);
	virtual BOOL SetNodeData(const CString& strFindPath,
		const CString& strNewValue,
		const CString& strAttrName = NULL, 
		const CString& strAttrValue = NULL);


	/********************************************************************************
	* �������� : ��ȡ���趨ָ���ڵ����������ֵ
	* �������� : CString strpFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 TiXmlNode* pNode : �ڵ�ָ��
	*			 CString strAttrName : ��Ҫ�趨ֵ��������
	*			 int nNewAttrValue : ���趨ֵ
	*			 int nDefault : ��ȡ���ɹ�ʱ��Ĭ��ֵ
	*			 CString strExtAttrName :  ���ڲ��ҵ���������
	*			 CString strExtAttrValue : ���ڲ��ҵ�����ֵ
	*  ����ֵ  : int : ��ȡ��ֵ��ʧ��ʱ����Ĭ��ֵ
	*			 BOOL : �趨�ɹ��񣬳ɹ�����TRUE,ʧ�ܷ���FALSE
	* �������� : 
	* ��    ע : 
	* ��    �� : lxqi
	* ������� : 2010-11-16
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual int GetNodeAttrInt(TiXmlNode* pNode,const CString& strAttrName,int nDefault = 0);
	virtual int	GetNodeAttrInt(const CString& strFindPath,
		const CString& strAttrName, 
		int nDefault = 0,
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "");
	virtual BOOL SetNodeAttrInt(TiXmlNode* pNode, const CString& strNewAttrName, int nNewAttrValue);
	virtual BOOL SetNodeAttrInt(const CString& strFindPath, 
		const CString& strNewAttrName, 
		int nNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* �������� : ��ȡ���趨ָ���ڵ�ĸ�������ֵ
	* �������� : CString strpFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 TiXmlNode* pNode : �ڵ�ָ��
	*			 CString strAttrName : ��Ҫ�趨ֵ��������
	*			 int nNewAttrValue : ���趨ֵ
	*			 int nDefault : ��ȡ���ɹ�ʱ��Ĭ��ֵ
	*			 CString strExtAttrName :  ���ڲ��ҵ���������
	*			 CString strExtAttrValue : ���ڲ��ҵ�����ֵ
	*  ����ֵ  : double : ��ȡ��ֵ��ʧ��ʱ����Ĭ��ֵ
	*			 BOOL : �趨�ɹ��񣬳ɹ�����TRUE,ʧ�ܷ���FALSE
	* �������� : 
	* ��    ע : 
	* ��    �� : lxqi
	* ������� : 2010-11-16
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual double GetNodeAttrDouble(TiXmlNode* pNode,const CString& strAttrName,double dDefault = 0.0);
	virtual double GetNodeAttrDouble(const CString& strFindPath,
		const CString& strAttrName, 
		double dDefault = 0.0,
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "");
	virtual BOOL SetNodeAttrDouble(TiXmlNode* pNode, const CString& strNewAttrName, double dNewAttrValue);
	virtual BOOL SetNodeAttrDouble(const CString& strFindPath, 
		const CString& strNewAttrName, 
		double dNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* �������� : ��ȡ���趨ָ���ڵ���ַ�������ֵ
	* �������� : CString strpFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 TiXmlNode* pNode : �ڵ�ָ��
	*			 CString strAttrName : ��Ҫ�趨ֵ��������
	*			 int nNewAttrValue : ���趨ֵ
	*			 int nDefault : ��ȡ���ɹ�ʱ��Ĭ��ֵ
	*			 CString strExtAttrName :  ���ڲ��ҵ���������
	*			 CString strExtAttrValue : ���ڲ��ҵ�����ֵ
	*  ����ֵ  : double : ��ȡ��ֵ��ʧ��ʱ����Ĭ��ֵ
	*			 BOOL : �趨�ɹ��񣬳ɹ�����TRUE,ʧ�ܷ���FALSE
	* �������� : 
	* ��    ע : 
	* ��    �� : lxqi
	* ������� : 2010-11-16
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual CString GetNodeAttrString(TiXmlNode* pNode,const CString& strAttrName,const CString& strDefault=_T(""));
	virtual CString GetNodeAttrString(const CString& strFindPath,
		const CString& strAttrName, 
		const CString& strDefault = "",
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "");
	virtual BOOL SetNodeAttrString(TiXmlNode* pNode, const CString& strNewAttrName, const CString& strNewAttrValue);
	virtual BOOL SetNodeAttrString(const CString& strFindPath, 
		const CString& strNewAttrName, 
		const CString& strNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");


	/********************************************************************************
	* �������� : ��ȡ/�����޸ı�ʶ
	* �������� : BOOL bModified : �Ƿ����޸�
	*  ����ֵ  : BOOL : �Ƿ����޸�
	* �������� : 
	* ��    ע : 
	* ��    �� : lxqi
	* ������� : 2010-11-18
	* �޸ļ�¼ : 
	*******************************************************************************/
	BOOL GetModified() {return m_bModified;}
	void SetModified(BOOL bModified){ m_bModified = bModified; }

	/********************************************************************************
	* �������� : ��ȡ/���ü��ر�ʶ
	* �������� : BOOL bLoaded : �Ƿ��Ѽ���
	*  ����ֵ  : BOOL : �Ƿ��Ѽ���
	* �������� : 
	* ��    ע : 
	* ��    �� : ����
	* ������� : 2010-12-14
	* �޸ļ�¼ : 
	*******************************************************************************/
	BOOL GetLoaded() {return m_bLoaded;}
	void SetLoaded(BOOL bLoaded){ m_bLoaded = bLoaded; }

	/********************************************************************************
	* �������� : �жϽڵ��Ƿ����ڱ��ļ�
	* �������� : TiXmlNode* : �ڵ�ָ��
	*  ����ֵ  : BOOL : �Ƿ�����
	* �������� : 
	* ��    ע : 
	* ��    �� : ����
	* ������� : 2010-12-15
	* �޸ļ�¼ : 
	*******************************************************************************/
	BOOL IsBelongToMy(TiXmlNode* pNode);

private:
	BOOL m_bModified;  //�Ƿ��޸�
	BOOL m_bLoaded;    //�Ƿ����
	TiXmlNode* m_pRootNode;
//	CMapStringToPtr m_CurNodeMap;   //��Ŷ�ȡ���Ľڵ� ����:�����·��
public:
	static IQuoteLang* m_iQuoteLang;
	static ITradeLang* m_iTradeLang;
};
