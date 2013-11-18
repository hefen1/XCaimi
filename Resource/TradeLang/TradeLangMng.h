/*******************************************************
  源程序名称:TradeLangMng.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易资源
  功能说明:  资源管理器类
  作    者:  shenglq
  开发日期:  20100921
*********************************************************/
#pragma once
#include "TradeLangMngIntf.h"

class CTradeLang : public ITradeLang
{
public:
	CTradeLang();
	~CTradeLang();
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
};