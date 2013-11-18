#pragma once


#include "afxcmn.h"
#include <afxbutton.h>

// CProp 对话框

class CProp : public CPropertyPage
{
	DECLARE_DYNAMIC(CProp)

public:
	CProp();
	virtual ~CProp();

// 对话框数据
	enum { IDD = IDD_PROP1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
//事件
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBkclr();
	afx_msg void OnBnClickedFontclr();
	afx_msg void OnStnClickedFontkind();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL DestroyWindow();
//公共函数
public:
	void OnDrawLabel();
//公共变量
public:
	COLORREF m_bkClr;    //滚动背景颜色
	COLORREF m_fontClr;  //滚动字体颜色
	CFont    m_fontKind; //滚动字体类型
	int      m_nSpeed;   //滚动速度
	int      m_clickWay; //鼠标单击选定方式
private:

	CMFCButton m_BtnFontClr;	
	CMFCButton m_BtnBkClr;
	CSliderCtrl m_ctrlSlider;
	
};

