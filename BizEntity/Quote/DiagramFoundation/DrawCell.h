/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDrawCell.h
*	文件标识：
*	摘	  要：  单元格类
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
#include "DrawTableStruct.h"
#include "DrawItem.h"
#include "DiagramFoundationDef.h"

// 单元格属性
#define CELL_BORDER_ENY        0x000F  // 全边框
#define CELL_BORDER_LEFT       0x0001  // 左边框
#define CELL_BORDER_RIGHT      0x0002  // 右边框
#define CELL_BORDER_TOP        0x0004  // 上边框
#define CELL_BORDER_BOTTOM     0x0008  // 下边框

#define CELL_RESIZABLEWE       0x0010  // 可以拖动左边框改变单元格大小
#define CELL_MOVABLE           0x0020  // 可以选中拖动位置
#define CELL_BK_HIGHLIGHT      0x0040  // 背景高亮
#define CELL_TIP               0x0080  // 显示Tip

#define CELL_MIN_WIDTH             40  // 单元格最小宽度
//////////////////////////////////////////////////////////////////////////
// CDrawCellArray
class CDrawCell;
class CDrawCellArray : public CPtrArray
{
public:
	CDrawCell*& operator[](int nIndex) {return (CDrawCell*&)ElementAt(nIndex); }
	//free all elements
	void DeleteAll();
	~CDrawCellArray() { DeleteAll(); }
};
//////////////////////////////////////////////////////////////////////////

class CDrawTableBase; // 表格基类前置声明 因为这里出现相互包含头文件的情况
class CDrawCell
{
public:
	CDrawCell(CArray<ItemConstructionStruct>* ayItemStruct, DWORD dwCellMask = 0);
	CDrawCell(CString str, WORD lClr, WORD lFont, DWORD nMask = 0, DWORD dwCellMask = 0, DWORD dwExtent = 0); // 当单元格只有一列的情况
	~CDrawCell(void);
	void  ReadCfg();  // 读配置文件
	void  ReCreate(CArray<ItemConstructionStruct>* pAyItemStruct, DWORD dwCellMask = 0);  // 根据属性重构单元格
	void  ReCreate(CString str, WORD lClr, WORD lFont, DWORD nMask = 0, DWORD dwCellMask = 0, DWORD dwExtent = 0);

	BOOL  DrawCell(CDC* pDC, CRect rect, CPoint pos, CDrawTableBase* pTableBase);   // 画单元格
	BOOL  DrawBoard(CDC* pDC, CRect rect);  // 画边框
	void  SetItemText(int nItemID, const char* strText, WORD lClr = 0, WORD lFont = 0, DWORD lMark = 0, DWORD lDelMark = 0); // 设置格式化文本
	int   GetItemSignal(CPoint point, int nCellWidth, int nLeft);  // 鼠标移动 主要是操作单元格内的Item 
	int   GetItemID(CPoint point, int nCellWidth,  int nLeft, int &nDiff);  // 判断当前鼠标所在哪个Item, 并且返回距离左边框的长度 
	CRect GetItemRect(CRect rectCell, int nItemID);   // 给单元格的矩形以及ItemID 得到Item的矩形

	BOOL  GetItemStyle(int nItemID, ItemConstructionStruct& sConstructionStruct); // 给单元小格小标 得到单元小格属性
	BOOL  GetItemStyle(CArray<ItemConstructionStruct>& ayItemStruct); // 获取所有单元小格属性
	
	// 调整两列位置
	void    ReSize(int nItemLeft, int nItemRight, int nCellWidth, int nDiff);
	// 还原Item的状态(如:去除高亮)
	void    ResetItem();
	// 将列从起始位置移动到目标位置
	void    Move(int nBegin, int nEnd);
	// 当需要移动时 获得拖动的矩形
	CRect   GetDrawRect(CPoint pos, int nCellWidth, int nCellHeight);
	// 返回Item的文本内容, 如果-1表示返回全部的
	CString GetItemText(int nItemID = -1);
	// 通过显示内容返回Item下标 
	int     GetItemID(CString str);
	// 复制单元格文本内容
	void    Copy(CDrawCell* pCell);
	// 将单元格内容清空
	void    SetEmpty(); 

	CDrawItemArray m_ayItem;     // 所有Item集合
	WORD		m_wSignal;       // 鼠标当前位置标志
	int			m_nItemID;       // 鼠标当前所在的Item
	
	DWORD		m_dwStyle;		 // 单元格属性
	HsLOGPEN	m_logPen;		 // 画笔结构
};
