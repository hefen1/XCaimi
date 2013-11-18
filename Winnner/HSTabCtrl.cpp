// HSTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "Winner.h"
#include "HSTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHSTabCtrl
IMPLEMENT_DYNAMIC(CHSTabCtrl, CMFCTabCtrl)

CHSTabCtrl::CHSTabCtrl()
{

}

CHSTabCtrl::~CHSTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CHSTabCtrl, CMFCTabCtrl)
//	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// CHSTabCtrl 消息处理程序

void CHSTabCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd* pWnd = this->GetParent();
	if(pWnd)
	{
		TabCtrlNotify tn;
		tn.nId = TN_RBUTTONUP;
		tn.pt = point;
		pWnd->SendMessage(TABCTRL_NOTIFY,0,LPARAM(&tn));
	}
}
