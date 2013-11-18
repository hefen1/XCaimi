// QuoteTabSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "QuoteReport.h"
#include "QuoteTabSetting.h"
#include "WinnerApplication.h"
#include "quotetabctrl.h"
#include "QuoteNewTabGroupDlg.h"
#include "..\QuoteComm\pageid.h"
#include "QuoteLangMngInterFace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CQuoteTabSetting 对话框

IMPLEMENT_DYNAMIC(CQuoteTabSetting, CDialog)

CQuoteTabSetting::CQuoteTabSetting(CWnd* pParent /* = NULL */,CString strCurGroup /* = _T */)
	: CDialog(CQuoteTabSetting::IDD, pParent)
{
	m_iTab = (IHsTabCfg*)CWinnerApplication::GetObject(OBJ_TAB);
	m_iBlockManager = (IBlockManager*)CWinnerApplication::GetObject(OBJ_BLOCKMANAGER);
	m_strCurGroup = strCurGroup;
	m_nPreSel = -1;
	m_nGuideCount = 0;
	m_bSaveCurGroup = TRUE;
}

CQuoteTabSetting::~CQuoteTabSetting()
{
}

void CQuoteTabSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_ALLTABITEM,m_wndAllTabItem);
	DDX_Control(pDX,IDC_TABITEM,m_wndCurTabItem);
	DDX_Control(pDX,IDC_TABGROUP,m_wndTabGroup);
	DDX_Control(pDX,IDC_ADD_TO_CURRENT,m_wndAddToCurGroup);
	DDX_Control(pDX,IDC_DEL_FROM_CURRENT,m_wndDelFromCurGroup);
	DDX_Control(pDX,IDC_DELETEALL,m_wndDelAllCurTab);
}

int CQuoteTabSetting::AddCurTabItem(CString strItemText, DWORD_PTR pdwData /* = 0 */)
{
	DelCurTabItem(strItemText);
	if (pdwData != 0 && ((TabItem*)pdwData)->m_lMenuMask > 0)
	{
		m_wndCurTabItem.InsertItem(m_nGuideCount,strItemText);
		m_wndCurTabItem.SetItemData(m_nGuideCount,pdwData);
		m_nGuideCount++;
	}
	else
	{
		int nCount = m_wndCurTabItem.GetItemCount();
		m_wndCurTabItem.InsertItem(nCount,strItemText);
		m_wndCurTabItem.SetItemData(nCount,pdwData);
	}
	m_bSaveCurGroup = FALSE;
	return m_wndCurTabItem.GetItemCount();
}

