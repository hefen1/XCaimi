#pragma once

#define OJB_WATCHERINFOMANAGER                             _T("AnalysisCore.CWatcherInfoManager")

struct ICombComponent
{
	virtual CString GetCode() = 0;
	virtual int GetAmount() = 0;
};

struct ICombInfo
{
	virtual CString    GetCombName() = 0;
	virtual int           GetAmount() = 0;
	virtual int           GetComponentCount() = 0;
	virtual ICombComponent* GetCombComponent(int nIndex) = 0;
};

struct ICombManager
{
	virtual BOOL LoadCombs() = 0;
	virtual BOOL AddComb(CString strName,int nAmount,CString strComponentFile) = 0;
	virtual BOOL DelComb(CString strName) = 0;
	virtual ICombInfo* GetComb(CString strName) = 0;
	virtual ICombInfo* GetComb(int nIndex) = 0;
	virtual int GetCombCount() = 0;
};

struct IParamInfo
{
	virtual CString GetParamName() = 0;
	virtual char     GetParamType()=0;
	virtual CString GetParamValue()=0;
};

struct IWatcherParams
{
	virtual int GetParamCount() = 0;
	virtual IParamInfo* GetParamInfo(int nIndex) = 0;
	virtual IParamInfo* GetParamInfo(CString strParmaName) = 0;
	virtual void Release() = 0;
	virtual void UpdateParam(CString strParamName,char cType,CString strParamValue)=0;
};

struct  IWatcherInfo
{
	virtual CString GetName() = 0 ;
	virtual void SetName(CString strName) = 0;
	virtual IWatcherParams*  GetParams() = 0;
	virtual IParamInfo* GetParamInfo(CString strParmaName) = 0;
	virtual void UpdateParam(CString strParamName,char cType,CString strParamValue) = 0;
	virtual void Release() = 0;
};

struct IWatcherInfoManager
{
	virtual BOOL LoadWatchers(CString strWatchSetName) = 0;
	virtual BOOL AddWatcherInfo(IWatcherInfo* pWatchInfo,CString strWatchSetName) = 0;
	virtual BOOL ModifyWatcherInfo(IWatcherInfo* pWatchInfo,CString strWatchSetName) = 0;
	virtual BOOL DelWatcherInfo(CString strWatcherName,CString strWatchSetName) = 0;
	virtual IWatcherInfo* GetWatcherInfo(CString strName) = 0;
	virtual IWatcherInfo* GetWatcherInfo(int nIndex) = 0;
	virtual IWatcherInfo* CreateWathcerInfo() = 0;
	virtual int GetWatcherInfoCount() = 0;
	virtual void Release() = 0;
};