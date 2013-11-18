/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgFormulaMan.h
*	文件标识：	公式管理窗口类
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_FORMULAMAN_H__DF745EF3_1042_11D5_A105_2A829F000000__INCLUDED_)
#define AFX_FORMULAMAN_H__DF745EF3_1042_11D5_A105_2A829F000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFormulaMan.h : header file
//
#include "TreeCtrlFormula.h"
#include "DlgFormulaEdit.h"
#include "ExpPublic.h"
#include "DataSourceDefineEx.h"
#include "DataSourceDefine.h"
#include "ConfigInterface.h"
#include "HSWinner.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaMan dialog

//#include "ETSLayout.h"
#include "DlgNotesAndEidolon.h"

// SHIFT键是否按下
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
// CTRL键是否按下
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

extern void SetActiveMain(CWnd* pRefWnd);



extern CMapStringToOb *Formular_CfgTopMapOb;
extern CMapStringToOb *Formular_UserCfgMapOb;
/*************************************************************
*	类 名 称：	CDlgFormulaMan
*	类 描 述：	公式管理窗口
***************************************************************/
class CDlgFormulaMan : public  CHSBaseDialog
{
	// 本宏说明类是可以运态创建
	DECLARE_DYNAMIC(CDlgFormulaMan)
// Construction
public:
	/******************************************************************
	*	函数功能：	构造,初始化指向体身的变量
	*	函数参数：	CWnd* pParent = NULL : [in] 父窗口指针
	*				long* pThis = NULL   : [in] 指向对象本身指针的指针
	*	返 回 值：	无
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	CDlgFormulaMan(CWnd* pParent = NULL,long* pThis = NULL);   // standard constructor
	~CDlgFormulaMan();

	/* DialogMgr: Add this: */
//	DECLARE_LAYOUT();
	/************************/

// Dialog Data
	//{{AFX_DATA(CDlgFormulaMan)
	enum { IDD = IDD_FORMULAMAN };
	// 分类TAB控件("技术指标", "交易系统", "条件选股", "五彩K线")
	CTabCtrl	m_wndTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFormulaMan)
