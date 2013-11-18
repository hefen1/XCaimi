// GradientProgressCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CHSProgressCtrl.h"
/*#include "..\comm\memdc.h"*/
#include "Memdc.h"
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHSProgressCtrl

CHSProgressCtrl::CHSProgressCtrl()
{
	m_fTransmited = 0;

	//Initial Color of show
	m_clrStart    = COLORREF(RGB(255,0,0));
	m_clrEnd      = COLORREF(RGB(0,0,255));
	m_clrBkGround = ::GetSysColor(COLOR_3DFACE);
	m_clrText = COLORREF(RGB(255,255,255));

	//Not show Word
	m_bShowPercent = TRUE;
}

CHSProgressCtrl::~CHSProgressCtrl()
{
}


BEGIN_MESSAGE_MAP(CHSProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CHSProgressCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHSProgressCtrl message handlers

BOOL CHSProgressCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;//CProgressCtrl::OnEraseBkgnd(pDC);
}

void CHSProgressCtrl::DrawGradient(CPaintDC *pDC, const RECT &rectClient, const int &nMaxWidth)
{
	RECT	rectFill;		//显示区域
	float	fStep;			//每一步的幅宽
	CBrush	brush;			//显示的颜色画刷
	CMemDC	memDC(pDC);	

	int		r,g,b;
	float	rStep,gStep,bStep;
	//得到不同颜色并相减，返回颜色之间的最大差值
	r=(GetRValue(m_clrEnd)-GetRValue(m_clrStart));
	g=(GetGValue(m_clrEnd)-GetGValue(m_clrStart));
	b=(GetBValue(m_clrEnd)-GetBValue(m_clrStart));
	//使进程条显示的总数 等于最大的颜色差值
	int		nSteps=max(abs(r),max(abs(g),abs(b)));
	//确定每一颜色填充多大的矩形区域
	fStep=(float)rectClient.right/(float)nSteps;
	//设置每一颜色填充的步数
	rStep=r/(float)nSteps;
	gStep=g/(float)nSteps;
	bStep=b/(float)nSteps;

	r=GetRValue(m_clrStart);
	g=GetGValue(m_clrStart);
	b=GetBValue(m_clrStart);
	//绘制颜色渐变的进程条
	for(int iOnBand=0;iOnBand<nSteps;iOnBand++)
	{
		::SetRect(&rectFill,
			//以下为填充矩形区域的左上角x,y和右下角x,y
			(int)(iOnBand*fStep),
			0,
			(int)((iOnBand+1)*fStep),
			rectClient.bottom+1);

		VERIFY(brush.CreateSolidBrush(RGB(r+rStep*iOnBand,g+gStep*iOnBand,b+bStep*iOnBand)));
		memDC.FillRect(&rectFill,&brush);
		VERIFY(brush.DeleteObject());
		//在结束绘制之前，使用背景色填充乘下的的客户区域
		if(rectFill.right>nMaxWidth)
		{
			::SetRect(&rectFill,rectFill.right,0,rectClient.right,rectClient.bottom);
			VERIFY(brush.CreateSolidBrush(m_clrBkGround));
			memDC.FillRect(&rectFill,&brush);
			VERIFY(brush.DeleteObject());
			return;
		}
	}
}


void CHSProgressCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	if(m_fTransmited <= 0)
	{
		CRect rect;
		GetClientRect(rect);
		CBrush brush;
		brush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
		dc.FillRect(&rect,&brush);
		VERIFY(brush.DeleteObject());
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	float maxWidth((float)m_fTransmited*(float)rectClient.right);

	//绘制
	DrawGradient(&dc,rectClient,(int)maxWidth);

	//显示进程条进度文字
	if(m_bShowPercent)
	{
		CString percent;
		percent.Format("%d%%",(int)(100*(float)m_fTransmited));
		dc.SetTextColor(m_clrText);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(percent,&rectClient,DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	}

	// Do not call CProgressCtrl::OnPaint() for painting messages
}


int CHSProgressCtrl::SetPos(float nPos)
{
	//Set the Position of the Progress
	if (fabs(nPos - m_fTransmited) >= 0 )
	{
		m_fTransmited = nPos;
		this->Invalidate();
	}

	//return 0;
	return (CProgressCtrl::SetPos(nPos));
}
