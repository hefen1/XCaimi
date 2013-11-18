/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	FormulaCalc.h
*	文件标识：	
*	摘	  要：	选股,排序,预警(旧),报表分析(区间分析,热门板块,强度分析,
				指标排序)等的计算控制
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明:	
***************************************************************/

#if !defined(AFX_HSCALCULATE_H__5909144D_1313_477B_B0C9_55B772666225__INCLUDED_)
#define AFX_HSCALCULATE_H__5909144D_1313_477B_B0C9_55B772666225__INCLUDED_

#include "PubStruct.h"
#include "FormulaManPublic.h"
#include "DataSourceDefineEx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PERIOD_TYPE_DAY_ANY		0x00A0		//日线任意周期
#define MINUTE_ANY				0x00B0		//分钟任意周期
#define HS_USR_DEF_BEGIN 1
// 结构AskDataEx和结构AskData前面部分必须相同
struct AskDataEx
{
	unsigned short 		m_nType;	     // 请求类型
	char				m_nIndex;     	 // 请求索引，与请求数据包一致
	char				m_cOperator;   	 // 操作(目前不使用)
	long				m_lKey;		 	 // 一级标识，通常为窗口句柄
	HSPrivateKey	    m_nPrivateKey;	 // 二级标识

	short   			m_nSize;         // 请求证券总数，小于零时，
	short 				m_nAlignment;    // 为了4字节对齐而添加的字段

	ReqDayData          m_pCode[1];

	AskDataEx()
	{
		memset(this,0,sizeof(AskDataEx));
	}
};


#define  SUPPORT_OLD
//#include "..\comm\sscomm3.h"

struct SelStockCondition;
struct InportDataParam;
class CSystemFun;
class CMapVariabile;
class CHSDataSource;

/*************************************************************
*	类 名 称：	CFormulaCalc
*	类 描 述：	选股,排序,预警(旧),报表分析(区间分析,热门板块,强度分析,
				指标排序)等的计算控制类
***************************************************************/
class BCGCONTROLBARDLLEXPORT CFormulaCalc  
{
public:
	CFormulaCalc();
	virtual ~CFormulaCalc();

public:
	// 自动删除数据(CFormulaCalc类数据)
	struct _Auto_Delete_Data
	{
		~_Auto_Delete_Data();
	};
	friend _Auto_Delete_Data;

	// 自定义表格列时使用的中间变量参数(不包含计算相关的信息)
	struct FormulaVarColData
	{
		int      m_nID;		// 列ID
		CString  m_strName; // 列名
		FormulaVarColData(int nID,CString strName)
		{
			m_nID = nID;
			m_strName = strName;
		}
	};

	// 指标排序返回（多个）
	struct FormulaIndicatorSortResult
	{
		// 当前股票
		void*	m_pStock;
		// 自定义列信息
		CArray<FormulaVarColData*,FormulaVarColData*>	m_ayColName;

		// 计算返回一行的数据--数字(KEY值为自定义列的ID)
		CMap<int,int,HSDouble*,HSDouble*>	m_ayValue;
		// 计算返回一行的数据--字符(KEY值为自定义列的ID)
		CMap<int, int, CString*, CString*>	m_ayValueEx;

		FormulaIndicatorSortResult()
		{
			m_pStock = NULL;
		}
		~FormulaIndicatorSortResult()
		{
			Free();
		}

		// 得到第nCol列的列信息
		CFormulaCalc::FormulaVarColData* GetColName(int nCol);
		// 加一个已nCol为列ID, 以strName为名的列信息
		int AddColName(int nCol,CString strName);
		// 清空私有空间(包含列信息)
		void	  Free();
		// 清空数据
		void	  EmptyData();
		// 得到列ID为nCol的列的数据,如果表中不有,则返回NULL
		HSDouble* GetData(int nCol);
		CString*  GetDataEx(int nCol);
		// 设置列ID为nCol的列的数据
		void	  SetData(int nCol,HSDouble* pRefData);
		void	  SetData(int nCol, CString& strData);
	};

	// 基本自定义列信息(包含计算相关的信息)
	struct FormulaBaseOtherColData
	{
		int		m_nID;					// 列id
		CString m_strName;				// 列名称
		FormulaGeneralParam*	m_pParam;	// 通用参数

