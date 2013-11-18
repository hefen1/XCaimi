/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	ExchData.h
*	文件标识：	
*	摘	  要：	交易系统相关信息定义
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	2003-1-8
*
***************************************************************/

#ifndef _ExchData_H_
#define _ExchData_H_

#include "Express.h"
#include "ExpPublic.h"
#include "VarValue.h"
#include "hsds_comudata.h"
// 交易系统
// 数据类型
#define EXCH_DATA_TYPE_Exp	1
#define EXCH_DATA_TYPE_Mode	2
#define EXCH_DATA_TYPE_Buy	3
#define EXCH_DATA_TYPE_Sell	4
#define EXCH_DATA_TYPE_Optimize	5

// 通知消息
// 进度通知
#define EXCH_MSG_WP_CACL_TEMPO	7770
// 开始
#define EXCH_MSG_WP_CACL_START	7771
// 停止
#define EXCH_MSG_WP_CACL_STOP	7772
// 计算失败
#define EXCH_MSG_WP_CACL_FAIL	7773
// 计算完成
#define EXCH_MSG_UP_CACL_SUCC	7774
// 优选开始
#define YouSel_MSG_WP_CACL_START 7775
// 优选停止
#define YouSel_MSG_WP_CACL_STOP	7776
// 优选失败
#define YouSel_MSG_WP_CACL_FAIL 7777
// 优先完成
#define YouSel_MSG_UP_CACL_SUCC	7778
// 重新设置BUY方式
#define EXCH_MSG_WP_CALC_SET_BUY 7779

// 买入方式
#define BuyMethod_All_Buy			0	// 全部买入
#define BuyMethod_Portion_Buy		1	// 部分买入
#define	BuyMethod_Fixation_Money	2	// 固定资金买入
#define	BuyMethod_Fixation_NUM		3	// 固定股票数买入

// 连续信号出现时的处理方式
#define FollowBuyMethod_NO			0	// 不再买入
#define FollowBuyMethod_DL			1	// 等量买入
#define FollowBuyMethod_DJ			2	// 递减买入
#define FollowBuyMethod_DZ			3	// 递增买入

// 交易类型名称定义
#define TDealFlow_DT_PingChang_Code	"平仓"
#define TDealFlow_DT_Buy_Code		"买入"	
#define TDealFlow_DT_Sell_Code		"卖出"
#define TDealFlow_DT_ZhiShun_Code	"止损"
#define TDealFlow_DT_ZhiYing_Code	"止赢"
#define TDealFlow_DT_HuiLuo_Code	"回落"
#define TDealFlow_DT_HenPang_Code	"横盘"
#define TDealFlow_DT_No_Code		"其它"
#define TDealFlow_DT_QZPC_Code		"平仓"
// 交易类型ID定义
#define TDealFlow_DT_PingChang		1	// 平仓
#define TDealFlow_DT_Buy			100	// 买入
#define TDealFlow_DT_Sell			3	// 卖出
#define TDealFlow_DT_ZhiShun		4	// 止损
#define TDealFlow_DT_ZhiYing		5	// 止赢
#define TDealFlow_DT_HuiLuo			6	// 回落
#define TDealFlow_DT_HenPang		7	// 横盘
#define TDealFlow_DT_QZPC			8	// 强制平仓

// 交易系统函数返回
#define TCaclReturn_NoData		100	// 没数据
#define TCaclReturn_VainParam	101 // 参数据无效
#define TCaclReturn_NoExperss	102 // 没有公式体
#define TCaclReturn_CalFail		103 // 计算失败
#define TCaclReturn_MoneryFail	104 // 内存分配失败
#define TCaclReturn_TotalFail	105 // 统计失败
#define TCaclReturn_Stop		106 // 用户停止
#define TCaclReturn_Succ		0   // 成功 
#define TCaclReturn_Fail		1   // 未知的失败

// 计算线程状态
#define TCaclThread_State_Stoping	-1 // 已经停止 
#define TCaclThread_State_Stop		0  // 要求停止
#define TCaclTherad_State_Start		1  // 正在运行

extern double EXCH_POUNDAGE;    // 手续费率(以后得设成可设置)

/*************************************************************
*	类 名 称：	TExchHead
*	类 描 述：	数据公共头
***************************************************************/
struct TExchHead
{
	BYTE	m_bType;	// 数据类型
	virtual void Serialize(CArchive& ar);
};

