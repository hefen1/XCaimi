// DlgSelStock.cpp : 实现文件
//

#include "stdafx.h"
#include "WinnerStatus.h"
#include "DlgSelStock.h"
#include "QuoteLangMngInterFace.h"
#include "..\..\Resource\QuoteLang\Resource.h"
#include "WinnerApplication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CDlgSelStock 对话框

IMPLEMENT_DYNAMIC(CDlgSelStock, CDialog)

CDlgSelStock::CDlgSelStock(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelStock::IDD, pParent)
{
	m_stockInfoSelected.m_cStockName[0]='\0';
}

CDlgSelStock::~CDlgSelStock()
{
	if (m_listImage.GetSafeHandle())
	{
		m_listImage.DeleteImageList();
	}
}

void CDlgSelStock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tabctrl);
	DDX_Control(pDX, IDD_STOCKLIST, m_listStock);
	DDX_Control(pDX, IDD_STOCKTYPE, m_listType);
	DDX_Control(pDX, IDC_TREE_SYSTEMBLOCKENTRY, m_treeCtrl);
}


BEGIN_MESSAGE_MAP(CDlgSelStock, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgSelStock::OnTcnSelchangeTab)
	ON_NOTIFY(LVN_ITEMCHANGED, IDD_STOCKTYPE, &CDlgSelStock::OnLvnItemchangedStocktype)
	ON_NOTIFY(LVN_ITEMCHANGED, IDD_STOCKLIST, &CDlgSelStock::OnLvnItemchangedStocklist)
	ON_NOTIFY(NM_DBLCLK, IDD_STOCKLIST, &CDlgSelStock::OnNMDblclkStocklist)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SYSTEMBLOCKENTRY, &CDlgSelStock::OnTvnSelchangedTreeSystemblockentry)
END_MESSAGE_MAP()


// CDlgSelStock 消息处理程序

BOOL CDlgSelStock::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_tabctrl.InsertItem(0,"系统分类");
	m_tabctrl.InsertItem(1,"系统板块");
	m_tabctrl.InsertItem(2,"用户板块");
	m_tabctrl.SetCurFocus(0);
	m_treeCtrl.ShowWindow(SW_HIDE);

	// 加载图片资源
	IQuoteLang* pQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
	HANDLE handle = pQuoteLang->LoadHImage(IDB_STOCKTYPE, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	if (handle)
	{
		CBitmap bit;
		bit.Attach((HBITMAP)handle);
		m_listImage.Create(16, 16, ILC_COLOR24|ILC_MASK, 2, 2);
		BOOL bRet = m_listImage.Add(&bit, RGB(255,0,255));
		bit.DeleteObject();
	}

	//m_listStock.SetExtendedStyle(LVS_SHOWSELALWAYS);
	//m_listType.SetExtendedStyle(LVS_SHOWSELALWAYS);
	m_listType.SetImageList(&m_listImage,LVSIL_SMALL);
	m_listStock.SetImageList(&m_listImage,LVSIL_SMALL);

	DWORD dwStyles=GetWindowLong(m_treeCtrl.m_hWnd,GWL_STYLE);//获取树控制原风格
	dwStyles|=TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT;
	SetWindowLong(m_treeCtrl.m_hWnd,GWL_STYLE,dwStyles);//设置风格
	m_treeCtrl.ModifyStyle(LVS_SORTASCENDING|LVS_SORTDESCENDING,0);

	m_treeCtrl.SetImageList(&m_listImage,TVSIL_NORMAL);

	//BOOL ret = m_listStock.ModifyStyle(WS_VSCROLL|WS_HSCROLL,0);
	//ret = m_listType.ModifyStyle(WS_VSCROLL|WS_HSCROLL, 0);
	
	requestStockTypeInfo(0);
	requestStockInfo(0);
	return TRUE; 
}

void CDlgSelStock::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_tabctrl.GetCurSel(); 
	m_stockInfoSelected.m_cStockName[0]='\0';
	switch (CurSel)
	{
	case SystemClassification:
		m_listType.ShowWindow(SW_SHOW);
		m_treeCtrl.ShowWindow(SW_HIDE);
		break;
	case SystemBlock:
		m_listType.ShowWindow(SW_HIDE);
		m_treeCtrl.ShowWindow(SW_SHOW);
		break;
	case UserBlock:
		m_listType.ShowWindow(SW_SHOW);
		m_treeCtrl.ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}
	requestStockTypeInfo(CurSel);
	*pResult = 0;
}

