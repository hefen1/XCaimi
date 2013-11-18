// QuoteColumnSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "QuoteReport.h"
#include "QuoteColumnSetting.h"
#include "WinnerApplication.h"
#include "QuoteReportTable.h"
#include "hsOtherColDefine.h"
#include "QuoteNewTabGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CQuoteColumnSetting 对话框

IMPLEMENT_DYNAMIC(CQuoteColumnSetting, CDialog)

CQuoteColumnSetting::CQuoteColumnSetting(CString strCurGroup/* = _T("")*/,CWnd* pParent /*=NULL*/)
	: CDialog(CQuoteColumnSetting::IDD, pParent)
{
	m_iCol = (IHsColumnProperty*)CWinnerApplication::GetObject(OBJ_COLUMNPRO);
	m_nPreCurGroup = -1;
	m_nPreAllGroup = -1;
	m_bCurChanged = FALSE;
	m_bColPropChanged = FALSE;
	m_strCurReportGroup = strCurGroup;
}

CQuoteColumnSetting::~CQuoteColumnSetting()
{
}

void CQuoteColumnSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_ALL_COLGROUP,m_wndAllColGroup);
	DDX_Control(pDX,IDC_ALL_COLUMN,m_wndGroupColumn);
	DDX_Control(pDX,IDC_CURCOLUMN,m_wndCurGroupCol);
	DDX_Control(pDX,IDC_COLGROUP,m_wndCurGroup);
}

BOOL CQuoteColumnSetting::DelCurColumn(CString strName)
{
	if (strName.IsEmpty())
	{
		return m_wndCurGroupCol.DeleteAllItems();
	} 
	else
	{
		for (int i=0; i<m_wndCurGroupCol.GetItemCount(); i++)
		{
			if (!strName.CompareNoCase(m_wndCurGroupCol.GetItemText(i,0)))
			{
				return m_wndCurGroupCol.DeleteItem(i);
			}
		}
	}
	return FALSE;
}

BOOL CQuoteColumnSetting::ChangeCurColGroup(CString strGroupName)
{
	int nCurSel = m_wndCurGroup.GetCurSel();
	if (nCurSel == m_nPreCurGroup)
	{
		return FALSE;
	}
	if (m_bCurChanged)
	{
		CString strGroup;
		m_wndCurGroup.GetLBText(m_nPreCurGroup,strGroup);
		SaveCurGroup(strGroup);
	}
	m_wndCurGroupCol.DeleteAllItems();
	if (strGroupName.IsEmpty())
	{
		m_wndCurGroup.GetWindowText(strGroupName);
	}

	CUIntArray* pAyDispCol = NULL;
	m_iCol->GetDispColGroup(strGroupName, pAyDispCol);
	if (pAyDispCol)
	{
		for (int i = 0; i < pAyDispCol->GetCount(); i++)
		{
			int nID = pAyDispCol->GetAt(i++);
			int nWidth = pAyDispCol->GetAt(i);
			CString strColName = m_iCol->GetColumnName(nID);
			int nIndex = m_wndCurGroupCol.InsertItem(m_wndCurGroupCol.GetItemCount(),strColName,nWidth);
			m_wndCurGroupCol.SetItemData(nIndex,nID);

		}
	}
// 	CArray<CStringArray*,CStringArray*>* payDispCol = m_iCol->GetColHeadTemplate(strGroupName);
// 	if (payDispCol)
// 	{
// 		for (int i=0; i<payDispCol->GetCount(); i++)
// 		{
// 			CStringArray* pDispCol = payDispCol->GetAt(i);
// 			if (pDispCol)
// 			{
// 				int nID = atoi(pDispCol->GetAt(0));
// 				int nWidth = atoi(pDispCol->GetAt(1));
// 				CString strColName = m_iCol->GetColumnName(nID);
// 				int nIndex = m_wndCurGroupCol.InsertItem(m_wndCurGroupCol.GetItemCount(),strColName,nWidth);
// 				m_wndCurGroupCol.SetItemData(nIndex,nID);
// 			}
// 		}
// 	}
	m_nPreCurGroup = nCurSel;
	return TRUE;
}