public:
	// 消息预处理,处理键盘的ESC键及ENTER键,同时,调用当前树的消息预处理函数	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	// 数据交换
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// 窗口函数
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	// Generated message map functions
	//{{AFX_MSG(CDlgFormulaMan)
	// 初始化
	virtual BOOL OnInitDialog();
	// 窗口消毁
	afx_msg void OnDestroy();
	// 按退出处理
	virtual void OnCancel();
	// 按确定处理
	virtual void OnOK();
	// 组改变选择处理
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	// 按新建处理
	afx_msg void OnNew();
	// 按更改处理
	afx_msg void OnUpdate();
	// 按删除处理
	afx_msg void OnDelete();
	// 按注释处理
	afx_msg void OnNotes();
	// 按输出处理
	afx_msg void OnOut();
	// 按导入处理
	afx_msg void OnIn();
	// 按临时输入处理
	afx_msg void OnTempin();
	// 按参数说明处理
	afx_msg void OnParam();
	// 大小改变处理
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 按查找处理
	afx_msg void OnButfind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	/******************************************************************
	*	函数功能：	拖放处理
	*	函数参数：	WPARAM wParam : [in] 如果为拖放公式,则恒为HX_USER_DRAGDATA-1,其它不做处理
	*				LPARAM lParam : [in] CExpression*类型,为正在拖放的公式体
	*	返 回 值：	LRESULT : 成功(1), 失败(0)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月30日
	*	修改说明：	
	*****************************************************************/
	virtual LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);

	/******************************************************************
	*	函数功能：	向父窗口g_hParentWnd发HX_USER_COMPILEDATA消息
	*	函数参数：	WPARAM wParam : [in] 无意义
	*				LPARAM lParam : [in] 无意义
	*	返 回 值：	LRESULT : 消息处理返回或是0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月30日
	*	修改说明：	
	*****************************************************************/
	virtual LRESULT OnExchangData(WPARAM wParam, LPARAM lParam);

	// 消息循环,参数没意义
	long OnRun(WPARAM wParam, LPARAM lParam);

	/******************************************************************
	*	函数功能：	加公式处理(公式管理发送过来的消息)
	*	函数参数：	WPARAM wParam = 0 : [in] 无意义
	*				LPARAM lParam = 0 : [in] CExpression*类型,要加入的公式指针
	*	返 回 值：	long : 恒为1
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);

public:	
	// 清除本模块所占用的所有私人空间
	void Delete();
	// 保存所有公式
	void Save();
	
	// 公式树控件数组
	CArray<CTreeCtrlFormula*,CTreeCtrlFormula*> m_pAyTree;
public:	
	// TAB的图标
	static CImageList	m_tabImages; 
	// TREE中选中的公式的图标
	static CImageList	m_expressCheckImages;
	// TREE中默认的公式的图标
	static CImageList	m_expressImages;


public:	

	/******************************************************************
	*	函数功能：	根据索引,得到树(里面包含了公式)
	*	函数参数：	int nIndex = -1 : [in] 树索引,为-1时表求要得到当前选中TAB的树
	*	返 回 值：	CTreeCtrlFormula* : 得到的公式树,指不到则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	CTreeCtrlFormula* GetCurTree(int nIndex = -1);

	/******************************************************************
	*	函数功能：	根据类型,得到树(里面包含了公式)
	*	函数参数：	WORD nMask   : [in] 树类型(有Tech,Condition,Exchange,MoreKLine,Union,ExpressBin,none等)
	*	返 回 值：	CTreeCtrlFormula* : 得到的公式树,指不到则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	CTreeCtrlFormula* GetCurTree(WORD nMask);

	/******************************************************************
	*	函数功能：	根据树名,得到树(里面包含了公式)
	*	函数参数：	CString strName : [in] 树名
	*	返 回 值：	CTreeCtrlFormula*    : 得到的公式树,指不到则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	CTreeCtrlFormula* GetCurTree(CString strName);

	/******************************************************************
	*	函数功能：	得到公式pExpresie所在的公式树
	*	函数参数：	CExpression* pExpresie : [in] 公式
	*	返 回 值：	CTreeCtrlFormula*		   : [in] 得到的公式树,指不到则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	CTreeCtrlFormula* GetCurTree(CExpression* pExpresie);

	/******************************************************************
	*	函数功能：	显示或隐藏公式树pCurTree界面
	*	函数参数：	CTreeCtrlFormula* pCurTree : [in] 要操作的公式树
	*				BOOL bShow = TRUE	  : [in] 显示(TRUE), 隐藏(FALSE)
	*	返 回 值：	void		 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	void		 ShowTree(CTreeCtrlFormula* pCurTree,BOOL bShow = TRUE);

// 公式相关信息
public:
	static CMapVariabile*  m_pExternExpression;      // 外部表达式表
	static CMapVariabile*  m_pExternVariabile;		 // 外部变量表
	static CMapVariabile*  m_pExternFunction;		 // 外部函数表

	static HSFunAddDefaultTechAnysis m_pOpeDefTechFun;		 // 缺省指标操作(函数指针)
	static class CDlgFormulaEdit*	  m_pEdiUpdatetExp;		 // 公式编辑窗口(窗口指针)
	static class CDlgNotesAndEidolon* m_pDlgNotes;			 // 公式注释窗口(窗口指针)
	static class CDlgNotesAndEidolon* m_pDlgParam;			 // 参数修改窗口(窗口指针)	

	/******************************************************************
	*	函数功能：	设置常用指标操作
	*	函数参数：	CExpression* pExpression : [in] 要设置的指标公式指针
	*				char Operator			 : [in] 操作符, '+' 设置为常用
															'-' 取消常用指标的设置
															'd' 清空常用指标列表
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	static BOOL  SetDefaultTech(CExpression* pExpression,char Operator);

	// 得到公式编辑窗口指针
	class CDlgFormulaEdit*		GetExpDlg(CExpression* pExpression = NULL);
	// 得到公式注释窗口指针
	class CDlgNotesAndEidolon* 	GetDlgNotes();
	// 得到参数修改窗口指针
	class CDlgNotesAndEidolon* 	GetDlgParam();

	/******************************************************************
	*	函数功能：	设置公式注释窗口m_pDlgNotes的公式定义内容及窗口标题文本
	*	函数参数：	CString strNotes   : [in] 公式定义文本
	*				CString strCaption : [in] 窗口标题文本
	*	返 回 值：	void  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	void  SetNotes(CString strNotes,CString strCaption);

public:
	// 改变窗口大小
	void	SetSize();

	/******************************************************************
	*	函数功能：	初始化,得到在主模块中分配了空间的外部表达式表,外部变
					量表,外部函数表,图片,表头及绘图数据空间分配函数指针,
					同时初始化外部函数表
	*	函数参数：	HWND hParentWnd = NULL : [in] 父窗口句柄, 为NULL时不改变当前的父窗口句柄
	*	返 回 值：	void  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	void    InitData(HWND hParentWnd = NULL);

	/******************************************************************
	*	函数功能：	窗口控件的有效处理
	*	函数参数：	BOOL bEnable : [in] 是否有效
	*				HSOpenExpressUseData* pData = NULL : [in] 当前选中的公式
	*	返 回 值：	void  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	void    Enable(BOOL bEnable,HSOpenExpressUseData* pData = NULL);
	// 窗口控件(更新,删除,注释及参数按钮)的有效处理
	void	EnableItem(BOOL bEnable);

	//
protected:
	// 浮动树(目前好象没看到什么地方有用到他???)
	// class CWndKeyboard* m_pIndividualInfo;
	// 键盘精灵窗口
	//class  CWndKeyboard* m_pKeyboard;

	IHsColumnProperty* m_pColProt;   //配置字段指针

public:
	/******************************************************************
	*	函数功能：	公式管理模块操作接口
	*	函数参数：	HSOpenExpressUseData* pData : [in] 操作方式根据HSOpenExpressUseData::m_wType&0xF00决定
							hxShowByExpressType   : 由公式类型来显示公式, 返回0
							hxShowReturnByExpressType : 由公式类型来显示公式,并返回选中公式
							hxOpenExpressByConditionStockType : 选股, 返回0
							hxOpenExpressBySortStockType : 指标排序, 返回0
							hxOpenBlockAnalysis   : 板块分析, 返回0
							hxOpenUniteAnalysis   : 联动分析, 返回0
							hxOpenAlarmManagerDlg : 打开预警窗口, 返回预警窗口指针
							hxOpenFindDataDlg     : 打开公式查找窗口, 返回0
							hxOpenAddStkToAlarm   : 打开加股票到预警窗口,  返回0
						
	*	返 回 值：	long : 与pData相关,看pData说明
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	long Open(HSOpenExpressUseData* pData);
	long Open(StrategyInfo* pData);//yulx add 策略交易参数配置
	long Open(StrategyHis * pData);//yulx add 策略交易测试
	long StrategyManage();//yulx add 策略管理
	BOOL  ReadStrategy(CMapWordToPtr *& pmap);//读取策略
	void  SerializeExp(CArchive & ar,CExpValue *& exp);
	// 浮动树
public:
	// 创建浮动树窗口, 没看到有什么地方用
	void				   CreateIndividualInfo();

	/******************************************************************
	*	函数功能：	得到浮动树,如果没有,则创建
	*	函数参数：	void* pData = NULL : [in] HSOpenExpressUseData*型数据
											  HSOpenExpressUseData::m_pData是CWnd*类型,为浮动树的父窗口指针
	*	返 回 值：	CTreeCtrlFormula* : 得到的浮动树
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	class CTreeCtrlFormula*	   GetIndividualInfo(void* pData = NULL);

	// 键盘精灵
public:
	// 创建键盘精灵窗口
// 	void				   CreateKeyboard();
// 	// 得到键盘精灵窗口
// 	class CWndKeyboard* GetKeyboard();
// 
// 	/******************************************************************
// 	*	函数功能：	初始化键盘精灵,看CKeyboardDataList::InitKeyboard函数说明
// 	*	函数参数：	WPARAM wParam : [in] 看CKeyboardDataList::InitKeyboard函数
// 	*				LPARAM lParam : [in] 看CKeyboardDataList::InitKeyboard函数
// 	*	返 回 值：	void				  
// 	*	调用依赖：	CKeyboardDataList::InitKeyboard
// 	*	作    者：	
// 	*	完成日期：	2003年5月6日
// 	*	修改说明：	
// 	*****************************************************************/
// 	void				   InitKeyboard(WPARAM wParam, LPARAM lParam);
// 
// 	/******************************************************************
// 	*	函数功能：	显示或是隐藏键盘精灵窗口
// 	*	函数参数：	WPARAM wParam : [in] 显示(SW_SHOW), 隐藏(SW_HIDE)
// 	*				LPARAM lParam : [in] HSOpenExpressUseData*类型数据
// 	*	返 回 值：	void				  
// 	*	调用依赖：	
// 	*	作    者：	
// 	*	完成日期：	2003年5月6日
// 	*	修改说明：	
// 	*****************************************************************/
// 	long				   ShowKeyboard(WPARAM wParam, LPARAM lParam);

