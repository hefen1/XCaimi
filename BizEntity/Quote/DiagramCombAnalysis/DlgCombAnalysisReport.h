#pragma once
#include "HSBizDialog.h"
#include ".\CombComm\AnalysisTable.h"
#include ".\CombComm\CombManagerInterface.h"
#include ".\Watcher.h"
#include ".\GridCtrl\YTGridCtrl.h"
#include "DataSourceDefine.h"
#include "DataSourceDefineEx.h"

#define Report_Grid_ID            100

//Grid右键菜单
#define GRID_MENU_AddWatcher             100
#define GRID_MENU_ModifyWatcher         101
#define GRID_MENU_DelWathcher             102
#define GRID_MENU_Refresh                     103

// CDlgCombAnalysisReport 对话框

class CDlgCombAnalysisReport : public CHSBizDialog
{
	DECLARE_DYNAMIC(CDlgCombAnalysisReport)

public:
	CDlgCombAnalysisReport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCombAnalysisReport();
// 对话框数据
	enum { IDD = IDD_COMBANALYSIS_REPROT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL OnInit();
	void InitAnalysisGridData();
	void InitAnalysisGridStyle();
	void ReflashGrid();
	void NotifyDragramRefresh();
	void NotifyWatcherChanged(CWatcher* pWatcher);
	void AddWatcher(IWatcherInfo* pWatcherInfo);
	void RemoveWatcher(int nRow);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	CYTGridCtrl*                   m_pCombGrid;
	IAnalysisTable*               m_pAnalysisTable;
	IWatcherInfoManager*    m_pWatcherInfoMgr;
	CString                          m_strWatcherSetName;
	CWatcherSet                  m_watcherSet;

	void NotifySingleIndicatrixChange(NM_GRIDVIEW * pMNGird);
	void NotifyMultiIndicatrixChange(NM_GRIDVIEW * pMNGird);
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAnalysisGridRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAnalysisGridDBClick(NMHDR *pNMHDR, LRESULT *pResult);
};
