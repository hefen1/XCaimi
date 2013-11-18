/*******************************************************
  源程序名称:TradeLangMngIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易资源
  功能说明:  资源管理器接口
  作    者:  shenglq
  开发日期:  20100921
*********************************************************/
#pragma once
#define OBJ_TRADELANG   _T("TradeLang.TradeLang")
struct ITradeLang 
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
};