BOOL CQuoteColumnSetting::ChangeColGroup(CString strGroupName /* = _T */)
{
	int nCurSel = m_wndAllColGroup.GetCurSel();
	if (nCurSel == m_nPreAllGroup)
	{
		return FALSE;
	}
	if (m_bColPropChanged)
	{
		//CString strGroup;
		//m_wndCurGroup.GetLBText(m_nPreCurGroup,strGroup);
		//SaveCurGroup(strGroup);
	}
	m_wndGroupColumn.ResetContent();
	if (strGroupName.IsEmpty())
	{
		m_wndAllColGroup.GetWindowText(strGroupName);
	}
	CColPropArray* payColProp = m_iCol->GetColumnGroupProp(strGroupName);
	if (payColProp)
	{
		for (int i=0; i<payColProp->GetCount(); i++)
		{
			const TableColProperty* pCol = payColProp->GetAt(i);
			if (pCol)
			{
				CString strColName = CString(pCol->strName);
				int nIndex = m_wndGroupColumn.AddString(strColName);
				m_wndGroupColumn.SetItemData(nIndex,pCol->m_ColumnID);
			}
		}
	}
	m_nPreAllGroup = nCurSel;
	return TRUE;
}

BOOL CQuoteColumnSetting::SaveCurGroup(CString strGroupName /* = _T */)
{
	if (!m_bCurChanged)
	{
		return FALSE;
	}
	if (strGroupName.IsEmpty())
	{
		m_wndCurGroup.GetWindowText(strGroupName);
	}

	//CArray<CStringArray*,CStringArray*> ayNewColGroup;
	CUIntArray ayNewColGroup;
	CMapWordToPtr mapFixedCol;
	//CString strTemp;
	// 生成所有显示列项
	for (int i=0; i<m_wndCurGroupCol.GetItemCount(); i++)
	{
		//CStringArray* pCol = new CStringArray;
		CUIntArray* pAyCol = new CUIntArray;
		
		LVITEM item;
		ZeroMemory(&item,sizeof(LVITEM));
		item.mask = LVIF_PARAM | LVIF_IMAGE;
		item.iItem = i;

		m_wndCurGroupCol.GetItem(&item);
		int nID = (int)item.lParam;
		pAyCol->Add(nID);
		//strTemp.Format("%d",nID);
		//pCol->Add(strTemp);
		
		long nWidth = (long)item.iImage;
		nWidth = nWidth>0 ? nWidth : m_iCol->GetColumnWidth(nID);
		pAyCol->Add(nWidth);
		//strTemp.Format("%d",nWidth);
		//pCol->Add(strTemp);

		if (CQuoteReportTable::IsFixedColumn(nID))		// 判断是否固定列
		{
			mapFixedCol.SetAt(nID,(void*)pAyCol);		// 固定列暂时放入哈希表，后面处理
		} 
		else
		{
			ayNewColGroup.Add(nID);
			ayNewColGroup.Add(nWidth);
			//ayNewColGroup.Add(pCol);			// 正常列直接放入数组
		}
	}

	if (mapFixedCol.GetCount() > 0)		// 处理固定列
	{
		int nID = 0;
		//CStringArray* payTempCol = NULL;
		CUIntArray* pAyCol = NULL;
		if (mapFixedCol.Lookup(COLUMN_HQ_BASE_ARROW,(void*&)pAyCol) && pAyCol)
		{
			ayNewColGroup.InsertAt(0, pAyCol->GetAt(0));
			ayNewColGroup.InsertAt(1, pAyCol->GetAt(1));
			delete pAyCol;
			mapFixedCol.RemoveKey(COLUMN_HQ_BASE_ARROW);
			//ayNewColGroup.InsertAt(0,payTempCol);
		}
		if (mapFixedCol.Lookup(COLUMN_HQ_BASE_INFO_MARK,(void*&)pAyCol) && pAyCol)
		{
			ayNewColGroup.InsertAt(0, pAyCol->GetAt(0));
			ayNewColGroup.InsertAt(1, pAyCol->GetAt(1));
			delete pAyCol;
			mapFixedCol.RemoveKey(COLUMN_HQ_BASE_INFO_MARK);
		}
		if (mapFixedCol.Lookup(COLUMN_HQ_BASE_NAME,(void*&)pAyCol) && pAyCol)
		{
			ayNewColGroup.InsertAt(0, pAyCol->GetAt(0));
			ayNewColGroup.InsertAt(1, pAyCol->GetAt(1));
			delete pAyCol;
			mapFixedCol.RemoveKey(COLUMN_HQ_BASE_NAME);
		}
		if (mapFixedCol.Lookup(COLUMN_HQ_BASE_CODE,(void*&)pAyCol) && pAyCol)
		{
			ayNewColGroup.InsertAt(0, pAyCol->GetAt(0));
			ayNewColGroup.InsertAt(1, pAyCol->GetAt(1));
			delete pAyCol;
			mapFixedCol.RemoveKey(COLUMN_HQ_BASE_CODE);
		}
		if (mapFixedCol.Lookup(COLUMN_HQ_BASE_NUMBER,(void*&)pAyCol) && pAyCol)
		{
			ayNewColGroup.InsertAt(0, pAyCol->GetAt(0));
			ayNewColGroup.InsertAt(1, pAyCol->GetAt(1));
			delete pAyCol;
			mapFixedCol.RemoveKey(COLUMN_HQ_BASE_NUMBER);
		}
	}
	m_iCol->SetDispColGroup(strGroupName, &ayNewColGroup);

// 	for (int i=0; i<ayNewColGroup.GetCount(); i++)
// 	{
// 		CStringArray* pTemp = ayNewColGroup.GetAt(i);
// 		if (pTemp)
// 		{
// 			delete pTemp;
// 		}
// 	}
	ayNewColGroup.RemoveAll();

	m_bCurChanged = FALSE;
	return TRUE;
}

