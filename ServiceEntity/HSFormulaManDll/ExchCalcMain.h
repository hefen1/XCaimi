/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	ExchCalcMain.h
*	文件标识：	
*	摘	  要：	交易系统测试计算主类定义
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/
#include "PubStruct.h"
#include "ExchData.h"

#pragma once

#define Sell_Mode_
#include <afxcoll.h>

struct CThreadStatusMutex
{
	CMutex m_mutex;
	long   m_lStatus;

	CThreadStatusMutex();
	BOOL SetVal(long lVal); // 设置值
	BOOL IsRun(); // 运行中
	BOOL IsStopping(); // 停止
	CThreadStatusMutex& operator=(long lVal);
	BOOL operator==(long lVal);
	BOOL operator!=(long lVal);
};

/*************************************************************
*	类 名 称：	CExchCalcMain
*	类 描 述：	交易系统评测计算主类(交易优选计算类的基类)
***************************************************************/
class CExchCalcMain: public TExchSys
{
public:
	CExchCalcMain();
	~CExchCalcMain();

protected:
	// 通知消息数据(窗口,消息ID及通知情况)
	TExchMsg	m_sMsg;

public:
	// 设置消息窗口句柄
	void SetMsgWnd(HWND hWnd) { m_sMsg.m_hNotifyWnd = hWnd; }
	// 设置消息窗口句柄
	void SetMsgWnd(CWnd* pWnd) { m_sMsg.m_hNotifyWnd = pWnd->GetSafeHwnd(); }
	// 设置消息ID
	void SetMsg(UINT hMsg) { m_sMsg.m_hNotifyMsg = hMsg; }
	// 设置消息数据
	void SetMsg(TExchMsg* pMsg){ if( pMsg == NULL ) return; memcpy(&m_sMsg, pMsg, sizeof(TExchMsg)); }
	// 得到消息窗口句柄
	HWND GetMsgWnd() { return m_sMsg.m_hNotifyWnd; }
	// 得到消息ID
	UINT GetMsgHead() { return m_sMsg.m_hNotifyMsg; }
	// 得到通知情况(是否发送)
	BOOL IsNotify() { return m_sMsg.m_bNotify; }
	// 向消息窗口发送消息
	static LRESULT	SendMsg(WPARAM wp, LPARAM lp);

public:
	// 是否设置数据(为1,表示正在评测运算,此时不再保存界面数据的改动, 其它则要保存)
	char	m_cIsApplySeting;
    // 设置m_cIsApplySeting状态
	void SetApplySeting(BOOL bSave);
	// 得到m_cIsApplySeting状态
	BOOL GetApplySeting();

public:
	// 从计算内存中得数据
	// 得到当前评测公式指针
	CExpression* GetExp(){return m_sExchExp.m_pMainTech;}
	// 得到包装了当前评测公式的对象
	TExchExp* GetExchExp() { return &m_sExchExp; }
	// 得到测试市场模型数据
	TExchModel* GetMode(){return &m_sTExchModel;}
	// 得到买入方法设置数据
	TExchBuy* GetBuy(){return &m_sTExchBuy;}
	// 得到卖出方法设置数据
	TExchSell* GetSell() {return &m_sTExchSell;}
	// 得到参数优选设置数据
	TExchOptimize* GetOptimize() { return &m_sTExchOptimize; }
	
	/******************************************************************
	*	函数功能：	得到相应的统计结果数据
	*	函数参数：	UINT uParamID = -1 : [in] 如果为-1,则返回最后一个计算结果数据,
											  否则,返回计算结果中,参数ID里为uParamID的计算结果数据
	*	返 回 值：	TCaclResult* : 返回的统计结果数据, 找不到则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	
	*****************************************************************/
	TCaclResult* GetOneResult(UINT uParamID = -1);

	/******************************************************************
	*	函数功能：	得到相应的成交明细数据
	*	函数参数：	UINT uParamID = -1 : [in] 如果为-1,则返回最后一个成交明细数据,
											  否则,返回计算结果中,参数ID里为uParamID的成交明细数据
	*	返 回 值：	TDealFlow* : 返回的成交明细数据,找不到则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	
	*****************************************************************/
	TDealFlow* GetDealFlow(UINT uParamID = -1);

