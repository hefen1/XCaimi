// DlgError.cpp : implementation file
//

#include "stdafx.h"
#include "DlgError.h"
#include "Express.h"
#include "CompileEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgError dialog


CDlgError::CDlgError(CWnd* pParent /*=NULL*/,CCompileEditView* pRichEdit /*= NULL*/)
	: CDialog(CDlgError::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgError)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRichEdit  = pRichEdit;	

	CExpression::SetCurWnd(this);

	//m_pWnd = this;
	Create(CDlgError::IDD,pParent);
}


void CDlgError::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgError)
	DDX_Control(pDX, IDC_LIST, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgError, CDialog)
	//{{AFX_MSG_MAP(CDlgError)
	//}}AFX_MSG_MAP
	
	ON_MESSAGE(HX_USER_COMPILE_INFO, AddCompileInfotoList)

	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgError message handlers

void CDlgError::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDlgError::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CDlgError::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_imaSmall.Create(IDB_OUTIMAGELIST, 16, 0, RGB(0,128,128));

	m_wndList.SetImageList(&m_imaSmall, LVSIL_SMALL);
	LV_COLUMN lvcolumn;
	CString str;
	for(int i = 0; i < 1; i++)
	{
		str.Format("aa%i",i);
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = (char*)LPCTSTR(str);
		lvcolumn.iSubItem = i;
		lvcolumn.cx = 10000;
		m_wndList.InsertColumn(i, &lvcolumn);
	}
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_wndList.ShowScrollBar(SB_HORZ,FALSE);
	m_wndList.ShowScrollBar(SB_VERT,TRUE);

	/*
	CreateRoot( VERTICAL )
		<< item(IDC_LIST);

	UpdateLayout ();
	*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

long CDlgError::AddCompileInfotoList(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	if(m_wndList.m_hWnd == NULL)
		return 0;

	CString strText;
	short nImage;
	nodErrorInfo* pError = (nodErrorInfo*)lParam;
	int nCount = m_wndList.GetItemCount();
	if(pError != NULL)
	{	
	/*
		if(wParam == 10)
		{
			nImage = 2;
			strText.Format("%s",pError->GetError());
		}
		else if(wParam == 11)
		{
			nImage = 2;
			strText.Format("%s - %i 错误,%i 警告",pError->GetError(),nCount,0);
		}
		else if(wParam == 12)
		{
			nImage = 2;
			strText.Format("%s - %i 错误,%i 警告",pError->GetError(),nCount>0?nCount-1:nCount,0);
		}
		else*/
		{
			nImage = 0;
			strText.Format(_T("错误号:%i;错误信息:%s (%s)错误行列:%i-%i"),pError->m_nError,
				hxRelaceReturnKey(pError->GetError(),"\r\n"),hxGetErrorInfoByErrorNum(pError->m_nError),pError->m_nBegin,pError->m_nEnd);

			if(nCount == 0 && pError->m_nBegin >= 0 && pError->m_nEnd >= 0)
			{
				m_pRichEdit->GetRichEditCtrl().SetSel(pError->m_nBegin,pError->m_nEnd);
			}
		}
	}
	else
	{
		strText = _T("公式编译（测试）通过!");
		nImage = 1;
	}	
	m_wndList.InsertItem(nCount,strText,nImage);
	return 0;
}

void CDlgError::PostNcDestroy()
{
	//delete this;
}

/*************************************************************
* 加入
* 时间：  2003年9月23日
* 描述：  清空错误信息
*************************************************************/
void CDlgError::ClearError()
{
	m_wndList.DeleteAllItems();
}

void CDlgError::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if( !::IsWindow(m_wndList.m_hWnd) )
		return;

	CRect rect;
	this->GetClientRect(rect);
	m_wndList.MoveWindow(rect);	
	// TODO: Add your message handler code here
}
