/*******************************************************
  Դ��������:DLLEntryBase.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  DLL Entry Base Class
  ��    ��:  shenglq
  ��������:  20100720
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