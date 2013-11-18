// ScrollNewsBar.cpp : 实现文件
//

#include "stdafx.h"
#include "WinnerStatus.h"
#include "ScrollNewsBar.h"
#include "StatuBar.h"
#include "HSMacro.h"
#include "PropSheet.h"
#include "Memdc.h"
#include "QuoteLangMngInterFace.h"
#include "..\..\Resource\QuoteLang\Resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CScrollNewsBar, CDialog)

CScrollNewsBar::CScrollNewsBar(CWnd* pParent /*=NULL*/)
	: CDialog(CScrollNewsBar::IDD, pParent)
{
	m_pStatuBar = NULL;              // 父窗口指针初始化为空
	m_bIsInitRect = FALSE;

	m_pScrollCfg = NULL;
	m_pColorCfg  = NULL;
	m_pFontCfg   = NULL;
          
	m_clrBk = RGB(0,0,0);          // 滚动背景颜色
	m_clrText = RGB(0,0,0);        // 滚动字体颜色
	m_pFontText = NULL;            // 文本字体
}

CScrollNewsBar::~CScrollNewsBar()
{
	if (m_lstImage.GetSafeHandle())
	{
		m_lstImage.DeleteImageList();
	}
}


BOOL CScrollNewsBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pColorCfg = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	if (m_pColorCfg == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Status_Log_Name,"WinnerStatus.Dll加载IHsColor失败！");
		return FALSE;
	}
	m_pFontCfg = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
	if (m_pFontCfg == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL, Status_Log_Name, "WinnerStatues.DLL加载IHsFont失败! ");
		return FALSE;
	}
	m_pScrollCfg = (IHsScrollNews*)CWinnerApplication::GetObject(OBJ_SCROLLNEW);
	if (m_pScrollCfg == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL, Status_Log_Name, "WinnerStatues.DLL加载IHsScrollNews失败! ");
		return FALSE;
	}

	if(FALSE == ReadCfg())
		return FALSE;

	m_nTimeID = this->SetTimer((long)this,m_nSpeed,NULL);

	menu.LoadMenu(IDR_POPUP_SCROLL);
	if(bScrollFixed)
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_FLOAT,MF_UNCHECKED);
	}
	else
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_FLOAT,MF_CHECKED);
	}
	if(bScrollMultiLine)
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_MULTILINE,MF_CHECKED);
	}
	else
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_MULTILINE,MF_UNCHECKED);
	}

	//////////////////////////////////////////////////////////////////////////
	// 加载图片资源
	IQuoteLang* pQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
	HANDLE handle = pQuoteLang->LoadHImage(IDB_SCROLL, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	if (handle)
	{
		CBitmap bit;
		bit.Attach((HBITMAP)handle);
		m_lstImage.Create(16, 16, ILC_COLOR24 | ILC_MASK, 3, 3);
		BOOL bRet = m_lstImage.Add(&bit, RGB(255,0,255));
		bit.DeleteObject();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CScrollNewsBar::InitRect(CDC* pDC)
{
	if(m_bIsInitRect)
		return;
	
	CFont* oldFont = pDC->SelectObject(m_pFontText);

	CSize blankSize = pDC->GetTextExtent("  ");  //位置偏移量
	int OffPos = blankSize.cx;

	CString title;
	CString content;
	CSize sz;
	CRect showRect;

	m_nShowIndex = 0; 
	m_showDataCRectArray.RemoveAll();
	if(FALSE == bScrollMultiLine)
	{//如果是单行 则向左或者向右

		m_nShowIndex = 0; //显示内容从第一条开始
		int beginPos = 0;
		int i = 0;
		while(beginPos < m_rectText.right)
		{
			title = m_sTitleArray.GetAt(i);
			content = m_sContentArray.GetAt(i);
			sz = pDC->GetTextExtent(title+content);//获得当前窗体中字体的大小
			sz.cx += SCROLL_BITMAP_WIDTH; //加入图片的大小
			sz.cx += OffPos;//标题与内容之间留一些空白地方

			showRect = m_rectText;
			showRect.left = beginPos; //起始位置
			showRect.right = beginPos + sz.cx;//结束位置
			m_showDataCRectArray.Add(showRect);//将填充位置记录

			beginPos += showRect.Width() + OffPos*4;//下一个起始位置为当前起始位置+显示内容长度+留白
			i = (i+1)%m_sTitleArray.GetCount();//循环取下一条记录
		}
	}
	else
	{//如果是多行 则向上或者向下

		m_nShowIndex = 0; //显示内容从第一条开始
		int beginPos = 0;
		int i = 0;
		while(beginPos < m_rectText.bottom)
		{
			title = m_sTitleArray.GetAt(i);
			content = m_sContentArray.GetAt(i);
			sz = pDC->GetTextExtent(title+content);
			sz.cx += SCROLL_BITMAP_WIDTH; //加入图片的大小

			showRect.left   = 0; 
			showRect.right  = showRect.left + sz.cx + OffPos;
			showRect.top    = beginPos;
			showRect.bottom = showRect.top + sz.cy;
			m_showDataCRectArray.Add(showRect);  //将填充位置记录
			beginPos += sz.cy + OffPos;
			i = (i+1)%m_sTitleArray.GetCount();//循环取下一条记录
		}	
	}

	pDC->SelectObject(oldFont);

}

BOOL CScrollNewsBar::ReadCfg()
{

	if (m_pColorCfg == NULL || ::IsBadReadPtr(m_pColorCfg, 1))
	{
		return FALSE;
	}
	if (m_pFontCfg == NULL || ::IsBadReadPtr(m_pFontCfg, 1))
	{
		return FALSE;
	}
	if (m_pScrollCfg == NULL || ::IsBadReadPtr(m_pScrollCfg, 1))
	{
		return FALSE;
	}

	//从配置文件中读取
	m_nShowScrollAlone = m_pScrollCfg->GetShowScrollAlone(); //跑马灯显示的方式
	bScrollFixed = m_pScrollCfg->IsScrollFixed();            //跑马灯是否固定在状态栏上
	bScrollMultiLine = m_pScrollCfg->IsScrollMultiLine();    //跑马灯是否多行显示

	m_nIsUpDownRate   = m_pScrollCfg->GetUpDownRate();
	m_nIsTotal        = m_pScrollCfg->GetTotal();
	m_nIsShowNewPirce = m_pScrollCfg->GetShowNewPirce();
	m_nIsUpDown       = m_pScrollCfg->GetUpDown();

	//设置滚动风格
	m_nHorizonScrollWay = m_pScrollCfg->GetHorizonScrollWay();
	m_nVerticalScrollWay = m_pScrollCfg->GetVerticalScrollWay();

	m_nClickWay = m_pScrollCfg->GetClickWay(); //设置选中方式
	m_nSpeed = m_pScrollCfg->GetScrollSpeed();//速度初始化

	CStringArray ayContent;
	m_pScrollCfg->GetScrollText(ayContent);
	for(int i=0; i<ayContent.GetCount(); i++)
	{
		CString strTemp = ayContent.GetAt(i);
		m_sTitleArray.Add(strTemp);
		i++;

		strTemp = ayContent.GetAt(i);
		m_sContentArray.Add(strTemp);
		i++;

		strTemp = ayContent.GetAt(i);
		m_sUrlArray.Add(strTemp);
	}

	//显示颜色初始化
	m_clrBk = m_pColorCfg->GetScrollBKColor();
	m_clrText = m_pColorCfg->GetScrollTestColor();
	
	//显示字体初始化
	m_pFontText = m_pFontCfg->GetScrollFontProt();

	return TRUE;
}

void CScrollNewsBar::WriteCfg()
{
	
	if (m_pColorCfg == NULL || ::IsBadReadPtr(m_pColorCfg, 1))
	{
		return;
	}
	if (m_pFontCfg == NULL || ::IsBadReadPtr(m_pFontCfg, 1))
	{
		return;
	}
	if (m_pScrollCfg == NULL || ::IsBadReadPtr(m_pScrollCfg, 1))
	{
		return;
	}
	
	BOOL bRet = FALSE;
	if (bScrollFixed != m_pScrollCfg->IsScrollFixed())
	{//保存跑马灯固定开关
		bRet = m_pScrollCfg->SetScrollFixed(bScrollFixed);
	}
	
	if (bScrollMultiLine != m_pScrollCfg->IsScrollMultiLine())
	{//保存跑马灯多行开关
		bRet = m_pScrollCfg->SetScrollMultiLine(bScrollMultiLine);
	}
	
	if (m_nSpeed != m_pScrollCfg->GetScrollSpeed())
	{//保存行情滚动速度
		bRet = m_pScrollCfg->SetScrollSpeed(m_nSpeed);
	}
	
	if (m_nHorizonScrollWay != m_pScrollCfg->GetHorizonScrollWay())
	{//保存行情水平滚动方向
		bRet = m_pScrollCfg->SetHorizonScrollWay(m_nHorizonScrollWay);
	}

	if (m_nVerticalScrollWay != m_pScrollCfg->GetVerticalScrollWay())
	{//保存行情垂直滚动方向
		bRet = m_pScrollCfg->SetVerticalScrollWay(m_nVerticalScrollWay);
	}
	
	if (m_nClickWay != m_pScrollCfg->GetClickWay())
	{//保存行情显示方式
		bRet = m_pScrollCfg->SetClickWay(m_nClickWay);
	}
     
	if (bRet)
	{
		m_pScrollCfg->SaveToFile();
	}
	

	m_pColorCfg->SaveToFile();
	m_pFontCfg->SaveToFile();
	

}
void CScrollNewsBar::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	SetSize();
}

void CScrollNewsBar::SetSize()
{
	CRect rect;
	GetClientRect(&rect);
	
	if(bScrollFixed)
	{//如果是固定在状态栏上 则跑马灯没有标题
		m_rectCaption.SetRectEmpty();

		//咨询区域
		m_rectText       = rect;
		m_rectText.right = rect.right - SCROLL_FLOAT_HEIGHT;

		//工具栏区域
		m_rectTool      = rect;
		m_rectTool.left = m_rectText.right;
	}
	else
	{ 
		if(bScrollMultiLine)
		{//多行显示
			//标题栏区域
			m_rectCaption        = rect;
			m_rectCaption.bottom = m_rectCaption.top + SCROLL_FLOAT_HEIGHT;
			m_rectCaption.right  = rect.right - SCROLL_FLOAT_HEIGHT;

			//咨询区域
			m_rectText     = rect;
			m_rectText.top = m_rectCaption.bottom;

			//工具栏区域
			m_rectTool        = rect;
			m_rectTool.bottom = m_rectTool.top + SCROLL_FLOAT_HEIGHT;
			m_rectTool.left   = m_rectCaption.right; 
		}
		else
		{//单行显示
			//标题栏区域
			m_rectCaption       = rect;
			m_rectCaption.right = rect.left + SCROLL_FLOAT_HEIGHT;

			//咨询区域
			m_rectText       = rect;
			m_rectText.left  = m_rectCaption.right;
			m_rectText.right = m_rectTool.left;

			//工具栏区域
			m_rectTool      = rect;
			m_rectTool.left = rect.right - SCROLL_FLOAT_HEIGHT;
		}
	}
}

void CScrollNewsBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScrollNewsBar, CDialog)
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FLOAT, &CScrollNewsBar::OnFloat)
	ON_COMMAND(ID_MULTILINE, &CScrollNewsBar::OnMultiline)
	ON_COMMAND(ID_SETTING, &CScrollNewsBar::OnSetting)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CScrollNewsBar 消息处理程序
