#pragma once

#include "quotestuctdefine.h"
#include "QuoteSort.h"
#include "DataSourceDefineEx.h"
#include "hsgridtable.h"
#include "skinscrollbar.h"
#include "..\..\..\HelperLib\GeneralHelper.h"
#include "QuoteLangMngInterFace.h"
#include "ConfigInterface.h"

#define INIT_FIXED_HEAD			0x00010000
#define INIT_FIXED_DATA			0x00020000 // 一般报价(1~6)
#define INIT_SORTRPT_DATA		0x00040000 // 排名(61~66)
#define INIT_FSDetail_DATA		0x00080000 // 分时明细表
#define INIT_PanHouDetail_DATA  0x00100000 // 盘后明细表
#define INIT_BigDetail_DATA		0x00200000 // 大盘明细表
#define INIT_FENJIA_DATA		0x00400000 // 分价表

#define DELAY_REQUEST_TIMESPAN		100		// 延时请求时间间隔
#define REFRESH_DISAPPEAR_TIMESPAN	100		// 刷新高亮单元格变淡一次时间
#define REFRESH_DISAPPEAR_TIMES		20		// 刷新高亮单元格消失时间（TIMES * REFRESH_DISAPPEAR_TIMESPAN）

//////////////////////////////////////////////////////////////////////////报价排序类
class CQuoteSort
{
public:
	CQuoteSort(void);
	~CQuoteSort(void);

	void Free();
	double GetData(int nRow,StockUserInfo* pStock = NULL);
	BOOL IsValid() { return (m_pData != NULL && m_nDataCount > 0 && m_bAscending != -1); }
	BOOL SetCWSortData();
	BOOL SetHQSortData();
	BOOL SetOtherSortData();

	HSSortData* Sort(int nBegin,int nEnd,
		CArray<StockUserInfo*,StockUserInfo*>* pStock,int nColumn,
		BOOL bAscending = TRUE);


	int									   m_nColumn;
	CArray<StockUserInfo*,StockUserInfo*>* m_pStock;

	HSSortData*		m_pData;
	int				m_nDataCount;

	CHSGridCtrl*	m_pGridCtrl;
	int				m_nFixedRow;

