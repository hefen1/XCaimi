/*******************************************************************************
 * Copyright (c)2003, 杭州恒生信息技术有限公司
 * All rights reserved.
 *
 * 文件名称：HSBaseDC.h
 * 文件标识：绘图设备包装类
 * 摘    要：绘图设备包装类,主意作用是设置打印机颜色
 *
 * 当前版本：2.0
 * 作    者：俞礼石
 * 完成日期：2003年05月09日 星期五 
 *
 * 取代版本：
 * 原 作 者：俞礼石
 * 完成日期：2003年05月09日 星期五 
 * 备	 注：
 *******************************************************************************/
// HSBaseDC.h: interface for the CHSBaseDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YLSBASEDC_H__81E4965B_0145_4D5B_90D1_277D327CDB99__INCLUDED_)
#define AFX_YLSBASEDC_H__81E4965B_0145_4D5B_90D1_277D327CDB99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHSBaseDC : public CDC
{
public:
	CHSBaseDC();
    CHSBaseDC(CDC* pDC);
    ~CHSBaseDC();

public:
    CHSBaseDC* operator->() { return this; }
    operator CHSBaseDC*()   { return this; }

protected:
	static COLORREF m_clPrintBKColor;	//打印背景颜色
	static COLORREF m_clPrintTextColor;	//打印文本颜色
	static CPen     m_pPrintPen;		//打印画笔
	static CBrush	m_pPrintBrush;		//打印画刷

protected:
	char	m_bSelf;		//绘图设备指针是否为内部所创建
	/*************************************************************
	* 吕连新加入
	* 时间：  2003年9月12日
	* 描述：  为了支持彩印
	*************************************************************/
	// 是否支持彩色打印
	BOOL	m_bSupportMulticolor; 
	/**********吕连新加入结束*************/

public:
	//初始化操作,设置打印画笔与画刷
	static BOOL Init();

public:
	//设置文本颜色与背景颜色
	virtual COLORREF SetTextColor(COLORREF crColor);
	virtual COLORREF SetBkColor(COLORREF crColor);

public:
	//设备选择操作
	virtual CGdiObject* SelectStockObject(int nIndex);
	CPen*			    SelectObject(CPen* pPen);
	CBrush*			    SelectObject(CBrush* pBrush);
	virtual CFont*	    SelectObject(CFont* pFont);
	CBitmap*		    SelectObject(CBitmap* pBitmap);
	int					SelectObject(CRgn* pRgn);
	CGdiObject*			SelectObject(CGdiObject* pObject);
	HGDIOBJ				SelectObject(HGDIOBJ);
	//填充操作
	void FillRect(LPCRECT lpRect, CBrush* pBrush);
	void FillSolidRect(LPCRECT lpRect, COLORREF clr);
	void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
	BOOL FloodFill(int x, int y, COLORREF crColor);
	BOOL FillRgn(CRgn* pRgn, CBrush* pBrush);


	/*************************************************************
	* 吕连新加入
	* 时间：  2003年9月12日
	* 描述：  为了支持彩印
	*************************************************************/
	// 是否为黑白打印
	BOOL IsMonochromePrint() { return m_bPrinting && !m_bSupportMulticolor; }
	/**********吕连新加入结束*************/
};

#endif // !defined(AFX_YLSBASEDC_H__81E4965B_0145_4D5B_90D1_277D327CDB99__INCLUDED_)