void CScrollNewsBar::OnPaint()
{
	CPaintDC dc(this);
	CMemdc MemDC(&dc);

	DrawMyText(&MemDC);
	
}

void CScrollNewsBar::DrawMyCaption( CDC* pDC )
{
	if(!bScrollFixed)
	{//只有当跑马灯浮动显示的时候才会有标题栏
		if(bScrollMultiLine)
		{
			
			CStatuBar* pBar = (CStatuBar*)m_pStatuBar;
			if (NULL == pBar || ::IsBadReadPtr(pBar, 1))
				return;

			CFont* oldFont = pDC->SelectObject(pBar->m_pStatusFont);
			CSize oneWordSize = pDC->GetTextExtent("A");
			oneWordSize.cx /= 2;

			pDC->FillSolidRect(m_rectCaption,STATUE_BKCOLOR);
			CPen GrayPen(PS_SOLID,1,RGB(85,85,85));
			CPen *oldPen = NULL;
			oldPen = pDC->SelectObject(&GrayPen);
			pDC->MoveTo(0,                  m_rectCaption.bottom);
			pDC->LineTo(m_rectTool.right,   m_rectCaption.bottom);
			pDC->SelectObject(oldPen);

			CPen BlackPen(PS_SOLID,1,RGB(0,0,0));
			oldPen = pDC->SelectObject(&BlackPen);
			pDC->MoveTo(0,                  m_rectCaption.bottom+1);
			pDC->LineTo(m_rectTool.right,   m_rectCaption.bottom+1);
			pDC->SelectObject(oldPen);

			//显示指数
			COLORREF oldColor;
			COLORREF crl;
			CRect rect = m_rectCaption;
			rect.right = rect.left;

			for(int i=0; i<INDEX_MAX_COUNT; i++)
			{
				if(pBar->m_indexString[i].trend == -2)
					continue;

				//股票名称
				CSize sz = pDC->GetTextExtent(pBar->m_indexString[i].name);
				rect.left  = rect.right;
				rect.right = rect.left + sz.cx;
				crl = pBar->m_clrCodeName;
				oldColor = pDC->SetTextColor(pBar->m_clrCodeName);
				pDC->DrawText(pBar->m_indexString[i].name, rect, DT_VCENTER|DT_SINGLELINE);

				//选择颜色
				if(pBar->m_indexString[i].trend == -1) //跌
				{
					pDC->SetTextColor(pBar->m_clrDown);
				}
				else if(pBar->m_indexString[i].trend == 0)//平
				{
					pDC->SetTextColor(pBar->m_clrEqual);
				}
				else if(pBar->m_indexString[i].trend == 1)//涨
				{
					pDC->SetTextColor(pBar->m_clrUp);
				}

				//现价
				if(m_nIsShowNewPirce == 1)
				{
					sz = pDC->GetTextExtent(pBar->m_indexString[i].nowPrice);
					rect.left  = rect.right;
					rect.right = rect.left + sz.cx;
					pDC->DrawText(pBar->m_indexString[i].nowPrice,rect,DT_VCENTER|DT_SINGLELINE);
				}

				//涨跌之前的符号
				if(m_nIsUpDown == 1)
				{
					rect.left = rect.right + oneWordSize.cx;
					rect.right = rect.left  + SCROLL_FLOAT_HEIGHT/4;
					CRect arrowRect = rect;
					arrowRect.top += arrowRect.Height()/8;
					if(pBar->m_indexString[i].trend == -1) //跌
					{
						CGeneralHelper::DrawArrow(pDC, arrowRect, pBar->m_clrDown, FALSE);
					}
					else if(pBar->m_indexString[i].trend == 0)//平
					{

					}
					else if(pBar->m_indexString[i].trend == 1)//涨
					{
						CGeneralHelper::DrawArrow(pDC, arrowRect, pBar->m_clrUp, TRUE);
					}
		
					//涨跌
					rect.right += oneWordSize.cx;
					sz = pDC->GetTextExtent(pBar->m_indexString[i].ricePrice);
					rect.left  = rect.right;
					rect.right = rect.left + sz.cx;
					pDC->DrawText(pBar->m_indexString[i].ricePrice, rect, DT_VCENTER|DT_SINGLELINE);
				}


				//涨幅
				if(m_nIsUpDown == 1)
				{
					sz = pDC->GetTextExtent(pBar->m_indexString[i].riceRatio);
					rect.left  = rect.right + oneWordSize.cx;
					rect.right = rect.left + sz.cx;
					pDC->DrawText(pBar->m_indexString[i].riceRatio, rect, DT_VCENTER|DT_SINGLELINE);
				}

				
				//总金额
				if(m_nIsTotal == 1)
				{
					sz = pDC->GetTextExtent(pBar->m_indexString[i].totalMoney);
					rect.left  = rect.right;
					rect.right = rect.left + sz.cx;
					pDC->SetTextColor(crl);
					pDC->DrawText(pBar->m_indexString[i].totalMoney, rect, DT_VCENTER|DT_SINGLELINE);
				}


				rect.right += oneWordSize.cx;
				oldPen = pDC->SelectObject(&GrayPen);
				pDC->MoveTo(rect.right,rect.top+3);
				pDC->LineTo(rect.right,rect.bottom-3);
				pDC->SelectObject(oldPen);

				pDC->SelectObject(&BlackPen);
				pDC->MoveTo(rect.right+1,rect.top+3);
				pDC->LineTo(rect.right+1,rect.bottom-3);
				pDC->SelectObject(oldPen);

				rect.right += oneWordSize.cx;
				if(rect.right > m_rectCaption.right)
					break;
				pDC->SetTextColor(oldColor);

			}
			
			pDC->SelectObject(oldFont);
			DrawMyTool(pDC);
		}
		else
		{// 绘制图片
			m_lstImage.Draw(pDC, BMP_CAPTION_INDEX, CPoint(m_rectCaption.left, m_rectCaption.top), ILD_TRANSPARENT);
		}		
	}
}

