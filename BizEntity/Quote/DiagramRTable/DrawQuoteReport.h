/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDrawQuoteReport.h
*	文件标识：
*	摘	  要：  咨询图元
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-3-17
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "DrawTableBase.h"
#include "ConfigInterface.h"
#include "DrawQuoteReportStruct.h"
#include "DrawQuoteTabCtrl.h"
#include "QuoteLangMngInterFace.h"

#define QUOTEREPORT_ROW_HEIGHT       22  // 行高
#define DELAY_REQUEST_TIMESPAN      100  // 请求延迟
#define REFRESH_DISAPPEAR_TIMESPAN	200	 // 刷新高亮单元格变淡一次时间
#define QUOTEREPORT_REFRESH_TIMES     5  // 刷新次数

#define QUOTEREPORT_SCROLL_VERT_SIZE   20  // 垂直滚动条
#define QUOTEREPORT_SCROLL_HORT_WIDTH 400  // 水平滚动条宽度
#define QUOTEREPORT_TAB_HEIGHT         20  // 标签页高度 

class CDrawQuoteReport :
	public CDrawTableBase,
	public IDrawQuoteTabNotify
{
public:
	CDrawQuoteReport(void);
	~CDrawQuoteReport(void);

	BOOL CreateSelf(); // 构造行情报价表格
    long GetMarketNameByType(const PageInfo* pvInfo,CStringArray& strMarketNames);
	BOOL SetPageInfo(PageInfo* pInfo,CString strColGroupName = _T("")); // 主要用于改变当前页股票代码
	BOOL SetPageViewInfo(PageInfo* pInfo, CString strColGroupName = "", BOOL bRequest = FALSE);    // 设置当前页面属性
	
	BOOL InitColGroup(CString strGroupName, BOOL bRefreshData = FALSE ); // 初始化列
	void InitRowData(); // 初始化行
	void InitFixedDataItem(int nFirst,int nCount);      // 初始化固定项
	void SetFixedData(int nRow, StockUserInfo* pStock); // 设置某一行的固定数据

	void RequestStockUserInfo(PageInfo* pPageInfo = NULL); // 请求股票代码

	BOOL AddColumn(UINT nID, UINT nWidth = 0); // 增加一行
	static BOOL IsFixedColumn(UINT nID);   // 通过列ID判断 是否为固定列
	int  GetColIndexByID(UINT nID);        // 通过列ID获取列位置(未移动)
	UINT GetColIDByIndex(int nCol);        // 通过(未移动)列位置获取列ID

	// 改变当前页面
	int ChangeContent(PageInfo* pvInfo, CArray<StockUserInfo>& ayStock, CString strColGroup = "", BOOL bForceRefresh = FALSE, BOOL bRequestData = TRUE);
	int ChangeContent(PageInfo* pvInfo, StockUserInfo* pStock = NULL, int nTotal = 0, CString strColGroup = "", BOOL bForceRefresh = FALSE, BOOL bRequestData = TRUE);

	void EmptyStock(); // 清空代码队列
	// 设置股票代码队列
	void SetStock(CArray<StockUserInfo>& payStock);
	void SetStock(StockUserInfo* pStock,int nTotal);
	StockUserInfo* GetStock(int nPos); // 获取队列中股票代码
	StockUserInfo* FindRowByCode(CodeInfo* pCode, int& nPos); // 根据代码查找行

	void OnColGroupChange(UINT nID);    // 切换显示列头组
	void OnColumnSetting();				// 设定显示列头属性

	void UpdateRefreshMap();            // 更新刷新map            
	//////////////////////////////////////////////////////////////////////////
	// 发送请求包相关
	void KillAllTimer();  // 清空所有定时器
	void RequestRealTimeData(int nStart, int nTotal); // 请求实时数据以及同时订阅主推信息
	void RequestSortData(PageInfo* pPageInfo = NULL, BOOL bNeedAutoPush = TRUE); // 请求排序数据
	static VOID CALLBACK RefreshProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);                    // 高亮显示
	static VOID CALLBACK DelayRealTimeRequestTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime ); // 实时数据请求
	static VOID CALLBACK DelaySortRequestTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);      // 排序数据请求
	
	//////////////////////////////////////////////////////////////////////////
	// 接受请求包相关
	void HandleNowData(DataHead* pHead, const CommRealTimeData_Ext* pnowData, int nSize); // 实时数据处理
	void HandleNowData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize);     // 实时数据处理
	void HandleAutoPushData(DataHead* pHead, const CommRealTimeData_Ext* pnowData, int nSize); // 主推数据处理
	void HandleAutoPushData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize);     // 主推数据处理
	void HandleSortData(DataHead* pHead, const AnsReportData* pReportData); // 排序数据处理
	void UpdateDataEntry(int nRow, StockUserInfo* pStock, HSStockRealTime_Ext* pData, StockOtherData* pOtherData); // 更新数据入口函数
	void UpdateStockData(int nRow, StockUserInfo* pStock, HSStockRealTime_Ext* pData, StockOtherData* pOtherData); // 更新股票
	void UpdateIndexData(int nRow, StockUserInfo* pStock, HSIndexRealTime* pData, StockOtherData* pOtherData);
	WORD GetColor(double dNewValue, double dPreValue); // 比较前后两个值 得到颜色ID	
    void UpdateCellText(int nCellID, int nItemID, const char* strText, WORD lClr = 0, WORD lFont = 0, DWORD lMark = 0, DWORD lDelMark = 0, BOOL bNeedRedraw = FALSE);
	//////////////////////////////////////////////////////////////////////////
	// 排序相关
	void SetSortColumn(int nCol);		        // 设定排序列
	int  CancelSort();                          // 取消排序
	BOOL GetDefaultSortTypeByCol(int nCol);     // 获取默认排序状态
	BOOL GetDefaultSortTypeByColID(UINT nColID);// 获取默认排序状态
	BOOL IsSupportSortByCol(int nCol);          // 当前列是否支持排序
	BOOL IsSupportSortByColID(UINT nColID);     // 当前列是否支持排序
	BOOL IsSupportLocalSortByCol(int nCol);     // 当前列是否支持本地排序
	BOOL IsSupportLocalSortByColID(UINT nColID);// 当前列是否支持本地排序
	BOOL GetSortInfoByPageInfo(IN const PageInfo* pvInfo, IN short nID, OUT int& nCount,OUT HSMarketDataType& cCodeType); //根据页面信息获取排序属性
	BOOL Sort(UINT nColID, BOOL bAscending, BOOL bForce = FALSE);  // 排序某列
	//////////////////////////////////////////////////////////////////////////
	// 添加标记相关
	void OnMarkCodeByIcon(UINT nID, int nRow);  // 添加图标标记
	void OnMarkCodeByString(int nRow);          // 添加文字标记
	CString GetMarkContent(CPoint& point);      // 获取用户输入的文字
	void OnCancelMarkCode(int nRow);            // 取消标记
	//////////////////////////////////////////////////////////////////////////
	// 板块相关
	void OnBlockAdscription(int nRow);          // 所属板块
	void OnAddToSelfBlock(int nRow);            // 加入自选板块
	void OnBlockDelete(int nRow);               // 从自选板块删除
	//////////////////////////////////////////////////////////////////////////
	// 页面切换
	void GotoDealDetail(int nRow);              // 切换到F1
	void GotoPriceBook(int nRow);               // 切换到F2
	void GotoFinanceInfo(int nRow);             // 切换到财务数据
	void GotoTechAnalysis(int nRow, int nMark = 1); // 切换到分时(nMark = 1)或者K线(nMark = 2)
	//////////////////////////////////////////////////////////////////////////
	// 相应键盘精灵消息
	int  GetMarketType(LPARAM lparam, BOOL& bSort);   // 传入键盘精灵的参数，返回市场类型，返回是否要涨幅排序
	void EnSureVisible(int nColID);                   // 确定该列显示出来
	//////////////////////////////////////////////////////////////////////////
