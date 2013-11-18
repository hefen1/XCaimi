#include "..\common\tablecommondefine.h"
#include "..\common\hsstruct.h"
#include "..\common\quotestuctdefine.h"
#include "quotesort.h"
#include "hsgridtable.h"
#include "skinscrollbar.h"

#pragma once

extern int AnalyzeStr(CString strResource,const char* strDelimit,CStringArray& ayRet);

#define INIT_FIXED_HEAD			0x00010000
#define INIT_FIXED_DATA			0x00020000 // 一般报价(1~6)
#define INIT_SORTRPT_DATA		0x00040000 // 排名(61~66)
#define INIT_FSDetail_DATA		0x00080000 // 分时明细表
#define INIT_PanHouDetail_DATA  0x00100000 // 盘后明细表
#define INIT_BigDetail_DATA		0x00200000 // 大盘明细表
#define INIT_FENJIA_DATA		0x00400000 // 分价表


// for test
//	查找操作的类型
enum FindType	
{
	none = 0x00, 
	SI_CODE = 0x01,	//	按照股票代码进行查找
	SI_NAME = 0x02,	//	按照股票名称进行查找
	SI_PYJC = 0x04, //	按照拼音简称进行查找
	SI_FOREX= 0x10	//  找不到代码不赋000001股票
};


/*类 用于管理表格列等*/
class CMapWordToPtrEx;
class CTableColume : public GV_ITEM
{
public:
	//列掩码	列风格
	enum { NewColMask = 0x1,NewColFormat = 0x2 };
	CTableColume();
	CTableColume(WORD dID);
	~CTableColume();

	//复制数据
	CTableColume* operator=(CTableColume* pCol);


	BYTE	m_bDelete;					//掩码,标注被清除的内容, 取值:NewColMask, NewColFormat
	WORD	m_dID;						//列ID
	TableColProperty* m_pProperty;		//列属性

	int m_nTempWidth;					// 临时宽度

protected:
	//字段大类哈希表
	static CMapStringToPtr 		 m_mapList;
	static COLORREF*			m_colTest;	//for testing

public:
	//根据掩码, 删除mask, nFormat内存
	void Delete();

	//将取得m_dID列的属性指针, 失败返回NULL
	TableColProperty* GetColProperty();
	//取得列名称, 如果失败, 返回:"无"
	CString				  GetName();
	//取得列掩码以及风格指针
	UINT				  GetRowMask();
	UINT				  GetRowFormat();
	//设置列掩码以及风格	bRow  - 是否为列
	void SetRowMask(UINT nMask,BOOL bRow = TRUE);
	void SetRowFormat(UINT nFormatRef,BOOL bRow = TRUE);

	//取得,设置列宽度	nWidth - 字符个数 * 2
	void SetColWidth(int nWidth);
	int  GetColWidth();

