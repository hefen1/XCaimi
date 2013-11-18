/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDrawTableBase.h
*	文件标识：
*	摘	  要：  表格基类
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-01-13
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "..\..\..\Common\QuoteComm\DrawBase.h"
#include "DiagramFoundationDef.h"
#include "HSScrollBar.h"
#include "HSTitle.h"
#include "DrawTableStruct.h"
#include "DrawCell.h"

#define  TABLE_STYLE_SCREEN     0x0001   // 普通表格
#define  TABLE_STYLE_SNAKE      0x0002   // 蛇形风格表格(列宽自动调整、行宽自动调整)

// 以下属性只有是普通表格才可以使用
#define  TABLE_WIDTH_CHANGABLE  0x0010   // 表格列宽自动调整
#define  TABLE_HEIGHT_CHANGABLE 0x0020   // 表格行宽自动调整
#define  TABLE_HSCROLL_NO_ENDING  0x0040 //表格滚动无限滚(行情报价水平滚动方式)
// 以下属性公有
#define  TABLE_ROW_SELECT       0x0100   // 表格行可选中 以高亮显示
//////////////////////////////////////////////////////////////////////////
// 定时器ID (long)this + (int)Span
#define TIMER_TIP               0x1000   // 显示TIP
#define TIMER_GETCELL           0x2000   // 计算鼠标当前所在单元格

//////////////////////////////////////////////////////////////////////////
// 声明表格操作类
// 坐标变换
class CPointChangeBehavior;
//////////////////////////////////////////////////////////////////////////

class  HS_EXT_CLASS CDrawTableBase :
	public CDrawBase, 
	public IScrollNotify,  // 滚动条
	public ITitleNotify    // 标题