int CQuoteTabSetting::DelCurTabItem(CString strItemText)
{
	if (!strItemText.CompareNoCase(_T("----------------")))
	{
		return -1;
	} 
	for (int i=0; i<m_wndCurTabItem.GetItemCount(); i++)
	{
		CString strText = m_wndCurTabItem.GetItemText(i,0);

		if (!strItemText.CompareNoCase(strText))
		{
			m_wndCurTabItem.DeleteItem(i);
			m_wndCurTabItem.SetItemState(i,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
			if (i < m_nGuideCount)
			{
				m_nGuideCount--;
			}
			m_bSaveCurGroup = FALSE;
			return i;
		}
	}
	return -1;
}

int CQuoteTabSetting::DelCurTabItem(int nIndex)
{
	if (nIndex < 0)
	{
		m_wndCurTabItem.DeleteAllItems();
		m_wndCurTabItem.InsertItem(0,_T("----------------"));
		m_nGuideCount = 0;
	}
	else
	{
		CString strItemText = m_wndCurTabItem.GetItemText(nIndex,0);
		if (!strItemText.CompareNoCase(_T("----------------")))
		{
			return -1;
		} 
		m_wndCurTabItem.DeleteItem(nIndex);
		m_wndCurTabItem.SetItemState(nIndex,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
		if (nIndex < m_nGuideCount)
		{
			m_nGuideCount--;
		}
	}
	m_bSaveCurGroup = FALSE;
	return nIndex;
}

BOOL CQuoteTabSetting::SaveCurTabGroup(CString strGroupName /* = _T */)
{
	if (m_bSaveCurGroup)
	{
		return FALSE;
	}
	if (strGroupName.IsEmpty())
	{
		m_wndTabGroup.GetWindowText(strGroupName);
	}

	CTabItemArray ayTabItem;
	for (int i=0; i<m_wndCurTabItem.GetItemCount(); i++)
	{
		TabItem* pTab = (TabItem*)m_wndCurTabItem.GetItemData(i);
		if (pTab)
		{
			ayTabItem.Add(pTab);
		}
	}

	m_iTab->SetTabList(strGroupName,&ayTabItem);
	m_bSaveCurGroup = TRUE;
	return TRUE;
}
BEGIN_MESSAGE_MAP(CQuoteTabSetting, CDialog)
	ON_CBN_SELCHANGE(IDC_TABGROUP,OnCbnSelChangeTabGroup)
	ON_BN_CLICKED(IDOK, &CQuoteTabSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CQuoteTabSetting::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ADD_TO_CURRENT, &CQuoteTabSetting::OnBnClickedAddToCurrent)
	ON_BN_CLICKED(IDC_DEL_FROM_CURRENT, &CQuoteTabSetting::OnBnClickedDelFromCurrent)
	ON_BN_CLICKED(IDC_DELETEALL, &CQuoteTabSetting::OnBnClickedDeleteall)
	ON_BN_CLICKED(IDC_SAVE_CURGROUP, &CQuoteTabSetting::OnBnClickedSaveCurgroup)
	ON_BN_CLICKED(IDC_NEW_GROUP, &CQuoteTabSetting::OnBnClickedNewGroup)
	ON_BN_CLICKED(IDC_RESTOR_CURGROUP, &CQuoteTabSetting::OnBnClickedRestoreCurgroup)
	ON_BN_CLICKED(IDC_DEL_CURGROUP, &CQuoteTabSetting::OnBnClickedDelCurgroup)
	ON_NOTIFY(NM_DBLCLK, IDC_ALLTABITEM, &CQuoteTabSetting::OnNMDblclkAlltabitem)
	ON_NOTIFY(NM_DBLCLK, IDC_TABITEM, &CQuoteTabSetting::OnNMDblclkTabitem)
END_MESSAGE_MAP()


// CQuoteTabSetting 消息处理程序

BOOL CQuoteTabSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	TabItem* pTab = NULL;
	CArray<StockType> ayAllMarket;
	CQuoteTabCtrl::m_pDataSource->HSDataSourceEx_GetSystemMarketInfo(0,0,ayAllMarket);
	HTREEITEM hGroup = m_wndAllTabItem.InsertItem("全部",TVI_ROOT,TVI_LAST);

	pTab = new TabItem;
	pTab->m_ID = 0;
	pTab->m_lPageType = PriceAnalisys1;
	pTab->m_lPageTag = PT_LIST|PT_TYPE;
	pTab->m_lMenuMask = TAB_ALLMARKET_MENUMASK;
	strncpy(pTab->m_szName, "全部", 64);
	m_wndAllTabItem.SetItemData(hGroup,(DWORD_PTR)pTab);
	for (int i=0; i<ayAllMarket.GetCount(); i++)
	{
		HTREEITEM hItem = m_wndAllTabItem.InsertItem(ayAllMarket.GetAt(i).m_stTypeName.m_szName,hGroup,TVI_LAST);
		pTab = new TabItem;
		pTab->m_ID = 0;
		pTab->m_lPageType = PriceAnalisys1;
		pTab->m_lPageTag = PT_LIST | PT_TYPE;
		pTab->m_sMarket[0] = (HSMarketDataType)ayAllMarket.GetAt(i).m_nStockType;
		strncpy(pTab->m_szColGroupName, m_iTab->GetDefaultTabGroup(), BLOCK_NAME_LENGTH);
		strncpy(pTab->m_szName, ayAllMarket.GetAt(i).m_stTypeName.m_szName, 64);
		m_wndAllTabItem.SetItemData(hItem,(DWORD_PTR)pTab);
	}

	CStringArray ayGroup;
	if (m_iBlockManager->GetAllSysBlockGroupName(ayGroup) > 0)
	{
		hGroup = m_wndAllTabItem.InsertItem("系统板块",TVI_ROOT,TVI_LAST);
		pTab = new TabItem;
		pTab->m_ID = 0;
		pTab->m_lPageType = PriceAnalisys1;
		pTab->m_lPageTag = PT_BLOCK;
		pTab->m_sMarket[0] = SYSBK_BOURSE;
		pTab->m_lMenuMask = SYSBK_BOURSE;
		strncpy(pTab->m_szName, "系统板块", 64);
		m_wndAllTabItem.SetItemData(hGroup,(DWORD_PTR)pTab);
		for (int i=0; i<ayGroup.GetCount(); i++)
		{
			CStringArray ayBlock;
			CString strGroupName = ayGroup.GetAt(i);
			if (m_iBlockManager->GetBlockName(strGroupName,ayBlock) > 0)
			{
				HTREEITEM hBlock = m_wndAllTabItem.InsertItem(ayGroup.GetAt(i),hGroup,TVI_LAST);
				pTab = new TabItem;
				pTab->m_ID = 0;
				pTab->m_lPageType = PriceAnalisys1;
				pTab->m_lPageTag = PT_LIST|PT_BLOCK;
				pTab->m_sMarket[0] = SYSBK_BOURSE;
				pTab->m_lMenuMask = TAB_BLOCK_MENUMASK;
				strncpy(pTab->m_szName, ayGroup.GetAt(i), 64);
				m_wndAllTabItem.SetItemData(hBlock,(DWORD_PTR)pTab);
				for (int j=0; j<ayBlock.GetCount(); j++)
				{
					HTREEITEM hItem = m_wndAllTabItem.InsertItem(ayBlock.GetAt(j),hBlock,TVI_LAST);
					pTab = new TabItem;
					pTab->m_ID = 0;
					pTab->m_lPageType = PriceAnalisys1;
					pTab->m_lPageTag = PT_LIST | PT_BLOCK;
					strncpy(pTab->m_szBlock, ayBlock.GetAt(j), 64);
					strncpy(pTab->m_szColGroupName, m_iTab->GetDefaultTabGroup(), BLOCK_NAME_LENGTH);
					strncpy(pTab->m_szName, ayBlock.GetAt(j), 64);
					m_wndAllTabItem.SetItemData(hItem,(DWORD_PTR)pTab);
				}
				ayBlock.RemoveAll();
			}
		}
		ayGroup.RemoveAll();
	}
	
	if (m_iBlockManager->GetAllUserBlockGroupName(ayGroup) > 0)
	{
		CString strSelfBlock = ((IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG))->LoadStr(IDS_SELF_BLOCK);
		hGroup = m_wndAllTabItem.InsertItem(strSelfBlock,TVI_ROOT,TVI_LAST);
		pTab = new TabItem;
		pTab->m_ID = 0;
		pTab->m_lPageType = PriceAnalisys1;
		pTab->m_lPageTag = PT_BLOCK;
		pTab->m_sMarket[0] = USERDEF_BOURSE;
		pTab->m_lMenuMask = USERDEF_BOURSE;
		strncpy(pTab->m_szName, strSelfBlock, 64);
		m_wndAllTabItem.SetItemData(hGroup,(DWORD_PTR)USERDEF_BOURSE);
		for (int i=0; i<ayGroup.GetCount(); i++)
		{
			CStringArray ayBlock;
			CString strGroupName = ayGroup.GetAt(i);
			if (m_iBlockManager->GetBlockName(strGroupName,ayBlock) > 0)
			{
				HTREEITEM hBlock = m_wndAllTabItem.InsertItem(ayGroup.GetAt(i),hGroup,TVI_LAST);
				pTab = new TabItem;
				pTab->m_ID = 0;
				pTab->m_lPageType = PriceAnalisys1;
				pTab->m_lPageTag = PT_LIST|PT_BLOCK;
				pTab->m_sMarket[0] = USERDEF_BOURSE;
				pTab->m_lMenuMask = TAB_BLOCK_MENUMASK;
				strncpy(pTab->m_szName, ayGroup.GetAt(i), 64);
				m_wndAllTabItem.SetItemData(hBlock,(DWORD_PTR)TAB_BLOCK_MENUMASK);
				for (int j=0; j<ayBlock.GetCount(); j++)
				{
					HTREEITEM hItem = m_wndAllTabItem.InsertItem(ayBlock.GetAt(j),hBlock,TVI_LAST);
					pTab = new TabItem;
					pTab->m_ID = 0;
					pTab->m_lPageType = PriceAnalisys1;
					pTab->m_lPageTag = PT_LIST | PT_BLOCK;
					strncpy(pTab->m_szBlock, ayBlock.GetAt(j), 64);
					strncpy(pTab->m_szColGroupName, m_iTab->GetDefaultTabGroup(), BLOCK_NAME_LENGTH);
					strncpy(pTab->m_szName, ayBlock.GetAt(j), 64);
					m_wndAllTabItem.SetItemData(hItem,(DWORD_PTR)pTab);
				}
				ayBlock.RemoveAll();
			}
		}
		ayGroup.RemoveAll();
	}

	CStringArray aySys;
	m_iTab->GetSysTabGroups(aySys);
	CStringArray ayUser;
	m_iTab->GetUserTabGroups(ayUser);
	
	for (int i = 0; i < aySys.GetCount(); i++)
	{// 删除修改过的系统组
		CString strTab = aySys.GetAt(i);
		int j(0);
		for (j = 0; j < ayUser.GetCount(); j++)
		{
			if (strTab == ayUser.GetAt(j))
				break;
		}
		if (j == ayUser.GetCount())
			m_aySysGroup.Add(strTab);
	}

	for (int i=0; i<m_aySysGroup.GetCount(); i++)
		m_wndTabGroup.AddString(m_aySysGroup.GetAt(i));
	for (int i=0; i<ayUser.GetCount(); i++)
		m_wndTabGroup.AddString(ayUser.GetAt(i));

	CRect rcList;
	m_wndCurTabItem.GetClientRect(rcList);
	m_wndCurTabItem.InsertColumn(0,_T("标签名称"),LVCFMT_LEFT,rcList.Width() - 20);
	if (m_wndTabGroup.SelectString(-1,m_strCurGroup) == CB_ERR)
	{
		m_wndTabGroup.SetCurSel(0);
		OnCbnSelChangeTabGroup();
	}

	return TRUE;
}

void CQuoteTabSetting::OnCbnSelChangeTabGroup()
{
	int nCurSel = m_wndTabGroup.GetCurSel();
	if (nCurSel == m_nPreSel)
	{
		return;
	}
	else
	{
		if (!m_bSaveCurGroup)
		{
			CString strText;
			if (m_nPreSel > -1 && m_nPreSel < m_wndTabGroup.GetCount())
			{
				m_wndTabGroup.GetLBText(m_nPreSel,strText);
				SaveCurTabGroup(strText);
			}

		}
		m_nPreSel = nCurSel;
	}

	m_wndCurTabItem.DeleteAllItems();
	CString strGroupName;
	m_wndTabGroup.GetWindowText(strGroupName);

	CTabItemArray ayTabItem;
	m_iTab->GetTabList(strGroupName,ayTabItem);
	int nCount = 0;
	for (int i=0; i<ayTabItem.GetCount(); i++)
	{
		if (ayTabItem.GetAt(i)->m_lMenuMask > 0)		// 向导
		{
			TabItem* pItem = ayTabItem.GetAt(i);
			m_wndCurTabItem.InsertItem(nCount,ayTabItem.GetAt(i)->m_szName);
			m_wndCurTabItem.SetItemData(nCount,(DWORD_PTR)(ayTabItem.GetAt(i)));
			nCount++;
		}
	}
	m_nGuideCount = nCount;
	m_wndCurTabItem.InsertItem(nCount++,_T("----------------"));
	for (int i=0; i<ayTabItem.GetCount(); i++)
	{
		if (ayTabItem.GetAt(i)->m_lMenuMask <= 0)		// 标签
		{
			TabItem* pItem = ayTabItem.GetAt(i);
			m_wndCurTabItem.InsertItem(nCount,ayTabItem.GetAt(i)->m_szName);
			m_wndCurTabItem.SetItemData(nCount,(DWORD_PTR)(ayTabItem.GetAt(i)));
			nCount++;
		}
	}
}
void CQuoteTabSetting::OnBnClickedOk()
{
	SaveCurTabGroup();

	m_iTab->SaveToFile();
	OnOK();
}

void CQuoteTabSetting::OnBnClickedCancel()
{
	OnCancel();
}

void CQuoteTabSetting::OnBnClickedAddToCurrent()
{
	HTREEITEM hSelItem = m_wndAllTabItem.GetSelectedItem();
	CString strItemText = m_wndAllTabItem.GetItemText(hSelItem);
	DWORD_PTR nMenuMask = m_wndAllTabItem.GetItemData(hSelItem);
	TabItem* test = (TabItem*)nMenuMask;
	AddCurTabItem(strItemText,nMenuMask);
}

void CQuoteTabSetting::OnBnClickedDelFromCurrent()
{
	POSITION pos = m_wndCurTabItem.GetFirstSelectedItemPosition();
	DelCurTabItem((int)(pos-1));
}

void CQuoteTabSetting::OnBnClickedDeleteall()
{
	DelCurTabItem(-1);
}

void CQuoteTabSetting::OnBnClickedSaveCurgroup()
{
	SaveCurTabGroup();
}

void CQuoteTabSetting::OnBnClickedNewGroup()
{
	CString strGroupName;
	CQuoteNewTabGroupDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		strGroupName = dlg.m_strGroupName;
	}
	else
	{
		return ;
	}

	for (int i=0; i<m_wndTabGroup.GetCount(); i++)
	{
		CString strTemp;
		m_wndTabGroup.GetLBText(i,strTemp);
		if (!strTemp.CompareNoCase(strGroupName))
		{
			MessageBox("与现有分组重名，请指定其他名称","注意");
			return;
		}
	}
	int nIndex = m_wndTabGroup.AddString(strGroupName);
	m_wndTabGroup.SetCurSel(nIndex);
	OnCbnSelChangeTabGroup();
	m_bSaveCurGroup = FALSE;
}