	//新分配一个字段分类哈希表, 返回指针
	static CMapWordToPtrEx*		GetMapColName();
	//在字段大类哈希表中,查找字段分类哈希表, 失败返回NULL, CString strName - KEY
	static CMapWordToPtrEx*		GetMap(CString strName);
	//在字段大类哈希表中,查找字段分类哈希表, 失败返回NULL, WORD key - KEY
	static CMapWordToPtrEx*		GetMap(WORD key);
	//根据字段ID, 取得所属分类哈希表的KEY
	static CString				GetMapName(WORD key);
	//建立哈希表
	static void					InitColName();
	/********************************************************************************
	* 函数功能 : 字段属性读配置文件
	* 函数参数 : CWinApp* pApp - 未使用,
	*			 WORD dNameID - 字段ID,
	*			 CString& strName - 返回值,
	*			 CString strSection - 段名称
	*  返回值  : BOOL
	* 调用依赖 : 
	* 作    者 : 
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	static BOOL					GetColName(CWinApp* pApp,WORD dNameID,CString& strName,CString strSection);

	/********************************************************************************
	* 函数功能 : 字段属性写配置文件
	* 函数参数 : WORD wKey - 字段ID, 
	*			 TableColProperty* pCol - 属性指针 , 
	*			 LPCTSTR lpszSection 段名称,
	*			 LPCTSTR lpszFileName 文件
	*  返回值  : void
	* 调用依赖 : 
	* 作    者 : 
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	static void					WriteColName(WORD wKey, TableColProperty* pCol = NULL, LPCTSTR lpszSection = NULL,LPCTSTR lpszFileName = NULL);

	//删除字段分类哈希表, 并写配置文件	strName - 分类KEY
	static void					DelMapColName(CString strName);
	//查找字段是否存在	key - ID
	static BOOL			  	    Find(WORD key);
	//通过字段ID查找该字段属性或者列名, 失败返回NULL,"无"
	static TableColProperty*	GetByKey(WORD key);
	static CString				GetName(WORD key);
	//通过字段ID取得行掩码或者属性指针
	static UINT				GetRowMask(WORD key);
	static UINT				GetRowFormat(WORD key);
	//通过字段ID取得列掩码或者属性指针
	static UINT				GetColMask(WORD key);
	static UINT				GetColFormat(WORD key);

	// 返回字段大类哈希表指针
	static CMapStringToPtr*		GetList() { return &m_mapList; }
	//添加字段分类哈希表		CString strName - KEY ,CMapWordToPtrEx* pValue - 分类哈希表指针
	static void					AddToList(CString strName,CMapWordToPtrEx* pValue);
	//删除字段大类哈希表 字段配置写配置文件
	static void					DelList();
	//删除一字段 写配置文件 未使用
	static void					DelColumn(WORD key);

	//通过字段ID取相关数据	
	static CString				GetNameByColID(WORD dID);		//列名
	static COLORREF				GetBkClrByColID(WORD dID);		//背景色
	static COLORREF				GetFgClrByColID(WORD dID);		//前景色
	static UINT					GetStyleByColID(WORD dID);		//风格
	static UINT					GetDataTypeByColID(WORD dID);	//数据类型
};

class CMapWordToPtrEx : public CMapWordToPtr
{
protected:
	/*类型风格	高16位,基本风格	定义:MPEXS系列 按位与或	*
	*	8 - 15位,类型,	定义:MPEXT系列 比较值
	*	0 - 7位,保留*/
	DWORD m_dwStyle;
public:
	CMapWordToPtrEx()
	{
		m_dwStyle = MAKELONG( 0, MPEXS_RWCONFIG);		
	}
public:
	//设置哈希表
	void SetAt(WORD key, void* newValue,CWinApp* pApp,CString& strName,CString& strSection)
	{
		TableColProperty* pCol = (TableColProperty*)newValue;

		if(pCol != NULL)
		{
			if( IsStyle(MPEXS_RWCONFIG) && 
				CTableColume::GetColName(pApp,key,strName,strSection) )
			{
				CStringArray ayRet;
				if(AnalyzeStr(strName,";",ayRet) >= 7)
				{
					if( !IsStyle(MPEXS_COLNAME_NORWCFG) )
					{
						pCol->m_strName.Format("%s",ayRet[0]);
					}
					pCol->m_nRowMask   = atol(ayRet[1]);
					pCol->m_nRowFormat = atol(ayRet[2]);
					pCol->m_nColMask   = atol(ayRet[3]);
					pCol->m_nColFormat = atol(ayRet[4]);
					pCol->m_TextColor  = atol(ayRet[5]);
					pCol->m_cWidth     = atol(ayRet[6]);
					pCol->m_cConfigWidth = pCol->m_cWidth;
				}
				else
				{
					pCol->m_strName.LoadString(atoi(pCol->m_strName));
				}
			}
			else
			{
				pCol->m_strName.LoadString(atoi(pCol->m_strName));
			}
		}
		CMapWordToPtr::SetAt(key,newValue);
	}
	void SetAt(WORD key, void* newValue)
	{
		void* pValue;
		if( CMapWordToPtr::Lookup(key,pValue) ) // 已经存在
		{
			delete (CTableColume*)pValue;
			RemoveKey(key);
			return;
		}
		CMapWordToPtr::SetAt(key,newValue);
	}
public:
	//判断风格
	BOOL IsStyle(WORD wStyle){return wStyle & HIWORD(m_dwStyle);}
	//设置风格
	void ModifyStyle(WORD wAddStyle, WORD wRemoveStyle)
	{
		m_dwStyle |= MAKELONG(0, wAddStyle);
		m_dwStyle &= ~ MAKELONG(0, wRemoveStyle);
	}
public:
	//取得或者设置类型
	BYTE GetType(){ return HIBYTE( LOWORD(m_dwStyle) );}
	void SetType(BYTE tType)
	{
		m_dwStyle &= 0xFFFF00FF;
		m_dwStyle |= MAKELONG(MAKEWORD(0, tType), 0);
	} 
public:
	//保留
	BYTE GetReserve(){ return LOBYTE( LOWORD(m_dwStyle) );}
	void SetReserve(BYTE tType)
	{
		m_dwStyle &= 0xFFFFFF00;
		m_dwStyle |= MAKELONG(MAKEWORD(tType, 0), 0);
	} 

};