/*************************************************************
*	类 名 称：	TExchMsg
*	类 描 述：	消息处理数据
***************************************************************/
struct TExchMsg
{
	HWND	m_hNotifyWnd; // 消息接收窗口
	UINT	m_hNotifyMsg; // 消息ID
	BOOL	m_bNotify;    // 是否发送

	TExchMsg();

	// 判断是否发送消息
	BOOL IsNotify();   
	// 向窗口m_hNotifyWnd发送m_hNotifyMsg消息
	LRESULT SendMsg(WPARAM wp, LPARAM lp);
};

/*************************************************************
*	类 名 称：	TExchExp
*	类 描 述：	公式数据
***************************************************************/
struct TExchExp: public TExchHead
{
	CExpression*	m_pMainTech;	// 公式体
	long			m_lCyc;			// 周期
	BOOL		    m_bExRights;			// 是否除权计算 

	TExchExp();
	TExchExp& operator=(TExchExp& sExp);
	// 得到公式类型名字
	CString GetExpTypeName();
	// 得到公式参数表达串
	CString GetParamStr();
	void Serialize(CArchive& ar, CMapVariabile* pMapExpression = NULL);
};

// 得到股票才务数据的函数指针类型
/*typedef StockUserInfo* (*GETSTOCKINFOPROC)(CodeInfo& sCode);*/
typedef void (*GETSTOCKINFOPROC)(CodeInfo sCode, StockUserInfo&);
/*************************************************************
*	类 名 称：	TExchModel
*	类 描 述：	市场数据
***************************************************************/
struct TExchModel: public TExchHead
{
	BOOL	m_bSingle;			// 单股票还是多股票
	short   m_nHoleStockCount;	// 同时持有股票数
	BOOL	m_bBuyAll;			// 全部买入还是多部分买入
	short   m_nOneBuyCount;		// 一次买入数量
	BOOL	m_bBuyRandom;		// 随机买入
	BOOL	m_bSToB;						// 从小到大  当m_bBuyRandom == 0
	SingleSelCondition	m_sSortReason;			// 排序参照
	double    m_lAllInvestCount;			// 所有资金
	GETSTOCKINFOPROC m_ProcGetStockUser;	// 股票数据获得函数指针
	CArray<StockUserInfo*, StockUserInfo*>	m_ayStkList; // 股票列表
	
	TExchModel();
	~TExchModel();
	
	void Clear();
	void Copy( TExchModel &Model );
	TExchModel& operator=(TExchModel& Mode);
	void Serialize(CArchive& ar);
};

/*************************************************************
*	类 名 称：	TExchBuy
*	类 描 述：	买入规则数据
***************************************************************/
struct TExchBuy: public TExchHead
{
	// 测试时间段数据
	long			m_lDateCount;		// 测试时间段长度
	static long		m_lBeginDate;		// 开始测试时间 
	static long		m_lEndDate;			// 结束测试时间
	BOOL			m_bNoCountIn;		// 强制平仓比计入测试结果，1不计入，0，计入

	// 买入规则数据
	CString			m_strTechLeft;		// 比较指标一
	FormulaCompareSubData m_SubData;			// 条件关系(买入规则)
	short			m_nBuyMethod;		// 买入股票的方式，全部买入，部分买入，固定资金买入，固定股票数买入
	float           m_fOneBuyCount;     // 一次买入数量

	// 连继信号数据
	short			m_nFollowBuyMethod;  // 出现连续信号时的买入方式 ，不再买入，等量买入，递减买入，递增买入
	float           m_fAddOrSubCount;    // 递增或递减数量
	BOOL			m_bSkip;			 // 忽略周期
	short			m_nInPeriod;		 // 忽略连续出现的周期

	TExchBuy();
	TExchBuy& operator=(TExchBuy& Buy);
	void Default(CExpression *pExp, CString strDefault);
	void Serialize(CArchive& ar);
};

/*************************************************************
*	类 名 称：	TExchSell
*	类 描 述：	卖出规则数据
***************************************************************/
struct TExchSell: public TExchHead
{
	BOOL	m_bMaidPeriod;		//目标周期平仓
	BOOL	m_bWinPeriod;		//目标利润止赢
	BOOL	m_bLosePeriod;		//目标止损
	BOOL	m_bFallPeriod;		//回落平仓
	BOOL	m_bUpPeriod;		//横盘平仓

