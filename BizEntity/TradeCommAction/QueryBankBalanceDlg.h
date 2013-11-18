/*******************************************************
  源程序名称:QueryBankBalanceDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  查询银行余额窗口类
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#pragma once
#include "BankTransferBaseDlg.h"

class CQueryBankBalanceDlg : public CBankTransferBaseDlg
{
	DECLARE_DYNAMIC(CQueryBankBalanceDlg)
public:
	CQueryBankBalanceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQueryBankBalanceDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual void LoadWnd();
	virtual void ShowPassword();
private:
	DECLARE_MESSAGE_MAP()

};