/*************************************************************
*	Copyright (c)2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	HSDataSource.h
*	文件标识：	
*	摘	  要：	数据定义
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
***************************************************************/

#if !defined(_HXDATASOURCE_H_)
#define _HXDATASOURCE_H_

#include "hsds_comudata.h"
#include "FormulaStruct.h"
#include "ExpPublic.h"
#include "DataSourceDefineEx.h"

#define HS_HxDataSource_DynNews			0x00010000 // 动态信息

#define HS_HxDataSource_SelectStock_Mark   0x00080000 // 选股提示


class CHSDisk;
#define FinanceData HisFinanceData
/*************************************************************
*	类 名 称：	CHSDataSource
*	类 描 述：	数据源管理类
***************************************************************/
class  CHSDataSource : public CObject
{
	DECLARE_DYNCREATE(CHSDataSource)
public:
	// 对请求包数据等进行初始化处理
	CHSDataSource(AskData* pCode = NULL,
				  DWORD dwStyle = HS_HxDataSource_Left_ChuQuan		| 
								  HS_HxDataSource_Read_CaiWu_Data);
	// 清空点用资源
	virtual ~CHSDataSource();
	// 清空点用资源
	virtual void Free();

protected:
	DWORD	   m_dwStyle;		// 数据处理风格 如左边除权,向后除权等
	AskData*   m_pAsk;			// 请求包数据

public:
	HSDouble   m_nVolumeUnit;    // 成交量单位
	HSDouble   m_nMoneyUnit;     // 成交额单位
	short	   m_nMoneyDecimal;  // 成交额小数位

	HSDouble   m_nPriceUnit;	 // 价格单位
	short	   m_nPriceDecimal;  // 价格小数位

	int		   m_nAnaPeriod;	 // 分析周期
	int        m_nSize;          // 日线数据个数
#ifdef SUPPORT_NETVALUE
	HSDouble	m_nNetVolueUint; // 净值及利率单位（目前均为1000)
#endif

public:
	// 风格操作
	BOOL	IsStyle(long dwStyle)		{ return (m_dwStyle & dwStyle); }
	void	AddStyle(long dwStyle)		{ m_dwStyle |= dwStyle;         }
	void	RemoveStyle(long dwStyle)	{ m_dwStyle &= (~dwStyle);      }
	void    SetStyle(long dwStyle)		{ m_dwStyle = dwStyle;          }
	
public:
	// 设置请求包信息
	void	SetAsk(AskData* pAsk, int nAnaPeriod = 1); //20080826 YJT 补充nAnaPeriod参数

	// 日线数据
protected:
	// 数据表(如开盘,收盘,最高....等等数据)
	CMapDataSource m_ayDayData;
	// 周期数
	int			   m_nDayCount;
	// 对日线结构数据内存区块的引用
	long*		   m_lRefDay;

	// 当前对象的所有者
	CObject*	   m_pDataOwnerObj;
	IDataSourceEx* m_pDataSource;

