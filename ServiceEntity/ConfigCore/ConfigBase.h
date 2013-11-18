/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�ConfigBase.h
* ժ    Ҫ���ļ�����ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-10
* ��	 ע��
*******************************************************************************/
#pragma once
#include "MyXmlDocument.h"

enum DocType{Default, ResourceDoc, SystemDoc, UserDoc};		// ָ������Դ����


#define IntDefault     INT_MAX
#define DoubleDefault  -2222.222

class CConfigBase
{
public:
	CConfigBase();
	~CConfigBase();

private:
	/********************************************************************************
	* �������� : ��ʼ����Դ�е������ļ�
	* �������� : UINT nRcID : ָ�������ص���Դ�ļ�ID
	*			 
	*  ����ֵ  : BOOL : �Ƿ���سɹ�
	* �������� : 
	* ��    ע :  
	* ��    �� : ����
	* ������� : 2010-12-14
	* �޸ļ�¼ : 
	*******************************************************************************/
	BOOL InitResourceDoc(UINT nRcID);
	/********************************************************************************
	* �������� : ��ʼ������ϵͳ�����ļ�
	* �������� : CString strDocFilePath : ָ�������ص��ļ�·��
	*			 
	*  ����ֵ  : BOOL : �Ƿ���سɹ�
	* �������� : 
	* ��    ע :  
	* ��    �� : ����
	* ������� : 2010-12-14
	* �޸ļ�¼ : 
	*******************************************************************************/
	BOOL InitSysFileDoc(const CString& strDocFilePath);
	/********************************************************************************
	* �������� : ��ʼ�������Զ��������ļ�
	* �������� : CString strDocFilePath : ָ�������ص��ļ�·��
	*			 
	*  ����ֵ  : BOOL : �Ƿ���سɹ�
	* �������� : 
	* ��    ע :  
	* ��    �� : ����
	* ������� : 2010-12-14
	* �޸ļ�¼ : 
	*******************************************************************************/
	BOOL InitUserFileDoc(const CString& strDocFilePath);

	/********************************************************************************
	* �������� : ���û��ĵ��������Ľڵ���ָ����·��
	* �������� : TiXmlNode* : ��Ҫ����Ľڵ�
	*            CString    : ·��
	*            TiXmlNode* : ϵͳ�ļ�������Դ�ж�Ӧ�Ľڵ�
	*            CString    : ·����չ������
	*            CString    : ·����չ����ֵ 
	*  ����ֵ  : BOOL       : ���ز����Ƿ�ɹ�
	* �������� : 
	* ��    ע : 
	* ��    �� : ����
	* ������� : 2010-12-15
	* �޸ļ�¼ : 
	*******************************************************************************/
	BOOL InsertNodes(TiXmlNode* pNewNode, CString strFindPath, TiXmlNode* pOtherNode, const CString strAttrName = "", const CString strAttrValue = "");

