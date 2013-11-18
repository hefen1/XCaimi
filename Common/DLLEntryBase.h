/*******************************************************
  源程序名称:DLLEntryBase.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  DLL Entry Base Class
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once

class CDLLEntryBase
{
public:
	CDLLEntryBase();
	~CDLLEntryBase();
protected:
	virtual CString GetDLLFileName() = 0;
	FARPROC GetLibProcAddress(const CString sProcName);
private:
	HINSTANCE GetDLLhinst();
	HINSTANCE m_hinstDLL; 

};