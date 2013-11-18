// Prop.cpp : 实现文件
//

#include "stdafx.h"
#include "WinnerStatus.h"
#include "Prop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CProp 对话框

IMPLEMENT_DYNAMIC(CProp, CPropertyPage)

CProp::CProp()
	: CPropertyPage(CProp::IDD)
	, m_clickWay(-1)
{

}

CProp::~CProp()
{
}

void CProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_BKCLR, m_BtnBkClr);

	//	DDX_Control(pDX, IDC_FONTCLR, m_BtnFontClr);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlSlider);
	DDX_Control(pDX, IDC_BKCLR, m_BtnBkClr);
	DDX_Control(pDX, IDC_FONTCLR, m_BtnFontClr);
	DDX_Radio(pDX, IDC_RADIO1, m_clickWay);

}


BEGIN_MESSAGE_MAP(CProp, CPropertyPage)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BKCLR, &CProp::OnBnClickedBkclr)
	ON_BN_CLICKED(IDC_FONTCLR, &CProp::OnBnClickedFontclr)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CProp::OnNMCustomdrawSlider1)
	ON_STN_CLICKED(IDC_FONTKIND, &CProp::OnStnClickedFontkind)
END_MESSAGE_MAP()


// CProp 消息处理程序

BOOL CProp::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_ctrlSlider.SetRange(0,100);   //设置滑块范围
	m_ctrlSlider.SetPos(m_nSpeed);

	CMFCButton::EnableWindowsTheming(FALSE);
	m_BtnBkClr.SetFaceColor(m_bkClr,TRUE);
	m_BtnFontClr.SetFaceColor(m_fontClr,TRUE);

	return TRUE; 
}

void CProp::OnPaint()
{
	CPaintDC dc(this);
	OnDrawLabel();
}

void CProp::OnDrawLabel()
{//绘制静态框
	
	CWnd* pLabel = GetDlgItem(IDC_FONTKIND);
	CPaintDC dc(pLabel);
	CRect rect;
	pLabel->GetClientRect(&rect);

	dc.FillSolidRect(rect,RGB(255,255,255));
	dc.SelectObject(&m_fontKind);
	CSize sz = dc.GetTextExtent("A");
	
	LOGFONT lf;
	m_fontKind.GetLogFont(&lf);
	
	CString message = lf.lfFaceName;
	message.Format("%s, %d",lf.lfFaceName,sz.cy);

	dc.DrawText(message,rect,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
	
}
void CProp::OnBnClickedFontclr()
{	
	//单击滚动字体 颜色按钮
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT;
	dlg.m_cc.rgbResult = m_fontClr;
	if(IDOK == dlg.DoModal())
	{
		m_fontClr = dlg.m_cc.rgbResult;
		m_BtnFontClr.SetFaceColor(m_fontClr,TRUE);
	}
}


void CProp::OnBnClickedBkclr()
{
	//单击滚动背景颜色按钮
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT;
	dlg.m_cc.rgbResult = m_bkClr;
	if(IDOK == dlg.DoModal())
	{
		m_bkClr = dlg.m_cc.rgbResult;
		m_BtnBkClr.SetFaceColor(m_bkClr,TRUE);
	}
}



void CProp::OnStnClickedFontkind()
{
	// TODO: 在此添加控件通知处理程序代码
	LOGFONT lf;
	m_fontKind.GetLogFont(&lf);
	CFontDialog dlg(&lf);

	if(IDOK == dlg.DoModal())
	{
		if(m_fontKind.m_hObject)
			m_fontKind.DeleteObject();
		m_fontKind.CreateFontIndirect(dlg.m_cf.lpLogFont);
		Invalidate();
	}
}

void CProp::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_nSpeed = m_ctrlSlider.GetPos();
	*pResult = 0;
}

BOOL CProp::DestroyWindow()
{
	m_BtnFontClr.DestroyWindow();
	m_BtnBkClr.DestroyWindow();
	m_ctrlSlider.DestroyWindow();

	return CPropertyPage::DestroyWindow();
}
