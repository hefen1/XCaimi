// PageSrvMan.cpp : 实现文件
//

#include "stdafx.h"
#include "Login.h"
#include "PageSrvMan.h"
#include "DlgSrvSet.h"
#include "DoLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CPageSrvMan 对话框

IMPLEMENT_DYNAMIC(CPageSrvMan, CPropertyPage)

CPageSrvMan::CPageSrvMan(Server_type nID, int nCellID)
: CPropertyPage(CPageSrvMan::IDD)
{
	m_nCurID = nID;
	m_nCellID = nCellID;
}

CPageSrvMan::~CPageSrvMan()
{
}

void CPageSrvMan::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SRV, m_cListSrv);
}


BEGIN_MESSAGE_MAP(CPageSrvMan, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CPageSrvMan::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CPageSrvMan::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CPageSrvMan::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CPageSrvMan::OnBnClickedButtonDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SRV, &CPageSrvMan::OnNMDblclkListSrv)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SRV, &CPageSrvMan::OnNMClickListSrv)
END_MESSAGE_MAP()


// CPageSrvMan 消息处理程序
BOOL CPageSrvMan::OnInitDialog()
{
	CPropertyPage::OnInitDialog(); 
	m_cListSrv.SetExtendedStyle(m_cListSrv.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	// 加入列
	m_cListSrv.InsertColumn(0,_T("服务器名称"),LVCFMT_CENTER,140);
	m_cListSrv.InsertColumn(1,_T("服务器地址"),LVCFMT_LEFT,120);
	m_cListSrv.InsertColumn(2,_T("端口"),LVCFMT_LEFT,50);
	CBitmap bm;
	m_imageList.Create(16,16,ILC_COLORDDB | ILC_MASK,0,0);
	bm.LoadBitmap(IDB_BITMAP2);
	m_imageList.Add(&bm,RGB(255,255,255));
	bm.DeleteObject();
	m_cListSrv.SetImageList(&m_imageList,LVSIL_SMALL);
	m_cListSrv.SetImageList(&m_imageList,LVSIL_NORMAL);
	Init();
	return TRUE;
}

void CPageSrvMan::OnBnClickedButtonDefault()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_cListSrv.GetSelectionMark();
	if (sel != -1)
	{
		SetDefault(sel);
		m_cListSrv.SetFocus();
	}
}

void CPageSrvMan::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgSrvSet dlg;
	CommInfo *pSrv = new CommInfo;
	memset(pSrv,0,sizeof(CommInfo));
	pSrv->m_cType = m_nCurID;
	dlg.m_type = m_nCurID;
	dlg.SetConn(pSrv);
	if(IDOK == dlg.DoModal())
	{
 		CArray<CommInfo*,CommInfo*> *pAy = g_pDoLogin->m_commData.GetUsrServerInfo();
 		if (pAy)
 		{
			if (m_cListSrv.GetItemCount() == 0)
			{
				pSrv->m_cIsDefault = 1;
			}
 			pAy->Add(pSrv);
			Init();
 		}
	}
}

void CPageSrvMan::OnBnClickedButtonModify()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_cListSrv.GetSelectionMark();
	if (sel != -1)
	{
 		CArray<CommInfo*,CommInfo*> *pAy = g_pDoLogin->m_commData.GetUsrServerInfo();
 		CString strname = m_cListSrv.GetItemText(sel,0);
 		CommInfo *pSite = NULL;
 		for (int i=0; i<pAy->GetCount(); i++)
 		{	
 			if (pAy->GetAt(i)->m_cType == m_nCurID && !strname.Compare(pAy->GetAt(i)->m_strServerID))
 			{
 				pSite = pAy->GetAt(i);
 				break;
 			}
 		}
 		if(pSite)
 		{
 			CDlgSrvSet dlg;
 			dlg.m_type = m_nCurID;
 			dlg.SetConn(pSite);
 			if(IDOK == dlg.DoModal())
 			{
				Init();
 			}
 		}
	}
	else
	{
		CString strText,strTitle;
		strTitle.LoadString(IDS_STRING_Error);
		strText.LoadString(IDS_STRING_EditQuoteNameNone);
		MessageBox(strText,strTitle);
	}
}

