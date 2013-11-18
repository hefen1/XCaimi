// MyListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "WinnerStatus.h"
#include "MyListCtrl.h"


// CMyListCtrl

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{

}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CMyListCtrl 消息处理程序



void CMyListCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(lpDrawItemStruct->CtlType == ODT_HEADER)   
	{   
		HDITEM	hdi;   
		TCHAR	lpBuffer[256];   
		CDC		dc;   
		dc.Attach(lpDrawItemStruct->hDC);   

		hdi.mask = HDI_TEXT;   
		hdi.pszText = lpBuffer;   
		hdi.cchTextMax = 256;   
		CHeaderCtrl* pHdrCtrl=   NULL;   
		pHdrCtrl= GetHeaderCtrl();   

		pHdrCtrl->GetItem(lpDrawItemStruct->itemID, &hdi);   
		dc.FillSolidRect(&lpDrawItemStruct->rcItem,RGB(224,222,223));   
		dc.DrawText(lpBuffer, strlen(lpBuffer),   
			&lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_LEFT);   
		dc.Detach();   
		return;   
	}   

	CListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
