// EntrustList.cpp : 实现文件
//

#include "stdafx.h"
#include "Transaction.h"
#include "EntrustList.h"

#include "..\..\Common\WinnerApplication.h"

// CEntrustList 对话框
BEGIN_WINDOW_MAP(WNDMAP_ENTSTRUSTLIST)
	BEGINCOLS(WRCT_REST,0,0)
		RCREST(IDC_ENTRUST)
	ENDGROUP()
END_WINDOW_MAP()

IMPLEMENT_DYNAMIC(CEntrustList, CHSBizDialog)

CEntrustList::CEntrustList(CWnd* pParent /*=NULL*/)
	: CHSBizDialog(CEntrustList::IDD, pParent),m_winMgr(WNDMAP_ENTSTRUSTLIST)
{

}

CEntrustList::~CEntrustList()
{
}

void CEntrustList::DoDataExchange(CDataExchange* pDX)
{
	CHSBizDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEntrustList, CHSBizDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CEntrustList 消息处理程序

BOOL CEntrustList::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();

	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);

	FillTestData();
	return TRUE;
}

void CEntrustList::OnSize(UINT nType, int cx, int cy)
{
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);
}


void CEntrustList::FillTestData()
{
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_ENTRUST);

	pListCtrl->InsertColumn(0,_T("序号"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(1,_T("合同号"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(2,_T("申报时间"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(3,_T("委托状态"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(4,_T("股东帐号"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(5,_T("证券代码"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(6,_T("证券名称"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(7,_T("买卖方向"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(8,_T("委托价格"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(9,_T("委托数量"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(10,_T("成交数量"),LVCFMT_LEFT,80);

	CString strText;
	int nColumnCount = pListCtrl->GetHeaderCtrl()->GetItemCount();

	for (int i=0;i < 10;i++)
	{
		strText.Format(TEXT("item %d"), i);

		pListCtrl->InsertItem(i, strText);

		for (int j=1;j < nColumnCount;j++)
		{
			strText.Format(TEXT("sub-item %d %d"), i, j);
			pListCtrl->SetItemText(i, j, strText);
		}
	}
}

BOOL CEntrustList::OnInit()
{
	RegisterHotKey(1,0,VK_F4);
	RegisterHotKey(2,MOD_CONTROL,'A');
	RegisterHotKey(3,MOD_ALT,'A');
	RegisterLinkageMsg(1000);
	return TRUE;
}

void CEntrustList::OnLinkageMsg(CLinkageParam* pParam)
{
	if(pParam->m_nMsg = 1000)
	{
		CWinnerApplication::FormatLog(DEBUG_LOG_LEVEL,_T("winner"),
			_T("[CEntrustList::OnLinkageMsg] name1=%d,name2=%d,name3=%s"),
			pParam->GetIntegerField(_T("name1")),
			pParam->GetIntegerField(_T("name2")),
			pParam->GetStringField(_T("name3")) );
	}
}

void CEntrustList::OnHotKey(UINT nId)
{
	if( nId == 1)
	{
		ActiveSelf();
	}
	else if (nId == 2)
	{
		if( IsShow() )
		{
			CString strMsg;
			strMsg.Format("(CEntrustList::OnHotKey)handle=%x: ctrl + a is pressed.",this->m_hWnd);
			OutputDebugString(strMsg + "\n");
			MessageBox(strMsg);
			this->SetFocus();
		}	
	}
	else if (nId == 3)
	{
		if( IsShow() )
		{
			CString strMsg;
			strMsg.Format("(CEntrustList::OnHotKey)handle=%x: alt + a is pressed.",this->m_hWnd);
			OutputDebugString(strMsg + "\n");
			MessageBox(strMsg);
			this->SetFocus();
		}	
	}
}

void CEntrustList::OnShow(BOOL bShow)
{
	CString str;
	str.Format("[CEntrustList::OnShow] bShow=%d\n",bShow);
	OutputDebugString(str);
}