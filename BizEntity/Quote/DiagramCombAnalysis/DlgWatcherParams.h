#pragma once
#include "CombComm\AnalysisTable.h"
#include "CombComm\CombManagerInterface.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "afxpropertygridctrl.h"
#include "afxmenubutton.h"


// CDlgWatcherParams 对话框

class CDlgWatcherParams : public CDialog
{
	DECLARE_DYNAMIC(CDlgWatcherParams)

public:
	CDlgWatcherParams(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWatcherParams();
// 对话框数据
	enum { IDD = IDD_WATHCER_PARAMS };
	IWatcherInfo*    m_pWatcherInfo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
private:
	IAnalysisTable*               m_pAnalysisTable;
	CMapStringToPtr            m_watherParams;
	CMFCPropertyGridCtrl     m_wndPropList;
	CEdit                              m_edtWatcherName;
	CRichEditCtrl                   m_rcedtFormula;
	CMenu                           m_indicatorMenu;
	CMFCMenuButton           m_btnMenu;
	CStatic                            m_stPropertyPos;

	void AdjustLayout();
	void InitPropStyle();
	void InitInidicatorBtnMenu();
	void FillParamsProperty(CString strExpName = _T(""));
	BOOL UpdateWatchInfo();

public:
	//设置表格信息
	void SetAnalysisTableObj(IAnalysisTable* pAnalysisTable )  { m_pAnalysisTable = pAnalysisTable;}
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnBnClickedBtnIndicator();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancle();
	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);
};
