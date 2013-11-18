#pragma once

class CPanelInfo
{
public:
	friend class CPanelInfoManager;
	CString    m_strPostion;
	CString    m_strCaption;
	CString    m_strLayoutName;
	CString    m_strGrant;
	CSize      m_size;
	BOOL	   m_bMaximize;

	CPanelInfo(CString strName = _T(""))
	{
		m_strName = strName;
	}
	CString    GetPanelName() { return this->m_strName; }

	void Copy(CPanelInfo* pInfo)
	{
		m_strPostion= pInfo->m_strPostion;
		m_strCaption = pInfo->m_strCaption;
		m_strLayoutName = pInfo->m_strLayoutName;
		m_size = pInfo->m_size;
		m_bMaximize = pInfo->m_bMaximize;
		m_strName = pInfo->GetPanelName();
	}
private:
	CString    m_strName;     //Ãæ°åÃû³Æ
};

class CPanelInfoManager:public CObject
{
private:
	CMapStringToOb m_mapPanelInfo;
public:
	CPanelInfoManager();
	~CPanelInfoManager();

	BOOL LoadAllPanelInfo(CString strFileName);
	void AddPanelInfo(CString strPanelName,CPanelInfo* pInfo);
	CPanelInfo* GetPanelInfo(CString strPanelName);
};
