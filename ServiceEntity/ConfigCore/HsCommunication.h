/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsColor.h
* ժ    Ҫ����¼��ͨѶ����ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-27
* ��	 ע��
*******************************************************************************/
#pragma once
#include "ConfigInterface.h"
#include "ConfigBase.h"

#define AccNodeValue    "AccInfo"
#define HeartBeat_MIN       10000
class CHsCommunication : public IHsCommunication
{
public:
	CHsCommunication(void);
	~CHsCommunication(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

public:
	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

private:
	// ��Server_typeת��Ϊ�ַ�
	CString GetTypeName(Server_type type);
	// ���ַ�ת��ΪServer_type
	Server_type GetServerType(CString strType);

    // ͨ�����ڵ�(pNode) ��ȡ�������������ݱ�����pAy�� 
	void GetServer(Server_type type, TiXmlNode* pNode, CCommInfoArray *pAy);
// ����ӿ�
public:
	virtual int GetSysServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1);    // ��ȡָ�����͵�ϵͳĬ��վ��
 	virtual int GetUsrServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1);    // ��ȡָ�����͵��û��Զ���վ��
	virtual int GetAllSysServer(CCommInfoArray *pAy, int nCellID);   // ��ȡ�������͵�ϵͳĬ��վ��
	virtual int GetAllUsrServer(CCommInfoArray *pAy, int nCellID);   // ��ȡ�������͵��û��Զ���վ��
 	virtual void SaveUsrServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1);  // ����ָ�����͵��û��Զ���վ��
 	virtual void SaveAllUsrServer(CCommInfoArray *pAy, int nCellID);  // �����������͵��û��Զ���Ĭ��վ��
 	virtual BOOL GetProxy(ProxyInfo *proxy); // ��ȡ������Ϣ
	virtual void SaveProxy(ProxyInfo *proxy);// ���������Ϣ

 	virtual BOOL GetAuto();                  // ��ȡ�Զ����ٿ���
 	virtual BOOL SetAutoChooseBest(UINT au); // ��ȡ�Զ��������վ�㿪��

	virtual int  GetHeartBeat(Server_type type); // ��ȡ�������

 	virtual BOOL GetSwitch(Server_type type); // ��ȡָ��������ʾ����
	virtual void GetAllSwitch(CMap<Server_type,Server_type,BOOL,BOOL> *pAy);  // ��ȡ����������ʾ����
 	virtual CString GetDefaultSite(Server_type type, int nCellID = -1); // ��ȡָ������Ĭ��վ��
	virtual void    GetAllDefaultSite(CMap<Server_type,Server_type,CString,CString> *pAy, int nCellID); // ��ȡ��������Ĭ��վ��
	virtual void    SetDefaultSite(Server_type type, CCommInfoArray *pSysAy, CCommInfoArray *pUsrAy, int nCellID = -1); // ����ָ������Ĭ��վ��
	virtual void    SetAllDefaultSite(CCommInfoArray *pSysAy, CCommInfoArray *pUsrAy, int nCellID);     // ������������Ĭ��վ��
	
	virtual void    RemoveSysMap(Server_type type, int nCellID); // �Ƴ�map������
	virtual void    RemoveUsrMap(Server_type type, int nCellID); // �Ƴ�map������

	virtual int     GetSaveAccount();                  // ��ȡ�����˺ſ���
	virtual BOOL    SetSaveAccount(UINT unSave);       // ���ñ����˺ſ���
	virtual int     GetHideAccount();                  // ��ȡ�����˺ſ���
	virtual BOOL    SetHideAccount(UINT unHide);       // ���������ʺſ���
	virtual BOOL    GetLevelAcc(CString &accName, CString &accPwd);  // ��ȡ����Level2�˺š�����
	// ��ȡ/�����˺���Ϣ ayAccInfo�а��˺����ơ��˺����͡�Ӫҵ��ID��� nAccCount��ʾ�洢�˺Ÿ���
	virtual int     GetAccount(CStringArray& ayAccInfo);
	virtual BOOL    SaveAccount(CStringArray& ayAccInfo, int nAccCount); 
	virtual BOOL    RemoveAllAccount(); 
private:
	BOOL m_bInited;
	CConfigBase* m_pConfigBase;

	CMapStringToPtr m_mapSysServer;  // key: Server_type ToString; value: CCommInfoArray
	CMapStringToPtr m_mapUserServer; // key: Server_type ToString; value: CCommInfoArray
};
