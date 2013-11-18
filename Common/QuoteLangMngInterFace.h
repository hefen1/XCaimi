#pragma once
#define OBJ_QUOTELANG   _T("QuoteLang.CQuoteLang")
struct IQuoteLang 
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
	// ��ȡͼƬ
	virtual HANDLE LoadHImage(UINT nIDResource,
		UINT uType,
		int cxDesired,
		int cyDesired,
		UINT fuLoad)=0;
	virtual HANDLE LoadHImage(HINSTANCE hinst,
		UINT nIDResource,
		UINT uType,
		int cxDesired,
		int cyDesired,
		UINT fuLoad)=0;
	//��ȡ�˵���Դ
	virtual HMENU	LoadMenu(const UINT nIDResource) = 0;
	virtual HMENU	LoadMenu(const HINSTANCE hInstance, const UINT nIDResource) = 0;
};