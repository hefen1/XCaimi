/*******************************************************
  源程序名称:TradeCommQueryDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  查询窗口基类
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#pragma once
#include "TradeCommAction.h"
#include "TradeDialogBase.h"
#include "TradeGridBase.h"
#include "QueryBaseIntf.h"

class TradeCommQueryDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(TradeCommQueryDlg)
public:
	TradeCommQueryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TradeCommQueryDlg();
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);
	virtual void SetSize();
//	virtual BOOL OnCmdMsg(
//		UINT nID,
//		int nCode,
//		void* pExtra,
//		AFX_CMDHANDLERINFO* pHandlerInfo 
//		);


	// 对话框数据
	enum { IDD = IDD_DIALOG_QUERYGRID};
	
protected:
	ITradeTable*		m_pTradeTable;
	CTradeGridBase*		m_pGrid;
	IDataList*			m_pDataList;
	DWORD				m_nQueryBegin;
	CString				m_sBeginDate;
	CString				m_sEndDate;
	BOOL				m_bShowQuery;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual void OnLinkageMsg(CLinkageParam* pParam);
	virtual void OnShow(BOOL bShow);
	virtual void SyncQuery();
	virtual void AsyncQuery();
	virtual void FillGrid();	
	//检查委托数据
	virtual BOOL Validate();
	virtual void DoEntrust();
	virtual void SendCount(int nCount);

	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridDBClick(NMHDR *pNotifyStruct, LRESULT* pResult);
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
//	virtual BOOL OnInitDialog();
};