//////////////////////////////////////////////////////////////////////////
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
struct InitTableData
{
	DWORD m_dStyle;
	int   m_nRowCount;
	int   m_nColCount;
	int   m_nFixRowCount;
	int   m_nFixColCount;
	void* m_pData;

	CString m_strGroupName;

	InitTableData()
	{
		m_dStyle = INIT_FIXED_HEAD;
		m_nRowCount = 1;
		m_nColCount = 3;
		m_nFixRowCount = 1;
		m_nFixColCount = 3;
		m_pData = NULL;
	}
	BOOL IsStyle(DWORD dStyle) {return (m_dStyle & dStyle);}
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CQuoteReportTable

class CQuoteReportTable : public CHSGridTable
{
	DECLARE_DYNAMIC(CQuoteReportTable)

public:
	CQuoteReportTable(int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
	virtual ~CQuoteReportTable();


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////重载操作
	// 设定指定列宽 注意：nWidth为英文半角字符数,需要通过计算得到像素数
	virtual int SetColumnWidth(int nColID,int nWidth = -1);

	CString GetMenuName(int nType = 0, int nSubType = 0);

	virtual BOOL ShowRow(int row, BOOL bRefresh = FALSE);	// 显示指定行
	virtual BOOL ShowCol(int col, BOOL bRefresh = FALSE);	// 显示指定列

	virtual BOOL MoveRowTo(int nSrc,int nDes);		// 移动指定行到目标行
	virtual BOOL MoveColTo(int nSrc,int nDes);		// 移动指定列到目标列


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////列（头）操作
	/********************************************************************************
	* 函数功能 : 获取默认列头组名称
	* 函数参数 : CString strFileName : 存放默认列头组名称的配置文件
	*  返回值  : CString 列头组名称
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-04
	* 修改记录 : 
	*******************************************************************************/
	CString	GetDefultColGroup(CString strFileName = _T(""));

	/********************************************************************************
	* 函数功能 : 返回列头模板所在的文件名
	* 函数参数 : int nType : 列头模板类型
	*  返回值  : CString 列头模板所在完整文件名
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	static CString GetColHeadTemplFileName(int nType = 0);

	/********************************************************************************
	* 函数功能 : 读取配置文件中的列头模板
	* 函数参数 : CString strTemplName : 列头模板名称
	*			 int nType : 模板类型
	*			 CArray<CStringArray*,CStringArray*>* ayColTemplList : 模板列表
	*  返回值  : int 模板列表项个数
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	static int LoadColHeadTemplate(CString strTemplName, int nType = 0,
		CArray<CStringArray*,CStringArray*>* ayColTemplList = NULL);

	/********************************************************************************
	* 函数功能 : 根据列头组名称初始化列头
	* 函数参数 : CString strGroupName : 列头组名称
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-04
	* 修改记录 : 
	*******************************************************************************/
	virtual void InitColGroup(CString strGroupName);

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
	* 完成日期 : 2010-08-04
	* 修改记录 : 
	*******************************************************************************/
	virtual int GetColIndexByID(UINT uiID);

	/********************************************************************************
	* 函数功能 : 根据列ID获取列头对象指针
	* 函数参数 : [IN] UINT uiID : 列ID
	*			 [OUT] int& nPos : 列头序号
	*  返回值  : CTableColume* 列头指针
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-04
	* 修改记录 : 
	*******************************************************************************/
	virtual CTableColume* GetColByID(UINT uiID);
	virtual CTableColume* GetColumn(UINT nID,int& nPos);

	/********************************************************************************
	* 函数功能 : 根据列序号获取列头对象指针
	* 函数参数 : [IN] int nPos : 列序号
	*  返回值  : CTableColume* 列头指针
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-11
	* 修改记录 : 
	*******************************************************************************/
	virtual CTableColume* GetHead(int nPos);

	/********************************************************************************
	* 函数功能 : 添加列
	* 函数参数 : UINT uiID : 列ID
	*			 InitTableData* pInitData : 
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	virtual void AddColumn(UINT nID);

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
	virtual void Sort(int nID, BOOL bAscending);

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

	void ResetMap();
	void AddColToMap(int nID,int nColPos);


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////股票代码列表操作
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
	virtual void SetStock(CArray<StockUserInfo*,StockUserInfo*>& payStock,int nTotal);
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





	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////页面（属性）操作
	/********************************************************************************
	* 函数功能 : 改变当前页面
	* 函数参数 : PageViewInfo* pvInfo : 页面属性
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	virtual int ChangeContent(PageViewInfo* pvInfo);

	/********************************************************************************
	* 函数功能 : 设定当前页面信息
	* 函数参数 : PageViewInfo* pvInfo : 页面属性
	*  返回值  : 
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-08-09
	* 修改记录 : 
	*******************************************************************************/
	virtual void SetPageViewInfo(PageViewInfo* pvInfo);

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

	virtual void InitFixedDataItem(int nFirst,int nCount);

	virtual void SetFixedData(int nRow,int nCol,CGridCellBase* pCell, StockUserInfo* pStock = NULL,
		int nNumber = -1, BOOL bRefresh = TRUE, CDC* pDC = NULL);

	virtual void GetClientRect(LPRECT lpRect);

	//////////////////////////////////////////////////////////////////////////滚动条操作
	BOOL SetHorScrollBarRef(CSkinScrollBar* pNewHorScrollBar = NULL);	// 设定第三方水平滚动条，替代系统默认水平滚动条
	BOOL SetVerScrollBarRef(CSkinScrollBar* pNewVerScrollBar = NULL);	// 设定第三方垂直滚动条，替代系统默认垂直滚动条

	virtual void ResetScrollBars();
	virtual int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	virtual BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);

