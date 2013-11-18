/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsColor.h
* 摘    要：登录框通讯配置实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-27
* 备	 注：
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
	// 将Server_type转化为字符
	CString GetTypeName(Server_type type);
	// 将字符转化为Server_type
	Server_type GetServerType(CString strType);

    // 通过父节点(pNode) 获取其所有子类数据保存在pAy中 
	void GetServer(Server_type type, TiXmlNode* pNode, CCommInfoArray *pAy);
// 对外接口
public:
	virtual int GetSysServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1);    // 获取指定类型的系统默认站点
 	virtual int GetUsrServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1);    // 获取指定类型的用户自定义站点
	virtual int GetAllSysServer(CCommInfoArray *pAy, int nCellID);   // 获取所有类型的系统默认站点
	virtual int GetAllUsrServer(CCommInfoArray *pAy, int nCellID);   // 获取所有类型的用户自定义站点
 	virtual void SaveUsrServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1);  // 保存指定类型的用户自定义站点
 	virtual void SaveAllUsrServer(CCommInfoArray *pAy, int nCellID);  // 保存所有类型的用户自定义默认站点
 	virtual BOOL GetProxy(ProxyInfo *proxy); // 获取代理信息
	virtual void SaveProxy(ProxyInfo *proxy);// 保存代理信息

 	virtual BOOL GetAuto();                  // 获取自动测速开关
 	virtual BOOL SetAutoChooseBest(UINT au); // 获取自动连接最快站点开关

	virtual int  GetHeartBeat(Server_type type); // 获取心跳间隔

 	virtual BOOL GetSwitch(Server_type type); // 获取指定类型显示开关
	virtual void GetAllSwitch(CMap<Server_type,Server_type,BOOL,BOOL> *pAy);  // 获取所有类型显示开关
 	virtual CString GetDefaultSite(Server_type type, int nCellID = -1); // 获取指定类型默认站点
	virtual void    GetAllDefaultSite(CMap<Server_type,Server_type,CString,CString> *pAy, int nCellID); // 获取所有类型默认站点
	virtual void    SetDefaultSite(Server_type type, CCommInfoArray *pSysAy, CCommInfoArray *pUsrAy, int nCellID = -1); // 设置指定类型默认站点
	virtual void    SetAllDefaultSite(CCommInfoArray *pSysAy, CCommInfoArray *pUsrAy, int nCellID);     // 设置所有类型默认站点
	
	virtual void    RemoveSysMap(Server_type type, int nCellID); // 移出map中内容
	virtual void    RemoveUsrMap(Server_type type, int nCellID); // 移出map中内容

	virtual int     GetSaveAccount();                  // 获取保存账号开关
	virtual BOOL    SetSaveAccount(UINT unSave);       // 设置保存账号开关
	virtual int     GetHideAccount();                  // 获取隐藏账号开关
	virtual BOOL    SetHideAccount(UINT unHide);       // 设置隐藏帐号开关
	virtual BOOL    GetLevelAcc(CString &accName, CString &accPwd);  // 获取配置Level2账号、密码
	// 获取/保存账号信息 ayAccInfo中按账号名称、账号类型、营业部ID存放 nAccCount表示存储账号个数
	virtual int     GetAccount(CStringArray& ayAccInfo);
	virtual BOOL    SaveAccount(CStringArray& ayAccInfo, int nAccCount); 
	virtual BOOL    RemoveAllAccount(); 
private:
	BOOL m_bInited;
	CConfigBase* m_pConfigBase;

	CMapStringToPtr m_mapSysServer;  // key: Server_type ToString; value: CCommInfoArray
	CMapStringToPtr m_mapUserServer; // key: Server_type ToString; value: CCommInfoArray
};