int CQuoteColumnSetting::InsertToCurrent(CString strName, int nID)
{
	if (strName.IsEmpty() || nID < 0)
	{
		return -1;
	}
	int nIndex = -1;
	DelCurColumn(strName);

	if (CQuoteReportTable::IsFixedColumn(nID))
	{
		for (int i=0; i<m_wndCurGroupCol.GetItemCount(); i++)
		{
			int nCurID = m_wndCurGroupCol.GetItemData(i);
			if (!CQuoteReportTable::IsFixedColumn(nCurID))
			{
				nIndex = m_wndCurGroupCol.InsertItem(i,strName,m_iCol->GetColumnWidth(nID));
				m_wndCurGroupCol.SetItemData(i,nID);
				return nIndex;
			}
			switch(nID)
			{
			case COLUMN_HQ_BASE_NAME:
				{
					if (nCurID == COLUMN_HQ_BASE_INFO_MARK
						|| nCurID == COLUMN_HQ_BASE_ARROW)
					{
						nIndex = m_wndCurGroupCol.InsertItem(i,strName,m_iCol->GetColumnWidth(nID));
						m_wndCurGroupCol.SetItemData(i,nID);
						return nIndex;
					}
				}
				break;
			case COLUMN_HQ_BASE_CODE:
				{
					if (nCurID == COLUMN_HQ_BASE_INFO_MARK
						|| nCurID == COLUMN_HQ_BASE_ARROW
						|| nCurID == COLUMN_HQ_BASE_NAME)
					{
						nIndex = m_wndCurGroupCol.InsertItem(i,strName,m_iCol->GetColumnWidth(nID));
						m_wndCurGroupCol.SetItemData(i,nID);
						return nIndex;
					}
				}
				break;
			case COLUMN_HQ_BASE_INFO_MARK:
				{
					if (nCurID == COLUMN_HQ_BASE_ARROW)
					{
						nIndex = m_wndCurGroupCol.InsertItem(i,strName,m_iCol->GetColumnWidth(nID));
						m_wndCurGroupCol.SetItemData(i,nID);
						return nIndex;
					}
				}
				break;

			case COLUMN_HQ_BASE_ARROW:
				break;
			case COLUMN_HQ_BASE_NUMBER:
			default:
				{
					nIndex = m_wndCurGroupCol.InsertItem(0,strName,m_iCol->GetColumnWidth(nID));
					m_wndCurGroupCol.SetItemData(0,nID);
					return nIndex;
				}
			}

		}
	} 
	else
	{
		int nInsert = m_wndCurGroupCol.GetItemCount();
		nIndex = m_wndCurGroupCol.InsertItem(nInsert,strName,m_iCol->GetColumnWidth(nID));
		m_wndCurGroupCol.SetItemData(nInsert,nID);
	}
	return nIndex;
}
BEGIN_MESSAGE_MAP(CQuoteColumnSetting, CDialog)
	ON_BN_CLICKED(IDC_ADDTOCURRENT, &CQuoteColumnSetting::OnBnClickedAddtocurrent)
	ON_BN_CLICKED(IDC_ADDALLTOCURRENT, &CQuoteColumnSetting::OnBnClickedAddalltocurrent)
	ON_BN_CLICKED(IDC_DELFROMCURRENT, &CQuoteColumnSetting::OnBnClickedDelfromcurrent)
	ON_BN_CLICKED(IDC_DELALLFROMCURRENT, &CQuoteColumnSetting::OnBnClickedDelallfromcurrent)
	ON_BN_CLICKED(IDC_NEW_COLUMN, &CQuoteColumnSetting::OnBnClickedNewColumn)
	ON_BN_CLICKED(IDC_DEL_COLUMN, &CQuoteColumnSetting::OnBnClickedDelColumn)
	ON_BN_CLICKED(IDC_COLUMN_PROPERTY, &CQuoteColumnSetting::OnBnClickedColumnProperty)
	ON_BN_CLICKED(IDC_SAVE_CURGROUP, &CQuoteColumnSetting::OnBnClickedSaveCurgroup)
	ON_BN_CLICKED(IDC_SAVEAS_CURGROUP, &CQuoteColumnSetting::OnBnClickedSaveasCurgroup)
	ON_BN_CLICKED(IDC_UP, &CQuoteColumnSetting::OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &CQuoteColumnSetting::OnBnClickedDown)
	ON_BN_CLICKED(IDOK, &CQuoteColumnSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CQuoteColumnSetting::OnBnClickedCancel)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_CURCOLUMN, &CQuoteColumnSetting::OnLvnBegindragCurcolumn)
	ON_NOTIFY(NM_DBLCLK, IDC_CURCOLUMN, &CQuoteColumnSetting::OnNMDblclkCurcolumn)
	ON_LBN_DBLCLK(IDC_ALL_COLUMN, &CQuoteColumnSetting::OnLbnDblclkAllColumn)
	ON_BN_CLICKED(IDC_DEL_CUR_DISPCOL_GROUP, &CQuoteColumnSetting::OnBnClickedDelCurDispcolGroup)
	ON_CBN_SELCHANGE(IDC_COLGROUP, &CQuoteColumnSetting::OnCbnSelchangeColgroup)
	ON_CBN_SELCHANGE(IDC_ALL_COLGROUP, &CQuoteColumnSetting::OnCbnSelchangeAllColgroup)