{
public:
	CDrawTableBase(void);
	virtual ~CDrawTableBase(void);

	/********************************************************************************
	 * 函数功能 : 设置表格风格
	 * 函数参数 : WORD wStyle - 表格风格
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	void SetStyle(WORD wStyle); 
	
	/********************************************************************************
	 * 函数功能 : 设置以及读取行数
	 * 函数参数 : int nRowCount - 行数
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	void SetRowCount(int nRowCount);      
	int  GetRowCount();

	/********************************************************************************
	 * 函数功能 : 设置以及读取固定行数
	 * 函数参数 : int nRowFixCount - 固定行数
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *********************************x**********************************************/
	void SetFixRowCount(int nRowFixCount);
	int  GetFixRowCount();

	/********************************************************************************
	 * 函数功能 : 设置以及读取行高
	 * 函数参数 : int nHeight - 行高
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	void SetRowHeight(int nHeight);        
	int  GetRowHeight();

	/********************************************************************************
	 * 函数功能 : 设置以及读取列数
	 * 函数参数 : int nColCount - 列数
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	void SetColCount(int nColCount);      
	int  GetColCount();

	/********************************************************************************
	 * 函数功能 : 设置以及读取固定列数
	 * 函数参数 : int nColFixCount - 固定列数
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	void SetFixColCount(int nColFixCount); 
	int  GetFixColCount();

	/********************************************************************************
	 * 函数功能 : 读取当前可见行、列
	 * 函数参数 : 
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	int GetShowColCount();
	int GetShowRowCount();

	/********************************************************************************
	 * 函数功能 : 读取当前表格区域
	 * 函数参数 : 
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	CRect GetTableRect();

	/********************************************************************************
	 * 函数功能 : 获取当前移动的位移
	 * 函数参数 : 
	 *  返回值  : CPoint
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	CPoint GetMove(); 
	/********************************************************************************
	 * 函数功能 : 设置以及读取列属性
	 * 函数参数 : int nIndex - 列索引
	 *            ColInfoStruct* pColInfo - 列属性指针
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	void  SetColInfo(int nIndex, ColInfoStruct* pColInfo); // 设置某列的列属性(针对每一列的宽度都不一样)
	const ColInfoStruct* GetColInfo(int nIndex);

	/********************************************************************************
	 * 函数功能 : 将行下标转化成列属性下标
	 * 函数参数 : int nIndex - 列索引
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	int   ColNumToIndex(int nColNum);

	/********************************************************************************
	 * 函数功能 : 根据属性计算当前可显示行列，宽度，高度等
	 * 函数参数 : 
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
    void CalCulateSize();          

	/********************************************************************************
	 * 函数功能 : 设置单元格数量 如果小于当前数量会自动释放内容
	 *            获取单元格数量
	 * 函数参数 : int  nCount - 单元格数量
	 *            BOOL bRelease - 释放内存标志
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	void SetCellCount(int nCount, BOOL bRelease = FALSE); 
	int  GetCellCount();

	/********************************************************************************
	 * 函数功能 : 创建滚动条以及标题
	 * 函数参数 : int dwStyle - 滚动条风格
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	void CreateVerScroll(int dwStyle); // 创建垂直滚动条
	void CreateHorScroll(int dwStyle); // 创建水平滚动条
	void CreateTitle();                // 创建标题
	/********************************************************************************
	 * 函数功能 : 构造单元格
	 * 函数参数 : CArray<ItemConstructionStruct>* pAyItemStuct - 单元小格属性数组
	 *            DWORD dwCellMask - 单元格属性
	 *            int nIndex - 添加上数组的那个索引位置
	 *
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	CDrawCell* CreateCell(CArray<ItemConstructionStruct>* pAyItemStuct, DWORD dwCellMask = 0, int nIndex = -1);
	CDrawCell* CreateCell(CString str, WORD lClr = 0, WORD lFont = 0, DWORD nMask = 0, DWORD nCellMask = 0, DWORD dwExtent = 0, int nIndex = -1);

	/********************************************************************************
	 * 函数功能 : 删除列
	 * 函数参数 : int nStartCol - 起始位置
	 *			  int nCount - 数量
	 *
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL RemoveCol(int nStartCol, int nCount = 1);

	/********************************************************************************
	 * 函数功能 : 删除行
	 * 函数参数 : int nStartRow - 起始位置
	 *			  int nCount - 数量
	 *
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL RemoveRow(int nStartRow, int nCount = 1);

	/********************************************************************************
	 * 函数功能 : 加入一列属性都相同的单元格
	 * 函数参数 : int nIndex - 位置
	 *			  CString strHead - 列标题
	 *            BOOL bFixRow    - 标记后面的属性仅仅是给固定行还是全部行
	 *            WORD wClr       - 字体颜色
	 *            WORD wFont      - 字体
	 *            DWORD dwMask    - 单元小格属性
	 *            DWORD dwCellMask- 单元格属性
	 *
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	void InsertCol(int nIndex, BOOL bFixRow, CArray<ItemConstructionStruct>* ayItemStuct, DWORD dwCellMask = 0);
	void InsertCol(int nIndex, BOOL bFixRow, CString strHead, WORD wClr = 0, WORD wFont = 0, DWORD dwMask = 0, DWORD dwCellMask = 0);
	
	/********************************************************************************
	 * 函数功能 : 创建行单元格 
	 * 函数参数 : int nRowCount - 行数
	 *			  
	 *
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	void CreateRowCell(int nRowCount);
	
	/********************************************************************************
	 * 函数功能 : 单元格行列编号 与 索引值互相转换
	 * 函数参数 : int nIndex - 索引值 CPoint pos - 单元格行列编号
	 *  返回值  : CPoint - 单元格行列编号 失败返回CPoint(-1, -1);
	 *			  int 索引值 失败返回-1
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	CPoint IndexToPoint(int nIndex);
	int PointToIndex(CPoint pos);

	/********************************************************************************
	 * 函数功能 : 单元格是否为行列固定
	 * 函数参数 : int nIndex - 索引值 CPoint pos - 单元格行列编号
	 *            BOOL bIsRow - 如果是固定行 返回TRUE 仅在返回值为TRUE才有效
	 *  返回值  : BOOL 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL  IsCellRowFixed(int nIndex);
	BOOL  IsCellRowFixed(CPoint pos);
	BOOL  IsCellColFixed(int nIndex);
	BOOL  IsCellColFixed(CPoint pos);

	/********************************************************************************
	 * 函数功能 : 从单元格的下标(一维或者二维) 获取在表格中列的位置
	 * 函数参数 : int nIndex - 一维坐标
	 *            CPoint pos - 二维坐标
	 *  返回值  : int - 列
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	int   GetColumn(int nIndex);
	int   GetColumn(CPoint pos);

	/********************************************************************************
	 * 函数功能 : 从单元格的下标(一维或者二维) 获取在表格中行的位置
	 * 函数参数 : int nIndex - 一维坐标
	 *            CPoint pos - 二维坐标
	 *  返回值  : int - 行
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	int   GetRow(int nIndex);
	int   GetRow(CPoint pos);
	
	/********************************************************************************
	 * 函数功能 : 取得单元格区域
	 * 函数参数 : CRect& rect - 返回区域
	 *			  int nIndex - 索引值 CPoint pos - 编排位置 
	 *  返回值  : BOOL - 成功返回TRUE 失败返回FALSE
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL GetCellRect(int index, CRect& rect);
	BOOL GetCellRect(CPoint pos, CRect& rect);

	/********************************************************************************
	 * 函数功能 : 设置一个单元格文本, 
	 * 函数参数 : CPoint pos - 单元格位置编排点 
	 *            int    nItemID - Item的ID号
	 *			  const char* strText - 字府串 非NULL
	 *            COLORREF textClr - 字体颜色
	 *            DWORD lMark - 显示格式 (特别注意 先进行去除操作 再进行添加操作)
	 *            DWORD lDelMark - 去除显示格式
	 *            BOOL bNeedRedraw - 是否需要重绘
	 *  返回值  : CDrawCell* - 当前单元格指针
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	CDrawCell* SetCellText(CPoint pos, int nItemID, const char* strText, WORD lClr = 0, WORD lFont = 0, DWORD lMark = 0, DWORD lDelMark = 0, BOOL bNeedRedraw = FALSE);
	CDrawCell* SetCellText(int nCellID, int nItemID, const char* strText, WORD lClr = 0, WORD lFont = 0, DWORD lMark = 0, DWORD lDelMark = 0, BOOL bNeedRedraw = FALSE);

	/********************************************************************************
	 * 函数功能 : 得到单元格指针
	 * 函数参数 : int nCellID - 单元格坐标
	 *            CPoint pos  - 单元格逻辑二维坐标
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	CDrawCell* GetCell(CPoint pos);
	CDrawCell* GetCell(int nCellID);

	/********************************************************************************
	 * 函数功能 : 得到单元小格指针
	 * 函数参数 : int nCellID - 单元格坐标
	 *            CPoint pos  - 单元格逻辑二维坐标
	 *            CDrawCell* pCell - 单元格指针
	 *            int nItem - 单元小格下标
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	CDrawItem* GetItem(CPoint pos, int nItem);
	CDrawItem* GetItem(int nCellID, int nItem);
	CDrawItem* GetItem(CDrawCell* pCell, int nItem);

	/********************************************************************************
	 * 函数功能 : 获取单元格文本内容
	 * 函数参数 : int nCellID - 单元格坐标
	 *            CPoint pos  - 单元格逻辑二维坐标
	 *            int nItem - 单元小格下标 如果 -1 则表示所有的单元小格
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	void GetCellText(CPoint pos, int nItem, CString& str);
	void GetCellText(int nCellID, int nItem, CString& str);

	/********************************************************************************
	 * 函数功能 : 删除所有单元格
	 * 函数参数 : 
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	void RemoveAllCell();

	/********************************************************************************
	 * 函数功能 : 单元格内数据整体平移(行)
	 * 函数参数 : int nDiff - 移动的行数(正数表示向上，负数表示向下)
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-04
	 * 修改记录 : 
	 *******************************************************************************/
	void CellMove(int nDiff);

	/********************************************************************************
	 * 函数功能 : 单元格整体滚动
	 * 函数参数 : int nDiff - 滚动行数(正数表示向下(向右)，负数表示向上(向左))
	 *            BOOL bIsVert - 滚动方向(TRUE表示垂直, FLASE表示水平) 
	 *            BOOL bReDraw - 是否重绘
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-04
	 * 修改记录 : 
	 *******************************************************************************/
	virtual BOOL Scroll(int nDiff, BOOL bIsVert = TRUE, BOOL bReDraw = TRUE);
	
	/********************************************************************************
	 * 函数功能 : 设置(移除、获得)激活行
	 * 函数参数 : int nRow - 行号
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-04
	 * 修改记录 : 
	 *******************************************************************************/
	virtual BOOL SetActiveRow(int nRow);
	virtual BOOL RemoveActiveRow(int nRow);
	virtual int  GetActiveRow();

	/********************************************************************************
	 * 函数功能 : 设置(移除、获得)激活列
	 * 函数参数 : int nRow - 行号
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2011-01-04
	 * 修改记录 : 
	 *******************************************************************************/
	virtual BOOL SetActiveCol(int nCol);
	virtual BOOL RemoveActiveCol(int nCol);
	virtual int  GetActiveCol();

	/********************************************************************************
	 * 函数功能 : 画背景
	 * 函数参数 : CDC* pDC - 设备上下文
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	virtual BOOL DrawTableBK(CDC* pDC);

	/********************************************************************************
	 * 函数功能 : 自定义绘制函数 子类需要在某一列的某一单元格中实现自定义绘图
	 * 函数参数 : CDC* pDC   - 设备上下文
	 *			  CPoint pos - 单元格位置编排点 
	 *            int    nItemID - 该单元格中的单元小格下标
	 *            CRect  rcItem  - 需要绘制的区域
	 *  返回值  : BOOL   执行成功返回 TRUE
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	virtual BOOL DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem);	
	
	/********************************************************************************
	 * 函数功能 : 绘制单元格
	 * 函数参数 : CDC* pDC   - 设备上下文
	 *			  CPoint pos - 单元格位置编排点 
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL DrawCell(CDC* pDC, CPoint pos);	

	/********************************************************************************
	 * 函数功能 : 绘制Tip
	 * 函数参数 : CDC* pDC   - 设备上下文
	 *			  const CString& strContent - Tip文本内容
	 *  返回值  : BOOL
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL DrawTip(CDC* pDC, const CString& strContent);

	/********************************************************************************
	 * 函数功能 : 还原绘图操作状态 主要用于还原tip 在鼠标滑轮滚动 按键里面调用
	 * 函数参数 : 
	 *			  
	 *  返回值  : void 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-13
	 * 修改记录 : 
	 *******************************************************************************/
	void RestoreOper();

	/********************************************************************************
	 * 函数功能 : 获得单元格
	 * 函数参数 : CPoint point - 像素 
	 *            int nCursor - 鼠标光标形状
	 *            int nLeft   - 左边框的横坐标
	 *  返回值  : int - 单元格ID
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	int  GetCellID(CPoint point, int &nCursor, int &nLeft);
	int  GetCellID(CPoint point, int* pCursor = NULL, int* pLeft = NULL, int* pSignal = NULL);

	/********************************************************************************
	 * 函数功能 : 调整左右两列单元格大小
	 * 函数参数 : int nCellCol -  列
	 *			  int nDiff - 移动的大小 
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	void  ReSize(int nCellCol, int nDiff);

	/********************************************************************************
	 * 函数功能 : 将列从起始位置移动到目标位置
	 * 函数参数 : int nBegin - 起始列的位置
	 *            int nEnd   - 移动到的位置
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void  MoveCol(int nBegin, int nEnd);

	/********************************************************************************
	 * 函数功能 : 画移动的垂直线
	 * 函数参数 : CPoint pos - 线的起始坐标
	 *            CDC* pDC   - 设备上下文
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	void  DrawLine(CPoint pos, CDC* pDC);

	/********************************************************************************
	 * 函数功能 : 画移动的矩形
	 * 函数参数 : CPoint pos - 矩形单元格坐标
	 *            CString strText - 文本内容
	 *            CDC* pDC - 设备上下文
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	void  DrawRect(CPoint pos, CString strText, CDC* pDC);

	/********************************************************************************
	 * 函数功能 : 通过单元格坐标获得矩形
	 * 函数参数 : CPoint pos - 单元格坐标
	 *            BOOL bIsCell - 是否是单元格
	 *  返回值  : CRect - 矩形
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	CRect GetDrawRect(CPoint pos, BOOL bIsCell = TRUE);
	
	/********************************************************************************
	 * 函数功能 : 通过单元格下标 以及单元小显示内容 找到单元小格索引下标
	 * 函数参数 : int nCellID - 单元格坐标
	 *            CString str - 单元小格显示内容
	 *  返回值  : int - 单元小格下标
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	int   GetItemID(int nCellID, CString str);

	/********************************************************************************
	 * 函数功能 : 通过单元格下标 单元小格下标 返回单元小格属性
	 * 函数参数 : int nCellID - 单元格坐标
	 *            int nItemID - 单元小格下标
	 *            ItemConstructionStruct& sConstructionStruct 单元小格属性
	 *  返回值  : BOOL - 是否成功
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	BOOL  GetItemStyle(int nCellID, int nItemID, ItemConstructionStruct& sConstructionStruct);

	/********************************************************************************
	 * 函数功能 : 获取鼠标当前所在的单元格下标
	 * 函数参数 : 
	 *  返回值  : int - 单元格下标
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	int   GetActiveCellID();

	/********************************************************************************
	 * 函数功能 : 计算当前表格滚动范围
	 * 函数参数 : int bIsVert - 是否为垂直距离
	 *  返回值  : int - 范围
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	int   GetScrollRange(int bIsVert);

	/********************************************************************************
	 * 函数功能 : 鼠标单击到固定列(行)
	 * 函数参数 : 
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void OnFixedColumnClick();
	virtual void OnFixedRowClick();

	/********************************************************************************
	 * 函数功能 : 设置/删除 Draw函数格外的操作
	 * 函数参数 : 
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	virtual void SetDrawOper(WORD wDrawOper) { m_wDrawOper |= wDrawOper; }
	virtual void DelDrawOper(WORD wDrawOper) { m_wDrawOper &= ~wDrawOper; }

	/********************************************************************************
	 * 函数功能 : 获取当前表格操作内容
	 * 函数参数 : 
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	virtual WORD GetSignal() {return m_wSignal;}

	/********************************************************************************
	 * 函数功能 : 设置鼠标移动函数中，需要重绘的次数
	 * 函数参数 : int nTimes - 新的重绘次数
	 *  返回值  : 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	void SetRedrawTimes(int nTimes){ m_nMouseRedrawTimes = m_nMouseRedrawTimes>nTimes?m_nMouseRedrawTimes:nTimes; }

	/********************************************************************************
	 * 函数功能 : 获取需要显示的Tip文本
	 * 函数参数 : int nCellID - 单元格下标
	 *            int nItemID - 单元小格下标
	 *  返回值  : CString - 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/	
	virtual CString GetTipText(int nCellID, int nItemID);

	/********************************************************************************
	 * 函数功能 : 根据光标样式宏 设定光标
	 * 函数参数 : int nCursor - 光标样式宏定义
	 *  返回值  : void  
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/	
	void SetMouseCursor(int nCursor);

	/********************************************************************************
	 * 函数功能 : 还原表格状态
	 * 函数参数 : 
	 *            
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/	
	void ReStoreTable(); 

	/********************************************************************************
	 * 函数功能 : 重置单元格
	 * 函数参数 : CDrawCell* pCell- 单元格指针 
	 *            int nCellID - 单元格ID
	 *            CPoint pos - 单元格二维坐标
	 *  返回值  : void 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	void ResetCell(CDrawCell* pCell);
	void ResetCell(int nCellID);
	void ResetCell(CPoint pos);

	/********************************************************************************
	 * 函数功能 :  刷新区域 给外部调用
	 * 函数参数 : CRect* pRect - 区域指针
	 *            BOOL bErase = TRUE - 是否刷背景
	 *  返回值  : void - 
	 * 调用依赖 : 
	 * 作    者 : 王超
	 * 完成日期 : 2010-01-18
	 * 修改记录 : 
	 *******************************************************************************/
	void  InValidate(CRect* pRect, BOOL bErase = TRUE);  // 刷新区域 给外部调用
	//////////////////////////////////////////////////////////////////////////
	// 实现滚动条回调函数
	virtual BOOL    SetPosition(int nPosition, BOOL bIsVert, BOOL bIsRedraw = TRUE);
	virtual void    ScrollInvalidate(CRect* pRect, BOOL bErase = TRUE);
	virtual HWND    GetHandle();
	//////////////////////////////////////////////////////////////////////////
	// 实现标题回调函数
	virtual void    TitleInvalidate(CRect* pRect, BOOL bErase = TRUE);
	//////////////////////////////////////////////////////////////////////////
//interface:
public:
	virtual BOOL			MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL			MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL            MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL            RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	virtual int				OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);         
	virtual BOOL			SetRect(CRect rc);
	virtual int		    	Draw(CDC* pDC);
	virtual int	            OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
