/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgBlockPicAnalysis.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	"������"����ģ��---"ͼ�η���"
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
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
*	�� �� �ƣ�	CDlgBlockPicAnalysis
*	�� �� ����	��������ͼ�η���ģ��
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
	// ��Ʊ�б�
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
	// �����������85170160,
	TYPEANDBLOCK*	m_pList;
	// ��������
	TabStockSortData* m_pIndex;
	// ���ߵĵ�ɫ
	COLORREF		m_colorref;
	// ���ڱ���
	CString			m_strWinText;
	// ��ͼ����
	int				m_iCurLine, m_iCurCol;
	// ��ǰ��Ʊ����
	int				m_iCurStock;
	// ����Ҫ���ߵ�����
	CArray<int, int>	ayBlockData;
	// ����Ҫ���ߵ�����
	CArray<int, int>	ayListData;
	// ��������Xֵ
	int					m_iMaxBlockX;
	// ��������Xֵ
	int					m_iMaxListX;
	// Y���������Сֵ
	double				m_dMinVal;
	double				m_dMaxVal;
	// ������
	CRect				m_DrawRect;
	// ÿ����λ�ĵ���
	double					m_iY;				
	double					m_iX;

	// �����Y�᷽��������Сֵ
	void GetMinMaxVal();
	// �����X�᷽��������Сֵ
	void GetMaxX();
	// ����m_iY, m_...X, m_DrawRect��������������Сֵ
	void Init();		
	// ��ʼ����ǰ���������
	void InitListData();
	// ��ʼ����ǰ�������
	void InitBlockData();
	// ��ͼ
	void DrawPic();
	// ����Ʊ�������ݵ��б�
	void AddCommbo();
	// �����Ʊ�б��й�Ʊʱ,���Ĵ���
	void SelectCurStock();
	// Generated message map functions
	//{{AFX_MSG(CDlgBlockPicAnalysis)
	virtual BOOL OnInitDialog();
	// �����Ʊ�б��
	afx_msg void OnDblclkListBox();
	// ˢ�´���
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// �˳�
	virtual void OnCancel();
	virtual void PostNcDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSPICANALYSEDLG1_H__659C82FA_7E89_4E0C_B28F_D03DA2102CF9__INCLUDED_)
