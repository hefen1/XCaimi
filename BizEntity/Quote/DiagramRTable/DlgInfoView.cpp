// DlgInfoView.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "DiagramRTable.h"
#include "DlgInfoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDlgInfoView �Ի���

IMPLEMENT_DYNAMIC(CDlgInfoView, CDialog)

CDlgInfoView::CDlgInfoView(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInfoView::IDD, pParent)
{
	m_pInfoView = NULL;
}

CDlgInfoView::~CDlgInfoView()
{
	delete m_pInfoView;
}

void CDlgInfoView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInfoView, CDialog)
END_MESSAGE_MAP()


// CDlgInfoView ��Ϣ�������

BOOL CDlgInfoView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pInfoView = new CHSHtmlView;
	if (!m_pInfoView->CreateView(this))
	{
		delete m_pInfoView;
		m_pInfoView = NULL;
		return FALSE;
	}

	m_pInfoView->Navigate2(m_strUrl);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgInfoView::SetUrlString( CString str )
{
	m_strUrl = str;
}