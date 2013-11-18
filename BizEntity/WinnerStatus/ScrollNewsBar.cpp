// ScrollNewsBar.cpp : ʵ���ļ�
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
	m_pStatuBar = NULL;              // ������ָ���ʼ��Ϊ��
	m_bIsInitRect = FALSE;

	m_pScrollCfg = NULL;
	m_pColorCfg  = NULL;
	m_pFontCfg   = NULL;
          
	m_clrBk = RGB(0,0,0);          // ����������ɫ
	m_clrText = RGB(0,0,0);        // ����������ɫ
	m_pFontText = NULL;            // �ı�����
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
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Status_Log_Name,"WinnerStatus.Dll����IHsColorʧ�ܣ�");
		return FALSE;
	}
	m_pFontCfg = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
	if (m_pFontCfg == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL, Status_Log_Name, "WinnerStatues.DLL����IHsFontʧ��! ");
		return FALSE;
	}
	m_pScrollCfg = (IHsScrollNews*)CWinnerApplication::GetObject(OBJ_SCROLLNEW);
	if (m_pScrollCfg == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL, Status_Log_Name, "WinnerStatues.DLL����IHsScrollNewsʧ��! ");
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
	// ����ͼƬ��Դ
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CScrollNewsBar::InitRect(CDC* pDC)
{
	if(m_bIsInitRect)
		return;
	
	CFont* oldFont = pDC->SelectObject(m_pFontText);

	CSize blankSize = pDC->GetTextExtent("  ");  //λ��ƫ����
	int OffPos = blankSize.cx;

	CString title;
	CString content;
	CSize sz;
	CRect showRect;

	m_nShowIndex = 0; 
	m_showDataCRectArray.RemoveAll();
	if(FALSE == bScrollMultiLine)
	{//����ǵ��� �������������

		m_nShowIndex = 0; //��ʾ���ݴӵ�һ����ʼ
		int beginPos = 0;
		int i = 0;
		while(beginPos < m_rectText.right)
		{
			title = m_sTitleArray.GetAt(i);
			content = m_sContentArray.GetAt(i);
			sz = pDC->GetTextExtent(title+content);//��õ�ǰ����������Ĵ�С
			sz.cx += SCROLL_BITMAP_WIDTH; //����ͼƬ�Ĵ�С
			sz.cx += OffPos;//����������֮����һЩ�հ׵ط�

			showRect = m_rectText;
			showRect.left = beginPos; //��ʼλ��
			showRect.right = beginPos + sz.cx;//����λ��
			m_showDataCRectArray.Add(showRect);//�����λ�ü�¼

			beginPos += showRect.Width() + OffPos*4;//��һ����ʼλ��Ϊ��ǰ��ʼλ��+��ʾ���ݳ���+����
			i = (i+1)%m_sTitleArray.GetCount();//ѭ��ȡ��һ����¼
		}
	}
	else
	{//����Ƕ��� �����ϻ�������

		m_nShowIndex = 0; //��ʾ���ݴӵ�һ����ʼ
		int beginPos = 0;
		int i = 0;
		while(beginPos < m_rectText.bottom)
		{
			title = m_sTitleArray.GetAt(i);
			content = m_sContentArray.GetAt(i);
			sz = pDC->GetTextExtent(title+content);
			sz.cx += SCROLL_BITMAP_WIDTH; //����ͼƬ�Ĵ�С

			showRect.left   = 0; 
			showRect.right  = showRect.left + sz.cx + OffPos;
			showRect.top    = beginPos;
			showRect.bottom = showRect.top + sz.cy;
			m_showDataCRectArray.Add(showRect);  //�����λ�ü�¼
			beginPos += sz.cy + OffPos;
			i = (i+1)%m_sTitleArray.GetCount();//ѭ��ȡ��һ����¼
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

	//�������ļ��ж�ȡ
	m_nShowScrollAlone = m_pScrollCfg->GetShowScrollAlone(); //�������ʾ�ķ�ʽ
	bScrollFixed = m_pScrollCfg->IsScrollFixed();            //������Ƿ�̶���״̬����
	bScrollMultiLine = m_pScrollCfg->IsScrollMultiLine();    //������Ƿ������ʾ

	m_nIsUpDownRate   = m_pScrollCfg->GetUpDownRate();
	m_nIsTotal        = m_pScrollCfg->GetTotal();
	m_nIsShowNewPirce = m_pScrollCfg->GetShowNewPirce();
	m_nIsUpDown       = m_pScrollCfg->GetUpDown();

	//���ù������
	m_nHorizonScrollWay = m_pScrollCfg->GetHorizonScrollWay();
	m_nVerticalScrollWay = m_pScrollCfg->GetVerticalScrollWay();

	m_nClickWay = m_pScrollCfg->GetClickWay(); //����ѡ�з�ʽ
	m_nSpeed = m_pScrollCfg->GetScrollSpeed();//�ٶȳ�ʼ��

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

	//��ʾ��ɫ��ʼ��
	m_clrBk = m_pColorCfg->GetScrollBKColor();
	m_clrText = m_pColorCfg->GetScrollTestColor();
	
	//��ʾ�����ʼ��
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
	{//��������ƹ̶�����
		bRet = m_pScrollCfg->SetScrollFixed(bScrollFixed);
	}
	
	if (bScrollMultiLine != m_pScrollCfg->IsScrollMultiLine())
	{//��������ƶ��п���
		bRet = m_pScrollCfg->SetScrollMultiLine(bScrollMultiLine);
	}
	
	if (m_nSpeed != m_pScrollCfg->GetScrollSpeed())
	{//������������ٶ�
		bRet = m_pScrollCfg->SetScrollSpeed(m_nSpeed);
	}
	
	if (m_nHorizonScrollWay != m_pScrollCfg->GetHorizonScrollWay())
	{//��������ˮƽ��������
		bRet = m_pScrollCfg->SetHorizonScrollWay(m_nHorizonScrollWay);
	}

	if (m_nVerticalScrollWay != m_pScrollCfg->GetVerticalScrollWay())
	{//�������鴹ֱ��������
		bRet = m_pScrollCfg->SetVerticalScrollWay(m_nVerticalScrollWay);
	}
	
	if (m_nClickWay != m_pScrollCfg->GetClickWay())
	{//����������ʾ��ʽ
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
	{//����ǹ̶���״̬���� �������û�б���
		m_rectCaption.SetRectEmpty();

		//��ѯ����
		m_rectText       = rect;
		m_rectText.right = rect.right - SCROLL_FLOAT_HEIGHT;

		//����������
		m_rectTool      = rect;
		m_rectTool.left = m_rectText.right;
	}
	else
	{ 
		if(bScrollMultiLine)
		{//������ʾ
			//����������
			m_rectCaption        = rect;
			m_rectCaption.bottom = m_rectCaption.top + SCROLL_FLOAT_HEIGHT;
			m_rectCaption.right  = rect.right - SCROLL_FLOAT_HEIGHT;

			//��ѯ����
			m_rectText     = rect;
			m_rectText.top = m_rectCaption.bottom;

			//����������
			m_rectTool        = rect;
			m_rectTool.bottom = m_rectTool.top + SCROLL_FLOAT_HEIGHT;
			m_rectTool.left   = m_rectCaption.right; 
		}
		else
		{//������ʾ
			//����������
			m_rectCaption       = rect;
			m_rectCaption.right = rect.left + SCROLL_FLOAT_HEIGHT;

			//��ѯ����
			m_rectText       = rect;
			m_rectText.left  = m_rectCaption.right;
			m_rectText.right = m_rectTool.left;

			//����������
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


// CScrollNewsBar ��Ϣ�������
void CScrollNewsBar::OnPaint()
{
	CPaintDC dc(this);
	CMemdc MemDC(&dc);

	DrawMyText(&MemDC);
	
}

void CScrollNewsBar::DrawMyCaption( CDC* pDC )
{
	if(!bScrollFixed)
	{//ֻ�е�����Ƹ�����ʾ��ʱ��Ż��б�����
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

			//��ʾָ��
			COLORREF oldColor;
			COLORREF crl;
			CRect rect = m_rectCaption;
			rect.right = rect.left;

			for(int i=0; i<INDEX_MAX_COUNT; i++)
			{
				if(pBar->m_indexString[i].trend == -2)
					continue;

				//��Ʊ����
				CSize sz = pDC->GetTextExtent(pBar->m_indexString[i].name);
				rect.left  = rect.right;
				rect.right = rect.left + sz.cx;
				crl = pBar->m_clrCodeName;
				oldColor = pDC->SetTextColor(pBar->m_clrCodeName);
				pDC->DrawText(pBar->m_indexString[i].name, rect, DT_VCENTER|DT_SINGLELINE);

				//ѡ����ɫ
				if(pBar->m_indexString[i].trend == -1) //��
				{
					pDC->SetTextColor(pBar->m_clrDown);
				}
				else if(pBar->m_indexString[i].trend == 0)//ƽ
				{
					pDC->SetTextColor(pBar->m_clrEqual);
				}
				else if(pBar->m_indexString[i].trend == 1)//��
				{
					pDC->SetTextColor(pBar->m_clrUp);
				}

				//�ּ�
				if(m_nIsShowNewPirce == 1)
				{
					sz = pDC->GetTextExtent(pBar->m_indexString[i].nowPrice);
					rect.left  = rect.right;
					rect.right = rect.left + sz.cx;
					pDC->DrawText(pBar->m_indexString[i].nowPrice,rect,DT_VCENTER|DT_SINGLELINE);
				}

				//�ǵ�֮ǰ�ķ���
				if(m_nIsUpDown == 1)
				{
					rect.left = rect.right + oneWordSize.cx;
					rect.right = rect.left  + SCROLL_FLOAT_HEIGHT/4;
					CRect arrowRect = rect;
					arrowRect.top += arrowRect.Height()/8;
					if(pBar->m_indexString[i].trend == -1) //��
					{
						CGeneralHelper::DrawArrow(pDC, arrowRect, pBar->m_clrDown, FALSE);
					}
					else if(pBar->m_indexString[i].trend == 0)//ƽ
					{

					}
					else if(pBar->m_indexString[i].trend == 1)//��
					{
						CGeneralHelper::DrawArrow(pDC, arrowRect, pBar->m_clrUp, TRUE);
					}
		
					//�ǵ�
					rect.right += oneWordSize.cx;
					sz = pDC->GetTextExtent(pBar->m_indexString[i].ricePrice);
					rect.left  = rect.right;
					rect.right = rect.left + sz.cx;
					pDC->DrawText(pBar->m_indexString[i].ricePrice, rect, DT_VCENTER|DT_SINGLELINE);
				}


				//�Ƿ�
				if(m_nIsUpDown == 1)
				{
					sz = pDC->GetTextExtent(pBar->m_indexString[i].riceRatio);
					rect.left  = rect.right + oneWordSize.cx;
					rect.right = rect.left + sz.cx;
					pDC->DrawText(pBar->m_indexString[i].riceRatio, rect, DT_VCENTER|DT_SINGLELINE);
				}

				
				//�ܽ��
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
		{// ����ͼƬ
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

	//��Ҫ�ѱ������ػ�һ��
	DrawMyCaption(pDC);
	DrawMyTool(pDC);
}


BOOL CScrollNewsBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CScrollNewsBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (TRUE == HitCaption(point))
	{
		lastPoint = point; //��¼��ʼλ��
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
{//��׽����¼�

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
		{//���û�й�����
			
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
{//���� ������ʾ�Ĳ˵�
	
	bScrollMultiLine = (bScrollMultiLine==TRUE)?FALSE:TRUE;
	if(bScrollMultiLine)
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_MULTILINE,MF_CHECKED);
	}
	else
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_MULTILINE,MF_UNCHECKED);
	}
	
	m_showDataCRectArray.RemoveAll();//��Ҫ���¼����������

	((CStatuBar*)m_pStatuBar)->SetSize();
	((CStatuBar*)m_pStatuBar)->MoveScrollNewsBarWnd();
	SetSize();
}

void CScrollNewsBar::OnFloat()
{//���� ������ʾ�Ĳ˵�
	
	bScrollFixed = (bScrollFixed==TRUE)?FALSE:TRUE;
	if(bScrollFixed)
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_FLOAT,MF_UNCHECKED);
	}
	else
	{
		menu.GetSubMenu(0)->CheckMenuItem(ID_FLOAT,MF_CHECKED);
	}

	m_showDataCRectArray.RemoveAll();//��Ҫ���¼����������
	((CStatuBar*)m_pStatuBar)->SetSize();
	((CStatuBar*)m_pStatuBar)->MoveScrollNewsBarWnd();
	this->SetSize();
}

void CScrollNewsBar::OnSetting()
{//���� �߼����õĲ˵�
	
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
		{//����ɫ���޸�
			m_clrBk = propSheet.m_prop.m_bkClr;
			m_pColorCfg->SetScrollBKColor(m_clrBk);
		}

		if(m_clrText != propSheet.m_prop.m_fontClr)
		{//ǰ��ɫ���޸�
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
		{//���屻�޸�
			
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
{//����ƶ�
   
	if(TRUE == HitCaption(point))
	{//�������ƶ�������������

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

	if(m_sTitleArray.GetCount() == 0) //��������û����ѯ
		return;

	InitRect(pDC);

	CFont* oldFont = pDC->SelectObject(m_pFontText);

	CSize blankSize = pDC->GetTextExtent("  ");  //λ��ƫ����
	int OffPos = blankSize.cx;
	

	if(FALSE == bScrollMultiLine)
	{//����ǵ��� �������������

		if(ScrollLeft == m_nHorizonScrollWay)
		{//����

			//����һ���¾���
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
				sz.cx += SCROLL_BITMAP_WIDTH; //����ͼƬ�Ĵ�С
				sz.cx += OffPos;

				CRect showRect = m_rectText;
				showRect.left = beginPos;
				showRect.right = beginPos + sz.cx;
				m_showDataCRectArray.Add(showRect);
			}
			
			//��һ�������Ѿ�������Ļ
			if(m_showDataCRectArray.GetAt(0).right <= 0)
			{
				m_showDataCRectArray.RemoveAt(0); //�����ƾ���ȥ��
				m_nShowIndex = (m_nShowIndex+1)%m_sTitleArray.GetCount(); //����һ����ʼ����			
			}

		}
		else 
		{//����

			//����һ���¾���
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
				sz.cx += SCROLL_BITMAP_WIDTH; //����ͼƬ�Ĵ�С
				sz.cx += OffPos;

				CRect showRect = m_rectText;
				showRect.right = lastPos;
				showRect.left = lastPos - sz.cx;
				m_showDataCRectArray.InsertAt(0,showRect);
			}

			//���һ�������Ѿ�������Ļ
			if(m_showDataCRectArray.GetAt(m_showDataCRectArray.GetCount()-1).left > m_rectText.right)
			{
				m_showDataCRectArray.RemoveAt(m_showDataCRectArray.GetCount()-1);//�����ƾ���ȥ��
			}

		}
		
	}
	else
	{//����ģʽ ���ϻ������¹���

		if(ScrollUp == m_nVerticalScrollWay)
		{//����
			
			//����һ���¾���
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
				sz.cx += SCROLL_BITMAP_WIDTH; //����ͼƬ�Ĵ�С

				CRect showRect;
				showRect.left   = 0;
				showRect.right  = showRect.left + sz.cx + OffPos;
				showRect.top    = beginPos;
				showRect.bottom = showRect.top + sz.cy;

				m_showDataCRectArray.Add(showRect);		
			}

			
			if(m_showDataCRectArray.GetAt(0).bottom <= m_rectText.top)
			{//��һ�������Ѿ�������Ļ
				m_showDataCRectArray.RemoveAt(0); //�����ƾ���ȥ��
				m_nShowIndex = (m_nShowIndex+1)%m_sTitleArray.GetCount(); //����һ����ʼ����			
			}
		}
		else
		{//����

			//����һ���¾���
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
				sz.cx += SCROLL_BITMAP_WIDTH; //����ͼƬ�Ĵ�С

				CRect showRect  = m_rectText;
				showRect.left   = 0;
				showRect.right  = showRect.left + sz.cx + OffPos;
				showRect.bottom = lastPos;
				showRect.top    = showRect.bottom - sz.cy;
				m_showDataCRectArray.InsertAt(0,showRect);
			}

			//���һ�������Ѿ�������Ļ
			nCount = m_showDataCRectArray.GetCount();
			if(m_showDataCRectArray.GetAt(nCount-1).top > m_rectText.bottom)
			{
				m_showDataCRectArray.RemoveAt(nCount-1);//�����ƾ���ȥ��
			}	
		}
	}

	pDC->SelectObject(oldFont);
}


void CScrollNewsBar::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	{//����ģʽ
		if (ScrollLeft == m_nHorizonScrollWay)
		{//����
			for(int i=0; i<m_showDataCRectArray.GetCount();i++)
			{
				m_showDataCRectArray.GetAt(i).left -= diff;
				m_showDataCRectArray.GetAt(i).right -= diff;
			}
		}
		else
		{//����
			for(int i=0; i<m_showDataCRectArray.GetCount();i++)
			{
				m_showDataCRectArray.GetAt(i).left += diff;
				m_showDataCRectArray.GetAt(i).right += diff;
			}
		}
	}
	else
	{//����ģʽ
		if (ScrollUp == m_nVerticalScrollWay)
		{//����
			for(int i=0; i<m_showDataCRectArray.GetCount();i++)
			{
				m_showDataCRectArray.GetAt(i).top -= diff;
				m_showDataCRectArray.GetAt(i).bottom -= diff;
			}
		}
		else
		{//����
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