	float	m_nMaidPeriod;		//目标周期平仓
	float	m_nWinPeriod;		//目标利润止赢
	float	m_nLosePeriod;		//目标止损
	short	m_nFallPeriod;		//回落平仓
	short	m_nUpPeriod;		//横盘平仓
	short   m_nFallPeriodCount;		//回落平仓数值
	short	m_nUpPeriodCount;		//横盘平仓数值

	TExchSell();
	TExchSell& operator=(TExchSell& sell);
	void Serialize(CArchive& ar);
};

/*************************************************************
*	类 名 称：	TExchParam
*	类 描 述：	公式的参数(优化时用)
***************************************************************/
struct TExchParam
{
	UINT	m_uParamID; // 参数ID
	long	m_lDecimal;	// 小数位
	long	m_lSize;	// m_pParam长度
	double*	m_pParam;   // 参数数组

	TExchParam()
	{
		m_pParam = NULL;
		m_lSize = 0;
		m_lDecimal = 0;
	}

	~TExchParam()
	{
		Free();
	}

	TExchParam& operator=(TExchParam& sData)
	{
		m_uParamID = sData.m_uParamID;
		m_lDecimal = sData.m_lDecimal;

		if( New(sData.m_lSize) != NULL )
		{
			memcpy(m_pParam, sData.m_pParam, sizeof(double)*m_lSize);
		}

		return *this;
	}

	// 空间分配并返回分配的空间
	double* New(long lSize)
	{
		Free();

		m_pParam = new double[lSize];
		if( m_pParam != NULL )
		{
			m_lSize = lSize;
		}

		ZeroMemory(m_pParam, sizeof(double)*m_lSize);

		return m_pParam;
	}

	// 清空
	void Free()
	{
		if( m_pParam != NULL && m_lSize > 0 )
			delete[m_lSize] m_pParam;
		m_pParam = NULL;
		m_lSize = 0;
	}

	// 空间是否有效
	BOOL IsValid()
	{
		if( m_pParam != NULL && m_lSize > 0
			&& ::AfxIsValidAddress(m_pParam, sizeof(double)*m_lSize, TRUE) )
			return TRUE;

		return FALSE;
	}

	// 转化到字串,如m_pParam={0.1, 1.2, 0.23} --> "(0.1,1.2,0.23)"
	CString ToStr()
	{
		if( !IsValid() )
			return CString("无参数");

		CString str("("), strTemp;
		for( int i = 0 ; i < m_lSize; i++ )
		{
			strTemp.Format("%.2f", m_pParam[i]);
			strTemp.TrimRight('0');
			strTemp.TrimRight('.');
			if( i != 0 ) str += ",";
			str += strTemp;
		}

		str += ")";
		return str;
	}

	// 将公式中的参数转到m_pParam中
	BOOL ConvertFromExpress(CExpression* pExpress)
	{
		if( pExpress == NULL )
			return FALSE;

		CMapVariabile* pMapVal = pExpress->GetParamVar();
		if( pMapVal == NULL )
			return FALSE;

		if( !New(pMapVal->GetSize() ) )
			return FALSE;

		for( int i = 0; i < m_lSize; i++ )
		{
			CNumericValue* pItem = (CNumericValue*)pMapVal->GetAt(i);

			if( pItem != NULL )
			{
				m_pParam[i] = pItem->GetValue();
			}
		}

		return TRUE;
	}

	// 将m_pParam内容设置到公式pExpress中
	BOOL ConvertToExpress(CExpression* pExpress)
	{
		if( pExpress == NULL )
			return FALSE;

		CMapVariabile* pMapVal = pExpress->GetParamVar();
		if( pMapVal == NULL )
			return FALSE;

		if( pMapVal->GetSize() != m_lSize )
			return FALSE;

		for( int i = 0; i < m_lSize; i++ )
		{
			CNumericValue* pItem = (CNumericValue*)pMapVal->GetAt(i);

			if( pItem != NULL )
			{
				pItem->SetValue(m_pParam[i]);
			}
		}

		return TRUE;
	}
};