void CScrollNewsBar::DrawMyTool( CDC* pDC )
{
	pDC->FillSolidRect(&m_rectTool, m_clrBk);
	m_lstImage.Draw(pDC, BMP_TOOL_INDEX, CPoint(m_rectTool.left, m_rectTool.top), ILD_TRANSPARENT);
}
void CScrollNewsBar::DrawMyText( CDC* pDC )
{
	CalculateRect(pDC);

	pDC->FillSolidRect(m_rectText, m_clrBk);

	COLORREF oldClr = pDC->SetTextColor(m_clrText);
	CFont* oldFont = pDC->SelectObject(m_pFontText);

	int j = m_nShowIndex;
	for(int i=0; i<m_showDataCRectArray.GetCount(); i++)
	{
		CRect rect = m_showDataCRectArray.GetAt(i);
		m_lstImage.Draw(pDC, BMP_TEXT_INDEX, CPoint(rect.left, rect.top), ILD_TRANSPARENT);
		
		rect.left += SCROLL_FLOAT_HEIGHT;
		pDC->DrawText(m_sTitleArray.GetAt(j) + "  " + m_sContentArray.GetAt(j), rect, DT_VCENTER | DT_SINGLELINE);
		j = (j+1)%m_sTitleArray.GetCount();
	}
	pDC->SetTextColor(oldClr);
	pDC->SelectObject(oldFont);

	//需要把标题栏重绘一下
	DrawMyCaption(pDC);
	DrawMyTool(pDC);
}


