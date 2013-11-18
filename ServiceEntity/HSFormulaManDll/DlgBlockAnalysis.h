/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgBlockAnalysis.h
*	文件标识：	
*	摘	  要：	"板块分析"功能主模块(??? 指针使用前要判断)
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
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

// 列表索引数据
struct StockSortData
{
	long	m_Stock; // 股票代码(数据表达)
	int		nIndex;  // 所在股票列表中的索引
	StockSortData()
	{
		m_Stock = 0;
		nIndex = -1;
	}
};

// Tab索引数据
struct TabStockSortData
{
	// 有序的股票信息排列
	CArray<StockSortData*, StockSortData*>  m_ayTabSortData;
	// 分类号
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

// 列定义数据
typedef struct ListColHandName
{
	CString		strColName;				//列名	也就是公式名
	CString		strDesc;				//列描述
	int			iType;					//计算方式: 0 算术平, 1 流通盘加权平均, 2 累加
	int			iFormat;				//数字格式: 0 1234.56, 1 1234.567, 2 1234, 3 1.234E + 003, 4 1,234.56;
	//int			iCyc;				//周期: 0 分笔成交, 1 1分线, 2 5分线, 3 15分线... 10 月线, 11 多日线
	int			iCx;					//所在列宽度
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

// 行显示数据
struct LineDrowData
{
	CString		strLineName;		// 股票名 
	CArray<double, double>	ayLine; // 统计数字
};

// 列表显示数据
struct ListDrowData
{
	int		iDrowNum;
	CArray<LineDrowData*, LineDrowData*> ayList; // 行数据
	~ListDrowData()
	{
		for( int i = 0; i < ayList.GetSize(); i++ )
		{
			delete ayList.GetAt(i);
		}
		ayList.RemoveAll();
	}
};

// List控件排序回调函数接口数据
typedef	struct 
{
	int				iCurCol;	// 排序的列索引
	int				iDownOrUp;  // 升序(1)还是降序(-1)
	LineDrowData	**pBlock;   // 行数据列表 
} PARAMVAL;


/*************************************************************
*	类 名 称：	CDlgBlockAnalysis
*	类 描 述：	析块分析窗口
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
	// 列表列头定义数据
	CArray<LISTCOLHANDNAME, LISTCOLHANDNAME>		m_AryListColHandName;	//CListCtrl控件中的列头名
	// 分析时要用到的股票分类分板块及计算结果数据
	BlockTabInitData								*m_TypeAndBlock;		//所有的TAB（类）及对应所包含的板块
	// 计算结果的显示数据
	CArray<ListDrowData*, ListDrowData*>			m_ayDrowData;			//画listctrl的数据

	// 列头定义数据保存文件名
	CString						m_strColHandFileName;						//所有行头的设置保存文件

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
	*	函数功能：	信号处理
	*	函数参数：	WPARAM wp : [in] 0 得到初始页--也就是第0页的所有股票, lp为BlockTabInitData*类型,为板块分类股票信息
									 1 本地统计开始, lp为int类型,为当前要统计的分类的全局索引
									 2 返回当前Tab的所有板块的股票, lp为int类型,为当前要统计的分类的全局索引
									 3 中止运行时,使窗体及按钮有效
									10 从handcol得到当前列, lp为int类型,为当前点击的列头的列索引
	*				LPARAM lp : [in] 如wp项描述,随wp的值而变
	*	返 回 值：	long : 0 无意义
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月28日
	*	修改说明：	
	*****************************************************************/
	long	GetBlockStockEventPress(WPARAM wp,LPARAM lp);

	// 从文件m_strColHandFileName中读入表头m_pAryListColHandName的数据
	int		ReadListColHandName();	
	// 将m_pAryListColHandName的数据保存到文件m_strColHandFileName中
	int		SaveListColHandName();

	// 加一个TAB项,名字为strTabName
	int		AddTab(CString strTabName);					
	// 删除索引号为iIndex的LIST列及相关数据
	int		RemoveListColHand(int iIndex);					

	/******************************************************************
	*	函数功能：	加一个LIST表头
	*	函数参数：	CString strExpressName : [in] 列名(统计公式或指标名)
	*				int iType			   : [in] 统计方式 	 
	*				int iFormat			   : [in] 显示格式
	*				CString strDesc        : [in] 列描述
	*	返 回 值：	int : 0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月28日
	*	修改说明：	
	*****************************************************************/
	int		AddListColHand(CString strExpressName, int iType, int iFormat, 
		CString strDesc);
	//int		SetTypeIniFileName(CString strFileName);		//设置板块类配置文件名
	// 设置表头配置文件名m_strColHandFileName
	int		SetColIniFileName(CString strFileName);			
	// 重画LIST列头
	int		ReDrowListColHandName();
	// 画TAB内容
	int		ReDrowTab();

	/******************************************************************
	*	函数功能：	重画LIST数据
	*	函数参数：	int iCurTabSel : [in] 要画的分类索引
	*	返 回 值：	int : 0无意义
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月28日
	*	修改说明：	
	*****************************************************************/
	int		ReDrowListData(int iCurTabSel);					

	/******************************************************************
	*	函数功能：	从单支股票的计算结果中统计出板块的分析结果
	*	函数参数：	int iCurTab : [in] 要统计的分类索引
	*	返 回 值：	int : 0无意义
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月28日
	*	修改说明：	
	*****************************************************************/
	int		ComputeListData(int iCurTab);
	// 设置BlockInfo中的列名
	int		SetHSBlockInfoColName();
	// 发送得到板块股票的信息(一般向childframe)
	int		SendGetBlockStockEvent();

	/******************************************************************
	*	函数功能：	建立当前板块股票的有序Index(统计数据时,可用二分法FIND,加快统计速度)
	*	函数参数：	CArray<StockUserInfo*
	*				StockUserInfo*> *payStock : [in] 股票信息
	*				TabStockSortData* pIndex  : [out] 排序结果
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月28日
	*	修改说明：	
	*****************************************************************/
	void	BuildStockIndex(CArray<StockUserInfo*,StockUserInfo*> *payStock, TabStockSortData* pIndex);

	/******************************************************************
	*	函数功能：	根据股票代码及股票排序结果得到股票在当前板块中的索引
	*	函数参数：	TabStockSortData* pIndex : [in] 排序信息
	*				CString StockCode        : [in] 股票代码
	*	返 回 值：	int : <0 失败, >=0股票在股票列表中的索引号
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月28日
	*	修改说明：	
	*****************************************************************/
	int		GetStockIndex(TabStockSortData* pIndex, CString StockCode);
	// 对控件有效性进行更改
	void	EnableCtrl(BOOL bFlag);

	// 当前列号
	int						m_iCurHandlerNum;
	// 打开本模块的接口数据
	HSOpenExpressUseData*	m_pData;
	// 付窗口指针(这里指公式管理窗口)
	CWnd*					m_Form;
	// 列名,传给计算时用
	CStringArray*			m_HandColName;
	// 当前Tab否已经得到板块名及股票(是否已点击过当前tab)
	CArray<BOOL, BOOL>		m_bClickFlag;
	// 有序的索引数据
	CArray<TabStockSortData*, TabStockSortData*>	m_ayStockIndex;
	// 保持唯一
//	static BYTE m_bRunFlag;
	afx_msg void OnBnClickedSaveas();
	afx_msg void OnBnClickedSaveas2();

public:
	// 改变大小
	void SetSize();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSBLOCKANALYSEDLG_H__3F81A853_E8C6_4661_B3D8_2FA0B6BEC74E__INCLUDED_)