/*************************************************************
*	类 名 称：	TExchOptimizeItem
*	类 描 述：	单个优化选项
***************************************************************/
struct TExchOptimizeItem
{
	TCHAR  m_szName[64]; // 参数名
	double m_dMin;		 // 最小值	
	double m_dMax;		 // 最大值
	double m_dAdd;		 // 测试步长	

	TExchOptimizeItem();
	~TExchOptimizeItem();
	TExchOptimizeItem& operator=(const TExchOptimizeItem& sItem);
	long GetNum();
	// 第到第N个数据
	double GetParam(int nIdx);
	CString GetName() {  return CString(m_szName); }
	void Serialize(CArchive& ar);
	// 将本对象的数据转换到CNumericValue
	void ConvertToNumVal(CNumericValue& sValue);
	// 从CNumericValue转换到本对象
	void ConvertFromNumVal(CNumericValue* sValue, CMapVariabile* sMapVal, BOOL bValid = FALSE);
};

/*************************************************************
*	类 名 称：	TExchOptimize
*	类 描 述：	所有优化先项数据
***************************************************************/
struct TExchOptimize: public TExchHead
{
	// 有效长度
	int lValidLen;
	// 缓冲区大小
	int lBufLen;
	// 数据缓冲区
	TExchOptimizeItem* pItem;
	// 相应项是否参加优化长度为lBufLen
	char* pIsValid;

	TExchOptimize();
	~TExchOptimize();

	TExchOptimize(const TExchOptimize& sData);
	TExchOptimize& operator=(const TExchOptimize& sData);
	TExchOptimizeItem* operator[](int iIdx);
	BOOL IsValid();
	BOOL IsValidItem(int nIdx);
	BOOL New(int lLen);
	void Free();
	void Serialize(CArchive& ar);
	// 得到有效参加计算的参数数
	int  GetValidItemCount(); 
	// 得到总共要计算的次数
	long GetCaclUionCount();
	// 得到第N个能数组和
	BOOL GetCaclUionItem(long nIdx, TExchParam& sRet);
	// 得到第N个有效的参数
	TExchOptimizeItem* GetValidItem(long nIdx);
	// 得到第N个有效参数的全局索引
	int ValidIdxToGlobalIdx(long nIdx);
	// 转换到CMapVariabile
	void ConvertToMapNumVal(CMapVariabile& sMapVal);
	// 从CMapVariabile转来
	BOOL ConvertFromMapNumVal(CMapVariabile* sMapVal, BOOL bValid = FALSE);
	// 从公式数据转换
	BOOL ConvertFromExpress(CExpression* pExpress, BOOL bValid = FALSE);
};

// 股票代码(由于原来的那个有vpt,所以另定了这个
struct CodeInfoEx
{
	HSMarketDataType	m_cCodeType;//证券类型
	char				m_cCode[6]; //证券代码
	CodeInfoEx& operator=(CodeInfo& sCode)
	{
		memcpy(this, &sCode, sizeof(CodeInfoEx));
		return *this;
	}
};

/*************************************************************
*	类 名 称：	TCaclOneStockResult
*	类 描 述：	一支股票结果
***************************************************************/
struct TCaclOneStockResult
{
	union
	{
		LONGLONG	m_llData;
		CodeInfoEx	m_sCode;					// 股票代码(单支股票时使用)
		long		m_lStockNum;				// 股票支数(综合统计时使用)
	};
	double		m_dNetProfit;					// 净利润
	double		m_dNPPercent;					// 净利润率NP=Net Profit
	double		m_dTotalProfit;					// 总盈利
	double		m_dTotalDificit;				// 总亏损
	long		m_lDealNum;						// 交易次数
	double		m_dWinPercent;					// 胜率
	double		m_lYearDealNum;					// 年均交易次数
	long		m_lProfitDealNum;				// 盈利交易次数
	long		m_lDificitDealNum;				// 亏损交易次数
	double		m_dMaxSingleProfit;				// 最大单次盈利
	double		m_dMaxSingleDificit;			// 最大单次亏损

	long		m_lMaxContinuProfitNum;			// 最大连续盈利次数
	long		m_lMaxContinuDificitNum;		// 最大连续亏损次数 
	double		m_dDealAverageCyc;				// 交易平均周期数
	double		m_dProfiltAverageCyc;			// 盈利平均交易周期
	double		m_dDiticitAverageCyc;			// 亏损平均交易周期