BOOL CScrollNewsBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CScrollNewsBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (TRUE == HitCaption(point))
	{
		lastPoint = point; //记录初始位置
		((CScrollFrame*)GetParent())->PostMessage(WM_NCLBUTTONDOWN,HTCAPTION, MAKELPARAM(point.x,point.y));	
	}
	else if(TRUE == HitTool(point))
	{
		CMenu* pPopup = menu.GetSubMenu(0);

		CPoint clientPoint(point.x,point.y); 
		ClientToScreen(&clientPoint);
		pPopup->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON, clientPoint.x,clientPoint.y,this);
	}
	else
	{
		MouseDown(nFlags,point);
	}

}

int CScrollNewsBar::MouseDown( UINT nFlags, CPoint point )
{//捕捉鼠标事件

	if(m_nTimeID != 0)
	{
		KillTimer(m_nTimeID);
		m_nTimeID = 0;
	}

	CPoint  pt;
	BOOL    bIsScrolling = FALSE;
	BOOL    bIsClick  = FALSE;
	BOOL    bIsDClick = FALSE;
	DWORD start = 0;
	{
		SetCapture();	

		int     xDiff, yDiff;
		xDiff = yDiff = 0;
				
		for (;;) 
		{
			
			MSG msg;
			VERIFY(::GetMessage(&msg, NULL, 0, 0));
			
			//	if (GetCapture() != this)
			//		break;
		
			switch (msg.message) 
			{
			case WM_MOUSEMOVE:
				{
					if(FALSE == bIsClick)
					{
						bIsScrolling = TRUE;
						pt.x = (int)(short)LOWORD(msg.lParam);
						pt.y = (int)(short)HIWORD(msg.lParam);

						xDiff = pt.x - point.x;
						yDiff = pt.y - point.y;

						if(msg.wParam == MK_LBUTTON)
						{
							point.x = pt.x;
							point.y = pt.y;

							if(bScrollMultiLine)
							{
								for(int i=0; i<m_showDataCRectArray.GetCount();i++)
								{
									m_showDataCRectArray.GetAt(i).top += yDiff;
									m_showDataCRectArray.GetAt(i).bottom += yDiff;
								}

								m_nVerticalScrollWay = (yDiff<0)?ScrollUp:ScrollDown;

							}
							else
							{
								for(int i=0; i<m_showDataCRectArray.GetCount();i++)
								{
									m_showDataCRectArray.GetAt(i).left += xDiff;
									m_showDataCRectArray.GetAt(i).right += xDiff;
								}
								m_nHorizonScrollWay = (xDiff<0)?ScrollLeft:ScrollRight;
							}	

							//Invalidate(FALSE);
							InvalidateRect(m_rectText,FALSE);
							UpdateWindow();
						}
						else
						{
							bIsScrolling = FALSE;
						}
					}	
				}
				break;
			case WM_KEYDOWN:
				if (msg.wParam == VK_ESCAPE)
				{
					goto ExitLoop;
				}
			case WM_LBUTTONDBLCLK:
				{
					
					bIsDClick = TRUE;
					goto ExitLoop; 
				}
				break;
			case WM_LBUTTONDOWN:
				{

				}
				break;
			case WM_LBUTTONUP:
				{
					bIsClick = TRUE;
					start = ::GetTickCount();	
						
				}
				break;
			case WM_RBUTTONUP:
				goto ExitLoop;
			default:
				DispatchMessage(&msg);
				break;
			}
			
			DWORD end(0);
			if(start != 0)
				end = ::GetTickCount();
			if((end-start > 3) && (FALSE==bIsScrolling))
				goto ExitLoop;
			if((TRUE==bIsClick) && (TRUE==bIsScrolling))
				goto ExitLoop;

		}
	
	}

ExitLoop:
	
		ReleaseCapture();
		if( (FALSE==bIsScrolling) && ((0==m_nClickWay) || (1==m_nClickWay&&TRUE==bIsDClick)))
		{//如果没有滚动过
			
			for(int i=0; i<m_showDataCRectArray.GetCount(); i++)
			{
				if(m_showDataCRectArray.GetAt(i).PtInRect(point))
				{
					int messageIndex = (m_nShowIndex+i)%m_sUrlArray.GetCount();
					CString strUrl = m_sUrlArray.GetAt(messageIndex);
					if(!strUrl.IsEmpty())
						ShellExecute(NULL,NULL,strUrl,NULL,NULL,SW_SHOWNORMAL);
				}
			}
		}
		m_nTimeID = this->SetTimer((long)this,m_nSpeed,NULL);
		return 1;
}


