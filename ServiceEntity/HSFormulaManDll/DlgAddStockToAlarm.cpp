// DlgAddStockToAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAddStockToAlarm.h"

// CDlgAddStockToAlarm dialog
extern HWND			 g_hParentWnd;
extern BOOL GetStockByCode(CodeInfo* pInfo, StockUserInfo& stockInfo );

IMPLEMENT_DYNAMIC(CDlgAddStockToAlarm, CDialog)
CDlgAddStockToAlarm::CDlgAddStockToAlarm(void *pData/* = NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddStockToAlarm::IDD, pParent)
{
	m_pManagerDlg = (CDlgAlarmMan*)pData;
}

CDlgAddStockToAlarm::~CDlgAddStockToAlarm()
{
	m_itemList.DeleteImageList();
}

void CDlgAddStockToAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCondition);
}


BEGIN_MESSAGE_MAP(CDlgAddStockToAlarm, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, OnBnClickedOk2)
	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnMsgPress)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST1, OnNMSetfocusList1)
	ON_BN_CLICKED(IDOK3, OnBnClickedOk3)
	ON_BN_CLICKED(IDC_VIEW, OnBnClickedView)
END_MESSAGE_MAP()

long CDlgAddStockToAlarm::OnMsgPress(WPARAM wp,LPARAM lp)
{
#if 1
	if( wp == 0 )
	{
		if( lp == NULL )
		{
			return -1;
		}
		CArray<StockUserInfo*,StockUserInfo*>* pData = (CArray<StockUserInfo*,StockUserInfo*>*) lp;
		m_ayStock.Copy( *pData );
	}
#endif
	return 0;
}

void CDlgAddStockToAlarm::InitList()
{
	LV_COLUMN	lvcol;
	lvcol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcol.fmt = LVCFMT_CENTER;//居中
	lvcol.cx = 70;

	m_ListCondition.DeleteAllItems();
	while( m_ListCondition.DeleteColumn(0) != 0 );

	lvcol.iSubItem = 0;
	CString str;
	str = _T(HS_CONDITION_CON);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_CONDITION_CON;
	m_ListCondition.InsertColumn(0, &lvcol);

	lvcol.iSubItem = 1;
	str = _T(HS_CONDITION_TYPE);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_CONDITION_TYPE;
	m_ListCondition.InsertColumn(1, &lvcol);

	lvcol.iSubItem = 2;
	str = _T(HS_CONDITION_STK);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_CONDITION_STK;
	m_ListCondition.InsertColumn(2, &lvcol);

	//lvcol.cx = 70;
	//lvcol.iSubItem = 3;
	//lvcol.pszText = HS_CONDITION_NUM;
	//m_ListCondition.InsertColumn(3, &lvcol);
	
	lvcol.iSubItem = 3;
	str = _T(HS_CONDITION_ALARM);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_CONDITION_ALARM;
	lvcol.cx = 80;
	m_ListCondition.InsertColumn(3, &lvcol);
}

void CDlgAddStockToAlarm::ReDrawCondition()
{
	LV_ITEM lvitem;
	CAdvancedAlarmData* pAlarm;
	char	chTemp[250];
	int iEnd = m_pManagerDlg->m_ayAlarmData.GetSize();

	if( m_pManagerDlg == NULL )
		return;

	BOOL bFalg = m_pManagerDlg->GetState();
	m_ListCondition.DeleteAllItems();
	m_ayDisplyIndex.RemoveAll();

	for(int i = 0; i < iEnd; i++)
	{
		pAlarm = m_pManagerDlg->m_ayAlarmData.GetAt(i);
		if( !pAlarm->m_strAlarmBlock.IsEmpty() )
		{
			continue;
		}

		m_ayDisplyIndex.Add(i);
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lvitem.iItem = m_ayDisplyIndex.GetSize() - 1;
		if( pAlarm->m_uAlarmType & HS_ALARM_ENABLE )
		{
			lvitem.iImage = 0;
		}
		else
		{
			lvitem.iImage = 1;
		}
		lvitem.iSubItem = 0;
		strcpy(chTemp, pAlarm->m_strName);
		lvitem.pszText = chTemp;
		lvitem.lParam = m_ayDisplyIndex.GetSize() - 1;
		m_ListCondition.InsertItem(&lvitem);//返回表项插入后的索引号

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 1;
		if( pAlarm->m_SingleOrUnion == TRUE && pAlarm->m_pSingleSel != NULL )
		{
			sprintf(chTemp, "%s", pAlarm->m_pSingleSel->m_strExp);
		}
		else if( pAlarm->m_pUnionCondition != NULL )
		{
			sprintf(chTemp, _T("组合条件"));
		}
		else
		{
			sprintf(chTemp, _T("不可知的类型"));
		}
		lvitem.pszText = chTemp;
		m_ListCondition.SetItem(&lvitem);

		lvitem.iSubItem = 2;
		sprintf(chTemp, _T("%d支股票"), pAlarm->m_ayStkList.GetSize());
		//else
		//	sprintf(chTemp, "%s", pAlarm->m_strAlarmBlock);
		lvitem.pszText = chTemp;
		m_ListCondition.SetItem(&lvitem);

		//lvitem.iSubItem = 3;
		//sprintf(chTemp, "%d", pAlarm->m_nCount);
		//lvitem.pszText = chTemp;
		//m_ListCondition.SetItem(&lvitem);

		lvitem.iSubItem = 3;
		
		if( (pAlarm->m_uAlarmType & HS_ALARM_ON)  && bFalg )
		{
			strcpy(chTemp, _T("预警中..."));
		}
		else
		{
			strcpy(chTemp, _T("预警停止"));
		}
		lvitem.pszText = chTemp;
		m_ListCondition.SetItem(&lvitem);
	}
}

// CDlgAddStockToAlarm message handlers

BOOL CDlgAddStockToAlarm::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dWord = 0;
	dWord |=  LVS_EX_FULLROWSELECT;
	dWord |=  LVS_EX_FLATSB;
	dWord |=  LVS_EX_CHECKBOXES;
	m_ListCondition.SetExtendedStyle(dWord);
	m_itemList.Create(IDB_BITMAP_ALARM, 16, 3, RGB(255, 255, 255));
	m_ListCondition.SetImageList(&m_itemList, LVSIL_SMALL);

	InitList();
	ReDrawCondition();
	if( GetCurStockArray() <= 0 ) 
	{
		CDialog::OnCancel();
	}
	else
	{
		CString strTemp;
		StockInitInfo* pData = (StockInitInfo*)m_ayStock.GetAt(0);
		char cTemp[9];
		char cCode[7];
		memset(cTemp, 0, sizeof(char) * 9 );
		memset(cCode, 0, sizeof(char) * 7 );
		memcpy(cTemp, pData->m_cStockName + 6, sizeof(char) * 8);
		memcpy(cCode, pData->m_cStockName, sizeof(char) * 6);
		strTemp.Format(_T("将股票%s(%s)加入预警"), cTemp, cCode);
		SetWindowText(strTemp);
		SetAddButton(FALSE);
	}

	return TRUE;  
}

void CDlgAddStockToAlarm::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	LPNMLISTVIEW pView = (LPNMLISTVIEW)pNMHDR;
	if( pView->iItem >= m_ListCondition.GetItemCount() || pView->iItem < 0 )
	{
		SetAddButton(FALSE);
		return;
	}
	if( m_ayDisplyIndex.GetSize() <= pView->iItem )
	{
		SetAddButton(FALSE);
		return;
	}
	int nCount = m_ayDisplyIndex.GetAt(pView->iItem);
	SetAddButton(TRUE);
	::SendMessage(m_pManagerDlg->m_hWnd, HX_USER_COMPILEDATA, Add_Stock_To_Alarm_CurSel, (LPARAM)nCount);
}

void CDlgAddStockToAlarm::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	LPNMLISTVIEW pView = (LPNMLISTVIEW)pNMHDR;
	if( pView->iItem >= m_ListCondition.GetItemCount() || pView->iItem < 0)
	{
		SetAddButton(FALSE);
		return;
	}
	if( m_ayDisplyIndex.GetSize() <= pView->iItem )
	{
		SetAddButton(FALSE);
		return;
	}
	int nCount = m_ayDisplyIndex.GetAt(pView->iItem);
	::SendMessage(m_pManagerDlg->m_hWnd, HX_USER_COMPILEDATA, Add_Stock_To_Alarm_CurSel, (LPARAM)nCount);
	int nReturn = AddStock();
	if( nReturn == 1 )
	{
		StockInitInfo* pData = (StockInitInfo*)m_ayStock.GetAt(0);
		MessageBox(_T("所选择的预警已包含股票") + pData->m_ciStockCode.GetCode() + 
			_T(",请重新选择预警!"), _T("提示..."),MB_ICONINFORMATION);
	}
	else if( nReturn == 0 )
	{
		OnOK();
		MessageBox(_T("已成功加入!"), _T("提示..."),MB_ICONINFORMATION);
	}
	else 
	{
		OnOK();
		//AfxMessageBox(_T("加入不成功！"));
		MessageBox(_T("加入不成功!"), _T("提示..."),MB_ICONINFORMATION);
	}
}

void CDlgAddStockToAlarm::OnBnClickedOk()
{
	CButton* pWnd = (CButton*)GetDlgItem(IDC_ISSTART);
	if( pWnd->GetCheck() )
	{
		OnBnClickedOk3();
		return;
	}

	int nReturn = AddStock();
	if( nReturn == 1 )
	{
		StockInitInfo* pData = (StockInitInfo*)m_ayStock.GetAt(0);
		MessageBox(_T("所选择的预警已包含股票") + pData->m_ciStockCode.GetCode() + 
			_T(",请重新选择预警!"), _T("提示..."),MB_ICONINFORMATION);
	}
	else if( nReturn == 0 )
	{
		OnOK();
		MessageBox(_T("已成功加入!"), _T("提示..."),MB_ICONINFORMATION);
	}
	else 
	{
		OnOK();
		//AfxMessageBox(_T("加入不成功！"));
		MessageBox(_T("加入不成功!"), _T("提示..."),MB_ICONINFORMATION);
	}
}

void CDlgAddStockToAlarm::OnBnClickedOk2()
{
	OnOK();
	if( m_ayStock.GetSize() > 0 )
	{
		::SendMessage(m_pManagerDlg->m_hWnd, HX_USER_COMPILEDATA, Add_Stock_To_New_Alarm, (LPARAM)m_ayStock.GetAt(0));
	}
}

int CDlgAddStockToAlarm::GetCurStockArray()
{
	//return m_ayStock.GetSize();
	//获取股票信息  need to modify
	return (int)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 32, (LPARAM)this);
}

int CDlgAddStockToAlarm::AddStock()
{
	if( m_ayStock.GetSize() > 0 )
	{
		CodeInfo cdInfo;
		StockUserInfo* pInfo = m_ayStock.GetAt(0);
		if( pInfo == NULL )
			return 2;

		cdInfo = pInfo->m_ciStockCode;
		//根据代码获取股票信息 
		GetStockByCode(&cdInfo, *pInfo);
		if( pInfo == NULL )
			return 2;

		int nReturn = (int)::SendMessage(m_pManagerDlg->m_hWnd,
						HX_USER_COMPILEDATA, Add_Stock_To_Cur_Alarm,
						(LPARAM)pInfo);
		return nReturn;
	}
	return 2;
}

void CDlgAddStockToAlarm::SetAddButton(BOOL nFlag)
{
	CWnd* pWnd = GetDlgItem(IDOK);
	if( m_ListCondition.GetSelectedCount() > 0 )
	{
		pWnd = GetDlgItem(IDOK);
		pWnd->EnableWindow(nFlag);
		if( m_pManagerDlg->GetState() == FALSE )
		{
			pWnd = GetDlgItem(IDOK3);
			pWnd->EnableWindow(nFlag);
		}
	}
	else
	{
		pWnd = GetDlgItem(IDOK);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDOK3);
		pWnd->EnableWindow(FALSE);
	}
}



void CDlgAddStockToAlarm::OnNMSetfocusList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetAddButton(TRUE);
	*pResult = 0;
}

void CDlgAddStockToAlarm::OnBnClickedOk3()
{
	int nReturn = AddStock();
	if( nReturn == 1 )
	{
		StockInitInfo* pData = (StockInitInfo*)m_ayStock.GetAt(0);
		MessageBox(_T("所选择的预警已包含股票") + pData->m_ciStockCode.GetCode() + 
			_T(",请重新选择预警（没有改变预警状态）！"), _T("提示..."),MB_ICONINFORMATION);
	}
	else if( nReturn == 0 )
	{
		OnOK();
		::SendMessage(m_pManagerDlg->m_hWnd, HX_USER_COMPILEDATA, Begin_Alarm_Msg, 0);
		MessageBox(_T("成功加入，并且启动了预警系统!"), _T("提示..."),MB_ICONINFORMATION);
	}
	else 
	{
		OnOK();
		//AfxMessageBox(_T("加入不成功（没有改变预警状态）！"),MB_ICONINFORMATION);
		MessageBox(_T("没有改变预警状态!"), _T("提示..."),MB_ICONINFORMATION);
	}
}

int  CDlgAddStockToAlarm::DoModal()
{
	return CDialog::DoModal();
}

void CDlgAddStockToAlarm::OnBnClickedView()
{
}
