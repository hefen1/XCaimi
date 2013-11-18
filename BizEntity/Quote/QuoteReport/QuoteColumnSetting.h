#include "HSBizDialog.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ConfigInterface.h"

#pragma once
// CQuoteColumnSetting 对话框

class CQuoteColumnSetting : public CDialog
{
	DECLARE_DYNAMIC(CQuoteColumnSetting)

public:
	CQuoteColumnSetting(CString strCurGroup = _T(""),CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQuoteColumnSetting();

// 对话框数据
	enum { IDD = IDD_QUOTE_COLUMN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	BOOL	DelCurColumn(CString strName);
	BOOL	ChangeCurColGroup(CString strGroupName = _T(""));
	BOOL	ChangeColGroup(CString strGroupName = _T(""));
	BOOL	SaveCurGroup(CString strGroupName = _T(""));

	int		InsertToCurrent(CString strName, int nID);
public:
	//IColumnProperty* m_iCol;
	IHsColumnProperty* m_iCol;

	CComboBox	m_wndAllColGroup;
	CListBox	m_wndGroupColumn;
	CListCtrl	m_wndCurGroupCol;
	CComboBox	m_wndCurGroup;
	int			m_nPreCurGroup;
	int			m_nPreAllGroup;
	BOOL		m_bCurChanged;
	BOOL		m_bColPropChanged;
	CString		m_strCurReportGroup;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddtocurrent();
	afx_msg void OnBnClickedAddalltocurrent();
	afx_msg void OnBnClickedDelfromcurrent();
	afx_msg void OnBnClickedDelallfromcurrent();
	afx_msg void OnBnClickedNewColumn();
	afx_msg void OnBnClickedDelColumn();
	afx_msg void OnBnClickedColumnProperty();
	afx_msg void OnBnClickedSaveCurgroup();
	afx_msg void OnBnClickedSaveasCurgroup();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLvnBegindragCurcolumn(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkCurcolumn(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnDblclkAllColumn();
	afx_msg void OnBnClickedDelCurDispcolGroup();
	afx_msg void OnCbnSelchangeColgroup();
	afx_msg void OnCbnSelchangeAllColgroup();
};
