// DlgStockList.cpp : implementation file
//

#include "stdafx.h"
#include "DlgStockList.h"

extern HWND			 g_hParentWnd;
// CDlgStockList dialog

IMPLEMENT_DYNAMIC(CDlgStockList, CDialog)
CDlgStockList::CDlgStockList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStockList::IDD, pParent)
{
}

CDlgStockList::~CDlgStockList()
{
}

void CDlgStockList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
}


BEGIN_MESSAGE_MAP(CDlgStockList, CDialog)
	ON_BN_CLICKED(IDD_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDD_DEL, OnBnClickedDel)
	ON_BN_CLICKED(IDOK4, OnBnClickedOk4)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)
END_MESSAGE_MAP()


// CDlgStockList message handlers

void CDlgStockList::OnBnClickedAdd()
{
	MSG pMsg;
	pMsg.message = WM_CHAR;
	pMsg.lParam = 0;
	pMsg.wParam = 0;
	SendAddStockMsg(&pMsg);
}

LRESULT CDlgStockList::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	HSCurKeyInfo* pData = (HSCurKeyInfo*)lParam;
	if(pData == NULL)
		return -1;

	StockUserInfo* pStock = (StockUserInfo*)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 19, (LPARAM)&pData->m_cCode);

	if( pStock == NULL )
		return -1;
	AddStock(pStock);

	return 0;
}

BOOL CDlgStockList::SendAddStockMsg(MSG *pMsg)
{
	AlarmMsgData srtData;
	srtData.pWnd = this->GetParent();
	srtData.pMsg = pMsg;
	return (BOOL)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 23, (LPARAM)&srtData);
}

void CDlgStockList::OnBnClickedDel()
{
	int iCurSel = m_wndList.GetSelectionMark();
	if( iCurSel == -1 )
		return;

	if( iCurSel == 0 && m_pFixeStock )
	{
		MessageBox(_T("固定股票，不能删除!"), _T("提示"));
		return;
	}

	DelStock(iCurSel);
}

BOOL CDlgStockList::AddStock(StockUserInfo* pStock)
{
	if( pStock == NULL )
		return false;
	if( IsHave(pStock) )
	{
		MessageBox(_T("重复加入"), _T("提示"));
		return false;
	}

	m_ayStock.Add(pStock);

	CString str(pStock->m_cStockName, sizeof(pStock->m_cStockName));
	m_wndList.InsertItem(m_wndList.GetItemCount(), str, 0);

	return true;
}

BOOL CDlgStockList::Display()
{
	int iAdd = 0;
	m_wndList.DeleteAllItems();
	if( m_pFixeStock )
	{
		CString str(m_pFixeStock->m_cStockName, sizeof(m_pFixeStock->m_cStockName));
		m_wndList.InsertItem(iAdd, str, 0);
		iAdd = 1;
	}

	for( int i = 0; i < m_ayStock.GetSize(); i++ )
	{
		StockUserInfo* pStock = m_ayStock.GetAt(i);
		if( pStock )
		{
			CString str(pStock->m_cStockName, sizeof(pStock->m_cStockName));
			m_wndList.InsertItem(i+iAdd, str, 0);
		}
	}

	return true;
}

BOOL CDlgStockList::InitCtrl()
{
	m_wndList.SetType(CHSListCtrlBase::StockList);

	return true;
}

BOOL CDlgStockList::DelStock(int nIdx)
{
	if( nIdx >= 0 && nIdx < m_wndList.GetItemCount() )
	{
		m_wndList.DeleteItem(nIdx);
	}

	nIdx--;
	if( nIdx >= 0 && nIdx < m_ayStock.GetSize() )
	{
		m_ayStock.RemoveAt(nIdx);
		return true;
	}

	return false;
}

BOOL CDlgStockList::IsHave(StockUserInfo* pStock)
{
	if( pStock == NULL )
		return false;

	for( int i = 0; i < m_ayStock.GetSize(); i++ )
	{
		StockUserInfo* pInfo = m_ayStock.GetAt(i);
		if( pInfo )
		{
			if( strncmp(pInfo->m_ciStockCode.m_cCode, pStock->m_ciStockCode.m_cCode, sizeof(pInfo->m_ciStockCode.m_cCode)) == 0 )
				return true;
		}
	}

	if( m_pFixeStock )
	{
		if( strncmp(m_pFixeStock->m_ciStockCode.m_cCode, pStock->m_ciStockCode.m_cCode, sizeof(m_pFixeStock->m_ciStockCode.m_cCode)) == 0 )
			return true;
	}

	return false;
}

void CDlgStockList::OnBnClickedOk4()
{
	CDialog::OnCancel();
}

BOOL CDlgStockList::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_CHAR) && SendAddStockMsg(pMsg) )
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgStockList::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitCtrl();
	Display();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgStockList::SetStock(StockUserInfo* pFixeStock, CArray<StockUserInfo*, StockUserInfo*> &ayStock)
{
	m_ayStock.RemoveAll();
	m_ayStock.Copy(ayStock);
	m_pFixeStock = pFixeStock;
}

void CDlgStockList::CopyStockTo(CArray<StockUserInfo*, StockUserInfo*> &ayStock)
{
	ayStock.RemoveAll();
	ayStock.Copy(m_ayStock);
}