void CScrollNewsBar::OnRButtonDown(UINT nFlags, CPoint point)
{

	if(FALSE == HitTool(point))
	{
		CMenu* pPopup = menu.GetSubMenu(0);

		CPoint clientPoint(point.x,point.y); 
		ClientToScreen(&clientPoint);
		pPopup->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON, clientPoint.x,clientPoint.y,this);
	}
}


void CScrollNewsBar::OnMultiline()
{//单击 多行显示的菜单
	
	bScrollMultiLine = (bScrollMultiLine==TRUE)?FALSE:TRUE;
	if(bScrollMultiLine)
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_MULTILINE,MF_CHECKED);
	}
	else
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_MULTILINE,MF_UNCHECKED);
	}
	
	m_showDataCRectArray.RemoveAll();//需要重新计算绘制区域

	((CStatuBar*)m_pStatuBar)->SetSize();
	((CStatuBar*)m_pStatuBar)->MoveScrollNewsBarWnd();
	SetSize();
}

void CScrollNewsBar::OnFloat()
{//单击 浮动显示的菜单
	
	bScrollFixed = (bScrollFixed==TRUE)?FALSE:TRUE;
	if(bScrollFixed)
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_FLOAT,MF_UNCHECKED);
	}
	else
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_FLOAT,MF_CHECKED);
	}

	m_showDataCRectArray.RemoveAll();//需要重新计算绘制区域
	((CStatuBar*)m_pStatuBar)->SetSize();
	((CStatuBar*)m_pStatuBar)->MoveScrollNewsBarWnd();
	this->SetSize();
}

