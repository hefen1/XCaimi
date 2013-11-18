/*******************************************************
  Դ��������:TradeLangMng.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ������Դ
  ����˵��:  ��Դ��������
  ��    ��:  shenglq
  ��������:  20100921
*********************************************************/
#pragma once
#include "TradeLangMngIntf.h"

class CTradeLang : public ITradeLang
{
public:
	CTradeLang();
	~CTradeLang();
	void Release();
	//���� ͼ��
	HICON LoadHICON(const UINT nIDResource);
	HICON LoadHICON(const HINSTANCE hInstance, const UINT nIDResource);
	//��ȡXML
	char* LoadXML(const UINT nIDResource);
	char* LoadXML(const HINSTANCE hInstance, const UINT nIDResource);
	//��ȡ�ַ���
	CString LoadStr(const UINT nIDResource);
	CString LoadStr(const HINSTANCE hInstance, const UINT nIDResource);
};