// 改写基类的函数
	virtual void    MoveCol(int nBegin, int nEnd); // 在基类的基础上，再要实现列ID与列位置的改变
	virtual BOOL    Scroll(int nDiff, BOOL bIsVert = TRUE, BOOL bReDraw = TRUE);     // 在基类的基础上，实现发送股票请求
	virtual BOOL    SetPosition(int nPosition, BOOL bIsVert, BOOL bIsRedraw = TRUE); // 在基类的基础上，实现发送股票请求
	virtual void    OnFixedRowClick(); // 当用户单击到固定行
	virtual BOOL    DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem); // 自定义绘图
	virtual CString GetTipText(int nCellID, int nItemID);
//interface
	virtual BOOL    Draw(CDC* pDC);
	virtual BOOL    SetRect(CRect rc);
	virtual BOOL    GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int	    OnUpdateData(DataHead* pData);
	virtual BOOL    MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	virtual int	    OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void    OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam);
	virtual BOOL    OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);
	//////////////////////////////////////////////////////////////////////////
	// 实现标签页回调函数
	virtual void    OnActiveTabChange(CDrawQuoteTab* pActiveItem);
	virtual void    TabInvalidate(CRect* pRect, BOOL bErase = TRUE);
	virtual CWnd*   TabGetCWnd();