	/******************************************************************
	*	函数功能：	得到相应的参数设置数据
	*	函数参数：	UINT uParamID = -1 : [in] 如果为-1,则返回最后一个参数设置数据,
											  否则,返回计算结果中,参数ID里为uParamID的参数设置数据
	*	返 回 值：	TExchParam*  : 返回的参数设置数据,找不到则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	
	*****************************************************************/
	TExchParam*  GetCurParam(UINT uParamID = -1);

	/******************************************************************
	*	函数功能：	得到参数ID为uParamID的计算结果在m_sCaclResult数组中的位置
	*	函数参数：	UINT uParamID : [in] 如果为-1 返回m_sCaclResult最后一个的索引
										 否则, 返回计算结果数组中,参数ID里为uParamID的计算结果在m_sCaclResult的索引
	*	返 回 值：	int : 返回的索引(>=0), 找不到(-1)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	
	*****************************************************************/
	int	GetCaclResult(UINT uParamID);
	// 得到计算结果数组(m_sCaclResult)的长度
	int GetCaclResultCount();

	/******************************************************************
	*	函数功能：	得到相应的计算结果数据(包含了成交明细,统计结果,参数设置)
	*	函数参数：	UINT uParamID = -1 : [in] 如果为-1,则返回最后一个计算结果数据,
											  否则,返回计算结果数组中,参数ID里为uParamID的计算结果数据
	*	返 回 值：	TExchCaclResult* : 返回的计算结果数据
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	
	*****************************************************************/
	TExchCaclResult* GetCaclResultItem(UINT uParamID = -1);

	/******************************************************************
	*	函数功能：	得到统计结果中,数据项uObjID的最大值的计算结果, 交易优选用使用
	*	函数参数：	UINT uObjID = HS_EXCH_CACLRESULT_STOCK_ID_PROFIT : [in] 要比较的统计数据项
	*	返 回 值：	TExchCaclResult* : 计算结果数组中数据项nObjID数值最大的计算结果
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	??? 比较最大值方面,应还存在点问题,具体开发时再改(HS)
	*****************************************************************/
	TExchCaclResult* GetObjResultItem(UINT uObjID = HS_EXCH_CACLRESULT_STOCK_ID_PROFIT);

	// 从测试市场模型数据中得到股票列表
	CArray<StockUserInfo*, StockUserInfo*>* GetStockList()
	{
		TExchModel* pModel = GetMode();
		return &pModel->m_ayStkList;
	}
	
	/******************************************************************
	*	函数功能：	最后一支计算股票是否有剩余金额
	*	函数参数：	（无）
	*	返 回 值：	BOOL : 有(TRUE), 无(FALSE)
	*	调用依赖：	GetOverplusMoney() 得到最后自由金额
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	
	*****************************************************************/
	BOOL IsHaveMoney() { return (GetOverplusMoney() > 0);}

	/******************************************************************
	*	函数功能：	得到相应股票或是最生一支股票剩余金额
	*	函数参数：	CodeInfo* pCode = NULL : [in] 股票代码, 为NULL时表示最后一支
	*	返 回 值：	double : 剩余金额数据
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	
	*****************************************************************/
	double GetOverplusMoney(CodeInfo* pCode = NULL);

	/******************************************************************
	*	函数功能：	得到相应股票或是最生一支股票剩余股票数-
	*	函数参数：	CodeInfo* pCode = NULL : [in] 股票代码, 为NULL时表示最后一支
	*	返 回 值：	long : 剩余股票数-
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	
	*****************************************************************/
	long GetLastStockNum(CodeInfo* pCode = NULL);
	// 得到最后一笔买入操作到目前的周期数-
	long GetLastCyc(CodeInfo* pCode = NULL);
	// 得到最后一笔买入的成交价
	double GetLastPrice(CodeInfo* pCode = NULL);
	// 得到最后一支股票的成效明细
	TDealFlowStock* GetLastFlow(CodeInfo* pCode = NULL);
	// 得到指定股票代码成交明细
	TDealFlowStock* GetFlowFormCode(CodeInfo *pCode);

	// 从界面保存数据到计算内存中-
	// 返回:保存成功(TRUE), 保存失败(FALSE)
	BOOL SetData(TExchHead* pData);
	// 保存公式
	BOOL SetExp(TExchHead* pData);
	// 保存公式
	BOOL SetExpress(CExpression* pPress);
	// 保存市场模型
	BOOL SetMode(TExchHead* pData);
	// 保存买入条件
	BOOL SetBuy(TExchHead* pData);
	// 保存卖出条件
	BOOL SetSell(TExchHead* pData);
	// 保存参数优化选项数据
	BOOL SetOptimize(TExchHead* pData);

