/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDrawItem.h
*	文件标识：
*	摘	  要：  Item类
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
#include "DiagramFoundationDef.h"
#include "DrawTableStruct.h"
// #include <hash_map>
// #include <list>
// #include <map>

#define ITEM_TEXT_LEFT         0x0001  // 左对齐
#define ITEM_TEXT_CENTER       0x0002  // 居中
#define ITEM_TEXT_RIGHT        0x0004  // 右对齐
#define ITEM_TEXT_SELECT       0x0008  // 文本内容可以被选中

#define ITEM_TEXT_HIGHLIGHT    0x0010  // 文本高亮显示
#define ITEM_TEXT_NOTRIM       0x0020  // 保留字符串左右空格 默认情况下在画的时候会去掉左右两边空格
#define ITEM_RESIZABLE         0x0040  // 可以拖动左边框改变大小
#define ITEM_MOVABLE           0x0080  // 可以选中拖动位置

// 箭头只能在文本的右边出现
#define ITEM_ARROW_NEED        0x0100  // 需要显示箭头 但是还不确定显示哪个
#define ITEM_ARROW_UP          0x0200  // 显示向上箭头
#define ITEM_ARROW_DOWN        0x0400  // 显示向下箭头
#define ITEM_ARROW_LEFT        0x0800  // 箭头显示在左边(默认显示在右边)

#define ITEM_ARROW_WIDTH           16  // 箭头宽度
#define ITEM_MIN_WIDTH             30  // Item最小宽度
#define ITEM_LEFT_MARGIN            5  // 左边预留空白
#define ITEM_RIGHT_MARGIN           5  // 右边预留空白
//////////////////////////////////////////////////////////////////////////
// CDrawCellArray
class CDrawItem;
class CDrawItemArray : public CPtrArray
{
public:
	CDrawItem*& operator[](int nIndex) {return (CDrawItem*&)ElementAt(nIndex); }
	//free all elements
	void DeleteAll();
	~CDrawItemArray() { DeleteAll(); }
};
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// 管理CDrawItem内存
//typedef stdext::hash_map<int, CDrawItem*>  mapItemMem;
// class CDrawItemMemControl
// { 
// public:
// 	// 申请内存
// 	CDrawItemMemControl();
// 	~CDrawItemMemControl();
// 	CDrawItem* AllocMem(ItemConstructionStruct* pConstructionStruct);   
// 	CDrawItem* AllocMem(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent);
// 	void FreeMem(CDrawItem* &pItem);    // 释放内存
// 
// private:
// 	CMapWordToPtr         m_mapAlloc;   // 已经分配的内存
// //	mapItemMem            m_mapAlloc;
// 	std::list<CDrawItem*> m_lstIdle;    // 释放出的空闲内存
// };
//////////////////////////////////////////////////////////////////////////
class CDrawItem
{
public:
	CDrawItem(ItemConstructionStruct* pConstructionStruct = NULL);
	CDrawItem(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent);
	~CDrawItem(void);

	void  ReCreate(ItemConstructionStruct* pConstructionStruct);
	void  ReCreate(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent);

	void  SetText(CString str, WORD wClr = 0, WORD wFont = 0, DWORD dwMask = 0, DWORD dwDelMask = 0 );
	void  SetText(const char* strText, WORD wClr = 0, WORD wFont = 0, DWORD wMask = 0, DWORD dwDelMask = 0);
	BOOL  DrawItem(CDC* pDC, CRect rect);

	void  Copy(CDrawItem* pItem); // 复制文本内容以及属性  
	void  SetEmpty(); // 将单元小格内容清空

	//////////////////////////////////////////////////////////////////////////
	// 
// 	static CDrawItem*  Create(ItemConstructionStruct* pConstructionStruct = NULL);
// 	static CDrawItem*  Create(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent);
// 	static void  Free(CDrawItem* pItem);
	//////////////////////////////////////////////////////////////////////////
//private:
	char        m_str[64];  // 文本内容
	WORD	    m_wColor;   // 前景色
	WORD	    m_wFont;    // 字体
	DWORD	    m_dwStyle;  // 属性值(如: 对齐、显示三角形、选中、左右拖动改变大小、移动等)
	CRect	    m_rectDraw; // 实际绘制的区域，在DrawItem函数执行时计算出来
	double     	m_dWidth;   // 所占百分比
	DWORD       m_dwExtent; // 留作扩展用 
// private:
// 	static CDrawItemMemControl m_memControl;  // 内存控制对象
};