		COMPILEFUNCTION   m_pFun;		//获取数据的函数指针 COMPILEFUNCTION

		FormulaBaseOtherColData()
		{
			m_nID = -1;
			m_pFun = NULL;
			m_pParam = NULL;
		}

		~FormulaBaseOtherColData()
		{
			if( m_pParam != NULL )
				delete m_pParam;
			m_pParam = NULL;
		}

		FormulaBaseOtherColData& operator=(FormulaBaseOtherColData& sData)
		{
			m_nID = sData.m_nID;
			m_strName.Format("%s", sData.m_strName);
			m_pFun = sData.m_pFun;
			if( m_pParam != NULL )
			{
				delete m_pParam;
				m_pParam = NULL;
			}
			if( sData.m_pParam == NULL )
			{
				return *this;
			}
			m_pParam = new FormulaGeneralParam;
			for( int i = 0; i < sData.m_pParam->GetSize(); i++ )
			{
				LPNODELEMENT pItem = sData.m_pParam->GetParam(i);
				if( pItem == NULL )
				{
					continue;
				}
				m_pParam->Add(pItem);
			}
			return *this;
		}
		/******************************************************************
		*	函数功能：	得到当前列的数据,并将数据放入到pBaseDataEx表中
						除了热门板块分析外的所有报表分析函数时,采用此函数
		*	函数参数：	CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx : [out] 存放数据
		*				int nPos : [in] 要求返回的数据的offset(如为0则返回当前所有数据中的第一个数据)
		*	返 回 值：	int : 1 成功, -1 参数不正确, 其它 计算错误产生的错误号
		*	调用依赖：	
		*	作    者：	
		*	完成日期：	2003年4月22日
		*	修改说明：	
		*****************************************************************/
		int GetData(CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx,int nPos);

		/******************************************************************
		*	函数功能：	得到当前列的数据,并将数据放入到pBaseDataEx表中	
						热门板块分析时,采用此函数
		*	函数参数：	CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx : [out] 存放数据
		*				void* ayNowData : [in] 板块计算过程中要用到的行情数据
		*	返 回 值：	int : 1 成功, -1 参数不正确, 其它 计算错误产生的错误号 
		*	调用依赖：	
		*	作    者：	
		*	完成日期：	2003年4月22日
		*	修改说明：	
		*****************************************************************/
		int GetData(CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx, void* ayNowData);
	};

	// 所有自定义列的信息(包含计算相关的信息)
	struct FormulaAddColParam
	{
		CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>* payOther; // 基本自定义列
		CArray<SingleSelCondition*,SingleSelCondition*>*			   pExArray; // 指标自定义列
	};


	/******************************************************************
	*	函数功能：	将ayOther列信息放入到pBaseDataEx表中,并重新分配空自由的ID给所有自定义列
					并将分配好ID的列信息发送给目标窗口,进行列表表头的初始化动作
	*	函数参数：	CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx : [out] 整理出的列信息(每列包含唯一ID)
	*				CArray<FormulaBaseOtherColData* 
	*				FormulaBaseOtherColData*>& ayOther : [in] 初始的列信息,ID没有分配好,但包含了所有计算相关的信息
	*	返 回 值：	int : 0 参数不合法, 1 处理成功
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	
	*****************************************************************/
	static int AddBaseOtherColData( CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx,
						       	    CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>& ayOther );

	/******************************************************************
	*	函数功能：	将ayOther列信息放入到pBaseDataEx表中,但不重新分配ID
					并将分配好ID的列信息发送给目标窗口,进行列表表头的初始化动作
	*	函数参数：	CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx : [out] 整理出的列信息(每列其实也包含唯一ID)
	*				CArray<FormulaBaseOtherColData*
	*				FormulaBaseOtherColData*>& ayOther : [in] 初始的列信息, 包含了所有计算相关的信息
	*	返 回 值：	int : 0 参数不合法, 1 处理成功
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月22日
	*	修改说明：	
	*****************************************************************/
	static int AddBaseOtherColDataEx( CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx,
						       	    CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>& ayOther );


	

	/*************************************************************
	*	类 名 称：	CalcRetData
	*	类 描 述：	选股相关中间变量数据结构
	***************************************************************/
	struct CalcRetData
	{
		SingleSelCondition*  pUnionData;		// 单个选股条件

