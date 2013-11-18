#pragma once

class CModuleInfo
{
private:
	CString            m_strName;  //Ä£¿éÃû³Æ
	HINSTANCE          m_hModule;   //Ä£¿é¾ä±ú
public:
	CModuleInfo(CString strName,HINSTANCE hModule)
	{
		m_strName = strName;
		m_hModule = hModule;
	}

	CString GetName()
	{
		return m_strName;
	}

	HINSTANCE GetHandle()
	{
		return m_hModule;
	}
};

class CModuleLoader: public CObject 
{
protected:
	CList<CModuleInfo*,CModuleInfo*> m_moduleList;
	CMutex m_mutex;

	HINSTANCE LoadModule(CString strName);
	HINSTANCE FindModule(CString strName);
	BOOL AddModuleInfo(CString strName,HINSTANCE hModule);
public:
	static const CString m_strClassName;
	CModuleLoader(void);
	~CModuleLoader(void);

	HINSTANCE GeModuleHandle(CString strName);	
};
