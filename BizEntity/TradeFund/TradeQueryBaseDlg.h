/*******************************************************
  Դ��������:TradeQueryBaseDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ��ѯ���ڻ���
  ��    ��:  shenglq
  ��������:  20100910
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
	CTradeQueryBaseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeQueryBaseDlg();
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);
	virtual void SetSize();
//	virtual BOOL OnCmdMsg(
//		UINT nID,
//		int nCode,
//		void* pExtra,
//		AFX_CMDHANDLERINFO* pHandlerInfo 
//		);


	// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnLinkageMsg(CLinkageParam* pParam);
	virtual void OnShow(BOOL bShow);
	virtual void AsyncQuery();
	virtual void FillGrid();	
	//����˫���¼�
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