	double		m_dMaxFloatProfit;				// 最大浮动盈利
	double		m_dMaxFloatDificit;				// 最大浮动亏损

	double		m_dAllJumpIn;					// 总投入
	double		m_dValidJumpIn;					// 有效投入
	double		m_dYearReturnPercent;			// 年回报率
	double		m_dYearValidReturnPercent;		// 年有效回报率
	double		m_dSimpleBuyHaveReturnPercent;	// 简单买入持有回报
	double		m_dBuyHaveYearPercent;			// 买入持有年回报率
	double		m_dAllDealAmount;				// 总交易额
	double		m_dAllDealCost;					// 总交易费
	long		m_lBeginDate;
	long		m_lEndDate;						// 测试时间
	long		m_lTestCycNum;					// 测试周期数
	double		m_dAverageCW;					// 平均仓位
	double		m_dMaxCW;						// 最大仓位
	double		m_dAverageCCL;					// 平均持仓量
	double		m_dMaxCCL;						// 最大持仓量
	double		m_dSucessPercent;				// 成功率
	double		m_dSignalNum;					// 信号数量
	double		m_d5DayProfitProbability;		// 五日获利概率
	double		m_d10DayProfitProbability;		// 十获利概率
	double		m_d20DayProfitProbability;		// 二十获利概率
	double		m_dAimCycProfitProbability;		// 目标周期获利概率

	TCaclOneStockResult();

	TCaclOneStockResult& operator=(TCaclOneStockResult& sData);
	TCaclOneStockResult& operator+=(TCaclOneStockResult& sData);
	void ToListCtrl(CListCtrl *pCtrl);
	void Serialize(CArchive& ar);
};			

// 单成交明细
struct TDealFlowItem
{
	WORD		m_wDealType;	// 交易类型
	unsigned long		m_lDealDate;	// 交易时间
	double		m_dDealPrice;	// 成交价
	LONG		m_lDealAmount;	// 成交量

	double		m_dDealProfit;	// 收益(在卖出时有效,正数表示获利,负数表示亏损)

	// 交易类型为买入时以下有效
	LONG		m_lCaclDays;	// 成交日到目前时间(最后值为成交周期=卖出时间-买入时间),计算周期平仓,回落平仓,横盘平仓用

	TDealFlowItem();
	TDealFlowItem& operator++();
	TDealFlowItem& operator=(TDealFlowItem& sData);
	CString GetDealType();
	void ToListCtrl(CListCtrl* pCtrl, CString& strName);
	void Serialize(CArchive& ar);
};

// 一支股票成交明细
struct TDealFlowStock
{
	static long		m_lLastBuy;		// 当前最后一笔有效的买入
	static double	m_dAllMoney;	// 初始资金
	static BOOL		m_bLastSell;	// 最后日平仓是否加入统计
	static double	s_dLastMonery;	// 全市场测试模型时用

	CodeInfo	m_sCode;		// 代码
	CString		m_strName;		// 名称
	long		m_lLastStockNum; // 最后一笔交易后所剩下的股票
	double		m_lLastMoney;	// 最后一笔交易后所剩下金钱
								// 看是否大于0,还有可投入金额(部分投入时使用)
								// 如果没有,则只要计算是否有合条件的卖出或平仓
								// 否则得同时计算是否还有可买入时机
	CArray<TDealFlowItem*, TDealFlowItem*> m_ayItem;	// 第笔明细

	double	m_dBeginPrice;		// 开始时价格
	double	m_dEndPrice;		// 结束时价格	
	long	m_lTestCycNum;		// 测试周期

	long	m_lBuySignalNum;	// 买入信号数
	long	m_lWinDays;			// 计算成功率用
	long	m_lWin5Day;			// 五日胜次
	long	m_lWin10Day;		// 十日胜次
	long	m_lWin20Day;		// 二十日胜次
	long	m_lWinTabCyc;		// 目标周期胜次

	double		m_dJumpInMoney; // 投入单次交易金额
	long		m_lSignalNum;	// 信号数量
	double		m_dMaxFDProfit; // 最大浮动盈利
	double		m_dMaxFDDificit; // 最大浮动亏损

	TDealFlowStock();
	TDealFlowStock(TDealFlowStock* pData);
	TDealFlowStock(CodeInfo& sCode, CString& strName, double dLastMoney);

