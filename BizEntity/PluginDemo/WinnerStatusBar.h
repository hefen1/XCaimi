#pragma once
#include "Transaction.h"
#include "ScrollNews.h"

#include "..\..\Common\HSBizDialog.h"

// CWinnerStatusBar �Ի���

struct HSSize
{
	int cx;
	int cy;
	char name[12];
};

class CWinnerStatusBar : public CHSBizDialog
{
	DECLARE_DYNAMIC(CWinnerStatusBar)

public:
	CWinnerStatusBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWinnerStatusBar();
	virtual void OnNoOperatTimeout();

// �Ի�������
	enum { IDD = IDD_STATUS_BAR };

protected:
	int m_nLayoutCY;
	CScrollNews  m_scrollNews;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInit();
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton2();
};
