/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgBlockAnalysis.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	"������"������ģ��(??? ָ��ʹ��ǰҪ�ж�)
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/

#if !defined(AFX_HSBLOCKANALYSEDLG_H__3F81A853_E8C6_4661_B3D8_2FA0B6BEC74E__INCLUDED_)
#define AFX_HSBLOCKANALYSEDLG_H__3F81A853_E8C6_4661_B3D8_2FA0B6BEC74E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBlockAnalysis.h : header file
//
#include "BaseListCtrl.h"
#include "ExpPublic.h"
#include "PubStruct.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgBlockAnalysis dialog

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort);

#define LISTDATA BlockInfo
#define	HS_BUTTON_Y	15
#define	HS_BUTTON_X	15
#define	HS_BUTTON_H	30
#define	HS_BUTTON_W	60
#define	HS_TAB_Y		10//65
#define	HS_TAB_X		10
#define	HS_TAB_B		10
#define HS_LIST_Y		25
#define	HS_LIST_X		0
#define HS_TEXT_X		20
#define HS_TEXT_W		150
#define HS_TEXT_H		15
#define HS_BORDER_H	5
#define HS_BORDER_W	5

// �б���������
struct StockSortData
{
	long	m_Stock; // ��Ʊ����(���ݱ��)
	int		nIndex;  // ���ڹ�Ʊ�б��е�����
	StockSortData()
	{
		m_Stock = 0;
		nIndex = -1;
	}
};

// Tab��������
struct TabStockSortData
{
	// ����Ĺ�Ʊ��Ϣ����
	CArray<StockSortData*, StockSortData*>  m_ayTabSortData;
	// �����
	int	nTab;  
	~TabStockSortData()
	{
		for(int i = 0; i < m_ayTabSortData.GetSize(); i++ )
		{
			delete m_ayTabSortData.GetAt(i);
		}
		m_ayTabSortData.RemoveAll();
	}
};

// �ж�������
typedef struct ListColHandName
{
	CString		strColName;				//����	Ҳ���ǹ�ʽ��
	CString		strDesc;				//������
	int			iType;					//���㷽ʽ: 0 ����ƽ, 1 ��ͨ�̼�Ȩƽ��, 2 �ۼ�
	int			iFormat;				//���ָ�ʽ: 0 1234.56, 1 1234.567, 2 1234, 3 1.234E + 003, 4 1,234.56;
	//int			iCyc;				//����: 0 �ֱʳɽ�, 1 1����, 2 5����, 3 15����... 10 ����, 11 ������
	int			iCx;					//�����п��
	ListColHandName()
	{
		iCx = 70;
	};

	void Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
			ar<<strColName;
			ar<<strDesc;
			ar<<iType;
			ar<<iFormat;
		}
		else
		{
			ar>>strColName;
			ar>>strDesc;
			ar>>iType;
			ar>>iFormat;
		}
	};

}LISTCOLHANDNAME;

// ����ʾ����
struct LineDrowData
{
	CString		strLineName;		// ��Ʊ�� 
	CArray<double, double>	ayLine; // ͳ������
};

// �б���ʾ����
struct ListDrowData
{
	int		iDrowNum;
	CArray<LineDrowData*, LineDrowData*> ayList; // ������
	~ListDrowData()
	{
		for( int i = 0; i < ayList.GetSize(); i++ )
		{
			delete ayList.GetAt(i);
		}
		ayList.RemoveAll();
	}
};

// List�ؼ�����ص������ӿ�����
typedef	struct 
{
	int				iCurCol;	// �����������
	int				iDownOrUp;  // ����(1)���ǽ���(-1)
	LineDrowData	**pBlock;   // �������б� 
} PARAMVAL;


/*************************************************************
*	�� �� �ƣ�	CDlgBlockAnalysis
*	�� �� ����	�����������
***************************************************************/
class CDlgBlockAnalysis : public CDialog
{
// Construction
public:

	CDlgBlockAnalysis(CWnd* pParent = NULL, HSOpenExpressUseData* pData = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDlgBlockAnalysis)
	enum { IDD = IDD_BLOCK_ANALYSE };
	CButton	m_begin;
	CButton	m_add;
	CTabCtrl		m_Tab_Ctrl;
	CBaseListCtrl	m_List_Ctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBlockAnalysis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	void PostNcDestroy();
	void OnCancel();

// Implementation
protected:
	// �б���ͷ��������
	CArray<LISTCOLHANDNAME, LISTCOLHANDNAME>		m_AryListColHandName;	//CListCtrl�ؼ��е���ͷ��
	// ����ʱҪ�õ��Ĺ�Ʊ����ְ�鼰����������
	BlockTabInitData								*m_TypeAndBlock;		//���е�TAB���ࣩ����Ӧ�������İ��
	// ����������ʾ����
	CArray<ListDrowData*, ListDrowData*>			m_ayDrowData;			//��listctrl������

	// ��ͷ�������ݱ����ļ���
	CString						m_strColHandFileName;						//������ͷ�����ñ����ļ�

	// Generated message map functions
	//{{AFX_MSG(CDlgBlockAnalysis)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddProject();
	afx_msg void OnBeginAnalyse();
	afx_msg void OnProjectadd();
	afx_msg void OnProjectdel();
	afx_msg void OnProjectchage();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:


	/******************************************************************
	*	�������ܣ�	�źŴ���
	*	����������	WPARAM wp : [in] 0 �õ���ʼҳ--Ҳ���ǵ�0ҳ�����й�Ʊ, lpΪBlockTabInitData*����,Ϊ�������Ʊ��Ϣ
									 1 ����ͳ�ƿ�ʼ, lpΪint����,Ϊ��ǰҪͳ�Ƶķ����ȫ������
									 2 ���ص�ǰTab�����а��Ĺ�Ʊ, lpΪint����,Ϊ��ǰҪͳ�Ƶķ����ȫ������
									 3 ��ֹ����ʱ,ʹ���弰��ť��Ч
									10 ��handcol�õ���ǰ��, lpΪint����,Ϊ��ǰ�������ͷ��������
	*				LPARAM lp : [in] ��wp������,��wp��ֵ����
	*	�� �� ֵ��	long : 0 ������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��28��
	*	�޸�˵����	
	*****************************************************************/
	long	GetBlockStockEventPress(WPARAM wp,LPARAM lp);

	// ���ļ�m_strColHandFileName�ж����ͷm_pAryListColHandName������
	int		ReadListColHandName();	
	// ��m_pAryListColHandName�����ݱ��浽�ļ�m_strColHandFileName��
	int		SaveListColHandName();

	// ��һ��TAB��,����ΪstrTabName
	int		AddTab(CString strTabName);					
	// ɾ��������ΪiIndex��LIST�м��������
	int		RemoveListColHand(int iIndex);					

	/******************************************************************
	*	�������ܣ�	��һ��LIST��ͷ
	*	����������	CString strExpressName : [in] ����(ͳ�ƹ�ʽ��ָ����)
	*				int iType			   : [in] ͳ�Ʒ�ʽ 	 
	*				int iFormat			   : [in] ��ʾ��ʽ
	*				CString strDesc        : [in] ������
	*	�� �� ֵ��	int : 0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��28��
	*	�޸�˵����	
	*****************************************************************/
	int		AddListColHand(CString strExpressName, int iType, int iFormat, 
		CString strDesc);
	//int		SetTypeIniFileName(CString strFileName);		//���ð���������ļ���
	// ���ñ�ͷ�����ļ���m_strColHandFileName
	int		SetColIniFileName(CString strFileName);			
	// �ػ�LIST��ͷ
	int		ReDrowListColHandName();
	// ��TAB����
	int		ReDrowTab();

	/******************************************************************
	*	�������ܣ�	�ػ�LIST����
	*	����������	int iCurTabSel : [in] Ҫ���ķ�������
	*	�� �� ֵ��	int : 0������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��28��
	*	�޸�˵����	
	*****************************************************************/
	int		ReDrowListData(int iCurTabSel);					

	/******************************************************************
	*	�������ܣ�	�ӵ�֧��Ʊ�ļ�������ͳ�Ƴ����ķ������
	*	����������	int iCurTab : [in] Ҫͳ�Ƶķ�������
	*	�� �� ֵ��	int : 0������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��28��
	*	�޸�˵����	
	*****************************************************************/
	int		ComputeListData(int iCurTab);
	// ����BlockInfo�е�����
	int		SetHSBlockInfoColName();
	// ���͵õ�����Ʊ����Ϣ(һ����childframe)
	int		SendGetBlockStockEvent();

	/******************************************************************
	*	�������ܣ�	������ǰ����Ʊ������Index(ͳ������ʱ,���ö��ַ�FIND,�ӿ�ͳ���ٶ�)
	*	����������	CArray<StockUserInfo*
	*				StockUserInfo*> *payStock : [in] ��Ʊ��Ϣ
	*				TabStockSortData* pIndex  : [out] ������
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��28��
	*	�޸�˵����	
	*****************************************************************/
	void	BuildStockIndex(CArray<StockUserInfo*,StockUserInfo*> *payStock, TabStockSortData* pIndex);

	/******************************************************************
	*	�������ܣ�	���ݹ�Ʊ���뼰��Ʊ�������õ���Ʊ�ڵ�ǰ����е�����
	*	����������	TabStockSortData* pIndex : [in] ������Ϣ
	*				CString StockCode        : [in] ��Ʊ����
	*	�� �� ֵ��	int : <0 ʧ��, >=0��Ʊ�ڹ�Ʊ�б��е�������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��28��
	*	�޸�˵����	
	*****************************************************************/
	int		GetStockIndex(TabStockSortData* pIndex, CString StockCode);
	// �Կؼ���Ч�Խ��и���
	void	EnableCtrl(BOOL bFlag);

	// ��ǰ�к�
	int						m_iCurHandlerNum;
	// �򿪱�ģ��Ľӿ�����
	HSOpenExpressUseData*	m_pData;
	// ������ָ��(����ָ��ʽ������)
	CWnd*					m_Form;
	// ����,��������ʱ��
	CStringArray*			m_HandColName;
	// ��ǰTab���Ѿ��õ����������Ʊ(�Ƿ��ѵ������ǰtab)
	CArray<BOOL, BOOL>		m_bClickFlag;
	// �������������
	CArray<TabStockSortData*, TabStockSortData*>	m_ayStockIndex;
	// ����Ψһ
//	static BYTE m_bRunFlag;
	afx_msg void OnBnClickedSaveas();
	afx_msg void OnBnClickedSaveas2();

public:
	// �ı��С
	void SetSize();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSBLOCKANALYSEDLG_H__3F81A853_E8C6_4661_B3D8_2FA0B6BEC74E__INCLUDED_)
