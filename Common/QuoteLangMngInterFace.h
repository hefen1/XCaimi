#pragma once
#define OBJ_QUOTELANG   _T("QuoteLang.CQuoteLang")
struct IQuoteLang 
{
	//释放
	virtual void Release() = 0;
	//加载 图标
	virtual HICON LoadHICON(const UINT nIDResource) = 0;
	virtual HICON LoadHICON(const HINSTANCE hInstance, const UINT nIDResource) = 0;
	//读取XML
	virtual char* LoadXML(const UINT nIDResource) = 0;
	virtual char* LoadXML(const HINSTANCE hInstance, const UINT nIDResource) = 0;
	//获取字符串
	virtual CString LoadStr(const UINT nIDResource) = 0;
	virtual CString LoadStr(const HINSTANCE hInstance, const UINT nIDResource) = 0;
	// 获取图片
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
	//获取菜单资源
	virtual HMENU	LoadMenu(const UINT nIDResource) = 0;
	virtual HMENU	LoadMenu(const HINSTANCE hInstance, const UINT nIDResource) = 0;
};