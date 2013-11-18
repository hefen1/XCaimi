// ShareHolderList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Transaction.h"
#include "ShareHolderList.h"


// CShareHolderList �Ի���
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


// CShareHolderList ��Ϣ�������

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

	pListCtrl->InsertColumn(0,_T("���"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(1,_T("֤ȯ����"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(2,_T("֤ȯ����"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(3,_T("֤ȯ���"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(4,_T("��������"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(5,_T("�ο��ɱ���"),LVCFMT_LEFT,100);
	pListCtrl->InsertColumn(6,_T("���¼�"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(7,_T("��ֵ"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(8,_T("�ο�ӯ��"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(9,_T("ί�м۸�"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(10,_T("�ɶ��ʺ�"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(11,_T("֤ȯ�г�"),LVCFMT_LEFT,80);
	pListCtrl->InsertColumn(12,_T("�ο�ӯ������"),LVCFMT_LEFT,120);
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