	/******************************************************************
	*	函数功能：	将公式pPress中的参数列表初始化到参数设置选项数据中
	*	函数参数：	CExpression* pPress : [in] 初始化的公式
	*				BOOL bValid = FALSE : [in] 参数选化默认是否有效
	*	返 回 值：	BOOL : 处理成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	
	*****************************************************************/
	BOOL SetOptimize(CExpression* pPress, BOOL bValid = FALSE);

	// 复制股票列表到测试市场模型数据中
	void CopyStock(CArray<StockUserInfo*,StockUserInfo*>& sStock);
	// 从列表中得到第nIdx支股票
	StockUserInfo* GetStockAt(int nIdx);
	// 得到股票数
	long GetStockNum();	

	/******************************************************************
	*	函数功能：	加一个计算结果到计算结果数组中,并返回所加入数据空间的指针(优选时用)
	*	函数参数：	int nIdx : [in] 预备加入的计算结果相应的参数数据中的参数ID
	*				TExchParam *pPrvParam = NULL : [in] 前一个计算结果的参数数据
	*				int lDecimal = 0 : [in] 小数位
	*	返 回 值：	TExchCaclResult* : 如果处理成功,则返回加入的数据空间指针,否则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	TExchCaclResult* GetNewCaclRetultItem(int nIdx, TExchParam *pPrvParam = NULL, int lDecimal = 0);

	/******************************************************************
	*	函数功能：	加入一个计算结果到结果数组(m_sCaclResult)中
	*	函数参数：	TExchCaclResult* pItem : [in] 预加入的计算结果数据
	*	返 回 值：	BOOL : 加入成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	BOOL AddCaclResultItem(TExchCaclResult* pItem);

	// 小数
	int m_nDecimal;
	// 单位
	long m_nPriceUnit;

	// 计算线程相关
public:
	// 线程状态TCaclThread_State_Stoping停止, TCaclTherad_State_Start在运行-
	static CThreadStatusMutex s_uTheradState;
	// 指向当前计算数据(指向自己,一般情况下)
	static CExchCalcMain* s_pThis;

	/******************************************************************
	*	函数功能：	计算线程主函数
	*	函数参数：	LPVOID pData : [in] 为NULL或是调用本函数的CExchCalcMain对象指针
	*	返 回 值：	long : 计算返回信息
								TCaclReturn_NoData		没数据
								TCaclReturn_VainParam	参数据无效
								TCaclReturn_NoExperss	没有公式体
								TCaclReturn_CalFail		计算失败
								TCaclReturn_MoneryFail	内存分配失败
								TCaclReturn_TotalFail	统计失败
								TCaclReturn_Stop		用户停止
								TCaclReturn_Succ		成功 
								TCaclReturn_Fail		未知的失败
	*	调用依赖：	SingleStockTest(), EntireMarketTest()
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static long WINAPI CaclMain(LPVOID pData);

	/******************************************************************
	*	函数功能：	单股票市场模型测试
	*	函数参数：	int iCurParamIdx : [in] 当前计算在全局计算次数中的索引号(计算进度用,不使用参数优选时为0)
	*				int iCount		 : [in] 总共要计算的次数(不使用参数优选时为1)
	*	返 回 值：	long : 计算返回信息
								TCaclReturn_NoData		没数据
								TCaclReturn_VainParam	参数据无效
								TCaclReturn_NoExperss	没有公式体
								TCaclReturn_CalFail		计算失败
								TCaclReturn_MoneryFail	内存分配失败
								TCaclReturn_TotalFail	统计失败
								TCaclReturn_Stop		用户停止
								TCaclReturn_Succ		成功 
								TCaclReturn_Fail		未知的失败
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static long SingleStockTest(int iCurParamIdx, int iCount);
	
	/******************************************************************
	*	函数功能：	全市场模型测试 ??? 得再改进,算法中还是有不妥当之处HS
	*	函数参数：	int iCurParamIdx : [in] 当前计算在全局计算次数中的索引号(计算进度用,不使用参数优选时为0)
	*				int iCount		 : [in] 总共要计算的次数(不使用参数优选时为1)
	*	返 回 值：	long : 计算返回信息
								TCaclReturn_NoData		没数据
								TCaclReturn_VainParam	参数据无效
								TCaclReturn_NoExperss	没有公式体
								TCaclReturn_CalFail		计算失败
								TCaclReturn_MoneryFail	内存分配失败
								TCaclReturn_TotalFail	统计失败
								TCaclReturn_Stop		用户停止
								TCaclReturn_Succ		成功 
								TCaclReturn_Fail		未知的失败
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static long EntireMarketTest(int iCurParamIdx, int iCount);

	/******************************************************************
	*	函数功能：	计算停止处理
	*	函数参数：	（无）
	*	返 回 值：	long : TCaclReturn_Stop 用户停止
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static long CaclStop();

	/******************************************************************
	*	函数功能：	失败处理,向消息窗口发消息,将处理结果通知给目标窗口,让其做相应处理
	*	函数参数：	UINT msg = TCaclReturn_Fail : [in] 处理结果ID
														TCaclReturn_NoData		没数据
														TCaclReturn_VainParam	参数据无效
														TCaclReturn_NoExperss	没有公式体
														TCaclReturn_CalFail		计算失败
														TCaclReturn_MoneryFail	内存分配失败
														TCaclReturn_TotalFail	统计失败
														TCaclReturn_Stop		用户停止
														TCaclReturn_Succ		成功 
														TCaclReturn_Fail		未知的失败
	*	返 回 值：	long : 输入的msg参数
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static long Fail(UINT msg = TCaclReturn_Fail);

	/******************************************************************
	*	函数功能：	计算失败处理,往成交晚细数组中加一个空的明细
	*	函数参数：	CodeInfo* pCode : [in] 要处理的股票
	*	返 回 值：	long : 成功(TCaclReturn_Succ), 失败(TCaclReturn_Fail)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static long CaclFail(CodeInfo* pCode);
	// -

	/******************************************************************
	*	函数功能：	计算一支股票各公式指标(也含数字)的计算结果
	*	函数参数：	TCaclTemp& sCaclTemp : [in/out] 计算过程中用到的数据并存放计算结果
	*	返 回 值：	long : 计算成功(TCaclReturn_Succ), 否则失败
	*	调用依赖：	发消息及计算信息给childframe,让然调用公式计算得到数据
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static long CaclValues(TCaclTemp& sCaclTemp);

	/******************************************************************
	*	函数功能：	str是否为指标线(第一个是否为数字,为数字则不是指标线)
	*	函数参数：	CString& str : [in] 要判断的字串
	*	返 回 值：	BOOL : 不指标线(TRUE), 不是(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static BOOL IsTechLine(CString& str);

	/******************************************************************
	*	函数功能：	从pRet中得到数据
	*	函数参数：	TCaclResultTemp* pRet : [in] 指标计算结果数据
	*				FormulaCompareSubData* pSub : [in] 条件比较结构数据
	*				int nTechNum : [in] 第几个技术指标
										0 从pRet->m_lLeftReqult或是pSub->m_dData1得数据
										1 从pRet->m_lRightReqult或是pSub->m_dData2得数据
	*				int iIdx : [in] 从pRet->m_lLeftReqult或pRet->m_lRightReqult取数据时的索引
	*	返 回 值：	double : 取得的数据, 否则为0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static double GetTechData(TCaclResultTemp* pRet, FormulaCompareSubData* pSub, int nTechNum, int iIdx);

	/******************************************************************
	*	函数功能：	预处理(成交明细中,成交周期处理,最大浮动盈利及最大浮动亏损的处理)
	*	函数参数：	CodeInfo* pCode : [in] 要处理的代码
	*				StockDay* pDay  : [in] 日线数据
	*	返 回 值：	(无) 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static void PreDeal(CodeInfo* pCode, StockDay* pDay);

	/******************************************************************
	*	函数功能：	iDay天数据是否满足买入条件
	*	函数参数：	TCaclTemp* pData : [in] 指标计算结果
	*				int iDay		 : [in] 天索引
	*	返 回 值：	BOOL : 可买入(TRUE), 不可买入(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static BOOL IsBuy(TCaclTemp* pData, int iDay);

	/******************************************************************
	*	函数功能：	信号数的统计,并且计算各种胜率的天数(最后统计胜率时用)
	*	函数参数：	CodeInfo* pCode : [in] 要处理的股票
	*				StockDay* pData : [in] 日线数据
	*				int iDay		: [in] 要处理的天的索引
	*				int iDayLen		: [in] 日线数据pData的长度
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static BOOL PreBuyStock(CodeInfo* pCode, StockDay* pData, int iDay, int iDayLen);

	/******************************************************************
	*	函数功能：	买入动做 
	*	函数参数：	CodeInfo* pCode : [in] 要处理的股票
	*				StockDay* pData	: [in] 日线数据
	*				int iDay		: [in] 要处理的天数
	*				BOOL bNewBuy	: [in] 是否为新一次的买入(为连续买入时为FALSE,其它情况为TRUE)
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static BOOL BuyStock(CodeInfo* pCode, StockDay* pData, int iDay, BOOL bNewBuy);

	/******************************************************************
	*	函数功能：	股票买卖测试时的预处理,设置成交明细中开始价,结束价及测试周期数
	*	函数参数：	CodeInfo* pCode : [in] 要处理的股票
	*				StockDay* pData : [in] 日线数据
	*				BOOL bBuy		: [in] TRUE第一天处理(设置开始价), FALSE最后一天处理(结束价)
	*				int iCyc = 0	: [in] 测试周期数bBuy为TRUE是无效
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static BOOL BeginBuyEndSell(CodeInfo* pCode, StockDay* pData, BOOL bBuy , int iCyc = 0);

	/******************************************************************
	*	函数功能：	是否满足卖出条件,不管是否有股票可卖
	*	函数参数：	TCaclTemp* pData : [in] 指标计算结果数据
	*				int iDay		 : [in] 要处理的天的索引
	*	返 回 值：	long :  -1	不合卖出条件
							其它为可卖出返回:
								TDealFlow_DT_PingChang		平仓
								TDealFlow_DT_Sell			卖出
								TDealFlow_DT_ZhiShun		止损
								TDealFlow_DT_ZhiYing		止赢
								TDealFlow_DT_HuiLuo			回落
								TDealFlow_DT_HenPang		横盘
								TDealFlow_DT_QZPC			强制平仓
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static long IsSell(TCaclTemp* pData, int iDay);

	/******************************************************************
	*	函数功能：	卖出动做
	*	函数参数：	CodeInfo* pCode : [in] 要处理的股票
	*				StockDay* pData : [in] 日线数据
	*				int iDay		: [in] 要处理的天的索引
	*				int nMode		: [in] 卖出方式(IsSell()的可买返回)
	*	返 回 值：	BOOL 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static BOOL SellStock(CodeInfo* pCode, StockDay* pData, int iDay, int nMode);

	/******************************************************************
	*	函数功能：	统计所有数据(从成交明细中统计出各支股票的计算结果,
								 再将从各股的计算结果中统计出整个测试的结果)
	*	函数参数：	（无）
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static BOOL Total();
	// 清除计算结果m_sCaclResult数据
	BOOL ClearData();

	/******************************************************************
	*	函数功能：	设sParam值设置到当前测试的公式参数表中
	*	函数参数：	TExchParam& sParam : [in] 要设置的参数
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE) 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static BOOL InitialExpressParam(TExchParam& sParam);

public:
	/******************************************************************
	*	函数功能：	停止测试计算,也就是停止CaclMain()线程
	*	函数参数：	（无）
	*	返 回 值：	long : 成功(TCaclReturn_Succ), 失败(TCaclReturn_Fail) 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	long Stop();

	/******************************************************************
	*	函数功能：	开始测试计算,也就是启动CaclMain()线程进行计算
	*	函数参数：	（无）
	*	返 回 值：	long : 成功(TCaclReturn_Succ), 失败(TCaclReturn_Fail)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	long Start();

public:
	// 计算结果保存文件的文件头结构
	struct _tagFileHead
	{
		char szFlags[2]; // 文件头标志
		char cVer;		 // 版本号

		_tagFileHead()
		{
			szFlags[0] = 'h';
			szFlags[1] = 's';
			cVer = 1;
		}

		// 检查文件是否正确
		BOOL Check()
		{
			if( szFlags[0] == 'h'
				&& szFlags[1] == 's'
				&& cVer == 1
				)
				return TRUE;
			return FALSE;
		}

		void Serialize(CArchive& ar)
		{
			if( ar.IsStoring() )
			{
				ar << szFlags[0];
				ar << szFlags[1];
				ar << cVer;
			}
			else
			{
				ar >> szFlags[0];
				ar >> szFlags[1];
				ar >> cVer;
			}
		}
	};

	// 文件头
	_tagFileHead m_fileHead;
	// 保存测试计算结果 ??? 有待改进
	long Save(CWnd* pParent = NULL);
	// 读入测试计算结果数据 ??? 有待改进
	long Open(CWnd* pParent = NULL);

public:
	// 当前持有股票列表
	CMapStringToString m_ayCurStock;
	// 最后买入日期
	long m_lLastBuyDay;

	// 将股票加入当前持有股票列表m_ayCurStock中
	void AddBuyStock(CodeInfo* pInfo);
	// 将股票从当前持有股票列表m_ayCurStock中删除
	void DelBuyStock(CodeInfo* pInfo);
	// 得到当前持有的股票支数
	long GetBuyStockNum();
	// 当前股票列表中是否有股票pInfo
	BOOL IsHaveBuyStock(CodeInfo* pInfo);
	// 得到一次买入股票最大金额(没用)
	double GetBuyStockMaxMonery();
	// 得到一次买入股票最大的数量
	long GetBuyStockMaxNum();


	/******************************************************************
	*	函数功能：	全市场测试时,是否可买入的进一步判断(最大持有股票支数有没有超出)
	*	函数参数：	TExchModel* pModel : [in] 市场模型数据
	*				CodeInfo* pInfo    : [in] 要处理的股票
	*				int nCurNum		   : [in] 要买入数量(没用???)
	*	返 回 值：	BOOL : 可买(TRUE), 不可买(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static BOOL CheckEntireIsBuy(TExchModel* pModel, CodeInfo* pInfo, int nCurNum);
	enum { MoneryAdd/*加*/, MoneryDel/*减*/, MonerySet/*重新设置*/ };

	/******************************************************************
	*	函数功能：	设置最后的金额
	*	函数参数：	double dMonery			: [in] 要操作的金额数
	*				int nMask = MoneryAdd	: [in] 操作方式(加,减或重设)
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	static void SetLastMonery(double dMonery, int nMask = MoneryAdd, CodeInfo* pCode = NULL);

public:
	// 列表图标资源
	CImageList			m_oImage;
	// 初始化列表(清空项目,初始化列表列及图标)
	BOOL InitialOptimizeCtrl(CListCtrl* pCtrl, CImageList* pImage = NULL);

	/******************************************************************
	*	函数功能：	显示测试结果列表
	*	函数参数：	CListCtrl* pCtrl : [in] 目标列表控件
	*	返 回 值：	BOOL : 参数不正确(FALSE), 其它(TRUE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	BOOL ToOptimizeCtrl(CListCtrl* pCtrl);

	/******************************************************************
	*	函数功能：	得到参数（其ID为uID）的（2,1,1)行式的参数字串
	*	函数参数：	BOOL bSignal  : [in] 单股票(TRUE), 全市场(FALSE)
	*				UINT uID = -1 : 参数ID
	*	返 回 值：	CString : 参数字串
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	CString GetParamStr(BOOL bSignal, UINT uID = -1);

	/******************************************************************
	*	函数功能：	比较大小，参数优化的表格排序使用
	*	函数参数：	UINT uID1 : [in] 比较的第一个计算结果的参数设置的参数ID
	*				UINT uID2 : [in] 比较的第二个计算结果的参数设置的参数ID
	*				int ColID : [in] 要比较的计算结果数据的ID
	*	返 回 值：	int : 0 相等
						 -1 第一个计算结果小于第二个计算结果 
						  1	第一个计算结果大于第二个计算结果
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	int CompareParamOfCol(UINT uID1, UINT uID2, int ColID);

	/******************************************************************
	*	函数功能：	将默认参数m_sDefParam设置到pExp的参数
	*	函数参数：	CExpression* pExp = NULL : [in] 要设置的公式, 为空则设置当前测试公式
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	BOOL ResetDefParam(CExpression* pExp = NULL);

	/******************************************************************
	*	函数功能：	将pExp的参数设置到默认参数m_sDefParam
	*	函数参数：	CExpression* pExp = NULL : [in] 要处理的公式,为空则指当前测试公式
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月24日
	*	修改说明：	
	*****************************************************************/
	BOOL FromExpToDefParam(CExpression* pExp = NULL);
	// 读入设置 2003-08-19 加入
	BOOL ReadSetting();
	// 写出设置 2003-08-19 加入
	BOOL WriteSetting();

	static long GetStockPriceDecimal(CString strCode, int& index);
	static long GetStockPriceUnit(CString strCode,int& index);
};

