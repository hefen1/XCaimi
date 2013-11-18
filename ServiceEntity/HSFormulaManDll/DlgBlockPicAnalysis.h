/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgBlockPicAnalysis.h
*	文件标识：	
*	摘	  要：	"板块分析"功能模块---"图形分析"
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/

#if !defined(AFX_HSPICANALYSEDLG1_H__659C82FA_7E89_4E0C_B28F_D03DA2102CF9__INCLUDED_)
#define AFX_HSPICANALYSEDLG1_H__659C82FA_7E89_4E0C_B28F_D03DA2102CF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HSPicAnalyseDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockPicAnalysis dialog
#include "resource.h"
#include "DlgBlockAnalysis.h"


#define		HS_PIC_ROWNUM	100
#define		HS_X_NUM		4
#define		HS_PIC_COLNUM	100
#define		HS_Y_NUM		10
#define		HS_MAX_VAL		99999
#define		HS_COL_PIX		35
#define		HS_ROW_PIX		20
#define		HS_RIGHT_W		5
#define		HS_TOP_W		5


/*************************************************************
*	类 名 称：	CDlgBlockPicAnalysis
*	类 描 述：	板块分析中图形分析模块
***************************************************************/
class CDlgBlockPicAnalysis : public CDialog
{
// Construction
public:
	CDlgBlockPicAnalysis(CWnd* pParent = NULL, TYPEANDBLOCK* pList = NULL,
		TabStockSortData* pIndex = NULL, int iItem = 0, int iSubItem = 2);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDlgBlockPicAnalysis)
	enum { IDD = IDD_PIC_ANALYSE };
	// 股票列表
	CListBox	m_ListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBlockPicAnalysis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// 分类计算数据85170160,
	TYPEANDBLOCK*	m_pList;
	// 排序数据
	TabStockSortData* m_pIndex;
	// 画线的底色
	COLORREF		m_colorref;
	// 窗口标题
	CString			m_strWinText;
	// 画图标线
	int				m_iCurLine, m_iCurCol;
	// 当前股票索引
	int				m_iCurStock;
	// 本块要画线的数据
	CArray<int, int>	ayBlockData;
	// 本类要画线的数据
	CArray<int, int>	ayListData;
	// 本块的最大X值
	int					m_iMaxBlockX;
	// 本类的最大X值
	int					m_iMaxListX;
	// Y方向最大最小值
	double				m_dMinVal;
	double				m_dMaxVal;
	// 画线区
	CRect				m_DrawRect;
	// 每个单位的点数
	double					m_iY;				
	double					m_iX;

	// 计算出Y轴方向的最大最小值
	void GetMinMaxVal();
	// 计算出X轴方向的最大最小值
	void GetMaxX();
	// 设置m_iY, m_...X, m_DrawRect及各方向的最大最小值
	void Init();		
	// 初始化当前分类块数据
	void InitListData();
	// 初始化当前板块数据
	void InitBlockData();
	// 画图
	void DrawPic();
	// 画股票及其数据的列表
	void AddCommbo();
	// 点击股票列表中股票时,做的处理
	void SelectCurStock();
	// Generated message map functions
	//{{AFX_MSG(CDlgBlockPicAnalysis)
	virtual BOOL OnInitDialog();
	// 点击股票列表框
	afx_msg void OnDblclkListBox();
	// 刷新窗口
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// 退出
	virtual void OnCancel();
	virtual void PostNcDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSPICANALYSEDLG1_H__659C82FA_7E89_4E0C_B28F_D03DA2102CF9__INCLUDED_)
