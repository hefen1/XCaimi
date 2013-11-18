/*******************************************************
  源程序名称:TradeQueryBaseDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  查询窗口基类
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#pragma once
#include "TradeFund.h"
#include "TradeDialogBase.h"
#include "TradeGridBase.h"
#include "TradingIntf.h"

class CTradeQueryBaseDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CTradeQueryBaseDlg)
public:
	CTradeQueryBaseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeQueryBaseDlg();
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);
	virtual void SetSize();
//	virtual BOOL OnCmdMsg(
//		UINT nID,
//		int nCode,
//		void* pExtra,
//		AFX_CMDHANDLERINFO* pHandlerInfo 
//		);


	// 对话框数据
	enum { IDD = IDD_DIALOG_FUNDQUERY};
	virtual DWORD GetDataType();
	virtual void OnReflash(IDataList * pDataList, DWORD dDataType); 
	virtual void OnChange(IDataItem * pDataItem, EDataChangeType dtChangeType, DWORD dDataType); 		
protected:
	ITradeTable* m_pTradeTable;
	CTradeGridBase* m_pGrid;
	IDataList* m_pDataList;
	DWORD m_nQueryBegin;
	INodeTreeData* m_EntrustStatusList;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual void OnLinkageMsg(CLinkageParam* pParam);
	virtual void OnShow(BOOL bShow);
	virtual void AsyncQuery();
	virtual void FillGrid();	
	//处理双击事件
	virtual void DoGridDBClick();
	virtual void DoDataCore(IDataList * pDataList);
	virtual void DoGridRClick();
	virtual void OpenLayOutMsg(CString sLayout);
	virtual void DoPopupMenuEntrust();
	virtual void SendCount(int nCount);

	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridDBClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	void OnClickPopupMenu(UINT Id);
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
//	virtual BOOL OnInitDialog();
};