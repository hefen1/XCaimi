#pragma once

#include "ScrollNewsBar.h"

// CScrollFrame 对话框

class CScrollFrame : public CDialog
{
	DECLARE_DYNAMIC(CScrollFrame)

public:
	CScrollFrame(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScrollFrame();

// 对话框数据
	enum { IDD = IDD_SCROLLFRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
//事件
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
//公用函数
public:
	CRect getPosition();
//属性
private:
	//CRect m_rectScrollFloat;       //当跑马灯不是固定在状态栏上 记录在窗口的位置，大小
public:
	CScrollNewsBar* m_pScrollWnd;  //记录跑马灯指针


};