	~TDealFlowStock();

	/******************************************************************
	*	函数功能：	根据成交明细,进行统计,并将结果放入pItem, 如果pItem为空,则New一个
	*	函数参数：	TCaclOneStockResult* pItem = NULL : [in] 统计结果存放
	*				BOOL bSingle = TRUE				  : [in] TRUE: 单股票, FALSE : 全市场 
	*	返 回 值：	TCaclOneStockResult* : 统计结果
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月18日
	*	修改说明：	
	*****************************************************************/
	TCaclOneStockResult* TotalTo(TCaclOneStockResult* pItem = NULL, BOOL bSingle = TRUE);
	// 得到成交次数
	long GetCount();
	// 加一个成交明细到最后,如果pItem为NULL,则New一块空间,然后返回
	TDealFlowItem* Append(TDealFlowItem* pItem = NULL);
	// 得到最生一个成交明细
	TDealFlowItem* GetLastItem();
	// 得到第iIdx的成交明细
	TDealFlowItem* GetItem(int iIdx);
	// 得到最后一笔成交的价格
	double GetLastPrice();
	// 得到最后一笔成交经过的周线数
	LONG GetLastCaclDays();
	// 是否还有剩余的钱
	BOOL IsHaveMoney();
	// 是否还有剩余的股票
	BOOL IsHaveStock();
	// 清空成交明细占用空间
	void Free(void);
	TDealFlowStock& operator=(TDealFlowStock& sData);
	// 将成交明细在pCtrl中显示出来
	void ToListCtrl(CListCtrl *pCtrl);
	// 清空所有
	void Clear();
	void Serialize(CArchive& ar);
};

// 成交明细
#define HS_EXCH_DEALFLOW_HANDLE_TIME		"成交日期"
#define HS_EXCH_DEALFLOW_HANDLE_STK		"股票代码"
#define HS_EXCH_DEALFLOW_HANDLE_TYPE		"交易类型"
#define HS_EXCH_DEALFLOW_HANDLE_PRICE		"成交价"
#define HS_EXCH_DEALFLOW_HANDLE_AMOUNT		"成交量"
#define HS_EXCH_DEALFLOW_HANDLE_PROFIT		"收益(元)"
struct TDealFlow
{
	CArray<TDealFlowStock*, TDealFlowStock*> m_ayData; // 所有股票的成交明细(参加计算)

	~TDealFlow();

	// 得到最后一支股票的成交明细
	TDealFlowStock* GetLastFlow();
	// 得到第iIdx支股票的成交明细
	TDealFlowStock* GetItem(int iIdx);
	// 得到股票pCode的成交明细
	TDealFlowStock* GetFlowFormCode(CodeInfo* pCode);
	// 加一支股票的成交明细
	void AddStockItem(TDealFlowStock* pItem);
	// 得到股票数
	int GetCount();
	// 清空
	void Free(void);
	TDealFlow& operator=(TDealFlow& sData);
	// 将第nIdx股票的数据显示到pCtrl中
	void ToListCtrl(CListCtrl *pCtrl, int nIdx);
	// 初始化pCtrl,删除原来所有,设置IMAGE,设置例
	void Initital(CListCtrl* pCtrl, CImageList *pImage);
	void Serialize(CArchive& ar);
};

// 计算结果
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_STKNAME	"股票名"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_STKCODE	"股票代码"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_PARAM		"参数"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_PROFIT		"利润率"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_YEAR		"年回报"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_WIN		"胜率"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_DEALNUM	"交易数"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_SUCC		"成功率"

#define HS_EXCH_CACLRESULT_STOCK_ID_STKNAME	0
#define HS_EXCH_CACLRESULT_STOCK_ID_STKCODE	0
#define HS_EXCH_CACLRESULT_STOCK_ID_PARAM		0
#define HS_EXCH_CACLRESULT_STOCK_ID_PROFIT		1
#define HS_EXCH_CACLRESULT_STOCK_ID_YEAR		2
#define HS_EXCH_CACLRESULT_STOCK_ID_WIN		3
#define HS_EXCH_CACLRESULT_STOCK_ID_DEALNUM	4
#define HS_EXCH_CACLRESULT_STOCK_ID_SUCC		5