	BOOL			m_bAscending;

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////行情报价
// CQuoteReportTable

class CQuoteReportTable : public CHSGridTable
{
	DECLARE_DYNAMIC(CQuoteReportTable)

public:
	CQuoteReportTable(IDataSourceEx* pDataSouce, long nDataSourceID, int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
	virtual ~CQuoteReportTable();


	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////重载操作
	// 设定指定列宽 注意：nWidth为英文半角字符数,需要通过计算得到像素数
	virtual int SetColumnWidth(int nColID,int nWidth = -1);

	CString GetMenuName(int nType = 0, int nSubType = 0);
	CString GetMenuName(UINT nMenuID);

	virtual BOOL ShowRow(int row, BOOL bRefresh = FALSE);	// 显示指定行
	virtual BOOL ShowCol(int col, BOOL bRefresh = FALSE);	// 显示指定列

	virtual BOOL MoveRowTo(int nSrc,int nDes);		// 移动指定行到目标行
	virtual BOOL MoveColTo(int nSrc,int nDes);		// 移动指定列到目标列s

	virtual void OnFixedRowClick(CCellID& cell);	// 点击固定行（排序）
	virtual void SetSortColumn(int nCol);			// 设定排序列

	virtual void GetClientRect(LPRECT lpRect);

	virtual void OnResizeColWidth(int nCol);	// 响应调整列宽
	//////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////列（头）操作
	/********************************************************************************
	* 函数功能 : 根据列头组名称初始化列头
	* 函数参数 : CString strGroupName : 列头组名称
	*			 BOOL bRefreshData : 是否同时刷新数据
	*  返回值  : BOOL : 成功为TRUE,失败为FALSE
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-04
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL InitColGroup(CString strGroupName, BOOL bRefreshData = FALSE);

	/********************************************************************************
	* 函数功能 : 删除指定范围的列头
	* 函数参数 : int nBegin : 待删除列序号起始值
	*			 int nEnd : 待删除列序号终止值
	*  返回值  : int 当前数组中剩余列头数量
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-04
	* 修改记录 : 
	*******************************************************************************/
	virtual int DelColHead(int nBegin = -1,int nEnd = -1);

	/********************************************************************************
	* 函数功能 : 重设列ID与列序号之间的对应表
	* 函数参数 : 
	*  返回值  :
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-04
	* 修改记录 : 
	*******************************************************************************/
	void ResetCorrespondMap();

	/********************************************************************************
	* 函数功能 : 根据列ID获取列头序号
	* 函数参数 : UINT uiID : 列ID
	*  返回值  : int 列头序号
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual int GetColIndexByID(UINT uiID);

	/********************************************************************************
	* 函数功能 : 根据列头序号获取列ID
	* 函数参数 : int nPos : 列头序号
	*  返回值  : UINT 列ID
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual UINT GetColHeadID(int nPos = 0);

	/********************************************************************************
	* 函数功能 : 添加列
	* 函数参数 : UINT uiID : 列ID
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	virtual void AddColumn(UINT nID);


	/********************************************************************************
	* 函数功能 : 获取列头字体指针
	* 函数参数 : int nType : 样式（1.系统设置的列头字体 2.特殊处理的列头字体 3.系统设置的动态表格字体）
	*  返回值  : 
	* 调用依赖 : 计算列头宽度时调用、画列头时调用
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	virtual CFont* GetColHeadFontPtr(int nType);

	/********************************************************************************
	* 函数功能 : 将列加入列序号索引
	* 函数参数 : int nID : 列ID
	*			 int nColPos : 列序号
	*  返回值  : BOOL : TRUE 支持排序
	*					FALSE 不支持排序
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	void AddColToMap(int nID,int nColPos);

	void SaveCurDispColHead();

	static BOOL IsFixedColumn(int nID);
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////排序操作
	/********************************************************************************
	* 函数功能 : 按列ID排序
	* 函数参数 : UINT uiID : 列ID
	*			 BOOL bAscending : 排序方式（TRUE 正序，FALSE 倒序）
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	virtual void Sort(int nID, BOOL bAscending, BOOL bForce = FALSE);

	/********************************************************************************
	* 函数功能 : 是否支持排序
	* 函数参数 : int nCol : 列序号
	*			 WORD nColID : 列ID
	*  返回值  : BOOL : TRUE 支持排序
	*					FALSE 不支持排序
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL IsSupportSort(int nCol);
	virtual BOOL IsSupportSort(WORD nColID);

	/********************************************************************************
	* 函数功能 : 返回默认排序方式
	* 函数参数 : int nCol : 列序号
	*			 WORD nColID : 列ID
	*  返回值  : BOOL : TRUE 正序排序
	*					FALSE 倒序排序
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL GetDefaultSortType(int nCol);
	virtual BOOL GetDefaultSortType(WORD nColID);

	/********************************************************************************
	* 函数功能 : 根据页面信息获取排序属性
	* 函数参数 : IN const PageViewInfo* pvInfo : 页面属性
	*			 IN short nID : 未用
	*			 OUT int& nCount : 排序股票个数
	*			 OUT HSMarketDataType& cCodeType : 排序市场类型
	*  返回值  : BOOL : TRUE 正确获取
	*					FALSE 没有获取
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL GetSortInfoByPageInfo(IN const PageViewInfo* pvInfo, IN short nID, OUT int& nCount,OUT HSMarketDataType& cCodeType);

	/********************************************************************************
	* 函数功能 : 取消排序
	* 函数参数 : 
	*  返回值  : int : 0
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual int CancelSort();

	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////股票代码列表操作
	/********************************************************************************
	* 函数功能 : 获取指定位置股票代码
	* 函数参数 : int nPos : 位置索引序号
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	virtual StockUserInfo* GetStock( int nPos );
	/********************************************************************************
	* 函数功能 : 设置当前股票代码（设置前会清空）
	* 函数参数 : CArray<StockUserInfo*,StockUserInfo*>& payStock : 股票代码数组引用
	*			 StockUserInfo* pStock : 股票数组第一个成员指针
	*			 int nTotal : 股票个数
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	virtual void SetStock(CArray<StockUserInfo*,StockUserInfo*>& payStock);
	virtual void SetStock(StockUserInfo* pStock,int nTotal);
	
	/********************************************************************************
	* 函数功能 : 清空当前股票代码
	* 函数参数 : int nType : 清空方式
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	void EmptyStock(int nType = 0);

	/********************************************************************************
	* 函数功能 : 根据代码查找行
	* 函数参数 : CodeInfo* pCode : 股票代码信息
	*			 OUT int& nPos : 找到的行序号
	*  返回值  : StockUserInfo* : 股票信息
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	StockUserInfo* FindRowByCode(CodeInfo* pCode, OUT int& nPos);



	//////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////页面（属性）操作
	/********************************************************************************
	* 函数功能 : 改变当前页面
	* 函数参数 : PageViewInfo* pvInfo : 页面属性
	*			 BOOL bForceRefresh : 当页面属性相同时是否强制刷新页面所有信息
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	virtual int ChangeContent(PageViewInfo* pvInfo, StockUserInfo* pStock = NULL, int nTotal = 0,CString strColGroup = _T(""), BOOL bForceRefresh = FALSE, BOOL bRequestData = TRUE);

	/********************************************************************************
	* 函数功能 : 设定当前页面信息
	* 函数参数 : PageViewInfo* pvInfo : 页面属性
	*			 CString strColGroupName : 列头组名
	*			 BOOL bRequest : 是否同时请求数据
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	virtual void SetPageViewInfo(PageViewInfo* pvInfo, CString strColGroupName = _T(""), BOOL bRequest = FALSE);

	/////////////////////////////////////////////////////////////////////////////////////////////界面显示相关操作
	/********************************************************************************
	* 函数功能 : 获取刷新属性
	* 函数参数 : 
	*  返回值  : RefreshDispTextProp* : 刷新属性
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual RefreshDispTextProp* GetRefreshDispTextProp();

	/********************************************************************************
	* 函数功能 : 渐隐高亮刷新的单元格
	* 函数参数 : 
	*  返回值  : int : 渐隐完成的单元格数量
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	int DelutionHilightCells();

	/********************************************************************************
	* 函数功能 : 停止所有定时器
	* 函数参数 : 
	*  返回值  : 
	* 调用依赖 : 重新请求会导致整屏数据刷新的请求时调用
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	void KillAllTimer();

	//////////////////////////////////////////////////////////////////////////////////////////列表数据操作
	/********************************************************************************
	* 函数功能 : 根据当前信息重新初始化所有行数据
	* 函数参数 : 
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	virtual void InitRowData();

	/********************************************************************************
	* 函数功能 : 初始化固定单元格数据
	* 函数参数 : int nFirst : 起始行
	*			 int nCount : 行数量
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void InitFixedDataItem(int nFirst,int nCount);

	/********************************************************************************
	* 函数功能 : 设定指定固定单元格数据
	* 函数参数 : int nRow : 行
	*			 int nCol : 列
	*			 CGridCellBase* pCell : 单元格指针
	*			 StockUserInfo* pStock : 股票信息
	*			 int nNumber : 序号
	*			 BOOL bRefresh : 是否刷新
	*			 CDC* pDC : 设备DC
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void SetFixedData(int nRow,int nCol,CGridCellBase* pCell, StockUserInfo* pStock = NULL,
		int nNumber = -1, BOOL bRefresh = TRUE, CDC* pDC = NULL);

	/********************************************************************************
	* 函数功能 : 初始化行序号
	* 函数参数 : int nBeginRow : 起始行
	*			 int nEndRow : 结束行
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void InitNumber(int nBeginRow = -1, int nEndRow = -1);

	//////////////////////////////////////////////////////////////////////////////////////////滚动条操作
	/********************************************************************************
	* 函数功能 : 设定第三方水平滚动条，替代系统默认水平滚动条
	* 函数参数 : CSkinScrollBar* pNewHorScrollBar : 水平滚动条指针
	*  返回值  : BOOL : 是否成功
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	BOOL SetHorScrollBarRef(CSkinScrollBar* pNewHorScrollBar = NULL);
	/********************************************************************************
	* 函数功能 : 设定第三方垂直滚动条，替代系统默认垂直滚动条
	* 函数参数 : CSkinScrollBar* pNewVerScrollBar : 垂直滚动条指针
	*  返回值  : BOOL : 是否成功
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	BOOL SetVerScrollBarRef(CSkinScrollBar* pNewVerScrollBar = NULL);

	/********************************************************************************
	* 函数功能 : 重设滚动条
	* 函数参数 : 
	*  返回值  : 
	* 调用依赖 : 每次调整表格宽度高度、增加删除表格等可能影响到滚动条长度的操作后调用
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void ResetScrollBars();

	/********************************************************************************
	* 函数功能 : 获取指定滚动条的滚动位置
	* 函数参数 : int nBar : 滚动条样式（水平或垂直）
	*			 BOOL bGetTrackPos : 是否获取拖动时的位置
	*  返回值  : int : 当前滚动条位置
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);

	/********************************************************************************
	* 函数功能 : 设定第三方垂直滚动条，替代系统默认垂直滚动条
	* 函数参数 : CSkinScrollBar* pNewVerScrollBar : 垂直滚动条指针
	*  返回值  : BOOL : 是否成功
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////数据请求处理
	/********************************************************************************
	* 函数功能 : 请求数据
	* 函数参数 : PageViewInfo* pPageInfo : 页面属性
	*  返回值  : 
	* 调用依赖 : 暂时不使用，具体功能直接调用具体功能请求
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void RequestData( PageViewInfo* pPageInfo = NULL );
	
	/********************************************************************************
	* 函数功能 : 请求股票代码列表
	* 函数参数 : PageViewInfo* pPageInfo : 页面属性
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void RequestStockUserInfo(PageViewInfo* pPageInfo = NULL);
	
	/********************************************************************************
	* 函数功能 : 请求实时数据（默认参数表示请求当前页的数据）
	* 函数参数 : int nFirst : 起始代码序号
	*			 int nCount : 代码个数
	*			 BOOL bNeedAutoPush : 是否同时请求主推
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void RequestRealTimeData(int nFirst = -1, int nCount = -1, BOOL bNeedAutoPush = TRUE);
	
	/********************************************************************************
	* 函数功能 : 请求排序数据（默认参数表示请求当前页的排序数据）
	* 函数参数 : PageViewInfo* pPageInfo : 页面属性
	*			 BOOL bNeedAutoPush : 是否同时请求主推
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void RequestSortData(PageViewInfo* pPageInfo = NULL, BOOL bNeedAutoPush = TRUE);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////数据应答处理
	/********************************************************************************
	* 函数功能 : 实时数据处理
	* 函数参数 : DataHead* pHead : 应答包头结构
	*			 const CommRealTimeData_Ext* pnowData : 实时数据
	*			 int nSize : 数据个数
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void HandleNowData(DataHead* pHead,const CommRealTimeData_Ext* pnowData, int nSize);
	/********************************************************************************
	* 函数功能 : 主推数据处理
	* 函数参数 : DataHead* pHead : 应答包头结构
	*			 const CommRealTimeData_Ext* pnowData : 实时数据
	*			 int nSize : 数据个数
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void HandleAutoPushData(DataHead* pHead, const CommRealTimeData_Ext* pnowData, int nSize);
	/********************************************************************************
	* 函数功能 : 排序数据处理
	* 函数参数 : DataHead* pHead : 应答包头结构
	*			 AnsReportData* pReportData : 排序数据
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void HandleSortData(DataHead* pHead,const AnsReportData* pReportData);

	/********************************************************************************
	* 函数功能 : 数据更新入口
	* 函数参数 : int nPos : 更新行
	*			 StockUserInfo* pStock : 股票信息数据
	*			 long nUnit : 每手股数
	*			 short nDecimal : 小数位数
	*			 long  lClose : 昨收价
	*			 HSStockRealTime_Ext* pData : 实时数据
	*			 StockOtherData* pOtherData : 其他数据
	*			 RefreshDispTextProp* pRefreshDispTextProp : 更新属性
	*			 int nType : 更新样式
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	virtual void UpdateDataEntry(
		int nPos,
		StockUserInfo* pStock,
		long nUnit,
		short nDecimal,
		long  lClose,
		HSStockRealTime_Ext* pData,
		StockOtherData* pOtherData,
		RefreshDispTextProp* pRefreshDispTextProp = NULL,
		int nType = MAKELONG(0,LTOA_MASK_DEFAULT));

	// 更新股票列
	/********************************************************************************
	* 函数功能 : 数据更新入口
	* 函数参数 : int nPos : 更新行
	*			 StockUserInfo* pStock : 股票信息数据
	*			 long nUnit : 每手股数
	*			 short nDecimal : 小数位数
	*			 long  lClose : 昨收价
	*			 HSStockRealTime_Ext* pData : 实时数据
	*			 StockOtherData* pOtherData : 其他数据
	*			 RefreshDispTextProp* pRefreshDispTextProp : 更新属性
	*			 int nType : 更新样式
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
 	virtual void UpdateStockData(
 		int nPos,
 		StockUserInfo* pStock,
 		long nUnit,
 		short nDecimal,
 		long  lClose,
 		HSStockRealTime_Ext* pData,
 		StockOtherData* pOtherData,
 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
 		int nType = 0);
// 
// 	// 更新指数列
// 	virtual void UpdateIndexData(
// 		int nPos,
// 		StockUserInfo* pStock,
// 		long nUnit,
// 		short nDecimal,
// 		long  lClose,
// 		HSIndexRealTime* pData,
// 		StockOtherData* pOtherData,
// 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
// 		short nType = 0);
// 	// 更新指数列
// 	virtual void UpdateIndexData(
// 		int nPos,
// 		StockUserInfo* pStock,
// 		long nUnit,
// 		short nDecimal,
// 		long  lClose,
// 		HSStockRealTime_Ext* pData,
// 		StockOtherData* pOtherData,
// 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
// 		short nType = 0);
// 	
// 	// 更新期货列
// 	virtual void UpdateFuturesData(
// 		int nPos,
// 		StockUserInfo* pStock,
// 		long nUnit,
// 		short nDecimal,
// 		long  lClose,
// 		HSQHRealTime* pData,
// 		StockOtherData* pOtherData,
// 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
// 		short nType = 0);
// 
// 	// 更新外汇列
// 	virtual void UpdateForeignData(
// 		int nPos,
// 		StockUserInfo* pStock,
// 		long nUnit,
// 		short nDecimal,
// 		long  lClose,
// 		HSStockRealTime* pForeignData,
// 		StockOtherData* pOtherData,
// 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
// 		short nType = 0);
// 
// 	// 更新港股列
// 	virtual void UpdateHKData(
// 		int nPos,
// 		StockUserInfo* pStock,
// 		long nUnit,
// 		short nDecimal,
// 		long  lClose,
// 		HSStockRealTime* pHKData,
// 		StockOtherData* pOtherData,
// 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
// 		short nType = 0);
protected:
	PageViewInfo	m_pviInfo;					// 当前页面信息
	IDataSourceEx*	m_pDataSource;				// 数据引擎接口指针
	long			m_nDataSourceID;			// 数据请求ID

	IHsColumnProperty*		m_iColProp;			// 列属性配置接口
	IHsColor*               m_iColorProt;       // 颜色配置接口
	IHsFont*                m_iFontProt;        // 字体配置接口
	IHsUserDataCfg*         m_iUserDefCfg;      // 用户数据接口（股票标记）
	IBlockManager*			m_iBlockManager;	// 板块管理
	IQuoteLang*				m_iQuoteLang;		// 行情资源接口

	BOOL			m_bNeedRequestSort;			// 是否需要请求排序数据（某些列不需要请求，如代码和名称等）
	CString			m_strCurColGroup;			// 当前列头组名称
	CString			m_strPreColGroup;			// 前一个列头组名称

	int				m_nCurActiveRow;			// 当前激活行

	CSkinScrollBar* m_pHorScrollBar;			// 第三方水平滚动条
	CSkinScrollBar* m_pVerScrollBar;			// 第三方垂直滚动条

	CUIntArray		m_ayColInfo;	// 所有列ID
	CArray<StockUserInfo*,StockUserInfo*>	m_pStock;		// 当前显示的所有股票代码信息
	
	CMap<int,int,int,int>					m_mapColumn;	//列ID和列序号对应表
	BOOL			m_bResizeColWidth;			// 手动调整过列宽
public:
	int				m_nRefreshTimerID;			// 刷新定时器ID
	int				m_nDelayRealTimeRequestTimerID;		// 延时实时数据请求定时器ID	
	int				m_nDelaySortRequestTimerID;			// 延时排序请求定时器ID
	CMap<int,int,int,int>					m_mapRefreshCells;	// 正在刷新的单元格列表
	CMap<UINT,UINT,CString,CString>						m_mapTempColGroup;	// （临时）列头组名称菜单对应项
	DECLARE_MESSAGE_MAP()
public:
	/********************************************************************************
	* 函数功能 : 接收数据应答
	* 函数参数 : void* pData : 应答包数据
	*			 int nLen : 应答包长度
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : 李晓奇
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	BOOL OnRecvData(void* pData, int nLen);

	void OnColGroupChange(UINT nID);			// 切换显示列头组
	void OnColumnSetting();				// 设定显示列头属性
	void OnMarkCodeByString();			// 字符串标记代码
	void OnMarkCodeByIcon(UINT nID);	// 图标标记代码
	void OnCancelMarkCode();			// 取消代码标记
	void OnAddToBlock();				// 添加到板块
	void OnAddToSelfBlock();			// 添加到自选股板块
	void OnBlockAdscription();			// 所属板块

	virtual void OnDraw(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	static VOID CALLBACK TimerProc( HWND hwnd,	UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
	static VOID CALLBACK DelayRealTimeRequestTimerProc(HWND hwnd,	UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
	static VOID CALLBACK DelaySortRequestTimerProc(HWND hwnd,	UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
};

