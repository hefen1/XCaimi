// MessagePromptDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Winner.h"
#include "MessagePromptDialog.h"

#include "..\..\HelperLib\GeneralHelper.h"
#include "..\..\Common\WinnerApplication.h"

// CMessagePromptDialog 对话框

IMPLEMENT_DYNAMIC(CMessagePromptDialog, CDialogEx)

CMessagePromptDialog::CMessagePromptDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessagePromptDialog::IDD, pParent)
{
	m_pImg = NULL;
}

CMessagePromptDialog::~CMessagePromptDialog()
{
	if(m_pImg)
		delete m_pImg;
}

void CMessagePromptDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessagePromptDialog, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMessagePromptDialog 消息处理程序

BOOL CMessagePromptDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CImage img;
	CString strImgFile = CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH)
		+ _T("cry_smile.png");

	DWORD dwMinSize = MultiByteToWideChar (CP_ACP, 0, strImgFile, -1, NULL, 0);
	wchar_t *pwText = new wchar_t[dwMinSize];
	MultiByteToWideChar (CP_ACP, 0, strImgFile, -1, pwText, dwMinSize); 
	m_pImg = new Image(pwText,FALSE);
	delete pwText;

	CWnd *pText = (CWnd *)GetDlgItem(IDC_TEXTINFO); 
	if(pText)
	{
		pText->SetWindowText(m_strMsg);
	}

	switch (m_mt)
	{
	case mtWaring:
		this->SetWindowText(_T("警告"));
		break;
	case mtError:
		this->SetWindowText(_T("错误"));
		break;
	default:
		this->SetWindowText(_T("提示"));
	}

	return TRUE;
}


void CMessagePromptDialog::ShowMsg(MessageType mt,CString strMsg)
{
	m_mt = mt;
	m_strMsg = strMsg;
	DoModal();
}
void CMessagePromptDialog::OnPaint()
{
	CPaintDC dc(this); 
	if(m_pImg)
	{
		Graphics graphics( dc.m_hDC);
		graphics.DrawImage(m_pImg, 10,10,m_pImg->GetWidth(),m_pImg->GetHeight());
	}
	
}