/*************************************************************
*	类 名 称：	TCaclResult
*	类 描 述：	计算结果
***************************************************************/
struct TCaclResult
{
	TCaclOneStockResult	m_sTotal;	// 所有股票的结果统计
	CArray<TCaclOneStockResult*, TCaclOneStockResult*> m_ayAllStock; // 单支股票的结果统计

	TCaclResult();
	~TCaclResult();
	// 加一个股票的统计
	void Add(TCaclOneStockResult* pResult);
	// 统计所有(m_ayAllStock到m_sTotal)
	BOOL Total(BOOL bSingle = TRUE);
	// 清空占用空间
	void Free(void);
	TCaclResult& operator=(TCaclResult& sData);
	// 将统计结果显示到pCtrl
	void ToListCtrl(CListCtrl* pCtrl);
	// 初始化pCtrl,删除原来所有,设置IMAGE,设置例
	void Initital(CListCtrl* pCtrl, CImageList *pImage = NULL);
	void Serialize(CArchive& ar);

	// 根据ID得到总的统计结果中的数据
	double GetIDData(int nID);
};

/*************************************************************
*	类 名 称：	TCaclResultTemp
*	类 描 述：	指标计算结果，计算中间过程使用
***************************************************************/
struct TCaclResultTemp
{
	long	m_lDaysLen;				// 相对日线是倒数长度
	long	m_lBufLen;				// 两BUF长度,应是相同

	long	m_lDescLen;				// 源计算结果长度
	HSDouble *m_lDescReqult;		// 源计算结果

	long	m_lLeftLen;				// 第一个比较指标结果长度
	HSDouble *m_lLeftReqult;		// 第一个比较指标计算结果

	long	m_lRithtLen;			// 第二个比较指标有效长度
	HSDouble *m_lRightReqult;		// 第二个比较指标计算结果



	TCaclResultTemp(TCaclResultTemp *pData = NULL);
	~TCaclResultTemp();

	BOOL Copy(TCaclResultTemp *pData);
	BOOL IsValid();
	void New(long lSize);
	void Del();
	enum { ToDesc = 0, ToLeft, ToRight, ToOther };
	BOOL Copy(HSDouble* pData, long lCount, int nMask = ToDesc);
};

// 给计算过程的参数结构
struct TCaclTemp
{
	CodeInfo	*pCode;				// 股票代码
	CExpression	*pPress;			// 公式名
	CString		szTechMain;			// 主指标
	CString		szTechSideLeft;			// 比较指指标(可无)
	CString		szTechSideRitht;			// 比较指指标(可无)
	long		lEndDate;			// 结束日期
	long		lBeginDate;			// 开始日期
	long		lCountDate;			// 周期数
	long		lBuffDiskData;		// pRetDiskData长度
	long		nChengJiaoDiv;		// 成交量单位
	long		nAnaPeriod;			// 周期单位

	StockDay	*pRetDiskData;		// 返回的日线数据空间,些空间不用删除
	TCaclResultTemp	*pRetCacl;		// 返回的最多两个指标的计算结果

	BOOL		bExRights;			// 是否除权计算 

	TCaclTemp();
	TCaclTemp(TCaclTemp& sTemp);
	~TCaclTemp();

	void New(int iDiskDataLen = 0);
	void Free(BOOL bAll = FALSE);
	void ClearData();
};

// 针对一个参数设置的计算结果
struct TExchCaclResult
{
	// 当前公式的参数
	TExchParam		m_sCurParam;
	// 计算结果(统计m_sDealFlow所得)
	TCaclResult		m_sCaclResult;
	// 成交明细
	TDealFlow		m_sDealFlow;

	TExchParam*  GetCurParam()	{ return &m_sCurParam; }
	TCaclResult* GetResult()	{ return &m_sCaclResult; }
	TDealFlow*   GetDealFlow()	{ return &m_sDealFlow; }
	// 得到对应ID的数据
	double GetIDData(int nID);
	TExchCaclResult& operator=(TExchCaclResult& sData)
	{
		m_sCurParam = sData.m_sCurParam;
		m_sCaclResult = sData.m_sCaclResult;
		m_sDealFlow = sData.m_sDealFlow;
		return *this;
	}
	// 显示到列表
	void ToListCtrl(CListCtrl* pCtrl);
};

#endif