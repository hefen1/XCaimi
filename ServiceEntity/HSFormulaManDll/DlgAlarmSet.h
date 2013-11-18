/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgAlarmSet.h
*	文件标识：	预警系统"条件设置模块
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#pragma once
#include "afxcmn.h"
#include "TreeCtrlFormula.h"
#include "resource.h"
#include "ExpTabCtrl.h"
#include "AdvancedAlarmData.h"
#include "DlgAlarmMan.h"
#include <afxwin.h>
#include "EditEidolon.h"

#define HS_STOCK_CODE	"股票代码"
#define HS_STOCK_NAME	"股票名称"
// CDlgAlarmSet dialog

class CDlgAlarmSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmSet)

public:
	CDlgAlarmSet(CWnd* pParent = NULL, CAdvancedAlarmData* pAlarmData = NULL, StockUserInfo* pStockData = NULL, HWND hAlarmWnd = 0);   // standard constructor
	virtual ~CDlgAlarmSet();

// Dialog Data
	enum { IDD = IDD_ALARM_SET };

	CExpTabCtrl	m_wndTab;															//指标树
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual BOOL OnInitDialog();	
	long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);							//读入公式--消息响应


public:
	
	// 预警管理指针
	HWND m_hAlarmWnd;
	// 是修改还是新建
	BOOL m_bChageOrBuild;
	// 默认加入的股票
	StockUserInfo* m_pAddStock;
	// 加入的股票的索引
	int m_nAddStockIndex;
	// 是不使用板块
	CButton m_ctlBlockSel;
	// 声音
	CButton m_ctlDelay;
	// 对话框
	CButton m_ctlBox;
	// 保持
	CButton m_ctlContinue;
	// 买出
	CButton m_ctlSell;
	// 荐股
	CButton m_ctlJG;
	// 列出所有板块
	CComboBox m_ctlBlockCom;
	// 当前板块名
	CString m_ctlStrBlockList;

	// 股票列表
	CListCtrl	m_ctlStockList;														
	// 当前股票列表索引
	int			m_nStockIndex;														

	// 主窗口传入的预警数据结构指针
	CAdvancedAlarmData* m_pAlarmData;													

	// 公式参数设置
	CEditEidolon m_EditEidolon;														

	//组合条件
	UnionSelCondition*		m_pUnion;														
	//单指标
	SingleSelCondition*	m_pSingleSel;													
	//是单指标：true 是组合条件：false;
	BOOL			m_SingOrUnion;													

	//股票列表
	CArray<StockUserInfo*, StockUserInfo*>	m_ayStkList;							
	//当前板块名
	CString									m_CurSBlockName;	

	// 预警名
	CString m_strConditionName;														

	//计算周期
	CComboBox m_ctlPeried;	

	// 用于显示列表框中图标
	CImageList m_itemList;

public:

	// 初始化
	void			Init(void);														
	//初始化板块列表的所有板块名--发消息
	void			InitBlockName();												
	//初始化指标树的选择
	void			InitTreeSel();													
	//初始化界面--加入功能树及参数设定
	void			InitFace();														
	//初始化股票列表头
	void			InitStkList();													

	//画股票列表数据
	void			DrawStkList();													

	//加/减股票
	BOOL			AddStock(StockUserInfo* pData);									
	void			DelStock(int nIndex);											

	//设置OK按钮状态
	void			SetOkEnable();													

	//读入联合功能树
	BOOL			SaveLoad(UINT nMode,CString strFileName = "",DWORD dStyle = 0);	

	//得到公式描述
	CString			GetDescribe(UnionSelCondition* pHSUnion);	

	// 得到板块名
	void			GetBlockBoxName();

	//设置/得到界面--复选框状态
	void			SetType();														
	WORD			GetType();														

	//得到所有板块名--消息响应
	long			OnGetBlockName(WPARAM wp,LPARAM lp);							

	//删除内存空间
	void			Free();	

	// 发送加股票信息
	BOOL			SendAddStockMsg(MSG *pMsg);
	// 发送得到板块所有股票信息
	BOOL			SendBlockInfoMsg(CString strBlockName);
	// 设置删除按键状态
	BOOL			SetDelStockButton(BOOL bFlag);
	
	
protected:
	//设置加/减股票按钮状态
	void	SetEnableAddDelButtons(BOOL Flag);										

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBlokcSel();
	afx_msg void OnBnClickedDesc();
	afx_msg void OnBnClickedDelStk();
	afx_msg void OnNMClickStkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnKillfocusBlockList();
	afx_msg void OnCbnSetfocusBlockList();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedSet();
	LRESULT		 OnUserDragData(WPARAM wParam, LPARAM lParam);
	//改变名字
	afx_msg void OnEnChangeName();													
	//加入股票
	afx_msg void OnBnClickedAddStk();	
	afx_msg void OnCbnSelchangeBlockList();
	afx_msg void OnNMSetfocusStkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusStkList(NMHDR *pNMHDR, LRESULT *pResult);
};
#pragma once