END_MESSAGE_MAP()


// CQuoteColumnSetting 消息处理程序

BOOL CQuoteColumnSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	CStringArray ayGroupName;
	int nGroupCount = m_iCol->GetAllColumnGroupName(ayGroupName);
	for (int i=0; i<nGroupCount; i++)
	{
		m_wndAllColGroup.AddString(ayGroupName.GetAt(i));
	}
	CString strDefaultGroup = m_iCol->GetDefaultColGroupName();
	if (strDefaultGroup.IsEmpty())
	{
		m_wndAllColGroup.SetCurSel(0);
	} 
	else
	{
		m_wndAllColGroup.SelectString(-1,strDefaultGroup);
	}
	OnCbnSelchangeAllColgroup();
	

	// 当前显示列组
	ayGroupName.RemoveAll();
	nGroupCount = m_iCol->GetAllDispColTemplName(ayGroupName);
	for (int i=0; i<nGroupCount; i++)
	{
		m_wndCurGroup.AddString(ayGroupName.GetAt(i));
	}
	if (m_strCurReportGroup.IsEmpty())
	{
		m_wndCurGroup.SetCurSel(0);
	} 
	else
	{
		m_wndCurGroup.SelectString(-1,m_strCurReportGroup);
	}

	CRect rcList;
	m_wndCurGroupCol.GetClientRect(rcList);
	m_wndCurGroupCol.InsertColumn(0, _T("列名称"), LVCFMT_LEFT, rcList.Width()-20);
	OnCbnSelchangeColgroup();
	return TRUE;  // return TRUE unless you set the focus to a control
}

