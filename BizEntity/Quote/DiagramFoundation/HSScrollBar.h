/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CHsScrollBar.h
*	文件标识：
*	摘	  要：  滚动条
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-01-05
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

#define HSSB_VERT         0X0001 // 滚动条垂直
#define HSSB_HORZ         0X0002 // 滚动条水平

#define HSSB_LEFT         0X0010 // 滚动条左边 针对滚动条垂直
#define HSSB_RIGHT        0X0020 // 滚动条右边
#define HSSB_UP           0X0010 // 滚动条上边 针对滚动条水平
#define HSSB_DOWN         0X0020 // 滚动条下边

// 仅当m_bBlockNeedBack为True才会执行
#define HSSB_BLOCK_ALWAYSTOP     0x0100  // 滑块默认位置在顶部
#define HSSB_BLOCK_ALWAYBOTTOM   0x0200  // 滑块默认位置在底部
#define HSSB_BLOCK_ALWAYLEFT     0x0100  // 滑块默认位置在左部
#define HSSB_BLOCK_ALWAYRIGHT    0x0200  // 滑块默认位置在右部

#define HSSB_SIZE          10    // 滚动条宽度(当滚动条垂直) 滚动条高度(当滚动条水平)
#define HSSB_BLOCK_MINSIZE 4     // 滑块最小的尺寸

interface IScrollNotify 
{
	// 滚动操作
	//	virtual void  OnScroll(int nDiff, BOOL bIsVert) = 0; 滚动功能并入到SetPosition函数中
	// 设置位置操作
	virtual BOOL  SetPosition(int nPosition, BOOL bIsVert, BOOL bIsRedraw = TRUE) = 0;
	// 刷新操作
	virtual void  ScrollInvalidate(CRect* pRect, BOOL bErase = TRUE) = 0;
	// 获取句柄操作
	virtual HWND  GetHandle() = 0;
};


class HS_EXT_CLASS CHSScrollBar
{
public:
	CHSScrollBar(DWORD Style); 
	~CHSScrollBar(void);
	DWORD   GetStyle() {return m_nStyle;}  // 获取滚动条风格
	void    SetStyle(DWORD nStyle){m_nStyle = nStyle;} // 设置滚动条风格

	BOOL	ReadFile(); // 读配置文件
	void    Reset();    // 重置一次滚动距离
    // 根据容器的大小(rc)以及自身的属性(m_nStyle)，计算出自己的位置返回容器剩下的区域
	CRect   SetRect(CRect rc);
	CRect   SetRect(CRect rc, int nRange);
	CRect   GetRect();

	// 外界可以设置滚动条的大小 当滚动垂直则是滚动条的宽度 当滚动条水平则是滚动条的高度 没有给则为默认值
	void    SetSize(int nSize);  
	
	void    SetAutoSize(BOOL bSize) {m_bAutoSize = bSize;}
	BOOL    GetAutoSize() {return m_bAutoSize;}

	BOOL    SetRange(int nRange);
	int     GetRange(){return m_nRange;}

	void    SetBlockToDefaultPosition();
	// 绘制滚动条
	void	Draw(CDC* pDC);
	void	DrawBorder(CDC* pDC);  // 画边框
	void	DrawArrow(CDC* pDC);   // 画上下箭头或者左右箭头
	void    DrawBlock(CDC* pDC);   // 画滑块
	
	BOOL	MouseDown(UINT nFlags, CPoint point);  // 鼠标事件
	void    SetScrollNotify(IScrollNotify* pNotify) {m_iNotify = pNotify;}

	void    OnScroll(int nMoveDiff);   // 供外部调用 触发滚动条滚动
	void    SetTop();                  // 将滑块置顶
	void    SetBottom();               // 将滑块置底
protected:
	// 计算滑块位置 不能小于滑块最小尺寸 会调整一次移动滚动的次数
	// 返回滑块的大小(水平：高度；垂直：宽度)
	int     CalcBlock(int nMaxSize); 
	BOOL    SetBlockRect();        // 将滑块移动当前位置(根据m_nPosition的值来绘制滑块)

	enum HitPlace{NONE = 0, Up = 1, Down, Block, UpBlank, DownBlank};  // 鼠标点击选中的地方分别是范围之外，上箭头，下箭头，滑块，滑块上面空白，滑块下面空白
    void     MouseEvent(HitPlace eHit, CPoint point); // 鼠标处理事件
	void     Scrolling(int nDiff);   // 滚动事件 正数表示(向上，向左) 负数表示(向下，向右)
	HitPlace GetHitPlace(CPoint pt); // 判断鼠标点击的位置
private:
	DWORD          m_nStyle;        // 滚动条风格

	int            m_nRange;        // 外部图元的移动范围
	int            m_nScrollRange;  // 滚动条滑块移动范围
	int            m_nPosition;     // 当前滑块位置
	int            m_nPrePosition;  // 之前的滑块位置
	int            m_nMoveDistance; // 滑块一次移动的像素(默认为3个像素,因为鼠标移动事件每3个像素系统捕捉一次)
	int            m_nScrollDiff;   // 滑块一次移动滚动的次数
	HsLOGPEN	   m_logpen;        // 画笔

	CRect          m_rect;          // 整个滚动条区域
	CRect          m_rcUpArrow;     // 滚动条上箭头(左箭头)区域
    CRect          m_rcDownArrow;   // 滚动条下箭头(右箭头)区域
	CRect          m_rcBlock;       // 滚动条滑块区域
	int            m_nSize;         // 滚动条大小
	BOOL           m_bAutoSize;     // 是否需要自动调整大小

	IHsColor*      m_iColor;        // 颜色读写接口 
	COLORREF       m_clrBK;         // 背景色
	COLORREF       m_clrBlock;      // 滑块颜色
	COLORREF       m_clrArrow;      // 箭头颜色

	double         m_dUserDiff;     // 如果外面一次传入的移动大小不足以使滚动条移动一格 则将该小数记录下来
	IScrollNotify* m_iNotify;       // 回调指针  
};