void CScrollNewsBar::OnSetting()
{//单击 高级设置的菜单
	
	CPropSheet propSheet("");
	propSheet.m_prop.m_bkClr   = m_clrBk;
	propSheet.m_prop.m_fontClr = m_clrText;
	propSheet.m_prop.m_nSpeed  = 101 - m_nSpeed;
	propSheet.m_prop.m_clickWay = m_nClickWay;
	if(propSheet.m_prop.m_fontKind.m_hObject)
	{
		propSheet.m_prop.m_fontKind.DeleteObject();
	}

	LOGFONT lf;
	m_pFontText->GetLogFont(&lf);
	propSheet.m_prop.m_fontKind.CreateFontIndirect(&lf);
	if(propSheet.DoModal() == IDOK)
	{
		if(m_clrBk != propSheet.m_prop.m_bkClr)
		{//背景色被修改
			m_clrBk = propSheet.m_prop.m_bkClr;
			m_pColorCfg->SetScrollBKColor(m_clrBk);
		}

		if(m_clrText != propSheet.m_prop.m_fontClr)
		{//前景色被修改
			m_clrText = propSheet.m_prop.m_fontClr;
			m_pColorCfg->SetScrollTextColor(m_clrText);
		}

		m_nSpeed = 101 - propSheet.m_prop.m_nSpeed;
		m_nClickWay = propSheet.m_prop.m_clickWay;

		LOGFONT newlf;
		propSheet.m_prop.m_fontKind.GetLogFont(&newlf);

		LOGFONT lf;
		m_pFontText->GetLogFont(&lf);

		if ( memcmp(&lf, &newlf, sizeof(LOGFONT) ) )
		{//字体被修改
			
			m_pFontCfg->SetScrollFontProt(&propSheet.m_prop.m_fontKind);
			m_pFontText = m_pFontCfg->GetScrollFontProt();
		}		

	//	WriteCfg();
	}

	if(m_nTimeID!=0)
	{
		KillTimer(m_nTimeID);
		m_nTimeID=0;
	}
	m_nTimeID = SetTimer((long)this,m_nSpeed,NULL);
}


void CScrollNewsBar::OnMouseMove(UINT nFlags, CPoint point)
{//鼠标移动
   
	if(TRUE == HitCaption(point))
	{//如果鼠标移动到标题栏里面

		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
	}
	else if(TRUE == HitTest(point) && FALSE == HitTool(point))
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));	
	}
	else
	{	
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	}
}