		CLineProperty* pCalDataLeft;	// 左边的计算结果
		CLineProperty* pCalDataRight;	// 右边的计算结果
		CLineProperty* pCalDataMidlle;	// 中间的计算结果

		CExpValue* m_pLeft;				// 左边的公式
		CExpValue* m_pMiddle;			// 中间的公式
		CExpValue* m_pRight;			// 右边的公式

		// 是否已经初始化公式(如果为TRUE则表示m_pLeft等已找到)
		BOOL       m_bInitLeftRight;	
		// 指标排序
		FormulaIndicatorSortResult* m_pHSTechSortResult; 
		// 计算结果存放
		CArray<CLineProperty*,CLineProperty*> m_ayData;	

		// 请求包数据
		AskDataEx ask;	
		// 成交量单位
		int     nChengJiaoDiv;	
		// 分析数据周期
		int		nAnaPeriod;				
		// 计算结果返回，一般0是计算失败，大于0值表示计算成
		int		nResult;	
		// 选股结果
		CArray<SuccResult*,SuccResult*>		  m_aySucc;			
		// 成功股票 本数据基本以不使用
		CArray<StockUserInfo*,StockUserInfo*> m_aySuccStock;


		// 根据ask设置nChengJiaoDiv及nAnaPeriod, 参数nPeriod无用
		void    SetAsk(int nPeriod = -1);
		void    SetCurCode(CodeInfo* pCode);
		// 得到计算结果个数据(一般相当于行数)
		int				   GetSize() { return m_ayData.GetSize(); } 
		// 删除数据m_ayData
		void			   DeleteData();  
		// 设置m_ayData大小,注意,只有nCount<OldnCount(原来大小)时,才能处理成功
		void			   SetDataSize(int nCount);
		// 得到第nPosistion个计算结果
		CLineProperty*     GetDataAt(int nPosistion);
		// 将每个现有的计算结果的长度设置为nNumber
		void			   SetValueNumber(int nNumber);
		// 加一个选股成功数据
		void AddSucc(long lDate,long nPos) { m_aySucc.Add(new SuccResult(lDate,nPos) ); }
		void AddSucc(SuccResult* pSucc)    { m_aySucc.Add(pSucc); }
		// 删除所有成功数据
		void RemoveAllSucc()
		{
			for(int i = 0; i < m_aySucc.GetSize(); i++)
			{
				delete m_aySucc.GetAt(i);
			}
			m_aySucc.RemoveAll();
		}
		
		CalcRetData()
		{
			pUnionData = NULL;

			pCalDataLeft   = NULL;
			pCalDataRight  = NULL;
			pCalDataMidlle = NULL;

			m_pLeft = NULL;
			m_pMiddle = NULL;
			m_pRight = NULL;
			m_bInitLeftRight = FALSE;

			//
			nChengJiaoDiv = 0; // 成交量单位
			nAnaPeriod = 0;
			memset(&ask,0,sizeof(ask));

			m_pHSTechSortResult = NULL;

			//
			nResult = 0;
		}

		~CalcRetData()
		{
			if( m_pHSTechSortResult )
			{
				delete m_pHSTechSortResult;
				m_pHSTechSortResult = NULL;
			}

			DeleteData();
			RemoveAllSucc();
		}
	};

	// 选股计算中间结构
	struct UnionCalc
	{
		// 计算周期数
		long	m_lDateCount;
		// 计算结束日期
		long	m_lEndDate;
		// 多条件之间是交集（TRUE）还是并集（FALSE）的关系
		BOOL	bAndOr;
		// 个股单条件选股计算数据及结果数组
		CArray<CalcRetData*,CalcRetData*> subArray;
		// 单股票选股最终结果（K线选中点）
		CArray<SuccResult*,SuccResult*>   m_aySucc;

		// 向m_aySucc加一个成功
		void AddSucc(long lDate,long nPos) { m_aySucc.Add(new SuccResult(lDate,nPos) ); }
		void AddSucc(SuccResult* pSucc)    { m_aySucc.Add(pSucc); }
		// 删除m_aySucc所有成功点数据
		void RemoveAllSucc()
		{
			for(int i = 0; i < m_aySucc.GetSize(); i++)
			{
				delete m_aySucc.GetAt(i);
			}
			m_aySucc.RemoveAll();
		}

