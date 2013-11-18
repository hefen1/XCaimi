/*******************************************************
  源程序名称:ReqHeart.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  心跳类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "..\..\Common\TradeComm\ReqHeartIntf.h"
#pragma warning (disable:4250)

class CReqHeart : public CTrading, public IReqHeart
{
	CString GetVersion();
	int GetClientAdd();
	CString GetJrName();
	CString GetUpdateCrc();
	int GetOnlineNums();
	int GetMaxNums();
};