// 添加到当前显示列组
void CQuoteColumnSetting::OnBnClickedAddtocurrent()
{
	int nSel = m_wndGroupColumn.GetCurSel();
	CString strText;
	m_wndGroupColumn.GetText(nSel,strText);
	if (strText.IsEmpty())
	{
		return;
	}
	m_bCurChanged = TRUE;

	int nID = m_wndGroupColumn.GetItemData(nSel);
	InsertToCurrent(strText,nID);

}
// 全部添加到当前显示列组
void CQuoteColumnSetting::OnBnClickedAddalltocurrent()
{
	CString strText;
	for (int i=0; i<m_wndGroupColumn.GetCount(); i++)
	{
		int nID = m_wndGroupColumn.GetItemData(i);
		m_wndGroupColumn.GetText(i,strText);
		InsertToCurrent(strText,nID);
	}
	m_bCurChanged = TRUE;
}
// 从当前显示列组删除指定项
void CQuoteColumnSetting::OnBnClickedDelfromcurrent()
{
	int nSel = (int)m_wndCurGroupCol.GetFirstSelectedItemPosition()-1;
	m_wndCurGroupCol.DeleteItem(nSel);
	m_wndCurGroupCol.SetItemState(nSel,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
	m_bCurChanged = TRUE;
}
// 从当前显示列组删除所有项
void CQuoteColumnSetting::OnBnClickedDelallfromcurrent()
{
	m_wndCurGroupCol.DeleteAllItems();
	m_bCurChanged = TRUE;
}
// 创建新列
void CQuoteColumnSetting::OnBnClickedNewColumn()
{
}
// 删除指定列
void CQuoteColumnSetting::OnBnClickedDelColumn()
{
}
// 更改列属性
void CQuoteColumnSetting::OnBnClickedColumnProperty()
{
}
// 保存当前显示列组
void CQuoteColumnSetting::OnBnClickedSaveCurgroup()
{
	SaveCurGroup();
}
// 删除当前显示列组
void CQuoteColumnSetting::OnBnClickedDelCurDispcolGroup()
{
	int nCurSel = m_wndCurGroup.GetCurSel();
	CString strGroup;
	CArray<CStringArray*,CStringArray*> ayGroup;
	m_wndCurGroup.GetWindowText(strGroup);
	m_wndCurGroup.DeleteString(nCurSel);

	//m_iCol->SetDispColGroup(strGroup,ayGroup);		// 删除直接保存
	m_iCol->DelDispColGroup(strGroup);                  // 删除直接保存

	m_wndCurGroup.SetCurSel(nCurSel>=m_wndCurGroup.GetCount() ? nCurSel-1 : nCurSel);
	m_nPreCurGroup = -1;
	OnCbnSelchangeColgroup();
}
// 当前显示列组另存为
void CQuoteColumnSetting::OnBnClickedSaveasCurgroup()
{
	CString strGroupName;
	CQuoteNewTabGroupDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		strGroupName = dlg.m_strGroupName;
		int nIndex = m_wndCurGroup.AddString(strGroupName);
		m_wndCurGroup.SetCurSel(nIndex);
	}
	m_bCurChanged = TRUE;
	SaveCurGroup();
}
// 指定当前显示列上移一个
void CQuoteColumnSetting::OnBnClickedUp()
{
	int nCurSel = (int)m_wndCurGroupCol.GetFirstSelectedItemPosition()-1;
	if (nCurSel <= 0)
	{
		return;
	}
	CString strCurText = m_wndCurGroupCol.GetItemText(nCurSel,0);
	int nID = (int)m_wndCurGroupCol.GetItemData(nCurSel);
	int nPreID = (int)m_wndCurGroupCol.GetItemData(nCurSel-1);
	if (CQuoteReportTable::IsFixedColumn(nPreID))
	{
		return;
	}
	m_wndCurGroupCol.DeleteItem(nCurSel);
	m_wndCurGroupCol.InsertItem(nCurSel-1,strCurText);
	m_wndCurGroupCol.SetItemData(nCurSel-1,nID);
	m_wndCurGroupCol.SetItemState(nCurSel-1,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);

	m_bCurChanged = TRUE;
}
// 指定当前显示列下移一个
void CQuoteColumnSetting::OnBnClickedDown()
{
	int nCurSel = (int)m_wndCurGroupCol.GetFirstSelectedItemPosition()-1;
	if (nCurSel == m_wndCurGroupCol.GetItemCount())
	{
		return;
	}
	CString strCurText = m_wndCurGroupCol.GetItemText(nCurSel,0);
	int nID = (int)m_wndCurGroupCol.GetItemData(nCurSel);
	if (CQuoteReportTable::IsFixedColumn(nID))
	{
		return;
	}
	m_wndCurGroupCol.DeleteItem(nCurSel);
	m_wndCurGroupCol.InsertItem(nCurSel+1,strCurText);
	m_wndCurGroupCol.SetItemData(nCurSel+1,nID);
	m_wndCurGroupCol.SetItemState(nCurSel+1,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);

	m_bCurChanged = TRUE;
}

void CQuoteColumnSetting::OnBnClickedOk()
{
	SaveCurGroup();
	OnOK();
}

void CQuoteColumnSetting::OnBnClickedCancel()
{
	OnCancel();
}

void CQuoteColumnSetting::OnLvnBegindragCurcolumn(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

void CQuoteColumnSetting::OnNMDblclkCurcolumn(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnBnClickedDelfromcurrent();
	*pResult = 0;
}

void CQuoteColumnSetting::OnLbnDblclkAllColumn()
{
	OnBnClickedAddtocurrent();
}


void CQuoteColumnSetting::OnCbnSelchangeColgroup()
{
	ChangeCurColGroup();
}

void CQuoteColumnSetting::OnCbnSelchangeAllColgroup()
{
	ChangeColGroup();
}