		UnionCalc()
		{
			bAndOr = 0;
		}

		~UnionCalc()
		{
			RemoveAllSucc();
		}
	};

public:
	/******************************************************************
	*	函数功能：	由公式名称得到公式内部绘图表达式表
	*	函数参数：	CString strName : [in] 公式名称 
	*				int nMask = Tech : [in] 公式类型
	*	返 回 值：	CMapVariabile* : 返回的公式内部绘图表
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月14日
	*	修改说明：	
	*****************************************************************/
	static CMapVariabile*  GetExpDraw(CString strName,int nMask = Tech);

	/******************************************************************
	*	函数功能：	由公式名称得到表达式(公式)
	*	函数参数：	CString strName : [in] 公式名称
	*				int nMask = Tech : [in] 公式类型
	*	返 回 值：	CExpression* : 公式指针
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月14日
	*	修改说明：	
	*****************************************************************/
	static CExpression*    GetExpress(CString strName,int nMask = Tech);

	/******************************************************************
	*	函数功能：	初始化retData数据,并返回指标线strDrawName
	*	函数参数：	CFormulaCalc::CalcRetData& retData : [in/out] 计算数据
	*				CString strDrawName : [in] 指标线名称
	*				CString strName : [in] 公式名称
	*				int nMask		: [in] 公式类型
	*				BOOL bTechLine  : [in] 设置公式数据源
											0x01 设置为CFormulaCalc::m_pHxDataSource
											0x02 设置为公式默认数据源
	*				CFormulaCalc::FormulaIndicatorSortResult* pHSTechSortResult = NULL : [in] 目前没用
	*	返 回 值：	CExpValue* : 得到的名称为strDrawName的指标线公式包含指针
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月14日
	*	修改说明：	
	*****************************************************************/
	static CExpValue* GetDrawLine(CFormulaCalc::CalcRetData& retData,
		CString strDrawName,CString strName,
		int nMask /*= Tech*/,BOOL bTechLine = FALSE,
		CFormulaCalc::FormulaIndicatorSortResult* pHSTechSortResult = NULL);

	// 清空m_UnionArray及m_ayFreeList数据
	static void DeleteAll();
	// 清空预警计算相关数据
	static void AlarmDeleteAll();
	// 创建预警相关数据
	static BOOL AlarmCreateAll();

	/******************************************************************
	*	函数功能：	选股计算(通过函数指针调用其它函获得)
	*	函数参数：	SingleSelCondition*  pUnionData : [in/out] 计算数据
	*				CalcRetData& retData : [out] 返回数据
	*				int nBegin : [in] 开始点
	*				int nEnd   : [in] 结束点
	*	返 回 值：	int : 0失败， 其它则成功
	*	调用依赖：	UnionCal,Condition,MoreKLine,Exchange,DaYu,XiaoYu,DengYu,JieYu,ShangChuan,XiaoPo,WeiChi,BiaoChi
	*	作    者：	
	*	完成日期：	2003年5月14日
	*	修改说明：	
	*****************************************************************/
	static int SingleCal( SingleSelCondition*  pUnionData,CalcRetData& retData,int nBegin,int nEnd);

	static int UnionCal( SelStockCondition* pUnionSubData,CalcRetData& retData,int nBegin,int nEnd);

	/******************************************************************
	*	函数功能：	计算所有指标线，并将计算结果放入retData中
	*	函数参数：	CString& strName  : [in] 公式名
	*				int& nMask		  : [in] 公式类型
	*				CString& strLine1 : [in] 指标线1名称
	*				CString& strLine2 : [in] 指标线2名称
	*				CString& strLine3 : [in] 指标线3名称
	*				CFormulaCalc::CalcRetData& retData : [in/out] 计算用数据及计算结果返回
	*				int& nBegin : [in] 计算开始点
	*				int& nEnd   : [in] 计算结束点
	*				FormulaIndicatorSortResult* pHSTechSortResult = NULL : [in] 无意义
	*	返 回 值：	int : 失败（-1）， 否则成功
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月14日
	*	修改说明：	
	*****************************************************************/
	static int CalcAll( CString& strName,int& nMask,
						CString& strLine1,CString& strLine2,CString& strLine3,
						CFormulaCalc::CalcRetData& retData,
						int& nBegin,int& nEnd,
						FormulaIndicatorSortResult* pHSTechSortResult = NULL);