void CScrollNewsBar::CalculateRect(CDC* pDC)
{

	if(m_sTitleArray.GetCount() == 0) //如果跑马灯没有咨询
		return;

	InitRect(pDC);

	CFont* oldFont = pDC->SelectObject(m_pFontText);

	CSize blankSize = pDC->GetTextExtent("  ");  //位置偏移量
	int OffPos = blankSize.cx;
	

	if(FALSE == bScrollMultiLine)
	{//如果是单行 则向左或者向右

		if(ScrollLeft == m_nHorizonScrollWay)
		{//向左

			//增加一个新矩形
			int nCount = m_showDataCRectArray.GetCount();
			int beginPos = 0;
			
			if ( nCount <= 0 
				 || ( (beginPos = m_showDataCRectArray.GetAt(nCount - 1).right + OffPos*4) < m_rectText.right )
			   )
			{
				int i = (m_nShowIndex + m_showDataCRectArray.GetCount())%m_sTitleArray.GetCount();
				CString title = m_sTitleArray.GetAt(i);
				CString content = m_sContentArray.GetAt(i);
				CSize sz = pDC->GetTextExtent(title+content);
				sz.cx += SCROLL_BITMAP_WIDTH; //加入图片的大小
				sz.cx += OffPos;

				CRect showRect = m_rectText;
				showRect.left = beginPos;
				showRect.right = beginPos + sz.cx;
				m_showDataCRectArray.Add(showRect);
			}
			
			//第一个矩形已经滑出屏幕
			if(m_showDataCRectArray.GetAt(0).right <= 0)
			{
				m_showDataCRectArray.RemoveAt(0); //将绘制矩形去除
				m_nShowIndex = (m_nShowIndex+1)%m_sTitleArray.GetCount(); //从下一个开始绘制			
			}

		}
		else 
		{//向右

			//增加一个新矩形
			int nCount = m_showDataCRectArray.GetCount();
			int lastPos = 0;
			
			if ( nCount <= 0
				 || ( (lastPos = m_showDataCRectArray.GetAt(0).left - OffPos*4) > m_rectText.left )
			   )
			{
				m_nShowIndex = (m_nShowIndex-1+m_sTitleArray.GetCount())%m_sTitleArray.GetCount();
				CString title = m_sTitleArray.GetAt(m_nShowIndex);
				CString content = m_sContentArray.GetAt(m_nShowIndex);
				CSize sz = pDC->GetTextExtent(title+content);
				sz.cx += SCROLL_BITMAP_WIDTH; //加入图片的大小
				sz.cx += OffPos;

				CRect showRect = m_rectText;
				showRect.right = lastPos;
				showRect.left = lastPos - sz.cx;
				m_showDataCRectArray.InsertAt(0,showRect);
			}

			//最后一个矩形已经滑出屏幕
			if(m_showDataCRectArray.GetAt(m_showDataCRectArray.GetCount()-1).left > m_rectText.right)
			{
				m_showDataCRectArray.RemoveAt(m_showDataCRectArray.GetCount()-1);//将绘制矩形去除
			}

		}
		
	}
	else
	{//多行模式 向上或者向下滚动

		if(ScrollUp == m_nVerticalScrollWay)
		{//向上
			
			//增加一个新矩形
			int nCount = m_showDataCRectArray.GetCount();
			int beginPos = 0;
			if ( nCount <= 0
				 || ( (beginPos = m_showDataCRectArray.GetAt(nCount - 1).bottom + OffPos) < m_rectText.bottom )  
			   )
			{

				int i = (m_nShowIndex + m_showDataCRectArray.GetCount())%m_sTitleArray.GetCount();
				CString title = m_sTitleArray.GetAt(i);
				CString content = m_sContentArray.GetAt(i);
				CSize sz = pDC->GetTextExtent(title+content);
				sz.cx += SCROLL_BITMAP_WIDTH; //加入图片的大小

				CRect showRect;
				showRect.left   = 0;
				showRect.right  = showRect.left + sz.cx + OffPos;
				showRect.top    = beginPos;
				showRect.bottom = showRect.top + sz.cy;

				m_showDataCRectArray.Add(showRect);		
			}

			
			if(m_showDataCRectArray.GetAt(0).bottom <= m_rectText.top)
			{//第一个矩形已经超过屏幕
				m_showDataCRectArray.RemoveAt(0); //将绘制矩形去除
				m_nShowIndex = (m_nShowIndex+1)%m_sTitleArray.GetCount(); //从下一个开始绘制			
			}
		}
		else
		{//向下

			//增加一个新矩形
			int nCount = m_showDataCRectArray.GetCount();
			int lastPos = 0;
			if (   nCount <= 0 
				   || ( (lastPos = m_showDataCRectArray.GetAt(0).top - OffPos) >  m_rectText.top) 
			   )
			{
				m_nShowIndex = (m_nShowIndex-1+m_sTitleArray.GetCount())%m_sTitleArray.GetCount();
				CString title = m_sTitleArray.GetAt(m_nShowIndex);
				CString content = m_sContentArray.GetAt(m_nShowIndex);
				CSize sz = pDC->GetTextExtent(title+content);
				sz.cx += SCROLL_BITMAP_WIDTH; //加入图片的大小

				CRect showRect  = m_rectText;
				showRect.left   = 0;
				showRect.right  = showRect.left + sz.cx + OffPos;
				showRect.bottom = lastPos;
				showRect.top    = showRect.bottom - sz.cy;
				m_showDataCRectArray.InsertAt(0,showRect);
			}

			//最后一个矩形已经滑出屏幕
			nCount = m_showDataCRectArray.GetCount();
			if(m_showDataCRectArray.GetAt(nCount-1).top > m_rectText.bottom)
			{
				m_showDataCRectArray.RemoveAt(nCount-1);//将绘制矩形去除
			}	
		}
	}

	pDC->SelectObject(oldFont);
}


