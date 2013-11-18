/*******************************************************
  Դ��������:TradeLangMngIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ������Դ
  ����˵��:  ��Դ�������ӿ�
  ��    ��:  shenglq
  ��������:  20100921
*********************************************************/
#pragma once
#define OBJ_TRADELANG   _T("TradeLang.TradeLang")
struct ITradeLang 
{
	//�ͷ�
	virtual void Release() = 0;
	//���� ͼ��
	virtual HICON LoadHICON(const UINT nIDResource) = 0;
	virtual HICON LoadHICON(const HINSTANCE hInstance, const UINT nIDResource) = 0;
	//��ȡXML
	virtual char* LoadXML(const UINT nIDResource) = 0;
	virtual char* LoadXML(const HINSTANCE hInstance, const UINT nIDResource) = 0;
	//��ȡ�ַ���
	virtual CString LoadStr(const UINT nIDResource) = 0;
	virtual CString LoadStr(const HINSTANCE hInstance, const UINT nIDResource) = 0;
};