/*******************************************************
  源程序名称:QueryBaseIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易查询基接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqQueryBase : virtual public ITrading
{
	//开始日期
	virtual void SetBeginDate(const CString sValue) = 0;
	//结束日期
	virtual void SetEndDate(const CString sValue) = 0;
};