	/********************************************************************************
	* �������� : ��Դ�ڵ�����������Լ��ı����� ���Ƶ�Ŀ��ڵ�
	* �������� : TiXmlNode* : Դ�ڵ�
	*  ����ֵ  : TiXmlNode* : Ŀ��ڵ�
	* �������� : 
	* ��    ע : 
	* ��    �� : ����
	* ������� : 2010-12-16
	* �޸ļ�¼ : 
	*******************************************************************************/
	TiXmlNode* CopyNodeFromAnther(TiXmlNode* pSourceNode);

public:
	// ���ó�ʼ��
	/********************************************************************************
	* �������� : ��ʼ������
	* �������� : UINT nRcID : ָ�������ص���Դ�ļ�ID
	*			 CString strSysDocFilePath : ָ�������ص�ϵͳĬ���ļ�·��
	*			 CString strUserDocFilePath : ָ�������ص��û��Զ����ļ�·��
	*			 
	*  ����ֵ  : BOOL : �Ƿ���سɹ�
	* �������� : 
	* ��    ע :  
	* ��    �� : ����
	* ������� : 2010-12-14
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL Init(UINT nRcID, const CString &strSysDocFilePath, const  CString &strUserDocFilePath);

	/********************************************************************************
	* �������� : �ж��Ƿ��ѳ�ʼ����
	* �������� : 
	*  ����ֵ  : BOOL : �ѳ�ʼ����ΪTRUE,δ��ʼ��ΪFALSE
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-11-12
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL IsInit();

	/********************************************************************************
	* �������� : �ͷ��ڴ�
	* �������� : UINT nFileID : ��ԴID
	*  ����ֵ  : BOOL : �ɹ�ΪTRUE,ʧ��ΪFALSE
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-11-12
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL Release(BOOL bSaveToFile = FALSE);

	/********************************************************************************
	* �������� : ���޸����ݱ��浽��Ӧ�ļ�
	* �������� : CString strFilePath : �����ļ�·��
	*  ����ֵ  : 
	* �������� : 
	* ��    ע : ������·��Ϊ�գ��򱣴��ļ�ΪUserDoc��ʼ��ʱ���ļ�����
	*			 ���޴��ļ�����Ĭ��ΪSystemDoc�ļ������"User"
	* ��    �� : lxqi
	* ������� : 2010-11-12
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void SaveToFile(const CString &strFileName = "");

	/********************************************************************************
	* �������� : ����ָ��·������ָ��������������ֵ�Ľڵ�
	* �������� : CString strFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 DocType nType : ָ�������ļ���Ĭ��Ϊ�Զ��������ң�User->System->Resource��
	*  ����ֵ  : TiXmlNode* : �ҵ��Ľڵ�ָ�룬δ�ҵķ���NULL
	*			 TiXmlElement* : �ҵ��Ľڵ�ָ�룬δ�ҵķ���NULL
	* �������� : 
	* ��    ע : ����Ľڵ�·������ڵ�֮�����ɡ�/���ָ�������System/HsQuote�����������ڵ㣩
	*			 ÿ�γɹ��������ڵ���Զ����Ҹýڵ��Ƿ���id��name���ԣ����У��������Ӧ����
	*			 ����Ҫ���������������Ҫ���ش˺���
	* ��    �� : lxqi
	* ������� : 2010-11-12
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual TiXmlNode* GetNode(const CString& strFindPath, DocType nType = Default);

	/********************************************************************************
	* �������� : ����ָ��·������ָ��������������ֵ�Ľڵ�
	* �������� : CString strFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 CString strAttrName : ��������
	*			 CString strAttrValue : ����ֵ
	*			 DocType nType : ָ�������ļ���Ĭ��Ϊ�Զ��������ң�User->System->Resource��
	*  ����ֵ  : TiXmlNode* : �ҵ��Ľڵ�ָ�룬δ�ҵķ���NULL
	*			 TiXmlElement* : �ҵ��Ľڵ�ָ�룬δ�ҵķ���NULL
	* �������� : 
	* ��    ע : ����Ľڵ�·������ڵ�֮�����ɡ�/���ָ�������System/HsQuote�����������ڵ㣩
	*			 ÿ�γɹ��������ڵ���Զ����Ҹýڵ��Ƿ���id��name���ԣ����У��������Ӧ����
	*			 ����Ҫ���������������Ҫ���ش˺���
	* ��    �� : lxqi
	* ������� : 2010-11-12
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strAttrName, const CString& strAttrValue, DocType nType = Default  );
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strAttrName, int nNameValue, DocType nType = Default  );
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strAttrName,double dNameValue, DocType nType = Default  );
	/********************************************************************************
	* �������� : ���û��ļ����½�һ���ڵ�(�ýڵ������ϵͳ�ļ�������Դ�ļ��д��ڵ�)
	* �������� : CString strPath : �ڵ�·��
	*  ����ֵ  : TiXmlNode* : �½��Ľڵ� ʧ����Ϊ��
	* �������� : 
	* ��    ע : 
	* ��    �� : ����
	* ������� : 2010-12-23
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual TiXmlNode* SetNode(const CString& strPath, const CString& strAttrName = "", const CString& strAttrValue = "");

	/********************************************************************************
	* �������� : ���û��ļ���ɾ��һ���ڵ�
	* �������� : CString strPath : �ڵ�·��
	*  ����ֵ  : BOOL : �����Ƿ�ɹ�
	* �������� : 
	* ��    ע : 
	* ��    �� : ����
	* ������� : 2010-12-23
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL RemoveNode(TiXmlNode* pNode);
	virtual BOOL RemoveNode(const CString& strPath, const CString& strAttrName = "", const CString& strAttrValue = "");
	/********************************************************************************
	* �������� : ��ȡ��ǰ���ڵ�
	* �������� : DocType nType : ָ���ĵ����ͣ�Ĭ��Ϊ�û��ļ�
	*  ����ֵ  : TiXmlNode* : ��ǰ���ڵ�
	* �������� : 
	* ��    ע : ��������ڵ㲻ͬ��Ҫ���ش˺���
	* ��    �� : lxqi
	* ������� : 2010-11-12
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual TiXmlNode*	GetRootNode(DocType nType = Default  );
	/********************************************************************************
	* �������� : ���ø��ڵ�
	* �������� : CString strFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 CString strAttrName : ��������
	*			 CString strAttrValue : ����ֵ
	*			 TiXmlNode* pRootNode : ָ�����ڵ�ָ��
	*			 DocType nType : ָ�������ļ���Ĭ��Ϊ�û��ļ�
	*  ����ֵ  : TiXmlNode* : ��ǰ���ڵ�
	* �������� : 
	* ��    ע : ��������ڵ㲻ͬ��Ҫ���ش˺���
	* ��    �� : lxqi
	* ������� : 2010-11-12
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL SetRootNode(const CString& strFindPath,
		const CString& strAttrName = "", 
		const CString& strAttrValuev = "", 
		DocType nType = Default  );
	virtual BOOL SetRootNode(TiXmlNode* pRootNode,DocType nType = Default  );
	//////////////////////////////////////////////////////////////////////////���¸����Һ���֧��xPath��׼���
	/********************************************************************************
	* �������� : ��ȡ���趨ָ���ڵ��
	* �������� : CString strFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 CString strNewValue : ���趨ֵ
	*			 int nIndex : ��ʼ�����ţ�0..n��
	*			 CString strAttrName : ��������
	*			 CString strAttrValue : ����ֵ
	*			 DocType nType : ָ�������ļ���Ĭ��Ϊ�Զ��������ң�User->System->Resource��
	*  ����ֵ  : CString : �ڵ�������ı�����
	*			 BOOL : �趨�ɹ���
	* �������� : 
	* ��    ע : 
	* ��    �� : lxqi 
	* ������� : 2010-12-1
	* �޸ļ�¼ : 
	*******************************************************************************/
//	virtual CString GetNodeData(TiXmlNode* pNode, DocType nType = Default  );
	virtual CString	GetNodeData(const CString& strFindPath,
		const CString& strAttrName = "", 
		const CString& strAttrValue = "", 
		DocType nType = Default  );
	virtual BOOL SetNodeData(TiXmlNode* pNode, const CString& strNewValue);
	virtual BOOL SetNodeData(const CString& strFindPath,
		const CString& strNewValue,
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* �������� : ��ȡ���趨ָ���ڵ����������ֵ
	* �������� : CString strpFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 CString strNewAttrName : ��Ҫ�趨ֵ��������
	*			 int nNewAttrValue : ���趨ֵ
	*			 int nDefault : ��ȡ���ɹ�ʱ��Ĭ��ֵ
	*			 CString strExtAttrName :  ���ڲ��ҵ���������
	*			 CString strExtAttrValue : ���ڲ��ҵ�����ֵ
	*			 DocType nType : ָ�������ļ���Ĭ��Ϊ�Զ��������ң�User->System->Resource��
	*  ����ֵ  : int : ��ȡ��ֵ��ʧ��ʱ����Ĭ��ֵ
	*			 BOOL : �趨�ɹ��񣬳ɹ�����TRUE,ʧ�ܷ���FALSE
	* �������� : 
	* ��    ע : 
	* ��    �� : lxqi 
	* ������� : 2010-11-16
	* �޸ļ�¼ : 2010-12-1 ʵ��by ����
	*******************************************************************************/
	//virtual int GetNodeAttrInt(TiXmlNode* pNode, CString strAttrName, int nDefault = IntDefault, DocType nType = Default);
	virtual int	GetNodeAttrInt(const CString& strFindPath,
		const CString& strAttrName, 
		int nDefault = IntDefault,
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "", 
		DocType nType = Default  );
	virtual BOOL SetNodeAttrInt(TiXmlNode* pNode, const CString& strNewAttrName, int nNewAttrValue);
	virtual BOOL SetNodeAttrInt(const CString& strFindPath, 
		const CString& strNewAttrName, 
		int nNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* �������� : ��ȡ���趨ָ���ڵ�ĸ�������ֵ
	* �������� : CString strpFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 CString strNewAttrName : ���趨������
	*			 double dNewAttrValue : ���趨ֵ
	*			 double dNewAttrValue : ��ȡ���ɹ�ʱ��Ĭ��ֵ
	*			 int nIndex : ��ʼ�����ţ�0..n��
	*			 CString strAttrName : ��������
	*			 CString strAttrValue : ����ֵ
	*			 DocType nType : ָ�������ļ���Ĭ��Ϊ�Զ��������ң�User->System->Resource��
	*  ����ֵ  : double : ��ȡ��ֵ��ʧ��ʱ����Ĭ��ֵ
	*			 BOOL : �趨�ɹ��񣬳ɹ�����TRUE,ʧ�ܷ���FALSE
	* �������� : 
	* ��    ע : Set��������Ҫ�ĵ����� ֻ���û��ļ����Ը���
	* ��    �� : lxqi
	* ������� : 2010-11-16
	* �޸ļ�¼ : 2010-12-1 ʵ��by ����
	*******************************************************************************/
	//virtual double GetNodeAttrDouble(TiXmlNode* pNode,CString strAttrName, double dDefault = DoubleDefault, DocType nType = Default);
	virtual double GetNodeAttrDouble(const CString& strFindPath,
		const CString& strAttrName = "", 
		double  dDefault = DoubleDefault,
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "", 
		DocType nType = Default);
	virtual BOOL SetNodeAttrDouble(TiXmlNode* pNode, const CString& strNewAttrName, double dNewAttrValue);
	virtual BOOL SetNodeAttrDouble(const CString& strFindPath, 
		const CString& strNewAttrName, 
		double dNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* �������� : ��ȡ���趨ָ���ڵ���ַ�������ֵ
	* �������� : CString strpFindPath : ·����Ϣ���á�/���ָ������������ڵ㣩
	*			 CString strNewAttrName : ���趨������
	*			 double dNewAttrValue : ���趨ֵ
	*			 double dNewAttrValue : ��ȡ���ɹ�ʱ��Ĭ��ֵ
	*			 int nIndex : ��ʼ�����ţ�0..n��
	*			 CString strAttrName : ��������
	*			 CString strAttrValue : ����ֵ
	*			 DocType nType : ָ�������ļ���Ĭ��Ϊ�Զ��������ң�User->System->Resource��
	*  ����ֵ  : double : ��ȡ��ֵ��ʧ��ʱ����Ĭ��ֵ
	*			 BOOL : �趨�ɹ��񣬳ɹ�����TRUE,ʧ�ܷ���FALSE
	* �������� : 
	* ��    ע : Set��������Ҫ�ĵ����� ֻ���û��ļ����Ը���
	* ��    �� : lxqi
	* ������� : 2010-11-16
	* �޸ļ�¼ : 2010-12-1 ʵ��by ����
	*******************************************************************************/
	//virtual CString GetNodeAttrString(TiXmlNode* pNode,CString strAttrName, CString strDefault = _T(""), DocType nType = Default);
	virtual CString GetNodeAttrString(const CString& strFindPath,
		const CString& strAttrName, 
		const CString& strDefault = "",
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "", 
		DocType nType = Default);
	virtual BOOL SetNodeAttrString(TiXmlNode* pNode, const CString& strNewAttrName, const CString& strNewAttrValue);
	virtual BOOL SetNodeAttrString(const CString& strFindPath, 
		const CString& strNewAttrName, 
		const CString& strNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* �������� : ��ȡ�ļ�����ָ��
	* �������� : DocType nType : ָ�������ļ���Ĭ��Ϊ�Զ��������ң�User->System->Resource��	 
	*  ����ֵ  : CMyXmlDocument* : �ļ�����ָ��
	* �������� : 
	* ��    ע : 
	* ��    �� : ����
	* ������� : 2010-12-15
	* �޸ļ�¼ : 
	*******************************************************************************/
	CMyXmlDocument* GetXmlDocument(DocType nType = Default);


protected:
	CMyXmlDocument	m_ResourceDoc;
	CMyXmlDocument	m_SystemDoc;
	CMyXmlDocument	m_UserDoc;

	BOOL			m_bInited;				// �Ƿ��ѳ�ʼ����

public:
	static IQuoteLang*		m_iQuoteLang;			// ������Դ��
	static ITradeLang*		m_iTradeLang;			// ������Դ��

};