void CQuoteTabSetting::OnBnClickedRestoreCurgroup()
{
}

void CQuoteTabSetting::OnBnClickedDelCurgroup()
{
	int nSel = m_wndTabGroup.GetCurSel();
	CString strText;
	m_wndTabGroup.GetWindowText(strText);
	if (MessageBox("系统默认分类无法删除，因此删除系统默认分类的操作会取消所有对系统默认分类的更改\r\n确认删除当前分类？","注意",MB_YESNOCANCEL)!=IDYES)
	{
		return;
	}
	
	m_wndTabGroup.DeleteString(nSel);
	DelCurTabItem(-1);
	if (nSel >= m_wndTabGroup.GetCount()-2)
	{
		m_wndTabGroup.SetCurSel(nSel-1);
	} 
	else
	{
		m_wndTabGroup.SetCurSel(nSel);
	}
	OnCbnSelChangeTabGroup();
	if (!m_bSaveCurGroup)
	{
		m_bSaveCurGroup = TRUE;
	}
	m_iTab->DelTabList(strText);
}

void CQuoteTabSetting::OnNMDblclkAlltabitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hSelItem = m_wndAllTabItem.GetSelectedItem();
	if (!m_wndAllTabItem.ItemHasChildren(hSelItem))
	{
		OnBnClickedAddToCurrent();
	}
	*pResult = 0;
}