public:

	/******************************************************************
	*	函数功能：	新键公式
	*	函数参数：	WORD nMask : [in] 新键公式类型(指标,条件,交易或五彩)
	*	返 回 值：	CDlgFormulaEdit* : [in] 公式编辑窗口指针
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	CDlgFormulaEdit* NewExpress(WORD nMask,short nType = CDlgFormulaEdit::NewExp);

	/******************************************************************
	*	函数功能：	打开注释窗口
	*	函数参数：	WPARAM wParam : [in] 无意义
	*				LPARAM lParam : [in] 为HSOpenExpressUseData*类型
										 HSOpenExpressUseData::m_pData为公式指针(CExpression*)
	*	返 回 值：	long : 成功(1), 失败(0)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	long OnNotes(WPARAM wParam,LPARAM lParam);

	/******************************************************************
	*	函数功能：	打开参数修改窗口
	*	函数参数：	WPARAM wParam : [in] 无意义
	*				LPARAM lParam : [in] 为HSOpenExpressUseData*类型
										 HSOpenExpressUseData::m_pData为公式指针(CExpression*)
	*	返 回 值：	long : 成功(1), 失败(0)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	long OnParam(WPARAM wParam,LPARAM lParam);

public:
	/******************************************************************
	*	函数功能：	从树列表m_pAyTree中,按方式nCopy拷贝树到目标树pTree中
	*	函数参数：	CTreeCtrlFormula* pTree = NULL : [in] 目标树,为NULL指浮动树m_wndFavorate
	*				int nCopy = CTreeCtrlFormula::CopyNone : [in] 拷贝方式
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	void CopyTree(CTreeCtrlFormula* pTree = NULL,int nCopy = CTreeCtrlFormula::CopyNone);

	void CopyToFloatTree(int nCopy = CTreeCtrlFormula::CopyNone);

protected:

	/******************************************************************
	*	函数功能：	从文件对话框中得到公式文件名
	*	函数参数：	BOOL bOpenFileDialog : [in] 打开还是保存
	*				CString& strFileName : [out] 得到的文件名
	*	返 回 值：	int : 文件对话DoModal的返回
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	int		OpenExpressFile(BOOL bOpenFileDialog,CString& strFileName);

	/******************************************************************
	*	函数功能：	从用户指定文件中读数据
	*	函数参数：	int& lLength		: [out] 文件长度(也就是返回的char*的空间长度)
	*				CString& fileName   : [out] 用户指定的文件名
	*	返 回 值：	char* : 从用户指定文件中读取的数据
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	char*	GetInportBuffer(int& lLength,CString& fileName);

	/******************************************************************
	*	函数功能：	导入分析家公式
	*	函数参数：	char* pBuffer : [in] 分析家公式文件内容
	*				long lLength  : [in] pBuffer长度
	*	返 回 值：	int : 导入成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	int 	InportFxjExpress(char* pBuffer,long lLength);

	/******************************************************************
	*	函数功能：	将字串pBuffer解析成分析家公式并放到数组ayExpress中
	*	函数参数：	char* pBuffer : [in] 要处理的字串
	*				long lCurPos  : [in] 开始位置
	*				long lNextPos : [in] 下个数据开始位置
	*				CArray<FxjExpress*, FxjExpress*>& ayExpress : [out] 解析结果,分析家公式数组
	*				int lLength   : [in] pBuffer长度
	*	返 回 值：	int : 成功(1), 失败(-1)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	int		GetFxjExpress(char* pBuffer,long lCurPos,long lNextPos,
						  CArray<FxjExpress*,FxjExpress*>& ayExpress,int lLength );
	// 当前点是否有效(nCurPos小于nSubCount及nMainCount)
	BOOL	IsValid(int nCurPos,int nSubCount,int nMainCount);

public:

	/******************************************************************
	*	函数功能：	加公式(从分析家公式)
	*	函数参数：	CMapVariabile* pExternExpression : [in] 外部公式列表
	*				WORD wStyle						 : [in] 公式引入方式,EXTERN_EXPRESS_OVR则覆盖已有公式
	*				FxjExpress* pFxjExpress          : [in] 分析家公式 
	*				CTreeCtrlFormula* pTree = NULL		 : [in] 要操作的公式树
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	void	AddExpress(CMapVariabile* pExternExpression,WORD wStyle,FxjExpress* pFxjExpress,CTreeCtrlFormula* pTree = NULL);

	/******************************************************************
	*	函数功能：	将公式pLoadData::pExpValue加入外部公式表,同类公式树及默认指指标表中
	*	函数参数：	CTreeCtrlFormula::LoadItemData* pLoadData : [in] 公式相关信息
	*				WORD wStyle : 公式引入方式,EXTERN_EXPRESS_OVR则覆盖已有公式
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	void	AddExpress(CTreeCtrlFormula::LoadItemData* pLoadData,WORD wStyle);

	// 设置树pTree的正常图标及选中图标和其m_pFormulaMan指向当前公式管理器窗口
	void	SetTree(CTreeCtrlFormula* pTree);
protected:
	// 公式引入方式
	WORD			m_wExternalExpress;
public:
	IMainWnd*		m_pMainWnd;    //主框架接口
public:

	/******************************************************************
	*	函数功能：	按键处理
	*	函数参数：	UINT nKey : [in] 键值
	*	返 回 值：	BOOL : 处理成功(TRUE), 没做处理(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	BOOL	Key(UINT nKey);

protected:
	
	struct ExternParam
	{
		int		m_nCopy/* = CopyNone*/;
		DWORD	m_dwStyle;
		DWORD	m_dwType;   // 类型
		ExternParam()
		{
			memset(this,0,sizeof(ExternParam));
		}
	};

	CTypedPtrMap<CMapPtrToPtr,CTreeCtrlFormula*,ExternParam*> m_ayExtern;
	
	//CArray<CTreeCtrlFormula*,CTreeCtrlFormula*> m_ayExtern; // 外部使用 //m_wndFavorate;
	//CTreeCtrlFormula* GetExternTree(int nPos);