void CDlgSelStock::requestStockTypeInfo( int CurSel )
{
	m_listStock.DeleteAllItems();
	m_listStock.UpdateWindow();

	if(SystemClassification == CurSel)
	{
		m_listType.DeleteAllItems();
		m_listType.UpdateWindow();

		CRect rect;
		m_listType.GetWindowRect(&rect);
		m_listType.InsertColumn(0,_T("市场名字"),LVCFMT_LEFT,rect.Width()/2);

		m_stockType.RemoveAll();
		((CDlgIndexUserDef*)GetParent())->m_pDataSource->HSDataSourceEx_GetSystemMarketInfo(NULL,0,m_stockType);
		for(int i=0; i<m_stockType.GetCount(); i++)
		{
			StockType test = m_stockType.GetAt(i);
			m_listType.InsertItem(i,"",0);
			m_listType.SetItemText(i,0,test.m_stTypeName.m_szName);	
		}
//		m_nTypeLastIndex = 0;
	}
	else if(SystemBlock == CurSel)
	{

		m_treeCtrl.DeleteAllItems();
		m_treeCtrl.UpdateWindow();

		CStringArray ayBlockGroup;
		((CDlgIndexUserDef*)GetParent())->m_pBlockManager->GetAllSysBlockGroupName(ayBlockGroup);

		HTREEITEM hItem;
		for(int i=0; i<ayBlockGroup.GetCount(); i++)
		{
			CString strGroupName = ayBlockGroup.GetAt(i);
			hItem = m_treeCtrl.InsertItem(strGroupName,0,0,TVI_ROOT);

			CStringArray ayBlock;
			((CDlgIndexUserDef*)GetParent())->m_pBlockManager->GetBlockName(strGroupName,ayBlock);
			for(int j=0; j<ayBlock.GetCount(); j++)
				m_treeCtrl.InsertItem(ayBlock.GetAt(j),0,0,hItem);
		}
		m_treeCtrl.Expand(m_treeCtrl.GetFirstVisibleItem(),TVE_EXPAND);
	}
	m_stockInfoSelected.m_cStockName[0]='\0';
}

void CDlgSelStock::requestStockInfo(int listIndex)
{
	m_ayStkInfo.RemoveAll();

	short nStockType = m_stockType.GetAt(listIndex).m_nStockType;
	if( (nStockType&0xF0FF) == 0x4000)
	{//如果是期货大市场

		int i=listIndex+1;
		while( (m_stockType.GetAt(i).m_nStockType&0xFF00)==nStockType )
		{
			((CDlgIndexUserDef*)GetParent())->m_pDataSource->HSDataSourceEx_GetMarketStocksInfo(m_stockType.GetAt(i).m_nStockType,m_ayStkInfo);
			i++;
			if(i >= m_stockType.GetCount())
				break;
		}
	}
	else
	{//如果不是期货大市场
		((CDlgIndexUserDef*)GetParent())->m_pDataSource->HSDataSourceEx_GetMarketStocksInfo(nStockType,m_ayStkInfo);
	}

	InsertListStock(m_ayStkInfo);

}
void CDlgSelStock::InsertListStock(const CArray<StockUserInfo>& m_stockInfo)
{
	m_listStock.DeleteAllItems();
	m_listStock.UpdateWindow();

	for(int i=0; i<m_stockInfo.GetCount(); i++)
	{
		StockUserInfo test = m_stockInfo.GetAt(i);
		m_listStock.InsertItem(i,test.m_cStockName,1);
	}
}
void CDlgSelStock::OnLvnItemchangedStocktype(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	POSITION pos = m_listType.GetFirstSelectedItemPosition();
	if(NULL == pos)
		return;

	int listIndex = (int)m_listType.GetNextSelectedItem(pos);
//	m_listType.SetItemState(m_nTypeLastIndex,0,LVS_SHOWSELALWAYS);
//	m_listType.SetItemState(listIndex,LVS_SHOWSELALWAYS ,LVS_SHOWSELALWAYS);

	requestStockInfo(listIndex);
//	m_nTypeLastIndex = listIndex;
	
	*pResult = 0;
}

void CDlgSelStock::OnLvnItemchangedStocklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	POSITION pos = m_listStock.GetFirstSelectedItemPosition();
	if(NULL == pos)
		return;

	int listIndex = (int)m_listStock.GetNextSelectedItem(pos);
	m_stockInfoSelected = m_ayStkInfo.GetAt(listIndex);


	*pResult = 0;
}

void CDlgSelStock::OnTvnSelchangedTreeSystemblockentry(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hItem = m_treeCtrl.GetSelectedItem();
	m_listStock.DeleteAllItems();
	m_listStock.UpdateWindow();

	if(!m_treeCtrl.ItemHasChildren(hItem))
	{//当前选中的子节点

		CArray<CodeInfo> ayStockList;
		((CDlgIndexUserDef*)GetParent())->m_pBlockManager->GetStockList(m_treeCtrl.GetItemText(hItem),ayStockList);
		m_ayStkInfo.RemoveAll();
		((CDlgIndexUserDef*)GetParent())->m_pDataSource->HSDataSourceEx_GetStocksUserInfo(ayStockList,m_ayStkInfo,TRUE);
		InsertListStock(m_ayStkInfo);
	}


	*pResult = 0;
}

void CDlgSelStock::OnNMDblclkStocklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	POSITION pos = m_listStock.GetFirstSelectedItemPosition();
	if(NULL == pos)
		return;

	int listIndex = (int)m_listStock.GetNextSelectedItem(pos);
	m_stockInfoSelected = m_ayStkInfo.GetAt(listIndex);
	
	OnOK();
	
	*pResult = 0;
}

