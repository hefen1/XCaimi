#pragma once
#include "quotelangmnginterface.h"

class CQuoteLang :
	public IQuoteLang
{
public:
	CQuoteLang(void);
	~CQuoteLang(void);

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
	// ��ȡͼƬ
	HANDLE LoadHImage(UINT nIDResource,
		UINT uType,
		int cxDesired,
		int cyDesired,
		UINT fuLoad);
	HANDLE LoadHImage(HINSTANCE hinst,
		UINT nIDResource,
		UINT uType,
		int cxDesired,
		int cyDesired,
		UINT fuLoad);
	//��ȡ�˵���Դ
	virtual HMENU	LoadMenu(const UINT nIDResource);
	virtual HMENU	LoadMenu(const HINSTANCE hInstance, const UINT nIDResource);

};