	// 更新数据
	//StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime_Ext* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//virtual void UpdateStdData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);

	//// 更新股票列
	//virtual void UpdateStockData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//virtual void UpdateStockDataExt(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime_Ext* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//// 更新指数列
	//virtual void UpdateIndexData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSIndexRealTime* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//// 更新指数列
	//virtual void UpdateIndexDataExt(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime_Ext* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//// 更新期货列
	//virtual void UpdateFuturesData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSQHRealTime* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);

	//// 更新股票列
	//virtual void UpdateForeignData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime* pForeignData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//// 更新股票列
	//virtual void UpdateHKData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime* pHKData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	////yangdl 2008.01.22 更新外汇中心
	//virtual void UpdateWHCenterData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSWHCenterRealTime* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);

	PageViewInfo	m_pviInfo;					// 当前页面信息

	int				m_nCurSortColumnID;			// 当前正在排序的列id
	BOOL			m_bAscending;				// 是否正序排列
	CString			m_strCurColGroup;			// 当前列头组名称
	int				m_nColHeadHeigh;			// 当前列头高度

	int				m_nCurActiveRow;			// 当前激活行

	CSkinScrollBar* m_pHorScrollBar;			// 第三方水平滚动条
	CSkinScrollBar* m_pVerScrollBar;			// 第三方垂直滚动条

	CArray<CTableColume*,CTableColume*>		m_ayColInfo;	// 所有列属性
	CArray<StockUserInfo*,StockUserInfo*>	m_pStock;		// 当前显示的所有股票代码信息
	
	CMap<int,int,int,int>					m_mapColumn;	//列ID和列序号对应表
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnDraw(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