//Timer
	void   KillTipTimer();  // 关闭Tip定时器
	static VOID CALLBACK    ShowTipProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);  // 显示Tip     

//behavior
protected:
	CPointChangeBehavior*   m_pPointChange;
protected:
	CDrawCellArray  m_ayCell;         // 所有的单元格
	CHSScrollBar*   m_pVerScrollBar;  // 垂直滚动条
	CHSScrollBar*   m_pHorScrollBar;  // 水平滚动条
	CHSTitle*       m_pTitle;         // 标题

	BOOL            m_bIsShow;        // 是否显示
private:
	CPoint	m_posMove;		// 水平以及垂直方向移动的距离
	CRect   m_rcTable;      // 表格区域
	
	WORD m_wStyle;          // 风格
	int	m_nRow;		        // 行数
	int	m_nCol;			    // 列数
	int	m_nShowRowNum;		// 当前能显示的行数
	int m_nShowColNum;      // 当前能显示的列数
	int m_nFixCol;          // 固定列
	int m_nFixRow;          // 固定行

	short m_nMouseRedrawTimes;     // 鼠标移动函数中 记录需要引起重绘事件的次数
	CArray<ColInfoStruct> m_ayColInfo; // 列属性数组
	int m_nRowHeight;                  // 行高(当前表格只支持所有行固定高度)
	int m_nColWidth;                   // 单元格列宽 当且仅当蛇形滚动才使用

	WORD  m_wDrawOper;        // Draw函数需要绘制的额外图形的标志
	CPoint m_posNow;          // 记录当前鼠标位置

	WORD m_wSignal;           // 当前需要操作的标志 比如:调整大小、移动、闪电买卖等
	int  m_nCellID;           // 鼠标当前所在的单元格

	int  m_nTipTimeID;        // 显示Tip定时器ID
};


