/*******************************************************
  源程序名称:QueryBase.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易查询基类实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "QueryBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CReqQueryBase::SetBeginDate(const CString sValue)
{
	CTrading::SetRequestARData("begin_date",sValue);
	CTrading::SetRequestARData("start_date",sValue);
}

void CReqQueryBase::SetEndDate(const CString sValue)
{
	CTrading::SetRequestARData("end_date", sValue);
}