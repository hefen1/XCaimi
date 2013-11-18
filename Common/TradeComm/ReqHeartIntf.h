/*******************************************************
  源程序名称:ReqHeartIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  心跳接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqHeart : virtual public ITrading
{
	virtual CString GetVersion() = 0;
	virtual int GetClientAdd() = 0;
	virtual CString GetJrName() = 0;
	virtual CString GetUpdateCrc() = 0;
	virtual int GetOnlineNums() = 0;
	virtual int GetMaxNums() = 0;
};