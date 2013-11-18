/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	ExchYouSelCaclData.h
*	文件标识：	交易优选计算类
*	摘	  要：	目前没有采用,暂时不写
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#pragma once
#include "ExchCalcMain.h"
#include "TreeCtrlFormula.h"
#include "DlgSingleExchCalcResult.h"


class CExchYouSelCaclData: public CExchCalcMain
{
public:
	CExchYouSelCaclData(void);
	~CExchYouSelCaclData(void);

protected:
	CArray<CExpression*, CExpression*>		m_ayListCtrl;
	CArray<CExpression*, CExpression*>		m_ayExpress;	// 参加测试的公式列表
	CMapStringToPtr	m_mapCaclResult;	// 最优测试结果

	CArray<StockUserInfo*,StockUserInfo*> 	m_ayStock;	// 参加测试的股票
	StockUserInfo*	m_pPageStock;
	int	m_lTestObj;		// 测试目标
	int m_lTestCycle;	// 计算周期

	BOOL m_bUseAllLocalData;	// 使用本地全部数据
	int m_lBeginDate;			// 否则使用本地指定时间段的数据
	int m_lCount;

public:
	// 显示数据管理
	void CopyExch(CTreeCtrlFormula* pTree);

	// 指标或交易系统公式管理
	void ClearExpression();
	void AddExpression(CExpression* pExpression);
	void DelExpression(int nIdx);
	CArray<CExpression*, CExpression*>* GetAyExpression();
	CMapStringToPtr* GetMapResult();
	int GetExpressIdx(CExpression* pExp);
	int ConvertToGlobe(int nIdx);
	BOOL SetExpressionFromCtrl(CListCtrl *pCtrl = NULL);
	BOOL SetSelectAll(CListCtrl *pCtrl = NULL, BOOL bSelCheck = TRUE);

	// 优化结果数据管理
	void ClearResult();
	BOOL AddResult(CExpression* pExpression, TExchCaclResult* pResult);
	BOOL DelResult(CExpression* pExpression);
	TExchCaclResult* GetResult(CExpression* pExpression, BOOL bNew = FALSE);
	BOOL IsHave(CExpression* pExpression);
	CArray<StockUserInfo*,StockUserInfo*>* GetAyStock();

	// 显示结果
	void InitCtrl(CListCtrl *pCtrl);
	int  Display(CListCtrl *pCtrl = NULL);
	int  Display(CListCtrl *pCtrl, CExpression* pExpression, int nIdx = -1);
	
	// 计算参数管理
	void SetDate(BOOL bUseAll, int nBegin = 0, int nCount = 0);
	inline int  GetDayBegin() { return m_lBeginDate; }
	inline int  GetDayCount() { return m_lCount; }
	inline void SetTestObj(int nObj) { m_lTestObj = nObj; }
	inline int GetTestObj() { return m_lTestObj; }
	inline void SetTestCycle(int nCycle) { m_lTestCycle = nCycle; }
	inline int GetTestCycle() { return m_lTestCycle; }
	// 股票操作
	void AddStock(StockUserInfo* pInfo);
	void DelStock(StockUserInfo* pInfo);
	StockUserInfo* GetStock(int nIdx);
	// -1则表求没有
	int IsHave(StockUserInfo* pInfo);

	CString ReturnMsgPress(LPARAM lpID);

	// 主函数
	static UINT s_uTheradStateEx;

	LRESULT StartEx();
	LRESULT StopEx();

	LRESULT OnExchangeData(WPARAM wp,LPARAM lp);

protected:
	CListCtrl*	m_pwndExchList;
	CProgressCtrl* m_pwndProgress;
public:
	inline void SetListCtrl(CListCtrl*	pwndExchList) { m_pwndExchList = pwndExchList; }
	inline CListCtrl* GetListCtrl() { return m_pwndExchList; }
	inline void SetProgress(CProgressCtrl* pwndProgress) { m_pwndProgress = pwndProgress; }
	inline CProgressCtrl* GetProgress() { return m_pwndProgress; }

protected:
	int m_nCurCaclIdx; // 当前计算的指标索引
	LRESULT CaclNext(int nIdx = -1); // 计算下一个 如果没有，则发送优选完成信号
	// 当前处理进度
	long			m_lCurProgress;
	// 每支指标所点有的进度
	double			m_dProgressUint;

public:
	// 设置默认测试策略
	void	SetDefTact();
	// 设置自定义测试策略
	void	SetCommTact();

	// 结果处理
protected:
	CDlgSingleExchCalcResult*	m_dlgTable;
public:
	BOOL OpenTableWnd(UINT uID = 0);
	BOOL CreageTable(CWnd* pParent);
	BOOL DescTable();
};
