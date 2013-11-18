#pragma once

#include "CombComm\CombManagerInterface.h"
#include "tinyxml.h"

class CWatcherInfo;
class CWatcherInfoManager;

class CCombComponent: public ICombComponent 
{
public:
	virtual CString GetCode();
	virtual int GetAmount();
	CCombComponent(CString strCode ,CString strName,int nAmount);
private:
	CString m_strCode;
	CString m_strName;
	int        m_nAmount;
};

typedef  CTypedPtrArray<CPtrArray,CCombComponent*>  CComponentArray;
class CCombInfo: public ICombInfo
{
public:
	CCombInfo(CString strName,int nAmount,CString m_strComponentFile);
	~CCombInfo();

	virtual CString    GetCombName();
	virtual int           GetAmount();
	virtual int           GetComponentCount();
	virtual ICombComponent* GetCombComponent(int nIndex);
	void AddComponent(CCombComponent *pComponent);
	BOOL LoadCombComponent(CString strComponentFile);
private:
	CString m_strCombName;
	CString strComponentFile;
	int m_nAmount; 
	CComponentArray  m_ayComponent;
};

typedef  CTypedPtrArray<CPtrArray,CCombInfo*>  CombInfoArray;
class CCombManager : public ICombManager
{
public:
	CCombManager(void);
	~CCombManager(void);

	virtual BOOL LoadCombs();
	virtual BOOL AddComb(CString strName,int nAmount,CString strComponentFile);
	virtual BOOL DelComb(CString strName);
	virtual ICombInfo* GetComb(CString strName);
	virtual ICombInfo* GetComb(int nIndex);
	virtual int GetCombCount();
private:
	CombInfoArray m_ayCombInfo;
};


class CParamInfo: public IParamInfo
{
public:
	friend class CWatcherInfoManager;
	friend class CWatcherParams;
	CParamInfo();
	virtual CString GetParamName();
	virtual char     GetParamType();
	virtual CString GetParamValue();
private:
	CString  m_strParamName;
	char      m_cParamType;
	CString  m_strParamValue;
};

typedef CTypedPtrArray<CPtrArray,CParamInfo*> CWatcherParamsArray;
class CWatcherParams: public IWatcherParams
{
public:
	friend class CWatcherInfoManager; 
	CWatcherParams();
	~CWatcherParams();
	virtual int GetParamCount();
	virtual IParamInfo* GetParamInfo(int nIndex);
	virtual IParamInfo* GetParamInfo(CString strParmaName);
	virtual void  UpdateParam(CString strParamName,char cType,CString strParamValue);
	virtual void Release();
private:
	CWatcherParamsArray  m_ayParams;	
};

class CWatcherInfo: public IWatcherInfo
{
public:
	friend class CWatcherInfoManager;
	virtual CString GetName();
	virtual void SetName(CString strName) { m_strName = strName; }
	virtual IWatcherParams*  GetParams();
	virtual void UpdateParam(CString strParamName,char cType,CString strParamValue);
	IParamInfo* GetParamInfo(CString strParamName);
	virtual void Release();
private:
	CString                     m_strName;
	CWatcherParams       m_params;
};

typedef CTypedPtrArray<CPtrArray,CWatcherInfo*> WatcherInfoArray;
class CWatcherInfoManager: IWatcherInfoManager
{
public:
	CWatcherInfoManager();
	~CWatcherInfoManager();
	virtual BOOL LoadWatchers(CString strWatcherSetName);
	virtual BOOL AddWatcherInfo(IWatcherInfo* pWatchInfo,CString strWatcherSetName);
	virtual BOOL ModifyWatcherInfo(IWatcherInfo* pWatchInfo,CString strWatcherSetName);
	virtual BOOL DelWatcherInfo(CString strWatcherSetName,CString strWatcherName);
	virtual IWatcherInfo* GetWatcherInfo(CString strName);
	virtual IWatcherInfo* GetWatcherInfo(int nIndex);
	virtual IWatcherInfo* CreateWathcerInfo();
	virtual int GetWatcherInfoCount();
	CString GetWatcherTypeName(CString strWatcherSetName);
	virtual void Release();
protected:
	BOOL              InitXmlDoc(TiXmlDocument &xmldoc);
 	TiXmlElement*  GetWatcherRootItem(TiXmlDocument &xmldoc,CString strWatcherSetName);
private:
	WatcherInfoArray   m_ayWatcherInfo;
};