	/******************************************************************
	*	函数功能：	条件选股计算
	*	函数参数：	SingleSelCondition*  pUnionData : [in] 计算用数据
	*				CalcRetData& retData	  : [in/out] 计算用数据同时保存计算结果(选股成功则retData.m_aySucc.GetSize()>0)
	*				int nBegin	: [in] 计算开始点
	*				int nEnd	: [in] 计算结束点
	*	返 回 值：	int : 0失败， 其它则成功
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月14日
	*	修改说明：	
	*****************************************************************/
	static int Condition(SingleSelCondition*  pUnionData,
							 CalcRetData& retData,
							 int nBegin,int nEnd);
	//	交易系统选股计算
	static int	Exchange(SingleSelCondition*  pUnionData,
							 CalcRetData& retData,
							 int nBegin,int nEnd);	
	//	五彩K线 选股计算
	static int MoreKLine(SingleSelCondition*  pUnionData,
							 CalcRetData& retData,
							 int nBegin,int nEnd);	

	//  大于选股计算
	static int DaYu(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	小于选股计算
	static int XiaoYu(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	等于选股计算
	static int DengYu(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	介于选股计算
	static int JieYu(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	上穿选股计算
	static int ShangChuan(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	下破选股计算
	static int XiaoPo(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	维持选股计算
	static int WeiChi(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	保持选股计算 
	static int BiaoChi(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);	

	static BOOL IsStoping() { return (m_nStopThread==0); }

public:
	// 线程状态 
	// -2 正在运行
	//  0 结束运行
	//  2 要求停止运行
	static int		m_nStopThread;
	// 线程句柄(没什么用 ??? )
	static HANDLE	m_hThread;

	// 预警计算线程状态 
	// -2 正在运行
	//  0 结束运行
	//  2 要求停止运行
	static int		m_nStopAlarmThread;
	// 线程句柄(没什么用 ??? )
	static HANDLE	m_hAlarmThread;

	// 找不到有什么用处，优化时建议删除( ??? )
	static CArray<CExpValue*,CExpValue*> m_ayFreeList;
	// 选股计算数据（条件及相应条件的计算返回）
	static CArray<UnionCalc*,UnionCalc*> m_UnionArray;
	// 预警计算数据（条件及相应条件的计算返回）
	static CArray<UnionCalc*,UnionCalc*> m_AlarmUnionArray;


	// 初始化读盘m_pHSDisk及数据源对象m_pHxDataSource
	static BOOL InitObj();
	// 初始化预警读盘m_pAlarmDisk及预警数据源对象m_pAlarmHxDataSource
	static BOOL InitAlarmObj();

public:
	// 数据源接口
	static InportDataParam* m_pHxDataSource;
	// 预警数据源接口 
	static InportDataParam* m_pAlarmHxDataSource;

	// 计算线程
public:
	// 开始计算线程
	static int  Start();
	// 停止计算线程
	static void Stop();
	// 以没用，优化时删除 ??? 
	static int  StartExchSys();

	// 创建线程
	static BOOL HSCreateThread(LPTHREAD_START_ROUTINE fnThreadFun);

	// 开始预警计算线程
	static int	StartAlarm();
	// 停止预警计算线程
	static void StopAlarm();

	/******************************************************************
	*	函数功能：	选股计算（除智能选股外)
	*	函数参数：	LPVOID pParameter : [in] 无意义
	*	返 回 值：	DWORD : 恒为0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	static DWORD WINAPI Calculate(LPVOID pParameter);
	// 指标排序计算
	static DWORD WINAPI TechSortCalculate(LPVOID pParameter);
	// 板块分析计算
	static DWORD WINAPI HSBlockCalculate(LPVOID pParameter);
	// 热门板块报表计算
	static DWORD WINAPI HSBlockReportCalulate(LPVOID pParameter);
	// 智能选股计算 目前不用
	static DWORD WINAPI HSCalculate(LPVOID pParameter);
	// 不用，优优时删除 ???
	static DWORD WINAPI HSExchCalc(LPVOID pParameter);
	// 不用，优优时删除 ???
	static DWORD WINAPI HSPhaseSortCalculate(LPVOID pParameter);
	// 高级版预警计算
	static DWORD WINAPI AlarmCalculate(LPVOID pParameter);
	//获取数据引擎接口指针
	static IDataSourceEx* GetDataSourcePtr();
	

	// 外部数据
	protected:
	static	IDataSourceEx* m_pDataSource;       //数据引擎接口
public:
	
	static CMapVariabile*  m_pExternExpression;      // 外部表达式表
	static CMapVariabile*  m_pExternVariabile;		 // 外部变量表
	static CMapVariabile*  m_pExternFunction;		 // 外部函数表
	static CSystemFun*	   m_pSystemFun;			 // 函数指针

	static CWnd* m_pMsgWnd;							 // 消息窗口
	static CWnd* m_pAlarmWnd;						 // 预警消息窗口

	static SelStockCondition* m_curSelStockCondition; // 先股条件
	static TExchSys*			 m_pExchSys;			 // 没用，去除 ??? 
	static CArray<AlarmCalcData*, AlarmCalcData*>* m_pAlarmCondition; // 预警计算结果存放


public:
	/******************************************************************
	*	函数功能：	寻找日线数据pDay中lDate所在的位移
	*	函数参数：	long lDate : [in] 日期数据 如20020901
	*				long lLen  : [in] 无用
	*				StockDay* pDay : [in] 日线数据 
	*				int lSize : [in] pDay日线数据个数
	*	返 回 值：	int : -1 找不到， 大于0 lDate在pDay中的位置
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月14日
	*	修改说明：	
	*****************************************************************/
	static int FindDate(unsigned long lDate,long lLen,StockDay* pDay,int lSize);//--2010.11.12 litao修正2012时间问题
	static int FindDateExact(unsigned long lDate,long lLen,StockDay* pDay, int lSize, BOOL bIsBegin = TRUE);//--2010.11.12 litao修正2012时间问题

	/******************************************************************
	*	函数功能：	向配置文件IntelligentSelect.ini中写入数据
	*	函数参数：	CString strSection : [in] 段名
	*				CString strKeyName : [in] Key名
	*	返 回 值：	BOOL : 成功(TRUE), 失败（FALSE）
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月14日
	*	修改说明：	
	*****************************************************************/
	static BOOL SaveToIni(CString strSection ,CString strKeyName);

public:
	/******************************************************************
	*	函数功能：	删除所有成功数据m_mapSelectSuccInfo
	*	函数参数：	BOOL bSelf = TRUE : [in] 是不时删除本身
	*	返 回 值：	int : 恒为0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月14日
	*	修改说明：	
	*****************************************************************/
	static int  DelAllSuccData(BOOL bSelf = TRUE);

	/******************************************************************
	*	函数功能：	加选股成功数据到表中
	*	函数参数：	CodeInfo* pCode : [in] 选出的股票代码信息
	*				CArray<SuccResult* 
	*				SuccResult*>* pArray : [in] 相对于K线的位置数据
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月14日
	*	修改说明：	
	*****************************************************************/
	static void AddSuccDataToMap(CodeInfo* pCode,CArray<SuccResult*,SuccResult*>* pArray);
//	static CArray<SuccResult*,SuccResult*>* AddSuccDataToMap(CodeInfo* pCode);


	//add by lxqi 20090310	策略交易计算//////////////////////////////////////////////////////
	/******************************************************************
	*	函数功能：	策略交易计算，判断买卖方向
	*	函数参数：	CLineProperty*& pRefData : [in] 监控相关数据（包括原始数据、监控结果和计算公式）
	*				HSCurDataInfoStatus* pCurDataStatus : [out] 存放计算结果，若为空则不存放
	*	返 回 值：	int		: >0时为买入信号，<0时为卖出信号，=0 为不发送买卖信息
	*	调用依赖：	
	*	作    者：	李晓奇
	*	完成日期：	2009年3月10日
	*	修改说明：	
	*****************************************************************/
	static int CalcStrategyTrade(CLineProperty*& pRefData,HSCurDataInfoStatus* pCurDataStatus = NULL,int nSize = 0);
	//end by lxqi 20090310

};

typedef int ( *fnHSCompareCondition)( SingleSelCondition*  pUnionData,
									   CFormulaCalc::CalcRetData& retData,
									   int nBegin,int nEnd );

#endif // !defined(AFX_HSCALCULATE_H__5909144D_1313_477B_B0C9_55B772666225__INCLUDED_)
