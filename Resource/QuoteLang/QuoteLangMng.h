#pragma once
#include "quotelangmnginterface.h"

class CQuoteLang :
	public IQuoteLang
{
public:
	CQuoteLang(void);
	~CQuoteLang(void);

	void Release();
	//加载 图标
	HICON LoadHICON(const UINT nIDResource);
	HICON LoadHICON(const HINSTANCE hInstance, const UINT nIDResource);
	//读取XML
	char* LoadXML(const UINT nIDResource);
	char* LoadXML(const HINSTANCE hInstance, const UINT nIDResource);
	//获取字符串
	CString LoadStr(const UINT nIDResource);
	CString LoadStr(const HINSTANCE hInstance, const UINT nIDResource);
	// 获取图片
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
	//获取菜单资源
	virtual HMENU	LoadMenu(const UINT nIDResource);
	virtual HMENU	LoadMenu(const HINSTANCE hInstance, const UINT nIDResource);

};
