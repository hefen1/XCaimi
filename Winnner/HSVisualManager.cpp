// HSVisualManager.cpp : 实现文件
//

#include "stdafx.h"
#include <io.h>
#include "Winner.h"
#include "HSVisualManager.h"

#include "..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHSVisualManager
IMPLEMENT_DYNCREATE(CHSVisualManager, CMFCVisualManagerVS2005)
CHSVisualManager::CHSVisualManager()
{
}

CHSVisualManager::~CHSVisualManager()
{
}


// CHSVisualManager 成员函数

void CHSVisualManager::OnFillBarBackground(CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea /*= FALSE*/)
{
	if(!pBar->IsKindOf(RUNTIME_CLASS(CMFCMenuBar)))
	{
		CMFCVisualManagerVS2005::OnFillBarBackground(pDC,pBar,rectClient,rectClip,bNCArea);
		return;
	}

	CString strBmpPath = CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH) +  _T("menu_bar_background.bmp");
	if (_taccess(strBmpPath,0) == -1)
	{
		CMFCVisualManagerVS2005::OnFillBarBackground(pDC,pBar,rectClient,rectClip,bNCArea);
		return;
	}

	CBitmap bmp;
	if(bmp.Attach(LoadImage(GetModuleHandle(NULL), strBmpPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) == TRUE )
	{
		CDC sdc;
		BITMAP bm;
		sdc.CreateCompatibleDC(pDC);
		CBitmap *pOld = sdc.SelectObject(&bmp);
		bmp.GetBitmap(&bm);
		pDC->StretchBlt(rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), &sdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		sdc.SelectObject(pOld);
	}
	else
	{
		CMFCVisualManagerVS2005::OnFillBarBackground(pDC,pBar,rectClient,rectClip,bNCArea);
	}
}