protected:
	IHsColumnProperty*		 m_iColProp;	    // 列属性配置接口
	IHsTabCfg*               m_iTab;            // 标签页配置接口
	IDataSourceEx*	         m_pDataSource;     // 数据引擎接口指针
	IBlockManager*			 m_iBlockManager;	// 板块管理
	IHsColor*                m_pColor;          // 颜色配置接口
	IHsFont*                 m_pFont;           // 字体配置接口
	IHsUserDataCfg*          m_iUserDefCfg;     // 用户数据接口（股票标记）
	IQuoteLang*				 m_iQuoteLang;		// 行情资源接口
	
	CDrawQuoteTabCtrl*       m_pQuoteTabCtrl;   // 标签页

	PageInfo    	            m_pviInfo;		// 当前页面信息
	CMap<UINT, UINT, int, int>  m_mapColumn;    // 记录列ID与位置的map key:列ID, value:列位置 
	CArray<StockUserInfo*,StockUserInfo*>	m_ayStock;	   	    // 当前显示的所有股票代码信息
	CMap<UINT,UINT,CString,CString>		    m_mapTempColGroup;	// （临时）列头组名称菜单对应项
	CMap<int,int,int,int>					m_mapRefreshCells;	// 正在刷新的单元格列表
//	CMapStringToPtr                         m_mapCodeInfo;      // 记录当前页所CArray<CurrentFinanceData>              m_ayFinanceData;    // 财务数据

	CImageList*     m_pImageList;           // 股票标记图标

	BOOL			m_bNeedRequestSort;	    // 是否需要请求排序数据（某些列不需要请求，如代码和名称等）
	CString			m_strCurColGroup;		// 当前列头组名称
	CString			m_strPreColGroup;	    // 前一个列头组名称

	int             m_nCurActiveRow;        // 当前选中行
	int				m_nDelayRealTimeRequestTimerID;		// 延时实时数据请求定时器ID	
	int				m_nDelaySortRequestTimerID;			// 延时排序请求定时器ID
	int				m_nRefreshTimerID;			        // 刷新定时器ID

	BOOL            m_bNeedRefresh;         // 是否需要刷新
	int             m_nSortColumnID;        // 排序行行ID
	BOOL            m_bAscending;           // 当前排序的方向

	DWORD  m_nTickCount; // for test
};