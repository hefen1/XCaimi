// ShareHolderList.cpp : 实现文件
//

#include "stdafx.h"
#include "Transaction.h"
#include "ShareHolderList.h"


// CShareHolderList 对话框
BEGIN_WINDOW_MAP(WNDMAP_SHARTHOLDERLIST)
	BEGINCOLS(WRCT_REST,0,0)
	RCREST(IDC_SHARTHOLDER)
	ENDGROUP()
END_WINDOW_MAP()

IMPLEMENT_DYNAMIC(CShareHolderList, CHSBizDialog)

CShareHolderList::CShareHolderList(CWnd* pParent /*=NULL*/)
: CHSBizDialog(CShareHolderList::IDD, pParent),m_winMgr(WNDMAP_SHARTHOLDERLIST)
{

}

CShareHolderList::~CShareHolderList()
{
}

void CShareHolderList::DoDataExchange(CDataExchange* pDX)
{
	CHSBizDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShareHolderList, CHSBizDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CShareHolderList 消息处理程序

BOOL CShareHolderList::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();

	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);

	FillTestData();

	return TRUE; 
}

void CShareHolderList::OnSize(UINT nType, int cx, int cy)
{
	CHSBizDialog::OnSize(nType, cx, cy);

	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);
}


void CShareHolderList::FillTestData()
{
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_SHARTHOLDER);

	pListCtrl->InsertColumn(0,_T("序号"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(1,_T("证券代码"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(2,_T("证券名称"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(3,_T("证券余额"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(4,_T("可卖数量"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(5,_T("参考成本价"),LVCFMT_LEFT,100);
	pListCtrl->InsertColumn(6,_T("最新价"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(7,_T("市值"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(8,_T("参考盈亏"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(9,_T("委托价格"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(10,_T("股东帐号"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(11,_T("证券市场"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(12,_T("参考盈亏比例"),LVCFMT_LEFT,120);
}


BOOL CShareHolderList::OnInit()
{
	RegisterHotKey(1,0,VK_F3);
	return TRUE;
}

void CShareHolderList::OnHotKey(UINT nId)
{
	if( nId == 1)
	{
		ActiveSelf();
	}
}

void CShareHolderList::OnShow(BOOL bShow)
{
	CString str;
	str.Format("[CShareHolderList::OnShow] bShow=%d\n",bShow);
	OutputDebugString(str);
}