//////////////////////////////////////////////////////////////////////////
// 下面开始声明 表格的一些操作类
// 坐标变换
class HS_EXT_CLASS CPointChangeBehavior
{// 坐标变换基类
public:
	CPointChangeBehavior(CDrawTableBase* pTableBase)
	{
		ASSERT(pTableBase);
		m_pTableBase = pTableBase;
	}
	~CPointChangeBehavior(void){}

	virtual CPoint IndexToPoint(int nIndex) = 0;
	virtual int    PointToIndex(CPoint pos) = 0;
	virtual BOOL   IsCellRowFixed(int nIndex) = 0;
	virtual BOOL   IsCellRowFixed(CPoint pos) = 0;
	virtual BOOL   IsCellColFixed(int nIndex) = 0;
	virtual BOOL   IsCellColFixed(CPoint pos) = 0;
protected:
	CDrawTableBase*  m_pTableBase;
};

//////////////////////////////////////////////////////////////////////////
/* 类似屏幕坐标的坐标变换 对应正常的上下左右移动滚动
*  0 3 6     (0,0) (1,0) (2,0)
*  1 4 7 --> (0,1) (1,1) (2,1)
*  2 5 8     (0,2) (1,2) (2,2)
*/
class HS_EXT_CLASS CScreenPointChangeBehavior : public CPointChangeBehavior
{
public:
	CScreenPointChangeBehavior(CDrawTableBase* pTableBase):CPointChangeBehavior(pTableBase){}
	~CScreenPointChangeBehavior(void){}