void CScrollNewsBar::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static BOOL bScrolling = TRUE;
	if(nIDEvent == m_nTimeID)
	{
		m_bIsInitRect = TRUE;
		if(TRUE == bScrolling)
		{
			Scrolling(1);
			InvalidateRect(m_rectText,FALSE);
		}

		CPoint point;
		point.SetPoint(-1,-1);
		::GetCursorPos(&point);
		this->ScreenToClient(&point);
		if(FALSE == HitCaption(point))
		{
			if(TRUE == HitTest(point) && FALSE == HitTool(point))
			{
				bScrolling = FALSE;
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
			}
			else
			{
				bScrolling = TRUE;
				if(AfxGetApp()->LoadStandardCursor(IDC_HAND)  == ::GetCursor())
				   ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			}
		}
		else
		{
			bScrolling = TRUE;
		}
	}

}

void CScrollNewsBar::Scrolling(int diff)
{
	if(FALSE == bScrollMultiLine)
	{//单行模式
		if (ScrollLeft == m_nHorizonScrollWay)
		{//向左
			for(int i=0; i<m_showDataCRectArray.GetCount();i++)
			{
				m_showDataCRectArray.GetAt(i).left -= diff;
				m_showDataCRectArray.GetAt(i).right -= diff;
			}
		}
		else
		{//向右
			for(int i=0; i<m_showDataCRectArray.GetCount();i++)
			{
				m_showDataCRectArray.GetAt(i).left += diff;
				m_showDataCRectArray.GetAt(i).right += diff;
			}
		}
	}
	else
	{//多行模式
		if (ScrollUp == m_nVerticalScrollWay)
		{//向上
			for(int i=0; i<m_showDataCRectArray.GetCount();i++)
			{
				m_showDataCRectArray.GetAt(i).top -= diff;
				m_showDataCRectArray.GetAt(i).bottom -= diff;
			}
		}
		else
		{//向下
			for(int i=0; i<m_showDataCRectArray.GetCount();i++)
			{
				m_showDataCRectArray.GetAt(i).top += diff;
				m_showDataCRectArray.GetAt(i).bottom += diff;
			}
		}
	}
}


BOOL CScrollNewsBar::HitTest(CPoint point)
{
	if(point.x < 0 || point.y < 0)
		return FALSE;

	for(int i=0; i<m_showDataCRectArray.GetCount(); i++)
	{
		if(m_showDataCRectArray.GetAt(i).PtInRect(point) && m_rectText.PtInRect(point))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CScrollNewsBar::HitCaption(CPoint point)
{
	if(m_rectCaption.PtInRect(point))
		return TRUE;
	else
		return FALSE;
}

BOOL CScrollNewsBar::HitTool(CPoint point)
{
	if(m_rectTool.PtInRect(point))
		return TRUE;
	else
		return FALSE;
}

void CScrollNewsBar::OnDestroy()
{
	WriteCfg();
	m_sTitleArray.RemoveAll();
	m_sContentArray.RemoveAll();
	m_sUrlArray.RemoveAll();
	m_showDataCRectArray.RemoveAll();
	if(m_nTimeID != 0)
		KillTimer(m_nTimeID);

}