void CQuoteTabSetting::OnNMDblclkTabitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	DelCurTabItem(pNMItemActivate->iItem);
	*pResult = 0;
}

BOOL CQuoteTabSetting::DestroyWindow()
{
	HTREEITEM hGroup = m_wndAllTabItem.GetRootItem();
	CString strGroup = m_wndAllTabItem.GetItemText(hGroup);
	while(hGroup)
	{
		TabItem* pItem = (TabItem*)m_wndAllTabItem.GetItemData(hGroup);
		if (pItem)
		{
			delete pItem;
			pItem = NULL;
		}

		if (m_wndAllTabItem.ItemHasChildren(hGroup))
		{
			HTREEITEM hItem = m_wndAllTabItem.GetChildItem(hGroup);
			while(hItem)
			{
				pItem = (TabItem*)m_wndAllTabItem.GetItemData(hItem);
				if (pItem)
				{
					delete pItem;
					pItem = NULL;
				}

				if (m_wndAllTabItem.ItemHasChildren(hItem))
				{
					HTREEITEM hSubItem = m_wndAllTabItem.GetChildItem(hItem);
					while(hSubItem)
					{
						pItem = (TabItem*)m_wndAllTabItem.GetItemData(hSubItem);
						if (pItem)
						{
							delete pItem;
							pItem = NULL;
						}
						hSubItem = m_wndAllTabItem.GetNextSiblingItem(hSubItem);
					}
				}
				hItem = m_wndAllTabItem.GetNextSiblingItem(hItem);
			}
		}
		hGroup = m_wndAllTabItem.GetNextSiblingItem(hGroup);
	}

	return CDialog::DestroyWindow();
}