void CPageSrvMan::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_cListSrv.GetSelectionMark();
	if (sel != -1)
	{
		CString str;
		str = m_cListSrv.GetItemText(sel,0);
 		CArray<CommInfo*,CommInfo*> *pAy = g_pDoLogin->m_commData.GetUsrServerInfo();
 		if (pAy)
 		{
 			for (int i=0; i<pAy->GetCount(); i++)
 			{
 				CommInfo *pSite = pAy->GetAt(i);
 				if (pSite->m_cType == m_nCurID && !str.Compare(pSite->m_strServerID))
 				{
					delete pSite;
					pAy->RemoveAt(i);
					Init();
					return;
				}
			}
		}
	}
	else
	{		
		CString strText,strTitle;
		strTitle.LoadString(IDS_STRING_Error);
		strText.LoadString(IDS_STRING_DeleteQutoNameNone);
		MessageBox(strText,strTitle);
	}
}
void CPageSrvMan::Init()
{
	m_cListSrv.DeleteAllItems();
 	CArray<CommInfo*,CommInfo*> *pAy = g_pDoLogin->m_commData.GetSysServerInfo();
 	if (pAy)
 	{
 		CommInfo *pSite = NULL;
		int n=0;
 		for (int i=0; i<pAy->GetCount(); i++)
 		{
			pSite = pAy->GetAt(i);
			if (pSite->m_cType == m_nCurID)
			{
				n = m_cListSrv.GetItemCount();
// 				if(pSite->m_cIsDefault == 1)
// 					g_pDoLogin->m_commData.SetDefault(pSite->m_cType,pSite->m_strServerID);
				m_cListSrv.InsertItem(n,pSite->m_strServerID,pSite->m_cIsDefault==0?1:0);
				m_cListSrv.SetItemText(n,1,pSite->m_strServerIP);
				CString str;
				str.Format("%d",pSite->m_nPort);
				m_cListSrv.SetItemText(n,2,str);
			}
 		}
	}
	pAy = g_pDoLogin->m_commData.GetUsrServerInfo();
	if (pAy)
	{
		CommInfo *pSite = NULL;
		int n=0;
		for (int i=0; i<pAy->GetCount(); i++)
		{
			pSite = pAy->GetAt(i);
			if (pSite->m_cType == m_nCurID)
			{
				n = m_cListSrv.GetItemCount();
				m_cListSrv.InsertItem(n,pSite->m_strServerID,pSite->m_cIsDefault==1?0:1);
				m_cListSrv.SetItemText(n,1,pSite->m_strServerIP);
				CString str;
				str.Format("%d",pSite->m_nPort);
				m_cListSrv.SetItemText(n,2,str);
			}
		}
	}
//	GetDlgItem(IDC_BUTTON_DEFAULT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(FALSE);
}
void CPageSrvMan::SetDefault(int index)//设置默认站点
{
	CString strname = m_cListSrv.GetItemText(index,0);
	g_pDoLogin->m_commData.SetDefault(m_nCurID,strname.GetBuffer());
	Init();
}
void CPageSrvMan::OnNMDblclkListSrv(NMHDR *pNMHDR, LRESULT *pResult)//设置默认
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (pNMItemActivate->iItem != -1)
	{
		SetDefault(pNMItemActivate->iItem);
	}
}

void CPageSrvMan::OnNMClickListSrv(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	BOOL bEnable = FALSE;
	if(pNMItemActivate->iItem != -1)
	{
		CString strName = m_cListSrv.GetItemText(pNMItemActivate->iItem,0);
		CArray<CommInfo*,CommInfo*> *pAy = g_pDoLogin->m_commData.GetUsrServerInfo();
		for (int i=0; i<pAy->GetCount(); i++)
		{	
			if (pAy->GetAt(i)->m_cType == m_nCurID && !strName.Compare(pAy->GetAt(i)->m_strServerID))
			{
				bEnable = TRUE;
				break;
			}
		}
	}
//	GetDlgItem(IDC_BUTTON_DEFAULT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(bEnable);
}