	virtual CPoint IndexToPoint(int nIndex);
	virtual int    PointToIndex(CPoint pos);
	virtual BOOL   IsCellRowFixed(int nIndex);
	virtual BOOL   IsCellRowFixed(CPoint pos);
	virtual BOOL   IsCellColFixed(int nIndex);
	virtual BOOL   IsCellColFixed(CPoint pos);
};

//////////////////////////////////////////////////////////////////////////
/* 蛇形滚动坐标变换 对应蛇形的滚动
* 0         (0,0) 
* 1 4 7     (0,1) (1,1) (2,1)
* 2 5 8 --> (0,2) (1,2) (2,2)
* 3 6 9     (0,3) (1,3) (2,3)
*/
class HS_EXT_CLASS CSnakePointChangeBehavior : public CPointChangeBehavior
{
public:
	CSnakePointChangeBehavior(CDrawTableBase* pTableBase):CPointChangeBehavior(pTableBase){}
	~CSnakePointChangeBehavior(void){}

	virtual CPoint IndexToPoint(int nIndex);
	virtual int    PointToIndex(CPoint pos);
	virtual BOOL   IsCellRowFixed(int nIndex);
	virtual BOOL   IsCellRowFixed(CPoint pos);
	virtual BOOL   IsCellColFixed(int nIndex);
	virtual BOOL   IsCellColFixed(CPoint pos);
};

