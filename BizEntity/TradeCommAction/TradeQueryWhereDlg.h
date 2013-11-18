/*******************************************************
  源程序名称:TradeQueryWhereDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  查询条件窗口基类
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#pragma once
#include "TradeCommAction.h"
#include "TradeDialogBase.h"
#include "afxwin.h"

class CTradeQueryWhereDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CTradeQueryWhereDlg)
public:
	CTradeQueryWhereDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeQueryWhereDlg();
	virtual void SetSize();

	// 对话框数据
	enum { IDD = IDD_DIALOG_QUERYWHERE};
	CDateTimeCtrl m_dtBeginDate;
	CDateTimeCtrl m_dtEndDate;
	CButton		  m_btQuery;
protected:
	BOOL m_bWithDrawShow;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual void OnShow(BOOL bShow);
	virtual BOOL Validate();
	virtual void OnLinkageMsg(CLinkageParam* pParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedQuery();
	CStatic m_stCount;
	CButton m_btWithDraw;
	CStatic m_stBeginDate;
	CStatic m_stEndDate;
	afx_msg void OnBnClickedBtWithdraw();
};