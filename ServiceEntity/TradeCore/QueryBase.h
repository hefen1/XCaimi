/*******************************************************
  源程序名称:QueryBase.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易查询基类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "QueryBaseIntf.h"
#pragma warning (disable:4250)

class CReqQueryBase : public CTrading, virtual public IReqQueryBase
{	
public:
	//开始日期
	void SetBeginDate(const CString sValue);
	//结束日期
	void SetEndDate(const CString sValue);
};