	/******************************************************************
	*	函数功能：	将nkey键值数据加入数据列表,并重新分配空间大小为lCurSize
	*	函数参数：	StockDay* pDay	: [in] 无用
	*				long lCurSize	: [in] 新的空间长度
	*				int nOldSize	: [in] 旧的空间长度
	*				WORD nkey		: [in] 要求处理数据项的键值
	*	返 回 值：	CDataSourceUseData* : 在数据列有中的空间
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	CDataSourceUseData* GetDay(StockDay* pDay,long lCurSize,int nOldSize,WORD nkey,StockDay* pLastDay = NULL);

	// 数据引用
protected:
	// 引用的数据源
	CHSDataSource* m_pRefDataSource;
public:
	// 设置引用的数据源
	void SetRefDataSource(CHSDataSource* pRefDataSource) { m_pRefDataSource = pRefDataSource; }
	/******************************************************************
	*	函数功能：	从文件中读入strCode股票数据,当strCode为空时,读入请求包m_pAsk中第一支股票的数据
	*	函数参数：	CString strCode = "" : [in] 股票代码
	*	返 回 值：	int  :	<=0 处理失败: 股票不存在(0), 引用的数据源或消息窗口不合法(-1), 请求包不合法(-2),
							1 处理成功
	*	调用依赖：	
	*	作    者：
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	int  ReadData(CString strCode = "");
	// 数据列表m_ayDayData是否为空
	BOOL IsEmpty();

	// 数据转换
public:	
	// 从数据列表中得到键值为nKey的数据数组
	HSDouble*			GetData(WORD nkey);
	// 得到数据的周期数据,其实就是GetData()结果中,数组的长度
	int					GetDaySize() { return m_nDayCount; }

	/******************************************************************
	*	函数功能：	设置日线数据到数据列表m_ayDayData中
	*	函数参数：	StockDay* pDay		: [in] 日线数据
	*				long lCurSize		: [in] 日线数据长度
	*				int nVolumeUnit		: [in] 成交量单位
	*				int nMoneyUnit		: [in] 成交额单位
	*				short nMoneyDecimal	: [in] 成交额小数位
	*				int nPriceUnit		: [in] 价格单位
	*				short nPriceDecimal	: [in] 价格小数位
	*				WORD nKey = 0		: [in] 为0时,表示设置所有的K线,否则,只设置一个对应nKey键值的数据
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	BOOL				SetDay(StockDay* pDay,long lCurSize,
							   int nVolumeUnit,
							   int nMoneyUnit,short nMoneyDecimal,
							   int nPriceUnit,short nPriceDecimal,
							   WORD nKey = 0,
							   StockDay* pLastDay = NULL);

	/******************************************************************
	*	函数功能：	功能与上述函数相同,只是当pDataOwnerObj为当前的m_pDataOwnerObj时,不重设数据, 目前此函数没有采用
	*	函数参数：	CObject* pDataOwnerObj : [in] 所有者
	*				StockDay* pDay		: [in] 日线数据	
	*				long lCurSize		: [in] 日线数据长度
	*				int nVolumeUnit		: [in] 成交量单位
	*				int nMoneyUnit		: [in] 成交额单位
	*				short nMoneyDecimal	: [in] 成交额小数位
	*				int nPriceUnit		: [in] 价格单位
	*				short nPriceDecimal	: [in] 价格小数位
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	BOOL				SetDay(CObject* pDataOwnerObj,StockDay* pDay,long lCurSize,
							   int nVolumeUnit,
							   int nMoneyUnit,short nMoneyDecimal,
							   int nPriceUnit,short nPriceDecimal);

	// 除权数据相关
protected:
	// 当前股票的除权数据
	CArray<ChuQuanData*,ChuQuanData*> m_ayChuQuanData;
public:
	// 设置除权数据(从文件中读出)
	void SetChuQuanData(CodeInfo* pCode,CArray<ChuQuanData*,ChuQuanData*>* pAyChuQuanData=NULL);  // 

	void SetPriceUnit(long lPrice = 1);  //设置价除数


	/******************************************************************
	*	函数功能：	除权数据计算
	*	函数参数：	ChuQuanData* pData	: [in] 除权数据
	*				long& lData			: [int/out] 要除权的数据
	*				int& nPriceUnit		: [in] 价格数据
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	void CalcChuQuan(ChuQuanData* pData,long& lData,int& nPriceUnit);

	/******************************************************************
	*	函数功能：	除权数据计算
	*	函数参数：	ChuQuanData* pData: [in] 除权数据
	*				double& dwData    : [in/out] 要除权的数据
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	void CalcChuQuan(ChuQuanData* pData,double& dwData);

	/******************************************************************
	*	函数功能：	对K线数据进行除权计算
	*	函数参数：	StockDay& pDay		: [in/out] K线数据
	*				DWORD dwStyle = 0	: [in] 不用
	*	返 回 值：	BOOL : 成功(TRUE),失败(FALSE)
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	BOOL CalcChuQuan(StockDay& pDay,DWORD dwStyle = 0);
	// 设置日线数据(如果不支持除权,则不设置)
	void CalcChuQuan(StockDay* pDay,long lCurSize,void* pData = NULL);
	// 得到除权数据数组指针
	CArray<ChuQuanData*,ChuQuanData*>* GetChuQuan() { return &m_ayChuQuanData; }

	// 财务数据相关
protected:
	// 财务数据管理对象
	CHSFinanceData  m_HSFinaceData;
	//CArray<CurrentFinanceData> 
public:
	// 返回财管理对象
	CHSFinanceData* GetFinaceData() { return &m_HSFinaceData; }
	// 得到某一项的财务数据
	HSDouble		GetFinaceDataByID(UINT nID);
	// 从文件中读入pCode股票的财务数据到内存m_HSFinaceData中
	void		    SetFinaceData(CodeInfo* pCode);
	// 得到当前股票的流通盘,被公式使用
	HSDouble        CAPITAL();
	// 根据财务数据项在财务数据数组中的位置得到财务数据项
	HSDouble        GetFinaceDataByPos(int nPos);

	// 时时数据相关
public:
	// 被公式使用

	/******************************************************************
	*	函数功能：	得到相应股票相应项的行情数据 
	*	函数参数：	int nPos			: [in] 行情数据的ID,比如3:昨收,4今天,5最高....
	*				char cType = 0		: [in] 目前不用
	*				char* pCode = NULL	: [in] 股票代码
	*	返 回 值：	HSDouble  : 返回相应股票相应项的行情数据 
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	HSDouble           GetNowDataByCode(int nPos,char cType = 0,char* pCode = NULL);

	// 选股相关
protected:
	// 选股结果
	CArray<SuccResult*,SuccResult*>* m_pSelStockSuccResult;
public:
	// 返回选股结果
	CArray<SuccResult*,SuccResult*>* SetSeleckStockData(CodeInfo* pCode = NULL);

	// 通讯，请求资讯数据
public:
	// 消息窗口
	static HWND m_wndMsg;
public:
	/******************************************************************
	*	函数功能：	给消息窗口发HX_USER_COMPILEDATA消息
	*	函数参数：	 void* pBuffer : [IN] 发送消息时的第二个参数
	*				WPARAM wParam  : [IN] 消息分类ID, 发送消息时的第一个参数
	*	返 回 值：	long : 消息处理结果
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	long SendDataToWnd( void* pBuffer, WPARAM wParam );

	// 批量财务数据操作
protected:
	AllFinancialData m_sAllFinancialData;
public:
	BOOL ReadFinancial(CString strCode,AllFinancialData* pAllFinancialData = NULL);

	HSDouble PROFFIN(CString strID,HSCurDataInfoStatus* pRefDataStatus,
		/*int nOffset,*/
		int Begin,int End,CString code,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	HSDouble PROFFINON(CString strID,
		int Begin,int End,
		HSCurDataInfoStatus* pRefDataStatus,
		int nOffset,
		CString code,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	HSDouble BlockCalc(CString strID,
		CString strBlockName,
		int Begin,int End,
		HSCurDataInfoStatus* pRefDataStatus,
		int nOffset,
		HSCurDataInfoStatus* pCurDataStatus, int& nError,
	    int nOperator = 0);
	
	int BlockCalc(CString strBlockName,
		int Begin,int End,
		AllFinancialData* pRetAllFinancialData,	
	    int nOperator = 0);
public:
	// 清除数据, 目前用于清除股票和板块的财务数据
	void EmptyData();
};

typedef CHSDataSource* lpHxDataSource;

#endif
