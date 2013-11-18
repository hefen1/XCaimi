#pragma once
#include "ConfigInterface.h"
#include "Resource.h"
#include "DataSourceDefineEx.h"


// CQuoteTabSetting 对话框

class CQuoteTabSetting : public CDialog
{
	DECLARE_DYNAMIC(CQuoteTabSetting)

public:
	CQuoteTabSetting(CWnd* pParent = NULL,CString strCurGroup = _T(""));   // 标准构造函数
	virtual ~CQuoteTabSetting();

// 对话框数据
	enum { IDD = IDD_QUOTE_TAB_SETTING };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	int		DelCurTabItem(CString strItemText);
	int		DelCurTabItem(int nIndex);
	int		AddCurTabItem(CString strItemText, DWORD_PTR pdwData = 0);
	BOOL	SaveCurTabGroup(CString strGroupName = _T(""));
	CString GetCurGroup(){return m_strCurGroup;}
	BOOL    IsSysGroup(CString strGroup);
	void    EnableControls(BOOL bEnable = TRUE);
protected:

	int m_nPreSel;
	int m_nGuideCount;
	CString m_strCurGroup;
	CStringArray m_aySysGroup;
	BOOL m_bSaveCurGroup;

	CTreeCtrl	m_wndAllTabItem;
	CListCtrl	m_wndCurTabItem;
	CComboBox	m_wndTabGroup;
	CButton		m_wndAddToCurGroup;
	CButton		m_wndDelFromCurGroup;
	CButton		m_wndDelAllCurTab;

	IDataSourceEx*    m_pDataSource;
	IHsTabCfg*        m_iTab;
	IBlockManager*    m_iBlockManager;

	afx_msg void OnCbnSelChangeTabGroup();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAddToCurrent();
	afx_msg void OnBnClickedDelFromCurrent();
	afx_msg void OnBnClickedDeleteall();
	afx_msg void OnBnClickedSaveCurgroup();
	afx_msg void OnBnClickedNewGroup();
	afx_msg void OnBnClickedRestoreCurgroup();
	afx_msg void OnBnClickedDelCurgroup();
	afx_msg void OnNMDblclkAlltabitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTabitem(NMHDR *pNMHDR, LRESULT *pResult);
};
