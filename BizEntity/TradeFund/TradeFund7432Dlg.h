/*******************************************************
  源程序名称:TradeFund7432Dlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  基金定投
  作    者:  shenglq
  开发日期:  20110216
*********************************************************/
#pragma once
#include "TradeFundBaseDlg.h"

class CTradeFund7432Dlg : public CTradeFundBaseDlg
{
	DECLARE_DYNAMIC(CTradeFund7432Dlg)
public:
	CTradeFund7432Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeFund7432Dlg();
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual void OnFundCodeExit();
	virtual void DoEntrust();
	virtual void LoadWnd();
	virtual BOOL Validate();
	virtual void OnCodeChangedFocus();

	DECLARE_MESSAGE_MAP()

};