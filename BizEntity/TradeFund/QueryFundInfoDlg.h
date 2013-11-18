/*******************************************************
  源程序名称:QueryFundInfoDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeFund
  功能说明:  查询基金信息
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQueryFundInfoDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQueryFundInfoDlg)
public:
	CQueryFundInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQueryFundInfoDlg();
protected:
	virtual BOOL OnInit();
	virtual void OnShow(BOOL bShow);
	virtual void OnLinkageMsg(CLinkageParam* pParam);
private:
	IFundInfoList* m_pFundInfoList;
};