public:
	/******************************************************************
	*	函数功能：	显示公式编辑器窗口
	*	函数参数：	CDlgFormulaEdit* Dialog : [in] 公式编辑器窗口
	*				WPARAM wParam = 0 : [in] 无意义
	*				LPARAM lParam = 0 : [in] 无意义
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	void ShowFomulaDlg(CDlgFormulaEdit* Dialog,WPARAM wParam = 0, LPARAM lParam = 0);

	/******************************************************************
	*	函数功能：	公式查找
	*	函数参数：	DWORD dwMask = 0xFFFF : [in] 查找范围
	*				CWnd* pWnd = NULL : [in] 查找窗口的父窗口
	*	返 回 值：	long : 查找窗口的指针(CDlgFormulaFind*)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	long Find(DWORD dwMask = 0xFFFF,CWnd* pWnd = NULL);

	//发送消息到框架
	void SendLinkMsg(WPARAM wParam ,LPARAM lPaeam);
	
		// 快速创建公式
	//long SpeedCreateTech();
public:
	// 无用
	virtual void MyNcDestroy();

	// 公式自动安装
public:
	void AutoSetupExpress(HSOpenExpressUseData* pData);

	// gbq add 20060209, 根据名称查找公式
public:
	CExpression* FindExpression(CString strName);
	CExpression* ShowExpression(CString strName);
	void  GetColumnProt();
	BOOL ShowWindow( int nCmdShow );

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULAMAN_H__DF745EF3_1042_11D5_A105_2A829F000000__INCLUDED_)
