/*******************************************************
  源程序名称:StockToBankDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  证券转银行窗口类
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#pragma once
#include "BankTransferBaseDlg.h"

class CStockToBankDlg : public CBankTransferBaseDlg
{
	DECLARE_DYNAMIC(CStockToBankDlg)
public:
	CStockToBankDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStockToBankDlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual void ShowPassword();
private:
	DECLARE_MESSAGE_MAP()

};